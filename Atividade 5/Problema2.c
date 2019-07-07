#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#define tam 1.0
#define dx 0.00001
#define dt 0.000001
#define T  0.01
#define N 0.000045

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double *u, *u_prev, *tmp, *maximos;
    int *maximosGlobalIndex;
    double x, t;
    int i;

    struct timeval inicio, fim;
    int timeDiff;

    long int size = tam/dx;
    int partitionSize = size/world_size;
    int first = world_rank*partitionSize;
    int last;
    int maximoIndex;
    double maximo;
    double maximoLocal;
    int maxlocIndex;
    int maxglobIndex;
    if (world_rank == world_size - 1) {
      last = size -first;
    } else {
      last = floor((float)(size)/world_size);
    }

    u = (double *) malloc((size+1)*sizeof(double));
    u_prev = (double *) malloc((size+1)*sizeof(double));
    maximos = (double *) malloc((world_size+1)*sizeof(double));
    maximosGlobalIndex = (int *) malloc((world_size+1)*sizeof(int));


    if (world_rank == 0) {
      printf("Inicio: qtde=%ld, dt=%g, dx=%g, dx²=%g, N=%f, const=%f\n",
    	 (size+1), dt, dx, dx*dx, N, N*dt/(dx*dx));
      printf("Iteracoes previstas: %g\n", T/dt);
      gettimeofday(&inicio, NULL); //começa a contar tempo
    }

    x = dx*first + dx;

    for (i=1; i<last; i++) {
      if (x <= 0.5)
        u_prev[i] = 200*x;
      else
        u_prev[i] = 200*(1.-x);
        x += dx;
    }

    t = 0.;
    while (t < T) {
      x = dx*world_rank*first + dx;
      for (i=1; i<last; i++) {
        u[i] = u_prev[i] + N*dt/(dx*dx)*(u_prev[i-1]-2*u_prev[i]+u_prev[i+1]);
        x += dx;
      }
      u[0] = u[last] = 0.;
      tmp = u_prev;
      u_prev = u;
      u = tmp;
      t += dt;
    }

    maximoLocal = u[0];
    maxlocIndex = 0;
    for (i=1; i<last+1; i++) {
      if (u[i] > u[maxlocIndex])
        maxlocIndex = i;
    }
    maximoLocal = u[maxlocIndex];

    if (world_rank != 0) {
      MPI_Send(&maximoLocal, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&maxlocIndex, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } else {
        for (i = 1; i<world_size; i++) {
          MPI_Recv(&maximos[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(&maximosGlobalIndex[i], 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        maximoIndex = maxlocIndex;
        maximos[0] = maximoLocal;
        maxglobIndex = 0;
        maximo = maximoLocal;
        for (i=1; i<world_size; i++) {
          if (maximos[i] > maximos[maxglobIndex]) {
            maximoIndex = maximosGlobalIndex[i];
            maximo = maximos[i];
            maxglobIndex = i;
          }
        }
        gettimeofday(&fim, NULL);
        timeDiff = (int) (1000 * (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec) / 1000);
        printf("Maior valor: u[%d] = %f\n", maximoIndex, maximo);
        printf("Tempo decorrido: %d milisegundos\n", timeDiff);
    }

    MPI_Finalize();

    return(0);
}
