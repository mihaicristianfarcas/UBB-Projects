/* IoTiny to C Translator - Parser */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void yyerror(const char *s);
int yylex(void);
extern int line_num;

FILE *output;
int indent_level = 0;
int temp_var_count = 0;
int listener_count = 0;

// Buffers for different sections
char global_code[8192] = "";
char main_code[8192] = "";
char function_code[8192] = "";
int seen_statement = 0;
int in_listener = 0;

void emit(const char *format, ...);
void emit_global(const char *format, ...);
void emit_indent();
char* new_temp();
%}

%union {
    int num;
    char *str;
}

%token <str> IDENTIFIER STRING
%token <num> NUMBER
%token ON WHEN SEND_COMMAND FUNC FOR IN IF END
%token ARROW EQ NE GE LE GT LT
%token PLUS MINUS TIMES DIVIDE MOD
%token ASSIGN COLON COMMA LPAREN RPAREN LBRACKET RBRACKET

%type <str> expression condition arithmetic_expr function_call
%type <str> relop arith_op

%left PLUS MINUS
%left TIMES DIVIDE MOD

%%

program:
    { 
        fprintf(output, "#include <stdio.h>\n");
        fprintf(output, "#include <stdlib.h>\n");
        fprintf(output, "#include <string.h>\n\n");
        fprintf(output, "// IoTiny Runtime Support\n");
        fprintf(output, "void send_command(const char* device, const char* cmd) {\n");
        fprintf(output, "    printf(\"[COMMAND] %%s: %%s\\n\", device, cmd);\n");
        fprintf(output, "}\n\n");
        fprintf(output, "void trigger(const char* event, int value) {\n");
        fprintf(output, "    printf(\"[TRIGGER] %%s -> %%d\\n\", event, value);\n");
        fprintf(output, "}\n\n");
        seen_statement = 0;
        in_listener = 0;
    }
    statement_list
    {
        // Emit collected code
        fprintf(output, "%s", global_code);
        fprintf(output, "%s", function_code);
        fprintf(output, "\nint main() {\n");
        fprintf(output, "    printf(\"IoTiny Program Starting...\\n\");\n");
        fprintf(output, "%s", main_code);
        fprintf(output, "    return 0;\n");
        fprintf(output, "}\n");
    }
    ;

statement_list:
    statement statement_list
    | statement
    ;

statement:
    listener_definition
    | assignment
    | if_statement
    | for_loop
    | function_definition
    | send_command_stmt
    | trigger_call
    | expression_statement
    ;

listener_definition:
    ON STRING WHEN condition COLON
    {
        listener_count++;
        seen_statement = 1;
        in_listener = 1;
        char buf[1024];
        snprintf(buf, sizeof(buf), "\n// Event listener #%d for %s\nvoid listener_%d() {\n", 
                 listener_count, $2, listener_count);
        strcat(function_code, buf);
        snprintf(buf, sizeof(buf), "    if (%s) {\n", $4);
        strcat(function_code, buf);
        free($2);
        free($4);
    }
    block END
    {
        strcat(function_code, "    }\n}\n");
        in_listener = 0;
    }
    ;

trigger_call:
    STRING ARROW expression
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "    trigger(%s, %s);\n", $1, $3);
        strcat(main_code, buf);
        free($1);
        free($3);
    }
    ;

send_command_stmt:
    SEND_COMMAND expression COMMA expression
    {
        char buf[256];
        if (in_listener) {
            snprintf(buf, sizeof(buf), "        send_command(%s, %s);\n", $2, $4);
            strcat(function_code, buf);
        } else {
            snprintf(buf, sizeof(buf), "    send_command(%s, %s);\n", $2, $4);
            strcat(main_code, buf);
        }
        seen_statement = 1;
        free($2);
        free($4);
    }
    ;

function_definition:
    FUNC IDENTIFIER LPAREN parameter_list RPAREN COLON
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "\nvoid %s() {\n", $2);
        strcat(function_code, buf);
        free($2);
    }
    block END
    {
        strcat(function_code, "}\n");
    }
    ;

parameter_list:
    /* empty */
    | IDENTIFIER
    | IDENTIFIER COMMA parameter_list
    ;

if_statement:
    IF condition COLON
    {
        char buf[256];
        seen_statement = 1;
        snprintf(buf, sizeof(buf), "    if (%s) {\n", $2);
        strcat(main_code, buf);
        free($2);
    }
    block END
    {
        strcat(main_code, "    }\n");
    }
    ;

for_loop:
    FOR IDENTIFIER IN expression COLON
    {
        char buf[512];
        seen_statement = 1;
        snprintf(buf, sizeof(buf), "    // for %s in %s\n    for (int %s = 0; %s < %s; %s++) {\n", 
                 $2, $4, $2, $2, $4, $2);
        strcat(main_code, buf);
        free($2);
        free($4);
    }
    block END
    {
        strcat(main_code, "    }\n");
    }
    ;

assignment:
    IDENTIFIER ASSIGN expression
    {
        char buf[256];
        if (in_listener) {
            // Inside listener - local assignment
            snprintf(buf, sizeof(buf), "        int %s = %s;\n", $1, $3);
            strcat(function_code, buf);
        } else if (seen_statement == 0) {
            // First assignment - make it global
            snprintf(buf, sizeof(buf), "int %s = %s;\n", $1, $3);
            strcat(global_code, buf);
        } else {
            // Subsequent assignments - in main
            snprintf(buf, sizeof(buf), "    int %s = %s;\n", $1, $3);
            strcat(main_code, buf);
        }
        free($1);
        free($3);
    }
    ;

expression_statement:
    expression
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "    %s;\n", $1);
        strcat(main_code, buf);
        free($1);
    }
    ;

block:
    statement_list
    ;

condition:
    expression relop expression
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s %s %s", $1, $2, $3);
        $$ = strdup(buf);
        free($1);
        free($2);
        free($3);
    }
    ;

expression:
    NUMBER
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", $1);
        $$ = strdup(buf);
    }
    | STRING
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "\"%s\"", $1);
        $$ = strdup(buf);
        free($1);
    }
    | IDENTIFIER
    {
        $$ = $1;
    }
    | arithmetic_expr
    {
        $$ = $1;
    }
    | function_call
    {
        $$ = $1;
    }
    | LBRACKET list_items RBRACKET
    {
        $$ = strdup("NULL /* list not supported */");
    }
    ;

arithmetic_expr:
    expression arith_op expression
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "(%s %s %s)", $1, $2, $3);
        $$ = strdup(buf);
        free($1);
        free($2);
        free($3);
    }
    ;

function_call:
    IDENTIFIER LPAREN argument_list RPAREN
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s()", $1);
        $$ = strdup(buf);
        free($1);
    }
    ;

argument_list:
    /* empty */
    | expression
    | expression COMMA argument_list
    ;

list_items:
    /* empty */
    | expression
    | expression COMMA list_items
    ;

relop:
    GT { $$ = strdup(">"); }
    | LT { $$ = strdup("<"); }
    | EQ { $$ = strdup("=="); }
    | NE { $$ = strdup("!="); }
    | GE { $$ = strdup(">="); }
    | LE { $$ = strdup("<="); }
    ;

arith_op:
    PLUS { $$ = strdup("+"); }
    | MINUS { $$ = strdup("-"); }
    | TIMES { $$ = strdup("*"); }
    | DIVIDE { $$ = strdup("/"); }
    | MOD { $$ = strdup("%"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Line %d: %s\n", line_num, s);
}

void emit_indent() {
    for (int i = 0; i < indent_level; i++) {
        fprintf(output, "    ");
    }
}

void emit(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(output, format, args);
    va_end(args);
}

char* new_temp() {
    char *temp = malloc(32);
    snprintf(temp, 32, "t%d", temp_var_count++);
    return temp;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.iotiny> [output.c]\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    const char *output_file = argc > 2 ? argv[2] : "output.c";
    output = fopen(output_file, "w");
    if (!output) {
        fprintf(stderr, "Error: Cannot open output file %s\n", output_file);
        fclose(input);
        return 1;
    }

    extern FILE *yyin;
    yyin = input;

    printf("Translating %s to %s...\n", argv[1], output_file);
    
    int result = yyparse();
    
    fclose(input);
    fclose(output);

    if (result == 0) {
        printf("Translation successful!\n");
        printf("Generated C code in: %s\n", output_file);
    } else {
        printf("Translation failed with errors.\n");
    }

    return result;
}
