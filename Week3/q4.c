#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    char s1[100], s2[100];
    char *res = NULL;
    char *sub_s1, *sub_s2, *sub_res;
    int n, m;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter String 1: ");
        fflush(stdout);
        scanf("%s", s1);
        printf("Enter String 2: ");
        fflush(stdout);
        scanf("%s", s2);

        n = strlen(s1);
        
        if (n % size != 0) {
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        m = n / size;
    }

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    sub_s1 = (char*)malloc(m * sizeof(char));
    sub_s2 = (char*)malloc(m * sizeof(char));
    sub_res = (char*)malloc(2 * m * sizeof(char));

    MPI_Scatter(s1, m, MPI_CHAR, sub_s1, m, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(s2, m, MPI_CHAR, sub_s2, m, MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int i = 0; i < m; i++) {
        sub_res[2 * i] = sub_s1[i];
        sub_res[2 * i + 1] = sub_s2[i];
    }

    if (rank == 0) {
        res = (char*)malloc((2 * n + 1) * sizeof(char));
    }

    MPI_Gather(sub_res, 2 * m, MPI_CHAR, res, 2 * m, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        res[2 * n] = '\0';
        printf("\nResult: %s\n", res);
        printf("Q4, Adarsh Ranjan 230962278\n");

        free(res);
    }

    free(sub_s1);
    free(sub_s2);
    free(sub_res);

    MPI_Finalize();
    return 0;
}