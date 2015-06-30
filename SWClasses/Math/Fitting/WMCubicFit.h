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

#ifndef WM_CubicFit
#define WM_CubicFit

#include "WMFunctBase.h"


class WMCubicFit : public WMFunctBase {

	public:
		WMCubicFit(DataSet* dArray, int maxPoints);
		
		WMCubicFit(DataFitSet* dArray, int maxPoints);
	virtual	
		~WMCubicFit();
		
	float
		CalcYFromX(float x);
		
	// NOTE - The default parameters are the 'natural' parameters for
	// use by the fit at either extreme.  Unless you know for sure what
	// the beginning and terminal conditions are, you should use these
	// parameters.  For a discussion, see the text mentioned above.
	virtual void
		FitData(float param0 = 0, float paramN = 0);
		
	private:
	
	DataSet*	mSet;
	DataFitSet*	mFitSet;
	float*		mX;
	float*		mY;
	float*		mY2;
	int			mMaxPts;
	int			mUsePts;
	bool		mFitted;
};

#endif
