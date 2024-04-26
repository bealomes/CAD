#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define qtd_Thread 6

//Essa versão garante ordem das taks não das threads
int main(){
    int token = 0;
    #pragma omp parallel num_threads(qtd_Thread) shared(token)
    {
        #pragma omp single 
        {
            for (int i = 0; i < qtd_Thread; i++)
            {
                //Após thread printar que a outra ira poder execultar, então
                #pragma omp task depend(inout:token)
                {
                    int num_thread = omp_get_thread_num();
                    printf("Thread %d e Token %d\n", omp_get_thread_num(), token);
                    #pragma omp atomic
                    token++;
                }
              
            }
            
        }
    }

}