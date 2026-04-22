#include <stdio.h>
#include <cuda.h>
#include <math.h>
#include <string.h>

#define N 1024

// ================= GENERAL KERNEL =================
__global__ void kernel(int *A, int *B, int *C, int n)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;

    if (idx < n)
    {
        // ====== CHANGE LOGIC HERE BASED ON QUESTION ======

        // 1. VECTOR ADDITION
        C[idx] = A[idx] + B[idx];

        // 2. SAXPY (y = a*x + y)
        // int a = 2;
        // C[idx] = a * A[idx] + B[idx];

        // 3. SIN FUNCTION
        // float *Af = (float*)A;
        // float *Cf = (float*)C;
        // Cf[idx] = sin(Af[idx]);

        // 4. ONE'S COMPLEMENT
        // C[idx] = ~A[idx];

        // 5. OCTAL CONVERSION
        // int num = A[idx], i = 1, oct = 0;
        // while(num != 0){
        //     oct += (num % 8) * i;
        //     num /= 8;
        //     i *= 10;
        // }
        // C[idx] = oct;
    }
}

// ================= CONVOLUTION =================
__global__ void convolution(int *A, int *M, int *P, int n, int mask_width)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;

    if (idx < n)
    {
        int sum = 0;

        for (int j = 0; j < mask_width; j++)
        {
            int k = idx + j - mask_width/2;

            if (k >= 0 && k < n)
                sum += A[k] * M[j];
        }

        P[idx] = sum;
    }
}

// ================= SELECTION SORT =================
__global__ void selection_sort(int *A, int n)
{
    int i = threadIdx.x;

    for (int j = i + 1; j < n; j++)
    {
        if (A[i] > A[j])
        {
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }
}

// ================= ODD EVEN SORT =================
__global__ void odd_even_sort(int *A, int n)
{
    int idx = threadIdx.x;

    for (int i = 0; i < n; i++)
    {
        if ((i % 2 == 0) && idx % 2 == 0 && idx + 1 < n)
        {
            if (A[idx] > A[idx + 1])
            {
                int temp = A[idx];
                A[idx] = A[idx + 1];
                A[idx + 1] = temp;
            }
        }

        if ((i % 2 == 1) && idx % 2 == 1 && idx + 1 < n)
        {
            if (A[idx] > A[idx + 1])
            {
                int temp = A[idx];
                A[idx] = A[idx + 1];
                A[idx + 1] = temp;
            }
        }
        __syncthreads(); //Threads interact
    }
}

// ================= ATOMIC WORD COUNT =================
__global__ void word_count(char *str, char target, int *count)
{
    int idx = threadIdx.x;

    if (str[idx] == target)
        atomicAdd(count, 1); //Atomic operations
}

// ================= STRING REPEAT =================
__global__ void string_repeat(char *S, char *R, int len, int repeat_count)
{
    int idx = threadIdx.x;

    for (int i = 0; i < repeat_count; i++)
    {
        R[i * len + idx] = S[idx];
    }
}

// ================= MAIN =================
int main()
{
    int *A, *B, *C;
    int *d_A, *d_B, *d_C;

    int size = N * sizeof(int);

    A = (int*)malloc(size);
    B = (int*)malloc(size);
    C = (int*)malloc(size);

    // Initialize
    for (int i = 0; i < N; i++)
    {
        A[i] = i;
        B[i] = i;
    }

    // Allocate GPU memory
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    // Copy to GPU
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Launch kernel
    kernel<<<(N + 255)/256, 256>>>(d_A, d_B, d_C, N); // blocks,threads max - 1024
    // for string word_count<<<1, len>>> 
    //strlen()

    // Copy back
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Print few outputs
    for (int i = 0; i < 10; i++)
        printf("%d ", C[i]);

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(A);
    free(B);
    free(C);

    return 0;
}