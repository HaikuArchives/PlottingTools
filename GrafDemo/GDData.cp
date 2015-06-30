/* ------------------------------------------------------------------ 

	Title: GDData

	Description:  A class holding and fitting data

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		27 December, 1997
			Adapted for BeOS
		31 March, 1995
			Even more reorganization
		09 Dec 94
			Much alteration for new PP and other classes
		08 May 94
			Added cubic fit routine
		22 Mar 94
			Created for CW C++ from an earlier M2 program
		
------------------------------------------------------------------ */ 

#include "GDData.h"
#include "WMCubicFit.h"
#include "WMPolyFit.h"

// ------------------------------------------------------------------ 
//	* CONSTRUCTOR AND DESTRUCTOR
// ------------------------------------------------------------------ 
// Constructor
GDData::GDData()
{
	mFitParams = 3;	// Default number of fit parameters
	
	// Create polynomial and cubic fit classes
	mPolyFit = new WMPolyFit(mFitSet,max_points);
	mCubicFit = new WMCubicFit(mFitSet,max_points);
	ClearData();
	
}
// ------------------------------------------------------------------ 
GDData::~GDData() 
{
	delete mPolyFit;
	delete mCubicFit;
}
// ------------------------------------------------------------------ 
//	* PUBLIC METHODS
// ------------------------------------------------------------------ 
// Adds a data point if possible
bool
GDData::AddPoint(float xVal, float yVal)
{
	bool canAdd = false;
	// Add the point to an existing open slot
	int n = 0;
	while(n < max_points && mFitSet[n].data.use) {
		n++;
	}
	if(n < max_points) {
		mFitSet[n].data.x = xVal;
		mFitSet[n].data.y = yVal;
		mFitSet[n].data.use = true;
		canAdd = true;
	}
	
	if(canAdd) {
		DoFit();
	}
	return canAdd;
}
// ------------------------------------------------------------------ 
// Given an x value, it returns a y value from the fit
float
GDData::CalcYFromX(float x)
{
	float y;
	
	if(mFitParams > 1) {
		y = mPolyFit->CalcYFromX(x);
		mMathErr = mPolyFit->mMathErr;	// Transfer error data
	} else {
		y = mCubicFit->CalcYFromX(x);
		mMathErr = mCubicFit->mMathErr;	// Transfer error data
	}
	return y;
}
// ------------------------------------------------------------------ 
float
GDData::ChiSqValue()
{
	if(mFitParams > 1 && mDataFitted) {
		return mPolyFit->ChiSqValue();
	} else {
		return 0;
	}
}
// ------------------------------------------------------------------ 
// Clears all of the data
void
GDData::ClearData()
{
	for(int n = 0; n < max_points; n++ ) {
		mFitSet[n].data.x = 0;
		mFitSet[n].data.y = 0;
		mFitSet[n].sig = 1;
		mFitSet[n].data.use = false;
	}
	mChiSqValid = false;
	DoFit();
}
// ------------------------------------------------------------------ 
// Tries to find and delete a specified data point
bool
GDData::DeletePoint(float xVal, float xErr,
							  float yVal, float yErr)
{
	bool anyFound = false;
	
	for(int n = 0; n < max_points; n++) {
		if(mFitSet[n].data.use 
			&& (xVal + xErr) > mFitSet[n].data.x
			&& (xVal - xErr) < mFitSet[n].data.x
			&& (yVal + yErr) > mFitSet[n].data.y
			&& (yVal - yErr) < mFitSet[n].data.y) {
			mFitSet[n].data.use = false;
			anyFound = true;
		}
	}
	
	if(anyFound) {
		DoFit();
	}
	return anyFound;
}
// ------------------------------------------------------------------ 
// Returns the requested parameter number
float
GDData::GetParam(int pNum)
{
	Params param;
	mPolyFit->GetFitParams(param);
	return param[pNum];
}
// ------------------------------------------------------------------ 
// Gets the data point at the index
bool
GDData::GetPoint(int index, float &x, float &y)
{
	if(index >= 0 && index < max_points) {
		x = mFitSet[index].data.x;
		y = mFitSet[index].data.y;
		return mFitSet[index].data.use;
	} else {
		return false;
	}
}
// ------------------------------------------------------------------ 
// Sets the fit parameters
void
GDData::SetFitParams(int numParams)
{
	int newParams = numParams > k_maxFitParams ? k_maxFitParams : numParams;
	if(newParams != mFitParams) {
		mFitParams = newParams;
		DoFit();
	}	
}
// ------------------------------------------------------------------ 
//	* PROTECTED METHODS
// ------------------------------------------------------------------ 
// Fits the data and sets the error state
void
GDData::DoFit()
{
	if(mFitParams > 1) {
		mPolyFit->SetNumParameters(mFitParams);
		mPolyFit->FitData();
		mChiSq = mPolyFit->ChiSqValue();
		mMathErr = mPolyFit->mMathErr;
		mChiSqValid = (mMathErr == 0);
	} else {
		SortData();
		mCubicFit->FitData();
		mMathErr = mCubicFit->mMathErr;
		mChiSqValid = false;
	}
	mDataFitted = (mMathErr == 0);
}
// ------------------------------------------------------------------ 
// Sorts the data for the cubic spline fit.  This is not needed for
// the polynomial fit, but the spline fit requires it.
void
GDData::SortData()
{
	int 			n;
	bool 			sorted;
	DataFitSet	tSet;
	
	// Do an exchange sort to order the X data low..high
	do {
		sorted = true;
		for(n = 0; n < (max_points - 1); n++) {
			if(mFitSet[n].data.x > mFitSet[n + 1].data.x) {
				tSet = mFitSet[n];
				mFitSet[n] = mFitSet[n + 1];
				mFitSet[n + 1] = tSet;
				sorted = false;
			}
		}
		
	} while(!sorted) ;
}
// ------------------------------------------------------------------ 

