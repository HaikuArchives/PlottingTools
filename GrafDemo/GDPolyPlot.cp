/* ------------------------------------------------------------------ 

	Title: GDPolyPlot

	Description:  A class which puts a graph window to demonstrate
		polynomial data fitting and cubic spline interpolation

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		28 September, 1999
			Adapted for new WGStdGraph
		27 December, 1997
			Adapted for BeOS
		31 March, 1995
			Broke original into three separate classes
		09 Dec 94
			Much alteration for new PP and other classes
		08 May 94
			Added cubic fit routine
		22 Mar 94
			Created for CW C++ from an earlier M2 program
		
------------------------------------------------------------------ */ 

#include "GDPolyPlot.h"
#include "GDView.h"
#include "Cross.h"
#include "WColorControl.h"

#include <AppKit.h>

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------

GDPolyPlot::GDPolyPlot(GDData* theData,
						BRect frame,  
					  	const char* title,
					  	uint32 mode,
					  	uint32 flags):WGStdGraph(frame,title,mode,flags)
{
	mData = theData;
}
 
		
GDPolyPlot::~GDPolyPlot()
{
}		
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
void
GDPolyPlot::MouseMoved(BPoint thePt, uint32 transit, const BMessage* theMesg)
{
	/* Since the plot area is somewhat smaller than the area of the view,
	 	we need to specifically test for it and set the cursors as appropriate.
	 	Note that this is a little sloppy, because it will not show a shift key
	 	change until after the cursor position is moved.
 	*/
 	// Call base clas for odometer tracking
 	WGStdGraph::MouseMoved(thePt,transit,theMesg);
 	
	key_info theKeys;
	bool shift = false;
	if(get_key_info(&theKeys) == B_OK) {
		shift = B_SHIFT_KEY & theKeys.modifiers;
	} 
	
	if(mPlotArea.Contains(thePt) && transit == B_INSIDE_VIEW) {	// Set new cursor type
		if(shift) {
			be_app->SetCursor(HollowCross);
		} else {
			be_app->SetCursor(Cross);
		}
		
	} else {
		be_app->SetCursor(B_HAND_CURSOR);
	}	
}
// -------------------------------------------------------------------
void
GDPolyPlot::RefreshPlot()
{
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
void
GDPolyPlot::MouseDown(BPoint thePoint)
{
	WGStdGraph::MouseDown(thePoint);	// Pass through for drag-resize
	
	// Save point and time for analysis on MouseUp;
	mClickTime = real_time_clock_usecs();
	mClickPt = thePoint;
	
}
// -------------------------------------------------------------------
void
GDPolyPlot::MouseUp(BPoint thePt)
{
	WGStdGraph::MouseUp(thePt);	// Pass through for drag-resize
	// Autoscale, if appropriate
	if(mSelectRect.Width() > 50 && mSelectRect.Height() > 50) {
		ReScaleTo(mSelectRect);
	}
	/*
		We consider a mouse press and release within 1/2 second to be
		a request for a point.
	*/
	bigtime_t cTime = real_time_clock_usecs() - mClickTime;
	if(cTime > 50000 && cTime < 500000) {
		const float pix_err = 3;
		float x,y;
		bool shift = false;
		
		if(!PosnToValue(mClickPt,x,y)) {
			return;	// EXIT HERE if not in a valid graph area
		}
		
		key_info theKeys;
		if(get_key_info(&theKeys) == B_OK) {
			shift = B_SHIFT_KEY & theKeys.modifiers;
		} 
		
		if(!shift) {	// Add data point
			if(mData->AddPoint(x,y)) {
				// Send an invalidate message
				BMessage theMesg(G_invalidate);
				Parent()->MessageReceived(&theMesg);
			}
			
		} else {		// See if there is a data point to be deleted
			float 	xErr, yErr;
			
			if(!mXAxis->XPosnToVal(mClickPt.x + pix_err,xErr)) {
				mXAxis->XPosnToVal(mClickPt.x - pix_err,xErr);
			}
			if(mYAxis->YPosnToVal(mClickPt.y - pix_err,yErr)) {
				mYAxis->YPosnToVal(mClickPt.y + pix_err,yErr);
			}
			xErr = fabs(x - xErr);
			yErr = fabs(y - yErr);
			if(mData->DeletePoint(x,xErr,y,yErr)) {
				BMessage theMesg(G_invalidate);
				Parent()->MessageReceived(&theMesg);
			}
		}
	}
}
// -------------------------------------------------------------------
void
GDPolyPlot::Draw(BRect frame)
{
	float x,y;
	WGStdGraph::Draw(frame);
	for(int n = 0; n < mData->MaxPoints(); n++) {
		if(mData->GetPoint(n,x,y)) {
			PlotPoint(x,y);
		}
	}
	SetHighColor(mPlotColor);
	PlotFunct(mData);
	SetHighColor(color_black);
}
	
