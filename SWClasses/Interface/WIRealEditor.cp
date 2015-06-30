/* ------------------------------------------------------------------ 

	Title: WIRealEditor

	Description:  A calss derived from BTextControl for editing
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
#include "WIRealEditor.h"

#include <stdio.h>
#include <SupportKit.h>

const char invalid_str[8]	= "*";

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WIRealEditor::WIRealEditor(BRect frame,
									const char* name,
									const char* label):BTextControl(frame,name,label,NULL,NULL)
{
	mValue = 0;
	mMaxVal = 1.0E10;
	mMinVal = -1.0E10;
	mDPs = 3;
	mFormat = realFmt_float;
	mValid = false;
	SetResult();
}
		
WIRealEditor::~WIRealEditor()
{
}
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
void
WIRealEditor::SetMinMax(float min, float max)
{
	if(max > min) {	// In case they were swapped
		mMinVal = min;
		mMaxVal = max;
	} else {
		mMinVal = max;
		mMaxVal = min;
	}
	
	if(mMaxVal == mMinVal) {	// Make sure there is a spread
		mMaxVal += 1.0;
	}
	
	CheckValue();
	SetResult();
}
// -------------------------------------------------------------------
void
WIRealEditor::SetValid(bool valid)
{
	mValid = valid;
	SetResult();
}
// -------------------------------------------------------------------
bool
WIRealEditor::SetValue(float value)
{
	mValue = value;
	bool valid = CheckValue();
	mValid = true;
	SetResult();
	return valid;
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
bool
WIRealEditor::CheckValue()
{
	if(mValue > mMaxVal) {
		mValue = mMaxVal;
		return false;
	}
	if(mValue < mMinVal) {
		mValue = mMinVal;
		return false;
	}
	return true;
}
// -------------------------------------------------------------------
void
WIRealEditor::ConvertText()
{
	::sscanf(Text(),"%f",&mValue);
	CheckValue();
	SetResult();
}
// -------------------------------------------------------------------
void
WIRealEditor::KeyDown(const char* bytes, int32 numBytes)
{
	BTextControl::KeyDown(bytes,numBytes);
}
// -------------------------------------------------------------------
void
WIRealEditor::MakeFocus(bool focus)
{
	if(focus == false) {	// Convert value
		ConvertText();
	}
	BTextControl::MakeFocus(focus);
}
// -------------------------------------------------------------------
void
WIRealEditor::SetResult()
{
	BAutolock lock(Window());
	
	char	dispStr[16];
	
	if(mValid) {
		::FormatReal(mValue,mFormat,mDPs,dispStr);
		SetText(dispStr);
	} else {
		SetText(invalid_str);
	}
}
// -------------------------------------------------------------------
