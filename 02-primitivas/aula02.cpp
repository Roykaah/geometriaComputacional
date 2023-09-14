
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../lib/utils.h"

void ClassificadorConvexoReflexo( float poligono[],int n){
    for(int i=0;i<n;i++){
        reordena(poligono, n);
        if(doisAreaT(&poligono[i==0?(n-1)*4:(i-1)*4],&poligono[i*4],&poligono[((i+1)*4)%(n*4)])>0)
            printf("Esse vértice é CONVEXO\n");
        else
            printf("Esse vértice é REFLEXO\n");
    }
}

int main(int argc, char **argv)
{
    
    float poligono[] = {
            0.0, 0.0, 0.0,0.0, 
            0.0, 1.0, 0.0,0.0,
            0.5, 0.6, 0.0,0.0,
            1.0, 1.0, 0.0,0.0,
            1.0, 0.0, 0.0,0.0
            };
    int numPontos = 5;
    //ClassificadorConvexoReflexo(poligono,numPontos);
    /*for(int i=0;i<numPontos;i++){
        doisAreaT(&vertices[(i==0?numPontos-1:i-1)%3],&vertices[(i)%3],&vertices[(i+1)%3])<0?printf("convexo\n"):printf("reflexo\n");
        
    }*/
    reordena(poligono, numPontos);
    for(int i=0;i<numPontos;i++){
        //se for convexo
        if(doisAreaT(&poligono[i==0?(numPontos-1)*4:(i-1)*4],&poligono[i*4],&poligono[((i+1)*4)%(numPontos*4)])>0){
           if(diagonal(i==0?(numPontos-1):(i-1),(i+1)%numPontos,poligono,numPontos)){
                printf("É Orelha\n");
                continue;
           }
        }
        printf("Não é Orelha\n");
    }
}
