/*****************************************************************************/
#include "cmt.h"
#include <stdlib.h>
#include <math.h>
double normal(double sigma, double u)
{
/*
   subroutine returns a Gaussian random variable with mean, u, and
   standard deviation = sigma.
 
    let x(i) be a uniformly distributed random number, 0 < x(i) < 1
 
                      12
                     ___
                     \    [ x(i)  - 6 ]
               Y  =  /__
                     i=1
 
     Y is the standard Gaussian random variable with zero mean
     and unity variance.
 
     A new Gaussian random variable, Z, can be generated with
     mean, u, and standard deviation, sigma, according to:
 
     Z = Y*sigma - u
 
    The method is from:
 
    Rabiner, L.R. and Gold, B. "Theory and Application of Digital Signal
            Processing" pp. 570-571.
*/
double y;
int i;
        y=0.0;
        for (i=1; i <= 12; i++) y += (double) rand()/RAND_MAX;
        return  (y-6)*sigma + u;
}
