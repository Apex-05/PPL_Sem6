#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int a = 20, b = 10;  

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 4)
    {
        if (rank == 0)
            printf("Please run with at least 4 processes\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
        printf("Addition: %d + %d = %d\n", a, b, a + b);
    else if (rank == 1)
        printf("Subtraction: %d - %d = %d\n", a, b, a - b);
    else if (rank == 2)
        printf("Multiplication: %d * %d = %d\n", a, b, a * b);
    else if (rank == 3)
        printf("Division: %d / %d = %d\n", a, b, a / b);

    if(rank == 0) printf("Q2,Adarsh Ranjan 230962278");


    MPI_Finalize();
    return 0;
}
