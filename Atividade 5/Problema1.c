#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 100000000
#define u 2.0


int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double passo = (u-1) / (double) N;
    double soma, x, somatotal;
    soma = 0.0;
    somatotal = 0.0;
    int i;

    struct timeval inicio, fim;
    int timeDiff;

    if (world_rank == 0)
      gettimeofday(&inicio, NULL); //começa a contar tempo

    for (i=world_rank; i<N; i += world_size) {
      x = 1 + i*passo;
      soma = soma+0.5*(1/x + 1/(x+passo));
    }

    MPI_Reduce(&soma, &somatotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
      printf("ln %f = %20.15f\n", u, passo*somatotal);
      gettimeofday(&fim, NULL);
      timeDiff = (int) (1000 * (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec) / 1000);
      printf("Tempo decorrido: %d milisegundos\n", timeDiff);
    }


}
