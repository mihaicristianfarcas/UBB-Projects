#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_TABLE_CAPACITY 128

static ParsingTable* create_parsing_table(void) {
    ParsingTable* table = malloc(sizeof(ParsingTable));
    table->capacity = INITIAL_TABLE_CAPACITY;
    table->size = 0;
    table->keys = malloc(sizeof(TableKey) * table->capacity);
    table->values = malloc(sizeof(TableValue) * table->capacity);
    return table;
}

static void free_parsing_table(ParsingTable* table) {
    if (!table) return;
    for (int i = 0; i < table->size; i++) {
        free(table->keys[i].nonterminal);
        free(table->keys[i].terminal);
        free(table->values[i].nonterminal);
    }
    free(table->keys);
    free(table->values);
    free(table);
}

static void add_table_entry(ParsingTable* table, const char* nonterminal, 
                           const char* terminal, const char* prod_nt, int prod_idx) {
    if (table->size >= table->capacity) {
        table->capacity *= 2;
        table->keys = realloc(table->keys, sizeof(TableKey) * table->capacity);
        table->values = realloc(table->values, sizeof(TableValue) * table->capacity);
    }
    
    table->keys[table->size].nonterminal = strdup(nonterminal);
    table->keys[table->size].terminal = strdup(terminal);
    table->values[table->size].nonterminal = strdup(prod_nt);
    table->values[table->size].prod_index = prod_idx;
    table->size++;
}

static TableValue* get_table_entry(ParsingTable* table, const char* nonterminal, const char* terminal) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->keys[i].nonterminal, nonterminal) == 0 &&
            strcmp(table->keys[i].terminal, terminal) == 0) {
            return &table->values[i];
        }
    }
    return NULL;
}

LL1Parser* create_parser(Grammar* grammar) {
    LL1Parser* parser = malloc(sizeof(LL1Parser));
    parser->grammar = grammar;
    parser->parsing_table = create_parsing_table();
    
    int total_symbols = set_size(grammar->terminals) + set_size(grammar->nonterminals) + 1;
    parser->num_first_sets = total_symbols;
    parser->num_follow_sets = set_size(grammar->nonterminals);
    
    parser->first_sets = malloc(sizeof(Set*) * total_symbols);
    parser->follow_sets = malloc(sizeof(Set*) * parser->num_follow_sets);
    
    for (int i = 0; i < total_symbols; i++) {
        parser->first_sets[i] = create_set();
    }
    for (int i = 0; i < parser->num_follow_sets; i++) {
        parser->follow_sets[i] = create_set();
    }
    
    compute_first_sets(parser);
    compute_follow_sets(parser);
    construct_parsing_table(parser);
    
    return parser;
}

Set* get_first_set(LL1Parser* parser, const char* symbol) {
    Grammar* g = parser->grammar;
    
    for (int i = 0; i < set_size(g->terminals); i++) {
        if (strcmp(set_get(g->terminals, i), symbol) == 0) {
            return parser->first_sets[i];
        }
    }
    
    int offset = set_size(g->terminals);
    for (int i = 0; i < set_size(g->nonterminals); i++) {
        if (strcmp(set_get(g->nonterminals, i), symbol) == 0) {
            return parser->first_sets[offset + i];
        }
    }
    
    if (strcmp(symbol, g->epsilon) == 0) {
        return parser->first_sets[offset + set_size(g->nonterminals)];
    }
    
    return NULL;
}

Set* get_follow_set(LL1Parser* parser, const char* symbol) {
    Grammar* g = parser->grammar;
    
    for (int i = 0; i < set_size(g->nonterminals); i++) {
        if (strcmp(set_get(g->nonterminals, i), symbol) == 0) {
            return parser->follow_sets[i];
        }
    }
    
    return NULL;
}

Set* first_of_sequence(LL1Parser* parser, char** symbols, int length) {
    Grammar* g = parser->grammar;
    Set* result = create_set();
    
    if (length == 0 || (length == 1 && is_epsilon(g, symbols[0]))) {
        add_to_set(result, g->epsilon);
        return result;
    }
    
    bool all_have_epsilon = true;
    
    for (int i = 0; i < length; i++) {
        Set* first_of_symbol = get_first_set(parser, symbols[i]);
        if (!first_of_symbol) {
            all_have_epsilon = false;
            break;
        }
        
        for (int j = 0; j < set_size(first_of_symbol); j++) {
            const char* elem = set_get(first_of_symbol, j);
            if (strcmp(elem, g->epsilon) != 0) {
                add_to_set(result, elem);
            }
        }
        
        if (!contains_set(first_of_symbol, g->epsilon)) {
            all_have_epsilon = false;
            break;
        }
    }
    
    if (all_have_epsilon) {
        add_to_set(result, g->epsilon);
    }
    
    return result;
}

void compute_first_sets(LL1Parser* parser) {
    Grammar* g = parser->grammar;
    
    for (int i = 0; i < set_size(g->terminals); i++) {
        const char* terminal = set_get(g->terminals, i);
        Set* first_set = get_first_set(parser, terminal);
        add_to_set(first_set, terminal);
    }
    
    Set* epsilon_set = get_first_set(parser, g->epsilon);
    add_to_set(epsilon_set, g->epsilon);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int i = 0; i < set_size(g->nonterminals); i++) {
            const char* nonterminal = set_get(g->nonterminals, i);
            Set* nt_first_set = get_first_set(parser, nonterminal);
            int old_size = set_size(nt_first_set);
            
            Production* prods;
            int num_prods = get_productions_for(g, nonterminal, &prods);
            
            for (int j = 0; j < abs(num_prods); j++) {
                Set* first_of_prod = first_of_sequence(parser, prods[j].symbols, prods[j].length);
                union_sets(nt_first_set, first_of_prod);
                free_set(first_of_prod);
            }
            
            if (num_prods < 0) free(prods);  // Only free if malloc'd
            
            if (set_size(nt_first_set) > old_size) {
                changed = true;
            }
        }
    }
}

void compute_follow_sets(LL1Parser* parser) {
    Grammar* g = parser->grammar;
    
    Set* start_follow = get_follow_set(parser, g->start_symbol);
    add_to_set(start_follow, "$");
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int i = 0; i < set_size(g->nonterminals); i++) {
            const char* nonterminal = set_get(g->nonterminals, i);
            
            Production* prods;
            int num_prods = get_productions_for(g, nonterminal, &prods);
            
            for (int j = 0; j < abs(num_prods); j++) {
                Production* prod = &prods[j];
                
                for (int k = 0; k < prod->length; k++) {
                    const char* symbol = prod->symbols[k];
                    if (!is_nonterminal(g, symbol)) {
                        continue;
                    }
                    
                    Set* symbol_follow = get_follow_set(parser, symbol);
                    int old_size = set_size(symbol_follow);
                    
                    int beta_len = prod->length - k - 1;
                    char** beta = beta_len > 0 ? &prod->symbols[k + 1] : NULL;
                    
                    Set* first_of_beta = first_of_sequence(parser, beta, beta_len);
                    
                    for (int m = 0; m < set_size(first_of_beta); m++) {
                        const char* elem = set_get(first_of_beta, m);
                        if (strcmp(elem, g->epsilon) != 0) {
                            add_to_set(symbol_follow, elem);
                        }
                    }
                    
                    if (contains_set(first_of_beta, g->epsilon)) {
                        Set* nt_follow = get_follow_set(parser, nonterminal);
                        union_sets(symbol_follow, nt_follow);
                    }
                    
                    free_set(first_of_beta);
                    
                    if (set_size(symbol_follow) > old_size) {
                        changed = true;
                    }
                }
            }
            
            if (num_prods < 0) free(prods);  // Only free if malloc'd
        }
    }
}

void construct_parsing_table(LL1Parser* parser) {
    Grammar* g = parser->grammar;
    
    for (int i = 0; i < set_size(g->nonterminals); i++) {
        const char* nonterminal = set_get(g->nonterminals, i);
        
        Production* prods;
        int num_prods = get_productions_for(g, nonterminal, &prods);
        
        for (int j = 0; j < abs(num_prods); j++) {
            Production* prod = &prods[j];
            Set* first_of_prod = first_of_sequence(parser, prod->symbols, prod->length);
            
            for (int k = 0; k < set_size(first_of_prod); k++) {
                const char* terminal = set_get(first_of_prod, k);
                if (strcmp(terminal, g->epsilon) != 0) {
                    if (get_table_entry(parser->parsing_table, nonterminal, terminal)) {
                        fprintf(stderr, "Error: Grammar is not LL(1) - conflict at [%s, %s]\n", 
                                nonterminal, terminal);
                        exit(1);
                    }
                    add_table_entry(parser->parsing_table, nonterminal, terminal, nonterminal, j);
                }
            }
            
            if (contains_set(first_of_prod, g->epsilon)) {
                Set* follow_set = get_follow_set(parser, nonterminal);
                for (int k = 0; k < set_size(follow_set); k++) {
                    const char* terminal = set_get(follow_set, k);
                    if (get_table_entry(parser->parsing_table, nonterminal, terminal)) {
                        fprintf(stderr, "Error: Grammar is not LL(1) - conflict at [%s, %s]\n", 
                                nonterminal, terminal);
                        exit(1);
                    }
                    add_table_entry(parser->parsing_table, nonterminal, terminal, nonterminal, j);
                }
            }
            
            free_set(first_of_prod);
        }
        
        if (num_prods < 0) free(prods);  // Only free if malloc'd
    }
}

char** parse(LL1Parser* parser, char** input, int input_len, int* prod_count) {
    Grammar* g = parser->grammar;
    
    char** stack = malloc(sizeof(char*) * 10000);
    int stack_size = 0;
    stack[stack_size++] = strdup("$");
    stack[stack_size++] = strdup(g->start_symbol);
    
    char** productions = malloc(sizeof(char*) * 1000);
    *prod_count = 0;
    
    int input_idx = 0;
    
    while (stack_size > 0) {
        char* top = stack[--stack_size];
        const char* current_input = (input_idx < input_len) ? input[input_idx] : "$";
        
        if (is_terminal(g, top) || strcmp(top, "$") == 0) {
            if (strcmp(top, current_input) == 0) {
                free(top);
                input_idx++;
            } else {
                fprintf(stderr, "Parse error: expected %s, got %s\n", top, current_input);
                exit(1);
            }
        } else if (is_nonterminal(g, top)) {
            TableValue* entry = get_table_entry(parser->parsing_table, top, current_input);
            if (!entry) {
                fprintf(stderr, "Parse error: no entry in parsing table for [%s, %s]\n", 
                        top, current_input);
                exit(1);
            }
            
            Production* prods;
            int num_prods = get_productions_for(g, entry->nonterminal, &prods);
            Production* prod = &prods[entry->prod_index];
            
            char prod_str[1024];
            sprintf(prod_str, "%s ->", top);
            for (int i = 0; i < prod->length; i++) {
                strcat(prod_str, " ");
                strcat(prod_str, prod->symbols[i]);
            }
            productions[(*prod_count)++] = strdup(prod_str);
            
            free(top);
            
            if (!(prod->length == 1 && is_epsilon(g, prod->symbols[0]))) {
                for (int i = prod->length - 1; i >= 0; i--) {
                    stack[stack_size++] = strdup(prod->symbols[i]);
                }
            }
            
            if (num_prods < 0) free(prods);  // Only free if malloc'd
        } else {
            fprintf(stderr, "Unknown symbol: %s\n", top);
            exit(1);
        }
    }
    
    free(stack);
    return productions;
}

void free_parser(LL1Parser* parser) {
    if (!parser) return;
    
    for (int i = 0; i < parser->num_first_sets; i++) {
        free_set(parser->first_sets[i]);
    }
    free(parser->first_sets);
    
    for (int i = 0; i < parser->num_follow_sets; i++) {
        free_set(parser->follow_sets[i]);
    }
    free(parser->follow_sets);
    
    free_parsing_table(parser->parsing_table);
    free(parser);
}
