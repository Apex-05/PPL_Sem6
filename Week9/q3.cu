#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void complement_k(int *a, int m, int n) {
    int i = blockIdx.x; 
    int j = threadIdx.x; 

    if (i > 0 && i < m - 1 && j > 0 && j < n - 1) {
        int v = a[i * n + j];
        int res = 0;
        int p = 1;
        if (v == 0) {
            res = 1;
        } else {
            while (v > 0) {
                int bit = v % 2;
                int flipped = (bit == 0) ? 1 : 0;
                res = res + (flipped * p);
                p *= 10;
                v /= 2;
            }
        }
        a[i * n + j] = res;
    }
}

int main() {
    int *a, m, n;
    int *d_a;

    printf("Enter dimensions m and n: ");
    if (scanf("%d %d", &m, &n) != 2) return -1;

    int size = m * n * sizeof(int);
    a = (int*)malloc(size);

    printf("Enter Matrix A:\n");
    for (int i = 0; i < m * n; i++) {
        scanf("%d", &a[i]);
    }

    cudaMalloc((void**)&d_a, size);
    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);

    complement_k<<<m, n>>>(d_a, m, n);
    cudaMemcpy(a, d_a, size, cudaMemcpyDeviceToHost);

    printf("\nResultant Matrix B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", a[i * n + j]);
        }
        printf("\n");
    }
    printf("\nAdarsh Ranjan, 230962278");

    cudaFree(d_a);
    free(a);

    
    return 0;
}