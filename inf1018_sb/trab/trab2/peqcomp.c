/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "peqcomp.h"

#define MAX_LINHAS 30
#define VAR_OFFSET(v) (-(4 * (v)))

// Estrutura para guardar informações de saltos a serem corrigidos (backpatching).
typedef struct {
    int pos_ajuste;   // Posição no buffer de código a ser corrigida.
    int linha_alvo; // Linha de destino do salto.
} Patch;

// Funções auxiliares para emitir código de máquina.
static void emite_byte(unsigned char codigo[], int *pos, unsigned char byte) {
    codigo[(*pos)++] = byte;
}

static void emite_int32(unsigned char codigo[], int *pos, int32_t valor) {
    memcpy(&codigo[*pos], &valor, sizeof(int32_t));
    *pos += sizeof(int32_t);
}

// Gera o prólogo da função (configura o stack frame).
static void gera_prologo(unsigned char codigo[], int *pos) {
    emite_byte(codigo, pos, 0x55);                                    // push   %rbp
    emite_byte(codigo, pos, 0x48); emite_byte(codigo, pos, 0x89); emite_byte(codigo, pos, 0xe5); // mov    %rsp, %rbp
    emite_byte(codigo, pos, 0x48); emite_byte(codigo, pos, 0x83); emite_byte(codigo, pos, 0xec); emite_byte(codigo, pos, 0x14); // sub    $20, %rsp (5 vars * 4 bytes)
}

// Gera o epílogo da função (limpa a pilha e retorna).
static void gera_epilogo(unsigned char codigo[], int *pos) {
    emite_byte(codigo, pos, 0xc9); // leave
    emite_byte(codigo, pos, 0xc3); // ret
}

funcp peqcomp(FILE *f, unsigned char codigo[]) {
    char linhas[MAX_LINHAS][128];
    int n_linhas = 0;
    int pos = 0;

    int end_linha[MAX_LINHAS];
    Patch ajustes[MAX_LINHAS];
    int cont_ajuste = 0;

    // 1. Lê todas as linhas do arquivo para a memória.
    while (n_linhas < MAX_LINHAS && fgets(linhas[n_linhas], sizeof(linhas[0]), f)) {
        linhas[n_linhas][strcspn(linhas[n_linhas], "\n\r")] = 0;
        if (strlen(linhas[n_linhas]) > 0) {
            n_linhas++;
        }
    }

    // 2. Gera o código de máquina, linha por linha.
    gera_prologo(codigo, &pos);

    for (int i = 0; i < n_linhas; i++) {
        end_linha[i] = pos;
        char *linha = linhas[i];
        int v1, v2, v3, val;
        char op;

        if (sscanf(linha, "v%d : $%d", &v1, &val) == 2) { // atribuição: v_dst : $const
            emite_byte(codigo, &pos, 0xc7); emite_byte(codigo, &pos, 0x45);
            emite_byte(codigo, &pos, VAR_OFFSET(v1));
            emite_int32(codigo, &pos, val);
        } else if (sscanf(linha, "v%d : p%d", &v1, &v2) == 2) { // atribuição: v_dst : p_src
            const unsigned char reg_map[] = {0x7d, 0x75, 0x55}; // p1:edi, p2:esi, p3:edx
            emite_byte(codigo, &pos, 0x89);
            emite_byte(codigo, &pos, reg_map[v2 - 1]);
            emite_byte(codigo, &pos, VAR_OFFSET(v1));
        } else if (sscanf(linha, "v%d : v%d", &v1, &v2) == 2) { // atribuição: v_dst : v_src
            emite_byte(codigo, &pos, 0x8b); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v2)); // mov v_src, %eax
            emite_byte(codigo, &pos, 0x89); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v1)); // mov %eax, v_dst
        } else if (sscanf(linha, "v%d = v%d %c v%d", &v1, &v2, &op, &v3) == 4) { // operação: v_dst = v_src1 op v_src2
            emite_byte(codigo, &pos, 0x8b); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v2)); // mov v_src1, %eax
            switch (op) {
                case '+': emite_byte(codigo, &pos, 0x03); break; // add
                case '-': emite_byte(codigo, &pos, 0x2b); break; // sub
                case '*': emite_byte(codigo, &pos, 0x0f); emite_byte(codigo, &pos, 0xaf); break; // imul
            }
            emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v3)); // op v_src2, %eax
            emite_byte(codigo, &pos, 0x89); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v1)); // mov %eax, v_dst
        } else if (sscanf(linha, "v%d = v%d %c $%d", &v1, &v2, &op, &val) == 4) { // operação: v_dst = v_src op $const
            emite_byte(codigo, &pos, 0x8b); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v2)); // mov v_src, %eax
            switch (op) {
                case '+': emite_byte(codigo, &pos, 0x05); break; // add $imm32, %eax
                case '-': emite_byte(codigo, &pos, 0x2d); break; // sub $imm32, %eax
                case '*': emite_byte(codigo, &pos, 0x69); emite_byte(codigo, &pos, 0xc0); break; // imul $imm32, %eax, %eax
            }
            emite_int32(codigo, &pos, val);
            emite_byte(codigo, &pos, 0x89); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v1)); // mov %eax, v_dst
        } else if (sscanf(linha, "ret $%d", &val) == 1) { // retorno de constante
            emite_byte(codigo, &pos, 0xb8); emite_int32(codigo, &pos, val); // mov $val, %eax
            gera_epilogo(codigo, &pos);
        } else if (sscanf(linha, "ret v%d", &v1) == 1) { // retorno de variável
            emite_byte(codigo, &pos, 0x8b); emite_byte(codigo, &pos, 0x45); emite_byte(codigo, &pos, VAR_OFFSET(v1)); // mov v_src, %eax
            gera_epilogo(codigo, &pos);
        } else if (sscanf(linha, "iflez v%d %d", &v1, &v2) == 2) { // desvio condicional
            emite_byte(codigo, &pos, 0x83); emite_byte(codigo, &pos, 0x7d); emite_byte(codigo, &pos, VAR_OFFSET(v1)); emite_byte(codigo, &pos, 0x00); // cmpl $0, v_src
            emite_byte(codigo, &pos, 0x0f); emite_byte(codigo, &pos, 0x8e); // jle <offset32>
            ajustes[cont_ajuste].pos_ajuste = pos;
            ajustes[cont_ajuste].linha_alvo = v2;
            cont_ajuste++;
            emite_int32(codigo, &pos, 0); // Salto temporário com offset 0
        }
    }
    
    // Garante que a função termina com um epílogo, caso a última instrução não seja 'ret'.
    if (pos > 0 && codigo[pos - 1] != 0xc3) {
        gera_epilogo(codigo, &pos);
    }

    // 3. Corrige todos os saltos pendentes.
    for (int i = 0; i < cont_ajuste; i++) {
        int pos_ajuste = ajustes[i].pos_ajuste;
        int end_destino = end_linha[ajustes[i].linha_alvo - 1];
        int32_t offset = end_destino - (pos_ajuste + 4); // Offset é relativo ao fim da instrução de salto.
        
        int pos_original = pos_ajuste; // Salva a posição para não alterá-la
        emite_int32(codigo, &pos_original, offset);
    }

    return (funcp)codigo;
}