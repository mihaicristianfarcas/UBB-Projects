#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        ST_IMPL_BST = 0,
        ST_IMPL_HASH = 1
    } STImplementation;

    typedef struct SymbolTable SymbolTable;

    SymbolTable *st_create(STImplementation impl, size_t capacity);
    void st_free(SymbolTable *table);
    int st_insert(SymbolTable *table, const char *symbol);
    int st_get_id(SymbolTable *table, const char *symbol);
    size_t st_size(const SymbolTable *table);
    const char *st_symbol_at(const SymbolTable *table, size_t index);

#ifdef __cplusplus
}
#endif

#endif
