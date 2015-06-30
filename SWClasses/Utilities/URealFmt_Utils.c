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

------------------------------------------------------------------- */

#include "URealFmt_Utils.h"
#include "UStringFmt_Utils.h"

#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
	

// -------------------------------------------------------------------
// Format a REAL number into the output string
bool
FormatReal(const float realValue,
		   ERealFormat	format,
		   int			reqPrecision,
		   char*		output)

{		   	
	const int		max_len = 31;
	bool				neg;
	int				n,m,k,indx1,indx2,dp,expn,absExpn,precision,dpPlace;
	char			localStr1[max_len + 1];
	char			localStr2[max_len + 1];
	const float 	min_Num = 1.0E-9;  // Smallest number to represent	
	const float 	max_Num = 1.0E+9;  // Largest formatted number
	float 			realNum = realValue;       
    
	if(fabs(realNum) > max_Num) {  // Exit here if too large
		strcpy(output,"> Range");
		return false;
	}     
	if(realNum != 0.0 && fabs(realNum) < min_Num) {  // Exit here if too small
		strcpy(output,"< Range");
		return false;
	}     
    // Set general boundary
	if(reqPrecision > 9) {
		reqPrecision = 9;
	}
	precision = reqPrecision;
    	
	switch (format) {
    
		case realFmt_general:
			return sprintf(output,"%.*G",precision,realNum) > 0;
			break;
			
		case realFmt_fixed :
    		return sprintf(output,"%.*f",precision,realNum) > 0;
			break;
			
		case realFmt_sciNot :
    		return sprintf(output,"%.*E",precision,realNum) > 0;
			break;
			
		case realFmt_formal :
			sprintf(output,"%.*f",precision,realNum);
			
   		if(fabs(realNum) >= 1000) {          // Add commas if >= 1000
   			dp = 0;	// Find DP posn
      		while(dp < max_len && localStr1[dp] && localStr1[dp] != '.') {
   				 dp++;
   			}
			neg = realNum < 0; // If a negative number;
      		if(neg) {
      			localStr2[0] = '-';
      			dp--;
      		}
	      		// Insert commas where required, ignore first instance	
				for(n = m = 0;dp;dp--,n++,m++) {
					if(n && !(dp % 3)) {
						localStr2[neg + m] = ',';
						m++;
					}
					localStr2[neg + m] = localStr1[neg + n];
				}	// for n = m
				for(neg + n; localStr1[neg + n]; m++,n++) {
					localStr2[neg + m] = localStr1[neg + n];
				}
      		localStr2[neg + m] = 0;
				strcpy(output,localStr2);
				} else {
					strcpy(output,localStr1);
				}
				return true;
				break;
			
		case realFmt_sigFig :
		case realFmt_float :
			neg = realNum < 0.0;
			if(neg) {
				realNum = realNum * -1.0;
			}
			if(realNum > max_Num) {  // Exit here if too large
				strcpy(output,">Range");
				return false;
			}     
			// Round down numbers near zero	
			if(realNum < min_Num) {
				realNum = 0;
			}
			if(realNum == 0) {
				strcpy(localStr1,"0.");
				if(!precision || (format == realFmt_float)) {
					localStr1[1] = 0;   // Term & remove dp
				} else {
					for(n = 2; n <= (precision +1); n++) {
						localStr1[n] = '0';
					}
					localStr1[n] = 0;
				}	// if !precision
				strcpy(output,localStr1);
				return true;             // Exit after assignment
			}  // Of (else) realNum == 0
    			
    		// Don't allow zero precision after round-down!	
			if(!precision) { 
				precision++;
			}
			sprintf(localStr1,"%.*E",precision - 1,realNum);
			break;
			
		default :
			output = 0;
			return false;
			
	} // End of switch (format)

	// NOW, the only formats left are the float and the sigFig.  
	// They started out as sciNot and are processed similarly
	
	n = 1;	// Move to the exponent of the number
	while(n < max_len && localStr1[n] != 'e' && localStr1[n] != 'E') {
		n++;
	}
	localStr1[n] = 0;          // Terminate at exponent	designator
	expn = strtol(&localStr1[n+1],0,10);  // Find numeric value of exponent
	
	// Build the output string
	indx1 = 0;       // Index into original string
	indx2 = 0;       // Index to output string
	if(neg) {
		localStr2[indx2] = '-';
		indx2++;
	}
	absExpn = abs(expn);	
	dpPlace = expn + 1;
	// If negative expn, add preceeding "0." and padding zeros	
	if(expn < 0) {
		k = 0;
		while( k < abs(expn)) {
			localStr2[indx2] = '0';
			indx2++;
			if(!k) {
				localStr2[indx2] = '.';
				indx2++;
			}
			k++;
		}	// while
	}	// if(expn < 0)
	// Transfer the figures, skipping over the dp
	while(localStr1[indx1]) {
		localStr2[indx2] = localStr1[indx1];
		indx2++;
		indx1++;
		if(localStr1[indx1] == '.') {
			indx1++;
		}
		precision--;
		absExpn--;
		dpPlace--;
		if(!dpPlace && (precision > 0)) {
			localStr2[indx2] = '.';
			indx2++;
		}
	}	// while(localSt
	if(expn > 0) {	// Add zeros to make up whole number
		absExpn++;
		while(absExpn > 0) {
			localStr2[indx2] = '0';
			indx2++;
			absExpn--;
		}
	}	
	// Trim trailing zeros and dp if float format
	if(format == realFmt_float) { 
		if((expn <= 0) || (reqPrecision > (abs(expn) + 1))) {	
			indx2--;			
			while(indx2 && (localStr2[indx2] == '0')) {
				indx2--;
			}
			if(localStr2[indx2] == '.') {  // Eat dp as well
				indx2--;
			}
			indx2++;
		}	
	}	// if(format…	
	localStr2[indx2] = 0;	
	strcpy(output,localStr2);
	return true;
}
				

// -------------------------------------------------------------------
// Returns the width of the numerical string prior to the dp.   
int
DecTabWidth(const char* realStr) 
{
	int	n = 0;
	int	len = 0;
	char	ch;
	
	while(1) {
		ch = realStr[n];
		if(ch && (ch != '.')) {
	//		len += CharWidth(ch);
		} else {
			return len;
		}
		n++;
	}
}	
