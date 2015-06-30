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
#include "WGPlotMarkMenu.h"

#include "WBitmapMenuItem.h"

// Resource constants for menu bitmaps
const int32 mark_none			= 'mNon';
const int32 mark_dot			= 'mDot';				
const int32 mark_plus			= 'mPlu';				
const int32 mark_smCirc			= 'mScr';			
const int32 mark_diam			= 'mDia';				
const int32 mark_lgCirc			= 'mLCr';			                                  
const int32 mark_filledDot		= 'mFDo';
const int32 mark_filledSmCirc	= 'mFSC';
//const int32 mark_filledSmDiam	= 'mFSD';
const int32 mark_filledLgCirc	= 'mFLC';

const float ctrl_ht = 18;
const float cview_wd = 24;
const float ctrl_margin = 2;
// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGPlotMarkMenu::WGPlotMarkMenu(BRect frame, 
							   const char* name,
							   const char* label,
							  BMessage* theMesg) : BControl(frame,name,label,theMesg,B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW)
{
	// Menu for plot marks
	mMenu = new BPopUpMenu("pmark");
	WBitmapMenuItem* theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_none);
	mMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_dot);
	mMenu->AddItem(theItem);
	theItem->SetMarked(true);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_plus);
	mMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_smCirc);
	mMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_diam);
	mMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_lgCirc);
	mMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_filledDot);
	mMenu->AddItem(theItem);
	//theItem = new WBitmapMenuItem();
	//theItem->AddBitmap(mark_filledSmDiam);
	//theMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_filledSmCirc);
	mMenu->AddItem(theItem);
	theItem = new WBitmapMenuItem();
	theItem->AddBitmap(mark_filledLgCirc);
	mMenu->AddItem(theItem);
	
	mLabelView = NULL;
	if(Label() != NULL) {
		mLabelView = new BStringView(BRect(0,0,5,ctrl_ht),"",Label());	
		AddChild(mLabelView);
	}
	mDisplay.Set(0,0,cview_wd,ctrl_ht);
	mDisplay.InsetBy(ctrl_margin,ctrl_margin);
}
					
WGPlotMarkMenu::~WGPlotMarkMenu()
{
}
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
void
WGPlotMarkMenu::GetPreferredSize(float *width, float *height)
{
	*width = Bounds().Width();
	*height = Bounds().Height();
}
// -------------------------------------------------------------------
EPlotMark
WGPlotMarkMenu::PlotMark()
{
	BMenuItem* theItem = mMenu->FindMarked();
	if(theItem != NULL) {
		int16 index = mMenu->IndexOf(theItem);
		if(index == 0) {
			return plotMark_none;
		} else {
			return (EPlotMark)(index - 1);
		}
	} else {
		return plotMark_dot;
	}
}
// -------------------------------------------------------------------
void
WGPlotMarkMenu::ResizeToPreferred()
{
	float width = 0;
	float height = 15;
	float lPosn = 0;
	if(mLabelView != NULL) {
		mLabelView->ResizeToPreferred();
		mLabelView->GetPreferredSize(&width,&height);
	}
	ResizeTo(width + cview_wd + ctrl_margin*3,height + ctrl_margin*2);
	if(mLabelView != NULL) {
		mLabelView->MoveTo(ctrl_margin,ctrl_margin);
		lPosn = mLabelView->Frame().right;
	}
	mDisplay.OffsetTo(lPosn + ctrl_margin,ctrl_margin);
}
// -------------------------------------------------------------------
void
WGPlotMarkMenu::SetDefaultMark(EPlotMark mark)
{
	BMenuItem* theItem = mMenu->ItemAt(mark);
	if(theItem != NULL) {
		theItem->SetMarked(true);
	}
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
void
WGPlotMarkMenu::AllAttached()
{
	BControl::AllAttached();
	ResizeToPreferred();
}
// -------------------------------------------------------------------
void
WGPlotMarkMenu::Draw(BRect frame)
{
	SetHighColor(0,0,0);
	WBitmapMenuItem* theItem = (WBitmapMenuItem*)mMenu->FindMarked();
	if(theItem != NULL) {
		const BBitmap* theBitmap = theItem->BitMap();
		BRect mapRect = theBitmap->Bounds();
		float leftMargin = (cview_wd - mapRect.Width())/2 - ctrl_margin;
		float topMargin = (ctrl_ht - mapRect.Height())/2 - ctrl_margin;
		MovePenTo(mDisplay.left + leftMargin,mDisplay.top + topMargin);
		DrawBitmap(theBitmap);
	}
	StrokeRoundRect(mDisplay,ctrl_margin,ctrl_margin);	
}
// -------------------------------------------------------------------
void
WGPlotMarkMenu::MouseDown(BPoint thePt)
{
	if(mDisplay.Contains(thePt)) {
		BPoint thePt(mDisplay.left,mDisplay.top);
		ConvertToScreen(&thePt);
		BMenuItem* theItem = mMenu->Go(thePt);
		if(theItem != NULL) {
			theItem->SetMarked(true);
		}
	}
}
// -------------------------------------------------------------------
