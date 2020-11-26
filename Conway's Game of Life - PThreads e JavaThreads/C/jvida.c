#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#define dim 2048
#define SRAND_VALUE 1985
#define MAX_ITER 2000
#define MAX_THREADS 1

int **grid;
int **newgrid;
int count;

void bordas(){
	int i;
	for(i = 1; i < dim+1; i++){
		grid[0][i] = grid[dim][i];
		grid[dim+1][i] = grid[1][i];
		grid[i][0] = grid[i][dim];
		grid[i][dim+1] = grid[i][1];
	}
	grid[0][0] = grid[dim][dim];
	grid[dim+1][dim+1] = grid[1][1];
	grid[0][dim+1] = grid[dim][1];
	grid[dim+1][0] = grid[1][dim];
}

void grids_init(){
	
	int i, j;

	grid = malloc((dim+2)*sizeof(int *));
	newgrid = malloc((dim+2)*sizeof(int *));	
	for(i = 0; i < dim+2; i++){
		grid[i] = malloc((dim+2)*sizeof(int));	
		newgrid[i] = malloc((dim+2)*sizeof(int));
	}
	
	srand(SRAND_VALUE);
	
	for(i = 1; i < dim+1; i++){
		for(j = 1; j < dim+1; j++){
			grid[i][j] = rand() % 2;	
		}
	}
	bordas();
}

void *conta_vivos(void *n){
	count = 0;
	long i, j;

	for(i = (long)n; i < dim+1; i+=MAX_THREADS){
		for(j = 1; j < dim+1; j++){
			if(newgrid[i][j] == 1)
				count++;
		}
	}
	pthread_exit(NULL);
	
}

void conta_vivos2(){
	long i, j;
	
	for(i = 1; i < dim+1; i++){
		for(j = 1; j < dim+1; j++){
			if(newgrid[i][j] == 1)
				count++;
		}
	}
	
	printf("CELULAS VIVAS DEPOIS DE %d GERACOES: %d\n", MAX_ITER, count);
	pthread_exit(NULL);
	
}

void copia_grid2(){
	//conta_vivos();
	long i, j;
	for(i = 1; i < dim+1; i++){
		for(j = 1; j < dim+1; j++){
			grid[i][j] = newgrid[i][j];
		}
	}
	//bordas();
}

void *copia_grid(void *n){
	//conta_vivos();
	long i, j;
	for(i = (long)n; i < dim+1; i+=MAX_THREADS){
		for(j = 1; j < dim+1; j++){
			grid[i][j] = newgrid[i][j];
		}
	}
	//bordas();
	pthread_exit(NULL);
}


int getNeighbors(int i, int j){
	int vivos = 0;

	if(grid[i][j+1] == 1)
		vivos++;
	if(grid[i][j-1] == 1)
		vivos++;
	if(grid[i+1][j+1] == 1)
		vivos++;
	if(grid[i+1][j-1] == 1)
		vivos++;
	if(grid[i-1][j] == 1)
		vivos++;
	if(grid[i+1][j] == 1)
		vivos++;
	if(grid[i-1][j-1] == 1)
		vivos++;
	if(grid[i-1][j+1] == 1)
		vivos++;

	return vivos;
}

void print_grids(){
	int i, j;
	printf("\n\n");
	for(i = 0; i < dim+2; i++){
		for(j = 0; j < dim+2; j++){
			printf("%d ", grid[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for(i = 0; i < dim+2; i++){
		for(j = 0; j < dim+2; j++){
			printf("%d ", newgrid[i][j]);
		}
		printf("\n");
	}
}


void *simula(void *n){
	long i, j;
	int vivos;
	//printf("thread %ld\n", (long)n);
	for(i = (long)n; i < dim+1; i+=MAX_THREADS){
		for(j = 1; j < dim+1; j++){
			vivos = getNeighbors(i, j); 
			if(vivos < 2){
				newgrid[i][j] = 0;
			}
			else if(vivos >= 4){
				newgrid[i][j] = 0;
			}
			else if(vivos == 3 && grid[i][j] == 0){
				newgrid[i][j] = 1;	
			}			
			else if(grid[i][j] == 1){
				newgrid[i][j] = 1;
			}
		}
	}
	pthread_exit(NULL);
}	

void *func(void *n){
	printf("THREAD: %ld", (long)n);
	pthread_exit(NULL);
}
int main(){
	count = 0;
	printf("N THREADS: %d\n", MAX_THREADS);
	grids_init();
	//print_grids();
	long n = 0;
	int i;
	pthread_t thread[MAX_THREADS];
	clock_t	start, end;
	struct timeval inicio, final;
	int tmili;

	start = clock();
	gettimeofday(&inicio, NULL);

	for(i = 0; i < MAX_ITER; i++){
		
		for(n = 0; n < MAX_THREADS; n++){
			pthread_create(&thread[n], NULL, simula, (void *)(n+1));
		}
		for(n = 0; n < MAX_THREADS; n++){
			pthread_join(thread[n], NULL);
		}

		
		for(n = 0; n < MAX_THREADS; n++){
			pthread_create(&thread[n], NULL, copia_grid, (void *)(n+1));
		}
		for(n = 0; n < MAX_THREADS; n++){
			pthread_join(thread[n], NULL);
			//copia_grid2();
		}
		//print_grids();
		//print_grids();
		
		bordas();
		
	}
	
	end = clock();
	gettimeofday(&final, NULL);	

	tmili = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
	
	printf("Tempo de execucao (clock): 	     %f segundos\n", (float)(end - start)/CLOCKS_PER_SEC); 
	printf("Tempo de execucao (gettimeofday):    %d milissegundos\n", tmili);

//	for(n = 0; n < MAX_THREADS; n++){
//	 	pthread_create(&thread[n], NULL, conta_vivos, (void *)(n+1));
//	}
//	for(n = 0; n < MAX_THREADS; n++){
//		pthread_join(thread[n], NULL);
//	}

	//printf("CELULAS VIVAS DEPOIS DE %d GERACOES: %d\n", MAX_ITER, count);
	conta_vivos2(); 

	return 0;
}
