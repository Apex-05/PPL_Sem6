#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("Enter an integer: ");
        printf("\n");
        scanf("%d", &value);

        value++;
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Final value at root: %d\n", value);
    }
    else
    {
        MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value++;

        if (rank == size - 1)
            MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        else
            MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    if(rank == 0) printf("Q4,Adarsh Ranjan 230962278\n");

    MPI_Finalize();
    return 0;
}
