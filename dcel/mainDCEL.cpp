
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "dcel.hpp"
#include <math.h>

DCEL dcel;
HalfEdge* halfEdge_atual;

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
    glColor3f(0.8, 0.5, 1.0); 
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("DCEL Display");
    
    //initializeDCEL();
     Ponto a,b,c,d,e,f,g;
    f.x = -0.5; f.y = -0.5;f.id = 'f';
    e.x = -0.5; e.y = 0;e.id = 'e';
    d.x = 0; d.y = 0.5;d.id = 'd';
    c.x = 0.5; c.y = 0.5;c.id = 'c';
    b.x = 0.5; b.y = 0;b.id = 'b';
    a.x = 0; a.y = 0;a.id = 'a';
    g.x = 0.1; g.y = -0.1;g.id = 'g';

    dcel.pontos.push_back(&a);
    dcel.pontos.push_back(&b);
    dcel.pontos.push_back(&c);
    dcel.pontos.push_back(&d);
    dcel.pontos.push_back(&e);
    dcel.pontos.push_back(&f);
    dcel.pontos.push_back(&g);
    



    connect(&c, &a, &dcel);
    connect(&f, &a, &dcel);
    connect(&b, &f, &dcel);
    connect(&f, &e, &dcel);
    connect(&e, &d, &dcel);
    connect(&d, &c, &dcel);
    connect(&c, &b, &dcel);
    connect(&d, &a, &dcel);
    connect(&b, &a, &dcel);

    
    //connect(&a, &f, &dcel);
    halfEdge_atual = dcel.arestas[0];

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








