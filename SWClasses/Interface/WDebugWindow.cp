/* ------------------------------------------------------------------ 

	Title: WDebugWindow

	Description:  
		A window class for showing debugging data.
	
	Author :
		Stephen Wardlaw, M.D.
		Medical Reserch & Development
		Highrock
		Lyme, Ct 06371
		<swardlaw@connix.com>

	Edit History:	
		08 September, 1999
			Split off from WDebugWindow
		18 February, 1998
			Created
		
------------------------------------------------------------------ */ 

#include "WDebugWindow.h"
#include <AppKit.h>

// -------------------------------------------------------------------
//	* CONSTRUCTOR
// -------------------------------------------------------------------
WDebugWindow::WDebugWindow(BRect frame,
						   char* name):WTextWindow(frame,name)
{
	mDbgMenu = new BMenu("Debug");
	BMessage* dbgMessage0 = new BMessage(SET_DEBUG_LEVEL);
	dbgMessage0->AddInt16("Level",0);
	BMenuItem* firstItem = new BMenuItem("None",dbgMessage0);
	mDbgMenu->AddItem(firstItem);
	BMessage* dbgMessage1 = new BMessage(SET_DEBUG_LEVEL);
	dbgMessage1->AddInt16("Level",1);
	mDbgMenu->AddItem(new BMenuItem("1",dbgMessage1));
	BMessage* dbgMessage2 = new BMessage(SET_DEBUG_LEVEL);
	dbgMessage2->AddInt16("Level",2);
	mDbgMenu->AddItem(new BMenuItem("2",dbgMessage2));
	BMessage* dbgMessage3 = new BMessage(SET_DEBUG_LEVEL);
	dbgMessage3->AddInt16("Level",3);
	mDbgMenu->AddItem(new BMenuItem("3",dbgMessage3));
	mDbgMenu->SetRadioMode(true);
	mMenuBar->AddItem(mDbgMenu);
	mDbgMenu->SetTargetForItems(be_app);
	firstItem->SetMarked(true);
}
// -------------------------------------------------------------------
//	* Public Methods
// -------------------------------------------------------------------
void
WDebugWindow::MessageReceived(BMessage* theMesg)
{
	int32 indx = 0;
	entry_ref theRef;
	BMessage* refMesg = NULL;
	const char* mesgStr;
	
	switch(theMesg->what) {
		
		case 'DATA' :	// May be references...
			refMesg = new BMessage(B_REFS_RECEIVED);
			while(theMesg->FindRef("refs",indx++,&theRef) == B_OK) {
				refMesg->AddRef("refs",&theRef);
			}
			be_app->PostMessage(refMesg);
			delete refMesg;
			break;	
			
		case DEBUG_MESSAGE :
			while(theMesg->FindString(DEBUG_CONTENT,indx++,&mesgStr) == B_OK) {
				SingleLine(mesgStr);
			}
			break;
			
		default :
			BWindow::MessageReceived(theMesg);
	}
}
// -------------------------------------------------------------------
//	* Protected Methods
// -------------------------------------------------------------------
// -------------------------------------------------------------------
