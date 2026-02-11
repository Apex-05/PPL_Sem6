#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Helper functions */
int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++) f *= i;
    return f;
}

int is_vowel(char c) {
    c = tolower(c);
    return (c=='a'||c=='e'||c=='i'||c=='o'||c=='u');
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* ----------------------------------------------------
       1) FACTORIAL + SUM (Scatter + Reduce)
    ----------------------------------------------------*/
    {
        int numbers[10], local_num, local_fact, sum_fact;

        if (rank == 0) {
            for (int i = 0; i < size; i++)
                numbers[i] = i + 1;   // example input
        }

        MPI_Scatter(numbers, 1, MPI_INT,
                    &local_num, 1, MPI_INT,
                    0, MPI_COMM_WORLD);

        local_fact = factorial(local_num);

        MPI_Reduce(&local_fact, &sum_fact, 1, MPI_INT,
                   MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0)
            printf("Sum of factorials = %d\n", sum_fact);
    }

    /* ----------------------------------------------------
       2) AVERAGE OF M ELEMENTS (Scatter + Reduce)
    ----------------------------------------------------*/
    {
        int M = 3;
        int data[30], local[3];
        float local_avg, total_avg;

        if (rank == 0) {
            for (int i = 0; i < size * M; i++)
                data[i] = i + 1;
        }

        MPI_Scatter(data, M, MPI_INT,
                    local, M, MPI_INT,
                    0, MPI_COMM_WORLD);

        int sum = 0;
        for (int i = 0; i < M; i++) sum += local[i];
        local_avg = (float)sum / M;

        MPI_Reduce(&local_avg, &total_avg, 1, MPI_FLOAT,
                   MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0)
            printf("Total average = %.2f\n", total_avg / size);
    }

    /* ----------------------------------------------------
       3) NON-VOWEL COUNT IN STRING (Scatter + Gather)
    ----------------------------------------------------*/
    {
        char str[] = "parallelmpi";
        int len = strlen(str);
        int chunk = len / size;

        char local_str[10];
        int local_count = 0, counts[10];

        MPI_Scatter(str, chunk, MPI_CHAR,
                    local_str, chunk, MPI_CHAR,
                    0, MPI_COMM_WORLD);

        for (int i = 0; i < chunk; i++)
            if (!is_vowel(local_str[i]))
                local_count++;

        MPI_Gather(&local_count, 1, MPI_INT,
                   counts, 1, MPI_INT,
                   0, MPI_COMM_WORLD);

        if (rank == 0) {
            int total = 0;
            for (int i = 0; i < size; i++) {
                printf("Process %d non-vowels = %d\n", i, counts[i]);
                total += counts[i];
            }
            printf("Total non-vowels = %d\n", total);
        }
    }

    /* ----------------------------------------------------
       4) STRING MERGE (Scatter + Gather)
    ----------------------------------------------------*/
    {
        char s1[] = "string";
        char s2[] = "length";
        int len = strlen(s1);
        int chunk = len / size;

        char a[10], b[10], c[20];
        char result[20];

        MPI_Scatter(s1, chunk, MPI_CHAR, a, chunk, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Scatter(s2, chunk, MPI_CHAR, b, chunk, MPI_CHAR, 0, MPI_COMM_WORLD);

        for (int i = 0; i < chunk; i++) {
            c[2*i]   = a[i];
            c[2*i+1] = b[i];
        }

        MPI_Gather(c, 2*chunk, MPI_CHAR,
                   result, 2*chunk, MPI_CHAR,
                   0, MPI_COMM_WORLD);

        if (rank == 0)
            printf("Resultant String: %s\n", result);
    }

    /* ----------------------------------------------------
       5) EVEN → 1, ODD → 0 + COUNT (Collective)
    ----------------------------------------------------*/
    {
        int A[9] = {1,2,3,4,5,6,7,8,9};
        int local, result[9];
        int even = 0, odd = 0, total_even, total_odd;

        MPI_Scatter(A, 1, MPI_INT,
                    &local, 1, MPI_INT,
                    0, MPI_COMM_WORLD);

        if (local % 2 == 0) {
            local = 1;
            even = 1;
        } else {
            local = 0;
            odd = 1;
        }

        MPI_Gather(&local, 1, MPI_INT,
                   result, 1, MPI_INT,
                   0, MPI_COMM_WORLD);

        MPI_Reduce(&even, &total_even, 1, MPI_INT,
                   MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&odd, &total_odd, 1, MPI_INT,
                   MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            printf("Resultant Array: ");
            for (int i = 0; i < size; i++)
                printf("%d ", result[i]);
            printf("\nEven count = %d\nOdd count = %d\n",
                   total_even, total_odd);
        }
    }

    MPI_Finalize();
    return 0;
}
