#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#define N 10
#define C 5

pthread_cond_t embarcar = PTHREAD_COND_INITIALIZER;
pthread_cond_t andar = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t id;

int produtos[N], p, c, NP, NC, count, fechou = 0;

void *passageiro(void *n){
    int getout = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		while(count == C)
			pthread_cond_wait(&embarcar, &mutex);

        if(fechou < NC){
            printf("Passageiro (%ld) embarcou\n", (long)n);
            p = (p+1)%C;
            count++;
        }
        else{
            getout = 1;
        }

		if(count == C)
			pthread_cond_broadcast(&andar);
		pthread_mutex_unlock(&mutex);
        pthread_barrier_wait(&id);
        if(getout){
            return NULL;
        }
	}
}

void *carro(void *n){
    int voltas = 0;
    int getout = 0;
	while(1){
		sleep(1);
		pthread_mutex_lock(&mutex);
		while(count != C)
			pthread_cond_wait(&andar, &mutex);

        if(voltas < 10){
            printf("Carro (%ld) fez a volta: %d\n", (long)n, voltas+1);
            voltas++;
        }
        if(voltas == 10){
            getout = 1;
            fechou++;
        }
        count = 0;

		if(count == 0)
			pthread_cond_broadcast(&embarcar);
		pthread_mutex_unlock(&mutex);	
        if(getout){
            return NULL;
        }	
	}
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("Quantidade de produtores e consumidores nao inserida como argumento\n"
            "Iniciando com 1 Passageiros e 1 Carros\n");
		NP = 1; 
		NC = 1;
	}
	else{
		NP = atoi(argv[1]);
		NC = atoi(argv[2]);
		printf("Iniciando com %d Passageiros e %d Carros\n", NP, NC);
	}
    pthread_barrier_init(&id, NULL, NP);
	p = 0; c = 0; count = 0;
	pthread_t prod[NP], cons[NC];
	srand(time(NULL));
	long i, j;

	for(i = 0; i < NP; i++){
		pthread_create(&prod[i], NULL, passageiro, (void *)(i+1));
	}
	for(j = 0; j < NC; j++){
		pthread_create(&cons[j], NULL, carro, (void *)(j+1));
	}
	for(i = 0; i < NP; i++){
		pthread_join(prod[i], NULL);
        printf("passageiro %ld foi embora\n", i+1);
	}
	for(j = 0; j < NC; j++){
		pthread_join(cons[j], NULL);
        printf("carro %ld encerrou atividades\n", j+1);
	}
	return 0;
}
