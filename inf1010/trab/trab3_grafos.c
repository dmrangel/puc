#include <stdio.h>
#include <stdlib.h>

#define TAM 10

int grafo[TAM][TAM] = {
    {0,4,0,0,0,0,0,0,8,0},
    {4,0,0,8,0,0,0,0,11,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,8,0,0,7,0,4,0,0,2},
    {0,0,0,7,0,9,14,0,0,0},
    {0,0,0,0,9,0,10,0,0,0},
    {0,0,0,4,14,10,0,2,0,0},
    {0,0,0,0,0,0,2,0,1,6},
    {8,11,0,0,0,0,0,1,0,7},
    {0,0,0,2,0,0,0,6,7,0}
};

void busca_amplitude(int primeiro) {
    int visitado[TAM] = {0}, fila[TAM], inicio = 0, fim = 0;
    fila[fim++] = primeiro;
    visitado[primeiro] = 1;

    printf("Busca em amplitude: ");
    while (inicio < fim) {
        int atual = fila[inicio++];
        printf("%d ", atual);

        for (int i = 0; i < TAM; i++) {
            if (grafo[atual][i] && !visitado[i]) {
                fila[fim++] = i;
                visitado[i] = 1;
            }
        }
    }
    printf("\n");
}

void visita_profundidade(int v, int visitado[]) {
    visitado[v] = 1;
    printf("%d ", v);

    for (int i = 0; i < TAM; i++) {
        if (grafo[v][i] && !visitado[i]) {
            visita_profundidade(i, visitado);
        }
    }
}

void busca_profundidade(int primeiro) {
    int visitado[TAM] = {0};
    printf("Busca em profundidade: ");
    visita_profundidade(primeiro, visitado);
    printf("\n");
}

typedef struct {
    int origem, destino, peso;
} Aresta;

int compara(const void *a, const void *b) {
    return ((Aresta*)a)->peso - ((Aresta*)b)->peso;
}

int busca(int pai[], int i) {
    if (pai[i] != i)
        pai[i] = busca(pai, pai[i]);
    return pai[i];
}

void uniao(int pai[], int rank[], int x, int y) {
    int raiz_x = busca(pai, x);
    int raiz_y = busca(pai, y);

    if (rank[raiz_x] < rank[raiz_y])
        pai[raiz_x] = raiz_y;
    else if (rank[raiz_x] > rank[raiz_y])
        pai[raiz_y] = raiz_x;
    else {
        pai[raiz_y] = raiz_x;
        rank[raiz_x]++;
    }
}

void kruskal() {
    Aresta arestas[TAM * TAM];
    int num_arestas = 0;

    // Converter matriz para lista de arestas
    for (int i = 0; i < TAM; i++) {
        for (int j = i + 1; j < TAM; j++) {
            if (grafo[i][j]) {
                arestas[num_arestas++] = (Aresta){i, j, grafo[i][j]};
            }
        }
    }

    qsort(arestas, num_arestas, sizeof(Aresta), compara);

    int pai[TAM], rank[TAM];
    for (int i = 0; i < TAM; i++) {
        pai[i] = i;
        rank[i] = 0;
    }

    printf("Kruskal:\n");
    int count = 0, i = 0, total = 0;
    while (count < TAM - 1 && i < num_arestas) {
        Aresta a = arestas[i++];
        int x = busca(pai, a.origem);
        int y = busca(pai, a.destino);

        if (x != y) {
            printf("(%d - %d) peso: %d\n", a.origem, a.destino, a.peso);
            uniao(pai, rank, x, y);
            total += a.peso;
            count++;
        }
    }
    printf("Custo total: %d\n", total);
}

int main() {
    busca_amplitude(0);
    busca_profundidade(0);
    kruskal();
    return 0;
}
