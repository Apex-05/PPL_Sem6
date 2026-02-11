#include <mpi.h>
#include <stdio.h>

int main(int argc,char *argv[]) {
    int rank,size,err;
    char err_string[MPI_MAX_ERROR_STRING];
    int err_len;
    int matrix[9],local_count=0,total_count=0,search;
    int chunk[3];

    err=MPI_Init(&argc,&argv);
    if(err!=MPI_SUCCESS){MPI_Error_string(err,err_string,&err_len);printf("%s\n",err_string);MPI_Abort(MPI_COMM_WORLD,err);}
    MPI_Comm_set_errhandler(MPI_COMM_WORLD,MPI_ERRORS_RETURN);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);


    if(rank==0){
        printf("Enter the elements \n");

        for(int i=0;i<9;i++) scanf("%d",&matrix[i]);
        printf("Enter element to search: \n");
        scanf("%d",&search);
    }

    MPI_Bcast(&search,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(matrix,3,MPI_INT,chunk,3,MPI_INT,0,MPI_COMM_WORLD);

    for(int i=0;i<3;i++)
        if(chunk[i]==search) local_count++;

    MPI_Reduce(&local_count,&total_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0) printf("Total Count:%d\n",total_count);
    if(rank == 0) {
        printf("Q2, Adarsh Ranjan 230962278\n");
    }

    MPI_Finalize();
    return 0;
}
