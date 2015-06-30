/* ------------------------------------------------------------------ 

	Title: WGAbscissaLabel

	Description:  A class for drawinf abscissa labels

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		07 Dec, 97
			Created for the BeOS		
------------------------------------------------------------------ */ 

#ifndef WG_AbscissaLabel
#define WG_AbscissaLabel

#include <InterfaceKit.h>

class WGAbscissaLabel : public BStringView {

	public:

			WGAbscissaLabel(BRect frame,
								 const char* = 		"AbscissaLabel",
								 const char* = 		"X-Axis",
								 uint32 resizeMode = B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT,
								 uint32 flags = 	B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	
};

#endif
