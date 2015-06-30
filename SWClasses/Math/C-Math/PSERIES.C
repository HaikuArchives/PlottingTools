#include "cmt.h"
/*------------------------------------------------------------------------*/
double pseries(double x, double coef[], unsigned n)
{
/* evaluate: sum = coef[0]+x*coef[1]+x^2*coef[2]+...+x^(n-1)*coef[n-1]  */
double sum;
int i;
 
       sum=coef[n-1];
       for (i=n-2; i >=0; i--) sum = sum*x + coef[i];
       return sum;
}
/*------------------------------------------------------------------------*/
