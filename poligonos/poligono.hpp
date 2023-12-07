struct HalfEdge;
struct Ponto;
struct Face;
struct DCEL;
#include <vector>

using namespace std;

struct Ponto{
    double x;
    double y;
    int id;
    char tipo;//e=entrando, s=saindo; usado só pra intersecção
    double distancia_ao_ponto_de_referencia;
    int visitado;
};

struct poligono{
    vector <Ponto*> polig;
    vector<vector <Ponto*>> buracos;

};


vector<poligono*> interseccao_entre_poligonos(poligono* p1, poligono* p2,char operacao);
vector<Ponto*> reoordena(vector<Ponto*> pontos,int ordem);
