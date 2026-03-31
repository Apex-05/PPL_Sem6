#include <stdio.h>
#include <string.h>

__global__ void buildRS(char *S, char *RS, int n) {
    int i = threadIdx.x;

    if (i < n) {
        int pos = 0;

        for (int k = 0; k < i; k++) {
            pos += (k + 1);
        }

        for (int j = 0; j <= i; j++) {
            RS[pos + j] = S[j];
        }
    }
}

int main() {
    char S[] = "PCAP";
    int n = strlen(S);

    int total_len = n * (n + 1) / 2;

    char *d_S, *d_RS;
    char RS[100];

    cudaMalloc((void**)&d_S, n);
    cudaMalloc((void**)&d_RS, total_len);

    cudaMemcpy(d_S, S, n, cudaMemcpyHostToDevice);

    buildRS<<<1, n>>>(d_S, d_RS, n);

    cudaMemcpy(RS, d_RS, total_len, cudaMemcpyDeviceToHost);
    RS[total_len] = '\0';

    printf("Input S: %s\n", S);
    printf("Output RS: %s\n", RS);
    printf("Adarsh Ranjan, 230962278");
    cudaFree(d_S);
    cudaFree(d_RS);

    return 0;
}