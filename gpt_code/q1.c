#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

/* ------------------ Exercise 1(a) ------------------
   Compute pow(x, rank)
----------------------------------------------------*/
void power_of_x(int rank) {
    int x = 2;   // constant value
    int result = pow(x, rank);
    printf("Process %d: %d^%d = %d\n", rank, x, rank, result);
}

/* ------------------ Exercise 1(b) ------------------
   Even rank -> Hello
   Odd rank  -> World
----------------------------------------------------*/
void hello_world(int rank) {
    if (rank % 2 == 0)
        printf("Process %d: Hello\n", rank);
    else
        printf("Process %d: World\n", rank);
}

/* ------------------ Exercise 2 ------------------
   Simple Calculator
   Each operation done by different process
----------------------------------------------------*/
void simple_calculator(int rank) {
    int a = 10, b = 5;

    if (rank == 0)
        printf("Addition: %d + %d = %d\n", a, b, a + b);
    else if (rank == 1)
        printf("Subtraction: %d - %d = %d\n", a, b, a - b);
    else if (rank == 2)
        printf("Multiplication: %d * %d = %d\n", a, b, a * b);
    else if (rank == 3)
        printf("Division: %d / %d = %d\n", a, b, a / b);
}

/* ------------------ Exercise 3 ------------------
   Toggle character based on rank
----------------------------------------------------*/
void toggle_string(int rank, int size) {
    char str[] = "HELLO";

    if (rank < size) {
        char ch = str[rank];
        if (isupper(ch))
            ch = tolower(ch);
        else
            ch = toupper(ch);

        printf("Process %d toggled %c -> %c\n", rank, str[rank], ch);
    }
}

/* ------------------ Exercise 4 ------------------
   Even rank  -> Factorial
   Odd rank   -> Fibonacci
----------------------------------------------------*/
int factorial(int n) {
    int fact = 1;
    for (int i = 1; i <= n; i++)
        fact *= i;
    return fact;
}

int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

void factorial_fibonacci(int rank) {
    if (rank % 2 == 0)
        printf("Process %d: Factorial = %d\n", rank, factorial(rank));
    else
        printf("Process %d: Fibonacci = %d\n", rank, fibonacci(rank));
}

/* ------------------ Additional Exercise 1 ------------------
   Reverse digits using rank index
----------------------------------------------------*/
int reverse_number(int n) {
    int rev = 0;
    while (n != 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}

void reverse_array_digits(int rank) {
    int arr[9] = {18, 523, 301, 1234, 2, 14, 108, 150, 1928};

    if (rank < 9) {
        int reversed = reverse_number(arr[rank]);
        printf("Process %d: %d -> %d\n", rank, arr[rank], reversed);
    }
}

/* ------------------ Additional Exercise 2 ------------------
   Find prime numbers between 1 and 100 using 2 processes
----------------------------------------------------*/
int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

void prime_numbers(int rank) {
    int start = (rank == 0) ? 1 : 51;
    int end   = (rank == 0) ? 50 : 100;

    printf("Process %d primes:\n", rank);
    for (int i = start; i <= end; i++) {
        if (is_prime(i))
            printf("%d ", i);
    }
    printf("\n");
}

/* ====================== MAIN FUNCTION ===================== */
int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);                // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total processes

    // Uncomment ONE exercise at a time for exam clarity

    // power_of_x(rank);
    // hello_world(rank);
    // simple_calculator(rank);
    // toggle_string(rank, size);
    // factorial_fibonacci(rank);
    // reverse_array_digits(rank);
    // prime_numbers(rank);

    MPI_Finalize();  // End MPI
    return 0;
}
