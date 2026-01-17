#include <mpi.h>
#include <stdio.h>

long power(int base, int exp) {
    long res = 1;
    for (int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

int main(int argc, char *argv[])
{
    int rank, size;
    int x = 2;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long result = power(x, rank);

    printf("Process %d: %d^%d = %ld\n", rank, x, rank, result);

    if (rank % 2 == 0)
        printf("Process %d says: Hello\n", rank);
    else
        printf("Process %d says: World\n", rank);
    
    
    if(rank == 0) {
        printf("Q1, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    
    return 0;
}