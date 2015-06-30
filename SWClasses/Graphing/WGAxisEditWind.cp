/* ------------------------------------------------------------------ 

	Title: WGAxisEditWind

	Description:  A Window class for editing a graph axis

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		01 October, 1999
			Created
------------------------------------------------------------------ */ 

#include "WGAxisEditWind.h"

#include "WColorControl.h"
#include "WGPanelView.h"
#include "WGGraphAxis.h"

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGAxisEditWind::WGAxisEditWind(BRect frame,
						bool useMarkColor,
				   	  	const char* name,
				   		window_look look,
				   		window_feel feel,
				   		int32 flags):BWindow(frame,name,look,feel,flags)
{
	frame = Bounds();
	mPanel = new WGPanelView(frame,useMarkColor);
	AddChild(mPanel);
}	
				   
WGAxisEditWind::~WGAxisEditWind()
{

}			   
// -------------------------------------------------------------------
// *  Public Methods           
// -------------------------------------------------------------------
void
WGAxisEditWind::MessageReceived(BMessage* theMesg)
{
	BWindow::MessageReceived(theMesg);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetPlotColorDefault(rgb_color defColor)
{
	mPanel->SetPlotColor(defColor);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetPlotMarkDefault(EPlotMark defMark)
{
	mPanel->SetPlotMark(defMark);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetTarget(BMessenger* theMsngr)
{
	mPanel->SetTarget(theMsngr);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetXDefaults(BMessage* theMesg)
{
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetXDefaults(const WGGraphAxis* theAxis)
{
	WGAxisEdit* theEditor = (WGAxisEdit*)mPanel->XEditor();
	theEditor->SetDefaultVals(theAxis);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetXLabel(const char* label)
{
	WGAxisEdit* theEditor = (WGAxisEdit*)mPanel->XEditor();
	theEditor->SetLabel(label);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetYDefaults(BMessage* theMesg)
{
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetYDefaults(const WGGraphAxis* theAxis)
{
	WGAxisEdit* theEditor = (WGAxisEdit*)mPanel->YEditor();
	theEditor->SetDefaultVals(theAxis);
}
// -------------------------------------------------------------------
void
WGAxisEditWind::SetYLabel(const char* label)
{
	WGAxisEdit* theEditor = (WGAxisEdit*)mPanel->YEditor();
	theEditor->SetLabel(label);
}
// -------------------------------------------------------------------
// *  Protected Methods         
// -------------------------------------------------------------------
bool
WGAxisEditWind::QuitRequested()
{
//	be_app->PostMessage(B_QUIT_REQUESTED);	//*************
	return true;
}
// -------------------------------------------------------------------
