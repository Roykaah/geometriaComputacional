struct HalfEdge;
struct Ponto;
struct Face;
struct DCEL;
#include <vector>

using namespace std;

struct Ponto{
    double x;
    double y;
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

double angle_between_points(Ponto* a, Ponto* b, Ponto* c);

void connect(Ponto* origem, Ponto* destino, DCEL* dcel);

void corta_aresta(HalfEdge* aresta_atual, DCEL* dcel);

int existe_outro_ponto_muito_perto(Ponto* p, DCEL* dcel);

Ponto* ponto_mais_perto(double x, double y, DCEL* dcel);