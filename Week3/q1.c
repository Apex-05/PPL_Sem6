#include <mpi.h>
#include <stdio.h>

long long factorial(int n) {
    long long fact = 1;
    for (int i = 1; i <= n; i++)
        fact *= i;
    return fact;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int number;
    long long fact, sum = 0;
    int values[100];
    long long facts[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter the numbers: \n");
        for (int i = 0; i < size; i++) {
            scanf("%d", &values[i]);
        }
    }

    MPI_Scatter(values, 1, MPI_INT,
                &number, 1, MPI_INT,
                0, MPI_COMM_WORLD);

    fact = factorial(number);

    MPI_Gather(&fact, 1, MPI_LONG_LONG_INT,
               facts, 1, MPI_LONG_LONG_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            sum += facts[i];
        }
        printf("Total: %lld\n", sum);
        printf("Q1, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    return 0;
}
