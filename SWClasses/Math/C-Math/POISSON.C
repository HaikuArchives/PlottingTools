/*****************************************************************************/
#include "cmt.h"
#include <math.h>
#include <stdlib.h>
unsigned poisson(int pmean)
{
/*
   subroutine returns a Poisson random variable with mean and variance = pmean
 
   pmean   is the input desired mean and variance of the Poisson process (>0)
   k       is also an integer and is the returned Poisson random variable
 
   method:
   1)  let x(i) be a uniformly distributed random number, 0 < x(i) < 1
   2)  y(i) = - ln [x(i)]  is exponentially distributed (mean = var = 1)
 
                      k
                     ___
                     \   - ln [x(i)]  for k = 1,2,... until Y >= pmean
        form   Y  =  /__
                     i=1
 
        or equivalently:
 
                     k
                   ______
                    |  |                                     -pmean
        form   X =  |  |  x(i)    for k=1,2,...  until X <= e
                     i=1
 
        then, in either case, k is the Poisson random variable and E(k) = pmean
        and VAR(k) = pmean.
 
*/
 
double prod,thresh,fmean,rmin,temp;
unsigned k;
 
        if(pmean <= 0) return 0;
        k = 1;
        fmean = (double) -pmean;
        thresh = exp(fmean);
        rmin = 1./RAND_MAX;
        prod = (double) rand()*rmin;
        if(prod < rmin) prod = rmin;
        while (prod >= thresh) {
             temp = (double) rand()*rmin;
             if(temp < rmin) temp = rmin;
             prod *= temp;
             k++;
        }
        return k;
}
