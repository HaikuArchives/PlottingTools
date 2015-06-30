/* ------------------------------------------------------------------ 

	Title: WGPanelView

	Description:  A view class for editing graph parameters

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		08 October, 1999
			Created
		
------------------------------------------------------------------ */ 

#include "WGPanelView.h"
#include "WBitmapMenuItem.h"
#include "WGAxisEditWind.h"
#include "U_Conversions.h"

const float ctrl_margin = 10;

const uint32 apply_mesg	= 'Mapp';

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGPanelView::WGPanelView(BRect frame,
					bool useMarkColor,
				   const char* name,
				   int32 mode,
				   int32 flags):BView(frame,name,mode,flags)
{
	SetViewColor(color_std_gray);
	frame.InsetBy(5,5);
	mTabView = new BTabView(frame,"axistabs",B_WIDTH_FROM_WIDEST);
	mTabView->SetResizingMode(B_FOLLOW_NONE);
	frame = mTabView->Bounds();
	frame.InsetBy(5,5);
	
	mXEditor = new WGAxisEdit(frame,"X-Axis");
	BTab* xTab = new BTab();
	mTabView->AddTab(mXEditor,xTab);
	
	mYEditor = new WGAxisEdit(frame,"Y-Axis");
	BTab* yTab = new BTab();
	mTabView->AddTab(mYEditor,yTab);
	AddChild(mTabView);
	
	mApply = new BButton(BRect(0,0,0,0),"apply","Apply",new BMessage(apply_mesg));
	AddChild(mApply);
	
	if(useMarkColor) {	// If we allow these
		mColorCtrl = new WColorControl(BRect(0,0,10,10));
		AddChild(mColorCtrl);
	
		// Menu for plot marks
		mPlotMark = new WGPlotMarkMenu(BRect(0,0,18,16));
		AddChild(mPlotMark);
	} else {
		mPlotMark = NULL;
		mColorCtrl = NULL;
	}
}	

WGPanelView::~WGPanelView()
{
	delete mMessenger;
}
// -------------------------------------------------------------------
void
WGPanelView::SetPlotColor(rgb_color defColor)
{
	if(mColorCtrl != NULL) {
		mColorCtrl->SetDefaultColor(defColor);
	}
}
// -------------------------------------------------------------------
void
WGPanelView::SetPlotMark(EPlotMark defMark)
{
	if(mPlotMark != NULL) {
		mPlotMark->SetDefaultMark(defMark);
	}
}
// -------------------------------------------------------------------
// *  Public Methods           
// -------------------------------------------------------------------
void
WGPanelView::MessageReceived(BMessage* theMesg)
{
	BMessage* outMesg;
	EPlotMark pMark;
	
	switch (theMesg->what) {
	
		case apply_mesg :	// 'Apply' button was pressed
			outMesg = new BMessage(WGAxisEditWind::class_ID);
			outMesg->AddMessage(x_axis_name,mXEditor->Data());
			outMesg->AddMessage(y_axis_name,mYEditor->Data());
			if(mPlotMark != NULL) {
				pMark = mPlotMark->PlotMark();
				outMesg->AddInt16(plot_mark_name,pMark);
				outMesg->AddInt32(plot_color_name,::ColorToValue(mColorCtrl->ColorValue()));
			}
			SendDataMesg(outMesg);
			delete outMesg;
			break;
			
		default :
			BView::MessageReceived(theMesg);
	}
}
// -------------------------------------------------------------------
// *  Protected Methods           
// -------------------------------------------------------------------
void
WGPanelView::AllAttached()
{
	// Start by sizing the tab view
	BRect frame = mXEditor->Bounds();
	frame.bottom += mTabView->TabHeight();
	frame.InsetBy(-ctrl_margin,-ctrl_margin);
	mTabView->ResizeTo(frame.Width(),frame.Height());	// Tab frame
	mTabView->MoveTo(ctrl_margin,ctrl_margin);
	mXEditor->MoveTo(ctrl_margin,ctrl_margin);
	mYEditor->MoveTo(ctrl_margin,ctrl_margin);
	
	mApply->ResizeToPreferred();
	float bWidth, height;
	mApply->GetPreferredSize(&bWidth,&height);
	
	float rMarg = mTabView->Frame().right + ctrl_margin;
	if(mPlotMark != NULL) {
		// Position the color control
		mColorCtrl->ResizeToPreferred();
		mColorCtrl->MoveTo(rMarg, mTabView->Frame().top + mTabView->TabHeight());
		rMarg += mColorCtrl->Bounds().Width();
		// Position the menu
		mPlotMark->MoveTo(mColorCtrl->Frame().left,mColorCtrl->Frame().bottom + ctrl_margin);
	} else {
		rMarg += bWidth;
	}
	
	// Resize the window
	Window()->ResizeTo(rMarg + ctrl_margin, mTabView->Frame().bottom + ctrl_margin);
	mApply->MoveTo(Bounds().right - (bWidth + ctrl_margin), Bounds().bottom - (height + ctrl_margin));
	mApply->SetTarget(this);
	Window()->Show();
}
// -------------------------------------------------------------------
void
WGPanelView::Draw(BRect frame)
{
}
// -------------------------------------------------------------------
void
WGPanelView::SendDataMesg(BMessage* theMesg)
{
	if(mMessenger != NULL) {
		mMessenger->SendMessage(theMesg);
	} else {
		be_app->PostMessage(theMesg);
	}
}
// -------------------------------------------------------------------
