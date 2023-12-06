/**
 * @file utils.h
 * Util functions.
 *
 * Defines a set of util frequently used functions.
 *
 * @author Ricardo Dutra da Silva
 */

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>


/** 
 * Create program.
 *
 * Creates a program from given shader codes.
 *
 * @param vertex_code String with code for vertex shader.
 * @param fragment_code String with code for fragment shader.
 * @return Compiled program.
 */
int createShaderProgram(const char *, const char *);
double doisAreaT(double*, double*, double*);
bool left(double*, double*, double*);
bool leftOn(double*, double*, double*);
bool collinear(double*, double*, double*);
double areaPoligono(double poligono[], int n);
bool interseccaoImpropria(double* p1, double* p2, double* p3);
bool interseccaoPropria(double* p1, double* p2, double* p3, double* p4);
//int interseccao(double* p1, double* p2, double* p3, double* p4);
bool reflexo(double* p1, double* p2, double* p3);
bool convexo(double* p1, double* p2, double* p3);
void reordena(double poligono[], int n);
bool diagonal(int numeroDoPa, int numeroDoPb, double poligono[], int n);
void interseccao(double* p1, double* p2, double* p3, double* p4,double* ixOut,double* iyOut);