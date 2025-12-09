#include "grammar.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_PROD_CAPACITY 64

Grammar* create_grammar(const char* start, const char* epsilon) {
    Grammar* g = malloc(sizeof(Grammar));
    g->start_symbol = strdup(start);
    g->epsilon = strdup(epsilon);
    g->terminals = create_set();
    g->nonterminals = create_set();
    g->productions = malloc(sizeof(Production) * INITIAL_PROD_CAPACITY);
    g->num_productions = 0;
    g->productions_capacity = INITIAL_PROD_CAPACITY;
    return g;
}

void add_terminal(Grammar* g, const char* terminal) {
    add_to_set(g->terminals, terminal);
}

void add_nonterminal(Grammar* g, const char* nonterminal) {
    add_to_set(g->nonterminals, nonterminal);
}

void add_production(Grammar* g, const char* nonterminal, char** symbols, int length) {
    if (g->num_productions >= g->productions_capacity) {
        g->productions_capacity *= 2;
        g->productions = realloc(g->productions, 
                                sizeof(Production) * g->productions_capacity);
    }
    
    Production* p = &g->productions[g->num_productions];
    p->nonterminal = strdup(nonterminal);
    p->length = length;
    p->symbols = malloc(sizeof(char*) * length);
    for (int i = 0; i < length; i++) {
        p->symbols[i] = strdup(symbols[i]);
    }
    g->num_productions++;
}

bool is_terminal(const Grammar* g, const char* symbol) {
    return contains_set(g->terminals, symbol);
}

bool is_nonterminal(const Grammar* g, const char* symbol) {
    return contains_set(g->nonterminals, symbol);
}

bool is_epsilon(const Grammar* g, const char* symbol) {
    return strcmp(symbol, g->epsilon) == 0;
}

int get_productions_for(const Grammar* g, const char* nonterminal, Production** prods) {
    int count = 0;
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].nonterminal, nonterminal) == 0) {
            count++;
        }
    }
    
    if (count == 0) {
        *prods = NULL;
        return 0;
    }
    
    *prods = malloc(sizeof(Production) * count);
    int idx = 0;
    for (int i = 0; i < g->num_productions; i++) {
        if (strcmp(g->productions[i].nonterminal, nonterminal) == 0) {
            (*prods)[idx++] = g->productions[i];
        }
    }
    return count;
}

void free_grammar(Grammar* g) {
    if (!g) return;
    
    free(g->start_symbol);
    free(g->epsilon);
    free_set(g->terminals);
    free_set(g->nonterminals);
    
    for (int i = 0; i < g->num_productions; i++) {
        free(g->productions[i].nonterminal);
        for (int j = 0; j < g->productions[i].length; j++) {
            free(g->productions[i].symbols[j]);
        }
        free(g->productions[i].symbols);
    }
    free(g->productions);
    free(g);
}

void print_grammar(const Grammar* g) {
    printf("Start: %s\n", g->start_symbol);
    printf("Epsilon: %s\n", g->epsilon);
    printf("Terminals: "); print_set(g->terminals); printf("\n");
    printf("Nonterminals: "); print_set(g->nonterminals); printf("\n");
    printf("Productions:\n");
    for (int i = 0; i < g->num_productions; i++) {
        printf("  %s ->", g->productions[i].nonterminal);
        for (int j = 0; j < g->productions[i].length; j++) {
            printf(" %s", g->productions[i].symbols[j]);
        }
        printf("\n");
    }
}
