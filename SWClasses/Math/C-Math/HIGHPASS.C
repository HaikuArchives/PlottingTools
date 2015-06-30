#include "cmt.h"
#include <math.h>
/*--------------------------------------------------------------------------*/
void highpass(Real weights[], int nweights, double fc, double dB, int half)
{
/*
        subroutine designs a symmetric FIR highpass filter using a
        Kaiser-Bessel window
 
        weights[] is the returned array of FIR filter weights
        fc = highpass cutoff freq (normalized freq) 0 < fc < 0.5
        nweights = filter length
        dB = the peak stopband attenuation (in decibels) that is desired
        if(half==0) the full impulse response is returned (all nweights points)
        if(half!=0) only half of the impulse response is returned
*/
/* call the bandpass routine with fh=0.5, to get a highpass filter */
 
        bandpass(weights, nweights, 0.5, fc, dB, half);
}
