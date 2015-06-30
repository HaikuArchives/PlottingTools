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

#include "GDPolyDisp.h"
#include "URealFmt_Utils.h"

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
GDPolyDisp::GDPolyDisp(GDData* theData,
					   BRect frame,  
					  	const char* title,
					  	uint32 mode,
					  	uint32 flags):BView(frame,title,mode,flags)
{
	// Size the bottom of the frame
	// We will have a max of 5 lines for this view
	mData = theData;
	BFont theFont;
	GetFont(&theFont);
	font_height fHeight;
	theFont.GetHeight(&fHeight);
	mSuperScript = fHeight.ascent/2;
	mLineSpace = fHeight.ascent + fHeight.descent + fHeight.leading + mSuperScript;
	mFontDescent = fHeight.descent;
	ResizeTo(frame.right - frame.left,mLineSpace*5);
}					  
// -------------------------------------------------------------------
// *  Public methods            
// -------------------------------------------------------------------
// *  Protected methods            
// -------------------------------------------------------------------
void
GDPolyDisp::Draw(BRect frame)
{
	const float rt_margin = 2;
	
	char nStr[15];
	
	float	vPosn = mLineSpace - mFontDescent;
	MovePenTo(rt_margin, vPosn);
	DrawString("Fit Params");
	vPosn += mLineSpace;
	MovePenTo(rt_margin, vPosn);
	if(mData->GetFitParams() == 0) {
		DrawString("Cubic Spline Fit");
	} else if(mData->HaveFit()) {
		for(int n = 1; n <= mData->GetFitParams(); n++) {
			MovePenTo(rt_margin,vPosn);
			DrawChar('X');
			MovePenBy(0,-mSuperScript);
			DrawChar(n - 1 + '0');
			MovePenBy(0,mSuperScript);
			DrawString(" = ");
			vPosn += mLineSpace;
			::FormatReal(mData->GetParam(n),realFmt_float,3,nStr);
			DrawString(nStr);
		}
	}
}
// -------------------------------------------------------------------
