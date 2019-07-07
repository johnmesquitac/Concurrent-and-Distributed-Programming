#include <stdio.h>
#include <stdlib.h>
#define tam 1.0
#define dx 0.00001
#define dt 0.000001
#define T  0.01
#define kappa 0.000045
#define N 100000 
#define MAX_THREAD 64

__global__ void add(double *u, double *u_prev){
  int index = threadIdx.x + blockIdx.x * blockDim.x;
  if(index < N)
    u[index] = u_prev[index] + kappa*dt/(dx*dx)*(u_prev[index - 1] - 2 * u_prev[index] + u_prev[index + 1]);
}

int main(void) {
  double *tmp, *u, *u_prev;
  double *a, *b;
  double x, t;
  long int i, maxloc;
  int size = N * sizeof(double);
  float temp_mili = 0.0;
  cudaEvent_t start, stop;
  const int blocks = (N + MAX_THREAD -1)/MAX_THREAD;

  /* Alocando vetores */
  u = (double *) malloc((N + 1)*sizeof(double));
  u_prev = (double *) malloc((N + 1)*sizeof(double));
  cudaMalloc((void **)&a, size);
  cudaMalloc((void **)&b, size);

  printf("Inicio: qtde=%ld, dt=%g, dx=%g, dx²=%g, kappa=%f, const=%f\n",
         (N + 1), dt, dx, dx*dx, kappa, kappa*dt/(dx*dx));
  printf("Iteracoes previstas: %g\n", T/dt);

  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  x = 0;
  for (i = 0; i < N + 1; i++) {
    if (x <= 0.5) u_prev[i] = 200*x;
    else        u_prev[i] = 200*(1.-x);
    x += dx;
  }

  printf("dx=%g, x=%g, x-dx=%g\n", dx, x, x-dx);
  printf("u_prev[0,1]=%g, %g\n", u_prev[0], u_prev[1]);
  printf("u_prev[n-1,n]=%g, %g\n", u_prev[N-1], u_prev[N]);

  cudaEventRecord(start, 0);

  t = 0.;
  while (t<T) {
    x = dx;
    cudaMemcpy(a, u, size, cudaMemcpyHostToDevice);
    cudaMemcpy(b, u_prev, size, cudaMemcpyHostToDevice);
    add<<<blocks,MAX_THREAD>>>(a, b);
    cudaMemcpy(u, a, size, cudaMemcpyDeviceToHost);
    u[0] = u[N] = 0.; /* forca condicao de contorno */
    tmp = u_prev; u_prev = u; u = tmp; /* troca entre ponteiros */
    t += dt;
  }

  /* Calculando o maior valor e sua localizacao */
  maxloc = 0;
  for (i=1; i < N+1; i++) {
    if (u[i] > u[maxloc]) maxloc = i;
  }

  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&temp_mili, start, stop);

  printf("Maior valor u[%ld] = %g\n", maxloc, u[maxloc]);

  printf("Tempo = %f\n", temp_mili);

  cudaEventDestroy(start);
        cudaEventDestroy(stop);

  free(u);
  free(u_prev);
  cudaFree(a);
  cudaFree(b);

  return 0;
}



