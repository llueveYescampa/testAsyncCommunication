#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include<time.h>

#include "real.h"


double do_work(double mseconds);

int main(int argc, char *argv[]) 
{
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);


    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize);

    // creating an intranode communicator 
    MPI_Comm sm_comm;
    MPI_Comm_split_type(MPI_COMM_WORLD,MPI_COMM_TYPE_SHARED, 0,MPI_INFO_NULL, &sm_comm);
    int sharedRank,sharedSize;
    MPI_Comm_rank(sm_comm,&sharedRank);
    MPI_Comm_size(sm_comm,&sharedSize);
    // creating an intranode communicator 

    // creating a communicator for the lead processes in each node
    MPI_Comm nodeComm;
    MPI_Comm_split(MPI_COMM_WORLD, sharedRank, (worldRank/sharedSize), &nodeComm );
    int nodeNumber,numberOfNodes;
    MPI_Comm_rank(nodeComm,&nodeNumber);
    MPI_Comm_size(nodeComm,&numberOfNodes);
    // creating a communicator for the lead processes in each node
    
    if (worldSize != 2) {
        if (worldRank == 0) printf("Please, run using two processes. Quiting ...\n");
        MPI_Finalize();
        exit(-1);
    } // end if // 

    if (worldRank == 0) {
        printf("num of nodes used: %d, worldSize: %d, sharedSize: %d\n", numberOfNodes,worldSize,sharedSize);
    } // end if //
    
    //const int mcount=386000000; // this take ~ 1.0 seconds in blackPanther GNU
    //const int mcount=415500000; // this take ~ 1.0 seconds in blackPanther Intel
    //const int mcount=387000000; // this take ~ 1.0 seconds in blackPanther Pgi
    
    //const int mcount=8870000; // this take ~ 10.0 seconds in blackPanther+blackEngineering Gnu
    const int mcount=8650000; // this take ~ 10.0 seconds in blackPanther+blackEngineering Intel
    //const int mcount=8650000; // this take ~ 10.0 seconds in blackPanther+blackEngineering Pgi
    
    
    MPI_Request req;
    real *rbuf, *sbuf;
    rbuf     = (real *) malloc((mcount)*sizeof(real)); 
    sbuf     = (real *) malloc((mcount)*sizeof(real)); 
    
    if (worldRank == 0) {
        double calcTime=0.0;
        real etime = -MPI_Wtime();
        //MPI_Irecv(rbuf,mcount,MPI_MY_REAL,1, 231,MPI_COMM_WORLD,&req);
        MPI_Isend(sbuf,mcount,MPI_MY_REAL,1, 231,MPI_COMM_WORLD,&req);
        
        //calcTime = do_work( (unsigned int)  250000); // 0.25 seconds
        calcTime = do_work( (double)  2500000); // 2.5 seconds
        
        MPI_Wait(&req,MPI_STATUS_IGNORE);
        etime += MPI_Wtime();
        printf("%g %g\n",calcTime,etime );
    } else {
        //MPI_Send(sbuf,mcount,MPI_MY_REAL,0, 231,MPI_COMM_WORLD);
        MPI_Recv(rbuf,mcount,MPI_MY_REAL,0, 231,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }  // end if //

    
    free(sbuf);
    free(rbuf);
    
    MPI_Finalize();

    return 0;
} // end main() //
