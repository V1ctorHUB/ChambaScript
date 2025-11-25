%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

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
#define MAX_VARS 1024

static Sym* sym_table[SYM_TABLE_SIZE];
static int current_scope = 0;
static int next_addr = 0;
static double vm_memory[MAX_VARS];

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

typedef enum {
    OP_PUSH_NUM,
    OP_LOAD,
    OP_STORE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_LT,
    OP_LTE,
    OP_GT,
    OP_GTE,
    OP_EQ,
    OP_NEQ,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_CALL_BUILTIN,
    OP_POP,
    OP_HALT,
    OP_LOAD_IND,
    OP_STORE_IND
} OpCode;

typedef struct {
    OpCode op;
    int a;
    double d;
} Instr;

#define MAX_CODE 4096

static Instr code[MAX_CODE];
static int code_size = 0;

static int emit(OpCode op, int a, double d) {
    if (code_size >= MAX_CODE) {
        fprintf(stderr, "Code overflow\n");
        exit(1);
    }
    code[code_size].op = op;
    code[code_size].a = a;
    code[code_size].d = d;
    return code_size++;
}

static double to_bool(double v) {
    if (v != 0.0) return 1.0;
    return 0.0;
}

typedef enum {
    BI_ABS,
    BI_MIN,
    BI_MAX,
    BI_SQRT,
    BI_POW,
    BI_CHECKLINELEFT,
    BI_CHECKLINERIGHT,
    BI_ACCELERATE,
    BI_SETFORWARD,
    BI_SETBACKWARD,
    BI_BRAKE,
    BI_TURNLEFT,
    BI_TURNRIGHT,
    BI_TURNANGLE
} BuiltinId;

void run_vm(void);
void save_bytecode(const char* filename);
%}

%union {
    int   int_val;
    float float_val;
    char* string_val;
}

%token <string_val> IDENTIFIER
%token <int_val>    INTEGER
%token <float_val>  FLOAT_LITERAL

%token ABS MIN MAX SQRT POW
%token CHECKLINELEFT CHECKLINERIGHT
%token ACCELERATE SETFORWARD SETBACKWARD BRAKE
%token TURNLEFT TURNRIGHT TURNANGLE

%token VAR CONST FUNC RETURN IF ELSE WHILE DO FOR
%token BREAK CONTINUE PASS
%token TYPE_INT TYPE_TINY TYPE_LONG TYPE_FLOAT TYPE_CHAR TYPE_BOOL TYPE_VOID
%token TRUE FALSE
%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token EQ NEQ LT LTE GT GTE
%token AND OR NOT
%token ASSIGN LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COLON COMMA

%type <int_val> var_type
%type <int_val> builtin_func
%type <int_val> arguments
%type <int_val> arguments_tail
%type <int_val> array_part
%type <int_val> while_start
%type <int_val> while_jmp

%%

program:
    statement_list
    {
        emit(OP_HALT, 0, 0.0);
    }
    ;

statement_list:
      statement SEMICOLON statement_list
    | statement SEMICOLON
    |
    ;

statement:
      declaration
    | assignment
    | control_structure
    | function_call   { emit(OP_POP, 0, 0.0); }
    | function_declaration
    | return_statement
    | pass_statement
    | break_statement
    | continue_statement
    ;

declaration:
      VAR IDENTIFIER COLON var_type array_part
        {
            if (sym_lookup_local($2)) {
                semantic_error("variable %s already declared in this scope", $2);
            }
            sym_insert($2, SYM_VAR, $4, $5);
        }
    | VAR IDENTIFIER COLON var_type array_part ASSIGN expression
        {
            if (sym_lookup_local($2)) {
                semantic_error("variable %s already declared in this scope", $2);
            }
            if ($5 > 1) {
                semantic_error("array %s cannot be initialized with scalar expression", $2);
            }
            Sym* s = sym_insert($2, SYM_VAR, $4, $5);
            emit(OP_STORE, s->addr, 0.0);
        }
    | CONST IDENTIFIER COLON var_type array_part ASSIGN expression
        {
            if (sym_lookup_local($2)) {
                semantic_error("constant %s already declared in this scope", $2);
            }
            if ($5 > 1) {
                semantic_error("array constants not supported yet for %s", $2);
            }
            Sym* s = sym_insert($2, SYM_CONST, $4, $5);
            emit(OP_STORE, s->addr, 0.0);
        }
    ;

array_part:
      LBRACKET INTEGER RBRACKET   { $$ = $2; }
    |                            { $$ = 1; }
    ;

assignment:
      IDENTIFIER ASSIGN expression
        {
            Sym* s = sym_lookup($1);
            if (!s) {
                semantic_error("use of undeclared identifier %s in assignment", $1);
            }
            if (s->kind == SYM_CONST) {
                semantic_error("assignment to constant %s", $1);
            }
            if (s->is_array) {
                semantic_error("array %s requires an index", $1);
            }
            emit(OP_STORE, s->addr, 0.0);
        }
    | IDENTIFIER LBRACKET expression RBRACKET
        {
            Sym* s = sym_lookup($1);
            if (!s) {
                semantic_error("use of undeclared identifier %s in array assignment", $1);
            }
            if (!s->is_array) {
                semantic_error("identifier %s is not an array", $1);
            }
            if (s->kind == SYM_CONST) {
                semantic_error("assignment to constant array %s", $1);
            }
            emit(OP_PUSH_NUM, 0, (double)s->addr);
            emit(OP_ADD, 0, 0.0);
        }
      ASSIGN expression
        {
            emit(OP_STORE_IND, 0, 0.0);
        }
    ;

var_type:
      TYPE_INT   { $$ = TYPE_INT; }
    | TYPE_TINY  { $$ = TYPE_TINY; }
    | TYPE_LONG  { $$ = TYPE_LONG; }
    | TYPE_FLOAT { $$ = TYPE_FLOAT; }
    | TYPE_CHAR  { $$ = TYPE_CHAR; }
    | TYPE_BOOL  { $$ = TYPE_BOOL; }
    | TYPE_VOID  { $$ = TYPE_VOID; }
    ;

expression:
      logical_or
    ;

logical_or:
      logical_and
    | logical_or OR logical_and
        {
            emit(OP_OR, 0, 0.0);
        }
    ;

logical_and:
      equality
    | logical_and AND equality
        {
            emit(OP_AND, 0, 0.0);
        }
    ;

equality:
      relational
    | equality EQ relational
        {
            emit(OP_EQ, 0, 0.0);
        }
    | equality NEQ relational
        {
            emit(OP_NEQ, 0, 0.0);
        }
    ;

relational:
      additive
    | relational LT additive
        {
            emit(OP_LT, 0, 0.0);
        }
    | relational LTE additive
        {
            emit(OP_LTE, 0, 0.0);
        }
    | relational GT additive
        {
            emit(OP_GT, 0, 0.0);
        }
    | relational GTE additive
        {
            emit(OP_GTE, 0, 0.0);
        }
    ;

additive:
      term
    | additive PLUS term
        {
            emit(OP_ADD, 0, 0.0);
        }
    | additive MINUS term
        {
            emit(OP_SUB, 0, 0.0);
        }
    ;

term:
      factor
    | term MULTIPLY factor
        {
            emit(OP_MUL, 0, 0.0);
        }
    | term DIVIDE factor
        {
            emit(OP_DIV, 0, 0.0);
        }
    | term MODULO factor
        {
            emit(OP_MOD, 0, 0.0);
        }
    ;

factor:
      NOT factor
        {
            emit(OP_NOT, 0, 0.0);
        }
    | LPAREN expression RPAREN
    | number
    | IDENTIFIER
        {
            Sym* s = sym_lookup($1);
            if (!s) {
                semantic_error("use of undeclared identifier %s in expression", $1);
            }
            if (s->addr < 0) {
                semantic_error("identifier %s is not a variable", $1);
            }
            if (s->is_array) {
                semantic_error("array %s requires an index", $1);
            }
            emit(OP_LOAD, s->addr, 0.0);
        }
    | IDENTIFIER LBRACKET expression RBRACKET
        {
            Sym* s = sym_lookup($1);
            if (!s) {
                semantic_error("use of undeclared identifier %s in array access", $1);
            }
            if (!s->is_array) {
                semantic_error("identifier %s is not an array", $1);
            }
            emit(OP_PUSH_NUM, 0, (double)s->addr);
            emit(OP_ADD, 0, 0.0);
            emit(OP_LOAD_IND, 0, 0.0);
        }
    | function_call
    | boolean
    ;

control_structure:
      if_statement
    | while_loop
    | do_while_loop
    | for_loop
    ;

if_statement:
      IF LPAREN boolean_expression RPAREN
        {
            int jmp_false = emit(OP_JUMP_IF_FALSE, 0, 0.0);
            $<int_val>$ = jmp_false;
        }
      LBRACE statement_list RBRACE
        {
            int jmp_false = $<int_val>5;
            int jmp_end = emit(OP_JUMP, 0, 0.0);
            code[jmp_false].a = code_size;
            $<int_val>$ = jmp_end;
        }
      ELSE LBRACE statement_list RBRACE
        {
            int jmp_end = $<int_val>9;
            code[jmp_end].a = code_size;
        }
    ;

while_start:
    {
        $$ = code_size;
    }
    ;

while_jmp:
    {
        int j = emit(OP_JUMP_IF_FALSE, 0, 0.0);
        $$ = j;
    }
    ;

while_loop:
      WHILE while_start LPAREN boolean_expression RPAREN while_jmp LBRACE statement_list RBRACE
        {
            int start = $2;
            int jmp_false = $6;
            emit(OP_JUMP, start, 0.0);
            code[jmp_false].a = code_size;
        }
    ;

do_while_loop:
      DO LBRACE statement_list RBRACE WHILE LPAREN boolean_expression RPAREN
        {
            semantic_error("do-while loop not supported in VM yet");
        }
    ;

for_loop:
      FOR LPAREN declaration SEMICOLON boolean_expression SEMICOLON assignment RPAREN LBRACE statement_list RBRACE
        {
            semantic_error("for loop not supported in VM yet");
        }
    ;

boolean_expression:
      expression
    ;

function_declaration:
      FUNC IDENTIFIER LPAREN parameters RPAREN COLON var_type LBRACE statement_list RBRACE
        {
            semantic_error("user-defined functions not supported in VM yet");
        }
    ;

parameters:
      parameter parameter_tail
    |
    ;

parameter_tail:
      COMMA parameter parameter_tail
    |
    ;

parameter:
      IDENTIFIER COLON var_type
    ;

return_statement:
      RETURN expression
    ;

builtin_func:
      ABS            { $$ = BI_ABS; }
    | MIN            { $$ = BI_MIN; }
    | MAX            { $$ = BI_MAX; }
    | SQRT           { $$ = BI_SQRT; }
    | POW            { $$ = BI_POW; }
    | CHECKLINELEFT  { $$ = BI_CHECKLINELEFT; }
    | CHECKLINERIGHT { $$ = BI_CHECKLINERIGHT; }
    | ACCELERATE     { $$ = BI_ACCELERATE; }
    | SETFORWARD     { $$ = BI_SETFORWARD; }
    | SETBACKWARD    { $$ = BI_SETBACKWARD; }
    | BRAKE          { $$ = BI_BRAKE; }
    | TURNLEFT       { $$ = BI_TURNLEFT; }
    | TURNRIGHT      { $$ = BI_TURNRIGHT; }
    | TURNANGLE      { $$ = BI_TURNANGLE; }
    ;

function_call:
      builtin_func LPAREN arguments RPAREN
        {
            emit(OP_CALL_BUILTIN, $1, (double)$3);
        }
    | IDENTIFIER LPAREN arguments RPAREN
        {
            Sym* s = sym_lookup($1);
            if (!s || s->kind != SYM_FUNC) {
                semantic_error("call to undeclared function %s", $1);
            }
            semantic_error("user-defined function calls not supported in VM yet");
        }
    ;

arguments:
      expression arguments_tail
        {
            $$ = 1 + $2;
        }
    |
        {
            $$ = 0;
        }
    ;

arguments_tail:
      COMMA expression arguments_tail
        {
            $$ = 1 + $3;
        }
    |
        {
            $$ = 0;
        }
    ;

number:
      integer
    | float
    ;

integer:
      MINUS INTEGER
        {
            emit(OP_PUSH_NUM, 0, -(double)$2);
        }
    | INTEGER
        {
            emit(OP_PUSH_NUM, 0, (double)$1);
        }
    ;

float:
      MINUS FLOAT_LITERAL
        {
            emit(OP_PUSH_NUM, 0, -(double)$2);
        }
    | FLOAT_LITERAL
        {
            emit(OP_PUSH_NUM, 0, (double)$1);
        }
    ;

boolean:
      TRUE
        {
            emit(OP_PUSH_NUM, 0, 1.0);
        }
    | FALSE
        {
            emit(OP_PUSH_NUM, 0, 0.0);
        }
    ;

break_statement:
      BREAK
    ;

continue_statement:
      CONTINUE
    ;

pass_statement:
      PASS
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

void run_vm(void) {
    double stack[1024];
    int sp = -1;
    int pc = 0;

    for (;;) {
        Instr in = code[pc];
        switch (in.op) {
        case OP_PUSH_NUM:
            sp++;
            stack[sp] = in.d;
            pc++;
            break;
        case OP_LOAD:
            sp++;
            stack[sp] = vm_memory[in.a];
            pc++;
            break;
        case OP_STORE:
            vm_memory[in.a] = stack[sp];
            sp--;
            pc++;
            break;
        case OP_ADD: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = a + b;
            pc++;
            break;
        }
        case OP_SUB: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = a - b;
            pc++;
            break;
        }
        case OP_MUL: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = a * b;
            pc++;
            break;
        }
        case OP_DIV: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = a / b;
            pc++;
            break;
        }
        case OP_MOD: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = fmod(a, b);
            pc++;
            break;
        }
        case OP_LT: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(a < b);
            pc++;
            break;
        }
        case OP_LTE: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(a <= b);
            pc++;
            break;
        }
        case OP_GT: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(a > b);
            pc++;
            break;
        }
        case OP_GTE: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(a >= b);
            pc++;
            break;
        }
        case OP_EQ: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(a == b);
            pc++;
            break;
        }
        case OP_NEQ: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(a != b);
            pc++;
            break;
        }
        case OP_AND: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(to_bool(a) && to_bool(b));
            pc++;
            break;
        }
        case OP_OR: {
            double b = stack[sp--];
            double a = stack[sp--];
            stack[++sp] = to_bool(to_bool(a) || to_bool(b));
            pc++;
            break;
        }
        case OP_NOT: {
            double a = stack[sp--];
            stack[++sp] = to_bool(a) ? 0.0 : 1.0;
            pc++;
            break;
        }
        case OP_JUMP:
            pc = in.a;
            break;
        case OP_JUMP_IF_FALSE: {
            double cond = stack[sp--];
            if (to_bool(cond) == 0.0) {
                pc = in.a;
            } else {
                pc++;
            }
            break;
        }
        case OP_CALL_BUILTIN: {
            int id = in.a;
            int argc = (int)in.d;
            double args[8];
            int i;
            for (i = argc - 1; i >= 0; --i) {
                args[i] = stack[sp--];
            }
            double res = 0.0;
            switch (id) {
            case BI_ABS:
                if (argc >= 1) res = fabs(args[0]);
                break;
            case BI_MIN:
                if (argc >= 2) res = args[0] < args[1] ? args[0] : args[1];
                break;
            case BI_MAX:
                if (argc >= 2) res = args[0] > args[1] ? args[0] : args[1];
                break;
            case BI_SQRT:
                if (argc >= 1) res = sqrt(args[0]);
                break;
            case BI_POW:
                if (argc >= 2) res = pow(args[0], args[1]);
                break;
            case BI_CHECKLINELEFT:
                printf("checkLineLeft() -> 1\n");
                res = 1.0;
                break;
            case BI_CHECKLINERIGHT:
                printf("checkLineRight() -> 1\n");
                res = 1.0;
                break;
            case BI_ACCELERATE:
                if (argc >= 1) {
                    printf("accelerate(%f)\n", args[0]);
                }
                res = 0.0;
                break;
            case BI_SETFORWARD:
                if (argc >= 1) {
                    printf("setForward(%f)\n", args[0]);
                }
                res = 0.0;
                break;
            case BI_SETBACKWARD:
                if (argc >= 1) {
                    printf("setBackward(%f)\n", args[0]);
                }
                res = 0.0;
                break;
            case BI_BRAKE:
                printf("brake()\n");
                res = 0.0;
                break;
            case BI_TURNLEFT:
                if (argc >= 1) {
                    printf("turnLeft(%f)\n", args[0]);
                }
                res = 0.0;
                break;
            case BI_TURNRIGHT:
                if (argc >= 1) {
                    printf("turnRight(%f)\n", args[0]);
                }
                res = 0.0;
                break;
            case BI_TURNANGLE:
                if (argc >= 1) {
                    printf("turnAngle(%f)\n", args[0]);
                }
                res = 0.0;
                break;
            default:
                res = 0.0;
                break;
            }
            stack[++sp] = res;
            pc++;
            break;
        }
        case OP_POP: {
            if (sp < 0) {
                fprintf(stderr, "Runtime error: POP on empty stack\n");
                exit(1);
            }
            sp--;
            pc++;
            break;
        }
        case OP_LOAD_IND: {
            double addr = stack[sp--];
            int i = (int)addr;
            if (i < 0 || i >= MAX_VARS) {
                fprintf(stderr, "Runtime error: invalid indirect load address %d\n", i);
                exit(1);
            }
            stack[++sp] = vm_memory[i];
            pc++;
            break;
        }
        case OP_STORE_IND: {
            double value = stack[sp--];
            double addr = stack[sp--];
            int i = (int)addr;
            if (i < 0 || i >= MAX_VARS) {
                fprintf(stderr, "Runtime error: invalid indirect store address %d\n", i);
                exit(1);
            }
            vm_memory[i] = value;
            pc++;
            break;
        }
        case OP_HALT:
            printf("Ejecucion terminada\n");
            return;
        default:
            fprintf(stderr, "Unknown opcode %d\n", in.op);
            return;
        }
    }
}

void save_bytecode(const char* filename) {
    FILE* f = fopen(filename, "w");
    int i;
    if (!f) {
        perror("Error al abrir archivo de bytecode");
        return;
    }
    fprintf(f, "%d\n", code_size);
    for (i = 0; i < code_size; i++) {
        fprintf(f, "%d %d %.17g\n", code[i].op, code[i].a, code[i].d);
    }
    fclose(f);
}
