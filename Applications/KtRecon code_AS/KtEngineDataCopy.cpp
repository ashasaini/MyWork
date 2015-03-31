// KtEngineDataCopy.cpp: implementation of the CKtEngineDataCopy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtEngineDataCopy.h"
#include "KtReconErrors.h"
#include "KtReconLogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define RAWCPY_SRC_HOST _T("mrengine00@/study/")
#define RAWCPY_DST_HOST _T(" mrsm00@/study/")

CKtEngineDataCopy::CKtEngineDataCopy()
{
    
}

CKtEngineDataCopy::~CKtEngineDataCopy()
{
    
}

int CKtEngineDataCopy::GetCommand(CString f_csStudyName, 
                                  int f_nProtIndx, 
                                  CString &f_csCommand)
{    
    try{
        if((TRUE == f_csStudyName.IsEmpty()) || (0 > f_nProtIndx)){
			CString l_error_string(_T("Study file name not present or invalid protocol index"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);	
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        //eg: "mrengine00@/study/Run10911.-1001:/1.5 mrsm00@/study/Run10911.-1001:/1.5 >>& /usr/tmp/ktRawCopy.log"
        f_csCommand.Empty();
        f_csCommand.Format(_T("%s%s/1.%d %s%s/1.%d"),RAWCPY_SRC_HOST,
                                                    f_csStudyName,
                                                    f_nProtIndx,
                                                    RAWCPY_DST_HOST,
                                                    f_csStudyName,
                                                    f_nProtIndx);
		CKtReconLogHelper::WriteToTraceLog(_T("MRI_KTRECON"), MRTraceMarker1, f_csCommand);
        //pending log command
        
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in GetCommand"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);	
        return(E_ERR_EXCEPTION);
    }    
    return(E_NO_ERROR);
}
/////////////////////////////////END OF FILE /////////////////////////////////////