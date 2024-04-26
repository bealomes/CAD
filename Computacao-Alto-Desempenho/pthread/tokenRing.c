#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 3

sem_t mutex[NUM_THREADS];
int token = 0, executions = 0;

void *thread_function(void *arg){
    int id = (int)arg;
    int real_array_pos = id - 1; //because there's a shift between ID and mutex positions we need to correct this

    sem_wait(&mutex[real_array_pos]);
    printf("Thread %d: %d\n", id, token);
	fflush(0); //flushing to print immediately 
    token++;
    int next = (real_array_pos + 1) % NUM_THREADS; //circular array, could be an if here
    sem_post(&mutex[next]);

	pthread_exit(0);
}

int main(void){ //thread 0
    pthread_t t_handler[NUM_THREADS];
    sem_init(&mutex[0], 0, 1); //initializes the first mutex as open (position 0 is thread id 1)
    for (int i = 1; i < NUM_THREADS; i++){
        sem_init(&mutex[i], 0, 0);
    }

    printf("Thread 0: %d\n", token);
    token++; //thread 0 does not need mutex protection because other threads will only be created latter
    //this means that token is not yet shared memory

    for(int i = 0; i < NUM_THREADS; i++){
        int id = i+1; //as thread 0 is main, the identifiers are shifted by 1
        if(pthread_create(&t_handler[i], NULL, thread_function, (void*) id) != 0){
            printf("Error creating thread %d\n", i);
            exit(1);
        }
    }

    for(int i = 1; i < NUM_THREADS; i++){
        pthread_join(t_handler[i], NULL); 
    }

    printf("Thread 0: %d\n", token); //this only executes after the last thread returns because of join
}