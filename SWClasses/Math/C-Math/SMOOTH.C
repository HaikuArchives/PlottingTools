/*--------------------------------------------------------------------*/
#include "cmt.h"
Real_Vector smooth(Real_Vector data, int ndata, int factor)
{
/*
   Routine smooths array data[] of length ndata with a Kaiser-Bessel FIR
   lowpass filter. The resulting output (smoothed) array y[] is also of
   length ndata.
 
   factor  controls the amount of smoothing & must be between 2 and 10.
           The most smoothing occurs for factor = 10; the least smoothing
           occurs for factor = 2. The smoothing results from reducing the
           bandwidth of the input data by an amount = factor. Thus; some
           care must be taken, for smoothing with large factors, to assure
           that the input data is appropriately oversampled or the input
           data will be filtered out with the noise.
 
    nfilt  is the length of the FIR filter and is computed by this routine
           empiricallly according to :
 
           (1) nfilt = [1.08*|log10(error_R)|+1.174]*factor + 0.5; where:
 
           error_R is the relative error introduced by the smoothing process.
                         (error_R is set to 1.e-3 in this program)
 
           An equivalent expression for nfilt is given in the program from a
           digital signal processing point of view:
 
           (2) nfilt=[0.027*sl+0.587]/fc + 0.5; where:
 
           sl = |20*log10(error_R)| ; sl is the resampling ambiguity level in dB
           fc = 1./(2*factor); fc is the normalized cutoff freq of the filter
 
    The routine smooths only the central part of the array data[], the
    upper and lower edges (i.e. (i < nfilt/2) or (i > ndata-nfilt/2) ) of the
    array are filtered with the fixed 3-pt. FIR filter: 1/4, 1/2, 1/4.
    There must be at least nfilt data points, or nothing is done.
 */
 
int ndec,itype,isym,i,i1,ny,ndm1,nl,nr,nn,half;
double sl,fc;
static Real h[23]; /* dimensioned to length MAX(nfilt)/2 if half!=0      */
static int nd2,nfilt,factor1;
Real_Vector y;
 
   if( data == NULL) {
       matherr_("smooth", E_NULLPTR);
       return NULL;
   }
 if(factor < 2 && factor > 10) {
      matherr_("smooth", E_FACTOR);
      return NULL;
 }
 
 if(factor1 != factor) {
      factor1=factor;/* design Kaiser-Bessel lowpass FIR filter for smoothing */
      sl = 60.;   /* set low pass filter sidelobe level to about -60 dB */
      fc = 1./(factor+factor);  /* define smoothing filter cutoff frequency */
      nfilt= (int) ((0.027*sl+0.587)/fc + 0.5);  /* for these values; nfilt < 45   */
      nd2=nfilt/2;
      nn=nfilt-(nd2+nd2);
      if(nn == 0) nfilt=nfilt+1;   /* make sure nfilt is odd  */
      half = 1;
      lowpass(h, nfilt, fc, sl, half);
  }
 
  if(ndata < nfilt) {
       matherr_("smooth", E_NOTENOUGH);
       return NULL;
  }
 
/*   convolve data with smoothing filter                                  */
 
  ndec=1;
  itype=1;
  isym=1;
 
  y = convolve(data,ndata,h,nfilt,ndec,itype,isym,&ny);
  if (y == NULL) return NULL;
/*    ny=(int)(ndata-nfilt)/ndec +1 */
 
/* fill edges of smoothed array with a 3pt. weighted average of original data */
 
  ndm1=ndata-1;
  y[ndm1] = data[ndm1];
  for (i=1; i<nd2; i++) { /* filter right edge  of data */
      i1=ndm1-i;
      y[i1]= data[i1-1]+2*data[i1]+data[i1+1];
      y[i1] /= 4.;
  }
 
  nl = ndm1-nd2;
  nr = ny-1;
  for (i=0; i<ny; i++) y[nl-i]=y[nr-i]; /*shift data right nfilt/2 places*/
 
  y[0] = data[0];  /* filter left edge  of data */
  for (i=1; i<nd2; i++) {
       y[i]=data[i-1]+2*data[i]+data[i+1];
       y[i] /= 4.;
  }
  return y;
}
