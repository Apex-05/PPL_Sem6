#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

long long factorial(int n) {
    long long fact = 1;
    for (int i = 1; i <= n; i++)
        fact *= i;
    return fact;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int err;
    char err_string[MPI_MAX_ERROR_STRING];
    int err_len;
    long long local_fact;
    long long scan_sum;

    err = MPI_Init(&argc, &argv);
    if (err != MPI_SUCCESS) {
        MPI_Error_string(err, err_string, &err_len);
        printf("%s\n", err_string);
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (err != MPI_SUCCESS) {
        MPI_Error_string(err, err_string, &err_len);if(rank == 0) {
        printf("Q1, Adarsh Ranjan 230962278\n");
    }

        printf("%s\n", err_string);
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (err != MPI_SUCCESS) {
        MPI_Error_string(err, err_string, &err_len);
        printf("%s\n", err_string);
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    local_fact = factorial(rank + 1);

    err = MPI_Scan(&local_fact, &scan_sum, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);
    if (err != MPI_SUCCESS) {
        MPI_Error_string(err, err_string, &err_len);
        printf("%s\n", err_string);
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    if (rank == size - 1) {
        printf("Final Output: %lld\n", scan_sum);
    }
    if(rank == 0) {
        printf("Q1, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    return 0;
}
