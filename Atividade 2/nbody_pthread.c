#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

/*
 * pRNG based on http://www.cs.wm.edu/~va/software/park/park.html
 */
#define MODULUS    2147483647
#define MULTIPLIER 48271
#define DEFAULT    123456789
#define MAX_THREADS 8

static long seed = DEFAULT;

double Random(void)
/* ----------------------------------------------------------------
 * Random returns a pseudo-random real number uniformly distributed
 * between 0.0 and 1.0.
 * ----------------------------------------------------------------
 */
{
  const long Q = MODULUS / MULTIPLIER;
  const long R = MODULUS % MULTIPLIER;
  long t;

  t = MULTIPLIER * (seed % Q) - R * (seed / Q);
  if (t > 0)
    seed = t;
  else
    seed = t + MODULUS;
  return ((double) seed / MODULUS);
}

/*
 * End of the pRNG algorithm
 */

typedef struct {
  double x, y, z;
  double mass;
} Particle;
typedef struct {
  double xold, yold, zold;
  double fx, fy, fz;
} ParticleV;
typedef struct inicio_fim{
    int inicio;
    int fim;
}Valor;

Particle  * particles1;   /* Particulas */
Particle  * particles2;   /* Particulas */
ParticleV * pv;          /* Velocidade da Particula */
int aux;
double max_f;
static double dt_old = 0.001;

void *InitParticles( Valor *x );
void *ComputeForces( Valor *x );
void *ComputeNewPos( Valor *x );

int main()
{

   pthread_t t[MAX_THREADS];
  //double time;
  int         npart,i,controle;
  double      sim_t;       /* Tempo de Simulacao */
  Valor inicio_fim[MAX_THREADS];

  scanf("%d",&npart);
  aux=npart;
  controle = aux/MAX_THREADS;
  //npart = 25000;
  /* Allocate memory for particles */
  particles1 = (Particle *) malloc(sizeof(Particle)*npart);
  pv = (ParticleV *) malloc(sizeof(ParticleV)*npart);
  /* Generate the initial values */

  for(i=0; i<MAX_THREADS; i++) {

  if(i==0){
    inicio_fim[i].inicio = i * controle;
  }
  else{
    inicio_fim[i].inicio = ((i * controle) + 1);
  }
  inicio_fim[i].fim = (i+1) * controle;
  pthread_create(&t[i], NULL,InitParticles, &inicio_fim[i]);
 }

 for(i=0;i<MAX_THREADS;i++){
    pthread_join(t[i],NULL);
 }

  struct timeval inicio, final2;
  int tmili;

  int tid;

  gettimeofday(&inicio, NULL);

  for(i=0; i<MAX_THREADS; i++) {
  if(i==0){
    inicio_fim[i].inicio = i * controle;
  }
  else{
    inicio_fim[i].inicio = ((i * controle) + 1);
  }
  inicio_fim[i].fim = (i+1) * controle;
  pthread_create(&t[i], NULL,ComputeForces, &inicio_fim[i]);
 }

 for(i=0;i<MAX_THREADS;i++){
    pthread_join(t[i],NULL);
 }

 for(i=0; i<MAX_THREADS; i++) {
  if(i==0){
    inicio_fim[i].inicio = i * controle;
  }
  else{
    inicio_fim[i].inicio = ((i * controle) + 1);
  }
  inicio_fim[i].fim = (i+1) * controle;
  pthread_create(&t[i], NULL,ComputeNewPos, &inicio_fim[i]);
 }

 for(i=0;i<MAX_THREADS;i++){
    pthread_join(t[i],NULL);
 }
  /* Compute forces (2D only) */
  //ComputeForces();
  /* Once we have the forces, we compute the changes in position */
  //ComputeNewPos();

  gettimeofday(&final2, NULL);
  tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) +
		 (final2.tv_usec - inicio.tv_usec) / 1000);

  //for (i=0; i<npart; i++)
  //fprintf(stdout,"%.5lf %.5lf %.5lf\n", particles[i].x, particles[i].y, particles[i].z);

  printf("%g\n", max_f);
  printf("%g\n", dt_old);

  printf("%d\n", tmili);

  return 0;
}

void *InitParticles(Valor *x)
{
  int i;
  for (i=x->inicio; i<x->fim; i++) {
    particles1[i].x	  = Random();
    particles1[i].y	  = Random();
    particles1[i].z	  = Random();
    particles1[i].mass = 1.0;
    pv[i].xold	  = particles1[i].x;
    pv[i].yold	  = particles1[i].y;
    pv[i].zold	  = particles1[i].z;
    pv[i].fx	  = 0;
    pv[i].fy	  = 0;
    pv[i].fz	  = 0;
  }
  pthread_exit(NULL);
}

void *ComputeForces( Valor *x )
{
  int i;
  max_f = 0.0;

  int j;
  double xi, yi, rx, ry, mj, r, fx, fy, rmin;

  for (i=x->inicio; i<x->fim; i++) {
    rmin = 100.0;
    xi   = particles1[i].x;
    yi   = particles1[i].y;
    fx   = 0.0;
    fy   = 0.0;
    for (j=0; j<aux; j++) {
      rx = xi - particles1[j].x;
      ry = yi - particles1[j].y;
      mj = particles1[j].mass;
      r  = rx * rx + ry * ry;
      /* ignore overlap and same particle */
      if (r == 0.0) continue;
      if (r < rmin) rmin = r;
      r  = r * sqrt(r);
      fx -= mj * rx / r;
      fy -= mj * ry / r;
    }
    pv[i].fx += fx;
    pv[i].fy += fy;
    fx = sqrt(fx*fx + fy*fy)/rmin;
    if (fx > max_f) max_f = fx;
  }
}

void *ComputeNewPos( Valor *x )
{
  int i;
  double a0, a1, a2;
  static double dt = 0.001;
  double dt_new;
  a0	 = 2.0 / (dt * (dt + dt_old));
  a2	 = 2.0 / (dt_old * (dt + dt_old));
  a1	 = -(a0 + a2);

  for (i=x->inicio; i<x->fim; i++) {
    double xi, yi;
    xi	           = particles1[i].x;
    yi	           = particles1[i].y;
    particles1[i].x = (pv[i].fx - a1 * xi - a2 * pv[i].xold) / a0;
    particles1[i].y = (pv[i].fy - a1 * yi - a2 * pv[i].yold) / a0;
    pv[i].xold     = xi;
    pv[i].yold     = yi;
    pv[i].fx       = 0;
    pv[i].fy       = 0;
  }
  dt_new = 1.0/sqrt(max_f);
  /* Set a minimum: */
  if (dt_new < 1.0e-6) dt_new = 1.0e-6;
  /* Modify time step */
  if (dt_new < dt) {
    dt_old = dt;
    dt     = dt_new;
  }
  else if (dt_new > 4.0 * dt) {
    dt_old = dt;
    dt    *= 2.0;
  }
}
