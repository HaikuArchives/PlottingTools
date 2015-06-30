/* ------------------------------------------------------------------ 

	Title: WGStdGraph

	Description:  An class which makes a standard 2-D graph 
	skeleton using user-specified criteria and plots
	real data.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		01 October, 1999
			Changed drag-selection method
		28 September, 1999
			Made odometer integral with this class
		14 July, 1999
			Drag scaling added
		07 Dec, 97
			Adapted for the BeOS
		14 Feb 94
			Rewritten again
		18 Jan 94
			Re-written for MetroWerks C++
		16 May 90
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87
			Last IBM Version 6.0
		?? Aug 80
			First version for Apple IIe
		
------------------------------------------------------------------ */

#include "WGStdGraph.h"
#include "WGAbscissa.h"
#include "WGOrdinate.h"
#include "WGOdometer.h"

#include "UFloating_Util.h"
#include "UGraph_Util.h"
#include <SupportKit.h>

const float out_of_bounds = -1;

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGStdGraph*	
WGStdGraph::Instantiate(BMessage* theArchive)
{
   if (validate_instantiation(theArchive, "StdGraph") ) {
		return new WGStdGraph(theArchive); 
   } else {
   	return NULL;
   }
}

WGStdGraph::WGStdGraph(BMessage* theMessage):BView(theMessage)
{
	InitGraph();
}


WGStdGraph::WGStdGraph(BRect frame, 
							  const char* title,
							  uint32 mode, 
							  uint32 flags):BView(frame,title,mode,flags)
{
	InitGraph();
}
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
// Gets the pointer to the X-Axis label
const char* const
WGStdGraph::GetXLabel()
{
	return mXLabel->Text();
}
// -------------------------------------------------------------------
// Gets the pointer to the Y-Axis label
const char* const
WGStdGraph::GetYLabel()
{
	return mYLabel->Text();
}
// -------------------------------------------------------------------
// Forces a complete update of the graph
void
WGStdGraph::ForceReDraw() 
{
	BAutolock lock(Window());
	if(lock.IsLocked()) {
		mChangedAxes = true;
		Invalidate();
		mXAxis->Invalidate();
		mYAxis->Invalidate();
	}
}
// -------------------------------------------------------------------
// Plots the value y := 'y_0f_XFunct(x)' from min to max X.
void
WGStdGraph::PlotFunct(WMFunctBase* extFunct) 
{
	/*
		We divide up the 'visual' x-axis into equal segments and then locate
		the x-value for that point. That is then sent to the function to get
		the y-value. We divide the graphical axis rather than the numeric 
		axix so that non-linear axes will make a uniform-appearing curve.
	*/
	
	float xMinPosn, xMaxPosn;
	mXAxis->GetXAxisPosition(xMinPosn,xMaxPosn);
	float xInterval = (xMaxPosn - xMinPosn)/100.0;
	mLastX = out_of_bounds;	// So won't plot first line
	for(float xPosn = 0; xPosn <= (xMaxPosn - xMinPosn); xPosn += xInterval) {
		float xVal;
		if(mXAxis->XPosnToVal(xPosn + xMinPosn, xVal)) {
			float yVal = extFunct->CalcYFromX(xVal);
			if(!extFunct->MathError()) {
				PlotTo(xVal,yVal);	// Plot if ok
			} else {
				mLastX = out_of_bounds;	// Set out-of-bounds
			}
		} else {
			mLastX = out_of_bounds;	// Set out-of-bounds
		}
	}
}	
// -------------------------------------------------------------------
// Plots a line between the two coordinates.
void
WGStdGraph::PlotLine(float x1,float y1,float x2,float y2) 
{
	float X1,Y1,X2,Y2;

	if(PosnX(x1,X1) && PosnX(x2,X2) &&
		PosnY(y1,Y1) && PosnY(y2,Y2) ) {
		MovePenTo(X1,Y1);
		BPoint thePt(X2,Y2);
		StrokeLine(thePt); 
		mLastX = X2;
		mLastY = Y2;
	}
}
// -------------------------------------------------------------------
// Plots a colored point.
bool
WGStdGraph::PlotPoint(SPlotData* theData) 
{	
	// Set color & mark
	bool valid = PlotPoint(theData->x,theData->y);
	// Reset color
	return valid;
}
// -------------------------------------------------------------------
// Plots a point.
bool
WGStdGraph::PlotPoint(float x,float y) 
{
	float X,Y;
	BRect r;
	BPoint pt1, pt2;
	
	if(!mXAxis->ValToXPosn(x,X) || !mYAxis->ValToYPosn(y,Y)) {
		mLastX = out_of_bounds;
		mLastY = out_of_bounds;
		return false;
	}

	SetHighColor(mPlotColor);
	
	switch (mPlotMark) {
	
		case plotMark_none :
			;	// No drawing, but update position
			break;
			
		case plotMark_spec :
			r.Set(X,Y,X,Y);
			r.InsetBy(-3,-3);
			FillRect(r);
			break;
			
		case plotMark_plus :
			SetPenSize(1);
			BeginLineArray(2);
			pt1.Set(X,Y - 2);
			pt2.Set(X,Y + 2);
			AddLine(pt1,pt2,mPlotColor);
			pt1.Set(X - 2,Y);
			pt2.Set(X + 2,Y);
			AddLine(pt1,pt2,mPlotColor);
			EndLineArray();
			break;
			
		case plotMark_smCirc :
			r.Set(X,Y,X,Y);
			r.InsetBy(-2,-2);
			StrokeEllipse(r);
			break;
			
		case plotMark_diam :
			pt1.Set(X-2,Y);
			pt2 = pt1;
			pt2.x += 2;
			pt2.y -=2;
			BeginLineArray(4);
			AddLine(pt1,pt2,mPlotColor);
			pt1 = pt2;
			pt2.x += 2;
			pt2.y +=2;
			AddLine(pt1,pt2,mPlotColor);
			pt1 = pt2;
			pt2.x -= 2;
			pt2.y +=2;
			AddLine(pt1,pt2,mPlotColor);
			pt1 = pt2;
			pt2.x -= 2;
			pt2.y -=2;
			AddLine(pt1,pt2,mPlotColor);
			EndLineArray();
			break;
			
		case plotMark_lgCirc :
			r.Set(X,Y,X,Y);
			r.InsetBy(-4,-4);
			StrokeEllipse(r);
			break;
			
		case plotMark_filledDot :
			r.Set(X,Y,X,Y);
			r.InsetBy(-2,-2);
			FillRect(r);
			break;
			
		case plotMark_filledSmCirc :
			r.Set(X,Y,X,Y);
			r.InsetBy(-2,-2);
			FillEllipse(r);
			break;
			
		case plotMark_filledLgCirc :
			r.Set(X,Y,X,Y);
			r.InsetBy(-4,-4);
			FillEllipse(r);
			break;
			
		case plotMark_dot :
		default :
			r.Set(X,Y,X,Y);
			r.InsetBy(-2,-2);
			StrokeRect(r);
		}	
	mLastX = X;
	mLastY = Y;
	SetHighColor(0,0,0);	// Black
	return true;
}
// -------------------------------------------------------------------
// Plots from  the last positioned point to X,Y in the graph axes.
bool
WGStdGraph::PlotTo(float x,float y) 
{
	float X,Y;

	if(!PosnX(x,X) || !PosnY(y,Y)) {
		mLastX = out_of_bounds;
		mLastY = out_of_bounds;
		return false;
	}
	if((mLastX == out_of_bounds) || (mLastY == out_of_bounds)) {
		mLastX = X;
		mLastY = Y;
		return true;
	}
	BPoint pt1(mLastX,mLastY);
	BPoint pt2(X,Y);
	StrokeLine(pt1,pt2);
	mLastX = X;
	mLastY = Y;
	
	return true;
}
// -------------------------------------------------------------------
// Finds the x & y data values from the graphic coordinate.
bool
WGStdGraph::PosnToValue(BPoint pt, float &x, float &y) 
{
	return mXAxis->XPosnToVal(pt.x,x) && mYAxis->YPosnToVal(pt.y,y);	
}
// -------------------------------------------------------------------
bool
WGStdGraph::PosnToValue(BRect theRect, float &xHi, float &xLow, float &yHi, float &yLow)
{
	bool inBounds = mXAxis->XPosnToVal(theRect.left,xLow);
	inBounds |= mXAxis->XPosnToVal(theRect.right,xHi);
	inBounds |= mYAxis->YPosnToVal(theRect.bottom,yLow);
	inBounds |= mYAxis->YPosnToVal(theRect.top,yHi);
	float temp;
	if(xHi < xLow) {
		temp = xLow;
		xLow = xHi;
		xHi = temp;
	}
	if(yHi < yLow) {
		temp = yLow;
		yLow = yHi;
		yHi = temp;
	}
	return inBounds;
}
// -------------------------------------------------------------------
// Returns true if "X" is within the current X-Axis limits.
bool
WGStdGraph::PosnX(float x, float &X) 
{
	return mXAxis->ValToXPosn(x,X);	
}
// -------------------------------------------------------------------
// Returns true if "Y" is within the current Y-Axis limits.
bool
WGStdGraph::PosnY(float y, float &Y) 
{
	return mYAxis->ValToYPosn(y,Y);	
}
// -------------------------------------------------------------------
void
WGStdGraph::Refresh()
{
	BAutolock lock(Window());
	if(lock.IsLocked()) {
		Invalidate();
		mXAxis->Invalidate();
		mYAxis->Invalidate();
	}
}
// -------------------------------------------------------------------
void
WGStdGraph::SetGraphColor(rgb_color lineColor)
{
	mXAxis->SetHighColor(lineColor);			
	mXAxis->SetLowColor(ViewColor());	
			
	mYAxis->SetHighColor(lineColor);			
	mYAxis->SetLowColor(ViewColor());	

   mXLabel->SetHighColor(lineColor);			
	mXLabel->SetLowColor(ViewColor());	

 	mYLabel->SetHighColor(lineColor);	
	mYLabel->SetLowColor(ViewColor());	
	ForceReDraw();
}
// -------------------------------------------------------------------
void
WGStdGraph::SetGraphFont(BFont* theFont)
{
	BAutolock lock(Window());
	SetFont(theFont);
	mXAxis->SetFont(theFont);			
	mYAxis->SetFont(theFont);			
   mXLabel->SetFont(theFont);	
   theFont->SetRotation(90);		
   mYLabel->SetFont(theFont);		
   ForceReDraw();
}
// -------------------------------------------------------------------
// Calculates frame positioning as required
void
WGStdGraph::SetFrame()
{
	font_height	fHeight;
	BRect		gFrame;
	BRect		xFrame;
//	BRect		yFrame;
	BPoint		origin;
	float		yMargTop, yMargBot, yWd;
	float		xMargRt, xMargLt, xHt;
	float		xSize, ySize;
	float		xLabHt;
	float		yLabWd;
	
	mChangedAxes = false;
	BAutolock lock(Window());

	// Make room for the axis labels
	GetFontHeight(&fHeight);	// NOTE! All views have the same font
//	xLabHt = (fHeight.ascent + fHeight.descent)*1.5;
	xLabHt = (fHeight.ascent + fHeight.descent);
	yLabWd = fHeight.ascent + fHeight.descent;
	
	// Now, fit the graph axis panels
	mXAxis->GetXMargins(xMargLt, xMargRt, xHt);
	mYAxis->GetYMargins(yMargTop, yMargBot, yWd);
	
	// Only count greater of y-axis width or left x-margin
	float ltMargin = yWd > xMargLt ? yWd : xMargLt;
	
	// Only count greater of x-axis height or bottom Y-margin
	float botMargin = xHt > yMargBot ? xHt : yMargBot;
	
	// Calc max size of axes and adjust if they are to be equal
	gFrame = Bounds();
	xSize = gFrame.Width() - (ltMargin + xMargRt + yLabWd);
//	ySize = gFrame.Height() - (botMargin + yMargTop + xLabHt + xLabHt/3);
//	ySize = gFrame.Height() - (botMargin + yMargTop + xLabHt);
	ySize = gFrame.Height() - (botMargin + yMargTop + xLabHt);
	if(mEqualAxes) {
		if(xSize > ySize) {
			xSize = ySize;
		} else {
			ySize = xSize;
		}
	}

	mYAxis->SizeAxisTo(ySize);
	mXAxis->SizeAxisTo(xSize);
	
	origin.x = yWd + yLabWd + yLabWd/3;;
//	origin.y = gFrame.Height() - xHt - xLabHt + xLabHt/3;
	origin.y = gFrame.Height() - xHt - xLabHt;
	mXAxis->MoveXOriginTo(origin);
	mYAxis->MoveYOriginTo(origin);
	mPlotArea.Set(origin.x,origin.y - ySize, origin.x + xSize, origin.y);
	
	// Center the axes along the margins of the plot area
	xFrame = mXAxis->Frame();
	mXLabel->ResizeTo(mPlotArea.right - mPlotArea.left,Bounds().bottom - xFrame.bottom);
	mXLabel->MoveTo(mPlotArea.left,xFrame.bottom);
	
	//yFrame = mYAxis->Frame();
	mYLabel->ResizeTo(yLabWd,mPlotArea.bottom - mPlotArea.top);
	mYLabel->MoveTo(0,mPlotArea.top);
	
	mLastX = out_of_bounds;
	mLastY = out_of_bounds;
	
}
// -------------------------------------------------------------------
// Sets the X-Axis label
void
WGStdGraph::SetXLabel(const char* label) 
{
	mXLabel->SetText(label);
	mChangedAxes = true;
}
// -------------------------------------------------------------------
// Sets the Y-Axis label
void
WGStdGraph::SetYLabel(const char* label) 
{
	mYLabel->SetText(label);
	mChangedAxes = true;
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
void
WGStdGraph::AutoScale(float &xLo, float &xHi, float &yLo, float &yHi)
{
	UGraph_Util::AutoScale(mXAxis, xLo, xHi);
	UGraph_Util::AutoScale(mYAxis, yLo, yHi);
}
// -------------------------------------------------------------------
void
WGStdGraph::Draw(BRect theRect) 
{
	 if(mChangedAxes) {	// Always draw if resized
	 	SetFrame();
	 }
}
// -------------------------------------------------------------------
void
WGStdGraph::FrameResized(float width, float height)
{
	// This could be called by the system or by the application.
	mChangedAxes = true;
} 
// -------------------------------------------------------------------
// Initialize graph characteristics
void
WGStdGraph::InitGraph()
{
	BRect frame;
	
	// Add the X-Axis label
	frame.Set(0,0,0,0);
	mXLabel = new WGAbscissaLabel(frame);
	AddChild(mXLabel);

	// Add the Y-Axis label
	mYLabel = new WGOrdinateLabel(frame);
	AddChild(mYLabel);
	
	// Add the abscissa
	mXAxis = new WGAbscissa(frame);
	AddChild(mXAxis);
	
	// Add the ordinate
	mYAxis = new WGOrdinate(frame);
	AddChild(mYAxis);
	
	mPlotMark = plotMark_dot;
	mPlotColor = HighColor();
	mEqualAxes = false;
	mLastX = out_of_bounds;
	mLastY = out_of_bounds;
	mChangedAxes = true;
	mSelectEnable = false;
	mTracking = false;
	mSelectRect.Set(0,0,0,0);
	mOdometer = NULL;
	
}
// -------------------------------------------------------------------
void
WGStdGraph::MouseDown(BPoint thePt)
{
	if(mSelectEnable) {
		mSelectRect.Set(thePt.x,thePt.y,thePt.x,thePt.y);
		mTracking = true;
		BeginRectTracking(BRect(thePt,thePt), B_TRACK_RECT_CORNER);
		SetMouseEventMask(B_POINTER_EVENTS);
	}
}
// -------------------------------------------------------------------
void
WGStdGraph::MouseMoved(BPoint thePt, uint32 transit, const BMessage* message)
{
	// Since the plot area is somewhat smaller than the area of the view,
	// 	we need to specifically test for it. 
	if(mOdometer != NULL) {
		if(transit == B_INSIDE_VIEW && mPlotArea.Contains(thePt)) {
			mOdometer->Update(thePt);
		} else {
			mOdometer->Update(BPoint(-1,-1));
		}	
	}
}
// -------------------------------------------------------------------
void
WGStdGraph::MouseUp(BPoint thePoint)
{
	if(mTracking) {
		EndRectTracking();
		mTracking = false;
		// Make sure rectangle is valid
		if(thePoint.x > mSelectRect.left) {
			mSelectRect.right = thePoint.x;
		} else {
			mSelectRect.right = mSelectRect.left;
			mSelectRect.left = thePoint.x;
		}
		if(thePoint.y > mSelectRect.top) {
			mSelectRect.bottom = thePoint.y;
		} else {
			mSelectRect.bottom = mSelectRect.top;
			mSelectRect.top = thePoint.y;
		}
		// Now, a derived class can use the BRect for whatever purpose
	}
}
// -------------------------------------------------------------------
void
WGStdGraph::ReScaleTo(BRect theRect)
{
	BAutolock lock(Window());
	// The coords will be trimmed if out of bounds
	float xHi,yHi,xLo,yLo;
	PosnToValue(theRect,xHi,xLo,yHi,yLo);
	AutoScale(xLo, xHi, yLo, yHi);
	ForceReDraw();
}
// -------------------------------------------------------------------
