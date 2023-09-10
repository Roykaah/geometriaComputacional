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

bool reflexo(float* p1, float* p2, float* p3){
    return doisAreaT(p1, p2, p3) > 0;
}
bool convexo(float* p1, float* p2, float* p3){
    return doisAreaT(p1, p2, p3) < 0;
}

/** 
 * Calcula area do polígono.
 *
 * @param poligono vetor[n*4].
 * @param n Número de pontos no polígono.
 * @return Area float*/
float areaPoligono(float poligono[], int n){
    float area = 0.0;
    float pZero[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < n-1; i++){
        area +=  doisAreaT(pZero, &poligono[i*4], &poligono[(i+1)*4]);
    }
    area +=  doisAreaT(pZero, &poligono[(n-1)*4], &poligono[0]);
    return area/2;
}

/**
 *  Reordena polígino para ser anti-horário*/
void reordena(float poligono[], int n){
    float area = areaPoligono(poligono, n);
    //Se a área é menor que 0, então ele é horário, então inverte
    if (area < 0){
        float aux[4];
        for (int i = 0; i < n/2; i++){
            for (int j = 0; j < 4; j++){
                aux[j] = poligono[(i*4)+j];
                poligono[(i*4)+j] = poligono[(n-i-1)*4+j];
                poligono[(n-i-1)*4+j] = aux[j];
            }
        }
    }
}


bool diagonal(int numeroDoPa, int numeroDoPb, float poligono[], int n){

    //in Cone()
    float pa[3], pb[3] ,anteriorPa[3],anteriorPb[3],posteriorPa[3],posteriorPb[3];
    for(int i = 0; i < 3; i++){
        pa[i] = poligono[numeroDoPa*4+i];
        pb[i] = poligono[numeroDoPb*4+i];
        anteriorPa[i] = poligono[(numeroDoPa-1)*4+i];
        anteriorPb[i] = poligono[(numeroDoPb-1)*4+i];
        posteriorPa[i] = poligono[(numeroDoPa+1)*4+i];
        posteriorPb[i] = poligono[(numeroDoPb+1)*4+i];
    }
    //Arrumar!
    /*if (convexo(anteriorPa,pa,posteriorPa)){
        if (!(left(pa,pb,anteriorPa) && left(pb,pa,posteriorPa))){
            return false;
        }
        
    } else{
        if(!(leftOn(pa,pb,posteriorPa)&&leftOn(pb,pa,anteriorPa))){
            return false;
        }
    }*/
    //Tratamentos para INTERSECÇÃO:
    for(int i =0; i<n; i++){
        if(i != numeroDoPa && i != numeroDoPb && interseccaoImpropria(pa,pb,&poligono[i*4])){
            printf("Intersecção impropria com i=%d, a=%d e b=%d \n",i,numeroDoPa,numeroDoPb);
            return false;
        }
        if((i+1)%n == numeroDoPa || i == numeroDoPa || (i+1)%n == numeroDoPb || i == numeroDoPb){
            continue;
        }
        if(interseccaoPropria(pa,pb,&poligono[(i)%n],&poligono[((i+1)*4)%n*4])){
            return false;
        }
        
    }

    return true;
}

/** 
 * Vê se tem intersecção impropria entre  linha (p1,p2) e ponto(p3).
 *
 * @return 0 se não tem, 1 se tem*/
bool interseccaoImpropria(float* p1, float* p2, float* p3){
    if (collinear(p1, p2, p3) && ((p1[0] <= p3[0] && p3[0] <= p2[0])|| (p2[0] <= p3[0] && p3[0] <= p1[0]))){
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
 * @return ponto de interseccao (float[2])
int interseccao(float* p1, float* p2, float* p3, float* p4){

/*
    //As contas com ctz estão erradas!!!!
    float alpha = (p4[0]-p3[0])*(p1[1]-p3[1]) - (p4[1]-p3[1])*(p1[0]-p3[0])
    /((p4[1]-p3[1])*(p2[0]-p1[0]) - (p4[0]-p3[0])*(p2[1]-p1[1]));

    float beta = (p2[0]-p1[0])*(p1[1]-p3[1]) - (p2[1]-p1[1])*(p1[0]-p3[0])/
    ((p4[1]-p3[1])*(p2[0]-p1[0]) - (p4[0]-p3[0])*(p2[1]-p1[1]));;
    //!!!!!!!!!!!!!

    float ponto[2] = {p1[0] + alpha*(p2[0]-p1[0]), p1[1] + alpha*(p2[1]-p1[1])};
    return ponto;*/
    /*float v = 0;
    return 1;
}*/
