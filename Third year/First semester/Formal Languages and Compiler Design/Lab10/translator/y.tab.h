/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     STRING = 259,
     NUMBER = 260,
     ON = 261,
     WHEN = 262,
     SEND_COMMAND = 263,
     FUNC = 264,
     FOR = 265,
     IN = 266,
     IF = 267,
     END = 268,
     ARROW = 269,
     EQ = 270,
     NE = 271,
     GE = 272,
     LE = 273,
     GT = 274,
     LT = 275,
     PLUS = 276,
     MINUS = 277,
     TIMES = 278,
     DIVIDE = 279,
     MOD = 280,
     ASSIGN = 281,
     COLON = 282,
     COMMA = 283,
     LPAREN = 284,
     RPAREN = 285,
     LBRACKET = 286,
     RBRACKET = 287
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define STRING 259
#define NUMBER 260
#define ON 261
#define WHEN 262
#define SEND_COMMAND 263
#define FUNC 264
#define FOR 265
#define IN 266
#define IF 267
#define END 268
#define ARROW 269
#define EQ 270
#define NE 271
#define GE 272
#define LE 273
#define GT 274
#define LT 275
#define PLUS 276
#define MINUS 277
#define TIMES 278
#define DIVIDE 279
#define MOD 280
#define ASSIGN 281
#define COLON 282
#define COMMA 283
#define LPAREN 284
#define RPAREN 285
#define LBRACKET 286
#define RBRACKET 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 30 "iotiny.y"
{
    int num;
    char *str;
}
/* Line 1529 of yacc.c.  */
#line 118 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

