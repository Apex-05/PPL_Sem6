#include <stdio.h>
#include <cuda.h>

__global__ void oddEvenSortStep(int *arr, int n, int phase)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int i = 2 * tid + phase;
    if (i + 1 < n)
    {
        if (arr[i] > arr[i + 1])
        {
            int temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
        }
    }
}

int main()
{
    int n = 8;
    int h_arr[8] = {9, 3, 7, 1, 8, 2, 6, 5};
    int *d_arr;
    int i;

    cudaMalloc((void**)&d_arr, n * sizeof(int));
    cudaMemcpy(d_arr, h_arr, n * sizeof(int), cudaMemcpyHostToDevice);

    int threads = n / 2;
    int blockSize = threads;
    int gridSize = 1;

    for (i = 0; i < n; i++)
    {
        int phase = i % 2;
        oddEvenSortStep<<<gridSize, blockSize>>>(d_arr, n, phase);
        cudaDeviceSynchronize();
    }

    cudaMemcpy(h_arr, d_arr, n * sizeof(int), cudaMemcpyDeviceToHost);


    printf("Final Output: ");
    for (i = 0; i < n; i++)
        printf("%d ", h_arr[i]);
    printf("\nAdarsh Ranjan, 230962278");


    cudaFree(d_arr);

    return 0;
}
