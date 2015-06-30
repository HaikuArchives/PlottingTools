#include "cmt.h"
#include <math.h>
int fpoly_(Real x, Real_Vector p, unsigned np);
/*------------------------------------------------------------------------*/
Real_Vector curvreg(Real_Vector vx, Real_Vector vy, unsigned n,
                    unsigned nord1, Real *mse)
{
/*------------------------------------------------------------------------*/
/*
 subroutine finds the minimum mean square polynomial that fits the input data:
 
 p[x] = s[0] + s[1]*x + s[2]*x^2 + s[3]*x^3 + ... + s[nord1-1]*x^(nord1-1)
 
               n   ==   length of vectors vx[] and vy[]
            vx[n]  ==   one dimensional vector of x values of data (length n)
            vy[n]  ==   one dimensional vector of y values of data (length n)
            nord1  ==   degree of the chosen polynomial + 1
   m[nord1][nord1] ==   internal work matrix of size nord1 by nord1
         s[nord1]  ==   returned vector of polynomial coefficients
 
    method: solve the following set of normal equations:
 
    |            ___       ___  2      |   |      |        | ___     |
    |  n         \   x     \   x  .... |   | s[0] |        | \   y   |
    |            /__       /__         |   |      |        | /__     |
    |                                  |   |      |        |         |
    |  ___       ___  2    ___  3      |   |      |        | ___     |
    |  \  x      \   x     \   x  .... |   | s[1] |        | \   x*y |
    |  /__       /__       /__         |   |      |        | /__     |
    |                                  |   |      |   =    |         |
    |  ___  2    ___  3    ___  4      |   |      |        | ___  2  |
    |  \   x     \   x     \   x  .... |   | s[2] |        | \   x *y|
    |  /__       /__       /__  .      |   |      |        | /__     |
    |                             .    |   |      |        |         |
    |   .         .         .       .  |   |  .   |        |  .      |
    |   .         .         .          |   |  .   |        |  .      |
    |   .         .         .          |   |  .   |        |  .      |
 
 
reference:
 
R.W. Hamming, "Numerical Methods for Scientists and Engineers," 2nd edition,
               Chapt. 25, Least Squares: Theory, Polynomial Curve Fitting,
               pp. 437-438.
 
*/
 
Real_Vector s;
 
    s=least_sq(fpoly_, vx, vy, n, nord1, mse);
    return s;
}

int fpoly_(Real x, Real_Vector p, unsigned np)
{
unsigned j;
        p[0]=(Real)1.0;
        for(j=1;j<np;j++) p[j]=p[j-1]*x;
        return 0;
}
