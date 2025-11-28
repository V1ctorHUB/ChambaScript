#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdint.h>

// -------------------- MOTOR PINS --------------------
const int IN1 = 7;
const int IN2 = 4;
const int ENA = 6;
const int IN3 = 3;
const int IN4 = 2;
const int ENB = 5;

// -------------------- SENSORS --------------------
const int sensorIzqPin = 9;
const int sensorDerPin = 8;

const bool LINEA_ALTA = true;

// -------------------- SD CARD --------------------
const int SD_CS_PIN = 53;     // Arduino Mega hardware SS
File myFile;

// -------------------- VM CONSTANTS --------------------
#define MAX_CODE 84
#define MAX_VARS 48

typedef enum {
  OP_PUSH_NUM, OP_LOAD, OP_STORE, OP_ADD, OP_SUB,
  OP_MUL, OP_DIV, OP_MOD, OP_LT, OP_LTE, OP_GT,
  OP_GTE, OP_EQ, OP_NEQ, OP_AND, OP_OR, OP_NOT,
  OP_JUMP, OP_JUMP_IF_FALSE, OP_CALL_BUILTIN,
  OP_POP, OP_HALT, OP_LOAD_IND, OP_STORE_IND,
  OP_CALL, OP_RET
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
  BI_TURNANGLE,
  BI_ACCELERATELEFT,
  BI_ACCELERATERIGHT,
  BI_ROTW,           // nuevo
  BI_ROTC,           // nuevo
  BI_DELAYSECONDS    // nuevo
} BuiltinId;

typedef struct {
  uint8_t op;
  int8_t  a;
  float   d;
} Instr;

Instr code[MAX_CODE];
int   code_size = 0;
float vm_memory[MAX_VARS];
int   entry_pc = 0;
bool  programLoaded = false;
bool  executed      = false;

// PWM actual por lado (para accelerateLeft / accelerateRight)
int currentPwmLeft  = 0;
int currentPwmRight = 0;

// -------------------- UTILS --------------------
static float to_bool(float v) { return (v != 0.0f) ? 1.0f : 0.0f; }

int clampPWM(int v) {
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  return v;
}

bool hayLinea(int pin) {
  int v = digitalRead(pin);
  return LINEA_ALTA ? (v == HIGH) : (v == LOW);
}

// -------------------- MOTOR CONTROL --------------------
void adelante() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}
void atras() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void setPWM(int pwmIzq, int pwmDer) {
  currentPwmLeft  = clampPWM(pwmIzq);
  currentPwmRight = clampPWM(pwmDer);
  analogWrite(ENA, currentPwmLeft*0.85f);
  analogWrite(ENB, currentPwmRight);
}

// -------------------- MATH HELPERS --------------------
static float cs_abs(float x){ return x < 0 ? -x : x; }

static float cs_sqrt(float x){
  if (x <= 0.0f) return 0.0f;
  float g = x;
  for (int i=0;i<5;i++) g = 0.5f*(g + x/g);
  return g;
}

static float cs_pow(float b, float e){
  int n = (int)e;
  float r = 1.0f;
  bool pos = (n >= 0);
  if (!pos) n = -n;

  while (n){
    if (n & 1) r *= b;
    b *= b;
    n >>= 1;
  }
  return pos ? r : (r != 0 ? 1.0f/r : 0.0f);
}

// --------------------------------------------------------------
// LOW-LEVEL ASCII BYTECODE LINE PARSER (3 numbers per line)
// --------------------------------------------------------------
void processBytecodeLine(String line, int index) {
    String nums[3];
    int count = 0;

    String tok = "";
    bool inNumber = false;

    for (int i = 0; i < line.length(); i++) {
        char c = line[i];

        if ((c >= '0' && c <= '9') || c == '-') {
            tok += c;
            inNumber = true;
        } else {
            if (inNumber) {
                if (count < 3) nums[count++] = tok;
                tok = "";
                inNumber = false;
            }
        }
    }

    if (inNumber && count < 3) nums[count++] = tok;

    if (count != 3) return;

    code[index].op = (uint8_t) nums[0].toInt();
    code[index].a  = (int8_t)  nums[1].toInt();
    code[index].d  = nums[2].toFloat();

    Serial.print("Loaded instruction ");
    Serial.print(index);
    Serial.print(": ");
    Serial.print(code[index].op);
    Serial.print(" ");
    Serial.print(code[index].a);
    Serial.print(" ");
    Serial.println(code[index].d);
}

// --------------------------------------------------------------
// LOW-LEVEL ASCII LOADBYTECODE()
// --------------------------------------------------------------
bool loadBytecode(const char *filename) {

    File f = SD.open(filename, FILE_READ);
    if (!f) return false;

    Serial.println("Reading bytecode (ASCII low-level parser)...");

    String line = "";

    // -------- FIRST LINE: NUMBER OF INSTRUCTIONS ----------
    int n = -1;
    while (f.available()) {
        char c = f.read();
        if (c == '\r') continue;
        if (c == '\n') {
            n = line.toInt();
            line = "";
            break;
        }
        line += c;
    }

    if (n <= 0 || n > MAX_CODE) {
        f.close();
        return false;
    }

    code_size = n;

    // -------- NEXT N LINES: PARSE INSTRUCTIONS ----------
    int index = 0;

    while (index < n && f.available()) {
        char c = f.read();

        if (c == '\r') continue;

        if (c == '\n') {
            if (line.length() > 0) {
                processBytecodeLine(line, index);
                index++;
            }
            line = "";
        } else {
            line += c;
        }
    }

    if (index < n && line.length() > 0) {
        processBytecodeLine(line, index);
    }

    f.close();
    entry_pc = 0;

    return true;
}

// --------------------------------------------------------------
// VM INTERPRETER
// --------------------------------------------------------------
void run_vm() {
  float stack[32];
  int sp = -1;
  int pc = entry_pc;
  int call_stack[16];
  int call_sp = 0;

  for (;;) {
    Instr in = code[pc];

    switch ((OpCode)in.op) {
      case OP_PUSH_NUM: stack[++sp] = in.d; pc++; break;
      case OP_LOAD:     stack[++sp] = vm_memory[in.a]; pc++; break;
      case OP_STORE:    vm_memory[in.a] = stack[sp--]; pc++; break;
      case OP_ADD:      stack[sp-1] += stack[sp]; sp--; pc++; break;
      case OP_SUB:      stack[sp-1] -= stack[sp]; sp--; pc++; break;
      case OP_MUL:      stack[sp-1] *= stack[sp]; sp--; pc++; break;

      case OP_DIV: {
        float b = stack[sp--];
        float a = stack[sp--];
        stack[++sp] = (b == 0 ? 0 : a/b);
        pc++; break;
      }

      case OP_MOD: {
        int b = (int)stack[sp--];
        int a = (int)stack[sp--];
        stack[++sp] = (b == 0 ? 0 : a % b);
        pc++; break;
      }

      case OP_LT:  stack[sp-1] = to_bool(stack[sp-1] < stack[sp]); sp--; pc++; break;
      case OP_LTE: stack[sp-1] = to_bool(stack[sp-1] <= stack[sp]); sp--; pc++; break;
      case OP_GT:  stack[sp-1] = to_bool(stack[sp-1] > stack[sp]); sp--; pc++; break;
      case OP_GTE: stack[sp-1] = to_bool(stack[sp-1] >= stack[sp]); sp--; pc++; break;
      case OP_EQ:  stack[sp-1] = to_bool(stack[sp-1] == stack[sp]); sp--; pc++; break;
      case OP_NEQ: stack[sp-1] = to_bool(stack[sp-1] != stack[sp]); sp--; pc++; break;

      case OP_AND:
        stack[sp-1] = to_bool(to_bool(stack[sp-1]) && to_bool(stack[sp]));
        sp--; pc++; break;

      case OP_OR:
        stack[sp-1] = to_bool(to_bool(stack[sp-1]) || to_bool(stack[sp]));
        sp--; pc++; break;

      case OP_NOT:
        stack[sp] = to_bool(stack[sp]) ? 0 : 1;
        pc++; break;

      case OP_JUMP: pc = in.a; break;

      case OP_JUMP_IF_FALSE:
        if (!to_bool(stack[sp--])) pc = in.a;
        else pc++;
        break;

      case OP_CALL_BUILTIN: {
        int id = in.a;
        int argc = (int)in.d;
        float args[4];
        for (int i = argc-1; i >= 0; i--) args[i] = stack[sp--];
        float res = 0;

        switch ((BuiltinId)id) {
          case BI_CHECKLINELEFT:  res = hayLinea(sensorIzqPin); break;
          case BI_CHECKLINERIGHT: res = hayLinea(sensorDerPin); break;
          case BI_ACCELERATE:     setPWM(args[0], args[0]); break;
          case BI_SETFORWARD:     adelante(); setPWM(args[0], args[0]); break;
          case BI_SETBACKWARD:    atras();    setPWM(args[0], args[0]); break;
          case BI_BRAKE:          setPWM(0,0); break;
          case BI_TURNLEFT:       setPWM(0, args[0]); break;
          case BI_TURNRIGHT:      setPWM(args[0], 0); break;
          case BI_TURNANGLE:
            if (args[0] > 0) setPWM(0,180);
            else setPWM(180,0);
            break;

          case BI_ACCELERATELEFT:
            if (argc >= 1) {
              int s = (int)args[0];
              setPWM(s, currentPwmRight);
            }
            break;

          case BI_ACCELERATERIGHT:
            if (argc >= 1) {
              int s = (int)args[0];
              setPWM(currentPwmLeft, s);
            }
            break;

          case BI_ROTW:
            // de momento no hace nada
            break;

          case BI_ROTC:
            // de momento no hace nada
            break;

          case BI_DELAYSECONDS:
            if (argc >= 1) {
              int secs = (int)args[0];
              if (secs < 0) secs = 0;
              delay((unsigned long)secs * 1000UL);
            }
            break;

          case BI_ABS:  res = cs_abs(args[0]); break;
          case BI_MIN:  res = (args[0] < args[1] ? args[0] : args[1]); break;
          case BI_MAX:  res = (args[0] > args[1] ? args[0] : args[1]); break;
          case BI_SQRT: res = cs_sqrt(args[0]); break;
          case BI_POW:  res = cs_pow(args[0], args[1]); break;
          default: break;
        }

        stack[++sp] = res;
        pc++;
        break;
      }

      case OP_POP: sp--; pc++; break;

      case OP_LOAD_IND: {
        int addr = (int)stack[sp--];
        stack[++sp] = (addr>=0 && addr<MAX_VARS) ? vm_memory[addr] : 0;
        pc++; break;
      }

      case OP_STORE_IND: {
        float val = stack[sp--];
        int addr  = (int)stack[sp--];
        if (addr>=0 && addr<MAX_VARS) vm_memory[addr] = val;
        pc++; break;
      }

      case OP_CALL:
        call_stack[call_sp++] = pc+1;
        pc = in.a;
        break;

      case OP_RET:
        pc = call_stack[--call_sp];
        break;

      case OP_HALT:
        return;

      default:
        return;
    }
  }
}

// --------------------------------------------------------------
// SETUP
// --------------------------------------------------------------
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

  Serial.println("Initializing SD...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("ERROR: SD init failed");
    return;
  }

  Serial.println("SD OK. Loading bytecode...");

  if (loadBytecode("programa.chamba.bc")) {
    Serial.println("Bytecode loaded!");
    programLoaded = true;
  } else {
    Serial.println("Bytecode load ERROR.");
  }
}

// --------------------------------------------------------------
// LOOP
// --------------------------------------------------------------
void loop() {
  if (programLoaded && !executed) {
    run_vm();
    executed = true;
  }
}
