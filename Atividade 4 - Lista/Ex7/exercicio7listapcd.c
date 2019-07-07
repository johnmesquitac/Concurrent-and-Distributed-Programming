#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include <unistd.h>
#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif
#define MAX_CAR_THREAD 2 //capacidade dos carros 2 em cada um
#define MAX_PASS_THREAD 16 //o numero de passageiros deve ser 4 vezes o tamanho da soma da capacidade dos dois carros

int parquefechou=0,volta=1,existemPass=0,entra=0;;
pthread_t pass[MAX_PASS_THREAD],car[MAX_CAR_THREAD];
pthread_mutex_t p1,p2,p3,p4,p5,p6,p7,p8;

void verifica()
{
    printf("verificando,volta= %d\n",volta);
    if(volta==10){
        parquefechou=1;
        existemPass=0;
        pthread_mutex_lock(&p8);
    }
}
void entraNoCarro()
{
    entra++;
}

void daUmaVolta()
{
     Sleep(1);
    pthread_mutex_unlock(&p4);
}
void esperaEncher()
{//printf("esperando encher \n");
    if(entra==2)
    {
       // printf("esperando encher, entra= %d \n",entra);
        pthread_mutex_unlock(&p3);
    }

}

 void esperaVoltaAcabar()
 {
     // printf("esperando volta acabar \n");
     Sleep(1);
     pthread_mutex_unlock(&p5);
 }

 void saiDoCarro()
 {
    // printf("saindo do carro \n");
     entra--;
 }

 void esperaEsvaziar()
 {
     // printf("esperando esvaziar \n");
     if(entra==0)
        pthread_mutex_unlock(&p6);
 }
void passeiaPeloParque()
{
    // printf("passeando \n");
     Sleep(1);
}
void *passageiro()
{
    existemPass++;
     while(parquefechou==0) {
        pthread_mutex_lock(&p2);
        entraNoCarro();
        pthread_mutex_unlock(&p1);
        pthread_mutex_lock(&p4);
        esperaVoltaAcabar();
        pthread_mutex_lock(&p5);
        saiDoCarro();
        if(entra==0)
            pthread_mutex_unlock(&p7);
        passeiaPeloParque();
}
}

void *carro()
{
    pthread_mutex_lock(&p8);
     while (existemPass){
         verifica();
         if(entra==2)
            pthread_mutex_unlock(&p2);
         pthread_mutex_lock(&p1);
         esperaEncher();
         pthread_mutex_lock(&p3);
         daUmaVolta();
         pthread_mutex_lock(&p7);
         esperaEsvaziar();
         pthread_mutex_lock(&p6);
         volta++; // serve como parametro para fechar o parque
         pthread_mutex_unlock(&p8);
}}

int main(void)
{
    int i;
    pthread_mutex_unlock(&p8);
	pthread_mutex_lock(&p1);
	pthread_mutex_lock(&p2);
	pthread_mutex_lock(&p3);
	pthread_mutex_lock(&p4);
	pthread_mutex_lock(&p5);
	pthread_mutex_lock(&p6);
	pthread_mutex_lock(&p7);
	pthread_mutex_lock(&p8);

    for(i=0;i<MAX_PASS_THREAD;i++){
        pthread_create(&pass[i],NULL,passageiro,NULL);}
    for(i=0;i<MAX_CAR_THREAD;i++){
        pthread_create(&car[i],NULL,carro,NULL);}

	for(i=0;i<MAX_CAR_THREAD;i++){
        pthread_join(car[i],NULL);}

	for(i=0;i<MAX_PASS_THREAD;i++){
       pthread_join(pass[i],NULL);
	}
    }
