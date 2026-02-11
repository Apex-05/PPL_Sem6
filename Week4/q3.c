#include <mpi.h>
#include <stdio.h>

int main(int argc,char *argv[]) {
    int rank,size,err;
    char err_string[MPI_MAX_ERROR_STRING];
    int err_len;
    int matrix[16],row[4],result[16];

    err=MPI_Init(&argc,&argv);
    if(err!=MPI_SUCCESS){
        MPI_Error_string(err,err_string,&err_len);
        printf("%s\n",err_string);
        MPI_Abort(MPI_COMM_WORLD,err);
    }

    MPI_Comm_set_errhandler(MPI_COMM_WORLD,MPI_ERRORS_RETURN);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0){
        printf("Enter elements of 4x4 matrix:\n");
        for(int i=0;i<16;i++)
            scanf("%d",&matrix[i]);
    }

    MPI_Scatter(matrix,4,MPI_INT,row,4,MPI_INT,0,MPI_COMM_WORLD);

    for(int i=0;i<4;i++)
        row[i]+=rank;

    MPI_Gather(row,4,MPI_INT,result,4,MPI_INT,0,MPI_COMM_WORLD);

    if(rank==0){
        printf("Output matrix:\n");
        for(int i=0;i<16;i++){
            printf("%d ",result[i]);
            if((i+1)%4==0) printf("\n");
        }
    }
    if(rank == 0) {
        printf("Q3, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    return 0;
}
