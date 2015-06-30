/* ------------------------------------------------------------------ 

	Title: WGAbscissa

	Description:  A class to be used in constructing graphs.  One or
		more of these can be used as an X-Axis referent.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		09 Dec 97
			Adapted for BeOS
		24 Jan 94
			Split from the old Module Graph in MW M-2 and
			Re-written for MetroWerks C++
		16 May 90	
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87 
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#ifndef WG_Abscissa
#define WG_Abscissa

#include "WGGraphAxis.h"

#include <InterfaceKit.h>

class WGAbscissa : public WGGraphAxis, public BView {

	public:
	
		// NOTE that these views will not automatically draw!
		// The parent view is responsible for drawing them
		
		WGAbscissa(BRect frame, 
					  char* label = "XAxis",
					  uint32 resizeMode = B_FOLLOW_NONE,
					  uint32 flags = B_FULL_UPDATE_ON_RESIZE|B_WILL_DRAW);
	void
		Draw(BRect rect);	
	void
		GetXAxisPosition(float &origin, float &end) const {origin = mOriginX; end = mEndX;}
	void
		GetXMargins(float &leftMargin, float &rightMargin, float &height);
	void
		MoveXOriginTo(BPoint pt);
	void
		SizeAxisTo(float length);
	bool
		ValToXPosn(float val, float &posn);
	bool
		XPosnToVal(float posn, float &val);
		
	protected:
	
	void
		LabelTic(float posn, float num);
	float
		MakeLabel(float num, char* temp);
		
		
	protected:
	float 		mXformedMin;		// Minimum axis values (Xformed)
	float		mSpanFactX;			// Span factor for calculating point posns
	float			mLeftMargin;		// Distance from origin to left
	float			mRightMargin;		// Distance from max end to right
	float			mOriginX;			// Where the x-axis begins
	float			mEndX;				// Where the x-axis ends
	float 		mTicWidthX;			// Tic width for x-axis
	float			mLimitRight;		// Horizontal spacing limit to right
	float 		mLimitLeft;			// Horizontal spacing limit to left
	float			mLabelMargin;		// Allowable label spacing
	bool			mTicsLabeled;		// Are teh tics labelled?
	bool			mTicsBelow;			// Are tics below the axis?
	
	// Data required for drawing within view
	float			mTicBase;
	float			mTicLen;
	float			mLabelPosn;			// Vert placement of axis tic label
	};
    
#endif
