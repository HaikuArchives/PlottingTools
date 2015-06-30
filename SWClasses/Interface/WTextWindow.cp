/* ------------------------------------------------------------------ 

	Title: WTextWindow

	Description:  
		A window class for showing string data.
		
	Author :
		Stephen Wardlaw, M.D.
		Medical Reserch & Development
		Highrock
		Lyme, Ct 06371
		<swardlaw@connix.com>

	Edit History:	
		08 September, 1999
			Modified, with debug menu split out
		18 February, 1998
			Created
		
------------------------------------------------------------------ */ 

#include "WTextWindow.h"

#include <stdio.h>
#include <AppKit.h>

const int32 def_max_lines = 128;	// Default max lines in window

// -------------------------------------------------------------------
//	* CONSTRUCTOR
// -------------------------------------------------------------------
WTextWindow::WTextWindow(BRect 			frame,
						 const char*	name,
						 window_type 	type,
						 uint32 flags):BWindow(frame,name,type,flags)
{
	// Add the main menu bar so that we can at least quit. Note that
	// the menu bar is not actually visible..
	BRect mFrame(0,0,0,0);
	mMenuBar = new BMenuBar(mFrame,"MB");
	mFileMenu = new BMenu("File");
	mFileMenu->AddItem(new BMenuItem("Quit",new BMessage(B_QUIT_REQUESTED),'Q'));
	mMenuBar->AddItem(mFileMenu);
	AddChild(mMenuBar);
	
	// Add the text view
	
	BRect textFrame = Bounds();
	textFrame.top = mMenuBar->Bounds().bottom + 1.0;
	textFrame.right -= B_V_SCROLL_BAR_WIDTH;
	BRect textRect = textFrame;
	textRect.OffsetTo(B_ORIGIN);
	BRect r = Bounds();
	r.InsetBy(3.0,3.0);
	mTextView = new BTextView(textFrame, "text_view", textRect,
				B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_PULSE_NEEDED);
	AddChild(new BScrollView("scroll_view", mTextView,
				B_FOLLOW_ALL_SIDES, 0, false, true, B_NO_BORDER));
	mTextView->MakeSelectable(false);
	mTextView->MakeEditable(false);
	//mTextView->SetDoesUndo(true);
	//mTextView->MakeFocus(true);
	mMaxLines = def_max_lines;
}
// -------------------------------------------------------------------
//	* Public Methods
// -------------------------------------------------------------------
void
WTextWindow::Append(const char* theText)
{
	Lock();
	mTextView->Select(mTextView->TextLength(),mTextView->TextLength());
	mTextView->Insert(theText);
	CheckLength();
	mTextView->ScrollToSelection();
	Unlock();
}
// -------------------------------------------------------------------
void
WTextWindow::ClearText()
{
	bool selectable = mTextView->IsSelectable();	// Save old state
	mTextView->MakeSelectable(true);
	mTextView->SelectAll();
	mTextView->Clear();
	mTextView->MakeSelectable(selectable);
}
// -------------------------------------------------------------------
void
WTextWindow::NewLine()
{
	Lock();
	// Add a new line
	char* crlf = {"\n"};
	mTextView->Insert(crlf);
	Unlock();
}
// -------------------------------------------------------------------
void
WTextWindow::ShowError(int32 error)
{
	char errStr[64];
	
	sprintf(errStr,"Error Code = %ld",error);
	SingleLine(errStr);
	CheckLength();
}
// -------------------------------------------------------------------
void		// Writes a single line with a CR
WTextWindow::SingleLine(const char* theText)
{
	Append(theText);
	NewLine();
}
// -------------------------------------------------------------------
//	* Protected Methods
// -------------------------------------------------------------------
void
WTextWindow::CheckLength()
{
	if(mTextView->CountLines() > mMaxLines) {
		mTextView->Delete(0,mTextView->OffsetAt(mMaxLines/4));
		mTextView->Select(mTextView->TextLength(),mTextView->TextLength());
	}
}
// -------------------------------------------------------------------
void
WTextWindow::FrameResized(float width, float height) 
{
	BRect textRect = mTextView->TextRect();
	
	textRect.right = textRect.left + (width - B_V_SCROLL_BAR_WIDTH - 3.0);
	mTextView->SetTextRect(textRect);
}
// -------------------------------------------------------------------
bool 
WTextWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}
// -------------------------------------------------------------------
