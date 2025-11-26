# ChambaScript

=========================================================

VISION GENERAL
=========================================================

El proyecto es un pequeño compilador–intérprete para el lenguaje ChambaScript:

Toma un archivo fuente .chamba.

El lexer (lexer.l) lo convierte en una secuencia de tokens.

El parser (parser.y) hace:

análisis sintáctico,

comprobaciones semánticas básicas (variables, arrays, etc.),

generación de bytecode para una máquina virtual de pila.

El bytecode se guarda en un archivo .chamba.bc

La máquina virtual (run_vm) ejecuta ese bytecode y simula:
operaciones aritméticas y lógicas,
variables y arreglos,
estructuras de control (if, while),
funciones builtin del carrito (accelerate, turnLeft, etc.).

El main.c solo orquesta el flujo: abrir archivo, llamar a yyparse, guardar bytecode y ejecutar la VM.

=========================================================
2. FLUJO COMPLETO DE COMPILACION Y EJECUCION

instalacion de los recursos necesarios: 

-macOS:
Instalar Homebrew: 
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Instalar Bison y Flex:
brew install bison flex

verificaciones:
brew info bison
brew info flex

-Linux (Debian / Ubuntu y derivados):
sudo apt update
sudo apt install flex bison

-Linux (Fedora / RHEL / CentOS):
sudo dnf install flex bison

-Linux (Arch / Manjaro):
sudo pacman -S flex bison

Compilación de las herramientas:
bison -d parser.y
flex lexer.l
gcc -Wall -Wextra -o chamba main.c parser.tab.c lex.yy.c -lm

Ejecución de un programa:
./chamba <programa>.chamba

Si todo va bien:

Imprime “Analisis sintactico y semantico completado exitosamente”.
Genera programa.chamba.bc con el bytecode.
Llama a run_vm() para ejecutar el bytecode y simular el programa.

=========================================================
3. LEXER.L – ANALIZADOR LEXICO

El lexer está escrito con Flex. 
Su objetivo es leer texto y producir tokens para Bison.

Definiciones de patrones:
-DIGIT: un dígito [0-9].
-LETTER: una letra o underscore [A-Za-z_].
-ID: un identificador, que empieza con LETTER y puede tener LETTER o DIGIT.

Palabras reservadas:
-var, const, func, return, if, else, while, do, for, break, continue, pass
-tipos: int, tiny, long, float, char, bool, void
-booleanos: true, false

funciones builtin del lenguaje:
abs, min, max, sqrt, pow,
checkLineLeft, checkLineRight,
accelerate, setForward, setBackward, brake,
turnLeft, turnRight, turnAngle

Cada una se mapea a un token específico (VAR, CONST, IF, WHILE, ABS, MIN, ACCELERATE, etc.) que está declarado en parser.y

Operadores y símbolos:
-Comparación: ==, !=, <=, >=, <, >
-Lógicos: &&, ||, !
-Aritméticos: +, -, *, /, %
-Asignación y puntuación: =, (, ), {, }, [, ], ;, :, ,

Literales numéricos:
-Flotantes: {DIGIT}+"."{DIGIT}+
Se convierten a double con atof y se guardan en yylval.float_val. Token: FLOAT_LITERAL.

-Enteros: {DIGIT}+
Se convierten a int con atoi y se guardan en yylval.int_val. Token: INTEGER.

Identificadores:
{ID}
Se copian con strdup(yytext) a yylval.string_val y se devuelven como token IDENTIFIER.

Comentarios:
-Comentarios de línea: "//" seguido de cualquier cosa hasta el salto de línea. Se ignoran.
-Comentarios multilínea: "/* ... /"
Se consume carácter por carácter usando input() hasta detectar la secuencia "/". También se ignoran.

Espacios en blanco:
[ \t\r\n]+
Se ignoran.

Cualquier otro carácter:
Se imprime un mensaje de error “Caracter no reconocido: X”.

El lexer no conoce nada de la semántica; solo clasifica texto en tokens y rellena yylval cuando hace falta (identificadores y literales).

=========================================================
4. PARSER.Y – ANALISIS SINTACTICO, TABLA DE SIMBOLOS Y GENERACION DE BYTECODE
4.1. Zona de código C inicial

Incluye:
stdio.h, stdlib.h, string.h, stdarg.h, math.h

Declaraciones de yylex, yyerror, yyin.
Definición de tipos para la tabla de símbolos:
SymKind: SYM_VAR, SYM_CONST, SYM_FUNC.

struct Sym:
-name: nombre de la variable/constante/función.
-kind: tipo de símbolo (variable, constante, función).
-type: tipo declarado (TOKEN del tipo: TYPE_INT, TYPE_FLOAT, etc.).
-scope_level: nivel de ámbito (actualmente se usa un solo nivel).
-addr: dirección base en la memoria de la VM.
-is_array: 1 si es arreglo, 0 si es escalar.
-length: número de elementos (1 para escalar).
-next: siguiente símbolo en la cadena para el mismo hash.

Tabla de símbolos:
-sym_table: arreglo de punteros a Sym con tamaño fijo SYM_TABLE_SIZE.
-current_scope: entero para el nivel de scope (en esta fase se mantiene en 0).
-next_addr: siguiente índice libre en vm_memory.
-vm_memory: arreglo double de tamaño MAX_VARS que modela la memoria de la VM.

Funciones de la tabla de símbolos:
-sym_hash(nombre): usa un hash tipo djb2 para mapear strings a índices en la tabla.
-sym_lookup(nombre): busca cualquier símbolo con ese nombre en toda la tabla.
-sym_lookup_local(nombre): busca solo en el scope actual (current_scope).
-sym_insert(nombre, kind, type, length):

Crea un nuevo Sym.
Ajusta length (1 si se pasa <= 0).
Marca is_array si length > 1.

Si es variable o constante:
Asigna un bloque de length posiciones en vm_memory a partir de next_addr.
Inicializa esas posiciones a 0.0.
Incrementa next_addr en length.
Inserta el símbolo en la lista enlazada correspondiente al hash.

Errores semánticos:
semantic_error(fmt, ...): imprime “Semantic error: ...” y termina el programa.

4.2. Definición de bytecode y generación de instrucciones
Se define:
OpCode: enum con todas las instrucciones:
-OP_PUSH_NUM (empuja número a la pila)
-OP_LOAD (carga vm_memory[addr])
-OP_STORE (guarda en vm_memory[addr])
-OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD
-OP_LT, OP_LTE, OP_GT, OP_GTE, OP_EQ, OP_NEQ
-OP_AND, OP_OR, OP_NOT
-OP_JUMP, OP_JUMP_IF_FALSE
-OP_CALL_BUILTIN
-OP_POP
-OP_HALT
-OP_LOAD_IND (acceso indirecto para arreglos)
-OP_STORE_IND (escritura indirecta para arreglos)

struct Instr:
-op: opcode
-a: entero auxiliar
-d: double auxiliar
-code[MAX_CODE]: arreglo de instrucciones.
-code_size: número de instrucciones generadas.

Función emit(op, a, d):
-Inserta una instrucción en code[code_size].
-Devuelve el índice donde se insertó (útil para backpatch de saltos).
-Comprueba que no se exceda MAX_CODE.

Función to_bool(v):
Convierte un double a 0.0 o 1.0 según sea falso o verdadero.

Identificadores de builtins (BuiltinId):
-BI_ABS, BI_MIN, BI_MAX, BI_SQRT, BI_POW
-BI_CHECKLINELEFT, BI_CHECKLINERIGHT
-BI_ACCELERATE, BI_SETFORWARD, BI_SETBACKWARD
-BI_BRAKE, BI_TURNLEFT, BI_TURNRIGHT, BI_TURNANGLE

4.3. %union y tokens

%union define:

-int_val (para enteros, tipos, índices de salto, etc.)
-float_val (para literales flotantes)
-string_val (para nombres de identificador)

Se declaran tokens con su tipo de atributo:
-IDENTIFIER usa string_val.
-INTEGER usa int_val.
-FLOAT_LITERAL usa float_val.

Se declaran tokens para:
-Palabras clave, tipos, operadores, funciones builtin.
-No terminales con tipo int_val: var_type, builtin_func, arguments, arguments_tail, array_part, while_start, while_jmp.

4.4. Regla program

program:
statement_list
{
emit(OP_HALT, 0, 0.0);
}
;

Genera un OP_HALT al final, que hace que la VM termine la ejecución.

4.5. Lista de sentencias

– statement_list define una secuencia de statements separados por punto y coma, permitiendo también listas vacías.
– statement agrupa todos los tipos de sentencia del lenguaje.
– Las llamadas a funciones builtin usadas como statement descartan su valor de retorno con OP_POP, para no llenar la pila.
– Cada statement genera bytecode en el orden en que aparece, y ese orden define la ejecución real de la máquina virtual.
– Esta parte del parser conecta la sintaxis de alto nivel del programa con la secuencia lineal de instrucciones que interpreta la VM.

4.6. Declaración de variables y constantes

declaration maneja tres casos:
-var IDENT : tipo [size]
Comprueba que no esté ya declarada en el mismo scope.
-sym_insert como SYM_VAR con ese tipo y tamaño.
No genera bytecode extra: la reserva de memoria ya se hizo al insertar.
-var IDENT : tipo [size] = expresión
Igual, pero exige que size sea 1 (solo escalar).
-sym_insert.
La expresión deja el valor en la pila.
-OP_STORE en s->addr para guardar el valor inicial.
-const IDENT : tipo [size] = expresión
Similar a var, pero kind = SYM_CONST.
-No permite arrays constantes.
-Guarda el valor inicial con OP_STORE.

array_part:
[INTEGER] da length = ese entero.
vacío da length = 1 (no es arreglo).

4.7. Asignaciones
assignment cubre:
IDENT = expresión
Busca el símbolo.
Comprueba:
que exista,
que no sea const,
que no sea array (por qué entonces falta índice).
Emite OP_STORE en la dirección base.
IDENT [ expresión ] = expresión
Primero se procesa IDENT [ expresión ]:
Busca símbolo.
Comprueba que exista, que sea array y que no sea const.
Emite:
expresión deja el índice en la pila.
OP_PUSH_NUM con la dirección base del array.
OP_ADD para sumar base + indice → dirección efectiva.
Luego se procesa la parte de ASSIGN expression:
expresión deja el valor en la pila.
OP_STORE_IND:
Saca valor y dirección de la pila.
Guarda vm_memory[addr] = valor.

4.8. Tipos

var_type devuelve el token del tipo (TYPE_INT, TYPE_FLOAT, etc.) y se guarda en int_val para la tabla de símbolos. De momento la VM usa double para todos los datos; el tipo sirve para validaciones y documentación futura.

4.9. Expresiones y opcodes

Se siguen niveles clásicos de precedencia:

logical_or (||)

logical_and (&&)

equality (==, !=)

relational (<, <=, >, >=)

additive (+, -)

term (*, /, %)

factor

En cada nivel, la gramática genera el bytecode con la semántica de pila:

Ejemplo additive:

additive PLUS term:

primero se genera bytecode de additive (quedando A en la pila),

luego el de term (quedando B),

luego OP_ADD que consume A y B y empuja A+B.

Lo mismo para SUB, MUL, DIV, MOD, comparaciones, etc.

NOT factor:

Se compila factor (deja valor).

NOT añade OP_NOT, que invierte el booleano (1 → 0, 0 → 1).

4.10. Factor: números, identificadores, arrays, booleanos y llamadas

Casos:

number:

integer MINUS/positivo genera OP_PUSH_NUM con el valor correspondiente.

float igual.

TRUE / FALSE:

OP_PUSH_NUM 1.0 / 0.0.

IDENTIFIER:

Se busca el símbolo.

Comprueba que exista, que tenga addr válido (sea var/const) y que no sea array.

Emite OP_LOAD s->addr.

IDENTIFIER [ expresión ]:

Genera bytecode para expresión (índice en la pila).

Empuja la base del array (OP_PUSH_NUM addr).

OP_ADD → dirección efectiva.

OP_LOAD_IND → carga vm_memory[addr] y la deja en la pila.

function_call:

Se maneja con la regla function_call y builtin_func (ver abajo).

4.11. Estructuras de control

If-else:

La idea es:

if (cond) { then_block } else { else_block }

Se compila como:

cond deja valor en la pila.

OP_JUMP_IF_FALSE jmp_false (destino aún desconocido).

then_block...

OP_JUMP jmp_end (salto para saltarse el else).

backpatch: code[jmp_false].a = inicio_else;

else_block...

backpatch: code[jmp_end].a = final_else;

Se usa un atributo sintético (int_val) en if_statement para guardar las posiciones de los saltos, luego se rellenan.

While:

while_start:
$$ = code_size;

while_jmp:
$$ = emit(OP_JUMP_IF_FALSE, 0, 0.0);

while_loop:
WHILE while_start LPAREN boolean_expression RPAREN while_jmp LBRACE statement_list RBRACE
{
int start = $2;
int jmp_false = $6;
emit(OP_JUMP, start, 0.0);
code[jmp_false].a = code_size;
}

Traducción:

while_start captura la posición code_size al inicio del while (inicio de la condición).

Se genera boolean_expression.

while_jmp emite un OP_JUMP_IF_FALSE con destino 0 por ahora, y devuelve su índice.

Se genera el cuerpo LBRACE statement_list RBRACE.

Al final:

Se emite OP_JUMP start para volver al principio de la condición.

Se rellena code[jmp_false].a con la posición después del cuerpo (salida del bucle).

do_while_loop y for_loop:

Actualmente emiten semantic_error indicando que no están soportados en la VM.

4.12. Declaraciones de funciones y return

function_declaration:

Detecta “func nombre(...) : tipo { ... }”.

De momento, no se genera código ejecutable para funciones de usuario; se llama semantic_error diciendo que no están soportadas.

return_statement:

RETURN expression:

Genera el bytecode de la expresión.

Aún no hay opcodes específicos de return ni de llamadas a funciones de usuario.

4.13. Llamadas a funciones builtin y argumentos

builtin_func asocia tokens como ABS, MIN, ACCELERATE, etc. a un entero (BuiltinId).

function_call:

builtin_func LPAREN arguments RPAREN:

arguments deja en la pila sus valores en orden.

function_call emite OP_CALL_BUILTIN con:

a = id de builtin,

d = número de argumentos.

El resultado de la builtin queda en la pila.

IDENTIFIER LPAREN arguments RPAREN:

Verifica que exista y sea función de usuario.

Lanza semantic_error porque aún no están soportadas.

arguments y arguments_tail:

Cuentan cuántos argumentos hay:

expression arguments_tail → 1 + arguments_tail.

vacío → 0.

Ese recuento se pasa como double en in.d para OP_CALL_BUILTIN.

=========================================================
5. MAQUINA VIRTUAL – RUN_VM

run_vm es un intérprete de bytecode basado en pila.

Estructuras internas:

stack[1024]: pila de ejecución de double.

sp: índice del tope de la pila (empieza en -1).

pc: program counter, índice de la instrucción actual en code[].

Bucle principal:

for (;;) {
Instr in = code[pc];
switch (in.op) {
...
}
}

Para cada opcode:

OP_PUSH_NUM:

sp++; stack[sp] = in.d; pc++;

OP_LOAD:

sp++; stack[sp] = vm_memory[in.a]; pc++;

OP_STORE:

vm_memory[in.a] = stack[sp]; sp--; pc++;

OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD:

Sacan dos valores de la pila, operan y empujan el resultado.

Comparaciones y lógicos:

Sacan dos valores, comparan o combinan, y empujan 0.0 o 1.0 según resultado.

OP_NOT:

Saca un valor, si es “verdadero” se convierte en 0.0, si es “falso” se convierte en 1.0.

OP_JUMP:

pc = in.a.

OP_JUMP_IF_FALSE:

Saca cond de la pila.

Si cond es falso (0.0), pc = in.a; si no, pc++.

OP_CALL_BUILTIN:

Lee id de builtin de in.a y argc (número de argumentos) de in.d.

Saca argc valores de la pila en orden inverso y los coloca en un arreglo args[ ].

Ejecuta el caso correspondiente:

ABS, MIN, MAX, SQRT, POW usan funciones de math.h.

CHECKLINELEFT / CHECKLINERIGHT:

Imprimen un mensaje (“checkLineLeft() -> 1”) y devuelven 1.0 (simulación de sensores).

ACCELERATE, SETFORWARD, SETBACKWARD, BRAKE, TURNLEFT, TURNRIGHT, TURNANGLE:

Imprimen el nombre y el argumento, simulando que el carrito se mueve.

Devuelven 0.0.

Empuja el resultado res en la pila.

pc++.

OP_POP:

Comprueba que la pila no esté vacía.

sp--.

pc++.

OP_LOAD_IND y OP_STORE_IND:

LOAD_IND:

Saca una dirección de la pila, la convierte a int.

Comprueba que esté en rango [0, MAX_VARS).

Empuja vm_memory[addr].

STORE_IND:

Saca value y addr de la pila.

Comprueba rango.

vm_memory[addr] = value.

OP_HALT:

Imprime “Ejecucion terminada”.

return para salir de run_vm.

=========================================================
6. SAVE_BYTECODE – FORMATO DEL ARCHIVO .CHAMBA.BC

Función save_bytecode(filename):

Abre el archivo en modo texto.

Escribe primero el número de instrucciones: code_size.

Luego, por cada instrucción i:

Escribe: op a d

Cada uno separado por espacios:

op: entero correspondiente a OpCode.

a: entero auxiliar.

d: double en formato %.17g para no perder precisión.

Ejemplo de contenido:

28
0 0 5
2 0 0
...

Donde 28 es el número de instrucciones y cada línea siguiente es una instrucción.

=========================================================
7. MAIN.C – PUNTO DE ENTRADA

main:

Comprueba que el usuario pase un archivo .chamba.

Abre el archivo y asigna el puntero a yyin para que lexer/ parser lo lean.

Llama yyparse():

Si devuelve 0:

Muestra “Analisis sintactico y semantico completado exitosamente”.

Cierra yyin.

Construye el nombre del bytecode:

Reemplaza extensión por .chamba.bc si había punto.

O añade .bc si no había punto.

Llama save_bytecode(bc_name).

Llama run_vm() para ejecutar el bytecode.

Si yyparse() falla:

Cierra yyin.

Imprime “Error en el analisis”.

Devuelve 1.

=========================================================