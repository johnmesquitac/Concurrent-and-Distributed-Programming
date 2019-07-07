#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "omp.h"

#define tamLinha 500

typedef struct
{
    int id;
    int qtd;
}Titem;

typedef struct
{
    Titem item[1000];
    int tam;
}Tservico;

void Inicia(Tservico *vet)
{
    for(int i = 0; i < 1000; i++)
    {
        vet->item[i].qtd = 0;
    }
}

void Mostrar(Tservico *vet)
{
    int valor;
    Titem aux;
    for(int i = 1; i < vet->tam; i++)
    {
        for(int j = 0; j < vet->tam-1; j++)
        {
            if(vet->item[j].id > vet->item[j+1].id)
            {
                aux = vet->item[j];
                vet->item[j] = vet->item[j+1];
                vet->item[j+1] = aux;
            }
        }
    }

    for(int i = 0; i < vet->tam; i++)
    {
        valor = vet->item[i].qtd;
        printf("%d apareceu %d vezes\n", vet->item[i].id, vet->item[i].qtd);

    }
}

void Inserir(Tservico *vet, int valor)
{
    for(int i = 0; i < 1000; i++)
    {
        if(vet->item[i].qtd == 0)
        {
            vet->item[i].id = valor;
            vet->item[i].qtd++;
            vet->tam++;
            break;
        }
        else
        {
            if(vet->item[i].id == valor)
            {
                vet->item[i].qtd++;
                break;
            }
        }
    }
}

int main(){
    const char s[2] = ",";
    char *token;
    char *result;
    int cont = 0, posicao = 0, i = 0, max = 0;
    Tservico *vet = (Tservico*) malloc(sizeof(Tservico));
	result = (char *) malloc(tamLinha * sizeof(char));
    double tempoInicial, tempoFinal;

    Inicia(vet);

    FILE *arq;
    rewind(arq);
   token = strtok(arq, s);

   tempoInicial = omp_get_wtime();
   #pragma omp parallel
    {
        #pragma omp single
        for(i = 0; i < 350000; i++)
        {
            cont = 0;
            fgets(result, tamLinha, arq);

            if (result)
            token = strtok(result, s);

            while( token != NULL )
            {
                cont++;
                if(cont == 8)
                {
                    posicao = atoi(token);
                    if(posicao > 0)
                    {
                        #pragma omp critical
                        inserirItem(vet, posicao);
                    }
                }
                token = strtok(NULL, s);
            }
        }
    }
    tempoFinal = omp_get_wtime();
    fclose(arq);
    Mostrar(vet);

    printf("Tempo: %f\n",tempoFinal-tempoInicial);

    return(0);
}

