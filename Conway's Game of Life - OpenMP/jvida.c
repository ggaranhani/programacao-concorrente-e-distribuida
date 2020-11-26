#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#define dim 2048
#define SRAND_VALUE 1985
#define MAX_ITER 2000


int MAX_THREADS;
int **grid;
int **newgrid;
int count;

void bordas(){
	int i;
	#pragma omp parallel for
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

void conta_vivos(){
	count = 0;
	int i, j;
	for(i = 1; i < dim+1; i++){
		for(j = 1; j < dim+1; j++){
			if(newgrid[i][j] == 1)
				count++;
		}
	}	
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
}

void copia_grid2(){
	long i, j;
	for(i = 1; i < dim+1; i++){
		for(j = 1; j < dim+1; j++){
			grid[i][j] = newgrid[i][j];
		}
	}
}

void copia_grid(){
	int i, j;
	for(i = 1; i < dim+1; i++){
		for(j = 1; j < dim+1; j++){
			grid[i][j] = newgrid[i][j];
		}
	}
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


void simula(){
	long i, j;
	int vivos;
	#pragma omp parallel private(vivos, i, j)
	#pragma omp for
	for(i = 1; i < dim+1; i++){
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
}	

int main(int argc, char *argv[]){
	count = 0;
	if(atoi(argv[1]) > 0)
		MAX_THREADS = atoi(argv[1]);
	else 
		MAX_THREADS = 1;
	
	printf("N THREADS: %d\n", MAX_THREADS);
	omp_set_num_threads(MAX_THREADS);
	grids_init();
	int i;

	double ini = omp_get_wtime();
	for(i = 0; i < MAX_ITER; i++){
		simula();
		copia_grid2();
		bordas();
	}	
	double fim = omp_get_wtime();
	
	printf("Tempo de execucao: %lf segundos\n", (fim-ini)); 
	
	conta_vivos2(); 

	return 0;
}
