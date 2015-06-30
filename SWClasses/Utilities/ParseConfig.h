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
		
The config file consists of any number of separated data groups in 
the following form:

[Group Title]  ALWAYS in brackets
FLDA=123
FLDB=234
FLDC=345
etc

NOTES: An open and valid BFile* is passed to this class. 
Isolated tokens can be found by calling:
bool GetField(const char* token, char* rslt)
bool GetField(const char* token, int16* rslt)
bool GetField(const char* token, int32* rslt)
bool GetField(const char* token, float* rslt)

If the tokens are in a group, first call:
bool FindSection(const char* group), where "group" is 
NOT enclosed in brackets.
------------------------------------------------------------------ */ 
#ifndef _ParseConfig
#define _ParseConfig

#include <SupportKit.h>

class ParseConfig {

	public:
	
	enum {	// Max length of a single result field
		max_rslt_len = 64
	};
		ParseConfig(BFile* theFile);
		
	virtual
		~ParseConfig();
		
	bool
		FindSection(const char* sectToken);
		
	bool
		FirstSection();
		
	bool	// Gets 'result" up to max_rslt_len
		GetField(const char* token, char* result);
	bool	
		GetField(const char* token, float* result);
	bool	
		GetField(const char* token, int16* result);
	bool	
		GetField(const char* token, int32* result);
	bool	
		GetField(const char* token, bool* result);
	
	bool
		NextSection();
	const char*
		SectionName() {return (const char*)mSectName;}
							
	protected:	// Data
	
	BFile*		mFile;
	off_t 		mSize;
	off_t			mSectionPosn;
	char*			mBuffer;
	char		mSectName[64];
};
#endif
