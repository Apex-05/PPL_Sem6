#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int rank, size;
    char str[] = "HELLO";
    int len = strlen(str);
    char ch;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < len)
    {
        ch = str[rank];

        if (islower(ch))
            ch = toupper(ch);
        else
            ch = tolower(ch);
    }

    MPI_Gather(&ch, 1, MPI_CHAR,
               str, 1, MPI_CHAR,
               0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Toggled String: %s\n", str);

    if(rank == 0) printf("Q3, Adarsh Ranjan 230962278");


    MPI_Finalize();
    return 0;
}
