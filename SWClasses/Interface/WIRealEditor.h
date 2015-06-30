/* ------------------------------------------------------------------ 

	Title: WIRealEditor

	Description:  A clsss derived from BTextControl for editing
		real numbers

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		17 December, 1999
			Created
		
------------------------------------------------------------------ */ 
#ifndef _WIRealEditor
#define _WIRealEditor

#include "URealFmt_Utils.h"
#include <AppKit.h>
#include <InterfaceKit.h>

class WIRealEditor : public BTextControl {

	public:
	
		WIRealEditor(BRect frame,
						const char* name,
						const char* label);
		
	virtual
		~WIRealEditor();
		
	void
		SetMinMax(float min, float max);
		
	void
		SetValid(bool valid);
		
	bool
		SetValue(float value);
		
	bool
		Valid() {return mValid;}
		
	float 
		Value() {return mValue;}
		
	protected :	// Methods
	
	bool
		CheckValue();
		
	void
		ConvertText();
		
	virtual void
		KeyDown(const char* bytes, int32 numBytes);
		
	virtual void
		MakeFocus(bool focus);
		
	void
		SetResult();
		
	protected :	// Data
	
	float			mValue;
	float			mMaxVal;
	float			mMinVal;
	int16			mDPs;
	ERealFormat	mFormat;
	bool			mValid;
		
};

#endif
