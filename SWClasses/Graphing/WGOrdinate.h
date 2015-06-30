/* ------------------------------------------------------------------ 

	Title: WGOrdinate

	Description:  A class to be used in constructing graphs.  One or
		more of these can be used as a Y-Axis referent.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		09 Dec 97
			Adapted for BeOS
		04 Feb 94
			Split from the old Module Graph in MW M-2 and
			re-written for MetroWerks C++
		16 May 90
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87
			Last IBM Version 6.0
		?? Aug 80
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#ifndef WG_Ordinate
#define WG_Ordinate

#include "WGGraphAxis.h"
#include <InterfaceKit.h>

	
class WGOrdinate : public WGGraphAxis, public BView {

	public:
		// NOTE that these views will not automatically draw!
		// The parent view is responsible for drawing them
		
		WGOrdinate(BRect frame, 
					  char* label = "YAxis",
					  uint32 resizeMode = B_FOLLOW_NONE,
					  uint32 flags = B_FULL_UPDATE_ON_RESIZE|B_WILL_DRAW);
	void
		Draw(BRect rect);
	void
		GetYAxisPosition(float &origin, float &end) const {origin = mOriginY; end = mEndY;}
	void
		GetYMargins(float &topMargin, float &bottomMargin, float &width);
	void
		MoveYOriginTo(BPoint pt);		
	void
		SizeAxisTo(float length);
	bool
		ValToYPosn(float val, float &posn);
	bool
		YPosnToVal(float posn, float &val);
		
	protected:	// Methods
	void
		LabelTic(float posn, float num);
	float
		MakeLabel(float num, char* temp);
	
	protected:	// Data
	float 		mXformedMin;		// Minimum axis values (Xformed)
	float		mSpanFactY;			// Span factor for calculating point posns
	float			mTopMargin;			// Distance from top to max end
	float			mBottomMargin;		// Distance from bottom to min end
	float			mOriginY;			// Where the x-axis begins
	float			mEndY;				// Where the x-axis ends
	float 		mTicWidthY;			// Tic width for x-axis
	float			mLimitTop;			// Vertical spacing limit to top
	float 		mLimitBottom;		// Vertical spacing limit to bottom
	float			mLabelMargin;		// Allowable label spacing
	float			mFontAdj;			// Adjustment for vert centering of font
	bool			mTicsLabeled;		// Are teh tics labelled?
	bool			mTicsLeft;			// Tics to left of axis border?

	// Data required for drawing within view
	float			mTicBase;
	float			mTicLen;
	float			mLabelPosn;			// Vert placement of axis tic label
	};
    
    #endif
