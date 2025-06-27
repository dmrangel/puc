/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "peqcomp.h"

#define TAMANHO_CODIGO 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.sbas> [arg1] [arg2] [arg3]\n", argv[0]);
        return 1;
    }

    FILE *arquivo_sbas = fopen(argv[1], "r");
    if (!arquivo_sbas) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Aloca memoria executavel.
    unsigned char *codigo_maquina = mmap(NULL, TAMANHO_CODIGO,
                                         PROT_READ | PROT_WRITE | PROT_EXEC,
                                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (codigo_maquina == MAP_FAILED) {
        perror("Erro no mmap");
        fclose(arquivo_sbas);
        return 1;
    }

    // Compila o codigo SBas.
    funcp ponteiro_funcao = peq_compila(arquivo_sbas, codigo_maquina);
    fclose(arquivo_sbas);

    if (!ponteiro_funcao) {
        fprintf(stderr, "Erro na compilacao\n");
        munmap(codigo_maquina, TAMANHO_CODIGO);
        return 1;
    }

    // Prepara os argumentos para a funcao compilada.
    int arg1 = (argc > 2) ? atoi(argv[2]) : 0;
    int arg2 = (argc > 3) ? atoi(argv[3]) : 0;
    int arg3 = (argc > 4) ? atoi(argv[4]) : 0;

    printf("Executando '%s' com args (%d, %d, %d)\n", argv[1], arg1, arg2, arg3);

    // Chama a funcao gerada e imprime o resultado.
    int resultado = ponteiro_funcao(arg1, arg2, arg3);
    printf("Resultado: %d\n", resultado);

    // Libera a memoria alocada.
    munmap(codigo_maquina, TAMANHO_CODIGO);

    return 0;
}