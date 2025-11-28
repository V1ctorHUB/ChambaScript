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
     ACCELERATELEFT = 275,
     ACCELERATERIGHT = 276,
     VAR = 277,
     CONST = 278,
     FUNC = 279,
     RETURN = 280,
     IF = 281,
     ELSE = 282,
     WHILE = 283,
     DO = 284,
     FOR = 285,
     BREAK = 286,
     CONTINUE = 287,
     PASS = 288,
     TYPE_INT = 289,
     TYPE_TINY = 290,
     TYPE_LONG = 291,
     TYPE_FLOAT = 292,
     TYPE_CHAR = 293,
     TYPE_BOOL = 294,
     TYPE_VOID = 295,
     TRUE = 296,
     FALSE = 297,
     PLUS = 298,
     MINUS = 299,
     MULTIPLY = 300,
     DIVIDE = 301,
     MODULO = 302,
     EQ = 303,
     NEQ = 304,
     LT = 305,
     LTE = 306,
     GT = 307,
     GTE = 308,
     AND = 309,
     OR = 310,
     NOT = 311,
     ASSIGN = 312,
     LPAREN = 313,
     RPAREN = 314,
     LBRACE = 315,
     RBRACE = 316,
     LBRACKET = 317,
     RBRACKET = 318,
     SEMICOLON = 319,
     COLON = 320,
     COMMA = 321
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
#define ACCELERATELEFT 275
#define ACCELERATERIGHT 276
#define VAR 277
#define CONST 278
#define FUNC 279
#define RETURN 280
#define IF 281
#define ELSE 282
#define WHILE 283
#define DO 284
#define FOR 285
#define BREAK 286
#define CONTINUE 287
#define PASS 288
#define TYPE_INT 289
#define TYPE_TINY 290
#define TYPE_LONG 291
#define TYPE_FLOAT 292
#define TYPE_CHAR 293
#define TYPE_BOOL 294
#define TYPE_VOID 295
#define TRUE 296
#define FALSE 297
#define PLUS 298
#define MINUS 299
#define MULTIPLY 300
#define DIVIDE 301
#define MODULO 302
#define EQ 303
#define NEQ 304
#define LT 305
#define LTE 306
#define GT 307
#define GTE 308
#define AND 309
#define OR 310
#define NOT 311
#define ASSIGN 312
#define LPAREN 313
#define RPAREN 314
#define LBRACE 315
#define RBRACE 316
#define LBRACKET 317
#define RBRACKET 318
#define SEMICOLON 319
#define COLON 320
#define COMMA 321




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 110 "parser.y"
{
    int   int_val;
    float float_val;
    char* string_val;
}
/* Line 1529 of yacc.c.  */
#line 187 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

