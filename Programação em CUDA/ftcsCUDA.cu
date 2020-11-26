#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
//#include <sys/time.h>

int MAX_THREADS;
#define tam 1.0
#define dx 0.00001
#define dt 0.000001
#define T 0.01
#define kappa 0.000045

typedef long long int ind;

__global__ void calculo(double *u, double *prev, double k, double t, double x, int n) {
    ind i =  blockDim.x * blockIdx.x + threadIdx.x;
    if (i > 0 && i < (n - 1))
        u[i] = prev[i] + k * t / (x * x) * (prev[i-1] - 2 * prev[i] + prev[i+1]);
}

__global__ void contorno(double *u, int n) {
    ind i =  blockDim.x * blockIdx.x + threadIdx.x;
    if(i == 1) 
	u[0] = u[n] = 0.0;
}

int main(int argc, char **argv) {
  if(argc >= 2){
      MAX_THREADS = atoi(argv[1]);
  }
  else{
      MAX_THREADS = 64;
  }
  printf("INICIANDO COM %d THREADS\n", MAX_THREADS);
  double *tmp, *u, *u_prev;
  double x = dx, t;
  ind i, n, maxloc, size, num_blocos;
  double *d_u, *d_u_prev;
  float tempo = 0.0;
  cudaEvent_t start, stop;
  /* Calculando quantidade de pontos */
  n = tam / dx;
  num_blocos = ceil((n+1)/MAX_THREADS);

  /* Alocando vetores */
  u = (double *) malloc((n+1)*sizeof(double));
  u_prev = (double *) malloc((n+1)*sizeof(double));

  size = (n + 1) * sizeof(ind);
  cudaMalloc((void **) &d_u, size);
  cudaMalloc((void **) &d_u_prev, size);

  printf("Inicio: qtde=%ld, dt=%g, dx=%g, dx²=%g, kappa=%f, const=%f\n",
	 (n+1), dt, dx, dx*dx, kappa, kappa*dt/(dx*dx));
  printf("Iteracoes previstas: %g\n", T/dt);

  x = 0;
  for (i = 0; i < n+1; i++) {
      if (x <= 0.5)
          u_prev[i] = 200 * x;
      else
          u_prev[i] = 200 * (1.-x);
      x += dx;
  }

  printf("dx=%g, x=%g, x-dx=%g\n", dx, x, x-dx);
  printf("u_prev[0,1]=%g, %g\n",u_prev[0],u_prev[1]);
  printf("u_prev[n-1,n]=%g, %g\n",u_prev[n-1],u_prev[n]);

  cudaMemcpy(d_u_prev, u_prev, size, cudaMemcpyHostToDevice);

  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start, 0);

  t = 0.;
  while (t < T) {
	x = dx;
	calculo <<<num_blocos, MAX_THREADS>>> (d_u, d_u_prev, kappa, dt, dx, n+1);
	cudaDeviceSynchronize();
	contorno <<<num_blocos, MAX_THREADS>>> (d_u, n);
	tmp = d_u_prev; 
	d_u_prev = d_u;
	d_u = tmp;
	t += dt;
  }

  cudaMemcpy(u, d_u, size, cudaMemcpyDeviceToHost);

  /* Calculando o maior valor e sua localizacao */
  maxloc = 0;
  for (i = 1; i < n+1; i++)
      if (u[i] > u[maxloc]) maxloc = i;

  printf("Maior valor u[%ld] = %g\n", maxloc, u[maxloc]);

  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&tempo, start, stop);
  printf("Tempo total: %f ms\n", tempo);
  cudaEventDestroy(start);
        cudaEventDestroy(stop);

  free(u);
  free(u_prev);
  cudaFree(d_u);
  cudaFree(d_u_prev);

  return 0;
}
