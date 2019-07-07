#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>

#define N 100000000
#define MAX_THREADS 4

typedef struct inicio_fim{
    int inicio;
    int fim;
    double maior;
}Valor;

sem_t mutex;

struct timeval inicio, final2;

double *r1, *r2;
double total;

void *calcula_distancia(Valor *x){

    int k;
    double total_parcial = 0.0;

    for(k=x->inicio;k<x->fim;k++){
        total_parcial += r1[k]*r2[k];
    }

    sem_wait(&mutex);
    total += total_parcial;
    sem_post(&mutex);

    pthread_exit(NULL);

}

void inicializa(void){
	sem_init(&mutex, 0, 1);
	total = 0.0;
}


int main(void){


    pthread_t t[MAX_THREADS];
    int i, controle;
    Valor inicio_fim[MAX_THREADS];
    int tmili;
    double maior_total=0.0;

    controle = N/MAX_THREADS;

    r1 = malloc(sizeof(double) * N);
    r2 = malloc(sizeof(double) * N);

    inicializa();


    for(i=0; i<N; i++){
        r1[i] = ((double) rand()/(double)RAND_MAX)*100;
        r2[i] = ((double) rand()/(double)RAND_MAX)*100;
    }


    gettimeofday(&inicio, NULL);

    for (i=0;i<MAX_THREADS;i++){
        inicio_fim[i].inicio = i * controle;
        inicio_fim[i].fim = (i+1) * controle;
        pthread_create(&t[i], NULL, calcula_distancia, &inicio_fim[i]);
    }

    for (i=0;i<MAX_THREADS;i++){
        pthread_join(t[i],NULL);
    }

    gettimeofday(&final2, NULL);
    tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

    /*printf("Vetor 1: ");
    for(i=0;i<N;i++){
        printf("%.2f ", r1[i]);
    }*/

    //printf("\n\n");

    /*printf("Vetor 2: ");
    for(i=0;i<N;i++){
        printf("%.2f ", r2[i]);
    }*/

    printf("\n\n");

    printf("total: %.2f\n", total);

    printf("\n\t tempo decorrido: %d milisegundos\n", tmili);

    return 0;
}
