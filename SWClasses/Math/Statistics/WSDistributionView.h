// ------------------------------------------------------------------
/*

	Title:
		WSDistributionView

	Description:
		Displays linear distession statistics.

	Author:
		Stephen C. Wardlaw, M.D.
		Medical R&D
		Highrock
		Lyme, CT 06371
		
	Edit History:
		21 September, 1999
			Adapted to BeOS from Mac C++
		22 May, 1995
			Adapted from M2 to MW C++
		19 Mar 92
			Compiled under MW M2 Vers 4.0
		28 Feb 91
			Adapted to Macintosh model
		23 Dec 88
			Last IBM version 4.0
		06 Aug 84
			First IBM version, adated from Apple-II
			version started in 1982.

*/
// ------------------------------------------------------------------

#ifndef _WSDistributionView
#define _WSDistributionView

#include "WSDistribution.h"
#include "URealFmt_Utils.h"
#include <InterfaceKit.h>
#include <string.h>

class WSDistributionView : public BView {

	public:
	
		WSDistributionView(const WSDistribution* theDist);
	virtual
		~WSDistributionView();
	virtual void
		GetPreferredSize(float* width, float* height);
	virtual void
		ResizeToPreferred();
	void	// The title of the data set
		SetTitle(const char* title) {::strncpy(mSetTitle,title,dist_setTitle);}
	void	// Display format for X-axis data
		SetDataFormat(ERealFormat format, int prec) {mFormat = format; mPrec = prec;}
	void	// Sets the title for the X-axis data
		SetLabel(const char* title) {::strncpy(mTitle,title,dist_titleMax);}
	const char*	// The title of the data set
		Title();
				
	protected:	// Methods
	
	void
		CalcDisplay();
	void
		Draw(BRect frame);
		
	protected: // Data
	
	enum {dist_titleMax = 15,
			dist_setTitle = 31};
	
	char			mTitle[dist_titleMax + 1];
	char			mSetTitle[dist_setTitle + 1];
	const WSDistribution*	mData;	// The data to be displayed
	BRect			mFrame;
	BFont			mFont;	// Current font info
	int				mPrec;
	float 			mWidMax;
	float			mWidth;
	float			mHeight;
	float			mCharHt;
	float			mCharDrop;
	float			mWordSpc;
	float			mEndSpc;
	float			mLineHt;
	float			mMeanLen;
	float			mSDLen;
	ERealFormat		mFormat;
	};
		
#endif

	
