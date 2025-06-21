/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "peqcomp.h"

#define MAX_LINHAS 30
#define VAR_OFFSET(i) (-(4 * (i)))

typedef struct {
    int linha_destino;
    int pos_patch;
} Desvio;

static int linha_pos[MAX_LINHAS];
static Desvio desvios[MAX_LINHAS];
static int qtd_desvios = 0;

static void emitir(unsigned char *codigo, int *tam, unsigned char byte) {
    codigo[(*tam)++] = byte;
}

static void emitir32(unsigned char *codigo, int *tam, int val) {
    memcpy(&codigo[*tam], &val, 4);
    *tam += 4;
}

static void mov_const_para_var(unsigned char *codigo, int *tam, int val, int var) {
    emitir(codigo, tam, 0xc7);
    emitir(codigo, tam, 0x45);
    emitir(codigo, tam, VAR_OFFSET(var));
    emitir32(codigo, tam, val);
}

static void mov_param_para_var(unsigned char *codigo, int *tam, int param, int var) {
    static const unsigned char modrm[3] = { 0x7d, 0x75, 0x55 }; // edi, esi, edx
    emitir(codigo, tam, 0x89);
    emitir(codigo, tam, modrm[param]);
    emitir(codigo, tam, VAR_OFFSET(var));
}

static void mov_var_para_var(unsigned char *codigo, int *tam, int origem, int destino) {
    emitir(codigo, tam, 0x8b);
    emitir(codigo, tam, 0x45);
    emitir(codigo, tam, VAR_OFFSET(origem));
    emitir(codigo, tam, 0x89);
    emitir(codigo, tam, 0x45);
    emitir(codigo, tam, VAR_OFFSET(destino));
}

static void ret_const(unsigned char *codigo, int *tam, int val) {
    emitir(codigo, tam, 0xb8);
    emitir32(codigo, tam, val);
}

static void ret_var(unsigned char *codigo, int *tam, int var) {
    emitir(codigo, tam, 0x8b);
    emitir(codigo, tam, 0x45);
    emitir(codigo, tam, VAR_OFFSET(var));
}

static void gerar_prologo(unsigned char *codigo, int *tam) {
    emitir(codigo, tam, 0x55);                              // push %rbp
    emitir(codigo, tam, 0x48); emitir(codigo, tam, 0x89); emitir(codigo, tam, 0xe5); // mov %rsp, %rbp
    emitir(codigo, tam, 0x48); emitir(codigo, tam, 0x83); emitir(codigo, tam, 0xec); emitir(codigo, tam, 0x20); // sub $32, %rsp
}

static void gerar_epilogo(unsigned char *codigo, int *tam) {
    emitir(codigo, tam, 0x5d); // pop %rbp
    emitir(codigo, tam, 0xc3); // ret
}

funcp peqcomp(FILE *f, unsigned char codigo[]) {
    char linha[128];
    int tam = 0, num_linha = 0;

    gerar_prologo(codigo, &tam);

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0'; // remove \n
        linha_pos[num_linha] = tam;

        int v1, v2, val;

        if (sscanf(linha, "v%d : $%d", &v1, &val) == 2) {
            mov_const_para_var(codigo, &tam, val, v1);
        } else if (sscanf(linha, "v%d : p%d", &v1, &v2) == 2) {
            mov_param_para_var(codigo, &tam, v2 - 1, v1);
        } else if (sscanf(linha, "v%d : v%d", &v1, &v2) == 2) {
            mov_var_para_var(codigo, &tam, v2, v1);
        } else if (sscanf(linha, "ret $%d", &val) == 1) {
            ret_const(codigo, &tam, val);
        } else if (sscanf(linha, "ret v%d", &v1) == 1) {
            ret_var(codigo, &tam, v1);
        } else if (sscanf(linha, "iflez v%d %d", &v1, &val) == 2) {
            emitir(codigo, &tam, 0x83); emitir(codigo, &tam, 0x7d); emitir(codigo, &tam, VAR_OFFSET(v1)); emitir(codigo, &tam, 0x00);
            emitir(codigo, &tam, 0x7e); emitir(codigo, &tam, 0x00);
            desvios[qtd_desvios++] = (Desvio){ .linha_destino = val - 1, .pos_patch = tam - 1 };
        }

        num_linha++;
    }

    for (int i = 0; i < qtd_desvios; i++) {
        int origem = desvios[i].pos_patch;
        int destino = linha_pos[desvios[i].linha_destino];
        int offset = destino - (origem + 1);
        codigo[origem] = (unsigned char)offset;
    }

    gerar_epilogo(codigo, &tam);
    return (funcp)codigo;
}
