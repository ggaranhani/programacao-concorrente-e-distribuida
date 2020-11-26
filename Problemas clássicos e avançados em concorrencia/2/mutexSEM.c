#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#define N 10

pthread_cond_t produzir = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumir = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int produtos[N], p, c, NP, NC, count;

void *produtor(void *n){
	while(1){
		pthread_mutex_lock(&mutex);
		while(count == N)
			pthread_cond_wait(&produzir, &mutex);

		produtos[p] = rand()%100000000;
		printf("Prod(%ld) --> %d\n", (long)n, produtos[p]);
		p = (p+1)%N;
		count++;

		if(count == 1)
			pthread_cond_broadcast(&consumir);
		pthread_mutex_unlock(&mutex);
	}
}

void *consumidor(void *n){
	while(1){
		sleep(1);
		pthread_mutex_lock(&mutex);
		while(count == 0)
			pthread_cond_wait(&consumir, &mutex);

		//produtos[c] = 0;	
		printf("Cons(%ld) <-- %d\n", (long)n, produtos[c]);
		c = (c+1)%N;
		count--;

		if(count == N-1)
			pthread_cond_broadcast(&produzir);
		pthread_mutex_unlock(&mutex);		
	}
}

int main(int argc, char **argv){
	if(argc < 2){
		NP = 1; 
		NC = 1;
		printf("Quantidade de produtores e consumidores nao inserida como argumento\n"
            "Iniciando com 1 produtor e 1 consumidor\n");
	}
	else{
		NP = atoi(argv[1]);
		NC = atoi(argv[2]);
		printf("Iniciando com %d Produtores e %d Consumidores\n", NP, NC);
	}
	p = 0; c = 0; count = 0;
	pthread_t prod[NP], cons[NC];
	srand(time(NULL));
	long i, j;

	for(i = 0; i < NP; i++){
		pthread_create(&prod[i], NULL, produtor, (void *)(i+1));
	}
	for(j = 0; j < NC; j++){
		pthread_create(&prod[j], NULL, consumidor, (void *)(j+1));
	}
	for(i = 0; i < NP; i++){
		pthread_join(prod[i], NULL);
	}
	for(j = 0; j < NC; j++){
		pthread_join(cons[j], NULL);
	}
	return 0;
}
