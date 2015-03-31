// KtIniFileReader.cpp: implementation of the KtIniFileReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtIniFileReader.h"
#include "PDataParser/INIReader.h"
#include "KtReconErrors.h"
#include "shlwapi.h"
#include "KtReconLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define CONFIG_FILE_PATH _T("C:/MRMPlus/Data/KtRecon/ktReconcfg.ini")
#define THREADING_KEY _T("THREADING")
#define NUMBER_OF_THREADS _T("NUMBER_OF_THREADS")
const CString KTRECON_APP = L"KtRecon";

KtIniFileReader::KtIniFileReader()
{
    m_cfg_file_name = CONFIG_FILE_PATH;
}

KtIniFileReader::~KtIniFileReader()
{
    
}

int KtIniFileReader::GetNumberOfThreadsFromConfigFile( int &f_nNumOfThreads )
{
    try{
        if(TRUE == m_cfg_file_name.IsEmpty()){
			CString l_error_string(_T("Config file not present"));
		    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);	
            return(E_ERROR);
        }

        if(PathFileExists(m_cfg_file_name)) {
            
            CIniReader l_ini_file_name(m_cfg_file_name);
            
            CString l_csThreading  = l_ini_file_name.GetKeyValue(THREADING_KEY, KTRECON_APP);            
            if(l_csThreading.CompareNoCase(_T("DEFAULT")) == 0){               
				CString l_error_string(_T("Key value for THREADING is DEFAULT"));
		       CKtReconLogHelper::WriteToTraceLog(_T("MRI_KTRECON"), MRTraceMarker1, l_error_string);
                return(E_ERROR);

            }else if(l_csThreading.CompareNoCase(_T("MANUAL")) == 0){
                
                CString l_csNumOfThreads = l_ini_file_name.GetKeyValue(NUMBER_OF_THREADS, KTRECON_APP);
                char l_tmpStr[150] = {'\0'};
                memset(l_tmpStr, 0, 150 *(sizeof(char)));
                wcstombs(l_tmpStr, l_csNumOfThreads, 150);
                f_nNumOfThreads = atoi(l_tmpStr);
            
            }else{
				CString l_error_string(_T("Key value for THREADING not found in config file"));
		        CKtReconLogHelper::WriteToTraceLog(_T("MRI_KTRECON"), MRTraceMarker1, l_error_string);
                //Setting in config file: key \"THREADING\" was not found in the config file
                return(E_ERROR);              
            }            
        }else{            
            //path or file does not exist 
			CString l_error_string(_T("Config File not present or Incorrect path"));
		       CKtReconLogHelper::WriteToTraceLog(_T("MRI_KTRECON"), MRTraceMarker1, l_error_string);
            return(E_ERROR);
        }
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in GetNumberOfThreadsFromConfigFile"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);	
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}
/////////////////////////////////END OF FILE /////////////////////////////////////