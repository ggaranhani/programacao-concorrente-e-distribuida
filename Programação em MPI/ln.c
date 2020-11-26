#include "mpi.h"
#include <stdio.h>

#define N 100000000
#define u 2.0


int main(int argc, char **argv) {
    double passo, soma = 0, x, somatotal = 0;
    int i;
    int world_rank, world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    passo = (u-1) / (double) N;    
    for (i = world_rank; i < N; i += world_size) {
        x = 1 + i*passo;
        soma = soma+0.5*(1/x+1/(x+passo));
    }

    MPI_Reduce(&soma, &somatotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("ln %f = %20.15f\n", u, passo*somatotal);
    }
    MPI_Finalize();
    return 0;
}
