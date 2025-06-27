/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "peqcomp.h"

#define TAM_CODIGO 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.sbas> [arg1] [arg2] [arg3]\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    // Aloca memória executável.
    unsigned char *codigo = mmap(NULL, TAM_CODIGO,
                                 PROT_READ | PROT_WRITE | PROT_EXEC,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (codigo == MAP_FAILED) {
        perror("Erro de mmap");
        fclose(f);
        return 1;
    }

    // Compila o código SBas.
    funcp fptr = peqcomp(f, codigo);
    fclose(f);

    if (!fptr) {
        fprintf(stderr, "Erro de compilação\n");
        munmap(codigo, TAM_CODIGO);
        return 1;
    }

    // Prepara os argumentos para a função compilada.
    int arg1 = (argc > 2) ? atoi(argv[2]) : 0;
    int arg2 = (argc > 3) ? atoi(argv[3]) : 0;
    int arg3 = (argc > 4) ? atoi(argv[4]) : 0;

    printf("Função: '%s' | Args: (%d, %d, %d)\n", argv[1], arg1, arg2, arg3);

    // Chama a função gerada e imprime o resultado.
    int resultado = fptr(arg1, arg2, arg3);
    printf("Resultado: %d\n", resultado);

    // Libera a memória alocada.
    munmap(codigo, TAM_CODIGO);

    return 0;
}