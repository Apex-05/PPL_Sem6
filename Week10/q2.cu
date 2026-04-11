#include <stdio.h>
#include <cuda_runtime.h>

#define N 8
#define MASK_WIDTH 3

__constant__ int M[MASK_WIDTH];

__global__ void convolution1DConstant(int *N_arr, int *P, int width) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int radius = MASK_WIDTH / 2;

    if (i < width) {
        int sum = 0;
        for (int j = 0; j < MASK_WIDTH; j++) {
            int idx = i - radius + j;
            if (idx >= 0 && idx < width) {
                sum += N_arr[idx] * M[j];
            }
        }
        P[i] = sum;
    }
}

int main() {
    int h_N[N] = {1,2,3,4,5,6,7,8};
    int h_M[MASK_WIDTH] = {1,0,-1};
    int h_P[N];

    int *d_N, *d_P;

    cudaMalloc((void**)&d_N, N * sizeof(int));
    cudaMalloc((void**)&d_P, N * sizeof(int));

    cudaMemcpy(d_N, h_N, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(M, h_M, MASK_WIDTH * sizeof(int));

    convolution1DConstant<<<1, N>>>(d_N, d_P, N);

    cudaMemcpy(h_P, d_P, N * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        printf("%d ", h_P[i]);
    }
    printf("\n");
    printf("Adarsh Ranjan, 230962278");


    cudaFree(d_N);
    cudaFree(d_P);

    return 0;
}