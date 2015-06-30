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

#ifndef _WSLinearRegr
#define _WSLinearRegr

#include "WMFunctBase.h"
#include <AppKit.h>

class WSLinearRegr : public WMFunctBase {

	public:
	
		WSLinearRegr();
		virtual ~WSLinearRegr();
		
	virtual void	// Adds a data pair to the statistics
		AddData(float x, float y);
	virtual void	// Calculates the regression & correlation
		CalcRegression();
	virtual float
		CalcXFromY(float yVal);
	virtual float
		CalcYFromX(float xVal);
	virtual void	// Clears all the data
		ClearData();	
	virtual void	// Removes a single data point
		RemoveData(float x, float y);
		
	//--------------------------------------------	
	// A 'data window' can be set so as to automatically
	// exclude outliers from being included into the data
	// base.
	bool
		HasDataWindow()	const	{return mUseLimits;}
	void
		SetDataWindow(float xMin, float xMax,
						  float yMin, float yMax)
						  {
						  		mLimitXMin = xMin; mLimitXMax = xMax;
						  		mLimitYMin = yMin; mLimitYMax = yMax;
						  }
	void
		UseDataWindow(bool use)		{mUseLimits = use; ResetFlags();}
		
	//--------------------------------------------	
	// These are always valid
	int
		PointsIn() const			{return mPointsIn;}
	int
		PointsOut()	const			{return mPointsOut;}
	float
		MeanX() const				{return mMeanX;}
	float
		MeanY() const				{return mMeanY;}
		
	//--------------------------------------------	
	// Types of data returned after "CalcRegression()"
	//--------------------------------------------	
	bool
		SDValid() const				{return mValidSD;};
	// These are valid after "SDValid()"
	float
		StdDevX() const				{return mStdDevX;}
	float
		StdDevY() const				{return mStdDevY;}
		
	//--------------------------------------------	
	bool
		RegressionValid() const		{return mValidRegr;}
	// These are valid if "RegressionValid()"
	float
		CorrelRValue() const		{return mRValue;}
	float
		RegressionStdError() const	{return mStdErr;}
	float
		CorrelTValue() const		{return mTValue;}
	float
		RegressionIntercept() const	{return mIntercept;}
	float
		RegressionSDIntercept() const {return mStdDevIntercept;}
	float
		RegressionSDSlope() const		{return mStdDevSlope;}
	float
		RegressionSlope() const			{return mSlope;}
		
	//--------------------------------------------	
	bool
		ProbabilityValid() const		{return mValidProb;}
	// This is valid if "ProbabilityValid()"
	float
		PValue() const					{return mPValue;}
		
		
	protected: // Methods
	
	void
		ResetFlags();
		
		
	protected: // Data
	
	float		mLimitXMin;	// These are the data window limits
	float		mLimitXMax;
	float		mLimitYMin;
	float		mLimitYMax;
	
	// Data sums and mean values
	float		mSumX;		// Sum of all X-values
	float		mSumY;		// Sum of all Y-values
	float		mSumXX;		// Sum of all X-squared values
	float		mSumYY;		// Sum of all Y-squared values
	float		mSumXY;		// Sum of all X*Y values
	float		mMeanX;		// The mean of X
	float		mMeanY;		// The mean of Y
	
	// Data SDs and regression line
	float		mStdDevX;	// The standard deviation of all the X-values
	float		mStdDevY;	// The standard deviation of all the Y-values
	float		mSlope;		// The least-squares regression slope
	float		mIntercept;	// The least-squares regression Y-intercept
	float		mStdDevSlope;	// The SD of the slope value
	float		mStdDevIntercept;	// The SD of the intercept value
	
	// Data regression correlations
	float		mStdErr;		// Standard error of the measurement
	float		mTValue;		// Significance (T-value) of the data pairs
	float		mPValue; 	// Significance (P-value) of the data pairs
	float		mRValue; 	// Pearson's R-value (regression coefficient)

	int16		mPointsIn;	// Number of points within the data window
	int16		mPointsOut;	// Number of points outside of the data window
	
	bool		mUseLimits;	// Is the data window in effect?
	bool		mValidSD;	// Are the SD values valid?
	bool		mValidRegr;	// Are the regression values valid?
	bool		mValidProb;	// Is the probability value valid?
};
#endif

