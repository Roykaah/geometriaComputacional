#include "vetor.h"
#include "face.h"
typedef struct aresta{
    struct vetor origem;
    struct aresta* twin;
    struct aresta* next;
    struct aresta* previous;    
    struct face* face;
    bool visited;
}tAresta;