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

#include "U_Conversions.h"

#include <stdio.h>

const int32 val_to_third = 256*256*256;
const int32 val_to_two	= 256*256;

// -------------------------------------------------------------------
int32
AlphaToLong(const char* valStr)
{
	long result = 0;
	// See if it is a number, otherwise translate characters
	if(sscanf(valStr,"%ld",&result) == 0) {
		// Try translating characters
		int16 indx = 0;
		if(valStr[indx] == 047) indx++ ;	// If starts with apostrophe
		int32 factor = val_to_third;
		while(valStr[indx] > 0 && factor > 0) {
			result += (int32)valStr[indx++]*factor;
			factor /= 256;
		}
	}
	return result;
}
// -------------------------------------------------------------------
int32
ColorToValue(rgb_color color)
{
	return color.red*val_to_third + color.green*val_to_two + color.blue*256 + color.alpha;
}
// -------------------------------------------------------------------
void
Long2Str(int32 val,char* result)
{
	int32 vals[4];
	int32 rem = val;
	int32 fact = 256*256*256;
	bool hasChar = false;
	for(int n = 0; n < 4; n++) {
		vals[n] = rem/fact;
		rem -= vals[n]*fact;
		fact /= 256;
		if(vals[n] > 0x30) {
			hasChar = true;
		}
	}
	if(hasChar) {
		sprintf(result,"%c%c%c%c",vals[0],vals[1],vals[2],vals[3]);
	} else {
		result[0] = 0;
	}
}
// -------------------------------------------------------------------
void
TypeString(type_code theType, char* outStr)
{
	switch (theType) {
		case B_ANY_TYPE :
			::strcpy(outStr,"B_ANY_TYPE");
			break;
		case B_ASCII_TYPE :
			::strcpy(outStr,"B_ASCII_TYPE");
			break;
		case B_BOOL_TYPE :
			::strcpy(outStr,"B_BOOL_TYPE");
			break;
		case B_CHAR_TYPE :
			::strcpy(outStr,"B_CHAR_TYPE");
			break;
		case B_COLOR_8_BIT_TYPE :
			::strcpy(outStr,"B_COLOR_8_BIT_TYPE");
			break;
		case B_DOUBLE_TYPE :
			::strcpy(outStr,"B_DOUBLE_TYPE");
			break;
		case B_FLOAT_TYPE :
			::strcpy(outStr,"B_FLOAT_TYPE");
			break;
		case B_GRAYSCALE_8_BIT_TYPE :
			::strcpy(outStr,"B_GRAYSCALE_8_BIT_TYPE");
			break;
		case B_INT64_TYPE :
			::strcpy(outStr,"B_INT64_TYPE");
			break;
		case B_INT32_TYPE :
			::strcpy(outStr,"B_INT32_TYPE");
			break;
		case B_INT16_TYPE :
			::strcpy(outStr,"B_INT16_TYPE");
			break;
		case B_INT8_TYPE :
			::strcpy(outStr,"B_INT8_TYPE");
			break;
		case B_MESSAGE_TYPE :
			::strcpy(outStr,"B_MESSAGE_TYPE");
			break;
		case B_MESSENGER_TYPE :
			::strcpy(outStr,"B_MESSENGER_TYPE");
			break;
		case B_MIME_TYPE :
			::strcpy(outStr,"B_MIME_TYPE");
			break;
		case B_MONOCHROME_1_BIT_TYPE :
			::strcpy(outStr,"B_MONOCHROME_1_BIT_TYPE");
			break;
		case B_OBJECT_TYPE :
			::strcpy(outStr,"B_OBJECT_TYPE");
			break;
		case B_OFF_T_TYPE :
			::strcpy(outStr,"B_OFF_T_TYPE");
			break;
		case B_PATTERN_TYPE :
			::strcpy(outStr,"B_PATTERN_TYPE");
			break;
		case B_POINTER_TYPE :
			::strcpy(outStr,"B_POINTER_TYPE");
			break;
		case B_POINT_TYPE :
			::strcpy(outStr,"B_POINT_TYPE");
			break;
		case B_RAW_TYPE :
			::strcpy(outStr,"B_RAW_TYPE");
			break;
		case B_RECT_TYPE :
			::strcpy(outStr,"B_RECT_TYPE");
			break;
		case B_REF_TYPE :
			::strcpy(outStr,"B_REF_TYPE");
			break;
		case B_RGB_32_BIT_TYPE :
			::strcpy(outStr,"B_RGB_32_BIT_TYPE");
			break;
		case B_RGB_COLOR_TYPE :
			::strcpy(outStr,"B_RGB_COLOR_TYPE");
			break;
		case B_SIZE_T_TYPE :
			::strcpy(outStr,"B_SIZE_T_TYPE");
			break;
		case B_SSIZE_T_TYPE :
			::strcpy(outStr,"B_SSIZE_T_TYPE");
			break;
		case B_STRING_TYPE :
			::strcpy(outStr,"B_STRING_TYPE");
			break;
		case B_TIME_TYPE :
			::strcpy(outStr,"B_TIME_TYPE");
			break;
		case B_UINT64_TYPE :
			::strcpy(outStr,"B_UINT64_TYPE");
			break;
		case B_UINT32_TYPE :
			::strcpy(outStr,"B_UINT32_TYPE");
			break;
		case B_UINT16_TYPE :
			::strcpy(outStr,"B_UINT16_TYPE");
			break;
		case B_UINT8_TYPE :
			::strcpy(outStr,"B_UINT8_TYPE");
			break;
		case B_MEDIA_PARAMETER_TYPE :
			::strcpy(outStr,"B_MEDIA_PARAMETER_TYPE");
			break;
		case B_MEDIA_PARAMETER_WEB_TYPE :
			::strcpy(outStr,"B_MEDIA_PARAMETER_WEB_TYPE");
			break;
		case B_MEDIA_PARAMETER_GROUP_TYPE :
			::strcpy(outStr,"B_MEDIA_PARAMETER_GROUP_TYPE");
			break;
		default :
			Long2Str(theType,outStr);
	}
}
// -------------------------------------------------------------------
rgb_color
ValueToColor(int32 val)
{
	uint32 uVal = (uint32)val;
	rgb_color cVal;	
	cVal.red = uVal/val_to_third;
	uVal -= cVal.red*val_to_third ;
	cVal.green = uVal/val_to_two;
	uVal -= cVal.green*val_to_two;
	cVal.blue = uVal/256;
	uVal -= cVal.blue*256;
	cVal.alpha = uVal;
	
	return cVal;
}
// -------------------------------------------------------------------
