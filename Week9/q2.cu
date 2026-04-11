#include <stdio.h>
#include <cuda.h>
#include <math.h>

__global__ void transform(float *A, int m, int n) {
    int row = blockIdx.y;
    int col = threadIdx.x;

    if (row < m && col < n) {
        int idx = row * n + col;
        A[idx] = pow(A[idx], row + 1); 
    }
}

int main() {
    int m = 3, n = 3;
    float A[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    float *d_A;
    cudaMalloc(&d_A, sizeof(A));
    cudaMemcpy(d_A, A, sizeof(A), cudaMemcpyHostToDevice);

    dim3 grid(1, m);
    dim3 block(n);

    transform<<<grid, block>>>(d_A, m, n);

    cudaMemcpy(A, d_A, sizeof(A), cudaMemcpyDeviceToHost);

    printf("Output:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%.0f ", A[i*n + j]);
        printf("\n");
    }
    printf("\nAdarsh Ranjan, 230962278");


    cudaFree(d_A);
    return 0;
}