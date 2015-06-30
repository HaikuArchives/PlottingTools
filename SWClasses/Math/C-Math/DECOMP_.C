#include "cmt.h"
#include <math.h> /* declare fabs function */
 
/*----------------------------------------------------------------------------*/
Real_Vector decomp_(Real_Matrix m, unsigned n)
{
/*
   routine factors matrix m[n][n] into the product of a Lower and an Upper
   triangular matrices (& a permutation matrix). This factorization is useful
   for several matrix functions, such as: matrix inverse, matrix determinant,
   and solving linear equations.
  ----------------------------------------------------------------------------
   usage:
   m[n][n] is the input matrix to be factored. On exit, m[n][n] is the desired
           output factored matrix (i.e. the original input matrix is destroyed)
     n --- is the length of one of the dimensions of square matrix m[][]
   work[n] is used by the subroutine for scaling, on exit work[] records the
           row swaps that define the permutation matrix for the decomposition
  ----------------------------------------------------------------------------
   m[n][n] is factored into the following decomposition:
 
    m[n][n] = P[n][n]*L[n][n]*U[n][n]
    where:
    P is the permuted identity matrix that records row swaps of m[n][n]
                                      | 1   0   0......|
                                      |l21  1   0......|
    L[n][n] = lower diagonal matrix = |l31 l32  1......|
                                      | .   .   .......|
                                      |ln1 ln2  ln3...1|
 
                                      |u11 u12 u13 ...u1n|
                                      | 0  u22 u23 ...u2n|
    U[n][n] = upper diagonal matrix = | 0   0  u33 ...u3n|
                                      | .   .   .........|
                                      | 0   0   0.....unn|
    Since both L[][] and U[][] are triangular, they can be written onto m[][],
    provided the 1's along the diagonal of L[][] are not stored. Thus,
          U[i][j] = m[i][j] on and above the diagonal of m[][] and
          L[i][j] = m[i][j] off of and below the diagonal of m[][]
  ----------------------------------------------------------------------------
   examples:  For n=3, there are 3!=6 distinct permutation matrices:
            |0|           |1 0 0|           |0|           |1 0 0|
     work[]=|1| ==> P[][]=|0 1 0| ;  work[]=|2| ==> P[][]=|0 0 1|
            |2|           |0 0 1|           |2|           |0 1 0|
 
            |2|           |0 0 1|           |1|           |0 1 0|
     work[]=|2| ==> P[][]=|1 0 0| ;  work[]=|1| ==> P[][]=|1 0 0|
            |2|           |0 1 0|           |2|           |0 0 1|
 
            |2|           |0 0 1|           |1|           |0 1 0|
     work[]=|1| ==> P[][]=|0 1 0| ;  work[]=|2| ==> P[][]=|0 0 1|
            |2|           |1 0 0|           |2|           |1 0 0|
  ----------------------------------------------------------------------------
   LEMMA (1): if P is a permutation matrix, then INV{P} = TRANSPOSE{P}
              i.e., P is orthogonal.
   LEMMA (2): if two rows (or columns) in a matrix are exchanged the
                determinant changes its sign
   LEMMA (3): det(m)=(-1)^swap*{u11*u22*...unn}; where swap = # of row swaps
*/
/*---------------------------------------------------------------------------*/
unsigned i,j,piv,k;
char *fname = "decomp_";
double x,val;
Real tmp,dp;
Real_Vector work;
 
         if ((work = valloc(NULL, n)) == NULL) return NULL;
 
          for(i=0; i<n; i++) {  /* do this for each row */
           x=0.0;
           for(j=0; j<n; j++) {
             dp = (Real) fabs(m[i][j]);
             if(dp > (Real) x)  x = dp;
           }
           if(x <= 0.0) {      /* check for matrix singularity */
                vfree(work);
                matherr_(fname, E_MSING);
                return NULL;
           }
           work[i]=(Real)1.0/x;     /* save the largest element of each row */
         }
/***** do this for each column    *************************************/
       for(i=0; i<n; i++) {
           piv=i;
           x=0.0;
           for(k=i; k<n; k++) {
             dp=m[k][i];
             if(i>0)  for(j=0; j<i; j++) dp -= m[k][j]*m[j][i];
             m[k][i] = (Real) dp;
             val = fabs(dp)*work[k];
             if(x < val) {           /* if (val > x) then val is new pivot */
               piv=k;
               x=val;
             }
           }
           if(x<=0.0) {   /* if largest pivot is < 0 then matrix is singular */
                vfree(work);
                matherr_(fname, E_MSING);
                return NULL;
           }
           if(piv<i) {
                vfree(work);
                matherr_(fname, E_MSING);
                return NULL;
           }
           if(piv>i) {        /* if(ipiv>i) then interchange row piv */
                for (j=0; j<n; j++) {   /* with row i                */
                     tmp = m[i][j];
                     m[i][j] = m[piv][j];
                     m[piv][j] = tmp;
                }
                work[piv]=work[i]; /* record row swaps in permutation vector */
           }
           work[i] = (Real) piv;
           if(i < n-1)  {
                x=1.0/m[i][i];
                for(k=i+1; k<n; k++) {
                     m[k][i] *= x;  /* scale data with selected pivot */
                     if(i>0) {
                          dp=m[i][k];
                          for(j=0; j<i; j++) dp -= m[i][j]*m[j][k];
                          m[i][k]=dp;
                     }
                }
           }
       }
       return work;
}
