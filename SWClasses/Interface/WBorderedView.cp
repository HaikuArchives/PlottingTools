/* ------------------------------------------------------------------ 

	Title: WBorderedView

	Description:  
		A view which has a 3D border
	
	Author :
		Stephen Wardlaw, M.D.
		Medical Reserch & Development
		Highrock
		Lyme, Ct 06371
		<swardlaw@connix.com>

	Edit History:	
		08 September, 1999
			Created		
------------------------------------------------------------------ */ 
#include "WBorderedView.h"

const int16	def_width = 7;

// -------------------------------------------------------------------
//	* CONSTRUCTOR
// -------------------------------------------------------------------
WBorderedView::WBorderedView(BRect frame, 
					  const char *name, 
					  uint32 resizingMode, 
					  uint32 flags):BView(frame,name,resizingMode,flags)
{
	mWidth = def_width;
	mBType = border_round;
	mColors = NULL;
	mMaxColor = 255;
	mMinColor = 0;
	mInitialized = false;
}					  
WBorderedView::~WBorderedView()
{
	delete mColors;
}		
// -------------------------------------------------------------------
//	* Public Methods
// -------------------------------------------------------------------
void
WBorderedView::Draw(BRect rFrame)
{
	if(!mInitialized) {
		SetUpBorder();
	}
	BRect frame = Bounds();
	frame.left += 7;
	frame.bottom -= 7;
	rgb_color gray = {209,209,209};
	for(int n = 0; n < 6; n++) {
		SetHighColor(gray);
		StrokeRect(frame);
		frame.OffsetBy(-1,1);
		if(n < 2) {
			gray.red += 8;
			gray.green += 8;
			gray.blue += 8;
		} else if(n < 4) {
			gray.red -= 8;
			gray.green -= 8;
			gray.blue -= 8;
		}
	}	
	SetHighColor(0,0,0);
}		
// -------------------------------------------------------------------
void
WBorderedView::SetBorderWidth(int16 width)
{
}		
// -------------------------------------------------------------------
void
WBorderedView::SetBorderGradient(unsigned char min, unsigned char max)
{
}
// -------------------------------------------------------------------
//	* Protected Methods
// -------------------------------------------------------------------
void
WBorderedView::SetUpBorder()
{
	mInitialized = true;
}
// -------------------------------------------------------------------
		
