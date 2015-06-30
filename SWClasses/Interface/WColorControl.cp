/* ------------------------------------------------------------------ 

	Title: WColorControl

	Description:  Basic color definitions and a color control
		for the primary colors

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		08 October, 1999
			Created
		
------------------------------------------------------------------ */ 

#include "WColorControl.h"

#include "U_Conversions.h"
#include <SupportKit.h>

const float min_block_ht = 10;

static const rgb_color sFldColor[9] = {color_black,color_white,color_black,color_red,color_green,color_blue,color_cyan,color_magenta,color_yellow};

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WColorControl::WColorControl(BRect frame, const char* name,
					const char* label,
					BMessage* theMesg):BControl(frame,name,label,theMesg,B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
	mTracking = false;
	SetValue(ColorToValue(color_black));
	mResized = false;
}
					
WColorControl::~WColorControl()
{
}
// -------------------------------------------------------------------
void
WColorControl::SetDefaultColor(rgb_color color)
{
	BAutolock lock(Window());
	SetValue(::ColorToValue(color));
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
void
WColorControl::Draw(BRect frame)
{
	if(!mResized) {
		ResizeToPreferred();
	}
	MovePenTo(mFPoint);
	if(Label() != NULL) {
		DrawString(Label());
	}
	for(int n = 1; n < 9; n++) {		// Draw the color selections
		SetHighColor(sFldColor[n]);
		FillRect(mColorRect[n]);
	}
	SetHighColor(ColorValue()); 		// Set display area to chosen color
	FillRect(mColorRect[0]);			 
	SetHighColor(color_white);
	StrokeRect(mColorRect[0]); 
	SetHighColor(color_black);
	StrokeRect(mCtrlRect);
}
// -------------------------------------------------------------------
int16
WColorControl::InRect(BPoint thePt)
{
	for(int n = 1; n < 9; n++) {
		if(mColorRect[n].Contains(thePt)) {
			return n;
		}
	}
	return -1;
}
// -------------------------------------------------------------------
void
WColorControl::MouseDown(BPoint thePt)
{
	if(IsEnabled() && InRect(thePt) > 0) {
		mRectHit = InRect(thePt);
		SetHighColor(color_white);
		StrokeRect(mColorRect[mRectHit]);
		mHilited = true;
		SetHighColor(color_black);
		mTracking = true;
		SetMouseEventMask(B_POINTER_EVENTS);
	}
}
// -------------------------------------------------------------------
void
WColorControl::MouseMoved(BPoint thePt, uint32 transit, const BMessage* message)
{
	if(mTracking) {
		if(InRect(thePt) == mRectHit) {
			if(!mHilited) {
				SetHighColor(color_white);
				StrokeRect(mColorRect[mRectHit]);
				SetHighColor(color_black);
				mHilited = true;
			}
		} else {	// If not in same rect
			if(mHilited) {
				SetHighColor(sFldColor[mRectHit]);
				StrokeRect(mColorRect[mRectHit]);
				SetHighColor(color_black);
				mHilited = false;
			}
		}
	}	
}
// -------------------------------------------------------------------
void
WColorControl::MouseUp(BPoint thePt)
{
	if(mTracking) {
		mTracking = false;
		if(InRect(thePt) == mRectHit) {
			SetValue(::ColorToValue(sFldColor[mRectHit]));
			if(IsEnabled()) {
				Invoke();
			}
		}
		if(mHilited) {
			SetHighColor(sFldColor[mRectHit]);
			StrokeRect(mColorRect[mRectHit]);
			SetHighColor(color_black);
			mHilited = false;
		}
	}
}
// -------------------------------------------------------------------
void
WColorControl::ResizeToPreferred()
{
	const float ctrl_margin = 2;
	float lLab = 0;
	if(Label() != NULL) {
		lLab = StringWidth(Label()) + ctrl_margin*2;
	}
	font_height fHeight;
	GetFontHeight(&fHeight);
	mFPoint.Set(ctrl_margin,fHeight.ascent + 1);
	float cHeight = fHeight.ascent + fHeight.descent + ctrl_margin*2;
	// Make sure height is multiple of two - round up
	int iHt = (int)(cHeight + 0.5);
	cHeight = 2*(iHt/2);
	if(cHeight < min_block_ht*2) {
		cHeight = min_block_ht*2;
		mFPoint.y = cHeight/2 + fHeight.ascent/2;
	}
	ResizeTo(lLab + cHeight*3 + ctrl_margin*3 + 3,cHeight + 1);
	float cLeft = lLab + ctrl_margin*2;
	mColorRect[0].Set(cLeft,1,cLeft + cHeight, cHeight);
	
	// Divide rest into eight smaller rectangles 
	float width = cHeight/2;
	mColorRect[1].Set(mColorRect[0].right + 1,1,mColorRect[0].right + width + 1,width);
	for(int n = 2; n < 5; n++) {
		mColorRect[n] = mColorRect[n-1];
		mColorRect[n].OffsetBy(width + 1,0);
	}
	mColorRect[5] = mColorRect[1];
	mColorRect[5].OffsetBy(0,width);
	for(int n = 6; n < 9; n++) {
		mColorRect[n] = mColorRect[n-1];
		mColorRect[n].OffsetBy(width + 1,0);
	}
	mCtrlRect.Set(mColorRect[0].left,mColorRect[0].top,mColorRect[8].right,mColorRect[8].bottom);
	mCtrlRect.InsetBy(-1,-1);
	mResized = true;
}
// -------------------------------------------------------------------
rgb_color
WColorControl::ColorValue()
{
	return ::ValueToColor(Value());
}
// -------------------------------------------------------------------
