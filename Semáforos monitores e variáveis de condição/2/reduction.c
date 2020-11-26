#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#define N 10000000

    struct timeval  inicio, final;
    int     tmili;

    int     i,
            MAX_THREADS;
    double  soma = 0,
            a = 100,
            A[N],
            B[N];

int main(int argc, char **argv){

    if(argc > 1)
            MAX_THREADS = atoi(argv[1]);
	else 
	    MAX_THREADS = 1;
	
     printf("N THREADS: %d\n", MAX_THREADS);
     omp_set_num_threads(MAX_THREADS);

    srand((unsigned)time(NULL));
    for(i = 0; i < N; i++){
        A[i] = (double)rand()/(double)(RAND_MAX/a);
        B[i] = (double)rand()/(double)(RAND_MAX/a);
    }

    gettimeofday(&inicio, NULL);
    #pragma omp parallel for reduction(+:soma)
    for(i = 0; i < N; i++){
            soma += A[i]*B[i];    
    }

    gettimeofday(&final, NULL);
    
    printf("SOMA: %lf\n", soma);

    tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
    printf("Tempo de execucao (gettimeofday):  %d milissegundos\n", tmili);

    FILE *time = fopen("time.txt", "a");
    fprintf(time, "%d\n", tmili);

    return 0;
}