/* ------------------------------------------------------------------ 

	Title: WGOdometer

	Description:  A class which acts as an "odometer", continuously
		giving the X/Y graph coordinates

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		09 January 98
			Adapted for BeOS
		04 Apr 94
			Created for CW DR2
		
------------------------------------------------------------------ */ 

#include "WGOdometer.h"

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGOdometer::WGOdometer(WGStdGraph* theGraph,
					  BRect frame, 
					  bool drawStacked,		
					  char* label,
					  const char* xLabel,
					  const char* yLabel,
					  uint32 resizeMode,
					  uint32 flags):BView(frame,label,resizeMode,flags)
{
	mGraph = theGraph;
	mGraph->AttachOdometer(this);
	mStacked = drawStacked;
	
	mXLabel = new BStringView(frame,"OdoXLabel",xLabel);
	mXLabel->SetAlignment(B_ALIGN_RIGHT);
	AddChild(mXLabel);
	mXValue = new BStringView(frame,"OdoXValue","");
	mXValue->SetAlignment(B_ALIGN_LEFT);
	AddChild(mXValue);
	mYLabel = new BStringView(frame,"OdoYLabel",yLabel);
	mYLabel->SetAlignment(B_ALIGN_RIGHT);
	AddChild(mYLabel);
	mYValue = new BStringView(frame,"OdoYValue","");
	mYValue->SetAlignment(B_ALIGN_LEFT);
	AddChild(mYValue);
}

WGOdometer::~WGOdometer()
{
}	
// -------------------------------------------------------------------
// *  Public Methods           
// -------------------------------------------------------------------
void
WGOdometer::ResizeToPreferred()
{
	// Make the label parts just long enough to hold the caption
	float labLen = StringWidth(mXLabel->Text());
	if(StringWidth(mYLabel->Text()) > labLen) {
		labLen = StringWidth(mYLabel->Text());
	}
	font_height fHeight;
	GetFontHeight(&fHeight);
	labLen += 5;	// Add a small margin
	mXLabel->ResizeTo(labLen,fHeight.ascent + fHeight.descent);
	mYLabel->ResizeTo(labLen,fHeight.ascent + fHeight.descent);
	
	// Make the value view long enough to hold the longest result
	char numStr[32];
	if(mGraph != NULL) {
		WGGraphAxis xAxis(*mGraph->mXAxis);
		mXDPs = xAxis.NumberPrecision() + 1;
		if(xAxis.NumberFormat() == realFmt_float) {
			mXFmt = realFmt_sigFig;
		} else {
			mXFmt = xAxis.NumberFormat();
		}
		::FormatReal(xAxis.Max(),mXFmt,mXDPs,numStr);
		labLen = StringWidth(numStr);
		::FormatReal(xAxis.Min(),mXFmt,mXDPs,numStr);
		if(StringWidth(numStr) > labLen) {
			labLen = StringWidth(numStr);
		}
		WGGraphAxis yAxis(*mGraph->mYAxis);
		mYDPs = yAxis.NumberPrecision() + 1;
		if(yAxis.NumberFormat() == realFmt_float) {
			mYFmt = realFmt_sigFig;
		} else {
			mYFmt = yAxis.NumberFormat();
		}
		::FormatReal(yAxis.Max(),mYFmt,mYDPs,numStr);
		if(StringWidth(numStr) > labLen) {
			labLen = StringWidth(numStr);
		}
		::FormatReal(yAxis.Min(),mYFmt,mYDPs,numStr);
		if(StringWidth(numStr) > labLen) {
			labLen = StringWidth(numStr);
		}
	} else {
		labLen = StringWidth("0.0");
	}
	mXValue->ResizeTo(labLen,fHeight.ascent + fHeight.descent);
	mYValue->ResizeTo(labLen,fHeight.ascent + fHeight.descent);
	
	// Size the main view and arrange the children
	BRect lFrame = mXLabel->Bounds();
	BRect vFrame = mXValue->Bounds();
	float height;
	float width;
	width = lFrame.Width() + vFrame.Width();
	height = lFrame.Height();
	if(mStacked) {
		ResizeTo(width,height*2);
		mXLabel->MoveTo(0,0);
		mXValue->MoveTo(lFrame.Width(),0);
		mYLabel->MoveTo(0,height);
		mYValue->MoveTo(lFrame.Width(),height);
	} else {
		ResizeTo(width*2,height);
		mXLabel->MoveTo(0,0);
		mXValue->MoveTo(lFrame.Width(),0);
		mYLabel->MoveTo(width,0);
		mYValue->MoveTo(width + lFrame.Width(),0);
	}
}	
// -------------------------------------------------------------------
void
WGOdometer::SetHighColor(rgb_color color)
{
	BView::SetHighColor(color);
	mXLabel->SetHighColor(color);
	mYLabel->SetHighColor(color);
	mXValue->SetHighColor(color);
	mYValue->SetHighColor(color);
}		
// -------------------------------------------------------------------
void
WGOdometer::SetLowColor(rgb_color color)
{
	BView::SetLowColor(color);
	mXLabel->SetLowColor(color);
	mYLabel->SetLowColor(color);
	mXValue->SetLowColor(color);
	mYValue->SetLowColor(color);
}	
// -------------------------------------------------------------------
void
WGOdometer::SetViewColor(rgb_color viewColor)
{
	BView::SetViewColor(viewColor);
	mXLabel->SetViewColor(viewColor);
	mYLabel->SetViewColor(viewColor);
	mXValue->SetViewColor(viewColor);
	mYValue->SetViewColor(viewColor);
}
// -------------------------------------------------------------------
void
WGOdometer::Update(BPoint pt)	
{
	char 	xStr[32] = {0};
	char	yStr[32] = {0};
	
	if(mGraph != NULL) {
		float xVal, yVal;
		if(mGraph->PosnToValue(pt,xVal,yVal)) {
			::FormatReal(xVal,mXFmt,mXDPs,xStr);
			::FormatReal(yVal,mYFmt,mYDPs,yStr);
		}
	}
	mXValue->SetText(xStr);
	mYValue->SetText(yStr);	
}	
// -------------------------------------------------------------------
// *  Protected Methods         
// -------------------------------------------------------------------
void
WGOdometer::Draw(BRect rect)
{
}		
// -------------------------------------------------------------------
