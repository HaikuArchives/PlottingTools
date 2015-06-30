/* ------------------------------------------------------------------ 

	Title: ParseConfig

	Description:  A class which parses a configuration file. 

	Author :
		Stephen Wardlaw, M.D.
		Medical Research & Development
		Highrock
		Lyme, CT 06371

	Edit History:	
		10 January, 2000
			Modified for data extraction
		15 June, 1999
			Created
------------------------------------------------------------------ */ 
#include "ParseConfig.h"

#include <StorageKit.h>
#include <stdio.h>

// -------------------------------------------------------------------
//	* Constructor & Destructor
// -------------------------------------------------------------------
ParseConfig::ParseConfig(BFile* theFile)
{
	mBuffer = NULL;
	
	if(theFile->InitCheck() == B_OK) {
		theFile->GetSize(&mSize);
		if(mSize > 0) {
			mBuffer = new char[mSize + 1];
			theFile->Read(mBuffer,mSize);
		}
	}
	mSectionPosn = -1;
}
		
ParseConfig::~ParseConfig()
{
	delete mBuffer;
}
// -------------------------------------------------------------------
//	* Public Methods
// -------------------------------------------------------------------
bool
ParseConfig::FindSection(const char* sectToken)
{
	while(mSectionPosn < mSize && mBuffer[mSectionPosn++] != '[') {
		int sIndx = 0;
		while(mSectionPosn < mSize && sectToken[sIndx] == mBuffer[mSectionPosn]) {
			sIndx++;
			mSectionPosn++;
		}
		if(mSectionPosn < mSize && sectToken[sIndx] == 0 && mBuffer[mSectionPosn] == ']') {
			mSectionPosn++;	// Start of new section
			return true;
		}
	}
	mSectionPosn = -1;
	return false;
}
// -------------------------------------------------------------------
bool
ParseConfig::FirstSection()
{
	mSectionPosn = 0;
	return NextSection();
}
// -------------------------------------------------------------------
bool	// Gets 'result" up to max_rslt_len
ParseConfig::GetField(const char* token, char* result)
{
	if(mSectionPosn == -1) {
		return false;
	}
	result[0] = 0;
	char* sPosn = ::strstr(&mBuffer[mSectionPosn],token);
	if(sPosn != NULL) {
		// Look to see if a section was passed
		char* cPtr = &mBuffer[mSectionPosn];
		while(cPtr++ < sPosn) {
			if(*cPtr == '[') {
				return false;
			}
		}
		// Make sure ends in '='
		sPosn += ::strlen(token);
		if(*sPosn != '=') {
			return false;
		}
		
		sPosn++;
		// Transfer string
		int tIndx = 0;
		while(tIndx < max_rslt_len
		&& *sPosn != '\r' 
		&& *sPosn != '\n') {
			result[tIndx++] = *sPosn;
			sPosn++;
		}
		result[tIndx] = 0;
	}
	return true;
}
// -------------------------------------------------------------------
bool	
ParseConfig::GetField(const char* token, float* result)
{
	char val[max_rslt_len + 1];
	
	if(GetField(token,val)) {
		float fval;
		if(::sscanf(val,"%f",&fval) != 0) {
			*result = fval;
			return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------
bool	
ParseConfig::GetField(const char* token, int16* result)
{
	char val[max_rslt_len + 1];
	if(GetField(token,val)) {
		int iVal;
		if(::sscanf(val,"%d",&iVal) != 0) {
			*result = iVal;
			return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------
bool	
ParseConfig::GetField(const char* token, int32* result)
{
	char val[max_rslt_len + 1];
	if(GetField(token,val)) {
		long lVal;
		if(::sscanf(val,"%ld",&lVal) != 0) {
			*result = lVal;
			return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------
bool	
ParseConfig::GetField(const char* token, bool* result)
{
	char val[max_rslt_len + 1];
	if(GetField(token,val)) {
		int bRes;
		if(::sscanf(val,"%d",&bRes) != 0) {
			*result = (bRes > 0);
			return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------
bool
ParseConfig::NextSection()
{
	mSectName[0] = 0;
	while(mSectionPosn < mSize && mBuffer[mSectionPosn++] != '[') {
		;
	}
	if(mSectionPosn > mSize) {
		mSectionPosn = -1;
		return false;
	}
	
	// Extract the section header
	int sIndx = 0;
	while(mSectionPosn < mSize && sIndx < 64 && mBuffer[mSectionPosn] != ']') {
		mSectName[sIndx++] = mBuffer[mSectionPosn++];
	}
	if(mSectionPosn < mSize && sIndx < 64) {
		mSectName[sIndx] = 0;
		mSectionPosn++;	// Set to start of data fields
		return true;
	}
	mSectionPosn = -1;
	return false;
}
// -------------------------------------------------------------------
							
