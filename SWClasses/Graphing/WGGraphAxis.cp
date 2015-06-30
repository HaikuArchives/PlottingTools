/* ------------------------------------------------------------------ 

	Title: WGGraphAxis

	Description:  A class to be used in constructing graph axis.
		It provides base operations for the abscissa & ordinate.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		18 March, 1998
			Removed all structs to make it "endian-safe"
		09 Dec 97
			Adapted for BeOS
		09 Feb 94
			Split again from 'Abscissa' & 'Ordinate'
		24 Jan 94
			Split from the old Module Graph in MW M-2 and
			re-written for MetroWerks C++
		16 May 90
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87
			Last IBM Version 6.0
		?? Aug 80
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#include "WGGraphAxis.h"
#include "UFloating_Util.h"

// Constants for data archiving
const char max_val_name[8] = "maxval";
const char min_val_name[8] = "minval";
const char interval_name[8] = "intrval";
const char logbase_name[8] = "logbase";
const char divs_name[8] = "numdivs";
const char prec_name[8] = "numprec";
const char scale_name[8] = "scalety";
const char format_name[8] = "numfmt";

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGGraphAxis::WGGraphAxis()
{
	SetDefaults();
}
// -------------------------------------------------------------------
WGGraphAxis::WGGraphAxis(const WGGraphAxis &theAxis)
{
	mMax = theAxis.mMax;
	mMin = theAxis.mMin;
	mInterval = theAxis.mInterval;
	mDivs = theAxis.mDivs;
	mScale = theAxis.mScale;
	mNumFmt = theAxis.mNumFmt;
	mNumDPs = theAxis.mNumDPs;
	mLogBase = theAxis.mLogBase;
}
// -------------------------------------------------------------------
// *  Public Methods             
// -------------------------------------------------------------------
status_t
WGGraphAxis::Archive(BMessage* theMesg)
{
	theMesg->AddString("class","WGGraphAxis");
	theMesg->AddFloat(max_val_name,mMax);
	theMesg->AddFloat(min_val_name,mMin);
	theMesg->AddFloat(interval_name,mInterval);
	theMesg->AddFloat(logbase_name,mLogBase);
	theMesg->AddInt16(divs_name,mDivs);
	theMesg->AddInt16(prec_name,mNumDPs);
	theMesg->AddInt16(scale_name,mScale);
	theMesg->AddInt16(format_name,mNumFmt);
	return B_OK;
}
// -------------------------------------------------------------------
EAxisError
WGGraphAxis::SetInterval(float interval)
{
	mInterval = interval;
	return TestValues();
}
// -------------------------------------------------------------------
EAxisError
WGGraphAxis::SetLabelFormat(ERealFormat format, int16 numDPs)
{
	mNumFmt = format;
	mNumDPs = numDPs;
	return axErr_noErr;
}
// -------------------------------------------------------------------
EAxisError
WGGraphAxis::SetMinorDivisions(int16 divs)
{
	if(divs >= 1 && divs <= 9) {
		mDivs = divs;
		return axErr_noErr;
	} else {
		return axErr_interval;
	}
}
// -------------------------------------------------------------------
EAxisError
WGGraphAxis::SetScale(EAxisScale newScale)
{
	if(newScale >= scale_linear && newScale <= scale_log10) {
		mScale = newScale;
		return TestValues();
	} else {
		return axErr_badScale;
	}
}
// -------------------------------------------------------------------
EAxisError
WGGraphAxis::SetSpan(float min, float max)
{
	// Make sure max > min
	if(max > min) {
		mMax = max;
		mMin = min;
	} else {
		mMax = min;
		mMin = max;
	}
	return TestValues();
}
// -------------------------------------------------------------------
EAxisError
WGGraphAxis::SetValues(BMessage* theMesg)
{
	const char* tStr;
	tStr = theMesg->FindString("class");
	if(tStr != NULL && ::strcmp(tStr,"WGGraphAxis") == 0) {
		theMesg->FindFloat(max_val_name,&mMax);
		theMesg->FindFloat(min_val_name,&mMin);
		theMesg->FindFloat(interval_name,&mInterval);
		theMesg->FindFloat(logbase_name,&mLogBase);
		theMesg->FindInt16(divs_name,&mDivs);
		theMesg->FindInt16(prec_name,&mNumDPs);
		int16 iVal;
		theMesg->FindInt16(scale_name,&iVal);
		mScale = (EAxisScale)iVal;
		theMesg->FindInt16(format_name,&iVal);
		mNumFmt = (ERealFormat)iVal;
		return TestValues();
	}
	return axErr_badScale;
}
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
// This method sets the axis and checks for errors. If errors are found,
//	it corrects them and returns false.
EAxisError
WGGraphAxis::TestValues()
{
	if(mScale != scale_linear && mMin <= 0.0) {
		// Axis Minimum Must Be > 0 With Log Scale
		mMin = 1;
		if(mMax <= mMin) {
			mMax = mMin + 1.0;
		}
		return axErr_logZero;
		}
		
	switch (mScale) {
		case scale_log2:
			mLogBase = 2.0;
			mDivs = 2;
			break;
		
		case scale_log10:
			mLogBase = 10;
			mDivs = 9;
			break;
		
		default :
			;// No default state
	}
	
	// Ensure max > min
	if(mMax == mMin) {
		mMax = mMin + 1.0;
		return axErr_span;
	}
	
	// Check for too small a tic interval
	if(mInterval <= 0.0 || ((mMax - mMin)/mInterval) > 20.0) {
		// Tic Interval Too Small;
		mInterval = (mMax - mMin)/20.0;
		return axErr_interval;
	}
	
	return axErr_noErr;
}
// -------------------------------------------------------------------
// Transforms numbers from graph coord #s to orig input values
float
WGGraphAxis::XformFrom(float num) 
{
	mMathErr = 0;
	switch (mScale) {
	
		case scale_log2:
		case scale_log10:
			if(num < log(FLT_MAX)) {
				return Power(mLogBase,num);
			} else {
				mMathErr = -1;
				return FLT_MAX;
			}
			
		default:
			return num;
	}
}	
// -------------------------------------------------------------------
// Transforms numbers from input values to the graph coord #s
float
WGGraphAxis::XformTo(float num) 
{
	mMathErr = 0;
	
	switch (mScale) {
	
		case scale_log2:
		case scale_log10:
			if(num > 0.0) {
				return LogOfBase(num,mLogBase);
			} else {
				mMathErr = -1;
				return min_num;
			}

		default:
			return num;
	}
}	
// -------------------------------------------------------------------
// *  Protected Methods             
// -------------------------------------------------------------------
void
WGGraphAxis::SetDefaults()
{
	mMax = 100.0;
	mMin = 0.0;
	mInterval = 10.0;
	mDivs = 2;
	mScale = scale_linear;
	mNumFmt = realFmt_float;
	mNumDPs = 3;
	mLogBase = 10;
}
// -------------------------------------------------------------------
