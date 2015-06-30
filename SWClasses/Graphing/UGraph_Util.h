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
		25 October, 1995
			Adapted for MetroWerks C++
		17 May, 85
			First IBM version by author.

------------------------------------------------------------------- */

#ifndef U_GraphUtil
#define U_GraphUtil

#include "WGGraphAxis.h"


class UGraph_Util {

	public:

	static void
		AutoScale(WGGraphAxis* axis, float min, float max);
		
	static float
		ConvertDown(float val);
		
	static float
		ConvertUp(float val);
		
};
	
#endif
// -------------------------------------------------------------------
