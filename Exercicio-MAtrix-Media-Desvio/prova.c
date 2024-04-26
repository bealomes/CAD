#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Definir tamanho da matriz
#define N 6

int main(){
    //int A[N][N];
    float medial;
    int abaixoMedia[N];
    int dsvpdr[N];


    int A[N][N] =
{
    {9, 8, 4, 5,3,1},
    {4, 12, 20, 40,8,10},
    {8, 8, 4, 4,9,9},
    {8, 12, 4, 21,20,14},
    {33, 44, 20, 1,7,2},
    {10, 18, 17, 10,6,9}
};


    

    //Iniciar com 0 os vetors
    #pragma omp parallel for 
    for (int i = 0; i < N; i++)
    {
     abaixoMedia[i]=0;
     dsvpdr[i] = 0;  
    }
    

/*
    //Entrada de valores
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            scanf("%d", &A[i][j]);
        } 
    }

    */

    //Calcular a média
    float soma = 0;
    #pragma omp parallel for reduction(+:soma) collapse(2) 
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            soma += A[i][j];
        } 
    }
    medial = soma/(N*N);

    //Calculae o desvio padrão
    #pragma omp parallel for 
    for (int i = 0; i < N; i++){
        double desvio = 0;
        #pragma omp parallel for reduction(+:desvio)
        for (int j = 0; j < N; j++){
           desvio+=  ((A[i][j]-medial)*(A[i][j]-medial))/N;
        } 
        dsvpdr[i] = sqrt(desvio);
    }

    //verificar quantos estão abaixo da média em cada coluna
    #pragma omp parallel for 
    for (int i = 0; i < N; i++){
        #pragma omp parallel for reduction(+:abaixoMedia[i])
        for (int j = 0; j < N; j++){
            if (A[i][j] < medial){
                abaixoMedia[i]+= 1;
            }
        } 
    }



    // Imprimir média
    printf("\nMédia: %f\n", medial);

    // Imprimir desvio padrão
    printf("\nDesvio Padrão:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", dsvpdr[i]);
    }
    printf("\n");

    // Imprimir elementos abaixo da média em cada linha
    printf("\nQuantidade de elementos abaixo da média em cada linha:\n");
    for (int i = 0; i < N; i++) {
        printf("Linha %d: %d\n", i + 1, abaixoMedia[i]);
    }

    
    


}

