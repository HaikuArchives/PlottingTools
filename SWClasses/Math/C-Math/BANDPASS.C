#include "cmt.h"
#include <math.h>
/*--------------------------------------------------------------------------*/
void bandpass(Real weights[], int nweights, double fh, double fl,
              double dB, int half)
{
/*
        subroutine designs a symmetric FIR bandpass filter using a
        Kaiser-Bessel window
 
        weights[] is the returned array of FIR filter weights
        fh = upper cutoff freq (normalized freq) 0 < fh < 0.5
        fl = upper cutoff freq (normalized freq) 0 < fl < 0.5
        nweights = filter length
        dB = the peak stopband attenuation (in decibels) that is desired
        if(half==0) the full impulse response is returned (all nweights points)
        if(half!=0) only half of the impulse response is returned
*/
int m,nn,nd2,i;
double fn2,bi0,si,b,bi1,bi2,fi,fidfn2,ah,al,ph,pl;
 
        nd2=nweights/2;
        nn=nweights-(nd2+nd2);
 
        fn2=(double)(nweights)/2.;
        m=nweights/2;
        ph = twopi_*fh;
        pl = twopi_*fl;
        dB = fabs(dB);
        b = stopbnd_(dB);
/********** create filter ************************************************/
        if (nn == 1) weights[m] = (Real) 2*(fh-fl);
        bi1=besi0_(b)*pi_;
        for (i=1; i<=m; i++) {
              fi=(double)(i);
              if(nn == 0) fi -= 0.5;
              ah = ph*fi;
              al = pl*fi;
              fidfn2=fi/fn2;
              bi2=b*sqrt(1.0-fidfn2*fidfn2);
              bi0=besi0_(bi2)/bi1;
              si=(sin(ah)-sin(al))/fi;
              weights[m-i]=(Real)si*bi0;
        }
        if(half==0) for(i=0; i<m; i++) weights[nweights-1-i]=weights[i];
}
