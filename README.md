ChambaScript

## INDICE 
- [VISION GENERAL](#vision-general)
- [FLUJO COMPLETO DE COMPILACION Y EJECUCION EN PC](#flujo-completo-de-compilacion-y-ejecucion-en-pc)
- [LEXER.L – ANALIZADOR LEXICO](#lexer.l-–-analizador-lexico)
- [PARSER.Y – ANALISIS SINTACTICO, TABLA DE SIMBOLOS Y GENERACION DE BYTECODE](#parser.y-–-analisis-sintactico,-tabla-de-simbolos-y-generacion-de-bytecode)
- [MAQUINA VIRTUAL EN PC – RUN_VM (vm.c)](#maquina-virtual-en-pc---run_vm-(vm.c))
- [SAVE_BYTECODE – FORMATO DEL ARCHIVO .CHAMBA.BC](#save_bytecode---formato-del-archivo-.chamba.bc)
- [MAIN.C – PUNTO DE ENTRADA EN PC](#main.c---punto-de-entrada-en-pc)
- [MAQUINA VIRTUAL EN ARDUINO – CHAMBA CAR (RUNTIME EN HARDWARE)](#maquina-virtual-en-arduino---chamba-car-(runtime-en-hardware))


## VISION GENERAL


El proyecto es un pequeño compilador–intérprete para el lenguaje ChambaScript, pensado para controlar un carrito seguidor de línea mediante un lenguaje de alto nivel y una máquina virtual.

Flujo lógico general:

Toma un archivo fuente .chamba.

El lexer (lexer.l) lo convierte en una secuencia de tokens.

El parser (parser.y) hace:

análisis sintáctico,

comprobaciones semánticas básicas (variables, constantes, arrays),

generación de bytecode para una máquina virtual de pila.

El bytecode se guarda en un archivo .chamba.bc.

Existen dos “runtimes” que consumen ese bytecode:

VM en PC (dentro del mismo proyecto): interpreta el bytecode y simula:

operaciones aritméticas y lógicas,

variables y arreglos,

estructuras de control (if, while),

llamadas a funciones builtin (incluyendo las del carrito).

VM en Arduino (sketch ChambaCar.ino): lee el mismo .chamba.bc desde una SD, ejecuta el bytecode y controla físicamente el carrito (motores y sensores).

El main.c orquesta el flujo en PC: abre el archivo fuente, llama a yyparse, genera el bytecode .chamba.bc, lo guarda en disco y, opcionalmente, ejecuta la VM de PC para depuración.

La VM de Arduino no compila nada: únicamente carga y ejecuta el bytecode que fue generado en la PC.

## FLUJO COMPLETO DE COMPILACION Y EJECUCION EN PC

2.1 Instalación de herramientas necesarias

macOS (con Homebrew):

Instalar Homebrew:

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh
)"

Instalar Bison y Flex:

brew install bison flex

Verificaciones:

brew info bison
brew info flex

Linux (Debian / Ubuntu y derivados):

sudo apt update
sudo apt install flex bison

Linux (Fedora / RHEL / CentOS):

sudo dnf install flex bison

Linux (Arch / Manjaro):

sudo pacman -S flex bison

2.2 Compilación del compilador/VM de ChambaScript

En la carpeta del proyecto (donde están parser.y, lexer.l, main.c, vm.c):

bison -d parser.y
flex lexer.l
gcc -Wall -Wextra -o chamba main.c parser.tab.c lex.yy.c vm.c -lm

Esto genera el ejecutable chamba.

2.3 Ejecución de un programa .chamba en PC

./chamba programa.chamba

Este comando:

Ejecuta análisis léxico, sintáctico y semántico.

Si todo va bien:

Imprime: Analisis sintactico y semantico completado exitosamente.

Genera programa.chamba.bc con el bytecode.

Llama a run_vm() (en vm.c) para ejecutar el bytecode en la VM de PC:

útil para depuración, ya que imprime las llamadas a funciones builtin (por ejemplo accelerate(200), turnLeft(90), etc.).

## LEXER.L – ANALIZADOR LEXICO

El lexer está escrito con Flex. Su objetivo es leer texto fuente .chamba y producir tokens que el parser (parser.y) entiende.

3.1 Definiciones de patrones

DIGIT: un dígito 0–9.

LETTER: una letra o underscore [A-Za-z_].

ID: un identificador, que empieza con LETTER y puede contener LETTER o DIGIT:

ID = LETTER (LETTER | DIGIT)*

3.2 Palabras reservadas

Tokens para palabras clave del lenguaje:

Control y declaración:

var, const, func, return, if, else, while, do, for, break, continue, pass.

Tipos:

int, tiny, long, float, char, bool, void.

Booleanos:

true, false.

3.3 Funciones builtin del lenguaje

Reconoce los nombres de funciones builtin y los mapea a tokens específicos:

Matemáticas genéricas:

abs, min, max, sqrt, pow.

Funciones relacionadas con el carrito:

checkLineLeft, checkLineRight,

accelerate, setForward, setBackward, brake,

turnLeft, turnRight, turnAngle.

Estas se convierten en tokens como ABS, MIN, ACCELERATE, CHECKLINELEFT, etc., que están declarados en parser.y.

3.4 Operadores y símbolos

Comparación:

==, !=, <=, >=, <, >.

Lógicos:

&&, ||, !.

Aritméticos:

+, -, *, /, %.

Asignación y puntuación:

=, (, ), {, }, [, ], ;, :, ,.

3.5 Literales numéricos

Flotantes:

Patrón: {DIGIT}+"."{DIGIT}+

Se convierten a double con atof(yytext) y se almacenan en yylval.float_val.

Token: FLOAT_LITERAL.

Enteros:

Patrón: {DIGIT}+

Se convierten a int con atoi(yytext) y se almacenan en yylval.int_val.

Token: INTEGER.

3.6 Identificadores

Patrón: {ID}

Se copian con strdup(yytext) a yylval.string_val.

Se devuelven como token IDENTIFIER.

3.7 Comentarios

Comentarios de línea:

Prefijo // seguido de cualquier cosa hasta el salto de línea.

Se ignoran completamente.

Comentarios multilínea:

Delimitadores /* y */.

Se consume carácter por carácter usando input() hasta ver la secuencia */.

También se ignoran.

3.8 Espacios en blanco

Patrón: [ \t\r\n]+

Se ignoran; el lexer simplemente salta estos caracteres.

3.9 Caracteres desconocidos

Cualquier otro carácter produce un mensaje:

Caracter no reconocido: X

El lexer no toma decisiones semánticas: sólo clasifica el texto en tokens y rellena yylval cuando es necesario (identificadores y literales).

## PARSER.Y – ANALISIS SINTACTICO, TABLA DE SIMBOLOS Y GENERACION DE BYTECODE

4.1 Zona de código C inicial

Incluye:

stdio.h, stdlib.h, string.h, stdarg.h, math.h.

Declara:

int yylex(void);

void yyerror(const char* s);

extern FILE* yyin;

Define la tabla de símbolos:

SymKind:

SYM_VAR, SYM_CONST, SYM_FUNC.

struct Sym:

name: nombre del símbolo (variable/constante/función).

kind: clase del símbolo (SYM_VAR, SYM_CONST, SYM_FUNC).

type: tipo declarado (por ahora token del tipo: TYPE_INT, TYPE_FLOAT, etc.).

scope_level: nivel de ámbito (en esta versión se trabaja principalmente con un único nivel global).

addr: dirección base en la memoria de la VM.

is_array: indica si es arreglo (1) o escalar (0).

length: número de elementos (1 para un escalar).

next: siguiente símbolo en la lista enlazada para el mismo bucket de hash.

Estructuras globales:

sym_table[SYM_TABLE_SIZE]: tabla de símbolos como hash.

current_scope: entero para el nivel de scope (por ahora = 0).

next_addr: siguiente índice libre en vm_memory.

vm_memory[MAX_VARS]: arreglo de double que modela la memoria de datos de la VM.

4.2 Funciones de la tabla de símbolos

sym_hash(nombre): hash tipo djb2 para mapear strings a índices de la tabla.

sym_lookup(nombre): busca un símbolo por nombre en toda la tabla (cualquier scope).

sym_lookup_local(nombre): busca un símbolo sólo en el scope actual.

sym_insert(nombre, kind, type, length):

Crea un Sym nuevo.

Ajusta length (si es <= 0, se pone 1).

Si length > 1, marca is_array = 1.

Si el símbolo es variable o constante:

Asigna un bloque de length posiciones consecutivas en vm_memory a partir de next_addr.

Inicializa esas celdas a 0.0.

Incrementa next_addr en length.

Inserta el símbolo en la lista enlazada asociada al índice de hash calculado.

semantic_error(fmt, ...):

Imprime Semantic error: ... y termina el programa.

Se usa para errores como:

variable redeclarada,

asignación a constante,

uso de identificador no declarado,

uso incorrecto de arrays, etc.

4.3 Definición de bytecode e instrucciones

Se define:

enum OpCode con las instrucciones soportadas por la VM (en PC y en Arduino):

OP_PUSH_NUM (empuja un número literal).

OP_LOAD (carga vm_memory[addr]).

OP_STORE (almacena en vm_memory[addr]).

OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD.

OP_LT, OP_LTE, OP_GT, OP_GTE, OP_EQ, OP_NEQ.

OP_AND, OP_OR, OP_NOT.

OP_JUMP, OP_JUMP_IF_FALSE.

OP_CALL_BUILTIN.

OP_POP.

OP_HALT.

OP_LOAD_IND (lectura indirecta para arreglos: vm_memory[addr] donde addr se calcula en tiempo de ejecución).

OP_STORE_IND (escritura indirecta para arreglos).

En la VM de Arduino también existen OP_CALL y OP_RET como base para funciones de usuario, aunque en el compilador actual todavía no se generan.

struct Instr:

op: opcode (OpCode).

a: entero auxiliar (por ejemplo, dirección de memoria, índice de salto, id de builtin).

d: double auxiliar (por ejemplo, literales numéricos o número de argumentos en builtins).

code[MAX_CODE]: arreglo global de instrucciones.

code_size: número de instrucciones generadas.

Función emit(op, a, d):

Inserta una instrucción en code[code_size].

Incrementa code_size.

Retorna el índice usado (sirve para backpatching, por ejemplo en saltos).

Función to_bool(v):

Convierte un double a 0.0 o 1.0 según si es falso/verdadero.

4.4 Builtins y %union

Se define un enum BuiltinId:

BI_ABS, BI_MIN, BI_MAX, BI_SQRT, BI_POW,

BI_CHECKLINELEFT, BI_CHECKLINERIGHT,

BI_ACCELERATE, BI_SETFORWARD, BI_SETBACKWARD,

BI_BRAKE, BI_TURNLEFT, BI_TURNRIGHT, BI_TURNANGLE.

En el %union:

int_val (enteros generales: tipos, tamaños, índices).

float_val (para literales flotantes).

string_val (nombres de identificadores).

Tokens:

IDENTIFIER usa string_val.

INTEGER usa int_val.

FLOAT_LITERAL usa float_val.

No terminales con int_val:

var_type, builtin_func, arguments, arguments_tail, array_part, while_start, while_jmp.

4.5 Regla program

program:
statement_list
{
emit(OP_HALT, 0, 0.0);
}
;

Traducción:

El programa es una lista de sentencias.

Al final se añade un OP_HALT para indicar a la VM que termine la ejecución.

4.6 Lista de sentencias

statement_list:

Permite:

statement ; statement_list,

statement ;,

o lista vacía (ε).

Genera bytecode de cada statement en el orden del código fuente.

statement puede ser:

declaration

assignment

control_structure

function_call (en este caso se emite un OP_POP para descartar el valor de retorno).

function_declaration (actualmente no genera código ejecutable, sólo error semántico).

return_statement

pass_statement

break_statement

continue_statement

La combinación de statement_list + statement define el orden lineal de instrucciones en code[], que es exactamente el orden en que la VM las ejecuta.

4.7 Declaración de variables y constantes

Regla declaration:

var IDENTIFIER : var_type array_part

Comprueba que el identificador no exista en el scope actual (sym_lookup_local).

Inserta símbolo como SYM_VAR con el tipo y tamaño (length) dados (sym_insert).

No genera bytecode adicional: la reserva e inicialización de memoria se hace en la inserción.

var IDENTIFIER : var_type array_part = expression

Igual que arriba, pero además:

Exige que array_part sea tamaño 1 (no se inicializan arrays enteros con un solo escalar).

Después de la expresión, que deja un valor en la pila, emite OP_STORE en s->addr.

const IDENTIFIER : var_type array_part = expression

Similar a var, pero kind = SYM_CONST.

No se permite array_part > 1 para constantes (no arrays constantes).

Al final, OP_STORE inicializa la constante.

array_part:

[ INTEGER ] → length = INTEGER.

vacío → length = 1.

4.8 Asignaciones

Regla assignment:

Asignación a escalar:

IDENTIFIER = expression

Busca el símbolo (sym_lookup).

Comprueba:

que exista,

que no sea constante (SYM_CONST),

que no sea array (is_array == 0).

La expresión deja el valor en la pila.

Se emite OP_STORE s->addr, que guarda en vm_memory[addr] y saca el valor de la pila.

Asignación a arreglo:

IDENTIFIER [ expression ] = expression

Se divide en dos partes en el parser:

IDENTIFIER LBRACKET expression RBRACKET:

Busca el símbolo.

Comprueba:

que exista,

que sea array,

que no sea constante.

Genera bytecode:

expression deja el índice en la pila.

OP_PUSH_NUM con la dirección base del array (s->addr).

OP_ADD suma índice + base → produce en la pila la dirección efectiva.

ASSIGN expression:

La nueva expression deja el valor en la pila.

OP_STORE_IND toma addr y value de la pila y hace vm_memory[addr] = value.

Esto permite trabajar con arrays variables de forma indirecta.

4.9 Tipos

var_type:

Mapea TYPE_INT, TYPE_TINY, TYPE_LONG, TYPE_FLOAT, TYPE_CHAR, TYPE_BOOL, TYPE_VOID a un entero que se guarda en int_val.

En la VM actual se representa todo numéricamente como double, pero el tipo lógico se usa para validaciones semánticas y para futuras extensiones.

4.10 Expresiones y precedencia

Se siguen niveles clásicos de precedencia:

logical_or (||)

logical_and (&&)

equality (==, !=)

relational (<, <=, >, >=)

additive (+, -)

term (*, /, %)

factor

Cada nivel usa el estilo “expresión izquierda + operador + expresión derecha” y genera bytecode de pila:

Evaluar operandos en orden.

Aplicar la operación con el opcode correspondiente (OP_ADD, OP_SUB, OP_LT, etc.).

Por ejemplo, para additive:

additive PLUS term:

Genera bytecode para additive (valor A en pila).

Luego para term (valor B en pila).

Emite OP_ADD, que consume A y B y empuja A+B.

NOT factor genera primero el factor y luego OP_NOT para invertir el booleano.

4.11 Factor

factor cubre:

number:

integer o float, que generan OP_PUSH_NUM con el valor correspondiente (positivo o negativo).

TRUE / FALSE:

OP_PUSH_NUM 1.0 / OP_PUSH_NUM 0.0.

IDENTIFIER:

Busca el símbolo.

Comprueba que exista, que tenga dirección válida (addr >= 0) y que no sea array.

Emite OP_LOAD s->addr.

IDENTIFIER [ expression ]:

Similar a la asignación de arrays:

expression deja índice en la pila.

OP_PUSH_NUM con dirección base.

OP_ADD suma base + índice.

OP_LOAD_IND lee vm_memory[addr] y empuja el valor.

function_call:

Se maneja en una regla aparte, pero factor permite que una llamada devuelva un valor (por ejemplo, x = min(a, b);).

LPAREN expression RPAREN:

Agrupa subexpresiones.

4.12 Estructuras de control

control_structure:

if_statement

while_loop

do_while_loop (no soportado en VM por ahora, lanza semantic_error)

for_loop (no soportado en VM por ahora, lanza semantic_error)

If-else:

Traducción estándar:

Evalúa la condición → valor en la pila.

Emite OP_JUMP_IF_FALSE con destino pendiente (jmp_false).

Genera el bloque then.

Emite OP_JUMP para saltarse el else (jmp_end).

Backpatch: se rellena code[jmp_false].a con el inicio del bloque else.

Genera el bloque else.

Backpatch: se rellena code[jmp_end].a con la posición al final del if-else.

While:

Se usa:

while_start para capturar el code_size (inicio del while).

while_jmp para emitir un OP_JUMP_IF_FALSE cuya dirección se rellena al final.

Traducción:

Marca start = code_size antes de la condición.

Compila la condición (boolean_expression).

Emite OP_JUMP_IF_FALSE jmp_false.

Genera el cuerpo del bucle.

Emite OP_JUMP start para volver a la condición.

Rellena code[jmp_false].a con la posición al final del bucle.

Do-while y for:

Por ahora no generan bytecode: se detectan y se lanza semantic_error("... not supported in VM yet").

4.13 Declaraciones y llamadas a funciones

function_declaration:

Reconoce func nombre (params) : tipo { statement_list }.

En esta versión del compilador, las funciones de usuario todavía no están soportadas a nivel de VM:

Se lanza semantic_error("user-defined functions not supported in VM yet").

return_statement:

RETURN expression genera el bytecode de la expresión.

A nivel de VM, todavía no hay un flujo completo de CALL/RET para funciones de usuario en el compilador de PC.

4.14 Llamadas a funciones builtin y argumentos

builtin_func:

Asocia tokens ABS, MIN, MAX, SQRT, POW, CHECKLINELEFT, ACCELERATE, etc. a un BuiltinId entero.

function_call:

builtin_func LPAREN arguments RPAREN:

arguments genera el bytecode de cada argumento, dejando sus valores en la pila.

Devuelve el número de argumentos.

function_call emite:

OP_CALL_BUILTIN con:

a = id de la builtin (BuiltinId).

d = número de argumentos.

IDENTIFIER LPAREN arguments RPAREN:

Destinado a funciones de usuario.

Actualmente, se verifica que exista como SYM_FUNC y luego se lanza semantic_error porque aún no están soportadas en la VM.

arguments / arguments_tail:

Cuentan el número de argumentos:

expression arguments_tail → 1 + arguments_tail.

vacío → 0.

El número de argumentos se pasa en in.d a OP_CALL_BUILTIN, y luego la VM sabe cuántos valores debe sacar de la pila.

## MAQUINA VIRTUAL EN PC – RUN_VM (vm.c)

run_vm es un intérprete de bytecode basado en pila que se ejecuta en la PC para depuración.

Estructuras:

double stack[1024]: pila de ejecución.

int sp: índice del tope de la pila (inicia en -1).

int pc: program counter, índice de la instrucción actual en code[].

Bucle principal:

for (;;) {
Instr in = code[pc];
switch (in.op) {
...
}
}

Para cada opcode:

OP_PUSH_NUM:

stack[++sp] = in.d; pc++;

OP_LOAD:

stack[++sp] = vm_memory[in.a]; pc++;

OP_STORE:

vm_memory[in.a] = stack[sp--]; pc++;

OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD:

Sacan dos valores, aplican la operación y empujan el resultado.

Comparaciones (OP_LT, OP_LTE, OP_GT, OP_GTE, OP_EQ, OP_NEQ):

Sacan dos valores, comparan y empujan 0.0 o 1.0.

Lógicos (OP_AND, OP_OR, OP_NOT):

Trabajan con 0.0/1.0 usando to_bool para normalizar.

OP_JUMP:

pc = in.a;

OP_JUMP_IF_FALSE:

Saca cond de la pila.

Si to_bool(cond) == 0.0, pc = in.a; sino pc++;.

OP_CALL_BUILTIN:

Lee id = in.a y argc = (int)in.d.

Saca argc argumentos de la pila en orden inverso y los almacena en args[].

Según el id, ejecuta la builtin:

ABS, MIN, MAX, SQRT, POW usan funciones de math.h.

CHECKLINELEFT / CHECKLINERIGHT:

En la VM de PC imprimen algo como checkLineLeft() -> 1 y devuelven 1.0 (simulan ver línea).

ACCELERATE, SETFORWARD, SETBACKWARD, BRAKE, TURNLEFT, TURNRIGHT, TURNANGLE:

Imprimen el nombre y parámetros, simulando los comandos al carrito:

por ejemplo turnLeft(90.000000).

Devuelven 0.0.

Empuja res a la pila.

pc++;.

OP_POP:

Descarta el valor del tope de la pila.

OP_LOAD_IND:

Saca una dirección de la pila.

Comprueba rango.

Empuja vm_memory[addr].

OP_STORE_IND:

Saca value y addr de la pila.

Comprueba rango.

Guarda en vm_memory[addr].

OP_HALT:

Imprime Ejecucion terminada.

Sale de run_vm.

Esta VM de PC se usa sobre todo para:

Validar que el compilador genera bytecode correcto.

Ver la secuencia de llamadas a funciones builtin del carrito antes de portar a Arduino.

## SAVE_BYTECODE – FORMATO DEL ARCHIVO .CHAMBA.BC

save_bytecode(const char* filename):

Abre el archivo de salida.

Escribe en la primera línea el número de instrucciones:

code_size

Para cada instrucción i:

Escribe una línea con:

op a d

donde:

op: entero que representa el OpCode.

a: entero auxiliar (por ejemplo, dirección, id de builtin, destino de salto).

d: número de punto flotante (double), típicamente usado para literales o número de argumentos.

Ejemplo (simplificado):

28
0 0 5
2 0 0
0 0 3.14
...

La VM en PC y la VM en Arduino leen este mismo formato de archivo.

## MAIN.C – PUNTO DE ENTRADA EN PC

main(int argc, char** argv):

Comprueba que se haya pasado un archivo .chamba:

Si no, muestra un mensaje de uso y termina.

Abre el archivo fuente y lo asigna a yyin para que el lexer/parser lo procesen.

Llama yyparse():

Si yyparse() devuelve 0 (éxito):

Muestra: Analisis sintactico y semantico completado exitosamente.

Cierra yyin.

Construye el nombre del archivo de bytecode:

Reemplaza la extensión por .chamba.bc si el nombre tenía punto.

O agrega .bc si no tenía.

Llama save_bytecode(bc_name) para guardar el bytecode.

Llama run_vm() para ejecutar el bytecode en la VM de PC.

Si yyparse() falla:

Cierra yyin.

Imprime Error en el analisis.

Devuelve 1.

Este flujo es el “toolchain” de PC: compilar, generar bytecode y simular la ejecución.

## MAQUINA VIRTUAL EN ARDUINO – CHAMBA CAR (RUNTIME EN HARDWARE)

Además de la VM en PC, el proyecto incluye una VM adaptada a Arduino (ChambaCar.ino) con estas características:

Usa la misma definición de OpCode, Instr y formato de bytecode que la VM de PC.

Lee el archivo programa.chamba.bc desde una tarjeta SD.

La primera línea indica code_size.

Cada línea posterior se parsea como op a d y se guarda en code[i].

Implementa run_vm() en Arduino:

Mismo esquema de pila y pc.

Interpretación de instrucciones:

aritméticas, lógicas, saltos, load/store, arrays, etc.

Implementa los builtins del carrito con lógica real sobre los pines:

Pines de motores (L298N): IN1, IN2, IN3, IN4, ENA, ENB.

Pines de sensores de línea: sensorIzqPin, sensorDerPin.

En OP_CALL_BUILTIN, según el BuiltinId:

CHECKLINELEFT / CHECKLINERIGHT:

Llaman a hayLinea(sensorIzqPin) / hayLinea(sensorDerPin).

Devuelven 1.0 si el sensor ve línea, 0.0 si no.

ACCELERATE(speed):

Ajusta el PWM de ambas ruedas con setPWM(speed, speed).

SETFORWARD(speed):

Fija el sentido hacia delante (adelante()) y aplica setPWM(speed, speed).

SETBACKWARD(speed):

Fija el sentido hacia atrás (atras()) y aplica setPWM(speed, speed).

BRAKE():

Llama a setPWM(0, 0) para detener el carrito.

TURNLEFT(value):

Mueve solo la rueda derecha (setPWM(0, value)) para pivotear a la izquierda.

TURNRIGHT(value):

Mueve solo la rueda izquierda (setPWM(value, 0)) para pivotear a la derecha.

TURNANGLE(degrees):

Interpreta el signo o valor del ángulo para decidir dirección y velocidad de giro (la lógica interna se puede refinar).

De esta forma:

El compilador y la generación de bytecode ocurren en la PC.

El archivo .chamba.bc se copia a la SD.

El Arduino copia ese bytecode a memoria y lo ejecuta con run_vm().

Las mismas instrucciones que en PC solo imprimen logs, en Arduino se traducen en movimiento físico del carrito seguidor de línea.

Con esto, ChambaScript se convierte en el “lenguaje” de alto nivel para programar el comportamiento del robot, y la máquina virtual es el puente que traduce el bytecode a acciones reales sobre el hardware.
