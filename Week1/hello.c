#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;

    
    MPI_Init(&argc, &argv);

    
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    printf("Hello World from process %d out of %d processes\n", rank, size);

    if(rank == 0) printf("Q0, Adarsh Ranjan 230962278");


    MPI_Finalize();

    return 0;
}
