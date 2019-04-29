#include <math.h>
#include <sys/time.h>

#include<time.h>

double do_work(double mseconds) 
{
    struct timeval tp;

    double etimeS, etimeE;
    
    gettimeofday(&tp,NULL);
    etimeS = (tp.tv_sec*1.0e6 + tp.tv_usec); 
    
    
    const double goal = etimeS + mseconds;
    double partial;
    
    do {
        gettimeofday(&tp,NULL);
        partial = (tp.tv_sec*1.0e6 + tp.tv_usec) ;
    } while (goal > partial );

    etimeE = (partial-etimeS)*1.0e-6;


    return etimeE;
} // end of do_work() //


