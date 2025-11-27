#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "vm.h"

extern FILE* yyin;
int yyparse(void);

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo.chamba>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error al abrir el archivo");
        return 1;
    }

    if (yyparse() == 0) {
        printf("Analisis sintactico y semantico completado exitosamente\n");
        fclose(yyin);

        char bc_name[1024];
        strncpy(bc_name, argv[1], sizeof(bc_name) - 1);
        bc_name[sizeof(bc_name) - 1] = '\0';

        char* dot = strrchr(bc_name, '.');
        if (dot) {
            strcpy(dot, ".chamba.bc");
        } else {
            strcat(bc_name, ".bc");
        }

        save_bytecode(bc_name);
        run_vm();
    } else {
        fclose(yyin);
        printf("Error en el analisis\n");
        return 1;
    }

    return 0;
}
