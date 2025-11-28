#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

const int IN1 = 7;
const int IN2 = 4;
const int ENA = 6;

const int IN3 = 3;
const int IN4 = 2;
const int ENB = 5;

const int sensorIzqPin = 9;
const int sensorDerPin = 8;

const bool LINEA_ALTA = true;
const int SD_CS_PIN = 10;

#define MAX_CODE 84
#define MAX_VARS 48

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

typedef struct {
  OpCode op;
  int a;
  float d;
} Instr;

Instr code[MAX_CODE];
int code_size = 0;
float vm_memory[MAX_VARS];
int entry_pc = 0;

bool programLoaded = false;
bool executed = false;

static float to_bool(float v) {
  if (v != 0.0f) return 1.0f;
  return 0.0f;
}

int clampPWM(int v) {
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  return v;
}

bool hayLinea(int pin) {
  int v = digitalRead(pin);
  if (LINEA_ALTA) {
    return v == HIGH;
  } else {
    return v == LOW;
  }
}

void adelante() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void atras() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void setPWM(int pwmIzq, int pwmDer) {
  pwmIzq = clampPWM(pwmIzq);
  pwmDer = clampPWM(pwmDer);
  analogWrite(ENA, pwmIzq);
  analogWrite(ENB, pwmDer);
}

void run_vm() {
  float stack[32];
  int sp = -1;
  int pc = entry_pc;
  int call_stack[16];
  int call_sp = 0;

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
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = a + b;
      pc++;
      break;
    }
    case OP_SUB: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = a - b;
      pc++;
      break;
    }
    case OP_MUL: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = a * b;
      pc++;
      break;
    }
    case OP_DIV: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = a / b;
      pc++;
      break;
    }
    case OP_MOD: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = fmod(a, b);
      pc++;
      break;
    }
    case OP_LT: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(a < b);
      pc++;
      break;
    }
    case OP_LTE: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(a <= b);
      pc++;
      break;
    }
    case OP_GT: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(a > b);
      pc++;
      break;
    }
    case OP_GTE: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(a >= b);
      pc++;
      break;
    }
    case OP_EQ: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(a == b);
      pc++;
      break;
    }
    case OP_NEQ: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(a != b);
      pc++;
      break;
    }
    case OP_AND: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(to_bool(a) && to_bool(b));
      pc++;
      break;
    }
    case OP_OR: {
      float b = stack[sp--];
      float a = stack[sp--];
      stack[++sp] = to_bool(to_bool(a) || to_bool(b));
      pc++;
      break;
    }
    case OP_NOT: {
      float a = stack[sp--];
      stack[++sp] = to_bool(a) ? 0.0f : 1.0f;
      pc++;
      break;
    }
    case OP_JUMP:
      pc = in.a;
      break;
    case OP_JUMP_IF_FALSE: {
      float cond = stack[sp--];
      if (to_bool(cond) == 0.0f) {
        pc = in.a;
      } else {
        pc++;
      }
      break;
    }
    case OP_CALL_BUILTIN: {
      int id = in.a;
      int argc = (int)in.d;
      float args[4];
      int i;
      for (i = argc - 1; i >= 0; --i) {
        args[i] = stack[sp--];
      }
      float res = 0.0f;
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
      case BI_CHECKLINELEFT: {
        bool v = hayLinea(sensorIzqPin);
        res = v ? 1.0f : 0.0f;
        break;
      }
      case BI_CHECKLINERIGHT: {
        bool v = hayLinea(sensorDerPin);
        res = v ? 1.0f : 0.0f;
        break;
      }
      case BI_ACCELERATE:
        if (argc >= 1) {
          int s = clampPWM((int)args[0]);
          setPWM(s, s);
        }
        res = 0.0f;
        break;
      case BI_SETFORWARD:
        if (argc >= 1) {
          int s = clampPWM((int)args[0]);
          adelante();
          setPWM(s, s);
        }
        res = 0.0f;
        break;
      case BI_SETBACKWARD:
        if (argc >= 1) {
          int s = clampPWM((int)args[0]);
          atras();
          setPWM(s, s);
        }
        res = 0.0f;
        break;
      case BI_BRAKE:
        setPWM(0, 0);
        res = 0.0f;
        break;
      case BI_TURNLEFT:
        if (argc >= 1) {
          int s = clampPWM((int)args[0]);
          setPWM(0, s);
        }
        res = 0.0f;
        break;
      case BI_TURNRIGHT:
        if (argc >= 1) {
          int s = clampPWM((int)args[0]);
          setPWM(s, 0);
        }
        res = 0.0f;
        break;
      case BI_TURNANGLE:
        if (argc >= 1) {
          int s = clampPWM(180);
          if (args[0] > 0) {
            setPWM(0, s);
          } else {
            setPWM(s, 0);
          }
        }
        res = 0.0f;
        break;
      default:
        res = 0.0f;
        break;
      }
      stack[++sp] = res;
      pc++;
      break;
    }
    case OP_POP:
      if (sp < 0) {
        return;
      }
      sp--;
      pc++;
      break;
    case OP_LOAD_IND: {
      float addr = stack[sp--];
      int i = (int)addr;
      if (i < 0 || i >= MAX_VARS) {
        return;
      }
      stack[++sp] = vm_memory[i];
      pc++;
      break;
    }
    case OP_STORE_IND: {
      float value = stack[sp--];
      float addr = stack[sp--];
      int i = (int)addr;
      if (i < 0 || i >= MAX_VARS) {
        return;
      }
      vm_memory[i] = value;
      pc++;
      break;
    }
    case OP_CALL:
      if (call_sp >= 16) {
        return;
      }
      call_stack[call_sp++] = pc + 1;
      pc = in.a;
      break;
    case OP_RET:
      if (call_sp <= 0) {
        return;
      }
      pc = call_stack[--call_sp];
      break;
    case OP_HALT:
      return;
    default:
      return;
    }
  }
}

bool loadBytecode(const char* filename) {
  File f = SD.open(filename, FILE_READ);
  if (!f) {
    return false;
  }
  String line = f.readStringUntil('\n');
  line.trim();
  int n = line.toInt();
  if (n <= 0 || n > MAX_CODE) {
    f.close();
    return false;
  }
  code_size = n;
  for (int i = 0; i < n; i++) {
    String l = f.readStringUntil('\n');
    l.trim();
    if (l.length() == 0) {
      f.close();
      return false;
    }
    int op, a;
    float d;
    const char* cstr = l.c_str();
    if (sscanf(cstr, "%d %d %f", &op, &a, &d) != 3) {
      f.close();
      return false;
    }
    code[i].op = (OpCode)op;
    code[i].a = a;
    code[i].d = d;
  }
  f.close();
  entry_pc = 0;
  return true;
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(sensorIzqPin, INPUT);
  pinMode(sensorDerPin, INPUT);

  Serial.begin(9600);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD ERROR: no se pudo inicializar la tarjeta");
    programLoaded = false;
    return;
  }

  if (loadBytecode("chamba")) {
    Serial.println("Bytecode cargado correctamente");
    programLoaded = true;
  } else {
    Serial.println("ERROR: no se pudo cargar programa.chamba.bc");
    programLoaded = false;
  }
}

void loop() {
  if (programLoaded && !executed) {
    run_vm();
    executed = true;
  }
}
