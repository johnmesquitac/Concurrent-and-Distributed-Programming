#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#define N 1000000
#define nthread 8

double cont=0;
int main(void)
{
    int i,tnp=0,timili=0,total=0,tfinal=0;
    double *A,*B,calc=0;
    struct timeval in, fim;
    struct timeval in2,fim2;

    A= malloc(sizeof(double) * N);
    B= malloc(sizeof(double) * N);

    gettimeofday(&in2, NULL);
    for(i=0;i<N;i++)
    {
        A[i] =((double) rand()/(double)RAND_MAX)*100;
        B[i] =((double) rand()/(double)RAND_MAX)*100;
    }
    gettimeofday(&fim2, NULL);

    gettimeofday(&in, NULL);

    #pragma omp parallel private(i,calc) num_threads(nthread)
   {
    #pragma omp for
    for(i=0;i<N;i++)
        calc+=A[i]*B[i];
    #pragma omp critical
	cont+=calc;
    }

   gettimeofday(&fim, NULL);

   timili= (int) (1000 * (fim2.tv_sec - in2.tv_sec) +
		 (fim2.tv_usec - in2.tv_usec) / 1000);

   tnp = (int) (1000 * (fim.tv_sec - in.tv_sec) +
		 (fim.tv_usec - in.tv_usec) / 1000);
  tfinal=timili+tnp;

  printf("tempo paralelo: %d milisegundos, %f%\n", tnp, (float)tnp/tfinal);// tempo serial, porcentagem
  printf("tempo serial:%d milisegundos, %f%\n", timili, (float)timili/tfinal);//Tempo paralelo, porcentagem
  printf("tempo total:%dmilisegundos\n", tfinal);//Tempo total

   printf("resultado:%f\n",cont);

free(A);
free(B);
}
