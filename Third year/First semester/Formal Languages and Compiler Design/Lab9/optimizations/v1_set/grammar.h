#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "set.h"
#include <stdbool.h>

typedef struct {
    char* nonterminal;
    char** symbols;
    int length;
} Production;

typedef struct {
    char* start_symbol;
    char* epsilon;
    Set* terminals;
    Set* nonterminals;
    Production* productions;
    int num_productions;
    int productions_capacity;
} Grammar;

Grammar* create_grammar(const char* start, const char* epsilon);
void add_terminal(Grammar* g, const char* terminal);
void add_nonterminal(Grammar* g, const char* nonterminal);
void add_production(Grammar* g, const char* nonterminal, char** symbols, int length);
bool is_terminal(const Grammar* g, const char* symbol);
bool is_nonterminal(const Grammar* g, const char* symbol);
bool is_epsilon(const Grammar* g, const char* symbol);
int get_productions_for(const Grammar* g, const char* nonterminal, Production** prods);
void free_grammar(Grammar* g);
void print_grammar(const Grammar* g);

#endif
