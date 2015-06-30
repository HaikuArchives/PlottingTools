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

#include "WBitmapMenuItem.h"

#include <AppKit.h>
#include <StorageKit.h>
// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
		
WBitmapMenuItem::WBitmapMenuItem(const char* label,
					    BMessage* theMesg,
					    char shortcut,
					    uint32 mods):BMenuItem(label,theMesg,shortcut,mods)
{
	mBitmap = NULL;
}				

WBitmapMenuItem::~WBitmapMenuItem()
{
	delete mBitmap;
}		
// -------------------------------------------------------------------
// *  Public Methods           
// -------------------------------------------------------------------
void
WBitmapMenuItem::DrawContent()
{
	if(mBitmap != NULL) {	
		BRect mapRect = mBitmap->Bounds();
		float width, height;
		GetContentSize(&width, &height);
		BPoint thePt = ContentLocation();
		float margin = height/2 - mapRect.Height()/2;
		Menu()->MovePenTo(thePt.x + margin,thePt.y + margin);
		Menu()->DrawBitmap(mBitmap);
	}
}
// -------------------------------------------------------------------
void
WBitmapMenuItem::GetContentSize(float* width, float* height)
{
	BRect mapRect = mBitmap->Bounds();
	*width = mapRect.Width();
	*height = mapRect.Height();
}
// -------------------------------------------------------------------
// *  Protected Methods           
// -------------------------------------------------------------------
// -------------------------------------------------------------------
