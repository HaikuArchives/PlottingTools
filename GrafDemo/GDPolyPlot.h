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
		02 August, 1999
			Added drag-resizing and adapted for BeOS 4.5
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

#ifndef GD_PolyPlot
#define GD_PolyPlot

#include "GDData.h"
#include "WGStdGraph.h"


class GDPolyPlot : public WGStdGraph {

	public:
	
		GDPolyPlot(GDData* theData,
					BRect frame,  
					const char* title	= "GDPolyPlot",
					uint32 mode			= B_FOLLOW_ALL_SIDES,
					uint32 flags		= B_WILL_DRAW | 
					  						B_FULL_UPDATE_ON_RESIZE | 
											B_FRAME_EVENTS); 
	virtual	
		~GDPolyPlot();
		
	virtual void
		MouseMoved(BPoint thePt, uint32 transit, const BMessage* theMesg);
	virtual void
		MouseUp(BPoint thePt);
	void
		RefreshPlot();
		
	protected:

	virtual void
		MouseDown(BPoint thePoint);
	virtual void
		Draw(BRect frame);
	
	GDData*		mData;
	bigtime_t	mClickTime;
	BPoint		mClickPt;
};

#endif