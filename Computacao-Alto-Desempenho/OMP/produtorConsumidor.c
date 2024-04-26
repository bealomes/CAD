#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


#define N_MAX_PRODUCAO 18
#define N_CONSUMIDORES 5
#define N_PRODUTORES 6
#define NUM_MAX_PROD_THREAD 8

int main(){
    //criar variável de produtos
    int num_produtos = 0;
    int max_produzido_thread[NUM_MAX_PROD_THREAD];

    #pragma omp paralel num_thread(N_CONSUMIDORES+N) shared(NUM_PRODUZIDO)
    {
        #pragma omp single
        {
            //Gerar tarefas de produzir
            #pragma omp taks
            {
                if ( > )
                {
                    /* code */
                }
                
            }
            #pragma omp taks
            {
                
                            
            }
        }

    }
}