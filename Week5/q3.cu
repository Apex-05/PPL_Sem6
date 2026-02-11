#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

__global__ void computeSine(float *angles, float *output, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        output[i] = sinf(angles[i]);
    }
}

int main() {
    int N;
    scanf("%d", &N);

    size_t size = N * sizeof(float);

    float *h_angles = (float*)malloc(size);
    float *h_output = (float*)malloc(size);

    for (int i = 0; i < N; i++) {
        scanf("%f", &h_angles[i]);
    }

    float *d_angles, *d_output;

    cudaMalloc(&d_angles, size);
    cudaMalloc(&d_output, size);

    cudaMemcpy(d_angles, h_angles, size, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;

    computeSine<<<blocks, threadsPerBlock>>>(d_angles, d_output, N);

    cudaMemcpy(h_output, d_output, size, cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        printf("%f ", h_output[i]);
    }

    cudaFree(d_angles);
    cudaFree(d_output);

    free(h_angles);
    free(h_output);

    return 0;
}
