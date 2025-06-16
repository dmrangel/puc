#include <stdio.h>
#include "Trabalho2_DaviRangel_2411930.h"

int main(void){
    //1
    //item a
    Nodo* a_n10 = itemA();
    //item b
    Nodo* b_n10 = itemA();
    deleta(1, &b_n10);
    //item c
    Nodo* c_n10 = itemA();
    deleta(22, &c_n10);
    insere(c_n10, 17);
    //item d
    printf("[Item A]\nPré-Ordem:\n");
    exibe_preordem(a_n10);
    printf("\nSimétrica:\n");
    exibe_simetrica(a_n10);
    printf("\n[Item B]\nPré-Ordem:\n");
    exibe_preordem(b_n10);
    printf("\nSimétrica:\n");
    exibe_simetrica(b_n10);
    printf("\n[Item C]\nPré-Ordem:\n");
    exibe_preordem(c_n10);
    printf("\nSimétrica:\n");
    exibe_simetrica(c_n10);
    //2 
    int valores_nivel[] = {10, 5, 20, 1, 7, 15, 21, 6, 8, 11, 22};
    int tam = sizeof(valores_nivel) / sizeof(valores_nivel[0]);
    Nodo* n10 = NULL;
    n10 = cria_por_fila(valores_nivel, tam);
    printf("\n[2]\nPré-Ordem:\n");
    exibe_preordem(n10);
    printf("\nSimétrica:\n");
    exibe_simetrica(n10);
    //3
    printf("\n[3] Verifica ABB");
    Nodo* listaArvores[3] = {a_n10, b_n10, c_n10};
    char nomeItem[3] = {'A', 'B', 'C'};
    for (int i = 0; i < 3; i++){
        if (verificaABB(listaArvores[i])){
            printf("\nA árvore do item %c é uma ABB", nomeItem[i]);
        } else {
            printf("\nA árvore do item %c não é uma ABB", nomeItem[i]);
        }
    }
    //4
    printf("\n\n[4] Verifica AVL");
    for (int i = 0; i < 3; i++){
        if (verificaAVL(listaArvores[i])){
            printf("\nA árvore do item %c é uma AVL", nomeItem[i]);
        } else {
            printf("\nA árvore do item %c não é uma AVL", nomeItem[i]);
        }
    }
    printf("\n");
    return 0;
}