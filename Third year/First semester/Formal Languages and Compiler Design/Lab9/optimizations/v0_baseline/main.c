#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "parser.h"
#include "json_loader.h"
#include <cjson/cJSON.h>

typedef struct {
    int code;
    char* name;
} TokenMapping;

TokenMapping* load_token_mappings(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* json_str = malloc(file_size + 1);
    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';
    fclose(file);
    
    cJSON* json = cJSON_Parse(json_str);
    free(json_str);
    
    if (!json) return NULL;
    
    cJSON* tokens = cJSON_GetObjectItem(json, "tokens");
    if (!tokens) {
        cJSON_Delete(json);
        return NULL;
    }
    
    int size = cJSON_GetArraySize(tokens);
    TokenMapping* mappings = malloc(sizeof(TokenMapping) * size);
    *count = 0;
    
    cJSON* token;
    cJSON_ArrayForEach(token, tokens) {
        mappings[*count].name = strdup(token->string);
        mappings[*count].code = token->valueint;
        (*count)++;
    }
    
    cJSON_Delete(json);
    return mappings;
}

const char* get_token_name(TokenMapping* mappings, int count, int code) {
    for (int i = 0; i < count; i++) {
        if (mappings[i].code == code) {
            return mappings[i].name;
        }
    }
    return NULL;
}

char** read_pif(const char* filename, const char* token_file, int* count) {
    int mapping_count;
    TokenMapping* mappings = load_token_mappings(token_file, &mapping_count);
    if (!mappings) {
        fprintf(stderr, "Error: Cannot load token mappings from %s\n", token_file);
        return NULL;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open PIF file %s\n", filename);
        return NULL;
    }
    
    char** tokens = malloc(sizeof(char*) * 1000);
    *count = 0;
    char line[256];
    
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) <= 1) continue;
        
        int code;
        if (sscanf(line, "%d", &code) == 1) {
            const char* token_name = get_token_name(mappings, mapping_count, code);
            if (token_name) {
                tokens[*count] = strdup(token_name);
                (*count)++;
            }
        }
    }
    
    fclose(file);
    
    for (int i = 0; i < mapping_count; i++) {
        free(mappings[i].name);
    }
    free(mappings);
    
    return tokens;
}

void write_output(const char* filename, char** productions, int prod_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot write to file %s\n", filename);
        return;
    }
    
    fprintf(file, "Productions:\n");
    for (int i = 0; i < prod_count; i++) {
        fprintf(file, "%d. %s\n", i + 1, productions[i]);
    }
    
    fclose(file);
}

int main(int argc, char** argv) {
    if (argc < 7) {
        printf("Usage: %s --grammar <file> --pif <file> --tokens <file> [--output <file>]\n", argv[0]);
        return 1;
    }
    
    char* grammar_file = NULL;
    char* pif_file = NULL;
    char* token_file = NULL;
    char* output_file = "output.txt";
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--grammar") == 0 && i + 1 < argc) {
            grammar_file = argv[++i];
        } else if (strcmp(argv[i], "--pif") == 0 && i + 1 < argc) {
            pif_file = argv[++i];
        } else if (strcmp(argv[i], "--tokens") == 0 && i + 1 < argc) {
            token_file = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        }
    }
    
    if (!grammar_file || !pif_file || !token_file) {
        fprintf(stderr, "Error: Missing required arguments\n");
        return 1;
    }
    
    printf("Loading grammar from %s...\n", grammar_file);
    Grammar* grammar = load_grammar_from_json(grammar_file);
    if (!grammar) {
        return 1;
    }
    
    printf("Creating LL(1) parser...\n");
    LL1Parser* parser = create_parser(grammar);
    
    printf("Reading input from %s...\n", pif_file);
    int token_count;
    char** tokens = read_pif(pif_file, token_file, &token_count);
    
    printf("Parsing %d tokens...\n", token_count);
    int prod_count;
    char** productions = parse(parser, tokens, token_count, &prod_count);
    
    printf("Writing output to %s...\n", output_file);
    write_output(output_file, productions, prod_count);
    
    printf("Done! %d productions used.\n", prod_count);
    
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
    
    for (int i = 0; i < prod_count; i++) {
        free(productions[i]);
    }
    free(productions);
    
    free_parser(parser);
    free_grammar(grammar);
    
    return 0;
}
