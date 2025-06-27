/* Davi_Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "peqcomp.h"

#define MAX_LINHAS 30
// Converte o indice da variavel sbas (1-5) pro deslocamento na pilha
#define DESLOCAMENTO_VAR(v) (-(4 * (v)))

// Estrutura pra guardar informacoes de saltos a serem corrigidos (backpatching).
typedef struct {
    int posicao_ajuste; // Posicao no buffer de codigo a ser corrigida.
    int linha_alvo;     // Linha de destino do salto.
} Ajuste;

// Funcoes auxiliares pra emitir codigo de maquina.
static void emitir_byte(unsigned char codigo[], int *pos, unsigned char byte) {
    codigo[(*pos)++] = byte;
}

static void emitir_inteiro32(unsigned char codigo[], int *pos, int32_t valor) {
    memcpy(&codigo[*pos], &valor, sizeof(int32_t));
    *pos += sizeof(int32_t);
}

// Funcao especifica pra escrever um valor em qualquer ponto do codigo (usada no backpatching).
static void escrever_inteiro32_em(unsigned char codigo[], int posicao, int32_t valor) {
    memcpy(&codigo[posicao], &valor, sizeof(int32_t));
}

// Gera o prologo da funcao (configura o stack frame).
static void gerar_prologo(unsigned char codigo[], int *pos) {
    emitir_byte(codigo, pos, 0x55);                                    // push   %rbp
    emitir_byte(codigo, pos, 0x48); emitir_byte(codigo, pos, 0x89); emitir_byte(codigo, pos, 0xe5); // mov    %rsp, %rbp
    emitir_byte(codigo, pos, 0x48); emitir_byte(codigo, pos, 0x83); emitir_byte(codigo, pos, 0xec); emitir_byte(codigo, pos, 0x14); // sub    $20, %rsp
}

// Gera o epilogo da funcao (limpa a pilha e retorna).
static void gerar_epilogo(unsigned char codigo[], int *pos) {
    emitir_byte(codigo, pos, 0xc9); // leave
    emitir_byte(codigo, pos, 0xc3); // ret
}

funcp peq_compila(FILE *f, unsigned char codigo_maquina[]) {
    char linhas_fonte[MAX_LINHAS][128];
    int num_linhas = 0;
    int pos_codigo = 0;

    int endereco_linha[MAX_LINHAS];
    Ajuste lista_ajustes[MAX_LINHAS];
    int contador_ajustes = 0;

    // 1. Le todas as linhas do arquivo pra memoria.
    while (num_linhas < MAX_LINHAS && fgets(linhas_fonte[num_linhas], sizeof(linhas_fonte[0]), f)) {
        linhas_fonte[num_linhas][strcspn(linhas_fonte[num_linhas], "\n\r")] = 0;
        if (strlen(linhas_fonte[num_linhas]) > 0) {
            num_linhas++;
        }
    }

    // 2. Gera o codigo de maquina, linha por linha.
    gerar_prologo(codigo_maquina, &pos_codigo);

    for (int i = 0; i < num_linhas; i++) {
        endereco_linha[i] = pos_codigo;
        char *linha = linhas_fonte[i];
        int v1, v2, v3, val;
        char op;

        if (sscanf(linha, "v%d : $%d", &v1, &val) == 2) { // atribuicao: v_dst : $const
            emitir_byte(codigo_maquina, &pos_codigo, 0xc7); emitir_byte(codigo_maquina, &pos_codigo, 0x45);
            emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1));
            emitir_inteiro32(codigo_maquina, &pos_codigo, val);
        } else if (sscanf(linha, "v%d : p%d", &v1, &v2) == 2) { // atribuicao: v_dst : p_src
            const unsigned char mapa_regs[] = {0x7d, 0x75, 0x55}; // p1:edi, p2:esi, p3:edx
            emitir_byte(codigo_maquina, &pos_codigo, 0x89);
            emitir_byte(codigo_maquina, &pos_codigo, mapa_regs[v2 - 1]);
            emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1));
        } else if (sscanf(linha, "v%d : v%d", &v1, &v2) == 2) { // atribuicao: v_dst : v_src
            emitir_byte(codigo_maquina, &pos_codigo, 0x8b); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v2));
            emitir_byte(codigo_maquina, &pos_codigo, 0x89); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1));
        } else if (sscanf(linha, "v%d = v%d %c v%d", &v1, &v2, &op, &v3) == 4) { // operacao: v_dst = v_src1 op v_src2
            emitir_byte(codigo_maquina, &pos_codigo, 0x8b); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v2));
            switch (op) {
                case '+': emitir_byte(codigo_maquina, &pos_codigo, 0x03); break;
                case '-': emitir_byte(codigo_maquina, &pos_codigo, 0x2b); break;
                case '*': emitir_byte(codigo_maquina, &pos_codigo, 0x0f); emitir_byte(codigo_maquina, &pos_codigo, 0xaf); break;
            }
            emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v3));
            emitir_byte(codigo_maquina, &pos_codigo, 0x89); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1));
        } else if (sscanf(linha, "v%d = v%d %c $%d", &v1, &v2, &op, &val) == 4) { // operacao: v_dst = v_src op $const
            emitir_byte(codigo_maquina, &pos_codigo, 0x8b); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v2));
            switch (op) {
                case '+': emitir_byte(codigo_maquina, &pos_codigo, 0x05); break;
                case '-': emitir_byte(codigo_maquina, &pos_codigo, 0x2d); break;
                case '*': emitir_byte(codigo_maquina, &pos_codigo, 0x69); emitir_byte(codigo_maquina, &pos_codigo, 0xc0); break;
            }
            emitir_inteiro32(codigo_maquina, &pos_codigo, val);
            emitir_byte(codigo_maquina, &pos_codigo, 0x89); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1));
        } else if (sscanf(linha, "ret $%d", &val) == 1) { // retorno de constante
            emitir_byte(codigo_maquina, &pos_codigo, 0xb8); emitir_inteiro32(codigo_maquina, &pos_codigo, val);
            gerar_epilogo(codigo_maquina, &pos_codigo);
        } else if (sscanf(linha, "ret v%d", &v1) == 1) { // retorno de variavel
            emitir_byte(codigo_maquina, &pos_codigo, 0x8b); emitir_byte(codigo_maquina, &pos_codigo, 0x45); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1));
            gerar_epilogo(codigo_maquina, &pos_codigo);
        } else if (sscanf(linha, "iflez v%d %d", &v1, &v2) == 2) { // desvio condicional
            emitir_byte(codigo_maquina, &pos_codigo, 0x83); emitir_byte(codigo_maquina, &pos_codigo, 0x7d); emitir_byte(codigo_maquina, &pos_codigo, DESLOCAMENTO_VAR(v1)); emitir_byte(codigo_maquina, &pos_codigo, 0x00);
            emitir_byte(codigo_maquina, &pos_codigo, 0x0f); emitir_byte(codigo_maquina, &pos_codigo, 0x8e);
            lista_ajustes[contador_ajustes].posicao_ajuste = pos_codigo;
            lista_ajustes[contador_ajustes].linha_alvo = v2;
            contador_ajustes++;
            emitir_inteiro32(codigo_maquina, &pos_codigo, 0); // Salto temporario com offset 0
        }
    }
    
    // Garante que a funcao termina com um epilogo, caso a ultima instrucao nao seja 'ret'.
    if (pos_codigo > 0 && codigo_maquina[pos_codigo - 1] != 0xc3) {
        gerar_epilogo(codigo_maquina, &pos_codigo);
    }

    // 3. Corrige todos os saltos pendentes (backpatching).
    for (int i = 0; i < contador_ajustes; i++) {
        int pos_a_ajustar = lista_ajustes[i].posicao_ajuste;
        int endereco_de_destino = endereco_linha[lista_ajustes[i].linha_alvo - 1];
        int32_t deslocamento = endereco_de_destino - (pos_a_ajustar + 4); // Offset e relativo ao fim da instrucao de salto.
        
        escrever_inteiro32_em(codigo_maquina, pos_a_ajustar, deslocamento);
    }

    return (funcp)codigo_maquina;
}