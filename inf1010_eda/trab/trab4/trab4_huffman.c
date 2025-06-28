#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_ALFABETO 256

// Estrutura para o nó da árvore de Huffman e da lista encadeada
typedef struct NoHuffman {
    unsigned char caractere;
    unsigned int frequencia;
    struct NoHuffman *esquerda, *direita, *proximo;
} NoHuffman;

// --- Funções Principais ---
void compactar(const char* arq_entrada, const char* arq_saida);
void descompactar(const char* arq_entrada, const char* arq_saida);

int main() {
    const char* arquivo_entrada = "irene.txt";
    const char* arquivo_compactado = "irene_compactado.bin";
    const char* arquivo_descompactado = "irene_descompactado.txt";

    // Lembre-se de criar o arquivo "irene.txt" antes de executar.
    printf("--- INICIANDO PROCESSO DE COMPACTAÇÃO ---\n");
    compactar(arquivo_entrada, arquivo_compactado);
    
    printf("\n--- INICIANDO PROCESSO DE DESCOMPACTAÇÃO ---\n");
    descompactar(arquivo_compactado, arquivo_descompactado);
    
    printf("\nProcesso concluído.\n");
    return 0;
}

// --- Funções Auxiliares ---

// Aloca e inicializa um novo nó
NoHuffman* criar_no(unsigned char caractere, unsigned int frequencia) {
    NoHuffman* no = (NoHuffman*)malloc(sizeof(NoHuffman));
    no->caractere = caractere;
    no->frequencia = frequencia;
    no->esquerda = no->direita = no->proximo = NULL;
    return no;
}

// Insere um nó em uma lista, mantendo-a ordenada pela frequência
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

// Libera a memória da árvore recursivamente
void liberar_arvore(NoHuffman* raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

// Constrói a árvore de Huffman a partir da tabela de frequências
NoHuffman* construir_arvore(unsigned int* tab_freq) {
    NoHuffman* lista = NULL;
    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            inserir_ordenado(&lista, criar_no((unsigned char)i, tab_freq[i]));
        }
    }
    
    printf("\nPASSO 2: Lista de prioridades inicial (baseada na frequência)\n");
    NoHuffman* temp = lista;
    while(temp != NULL) {
        printf("Nó['%c', %d] ", temp->caractere == '\n' ? ' ' : temp->caractere, temp->frequencia);
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

// Gera os códigos de Huffman percorrendo a árvore
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

// --- Implementação da Compactação e Descompactação ---

void compactar(const char* arq_entrada, const char* arq_saida) {
    // 1. Contar frequências
    unsigned int tab_freq[TAM_ALFABETO] = {0};
    FILE* entrada = fopen(arq_entrada, "r");
    if (!entrada) { perror("Erro ao abrir arquivo de entrada"); return; }
    
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        tab_freq[(unsigned char)c]++;
    }
    rewind(entrada);

    printf("PASSO 1: Tabela de Frequência dos Caracteres\n");
    for (int i=0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            // Imprime o caractere de forma legível
            printf("Caractere '%c' (ASCII %d): %d\n", (i == '\n' ? ' ' : i), i, tab_freq[i]);
        }
    }

    // 2. Construir a árvore de Huffman
    NoHuffman* raiz = construir_arvore(tab_freq);
    printf("\nPASSO 3: Árvore de Huffman foi construída com sucesso.\n");

    // 3. Gerar os códigos
    char* codigos[TAM_ALFABETO] = {NULL};
    char codigo_atual[100];
    gerar_codigos_recursivo(raiz, codigos, codigo_atual, 0);

    printf("\nPASSO 4: Códigos de Huffman gerados para cada caractere\n");
    for (int i=0; i < TAM_ALFABETO; i++) {
        if(codigos[i]) {
            printf("Caractere '%c': %s\n", (i == '\n' ? ' ' : i), codigos[i]);
        }
    }

    // 4. Escrever o arquivo compactado (cabeçalho + dados)
    FILE* saida = fopen(arq_saida, "w");
    if (!saida) { perror("Erro ao abrir arquivo de saída"); return; }

    for (int i = 0; i < TAM_ALFABETO; i++) {
        if (tab_freq[i] > 0) {
            fprintf(saida, "%d:%u\n", i, tab_freq[i]);
        }
    }
    fprintf(saida, "---\n");

    while ((c = fgetc(entrada)) != EOF) {
        fprintf(saida, "%s", codigos[(unsigned char)c]);
    }

    printf("\nPASSO 5: Arquivo '%s' salvo com o conteúdo compactado.\n", arq_saida);

    // 5. Liberar memória
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
    for (int i = 0; i < TAM_ALFABETO; i++) if (codigos[i]) free(codigos[i]);
}

void descompactar(const char* arq_entrada, const char* arq_saida) {
    // 1. Ler cabeçalho e reconstruir tabela de frequência
    unsigned int tab_freq[TAM_ALFABETO] = {0};
    FILE* entrada = fopen(arq_entrada, "r");
    if (!entrada) { perror("Erro ao abrir arquivo compactado"); return; }
    
    int char_code;
    unsigned int freq;
    while (fscanf(entrada, "%d:%u\n", &char_code, &freq) == 2) {
        tab_freq[char_code] = freq;
    }
    char buffer[4];
    fgets(buffer, 4, entrada); // Consumir o separador "---"
    printf("PASSO 1: Tabela de Frequência lida do arquivo compactado.\n");

    // 2. Reconstruir a árvore de Huffman
    NoHuffman* raiz = construir_arvore(tab_freq);
    printf("\nPASSO 2: Árvore de Huffman reconstruída com sucesso.\n");

    // 3. Decodificar o conteúdo
    FILE* saida = fopen(arq_saida, "w");
    if (!saida) { perror("Erro ao criar arquivo de saída"); return; }

    printf("\nPASSO 3: Decodificando o conteúdo e escrevendo no arquivo de saída...\n");
    NoHuffman* no_atual = raiz;
    int c;
    while ((c = fgetc(entrada)) != EOF) {
        no_atual = (c == '0') ? no_atual->esquerda : no_atual->direita;

        if (no_atual->esquerda == NULL && no_atual->direita == NULL) {
            fputc(no_atual->caractere, saida);
            no_atual = raiz;
        }
    }

    printf("\nPASSO 4: Arquivo '%s' descompactado com sucesso.\n", arq_saida);

    // 4. Liberar memória
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}