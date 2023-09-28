#include "vetor.h"
#include "face.h"
#include "aresta.h"

typedef struct dcel{
    struct vetor* vetor;
    struct aresta* aresta;
    struct face* face;
}DCEL;