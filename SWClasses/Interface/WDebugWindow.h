/* ------------------------------------------------------------------ 

	Title: WDebugWindow

	Description:  
		A window class for showing debugging data.
	
	Author :
		Stephen Wardlaw, M.D.
		Medical Reserch & Development
		Highrock
		Lyme, Ct 06371
		<swardlaw@connix.com>

	Edit History:	
		08 September, 1999
			Split off from WTextWindow
		18 February, 1998
			Created
		
------------------------------------------------------------------ */ 

#ifndef _WDebugWindow
#define _WDebugWindow

#include "WTextWindow.h"

const int32	SET_DEBUG_LEVEL 	= 'sDbg';	// Debug message
const int32 DEBUG_MESSAGE		= 'MDbg';	// Debug string(s)
const char	DEBUG_CONTENT[8]	= "dbgcon";

class WDebugWindow : public WTextWindow {

	public:
		WDebugWindow(BRect 	frame,
					char*	name	= "Debug Window");
					    
					    
	virtual void
		MessageReceived(BMessage* theMesg);
		
	protected :	// Data
	BMenu*		mDbgMenu;
};
#endif

