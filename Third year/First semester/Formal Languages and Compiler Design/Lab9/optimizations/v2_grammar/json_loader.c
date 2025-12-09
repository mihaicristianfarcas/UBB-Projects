#include "json_loader.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grammar* load_grammar_from_json(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* json_str = malloc(file_size + 1);
    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';
    fclose(file);
    
    cJSON* json = cJSON_Parse(json_str);
    free(json_str);
    
    if (!json) {
        fprintf(stderr, "Error: Invalid JSON\n");
        return NULL;
    }
    
    cJSON* start = cJSON_GetObjectItem(json, "start_symbol");
    cJSON* epsilon = cJSON_GetObjectItem(json, "epsilon");
    cJSON* terminals = cJSON_GetObjectItem(json, "terminals");
    cJSON* nonterminals = cJSON_GetObjectItem(json, "nonterminals");
    cJSON* productions = cJSON_GetObjectItem(json, "productions");
    
    if (!start || !epsilon || !terminals || !nonterminals || !productions) {
        fprintf(stderr, "Error: Missing required fields in JSON\n");
        cJSON_Delete(json);
        return NULL;
    }
    
    Grammar* g = create_grammar(start->valuestring, epsilon->valuestring);
    
    cJSON* term;
    cJSON_ArrayForEach(term, terminals) {
        add_terminal(g, term->valuestring);
    }
    
    cJSON* nonterm;
    cJSON_ArrayForEach(nonterm, nonterminals) {
        add_nonterminal(g, nonterm->valuestring);
    }
    
    cJSON* nt_prod;
    cJSON_ArrayForEach(nt_prod, productions) {
        const char* nt_name = nt_prod->string;
        cJSON* prod_array = nt_prod;
        
        if (cJSON_IsArray(prod_array)) {
            cJSON* prod;
            cJSON_ArrayForEach(prod, prod_array) {
                int prod_len = cJSON_GetArraySize(prod);
                char** symbols = malloc(sizeof(char*) * prod_len);
                
                for (int i = 0; i < prod_len; i++) {
                    cJSON* symbol = cJSON_GetArrayItem(prod, i);
                    symbols[i] = symbol->valuestring;
                }
                
                add_production(g, nt_name, symbols, prod_len);
                free(symbols);
            }
        }
    }
    
    cJSON_Delete(json);
    
    // OPTIMIZATION 2: Build production index after loading
    build_production_index(g);
    
    return g;
}
