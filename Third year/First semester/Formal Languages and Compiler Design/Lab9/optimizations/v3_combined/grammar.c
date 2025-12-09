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
    g->production_index = NULL;  // OPTIMIZATION 2: Will be built later
    g->index_size = 0;
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

// OPTIMIZATION 2: Build production index for O(1) lookup
static int find_nonterminal_index(const Grammar* g, const char* nt) {
    for (int i = 0; i < set_size(g->nonterminals); i++) {
        if (strcmp(set_get(g->nonterminals, i), nt) == 0) {
            return i;
        }
    }
    return -1;
}

void build_production_index(Grammar* g) {
    g->index_size = set_size(g->nonterminals);
    g->production_index = calloc(g->index_size, sizeof(ProductionRange));
    
    // Initialize all ranges
    for (int i = 0; i < g->index_size; i++) {
        g->production_index[i].start_idx = -1;
        g->production_index[i].count = 0;
    }
    
    // Build index by scanning productions once
    for (int i = 0; i < g->num_productions; i++) {
        int nt_idx = find_nonterminal_index(g, g->productions[i].nonterminal);
        if (nt_idx >= 0) {
            if (g->production_index[nt_idx].start_idx == -1) {
                g->production_index[nt_idx].start_idx = i;
            }
            g->production_index[nt_idx].count++;
        }
    }
}

// OPTIMIZATION 2: Use index for O(1) lookup (no malloc!)
// Returns: negative count if pointer needs freeing, positive if not
int get_productions_for(const Grammar* g, const char* nonterminal, Production** prods) {
    // Use index if available - returns pointer into array
    if (g->production_index) {
        int idx = find_nonterminal_index(g, nonterminal);
        if (idx < 0 || g->production_index[idx].count == 0) {
            *prods = NULL;
            return 0;
        }
        
        // Return pointer into production array - return positive to indicate NO FREE needed
        *prods = &g->productions[g->production_index[idx].start_idx];
        return g->production_index[idx].count;
    }
    
    // Fallback to old method - return negative to indicate FREE needed
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
    return -count;  // Negative indicates caller should free
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
    
    // OPTIMIZATION 2: Free index
    if (g->production_index) {
        free(g->production_index);
    }
    
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
