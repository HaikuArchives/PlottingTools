/* -------------------------------------------------------------------

	Title:
		UFloating_Util

	Description:
		Misc real number utilities.

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School of Medicine
		462-CB Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		19 Jan, 94 - Adapted for MetroWerks C++
		15 Mar, 92 - Added 'LogOfBase' function.
		25 Mar, 91 - Adapted to Macintosh by author.
		24 Sep, 87 - Last (V3.0) IBM version by author.
		17 May, 85 - First IBM version by author.

------------------------------------------------------------------- */

#ifndef U_FloatingUtil
#define U_FloatingUtil

// -------------------------------------------------------------------
float
BLog(float R);

// Finds the log base 2 of R

// -------------------------------------------------------------------
float
CLog(float R);

// Finds the common (base 10) log of R *)

// -------------------------------------------------------------------
int
Exponent(float num);

// Where num = x.xx X 10^Int, this procedure returns 'Int'.

// -------------------------------------------------------------------
float
FractPart(float num);

// Returns the fractional part of 'num' - ie Fract(4.32) = 0.32.

// -------------------------------------------------------------------
float
IntPart(float num);

// Returns the integer part of 'num' - ie Int(4.32) = 4.00.

// -------------------------------------------------------------------
float
LogOfBase(float number, float base);

// Returns the log of 'number' to the base 'base'.

// -------------------------------------------------------------------
bool
MathErr();

// Returns TRUE if the last operation called from this module was
//	in error.  Note that errors such as divide by 0 do not cause the
//	program to bomb, but will usually return a zero, or 10E10, etc.
	
// -------------------------------------------------------------------
float
Root(float num, float root);

// Finds num^1/root

// -------------------------------------------------------------------
float
Power(float base, float exp);

// Finds base^exp

// -------------------------------------------------------------------

#endif
