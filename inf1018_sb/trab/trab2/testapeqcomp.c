/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include "peqcomp.h"

#define TAMANHO_CODIGO 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Erro: faltou o arquivo sbas\n");
        return 1;
    }

    FILE *arquivo_sbas = fopen(argv[1], "r");
    if (!arquivo_sbas) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Aloca um buffer na pilha para o codigo de maquina
    unsigned char codigo_maquina[TAMANHO_CODIGO];

    // Compila o codigo sbas
    funcp ponteiro_funcao = peq_compila(arquivo_sbas, codigo_maquina);
    fclose(arquivo_sbas);

    if (!ponteiro_funcao) {
        fprintf(stderr, "Erro na compilacao\n");
        return 1;
    }

    // Prepara os argumentos para a funcao compilada
    int args[3] = {0, 0, 0}; // Default 0
    for (int i = 0; i < 3; i++) {
        if ((i + 2) < argc) {
            args[i] = atoi(argv[i + 2]);
        } else {
            break;
        }
    }

    printf("Executando %s com args %d, %d, %d\n", argv[1], args[0], args[1], args[2]);

    // Chama a funcao gerada e imprime o resultado
    int resultado = ponteiro_funcao(args[0], args[1], args[2]);
    printf("Resultado: %d\n", resultado);

    // Nao e necessario liberar memoria, pois ela esta na pilha
    return 0;
}