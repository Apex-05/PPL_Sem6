#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_vowel(char c)
{
    c = tolower(c);
    return (c=='a'||c=='e'||c=='i'||c=='o'||c=='u');
}

int main(int argc, char *argv[])
{
    int rank, size;
    char *str = NULL;
    char *local_str;
    int n, local_n;
    int local_count = 0;
    int *counts = NULL;
    int total = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        str = (char *)malloc(1024 * sizeof(char));
        printf("Enter the string: ");
        fflush(stdout);
        fgets(str, 1024, stdin);
        str[strcspn(str, "\n")] = '\0';
        n = strlen(str);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n % size != 0)
    {
        if (rank == 0)
            printf("Error: String length must be evenly divisible by number of processes\n");
        MPI_Finalize();
        return 0;
    }

    local_n = n / size;
    local_str = (char *)malloc(local_n * sizeof(char));

    MPI_Scatter(str, local_n, MPI_CHAR, local_str, local_n, MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
    {
        if (!is_vowel(local_str[i]))
            local_count++;
    }

    if (rank == 0)
        counts = (int *)malloc(size * sizeof(int));

    MPI_Gather(&local_count, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
            printf("Process %d: %d\n", i, counts[i]);
        printf("Total non-vowels: %d\n", total);
        printf("Q3, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    return 0;
}
