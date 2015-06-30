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

#ifndef WG_GraphAxis
#define WG_GraphAxis

#include "WMFunctBase.h"
#include "URealFmt_Utils.h"
#include <AppKit.h>

const float	border_wd		= 2;	// Width for drawing border & major tics
const float	min_num			= 1E-10;// Minimum allowable number
const float	rounding_err	= 1 + 1.0E-5;	// Label error
const float	tic_len			= 9;	// Length of a graph tic
const float tic_space		= 3;	// Space between tic and character

//The types of scales available
enum EAxisScale {
	scale_linear,
	scale_log2,
	scale_log10
	};
	
// Errors in constructing axes
enum EAxisError {
	axErr_noErr,
	axErr_span,
	axErr_logZero,
	axErr_badScale,
	axErr_interval
};

	
class WGGraphAxis : public WMFunctBase {

	public:
	
	enum {	// Message type for archive
		class_ID = 'WGax'
	};
	
// Default constructor	
		WGGraphAxis();	
		
// Copy constructor		
		WGGraphAxis(const WGGraphAxis &theAxis);
		
	status_t
		Archive(BMessage* theMesg);
		
	int16
		Divisions() const {return mDivs;}
		
	float
		Interval() const {return mInterval;}
		
	float
		Max() const {return mMax;}
	
	float
		Min() const {return mMin;}
		
	ERealFormat
		NumberFormat() const {return mNumFmt;}
		
	int16
		NumberPrecision() const {return mNumDPs;}
		
	EAxisScale
		Scale() const {return mScale;}
			
	/*
		The following 'Set' methods try to set the axis parameters.
		If a value is out of range, or if there is a conflict amongst
		the settings, the method sets values to a non-destructive
		setting and returns the appropriate error message.
	*/
	EAxisError
		SetInterval(float interval);
		
	EAxisError
		SetLabelFormat(ERealFormat format, int16 numDPs);
		
	EAxisError
		SetMinorDivisions(int16 divs);
		
	EAxisError
		SetScale(EAxisScale newScale);
		
	EAxisError
		SetSpan(float min, float max);
		
	EAxisError	// Sets all axis values from this message
		SetValues(BMessage* theMesg);
		
	virtual float	// Axis->data transform
		XformFrom(float num);
		
	virtual float	// Data->axis transform
		XformTo(float num);
		
	protected:	// Methods
	
	void
		SetDefaults();
		
	EAxisError
		TestValues();
	
	protected:	// Data
	
	float			mMax;		// Axis limits
	float			mMin;	
	float			mInterval;	// Interval for labelling & major tics
	float			mLogBase;	// The base used for the log scale
	int16			mDivs;		// Minor axis divisions
	int16			mNumDPs;	// # of DPs/SigFigs
	EAxisScale		mScale;		// Scale type
	ERealFormat		mNumFmt;	// Axis marking format
	};

#endif
