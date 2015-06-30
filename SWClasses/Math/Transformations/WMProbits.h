/* -------------------------------------------------------------------

	Title:
		WMProbits

	Description:
		Calculates to and from probit values

	Author:
		Stephen C. Wardlaw, M.D.
		Medical Diagnostic Inventions
		Highrock
		Lyme, CT 06371
		<swardlaw@connix.com>

	Edit History:
		26 June, 2000
			Fixed for gcc compiler
		19 April, 1995
			Changed from integral interpolation to polynomial approximation 
		13 April, 1994
			Modified to be a stand-alone math function
		17 February 1994
			Created
				
------------------------------------------------------------------- */

#ifndef _WMProbits
#define _WMProbits

#include "WMFunctBase.h"

class WMProbits : public WMFunctBase {

	public:
		WMProbits();
		
	virtual
		~WMProbits();
		
	public:
		
	float	// Transforms a percentage (0..100) to a probit
		XformTo(float pctVal);
		
	float	// Transforms a probit to a percentage
		XformFrom(float prob);
		
		
	private:
	
	float
		Series(float val, float* coeff, int numVals);
		
};

#endif
