#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define PI 3.14159265
#define N 10000000
#define MAX_THREADS 8

typedef struct inicio_fim{
    int inicio;
    int fim;
    double maior;
}Valor;

struct timeval inicio, final2;

double *r1, *r2, *o1, *o2, *res;

void *calcula_distancia(Valor *x){


    int k;
    x->maior = 0.0;

    for(k=x->inicio;k<x->fim;k++){
        res[k] = sqrt((pow(r1[k],2) + pow(r2[k],2)) - (2*r1[k]*r2[k]*cos((o1[k]-o2[k])*PI/180)));

        if(x->maior < res[k]){
            x->maior = res[k];
        }
    }

    pthread_exit(NULL);

}


int main(void){

    gettimeofday(&inicio, NULL);

    pthread_t t[MAX_THREADS];
    int i, controle;
    Valor inicio_fim[MAX_THREADS];
    int tmili;
    double maior_total=0.0;

    controle = N/MAX_THREADS;

    r1 = malloc(sizeof(double) * N);
    r2 = malloc(sizeof(double) * N);
    o1 = malloc(sizeof(double) * N);
    o2 = malloc(sizeof(double) * N);
    res = malloc(sizeof(double) * N);


    for(i=0; i<N; i++){
        r1[i] = ((double) rand()/(double)RAND_MAX)*100;
        r2[i] = ((double) rand()/(double)RAND_MAX)*100;
        o1[i] = ((double) rand()/(double)RAND_MAX)*360;
        o2[i] = ((double) rand()/(double)RAND_MAX)*360;
    }



    for (i=0;i<MAX_THREADS;i++){
        inicio_fim[i].inicio = i * controle;
        inicio_fim[i].fim = (i+1) * controle;
        pthread_create(&t[i], NULL, calcula_distancia, &inicio_fim[i]);
    }

    for (i=0;i<MAX_THREADS;i++){
        pthread_join(t[i],NULL);
    }

    for (i=0;i<MAX_THREADS;i++){
        if(maior_total < inicio_fim[i].maior){
            maior_total = inicio_fim[i].maior;
        }
    }
    gettimeofday(&final2, NULL);

    tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

    printf("\ntempo decorrido: %d milisegundos e a maior distancia eh %lf\n", tmili, maior_total );

    return 0;
}

