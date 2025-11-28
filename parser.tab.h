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
     ROTW = 277,
     ROTC = 278,
     DELAYSECONDS = 279,
     VAR = 280,
     CONST = 281,
     FUNC = 282,
     RETURN = 283,
     IF = 284,
     ELSE = 285,
     WHILE = 286,
     DO = 287,
     FOR = 288,
     BREAK = 289,
     CONTINUE = 290,
     PASS = 291,
     TYPE_INT = 292,
     TYPE_TINY = 293,
     TYPE_LONG = 294,
     TYPE_FLOAT = 295,
     TYPE_CHAR = 296,
     TYPE_BOOL = 297,
     TYPE_VOID = 298,
     TRUE = 299,
     FALSE = 300,
     PLUS = 301,
     MINUS = 302,
     MULTIPLY = 303,
     DIVIDE = 304,
     MODULO = 305,
     EQ = 306,
     NEQ = 307,
     LT = 308,
     LTE = 309,
     GT = 310,
     GTE = 311,
     AND = 312,
     OR = 313,
     NOT = 314,
     ASSIGN = 315,
     LPAREN = 316,
     RPAREN = 317,
     LBRACE = 318,
     RBRACE = 319,
     LBRACKET = 320,
     RBRACKET = 321,
     SEMICOLON = 322,
     COLON = 323,
     COMMA = 324
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
#define ROTW 277
#define ROTC 278
#define DELAYSECONDS 279
#define VAR 280
#define CONST 281
#define FUNC 282
#define RETURN 283
#define IF 284
#define ELSE 285
#define WHILE 286
#define DO 287
#define FOR 288
#define BREAK 289
#define CONTINUE 290
#define PASS 291
#define TYPE_INT 292
#define TYPE_TINY 293
#define TYPE_LONG 294
#define TYPE_FLOAT 295
#define TYPE_CHAR 296
#define TYPE_BOOL 297
#define TYPE_VOID 298
#define TRUE 299
#define FALSE 300
#define PLUS 301
#define MINUS 302
#define MULTIPLY 303
#define DIVIDE 304
#define MODULO 305
#define EQ 306
#define NEQ 307
#define LT 308
#define LTE 309
#define GT 310
#define GTE 311
#define AND 312
#define OR 313
#define NOT 314
#define ASSIGN 315
#define LPAREN 316
#define RPAREN 317
#define LBRACE 318
#define RBRACE 319
#define LBRACKET 320
#define RBRACKET 321
#define SEMICOLON 322
#define COLON 323
#define COMMA 324




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 110 "parser.y"
{
    int   int_val;
    float float_val;
    char* string_val;
}
/* Line 1529 of yacc.c.  */
#line 193 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

