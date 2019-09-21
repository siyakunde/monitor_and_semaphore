#include "monitor.h"
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

//Initialize condition variable components
void cond_init(Condition *cond_var, int numItems){
	sem_init(&cond_var->sem, 0, numItems);
	pthread_mutex_init(&cond_var->mutex, NULL);
	cond_var->waitingProcs = 0;
}

//Condition wait
int cond_wait(Condition *cond_var, pthread_mutex_t *mutex){
	pthread_mutex_lock(mutex);
	pthread_mutex_lock(&cond_var->mutex);
	if(cond_var->waitingProcs < items_to_insert){
		cond_var->waitingProcs++;
	}
	pthread_mutex_unlock(mutex);
	pthread_mutex_unlock(&cond_var->mutex);

	sem_wait(&cond_var->sem);
	pthread_mutex_lock(mutex);

	return 0;
}

//Condition signal
void cond_signal(Condition *cond_var, pthread_mutex_t *mutex){
	pthread_mutex_lock(&cond_var->mutex);
	cond_var->waitingProcs--;
	sem_post(&cond_var->sem);
	pthread_mutex_unlock(&cond_var->mutex);
	pthread_mutex_unlock(mutex);
}

//Initialize the monitor
void mon_init(int bs){
	BUFFER_SIZE = bs;
	cond_init(&cond_full, bs);
	cond_init(&cond_empty, 0);
	pthread_mutex_init(&mutex, NULL);
	counter = 0;
   	buffer = (char *)malloc(BUFFER_SIZE/sizeof(char));
}

//Monitor insert
int mon_insert(char item){
	while(cond_wait(&cond_full, &mutex)){}

	if(items_to_insert > 0){
		items_to_insert--;
	}
	else{
		cond_signal(&cond_empty, &mutex);
		return 0;
	}

	if(insert_item(item)) {
		fprintf(stderr, " Producer report error condition\n");
		return 1;
	}
	else {
        printf("p:<%lu>, item: %c, at %d\n", gettid(), item, counter);
	}

	cond_signal(&cond_empty, &mutex);
	return 1;
}

//Monitor remove
int mon_remove(char *item){
	*item = '\0';
	while(cond_wait(&cond_empty, &mutex)){}
	if(counter <= 0 && items_to_insert == 0) {
		cond_signal(&cond_full, &mutex);
		return 0;
	}

	if(!(*item = remove_item())) {
		fprintf(stderr, "Consumer report error condition\n");
		pthread_exit(0);
	}
	else {
		printf("c:<%lu>, item: %c, at %d\n", gettid(), *item, counter+1);
	}

	cond_signal(&cond_full, &mutex);

	return 1;
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