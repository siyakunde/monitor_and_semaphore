#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void initializeData();

void *consumer(void *param);
void *producer(void *param);

int insert_item(char item);
char remove_item();
