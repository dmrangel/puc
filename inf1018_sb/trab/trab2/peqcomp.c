/* Davi Rangel 2411930 3WB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "peqcomp.h"

#define MAX_LINHAS 30
#define MAX_CODE_SIZE 4096

// Converte o índice da variável SBas (1-5) para o deslocamento na pilha
#define VAR_OFFSET(v) (-(4 * (v)))

// Estrutura para backpatching: guarda informações de saltos a serem corrigidos
typedef struct {
    int pos_patch;   // Posição no buffer de código a ser corrigida
    int target_line; // Linha de destino do salto
} Patch;

// Funções auxiliares para emitir código de máquina
static void emit_byte(unsigned char codigo[], int *pos, unsigned char byte) {
    codigo[(*pos)++] = byte;
}

static void emit_dword(unsigned char codigo[], int *pos, int32_t dword) {
    memcpy(&codigo[*pos], &dword, sizeof(int32_t));
    *pos += sizeof(int32_t);
}

// Gera o prólogo da função (setup do stack frame)
static void gen_prologue(unsigned char codigo[], int *pos) {
    emit_byte(codigo, pos, 0x55);                                    // push   %rbp
    emit_byte(codigo, pos, 0x48); emit_byte(codigo, pos, 0x89); emit_byte(codigo, pos, 0xe5); // mov    %rsp,%rbp
    emit_byte(codigo, pos, 0x48); emit_byte(codigo, pos, 0x83); emit_byte(codigo, pos, 0xec); emit_byte(codigo, pos, 0x14); // sub    $20,%rsp (5 vars * 4 bytes)
}

// Gera o epílogo da função (cleanup e retorno)
static void gen_epilogue(unsigned char codigo[], int *pos) {
    emit_byte(codigo, pos, 0xc9); // leave
    emit_byte(codigo, pos, 0xc3); // ret
}

funcp peqcomp(FILE *f, unsigned char codigo[]) {
    char lines[MAX_LINHAS][128];
    int num_lines = 0;
    int code_pos = 0;

    int line_addr[MAX_LINHAS];
    Patch patches[MAX_LINHAS];
    int patch_count = 0;

    // 1. Ler todas as linhas do arquivo para a memória
    while (num_lines < MAX_LINHAS && fgets(lines[num_lines], sizeof(lines[0]), f)) {
        lines[num_lines][strcspn(lines[num_lines], "\n\r")] = 0; // Remove newline
        if (strlen(lines[num_lines]) > 0) {
            num_lines++;
        }
    }

    // 2. Gerar código de máquina (com placeholders para saltos)
    gen_prologue(codigo, &code_pos);

    for (int i = 0; i < num_lines; i++) {
        line_addr[i] = code_pos;
        char *line = lines[i];

        int v1, v2, v3, val;
        char op;

        // Tenta fazer o parse para cada tipo de instrução
        if (sscanf(line, "v%d : $%d", &v1, &val) == 2) { // v_dst : $const
            emit_byte(codigo, &code_pos, 0xc7); emit_byte(codigo, &code_pos, 0x45);
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
            emit_dword(codigo, &code_pos, val);
        } else if (sscanf(line, "v%d : p%d", &v1, &v2) == 2) { // v_dst : p_src
            emit_byte(codigo, &code_pos, 0x89);
            // p1->edi, p2->esi, p3->edx
            const unsigned char reg_map[] = {0x7d, 0x75, 0x55}; // edi, esi, edx
            emit_byte(codigo, &code_pos, reg_map[v2 - 1]);
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
        } else if (sscanf(line, "v%d : v%d", &v1, &v2) == 2) { // v_dst : v_src
            emit_byte(codigo, &code_pos, 0x8b); emit_byte(codigo, &code_pos, 0x45); // mov -off_src(%rbp), %eax
            emit_byte(codigo, &code_pos, VAR_OFFSET(v2));
            emit_byte(codigo, &code_pos, 0x89); emit_byte(codigo, &code_pos, 0x45); // mov %eax, -off_dst(%rbp)
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
        } else if (sscanf(line, "v%d = v%d %c v%d", &v1, &v2, &op, &v3) == 4) { // v_dst = v_src1 op v_src2
            emit_byte(codigo, &code_pos, 0x8b); emit_byte(codigo, &code_pos, 0x45); // mov v_src1, %eax
            emit_byte(codigo, &code_pos, VAR_OFFSET(v2));
            switch (op) {
                case '+': emit_byte(codigo, &code_pos, 0x03); break; // add
                case '-': emit_byte(codigo, &code_pos, 0x2b); break; // sub
                case '*': emit_byte(codigo, &code_pos, 0x0f); emit_byte(codigo, &code_pos, 0xaf); break; // imul
            }
            emit_byte(codigo, &code_pos, 0x45);
            emit_byte(codigo, &code_pos, VAR_OFFSET(v3));
            emit_byte(codigo, &code_pos, 0x89); emit_byte(codigo, &code_pos, 0x45); // mov %eax, v_dst
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
        } else if (sscanf(line, "v%d = v%d %c $%d", &v1, &v2, &op, &val) == 4) { // v_dst = v_src op $const
            emit_byte(codigo, &code_pos, 0x8b); emit_byte(codigo, &code_pos, 0x45); // mov v_src, %eax
            emit_byte(codigo, &code_pos, VAR_OFFSET(v2));
            switch (op) {
                case '+': emit_byte(codigo, &code_pos, 0x05); break; // add $imm32, %eax
                case '-': emit_byte(codigo, &code_pos, 0x2d); break; // sub $imm32, %eax
                case '*': emit_byte(codigo, &code_pos, 0x69); emit_byte(codigo, &code_pos, 0xc0); break; // imul $imm32, %eax, %eax
            }
            emit_dword(codigo, &code_pos, val);
            emit_byte(codigo, &code_pos, 0x89); emit_byte(codigo, &code_pos, 0x45); // mov %eax, v_dst
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
        } else if (sscanf(line, "ret $%d", &val) == 1) { // ret $const
            emit_byte(codigo, &code_pos, 0xb8); // mov $val, %eax
            emit_dword(codigo, &code_pos, val);
            gen_epilogue(codigo, &code_pos);
        } else if (sscanf(line, "ret v%d", &v1) == 1) { // ret v_src
            emit_byte(codigo, &code_pos, 0x8b); emit_byte(codigo, &code_pos, 0x45); // mov -off(%rbp), %eax
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
            gen_epilogue(codigo, &code_pos);
        } else if (sscanf(line, "iflez v%d %d", &v1, &v2) == 2) { // iflez v_src, line_num
            emit_byte(codigo, &code_pos, 0x83); emit_byte(codigo, &code_pos, 0x7d); // cmpl $0, -off(%rbp)
            emit_byte(codigo, &code_pos, VAR_OFFSET(v1));
            emit_byte(codigo, &code_pos, 0x00);
            emit_byte(codigo, &code_pos, 0x0f); emit_byte(codigo, &code_pos, 0x8e); // jle <offset32>
            patches[patch_count].pos_patch = code_pos;
            patches[patch_count].target_line = v2;
            patch_count++;
            emit_dword(codigo, &code_pos, 0); // Placeholder for offset
        }
    }
    
    // Se a última instrução não foi um `ret`, adiciona um epílogo
    if (code_pos > 0 && codigo[code_pos-1] != 0xc3) {
        gen_epilogue(codigo, &code_pos);
    }

    // 3. Backpatching: resolver todos os saltos
    for (int i = 0; i < patch_count; i++) {
        int patch_addr = patches[i].pos_patch;
        int target_addr = line_addr[patches[i].target_line - 1];
        int32_t offset = target_addr - (patch_addr + 4); // O offset é relativo ao final da instrução de salto
        
        // Escreve o offset calculado no buffer de código
        emit_dword(codigo, &patch_addr, offset);
    }

    return (funcp)codigo;
}