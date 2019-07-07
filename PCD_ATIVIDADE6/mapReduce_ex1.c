#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "omp.h"

#define N 10000000
#define tamLinha 100

typedef struct
{
    char *t[tamLinha];
}linha;

typedef struct
{
	FILE *f;
	int pos, MAX;
	linha *buffer;
}arquivo;

int gerarNumeros()
{
    int i = 20, j;
    srand((unsigned)time(NULL));

    FILE *resp;
    resp = fopen("1desordenado.txt", "w");

    for(i = 0; i < N; i++)
    {
        j = rand()%999999999;
        fprintf(resp,"%d\n", j);
    }

    fclose (resp);

    return 0;
}

void preencheBuffer(arquivo *arq, int K)
{
	int i;
	if(arq->f == NULL)
		return;

	arq->pos = 0;
	arq->MAX = 0;

	for(i = 0; i < K; i++)
	{
		if(!feof(arq->f))
		{
			fgets(&arq->buffer[arq->MAX].t, tamLinha, arq->f);
			if(feof(arq->f))
			{
				fclose(arq->f);
				arq->f = NULL;
				break;
			}
			(arq->MAX)++;
		}
		else
		{
			fclose(arq->f);
			arq->f = NULL;
			break;
		}
	}

}

void mergeArquivos(char *nome, int numArqs, int K)
{
	char novo[20];
	int i;
	linha *buffer;
	buffer = (linha *) malloc((K) * sizeof(linha));
	arquivo *arq;
	arq = (arquivo *) malloc(numArqs * sizeof(arquivo));

	for(i = 0; i < numArqs; i++)
	{
		sprintf(novo, "arq%d.txt", (i + 1));
		arq[i].f = fopen(novo, "r");
		arq[i].MAX = 0;
		arq[i].pos = 0;
		arq[i].buffer = (linha *) malloc(K * sizeof(linha));
		preencheBuffer(&arq[i], K);
	}

	int qtdBuffer = 0;
	char menor[tamLinha];


	while( procuraMenor(arq, numArqs, K, &menor) == 1)
	{
		strcpy(buffer[qtdBuffer].t, menor);
		qtdBuffer++;
		if(qtdBuffer == K)
		{
			salvaArquivo(nome, buffer, K);
			qtdBuffer = 0;
		}
	}

	if(qtdBuffer > 0)
		salvaArquivo(nome, buffer, qtdBuffer);

	for(i = 0; i < numArqs; i++)
		free(arq[i].buffer);

	free(arq);
	free(buffer);

}

void mergeSort (linha *vStr, linha *auxS, int esq, int dir)
{
    int meio,i;

    if(esq<dir)
    {
        meio=(esq+dir)/2;
        mergeSort(vStr,auxS,esq,meio);
        mergeSort(vStr,auxS,meio+1,dir);
        intercalar(vStr,auxS,esq,meio+1,dir);
    }
}

void salvaArquivo(char nome[20], linha *vStr, int max)
{
	int i;
	FILE *f = fopen(nome, "a+");
	for(i = 0; i < max; i++)
	{
		fprintf(f,"%s", vStr[i].t);
	}
	fclose(f);
}

int procuraMenor(arquivo *arq, int numArqs, int K, char menor[tamLinha])
{
	int i, aux = -1;

	for( i = 0; i < numArqs; i++)
	{
		if(arq[i].pos < arq[i].MAX)
		{
			if(aux == -1)
				aux = i;
			else
			{
				if( atoi(arq[i].buffer[arq[i].pos].t) < atoi(arq[aux].buffer[arq[aux].pos].t))
					aux = i;
			}
		}
	}

	if(aux != -1)
	{
		strcpy(menor, arq[aux].buffer[arq[aux].pos].t);
		(arq[aux].pos)++;
		if(arq[aux].pos == arq[aux].MAX)
		{
			preencheBuffer(&arq[aux], K);
		}
		return 1;
	}
	else
	{
		return 0;
	}

}


void intercalar (linha *vStr,linha *auxS,int ini1, int ini2,int fim2)
{
    int in1=ini1,in2=ini2,fim1=in2-1,au=0,i;

    while(in1<=fim1 && in2<=fim2)
    {
        if(atoi(vStr[in1].t) < atoi(vStr[in2].t))
        {
            strcpy(auxS[au++].t, vStr[in1++].t);
        }
        else
        {
            strcpy(auxS[au++].t, vStr[in2++].t);
        }
    }
    while(in1<=fim1)
    {
        strcpy(auxS[au++].t, vStr[in1++].t);
    }
    while(in2<=fim2)
    {
        strcpy(auxS[au++].t,  vStr[in2++].t);
    }

    for(i=0;i<au;i++)
    {
        strcpy(vStr[i+ini1].t,  auxS[i].t);
    }
}

int main()
{
    int k, i = 0, ii = 1, cont = 0, tam = 1200000 ;
    char nome[20];
    double tempoInicial, tempoFinal;

	gerarNumeros();

    linha *vStr;
	vStr = (linha *) malloc(tam * sizeof(linha));

	linha *auxS;
	auxS = (linha *) malloc(tam * sizeof(linha));

    FILE * pFile;
    pFile = fopen ("1desordenado.txt","rt");

    rewind(pFile);
    fgets(&vStr[i].t, tamLinha, pFile);

    tempoInicial = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
		while(feof(pFile) == 0)
		{
			i++;
			fgets(&vStr[i].t, tamLinha, pFile);
			if ((i) % (tam - 1) == 0)
			{
				if((i) > 0)
				{
					cont++;
					mergeSort(vStr, auxS, 0, (i));
					sprintf(nome, "arq%d.txt", cont);
					remove(nome);
					salvaArquivo(nome, vStr, (i + 1));
					i = -1;
				}
			}
		}
	}

    if(i > 0)
    {
    	cont++;
		mergeSort(vStr, auxS, 0, (i - 1));
	    sprintf(nome, "arq%d.txt", cont);
	    remove(nome);
	    salvaArquivo(nome, vStr, i);
	    i = 0;
	}

    fclose (pFile);

	k = tam / (cont);
	if(k == 0)
		k = 1;
	sprintf(nome, "1ordenado.txt", cont);
	remove(nome);
	mergeArquivos(nome, cont, k);
    tempoFinal = omp_get_wtime();

	for(i = 1; i <= cont; i++)
	{
		sprintf(nome, "arq%d.txt", i);
		remove(nome);
	}

    printf("\nTempo: %f\n",tempoFinal-tempoInicial);
	free(vStr);
    return 0;
}
