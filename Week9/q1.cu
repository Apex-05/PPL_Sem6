#include <stdio.h>
#include <cuda.h>

__global__ void csr_spmv(int n, int *rowPtr, int *colInd, float *val, float *x, float *y) {
    int row = threadIdx.x + blockIdx.x * blockDim.x;

    if (row < n) {
        float sum = 0;
        for (int j = rowPtr[row]; j < rowPtr[row + 1]; j++) {
            sum += val[j] * x[colInd[j]];
        }
        y[row] = sum;
    }
}

int main() {
    int n = 3;

    int rowPtr[] = {0, 2, 4, 5};
    int colInd[] = {0, 2, 0, 1, 2};
    float val[] = {1, 2, 3, 4, 5};

    float x[] = {1, 2, 3};  
    float y[3];

    int *d_rowPtr, *d_colInd;
    float *d_val, *d_x, *d_y;

    cudaMalloc(&d_rowPtr, sizeof(rowPtr));
    cudaMalloc(&d_colInd, sizeof(colInd));
    cudaMalloc(&d_val, sizeof(val));
    cudaMalloc(&d_x, sizeof(x));
    cudaMalloc(&d_y, sizeof(y));

    cudaMemcpy(d_rowPtr, rowPtr, sizeof(rowPtr), cudaMemcpyHostToDevice);
    cudaMemcpy(d_colInd, colInd, sizeof(colInd), cudaMemcpyHostToDevice);
    cudaMemcpy(d_val, val, sizeof(val), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, x, sizeof(x), cudaMemcpyHostToDevice);

    csr_spmv<<<1, n>>>(n, d_rowPtr, d_colInd, d_val, d_x, d_y);

    cudaMemcpy(y, d_y, sizeof(y), cudaMemcpyDeviceToHost);

    printf("Result:\n");
    for (int i = 0; i < n; i++)
        printf("%f ", y[i]);
    printf("\nAdarsh Ranjan, 230962278");

    cudaFree(d_rowPtr); cudaFree(d_colInd);
    cudaFree(d_val); cudaFree(d_x); cudaFree(d_y);

    return 0;
}