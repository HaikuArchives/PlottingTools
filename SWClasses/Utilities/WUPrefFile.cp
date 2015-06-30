/* ------------------------------------------------------------------ 

	Title: WUPrefFile

	Description:  A class for handling common pref file items.

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		03 February, 2000
			Created
		
------------------------------------------------------------------ */
#include "WUPrefFile.h"
	
#include <StorageKit.h>
#include <SupportKit.h>

// -------------------------------------------------------------------
//	* CONSTRUCTORS and DESTRUCTORS
// -------------------------------------------------------------------
WUPrefFile::WUPrefFile(const char* prefFileDir, const char* prefFileName)
{
	// Open the preferences file and see if there is anything in it
	BPath prefPath;
	if(find_directory(B_USER_SETTINGS_DIRECTORY,&prefPath) == B_OK
	&& prefPath.Append(prefFileDir) == B_OK) {
		create_directory(prefPath.Path(),0777);
		prefPath.Append(prefFileName);
		mFile.SetTo(prefPath.Path(),B_READ_WRITE|B_CREATE_FILE);
	}
}
// -------------------------------------------------------------------
//	* Public Methods
// -------------------------------------------------------------------
BMessage*
WUPrefFile::GetPrefData()
{
	if(mFile.InitCheck() == B_OK) {
		BMessage* theMesg = new BMessage();
		if(theMesg->Unflatten(&mFile) == B_OK) {
			return theMesg;
		} else {
			delete theMesg;
		}
	}
	return NULL;
}
// -------------------------------------------------------------------
bool
WUPrefFile::SetPrefData(BMessage* theData)
{
	if(mFile.InitCheck() == B_OK) {
		mFile.Seek(0,SEEK_SET);
		return (theData->Flatten(&mFile) == B_OK);
	}
	return false;
}
// -------------------------------------------------------------------
