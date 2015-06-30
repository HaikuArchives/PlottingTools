// ------------------------------------------------------------------
/*

	Title:
		WSRegressionView

	Description:
		Displays linear regression statistics.

	Author:
		Stephen C. Wardlaw, M.D.
		Medical R&D
		Highrock
		Lyme, CT 06371

	Edit History:
		21 September, 1999
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

#include "WSRegressionView.h"
#include <string.h>
#include <SupportKit.h>
#include "WU_AppUtils.h"

// Constants for the display window
const float	regr_frameThick 	= 3;	// Frame thickness of the window
const float	regr_numLines 		= 9;	// # Of lines in the display
const int	regr_axisMax 		= 19;	// Max length of axis name
const int	regr_maxTitle		= 36;	// Max length of data set title
const char*	regr_header			= "Regression Data For: ";
const char*	regr_totalPts 		= " Total Data Points Used In Calculations";
const char* regr_ptsOut 		= " Point(s) Outside Limits Excluded From Calculations";
//const char*	regr_line3 			= " Included In Total";
const char* regr_slope			= "Slope Of Y/X = ";
const char* regr_intercept		= "Intercept = ";
const char* regr_SDSlope		= "Std.Dev.Of Slope = ";
const char* regr_SDIntercept	= "Std.Dev.Of Intercept = ";
const char* regr_stdErr			= "Std. Error Of Y = ";
const char* regr_stdErrPct		= "Std. Error As % = ";
const char*	regr_r 				= "r-Value = ";
const char*	regr_t 				= "t-Value = ";
const char*	regr_p 				= "p-Value = ";
const char*	regr_axis 			= "X-Axis";
const char*	regr_mean			= "Mean";
const char*	regr_SD				= "SD";
const char*	regr_tooFew			= "Too Few Data Points";
	
// ------------------------------------------------------------------
// * Constructors & Destructor for WSRegressionView
// ------------------------------------------------------------------
WSRegressionView::WSRegressionView(const WSLinearRegr* theRegr) 
				 : BView(BRect(0,0,0,0),"regrDisp",B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
	mData = theRegr;
	mXPrec = mYPrec = 3;
	mXFormat = mYFormat = realFmt_float;
	mXTitle[0] = mYTitle[0] = 0;
	mFrame.Set(0,0,0,0);
	mWidth = 0;
	mHeight = 0;
}

WSRegressionView::~WSRegressionView()
{
	// Data deleted elsewhere
}
// ------------------------------------------------------------------
// * Public Methods
// ------------------------------------------------------------------
void
WSRegressionView::GetPreferredSize(float* width, float* height)
{
	*width = mWidth;
	*height = mHeight;
}
// ------------------------------------------------------------------
// Resizes for current font and data
void
WSRegressionView::ResizeToPreferred()
{
	char temp[64];

	GetFont(&mFont);
	font_height fHeight;
	mFont.GetHeight(&fHeight);
	mCharHt = fHeight.ascent + fHeight.descent;
	mCharDrop = fHeight.descent + fHeight.leading;
	mLineHt = fHeight.ascent + mCharDrop + 1;
	mWidMax = StringWidth("M");	// Width of largest character
	mEndSpc = mWidMax/2;
	mWordSpc = mEndSpc*3;
	
	// Find longest line - it may be one of several...
	// The length of the "Points Out" line
	mWidth = (StringWidth(regr_ptsOut))/2 + mEndSpc;
	
	// The length of the line giving the mean & SD
	float xLen = StringWidth(mXTitle);
	float yLen = StringWidth(mYTitle);
	float tWidth =  xLen > yLen ? xLen : yLen;
	
	::FormatReal(mData->MeanX(),mXFormat,mXPrec,temp);
	xLen = StringWidth(temp);
	::FormatReal(mData->MeanY(),mYFormat,mYPrec,temp);
	yLen = StringWidth(temp);
	mMeanLen = xLen > yLen ? xLen : yLen;	
	// The longest Mean result must be as long as the title..
	xLen = StringWidth(regr_mean);
	mMeanLen = mMeanLen > xLen ? mMeanLen : xLen;
	mMeanLen += mWordSpc;
	
	::FormatReal(mData->StdDevX(),mXFormat,mXPrec + 2,temp);
	xLen = StringWidth(temp);
	::FormatReal(mData->StdDevY(),mYFormat,mYPrec + 2,temp);
	yLen = StringWidth(temp);
	mSDLen = xLen > yLen ? xLen : yLen;		
	// The longest SD result must be at least as long as the title
	xLen = StringWidth(regr_SD);
	mSDLen = mSDLen > xLen ? mSDLen : xLen;
	mSDLen += mEndSpc;
	
	tWidth += mSDLen + mMeanLen;
	// Choose the longer of the lines
	mWidth = mWidth > tWidth ? mWidth : tWidth;
	
	// The length of the line giving the SD Slope
	if(mData->RegressionValid()) {
		::FormatReal(mData->RegressionSDSlope(),realFmt_general,4,temp);
		xLen = StringWidth(temp) + StringWidth(regr_SDSlope) + mWidMax;
		::FormatReal(mData->RegressionSDIntercept(),mYFormat,mYPrec + 2,temp);
		yLen = StringWidth(temp) + StringWidth(regr_SDIntercept) + mWidMax;
		tWidth = xLen > yLen ? xLen : yLen;
		// Choose the longer of the lines
		mWidth = mWidth > tWidth ? mWidth : tWidth;
	}
	mWidth = (mWidth + regr_frameThick)*2;
	mHeight = mLineHt*regr_numLines + regr_frameThick*2;
	ResizeTo(mWidth,mHeight);
}		
// ------------------------------------------------------------------
// * Protected Methods
// ------------------------------------------------------------------
void
WSRegressionView::Draw(BRect dFrame)
{
	BPoint		pPosn;
	char		temp[64];
	char		dStr[16];
	
	if(mData == NULL) {
		return;	// EXIT HERE if no data 
	}
	SetHighColor(0,0,0);	// Make sure of color - black...
	SetPenSize(regr_frameThick);
	mFrame = Bounds();
	StrokeRect(mFrame);
	float half = mWidth/2;
	pPosn.x = mFrame.left + regr_frameThick;			// Adjust pen position for frame
	pPosn.y = mFrame.top + regr_frameThick;
	float line1 = pPosn.y + mLineHt*3; // Posn of horiz sep lines
	float line2 = pPosn.y + mLineHt*5;
	float line3 = pPosn.y + mLineHt*7;
	float line4 = pPosn.y + mLineHt*9;
	SetPenSize(1);
	// Draw horizontal separation lines
	MovePenTo(mFrame.left,line1);                  
	StrokeLine(BPoint(mFrame.right - 1,line1));
	MovePenTo(mFrame.left,line2);
	StrokeLine(BPoint(mFrame.right - 1,line2));
	MovePenTo(mFrame.left,line3);
	StrokeLine(BPoint(mFrame.right - 1,line3));
	MovePenTo(half,line1);
	StrokeLine(BPoint(half,line2));
	
	// Draw the title line
	::strcpy(temp,regr_header);	
	::strcat(temp,mSetTitle);
	float tWidth = StringWidth(temp);
	pPosn.y += (mCharHt - mCharDrop);
	MovePenTo(half-(tWidth/2),pPosn.y);
	DrawString(temp);
	
	// Show # of points
	pPosn.y += mLineHt;	// Move to next line
	pPosn.x += mEndSpc;	// Adjust for frame clearance
	half += 2;
	::sprintf(temp,"%d",mData->PointsIn());
	::strcat(temp,regr_totalPts);
	MovePenTo(pPosn.x,pPosn.y);
	DrawString(temp);
	
	// Points out message
	pPosn.y += mLineHt;	// Move to next line
	if(mData->PointsOut() > 0) {
		::sprintf(temp,"%d",mData->PointsOut());
		::strcat(temp,regr_ptsOut);
		MovePenTo(pPosn.x,line1 - mCharDrop);
		DrawString(temp);
	}

	// Draw X-Axis data
	MovePenTo(pPosn.x,line1 + mLineHt);
	if(!mData->SDValid()) {
		DrawString(regr_tooFew);
		return;	// EXIT HERE if too few points
	}	

	DrawString(regr_axis);
	MovePenTo(pPosn.x,line2 - mCharDrop);
	DrawString(mXTitle);			// Draw X-Axis label 
	
	::FormatReal(mData->StdDevX(),mXFormat,mXPrec + 2,temp);
	float sdPosn = half - mSDLen;
	MovePenTo(sdPosn,line1 + mLineHt);
	DrawString(regr_SD);
	MovePenTo(sdPosn,line2 - mCharDrop);
	DrawString(temp);					// Draw SD of X-Axis
	
	::FormatReal(mData->MeanX(),mXFormat,mXPrec+1,temp);
	float meanPosn = sdPosn - mMeanLen;
	MovePenTo(meanPosn,line1 + mLineHt);
	DrawString(regr_mean);
	MovePenTo(meanPosn,line2 - mCharDrop);
	DrawString(temp);					// Draw mean of X-Axis result
	
	// Draw Y-Axis Data
	::strcpy(temp,regr_axis);
	temp[0] = 'Y';
	MovePenTo(half + mWidMax/2,line1 + mLineHt);
	DrawString(temp);
	MovePenTo(half + mWidMax/2,line2 - mCharDrop);
	DrawString(mYTitle);				// Draw Y-Axis label *)
	
	::FormatReal(mData->StdDevY(),mYFormat,mYPrec + 2,temp);
	sdPosn = mFrame.right - (regr_frameThick + mSDLen);
	MovePenTo(sdPosn,line1 + mLineHt);
	DrawString(regr_SD);
	MovePenTo(sdPosn,line2 - mCharDrop);
	DrawString(temp);					// Draw SD of Y-Axis
	
	::FormatReal(mData->MeanY(),mYFormat,mYPrec+1,temp);
	meanPosn = sdPosn - mMeanLen;
	MovePenTo(meanPosn,line1 + mLineHt);
	DrawString(regr_mean);
	MovePenTo(meanPosn,line2 - mCharDrop);
	DrawString(temp);					// Draw mean of Y-Axis result
	
	// Draw slope and intercept data
	MovePenTo(pPosn.x,line2 + mLineHt);
	DrawString(regr_slope);
	if(!mData->RegressionValid()) {
		DrawString(regr_tooFew);	// EXIT HERE if too few points
		return;
	}
	::FormatReal(mData->RegressionSlope(),realFmt_general,4,temp);
	DrawString(temp);
	MovePenTo(half + mEndSpc,line2 + mLineHt);
	DrawString(regr_intercept);
	::FormatReal(mData->RegressionIntercept(),mYFormat,mYPrec + 1, temp);
	DrawString(temp);
	MovePenTo(pPosn.x,line3 - mCharDrop);
	DrawString(regr_SDSlope);
	::FormatReal(mData->RegressionSDSlope(),realFmt_general,4,temp);
	DrawString(temp);
	MovePenTo(half + mEndSpc,line3 - mCharDrop);
	DrawString(regr_SDIntercept);
	::FormatReal(mData->RegressionSDIntercept(),mYFormat,mYPrec + 2,temp);
	DrawString(temp);
	
	// Show the Std error as absolute and as %
	MovePenTo(pPosn.x,line3 + mLineHt);
	DrawString(regr_stdErr);
	::FormatReal(mData->RegressionStdError(),mYFormat,mYPrec + 2,temp);
	DrawString(temp);
	MovePenTo(half + mEndSpc,line3 + mLineHt);
	DrawString(regr_stdErrPct);
	::FormatReal(mData->RegressionStdError()*100/mData->MeanY(),realFmt_fixed,2,temp);
	DrawString(temp);

	// Show the regression values if enough data
	if(!mData->ProbabilityValid()) {
		MovePenTo(pPosn.x,line4 - mCharDrop);
		DrawString(regr_r);
		DrawString(regr_tooFew);
		return;	// EXIT HERE if too few points
	}
	::strcpy(temp,regr_r);
	::FormatReal(mData->CorrelRValue(),realFmt_float,4,dStr);
	::strcat(temp,dStr);
	MovePenTo(pPosn.x,line4 - mCharDrop);
	DrawString(temp);
	float endR = pPosn.x + StringWidth(temp);
	
	::strcpy(temp,regr_p);
 	if(mData->PValue() < 1.0e-6) {
		::strcpy(dStr,"< 1.0E-6");
	} else {
		::FormatReal(mData->PValue(),realFmt_float,3,dStr);
	}
	::strcat(temp,dStr);
	float begP = mFrame.right - (StringWidth(temp) + mEndSpc + regr_frameThick);
	MovePenTo(begP,line4 - mCharDrop);
	DrawString(temp);
	
	::strcpy(temp,regr_t);
	::FormatReal(mData->CorrelTValue(),realFmt_fixed,1,dStr);
	::strcat(temp,dStr);
	MovePenTo(endR + (begP - endR - StringWidth(temp))/2,line4 - mCharDrop);
	DrawString(temp);
}
// ------------------------------------------------------------------
	
