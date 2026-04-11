#include <stdio.h>
#include <cuda_runtime.h>

#define WIDTH 8
#define MASK_WIDTH 3
#define TILE_SIZE 4

__constant__ int M[MASK_WIDTH];

__global__ void tiledConvolution1D(int *N, int *P, int width) {
    __shared__ int N_s[TILE_SIZE + MASK_WIDTH - 1];

    int tx = threadIdx.x;
    int start = blockIdx.x * TILE_SIZE;
    int radius = MASK_WIDTH / 2;
    int gIndex = start + tx;

    if (gIndex < width)
        N_s[tx + radius] = N[gIndex];
    else
        N_s[tx + radius] = 0;

    if (tx < radius) {
        int left = gIndex - radius;
        N_s[tx] = (left >= 0) ? N[left] : 0;
    }

    if (tx < radius) {
        int right = gIndex + TILE_SIZE;
        N_s[tx + TILE_SIZE + radius] = (right < width) ? N[right] : 0;
    }

    __syncthreads();

    if (gIndex < width) {
        int sum = 0;
        for (int j = 0; j < MASK_WIDTH; j++) {
            sum += N_s[tx + j] * M[j];
        }
        P[gIndex] = sum;
    }
}

int main() {
    int h_N[WIDTH] = {1,2,3,4,5,6,7,8};
    int h_M[MASK_WIDTH] = {1,0,-1};
    int h_P[WIDTH];

    int *d_N, *d_P;

    cudaMalloc((void**)&d_N, WIDTH * sizeof(int));
    cudaMalloc((void**)&d_P, WIDTH * sizeof(int));

    cudaMemcpy(d_N, h_N, WIDTH * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(M, h_M, MASK_WIDTH * sizeof(int));

    int blocks = (WIDTH + TILE_SIZE - 1) / TILE_SIZE;

    tiledConvolution1D<<<blocks, TILE_SIZE>>>(d_N, d_P, WIDTH);

    cudaMemcpy(h_P, d_P, WIDTH * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < WIDTH; i++) {
        printf("%d ", h_P[i]);
    }
    printf("\n");
    printf("Adarsh Ranjan, 230962278");

    cudaFree(d_N);
    cudaFree(d_P);

    return 0;
}