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

#ifndef _WGPanelView
#define _WGPanelView

#include <AppKit.h>
#include <InterfaceKit.h>
#include "WGAxisEdit.h"
#include "WColorControl.h"
#include "WGStdGraph.h"
#include "WGPlotMarkMenu.h"

class WGPanelView : public BView {

	public:

		WGPanelView(BRect frame,
				   bool useMarkColor,
				   const char* name = "wgpanelview",
				   int32 mode 		= B_FOLLOW_ALL,
				   int32 flags		= B_WILL_DRAW);	
	virtual
		~WGPanelView();
			   
	void
		MessageReceived(BMessage* theMesg);
		
	void
		SetPlotColor(rgb_color defColor);
		
	void
		SetPlotMark(EPlotMark defMark);
		
	const WGAxisEdit*
		XEditor() {return (const WGAxisEdit*)mXEditor;}
		
	const WGAxisEdit*
		YEditor() {return (const WGAxisEdit*)mYEditor;}
		
	void
		SetTarget(BMessenger* theMessenger) {mMessenger = theMessenger;}
		
	protected:	// Methods
	
	void
		AllAttached();
	void
		Draw(BRect frame);
	void
		SendDataMesg(BMessage* theMesg);
		
	protected:	// Data
	BTabView*		mTabView;
	WGAxisEdit*		mXEditor;
	WGAxisEdit*		mYEditor;
	WColorControl*	mColorCtrl;
	WGPlotMarkMenu*	mPlotMark;
	BButton*		mApply;
	BMessenger*		mMessenger;
	};
    
	#endif
