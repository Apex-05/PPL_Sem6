#include <stdio.h>
#include <cuda_runtime.h>

__global__ void convolution1D(float *N, float *M, float *P, int width, int mask_width)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int half = mask_width / 2;
    if (i < width)
    {
        float sum = 0.0f;
        for (int j = 0; j < mask_width; j++)
        {
            int idx = i - half + j;
            if (idx >= 0 && idx < width)
                sum += N[idx] * M[j];
        }
        P[i] = sum;
    }
}

int main()
{
    int width = 16;
    int mask_width = 5;

    float h_N[16], h_M[5], h_P[16];

    for (int i = 0; i < width; i++)
        h_N[i] = i + 1;

    for (int i = 0; i < mask_width; i++)
        h_M[i] = 1.0f;

    float *d_N, *d_M, *d_P;

    cudaMalloc((void**)&d_N, width * sizeof(float));
    cudaMalloc((void**)&d_M, mask_width * sizeof(float));
    cudaMalloc((void**)&d_P, width * sizeof(float));

    cudaMemcpy(d_N, h_N, width * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_M, h_M, mask_width * sizeof(float), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int gridSize = (width + blockSize - 1) / blockSize;

    convolution1D<<<gridSize, blockSize>>>(d_N, d_M, d_P, width, mask_width);

    cudaMemcpy(h_P, d_P, width * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < width; i++)
        printf("%f ", h_P[i]);
    printf("\nAdarsh Ranjan, 230962278");

    printf("\n");

    cudaFree(d_N);
    cudaFree(d_M);
    cudaFree(d_P);

    return 0;
}
