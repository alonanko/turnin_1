#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "cs160String.h"

#define DATATAG 100
#define MSGLEN (2*MPI_MAX_PROCESSOR_NAME) 
int
main(int argc, char *argv[])
{
	MPI_Status	status;
	char		processor_name[MPI_MAX_PROCESSOR_NAME];
	int		namelen;
	int		numprocs;
	int		myid;
	STRING		* msg, *tmpS;
	int 		count;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);
	
	if (argc > 1) 
		stringDebugID(myid);
	msg = newString(0);
	tmpS = newString(MSGLEN);
	sprintf(tmpS -> string,"Process %d on %s\n", myid, processor_name);
	int to = (myid + 1) % numprocs;
	int from = (numprocs + myid - 1) % numprocs;

	if ( myid == 0 )
	{
		stringCat(msg,tmpS);
		MPI_Send(msg->string, stringLen(msg)+1, MPI_CHAR, to, DATATAG, MPI_COMM_WORLD);
		MPI_Probe(from, DATATAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_CHAR,&count);
		if (count > msg -> capacity)
			expandCapacity(msg, count);
		MPI_Recv(msg->string,count,MPI_CHAR, from, DATATAG, MPI_COMM_WORLD,&status);
		printf("%s", msg -> string);
		
	}
	else
	{
		MPI_Probe(from, DATATAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_CHAR,&count);
		if (count > msg -> capacity)
			expandCapacity(msg, count);
		MPI_Recv(msg->string,count,MPI_CHAR, from, DATATAG, MPI_COMM_WORLD,&status);
		stringCat(msg, tmpS);
		MPI_Send(msg->string, stringLen(msg)+1, MPI_CHAR, to, DATATAG, MPI_COMM_WORLD);
	}
	stringFree(msg);
	stringFree(tmpS);
	MPI_Finalize();
}
