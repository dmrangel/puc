/* Davi Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "peqcomp.h"

#define CODE_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.sbas> [arg1] [arg2] [arg3]\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Erro ao abrir arquivo fonte");
        return 1;
    }

    // Aloca memória executável usando mmap
    unsigned char *codigo = mmap(NULL, CODE_SIZE,
                                 PROT_READ | PROT_WRITE | PROT_EXEC,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (codigo == MAP_FAILED) {
        perror("mmap failed");
        fclose(f);
        return 1;
    }

    // Compila o código SBas
    funcp fptr = peqcomp(f, codigo);
    fclose(f);

    if (!fptr) {
        fprintf(stderr, "Falha ao compilar o código.\n");
        munmap(codigo, CODE_SIZE);
        return 1;
    }

    // Prepara os argumentos para a função SBas a partir da linha de comando
    int arg1 = (argc > 2) ? atoi(argv[2]) : 0;
    int arg2 = (argc > 3) ? atoi(argv[3]) : 0;
    int arg3 = (argc > 4) ? atoi(argv[4]) : 0;

    printf("Executando a função de '%s' com os argumentos (%d, %d, %d)...\n", argv[1], arg1, arg2, arg3);

    // Chama a função gerada
    int resultado = fptr(arg1, arg2, arg3);
    printf("Resultado: %d\n", resultado);

    // Libera a memória alocada
    munmap(codigo, CODE_SIZE);

    return 0;
}