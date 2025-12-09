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

void add_to_set(Set* set, const char* element) {
    if (contains_set(set, element)) {
        return;
    }
    
    if (set->size >= set->capacity) {
        set->capacity *= 2;
        set->elements = realloc(set->elements, sizeof(char*) * set->capacity);
    }
    
    set->elements[set->size] = strdup(element);
    set->size++;
}

bool contains_set(const Set* set, const char* element) {
    for (int i = 0; i < set->size; i++) {
        if (strcmp(set->elements[i], element) == 0) {
            return true;
        }
    }
    return false;
}

void union_sets(Set* dest, const Set* src) {
    for (int i = 0; i < src->size; i++) {
        add_to_set(dest, src->elements[i]);
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
