#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

pthread_mutex_t t1;
pthread_mutex_t t2;
pthread_mutex_t t3;
pthread_mutex_t t4;

pthread_t ta,tb;
int cont=0;
void *P1()
{
	while(cont<10){
		pthread_mutex_lock(&t1);
		printf("C");
		pthread_mutex_unlock(&t3);
         		sleep(1);
		printf("E");
		pthread_mutex_unlock(&t4);}
}

void *P2()
{
	while(cont<10){
	pthread_mutex_lock(&t2);
	printf("A");
	pthread_mutex_unlock(&t1);
	pthread_mutex_lock(&t3);
              sleep(1);
	printf("R");
	pthread_mutex_lock(&t4);
	printf("O\n");
 	cont++;
	pthread_mutex_unlock(&t2);
	}

}

int main(void)
{
	pthread_mutex_unlock(&t2);
	pthread_mutex_lock(&t1);
	pthread_mutex_lock(&t3);
	pthread_mutex_lock(&t4);

	pthread_create(&ta,NULL,P1,NULL);
	pthread_create(&tb,NULL,P2,NULL);
	pthread_join(ta, NULL);
	pthread_join(tb, NULL);

}

