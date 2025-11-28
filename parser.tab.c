/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "vm.h"

int yylex(void);
void yyerror(const char* s);
extern FILE* yyin;

typedef enum { SYM_VAR, SYM_CONST, SYM_FUNC } SymKind;

typedef struct Sym {
    char* name;
    SymKind kind;
    int type;
    int scope_level;
    int addr;
    int is_array;
    int length;
    struct Sym* next;
} Sym;

#define SYM_TABLE_SIZE 997

static Sym* sym_table[SYM_TABLE_SIZE];
static int current_scope = 0;
static int next_addr = 0;

static int current_param_addrs[64];
static int current_param_count = 0;

static unsigned sym_hash(const char* s) {
    unsigned h = 5381u;
    unsigned char c;
    while ((c = (unsigned char)*s++) != 0) {
        h = ((h << 5) + h) + c;
    }
    return h % SYM_TABLE_SIZE;
}

static Sym* sym_lookup(const char* name) {
    unsigned h = sym_hash(name);
    Sym* e = sym_table[h];
    while (e) {
        if (strcmp(e->name, name) == 0) {
            return e;
        }
        e = e->next;
    }
    return NULL;
}

static Sym* sym_lookup_local(const char* name) {
    unsigned h = sym_hash(name);
    Sym* e = sym_table[h];
    while (e) {
        if (e->scope_level == current_scope && strcmp(e->name, name) == 0) {
            return e;
        }
        e = e->next;
    }
    return NULL;
}

static Sym* sym_insert(const char* name, SymKind kind, int type, int length) {
    unsigned h = sym_hash(name);
    Sym* e = malloc(sizeof(Sym));
    if (!e) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    e->name = strdup(name);
    e->kind = kind;
    e->type = type;
    e->scope_level = current_scope;
    if (length <= 0) length = 1;
    e->length = length;
    e->is_array = (length > 1);
    if (kind == SYM_VAR || kind == SYM_CONST) {
        if (next_addr + e->length > MAX_VARS) {
            fprintf(stderr, "Too many variables\n");
            exit(1);
        }
        e->addr = next_addr;
        for (int i = 0; i < e->length; ++i) {
            vm_memory[next_addr + i] = 0.0;
        }
        next_addr += e->length;
    } else {
        e->addr = -1;
    }
    e->next = sym_table[h];
    sym_table[h] = e;
    return e;
}

static void semantic_error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Semantic error: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 110 "parser.y"
{
    int   int_val;
    float float_val;
    char* string_val;
}
/* Line 193 of yacc.c.  */
#line 343 "parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 356 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  77
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   218

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNRULES -- Number of states.  */
#define YYNSTATES  203

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    12,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    37,    45,    53,    57,
      58,    62,    63,    71,    73,    75,    77,    79,    81,    83,
      85,    87,    89,    93,    95,    99,   101,   105,   109,   111,
     115,   119,   123,   127,   129,   133,   137,   139,   143,   147,
     151,   154,   158,   160,   162,   167,   169,   171,   173,   175,
     177,   179,   180,   181,   195,   196,   197,   207,   216,   228,
     230,   231,   232,   245,   248,   249,   253,   254,   258,   261,
     263,   265,   267,   269,   271,   273,   275,   277,   279,   281,
     283,   285,   287,   289,   291,   293,   298,   303,   306,   307,
     311,   312,   314,   316,   319,   321,   324,   326,   328,   330,
     332,   334
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      68,     0,    -1,    69,    -1,    70,    64,    69,    -1,    70,
      64,    -1,    -1,    71,    -1,    73,    -1,    84,    -1,   102,
      -1,    94,    -1,   100,    -1,   111,    -1,   109,    -1,   110,
      -1,    22,     3,    65,    75,    72,    -1,    22,     3,    65,
      75,    72,    57,    76,    -1,    23,     3,    65,    75,    72,
      57,    76,    -1,    62,     4,    63,    -1,    -1,     3,    57,
      76,    -1,    -1,     3,    62,    76,    63,    74,    57,    76,
      -1,    34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,
      -1,    39,    -1,    40,    -1,    77,    -1,    78,    -1,    77,
      55,    78,    -1,    79,    -1,    78,    54,    79,    -1,    80,
      -1,    79,    48,    80,    -1,    79,    49,    80,    -1,    81,
      -1,    80,    50,    81,    -1,    80,    51,    81,    -1,    80,
      52,    81,    -1,    80,    53,    81,    -1,    82,    -1,    81,
      43,    82,    -1,    81,    44,    82,    -1,    83,    -1,    82,
      45,    83,    -1,    82,    46,    83,    -1,    82,    47,    83,
      -1,    56,    83,    -1,    58,    76,    59,    -1,   105,    -1,
       3,    -1,     3,    62,    76,    63,    -1,   102,    -1,   108,
      -1,    85,    -1,    90,    -1,    91,    -1,    92,    -1,    -1,
      -1,    26,    58,    93,    59,    86,    60,    69,    61,    87,
      27,    60,    69,    61,    -1,    -1,    -1,    28,    88,    58,
      93,    59,    89,    60,    69,    61,    -1,    29,    60,    69,
      61,    28,    58,    93,    59,    -1,    30,    58,    71,    64,
      93,    64,    73,    59,    60,    69,    61,    -1,    76,    -1,
      -1,    -1,    24,     3,    95,    58,    97,    59,    65,    75,
      60,    96,    69,    61,    -1,    99,    98,    -1,    -1,    66,
      99,    98,    -1,    -1,     3,    65,    75,    -1,    25,    76,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    10,
      -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,    18,    -1,    19,    -1,    20,
      -1,    21,    -1,   101,    58,   103,    59,    -1,     3,    58,
     103,    59,    -1,    76,   104,    -1,    -1,    66,    76,   104,
      -1,    -1,   106,    -1,   107,    -1,    44,     4,    -1,     4,
      -1,    44,     5,    -1,     5,    -1,    41,    -1,    42,    -1,
      31,    -1,    32,    -1,    33,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   147,   147,   154,   155,   156,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   172,   179,   190,   204,   205,
     209,   224,   223,   245,   246,   247,   248,   249,   250,   251,
     255,   259,   260,   267,   268,   275,   276,   280,   287,   288,
     292,   296,   300,   307,   308,   312,   319,   320,   324,   328,
     335,   339,   340,   341,   355,   368,   369,   373,   374,   375,
     376,   381,   386,   380,   400,   406,   413,   423,   430,   437,
     442,   454,   441,   467,   468,   472,   473,   477,   491,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   517,   521,   532,   537,   543,
     548,   554,   555,   559,   563,   570,   574,   581,   585,   592,
     596,   600
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INTEGER", "FLOAT_LITERAL",
  "ABS", "MIN", "MAX", "SQRT", "POW", "CHECKLINELEFT", "CHECKLINERIGHT",
  "ACCELERATE", "SETFORWARD", "SETBACKWARD", "BRAKE", "TURNLEFT",
  "TURNRIGHT", "TURNANGLE", "ACCELERATELEFT", "ACCELERATERIGHT", "VAR",
  "CONST", "FUNC", "RETURN", "IF", "ELSE", "WHILE", "DO", "FOR", "BREAK",
  "CONTINUE", "PASS", "TYPE_INT", "TYPE_TINY", "TYPE_LONG", "TYPE_FLOAT",
  "TYPE_CHAR", "TYPE_BOOL", "TYPE_VOID", "TRUE", "FALSE", "PLUS", "MINUS",
  "MULTIPLY", "DIVIDE", "MODULO", "EQ", "NEQ", "LT", "LTE", "GT", "GTE",
  "AND", "OR", "NOT", "ASSIGN", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
  "LBRACKET", "RBRACKET", "SEMICOLON", "COLON", "COMMA", "$accept",
  "program", "statement_list", "statement", "declaration", "array_part",
  "assignment", "@1", "var_type", "expression", "logical_or",
  "logical_and", "equality", "relational", "additive", "term", "factor",
  "control_structure", "if_statement", "@2", "@3", "while_start",
  "while_jmp", "while_loop", "do_while_loop", "for_loop",
  "boolean_expression", "function_declaration", "@4", "@5", "parameters",
  "parameter_tail", "parameter", "return_statement", "builtin_func",
  "function_call", "arguments", "arguments_tail", "number", "integer",
  "float", "boolean", "break_statement", "continue_statement",
  "pass_statement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    67,    68,    69,    69,    69,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    71,    71,    71,    72,    72,
      73,    74,    73,    75,    75,    75,    75,    75,    75,    75,
      76,    77,    77,    78,    78,    79,    79,    79,    80,    80,
      80,    80,    80,    81,    81,    81,    82,    82,    82,    82,
      83,    83,    83,    83,    83,    83,    83,    84,    84,    84,
      84,    86,    87,    85,    88,    89,    90,    91,    92,    93,
      95,    96,    94,    97,    97,    98,    98,    99,   100,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   102,   102,   103,   103,   104,
     104,   105,   105,   106,   106,   107,   107,   108,   108,   109,
     110,   111
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     2,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     7,     7,     3,     0,
       3,     0,     7,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       2,     3,     1,     1,     4,     1,     1,     1,     1,     1,
       1,     0,     0,    13,     0,     0,     9,     8,    11,     1,
       0,     0,    12,     2,     0,     3,     0,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     2,     0,     3,
       0,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,    64,     0,     0,   109,   110,   111,     0,
       2,     0,     6,     7,     8,    57,    58,    59,    60,    10,
      11,     0,     9,    13,    14,    12,     0,    98,     0,     0,
       0,    70,    53,   104,   106,   107,   108,     0,     0,     0,
      78,    30,    31,    33,    35,    38,    43,    46,    55,    52,
     101,   102,    56,     0,     0,     5,     0,     1,     4,    98,
      20,   100,     0,     0,     0,     0,     0,     0,   103,   105,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
       3,     0,     0,    97,    96,    21,    23,    24,    25,    26,
      27,    28,    29,    19,    19,    74,     0,    51,    32,    34,
      36,    37,    39,    40,    41,    42,    44,    45,    47,    48,
      49,    61,     0,     0,     0,    95,   100,     0,     0,    15,
       0,     0,     0,    76,    54,     0,    65,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,    73,     5,     0,
       0,     0,    22,    18,    16,    17,    77,     0,    76,     0,
       5,     0,     0,     0,     0,    75,    62,     0,    67,     0,
      71,     0,    66,     5,     5,     0,     0,     0,     5,    68,
      72,     0,    63
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    29,    30,    31,    32,   149,    33,   147,   123,   105,
      61,    62,    63,    64,    65,    66,    67,    34,    35,   155,
     191,    74,   169,    36,    37,    38,   106,    39,    86,   194,
     152,   167,   153,    40,    41,    68,    82,   113,    69,    70,
      71,    72,    43,    44,    45
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -105
static const yytype_int16 yypact[] =
{
     141,   -21,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,    39,    41,
      44,     4,    -4,  -105,     1,    16,  -105,  -105,  -105,    88,
    -105,    25,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,    32,  -105,  -105,  -105,  -105,     4,     4,     4,    26,
      29,  -105,    -9,  -105,  -105,  -105,  -105,    68,     4,     4,
    -105,    38,    43,    28,     5,    36,    18,  -105,  -105,  -105,
    -105,  -105,  -105,     4,    40,   141,    59,  -105,   141,     4,
    -105,    33,    42,    37,    -5,    -5,    45,     4,  -105,  -105,
    -105,    46,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,  -105,    47,     4,    48,    49,
    -105,    51,     4,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,    50,    50,    99,    52,  -105,    43,    28,
       5,     5,    36,    36,    36,    36,    18,    18,  -105,  -105,
    -105,  -105,    55,    76,     4,  -105,    33,    54,   104,    60,
      61,    57,    64,    53,  -105,    56,  -105,    66,    62,  -105,
       4,    65,     4,     4,    -5,    67,    99,  -105,   141,    69,
       4,   124,  -105,  -105,  -105,  -105,  -105,    -5,    53,    70,
     141,    71,   -19,    74,    75,  -105,  -105,    73,  -105,    77,
    -105,   109,  -105,   141,   141,    78,    80,    84,   141,  -105,
    -105,    85,  -105
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -105,  -105,   -73,  -105,    63,    58,     6,  -105,   -81,   -20,
    -105,    83,    86,   -10,   -28,   -14,   -52,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -104,  -105,  -105,  -105,
    -105,    -2,    12,  -105,  -105,     0,   102,    72,  -105,  -105,
    -105,  -105,  -105,  -105,  -105
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      42,    60,   108,   142,   124,   110,    90,    52,    53,    54,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    80,    81,    83,   116,
     117,   118,   119,   120,   121,   122,    46,    47,    46,    91,
     158,    48,    49,    48,    50,    55,    56,    51,    57,    47,
     138,   139,   140,    87,    73,    96,    97,    98,    99,    81,
      58,    75,    59,   102,   103,   104,   181,   126,   132,   133,
     134,   135,    88,    89,    76,    42,    94,    95,    42,   100,
     101,    18,    19,   176,   130,   131,   136,   137,    77,    78,
      79,    84,   146,    92,    85,   179,   184,    93,   107,   112,
     115,   114,   151,   125,   157,   127,   141,   187,   161,   143,
     145,   160,   148,   144,   156,   154,   168,   162,   163,   166,
     196,   197,   164,   165,   170,   201,   171,   182,   173,   180,
     188,   186,   177,   189,   192,   190,   195,   193,   198,   109,
     172,   199,   174,   175,     1,   200,   202,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    42,    23,
      24,    25,    26,    27,    28,   128,   185,   183,   178,   129,
      42,   111,   150,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42,    42,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   159
};

static const yytype_int16 yycheck[] =
{
       0,    21,    75,   107,    85,    78,    58,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    46,    47,    48,    34,
      35,    36,    37,    38,    39,    40,    57,    58,    57,    59,
     144,    62,     3,    62,     3,    41,    42,     3,    44,    58,
     102,   103,   104,    62,    58,    50,    51,    52,    53,    79,
      56,    60,    58,    45,    46,    47,   170,    87,    96,    97,
      98,    99,     4,     5,    58,    75,    48,    49,    78,    43,
      44,    22,    23,   164,    94,    95,   100,   101,     0,    64,
      58,    65,   112,    55,    65,   168,   177,    54,    58,    66,
      63,    59,     3,    58,    28,    59,    59,   180,     4,    61,
      59,    57,    62,    64,    59,    63,    60,    57,    57,    66,
     193,   194,    65,    59,    58,   198,    64,     3,    63,    60,
      59,    61,    65,    59,    61,    60,    27,    60,    60,    76,
     160,    61,   162,   163,     3,    61,    61,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   168,    28,
      29,    30,    31,    32,    33,    92,   178,   171,   166,    93,
     180,    79,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   194,    -1,    -1,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   146
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    28,    29,    30,    31,    32,    33,    68,
      69,    70,    71,    73,    84,    85,    90,    91,    92,    94,
     100,   101,   102,   109,   110,   111,    57,    58,    62,     3,
       3,     3,     3,     4,     5,    41,    42,    44,    56,    58,
      76,    77,    78,    79,    80,    81,    82,    83,   102,   105,
     106,   107,   108,    58,    88,    60,    58,     0,    64,    58,
      76,    76,   103,    76,    65,    65,    95,    62,     4,     5,
      83,    76,    55,    54,    48,    49,    50,    51,    52,    53,
      43,    44,    45,    46,    47,    76,    93,    58,    69,    71,
      69,   103,    66,   104,    59,    63,    34,    35,    36,    37,
      38,    39,    40,    75,    75,    58,    76,    59,    78,    79,
      80,    80,    81,    81,    81,    81,    82,    82,    83,    83,
      83,    59,    93,    61,    64,    59,    76,    74,    62,    72,
      72,     3,    97,    99,    63,    86,    59,    28,    93,   104,
      57,     4,    57,    57,    65,    59,    66,    98,    60,    89,
      58,    64,    76,    63,    76,    76,    75,    65,    99,    69,
      60,    93,     3,    73,    75,    98,    61,    69,    59,    59,
      60,    87,    61,    60,    96,    27,    69,    69,    60,    61,
      61,    69,    61
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 148 "parser.y"
    {
        emit(OP_HALT, 0, 0.0);
    ;}
    break;

  case 9:
#line 163 "parser.y"
    { emit(OP_POP, 0, 0.0); ;}
    break;

  case 15:
#line 173 "parser.y"
    {
            if (sym_lookup_local((yyvsp[(2) - (5)].string_val))) {
                semantic_error("variable %s already declared in this scope", (yyvsp[(2) - (5)].string_val));
            }
            sym_insert((yyvsp[(2) - (5)].string_val), SYM_VAR, (yyvsp[(4) - (5)].int_val), (yyvsp[(5) - (5)].int_val));
        ;}
    break;

  case 16:
#line 180 "parser.y"
    {
            if (sym_lookup_local((yyvsp[(2) - (7)].string_val))) {
                semantic_error("variable %s already declared in this scope", (yyvsp[(2) - (7)].string_val));
            }
            if ((yyvsp[(5) - (7)].int_val) > 1) {
                semantic_error("array %s cannot be initialized with scalar expression", (yyvsp[(2) - (7)].string_val));
            }
            Sym* s = sym_insert((yyvsp[(2) - (7)].string_val), SYM_VAR, (yyvsp[(4) - (7)].int_val), (yyvsp[(5) - (7)].int_val));
            emit(OP_STORE, s->addr, 0.0);
        ;}
    break;

  case 17:
#line 191 "parser.y"
    {
            if (sym_lookup_local((yyvsp[(2) - (7)].string_val))) {
                semantic_error("constant %s already declared in this scope", (yyvsp[(2) - (7)].string_val));
            }
            if ((yyvsp[(5) - (7)].int_val) > 1) {
                semantic_error("array constants not supported yet for %s", (yyvsp[(2) - (7)].string_val));
            }
            Sym* s = sym_insert((yyvsp[(2) - (7)].string_val), SYM_CONST, (yyvsp[(4) - (7)].int_val), (yyvsp[(5) - (7)].int_val));
            emit(OP_STORE, s->addr, 0.0);
        ;}
    break;

  case 18:
#line 204 "parser.y"
    { (yyval.int_val) = (yyvsp[(2) - (3)].int_val); ;}
    break;

  case 19:
#line 205 "parser.y"
    { (yyval.int_val) = 1; ;}
    break;

  case 20:
#line 210 "parser.y"
    {
            Sym* s = sym_lookup((yyvsp[(1) - (3)].string_val));
            if (!s) {
                semantic_error("use of undeclared identifier %s in assignment", (yyvsp[(1) - (3)].string_val));
            }
            if (s->kind == SYM_CONST) {
                semantic_error("assignment to constant %s", (yyvsp[(1) - (3)].string_val));
            }
            if (s->is_array) {
                semantic_error("array %s requires an index", (yyvsp[(1) - (3)].string_val));
            }
            emit(OP_STORE, s->addr, 0.0);
        ;}
    break;

  case 21:
#line 224 "parser.y"
    {
            Sym* s = sym_lookup((yyvsp[(1) - (4)].string_val));
            if (!s) {
                semantic_error("use of undeclared identifier %s in array assignment", (yyvsp[(1) - (4)].string_val));
            }
            if (!s->is_array) {
                semantic_error("identifier %s is not an array", (yyvsp[(1) - (4)].string_val));
            }
            if (s->kind == SYM_CONST) {
                semantic_error("assignment to constant array %s", (yyvsp[(1) - (4)].string_val));
            }
            emit(OP_PUSH_NUM, 0, (double)s->addr);
            emit(OP_ADD, 0, 0.0);
        ;}
    break;

  case 22:
#line 239 "parser.y"
    {
            emit(OP_STORE_IND, 0, 0.0);
        ;}
    break;

  case 23:
#line 245 "parser.y"
    { (yyval.int_val) = TYPE_INT; ;}
    break;

  case 24:
#line 246 "parser.y"
    { (yyval.int_val) = TYPE_TINY; ;}
    break;

  case 25:
#line 247 "parser.y"
    { (yyval.int_val) = TYPE_LONG; ;}
    break;

  case 26:
#line 248 "parser.y"
    { (yyval.int_val) = TYPE_FLOAT; ;}
    break;

  case 27:
#line 249 "parser.y"
    { (yyval.int_val) = TYPE_CHAR; ;}
    break;

  case 28:
#line 250 "parser.y"
    { (yyval.int_val) = TYPE_BOOL; ;}
    break;

  case 29:
#line 251 "parser.y"
    { (yyval.int_val) = TYPE_VOID; ;}
    break;

  case 32:
#line 261 "parser.y"
    {
            emit(OP_OR, 0, 0.0);
        ;}
    break;

  case 34:
#line 269 "parser.y"
    {
            emit(OP_AND, 0, 0.0);
        ;}
    break;

  case 36:
#line 277 "parser.y"
    {
            emit(OP_EQ, 0, 0.0);
        ;}
    break;

  case 37:
#line 281 "parser.y"
    {
            emit(OP_NEQ, 0, 0.0);
        ;}
    break;

  case 39:
#line 289 "parser.y"
    {
            emit(OP_LT, 0, 0.0);
        ;}
    break;

  case 40:
#line 293 "parser.y"
    {
            emit(OP_LTE, 0, 0.0);
        ;}
    break;

  case 41:
#line 297 "parser.y"
    {
            emit(OP_GT, 0, 0.0);
        ;}
    break;

  case 42:
#line 301 "parser.y"
    {
            emit(OP_GTE, 0, 0.0);
        ;}
    break;

  case 44:
#line 309 "parser.y"
    {
            emit(OP_ADD, 0, 0.0);
        ;}
    break;

  case 45:
#line 313 "parser.y"
    {
            emit(OP_SUB, 0, 0.0);
        ;}
    break;

  case 47:
#line 321 "parser.y"
    {
            emit(OP_MUL, 0, 0.0);
        ;}
    break;

  case 48:
#line 325 "parser.y"
    {
            emit(OP_DIV, 0, 0.0);
        ;}
    break;

  case 49:
#line 329 "parser.y"
    {
            emit(OP_MOD, 0, 0.0);
        ;}
    break;

  case 50:
#line 336 "parser.y"
    {
            emit(OP_NOT, 0, 0.0);
        ;}
    break;

  case 53:
#line 342 "parser.y"
    {
            Sym* s = sym_lookup((yyvsp[(1) - (1)].string_val));
            if (!s) {
                semantic_error("use of undeclared identifier %s in expression", (yyvsp[(1) - (1)].string_val));
            }
            if (s->addr < 0) {
                semantic_error("identifier %s is not a variable", (yyvsp[(1) - (1)].string_val));
            }
            if (s->is_array) {
                semantic_error("array %s requires an index", (yyvsp[(1) - (1)].string_val));
            }
            emit(OP_LOAD, s->addr, 0.0);
        ;}
    break;

  case 54:
#line 356 "parser.y"
    {
            Sym* s = sym_lookup((yyvsp[(1) - (4)].string_val));
            if (!s) {
                semantic_error("use of undeclared identifier %s in array access", (yyvsp[(1) - (4)].string_val));
            }
            if (!s->is_array) {
                semantic_error("identifier %s is not an array", (yyvsp[(1) - (4)].string_val));
            }
            emit(OP_PUSH_NUM, 0, (double)s->addr);
            emit(OP_ADD, 0, 0.0);
            emit(OP_LOAD_IND, 0, 0.0);
        ;}
    break;

  case 61:
#line 381 "parser.y"
    {
            int jmp_false = emit(OP_JUMP_IF_FALSE, 0, 0.0);
            (yyval.int_val) = jmp_false;
        ;}
    break;

  case 62:
#line 386 "parser.y"
    {
            int jmp_false = (yyvsp[(5) - (8)].int_val);
            int jmp_end = emit(OP_JUMP, 0, 0.0);
            code[jmp_false].a = code_size;
            (yyval.int_val) = jmp_end;
        ;}
    break;

  case 63:
#line 393 "parser.y"
    {
            int jmp_end = (yyvsp[(9) - (13)].int_val);
            code[jmp_end].a = code_size;
        ;}
    break;

  case 64:
#line 400 "parser.y"
    {
        (yyval.int_val) = code_size;
    ;}
    break;

  case 65:
#line 406 "parser.y"
    {
        int j = emit(OP_JUMP_IF_FALSE, 0, 0.0);
        (yyval.int_val) = j;
    ;}
    break;

  case 66:
#line 414 "parser.y"
    {
            int start = (yyvsp[(2) - (9)].int_val);
            int jmp_false = (yyvsp[(6) - (9)].int_val);
            emit(OP_JUMP, start, 0.0);
            code[jmp_false].a = code_size;
        ;}
    break;

  case 67:
#line 424 "parser.y"
    {
            semantic_error("do-while loop not supported in VM yet");
        ;}
    break;

  case 68:
#line 431 "parser.y"
    {
            semantic_error("for loop not supported in VM yet");
        ;}
    break;

  case 70:
#line 442 "parser.y"
    {
            if (sym_lookup_local((yyvsp[(2) - (2)].string_val))) {
                semantic_error("function %s already declared", (yyvsp[(2) - (2)].string_val));
            }
            Sym* f = sym_insert((yyvsp[(2) - (2)].string_val), SYM_FUNC, TYPE_VOID, 0);
            f->addr = code_size;
            if (strcmp((yyvsp[(2) - (2)].string_val), "main") == 0) {
                entry_pc = code_size;
            }
            current_param_count = 0;
        ;}
    break;

  case 71:
#line 454 "parser.y"
    {
            int i;
            for (i = current_param_count - 1; i >= 0; --i) {
                emit(OP_STORE, current_param_addrs[i], 0.0);
            }
        ;}
    break;

  case 72:
#line 461 "parser.y"
    {
            emit(OP_RET, 0, 0.0);
        ;}
    break;

  case 77:
#line 478 "parser.y"
    {
            if (sym_lookup_local((yyvsp[(1) - (3)].string_val))) {
                semantic_error("parameter %s already declared", (yyvsp[(1) - (3)].string_val));
            }
            Sym* p = sym_insert((yyvsp[(1) - (3)].string_val), SYM_VAR, (yyvsp[(3) - (3)].int_val), 1);
            if (current_param_count >= 64) {
                semantic_error("too many parameters");
            }
            current_param_addrs[current_param_count++] = p->addr;
        ;}
    break;

  case 78:
#line 492 "parser.y"
    {
            emit(OP_RET, 0, 0.0);
        ;}
    break;

  case 79:
#line 498 "parser.y"
    { (yyval.int_val) = BI_ABS; ;}
    break;

  case 80:
#line 499 "parser.y"
    { (yyval.int_val) = BI_MIN; ;}
    break;

  case 81:
#line 500 "parser.y"
    { (yyval.int_val) = BI_MAX; ;}
    break;

  case 82:
#line 501 "parser.y"
    { (yyval.int_val) = BI_SQRT; ;}
    break;

  case 83:
#line 502 "parser.y"
    { (yyval.int_val) = BI_POW; ;}
    break;

  case 84:
#line 503 "parser.y"
    { (yyval.int_val) = BI_CHECKLINELEFT; ;}
    break;

  case 85:
#line 504 "parser.y"
    { (yyval.int_val) = BI_CHECKLINERIGHT; ;}
    break;

  case 86:
#line 505 "parser.y"
    { (yyval.int_val) = BI_ACCELERATE; ;}
    break;

  case 87:
#line 506 "parser.y"
    { (yyval.int_val) = BI_SETFORWARD; ;}
    break;

  case 88:
#line 507 "parser.y"
    { (yyval.int_val) = BI_SETBACKWARD; ;}
    break;

  case 89:
#line 508 "parser.y"
    { (yyval.int_val) = BI_BRAKE; ;}
    break;

  case 90:
#line 509 "parser.y"
    { (yyval.int_val) = BI_TURNLEFT; ;}
    break;

  case 91:
#line 510 "parser.y"
    { (yyval.int_val) = BI_TURNRIGHT; ;}
    break;

  case 92:
#line 511 "parser.y"
    { (yyval.int_val) = BI_TURNANGLE; ;}
    break;

  case 93:
#line 512 "parser.y"
    { (yyval.int_val) = BI_ACCELERATELEFT; ;}
    break;

  case 94:
#line 513 "parser.y"
    { (yyval.int_val) = BI_ACCELERATERIGHT; ;}
    break;

  case 95:
#line 518 "parser.y"
    {
            emit(OP_CALL_BUILTIN, (yyvsp[(1) - (4)].int_val), (double)(yyvsp[(3) - (4)].int_val));
        ;}
    break;

  case 96:
#line 522 "parser.y"
    {
            Sym* s = sym_lookup((yyvsp[(1) - (4)].string_val));
            if (!s || s->kind != SYM_FUNC) {
                semantic_error("call to undeclared function %s", (yyvsp[(1) - (4)].string_val));
            }
            emit(OP_CALL, s->addr, 0.0);
        ;}
    break;

  case 97:
#line 533 "parser.y"
    {
            (yyval.int_val) = 1 + (yyvsp[(2) - (2)].int_val);
        ;}
    break;

  case 98:
#line 537 "parser.y"
    {
            (yyval.int_val) = 0;
        ;}
    break;

  case 99:
#line 544 "parser.y"
    {
            (yyval.int_val) = 1 + (yyvsp[(3) - (3)].int_val);
        ;}
    break;

  case 100:
#line 548 "parser.y"
    {
            (yyval.int_val) = 0;
        ;}
    break;

  case 103:
#line 560 "parser.y"
    {
            emit(OP_PUSH_NUM, 0, -(double)(yyvsp[(2) - (2)].int_val));
        ;}
    break;

  case 104:
#line 564 "parser.y"
    {
            emit(OP_PUSH_NUM, 0, (double)(yyvsp[(1) - (1)].int_val));
        ;}
    break;

  case 105:
#line 571 "parser.y"
    {
            emit(OP_PUSH_NUM, 0, -(double)(yyvsp[(2) - (2)].float_val));
        ;}
    break;

  case 106:
#line 575 "parser.y"
    {
            emit(OP_PUSH_NUM, 0, (double)(yyvsp[(1) - (1)].float_val));
        ;}
    break;

  case 107:
#line 582 "parser.y"
    {
            emit(OP_PUSH_NUM, 0, 1.0);
        ;}
    break;

  case 108:
#line 586 "parser.y"
    {
            emit(OP_PUSH_NUM, 0, 0.0);
        ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2307 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 603 "parser.y"


void yyerror(const char* s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

