#variables
#compiler
CC = gcc
#compile and warn
CFLAGS = -c -Wall 
TFLAG = -lpthread -lrt

#Executes pc_sem
all: pc_sem

#creates executable pc_sem
pc_sem: pc_sem.o
	$(CC) pc_sem.o -o pc_sem ${TFLAG}

#compiles pc_sem.c
pc_sem.o: pc_sem.c
	$(CC) ${CFLAGS} pc_sem.c ${TFLAG}

#cleans out object and executable file
clean:
	rm -rf *o pc_sem