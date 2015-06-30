/* ------------------------------------------------------------------ 

	Title: GDWindow

	Description:  A window for drawing the WGStdGraph view.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		03 Dec 97
			Created
------------------------------------------------------------------ */ 

#ifndef GD_Window
#define GD_Window

#include <InterfaceKit.h>
#include "GDView.h"

class GDWindow : public BWindow {

	public:
		GDWindow(BRect frame); 
		
	virtual	bool	
		QuitRequested();
		
	protected:
		GDView*	mGraphView;
};

#endif