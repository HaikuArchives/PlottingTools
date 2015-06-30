/*----------------------------------------------------------------------------*/
#include "cmt.h"
#include <math.h>
double invprob(double p0)
{
/* routine returns the value of x0 for a given probability, p0, such that    */
/* cumulative normal distribution function satisfies P(x <= x0) = p0         */
/* The method uses a truncated power series from Abramowitz and Stegun       */
/* (pg. 933 eqn 26.2.23). The error of the approximation is less than 4.5e-4.*/
/* (see also nprob())                                                        */
/*                                                                           */
/* subroutine pseries() is also required.                                    */
/*                                                                           */
 
double q,x0,t;
double c[3];
double d[4];
 
       c[0]=2.515517; c[1]=0.802853; c[2]=0.010328;
       d[0]=1.0; d[1]=1.432788; d[2]=0.189269; d[3]=0.001308;
       if(p0<0.0) {
           matherr_("invprob", E_NEGPROB);
           return -HUGE_VAL;
       }
       else if (p0 == 0.0) { return -HUGE_VAL; }
       else if (p0 >= 1.0) { return HUGE_VAL; }
/* at this point, p0 is a valid probability and:  0 < p0 < 1              */
       q = p0;
       if(q > 0.5)  q = 1.0 - q;
       t = sqrt(log(1.0/(q*q)));
       x0 = t - pseries(t, c, 3)/pseries(t, d, 4);
       if(p0 < 0.5) x0 = -x0;
       return x0;
}
