#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "pc_sem.h"
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

//Mutex lock
pthread_mutex_t mutex;

//Semaphores
sem_t full, empty;

//Buffer variables
int BUFFER_SIZE, items_to_insert;
char *buffer;

//Buffer counter
int counter;

//Thread IDs
pthread_t* tid;       
//Set of thread attributes
pthread_attr_t attr; 

//Initialize all semaphores, mutex, buffer and thread id variables
void initializeData(int bs, int pNum, int cNum) {
	BUFFER_SIZE = bs;
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_attr_init(&attr);
	counter = 0;
	buffer = (char *)malloc(BUFFER_SIZE/sizeof(char));
	tid = (pthread_t *)malloc((pNum + cNum) * sizeof(pthread_t));
}

//Producer Thread
void *producer(void *param) {
	char item;

	while(1) {
		item = 'X';
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		if(items_to_insert > 0){
			items_to_insert--;
		}
		else{
			pthread_mutex_unlock(&mutex);
			sem_post(&full);
			break;
		}

		if(insert_item(item)) {
			fprintf(stderr, " Producer report error condition\n");
			pthread_exit(0);
		}
		else {
			printf("p:<%lu>, item: %c, at %d\n", gettid(), item, counter);
		}

		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
	pthread_exit(0);
}

//Consumer Thread
void *consumer(void *param) {
	char item;

	while(1) {
		sem_wait(&full);
		pthread_mutex_lock(&mutex);

		if(counter <= 0 && items_to_insert == 0) {
			pthread_mutex_unlock(&mutex);
			sem_post(&empty);
			break;
		}

		if(!(item = remove_item())) {
			fprintf(stderr, "Consumer report error condition\n");
			pthread_exit(0);
		}
		else {
			printf("c:<%lu>, item: %c, at %d\n", gettid(), item, counter);
		}

		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
	pthread_exit(0);
}

//Add an item to the buffer
int insert_item(char item) {
	if(counter < BUFFER_SIZE) {
		buffer[counter] = item;
		counter++;
		return 0;
	}
	else {
		return 1;
	}
}

//Remove an item from the buffer
char remove_item() {
	char item = '\0';
	if(counter > 0) {
		item = buffer[(counter-1)];
		counter--;
	}
	return item;
}

int main(int argc, char *argv[]) {
	int i, thread_count = 0;

	int buffer_size, numProd, numCons;

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'b':
				++argv;
				buffer_size = atoi(argv[1]);
				break;

			case 'p':
				++argv;
				numProd = atoi(argv[1]);
				break;

			case 'c':
				++argv;
				numCons = atoi(argv[1]);
				break;

			case 'i':
				++argv;
				items_to_insert = atoi(argv[1]);
				break;

			default:
				printf("Wrong Argument: %s\n", argv[1]);
				fprintf(stderr, "USAGE:./pc_sem -b <INT> -p <INT> -c <INT> -i <INT>\n");
				//eg. ./pc_sem -b 1000 -p 4 -c 4 -i 5
		}

		++argv;
		--argc;
		--argc;
	}

	initializeData(buffer_size, numProd, numCons);

	for(i = 0; i < numProd; i++) {
		pthread_create(&(tid[thread_count]),&attr,producer,NULL);
		thread_count++;
	}

	for(i = 0; i < numCons; i++) {
		pthread_create(&(tid[thread_count]),&attr,consumer,NULL);
		thread_count++;
	}

	for(i = 0; i < thread_count; i++){
		pthread_join(tid[i], NULL);
	}

	exit(0);
}