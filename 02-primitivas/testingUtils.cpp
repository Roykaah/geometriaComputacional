
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../lib/utils.h"

int main(int argc, char** argv)
{
    float p1[3] = {0.0, 0.0, 0.0};
    float p2[3] = {1.0, 0.0, 0.0};
    float p3[3] = {0.0, 1.0, 0.0};
    /*
    float areaTriangulo = doisAreaT(p1, p2, p3)/2;
    printf("Area do triangulo: %f\n", areaTriangulo);
    printf("left: %d\n", left(p1, p2, p3));
    printf("leftOn: %d\n", leftOn(p1, p2, p3));
    printf("collinear: %d\n", collinear(p1, p2, p3));*/
    float poligono[] = {
            0.0, 0.0, 0.0,0.0,
            1.0, 0.0, 0.0,0.0,
            1.0, 1.0, 0.0,0.0,
            0.0, 1.0, 0.0,0.0
            };
    printf("Area do poligono: %f\n", areaPoligono(poligono,4));
    return 0;

}