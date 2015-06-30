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

#ifndef _WColorControl
#define _WColorControl

#include <InterfaceKit.h>

// Pre-defined colors 
const rgb_color color_white 	= {255,255,255};
const rgb_color color_black 	= {0,0,0};
const rgb_color color_std_gray	= {216,216,216};	// Be standard gray
const rgb_color color_gray18	= {209,209,209};	// 18% gray
const rgb_color color_med_gray	= {128,128,128};	// 50% gray
const rgb_color color_red 		= {255,0,0};
const rgb_color color_green 	= {0,255,0};
const rgb_color color_blue 		= {0,0,255};
const rgb_color color_yellow 	= {255,255,0};
const rgb_color color_cyan		= {0,255,255};
const rgb_color color_magenta	= {255,0,255};


class WColorControl : public BControl {

	public :
		
		WColorControl(BRect frame, const char* name = "wcolor",
					const char* label = "Select Color:",
					BMessage* theMesg = NULL);
					
		virtual
			~WColorControl();
			
		rgb_color
			ColorValue();
			
		void
			ResizeToPreferred();
			
		void
			SetDefaultColor(rgb_color color);
			
		protected :
		
		void
			Draw(BRect frame);
		
		int16
			InRect(BPoint thePt);
		
		void
			MouseDown(BPoint thePt);
			
		void
			MouseMoved(BPoint thePt, uint32 transit, const BMessage* message);
			
		void
			MouseUp(BPoint thePt);
			
			
			
		protected:	// Data
		
		BRect	mColorRect[9];
		BRect	mCtrlRect;
		BPoint	mFPoint;
		int16	mRectHit;
		bool	mTracking;
		bool	mHilited;
		bool	mResized;
};

#endif

