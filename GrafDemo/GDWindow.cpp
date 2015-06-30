/* ------------------------------------------------------------------ 

	Title: GDWindow

	Description:  A window for drawing the WGStdGraph view.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		03 Dec 97
			Created
------------------------------------------------------------------ */ 
#include "GDWindow.h"
#include <stdlib.h>

const float mb_height = 15;

GDWindow::GDWindow(BRect frame)
				: BWindow(frame, "GrafDemo 4.6", B_DOCUMENT_WINDOW,0)
{
	Lock();
	// Add the graph to the window
	BRect grafRect = Bounds();
	grafRect.top += mb_height + 2;
	mGraphView= new GDView(grafRect);
	AddChild(mGraphView);
	SetSizeLimits(frame.right - frame.left,9999,frame.bottom - frame.top,9999);

	// Add the main menu bar and menus
	BRect menuRect(0,0,1000,mb_height);
	BMenuBar* menuBar = new BMenuBar(menuRect,"MB");
	
	BMenu*	fileMenu = new BMenu("File");
	BMenuItem* aboutItem = new BMenuItem("About...",new BMessage(B_ABOUT_REQUESTED));
	aboutItem->SetTarget(be_app);
	fileMenu->AddItem(aboutItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(new BMenuItem("Quit",new BMessage(B_QUIT_REQUESTED)));
	menuBar->AddItem(fileMenu);
	
	BMenu*	editMenu = new BMenu("Edit");
	editMenu->AddItem(new BMenuItem("Erase",new BMessage(G_erase_data)));
	editMenu->AddItem(new BMenuItem("Reset Axes",new BMessage(G_reset_axes)));
	editMenu->SetTargetForItems(mGraphView);
	menuBar->AddItem(editMenu);
	
	BMenu*	paramsMenu = new BMenu("Params");
	BMessage* cFitMesg = new BMessage(G_set_params);
	cFitMesg->AddInt16(pName,0);
	BMessage* p2Mesg = new BMessage(G_set_params);
	p2Mesg->AddInt16(pName,2);
	BMessage* p3Mesg = new BMessage(G_set_params);
	p3Mesg->AddInt16(pName,3);
	BMessage* p4Mesg = new BMessage(G_set_params);
	p4Mesg->AddInt16(pName,4);
	paramsMenu->AddItem(new BMenuItem("Cubic Spline",cFitMesg));
	paramsMenu->AddSeparatorItem();
	paramsMenu->AddItem(new BMenuItem("1st Order",p2Mesg));
	paramsMenu->AddItem(new BMenuItem("2nd Order",p3Mesg));
	paramsMenu->AddItem(new BMenuItem("3rd Order",p4Mesg));
	paramsMenu->SetRadioMode(true);
	paramsMenu->FindItem("2nd Order")->SetMarked(true);
	paramsMenu->SetTargetForItems(mGraphView);
	menuBar->AddItem(paramsMenu);

	BMenu*	axisMenu = new BMenu("Axes");
	axisMenu->AddItem(new BMenuItem("Set Axes"B_UTF8_ELLIPSIS,new BMessage(G_change_axis)));
	axisMenu->SetTargetForItems(mGraphView);
	menuBar->AddItem(axisMenu);

	AddChild(menuBar);
	
	Unlock();
}
// -------------------------------------------------------------------
bool GDWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}
