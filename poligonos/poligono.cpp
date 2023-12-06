#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "poligono.hpp"
#include <math.h>
#include "../lib/utils.h"

/** 
 * Retorna angulo b(^a)c
 *
 */

void imprime_pontos(vector<Ponto*> pontos){
    for (Ponto* v : pontos) {
        printf("Ponto %d: (%lf, %lf) do tipo %c  e distancia %lf\n", v->id, v->x, v->y, v->tipo, v->distancia_ao_ponto_de_referencia);
    }
}

char colocar_label_entrando_saindo(double* a1, double* b1, double* b2){

    if(left(b1,b2,a1)){
        return 's';
    }
    return 'e';
    
}

double distancia_euclidiana(Ponto* p1, Ponto* p2){
    double x = p1->x - p2->x;
    double y = p1->y - p2->y;
    double distancia = sqrt(x*x + y*y);
    return distancia;
}

vector<Ponto*> merge_pontos(vector<Ponto*> p1, vector<Ponto*> p2){
    vector<Ponto*> saida;
    for (Ponto* v : p1) {
        saida.push_back(v);
    }
    for (Ponto* v : p2) {
        saida.push_back(v);
    }
    return saida;
}

void insere_ponto_ordenadamente(vector<Ponto*> p1, Ponto* p2, double distancia){
    for(int i = 0; i < p1.size(); i++){
        if (p1[i]->distancia_ao_ponto_de_referencia > distancia){
            p1.insert(p1.begin() + i,p2);
            return;
        }
    }
}


// OZAO de n²
vector<Ponto*>  reoordena_interseccoes(vector<Ponto*> pontos_interseccao){
    int i = 0;
    int j = 0;
    vector<Ponto*> pontos_interseccao_retorno = pontos_interseccao;
    vector<Ponto*> vetor_interseccoes_aux;
    while(i<pontos_interseccao_retorno.size()){
        if (pontos_interseccao_retorno[i]->id < 0){

            vetor_interseccoes_aux.push_back(pontos_interseccao_retorno[i]);
            pontos_interseccao_retorno.erase(pontos_interseccao_retorno.begin() + i);

            while(pontos_interseccao_retorno[i]->id < 0){
                vetor_interseccoes_aux.push_back(pontos_interseccao_retorno[i]);
                pontos_interseccao_retorno.erase(pontos_interseccao_retorno.begin() + i);
            }

            double dist_minima = 100000000;
            while(vetor_interseccoes_aux.size() > 0){
                int pos = 0;
                for(int j = 0; j < vetor_interseccoes_aux.size(); j++){
                    if (vetor_interseccoes_aux[j]->distancia_ao_ponto_de_referencia < dist_minima){
                        dist_minima = vetor_interseccoes_aux[j]->distancia_ao_ponto_de_referencia;
                        pos = j;
                    }
                }
                printf("Inserindo o ponto %d na posicao %d\n",vetor_interseccoes_aux[pos]->id,i);
                pontos_interseccao_retorno.insert(pontos_interseccao_retorno.begin() + i,vetor_interseccoes_aux[pos]);
                vetor_interseccoes_aux.erase(vetor_interseccoes_aux.begin() + pos);
                dist_minima = 100000000;
                i++;
            }
        }
        else{
            i++;
        }
    }
    return pontos_interseccao_retorno;
}
vector<vector<Ponto*>> poligono_e_suas_interseccoes(vector<Ponto*> pontos1, vector<Ponto*> pontos2){
    vector<vector<Ponto*>> poligono_e_interseccoes;
    vector<Ponto*> poligono1;
    poligono_e_interseccoes.push_back(poligono1);
    poligono_e_interseccoes.push_back(pontos2);
    vector<Ponto*> pontos_interseccao_pol1_aux;
    vector<Ponto*> pontos_interseccao_pol2_aux;
    int id_interseccao = 1;
    for (int i = 0; i < pontos1.size()-1; i++){
        double p1[3] = {pontos1[i]->x, pontos1[i]->y,0.0};
        double p2[3] = {pontos1[i+1]->x, pontos1[i+1]->y,0.0};
        poligono_e_interseccoes[0].push_back(pontos1[i]);
        for (int j = 0; j < poligono_e_interseccoes[1].size()-1; j++){
            double p3[3] = {poligono_e_interseccoes[1][j]->x, poligono_e_interseccoes[1][j]->y,0.0};
            double p4[3] = {poligono_e_interseccoes[1][j+1]->x, poligono_e_interseccoes[1][j+1]->y,0.0};
            int tem_interseccao = interseccaoPropria(p1, p2, p3, p4);
            //Se tem intersecção, calcula ponto de intersecção e insere no poligono
            if (tem_interseccao){
                double x,y;
                interseccao(p1, p2, p3, p4,&x,&y);
                //adiciona para o poligono 1
                Ponto* ponto = new Ponto;
                ponto->x = x;
                ponto->y = y;
                ponto->id = -id_interseccao;
                ponto->distancia_ao_ponto_de_referencia = distancia_euclidiana(pontos1[i],ponto);
                ponto->tipo = colocar_label_entrando_saindo(p1,p3,p4);
                pontos_interseccao_pol1_aux.push_back(ponto);
                
                //poligono_e_interseccoes[0].push_back(ponto);

                //adiciona para o poligono 2
                ponto = new Ponto;
                ponto->x = x;
                ponto->y = y;
                ponto->id = -id_interseccao;
                ponto->distancia_ao_ponto_de_referencia = distancia_euclidiana(poligono_e_interseccoes[1][j],ponto);
                ponto->tipo = colocar_label_entrando_saindo(p1,p3,p4);//Talvez mudar
                poligono_e_interseccoes[1].insert(poligono_e_interseccoes[1].begin() + j+1,ponto);
                //insere_ponto_ordenadamente(pontos_interseccao_pol2_aux,ponto,ponto->distancia_ao_ponto_de_referencia);
                //merge_pontos(poligono_e_interseccoes[1],pontos_interseccao_pol2_aux,0);


                //ponto->tipo = colocar_label_entrando_saindo(p3,p1,p2);
                j++;   id_interseccao++;
                //poligono_e_interseccoes[1].insert(poligono_e_interseccoes[1].begin() + j,ponto);
                
            }

        }
        printf("Fez o merge dos pontos do pol1 na posicao %d\n",i);
        imprime_pontos(pontos_interseccao_pol1_aux);
        if(pontos_interseccao_pol1_aux.size() > 0){
            poligono_e_interseccoes[0] = merge_pontos(poligono_e_interseccoes[0],pontos_interseccao_pol1_aux);
        }
        pontos_interseccao_pol1_aux.clear();

    }
    poligono_e_interseccoes[0].push_back(pontos1[0]);   
    poligono_e_interseccoes[0] = reoordena_interseccoes(poligono_e_interseccoes[0]);
    poligono_e_interseccoes[1] = reoordena_interseccoes(poligono_e_interseccoes[1]);
    return poligono_e_interseccoes;
}

vector<poligono*> weiler_atherton(vector<Ponto*> pontos_interseccao_pol1,vector<Ponto*> pontos_interseccao_pol2){
    vector<poligono*> poligonos_interseccao;
    poligono* poligono_atual = new poligono;
    char tipo_atual = 'n';
    int completou_uma_volta = false;
    int i = 0;
    int j = 0;
    int tamanho_pol1 = pontos_interseccao_pol1.size();
    int tamanho_pol2 = pontos_interseccao_pol2.size();
    int qtd_voltas = 0;
    int id_primeiro_ponto;
    while(true){
        //Se ainda não tinha achado nenhuma intersecção e era uma entrada, insere
        if (pontos_interseccao_pol1[i]->tipo == 'e' && (tipo_atual == 'n' || tipo_atual == 's')){ 
            printf("Encontrou a primeira entrada %d e i=%d\n", pontos_interseccao_pol1[i]->id, i);
            poligono_atual->polig.push_back(pontos_interseccao_pol1[i]);
            if (id_primeiro_ponto == pontos_interseccao_pol1[i]->id) break; //se já deu uma volta, sai do while
            if (tipo_atual =='n') id_primeiro_ponto = pontos_interseccao_pol1[i]->id;
                        tipo_atual = 'e';


        }
        //Se já achou a entrada E o ponto não é uma intersecção, insere
        if (tipo_atual == 'e' && pontos_interseccao_pol1[i]->id >= 0){
            printf("Encontrou um ponto extra no pol1 %d e i=%d\n", pontos_interseccao_pol1[i]->id,i);
            poligono_atual->polig.push_back(pontos_interseccao_pol1[i]);
        }
        //Se já achou uma entrada E é uma intersecção de saída, busca no outro poligono
        if (tipo_atual == 'e' && pontos_interseccao_pol1[i]->tipo == 's'){
            printf("Encontrou uma saída pol1 %d e i=%d\n", pontos_interseccao_pol1[i]->id, i);
            poligono_atual->polig.push_back(pontos_interseccao_pol1[i]);

            int achou_j = false;
            for(;;j++){
                if (j == tamanho_pol2){
                    printf("Deu uma volta no pol2\n");
                    qtd_voltas++;
                    if (qtd_voltas == 3){
                        printf("Deu 3 voltas no pol2\n");
                        break;
                    }
                    j = 1;
                }
                if (pontos_interseccao_pol1[i]->id == pontos_interseccao_pol2[j]->id){
                    printf("Encontrou j %d\n", j);
                    achou_j = true;
                }
                //Se já achou j e não é um ponto de intersecção, então insere 
                if (achou_j &&  pontos_interseccao_pol2[j]->id>= 0){
                    printf("Encontrou um ponto extra no pol 2 %d\n", pontos_interseccao_pol2[j]->id);
                    poligono_atual->polig.push_back(pontos_interseccao_pol2[j]);
                }
                //Se já achou j e    j é uma saída de pol2, então insere e sai do for
                if (achou_j && (pontos_interseccao_pol2[j]->tipo == 'e')){
                    printf("Encontrou uma saída pol2 %d\n", pontos_interseccao_pol2[j]->id);
                    tipo_atual = 's';
                    
                    break;
                }
            }
            poligonos_interseccao.push_back(poligono_atual);
            poligono_atual = new poligono;
        }
        i++;
        if (i == tamanho_pol1) {
            printf("Deu uma volta no pol 1\n");
            i = 1;
            completou_uma_volta = true;
        }
    }

    return poligonos_interseccao;
}



vector<poligono*> interseccao_entre_poligonos(poligono* p1, poligono* p2){
    vector<poligono*> poligonos_interseccao;
    //Pega os polígonos, mas agora com interseccao
    vector<vector<Ponto*>> pontos_interseccao = poligono_e_suas_interseccoes(p1->polig, p2->polig);
    vector<Ponto*> pontos_interseccao_pol1 = pontos_interseccao[0];
    vector<Ponto*> pontos_interseccao_pol2 = pontos_interseccao[1];
    /*printf("Pontos interseccao pol1:\n");
    imprime_pontos(pontos_interseccao_pol1);
    printf("Pontos interseccao pol2:\n");
    imprime_pontos(pontos_interseccao_pol2);*/

    //Aplica weiler-atherton
    poligonos_interseccao = weiler_atherton(pontos_interseccao_pol1, pontos_interseccao_pol2);

    //Retira buracos
    //poligono poligono_interseccao = poligono_e_suas_interseccoes_com_os_buracos(poligonos_interseccao, p1->buracos,p2->buracos);


    //LEMBRAR DOS BURACOS INTERNOS TODO
    return poligonos_interseccao;
}
