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
     INTEGER = 259,
     FLOAT_LITERAL = 260,
     ABS = 261,
     MIN = 262,
     MAX = 263,
     SQRT = 264,
     POW = 265,
     CHECKLINELEFT = 266,
     CHECKLINERIGHT = 267,
     ACCELERATE = 268,
     SETFORWARD = 269,
     SETBACKWARD = 270,
     BRAKE = 271,
     TURNLEFT = 272,
     TURNRIGHT = 273,
     TURNANGLE = 274,
     VAR = 275,
     CONST = 276,
     FUNC = 277,
     RETURN = 278,
     IF = 279,
     ELSE = 280,
     WHILE = 281,
     DO = 282,
     FOR = 283,
     BREAK = 284,
     CONTINUE = 285,
     PASS = 286,
     TYPE_INT = 287,
     TYPE_TINY = 288,
     TYPE_LONG = 289,
     TYPE_FLOAT = 290,
     TYPE_CHAR = 291,
     TYPE_BOOL = 292,
     TYPE_VOID = 293,
     TRUE = 294,
     FALSE = 295,
     PLUS = 296,
     MINUS = 297,
     MULTIPLY = 298,
     DIVIDE = 299,
     MODULO = 300,
     EQ = 301,
     NEQ = 302,
     LT = 303,
     LTE = 304,
     GT = 305,
     GTE = 306,
     AND = 307,
     OR = 308,
     NOT = 309,
     ASSIGN = 310,
     LPAREN = 311,
     RPAREN = 312,
     LBRACE = 313,
     RBRACE = 314,
     LBRACKET = 315,
     RBRACKET = 316,
     SEMICOLON = 317,
     COLON = 318,
     COMMA = 319
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define INTEGER 259
#define FLOAT_LITERAL 260
#define ABS 261
#define MIN 262
#define MAX 263
#define SQRT 264
#define POW 265
#define CHECKLINELEFT 266
#define CHECKLINERIGHT 267
#define ACCELERATE 268
#define SETFORWARD 269
#define SETBACKWARD 270
#define BRAKE 271
#define TURNLEFT 272
#define TURNRIGHT 273
#define TURNANGLE 274
#define VAR 275
#define CONST 276
#define FUNC 277
#define RETURN 278
#define IF 279
#define ELSE 280
#define WHILE 281
#define DO 282
#define FOR 283
#define BREAK 284
#define CONTINUE 285
#define PASS 286
#define TYPE_INT 287
#define TYPE_TINY 288
#define TYPE_LONG 289
#define TYPE_FLOAT 290
#define TYPE_CHAR 291
#define TYPE_BOOL 292
#define TYPE_VOID 293
#define TRUE 294
#define FALSE 295
#define PLUS 296
#define MINUS 297
#define MULTIPLY 298
#define DIVIDE 299
#define MODULO 300
#define EQ 301
#define NEQ 302
#define LT 303
#define LTE 304
#define GT 305
#define GTE 306
#define AND 307
#define OR 308
#define NOT 309
#define ASSIGN 310
#define LPAREN 311
#define RPAREN 312
#define LBRACE 313
#define RBRACE 314
#define LBRACKET 315
#define RBRACKET 316
#define SEMICOLON 317
#define COLON 318
#define COMMA 319




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 172 "parser.y"
{
    int int_val;
    float float_val;
    char* string_val;
}
/* Line 1529 of yacc.c.  */
#line 183 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

