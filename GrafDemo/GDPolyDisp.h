/* ------------------------------------------------------------------ 

	Title: GDPolyDisp

	Description:  A class which puts a window to display
		the results of the polynomial data fitting

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		08 January, 1998
			Adapted for BeOS
		22 Mar 94
			Created for CW C++ from an earlier M2 program
		
------------------------------------------------------------------ */ 

#ifndef GD_PolyDisp
#define GD_PolyDisp

#include "GDData.h"
#include <InterfaceKit.h>


class GDPolyDisp : public BView {

	public:
	
		GDPolyDisp(GDData* theData,
					  BRect frame,  
					  const char* title	= "GDPolyDisp",
					  uint32 mode			= B_FOLLOW_TOP | B_FOLLOW_RIGHT,
					  uint32 flags			= B_WILL_DRAW | 
					  							  B_FULL_UPDATE_ON_RESIZE); 
	protected:
	
	virtual void
		Draw(BRect frame);
	
	GDData*		mData;
	float		mFontDescent;
	float		mLineSpace;
	float 		mSuperScript;
};

#endif
