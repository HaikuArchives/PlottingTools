/* -------------------------------------------------------------------

	Title:
		U_Conversions

	Description:
		Misc string conversions.

	Author :
		Stephen Wardlaw, M.D.
		Medical Reserch & Development
		Highrock
		Lyme, Ct 06371
		<swardlaw@connix.com>

	Edit History:
		08 September, 1999
			Assembled from bits & pieces

------------------------------------------------------------------- */

#ifndef _U_Conversions
#define _U_Conversions

#include <AppKit.h>

// NOTE! These assume that the 'result' string is long enough!
// -------------------------------------------------------------------
int32
AlphaToLong(const char* valStr);
// Converts either a numerical string or four-byte alpha string into a long.
// If the alpha string starts with a ', it is discarded.
// -------------------------------------------------------------------
int32
ColorToValue(rgb_color color);
// Converts an rgb_color to a signed 32 bit integer		
// -------------------------------------------------------------------
void
Long2Str(int32 val,char* result);

// Converts a long value into a four-byte a/n string, where possible.
// Sets string blank if not convertable.
// -------------------------------------------------------------------
void
TypeString(type_code theType, char* result);

// Converts a BeOS code_type to a definition or an a/n string
// -------------------------------------------------------------------
rgb_color
ValueToColor(int32 val);
// Converts a sighed 32-bit integer to a color		
// -------------------------------------------------------------------
#endif
