/* ------------------------------------------------------------------ 

	Title: WGOrdinate

	Description:  A class to be used in constructing graphs.  One or
		more of these can be used as a Y-Axis referent.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		09 Dec 97
			Adapted for BeOS
		04 Feb 94
			Split from the old Module Graph in MW M-2 and
			re-written for MetroWerks C++
		16 May 90
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87
			Last IBM Version 6.0
		?? Aug 80
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#include "WGOrdinate.h"
#include "UFloating_Util.h"
#include <SupportKit.h>
// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGOrdinate::WGOrdinate(BRect frame, char* label, uint32 resizeMode, uint32 flags)
							  :BView(frame,label,resizeMode,flags)
{
	mTicsLabeled = true;
	mTicsLeft = true;
	mTicWidthY = border_wd;
}
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
//	This draws the graph axis.
void
WGOrdinate::Draw(BRect rect) 
{
	float		divs, ticPosn,origin,end;
	float	root,intervalSpan,highInt,loInt,num,dNum,divValue,rdgErr;

	// Axis origin relative to this frame
	BRect		frame = Frame();
	frame.OffsetTo(B_ORIGIN);
	origin = frame.bottom - mBottomMargin;
	end = frame.top + mTopMargin;

	//Draw Y-Axis of graph
	SetPenSize(mTicWidthY);
	MovePenTo(mTicBase - mTicWidthY/2,origin);
	BPoint	thePt(mTicBase - mTicWidthY/2,end);
	StrokeLine(thePt);
	mLimitTop = -9999;                    // Reset high label posn limit
	mLimitBottom = 9999;

	// Label the maximum graph position
	thePt.Set(mTicBase + mTicLen,end);
	StrokeLine(thePt);
	// If we want to make sure the highest value is always labelled...
	//LabelTic(end,mAxis.max);  // Label max posn
	//mLimitTop = end - mLabelMargin;
	SetPenSize(mTicWidthY/2);
	mLimitBottom = 9999;                   // Reset minimum limit
	mLimitTop = -9999;

	// Fill in remainder of mAxis
	if(mScale != scale_linear) {     // Find first marking interval
		root = ::LogOfBase(mMin,mLogBase);
		if(::Power(mLogBase,root) > mMin) {
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
		SetPenSize(mTicWidthY/2);
		while(divs > 1) {
			dNum = dNum + divValue;
			if((dNum >= mMin) && (dNum <= mMax)) {
				ValToYPosn(dNum,ticPosn);
				ticPosn = origin - ((XformTo(dNum) - mXformedMin)*mSpanFactY);
   			MovePenTo(mTicBase,ticPosn);
   			thePt.Set(mTicBase + mTicLen/2,ticPosn);
	   		StrokeLine(thePt);
			}
			divs--;
		}	// while divx
		// Label major division
		if(num <= mMax*rounding_err) {
			SetPenSize(mTicWidthY);
			ticPosn = origin - ((XformTo(num) - mXformedMin)*mSpanFactY);
   		MovePenTo(mTicBase,ticPosn);
   		thePt.Set(mTicBase + mTicLen,ticPosn);
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
// Gets the minimum margin sizes of the pane
void
WGOrdinate::GetYMargins(float &topMargin, float &bottomMargin, float &width) 
{
	char	temp[16];
	float	wid1, wid2;
	font_height fHeight;

	BAutolock lock(Window());
	GetFontHeight(&fHeight);
	if(mTicsLabeled) {
		mTopMargin = mBottomMargin = mFontAdj = (fHeight.ascent + fHeight.descent)/2;
		wid1 = MakeLabel(mMin,temp);
		wid2 = MakeLabel(mMax,temp);
		width = wid1 > wid2 ? wid1 : wid2;
	} else {
		mTopMargin = mBottomMargin = width = 0;
	}
	width += mTicWidthY + tic_len + tic_space;
	topMargin = mTopMargin;
	bottomMargin = mBottomMargin;
}
// -------------------------------------------------------------------
void
WGOrdinate::MoveYOriginTo(BPoint pt) 
{
	BRect	frame;

	BAutolock lock(Window());
	pt.y -= (mOriginY - mEndY) + mTopMargin;
	pt.y += mTicWidthY/4 + 1;
	if(mTicsLeft) {
		frame = Bounds();
		pt.x -= frame.right - frame.left;
	}
	MoveTo(pt.x,pt.y);
	frame = Frame();
	mOriginY = frame.bottom - mBottomMargin;
	mEndY = frame.top + mTopMargin;
}		
// -------------------------------------------------------------------
// Calculates the working rectangle used by the axis 
void
WGOrdinate::SizeAxisTo(float length) 
{
	float		width, span, dummy;
	BRect		frame;
	font_height fHeight;

	BAutolock lock(Window());
	span = length;
	// ++ Check mMin size of pane??
	GetYMargins(dummy, dummy,width);
	length += mTopMargin + mBottomMargin;
		
	// Calculate the span factor
	mXformedMin = XformTo(mMin);
	mSpanFactY = span/(XformTo(mMax) - mXformedMin);
	
  	// Resize the frame
	ResizeTo(width, length);
	frame = Frame();  				// Get size of new pane
	mOriginY = frame.bottom - mBottomMargin;
	mEndY = frame.top + mTopMargin;
	
	// Create view-relative variables
	frame.OffsetTo(B_ORIGIN);
	if(mTicsLeft) {
		mTicBase = frame.right;
		mTicLen = -tic_len;
	} else {
		mTicBase = frame.left;
		mTicLen = tic_len;
	}
	mLabelPosn = mTicBase + mTicLen;
	GetFontHeight(&fHeight);
	mLabelMargin = (fHeight.ascent + fHeight.descent)/2;     // Min distance between labels
	mFontAdj = mLabelMargin*2/5;
}
// -------------------------------------------------------------------
// Returns true if "val" is within the current Y-Axis limits and sets
// 'posn' to the correct port value in reference to the axis
bool
WGOrdinate::ValToYPosn(float val, float &posn) 
{
	float value;

	if((val >= mMin) && (val <= mMax)) {
		value = XformTo(val) - mXformedMin;
		posn = mOriginY - (value * mSpanFactY);
		return true;
	} else {
		posn = 0;
		return false;
	}
}
// -------------------------------------------------------------------
// Finds the data value from the graphic coordinate.  Returns true if
// the position is within the axis scope.
bool
WGOrdinate::YPosnToVal(float posn, float &val) 
{
	bool 		inGraf;
	float	R;

	inGraf = false;
	if(posn <= mEndY) {
		val = mMax;
	} else if(posn >= mOriginY) {
		val = mMin;
	} else {
		R = ((mOriginY - posn)/mSpanFactY) + mXformedMin;
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
WGOrdinate::LabelTic(float posn, float num) 
{
	char	temp[15];
	float	len;

	if(mTicsLabeled) {
		len = MakeLabel(num,temp);
		if(((posn - mLabelMargin) > mLimitTop) && ((posn + mLabelMargin) < mLimitBottom)) {
			if(mTicsLeft) {
				len = MakeLabel(num,temp);
				MovePenTo(mLabelPosn - (tic_space + len),posn + mFontAdj);
			} else {
				MovePenTo(mLabelPosn + tic_space,posn + mFontAdj);
			}
			DrawString(temp);
			mLimitBottom = posn - mFontAdj;
		}
	}
}	
// -------------------------------------------------------------------
// Makes a label and returns the length
float
WGOrdinate::MakeLabel(float num, char* temp) 
{
	::FormatReal(num,mNumFmt,mNumDPs,temp);
	return StringWidth(temp);
}
