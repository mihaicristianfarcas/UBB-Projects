#ifndef PARSER_H
#define PARSER_H

#include "grammar.h"
#include "set.h"
#include <stdbool.h>

typedef struct {
    char* nonterminal;
    char* terminal;
} TableKey;

typedef struct {
    char* nonterminal;
    int prod_index;
} TableValue;

typedef struct {
    TableKey* keys;
    TableValue* values;
    int size;
    int capacity;
} ParsingTable;

typedef struct {
    Grammar* grammar;
    Set** first_sets;
    int num_first_sets;
    Set** follow_sets;
    int num_follow_sets;
    ParsingTable* parsing_table;
} LL1Parser;

LL1Parser* create_parser(Grammar* grammar);
void compute_first_sets(LL1Parser* parser);
void compute_follow_sets(LL1Parser* parser);
void construct_parsing_table(LL1Parser* parser);
char** parse(LL1Parser* parser, char** input, int input_len, int* prod_count);
void free_parser(LL1Parser* parser);
Set* first_of_sequence(LL1Parser* parser, char** symbols, int length);
Set* get_first_set(LL1Parser* parser, const char* symbol);
Set* get_follow_set(LL1Parser* parser, const char* symbol);

#endif
