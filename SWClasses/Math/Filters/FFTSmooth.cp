/* ------------------------------------------------------------------ 	Title: 		FFTSmooth	Description: 		An FFT data smoothing function	Author :		Stephen Wardlaw, M.D.		Yale University School of Medicine		20 York St.		New Haven, CT  06504	Edit History:				03 March, 1995			Created from a routine in 'Numerical Recipes' by W. Press------------------------------------------------------------------ */ #include "FFTSmooth.h"#include <math.h>#include <UException.h>#include <stdio.h> //*********const double two_pi = 3.14159*2;static int pass = 1; //************	// ------------------------------------------------------------------ //	� CONSTRUCTORS & DESTRUCTOR// ------------------------------------------------------------------ FFTSmooth::FFTSmooth(){	mSize = 0;	mWork = nil;}	FFTSmooth::~FFTSmooth(){	delete mWork;}// ------------------------------------------------------------------ //	� Public Methods// ------------------------------------------------------------------ // Smooth the array 'dArray', using a smoothing function 'span' longvoidFFTSmooth::SmoothData(double* dArray, int aryLen, double span){	int mo2,k,j;	double yn, y1, rn1, fac, cnst;		// Create a working array an integral power of two long, which is	// at least as large as (aryLen + 2*span)	delete mWork;	// Delete any existing array	mSize = 2;	int nmin = aryLen + (2.0*span + 0.5);	while(mSize < nmin) {		mSize = 2*mSize;	}	pass = 0; //******************		// Try to make an array of the appropriate size	Try_{					mWork = new double[mSize];				// Transfer the data to the working array		::BlockMove(dArray,mWork,aryLen*sizeof(double));		ShowData(0,pass); //*********************				cnst = (span/mSize)*(span/mSize);		y1 = mWork[0];		yn = mWork[aryLen - 1];		rn1 = 1.0/(aryLen - 1.0);		// Remove any constant slope factor		for (j = 0; j < aryLen; j++) {			mWork[j] = mWork[j] - rn1*(y1*(aryLen - j - 1) + yn*j);		}				// Zero out the portion after the data array		for(j = aryLen; j < mSize; j++) {			mWork[j] = 0.0;		}		ShowData(1,pass); //*********************				mo2 = mSize/2;	// This prevents overflow in the following procedure		RealFFT(mo2,1);		mWork[0] = mWork[0]/mo2;		fac = 1.0;		for(j = 1; j < mo2; j++) {				k = 2*j;			printf("j = %2d  k = %2d\n",j,k); //**************			if(fac != 0) {				fac = (1.0 - cnst*j*j)/mo2;				if(fac < 0.0) {					fac = 0;				}				mWork[k] = fac*mWork[k];				mWork[k + 1] = fac*mWork[k + 1];			} else {				mWork[k] = 0.0;				mWork[k + 1] = 0.0;			}		} // for(j		ShowData(2,pass); //*********************				fac = (1.0 - 0.25*span*span)/mo2;		if(fac < 0.0) {			fac = 0.0;		}		mWork[1] = fac*mWork[1];		RealFFT(mo2,-1);		for(j = 0; j < aryLen; j++) {			mWork[j] = rn1*(y1*(aryLen - j - 1) + yn*j) + mWork[j];		}		ShowData(3,pass); //*********************				// Put smoothed data back into original array		::BlockMove(mWork,dArray,aryLen*sizeof(double));	}	// End Try_		Catch_(inErr) {		delete this;		Throw_(inErr);	} EndCatch_}// ------------------------------------------------------------------ //	� Protected Methods// ------------------------------------------------------------------ voidFFTSmooth::FourSub(int wkSize, int isign){	int ii, jj, nMax, mmax,m,j,istep,i;	double wtemp,wr,wpr,wpi,wi,theta;	short double	tempr,tempi;		nMax = 2*wkSize;	j = 0;	for(ii = 0; ii < wkSize; ii++) {		i = 2*ii;		if(j > i) {			printf("ii = %2d  j = %2d  i = %2d\n",ii,j,i); //********			tempr = mWork[j];			tempi = mWork[j + 1];			mWork[j] = mWork[i];			mWork[j + 1] = mWork[i + 1];			mWork[i] = tempr;			mWork[i + 1] = tempi;			printf("\nj = %2d  i = %2d\n",j,i);		}				m = nMax/2;		while((m >= 2) && (j >= m)) {			j = j - m;			m = m/2;		}		j = j + m;	}		ShowData(4,pass); //*********************		mmax = 2;	while(nMax > mmax) {		istep = 2*mmax;		theta = two_pi/(isign*mmax);		wpr = -2*sin(0.5*theta)*sin(0.5*theta);		wpi = sin(theta);		wr = 1.0;		wi = 0.0;		for(ii = 1; ii <= (mmax/2); ii++) {			m = 2*ii - 1;			for(jj = 0; jj <= (nMax-m)/istep; jj++) {				i = m + jj*istep - 1;				j = i + mmax;				printf("ii = %2d  m = %2d  jj = %2d  i = %2d\n",ii,m,jj,i); //********				tempr = wr*mWork[j] - wi*mWork[j + 1];				tempi = wr*mWork[j + 1] + wi*mWork[j];				mWork[j] = mWork[i] - tempr;				mWork[j + 1] = mWork[i + 1] - tempi;				mWork[i] = mWork[i] + tempr;				mWork[i + 1] = mWork[i + 1] + tempi;			}	// for jj					wtemp = wr;			wr = wr*wpr - wi*wpi + wr;			wi = wi*wpr + wtemp*wpi + wi;		}	// for ii				mmax = istep;	}	// while( nMax > mmax)				ShowData(5,pass); //*********************	pass++; //******** }// ------------------------------------------------------------------ voidFFTSmooth::RealFFT(int wkSize, int isign){	double	wr,wi,wpr,wpi,wtemp,theta;	int	i,i1,i2,i3,i4;	short double	c1,c2,h1r,h1i,h2r,h2i,wrs,wis;	// single precision		theta = two_pi/2*wkSize;	c1 = 0.5;	if(isign == 1) {		c2 = -0.5;		FourSub(wkSize,1);	} else {		c2 = 0.5;		theta = -theta;	}		wpr = -2.0*sin(0.5*theta)*sin(0.5*theta);	wpi = sin(theta);	wr = wpr + 1.0;	wi = wpi;	for(i = 1; i < wkSize/2; i++) {		i1 = i + i;		i2 = i1 + 1;		i3 = wkSize + wkSize + 1 - i2;		i4 = i3 + 1;		printf("i = %2d i1 = %2d  i2 = %2d  13 = %2d  14 = %2d\n",i,i1,i2,i3,i4); //********		wrs = wr;		wis = wi;		h1r = c1*(mWork[i1] + mWork[i3]);		h1i = c1*(mWork[i2] - mWork[i4]);		h2r = -c2*(mWork[i2] + mWork[i4]);		h2i = c2*(mWork[i1] - mWork[i3]);		mWork[i1] = h1r + wrs *h2r - wis*h2i;		mWork[i2] = h1i + wrs*h2i + wis*h2r;		mWork[i3] = h1r - wrs*h2r + wis*h2i;		mWork[i4] = -h1i + wrs*h2i + wis*h2r;		wtemp = wr;		wr = wr*wpr - wi*wpi + wr;		wi = wi*wpr + wtemp*wpi + wi;	}	// for(i		ShowData(6,pass); //*********************		if(isign == 1) {		h1r = mWork[0];		mWork[0] = h1r + mWork[1];		mWork[1] = h1r - mWork[1];	} else {		h1r = mWork[0];		mWork[0] = c1*(h1r + mWork[1]);		mWork[1] = c1*(h1r - mWork[1]);		FourSub(wkSize, -1);	}}// ------------------------------------------------------------------ voidFFTSmooth::ShowData(int posn, int passNum){	printf("\n\nData Array State At %d Position, Pass# %d\n",posn,pass);	for(int n = 0; n < mSize; n++) {		printf("Index = %2d  Data = %f\n",n,mWork[n]);	}	printf("\n");}		