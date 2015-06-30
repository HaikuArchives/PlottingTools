/* ------------------------------------------------------------------ 

	Title: GrafDemo

	Description:  A demo application to show the graphing classes.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		08 April, 1999
			Modified for Be 4.1
		12 May, 1998
			Version 2.0
		04 Dec 97
			Created
		
------------------------------------------------------------------ */ 

#include "GrafDemo.h"
#include "GDWindow.h"
#include "WGAxisEdit.h"	//***********

const char* appSig = "application/x-vnd.SW.GrafDemo";

int
main()
{	
	GrafDemo* theTest;

	theTest = new GrafDemo();
	theTest->Run();
	
	delete(theTest);
	return(0);
}


// ------------------------------------------------------------------ 
GrafDemo::GrafDemo() : BApplication(appSig)
{
	GDWindow* 	theWindow;
	BRect			graphRect;

	// set up a rectangle and instantiate a new window
	graphRect.Set(100, 100, 380, 340);
	theWindow = new GDWindow(graphRect);
	theWindow->Show();
	
}
// ------------------------------------------------------------------ 
void
GrafDemo::AboutRequested()
{
	BAlert* theAlert = new BAlert("","Graph Demo V4.6...  \nClick on the graph to add a point.\nShift-click to delete one.\nClick-drag to resize.\n By: Stephen Wardlaw, M.D. swardlaw@connix.com","OK");
	theAlert->Go();
}
// ------------------------------------------------------------------ 
