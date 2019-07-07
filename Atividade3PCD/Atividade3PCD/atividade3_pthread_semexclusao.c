#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <pthread.h>

#define MAX_THREADS 2
#define N 5

int request=0;
int respond=0;
int total=0;

void *Servidor_function(void){

    while(1){
            respond = request;
        }
    }


void *Consumidor_function(void){

    int i, id;
    for(i=0;i<N;i++){
        id = pthread_self();
        while(respond!=id){
            request = id;
        }
        if(respond==id){
        printf("Thread %d entrando na regiao critica!\n", pthread_self()-1);
        Sleep(500);
        printf("Thread %d saindo da regiao critica!\n\n", pthread_self()-1);
        Sleep(500);
        respond = 0;
        }
    }
}

int main(void){

    int i;
    pthread_t consumidores[MAX_THREADS];
    pthread_t servidor;

    pthread_create(&servidor, NULL, Servidor_function, NULL);

    for(i=0; i<MAX_THREADS; i++) {
        pthread_create(&consumidores[i], NULL, Consumidor_function, NULL);
    }

    for(i=0; i<MAX_THREADS; i++) {
    pthread_join(consumidores[i],NULL);
    }

    printf("\nTotal de acessos a RC: %d\n", total);

    return 0;
}
