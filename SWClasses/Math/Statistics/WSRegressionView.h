// ------------------------------------------------------------------
/*

	Title:
		WSRegressionView

	Description:
		Displays linear regression statistics.

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

#ifndef _WSRegressionView
#define _WSRegressionView

#include "WSLinearRegr.h"
#include "URealFmt_Utils.h"
#include <InterfaceKit.h>
#include <string.h>

class WSRegressionView : public BView {

	public:
	
		WSRegressionView(const WSLinearRegr* theRegr);
	virtual
		~WSRegressionView();
	virtual void
		GetPreferredSize(float* width, float* height);
	virtual void
		ResizeToPreferred();
	void	// The title of the data set
		SetTitle(const char* title) {::strncpy(mSetTitle,title,regr_setTitle);}
	void	// Display format for X-axis data
		SetXDataFormat(ERealFormat format, int prec) {mXFormat = format; mXPrec = prec;}
	void	// Sets the title for the X-axis data
		SetXLabel(const char* title) {::strncpy(mXTitle,title,regr_titleMax);}
	void	// Display format for Y-axis data
		SetYDataFormat(ERealFormat format, int prec) {mYFormat = format; mYPrec = prec;}
	void	// Sets the title for the Y-axis data
		SetYLabel(const char* title) {::strncpy(mYTitle,title,regr_titleMax);}
	const char*	// The title of the data set
		Title();
				
	protected:	// Methods
	
	void
		CalcDisplay();
	void
		Draw(BRect frame);
		
	protected: // Data
	
	enum {regr_titleMax = 15,
			regr_setTitle = 31};
	
	char			mXTitle[regr_titleMax + 1];
	char			mYTitle[regr_titleMax + 1];
	char			mSetTitle[regr_setTitle + 1];
	const WSLinearRegr*	mData;	// The data to be displayed
	BRect			mFrame;
	BFont			mFont;	// Current font info
	int				mXPrec;
	int				mYPrec;
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
	ERealFormat		mXFormat;
	ERealFormat		mYFormat;
	};
		
#endif

	
