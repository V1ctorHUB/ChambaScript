#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <math.h>

// ===== DEBUG =====
#define DEBUG 1

// Motores
const int IN1 = 7;
const int IN2 = 4;
const int ENA = 6;

const int IN3 = 3;
const int IN4 = 2;
const int ENB = 5;

// Sensores de línea
const int sensorIzqPin = 9;
const int sensorDerPin = 8;

const bool LINEA_ALTA = true;

// SD en Arduino MEGA (SPI: MISO=50, MOSI=51, SCK=52)
const int SD_CS_PIN = 53;

#define MAX_CODE 128
#define MAX_VARS 64

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

// archivo global SD
File myFile;

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
  if (DEBUG) {
    Serial.println("== INICIANDO VM ==");
  }

  float stack[32];
  int sp = -1;
  int pc = entry_pc;
  int call_stack[16];
  int call_sp = 0;

  for (;;) {
    Instr in = code[pc];

    if (DEBUG) {
      Serial.print("PC=");
      Serial.print(pc);
      Serial.print(" OP=");
      Serial.print((int)in.op);
      Serial.print(" A=");
      Serial.print(in.a);
      Serial.print(" D=");
      Serial.println(in.d);
    }

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
        if (argc >= 2) res = (args[0] < args[1]) ? args[0] : args[1];
        break;
      case BI_MAX:
        if (argc >= 2) res = (args[0] > args[1]) ? args[0] : args[1];
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
      if (sp < 0) return;
      sp--;
      pc++;
      break;

    case OP_LOAD_IND: {
      float addr = stack[sp--];
      int i = (int)addr;
      if (i < 0 || i >= MAX_VARS) return;
      stack[++sp] = vm_memory[i];
      pc++;
      break;
    }

    case OP_STORE_IND: {
      float value = stack[sp--];
      float addr = stack[sp--];
      int i = (int)addr;
      if (i < 0 || i >= MAX_VARS) return;
      vm_memory[i] = value;
      pc++;
      break;
    }

    case OP_CALL:
      if (call_sp >= 16) return;
      call_stack[call_sp++] = pc + 1;
      pc = in.a;
      break;

    case OP_RET:
      if (call_sp <= 0) return;
      pc = call_stack[--call_sp];
      break;

    case OP_HALT:
      if (DEBUG) {
        Serial.println("OP_HALT alcanzado, fin de VM.");
      }
      return;

    default:
      if (DEBUG) {
        Serial.print("OPCODE desconocido: ");
        Serial.println((int)in.op);
      }
      return;
    }
  }
}

// === PARSER: lee archivo TEST sin sscanf ===
bool loadBytecode(const char* filename) {
  if (DEBUG) {
    Serial.print("Abriendo archivo: ");
    Serial.println(filename);
  }

  myFile = SD.open(filename, FILE_READ);
  if (!myFile) {
    if (DEBUG) {
      Serial.println("ERROR: no se pudo abrir el archivo");
    }
    return false;
  }

  String line = myFile.readStringUntil('\n');
  line.trim();
  int n = line.toInt();

  if (DEBUG) {
    Serial.print("Line count read: ");
    Serial.println(n);
  }

  if (n <= 0 || n > MAX_CODE) {
    if (DEBUG) {
      Serial.println("ERROR: tamaño de código inválido");
    }
    myFile.close();
    return false;
  }

  code_size = n;

  for (int i = 0; i < n; i++) {
    if (!myFile.available()) {
      if (DEBUG) {
        Serial.print("ERROR: fin de archivo antes de tiempo en i=");
        Serial.println(i);
      }
      myFile.close();
      return false;
    }

    String l = myFile.readStringUntil('\n');
    l.trim();
    if (l.length() == 0) {
      if (DEBUG) {
        Serial.print("ERROR: línea vacía en i=");
        Serial.println(i);
      }
      myFile.close();
      return false;
    }

    int firstSpace  = l.indexOf(' ');
    int secondSpace = l.indexOf(' ', firstSpace + 1);

    if (firstSpace <= 0 || secondSpace <= firstSpace) {
      if (DEBUG) {
        Serial.print("ERROR formato en línea ");
        Serial.print(i);
        Serial.print(" => '");
        Serial.print(l);
        Serial.println("'");
      }
      myFile.close();
      return false;
    }

    String sOp = l.substring(0, firstSpace);
    String sA  = l.substring(firstSpace + 1, secondSpace);
    String sD  = l.substring(secondSpace + 1);

    int   opInt = sOp.toInt();
    int   aInt  = sA.toInt();
    float dVal  = sD.toFloat();

    code[i].op = (OpCode)opInt;
    code[i].a  = aInt;
    code[i].d  = dVal;

    if (DEBUG) {
      Serial.print("Instr[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(opInt);
      Serial.print(" ");
      Serial.print(aInt);
      Serial.print(" ");
      Serial.println(dVal);
    }
  }

  myFile.close();
  entry_pc = 0;

  if (DEBUG) {
    Serial.println("Bytecode cargado OK.");
  }

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
  delay(500);

  if (DEBUG) {
    Serial.println("Inicializando SD...");
  }

  if (!SD.begin(SD_CS_PIN)) {
    if (DEBUG) {
      Serial.println("SD ERROR: no se pudo inicializar la tarjeta");
    }
    programLoaded = false;
    return;
  }

  if (DEBUG) {
    Serial.println("SD inicializada correctamente.");
    Serial.println("Intentando cargar TEST...");
  }

  if (loadBytecode("TEST")) {
    if (DEBUG) {
      Serial.println("Programa cargado correctamente.");
    }
    programLoaded = true;
  } else {
    if (DEBUG) {
      Serial.println("ERROR: no se pudo cargar TEST");
    }
    programLoaded = false;
  }
}

void loop() {
  if (programLoaded && !executed) {
    run_vm();
    executed = true;
  }
}
