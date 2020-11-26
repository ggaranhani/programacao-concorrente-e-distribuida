#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define qtdThread 2

pthread_t t[qtdThread];
pthread_mutex_t sem1;
pthread_mutex_t sem2;
pthread_mutex_t sem3;

void *show2(){

    for(int i = 0; i < 10; i++){
		pthread_mutex_lock(&sem1);
		pthread_mutex_unlock(&sem2);
		printf(" CAR");
		pthread_mutex_lock(&sem1);
		printf(" IS FUN TO WATCH\n");
		pthread_mutex_unlock(&sem3);
    }
  pthread_exit(NULL);
}

void *show1(){
    for(int i = 0; i < 10; i++){
		printf("A");
		pthread_mutex_unlock(&sem1);
      	pthread_mutex_lock(&sem2);
		printf(" RACE");
		pthread_mutex_unlock(&sem1);
		pthread_mutex_lock(&sem3);
    }
  	pthread_exit(NULL);
}

void *exibe1(){

    for(int i = 0; i < 10; i++){
      pthread_mutex_lock(&sem1);
      printf("C");
      pthread_mutex_unlock(&sem2);
      printf("E");
      pthread_mutex_unlock(&sem3);
    }
  pthread_exit(NULL);
}

void *exibe2(){

    for(int i = 0; i < 10; i++){
      printf("A");
      pthread_mutex_unlock(&sem1);
      pthread_mutex_lock(&sem2);
      printf("R");
      pthread_mutex_lock(&sem3);
      printf("O\n");
    }
  pthread_exit(NULL);
}

int main(void){
	int i = 0;

	pthread_mutex_lock(&sem1);
	pthread_mutex_lock(&sem2);
	pthread_mutex_lock(&sem3);

	pthread_create(&t[0], NULL, show1, NULL);
	pthread_create(&t[1], NULL, show2, NULL);

	for(i = 0; i < qtdThread; i++){
		pthread_join(t[i], NULL);
	}

	pthread_exit(NULL);
	return 0;
}
