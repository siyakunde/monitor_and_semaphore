#variables
#compiler
CC = gcc
#compile and warn
CFLAGS = -c -Wall 
TFLAG = -lpthread -lrt

#Executes pc_mon
all: pc_mon

#creates executable pc_mon
pc_mon: pc_mon.o
	$(CC) pc_mon.o -o pc_mon ${TFLAG}

#compiles pc_mon.c
pc_mon.o: pc_mon.c monitor.c monitor.h
	$(CC) ${CFLAGS} pc_mon.c ${TFLAG}

#cleans out object and executable file
clean:
	rm -rf *o pc_mon