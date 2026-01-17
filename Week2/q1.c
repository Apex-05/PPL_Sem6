#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int rank;
    char word[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("Enter a word: ");
        printf("\n");
        scanf("%s", word);

        MPI_Ssend(word, strlen(word) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(word, 100, MPI_CHAR, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Toggled word received back: %s\n", word);
    }
    else if (rank == 1)
    {
        MPI_Recv(word, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; word[i] != '\0'; i++)
        {
            if (islower(word[i]))
                word[i] = toupper(word[i]);
            else if (isupper(word[i]))
                word[i] = tolower(word[i]);
        }

        MPI_Ssend(word, strlen(word) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }
    if(rank == 0) printf("Q1,Adarsh Ranjan 230962278\n");


    MPI_Finalize();
    return 0;
}
