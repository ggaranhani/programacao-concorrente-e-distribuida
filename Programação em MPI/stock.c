#include <stdio.h>

#define N 100000000
#define u 2.0

int main(void) {
    double passo, soma,x;
    int i;

    passo = (u-1) / (double) N;
    for (i=0; i<N;i++) {
        x=1+i*passo;
        soma=soma+0.5*(1/x+1/(x+passo));
        }

    printf("ln %f = %20.15f\n", u, passo*soma);
    return 0; 
}