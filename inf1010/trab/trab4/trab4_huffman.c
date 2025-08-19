#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_ALFABETO 256

typedef struct NoHuffman {
    unsigned char caractere;
    unsigned long frequencia;
    struct NoHuffman *esquerda, *direita, *proximo;
} NoHuffman;

void compactar(const char* arq_entrada, const char* arq_saida);
void descompactar(const char* arq_entrada, const char* arq_saida);
NoHuffman* criar_no(unsigned char caractere, unsigned long frequencia);
void inserir_ordenado(NoHuffman** lista, NoHuffman* no);
void liberar_arvore(NoHuffman* raiz);
NoHuffman* construir_arvore(unsigned long* tab_freq);
void gerar_codigos_recursivo(NoHuffman* raiz, char** codigos, char* codigo_atual, int profundidade);
void escrever_bit(FILE* saida, int bit, unsigned char* byte_buffer, int* bit_count);
void flush_bits(FILE* saida, unsigned char* byte_buffer, int* bit_count);
int ler_bit(FILE* entrada, unsigned char* byte_buffer, int* bit_count);
void escrever_arvore(NoHuffman* no, FILE* saida, unsigned char* byte_buffer, int* bit_count);
NoHuffman* reconstruir_arvore(FILE* entrada, unsigned char* byte_buffer, int* bit_count);


int main() {
    const char* arquivo_entrada = "EDA textos Huffman.txt";
    const char* arquivo_compactado = "arq_compactado.dat";
    const char* arquivo_descompactado = "arq_descompactado.txt";

    printf("--- COMPACTACAO ---\n");
    compactar(arquivo_entrada, arquivo_compactado);
    
    printf("\n--- DESCOMPACTACAO ---\n");
    descompactar(arquivo_compactado, arquivo_descompactado);

    return 0;
}

void escrever_bit(FILE* saida, int bit, unsigned char* byte_buffer, int* bit_count) {
    *byte_buffer <<= 1;
    if (bit) *byte_buffer |= 1;
    
    (*bit_count)++;
    if (*bit_count == 8) {
        fputc(*byte_buffer, saida);
        *bit_count = 0;
        *byte_buffer = 0;
    }
}

void flush_bits(FILE* saida, unsigned char* byte_buffer, int* bit_count) {
    if (*bit_count > 0) {
        *byte_buffer <<= (8 - *bit_count);
        fputc(*byte_buffer, saida);
    }
}

int ler_bit(FILE* entrada, unsigned char* byte_buffer, int* bit_count) {
    if (*bit_count == 0) {
        *byte_buffer = (unsigned char)fgetc(entrada);
        *bit_count = 8;
    }
    int bit = (*byte_buffer & 0x80) ? 1 : 0;
    *byte_buffer <<= 1;
    (*bit_count)--;
    return bit;
}

void escrever_arvore(NoHuffman* no, FILE* saida, unsigned char* byte_buffer, int* bit_count) {
    if (no->esquerda == NULL && no->direita == NULL) {
        escrever_bit(saida, 1, byte_buffer, bit_count);
        for (int i = 0; i < 8; i++) {
            escrever_bit(saida, (no->caractere >> (7 - i)) & 1, byte_buffer, bit_count);
        }
    } else {
        escrever_bit(saida, 0, byte_buffer, bit_count);
        escrever_arvore(no->esquerda, saida, byte_buffer, bit_count);
        escrever_arvore(no->direita, saida, byte_buffer, bit_count);
    }
}

NoHuffman* reconstruir_arvore(FILE* entrada, unsigned char* byte_buffer, int* bit_count) {
    int bit = ler_bit(entrada, byte_buffer, bit_count);

    if (bit == 1) {
        unsigned char caractere = 0;
        for (int i = 0; i < 8; i++) {
            caractere <<= 1;
            if (ler_bit(entrada, byte_buffer, bit_count)) {
                caractere |= 1;
            }
        }
        return criar_no(caractere, 0);
    } else {
        NoHuffman* no = criar_no(0, 0);
        no->esquerda = reconstruir_arvore(entrada, byte_buffer, bit_count);
        no->direita = reconstruir_arvore(entrada, byte_buffer, bit_count);
        return no;
    }
}

void compactar(const char* arq_entrada, const char* arq_saida) {
    // Conta frequencias
    unsigned long tab_freq[TAM_ALFABETO] = {0};
    unsigned long total_caracteres = 0;
    FILE* entrada = fopen(arq_entrada, "r");
    if (!entrada) {
        perror("Erro ao abrir arquivo de entrada");
        return;
    }
    
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        tab_freq[(unsigned char)c]++;
        total_caracteres++;
    }
    rewind(entrada);

    printf("Frequencia dos Caracteres:\n");
    for (int i=0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            if (i == '\n') printf("Caractere '\\n' (ASCII %d): %lu\n", i, tab_freq[i]);
            else if (i == '\r') printf("Caractere '\\r' (ASCII %d): %lu\n", i, tab_freq[i]);
            else printf("Caractere '%c' (ASCII %d): %lu\n", i, i, tab_freq[i]);
        }
    }

    // Constroi a arvore
    NoHuffman* raiz = construir_arvore(tab_freq);
    if (!raiz) {
        fclose(entrada);
        FILE* saida = fopen(arq_saida, "wb");
        if (saida) fclose(saida);
        return;
    }
    printf("\nArvore construida com sucesso.\n");

    // Gera os codigos
    char* codigos[TAM_ALFABETO] = {0};
    char codigo_atual[100] = {0};
    gerar_codigos_recursivo(raiz, codigos, codigo_atual, 0);

    printf("\nCodigos gerados pra cada caractere:\n");
    for (int i=0; i < TAM_ALFABETO; i++) {
        if(codigos[i]) {
            if (i == '\n') printf("Caractere '\\n': %s\n", codigos[i]);
            else if (i == '\r') printf("Caractere '\\r': %s\n", codigos[i]);
            else printf("Caractere '%c': %s\n", i, codigos[i]);
        }
    }

    // Escreve o arquivo compactado em binario
    FILE* saida = fopen(arq_saida, "wb");
    if (!saida) {
        perror("Erro ao abrir arquivo de saida");
        fclose(entrada);
        liberar_arvore(raiz);
        return;
    }

    fwrite(&total_caracteres, sizeof(unsigned long), 1, saida);
    
    unsigned char byte_buffer = 0;
    int bit_count = 0;
    escrever_arvore(raiz, saida, &byte_buffer, &bit_count);
    flush_bits(saida, &byte_buffer, &bit_count);
    bit_count = 0;
    byte_buffer = 0;
    fclose(entrada);
    entrada = fopen(arq_entrada, "r");
    if (!entrada) {
        perror("Erro ao reabrir arquivo");
        return;
    }
    while ((c = fgetc(entrada)) != EOF) {
        char* codigo = codigos[(unsigned char)c];
        for (int i = 0; codigo[i] != '\0'; i++) {
            escrever_bit(saida, codigo[i] - '0', &byte_buffer, &bit_count);
        }
    }
    flush_bits(saida, &byte_buffer, &bit_count);

    // Libera memoria
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
    for (int i = 0; i < TAM_ALFABETO; i++) if (codigos[i]) free(codigos[i]);
}

void descompactar(const char* arq_entrada, const char* arq_saida) {
    // Le cabecalho
    FILE* entrada = fopen(arq_entrada, "rb");
    if (!entrada) {
        perror("Erro ao abrir arquivo compactado");
        return;
    }
    
    unsigned long total_caracteres = 0;
    fread(&total_caracteres, sizeof(unsigned long), 1, entrada);
    if (total_caracteres == 0) {
        fclose(entrada);
        FILE* saida = fopen(arq_saida, "w");
        if(saida) fclose(saida);
        printf("Arquivo descompactado vazio criado.\n");
        return;
    }
    
    // Reconstroi a arvore
    unsigned char byte_buffer = 0;
    int bit_count = 0;
    NoHuffman* raiz = reconstruir_arvore(entrada, &byte_buffer, &bit_count);
    if (!raiz) {
        fclose(entrada);
        return;
    }
    printf("Arvore reconstruida com sucesso\n");
    bit_count = 0;

    // Decodifica o arquivo binario
    FILE* saida = fopen(arq_saida, "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saida");
        fclose(entrada);
        liberar_arvore(raiz);
        return;
    }
    
    NoHuffman* no_atual = raiz;
    for (unsigned long i = 0; i < total_caracteres; i++) {
        while (no_atual->esquerda != NULL || no_atual->direita != NULL) {
            int bit = ler_bit(entrada, &byte_buffer, &bit_count);
            no_atual = (bit == 0) ? no_atual->esquerda : no_atual->direita;
        }
        fputc(no_atual->caractere, saida);
        no_atual = raiz;
    }

    printf("\nArquivo '%s' descompactado\n", arq_saida);

    // Libera memoria
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}

// --- DEFINIÇÕES DAS FUNÇÕES RESTANTES ---
NoHuffman* criar_no(unsigned char caractere, unsigned long frequencia) {
    NoHuffman* no = (NoHuffman*)malloc(sizeof(NoHuffman));
    if (no == NULL) {
        perror("Erro na alocacao de memoria");
        exit(EXIT_FAILURE);
    }
    no->caractere = caractere;
    no->frequencia = frequencia;
    no->esquerda = no->direita = no->proximo = NULL;
    return no;
}

void inserir_ordenado(NoHuffman** lista, NoHuffman* no) {
    if (*lista == NULL || (*lista)->frequencia >= no->frequencia) {
        no->proximo = *lista;
        *lista = no;
    } else {
        NoHuffman* atual = *lista;
        while (atual->proximo != NULL && atual->proximo->frequencia < no->frequencia) {
            atual = atual->proximo;
        }
        no->proximo = atual->proximo;
        atual->proximo = no;
    }
}

void liberar_arvore(NoHuffman* raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

NoHuffman* construir_arvore(unsigned long* tab_freq) {
    NoHuffman* lista = NULL;
    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            inserir_ordenado(&lista, criar_no((unsigned char)i, tab_freq[i]));
        }
    }
    
    printf("\nLista de prioridades:\n");
    NoHuffman* temp = lista;
    while(temp != NULL) {
        if (temp->caractere == '\n') printf("No: '\\n' Frequencia: %lu\n", temp->frequencia);
        else if (temp->caractere == '\r') printf("No: '\\r' Frequencia: %lu\n", temp->frequencia);
        else printf("No: '%c' Frequencia: %lu\n", temp->caractere, temp->frequencia);
        temp = temp->proximo;
    }

    if (lista != NULL && lista->proximo == NULL) {
        NoHuffman* esquerdo = lista;
        lista = NULL;
        NoHuffman* pai = criar_no(0, esquerdo->frequencia);
        pai->esquerda = esquerdo;
        return pai;
    }

    while (lista != NULL && lista->proximo != NULL) {
        NoHuffman* esquerdo = lista;
        lista = lista->proximo;
        NoHuffman* direito = lista;
        lista = lista->proximo;
        NoHuffman* pai = criar_no(0, esquerdo->frequencia + direito->frequencia);
        pai->esquerda = esquerdo;
        pai->direita = direito;
        inserir_ordenado(&lista, pai);
    }
    return lista;
}

void gerar_codigos_recursivo(NoHuffman* raiz, char** codigos, char* codigo_atual, int profundidade) {
    if (raiz == NULL) return;

    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        if(profundidade == 0) {
            strcpy(codigo_atual, "0");
            profundidade = 1;
        }
        codigo_atual[profundidade] = '\0';
        codigos[raiz->caractere] = strdup(codigo_atual);
        return;
    }

    codigo_atual[profundidade] = '0';
    gerar_codigos_recursivo(raiz->esquerda, codigos, codigo_atual, profundidade + 1);
    codigo_atual[profundidade] = '1';
    gerar_codigos_recursivo(raiz->direita, codigos, codigo_atual, profundidade + 1);
}