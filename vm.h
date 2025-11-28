#ifndef VM_H
#define VM_H

#define MAX_VARS 1024
#define MAX_CODE 4096

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
    OP_STORE_IND,
    OP_CALL,
    OP_RET
} OpCode;

typedef struct {
    OpCode op;
    int a;
    double d;
} Instr;

extern Instr code[MAX_CODE];
extern int code_size;
extern double vm_memory[MAX_VARS];
extern int entry_pc;

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
    BI_TURNANGLE,
    BI_ACCELERATELEFT,   // NUEVO
    BI_ACCELERATERIGHT   // NUEVO
} BuiltinId;

int emit(OpCode op, int a, double d);
void run_vm(void);
void save_bytecode(const char* filename);

#endif
