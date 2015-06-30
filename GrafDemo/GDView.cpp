/* ------------------------------------------------------------------ 

	Title: GDView

	Description:  The master view for the GrafDemo application

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		12 Dec 97
			Created
------------------------------------------------------------------ */ 
#include "GDView.h"

#include "WGAxisEditWind.h"
#include "WGAxisEdit.h"
#include "U_Conversions.h"

#include <stdio.h>
#include <string.h>

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
GDView::GDView(BRect frame,
					const char* title,
					uint32 mode, 
					uint32 flags):BView(frame,title,mode,flags)
{
	const float fView_ht = 20;
	
	// Add the display for the goodness of fit
	BRect fFrame(0,5,150,fView_ht);
	mFitView = new BStringView(fFrame,"Fit View","");
	mFitView->SetFontSize(12);
	AddChild(mFitView);
	
	// Offset the frame to make room for the data display
	frame.OffsetTo(B_ORIGIN);
	BFont theFont;
	GetFont(&theFont);
	float width = theFont.StringWidth("Cubic Spline Fit  ");
	frame.right -= width;
	frame.top += fView_ht;
	mGraph = new GDPolyPlot(&mData,frame);	
	mGraph->SelectEnable(true);	// Allow drag-rescaling
	mGraph->SetPlotMark(plotMark_plus);

	AddChild(mGraph);	
	
	
	// Add the display for the parameters
	BRect dFrame(frame.right,0,Bounds().right,50);
	mDisp = new GDPolyDisp(&mData,dFrame);
	AddChild(mDisp);
	
	// Add the odometer
	mOdometer = new WGOdometer(mGraph);
	mOdometer->SetResizingMode(B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	AddChild(mOdometer);
	mOdometer->ResizeToPreferred();
	BRect oFrame = mOdometer->Bounds();
	frame = Bounds();	// Get parent's size
	mOdometer->MoveTo(frame.right - oFrame.Width(),frame.bottom - oFrame.Height() - 15);
	
   	// See what these do...
	// SetFontSize(12);
	// GetFont(&theFont);
	// mGraph->SetGraphFont(&theFont);
	
	// rgb_color red = {255,100,100,0};
	// rgb_color black = {0,0,0,0};
	// mGraph->SetGraphColor(red);
 }
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
void
GDView::MessageReceived(BMessage* theMessage)
{   
	WGAxisEditWind* editWind;
	BRect frame;
	BMessenger* theMessenger;
	BMessage	axisMesg;
	int32	colorVal;
	int16	markVal;
	
	const char* labelStr;
	
 	switch ( theMessage->what ){
 	
 		case WGAxisEditWind::class_ID :
 			if(theMessage->FindMessage(x_axis_name,&axisMesg) == B_OK) {
 				labelStr = axisMesg.FindString(axis_label);
 				if(labelStr != NULL) {
 					mGraph->SetXLabel(labelStr);
 				}
 				mGraph->mXAxis->SetValues(&axisMesg);
 			}
 			axisMesg.MakeEmpty();
 			if(theMessage->FindMessage(y_axis_name,&axisMesg) == B_OK) {
 				labelStr = axisMesg.FindString(axis_label);
 				if(labelStr != NULL) {
 					mGraph->SetYLabel(labelStr);
 				}
 				mGraph->mYAxis->SetValues(&axisMesg);
 			}
 			if(theMessage->FindInt32(plot_color_name,&colorVal) == B_OK) {
 				mGraph->SetPlotColor(::ValueToColor(colorVal));
 			}
 			if(theMessage->FindInt16(plot_mark_name,&markVal) == B_OK) {
 				mGraph->SetPlotMark((EPlotMark)markVal);
 			}
			mGraph->ForceReDraw();
 			break;
 			
		case G_erase_data:
			mData.ClearData();
 	   		Reset();
 			break;
 			
 		case G_reset_axes :
			SetGraphAxis(mGraph->mXAxis,scale_linear);
			SetGraphAxis(mGraph->mYAxis,scale_linear);
			mGraph->ForceReDraw();
			break;
			
		case G_invalidate :
			Reset();
			break;
				
		case G_set_params:
			int16 theParam;
			if((theMessage->FindInt16(pName,&theParam) == B_OK) &&
				(theParam != mData.GetFitParams())) {
				mData.SetFitParams(theParam);
 	   			Reset();
			}
			break;
			
		case G_change_axis:
			frame = Window()->Frame();
			frame.top = frame.bottom;
			frame.left = frame.right;
			editWind = new WGAxisEditWind(frame);
			editWind->SetXDefaults(mGraph->mXAxis);
			editWind->SetYDefaults(mGraph->mYAxis);
			editWind->SetXLabel(mGraph->GetXLabel());
			editWind->SetYLabel(mGraph->GetYLabel());
			editWind->SetPlotMarkDefault(mGraph->PlotMark());
			editWind->SetPlotColorDefault(mGraph->PlotColor());
			// Set messenger here
			theMessenger = new BMessenger(this);
			editWind->SetTarget(theMessenger);
			break;
			
   	
		default :
			this->BView::MessageReceived(theMessage);
	}
}
// -------------------------------------------------------------------
void
GDView::Reset()
{
	char text[32] = {0};
	char numStr[16];
	
	if(mData.GetFitParams() > 0 && mData.HaveFit()) {
		::strcpy(text," Chi Sq.of Fit: ");
		::FormatReal(mData.ChiSqValue(),realFmt_float,3,numStr);
		::strcat(text,numStr);
	}
	mFitView->SetText(text);
	mDisp->Invalidate();
	mGraph->Invalidate();
}
// -------------------------------------------------------------------
void
GDView::SetGraphAxis(WGGraphAxis* theAxis, EAxisScale newScale)
{
	theAxis->SetScale(newScale);
	theAxis->SetLabelFormat(realFmt_fixed,0);
	theAxis->SetMinorDivisions(1);
	
	switch (newScale) {
	
		case scale_log2:
			theAxis->SetSpan(1,128);
			break;
		
		case scale_log10:
			theAxis->SetSpan(0.1,1000);
			theAxis->SetLabelFormat(realFmt_float,1);
			theAxis->SetMinorDivisions(9);
			break;

		default:	// Default is linear
			theAxis->SetSpan(0,100);
			theAxis->SetInterval(10);
			break;
	}
}
// -------------------------------------------------------------------

		
