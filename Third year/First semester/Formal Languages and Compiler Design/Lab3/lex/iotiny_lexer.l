%{
#include <stdio.h>
%}

IDENTIFIER    [A-Za-z][A-Za-z0-9_]*
NUMBER        [0-9]+
STRING        "[A-Za-z0-9 _\-\.,:]+"
ARROW         ->
RELOP_EQ      ==
RELOP_NE      !=
RELOP_GE      >=
RELOP_LE      <=

%%
"on"              { printf("ON "); }
"when"            { printf("WHEN "); }
"send_command"    { printf("SEND_COMMAND "); }
"func"            { printf("FUNC "); }
"for"             { printf("FOR "); }
"in"              { printf("IN "); }
"if"              { printf("IF "); }
"end"             { printf("END "); }
{RELOP_EQ}         { printf("EQ "); }
{RELOP_NE}         { printf("NE "); }
{RELOP_GE}         { printf("GE "); }
{RELOP_LE}         { printf("LE "); }
{ARROW}            { printf("ARROW "); }
":"                { printf("COLON "); }
"="                { printf("ASSIGN "); }
","                { printf("COMMA "); }
"("                { printf("LPAREN "); }
")"                { printf("RPAREN "); }
"["                { printf("LBRACKET "); }
"]"                { printf("RBRACKET "); }
"+"                { printf("PLUS "); }
"-"                { printf("MINUS "); }
"*"                { printf("STAR "); }
"/"                { printf("SLASH "); }
"%"                { printf("PERCENT "); }
">"                { printf("GT "); }
"<"                { printf("LT "); }
{IDENTIFIER}       { printf("IDENT(%s) ", yytext); }
{NUMBER}           { printf("INT(%s) ", yytext); }
{STRING}           { printf("STRING(%s) ", yytext); }
"#"[A-Za-z0-9 _\-\.,:\(\)\[\]'\"]* { /* comment - ignore */ }
[\t\r\n ]+        { /* skip whitespace */ }
.                  { printf("LEXERR(%s) ", yytext); }
%%

int yywrap(void) {
    return 1;
}
