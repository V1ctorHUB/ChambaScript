#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern FILE* yyin;
int yyparse(void);
void run_vm(void);
void save_bytecode(const char* filename);

int main(int argc, char** argv) {
    char outname[512];

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_fuente.chamba>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error al abrir el archivo");
        return 1;
    }

    if (yyparse() == 0) {
        printf("Analisis sintactico y semantico completado exitosamente\n");

        snprintf(outname, sizeof(outname), "%s.bc", argv[1]);
        save_bytecode(outname);

        run_vm();
    } else {
        printf("Error en el analisis sintactico\n");
    }

    fclose(yyin);
    return 0;
}
