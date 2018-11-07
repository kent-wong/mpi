#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int my_rank = -1;
	int world_size = -1;
	int token = -1;
	int left_rank = -1;
	int right_rank = -1;
	int turn = -1;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	printf("my rank: %d, world size: %d\n", my_rank, world_size);

	if (my_rank == 0)
	{
		// process 0触发整个流程，发出第一个消息
		token = 0;
		turn = 1;
		MPI_Send(&token, 1, MPI_INT, (my_rank+1)%world_size, 0, MPI_COMM_WORLD);
	}


	while (1)
	{
		left_rank = (my_rank - 1 + world_size) % world_size;
		right_rank = (my_rank + 1) % world_size;

		// receive
		MPI_Recv(&token, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d received token %d from process %d\n",
				my_rank, token, left_rank);

		if (my_rank == 0)
		{
			if (turn >= 2)
				token = -1;

			turn ++;
		}

		if (token >= 0)
		{
			token ++;
		}

		// send
		MPI_Send(&token, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD);
		if (token < 0)
			break;
	}




	MPI_Finalize();

	return 0;
}

