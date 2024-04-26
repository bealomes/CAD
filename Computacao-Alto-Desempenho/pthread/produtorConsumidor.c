// para compilar: gcc prodcons_1_thread_sem.c -o prodcons_1_thread_sem -pthread
// para executar: prodcons_1_thread_sem
//
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_PRODUCED 17
#define MAX_QUEUE 7
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3

sem_t  mutex, empty, full;

int queue[MAX_QUEUE], item_available=0, produced=0, consumed=0;


int create_item(void) {

	return(rand()%1000);

} //fim  create_item()

void insert_into_queue(int item) {
	
	queue[item_available++] = item;
	produced++;
	
	printf("producing item:%d, value:%d, queued:%d \n", produced, item, item_available); 
	return;

} // fim insert_into_queue()

int extract_from_queue() {

	consumed++;

	printf("cosuming item:%d, value:%d, queued:%d \n", consumed, queue[item_available-1], item_available-1); 
	
	return(queue[--item_available]);

} // fim extract_from_queue()

void process_item(int my_item) {
	static int printed=0;

	printf("Printed:%d, value:%d, queued:%d \n", printed++, my_item, item_available);

	return;

} // fim_process_item()


void *producer(void) {
	int item, stay = 1;
	
	while (stay) {
	  item = create_item();
	  sem_wait(&empty);
	  sem_wait(&mutex);
			if (produced < MAX_PRODUCED) {
				insert_into_queue(item);
			} else {
				stay = 0;
			}
	  sem_post(&mutex);
	  sem_post(&full);
	}

	printf("\nThread producer saindo.\n\n");
	fflush(0);

	pthread_exit(0);
} // fim producer

void *consumer(void) {
	int my_item = 0;
	int stay = 1;

	while (stay) {
	  sem_wait(&full);
	  sem_wait(&mutex);
	   if(consumed < MAX_PRODUCED) {
			my_item = extract_from_queue();
		 } else {
			stay = 0;
		 }
	  sem_post(&mutex);
	  sem_post(&empty);
	  process_item(my_item);
	}

	printf("\nThread consumer saindo.\n\n");
	fflush(0);
	
	pthread_exit(0);
} // fim_consumer

void all_consumers(pthread_t* list_cons_handle) {
	for (int i = 0; i < NUM_CONSUMERS; i++) {
		if (pthread_create(&(list_cons_handle[i]), 0, (void *) producer, (void *) 0) != 0) { 
		printf("Error creating thread producer! Exiting! \n");
		exit(0);
		}
	}
}

void all_producers(pthread_t* list_prod_handle) {
	for (int i = 0; i < NUM_CONSUMERS; i++) {
		if (pthread_create(&(list_prod_handle[i]), 0, (void *) consumer, (void *) 0) != 0) { 
			printf("Error creating thread consumer! Exiting! \n");
			exit(0);
		}
	}
}

int main(void) {
	pthread_t prod_handles[NUM_PRODUCERS];
	pthread_t cons_handles[NUM_CONSUMERS];

	/* declarations and initializations */
    	item_available = 0;

	sem_init (&mutex, 0 , 1);
	sem_init(&empty, 0, MAX_QUEUE);
	sem_init(&full, 0, 0);

	/* create and join producer and consumer threads */

	// if (pthread_create(&prod_handle, 0, (void *) producer, (void *) 0) != 0) { 
	// 	printf("Error creating thread producer! Exiting! \n");
	// 	exit(0);
	// }
	// if (pthread_create(&cons_handle, 0, (void *) consumer, (void *) 0) != 0) { 
	// 	printf("Error creating thread consumer! Exiting! \n");
	// 	exit(0);
	// }

	all_consumers(prod_handles);
	all_producers(cons_handles);

	printf("\n Fathers thread waiting for his kids.\n\n");
	fflush(0);

	for (int i = 0; i < NUM_CONSUMERS; i++) {
		pthread_join(prod_handles[i], 0);
	}

	for (int i = 0; i < NUM_CONSUMERS; i++) {
		pthread_join(cons_handles[i], 0);
	}

	//getchar();

	printf("Thread pai saindo.\n");
	fflush(0);
	
	exit(0);	
} // main()