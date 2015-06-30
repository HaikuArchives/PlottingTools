/* ------------------------------------------------------------------ 

	Title: WBitmapMenuItem

	Description:  A derived BMenuItem class for showing bitmaps 

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		09 October, 1999
			Created
		
------------------------------------------------------------------ */ 

#ifndef _WBitmapMenuItem
#define _WBitmapMenuItem

#include "WU_AppUtils.h"
#include <InterfaceKit.h>

class WBitmapMenuItem : public BMenuItem {

	public :
		
		WBitmapMenuItem(const char* label = "",
					    BMessage* theMesg = NULL,
					    char shortcut = 0,
					    uint32 modifiers = 0);

	virtual
		~WBitmapMenuItem();
		
	void
		AddBitmap(BBitmap* theBitmap) {mBitmap = theBitmap;}
		
	void
		AddBitmap(int32 resID) {mBitmap = ::GetAppBitmap(resID);}
		
	const BBitmap*
		BitMap() const {return mBitmap;}
		
	virtual void
		DrawContent();
		
	virtual void
		GetContentSize(float* width, float* height);
		
	protected :	// Data
	
	BBitmap*	mBitmap;
};

#endif

