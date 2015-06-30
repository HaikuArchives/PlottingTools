/* ------------------------------------------------------------------ 

	Title: WMFunctBase

	Description:  A abstract class which supports some very basic
		math functions.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:	
		03 Dec 97
			Adapted for BeOS
		22 Mar 94
			Developed as a super-class of 'PolyFit' and others
		
------------------------------------------------------------------ */ 

#include "WMFunctBase.h"



WMFunctBase::WMFunctBase() 
{
	mMathErr = 0;
}


float                                                                                 	
WMFunctBase::CalcYFromX(float xVal) 
{
	mMathErr = -1;
	return 0;
}

float
WMFunctBase::XformFrom(float val) 
{
	mMathErr = -1;
	return 0;
}

float
WMFunctBase::XformTo(float val) 
{
	mMathErr = -1;
	return 0;
}
