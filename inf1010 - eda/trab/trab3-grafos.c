#include <stdio.h>
#include <stdlib.h>

#define V 10

// --- ESTRUTURAS ---
struct Fila {
    int itens[V], frente, tras;
};
struct Aresta {
    int origem, destino, peso;
};
struct DSU {
    int pai[V];
};

int grafo[V][V] = {
    {0,4,0,0,0,0,0,0,8,0},{4,0,0,8,0,0,0,0,11,0},{0,0,0,0,0,0,0,0,0,0},
    {0,8,0,0,7,0,4,0,0,2},{0,0,0,7,0,9,14,0,0,0},{0,0,0,0,9,0,10,0,0,0},
    {0,0,0,4,14,10,0,2,0,0},{0,0,0,0,0,0,2,0,1,6},{8,11,0,0,0,0,0,1,0,7},
    {0,0,0,2,0,0,0,6,7,0}
};

// --- BUSCA EM PROFUNDIDADE (DFS) ---
void dfs_recursive(int u, int visitado[]) {
    visitado[u] = 1;
    printf("%d ", u);
    for (int v = 0; v < V; v++) {
        if (grafo[u][v] && !visitado[v]) {
            dfs_recursive(v, visitado);
        }
    }
}
void buscaEmProfundidade(int start) {
    int visitado[V] = {0};
    printf("Busca em Profundidade: ");
    dfs_recursive(start, visitado);
    for (int i = 0; i < V; i++) {
        if (!visitado[i]) dfs_recursive(i, visitado);
    }
    printf("\n");
}

// --- BUSCA EM AMPLITUDE (BFS) ---
void enfileirar(struct Fila* q, int valor) {
    if (q->frente == -1) q->frente = 0;
    q->tras++;
    q->itens[q->tras] = valor;
}
int desenfileirar(struct Fila* q) {
    int item = q->itens[q->frente];
    q->frente++;
    if (q->frente > q->tras) q->frente = q->tras = -1;
    return item;
}
void buscaEmAmplitude(int start) {
    int visitado[V] = {0};
    struct Fila q = {.frente = -1, .tras = -1};
    printf("Busca em Amplitude:   ");
    
    visitado[start] = 1;
    enfileirar(&q, start);
    
    while (q.tras != -1) {
        int u = desenfileirar(&q);
        printf("%d ", u);
        for (int v = 0; v < V; v++) {
            if (grafo[u][v] && !visitado[v]) {
                visitado[v] = 1;
                enfileirar(&q, v);
            }
        }
    }
    for (int i = 0; i < V; i++) {
        if (!visitado[i]) printf("%d ", i);
    }
    printf("\n");
}

// --- ALGORITMO DE KRUSKAL ---
int encontrar(struct DSU* dsu, int i) {
    if (dsu->pai[i] == i) return i;
    return dsu->pai[i] = encontrar(dsu, dsu->pai[i]);
}
void unir(struct DSU* dsu, int x, int y) {
    int raizX = encontrar(dsu, x);
    int raizY = encontrar(dsu, y);
    if (raizX != raizY) dsu->pai[raizX] = raizY;
}
int compararArestas(const void* a, const void* b) {
    return ((struct Aresta*)a)->peso > ((struct Aresta*)b)->peso;
}
void kruskalMST() {
    struct Aresta arestas[V * V];
    int numArestas = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i; j < V; j++) {
            if (grafo[i][j]) {
                arestas[numArestas++] = (struct Aresta){i, j, grafo[i][j]};
            }
        }
    }
    qsort(arestas, numArestas, sizeof(struct Aresta), compararArestas);
    
    struct DSU dsu;
    for (int i = 0; i < V; i++) dsu.pai[i] = i;
    
    printf("Kruskal (MST):\n");
    int custoTotal = 0;
    for (int i = 0; i < numArestas; i++) {
        int u = arestas[i].origem;
        int v = arestas[i].destino;
        if (encontrar(&dsu, u) != encontrar(&dsu, v)) {
            unir(&dsu, u, v);
            custoTotal += arestas[i].peso;
            printf("  Aresta: %d-%d, Peso: %d\n", u, v, arestas[i].peso);
        }
    }
    printf("  Custo Total: %d\n", custoTotal);
}

// --- FUNÇÃO PRINCIPAL ---
int main() {
    buscaEmProfundidade(0);
    printf("\n");
    buscaEmAmplitude(0);
    printf("\n");
    kruskalMST();
    return 0;
}