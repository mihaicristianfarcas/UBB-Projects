#include "symbol_table.h"

#include <stdlib.h>
#include <string.h>

#define INITIAL_ENTRY_CAPACITY 32
#define HASH_DEFAULT_CAPACITY 31

struct BSTNode
{
    char *key;
    int id;
    struct BSTNode *left;
    struct BSTNode *right;
};

typedef struct HashNode
{
    char *key;
    int id;
    struct HashNode *next;
} HashNode;

typedef struct
{
    size_t bucket_count;
    HashNode **buckets;
} HashTable;

struct SymbolTable
{
    STImplementation impl;
    size_t count;
    int next_id;
    char **entries;
    size_t entries_capacity;
    union
    {
        struct BSTNode *bst_root;
        HashTable hash;
    } data;
};

static char *st_strdup(const char *src)
{
    if (!src)
    {
        return NULL;
    }
    size_t len = strlen(src);
    char *copy = (char *)malloc(len + 1);
    if (!copy)
    {
        return NULL;
    }
    memcpy(copy, src, len + 1);
    return copy;
}

static struct BSTNode *bst_create_node(const char *symbol, int id)
{
    struct BSTNode *node = (struct BSTNode *)malloc(sizeof(struct BSTNode));
    if (!node)
    {
        return NULL;
    }
    node->key = st_strdup(symbol);
    if (!node->key)
    {
        free(node);
        return NULL;
    }
    node->id = id;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void bst_free(struct BSTNode *node)
{
    if (!node)
    {
        return;
    }
    bst_free(node->left);
    bst_free(node->right);
    free(node->key);
    free(node);
}

static struct BSTNode *bst_insert_node(struct BSTNode *root, struct BSTNode *node)
{
    if (!root)
    {
        return node;
    }
    int cmp = strcmp(node->key, root->key);
    if (cmp < 0)
    {
        root->left = bst_insert_node(root->left, node);
    }
    else
    {
        root->right = bst_insert_node(root->right, node);
    }
    return root;
}

static int bst_get_id(struct BSTNode *node, const char *symbol)
{
    if (!node)
    {
        return -1;
    }
    int cmp = strcmp(symbol, node->key);
    if (cmp == 0)
    {
        return node->id;
    }
    if (cmp < 0)
    {
        return bst_get_id(node->left, symbol);
    }
    return bst_get_id(node->right, symbol);
}

static unsigned long djb2(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++) != 0)
    {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }
    return hash;
}

static void hash_table_init(HashTable *table, size_t capacity)
{
    table->bucket_count = capacity > 0 ? capacity : HASH_DEFAULT_CAPACITY;
    table->buckets = (HashNode **)calloc(table->bucket_count, sizeof(HashNode *));
}

static void hash_table_free(HashTable *table)
{
    if (!table || !table->buckets)
    {
        return;
    }
    for (size_t i = 0; i < table->bucket_count; ++i)
    {
        HashNode *node = table->buckets[i];
        while (node)
        {
            HashNode *next = node->next;
            free(node->key);
            free(node);
            node = next;
        }
    }
    free(table->buckets);
}

static int hash_table_get_id(HashTable *table, const char *symbol)
{
    if (!table || !symbol || !table->buckets)
    {
        return -1;
    }
    unsigned long hash = djb2(symbol);
    size_t index = hash % table->bucket_count;
    HashNode *node = table->buckets[index];
    while (node)
    {
        if (strcmp(node->key, symbol) == 0)
        {
            return node->id;
        }
        node = node->next;
    }
    return -1;
}

static int hash_table_insert_new(HashTable *table, const char *symbol, int id)
{
    if (!table || !symbol || !table->buckets)
    {
        return -1;
    }
    unsigned long hash = djb2(symbol);
    size_t index = hash % table->bucket_count;
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (!node)
    {
        return -1;
    }
    node->key = st_strdup(symbol);
    if (!node->key)
    {
        free(node);
        return -1;
    }
    node->id = id;
    node->next = table->buckets[index];
    table->buckets[index] = node;
    return 0;
}

static int ensure_entry_capacity(SymbolTable *table)
{
    if (table->entries && table->count < table->entries_capacity)
    {
        return 0;
    }
    size_t new_capacity = table->entries_capacity == 0 ? INITIAL_ENTRY_CAPACITY : table->entries_capacity * 2;
    char **new_entries = (char **)realloc(table->entries, new_capacity * sizeof(char *));
    if (!new_entries)
    {
        return -1;
    }
    table->entries = new_entries;
    table->entries_capacity = new_capacity;
    return 0;
}

SymbolTable *st_create(STImplementation impl, size_t capacity)
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!table)
    {
        return NULL;
    }
    table->impl = impl;
    table->count = 0;
    table->next_id = 0;
    table->entries = NULL;
    table->entries_capacity = 0;
    if (impl == ST_IMPL_BST)
    {
        table->data.bst_root = NULL;
    }
    else
    {
        hash_table_init(&table->data.hash, capacity);
        if (!table->data.hash.buckets)
        {
            free(table);
            return NULL;
        }
    }
    return table;
}

void st_free(SymbolTable *table)
{
    if (!table)
    {
        return;
    }
    if (table->impl == ST_IMPL_BST)
    {
        bst_free(table->data.bst_root);
    }
    else
    {
        hash_table_free(&table->data.hash);
    }
    if (table->entries)
    {
        for (size_t i = 0; i < table->count; ++i)
        {
            free(table->entries[i]);
        }
        free(table->entries);
    }
    free(table);
}

int st_insert(SymbolTable *table, const char *symbol)
{
    if (!table || !symbol)
    {
        return -1;
    }
    int existing_id = st_get_id(table, symbol);
    if (existing_id != -1)
    {
        return existing_id;
    }
    if (ensure_entry_capacity(table) != 0)
    {
        return -1;
    }
    char *entry_copy = st_strdup(symbol);
    if (!entry_copy)
    {
        return -1;
    }
    int id = table->next_id;
    if (table->impl == ST_IMPL_BST)
    {
        struct BSTNode *node = bst_create_node(symbol, id);
        if (!node)
        {
            free(entry_copy);
            return -1;
        }
        table->data.bst_root = bst_insert_node(table->data.bst_root, node);
        if (!table->data.bst_root)
        {
            free(entry_copy);
            return -1;
        }
    }
    else
    {
        if (hash_table_insert_new(&table->data.hash, symbol, id) != 0)
        {
            free(entry_copy);
            return -1;
        }
    }
    table->entries[table->count] = entry_copy;
    table->count += 1;
    table->next_id += 1;
    return id;
}

int st_get_id(SymbolTable *table, const char *symbol)
{
    if (!table || !symbol)
    {
        return -1;
    }
    if (table->impl == ST_IMPL_BST)
    {
        return bst_get_id(table->data.bst_root, symbol);
    }
    return hash_table_get_id(&table->data.hash, symbol);
}

size_t st_size(const SymbolTable *table)
{
    if (!table)
    {
        return 0;
    }
    return table->count;
}

const char *st_symbol_at(const SymbolTable *table, size_t index)
{
    if (!table || !table->entries || index >= table->count)
    {
        return NULL;
    }
    return table->entries[index];
}
