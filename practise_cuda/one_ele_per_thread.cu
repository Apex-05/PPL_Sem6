#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void matAdd(int *A, int *B, int *C, int N, int M) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < N && col < M)
        C[row * M + col] = A[row * M + col] + B[row * M + col];
}

int main() {
    int N = 4, M = 4;
    int size = N * M * sizeof(int);

    int *h_A = (int*)malloc(size);
    int *h_B = (int*)malloc(size);
    int *h_C = (int*)malloc(size);

    for (int i = 0; i < N * M; i++) {
        h_A[i] = i;
        h_B[i] = i;
    }

    int *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((M + 15)/16, (N + 15)/16);

    matAdd<<<blocks, threads>>>(d_A, d_B, d_C, N, M);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    for (int i = 0; i < N * M; i++) printf("%d ", h_C[i]);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    free(h_A);
    free(h_B);
    free(h_C);
    return 0;
}
