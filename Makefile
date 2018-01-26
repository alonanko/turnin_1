# Sample Makefile for CS160
CC = cc
MPIRUN_FLAGS = -lm

default: svtest 

cs160String.o : cs160String.c cs160String.h
	$(CC) -c cs160String.c

svalidate.o : svalidate.c svalidate.h
	$(CC) -c svalidate.c

svtest : svtest.c svalidate.o
	$(CC) -o svtest svtest.c svalidate.o

mpi-ring2 : mpi-ring2.c cs160String.o svalidate.o
	mpicc -o mpi-ring2 mpi-ring2.c cs160String.o svalidate.o -lm  

run : 
	mpirun -np 4 mpi-ring2  

clean:
	- /bin/rm svalidate.o
	- /bin/rm svtest 
	- /bin/rm cs160String.o
