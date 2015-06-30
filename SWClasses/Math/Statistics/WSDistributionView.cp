// ------------------------------------------------------------------
/*
WSDistributionView
	Title:
		WSDistributionView

	Description:
		Displays linear distession statistics.

	Author:
		Stephen C. Wardlaw, M.D.
		Medical R&D
		Highrock
		Lyme, CT 06371

	Edit History:
		05 June, 2000
			Adapted to BeOS from Mac C++
		22 May, 1995
			Adapted from M2 to MW C++
		19 Mar 92
			Compiled under MW M2 Vers 4.0
		28 Feb 91
			Adapted to Macintosh model
		23 Dec 88
			Last IBM version 4.0
		06 Aug 84
			First IBM version, adated from Apple-II
			version started in 1982.

*/
// ------------------------------------------------------------------

#include "WSDistributionView.h"
#include <string.h>
#include <stdio.h>
#include <SupportKit.h>
#include "URealFmt_Utils.h"

// Constants for the display window
const float	dist_frameThick 	= 3;	// Frame thickness of the window
const int16	dist_numLines 		= 7;	// # Of lines in the display
const char*	dist_header			= "Distribution Data For ";
const char*	dist_test			= " Mean: XXXXX | S.D: XXXXX | +/- 2SD: XXXXXX - XXXXXX";
const char*	dist_pctile			= "PERCENTILES";
const char*	dist_defTitle		= "Distribution Plot #";

const char*	dist_tooFew			= "Too Few Data Points";
	
// ------------------------------------------------------------------
// * Constructors & Destructor for WSDistributionView
// ------------------------------------------------------------------
WSDistributionView::WSDistributionView(const WSDistribution* thedist) 
				 : BView(BRect(0,0,0,0),"distDisp",B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
	mData = thedist;
	mPrec = 3;
	mFormat = realFmt_float;
	mTitle[0] = 0;
	mFrame.Set(0,0,0,0);
	mWidth = 0;
	mHeight = 0;
}

WSDistributionView::~WSDistributionView()
{
	// Data deleted elsewhere
}
// ------------------------------------------------------------------
// * Public Methods
// ------------------------------------------------------------------
void
WSDistributionView::GetPreferredSize(float* width, float* height)
{
	*width = mWidth;
	*height = mHeight;
}
// ------------------------------------------------------------------
// Resizes for current font and data
void
WSDistributionView::ResizeToPreferred()
{
	GetFont(&mFont);
	font_height fHeight;
	mFont.GetHeight(&fHeight);
	mCharHt = fHeight.ascent + fHeight.descent;
	mCharDrop = fHeight.descent + fHeight.leading;
	mLineHt = fHeight.ascent + mCharDrop + 1;
	mWidMax = StringWidth("M");	// Width of largest character
	mEndSpc = mWidMax/2;
	mWordSpc = mEndSpc*3;
	
	mWidth = StringWidth(dist_test) + 4 + dist_frameThick*2;
	mHeight = mLineHt*dist_numLines + dist_frameThick*2;
	
	ResizeTo(mWidth,mHeight);
	MoveTo(0,0);
}		
// ------------------------------------------------------------------
// * Protected Methods
// ------------------------------------------------------------------
void
WSDistributionView::Draw(BRect dFrame)
{
	char		temp[64];
	char		dStr1[16];
	char		dStr2[16];
	
	if(mData == NULL) {
		return;	// EXIT HERE if no data 
	}
	SetHighColor(0,0,0);	// Make sure of color - black...
	SetPenSize(dist_frameThick);
	BRect frame = Bounds();
	StrokeRect(frame);
	SetPenSize(1);
	if(!mData->ValidDistribution()) {
		MovePenTo(5,mLineHt);
		DrawString(dist_tooFew);
		return;
	}
	
	// Set up the lines
	float		row[dist_numLines];	
	frame.InsetBy(dist_frameThick,dist_frameThick);	// Adjust for border
	float mid = frame.Width()/2;
	float halfChar = dist_frameThick + dist_frameThick/2;  
	for(int n = 0; n < dist_numLines; n++) {
		row[n] = dist_frameThick + mLineHt*(n + 1);
		StrokeLine(BPoint(frame.left,row[n]),BPoint(frame.right,row[n]));
	}
	StrokeLine(BPoint(mid,row[0]),BPoint(mid,row[1]));
	StrokeLine(BPoint(mid,row[3]),BPoint(mid,row[6]));
	GetFont(&mFont);
	font_height fHeight;
	mFont.GetHeight(&fHeight);
	for(int n = 0; n < dist_numLines; n++) {	// Correct for font descent
		row[n] -= fHeight.descent;
	}
	
	// Line #1	
	::strcpy(temp,dist_header);
	::strcat(temp,mData->Title());
	MovePenTo(mid - StringWidth(temp)/2,row[0]);
	DrawString(temp);
	MovePenTo(mid - StringWidth(dist_pctile)/2,row[3]);
	DrawString(dist_pctile);
	
	// Line #2
	MovePenTo(halfChar,row[1]);
	::sprintf(temp,"Samples: %ld",mData->Points());
	DrawString(temp);
	MovePenTo(mid + halfChar,row[1]);
	::FormatReal(mData->Min(),mFormat,mPrec,dStr1);
	::FormatReal(mData->Max(),mFormat,mPrec,dStr2);
	::sprintf(temp,"Range: %s/%s",dStr1,dStr2);
	DrawString(temp);
	
	// Line #3
	MovePenTo(halfChar,row[2]);
	FormatReal(mData->MeanValue(),mFormat,mPrec + 1,dStr1);
	::sprintf(temp,"Mean: %s  ",dStr1);
	DrawString(temp);
	BPoint posn = PenLocation();
	MovePenTo(posn.x,row[1] + fHeight.descent);
	StrokeLine(BPoint(posn.x,row[2] + fHeight.descent));
	MovePenTo(posn.x + halfChar,row[2]);
	FormatReal(mData->StdDeviation(),mFormat,mPrec + 1,dStr1);
	::sprintf(temp,"SD: %s",dStr1);
	DrawString(temp);
	FormatReal(mData->MeanValue() - mData->StdDeviation()*2.0,mFormat,mPrec,dStr1);
	FormatReal(mData->MeanValue() + mData->StdDeviation()*2.0,mFormat,mPrec,dStr2);
	::sprintf(temp,"+/- 2SD: %s - %s",dStr1,dStr2);
	MovePenTo(frame.right - StringWidth(temp) - halfChar*2,row[2]);
	posn = PenLocation();
	MovePenTo(posn.x,row[1] + fHeight.descent);
	StrokeLine(BPoint(posn.x,row[2] + fHeight.descent));
	MovePenTo(posn.x + halfChar,row[2]);
	DrawString(temp);
	
	// Line #5
	MovePenTo(halfChar,row[4]);
	FormatReal(mData->ValueAtPercentile(1.0),mFormat,mPrec,dStr1);
	FormatReal(mData->ValueAtPercentile(99.0),mFormat,mPrec,dStr2);
	::sprintf(temp,"1/99: %s / %s",dStr1,dStr2);
	DrawString(temp);
	MovePenTo(mid + halfChar,row[4]);
	FormatReal(mData->ValueAtPercentile(3.0),mFormat,mPrec,dStr1);
	FormatReal(mData->ValueAtPercentile(97.0),mFormat,mPrec,dStr2);
	::sprintf(temp,"3/97: %s / %s",dStr1,dStr2);
	DrawString(temp);
	
	// Row #6
	MovePenTo(halfChar,row[5]);
	FormatReal(mData->ValueAtPercentile(5.0),mFormat,mPrec,dStr1);
	FormatReal(mData->ValueAtPercentile(95.0),mFormat,mPrec,dStr2);
	::sprintf(temp,"5/95: %s / %s",dStr1,dStr2);
	DrawString(temp);
	MovePenTo(mid + halfChar,row[5]);
	FormatReal(mData->ValueAtPercentile(10.0),mFormat,mPrec,dStr1);
	FormatReal(mData->ValueAtPercentile(90.0),mFormat,mPrec,dStr2);
	::sprintf(temp,"10/90: %s / %s",dStr1,dStr2);
	DrawString(temp);

	// Row #7
	MovePenTo(halfChar,row[6]);
	FormatReal(mData->MedianValue(),mFormat,mPrec,dStr1);
	::sprintf(temp,"Median Value: %s",dStr1);
	DrawString(temp);
	MovePenTo(mid + halfChar,row[6]);
	FormatReal(mData->ModalValue(),mFormat,mPrec,dStr1);
	::sprintf(temp,"Mode Value(Gp): %s",dStr1);
	DrawString(temp);

			
}
// ------------------------------------------------------------------
	
