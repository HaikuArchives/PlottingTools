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

#include "WMProbits.h"

#include <math.h>

const float pi = 3.14259;

// ------------------------------------------------------------------- 
//  * CONSTRUCTOR
// ------------------------------------------------------------------- 
WMProbits::WMProbits() 
{
}
// ------------------------------------------------------------------- 
// *  DESTRUCTOR
// ------------------------------------------------------------------- 
WMProbits::~WMProbits() 
{
	// Nothing to destroy
}
// ------------------------------------------------------------------- 
// *  PUBLIC METHODS
// ------------------------------------------------------------------- 
// Finds the probit value from the percentage value
//This method uses a truncated power series from Abramowitz and Stegun 
//(pg. 932 eqn 26.2.17). The error ofthe approximation is less than 7.5e-8.
float
WMProbits::XformTo(float pctVal) 
{
	float c[3] = {2.515517,0.802853,0.010328};
	float d[4] = {1.0,1.432788,0.189269,0.001308};
 
 	pctVal /= 100.0;	// Convert to fraction
 	// Check for proper bounds
   if(pctVal <= 0 || pctVal >= 1.0) {
   	mMathErr = 1;
   	return 0;
   } else {
   	mMathErr = 0;
   }
   
   bool over50 = (pctVal > 0.5);
   if(over50) {
   	pctVal = 1.0 - pctVal;
   }
   float t = sqrt(log(1.0/(pctVal*pctVal)));
   float probit = t - Series(t, c, 3)/Series(t, d, 4);
   
   
	if(!over50) {
		probit *= -1;
	}
	return probit + 5;
 }
// ------------------------------------------------------------------- 
//	Transform from probit to a percentage
// The method uses a truncated power series from Abramowitz and Stegun
// (pg. 933 eqn 26.2.23). The approximation error is less than 4.5e-4
float
WMProbits::XformFrom(float probit)
{
	float coef[6] = {0,0.31938153,-0.356563782,1.781477937,-1.821255978,1.330274429};
	mMathErr = 0;
	
	// Convert the probit (0..10) to a (-5..5) base
	bool negVal = (probit < 5.0);
	float cProb = fabs(probit - 5.0);
	
   float z = exp(-cProb*cProb/2.0)/sqrt(2*pi);
   float t = 1.0/(1.0 + 0.2316419*cProb);
   
   float pctVal = 1.0 - z*Series(t,coef,6);
  	if(negVal) {
   	pctVal = 1.0 - pctVal;
   }
   return pctVal*100.0;
}
// ------------------------------------------------------------------- 
// *  PRIVATE METHODS
// ------------------------------------------------------------------- 
// Performs a coefficient power series
float
WMProbits::Series(float val, float* coeff, int numVals)
{
	float sum = coeff[numVals - 1];
 
 	for(int n = numVals - 2; n >= 0; n--) {
 		sum = sum*val + coeff[n];
 	}
 	return sum;
}

// ------------------------------------------------------------------- 
