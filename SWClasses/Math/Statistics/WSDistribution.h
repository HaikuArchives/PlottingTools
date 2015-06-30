// ------------------------------------------------------------------
/*
	Title:
		WSDistribution

	Description:
		Calculates the distribution statistics.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		19 April, 2000
			Adapted for BeOS
		02 June, 1995
			Adapted from M2 to MW C++
		28 Feb 91
			Adapted to Macintosh model
		23 Dec 88
			Last IBM version 4.0
		06 Aug 84
			First IBM version, adated from Apple-II
			version started in 1982.
*/
//---------------------------------------------------------------------
#ifndef _WSDistribution
#define _WSDistribution

#include <AppKit.h>
#include <string.h>

struct SDistData {
	float value;
	rgb_color statusColor;
};

class WSDistribution {

	public:
		
		WSDistribution();
	
	virtual
		~WSDistribution();
		
	void
		CalcDistribution();
	float
		Max() const {return mData[mPoints - 1].value;}
	float
		Min() const {return mData[0].value;}
	float
		MeanValue() const	{return mMean;}
	float
		MedianValue() const	{return mMedian;}
	float
		ModalValue() const {return mMode;}
	float
		PercentileFromValue(float value) const;
	int32
		Points() const {return mPoints;}
	int32
		PointsBetween(float val1, float val2) const;
	void
		SetData(SDistData* theData, int32 numPoints);
	void
		SetMode(float value) {mMode = value;}
	void
		SetTitle(const char* title) {strcpy(mTitle,title);}
	float
		StdDeviation() const {return mSD;}
	bool
		ValidDistribution() const {return mValidDist;}
	float
		ValueAtPercentile(float percentile) const;
	const char*
		Title() const {return (const char*)mTitle;}
	
	protected:	// Methods	
	int32
		FindIndex(float val, bool grEqual) const;
	
	protected: // Data	
	enum {
		max_title = 64
	};
	SDistData*	mData;
	char		mTitle[max_title];
	int32		mPoints;		// Number of valid points
	int32		mMedianIndex;	// Where median lies
	float		mMean;			// Mean value in set
	float		mSD;			// SD of data set
	float		mMedian;		// Median value of data set
	float 		mMode;			// Modal value
	bool		mValidDist;		// Is the distribution valid?
};

#endif

	