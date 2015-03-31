// ACGLConfigReader.cpp: implementation of the CACGLConfigReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACGLConfigReader.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CString SECTION_NAME = _T("Tools");
#define CONFIG_FILE_NAME _T("AdvCardiacGLToolconfig.ini")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CACGLConfigReader::CACGLConfigReader()
{
    m_ini_file_path.Empty();
	//set the path for ini file
    m_ini_file_path = getenv(MRMP_CONFIG);
    m_ini_file_path += _T("\\");
    m_ini_file_path += APP_NAME;
    m_ini_file_path += _T("\\");
    m_ini_file_path += CONFIG_FILE_NAME;
}

CACGLConfigReader::~CACGLConfigReader()
{
	m_ini_file_path.Empty();
}

int CACGLConfigReader::ReadToolConfigSettings(std::vector<KeyValuePair>& f_KeyValuePairs)
{
    try{
		if(PathFileExists(m_ini_file_path)) {
			string iniFilePath;    // for m_ini_file_path;
			string sectionName;    // for ACGLTOOLS;
			
			wstring ws1(m_ini_file_path);
			wstring ws2(SECTION_NAME);
			
			iniFilePath.assign(ws1.begin(),ws1.end());     
			sectionName.assign(ws2.begin(),ws2.end());
			
			int l_nStatus = ReadIniFile(iniFilePath, sectionName, f_KeyValuePairs);     
			if(E_NO_ERROR != l_nStatus){
				CString l_csmsg(_T("ReadIniFile returned error"));
				CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
				return(E_ERROR);				
			}
		}else{
			CString l_csmsg(_T(""));
			l_csmsg.Format(_T("\"%S\" not found."),m_ini_file_path);
			CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
			return(E_ERROR);
		}     
    }catch(...) {
		CString l_csmsg(_T("Exception occurred in ReadToolConfigSettings"));
		CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int CACGLConfigReader::ReadIniFile( const std::string& iniFilePath, const std::string& sectionName, std::vector<KeyValuePair>& f_KeyValuePairs )
{
    try{
		const int bufferSize = 10000;	//pending size ?
		char buffer[bufferSize] = "";
		int charsRead = 0;
		
		if((iniFilePath.length() <= 0) || (sectionName.length() <= 0)){
			CString l_csmsg(_T("Invalid function parameter"));
			CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
			return(E_ERROR);
		}
		
		charsRead = GetPrivateProfileSectionA((LPCSTR) sectionName.c_str(),
			buffer,
			bufferSize,
			(LPCSTR)iniFilePath.c_str() );
		
		// if there isn't enough space, returns bufferSize - 2    
		
		if( (0 < charsRead) && ((bufferSize - 2) > charsRead) ) {
			// walk the buffer extracting values        
			// start at the beginning (const to remind us not to
			// change the contents of the buffer)
			const char* pSubstr = buffer;
			
			// while we have non-empty substrings...
			while('\0' != *pSubstr) {
				// length of key-value pair substring
				size_t substrLen = strlen(pSubstr);
				
				// split substring on '=' char
				const char* pos = strchr(pSubstr, '=');
				if(NULL != pos) {
					// pending remove "magic number" for buffer size 
					char name [256] = "";
					char value[256] = "";
					
					strncpy(name , pSubstr, pos - pSubstr);
					strncpy(value, pos + 1, substrLen - (pos - pSubstr));
					
					f_KeyValuePairs.push_back(KeyValuePair(name, value));
				}else{
					CString l_csmsg(_T("File reading error"));
					CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
					return(E_ERROR);
				}
				
				// jump over the current substring plus its null
				pSubstr += (substrLen + 1);
			}
		}else{
			CString l_csmsg(_T("File reading error"));
			CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
			return(E_ERROR);
		}
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in ReadIniFile"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		return(E_ERROR);
    }
	return(E_NO_ERROR);
}
///////////////////////////////// END OF FILE /////////////////////////////////
