/* ------------------------------------------------------------------ 

	Title: WGAxisEdit

	Description:  A view class for editing a graph axis

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:
		28 September, 1999
			Adapted for BeOS
		16 May 90
			Modified for MetroWerks Modula-2 on Macintosh
		06 Nov 87
			Last IBM Version 6.0
		?? Aug 80
			First version for Apple IIe
		
------------------------------------------------------------------ */ 

#include "WGAxisEdit.h"
#include "WColorControl.h"
#include "URealFmt_Utils.h"
#include <stdio.h>
#include <string.h>
#include <SupportKit.h>

const uint32	max_changed		= 'Cmax';
const uint32	min_changed		= 'Cmin';
const uint32	int_changed		= 'Cint';
const uint32	scale_changed	= 'Csca';
const uint32	fmt_changed		= 'Cfmt';
const uint32	prec_changed	= 'Cpre';
const uint32	divs_changed	= 'Cdiv';
const uint32	label_changed	= 'Clab';

const char* int_title  = "Interval:";
const char* base_title = "Log Base:";

// -------------------------------------------------------------------
// *  Constructors & Destructor             
// -------------------------------------------------------------------
WGAxisEdit::WGAxisEdit(BRect frame,
				   		const char* name,
				   		int32 mode,
				   		int32 flags):BView(frame,name,mode,flags)
{
	SetViewColor(209,209,209);
	mAxis = new WGGraphAxis();
	Initialize(name);
}

WGAxisEdit::~WGAxisEdit()
{
	delete mAxis;
}
// -------------------------------------------------------------------
// *  Public Methods           
// -------------------------------------------------------------------
BMessage*	// Data can be extracted from this class here
WGAxisEdit::Data()
{
	BMessage* theMesg = new BMessage(WGGraphAxis::class_ID);
	mAxis->Archive(theMesg);
	theMesg->AddString(axis_label,mNameView->Text());
	return theMesg;
}
// -------------------------------------------------------------------
void
WGAxisEdit::MessageReceived(BMessage* theMesg)
{
	float min, max, val;
	bool reset = false;
	ERealFormat format;
	int	prec,divs;
	EAxisError error = axErr_noErr;
	
	switch(theMesg->what) {
	
		case max_changed :
		case min_changed :
			::sscanf(mMaxVal->Text(),"%f",&max);
			::sscanf(mMinVal->Text(),"%f",&min);
			error = mAxis->SetSpan(min,max);
			reset = true;
			break;
			
		case int_changed :
			::sscanf(mInterval->Text(),"%f",&val);
			error = mAxis->SetInterval(val);
			reset = true;
			break;
			
		case scale_changed :
			Window()->Lock();
			error = mAxis->SetScale((EAxisScale)mScaleType->Menu()->IndexOf(mScaleType->Menu()->FindMarked()));
			if(mAxis->Scale() == scale_linear) {
				mInterval->SetLabel(int_title);
				mInterval->SetEnabled(true);
			} else {
				mInterval->SetLabel(base_title);
				mInterval->SetEnabled(false);
			}
			Window()->Unlock();
			reset = true;
			break;
			
		case fmt_changed :
		case prec_changed :
			format = (ERealFormat)mFormat->Menu()->IndexOf(mFormat->Menu()->FindMarked());
			prec = mPrecision->Menu()->IndexOf(mPrecision->Menu()->FindMarked());
			error = mAxis->SetLabelFormat(format,prec);
			reset = true;
			break;
			
		case divs_changed :
			::sscanf(mDivisions->Text(),"%d",&divs);
			error = mAxis->SetMinorDivisions(divs);
			reset = true;
			break;
			
		case label_changed :
			;
			break;
			
		default :
			BView::MessageReceived(theMesg);
	}
	
	if(reset) {
		SetDisplay();
		ShowError(error);
	}
}
// -------------------------------------------------------------------
void
WGAxisEdit::SetDefaultVals(const WGGraphAxis* theAxis)
{
	delete mAxis;
	mAxis = new WGGraphAxis(*theAxis);
	SetDisplay();
}
// -------------------------------------------------------------------
void
WGAxisEdit::SetDisplay() const 
{
	char tStr[16];
	if(mAxis == NULL) {
		return;	// EXIT HERE if no data
	}
	BAutolock lock(Window());
	::FormatReal(mAxis->Max(),mAxis->NumberFormat(),mAxis->NumberPrecision(),tStr);
	mMaxVal->SetText(tStr);
	::FormatReal(mAxis->Min(),mAxis->NumberFormat(),mAxis->NumberPrecision(),tStr);
	mMinVal->SetText(tStr);
	if(mAxis->Scale() == scale_linear) {
		::FormatReal(mAxis->Interval(),mAxis->NumberFormat(),mAxis->NumberPrecision(),tStr);
	} else {
		float val = 10;
		if(mAxis->Scale() == scale_log2) {
			val = 2;
		} 
		::FormatReal(val,realFmt_fixed,0,tStr);
	}
	mInterval->SetText(tStr);
	::sprintf(tStr,"%d",mAxis->Divisions());
	mDivisions->SetText(tStr);
	mScaleType->Menu()->ItemAt((int16)mAxis->Scale())->SetMarked(true);
	mFormat->Menu()->ItemAt((int16)mAxis->NumberFormat())->SetMarked(true);
	mPrecision->Menu()->ItemAt((int16)mAxis->NumberPrecision())->SetMarked(true);
}
// -------------------------------------------------------------------
void
WGAxisEdit::SetLabel(const char* theLabel)
{
	BAutolock lock(Window());
	mNameView->SetText(theLabel);
}
// -------------------------------------------------------------------
// *  Protected Methods         
// -------------------------------------------------------------------
void
WGAxisEdit::AttachedToWindow()
{
	BView::AttachedToWindow();
	mMaxVal->SetTarget(this);
	mMinVal->SetTarget(this);
	mInterval->SetTarget(this);
	mDivisions->SetTarget(this);
	mPrecision->Menu()->SetTargetForItems(this);
	mScaleType->Menu()->SetTargetForItems(this);
	mFormat->Menu()->SetTargetForItems(this);
}
// -------------------------------------------------------------------
void
WGAxisEdit::Initialize(const char* name)
{
	const float ctrl_margin = 5;
	const float menu_width = 65;
	// Names for TextControls
	const char*	name_title = "Axis Label:";
	const char* max_title = "Max:";
	const char* min_title = "Min:";
	const char* num_dummy = "1.2345E+23";
	// Names for menus
	const char* fmt_name = "Format:";
	const char* scale_name = "Scale:";
	const char* prec_name = "Prec:";
	const char*	div_title = "Divs:";
	
	// Textcontrol for the axis label
	BRect cFrame(ctrl_margin,ctrl_margin,ctrl_margin, ctrl_margin);
	mNameView = new BTextControl(cFrame,"axislabel",name_title,name,new BMessage(label_changed));
	AddChild(mNameView);
	// We want all controls the same height
	mNameView->ResizeToPreferred();
	float ctrlHt = mNameView->Bounds().Height();
	mNameView->ResizeTo(StringWidth(name_title) + StringWidth(name) + 4, mNameView->Bounds().Height());
	mNameView->SetDivider(StringWidth(name_title) + 2);
	
	// Menu for scale type
	BPopUpMenu* scaleMenu = new BPopUpMenu("scale");
	const char* lin_item = "Linear";
	const char* log2_item = "Log 2";
	const char* log10_item = "Log 10";
	scaleMenu->AddItem(new BMenuItem(lin_item,new BMessage(scale_changed)));
	scaleMenu->AddItem(new BMenuItem(log2_item,new BMessage(scale_changed)));
	scaleMenu->AddItem(new BMenuItem(log10_item,new BMessage(scale_changed)));
	scaleMenu->FindItem("Linear")->SetMarked(true);
	
	// Menu for number format
	const char* genFmt_item = "General";
	const char* fixed_item = "Fixed";
	const char* sci_item = "Sci.Not";
	const char*	sig_item = "Sig.Figs";
	const char* float_item = "Float";
	const char* formal_item = "Formal";
	BPopUpMenu* formatMenu = new BPopUpMenu("format");
	formatMenu->AddItem(new BMenuItem(genFmt_item,new BMessage(fmt_changed)));
	formatMenu->AddItem(new BMenuItem(fixed_item,new BMessage(fmt_changed)));
	formatMenu->AddItem(new BMenuItem(sci_item,new BMessage(fmt_changed)));
	formatMenu->AddItem(new BMenuItem(sig_item,new BMessage(fmt_changed)));
	formatMenu->AddItem(new BMenuItem(float_item,new BMessage(fmt_changed)));
	formatMenu->AddItem(new BMenuItem(formal_item,new BMessage(fmt_changed)));
	
	// Menu for precision
	BPopUpMenu* precMenu = new BPopUpMenu("precmenu");
	precMenu->AddItem(new BMenuItem("0",new BMessage(prec_changed)));
	precMenu->AddItem(new BMenuItem("1",new BMessage(prec_changed)));
	precMenu->AddItem(new BMenuItem("2",new BMessage(prec_changed)));
	precMenu->AddItem(new BMenuItem("3",new BMessage(prec_changed)));
	precMenu->AddItem(new BMenuItem("4",new BMessage(prec_changed)));
	
	float maxTWd = StringWidth(fmt_name);
	float maxMwd = menu_width;
	// Add scale menu
	cFrame.left = cFrame.right = ctrl_margin;
	cFrame.right += maxTWd + maxMwd;
	cFrame.top = cFrame.bottom = mNameView->Bounds().bottom + ctrl_margin*2;
	cFrame.bottom += ctrlHt;
	mScaleType = new BMenuField(cFrame,"scalefld",scale_name,scaleMenu);
	mScaleType->SetDivider(maxTWd + 2);
	AddChild(mScaleType);
	
	// Add format menu
	cFrame.OffsetBy(0,ctrlHt + ctrl_margin);
	mFormat = new BMenuField(cFrame,"numfmt",fmt_name,formatMenu);
	formatMenu->FindItem("Float")->SetMarked(true);
	mFormat->SetDivider(maxTWd + 2);
	AddChild(mFormat);	
	
	// Add precision menu
	cFrame.OffsetBy(0,ctrlHt + ctrl_margin);
	mPrecision = new BMenuField(cFrame,"prec",prec_name,precMenu);
	precMenu->FindItem("3")->SetMarked(true);
	mPrecision->SetDivider(maxTWd + 2);
	AddChild(mPrecision);

	// Add right side items, starting with the max value control
	float divWd = StringWidth(max_title) + 2;	// Width of ctrl titles
	cFrame = mScaleType->Frame();
	cFrame.left = cFrame.right + ctrl_margin;
	cFrame.right = cFrame.left + divWd + StringWidth(num_dummy) + 10;
	mMaxVal = new BTextControl(cFrame,"maxval",max_title,num_dummy,new BMessage(max_changed));
	mMaxVal->SetDivider(divWd);
	AddChild(mMaxVal);
	
	// Minimum value editor
	cFrame.OffsetBy(0,ctrlHt + ctrl_margin);
	mMinVal = new BTextControl(cFrame,"minval",min_title,num_dummy,new BMessage(min_changed));
	mMinVal->SetDivider(divWd);
	AddChild(mMinVal);
	
	// Marking interval
	cFrame.OffsetBy(0,ctrlHt + ctrl_margin);
	mInterval = new BTextControl(cFrame,"intval",int_title,num_dummy,new BMessage(int_changed));
	AddChild(mInterval);
	mInterval->SetDivider(StringWidth(base_title) + 2);
	
	// Minor divisions
	cFrame.OffsetBy(0,ctrlHt + ctrl_margin);
	mDivisions = new BTextControl(cFrame,"divs",div_title,"2",new BMessage(divs_changed));
	mDivisions->SetDivider(StringWidth(div_title) + 2);
	AddChild(mDivisions);
	
	
	// Size view 
	float width = mMaxVal->Frame().right + ctrl_margin;
	float height = mDivisions->Frame().bottom + ctrl_margin;
	ResizeTo(width,height);
	mNameView->ResizeTo(width - ctrl_margin*2, mNameView->Bounds().Height());
	SetDisplay();
}		
// -------------------------------------------------------------------
void
WGAxisEdit::Draw(BRect frame)
{
	BRect vFrame = Bounds();
	vFrame.InsetBy(1,1);
	SetHighColor(color_white);
	StrokeRect(vFrame);
	SetHighColor(color_black);
	StrokeRect(Bounds());
}
// -------------------------------------------------------------------
void
WGAxisEdit::ShowError(EAxisError theErr)
{
	char errStr[128];
	
	if(theErr > axErr_noErr) {
		::strcpy(errStr,"Error In Entering Axis Data:\n");
		switch (theErr) {
			case axErr_span:
				::strcat(errStr,"Axis Min. Must Be > Max.");
				break;
				
			case axErr_logZero:
				::strcat(errStr,"Axis Min. Must Be > 0 With Log Scale");
				break;
			
			case axErr_badScale:
				::strcat(errStr,"Illegal Scale");
				break;
			
			case axErr_interval:
				::strcat(errStr,"Axis Interval Too Small");
				break;
			
			default :
				;	// No default case
		}
		BAlert* theAlert = new BAlert("Axis Data Error",errStr,"OK");
		theAlert->Go();
	}
}
// -------------------------------------------------------------------
