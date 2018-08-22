#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "real.h"


real do_work(unsigned int mseconds);

int main(int argc, char *argv[]) 
{
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize);
    
    if (worldSize != 2) {
        if (worldRank == 0) printf("Please, run using two processes. Quiting ...\n");
        MPI_Finalize();
        exit(-1);
    } // end if // 
    
    //const int mcount=386000000; // this take ~ 1.0 seconds in blackPanther GNU
    //const int mcount=415500000; // this take ~ 1.0 seconds in blackPanther Intel
    const int mcount=387000000; // this take ~ 1.0 seconds in blackPanther Pgi
    MPI_Request req;
    real *rbuf, *sbuf;
    rbuf     = (real *) malloc((mcount)*sizeof(real)); 
    sbuf     = (real *) malloc((mcount)*sizeof(real)); 
    
    if (worldRank == 0) {
        real calcTime=0.0;
        real etime = -MPI_Wtime();
        MPI_Irecv(rbuf,mcount,MPI_MY_REAL,1, 231,MPI_COMM_WORLD,&req);
        //MPI_Recv(rbuf,mcount,MPI_MY_REAL,1, 231,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        calcTime = do_work( (unsigned int)  250000); // 0.25 seconds
        
        MPI_Wait(&req,MPI_STATUS_IGNORE);
        etime += MPI_Wtime();
        printf("%g %g\n",calcTime,etime );
    } else {
        MPI_Send(sbuf,mcount,MPI_MY_REAL,0, 231,MPI_COMM_WORLD);
    }  // end if //
    
    free(sbuf);
    free(rbuf);
    
    MPI_Finalize();

    return 0;
} // end main() //
