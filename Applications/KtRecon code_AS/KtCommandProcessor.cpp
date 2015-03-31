// KtCommandProcessor.cpp: implementation of the CKtCommandProcessor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KtCommandProcessor.h"
#include "KtReconErrors.h"
#include "KtReconLogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtCommandProcessor::CKtCommandProcessor()
{

}

CKtCommandProcessor::~CKtCommandProcessor()
{

}

/*************************************************************
* Method:    	GetCommand
* Description: 	This function is used 
* Parameter: 	CString f_sStudyName :
* Parameter: 	int f_nProtIndx :
* Parameter: 	CString & f_csCommand :
* Returns:   	int
*************************************************************/
int CKtCommandProcessor::GetCommand( CString f_csStudyName, int f_nProtIndx, CString &f_csCommand )
{
    UNREFERENCED_PARAMETER(f_csStudyName);
    UNREFERENCED_PARAMETER(f_nProtIndx);
    UNREFERENCED_PARAMETER(f_csCommand);
    try{
        //handled in derived classes        
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in GetCommand"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);

        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
/////////////////////////////////END OF FILE /////////////////////////////////////

