#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[]) {
    int rank,size,err;
    char err_string[MPI_MAX_ERROR_STRING];
    int err_len;
    char word[100],ch;
    char result[1000];
    int counts[100],displs[100];
    char local[100];

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
        printf("Enter the word:\n");
        scanf("%s",word);
    }

    MPI_Scatter(word,1,MPI_CHAR,&ch,1,MPI_CHAR,0,MPI_COMM_WORLD);

    for(int i=0;i<=rank;i++) local[i]=ch;
    local[rank+1]='\0';

    for(int i=0;i<size;i++){
        counts[i]=i+1;
        displs[i]=i*(i+1)/2;
    }

    MPI_Gatherv(local,rank+1,MPI_CHAR,result,counts,displs,MPI_CHAR,0,MPI_COMM_WORLD);
    if(rank==0) result[size*(size+1)/2] = '\0';


    if(rank==0) printf("Output word:\n%s\n",result);
    if(rank == 0) {
        printf("Q4, Adarsh Ranjan 230962278\n");
    }


    MPI_Finalize();
    return 0;
}
