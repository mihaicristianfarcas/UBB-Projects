/* IoTiny to C Translator - Lexer */
%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

int line_num = 1;
%}

%option noyywrap

%%

"on"            { return ON; }
"when"          { return WHEN; }
"send_command"  { return SEND_COMMAND; }
"func"          { return FUNC; }
"for"           { return FOR; }
"in"            { return IN; }
"if"            { return IF; }
"end"           { return END; }

"->"            { return ARROW; }
"=="            { return EQ; }
"!="            { return NE; }
">="            { return GE; }
"<="            { return LE; }
">"             { return GT; }
"<"             { return LT; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return DIVIDE; }
"%"             { return MOD; }

"="             { return ASSIGN; }
":"             { return COLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.str = strdup(yytext); 
    return IDENTIFIER; 
}

[0-9]+          { 
    yylval.num = atoi(yytext); 
    return NUMBER; 
}

\"[^\"]*\"      { 
    yylval.str = strndup(yytext+1, strlen(yytext)-2); 
    return STRING; 
}

[ \t]+          { /* skip whitespace */ }
\n              { line_num++; }
#.*             { /* skip comments */ }

.               { fprintf(stderr, "Line %d: Unknown character '%s'\n", line_num, yytext); }

%%
