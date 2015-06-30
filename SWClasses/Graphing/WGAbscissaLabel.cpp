/* ------------------------------------------------------------------ 

	Title: WGAbscissaLabel

	Description:  A class for drawing abscissa labels

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		07 Dec, 97
			Created for the BeOS		
------------------------------------------------------------------ */ 

#include "WGAbscissaLabel.h"

// ------------------------------------------------------------------ 
//               CONSTRUCTORS AND DESTRUCTOR
// ------------------------------------------------------------------ 
WGAbscissaLabel::WGAbscissaLabel(BRect frame, const char* label, const char* title, uint32 resizeMode, uint32 flags)
										  : BStringView(frame,label,title,resizeMode,flags)
{
	SetAlignment(B_ALIGN_CENTER);
}
// ------------------------------------------------------------------ 

