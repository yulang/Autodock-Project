#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char const *argv[])
{
	int my_rank, p;
	MPI_Status status;
	char *input_path;

	MPI_Init(&argc, &argv);

	input_path = argv[1];
	

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	//MPI_Send(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm)
	//MPI_Recv(void *buf, int count, MPI_Datatype type, int source, int tag, MPI_Comm comm, MPI_Status *status)

	MPI_Finalize();

	return 0;
}