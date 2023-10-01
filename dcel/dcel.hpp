struct HalfEdge;
struct Ponto;
struct Face;
struct DCEL;
#include <vector>

using namespace std;

struct Ponto{
    float x;
    float y;
    struct HalfEdge* aresta;
    char id;
};

struct HalfEdge{
    Ponto* origem;
    Ponto* destino;
    struct HalfEdge* twin;
    struct HalfEdge* next;
    struct HalfEdge* previous;    
    struct Face* face;
    char id[2];
    bool visited;
};


struct Face{
    struct HalfEdge* aresta;
    int id;
};


struct DCEL{
    vector <Ponto*> pontos;
    vector <HalfEdge*> arestas;
    vector <Face*> faces;
};

float angle_between_points(Ponto* a, Ponto* b, Ponto* c);

void connect(Ponto* origem, Ponto* destino, DCEL* dcel);
