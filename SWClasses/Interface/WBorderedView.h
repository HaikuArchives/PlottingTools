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

#ifndef _WBorderedView
#define _WBorderedView

#include <InterfaceKit.h>

class WBorderedView : public BView {

	public :
	
	enum EBorderType{
		border_rect,
		border_round,
		max_width = 21
	};
	
		WBorderedView(BRect frame, 
					  const char *name, 
					  uint32 resizingMode, 
					  uint32 flags );
					  
	virtual
		~WBorderedView();
		
	EBorderType
		BorderType() {return mBType;}
		
	int16
		BorderWidth() {return mWidth;}
		
	virtual void
		Draw(BRect frame);
		
	BRect
		InnerFrame() {return mInnerFrame;}
		
	void
		SetBorderType(EBorderType border) {mBType = border;}
		
	void
		SetBorderWidth(int16 width);
		
	void
		SetBorderGradient(unsigned char min, unsigned char max);

	protected :	// Methods
	
	void
		SetUpBorder();
		
	protected :	// Data
	
	BRect			mInnerFrame;
	int16			mWidth;
	EBorderType		mBType;
	rgb_color*		mColors;
	unsigned char	mMaxColor;
	unsigned char	mMinColor;
	bool			mInitialized;
};
#endif;
