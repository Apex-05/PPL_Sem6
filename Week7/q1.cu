#include <stdio.h>
#include <string.h>

__global__ void countWord(char *sentence, char *word, int *count, int n, int m) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx <= n - m) {
        int match = 1;

        for (int j = 0; j < m; j++) {
            if (sentence[idx + j] != word[j]) {
                match = 0;
                break;
            }
        }

        if (match) {
            atomicAdd(count, 1);
        }
    }
}

int main() {
    char sentence[] = "cuda is fast and cuda is powerful cuda";
    char word[] = "cuda";

    int n = strlen(sentence);
    int m = strlen(word);

    char *d_sentence, *d_word;
    int *d_count;
    int h_count = 0;

    cudaMalloc((void**)&d_sentence, n);
    cudaMalloc((void**)&d_word, m);
    cudaMalloc((void**)&d_count, sizeof(int));

    cudaMemcpy(d_sentence, sentence, n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_word, word, m, cudaMemcpyHostToDevice);
    cudaMemcpy(d_count, &h_count, sizeof(int), cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    countWord<<<blocks, threads>>>(d_sentence, d_word, d_count, n, m);

    cudaMemcpy(&h_count, d_count, sizeof(int), cudaMemcpyDeviceToHost);

    printf("Occurrences of '%s': %d\n", word, h_count);
    printf("Adarsh Ranjan, 230962278");


    cudaFree(d_sentence);
    cudaFree(d_word);
    cudaFree(d_count);

    return 0;
}