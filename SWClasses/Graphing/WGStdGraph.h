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
		28 September, 1999
			Made odometer integral with this class
		14 July, 1999
			Drag scaling added
		07 Dec 97
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
/*
	IMPLEMENTATION NOTES:
	
	This class will allow the production of very usable graphs with
	little additional code.
	
	The programmer may specify that the graph axes must be of equal 
	(graphic) length.In this case, the graph axes will be sized so that
	the longest one is shrunken to fit the other.
		
	The axes have a linear or log scale.  The client may also provide
	a transformation function.  If this is the case, the axes and any
	data subsequently plotted will be transformed accordingly.
*/		
//----------------------------------------------------------------------
//*  Notes on Methods		
//----------------------------------------------------------------------
/*
	->void ForceReDraw();
	Forces a complete re-drawing of the graph at the next update
	event, and creates an update event.
	
	->void PlotFunct(PlotFunction y_of_XFunct);
	Plots the value y := FunctBase::..CalcYFromX(x) from min to max X.

	->void PlotLine(float x1,y1,x2,y2);
	Plots a line between the two coordinates relative to the graph    
	scale in use.  No line is plotted if either end is out of the graph. 
	The color and line style are whatever is set.

	->bool PlotPoint(float X,Y);
	This procedure plots a point using the current marker characteristics.
	TRUE is returned if the point is inside of the graph bounds  

	->bool PlotTo(float X,Y);
	Plots from  the last positioned point to X,Y in the graph axes.   
	No line is plotted and FALSE is returned if the current point   
	is out of bounds. No line is plotted if the previous point was    
	out of bounds.

	->bool PosnToValue(Point pt,; float &x, float &y);
	Finds the real coordinate position from the graphic coordinate and   
	returns FALSE if the point is outside of the graph boundary.

	->bool PosnX(float x, int &X);
	Returns TRUE if "x" is within the current X-Axis limits and sets  
	"X" to the graph coordinate position.

	->bool PosnY(float y, int &Y);
	Returns TRUE if "y" is within the current Y-Axis limits and sets  
	"Y" to the graph coordinate position.
	
	->SetFrame();
	This sets up the internal graph coordinates so that the methods
	which plot the data will work properly.  This is normally called
	before the graph is drawn, but if you need to plot the data before
	then, call this first, after the drawing port is set.

	->float XformData(float val,
		                   bool onXAxis,
		                   bool toGraphUnits);
	This procedure transforms 'val' either to or from the actual 
	plotted graph units, for either the X ('xAxis' = TRUE) or Y axis.
	For example, if the X-Axis were a Log 10 scale:
		Xform(100.0,TRUE,TRUE) would return 2.0, and
		Xform(2.0,TRUE,FALSE) would return 100.0
	If a range error occurs, the function should not bomb but should return
	a very high number, a very low number, or zero, whichever is least
	likely to cause a major error.
*/
//----------------------------------------------------------------------

#ifndef _WGStdGraph
#define _WGStdGraph

#include "WGAbscissaLabel.h"
#include "WGOrdinateLabel.h"
#include "WGAbscissa.h"
#include "WGOrdinate.h"
	

// Plot markers
enum EPlotMark{
	plotMark_none = -2,				// Don't draw at all
	plotMark_spec = -1,				// Special marking symbol 
	plotMark_dot = 0,				// Very small square 'dot' 
	plotMark_plus = 1,				// A plus sign 
	plotMark_smCirc = 2,			// Small circle
	plotMark_diam = 3,				// A diamond 
	plotMark_lgCirc = 4,			// Large circle                                   
	plotMark_filledDot = 5,		// Very small filled square 'dot' 
	plotMark_filledSmCirc = 6,	// Small filled circle
	plotMark_filledLgCirc = 7	// Large filled circle                                   
};

struct SPlotData{
	float x;
	float y;
	rgb_color	color;
	EPlotMark	mark;
};

// NOTE that this view will not automatically draw!
// The parent view is responsible for calling Draw().

class WGOdometer;	// The odometer for the graph, if added
		
class WGStdGraph : public BView{
	
	public:
	
	// Create a graph from data contained in a BMessage
	static WGStdGraph*	
		Instantiate(BMessage* theArchive);
		
		WGStdGraph(BRect frame,  
					  const char* title	= "StdGraph",
					  uint32 mode			= B_FOLLOW_ALL_SIDES,
					  uint32 flags			= B_FULL_UPDATE_ON_RESIZE + 
												  B_FRAME_EVENTS); 

		WGStdGraph(BMessage* theMessage);
		
	void
		AttachOdometer(WGOdometer* theOdo) {mOdometer = theOdo;}
	virtual void
		Draw(BRect theRect);
	const char* const
		GetXLabel();
	const char* const
		GetYLabel();
	void
		ForceReDraw();
	rgb_color
		PlotColor() {return mPlotColor;}
	virtual void
		PlotFunct(WMFunctBase* extFunct);
	virtual void
		PlotLine(float x1,float y1,float x2,float y2);
	EPlotMark
		PlotMark() {return mPlotMark;}
	virtual bool
		PlotPoint(float X,float Y);
	virtual bool
		PlotPoint(SPlotData* theData);
	virtual bool
		PlotTo(float X,float Y);
	virtual bool
		PosnToValue(BPoint pt, float &x, float &y);
	virtual bool
		PosnToValue(BRect theRect, float &xHi, float &xLow, float &yHi, float &yLow);
	virtual bool
		PosnX(float x, float &X);
	virtual bool
		PosnY(float y, float &Y);
	void
		Refresh();
	void		// Allow drag resizing
		SelectEnable(bool enable) {mSelectEnable = enable;}
	void
		SetAxesEqual(bool axEqual) {mEqualAxes = axEqual;}
	void		// Calculates the frame posn
		SetFrame();
	void
		SetGraphColor(rgb_color lineColor);
	void
		SetGraphFont(BFont* theFont);
	void
		SetPlotColor(rgb_color theColor) {mPlotColor = theColor;}
	void
		SetPlotMark(EPlotMark theMark) {mPlotMark = theMark;}
	void
		SetXLabel(const char* label);
	void
		SetYLabel(const char* label);
	float
		XformData(float val,
					 bool onXAxis,
					 bool toGraphUnits);
				  
	protected:	// Methods

	virtual void
		AutoScale(float &xLo, float &xHi, float &yLo, float &yHi);
	virtual void
		FrameResized(float width, float height);
	void
		InitGraph();
	virtual void
		MouseDown(BPoint thePoint);
	virtual void
		MouseMoved(BPoint thePt, uint32 transit, const BMessage* message);
	virtual void
		MouseUp(BPoint thePoint);
	virtual void
		ReScaleTo(BRect theRect);
		
	public:	// Data
	WGAbscissa*			mXAxis;			// The x-axis
	WGOrdinate*			mYAxis;			// The y-axis
	WGAbscissaLabel*	mXLabel;		// Label for x-axis
	WGOrdinateLabel*	mYLabel;		// Label for y-axis
	WGOdometer*			mOdometer;		// Graph odometer (optional)
  
  protected:	// Data
   
   enum {label_len = 32};
   
	BRect		mPlotArea;				// Display area for data (inside skeleton)
	BRect		mSelectRect;			// Defined by the drag-selection
 	BPoint		mXLabOrigin;			// Where to draw X-Label
	BPoint		mYLabOrigin;			// Where to draw Y-Label
	float		mLastX;					// The last valid x-posn plotted 
	float		mLastY;					// The last valid y-posn plotted
	rgb_color	mPlotColor;				// Color of plot mark
	bool		mChangedAxes;			// Have the axes been changed?
	bool		mEqualAxes;				// Are X & Y axes equal?
	EPlotMark	mPlotMark;				// Type of plot mark to use
	bool		mSelectEnable;			// Allow drag mouse change of axes
	bool		mTracking;				// Mouse resizing is tracking
	};
#endif
