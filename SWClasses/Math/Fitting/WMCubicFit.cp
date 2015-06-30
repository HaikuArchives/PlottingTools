/* -------------------------------------------------------------------

	Title:
		WMCubicFit

	Description:
		A cubic spline fitting (interpolation) routine  
		Adapted from "Numerical Recipes" by W.H.Press,from Pascal

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School Of Medicine
		462-CB  Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		27 December, 1997
			Adapted for BeOS
		28 April, 1994
			Created from MetroWerks C++
			
------------------------------------------------------------------------
	This class uses a cubic spline fit to interpolate between data
	points.
	
	NOTE!! This only works in an ordered array and provides what is
	essentially a point-to-point interpolation. The data is NOT fitted,
	and there can be no results outside of the original array bounds.

--------------------------------------------------------------------- */

#include "WMCubicFit.h"

#include <stdio.h>

// -------------------------------------------------------------------
// * CONSTRUCTOR
// -------------------------------------------------------------------
WMCubicFit::WMCubicFit(DataSet* dArray, int maxPoints)
{
	mX = new float[maxPoints];
	mY = new float[maxPoints];
	mY2 = new float[maxPoints];
	mMaxPts = maxPoints;
	mSet = dArray;
	mFitSet = NULL;
	mFitted = false;
}
// -------------------------------------------------------------------
WMCubicFit::WMCubicFit(DataFitSet* dArray, int maxPoints)
{
	mX = new float[maxPoints];
	mY = new float[maxPoints];
	mY2 = new float[maxPoints];
	mMaxPts = maxPoints;
	mFitSet = dArray;
	mSet = NULL;
	mFitted = false;
}
// -------------------------------------------------------------------
// * DESTRUCTOR
// -------------------------------------------------------------------
WMCubicFit::~WMCubicFit()
{
	delete [] mX;
	delete [] mY;
	delete [] mY2;
}
// -------------------------------------------------------------------
// * PUBLIC METHODS
// -------------------------------------------------------------------
		
float
WMCubicFit::CalcYFromX(float x)
{
	int		left, right, indx;
	float	h,b,a,y;
	
	// Start by assuming conversion is ok
	mMathErr = 0;
	
	// Error if out of data bounds
	if(!mFitted || x < mX[0] || x > mX[mUsePts - 1]) {
		mMathErr = 1;
		return 0;
	}
	
	// Do binary search for bracketing values
	left = 0;
	right = mUsePts - 1;
	while(right >= left) {
		indx = (right + left)/2;
		if(x < mX[indx]) {	// If less than table
			right = indx - 1;	// Shift to left;
			
		} else if(x > mX[indx]) {
			left = indx + 1;	// Shift to right 
			
		} else {		// Found exact value
			right = -1;	// Stop search
		}
	}	// while( r > l)
	
	if(right >= 0) {	// If no exact match found, left is >, right is < than value
		h = mX[left] - mX[right];
		a = (mX[left] - x)/h;
		b = (x - mX[right])/h;
		y = a*mY[right] + b*mY[left] + ((a*a*a-a)*mY2[right] + (b*b*b-b)*mY2[left])*(h*h)/6.0;
		return y;
	} else {
		return mY[indx];
	}
}
// -------------------------------------------------------------------
void
WMCubicFit::FitData(float param0, float paramN)
{
	int 		n;
	float 	p,qn,sig,un;
	float*	scratch;
	int		aEnd;
	
	mFitted = false;
	mMathErr = 1;
	
	// Set up internal data, depending upon which type of data array used
	mUsePts = 0;
	if(mSet != NULL) {
		for(n = 0; n < mMaxPts; n++) {
			if(mSet[n].use) {
				mX[mUsePts] = mSet[n].x;
				mY[mUsePts] = mSet[n].y;
				mUsePts++;
			}
		}
	} else if(mFitSet != NULL) {
		for(n = 0; n < mMaxPts; n++) {
			if(mFitSet[n].data.use) {
				mX[mUsePts] = mFitSet[n].data.x;
				mY[mUsePts] = mFitSet[n].data.y;
				mUsePts++;
			}
		}
	}
	
	if(mUsePts > 2) {
		scratch = new float[mUsePts];
		for(n = 0; n < mUsePts; n++) {
			scratch[n] = 0;
		}
	} else {
		return;	// *** EXIT HERE if no data
	}
	
	// Set initial condition
	if(param0 == 0) {
		mY2[0] = 0;
		scratch[0] = 0;
	} else {
		mY2[0] = -0.5;
		scratch[0] = (3.0/(mX[1] - mX[0]))*((mY[1] - mY[0])/(mX[1] - mX[0]) - param0);
	}
	
	// Fill derivative arrays
	aEnd = mUsePts - 1;
	for(n = 1; n < aEnd; n++) {
		sig = (mX[n] - mX[n - 1])/(mX[n + 1] - mX[n - 1]);
		p = sig*mY2[n - 1] + 2.0;
		mY2[n] = (sig - 1.0)/p;
		scratch[n] = (mY[n + 1] - mY[n])/(mX[n + 1] - mX[n]) - (mY[n] - mY[n - 1])/(mX[n] - mX[n - 1]);
		scratch[n] = (6.0*scratch[n]/(mX[n + 1] - mX[n - 1]) - sig*scratch[n - 1])/p;
	}
	
	// Set terminal condition
	if(paramN == 0) {
		qn = 0;
		un = 0;
	} else {
		qn = 0.5;
		un = (3.0/(mX[aEnd] - mX[aEnd - 1]))*(paramN - (mY[aEnd] - mY[aEnd - 1])/(mX[aEnd] - mX[aEnd - 1]));
	}
	
	// Adjust coefficients
	mY2[aEnd] = (un - qn*scratch[aEnd - 1])/(qn*mY2[aEnd - 1] + 1.0);
	for(n = aEnd - 1; n >= 0; n--) {
		mY2[n] = mY2[n]*mY2[n + 1] + scratch[n];
	}
	
	delete [] scratch;
	mFitted = true;
	mMathErr = 0;
}
