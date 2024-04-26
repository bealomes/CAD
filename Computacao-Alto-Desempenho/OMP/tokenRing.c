#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define qtd_Thread 6

int main(){
    int token = 0;
    omp_lock_t bloquear[qtd_Thread];
    //iniciar todos todos lock
    omp_init_lock(&bloquear[0]);//Tem a primeira thread que vai começar ativa, e ela ira ativar as demais
    /*Para garantir que apenas um processo por vez possa ter o token e executar a seção crítica,
     é necessário ter uma trava específica para o token. Cada processo deve adquirir essa trava antes de acessar o token e 
    a seção crítica associada.*/
   for (int i = 1; i < qtd_Thread; i++){
          omp_init_lock(&bloquear[i]);//inicia as demais threads
          omp_set_lock(&bloquear[i]);//Inicial bloqueio de todas as threads 
    }
    
  

    #pragma omp parallel num_threads(qtd_Thread) shared(bloquear, token)
    {
        //determina o numero da thread
        int num_thread = omp_get_thread_num(); //Ṕega o número da thread que está
        /*Cada processo pode precisar de travas locais para controlar o acesso a recursos específicos 
        dentro da sua seção crítica. Por exemplo, se um processo precisa acessar uma estrutura de dados compartilhada, 
        ele pode precisar adquirir uma trava para garantir que outros processos não acessem os mesmos dados ao mesmo tempo.*/
        omp_set_lock(&bloquear[num_thread]);
        printf("Thread %d e Token %d\n", num_thread, token);
        token++;
        omp_destroy_lock(&bloquear[num_thread]);//Thread após execultar "deixa de existir", então pode remover apagar seu bloqueio
        //Desbloquear próxima Thread que vai ser execultada, após essa terminar sua execução
        int próxima = (omp_get_thread_num()+1) % qtd_Thread;
        omp_unset_lock(&bloquear[próxima]); //Liberar próxima thread a ser execultada       
    }
    
}