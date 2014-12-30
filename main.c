#include <stdio.h>
#include <pthread.h>
#include "mpi.h"
#include <file_manage.h>
#include <task_alloc.h>

struct worker
{
	type t;
	struct job_pool jobs;
};

int main(int argc, char const *argv[])
{
	// cmd_para: lig_lib, rcp_path, rst_path, conf
	int my_rank, p;
	MPI_Status status;
	char *input_path;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (my_rank == 0)
	{
		input_path = argv[1];
	}
	

	//MPI_Send(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm)
	//MPI_Recv(void *buf, int count, MPI_Datatype type, int source, int tag, MPI_Comm comm, MPI_Status *status)

	MPI_Finalize();

	return 0;
}