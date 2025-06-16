/*
Nome: Davi Rangel
Turma: 3WB
Matrícula: 2411930
*/

#include <stdio.h>
#include <stdlib.h>


//1:
struct lista {
	int n;
	struct lista* prox;
};
typedef struct lista Lista;

Lista* lst_cria(void) {
	return NULL;
}

Lista* lst_insere(Lista* lst, int num) {
	Lista* novo = (Lista*)malloc(sizeof(Lista));
    if (novo == NULL) return NULL;
	novo->n = num;
	novo->prox = NULL;
    if (lst == NULL || num < lst->n) {
        novo->prox = lst;
        return novo;
    }
    Lista* atual = lst;
    while (atual->prox != NULL && atual->prox->n < num) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
	return lst;
}

Lista* lst_intercala(Lista* lst1, Lista* lst2){
    Lista* lst3 = lst_cria();
    while(lst2 != NULL){
        if (lst1 == NULL) {
            lst3 = lst_insere(lst3, lst2->n);
            lst2 = lst2->prox;
        }
        else if (lst1-> n < lst2->n) {
            lst3 = lst_insere(lst3, lst1->n);
            lst1 = lst1->prox;
        } else {
            lst3 = lst_insere(lst3, lst2->n);
            lst2 = lst2->prox;
        }
    }
    return lst3;
}

void num1(void){
    Lista* lstPar = lst_cria();
	Lista* lstImpar = lst_cria();
    int pares[10] = {10, 20, 30, 34, 36, 38, 50, 60, 62, 70};
    int impares[10] = { 9, 11, 13, 33, 35, 41, 43, 55, 61, 71};
    printf("Número 1:\nLista de pares: ");
    for (int i = 0; i < 10; i++) {
        lstPar = lst_insere(lstPar, pares[i]);
        printf("%d ", pares[i]);
    }
    printf("\nLista de ímpares: ");
    for (int i = 0; i < 10; i++) {
        lstImpar = lst_insere(lstImpar, impares[i]);
        printf("%d ", impares[i]);
    }
    Lista* lstIntercalada = lst_intercala(lstPar, lstImpar);
    printf("\nLista intercalada: ");
    for (Lista* p = lstIntercalada; p != NULL; p = p->prox) {
        printf("%d ", p->n);
    }
}

//2:
void exibe_vetor(int vetor[], int len, int i){
    if (i >= len) return;
    printf("%d ", vetor[i]);
    exibe_vetor(vetor, len, i+1);
}

void intercala_vetor(int vetor1[], int vetor2[], int len, int i_vetor1, int i_vetor2){
    if (i_vetor1 >= len) {
        printf("%d ", vetor2[i_vetor2]);
        return;
    }
    if (i_vetor1 >= len && i_vetor2 >= len) return;
    if (vetor1[i_vetor1] <= vetor2[i_vetor2]) {
        printf("%d ", vetor1[i_vetor1]);
        intercala_vetor(vetor1, vetor2, len, i_vetor1+1, i_vetor2);
    }
    else if (vetor1[i_vetor1] >= vetor2[i_vetor2]) {
        printf("%d ", vetor2[i_vetor2]);
        intercala_vetor(vetor1, vetor2, len, i_vetor1, i_vetor2+1);
    }
}

void num2(void){
    int pares[10] = {10, 20, 30, 34, 36, 38, 50, 60, 62, 70};
    int impares[10] = { 9, 11, 13, 33, 35, 41, 43, 55, 61, 71};
    printf("\nNúmero 2:\nVetor de pares: ");
    exibe_vetor(pares, 10, 0);
    printf("\nVetor de ímpares: ");
    exibe_vetor(impares, 10, 0);
    printf("\nVetor intercalado: ");
    intercala_vetor(pares, impares, 10, 0, 0);
}

int main(void) {
	num1();
    num2();
	return 0;
}