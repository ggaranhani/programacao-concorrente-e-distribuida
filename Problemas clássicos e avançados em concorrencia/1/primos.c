#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define N 100
int TC, TP, fila[N+1], count, p, c, stay;
sem_t produzir, consumir;
pthread_barrier_t barreiraProd, barreiraCons;
pthread_mutex_t mutexP = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexC = PTHREAD_MUTEX_INITIALIZER;;

void init_sem(){
    pthread_barrier_init(&barreiraProd, NULL, TP);
    pthread_barrier_init(&barreiraCons, NULL, TC);
    sem_init(&produzir, 0, N);
    sem_init(&consumir, 0, 0);
}

int is_prime(int num){
    int i; 
    int div = 0;
    for (i = 1; i <= num; i++) {
        if (num % i == 0) { 
            div++;
        }
    }
    if (div == 2){
        return 1;
    }
    else{
        return 0;
    }       
}

void *produtor(void *n){
    int num;
    int getout = 0;
    while(1){
        sleep(1);
        num = rand()%100000000;

        pthread_mutex_lock(&mutexP);
        sem_wait(&produzir);

        if(p < N){ 
            fila[p] = num;
            printf("P%d - Produtor %ld criou: %d\n", p, (long)n, num);
            p++;
        }
        else{
            fila[p] = -1; 
            //printf("P%d - Produtor %ld esperando clientes encerrarem (%d)\n", p, (long)n, stay);   
            stay--;
        }
        if(stay < 0) getout = 1;

        sem_post(&consumir);
        pthread_mutex_unlock(&mutexP);
        //pthread_barrier_wait(&barreiraProd);

        if(getout){
            pthread_exit(NULL);
            //return NULL;
        }
    }
}

void *consumidor(void* n){
    int getout;
    while(1){
        getout = 1;
        pthread_mutex_lock(&mutexC);
        sem_wait(&consumir);
        
        if(fila[c] >= 0){
            if(is_prime(fila[c])){
                printf("C%d - Consumidor %ld recebeu PRIMO: %d\n", c, (long)n, fila[c]);
            }
            else{
                printf("C%d - Consumidor %ld recebeu: %d\n", c, (long)n, fila[c]);
            }
            getout = 0;
            c++;
        }

        sem_post(&produzir);
        pthread_mutex_unlock(&mutexC);
        //pthread_barrier_wait(&barreiraCons);

        if(getout){
            //printf("GETOUT!\n");
            pthread_exit(NULL);
        }
    }
}


int main(int argc, char **argv){
	p = 0; c = 0; count = 0;
	pthread_t prod[TP], cons[TC];
	long i;

    if(argc < 2){
        printf("Quantidade de produtores e consumidores nao inserida como argumento\n"
            "Iniciando com 1 produtor e 1 consumidor\n");
        TC = 1; TP = 1;
    }
    else{
        TP = atoi(argv[1]);
        TC = atoi(argv[2]);
        printf("Iniciando com %d Produtores e %d Consumidores\n", TP, TC);
    }
    stay = TC;
	srand(time(NULL));
    init_sem();

    for(i = 0; i < TP; i++){
		pthread_create(&prod[i], NULL, produtor, (void *)(i+1));
	}
	for(i = 0; i < TC; i++){
		pthread_create(&cons[i], NULL, consumidor, (void *)(i+1));
	}
	for(i = 0; i < TP; i++){
		pthread_join(prod[i], NULL);
        printf("produtor %ld encerrou atividades\n", i+1);
	}
	for(i = 0; i < TC; i++){
		pthread_join(cons[i], NULL);
        printf("consumidor %ld encerrou atividades\n", i+1);
	}
	return 0;
}