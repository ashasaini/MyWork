// KtVROMProc.cpp: implementation of the CKtVROMProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtVROMProc.h"
#include "KtReconErrors.h"
#include "CommonDefinitions.h"
#include "KtIniFileReader.h"
#include "KtReconLogHelper.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtVROMProc::CKtVROMProc()
{
    
}

CKtVROMProc::~CKtVROMProc()
{
    
}
int CKtVROMProc::GetCommand( CString f_csStudyName, int f_nProtIndx, CString &f_csCommand )
{
    try{   
        if((TRUE == f_csStudyName.IsEmpty()) || (0 > f_nProtIndx)){
			CString l_error_string(_T("Protocol name is incorrect or invalid protocol index"));
		    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERROR);
        }
        
        //"c:/usr/local/bin/VROMProcessor_kt.exe -oP -p/study/k-t_recon/ 2 /study/Run40042.-1959 3 "
        int l_nNumOfThreads = 1;
        int l_nMaxNumOfThreadsAllowed = 1;
        int l_nStatus = E_NO_ERROR;
        
        l_nStatus = GetMaxNumOfThreadsAllowed(l_nMaxNumOfThreadsAllowed);
        if(E_NO_ERROR != l_nStatus){
            l_nMaxNumOfThreadsAllowed = MAX_NO_OF_THREADS;
        }

        KtIniFileReader l_CfgReader;
        l_nStatus = l_CfgReader.GetNumberOfThreadsFromConfigFile(l_nNumOfThreads);
        
        if((E_NO_ERROR != l_nStatus) || (0 >= l_nNumOfThreads) || (l_nNumOfThreads > l_nMaxNumOfThreadsAllowed)){
            l_nNumOfThreads = l_nMaxNumOfThreadsAllowed;
            //no need to return error
        }

        f_csCommand.Empty();
        f_csCommand.Format(_T(" -oP -p %Sk-t_recon %d %S%s %d"),STUDY,l_nNumOfThreads,STUDY,f_csStudyName,f_nProtIndx);
        CKtReconLogHelper::WriteToTraceLog(_T("MRI_KTRECON"), MRTraceMarker1, f_csCommand);
        //log command
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in GetCommand"));
		 CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int CKtVROMProc::GetMaxNumOfThreadsAllowed( int &f_NumOfThreads )
{
    try{       
        char * l_cNumOfThreadsSys = getenv(NO_OF_THREADS);
        
        if(0 == strcmp(l_cNumOfThreadsSys, "") || (NULL == l_cNumOfThreadsSys)) {
            f_NumOfThreads = MAX_NO_OF_THREADS;
        } else {
            f_NumOfThreads = atoi(l_cNumOfThreadsSys);
        }
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in GetMaxNumOfThreadsAllowed"));
		 CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
/////////////////////////////////END OF FILE /////////////////////////////////////