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
float doisAreaT(float*, float*, float*);
bool left(float*, float*, float*);
bool leftOn(float*, float*, float*);
bool collinear(float*, float*, float*);
float areaPoligono(float poligono[][3], int n);
bool interseccaoImpropria(float* p1, float* p2, float* p3);
bool interseccaoPropria(float* p1, float* p2, float* p3, float* p4);
float* interseccao(float* p1, float* p2, float* p3, float* p4);
