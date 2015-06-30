#include "cmt.h"
#include <math.h>
/*------------------------------------------------------------------------*/
Real_Vector least_sq(int (*funct)(Real, Real_Vector, unsigned), Real_Vector vx,
                     Real_Vector vy, unsigned n, unsigned nord1, Real *mse)
{
/*------------------------------------------------------------------------*/
/*
 subroutine finds the generalized least squares vector s[*] that fits
 the input data:
 
 p[x] = s[0]*f0(x) + s[1]*f1(x) + s[2]*f2(x) + s[3]*f3(x) + ...
                                                 ... + s[nord1-1]*f(nord1-1)(x)
 
        (*funct)() ==   user-defined pointer to generalized function; prototype:
                        void (*funct)(Real x, Real_Vector fx, int nord1);
                        where fx[0] = f0(x), fx[1] = f1(x), fx[2] = f2(x), ...
            vx[n]  ==   one dimensional vector of x values of data (length n)
            vy[n]  ==   one dimensional vector of y values of data (length n)
               n   ==   length of vectors vx[] and vy[]
            nord1  ==   number of terms in the approximating function
             *mse  ==   the mean square error of the approximation
         s[nord1]  ==   returned vector of polynomial coefficients
 
    method: solve the following set of normal equations:
 
    |  ___       ___       ___         |   |      |        | ___     |
    |  \  f0*f0  \  f0*f1  \  f0*f2... |   | s[0] |        | \  f0*y |
    |  /__       /__       /__         |   |      |        | /__     |
    |                                  |   |      |        |         |
    |  ___       ___       ___         |   |      |        | ___     |
    |  \  f1*f0  \  f1*f1  \  f1*f2... |   | s[1] |        | \  f1*y |
    |  /__       /__       /__         |   |      |        | /__     |
    |                                  |   |      |   =    |         |
    |  ___       ___       ___         |   |      |        | ___     |
    |  \  f2*f0  \  f2*f1  \  f2*f2... |   | s[2] |        | \  f2*y |
    |  /__       /__       /__  .      |   |      |        | /__     |
    |                             .    |   |      |        |         |
    |   .         .         .       .  |   |  .   |        |  .      |
    |   .         .         .          |   |  .   |        |  .      |
    |   .         .         .          |   |  .   |        |  .      |
 
 
reference:
 
R.W. Hamming, "Numerical Methods for Scientists and Engineers," 2nd edition,
               Chapt. 25, Least Squares: Theory, Nonpolynomial Least Squares
               and other Generalizations, pp. 441-442.
 
*/
/*------------------------------------------------------------------------*/
unsigned i,j,k;
int usetemp;
double sum;
Real_Vector s,fx,stmp;
Real_Matrix m;
 
        if( vx == NULL || vy == NULL) {
            matherr_("least_sq", E_NULLPTR);
            return NULL;
        }
 
/* allocate temporary work arrays  */
        if ((m = mxalloc(NULL, nord1, nord1)) == NULL) return NULL;
        if ((fx = valloc(NULL, nord1)) == NULL) {
            mxfree(m);
            return NULL;
        }
        if ((stmp = valloc(NULL, nord1)) == NULL) {
            vfree(fx);
            mxfree(m);
            return NULL;
        }
 
/* zero out arrays */
        mxinit(m, nord1, nord1, (Real) 0.0);
        vinit(stmp, nord1, (Real) 0.0);
 
/* define upper diagonal of symmetric normal system matrix */
        for (i=0; i<n; i++) {
            if ((*funct)(vx[i], fx, nord1) != 0) {
                matherr_("least_sq", E_USERFUNC);
                mxfree(m);
                vfree(fx);
                vfree(stmp);
                return NULL;
            }
            for (k=0; k<nord1; k++) {
                for (j=k; j<nord1; j++) {
                    m[k][j] += fx[k]*fx[j];
                }
                stmp[k] += vy[i]*fx[k];
            }
        }
 
/* define lower diagonal of symmetric normal system matrix */
        for (k=1; k<nord1; k++) {
            for (j=0; j<k; j++)  m[k][j] = m[j][k];
        }
 
/* now find the least squares solution :      */
        usetemp = useinput_;
        useinput_ = 1;           /* set global flag for in-place computation */
        s = lineqn(m, stmp, nord1); /* solve the normal equations */
        useinput_ = usetemp;        /* reset global flag */
 
        if(s == NULL) {
             mxfree(m);   /* deallocate temporary matrix m[][]  */
             vfree(fx);   /* deallocate temporary vector fx[]   */
             vfree(stmp); /* deallocate temporary vector stmp[] */
             return NULL;
        }
 
/* find the error of the approximation       */
 
        *mse = (Real) 0.0;
        for (i=0; i<n; i++) {
            (*funct)(vx[i], fx, nord1);
            sum = 0.0;
            for (j=0; j<nord1; j++) sum += fx[j]*s[j];
            sum = vy[i] - sum;
            *mse += (Real) sum*sum;
        }
 
        mxfree(m);    /* deallocate temporary matrix m[][]  */
        vfree(fx);    /* deallocate temporary vector fx[]   */
        return s;
}
