/* ------------------------------------------------------------------ 

	Title: WUPrefFile

	Description:  A class for handling common pref file items.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		03 February, 2000
			Created
		
------------------------------------------------------------------ */
#ifndef _WUPrefFile
#define _WUPrefFile

#include <AppKit.h>

class WUPrefFile {

	public :
	
		WUPrefFile(const char* prefFileDir, const char* prefFileName);
		
	BMessage*
		GetPrefData();
		
	bool
		SetPrefData(BMessage* theData);
		
	protected :	// Data
	
	BFile		mFile;
};
#endif
