/* -------------------------------------------------------------------

	Title:
		URealFormat_Utils

	Description:
		Provides a number of REAL formatting choices.

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School Of Medicine
		462-CB  Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		17 Dec 93 - Adapted to MetroWerks C++
		27 Mar 91 - Adapted to Macintosh model
		06 Nov 85 - Last IBM version, 2.01
		05 Aug 84 - First IBM version, adapted from Apple-II
			        version started in 1982.
			        
			        
	The formats are:    
	                             
   general	- Generalized, flexible output format.                                                                 
   fixed		- Rounded to the specified decimal places.                           
   sciNot	- Scientific notation                                      
   sigFig	- Rounded to the specified significant figures                       
   float		- sigFig with trailing 0s ommitted.                        
   formal	- fixed, with commas                                        
                                                                     

------------------------------------------------------------------- */
#ifndef U_RealFormatUtils
#define U_RealFormatUtils

enum	ERealFormat	{
	realFmt_general,
	realFmt_fixed,
	realFmt_sciNot,
	realFmt_sigFig,
	realFmt_float,
	realFmt_formal
	};
	
// -------------------------------------------------------------------
// Format a real number as described above & return TRUE if valid
// Assumes adequate storage space
bool
FormatReal(const float realNum,
			  ERealFormat	format,
			  int				precision,
			  char*			output);
			  
// -------------------------------------------------------------------
// Returns the width of the part of the numerical string prior to the
// decimal point, or to the end if there is no decimal.  The width is
// in the currently set font and size for the currently active port
int
DecTabWidth(const char* realStr);

// -------------------------------------------------------------------

#endif
