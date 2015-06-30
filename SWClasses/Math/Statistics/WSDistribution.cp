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
# include "WSDistribution.h"


// ------------------------------------------------------------------
//	* CONSTRUCTORS & DESTRUCTOR
// ------------------------------------------------------------------

WSDistribution::WSDistribution()
{
	mData = NULL;
	mPoints = 0;
	mValidDist = false;
}

WSDistribution::~WSDistribution()
{
}		
// ------------------------------------------------------------------
//	* PUBLIC METHODS
// ------------------------------------------------------------------
// Calculates the distribution data
void
WSDistribution::CalcDistribution()
{
	mValidDist = false;
	if(mPoints < 2) {
		return;
	}
	
	// Calculate the parametric stats
	mValidDist = true;
	float sumX = 0;
	float sumXX = 0;
	for(int32 n = 0; n < mPoints; n++) {
		sumX += mData[n].value;
		sumXX += mData[n].value*mData[n].value;
	}
	
	mMean = sumX/mPoints;
	mSD = sqrt((sumXX - sumX * sumX / mPoints)/(mPoints - 1));	
	
	// Find the median
	mMedianIndex = mPoints/2;
	mMedian = mData[mMedianIndex].value;
	if(((mPoints/2)*2) == mPoints) {	// If even # of points, interpolate
		mMedian = (mMedian + mData[mMedianIndex + 1].value)/2.0;
	}
}
// ------------------------------------------------------------------
// Calculates the percentile from the value
float
WSDistribution::PercentileFromValue(float value) const
{
	if(mPoints == 0) {
		return 0.0;
	}
	int32 index = FindIndex(value,true);
	return (float)index*100.0/(float)mPoints;
}
// ------------------------------------------------------------------
// Finds the number of points between two values
int32
WSDistribution::PointsBetween(float val1, float val2) const
{
	if(mPoints == 0) {
		return 0;
	}
	int32 pt1 = FindIndex(val1,true);
	int32 pt2 = FindIndex(val2,false);
	return pt2 - pt1;
}
// ------------------------------------------------------------------
void
WSDistribution::SetData(SDistData* theData, int32 numPoints)
{
	mData = theData;
	mPoints = numPoints;
	CalcDistribution();
}
// ------------------------------------------------------------------
// Finds the value closest to the given percentile 
float
WSDistribution::ValueAtPercentile(float percentile) const
{
	if(mPoints == 0) {
		return 0.0;
	}
	
	if(percentile <= 0.0) {
		return mData[0].value;
		
	} else if(percentile >= 100.0) {
		return mData[mPoints - 1].value;
		
	} else {
		int32 index = mPoints*(percentile/100.0);
		if(index == mPoints) index-- ;
		return mData[index].value;
	}
}
// ------------------------------------------------------------------
//	* Protected Methods
// ------------------------------------------------------------------
// Finds the index of the data point either less than or greater than 
// (or equal to) the value
int32
WSDistribution::FindIndex(float val, bool grEqual) const
{
	int32 index = mPoints/2;
	int32 move = index/2;
	while(1) {
		if(mData[index].value >= val) {
			index -= move;
			
		} else if(mData[index].value < val) {
			index += move;
		} 
		move /= 2;
		if(move == 0) {
			if(mData[index].value == val) {
				return index;
				
			} else if(grEqual) {
				return index + 1;
				
			} else if(index > 0) {
				return index - 1;
				
			}
			return index;
		}
	}
}
// ------------------------------------------------------------------
	
