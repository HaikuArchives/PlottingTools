/* ------------------------------------------------------------------ 

	Title: WGAxisEdit

	Description:  A view class for editing a graph axis

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		28 September, 1999
			Adapted for BeOS
		16 May 90
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87
			Last IBM Version 6.0
		?? Aug 80
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#ifndef WG_AxisEdit
#define WG_AxisEdit

#include "WGGraphAxis.h"
#include <InterfaceKit.h>

const char axis_label[8] = "WGAEla";	// String data

class WGAxisEdit : public BView {

	public:
	
	enum {
		class_ID = 'WGae'
	};

		WGAxisEdit(BRect frame,
				   const char* name,
				   int32 mode 		= B_FOLLOW_NONE,
				   int32 flags		= B_WILL_DRAW);	
	
	virtual
		~WGAxisEdit();
		
	BMessage*	// Data can be extracted from this class here
		Data();
			   
	void
		MessageReceived(BMessage* theMesg);
		
	void
		SetDefaultVals(const WGGraphAxis* theAxis);
		
	void
		SetLabel(const char* theLabel);
		
	void
		SetDisplay() const;
		
	protected:	// Methods
	
	virtual void
		AttachedToWindow();
	
	virtual void
		Draw(BRect frame);
		
	void
		Initialize(const char* name);
		
	void
		ShowError(EAxisError theErr);
		
	protected:	// Data
	WGGraphAxis*	mAxis;
	BTextControl*	mNameView;
	BTextControl*	mMaxVal;
	BTextControl*	mMinVal;
	BTextControl*	mInterval;
	BTextControl*	mDivisions;
	BMenuField*		mPrecision;
	BMenuField*		mScaleType;
	BMenuField*		mFormat;
	};
    
	#endif
