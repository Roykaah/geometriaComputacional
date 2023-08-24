/**
 * @file utils.cpp
 * Util functions.
 *
 * Implements frequently used functions.
 *
 * @author Ricardo Dutra da Silva
 */

#include "utils.h"


/** 
 * Create program.
 *
 * Creates a program from given shader codes.
 *
 * @param vertex_code String with code for vertex shader.
 * @param fragment_code String with code for fragment shader.
 * @return Compiled program.
 */
int createShaderProgram(const char *vertex_code, const char *fragment_code)
{
	
    int success;
    char error[512];

    // Request a program and shader slots from GPU
    int program  = glCreateProgram();
    int vertex   = glCreateShader(GL_VERTEX_SHADER);
    int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Set shaders source
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compile shaders
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(vertex, 512, NULL, error);
	std::cout << "ERROR: Shader comilation error: " << error << std::endl;
    }
                
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(fragment, 512, NULL, error);
	std::cout << "ERROR: Shader comilation error: " << error << std::endl;
    }

    // Attach shader objects to the program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Build program
    glLinkProgram(program);
    glGetShaderiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
	glGetProgramInfoLog(program, 512, NULL, error);
	std::cout << "ERROR: Program link error: " << error << std::endl;
    }

    // Get rid of shaders (not needed anymore)
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}


float doisAreaT(float* p1, float* p2, float* p3){
    float v0[3], v1[3];

    //cria os dois vetores que irão formar o triangulo
    for (int i = 0; i < 3; i++)
    {
        v0[i] = p2[i] - p1[i];
        v1[i] = p3[i] - p1[i];
    }

    return ((v0[1]-v0[0])*(v1[2]-v1[0]) - (v0[2]-v0[0])*(v1[1]-v1[0]));
}

bool left(float* p1, float* p2, float* p3){
    return doisAreaT(p1, p2, p3) > 0;
}

bool leftOn(float* p1, float* p2, float* p3){
    return doisAreaT(p1, p2, p3) >= 0;
}

bool collinear(float* p1, float* p2, float* p3){
    return doisAreaT(p1, p2, p3) == 0;
}

/** 
 * Calcula area do programa.
 *
 * @param poligono vetor[x][3].
 * @param n Número de pontos no polígono.
 * @return Area float*/
float areaPoligono(float poligono[][3], int n){
    float area = 0.0;
    float pZero[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < n-1; i++){
        area +=  doisAreaT(pZero, poligono[i], poligono[i+1]);
    }
    area +=  doisAreaT(pZero, poligono[n-1], poligono[0]);
    return area/2;
}

/** 
 * Vê se tem intersecção impropria entre duas linhas (p1,p2) (p3,p4).
 *
 * @return 0 se não tem, 1 se tem*/
bool interseccaoImpropria(float* p1, float* p2, float* p3){
   
    if (collinear(p1, p2, p3) && (p1[0] <= p3[0] && p3[0] <= p2[0])|| (p2[0] <= p3[0] && p3[0] <= p1[0])){

        return true;
    }
    return false;
}
/** 
 * Vê se tem intersecção própria entre duas linhas (p1,p2) (p3,p4).
 *
 * @return 0 se não tem, 1 se tem*/
bool interseccaoPropria(float* p1, float* p2, float* p3, float* p4){
    if (collinear(p1,p2,p3)||collinear(p1,p2,p4)){
        return false;
    }
    if (left(p1, p2, p3) == left(p1, p2, p4)){
        return false;
    }
    if (left(p3, p4, p1) == left(p3, p4, p2)){
        return false;
    }
    return true;
}

/** 
 * Vê onde tem a interseccao. (2D)
 *
 * @return ponto de interseccao (float[2])*/
float* interseccao(float* p1, float* p2, float* p3, float* p4){

    //As contas com ctz estão erradas!!!!
    float alpha = (p4[0]-p3[0])*(p1[1]-p3[1]) - (p4[1]-p3[1])*(p1[0]-p3[0])
    /((p4[1]-p3[1])*(p2[0]-p1[0]) - (p4[0]-p3[0])*(p2[1]-p1[1]));

    float beta = (p2[0]-p1[0])*(p1[1]-p3[1]) - (p2[1]-p1[1])*(p1[0]-p3[0])/
    ((p4[1]-p3[1])*(p2[0]-p1[0]) - (p4[0]-p3[0])*(p2[1]-p1[1]));;
    //!!!!!!!!!!!!!

    float ponto[2] = {p1[0] + alpha*(p2[0]-p1[0]), p1[1] + alpha*(p2[1]-p1[1])};
    return ponto;
}