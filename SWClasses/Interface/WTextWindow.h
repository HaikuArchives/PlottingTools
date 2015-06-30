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

#ifndef _WTextWindow
#define _WTextWindow

#include <InterfaceKit.h>

class WTextWindow : public BWindow {

	public:
		WTextWindow(BRect 			frame,
					const char*		name	= "TWindow",
					window_type		type	= B_DOCUMENT_WINDOW,
					uint32 			flags	= B_NOT_CLOSABLE);
					    
	void	// Adds wrapping text without line feeds
		Append(const char* thetext);
		
	void
		ClearText();
		
	int32
		MaxLines() {return mMaxLines;}
		
	void
		NewLine();
		
	void
		SetMaxLines(int32 maxLines) {mMaxLines = maxLines;}
		
	void	// Shows the error code
		ShowError(int32 error);
		
	void	// Writes a single line with a CR
		SingleLine(const char* thetext);

	protected :	// Methods
	
	void
		CheckLength();
	
	bool
		QuitRequested();
	
	protected :	// Data
	
	void
		FrameResized(float width, float height);
		
	BTextView*	mTextView;
	BMenu*		mFileMenu;
	BMenuBar*	mMenuBar;
	int32		mMaxLines;
};
#endif

