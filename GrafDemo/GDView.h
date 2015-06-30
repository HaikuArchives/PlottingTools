/* ------------------------------------------------------------------ 

	Title: GDView

	Description:  The master view for theGraph-It application

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		12 Dec 97
			Created
------------------------------------------------------------------ */ 

#ifndef GD_View
#define GD_View

#include <InterfaceKit.h>
#include "GDPolyPlot.h"
#include "GDPolyDisp.h"
#include "GDData.h"
#include "WGOdometer.h"

const char pName[8] = "Params";
const char aName[8] = "Axis";

const uint32 G_invalidate 	= 'GInv';
const uint32 G_erase_data 	= 'Gera';
const uint32 G_set_params 	= 'Gset';
const uint32 G_change_axis	= 'Gchg';
const uint32 G_reset_axes	= 'Grst';

class GDView : public BView{

	public:
		GDView(BRect frame,
					  	 const char* title	= "GDView",
					  	 uint32 mode			= B_FOLLOW_ALL_SIDES,
					  	 uint32 flags			= B_WILL_DRAW + 
												  	  B_FULL_UPDATE_ON_RESIZE + 
												  	  B_FRAME_EVENTS); 
	GDData*
		Data() {return &mData;}

	protected: 	// Methods
	
	virtual void
		MessageReceived(BMessage* theMessage);
		
	void
		Reset();
		
	void
		SetGraphAxis(WGGraphAxis* theAxis, EAxisScale newScale);
		
	protected:	// Data
		
		GDData			mData;
		GDPolyPlot*		mGraph;
		GDPolyDisp*		mDisp;
		BStringView*	mFitView;
		WGOdometer*		mOdometer;
	
};

#endif