/*************************************
Alon Ankonina
aankonin@ucsd.edu
A12854998
******************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "svalidate.h"
#include "cs160String.h"

#define DATATAG 100
#define MSGLEN 100
int main(int argc, char *argv[])
{
	MPI_Status	status;
	char		processor_name[MPI_MAX_PROCESSOR_NAME];
	int		namelen;
	int		numprocs;
	int		myid;
	STRING		* msg, *tmpS;
	int 		count;
	float		seedval;
	int 		startid;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);
	
	msg = newString(0);
	tmpS = newString(MSGLEN);
	sprintf(tmpS -> string,"Process %d on %s\n", myid, processor_name);
	int to = (myid + 1) % numprocs;
	int from = (numprocs + myid - 1) % numprocs;

	//input validations
	//check for 2 arguments
	if ( argc != 3 ) { 
		if(myid == 0) {fprintf(stderr, "wrong number of arguments\n");} 
		return 1;
	}

	//read in arguments
	char* input_id = trimCopy(argv[1]);
	char* input_seed = trimCopy(argv[2]);

	//check for integer start id
	if ( !isInteger(input_id)) { 
		if (myid ==0) {fprintf(stderr, "invalid process ID\n");} 
		return 1;
	}

	//check for float seed value
	if ( !isFloat(input_seed)) { 
		if (myid == 0) {fprintf(stderr, "invalid seed value\n");} 
		return 1;
	}

	//convert items to correct types
	sscanf(argv[1],"%d",&startid);
	sscanf(argv[2],"%f",&seedval);
	
	//keep startid in range of available processes
	//allows for negative start ids
	startid = (startid + numprocs) % numprocs;

	//check for positive seedval
	if ( seedval < 0 ) { 
		if (myid==0) {fprintf(stderr, "seedval must be positive\n");} 
		return 1;
	}

	//return if only 1 process
	if( myid == 0){
		if (numprocs == 1) {
			return;
		}
	}

	if ( myid == startid)
	{
		float initial_seed = seedval;		//keep copy of initial seed value for output
		
		//message passing
		MPI_Send(&seedval, 1, MPI_FLOAT, to, DATATAG, MPI_COMM_WORLD);	
		MPI_Recv(&seedval, 1, MPI_FLOAT, from, DATATAG, MPI_COMM_WORLD,&status);
		
		//operations
		seedval = powf(seedval + myid,1.2);

		//output
		printf("start %d seed %f nproc %d\n", startid, initial_seed, numprocs );
		printf("%f\n", seedval);
	}
	else 
	{	
		//recieve seed, perform operations, and send to next process
		MPI_Recv(&seedval, 1, MPI_FLOAT, from, DATATAG, MPI_COMM_WORLD,&status);
		seedval = powf(seedval + myid,1.2f);
		MPI_Send(&seedval, 1, MPI_FLOAT, to, DATATAG, MPI_COMM_WORLD);	
	}

	//clean up
	stringFree(msg);
	stringFree(tmpS);
	MPI_Finalize();
}
