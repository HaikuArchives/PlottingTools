/* -------------------------------------------------------------------

	Title:
		UFloating_Util

	Description:
		Misc real number utilities.

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School of Medicine
		462-CB Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		19 Jan, 94 - Adapted for MetroWerks C++
		15 Mar, 92 - Added 'LogOfBase' function.
		25 Mar, 91 - Adapted to Macintosh by author.
		24 Sep, 87 - Last (V3.0) IBM version by author.
		17 May, 85 - First IBM version by author.

------------------------------------------------------------------- */

#include "UFloating_Util.h"

#include <math.h>

bool fuMathErr;
	
// -------------------------------------------------------------------
float
BLog(float R) 
{
	if(R > 0.0) {
		fuMathErr = false;
		return log(R)*1.4426950409;      // Ln(R) * 1 / Ln(2)
	} else{
		fuMathErr = true;
		return 0.0;
	}
}
// -------------------------------------------------------------------
float
CLog(float R)	
{
	if(R > 0.0) {
		fuMathErr = false;
		return log(R)*0.4342944819;   // Ln(R) * 1/Ln(10)
	} else {
		fuMathErr = true;
		return 0.0;
	}
}
// -------------------------------------------------------------------
int
Exponent(float num) 
{
	int expn;
	
	fuMathErr = false;
	frexp(num,&expn);
	return expn;
}	
// -------------------------------------------------------------------
// Returns the fractional part of the number.
float
FractPart(float num)
{
	double dummy;
	
	fuMathErr = false;
	return modf(num,&dummy);
}
// -------------------------------------------------------------------
// Returns the whole part of the number.
float
IntPart(float num)
{	
	double intPart;
	
	fuMathErr = false;
	modf(num,&intPart);
	return (float)intPart;
}
// -------------------------------------------------------------------
// Returns the log of 'number' to the base 'base'
float
LogOfBase(float number,float base) 
{
	if((number > 0.0) && (base > 0.0)) {
		fuMathErr = false;
		return log(number)/log(base);
	} else {
		fuMathErr = true;
		return 0.0;
	}
}
// -------------------------------------------------------------------
// Returns the error flag.
bool
MathErr()
{
	return fuMathErr;
}
// -------------------------------------------------------------------
float
Power(float base,float expn) 
{
//	errno = 0;
	return pow(base,expn);
//	fuMathErr = errno;
}
// -------------------------------------------------------------------
float
Root(float num,float root) 
{
	bool	neg;
	float	val,intRoot;
	int		iRoot;
	
	fuMathErr = false;
	if(num == 0.0) {
		return 0.0;
	}
	if(root == 0.0) {
		fuMathErr = true;
		return 0.0;
	}
	neg = (num < 0.0);
	//
	//	If the number is negative, the result is valid only if the root is
	//	an odd integer
	//	
	if(neg) {
		num = fabs(num);
		intRoot = IntPart(root);
		if(intRoot != root) {
			fuMathErr = true;
			return 0.0;
		}
		iRoot = (int)intRoot;	
		if(iRoot == (iRoot/2)*2) {  // If an even number
			fuMathErr = true;
			return 0.0;
		}
	}
	val =  exp(log(num)/root);
	if(neg) {
		val = val* -1.0;
	  }
	return val;   
}
// -------------------------------------------------------------------
