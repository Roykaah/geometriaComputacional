
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "dcel.hpp"
#include <math.h>

DCEL dcel;
HalfEdge* halfEdge_atual;

/* Globals */
/** Window width. */
int win_width = 800;
/** Window height. */
int win_height = 600;


Ponto* ponto_anterior;
Ponto* ponto_inicial;
Ponto* ponto_central_planarizacao;
Ponto* ponto2;

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
            halfEdge_atual = halfEdge_atual->twin;
            break;
        case 'n':
            halfEdge_atual = halfEdge_atual->next;
            break;
        case 'p':
            halfEdge_atual = halfEdge_atual->previous;
            break;
        case 'c':
            corta_aresta(halfEdge_atual, &dcel);
            break;
        case 'f':
            connect(ponto_inicial,ponto_anterior,&dcel);
            break;
        case 'o':
            halfEdge_atual = halfEdge_atual->twin->next;
            break;
    }
    glutPostRedisplay();
}
int modo_criacao = 1;
void mouse(int button, int state, int x, int y)
{
        Ponto *novo_ponto = (Ponto*) malloc(sizeof(Ponto));
		if (state == GLUT_DOWN){
            switch (button)
            {
                    case GLUT_LEFT_BUTTON:
                            
                            if (modo_criacao == 1){
                                novo_ponto = ponto_mais_perto((float(x)*2/win_width -1), 
                                                                -(float(y)*2/win_height -1), 
                                                                &dcel);
                                connect(novo_ponto,ponto_anterior,&dcel);  
                                ponto_anterior = novo_ponto;      
                                modo_criacao = 0;
                            }
                            else{
                                novo_ponto = ponto_mais_perto((float(x)*2/win_width -1), 
                                                                -(float(y)*2/win_height -1), 
                                                                &dcel);
                                connect(novo_ponto,ponto_central_planarizacao,&dcel);  
                            }
                            break;
                    case GLUT_RIGHT_BUTTON:
                            novo_ponto->x = (float(x)*2/win_width -1);
                            novo_ponto->y = -(float(y)*2/win_height -1);
                            if (modo_criacao == 1){
                                if(!existe_outro_ponto_muito_perto(novo_ponto,&dcel)){
                                    dcel.pontos.push_back(novo_ponto);
                                    connect(novo_ponto,ponto_anterior,&dcel);
                                    ponto_anterior = novo_ponto;
                                }
                            } 
                            else{
                                dcel.pontos.push_back(novo_ponto);
                                    ponto_anterior = novo_ponto;
                                    ponto_central_planarizacao = novo_ponto;
                            }
                            break;
                    case GLUT_MIDDLE_BUTTON:
                            
                            break;
            }
        }
	glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); 

    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (Ponto* v : dcel.pontos) {
        glVertex2f(v->x, v->y);
    }
    
    glEnd();

    glColor3f(0.0, 0.0, 1.0); 
    glPointSize(6.0);
    glBegin(GL_POINTS);
    for (Ponto* v : dcel.pontos) {
        glVertex2f(halfEdge_atual->origem->x, halfEdge_atual->origem->y);
    }
    glEnd();
    glColor3f(0.0, 1.0, 0.0); 
    glPointSize(6.0);
    glBegin(GL_POINTS);
    for (Ponto* v : dcel.pontos) {
        glVertex2f(halfEdge_atual->destino->x, halfEdge_atual->destino->y);
    }
    glEnd();



   
    glColor3f(1.0, 1.0, 1.0); 
    glBegin(GL_LINES);
    for (HalfEdge* a : dcel.arestas) {
        glVertex2f(a->origem->x, a->origem->y);
        glVertex2f(a->twin->origem->x, a->twin->origem->y);
    }
    glEnd();

    glColor3f(1.0, 0.0, 0.0); 
    glBegin(GL_LINES);
    glVertex2f(halfEdge_atual->origem->x, halfEdge_atual->origem->y);
    glVertex2f(halfEdge_atual->twin->origem->x, halfEdge_atual->twin->origem->y);
    glEnd();

    //printDCELInfo();
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("DCEL Display");
    
    printf("Clique botão direito para criar um ponto\nClique botão esquedo para ligar um ponto\nDepois que fechar o poligono, coloque um ponto no meio com o botão direito\nE junte com o direito em outros pontos.\nTECLADO:\nt para TWIN\no para onext\nn para next\np para previous\n");
    printf("AZUL = ORIGEM, VERDE = DESTINO\n");
    Ponto a,b,c,d,e,f,g;
    b.x = -0.8; b.y = 0.8;b.id = 'b';
    a.x = -0.8; a.y = 0.4;a.id = 'a';

    dcel.pontos.push_back(&a);
    dcel.pontos.push_back(&b);
    //dcel.pontos.push_back(&c);
    //dcel.pontos.push_back(&d);
    //dcel.pontos.push_back(&e);
    //dcel.pontos.push_back(&f);
    //dcel.pontos.push_back(&g);
    



    //connect(&c, &a, &dcel);
    //connect(&f, &a, &dcel);
    //connect(&b, &f, &dcel);
    //connect(&f, &e, &dcel);
    //connect(&e, &d, &dcel);
    //connect(&d, &c, &dcel);
    //connect(&c, &b, &dcel);
    //connect(&d, &a, &dcel);
    connect(&b, &a, &dcel);


    
    //connect(&a, &f, &dcel);
    ponto_inicial = &a;
    ponto_anterior = &b;
    halfEdge_atual = dcel.arestas[0];
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(keyboard);
    
    

    glutMainLoop();

    for (Ponto* v : dcel.pontos)
        delete v;
    for (HalfEdge* e : dcel.arestas){
        delete e->twin;
        delete e;
    }
    
    return 0;
}







