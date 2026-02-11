#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------
   1) SYNCHRONOUS SEND: Toggle word and send back
--------------------------------------------------*/
void sync_toggle_word(int rank) {
    char word[20];

    if (rank == 0) {
        strcpy(word, "HELLO");
        MPI_Ssend(word, strlen(word)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(word, 20, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received toggled word: %s\n", word);
    }

    if (rank == 1) {
        MPI_Recv(word, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; word[i] != '\0'; i++) {
            if (isupper(word[i]))
                word[i] = tolower(word[i]);
            else
                word[i] = toupper(word[i]);
        }
        MPI_Ssend(word, strlen(word)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}

/* --------------------------------------------------
   2) MASTER SENDS NUMBER TO SLAVES (STANDARD SEND)
--------------------------------------------------*/
void master_to_slaves(int rank, int size) {
    int num = 100;

    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d\n", rank, num);
    }
}

/* --------------------------------------------------
   3) BUFFERED SEND: Square / Cube
--------------------------------------------------*/
void buffered_square_cube(int rank, int size) {
    int arr[10], value;
    int buffer[100];
    MPI_Buffer_attach(buffer, 100 * sizeof(int));

    if (rank == 0) {
        for (int i = 0; i < size; i++)
            arr[i] = i + 1;

        for (int i = 1; i < size; i++)
            MPI_Bsend(&arr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (rank % 2 == 0)
            printf("Process %d square: %d\n", rank, value * value);
        else
            printf("Process %d cube: %d\n", rank, value * value * value);
    }

    MPI_Buffer_detach(&buffer, &(int){0});
}

/* --------------------------------------------------
   4) CHAIN COMMUNICATION (Increment and forward)
--------------------------------------------------*/
void chain_increment(int rank, int size) {
    int value;

    if (rank == 0) {
        value = 5;
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Final value at root: %d\n", value);
    } else {
        MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value++;
        if (rank == size - 1)
            MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        else
            MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
}

/* --------------------------------------------------
   ADDITIONAL 1: Prime check
--------------------------------------------------*/
int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

void prime_check(int rank, int size) {
    int arr[10], value;

    if (rank == 0) {
        for (int i = 0; i < size; i++)
            arr[i] = i + 10;

        for (int i = 1; i < size; i++)
            MPI_Send(&arr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        if (is_prime(arr[0]))
            printf("%d is prime\n", arr[0]);
    } else {
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (is_prime(value))
            printf("%d is prime\n", value);
    }
}

/* --------------------------------------------------
   ADDITIONAL 2: Expression evaluation
--------------------------------------------------*/
int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++) f *= i;
    return f;
}

int sum_n(int n) {
    return (n * (n + 1)) / 2;
}

void series_sum(int rank, int size) {
    int result = 0, local;

    if (rank % 2 == 0)
        local = factorial(rank + 1);
    else
        local = sum_n(rank + 2);

    if (rank != 0)
        MPI_Send(&local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    else {
        result += local;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result += local;
        }
        printf("Final series result: %d\n", result);
    }
}

/* ===================== MAIN ===================== */
int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Uncomment ONE at a time for exam clarity

    // sync_toggle_word(rank);
    // master_to_slaves(rank, size);
    // buffered_square_cube(rank, size);
    // chain_increment(rank, size);
    // prime_check(rank, size);
    // series_sum(rank, size);

    MPI_Finalize();
    return 0;
}
