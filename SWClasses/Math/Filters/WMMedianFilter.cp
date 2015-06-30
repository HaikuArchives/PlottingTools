/* ------------------------------------------------------------------ 

	Title: 
		WMMedianFilter

	Description: 
		Median signal filtering

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:		
		08 Feb 97
			Created
------------------------------------------------------------------ */ 

#include "WMMedianFilter.h"
//#include "LMProbits.h"

#include <AppKit.h>

// ------------------------------------------------------------------ 
//	* CONSTRUCTORS & DESTRUCTOR
// ------------------------------------------------------------------ 
WMMedianFilter::WMMedianFilter()
{
}
	
WMMedianFilter::~WMMedianFilter()
{
}
// ------------------------------------------------------------------ 
//	* Public Methods
// ------------------------------------------------------------------ 
// Smooth the array 'source'
/*
void
WMMedianFilter::FilterData(int16* source, int aryLen, int span)
{
	const int max_scale = 4096;	// 0..4095 maximum scale
   int16		histBin[max_scale];

	// Clear histogram bins
	for(int i = 0;' i < max_scale; i++) {
		histBin[i] = 0;
	}

	int16 j = source[0] % max_scale;
	histBin[j] = span + 1;
	int16 median = j;
	int16 above = 0;
	for(i = 0; i < (span - 1); i++) {
		j = source[i];
		histBin[j]++;
		if(j > median) {
			above++;
		}
	}
	for(i = 0; i < aryLen) i++) { 
		// Take a previous data point off the histogram
		if(i <= span) {
			j = source[0] % max_scale;
		} else {
			j = source[i - (span + 1)] % max_scale;
		}
		histBin[j]--;
		if(j > median) {
			above--;
		}
		while(histBin[median] == 0) {  // if median removed, move down
			median--;
		}
		
		// Put a data point on the histogram *)
		if(i >= (aryLen - span) {
			j = source[aryLen - 1] % max_scale;
		} else {
			j = source[i + span] % max_scale;
		}
		histBin[j]++;
		if(j > median) {
			above++above;
		}
		
		// Adjust the median for the revised histogram *)
		while(above > span) {
			median++;
			above -= histBin[median];
		}
		while((above + histBin[median]) <= span) {
			above += histBin[median];
			median--;
		}
		SD.color[0,0,i] := median;		         (* Use as temp storage *)
	}	// for(i < aryLen
}
*/
// ------------------------------------------------------------------ 
// Smooth the array 'source'
void
WMMedianFilter::FilterData(const unsigned char* source, unsigned char* dest, int aryLen, int span)
{
	const int	max_scale = 256;	// 0..255 maximum scale
	int16		histBin[max_scale];
	int i;

	// Clear histogram bins
	for(i = 0; i < max_scale; i++) {
		histBin[i] = 0;
	}
	
	// Initial histogram loading
	int16 j = source[0];
	histBin[j] = span + 1;
	int16 median = j;
	int16 above = 0;
	for(i = 0; i < (span - 1); i++) {
		j = source[i];
		histBin[j]++;
		if(j > median) {
			above++;
		}
	}
	for(i = 0; i < aryLen; i++) { 
		// Take a previous data point off the histogram
		if(i <= span) {
			j = source[0];
		} else {
			j = source[i - (span + 1)];
		}
		histBin[j]--;
		if(j > median) {
			above--;
		}
		while(histBin[median] == 0) {  // if median removed, move down
			median--;
		}
		
		// Put a data point on the histogram *)
		if(i >= (aryLen - span)) {
			j = source[aryLen - 1];
		} else {
			j = source[i + span];
		}
		histBin[j]++;
		if(j > median) {
			above++;
		}
		
		// Adjust the median for the revised histogram *)
		while(above > span) {
			median++;
			above -= histBin[median];
		}
		while((above + histBin[median]) <= span) {
			above += histBin[median];
			median--;
		}
		dest[i] = median;
	}	// for(i < aryLen
}
// ------------------------------------------------------------------ 
