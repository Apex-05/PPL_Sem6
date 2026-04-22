#include <stdio.h>
#include <cuda.h>

#define N 1024

// ================= KERNEL =================
__global__ void kernel(int *A, int *B, int *C, int *extra, int rows, int cols, int mode)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // ================= ELEMENT-WISE SAFE CHECK =================
    if (row < rows && col < cols)
    {
        int index = row * cols + col;

        // =========================================================
        // 🔥 SWITCH CASE → CHANGE MODE TO SOLVE QUESTION
        // =========================================================

        switch(mode)
        {
            // ================= MATRIX ADDITION (ELEMENT) =================
            case 1:
                C[index] = A[index] + B[index];
                break;

            // ================= MATRIX ADDITION (ROW THREAD) =================
            case 2:
                if (idx < rows)
                {
                    for (int j = 0; j < cols; j++)
                        C[idx * cols + j] = A[idx * cols + j] + B[idx * cols + j];
                }
                break;

            // ================= MATRIX ADDITION (COLUMN THREAD) =================
            case 3:
                if (idx < cols)
                {
                    for (int i = 0; i < rows; i++)
                        C[i * cols + idx] = A[i * cols + idx] + B[i * cols + idx];
                }
                break;

            // ================= MATRIX MULTIPLICATION (ELEMENT) =================
            case 4:
            {
                int sum = 0;
                for (int k = 0; k < cols; k++)
                    sum += A[row * cols + k] * B[k * cols + col];

                C[index] = sum;
                break;
            }

            // ================= MATRIX MULTIPLICATION (ROW THREAD) =================
            case 5:
                if (idx < rows)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        int sum = 0;
                        for (int k = 0; k < cols; k++)
                            sum += A[idx * cols + k] * B[k * cols + j];

                        C[idx * cols + j] = sum;
                    }
                }
                break;

            // ================= MATRIX MULTIPLICATION (COLUMN THREAD) =================
            case 6:
                if (idx < cols)
                {
                    for (int i = 0; i < rows; i++)
                    {
                        int sum = 0;
                        for (int k = 0; k < cols; k++)
                            sum += A[i * cols + k] * B[k * cols + idx];

                        C[i * cols + idx] = sum;
                    }
                }
                break;

            // ================= EVEN → ROW SUM, ODD → COLUMN SUM =================
            case 7:
            {
                int val = A[index];

                if (val % 2 == 0)
                {
                    int sum = 0;
                    for (int j = 0; j < cols; j++)
                        sum += A[row * cols + j];

                    C[index] = sum;
                }
                else
                {
                    int sum = 0;
                    for (int i = 0; i < rows; i++)
                        sum += A[i * cols + col];

                    C[index] = sum;
                }
                break;
            }

            // ================= DIAGONAL / FACTORIAL / SUM DIGITS =================
            case 8:
            {
                if (row == col)
                {
                    C[index] = 0;
                }
                else if (row < col)
                {
                    int fact = 1, n = A[index];
                    for (int i = 1; i <= n; i++)
                        fact *= i;

                    C[index] = fact;
                }
                else
                {
                    int sum = 0, n = A[index];
                    while (n > 0)
                    {
                        sum += n % 10;
                        n /= 10;
                    }
                    C[index] = sum;
                }
                break;
            }

            // ================= ROW POWER (1st same, 2nd square, etc.) =================
            case 9:
            {
                int power = row + 1;
                int val = A[index];

                int result = 1;
                for (int i = 0; i < power; i++)
                    result *= val;

                C[index] = result;
                break;
            }

            // ================= NON-BORDER → 1’s COMPLEMENT =================
            case 10:
                if (row > 0 && row < rows - 1 && col > 0 && col < cols - 1)
                    C[index] = ~A[index];
                else
                    C[index] = A[index];
                break;

            // ================= ROW SUM + COLUMN SUM =================
            case 11:
            {
                int rowSum = 0, colSum = 0;

                for (int j = 0; j < cols; j++)
                    rowSum += A[row * cols + j];

                for (int i = 0; i < rows; i++)
                    colSum += A[i * cols + col];

                C[index] = rowSum + colSum;
                break;
            }
        }
    }

    // ================= CSR SPARSE MATRIX =================
    if (mode == 12)
    {
        int row = idx;

        if (row < rows)
        {
            int sum = 0;

            for (int j = extra[row]; j < extra[row + 1]; j++)
            {
                sum += A[j] * B[extra[rows + j]]; // values * vector
            }

            C[row] = sum;
        }
    }
}

// ================= MAIN =================
int main()
{
    int rows = 3, cols = 3, mode;

    printf("Enter mode (1-12): ");
    scanf("%d", &mode);

    int size = rows * cols * sizeof(int);

    int h_A[N], h_B[N], h_C[N], h_extra[N];

    printf("Enter Matrix A:\n");
    for (int i = 0; i < rows * cols; i++)
        scanf("%d", &h_A[i]);

    if (mode <= 6)   // needs B matrix
    {
        printf("Enter Matrix B:\n");
        for (int i = 0; i < rows * cols; i++)
            scanf("%d", &h_B[i]);
    }

    int *d_A, *d_B, *d_C, *d_extra;

    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);
    cudaMalloc((void **)&d_extra, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((cols + 15)/16, (rows + 15)/16);

    kernel<<<blocks, threads>>>(d_A, d_B, d_C, d_extra, rows, cols, mode);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < rows * cols; i++)
        printf("%d ", h_C[i]);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(d_extra);

    return 0;
}