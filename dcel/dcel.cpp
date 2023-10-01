#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "dcel.hpp"
#include <math.h>

void create_ponto(float x, float y, DCEL* dcel){
    Ponto* ponto = (Ponto*) malloc(sizeof(Ponto));
    ponto->x = x;
    ponto->y = y;
    ponto->aresta = NULL;
    dcel->pontos.push_back(ponto);
}

void create_face(DCEL* dcel){
    Face* face = (Face*) malloc(sizeof(Face));
    face->aresta = NULL;
    face->id = dcel->faces.size();
    dcel->faces.push_back(face);
}

/** 
 * Retorna angulo b(^a)c
 *
 */
float angle_between_points(Ponto* c, Ponto* a, Ponto* b){
    float x1 = b->x - a->x;
    float y1 = b->y - a->y;
    float x2 = c->x - a->x;
    float y2 = c->y - a->y;
    float dot = x1*x2 + y1*y2;
    float det = x1*y2 - y1*x2;
    float angle = atan2(det, dot);
    if (angle < 0) angle = abs(angle)+M_PI;
    return angle;
}

HalfEdge* HalfEdge_to_link(HalfEdge* a, DCEL* dcel){
    HalfEdge* edge;
    float menor_angulo = 1000000.0;
     for (int i = 0; i < dcel->arestas.size(); i++){
        if (dcel->arestas[i]->destino != a->origem && dcel->arestas[i]->origem == a->destino){
            printf("Angulo entre %c%c e %c%c eh  %f\n", 
                    a->origem->id, a->destino->id, dcel->arestas[i]->origem->id, dcel->arestas[i]->destino->id,
                    (angle_between_points(a->origem, dcel->arestas[i]->origem, dcel->arestas[i]->destino)*180)/M_PI
                    );
            if (angle_between_points(a->origem, dcel->arestas[i]->origem, dcel->arestas[i]->destino) < menor_angulo){
                printf("Angulo entre %c%c e %c%c eh  %f\n", 
                    a->origem->id, a->destino->id, dcel->arestas[i]->origem->id, dcel->arestas[i]->destino->id,
                    (angle_between_points(a->origem, dcel->arestas[i]->origem, dcel->arestas[i]->destino)*180)/M_PI
                    );
                menor_angulo = angle_between_points(a->origem, dcel->arestas[i]->origem, dcel->arestas[i]->destino);
                edge = dcel->arestas[i];
            }
        }
        if (dcel->arestas[i]->twin->destino != a->origem && dcel->arestas[i]->twin->origem == a->destino){
            if (angle_between_points(a->origem, dcel->arestas[i]->twin->origem, dcel->arestas[i]->twin->destino) < menor_angulo){
                printf("Angulo entre %c%c e %c%c eh  %f\n", 
                    a->origem->id, a->destino->id, dcel->arestas[i]->twin->origem->id, dcel->arestas[i]->twin->destino->id,
                    (angle_between_points(a->origem, dcel->arestas[i]->twin->origem, dcel->arestas[i]->twin->destino)*180)/M_PI
                    );
                menor_angulo = angle_between_points(a->origem, dcel->arestas[i]->twin->origem, dcel->arestas[i]->twin->destino);
                edge = dcel->arestas[i]->twin;
            }
        }
        
    }
    if (menor_angulo == 1000000){
        edge = a;
    }
    printf("Conectado a aresta %c%c na %c%c. Angulo = %f\n", 
    a->origem->id, a->destino->id, edge->origem->id, edge->destino->id,menor_angulo
    );
    return edge;
}

void connect_orbit(HalfEdge* nova, HalfEdge* antiga){
    antiga->previous = nova;
    antiga->twin->next = nova->twin;
    nova->next = antiga;
    nova->twin->previous = antiga->twin;
}

void connect(Ponto* origem, Ponto* destino, DCEL* dcel){
    HalfEdge* nova_aresta = (HalfEdge*) malloc(sizeof(HalfEdge));
    HalfEdge* twin_nova_aresta = (HalfEdge*) malloc(sizeof(HalfEdge));
    nova_aresta->origem = origem;
    nova_aresta->destino = destino;
    nova_aresta->twin = twin_nova_aresta;
    nova_aresta->next = nova_aresta;
    nova_aresta->previous = nova_aresta;
    nova_aresta->face = NULL;
    nova_aresta->visited = false;
    twin_nova_aresta->origem = destino;
    twin_nova_aresta->destino = origem;
    twin_nova_aresta->twin = nova_aresta;
    twin_nova_aresta->next = twin_nova_aresta;
    twin_nova_aresta->previous = twin_nova_aresta;
    twin_nova_aresta->face = NULL;
    twin_nova_aresta->visited = false;
    

    //Adiciona A aresta na dcel
    dcel->arestas.push_back(nova_aresta);

    //Identifica as arestas parra ligar
    HalfEdge* aresta3 = NULL;
    HalfEdge* aresta4 = NULL;
    aresta3 = HalfEdge_to_link(nova_aresta, dcel);
    aresta4 = HalfEdge_to_link(twin_nova_aresta, dcel);
    

    //Liga as arestas
    connect_orbit(nova_aresta, aresta3);
    connect_orbit(twin_nova_aresta,aresta4);

}
