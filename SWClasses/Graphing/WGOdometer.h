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

#ifndef WG_Odometer
#define WG_Odometer

#include "WGStdGraph.h"

class WGOdometer : public BView {

	public:

		WGOdometer(WGStdGraph* theGraph,
					  BRect frame				= BRect(0,0,0,0),
					  bool drawStacked		= true,		
					  char* label 				= "WGOdometer",
					  const char* xLabel 	= "X = ",
					  const char* yLabel		= "Y = ",
					  uint32 resizeMode = B_FOLLOW_NONE,
					  uint32 flags = B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	virtual				  
		~WGOdometer();
		
	virtual void
		ResizeToPreferred();
		
	virtual void
		SetHighColor(rgb_color color);	
		
	virtual void
		SetLowColor(rgb_color color);	
		
	virtual void
		SetViewColor(rgb_color color);	
		
	void
		Update(BPoint pt);	
		
	protected:	// Methods
	
	virtual void
		Draw(BRect rect);
			
	protected:	// Data
	
		BStringView* 	mXLabel;
		BStringView* 	mYLabel;
		BStringView* 	mXValue;
		BStringView* 	mYValue;
		WGStdGraph*		mGraph;
		int				mXDPs;
		int				mYDPs;
		ERealFormat		mXFmt;
		ERealFormat		mYFmt;
		bool				mStacked;
	};
    
#endif
