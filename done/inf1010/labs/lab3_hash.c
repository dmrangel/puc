/*
Nome: Davi Rangel
Turma: 3WB
Matrícula: 2411930
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TAM_TAB 1200
#define TAM_PLACA 8

unsigned int hash(char *str) {
    unsigned int val = 0;
    for (int i = 0; str[i] != '\0'; i++)
        val = val * 31 + str[i];
    return val % TAM_TAB;
}

unsigned int tentativa(unsigned int h, int i) {
    return (h + i * i) % TAM_TAB;
}

int inserir(char tabela[TAM_TAB][TAM_PLACA], char *placa, int *colisoes) {
    unsigned int h = hash(placa);
    for (int i = 0; i < TAM_TAB; i++) {
        unsigned int idx = tentativa(h, i);
        if (tabela[idx][0] == '\0') {
            strncpy(tabela[idx], placa, TAM_PLACA);
            return 1;
        }
        if (strcmp(tabela[idx], placa) == 0) {
            return 0;
        }
        (*colisoes)++;
    }
    return 0;
}

int buscar(char tabela[TAM_TAB][TAM_PLACA], char *placa) {
    unsigned int h = hash(placa);
    for (int i = 0; i < TAM_TAB; i++) {
        unsigned int idx = tentativa(h, i);
        if (tabela[idx][0] == '\0') return 0;
        if (strcmp(tabela[idx], placa) == 0) return 1;
    }
    return 0;
}

int deletar(char tabela[TAM_TAB][TAM_PLACA], char *placa) {
    unsigned int h = hash(placa);
    for (int i = 0; i < TAM_TAB; i++) {
        unsigned int idx = tentativa(h, i);
        if (tabela[idx][0] == '\0') return 0;
        if (strcmp(tabela[idx], placa) == 0) {
            tabela[idx][0] = '\0';
            return 1;
        }
    }
    return 0;
}

double tempo_execucao(void (*func)(char tabela[TAM_TAB][TAM_PLACA], char *arquivo, int *total, int *sucesso, int *colisoes), char tabela[TAM_TAB][TAM_PLACA], char *arquivo, int *total, int *sucesso, int *colisoes) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    func(tabela, arquivo, total, sucesso, colisoes);
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

void processar_insercao(char tabela[TAM_TAB][TAM_PLACA], char *arquivo, int *total, int *sucesso, int *colisoes) {
    FILE *f = fopen(arquivo, "r");
    char placa[TAM_PLACA];
    while (fgets(placa, sizeof(placa), f)) {
        placa[strcspn(placa, "\r\n")] = '\0';
        if (strlen(placa) == 0) continue;
        (*total)++;
        if (inserir(tabela, placa, colisoes)) (*sucesso)++;
    }
    fclose(f);
}

void processar_busca(char tabela[TAM_TAB][TAM_PLACA], char *arquivo, int *total, int *sucesso, int *colisoes) {
    FILE *f = fopen(arquivo, "r");
    char placa[TAM_PLACA];
    while (fgets(placa, sizeof(placa), f)) {
        placa[strcspn(placa, "\r\n")] = '\0';
        if (strlen(placa) == 0) continue;
        (*total)++;
        if (buscar(tabela, placa)) (*sucesso)++;
    }
    fclose(f);
}

void processar_delecao(char tabela[TAM_TAB][TAM_PLACA], char *arquivo, int *total, int *sucesso, int *colisoes) {
    FILE *f = fopen(arquivo, "r");
    char placa[TAM_PLACA];
    while (fgets(placa, sizeof(placa), f)) {
        placa[strcspn(placa, "\r\n")] = '\0';
        if (strlen(placa) == 0) continue;
        (*total)++;
        if (deletar(tabela, placa)) (*sucesso)++;
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    char tabela[TAM_TAB][TAM_PLACA] = {0};
    int total = 0, sucesso = 0, colisoes = 0;
    double tempo = tempo_execucao(processar_insercao, tabela, argv[1], &total, &sucesso, &colisoes);
    printf("Inserção\nLinhas: %d, Inseridos: %d, Colisões: %d, Tempo: %.7f s\n", total, sucesso, colisoes, tempo);

    total = sucesso = colisoes = 0;
    tempo = tempo_execucao(processar_busca, tabela, argv[1], &total, &sucesso, &colisoes);
    printf("Busca\nLinhas: %d, Encontrados: %d, Tempo: %.7f s\n", total, sucesso, tempo);

    total = sucesso = colisoes = 0;
    tempo = tempo_execucao(processar_delecao, tabela, argv[1], &total, &sucesso, &colisoes);
    printf("Remoção\nLinhas: %d, Deletados: %d, Tempo: %.7f s\n", total, sucesso, tempo);

    return 0;
}
