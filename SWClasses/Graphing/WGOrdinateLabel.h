/* ------------------------------------------------------------------ 

	Title: WGOrdinateLabel

	Description:  A class for drawing ordinate labels

	Author :
		Stephen Wardlaw, M.D.
		Yale University School of Medicine
		20 York St.
		New Haven, CT  06504

	Edit History:
		07 Dec, 97
			Created for the BeOS		
------------------------------------------------------------------ */ 

#ifndef WG_OrdinateLabel
#define WG_OrdinateLabel

#include <InterfaceKit.h>

class WGOrdinateLabel : public BStringView {

	public:

				WGOrdinateLabel(BRect frame,
									 const char* label	= 	"OrdinateLabel",
									 const char* text		=	"Y-Axis",
									 uint32 resizeMode 	= 	B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM,
									 uint32 flags 			= 	B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
																	
	
	// Until the parent draws rotated text, we need to do it
	virtual void
		Draw(BRect theRect);

	
	};
	
#endif
