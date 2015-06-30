/* ------------------------------------------------------------------ 

	Title: WMFunctBase

	Description:  A abstract class which supports some very basic
		math functions.

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		03 Dec 97
			
		22 Mar 94
			Developed as a super-class of 'LMPolyFit' and others
		
------------------------------------------------------------------ */ 

#ifndef WM_FunctBase
#define WM_FunctBase

// This is the basic data structure for loading data into the
// various fit routines that derive from this class.  Data from
// the calling procedure, in this form, is extracted by the class
// and used internally if 'use' is true, otherwise that data point
// is not used. 


struct DataSet {
	float x,y;			// Data coordinates
	bool use;		// Use this data point?
	};
	
// The data fit set incorporates 'sig', which is a measure of the
// significance of a particular data point. This may be used in weighted data sets
struct DataFitSet {
	DataSet	data;
	float	sig;
};


class WMFunctBase {

	public:
	
		WMFunctBase();
	
	// Used to transform                                                                                 	
	virtual float
		CalcYFromX(float xVal);
		
	// Reports on any math errors
	virtual bool
		MathError() {return mMathErr != 0;}
	
	// Transform a number via the function represented by the derived class	
	virtual float
		XformTo(float val);
	
	// Take a transformed number and convert it back	
	virtual float
		XformFrom(float val);
	
	public : // Data
		
	int	mMathErr;
		
};

#endif
