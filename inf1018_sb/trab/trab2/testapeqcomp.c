/* Nome_do_Aluno1 Matricula Turma */
/* Nome_do_Aluno2 Matricula Turma */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peqcomp.h"

#define CODE_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.sbas> [param1 param2 param3]\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    unsigned char codigo[CODE_SIZE];
    funcp func = peqcomp(f, codigo);
    fclose(f);

    int p1 = argc > 2 ? atoi(argv[2]) : 0;
    int p2 = argc > 3 ? atoi(argv[3]) : 0;
    int p3 = argc > 4 ? atoi(argv[4]) : 0;

    int resultado = func(p1, p2, p3);
    printf("Resultado: %d\n", resultado);

    return 0;
}
