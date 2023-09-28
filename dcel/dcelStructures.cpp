#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "dcelStructures.hpp"

aresta* percorre_lista(listaAresta* lista, aresta* aresta){
    listaAresta* aux = lista;
    while(aux->aresta->origem != aresta->origem){
        aux = aux->prox;
    }
    return aux->aresta;
}
void insere_lista(listaAresta* lista, aresta* aresta){
    listaAresta* aux = lista;
    while(aux->prox != NULL){
        aux = aux->prox;
    }
    listaAresta* novo = (listaAresta*) malloc(sizeof(listaAresta));
    novo->aresta = aresta;
    novo->prox = NULL;
    aux->prox = novo;
}
void remove_lista(listaAresta* lista, aresta* aresta){
    listaAresta* aux = lista;
    while(aux->prox->aresta->origem != aresta->origem){
        aux = aux->prox;
    }
    listaAresta* aux2 = aux->prox;
    aux->prox = aux->prox->prox;
    free(aux2);
}

void print_lista(listaAresta* lista){
    listaAresta* aux = lista;
    while(aux != NULL){
        printf("%f %f\n", aux->aresta->origem->x, aux->aresta->origem->y);
        aux = aux->prox;
    }
}


void connect(vetor* origem, vetor* destino, dcel* dcel){
    aresta* aresta1 = (aresta*) malloc(sizeof(aresta));
    aresta* aresta2 = (aresta*) malloc(sizeof(aresta));
    aresta1->origem = origem;
    aresta1->end = destino;
    aresta1->twin = aresta2;
    aresta1->next = aresta1;
    aresta1->previous = aresta1;
    aresta1->face = NULL;
    aresta1->visited = false;
    aresta2->origem = destino;
    aresta2->end = origem;
    aresta2->twin = aresta1;
    aresta2->next = aresta2;
    aresta2->previous = aresta2;
    aresta2->face = NULL;
    aresta2->visited = false;
    if(dcel->aresta == NULL){
        dcel->aresta = aresta1;
    }
    else{
        aresta* aux = dcel->aresta;
        while(aux->next != dcel->aresta){
            aux = aux->next;
        }
        aux->next = aresta1;
        aresta1->previous = aux;
        aresta2->next = dcel->aresta;
        dcel->aresta->previous = aresta2;
    }
}