struct aresta;
struct vetor;
struct face;
struct dcel;
#include <vector>

struct vetor{
    float x;
    float y;
    struct aresta* aresta;
};

struct aresta{
    vetor* origem;
    vetor* end;
    struct aresta* twin;
    struct aresta* next;
    struct aresta* previous;    
    struct face* face;
    bool visited;
};


struct face{
    struct aresta* aresta;
    int id;
};


struct dcel{
    struct vetor* vetor;
    struct aresta* aresta;
    struct face* face;
};

struct listaAresta{
    struct aresta* aresta;
    struct listaAresta* prox;
};

aresta* percorre_lista(listaAresta* lista, aresta* aresta);
void insere_lista(listaAresta* lista, aresta* aresta);
void remove_lista(listaAresta* lista, aresta* aresta);
void print_lista(listaAresta* lista);


void connect(vetor* origem, vetor* destino, dcel* dcel);
//aresta criaAresta(vetor origem, vetor destino, aresta* twin, aresta* next, aresta* previous, face* face, bool visited);

/*
class vertex
{
  public:
    int key;
    float x, y;
};

class half_edge
{
  public:
    int origin_v, end_v;
    class vertex *origin, *end;
    class half_edge *twin;
};

class face
{
  public:
    int key = -1;
};

class vertex_table
{
  public:
    class vertex *v;
    class half_edge *e;
};

class face_table
{
  public:
    class face *face; //= NULL;
    half_edge * inner_components;
    class half_edge *outer_component; //= NULL;
    float area = -1;
};

class half_edge_table
{
  public:
    class half_edge *half_edge, *next, *prev;
    class face *incident_face; //= NULL;
};

#define INF 10000

struct Point
{
    float x;
    float y;
};*/

