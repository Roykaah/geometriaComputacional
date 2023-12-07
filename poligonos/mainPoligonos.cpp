
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "poligono.hpp"
#include <math.h>
#include "../lib/utils.h"

poligono p1;
poligono p2;

/* Globals */
/** Window width. */
int win_width = 800;
/** Window height. */
int win_height = 600;


Ponto* ponto_anterior;
Ponto* ponto_inicial;
Ponto* ponto_central_planarizacao;
Ponto* ponto2;

int id_pontos = 0;
int poligono_selecionado = 1;
vector<Ponto*> pontos_interseccao;
vector<poligono*> poligonos_interseccao;
int qtd_buracos1 = 0;
int qtd_buracos2 = 0;

int modo_de_impressao = 'p';
char modo_edicao = 'p'; //p = poligono, b = buraco


void imprime_pontos2(vector<Ponto*> pontos){
    for (Ponto* v : pontos) {
        printf("Ponto %d: (%lf, %lf)\n", v->id, v->x, v->y);
    }
}

void printa_arestas(vector<Ponto*> pontos){
    glBegin(GL_LINES);
            for (int i = 0; i < pontos.size()-1; i++) {
                glVertex2f(pontos[i]->x, pontos[i]->y);
                glVertex2f(pontos[i+1]->x, pontos[i+1]->y);
           }
            glEnd();
}



void printa_poligono(poligono p, double r, double g, double b){
   if (!p.polig.empty()) {
    
        glColor3f(r, g , b); 

        glPointSize(5.0);
        glBegin(GL_POINTS);
        for (Ponto* v : p.polig) {
            glVertex2f(v->x, v->y);
        }
        //Printa os pontos do poligono interno
        if (!p.buracos.empty()) {
            for(vector<Ponto*> buraco : p.buracos)
                for (Ponto* v : buraco) {
                    glVertex2f(v->x, v->y);
                }
        }
        glEnd();

        glColor3f(r, g , b); 
        printa_arestas(p.polig);
        if (!p.buracos.empty()) {
            for(vector<Ponto*> buraco : p.buracos)
                if (!buraco.empty())
                    printa_arestas(buraco);
        }

    }

    
}
void printa_interseccoes(){
    glColor3f(1.0, 0.0 , 0.0); 
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (Ponto* v : pontos_interseccao) {
        glVertex2f(v->x, v->y);
    }
    glEnd();
    
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            poligono_selecionado = 1;
            modo_de_impressao = 'p';

            break;
        case '2':
            poligono_selecionado = 2;
            modo_de_impressao = 'p';

            break;  
        case 'i':
            poligonos_interseccao = interseccao_entre_poligonos(&p1, &p2);
            modo_de_impressao = 'i';
            break;
        case 'f':
            //fim do poligono
            if (poligono_selecionado == 1) {
                if (modo_edicao == 'p') {
                    p1.polig = reoordena(p1.polig,1);
                    p1.polig.push_back(p1.polig.front());
                }
                else if (modo_edicao == 'b') {                    
                    p1.buracos[qtd_buracos1] = reoordena(p1.buracos[qtd_buracos1],0);
                    p1.buracos[qtd_buracos1].push_back(p1.buracos[qtd_buracos1].front());
                    qtd_buracos1++;
                    p1.buracos.push_back(vector<Ponto*>());
                }
            }
            else {
                if (modo_edicao == 'p'){
                    p2.polig = reoordena(p2.polig,1);
                    p2.polig.push_back(p2.polig.front());
                }
                else if (modo_edicao == 'b') {
                    p2.buracos[qtd_buracos2] = reoordena(p2.buracos[qtd_buracos2],0);
                    p2.buracos[qtd_buracos2].push_back(p2.buracos[qtd_buracos2].front());
                    qtd_buracos2++;
                    p2.buracos.push_back(vector<Ponto*>());
                }
            }
            modo_de_impressao = 'p';

            break;
    }
    glutPostRedisplay();

}
void mouse(int button, int state, int x, int y)
{
        Ponto *novo_ponto = (Ponto*) malloc(sizeof(Ponto));
        
		if (state == GLUT_DOWN){
            novo_ponto->x = (double(x)*2/win_width -1);
            novo_ponto->y = -(double(y)*2/win_height -1);
            novo_ponto->id = id_pontos;
            id_pontos++;
            switch (button)
            {
                    case GLUT_LEFT_BUTTON:
                        if (poligono_selecionado == 1) p1.polig.push_back(novo_ponto);
                        else p2.polig.push_back(novo_ponto);
                        modo_edicao = 'p';
                        break;
                    case GLUT_RIGHT_BUTTON:
                        modo_edicao = 'b';
                        if (poligono_selecionado == 1) p1.buracos[qtd_buracos1].push_back(novo_ponto);
                        else p2.buracos[qtd_buracos2].push_back(novo_ponto);    
                        break;
                    case GLUT_MIDDLE_BUTTON:
                        break;
            }
        }
    ponto_anterior = novo_ponto;
	glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (modo_de_impressao == 'p'){
        printa_poligono(p1, 0.0, 0.0, 1.0);
        printa_poligono(p2, 0.0, 1.0, 0.0);
    }
    else if (modo_de_impressao == 'i'){
        for (poligono* p : poligonos_interseccao){
            printf("Poligono interseccao:\n");
            imprime_pontos2(p->polig);
            printa_poligono(*p, 1.0, 0.0, 0.0);
        }
    }
    glFlush();

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("POLIGONOS");
    
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(keyboard);
    
    p1.buracos.push_back(vector<Ponto*>());
    p2.buracos.push_back(vector<Ponto*>());

    glutMainLoop();

    for (Ponto* v : p1.polig)
        delete v;
    for (vector<Ponto*> pv : p1.buracos){
        for (Ponto* v : pv)
            delete v;
    }
    
    return 0;
}







