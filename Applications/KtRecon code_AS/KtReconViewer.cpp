// KtReconViewer.cpp: implementation of the CKtReconViewer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtReconViewer.h"
#include "KtReconErrors.h"
#include "CommonDefinitions.h"
#include "KtReconLogHelper.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtReconViewer::CKtReconViewer()
{
    IsCalledFromKtReconButton = false;
}

CKtReconViewer::~CKtReconViewer()
{
    
}
int CKtReconViewer::GetCommand( CString f_csStudyName, int f_nProtIndx, CString &f_csCommand )
{
    try{
        if((TRUE == f_csStudyName.IsEmpty()) || (0 > f_nProtIndx)){
			CString l_error_string(_T("Incorrect function parameter"));
		    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        f_csCommand.Empty();
        if(true == IsCalledFromKtReconButton){
            
            //eg: "c:/usr/local/bin/ktReconViewer.exe /study/k-t_recon/dataRun40042.-1959.3"
            f_csCommand.Format(_T(" %Sk-t_recon/data%s.%d"),STUDY,f_csStudyName,f_nProtIndx);
            
        }else{
            
            //eg: "c:/usr/local/bin/ktReconViewer.exe"
            f_csCommand.Format(_T(""));//it will be empty in this case
            //as no parameter is passed when invoked from View button
            
        }
		CKtReconLogHelper::WriteToTraceLog(_T("MRI_KTRECON"), MRTraceMarker1, f_csCommand);
        //log command
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in GetCommand"));
		 CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERR_EXCEPTION);        
    }
    return(E_NO_ERROR);
}

void CKtReconViewer::SetIsCalledFromKtReconButton( bool f_bflag )
{
    IsCalledFromKtReconButton = f_bflag;
}
/////////////////////////////////END OF FILE /////////////////////////////////////