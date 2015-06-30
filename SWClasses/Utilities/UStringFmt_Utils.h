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

#ifndef U_StringFmtUtils
#define U_StringFmtUtils


// -------------------------------------------------------------------
void
CenterTruncate(char* string, int charLen);

// Truncates by removing characters from the center of the string and
// substituting an ellipsis
// -------------------------------------------------------------------
bool
LeftJustify(char* string, int totLen, int storageSize);

// Removes all leading spaces and adds enough trailing spaces to     
// form a string with a length of "totLen". TRUE if successful

// -------------------------------------------------------------------
bool
RightJustify(char* string, int totLen, int storageSize);

// Removes all trailing spaces and right justifies "string" by adding  
// enough leading spaces to form a string of "totLen".

// -------------------------------------------------------------------
void
Strip(char* string, char ch);

// Removes all occurrences of 'ch' in 'string'.

// -------------------------------------------------------------------

#endif
