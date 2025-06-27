/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include "peqcomp.h"

#define TAMANHO_CODIGO 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.sbas> [arg1] [arg2] [arg3]\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Aloca buffer na pilha
    unsigned char codigo[TAMANHO_CODIGO];

    // Compila o codigo
    funcp funcao_compilada = peqcomp(arquivo, codigo);
    fclose(arquivo);

    if (!funcao_compilada) {
        fprintf(stderr, "Erro na compilacao\n");
        return 1;
    }

    // Prepara os argumentos
    int args[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        if ((i + 2) < argc) {
            args[i] = atoi(argv[i + 2]);
        } else {
            break;
        }
    }

    // Chama a funcao e imprime
    printf("Executando '%s' com args (%d, %d, %d)\n", argv[1], args[0], args[1], args[2]);
    int resultado = funcao_compilada(args[0], args[1], args[2]);
    printf("Resultado: %d\n", resultado);

    return 0;
}