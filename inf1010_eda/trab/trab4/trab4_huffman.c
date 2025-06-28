#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_ALFABETO 256

typedef struct NoHuffman {
    unsigned char caractere;
    unsigned int frequencia;
    struct NoHuffman *esquerda, *direita, *proximo;
} NoHuffman;

// Funcoes principais
void compactar(const char* arq_entrada, const char* arq_saida);
void descompactar(const char* arq_entrada, const char* arq_saida);

int main() {
    const char* arquivo_entrada = "irene.txt";
    const char* arquivo_compactado = "irene_compactado.bin";
    const char* arquivo_descompactado = "irene_descompactado.txt";

    printf("--- COMPACTACAO ---\n");
    compactar(arquivo_entrada, arquivo_compactado);
    
    printf("\n--- DESCOMPACTACAO ---\n");
    descompactar(arquivo_compactado, arquivo_descompactado);
}

// Aloca e inicializa novo no
NoHuffman* criar_no(unsigned char caractere, unsigned int frequencia) {
    NoHuffman* no = (NoHuffman*)malloc(sizeof(NoHuffman));
    no->caractere = caractere;
    no->frequencia = frequencia;
    no->esquerda = no->direita = no->proximo = NULL;
    return no;
}

// Insere no na lista
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

// Libera memoria
void liberar_arvore(NoHuffman* raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

// Constroi a arvore a partir da tabela de frequencias
NoHuffman* construir_arvore(unsigned int* tab_freq) {
    NoHuffman* lista = NULL;
    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            inserir_ordenado(&lista, criar_no((unsigned char)i, tab_freq[i]));
        }
    }
    
    printf("\nLista de prioridades:\n");
    NoHuffman* temp = lista;
    while(temp != NULL) {
        printf("No: '%c' Frequencia: %d\n", (temp->caractere == '\n' || temp->caractere == '\r' ? ' ' : temp->caractere), temp->frequencia);
        temp = temp->proximo;
    }
    printf("\n");

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

// Gera codigo percorrendo a arvore
void gerar_codigos_recursivo(NoHuffman* raiz, char** codigos, char* codigo_atual, int profundidade) {
    if (raiz == NULL) return;

    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        codigo_atual[profundidade] = '\0';
        codigos[raiz->caractere] = strdup(codigo_atual);
        return;
    }

    codigo_atual[profundidade] = '0';
    gerar_codigos_recursivo(raiz->esquerda, codigos, codigo_atual, profundidade + 1);
    
    codigo_atual[profundidade] = '1';
    gerar_codigos_recursivo(raiz->direita, codigos, codigo_atual, profundidade + 1);
}

// Compactacao e descompactacao

void compactar(const char* arq_entrada, const char* arq_saida) {
    // Conta frequencias
    unsigned int tab_freq[TAM_ALFABETO] = {0};
    FILE* entrada = fopen(arq_entrada, "r");
    if (!entrada) { perror("Erro ao abrir arquivo de entrada"); return; }
    
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        tab_freq[(unsigned char)c]++;
    }
    rewind(entrada);

    printf("Frequencia dos Caracteres:\n");
    for (int i=0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            printf("Caractere '%c' (ASCII %d): %d\n", (i == '\n' || i == '\r' ? ' ' : i), i, tab_freq[i]);
        }
    }

    // Constroi a arvore
    NoHuffman* raiz = construir_arvore(tab_freq);

    // Gera os codigos
    char* codigos[TAM_ALFABETO] = {NULL};
    char codigo_atual[100] = {0};
    gerar_codigos_recursivo(raiz, codigos, codigo_atual, 0);

    printf("Codigos gerados pra cada caractere:\n");
    for (int i=0; i < TAM_ALFABETO; i++) {
        if(codigos[i]) {
            printf("Caractere '%c': %s\n", (i == '\n' || i == '\r' ? ' ' : i), codigos[i]);
        }
    }

    // Escreve o arquivo compactado
    FILE* saida = fopen(arq_saida, "w");
    if (!saida) { perror("Erro ao abrir arquivo"); return; }

    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            fprintf(saida, "%d:%u\n", i, tab_freq[i]);
        }
    }
    fprintf(saida, "---\n");

    while ((c = fgetc(entrada)) != EOF) {
        fprintf(saida, "%s", codigos[(unsigned char)c]);
    }

    printf("\nArquivo '%s' compactado salvo\n", arq_saida);

    // Libera memoria
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
    for (int i = 0; i < TAM_ALFABETO; i++) if (codigos[i]) free(codigos[i]);
}

void descompactar(const char* arq_entrada, const char* arq_saida) {
    // Le cabecalho e reconstroi tabela de frequencia
    unsigned int tab_freq[TAM_ALFABETO] = {0};
    FILE* entrada = fopen(arq_entrada, "r");
    if (!entrada) { perror("Erro ao abrir arquivo compactado"); return; }
    
    int char_code;
    unsigned int freq;
    while (fscanf(entrada, "%d:%u\n", &char_code, &freq) == 2) {
        tab_freq[char_code] = freq;
    }
    char buffer[4];
    fgets(buffer, 4, entrada);
    printf("Tabela de frequencia lida\n");

    // Reconstroi a arvore
    NoHuffman* raiz = construir_arvore(tab_freq);
    printf("Arvore reconstruida com sucesso\n");

    FILE* saida = fopen(arq_saida, "w");
    if (!saida) { perror("Erro ao criar arquivo"); return; }

    printf("\nDecodificando e escrevendo no arquivo\n");
    NoHuffman* no_atual = raiz;
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        no_atual = (c == '0') ? no_atual->esquerda : no_atual->direita;

        if (no_atual->esquerda == NULL && no_atual->direita == NULL) {
            fputc(no_atual->caractere, saida);
            no_atual = raiz;
        }
    }

    printf("\nArquivo '%s' descompactado com sucesso\n", arq_saida);

    // Libera memoria
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}