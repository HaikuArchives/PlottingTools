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

#ifndef GD_Data
#define GD_Data

#include "WMCubicFit.h"
#include "WMFunctBase.h"
#include "WMPolyFit.h"

const int	max_points = 25;	// Max # of data points

class GDData : public WMFunctBase {

	public:
			GDData();
			
		virtual
			~GDData();
		
	bool	// Tries to add a data point to the array
		AddPoint(float xVal, float yVal);
		
	float	// Returns a value based on fitted data
		CalcYFromX(float xVal);
		
	void	// Clears all of the data points
		ClearData();
		
	float	// The chi-sq value, if a polynomial fit
		ChiSqValue();
		
	bool	// Find and delete a point in specified region, if any
		DeletePoint(float xVal, float xErr,
						float yVal, float yErr);
	int
		GetFitParams() {return mFitParams;}
		
	float	// Returns the requested parameter number
		GetParam(int paramNum);
		
	bool	// Returns true if the requested set is valid
		GetPoint(int index, float &x, float &y);
		
	bool	// Returns true if a valid fit of some kind
		HaveFit() {return mDataFitted;}
		
	int
		MaxPoints() {return max_points;}
			
	void	// If < 2, a cubic spline fit, otherwise a polynomial(numParams) fit
		SetFitParams(int numParams);
		
	protected:
	void
		DoFit();
		
	void
		SortData();
	
	public : // Data
	DataFitSet		mFitSet[max_points];
	short			mFitParams;
	float			mChiSq;
	bool			mChiSqValid;
	bool			mDataFitted;
	
	protected:	// Data
	WMPolyFit*		mPolyFit;	
	WMCubicFit*		mCubicFit;
};

#endif