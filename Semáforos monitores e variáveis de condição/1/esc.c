#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#define N 100000
// #define N 12345678

pthread_mutex_t lock;
int MAX_THREADS;
double  A[N],
        B[N],
        soma;
        
void *soma_parcial(void *id){
    long iter;
  
    double parcial = 0;
    for(iter = (long)id; iter < N; iter += MAX_THREADS){
        parcial += A[iter]*B[iter];
    }
    printf("Soma parcial da thread %ld: %lf\n", (long)id, parcial);
    pthread_mutex_lock(&lock);
    	soma += parcial;
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main(int argc, char **argv){

    if(argc > 1){
		MAX_THREADS = atoi(argv[1]);
	} else {
		printf("A QUANTIDADE DE THREADS NAO FOI COLOCADA COMO ARGUMENTO!\n");
		return 0;
	}

 	struct timeval	inicio, final;
	int		tmili;
    long    i;

	printf("Número de threads: %d\n", MAX_THREADS);
    
    soma = 0;
    double a = 100;
    pthread_t tid[MAX_THREADS];
    srand((unsigned)time(NULL));

    double ha = 0;

    for(i = 0; i < N; i++){
        A[i] = (double)rand()/(double)(RAND_MAX/a);
        B[i] = (double)rand()/(double)(RAND_MAX/a);
        ha += A[i]*B[i];
    }

    printf("Soma teste: %lf\n", ha);

	gettimeofday(&inicio, NULL);
    for(i = 0; i < MAX_THREADS; i++)
        pthread_create(&tid[i], NULL, soma_parcial, (void *)i);
    for(i = 0; i < MAX_THREADS; i++)
        pthread_join(tid[i], NULL);
    
    gettimeofday(&final, NULL);

    printf("SOMA: %lf\n", soma);
    
	// tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
	tmili =  (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
	printf("Tempo de execucao (gettimeofday):  %f milissegundos\n", tmili); 

    FILE *time = fopen("time.txt", "a");
    fprintf(time, "%f\n", tmili);
    return 0;
}
