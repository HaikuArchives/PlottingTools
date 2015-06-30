/* ------------------------------------------------------------------ 

	Title: WGPlotMarkMenu

	Description:  A menu control for choosing plot marks

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		21 October, 1999
			Created
		
------------------------------------------------------------------ */ 

#ifndef _WGPlotMarkMenu
#define _WGPlotMarkMenu

#include <InterfaceKit.h>
#include "WGStdGraph.h"


class WGPlotMarkMenu : public BControl {

	public :
		
		WGPlotMarkMenu(BRect frame, const char* name = "plotmarkmenu",
					const char* label = "Plot Mark:",
					BMessage* theMesg = NULL);
					
		virtual
			~WGPlotMarkMenu();
		
		EPlotMark
			PlotMark();
			
		void
			GetPreferredSize(float *width, float *height);
			
		void
			ResizeToPreferred();
			
		void
			SetDefaultMark(EPlotMark mark);
			
		protected :
		
		virtual void
			AllAttached();
		
		virtual void
			Draw(BRect frame);
			
		virtual void
			MouseDown(BPoint thePt);
		
		protected:	// Data
		BPopUpMenu*		mMenu;
		BStringView*	mLabelView;
		BRect			mDisplay;
		
		
};

#endif

