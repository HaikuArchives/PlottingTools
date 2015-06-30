/* ------------------------------------------------------------------ 

	Title: GrafDemo

	Description:  A demo application to show the graphing classes.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		04 Dec 97
			Created
		
------------------------------------------------------------------ */ 

#ifndef Graf_Demo
#define Graf_Demo

#include <AppKit.h>

class GrafDemo : public BApplication {

public:
		GrafDemo();
		
	virtual void
		AboutRequested();
};

#endif