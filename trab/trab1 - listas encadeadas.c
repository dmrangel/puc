#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct paciente{
    int ordem;
    int prioridade;
    struct paciente* proximo;
} Paciente;

Paciente* inicia_lista(void){
    return NULL;
}

Paciente* cria_paciente(int ordem, int prioridade){
    Paciente* p = malloc(sizeof(Paciente));
    if (p == NULL) return NULL;
    p->ordem = ordem;
    p->prioridade = prioridade;
    p->proximo = NULL;
    return p;
}

Paciente* insere_paciente(Paciente* lista, Paciente* novo){
    if (lista == NULL) return novo;
    Paciente* atual = lista;
    if (novo->prioridade < lista->prioridade){
        novo->proximo = atual;
        return novo;
    }
    while(atual->proximo != NULL){
        if (novo->prioridade < atual->proximo->prioridade){
            novo->proximo = atual->proximo;
            atual->proximo = novo;
            return lista;
        }
        atual = atual->proximo;
    }
    atual->proximo = novo;
    return lista;
}

void exibe_lista(Paciente* lista){
    printf("================ Lista de Pacientes ===============\n");
    Paciente* atual = lista;
    int i = 0;
    char cprioridade[9];
    while(atual != NULL){
        if (atual->prioridade == 1) strcpy(cprioridade, "vermelha");
        else if (atual->prioridade == 2) strcpy(cprioridade, "amarela");
        else if (atual->prioridade == 3) strcpy(cprioridade, "verde");
        printf("Paciente %d: %d - %s\n", i, atual->ordem, cprioridade);
        atual = atual->proximo;
        i++;
    }
}

Paciente* remove_paciente(Paciente* lista, Paciente* p){
    if (lista == NULL){
        free(p);
        return NULL;
    }
    if (lista->ordem == p->ordem && lista->prioridade == p->prioridade) {
        Paciente* temp = lista;
        lista = lista->proximo;
        free(temp);
        free(p);
        return lista;
    }
    Paciente* atual = lista;
    while (atual->proximo != NULL){
        if (atual->proximo->ordem == p->ordem && atual->proximo->prioridade == p->prioridade){
            Paciente* temp = atual->proximo;
            atual->proximo = atual->proximo->proximo;
            free(temp);
            free(p);
            return lista;
        }
        atual = atual->proximo;
    }
    free(p);
    return lista;
}

int main(void){
    FILE* file = fopen("dados_trabalho1.txt", "r");
    if (file == NULL) return 1;
    int ordem, prioridade;
    char status;
    char cprioridade[9];
    Paciente* lista = inicia_lista();
    while (fscanf(file, " %c-%d-%s", &status, &ordem, cprioridade) != -1){
        if (strcmp(cprioridade, "verde") == 0) prioridade = 3;
        else if (strcmp(cprioridade, "amarela") == 0) prioridade = 2;
        else if (strcmp(cprioridade, "vermelha") == 0) prioridade = 1;
        Paciente* p = cria_paciente(ordem, prioridade);
        if (p == NULL) return 1;
        if (status == 'c'){
            lista = insere_paciente(lista, p);
        } else if (status == 'a'){
            lista = remove_paciente(lista, p);
        }
        exibe_lista(lista);
    }
    fclose(file);
    return 0;
}