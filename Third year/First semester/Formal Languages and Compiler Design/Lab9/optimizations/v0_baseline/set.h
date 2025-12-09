#ifndef SET_H
#define SET_H

#include <stdbool.h>

typedef struct {
    char** elements;
    int size;
    int capacity;
} Set;

Set* create_set(void);
void add_to_set(Set* set, const char* element);
bool contains_set(const Set* set, const char* element);
void union_sets(Set* dest, const Set* src);
Set* copy_set(const Set* src);
void free_set(Set* set);
void print_set(const Set* set);
int set_size(const Set* set);
const char* set_get(const Set* set, int index);

#endif
