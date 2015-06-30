/* ------------------------------------------------------------------ 

	Title: WGAbscissa

	Description:  A class to be used in constructing graphs.  One or
		more of these can be used as an X-Axis referent.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		09 Dec 97
			Adapted for BeOS
		24 Jan 94
			Split from the old Module Graph in MW M-2 and
			re-written for MetroWerks C++
		16 May 90	
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87 
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#include "WGAbscissa.h"
#include "UFloating_Util.h"
#include <SupportKit.h>

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGAbscissa::WGAbscissa(BRect frame, char* label, uint32 resizeMode, uint32 flags)
							  :BView(frame,label,resizeMode,flags)
{
	mTicsLabeled = true;
	mTicsBelow = true;
	mTicWidthX = border_wd;
}
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
//	This draws the graph axis.
void
WGAbscissa::Draw(BRect rect) 
{
	float		divs, ticPosn,origin,end;
	float	root,intervalSpan,highInt,loInt,num,dNum,divValue,rdgErr;
	SetPenSize(mTicWidthX);

	// Axis origin relative to this frame
	BRect		frame = Frame();
	frame.OffsetTo(B_ORIGIN);
	origin = frame.left + mLeftMargin;
	end = frame.right - mRightMargin;
		
	//Draw X-Axis of graph
	MovePenTo(origin,mTicBase + mTicWidthX/2);
	BPoint thePt(end, + mTicWidthX/2);
	StrokeLine(thePt);
	mLimitRight = 9999;                    // Reset high label posn limit
	mLimitLeft = -9999;
	
	// Label the maximum graph position
   MovePenTo(end,mTicBase);
   thePt.Set(end,mTicBase + mTicLen);
	StrokeLine(thePt);
	// If we want to make sure the highest value is always labelled...
	//LabelTic(mEndX,mAxis.max);  // Label mMax posn
	//mLimitRight = mEndX - (MakeLabel(mAxis.max,temp) + mLabelMargin);
	SetPenSize(mTicWidthX/2);
	mLimitRight = 9999;
	mLimitLeft = -9999;                   // Reset minimum limit
	
	// Fill in remainder of mAxis
	if(mScale != scale_linear) {     // Find first marking interval
		root = ::LogOfBase(mMin,mLogBase);
		if(::Power(mInterval,root) > mMin) {
			root--;
		}
		loInt = ::Power(mLogBase,root);
		root++;
		highInt = ::Power(mLogBase,root);
		rdgErr = (highInt - loInt)/10000.0;        // Rounding error
		loInt = loInt + rdgErr;        // So 1st tic will always print
	} else {             // If a linear or other function mScale
		highInt = mMin + mInterval;
		loInt = mMin;
	}
		
	while(1) {                 // Mark major & minor tics
		intervalSpan = highInt - loInt;
		divValue = intervalSpan/mDivs;
		num = loInt;
		dNum = num;
		divs = mDivs;
		SetPenSize(mTicWidthX/2);
		while(divs > 1) {
			dNum = dNum + divValue;
			if((dNum >= mMin) && (dNum <= mMax)) {
				ticPosn = (XformTo(dNum) - mXformedMin)*mSpanFactX + origin;
   			MovePenTo(ticPosn,mTicBase);
   			thePt.Set(ticPosn,mTicBase + mTicLen/2);
	   		StrokeLine(thePt);
			}
			divs--;
		}	// while divx
		// Label major division
		if(num <= mMax*rounding_err) {
			ticPosn = (XformTo(num) - mXformedMin)*mSpanFactX + origin;
			SetPenSize(mTicWidthX);
   		MovePenTo(ticPosn,mTicBase);
   		thePt.Set(ticPosn,mTicBase + mTicLen);
	   	StrokeLine(thePt);
			LabelTic(ticPosn,num);
		} else {
			return; //*** EXIT HERE if through marking
		}	
	                                          // Generate new interval *)
		loInt = highInt;  
		if(mScale != scale_linear) {
			root++;
	     	highInt = ::Power(mLogBase,root);
		} else {
			highInt = num + mInterval;
		}
	}
}
// -------------------------------------------------------------------
// Gets the abscissa margin requirements.  The margins are the distances
// from the actual axis to the end of the drawn areas
void
WGAbscissa::GetXMargins(float &leftMargin, float &rightMargin, float &height)
{
	char	temp[16];
	font_height fHeight;
	
	mLabelMargin = StringWidth("M");
	GetFontHeight(&fHeight);
	if(mTicsLabeled) {
		height = fHeight.ascent + fHeight.descent + tic_space;
		mLeftMargin = MakeLabel(mMin,temp);
		mRightMargin = MakeLabel(mMax,temp);
	} else {
		height = 0;
		mLeftMargin = 0;
		mRightMargin = 0;
	}
	height += mTicWidthX + tic_len;
	leftMargin = mLeftMargin;
	rightMargin = mRightMargin + 2;
}
// -------------------------------------------------------------------
//	Moves the abscissa pane so that the origin is at 'pt'
void
WGAbscissa::MoveXOriginTo(BPoint pt) 
{
	BRect	frame;
	BAutolock lock(Window());
	frame = Bounds();
	pt.x -= (mLeftMargin + mTicWidthX/4 + 1);
	if(!mTicsBelow) {
		pt.y -= mTicWidthX + frame.bottom - frame.top;
	}
	MoveTo(pt.x,pt.y);
	frame = Frame();
	mOriginX = frame.left + mLeftMargin;
	mEndX = frame.right - mRightMargin;
}
// -------------------------------------------------------------------
// Calculates the working rectangle used by the mAxis 
void
WGAbscissa::SizeAxisTo(float length) 
{
	float		height, span,dummy;
	BRect		frame;
	font_height fHeight;
	
	BAutolock lock(Window());
	span = length;
	// ++ Check mMin size of pane??
	GetXMargins(dummy,dummy,height);
	length += mLeftMargin + mRightMargin;

	// Calculate the span factor
	mXformedMin = XformTo(mMin);
	mSpanFactX = span/(XformTo(mMax) - mXformedMin);

  	// Resize the frame
	ResizeTo(length, height);
	frame = Frame();  				// Get size of new pane
	mOriginX = frame.left + mLeftMargin;
	mEndX = frame.right - mRightMargin;
	GetFontHeight(&fHeight);
	
	// Create view-relative variables
	frame.OffsetTo(B_ORIGIN);
	if(mTicsBelow) {
		mTicBase = frame.top;
		mTicLen = tic_len;
		mLabelPosn = mTicBase + mTicLen + tic_space + fHeight.ascent;
	} else {
		mTicBase = frame.bottom - mTicWidthX;
		mTicLen = -tic_len;
		mLabelPosn = mTicBase + mTicLen - tic_space - fHeight.descent;
	}
}
// -------------------------------------------------------------------
// Returns true if "val" is within the current X-Axis limits and sets
// 'posn' to the correct port value in reference to the mAxis
bool
WGAbscissa::ValToXPosn(float val, float &posn)
{
	float value;

	if((val >= mMin) && (val <= mMax)) {
		value = XformTo(val) - mXformedMin;
		posn = (value * mSpanFactX) + mOriginX;
		return true;
	} else {
		posn = 0;
		return false;
	}
}
// -------------------------------------------------------------------
// Finds the data value from the graphic coordinate.  Returns true if
// the position is within the mAxis scope.
bool
WGAbscissa::XPosnToVal(float posn, float &val)
{
	bool 		inGraf;
	float 	R;

	inGraf = false;
	if(posn > mEndX) {
		val = mMax;
	} else if(posn < mOriginX) {
		val = mMin;
	} else {
		R = ((posn - mOriginX)/mSpanFactX) + mXformedMin;
		val = XformFrom(R);
		inGraf = true;
	}
	return inGraf;
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
// Places a tic label where appropriate.
void
WGAbscissa::LabelTic(float posn, float num) 
{
	char temp[15];
	float len;

	if(mTicsLabeled) {
		len = MakeLabel(num,temp);
		if(((posn + len) < mLimitRight) && ((posn - len) > mLimitLeft)) {
			MovePenTo(posn-len,mLabelPosn);
			DrawString(temp);
			mLimitLeft = posn + len + mLabelMargin;
		}
	}
}
// -------------------------------------------------------------------
// Makes a label and returns the half-length
float
WGAbscissa::MakeLabel(float num, char* temp) 
{
	
	::FormatReal(num,mNumFmt,mNumDPs,temp);
	return StringWidth(temp) / 2;
}
