#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 
#include <stdlib.h>

#define loop 5

int MAX_THREADS = 0;
//int cont[MAX_THREADS]

int cont;

int req_cliente,
    req_servidor,
    inRC,
    permissao,
    requisicao;

void *cliente(void *args){
    long    i = 1,
            my_id = (long)args;
    
    printf("Thread %ld iniciada\n", my_id);
    
    while(i <= loop){
        while(permissao != my_id){
            //printf("-- %ld requer acesso\n", my_id);
            requisicao = my_id;
            //sleep(1);
        }
        printf("Thread %ld dentro da SC -- %ld -- %d\n", my_id, i, cont);
        cont++;
        i++;
        permissao = 0;
        //sleep(1);
    }
    pthread_exit(NULL);
}

void *servidor(void *args){
    int     i = 1;
    long    my_id = (long)args;
    printf("Servidor criado - thread %ld\n", my_id);
    while(cont != MAX_THREADS*loop){
        if(permissao == 0){
            if(requisicao != 0){
                permissao = requisicao;
                printf("permissao para %d\n", permissao);
            }
            requisicao = 0;
            //sleep(1);
            i++;
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){

    permissao = 0; requisicao = 0; cont = 0;
    long i;

    if(argc > 1){
        MAX_THREADS = atoi(argv[1]);
    }
    else{
        printf("QUANTIDADE DE THREADS NAO INSERIDA COMO ARGUMENTO!\n");
        return 0;
    }

    pthread_t tid[MAX_THREADS+1];
    
    pthread_create(&tid[0], NULL, servidor, (void *)0);
    for(i = 0; i < MAX_THREADS; i++){
        pthread_create(&tid[i+1], NULL, cliente, (void *)i+1);
    }
    for(i = 0; i < MAX_THREADS+1; i++){
        pthread_join(tid[i], NULL);
        printf("thread %ld joined\n", i);
    }
    printf("QTD THREADS (%d) x LIMITE DE LOOPS POR THREAD (%d) = %d\nCONT FINAL: %d\n", MAX_THREADS, loop, MAX_THREADS*loop, cont);
    return 0;
}
