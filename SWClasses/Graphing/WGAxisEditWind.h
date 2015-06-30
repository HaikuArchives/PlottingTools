/* ------------------------------------------------------------------ 

	Title: WGAxisEditWind

	Description:  A Window class for editing a graph axis

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		01 October, 1999
			Created
------------------------------------------------------------------ */ 

#ifndef _WGAxisEditWind
#define _WGAxisEditWind

#include "WGStdGraph.h"

class WGPanelView;	// Forward declaration

// Data is send by a message with these tags
const char x_axis_name[8] = "WGAExax";	// Message containing data
const char y_axis_name[8] = "WGAEyax";	// Message containing data
const char plot_mark_name[8] = "WGPMark";	// int16 containing mark
const char plot_color_name[8] = "WGPCol";	// int32 containing color

class WGAxisEditWind : public BWindow {

	public:
	
	enum {	// Messages containing graph data from here have this type
		class_ID = 'WGaw'
	};

		WGAxisEditWind(BRect frame,
						bool useMarkColor	= true,
				   	  	const char* name	= "Change Axes",
				   		window_look look	= B_TITLED_WINDOW_LOOK,
				   		window_feel feel	= B_NORMAL_WINDOW_FEEL,
				   		int32 flags			= B_NOT_ZOOMABLE | B_NOT_RESIZABLE);	
	virtual
		~WGAxisEditWind();			   
	void
		MessageReceived(BMessage* theMesg);
	void
		SetPlotColorDefault(rgb_color defColor);
	void
		SetPlotMarkDefault(EPlotMark defMark);
	void
		SetTarget(BMessenger* theMessenger);
	void
		SetXDefaults(BMessage* theMesg);
	void
		SetXDefaults(const WGGraphAxis* theAxis);
	void
		SetXLabel(const char* label);
	void
		SetYDefaults(BMessage* theMesg);
	void
		SetYDefaults(const WGGraphAxis* theAxis);
	void
		SetYLabel(const char* label);
		
	protected:	// Methods
	
	bool
		QuitRequested();
		
	protected:	// Data
	WGPanelView*	mPanel;
	};
    
#endif
