/* -------------------------------------------------------------------

	Title:
		UStringFmt_Utils

	Description:
		Misc string utilities.

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School of Medicine
		462-CB Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		21 Oct, 99 - Added CenterTruncate
		18 Dec, 93 - Adapted to MW C++ by author
		25 Mar, 91 - Adapted to Macintosh by author.
		24 Sep, 87 - Last (V4.0) IBM version by author.
		06 Jun, 85 - First IBM version by author.

------------------------------------------------------------------- */

#include "UStringFmt_Utils.h"

#include <string.h>
#include <InterfaceKit.h>

// -------------------------------------------------------------------
void
CenterTruncate(char* string, int charLen)
{
	int len = ::strlen(string);
	if(len <= charLen || charLen <= 3) {
		return;
	} else {
		int difference = len - charLen + 1;
		int center = len/2;
		int cPosn = center - difference/2 - 2;
		string[cPosn] = 0;
		::strcat(string,B_UTF8_ELLIPSIS);
		cPosn += 3;
		while(cPosn < charLen) {
			string[cPosn++] = string[cPosn + difference];
		}
		string[cPosn] = 0;
	}
}
// -------------------------------------------------------------------
// Left justify the string & pad to the total length
bool
LeftJustify(char* string, int totLen, int storageSize) 
{
	int	len;
	char* strPtr1;
	char* strPtr2;
	
	if((totLen > (storageSize - 1)) || (totLen < 2)) {
		return false;
	}
	// Move pointer to first non-space character	
	strPtr1 = string;
	len = 0;
	while(*strPtr1 && (*strPtr1 == ' ')) {
		strPtr1++;
	}
	strPtr2 = string;	
	//  If there were trailing spaces, shift string to left	
	if(strPtr1 != string) {  
		while(*strPtr1 && (len <= totLen)) {
			*strPtr2 = *strPtr1;
			strPtr1++;
			strPtr2++;
			len++;
		}
	} else {  // Otherwise, find end of string
		while(*strPtr2 && (len < totLen)) {
			strPtr2++;
			len++;
		}
	}	// else
	 while(len < totLen) {
	 	*strPtr2 = ' ';
	 	strPtr2++;
	 	len++;
	 }
	 *strPtr2 = 0;
	 return true;
}	

// -------------------------------------------------------------------
// Right justify the string in a field of 'totLen'
bool
RightJustify(char* string, int totLen, int storageSize) 
{
	int 	len;
	char* strPtr1;
	char* strPtr2;

	if((totLen > (storageSize - 1)) || (totLen < 2)) {
		return false;
	}
	if(*string == ' ') { // Remove leading spaces, if any
		LeftJustify(string,totLen,storageSize);
	}	
	len = strlen(string); 
	if(len > totLen) {
		len = totLen;    // Truncate if too long
	}
	// Backtrack over any trailing spaces	
	strPtr1 = string + len - 1;
	while((strPtr1 >= string) && (*strPtr1 == ' ')) {
		strPtr1--;
		len--;
	}
		
	// Shift right to end of string.
	strPtr2 = string + totLen;
	*strPtr2 = 0;                 // Terminate string
	strPtr2--;
	while(strPtr1 >= string) {
		strPtr1--;
		strPtr2--;
		*strPtr2 = *strPtr1;
	}
	// Pad string with spaces	
	while(strPtr2 >= string) {
		strPtr2--;
		*strPtr2 = ' ';
	}	
	return true;
}

// -------------------------------------------------------------------
// Strip all instances of 'ch' from "string"
void
Strip(char* string, char ch) 
{
	char* strPtr1;
	char* strPtr2;
	
	strPtr1 = strPtr2 = string;
	while(*strPtr1 != 0) {
		*strPtr2 = *strPtr1;
		if(*strPtr1 != ch) {
			strPtr2++;
		}
		strPtr1++;
	}
	*strPtr2 = 0;
}

