/* ------------------------------------------------------------------ 

	Title: WGOrdinateLabel

	Description:  A class for drawing ordinate labels

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		07 Dec, 97
			Created for the BeOS		
------------------------------------------------------------------ */ 

#include "WGOrdinateLabel.h"

// ------------------------------------------------------------------ 
//  *             CONSTRUCTORS AND DESTRUCTOR
// ------------------------------------------------------------------ 
WGOrdinateLabel::WGOrdinateLabel(BRect frame, const char* label, const char* text, uint32 resizeMode, uint32 flags)
											: BStringView(frame,label,text, resizeMode,flags)
{
	BFont theFont;
	GetFont(&theFont);
	theFont.SetRotation(90);
	SetFont(&theFont);
	// SetAlignment(B_ALIGN_CENTER);
}
// ------------------------------------------------------------------ 
//  *           PUBLIC METHODS
// ------------------------------------------------------------------ 
void
WGOrdinateLabel::Draw(BRect theRect)
{
	BRect frame = Bounds();
	font_height fHeight;
	GetFontHeight(&fHeight);
	float len = StringWidth(Text());
	MovePenTo(fHeight.ascent,frame.bottom - (frame.bottom - frame.top - len)/2);
	DrawString(Text());
}
