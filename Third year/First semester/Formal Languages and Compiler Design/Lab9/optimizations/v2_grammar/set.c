#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_CAPACITY 16

Set* create_set(void) {
    Set* set = malloc(sizeof(Set));
    set->size = 0;
    set->capacity = INITIAL_CAPACITY;
    set->elements = malloc(sizeof(char*) * set->capacity);
    return set;
}

// OPTIMIZATION 1: Binary search for contains_set - O(log n) instead of O(n)
static int binary_search(const Set* set, const char* element) {
    int left = 0, right = set->size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(set->elements[mid], element);
        
        if (cmp == 0) return mid;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    
    return -1;
}

bool contains_set(const Set* set, const char* element) {
    return binary_search(set, element) >= 0;
}

// OPTIMIZATION 1: Keep sorted + binary search
void add_to_set(Set* set, const char* element) {
    if (contains_set(set, element)) {
        return;
    }
    
    if (set->size >= set->capacity) {
        set->capacity *= 2;
        set->elements = realloc(set->elements, sizeof(char*) * set->capacity);
    }
    
    // Find insertion point to maintain sorted order
    int i = set->size - 1;
    while (i >= 0 && strcmp(set->elements[i], element) > 0) {
        set->elements[i + 1] = set->elements[i];
        i--;
    }
    
    set->elements[i + 1] = strdup(element);
    set->size++;
}

// OPTIMIZATION 1: Merge algorithm - O(n+m) instead of O(n*m)
void union_sets(Set* dest, const Set* src) {
    if (src->size == 0) return;
    
    int max_size = dest->size + src->size;
    char** merged = malloc(sizeof(char*) * max_size);
    
    int i = 0, j = 0, k = 0;
    
    // Merge two sorted arrays
    while (i < dest->size && j < src->size) {
        int cmp = strcmp(dest->elements[i], src->elements[j]);
        
        if (cmp < 0) {
            merged[k++] = dest->elements[i++];
        } else if (cmp > 0) {
            merged[k++] = strdup(src->elements[j++]);
        } else {
            merged[k++] = dest->elements[i++];
            j++;
        }
    }
    
    while (i < dest->size) merged[k++] = dest->elements[i++];
    while (j < src->size) merged[k++] = strdup(src->elements[j++]);
    
    free(dest->elements);
    dest->elements = merged;
    dest->size = k;
    
    if (dest->capacity < k) {
        dest->capacity = k * 2;
        dest->elements = realloc(dest->elements, sizeof(char*) * dest->capacity);
    }
}

Set* copy_set(const Set* src) {
    Set* copy = create_set();
    union_sets(copy, src);
    return copy;
}

void free_set(Set* set) {
    if (!set) return;
    
    for (int i = 0; i < set->size; i++) {
        free(set->elements[i]);
    }
    free(set->elements);
    free(set);
}

void print_set(const Set* set) {
    printf("{ ");
    for (int i = 0; i < set->size; i++) {
        printf("%s", set->elements[i]);
        if (i < set->size - 1) printf(", ");
    }
    printf(" }");
}

int set_size(const Set* set) {
    return set->size;
}

const char* set_get(const Set* set, int index) {
    if (index < 0 || index >= set->size) return NULL;
    return set->elements[index];
}
