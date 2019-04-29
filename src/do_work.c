#include <math.h>
#include <mpi.h>
#include "real.h"

#include<time.h>

real do_work(clock_t mseconds) {
    
    clock_t goal = mseconds + clock();
    
    real etime = -MPI_Wtime();
    while (goal > clock());
    etime += MPI_Wtime();

    return etime;
} // end of do_work() //


