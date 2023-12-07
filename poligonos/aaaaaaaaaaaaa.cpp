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
        printf("Ponto %d: (%lf, %lf) do tipo %c visitado=%d   distancia=%lf\n", v->id, v->x, v->y, v->tipo, v->visitado,v->distancia_ao_ponto_de_referencia);
    }
}
double dooisAreaT(double* p1, double* p2, double* p3){
    double v0[3], v1[3];

    //cria os dois vetores que irão formar o triangulo
    for (int i = 0; i < 3; i++)
    {
        v0[i] = p2[i] - p1[i];
        v1[i] = p3[i] - p1[i];
    }
    return ((v0[1]-v0[0])*(v1[2]-v1[0]) - (v0[2]-v0[0])*(v1[1]-v1[0]));
}

double areaPoligonoo(double poligono[][3], int n){
    double area = 0.0;
    double pZero[3] = {0.0, 0.0, 0.0};
    for (int i = 0; i < n-1; i++){
        area +=  dooisAreaT(pZero, poligono[i], poligono[i+1]);
    }
    area +=  dooisAreaT(pZero, poligono[n-1], poligono[0]);
    return area/2;
}


vector<Ponto*> reoordena(vector<Ponto*> pontos,int ordem){
    vector<Ponto*> pontos_retorno;
    //transforma pontos em *vetor[2]

    double vetor[pontos.size()][3];
    for (int i = 0; i < pontos.size(); i++){
        vetor[i][0] = pontos[i]->x;
        vetor[i][1] = pontos[i]->y;
        vetor[i][2] = 0.0;  
    }
    double area_poligono = areaPoligonoo(vetor,pontos.size());
    int ordem_atual = area_poligono > 0;
    if (ordem != ordem_atual){
        for (int i = pontos.size()-1; i >= 0; i--){
            pontos_retorno.push_back(pontos[i]);
        }
    }
    else{
        return pontos;
    }
    return pontos_retorno;
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
vector<Ponto*> marca_nao_visitados(vector<Ponto*> pontos){
    for (Ponto* v : pontos) {
        v->visitado = false;
    }
    return pontos;
}



// OZAO de n²
vector<Ponto*>  reoordena_interseccoes(vector<Ponto*> pontos_interseccao){
    int i = 0;
    int j = 0;
    vector<Ponto*> pontos_interseccao_retorno = pontos_interseccao;
    vector<Ponto*> vetor_interseccoes_aux;
    imprime_pontos(pontos_interseccao);
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
                pontos_interseccao_retorno.insert(pontos_interseccao_retorno.begin() + i,vetor_interseccoes_aux[pos]);
                vetor_interseccoes_aux.erase(vetor_interseccoes_aux.begin() + pos);
                dist_minima = 100000000;
                i++;
            }
        } //Só para enquanto não acha o ponto de entrada. Marca o resto como visitado
        else{
            i++;
        }
    }
    return pontos_interseccao_retorno;
}

vector<Ponto*> deixa_positivo(vector<Ponto*> pontos){
    vector<Ponto*> pontos_retorno;
    for (int i = 0; i < pontos.size(); i++){
        if (pontos[i]->id <= 0){
            pontos[i]->id = -pontos[i]->id;
        }
        pontos[i]->distancia_ao_ponto_de_referencia = 0;

        pontos_retorno.push_back(pontos[i]);
    }
    return pontos_retorno;
}
int id_interseccao = 1;


vector<vector<Ponto*>> poligono_e_suas_interseccoes(vector<Ponto*> pontos1, vector<Ponto*> pontos2){
    vector<vector<Ponto*>> poligono_e_interseccoes;
    pontos1 = deixa_positivo(pontos1);
    vector<Ponto*> poligono1;
    poligono_e_interseccoes.push_back(poligono1);
    poligono_e_interseccoes.push_back(pontos2);
    vector<Ponto*> pontos_interseccao_pol1_aux;
    vector<Ponto*> pontos_interseccao_pol2_aux;
    poligono_e_interseccoes[1] = deixa_positivo(poligono_e_interseccoes[1]);
    for (int i = 0; i < pontos1.size()-1; i++){
        double p1[3] = {pontos1[i]->x, pontos1[i]->y,0.0};
        double p2[3] = {pontos1[i+1]->x, pontos1[i+1]->y,0.0};
        poligono_e_interseccoes[0].push_back(pontos1[i]);
        for (int j = 0; j < poligono_e_interseccoes[1].size()-1; j++){
            double p3[3] = {poligono_e_interseccoes[1][j]->x, poligono_e_interseccoes[1][j]->y,0.0};
            double p4[3] = {poligono_e_interseccoes[1][j+1]->x, poligono_e_interseccoes[1][j+1]->y,0.0};
            int tem_interseccao = interseccaoPropria(p1, p2, p3, p4);
            //Se tem intersecção, calcula ponto de intersecção e insere no poligono
            printf("PontosAQUI %d e %d\n",pontos1[i]->id,poligono_e_interseccoes[1][j]->id);
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
                poligono_e_interseccoes[1].insert(poligono_e_interseccoes[1].begin() + j+1,ponto);      //CERTOOOO
                //insere_ponto_ordenadamente(pontos_interseccao_pol2_aux,ponto,ponto->distancia_ao_ponto_de_referencia);
                //merge_pontos(poligono_e_interseccoes[1],pontos_interseccao_pol2_aux,0);


                //ponto->tipo = colocar_label_entrando_saindo(p3,p1,p2);
                j++;   id_interseccao++;
                //poligono_e_interseccoes[1].insert(poligono_e_interseccoes[1].begin() + j,ponto);
                
            }

        }
        if(pontos_interseccao_pol1_aux.size() > 0){
            poligono_e_interseccoes[0] = merge_pontos(poligono_e_interseccoes[0],pontos_interseccao_pol1_aux);
        }
        pontos_interseccao_pol1_aux.clear();

    }
    printf("A\n");
    poligono_e_interseccoes[0].push_back(pontos1[0]);  
    poligono_e_interseccoes[0] = reoordena_interseccoes(poligono_e_interseccoes[0]);

    //poligono_e_interseccoes[1] = reoordena_interseccoes(poligono_e_interseccoes[1]);
    return poligono_e_interseccoes;
}

vector<Ponto *> encontra_id_entrada_e_saida_I(vector<Ponto*> pontos_interseccao, int id_objetivo,int *id_entrada,
                                            int *i_entrada, int *id_saida, int *i_saida){
    vector<Ponto*> poligono_atual;
    int i = 0;
    int j = 0;
    int achou_entrada = false;
    int achou_saida = false;
    int tipo_atual;
    for(int c=0; c<2;c++)
    for(i = 0;i< pontos_interseccao.size(); i++){
        printf("1ponto atual:  %d do tipo %c, mas estou procurando %d com visitado = %d\n",pontos_interseccao[i]->id,pontos_interseccao[i]->tipo,id_objetivo,pontos_interseccao[i]->visitado);
        //Se achou uma entrada, então insere e sai do for
        if (pontos_interseccao[i]->tipo == 'e' && pontos_interseccao[i]->visitado == false && id_objetivo == pontos_interseccao[i]->id){
            
            printf("Achou entrada %d para pol1\n", pontos_interseccao[i]->id);
            *id_entrada = pontos_interseccao[i]->id;
            *i_entrada = i;
            tipo_atual = 'e';
        }
        //Se achou uma ponto normal, então insere 
        if (pontos_interseccao[i]->id >= 0 && pontos_interseccao[i]->visitado == false && tipo_atual == 'e'){
            
            printf("Achou ponto extra %d para pol1\n", pontos_interseccao[i]->id);
            poligono_atual.push_back(pontos_interseccao[i]);
        }
        //Se achou uma saída, então insere e sai do for
        if (pontos_interseccao[i]->tipo == 's' && pontos_interseccao[i]->visitado == false && tipo_atual == 'e'){
            
            printf("Achou saída %d para pol1\n", pontos_interseccao[i]->id);
            *id_saida = pontos_interseccao[i]->id;
            *i_saida = i;
            poligono_atual.push_back(pontos_interseccao[i]);
            break;
        }
    }
    return poligono_atual;
}


vector<Ponto *> encontra_id_entrada_e_saida_J(vector<Ponto*> pontos_interseccao, int id_objetivo,int *id_entrada,
                                            int *i_entrada, int *id_saida, int *i_saida){
    vector<Ponto*> poligono_atual;
    int i = 0;
    int j = 0;
    int achou_entrada = false;
    int achou_saida = false;
    int tipo_atual;
    for(int c=0; c<2;c++)
    for(i = 0;i< pontos_interseccao.size(); i++){
        //Se achou uma entrada, então insere e sai do for
        if (pontos_interseccao[i]->tipo == 's' && pontos_interseccao[i]->visitado == false && id_objetivo == pontos_interseccao[i]->id){
            
            printf("Achou entrada %d para pol2\n", pontos_interseccao[i]->id);
            *id_entrada = pontos_interseccao[i]->id;
            *i_entrada = i;
            tipo_atual = 's';
        }
        //Se achou uma ponto normal, então insere 
        if (pontos_interseccao[i]->id >= 0 && pontos_interseccao[i]->visitado == false && tipo_atual == 's'){
            
            printf("Achou ponto extra %d para pol2\n", pontos_interseccao[i]->id);
            poligono_atual.push_back(pontos_interseccao[i]);
        }
        //Se achou uma saída, então insere e sai do for
        if (pontos_interseccao[i]->tipo == 'e' && pontos_interseccao[i]->visitado == false && tipo_atual == 's'){
            
            printf("Achou saída %d para pol2\n", pontos_interseccao[i]->id);
            *id_saida = pontos_interseccao[i]->id;
            *i_saida = i;
            poligono_atual.push_back(pontos_interseccao[i]);
            break;
        }
    }
    return poligono_atual;
}


vector<Ponto*> marca_como_visitado(vector<Ponto*> poligonos_interseccao,int id_entrada,int id_saida){
    vector<Ponto*> poligono;
    if (id_entrada < id_saida){
        for(int i = 0; i < poligonos_interseccao.size(); i++){
            if(i >= id_entrada && i <= id_saida){
                poligonos_interseccao[i]->visitado = true;
            }
        }
    }
    else{
        for(int i = 0; i < poligonos_interseccao.size(); i++){
            if(i >= id_entrada || i <= id_saida){
                poligonos_interseccao[i]->visitado = true;
            }
        }
    }
    for (Ponto* p : poligonos_interseccao){
        poligono.push_back(p);
    }
    return poligono;
}

vector<Ponto*> marca_como_visitado_id(vector<Ponto*> poligonos_interseccao,int id){
    vector<Ponto*> poligono;
    for(int i = 0; i < poligonos_interseccao.size(); i++){
        if(poligonos_interseccao[i]->id == id){
            poligonos_interseccao[i]->visitado = true;
        }
        poligono.push_back(poligonos_interseccao[i]);
    }
    return poligono;
}

vector<poligono*> weiler_atherton_2(vector<Ponto*> pontos_interseccao_pol1,vector<Ponto*> pontos_interseccao_pol2){

    //Inicializa itens
    vector<poligono*> poligonos_interseccao;
    vector<Ponto*> poligono_atual;
    //Acha um clip
    int i = 0;

    //Acha a primeira entrada
    int id_inicial = -9999999;
    for(i = 0; i< pontos_interseccao_pol1.size(); i++){
        if (pontos_interseccao_pol1[i]->tipo == 'e' && pontos_interseccao_pol1[i]->visitado == false){
            printf("Achou entrada INICIAL %d\n", pontos_interseccao_pol1[i]->id);
            id_inicial = pontos_interseccao_pol1[i]->id;
            break;
        }
    }
    //Se não achou inicial que não foi visitada, esse é o nosso caso base
    if (id_inicial == -9999999){
        printf("Não achou entrada\n");
        return poligonos_interseccao;
    }

    int j,id_entrada,i_entrada,id_saida,i_saida;
    int achou_entrada = false;
    id_saida = id_inicial;
    int id_objetivo = id_inicial;
    int qtd_voltas = 0;
    while(!achou_entrada){
        printf("Procurando por entrada %d no pol1\n",id_saida);
        poligono_atual = merge_pontos(poligono_atual,encontra_id_entrada_e_saida_I(pontos_interseccao_pol1,id_objetivo, &id_entrada, &i_entrada, &id_saida, &i_saida));
        printf("Achou entrada %d e saída %d\n", id_entrada,id_saida);
        pontos_interseccao_pol1 = marca_como_visitado(pontos_interseccao_pol1,i_entrada,i_saida);
        id_objetivo = id_saida;
        printf("Procurando por entrada %d no pol2\n",id_saida);
        //imprime_pontos(pontos_interseccao_pol1);
        poligono_atual = merge_pontos(poligono_atual,encontra_id_entrada_e_saida_J(pontos_interseccao_pol2,id_objetivo,&id_entrada, &i_entrada, &id_saida, &i_saida));
        printf("Achou entrada %d e saída %d\n", id_entrada,id_saida);
        pontos_interseccao_pol2 = marca_como_visitado(pontos_interseccao_pol2,i_entrada,i_saida);
        pontos_interseccao_pol1 = marca_como_visitado_id(pontos_interseccao_pol1,id_saida);
        id_objetivo = id_saida;
        qtd_voltas++;
        if (qtd_voltas == 3)return poligonos_interseccao;
        if(id_saida == id_inicial){
            achou_entrada = true;
        }else{
            printf("Não achou entrada, continua procurando o id %d\n",id_inicial);
        }
    }
    poligono_atual.push_back(poligono_atual[0]);
    //junta a saída com outras entradas
    poligono* p = new poligono;
    p->polig = poligono_atual;
    poligonos_interseccao = weiler_atherton_2(pontos_interseccao_pol1, pontos_interseccao_pol2);
    poligonos_interseccao.push_back(p);

    return poligonos_interseccao;

}

vector<poligono*> weiler_atherton(vector<Ponto*> pontos_interseccao_pol1,vector<Ponto*> pontos_interseccao_pol2){
    
    vector<poligono*> poligonos_interseccao;
    poligono* poligono_atual = new poligono;
    pontos_interseccao_pol1 = marca_nao_visitados(pontos_interseccao_pol1);
    pontos_interseccao_pol2 = marca_nao_visitados(pontos_interseccao_pol2);
    printf("BURACO NO MESCLLAAA:\n");
        imprime_pontos(pontos_interseccao_pol1);
        printf("INTERSECCAO NO MESCLLAAA:\n");
        imprime_pontos(pontos_interseccao_pol2);
    char tipo_atual = 'n';
    int completou_uma_volta = false;
    int i = 0;
    int j = 0;
    int tamanho_pol1 = pontos_interseccao_pol1.size();
    int tamanho_pol2 = pontos_interseccao_pol2.size();
    int qtd_voltas = 0;
    int id_primeiro_ponto = -99999999;
    int voltas_completadas = 0;
    while(voltas_completadas < 25){
        //Se ainda não tinha achado nenhuma intersecção e era uma entrada, insere
        if (pontos_interseccao_pol1[i]->tipo == 'e' && pontos_interseccao_pol1[i]->visitado == false && tipo_atual != 'e' ){ 
            printf("Encontrou a entrada %d em pol1 e i=%d\n", pontos_interseccao_pol1[i]->id, i);
            pontos_interseccao_pol1[i]->visitado = true;
            id_primeiro_ponto = pontos_interseccao_pol1[i]->id;
            poligono_atual->polig.push_back(pontos_interseccao_pol1[i]);
            tipo_atual = 'e';
        }
        //Se já achou a entrada E o ponto não é uma intersecção, insere
        if (tipo_atual == 'e' && pontos_interseccao_pol1[i]->id >= 0 && pontos_interseccao_pol1[i]->visitado == false ){
            printf("Encontrou um ponto extra no pol1 %d e i=%d\n", pontos_interseccao_pol1[i]->id,i);
            pontos_interseccao_pol1[i]->visitado = true;
            poligono_atual->polig.push_back(pontos_interseccao_pol1[i]);
        }
        //Se já achou uma entrada E é uma intersecção de saída, busca no outro poligono
        if (tipo_atual == 'e' && pontos_interseccao_pol1[i]->tipo == 's' && pontos_interseccao_pol1[i]->visitado == false ){
            printf("Encontrou uma saída pol1 %d e i=%d\n", pontos_interseccao_pol1[i]->id, i);
            pontos_interseccao_pol1[i]->visitado = true;
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
                if (!achou_j && pontos_interseccao_pol1[i]->id == pontos_interseccao_pol2[j]->id){
                    pontos_interseccao_pol2[j]->visitado = true;
                    printf("Encontrou j %d\n", j);
                    achou_j = true;
                }
                //Se já achou j e não é um ponto de intersecção, então insere 
                if (achou_j &&  pontos_interseccao_pol2[j]->id>= 0 && pontos_interseccao_pol2[j]->visitado == false){
                    pontos_interseccao_pol2[j]->visitado = true;
                    printf("Encontrou um ponto extra no pol 2 %d\n", pontos_interseccao_pol2[j]->id);
                    poligono_atual->polig.push_back(pontos_interseccao_pol2[j]);
                }
                //Se já achou j e    j é uma saída de pol2, então insere e sai do for
                if (achou_j && pontos_interseccao_pol2[j]->tipo == 'e' && pontos_interseccao_pol2[j]->visitado == false){
                    pontos_interseccao_pol2[j]->visitado = true;
                    printf("Encontrou uma saída pol2 %d, sendo que o id_inicial =%d \n", pontos_interseccao_pol2[j]->id,id_primeiro_ponto);
                    poligono_atual->polig.push_back(pontos_interseccao_pol2[j]);
                    tipo_atual = 's';
                    /*atualiza valor no outro vetor*/
                    for(int z=0;z<pontos_interseccao_pol1.size();z++){
                        if (pontos_interseccao_pol1[z]->id == pontos_interseccao_pol2[j]->id){
                                pontos_interseccao_pol1[z]->visitado = true;
                        }
                    }
                    if (pontos_interseccao_pol2[j]->id == id_primeiro_ponto){
                        poligonos_interseccao.push_back(poligono_atual);
                        printf("Fechou uma interseccao, poligono %ld\n", poligonos_interseccao.size());
                        poligono_atual = new poligono;
                        id_primeiro_ponto = -9999999;
                        break;
                    } else {
                        for(int z=0;z<pontos_interseccao_pol1.size();z++){
                            if (pontos_interseccao_pol1[z]->id == pontos_interseccao_pol2[j]->id){
                                tipo_atual = 'e';
                                pontos_interseccao_pol1[z]->visitado = true;
                                printf("Pesquisou por E encontrou o ponto %d no pol1 na posicao %d\n", pontos_interseccao_pol1[z]->id, z);
                                i = z;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            
        }
        i++;
        if (i == tamanho_pol1) {
            printf("Deu uma volta no pol 1\n");
            i = 1;
            voltas_completadas++;
            completou_uma_volta = true;
        }
    }

    return poligonos_interseccao;
}

vector<vector<Ponto*>> junta_buracos(vector<vector<Ponto*>> buracos1, vector<vector<Ponto*>> buracos2){
    vector<vector<Ponto*>> buracos;
    for (vector<Ponto*> buraco : buracos1){
        if(!buraco.empty())
        buracos.push_back(buraco);
    }
    for (vector<Ponto*> buraco : buracos2){
        if(!buraco.empty())
        buracos.push_back(buraco);
    }
    return buracos;
}

int compara_poligono(poligono* p1, poligono* p2){
    if (p1->polig.size() != p2->polig.size()) return 0;
    for (int i = 0; i < p1->polig.size(); i++){
        if (p1->polig[i]->id != p2->polig[i]->id) return 0;
    }
    return 1;
}

int compara_vetor_de_ponto(vector<Ponto*> p1, vector<Ponto*> p2){
    if (p1.size() != p2.size()) return 0;
    for (int i = 0; i < p1.size(); i++){
        if (p1[i]->id != p2[i]->id) return 0;
    }
    return 1;
}

vector<poligono*> mescla_intersseccao_com_buracos(poligono* interseccao, vector<vector<Ponto*>> buracos){
    vector<poligono*> novas_interseccoes;
    //Não funcionaria pra mais de uma interseccção voltando do weiler-atherton
    novas_interseccoes.push_back(interseccao);

    for (vector<Ponto*> b: buracos){
        vector<vector<Ponto*>> pontos_interseccao = poligono_e_suas_interseccoes(b,novas_interseccoes[0]->polig);
        vector<Ponto*> pontos_interseccao_pol1 = pontos_interseccao[1];
        vector<Ponto*> pontos_interseccao_buraco = pontos_interseccao[0];
        
        printf("ENTRANDO EM WEILER:\n");
        novas_interseccoes = weiler_atherton( pontos_interseccao_buraco, pontos_interseccao_pol1);
        printf("SAIIIINDO DE WEILER:\n");

    }
    
    return novas_interseccoes;

}
vector<poligono*> interseccao_entre_poligonos(poligono* p1, poligono* p2,char operacao){
    vector<poligono*> poligonos_interseccao;
    poligono* p1_aux = p1;
    poligono* p2_aux = p2;
    if(operacao == 'u'){
        p1_aux->polig = reoordena(p1_aux->polig,0);
        p2_aux->polig = reoordena(p2_aux->polig,0);
        /*vector<vector<Ponto*>> aux_buraco;
        for(vector<Ponto*> b : p1_aux->buracos){
            b = reoordena(b,1);
            aux_buraco.push_back(b);
        }
        p1_aux->buracos = aux_buraco;
        aux_buraco.clear();
        for(vector<Ponto*> b : p2_aux->buracos){
            b = reoordena(b,1);
            aux_buraco.push_back(b);
        }
        p2_aux->buracos = aux_buraco;*/

    }
    if(operacao == 'i'){
        p1_aux->polig = reoordena(p1_aux->polig,1);
        p2_aux->polig = reoordena(p2_aux->polig,1);
        /*vector<vector<Ponto*>> aux_buraco;
        for(vector<Ponto*> b : p1_aux->buracos){
            b = reoordena(b,0);
            aux_buraco.push_back(b);
        }
        p1_aux->buracos = aux_buraco;
        aux_buraco.clear();
        for(vector<Ponto*> b : p2_aux->buracos){
            b = reoordena(b,0);
            aux_buraco.push_back(b);
        }
        p2_aux->buracos = aux_buraco;*/

    }

    vector<vector<Ponto*>> pontos_interseccao = poligono_e_suas_interseccoes(p1_aux->polig, p2_aux->polig);
    vector<Ponto*> pontos_interseccao_pol1 = pontos_interseccao[0];
    vector<Ponto*> pontos_interseccao_pol2 = pontos_interseccao[1];
    printf("Pontos interseccao pol1:\n");
    imprime_pontos(pontos_interseccao_pol1);
    printf("Pontos interseccao pol2:\n");
    imprime_pontos(pontos_interseccao_pol2);

    
    //Aplica weiler-atherton
    pontos_interseccao_pol1 = marca_nao_visitados(pontos_interseccao_pol1);
    pontos_interseccao_pol2 = marca_nao_visitados(pontos_interseccao_pol2);
    
    
    poligonos_interseccao = weiler_atherton(pontos_interseccao_pol1, pontos_interseccao_pol2);
    if(operacao == 'b') return poligonos_interseccao;

    vector<poligono*> poligonos_interseccao_final;
    if (operacao == 'i'){
        //Pega os polígonos, mas agora com interseccao
        vector<vector<Ponto*>> buracos;
        if(!(p1->buracos.empty() && p2->buracos.empty()))
            buracos = junta_buracos(p1->buracos, p2->buracos);

         

        //Une Buracos
        
        //Retira buracos
        if (!buracos.empty()){
            vector<poligono*> aux_novos_poligonos_interseccoes;
            for(poligono* interseccao : poligonos_interseccao){
                vector<poligono*> aux_novos_poligonos_interseccoes = mescla_intersseccao_com_buracos(interseccao, buracos);
                poligonos_interseccao_final.push_back(aux_novos_poligonos_interseccoes[0]); //Não funciona pra mais de um
            }
        }
        
        //Adiciona buracos

        
        //poligono poligono_interseccao = poligono_e_suas_interseccoes_com_os_buracos(poligonos_interseccao, p1->buracos,p2->buracos);

        for (poligono* p : poligonos_interseccao){
            imprime_pontos(p->polig);
        }
    }
    else if (operacao == 'u'){
        
        //une buracos 2 e pol 1
        //Faz interseccao desse resultado com o buracos 2
        
        //reordena pol1 e pol2
        if(p1->buracos.empty() && p2->buracos.empty()){
            p1_aux->polig = reoordena(p1_aux->polig,1);
            p2_aux->polig = reoordena(p2_aux->polig,1);

            poligonos_interseccao_final = poligonos_interseccao;
            vector<vector<Ponto*>> buracos_poligonos_final;
            vector<poligono*> vec_pol_aux;
            poligono* pol_aux;
            for(vector<Ponto*> b : p1->buracos){
                b = reoordena(b,1);
                b = deixa_positivo(b);
                pol_aux->polig = b;
                vec_pol_aux = interseccao_entre_poligonos(p2_aux, pol_aux, 'b');
                
                for(poligono* p : vec_pol_aux){
                    printf("Adicionou um buraco no final\n");
                    buracos_poligonos_final.push_back(p->polig);
                }
            }
            for(vector<Ponto*> b : p2->buracos){
                                printf("RETORNOU ALGO DO BURACO?");

                b = reoordena(b,1);
                pol_aux->polig = b;
                vec_pol_aux = interseccao_entre_poligonos(p1_aux, pol_aux, 'b');
                imprime_pontos(vec_pol_aux[0]->polig);
                for(poligono* p : vec_pol_aux){
                    printf("Adicionou um buraco no final\n");
                    buracos_poligonos_final.push_back(p->polig);
                    
                }
            }
            poligonos_interseccao_final[0]->buracos = buracos_poligonos_final;
        }
    }


        //LEMBRAR DOS BURACOS INTERNOS TODO
    if(poligonos_interseccao_final.empty())
        poligonos_interseccao_final = poligonos_interseccao;
        
    return poligonos_interseccao_final;
}
