#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct Condition {
    int waitingProcs;
    sem_t sem;
    pthread_mutex_t mutex;
}Condition;

//Condition variables provide queues to manage the monitor
Condition cond_full, cond_empty;
//Controls entry to the monitor
pthread_mutex_t mutex;

//Buffer variables
int BUFFER_SIZE, items_to_insert;
char *buffer;

//Buffer counter
int counter;

void cond_init(Condition *cond_var, int numItems);
int cond_wait(Condition *cond_var, pthread_mutex_t *mutex);
void cond_signal(Condition *cond_var, pthread_mutex_t *mutex);

void mon_init(int bs);
int mon_insert(char item);
int mon_remove(char *item);

int insert_item(char item);
char remove_item();