/* -------------------------------------------------------------------

	Title:
		UGraph_Util

	Description:
		Misc graph utilities.

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School of Medicine
		462-CB Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		14 July, 1999
			Modified for BeOS 4.5
		25 October, 1995
			Adapted for MetroWerks C++
		17 May, 85
			First IBM version by author.

------------------------------------------------------------------- */

#include "UGraph_Util.h"
#include <math.h>
#include <stdio.h>

// -------------------------------------------------------------------
void
UGraph_Util::AutoScale(WGGraphAxis* axis, float min, float max)
{
	int iter;
	
	if(min >= max) {
		return;
	}
	float span = ConvertDown((max-min)/10.0);	// Find a 'nice' number
	float tMax = ConvertUp(max);                // Find another 'nice' one
	
	while(tMax > max) {
		tMax = tMax - span;
	}
	
	while(tMax < max) {
		tMax = tMax + span;
	}  
	iter = 20;
	float tMin = tMax;
	while(tMin > min && iter > 0) {
		tMin = tMin - span;
		iter--;
	}
	// Special case handling for numbers close to zero *)
	if((min >= 0.0) && (tMin < 0.0)) {
		tMin = 0.0;
		tMax = tMin;
		while(tMax < max) {
			tMax = tMax + span;
		}
	}
	
	axis->SetSpan(tMin,tMax);
	axis->SetInterval(span);
}		
// -------------------------------------------------------------------
// Convert to value LOWER than 'val'.
float
UGraph_Util::ConvertDown(float val)
{	
	int exponentOfNum;
	bool isNeg;
	
	if(val == 0.0) {
		return 0.0;
	}
	
	isNeg = val < 0.0;
	val = fabs(val);
	exponentOfNum = 0;
	
	while(val < 1.0) {
		exponentOfNum++;
		val = val * 10.0;
	}
	while(val > 10.0) {
		exponentOfNum--;
		val /= 10.0;
	}
	
	if(val < 1.5) {
		val = 1.0;
		
	} else if(val < 2.5) {
		val = 2.0;
		
	} else if(val < 7.5) {
		val = 5.0;
		
	} else {
		val = 10.0;
	}
	
	while(exponentOfNum < 0) {
		exponentOfNum++;
		val *= 10.0;
	}
	
	while(exponentOfNum > 0) {
		exponentOfNum--;
		val /= 10.0;
	}
	if(isNeg) {
		val *= -1.0;
	}
	return val;
}
// -------------------------------------------------------------------
// Convert to value HIGHER than 'val'.
float
UGraph_Util::ConvertUp(float val)
{
	int exponentOfNum;
	bool isNeg;
	
	if(val == 0.0) {
		return 0.0;
	}
	
	isNeg = val < 0.0;
	val = fabs(val);
	exponentOfNum = 0;
	while(val < 1.0) {
		exponentOfNum++;
		val *= 10.0;
	}
	
	while(val > 10.0) {
		exponentOfNum--;
		val /= 10.0;
	}
		
	if(val <= 1.0) {
		val = 1.0;
		
	} else if(val <= 2.0) {
		val = 2.0;
		
	} else if(val <= 2.5) {
		val = 2.5;
		
	} else if(val <= 5.0) {
		val = 5.0;
	
	} else {
		val = 10.0;
	}
		
	while(exponentOfNum < 0) {
		exponentOfNum++;
		val *= 10.0;
	}
	
	while(exponentOfNum > 0) {
		exponentOfNum--;
		val /= 10.0;
	}
	
	if(isNeg) {
		val *= -1.0;
	}
	return val;
}
// -------------------------------------------------------------------
							  							  
	
