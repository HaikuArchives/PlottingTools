/*----------------------------------------------------------------------------*/
#include "cmt.h"
#include <math.h>
double nprob(double x0)
{
/*
   routine approximates the cumulative normal distribution function and
   given x0, computes the probability P(x <= x0). The method uses a truncated
   power series from Abramowitz and Stegun (pg. 932 eqn 26.2.17). The error of
   the approximation is less than 7.5e-8. (see also invprob())
 
   subroutine pseries() is also required.
*/
double z,p,x,t;
double b[6];

       b[0]=0.0;         b[1]=0.31938153;   b[2]=-0.356563782;
       b[3]=1.781477937; b[4]=-1.821255978; b[5]=1.330274429;
       x = fabs(x0);
       z = exp(-x*x/2.0)/sqrt(twopi_);
       t = 1.0/(1.0+0.2316419*x);
       p = 1.0 - z*pseries(t, b, 6);
       if(x0 < 0.0)  p = 1.0 - p;
       return p;
}
