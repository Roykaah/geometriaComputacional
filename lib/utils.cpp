/**
 * @file utils.cpp
 * Util functions.
 *
 * Implements frequently used functions.
 *
 * @author Ricardo Dutra da Silva
 */

#include "utils.h"
#include <cmath>


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


double doisAreaT(double* p1, double* p2, double* p3){
    double v0[3], v1[3];

    //cria os dois vetores que irão formar o triangulo
    for (int i = 0; i < 3; i++)
    {
        v0[i] = p2[i] - p1[i];
        v1[i] = p3[i] - p1[i];
    }
    return ((v0[1]-v0[0])*(v1[2]-v1[0]) - (v0[2]-v0[0])*(v1[1]-v1[0]));
}

bool left(double* p1, double* p2, double* p3){
    return doisAreaT(p1, p2, p3) > 0;
}

bool leftOn(double* p1, double* p2, double* p3){
    return doisAreaT(p1, p2, p3) >= 0;
}

bool collinear(double* p1, double* p2, double* p3){
    return doisAreaT(p1, p2, p3) == 0;
}

bool reflexo(double* p1, double* p2, double* p3){
    return doisAreaT(p1, p2, p3) > 0;
}
bool convexo(double* p1, double* p2, double* p3){
    return doisAreaT(p1, p2, p3) < 0;
}

/** 
 * Calcula area do polígono.
 *
 * @param poligono vetor[n*4].
 * @param n Número de pontos no polígono.
 * @return Area double*/
double areaPoligono(double poligono[], int n){
    double area = 0.0;
    double pZero[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < n-1; i++){
        area +=  doisAreaT(pZero, &poligono[i*4], &poligono[(i+1)*4]);
    }
    area +=  doisAreaT(pZero, &poligono[(n-1)*4], &poligono[0]);
    return area/2;
}

/**
 *  Reordena polígino para ser anti-horário*/
void reordena(double poligono[], int n){
    double area = areaPoligono(poligono, n);
    //Se a área é menor que 0, então ele é horário, então inverte
    if (area < 0){
        double aux[4];
        for (int i = 0; i < n/2; i++){
            for (int j = 0; j < 4; j++){
                aux[j] = poligono[(i*4)+j];
                poligono[(i*4)+j] = poligono[(n-i-1)*4+j];
                poligono[(n-i-1)*4+j] = aux[j];
            }
        }
    }
}


bool diagonal(int numeroDoPa, int numeroDoPb, double poligono[], int n){

    //in Cone()
    double pa[3], pb[3] ,anteriorPa[3],anteriorPb[3],posteriorPa[3],posteriorPb[3];
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
            return false;-
        }
    }*/
    //Tratamentos para INTERSECÇÃO:
    for(int i =0; i<n; i++){
        if(i != numeroDoPa && i != numeroDoPb && interseccaoImpropria(pa,pb,&poligono[i*4])){
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
bool interseccaoImpropria(double* p1, double* p2, double* p3){
    if (collinear(p1, p2, p3) && ((p1[0] <= p3[0] && p3[0] <= p2[0])|| (p2[0] <= p3[0] && p3[0] <= p1[0]))){
        return true;
    }
    return false;
}
/** 
 * Vê se tem intersecção própria entre duas linhas (p1,p2) (p3,p4).
 *
 * @return 0 se não tem, 1 se tem*/
bool interseccaoPropria(double* p1, double* p2, double* p3, double* p4){
    if (collinear(p1,p2,p3)||collinear(p1,p2,p4)){
        return false;
    }

    if (left(p1, p2, p3) != left(p1, p2, p4) && left(p3, p4, p1) != left(p3, p4, p2)){
        return true;
    }
    
    return false;
}


inline double Det(double a, double b, double c, double d)
{
        return a*d - b*c;
}

/** 
 * Vê onde tem a interseccao. (2D)
 *
 * @return ponto de interseccao (double[2])*/
void interseccao(double* p1, double* p2, double* p3, double* p4,double* ixOut,double* iyOut){

    // Line AB represented as a1x + b1y = c1
    double a1 = p2[1] - p1[1];
    double b1 = p1[0] - p2[0];
    double c1 = a1*(p1[0]) + b1*(p1[1]);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = p4[1] - p3[1];
    double b2 = p3[0] - p4[0];
    double c2 = a2*(p3[0])+ b2*(p3[1]);
 
    double determinant = a1*b2 - a2*b1;
 
    if (determinant != 0)
    {
        *ixOut = (b2*c1 - b1*c2)/determinant;
        *iyOut = (a1*c2 - a2*c1)/determinant;
    }

}
