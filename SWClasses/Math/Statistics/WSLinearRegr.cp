/* ------------------------------------------------------------------

	Title:
		LinearRegression

	Description:
		Calculates linear regression statistics.

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School Of Medicine
		462-CB  Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		23 July, 1999
			Adapted for BeOS 4.5
		12 May, 1995
			Adapted from M2 to MW C++
		19 Mar 92
			Compiled under MW M2 Vers 4.0
		28 Feb 91
			Adapted to Macintosh model
		23 Dec 88
			Last IBM version 4.0
		06 Aug 84
			First IBM version, adated from Apple-II
			version started in 1982.

------------------------------------------------------------------ */
 
#include "WSLinearRegr.h"
//#include <math.h>

// ------------------------------------------------------------------- 
// * Constructor and Destructor
// ------------------------------------------------------------------- 
WSLinearRegr::WSLinearRegr()
{
	ClearData();
	mUseLimits = false;
	mLimitXMax = FLT_MAX;
	mLimitXMin = FLT_MIN;
	mLimitYMax = FLT_MAX;
	mLimitYMin = FLT_MIN;
}

WSLinearRegr::~WSLinearRegr()
{
}
// ------------------------------------------------------------------- 
// * Public Methods
// ------------------------------------------------------------------- 
void
WSLinearRegr::AddData(float x, float y)
{
	if(mUseLimits &&
			(x > mLimitXMax ||
			 x < mLimitXMin ||
			 y > mLimitYMax ||
			 y < mLimitYMin)) {
		mPointsOut++;
		return;	// *** EXIT HERE if out of window range
	}
			
	mPointsIn++;
	mSumX += x;
	mSumY += y;
	mSumXX += x*x;
	mSumYY += y*y;
	mSumXY += x*y;
	mValidSD = mValidRegr = mValidProb = false;
}
// ------------------------------------------------------------------- 
void
WSLinearRegr::CalcRegression()
{
	float pts = mPointsIn;
	float intX, intY, intXY, intZ, r2, rDiff, y, r, rCos, rCos2, rSin, x;
	int ADF = mPointsIn - 2;
	float adf = ADF;
	int n;
	
	mValidSD = mValidRegr = mValidProb = false;
	
	if(pts > 0) {
		mValidSD = true;
		mMeanX = mSumX/pts;
		mMeanY = mSumY/pts;
	} 
	
	// Calculate the regression stats if possible
	if(pts > 2) {
		mValidRegr = true;
      intX = mSumXX - mSumX * mSumX / pts;
      intY = mSumYY - mSumY * mSumY / pts;
      intXY = mSumXY - mSumX * mSumY / pts;
      mStdDevX = sqrt(intX / (pts - 1.0));
      mStdDevY = sqrt(intY / (pts - 1.0));
		if(intX != 0.0) {
			mSlope = intXY/intX;
			mIntercept = mMeanY - mSlope * mMeanX;
			intZ = fabs(mSumYY - (mIntercept * mSumY + mSlope * mSumXY));
		} else {
			mSlope = FLT_MAX;
			mIntercept = FLT_MIN;
			intZ = FLT_MAX;
		}
		
		mRValue = sqrt(pts * mSumXX - mSumX * mSumX) * sqrt(pts * mSumYY - mSumY * mSumY);
		if(fabs(mRValue) > 0.0) {
			mRValue = (pts * mSumXY - mSumX * mSumY) / mRValue;
		}
		
		// Calculate the t-value and the standard devs of the slope and intercept
		mStdErr = sqrt(intZ/adf);
		r2 = mRValue * mRValue;
		rDiff = 1.0-r2;
		if(rDiff < 1.0E-6) {
			rDiff = 1.0E-6;
		}
		mTValue = sqrt(r2*adf/rDiff);
		if(intX != 0.0) {
			mStdDevSlope = mStdErr*sqrt(1.0/intX);
			mStdDevIntercept = mStdDevSlope * sqrt(mSumXX/pts);
		} else {
			mStdDevSlope = mStdDevIntercept = FLT_MAX;
		}
	}
	
	// Calculate the probability stats if possible
	if(pts > 4) {
		mValidProb = true;
		r = atan(mTValue/sqrt(adf));
		rCos = cos(r);
		rCos2 = rCos*rCos;
		rSin = sin(r);
		x = 1.0;
		if((ADF % 2) == 0) {	// If the degrees of freedom are even
			y = 1.0;
			for(n = 2; n < (ADF - 2); n += 2) {
				x *= rCos2 * (n - 1)/n;
				y += x;
			}
			mPValue = 1.0 - y*rSin;
		
		} else {	// If the degrees of freedom are odd
			if(ADF != 1) {
				y = rCos;
				for(n = 3; n < (ADF - 2); n += 2) {
					x *= rCos2*(n - 1)/n;
					y += x*rCos;
				}
				y = r + rSin*y;
				mPValue = 1.0 - y * 0.6366197723657157;
			} else {	// if ADF == 1
				mPValue = 1.0 - r * 0.6366197723657157;
			}
		}	// if odd ADF
	}	// If pts > 4
}
// ------------------------------------------------------------------- 
float
WSLinearRegr::CalcXFromY(float y)
{
	if(mValidRegr) {
		mMathErr = 0;
		return (y - mIntercept)/mSlope;
	} else {
		mMathErr = -1;
		return 0.0;
	}
}
// ------------------------------------------------------------------- 
float
WSLinearRegr::CalcYFromX(float x)
{
	if(mValidRegr) {
		mMathErr = 0;
		return x*mSlope + mIntercept;
	} else {
		mMathErr = -1;
		return 0.0;
	}
}
// ------------------------------------------------------------------- 
void
WSLinearRegr::ClearData()
{
	mPointsIn = mPointsOut = 0;
	mSumX = mSumY = mSumXX = mSumYY = mSumXY = 0.0;
	ResetFlags();
}
// ------------------------------------------------------------------- 
void
WSLinearRegr::RemoveData(float x, float y)
{
	if(mUseLimits &&
			(x > mLimitXMax ||
			 x < mLimitXMin ||
			 y > mLimitYMax ||
			 y < mLimitYMin)) {
		if(mPointsOut > 0) {
			mPointsOut--;
		}
		return;	// *** EXIT HERE if out of window range
	}
			
	if(mPointsIn > 0) {
		mPointsIn--;	
		mSumX -= x;
		mSumY -= y;
		mSumXX -= x*x;
		mSumYY -= y*y;
		mSumXY -= x*y;
		mValidSD = mValidRegr = mValidProb = false;
	}
}
// ------------------------------------------------------------------- 
// * Protected Methods
// ------------------------------------------------------------------- 
void
WSLinearRegr::ResetFlags()
{
	mValidSD = mValidRegr = mValidProb = false;
}
// ------------------------------------------------------------------- 
