#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vm.h"

Instr code[MAX_CODE];
int code_size = 0;
double vm_memory[MAX_VARS];

static double to_bool(double v) {
    if (v != 0.0) return 1.0;
    return 0.0;
}

int emit(OpCode op, int a, double d) {
    if (code_size >= MAX_CODE) {
        fprintf(stderr, "Code overflow\n");
        exit(1);
    }
    code[code_size].op = op;
    code[code_size].a = a;
    code[code_size].d = d;
    return code_size++;
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
        case OP_POP:
            if (sp < 0) {
                fprintf(stderr, "Runtime error: POP on empty stack\n");
                exit(1);
            }
            sp--;
            pc++;
            break;
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
