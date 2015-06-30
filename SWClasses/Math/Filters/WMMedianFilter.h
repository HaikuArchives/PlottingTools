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
#ifndef _WMMedianFilter
#define _WMMedianFilter

class WMMedianFilter {

	public:
		WMMedianFilter();
	
	virtual
		~WMMedianFilter();
		
		// This filters the data to the median of the value over the
		// range 'span'.
		
		
//	void	// NOTE that the maximum value for any data point is 4095
//		FilterData(int16* dArray, int aryLen, int span);
		
	void	
		FilterData(const unsigned char* source, unsigned char* dest, int aryLen, int span);
		
		protected:	// Methods
		
		
	protected:	// Data
	
	};
	
#endif

