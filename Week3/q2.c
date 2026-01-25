#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size, M;
    int id[10000];
    int recv[10000];
    double local_avg, total_avg;
    double avgs[1000];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter the Integer M:\n");
        scanf("%d", &M);

        printf("Enter %d elements:\n", M * size);
        for (int i = 0; i < M * size; i++) {
            scanf("%d", &id[i]);
        }
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(id, M, MPI_INT,
                recv, M, MPI_INT,
                0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < M; i++) {
        sum += recv[i];
    }

    local_avg = (double)sum / M;

    MPI_Gather(&local_avg, 1, MPI_DOUBLE,
               avgs, 1, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        double total = 0.0;
        for (int i = 0; i < size; i++) {
            total += avgs[i];
        }
        total_avg = total / size;
        printf("Total Average = %lf\n", total_avg);
        printf("Q2, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    return 0;
}
