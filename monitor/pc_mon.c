#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "monitor.c"

//Thread ID
pthread_t* tid; 
//Set of thread attributes      
pthread_attr_t attr; 

//Initialize variables in main
void main_init(int pNum, int cNum){
   pthread_attr_init(&attr);
   tid = (pthread_t *)malloc((pNum + cNum) * sizeof(pthread_t));
}

//Generate random capital-case or lower-case letter
char getRandomChar(){
   return ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[rand() % 52]);
}

//Producer Thread
void *producer(void *param) {
   char item;
   int status = 1;

   while(status) {
      item = getRandomChar();
      status = mon_insert(item);
   }
   pthread_exit(0);
}

//Consumer Thread
void *consumer(void *param) {
   char item;
   int status = 1;

   while(status) {
      status = mon_remove(&item);
   }
   pthread_exit(0);
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
            fprintf(stderr, "USAGE:./pc_mon -b <INT> -p <INT> -c <INT> -i <INT>\n");
            //eg. ./pc_mon -b 1000 -p 4 -c 4 -i 5
      }

      ++argv;
      --argc;
      --argc;
   }

   mon_init(buffer_size);
   main_init(numProd, numCons);

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