#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int my_rank = -1;
	int world_size = -1;
	int token = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	printf("my rank: %d, world size: %d\n", my_rank, world_size);

	if (my_rank != 0)
	{
		// 非process 0的进程先接收再发送
		MPI_Recv(&token, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d received token %d from process %d\n",
				my_rank, token, my_rank-1);

		token ++;
		MPI_Send(&token, 1, MPI_INT, (my_rank+1)%world_size, 0, MPI_COMM_WORLD);
	}
	else
	{
		// process 0先发送再接收
		MPI_Send(&token, 1, MPI_INT, (my_rank+1)%world_size, 0, MPI_COMM_WORLD);

		MPI_Recv(&token, 1, MPI_INT, world_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d received token %d from process %d\n",
				my_rank, token, world_size-1);
	}


	MPI_Finalize();

	return 0;
}

