#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int buffer_size = size * (sizeof(int) + MPI_BSEND_OVERHEAD);
    char *buffer = (char *)malloc(buffer_size);

    MPI_Buffer_attach(buffer, buffer_size);

    if (rank == 0)
    {
        int arr[size];
        printf("Enter %d elements:\n", size);
        for (int i = 0; i < size; i++)
            scanf("%d", &arr[i]);

        for (int i = 0; i < size; i++)
            MPI_Bsend(&arr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank % 2 == 0)
        printf("Process %d (Even) Square = %d\n", rank, value * value);
    else
        printf("Process %d (Odd) Cube = %d\n", rank, value * value * value);

    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);

    if(rank == 0) printf("Q3,Adarsh Ranjan 230962278\n");


    MPI_Finalize();
    return 0;
}
