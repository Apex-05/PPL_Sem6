#include <stdio.h>
#include <cuda.h>

__global__ void findMinIndex(int *arr, int *minIdx, int n, int i)
{
    int tid = threadIdx.x;
    if (tid == 0)
    {
        int min_index = i;
        int j;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_index])
                min_index = j;
        }
        *minIdx = min_index;
    }
}

int main()
{
    int n = 8;
    int h_arr[8] = {64, 25, 12, 22, 11, 90, 34, 2};
    int *d_arr, *d_minIdx;
    int i, minIdx, temp;

    cudaMalloc((void**)&d_arr, n * sizeof(int));
    cudaMalloc((void**)&d_minIdx, sizeof(int));

    cudaMemcpy(d_arr, h_arr, n * sizeof(int), cudaMemcpyHostToDevice);

    for (i = 0; i < n - 1; i++)
    {
        findMinIndex<<<1, 1>>>(d_arr, d_minIdx, n, i);
        cudaMemcpy(&minIdx, d_minIdx, sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(h_arr, d_arr, n * sizeof(int), cudaMemcpyDeviceToHost);

        temp = h_arr[i];
        h_arr[i] = h_arr[minIdx];
        h_arr[minIdx] = temp;

        cudaMemcpy(d_arr, h_arr, n * sizeof(int), cudaMemcpyHostToDevice);
    }

    cudaMemcpy(h_arr, d_arr, n * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Final Sorted Values:");
    for (i = 0; i < n; i++)
        printf("%d ", h_arr[i]);
    printf("\nAdarsh Ranjan, 230962278");

    cudaFree(d_arr);
    cudaFree(d_minIdx);

    return 0;
}
