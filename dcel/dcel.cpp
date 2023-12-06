#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "dcel.hpp"
#include <math.h>

void create_ponto(double x, double y, DCEL* dcel){
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
double angle_between_points(Ponto* c, Ponto* a, Ponto* b){
    double x1 = b->x - a->x;
    double y1 = b->y - a->y;
    double x2 = c->x - a->x;
    double y2 = c->y - a->y;
    double dot = x1*x2 + y1*y2;
    double det = x1*y2 - y1*x2;
    double angle = atan2(det, dot);
    if (angle < 0) angle = angle + 2*M_PI;
    return angle;
}

HalfEdge* HalfEdge_to_link(HalfEdge* a, DCEL* dcel){
    HalfEdge* edge;
    double menor_angulo = 1000000.0;
     for (int i = 0; i < dcel->arestas.size(); i++){
        if (dcel->arestas[i]->destino != a->origem && dcel->arestas[i]->origem == a->destino){
            
            if (angle_between_points(a->origem, dcel->arestas[i]->origem, dcel->arestas[i]->destino) < menor_angulo){
                menor_angulo = angle_between_points(a->origem, dcel->arestas[i]->origem, dcel->arestas[i]->destino);
                edge = dcel->arestas[i];
            }
        }
        if (dcel->arestas[i]->twin->destino != a->origem && dcel->arestas[i]->twin->origem == a->destino){
            if (angle_between_points(a->origem, dcel->arestas[i]->twin->origem, dcel->arestas[i]->twin->destino) < menor_angulo){
                menor_angulo = angle_between_points(a->origem, dcel->arestas[i]->twin->origem, dcel->arestas[i]->twin->destino);
                edge = dcel->arestas[i]->twin;
            }
        }
        
    }
    if (menor_angulo == 1000000){
        edge = a;
    }
    return edge;
}

int existe_outro_ponto_muito_perto(Ponto* p, DCEL* dcel){
    for (Ponto* v : dcel->pontos) {
        if (abs (v->x - p->x) < 0.07 && abs(v->y - p->y)<0.07){
            return 1;
        }
    }
    return 0;
}

Ponto* ponto_mais_perto(double x, double y, DCEL* dcel){
    Ponto *ponto_perto = (Ponto*) malloc(sizeof(Ponto));
    double menor_distancia = 100000;
    for (Ponto* v : dcel->pontos) {
        if (sqrt((v->x - x) * (v->x - x)+(v->y - y)*(v->y - y))<menor_distancia){
            ponto_perto = v;
            menor_distancia = sqrt((v->x - x) * (v->x - x)+(v->y - y)*(v->y - y));
        }
    }
    return ponto_perto;
}

void corta_aresta(HalfEdge* aresta_atual, DCEL* dcel){
    //Identifica o sentido das arestas (anti-horário dita a origem)



    //Cria o ponto intermediário e seta em cada uma das arestas
    Ponto *aux_destino = aresta_atual->destino;
    double x_intermediario = (aresta_atual->origem->x + aresta_atual->destino->x)/2;
    double y_intermediario = (aresta_atual->origem->y + aresta_atual->destino->y)/2;
    Ponto *aux_intermediario = (Ponto*) malloc(sizeof(Ponto));
    aux_intermediario->x = x_intermediario;
    aux_intermediario->y = y_intermediario;


    //Cria a aresta e faz as ligações devidas
    HalfEdge* nova_aresta = (HalfEdge*) malloc(sizeof(HalfEdge));
    HalfEdge* twin_nova_aresta = (HalfEdge*) malloc(sizeof(HalfEdge));
    nova_aresta->origem = aux_intermediario;
    nova_aresta->destino = aresta_atual->destino;
    nova_aresta->twin = twin_nova_aresta;
    nova_aresta->next = aresta_atual->next;
    nova_aresta->previous = aresta_atual;
    nova_aresta->face = NULL;//aresta_atual->face;
    nova_aresta->visited = false;
    twin_nova_aresta->origem = aresta_atual->destino;
    twin_nova_aresta->destino = aux_intermediario;
    twin_nova_aresta->twin = nova_aresta;
    twin_nova_aresta->next = aresta_atual->twin;
    twin_nova_aresta->previous = aresta_atual->twin->previous;
    twin_nova_aresta->face = NULL;//aresta_atual->twin->face;
    twin_nova_aresta->visited = false;

    aresta_atual->next->previous = nova_aresta;
    aresta_atual->twin->previous->next = twin_nova_aresta;

    aresta_atual->destino = aux_intermediario;
    aresta_atual->next = nova_aresta;
    aresta_atual->twin->previous = twin_nova_aresta;
    aresta_atual->twin->origem = aux_intermediario;
    //Adiciona A aresta e o ponto na dcel
    dcel->arestas.push_back(nova_aresta);
    dcel->pontos.push_back(aux_intermediario);
    


}

void connect_orbit(HalfEdge* nova, HalfEdge* antiga){
    if (nova->origem == antiga->origem && nova->destino == antiga->destino){
        return;
    }
    nova->next = antiga; //100% certo
    nova->twin->previous = antiga->previous; //100% certo
    antiga->previous->next = nova->twin; //100% certo
    antiga->previous = nova; //100% certo


    //tentativa 3
    //nova->next = antiga;
    //nova->twin->previous = antiga->previous;
    //antiga->previous->next = nova->twin;
    //antiga->previous = nova;

    //tentativa 2
    //antiga->next = nova;
    //nova->twin->previous = antiga->previous;
    //antiga->previous->next = nova->twin;
    //antiga->previous = nova;

    //tentativa 1
    //antiga->previous = nova;
    //antiga->twin->next = nova->twin;
    //nova->next = antiga;
    //nova->twin->previous = antiga->twin;
}

void connect(Ponto* origem, Ponto* destino, DCEL* dcel){
    HalfEdge* nova_aresta = (HalfEdge*) malloc(sizeof(HalfEdge));
    HalfEdge* twin_nova_aresta = (HalfEdge*) malloc(sizeof(HalfEdge));
    nova_aresta->origem = origem;
    nova_aresta->destino = destino;
    nova_aresta->twin = twin_nova_aresta;
    nova_aresta->next = twin_nova_aresta;
    nova_aresta->previous = twin_nova_aresta;
    nova_aresta->face = NULL;
    nova_aresta->visited = false;
    twin_nova_aresta->origem = destino;
    twin_nova_aresta->destino = origem;
    twin_nova_aresta->twin = nova_aresta;
    twin_nova_aresta->next = nova_aresta;
    twin_nova_aresta->previous = nova_aresta;
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