#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++)
        f *= i;
    return f;
}

int main(int argc, char *argv[]) {
    int rank, size;

    /* ---------- MPI INITIALIZATION ---------- */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* =====================================================
       1) 1! + 2! + ... + N! USING MPI_SCAN
    ===================================================== */
    {
        int local_fact, prefix_sum;

        local_fact = factorial(rank + 1);

        /* MPI_Scan gives prefix sum */
        MPI_Scan(&local_fact, &prefix_sum, 1,
                 MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        if (rank == size - 1)
            printf("Sum of factorials = %d\n", prefix_sum);
    }

    /* =====================================================
       2) COUNT OCCURRENCES IN 3x3 MATRIX (SCATTER + REDUCE)
    ===================================================== */
    {
    int matrix[9], local;
    int target = 2;
    int local_count = 0, total_count;

    /* Root initializes 3x3 matrix */
    if (rank == 0) {
        int temp[9] = {1, 2, 3,
                       2, 4, 2,
                       5, 6, 2};

        for (int i = 0; i < 9; i++)
            matrix[i] = temp[i];
    }

    /* Distribute matrix elements */
    MPI_Scatter(matrix, 1, MPI_INT,
                &local, 1, MPI_INT,
                0, MPI_COMM_WORLD);

    /* Each process checks for target value */
    if (local == target)
        local_count = 1;

    /* Sum all local counts */
    MPI_Reduce(&local_count, &total_count, 1,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Root prints total occurrences */
    if (rank == 0)
        printf("Occurrences = %d\n", total_count);
}


    /* =====================================================
       3) WORD PATTERN (PCAP → PCCAAAPPPP)
    ===================================================== */
    {
        char word[] = "PCAP";
        char local_char;
        char result[50];
        int repeat = rank + 1;

        MPI_Scatter(word, 1, MPI_CHAR,
                    &local_char, 1, MPI_CHAR,
                    0, MPI_COMM_WORLD);

        char local_block[10];
        for (int i = 0; i < repeat; i++)
            local_block[i] = local_char;

        MPI_Gather(local_block, repeat, MPI_CHAR,
                   result, repeat, MPI_CHAR,
                   0, MPI_COMM_WORLD);

        if (rank == 0) {
            result[repeat * size] = '\0';
            printf("Result Word = %s\n", result);
        }
    }

    /* =====================================================
       4) PI COMPUTATION (NUMERICAL INTEGRATION)
    ===================================================== */
    {
        int n = 1000;
        double h = 1.0 / n;
        double local_sum = 0.0, pi;

        for (int i = rank; i < n; i += size) {
            double x = h * (i + 0.5);
            local_sum += 4.0 / (1.0 + x * x);
        }

        MPI_Reduce(&local_sum, &pi, 1,
                   MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            pi = pi * h;
            printf("PI value = %lf\n", pi);
        }
    }

    MPI_Finalize();
    return 0;
}
