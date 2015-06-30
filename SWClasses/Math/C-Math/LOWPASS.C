#include "cmt.h"
#include <math.h>
/*--------------------------------------------------------------------------*/
void lowpass(Real weights[], int nweights, double fc, double dB, int half)
{
/*
        subroutine designs a symmetric FIR lowpass filter using a
        Kaiser-Bessel window
 
        weights[] is the returned array of FIR filter weights
        fc = lowpass cutoff freq (normalized freq) 0 < fc < 0.5
        nweights = filter length
        dB = the peak stopband attenuation (in decibels) that is desired
        if(half==0) the full impulse response is returned (all nweights points)
        if(half!=0) only half of the impulse response is returned
*/
int m,nn,nd2,i;
double p,fn2,b,bi1,bi2,bi3,fi,fidfn2;
 
        nd2=nweights/2;
        nn=nweights-(nd2+nd2);
 
        fn2=(double)(nweights)/2.;
        m=nweights/2;
        dB = fabs(dB);
        b = stopbnd_(dB);
        p=twopi_*fc;
/********** create filter ************************************************/
        if (nn == 1) weights[m]=(Real)fc+fc;
        bi1=besi0_(b)*pi_;
        for (i=1; i<=m; i++) {
              fi=(double)(i);
              if(nn == 0) fi -= 0.5;
              fidfn2=fi/fn2;
              bi2=b*sqrt(1.0-fidfn2*fidfn2);
              bi3=sin(p*fi)*besi0_(bi2)/(bi1*fi);
              weights[m-i]=(Real)bi3;
        }
        if(half==0) for(i=0; i<m; i++) weights[nweights-1-i]=weights[i];
}
