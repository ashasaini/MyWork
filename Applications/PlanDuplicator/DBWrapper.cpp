// DBWrapper.cpp: implementation of the CDBWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "planduplicator.h"
#include "DBWrapper.h"
#include "PDTrace.h"
#include "PDLogHelper.h"
#include "vfcDBMgr.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBWrapper::CDBWrapper()
{
    PD_TRACE(CDBWrapper::CDBWrapper);
}

CDBWrapper::~CDBWrapper()
{
    PD_TRACE(CDBWrapper::~CDBWrapper);
}

/*************************************************************
* Method:    	getTotalNumImages
* Description: 	This function is used to get total images in given protocol
* Parameter: 	char * f_cstudyName :
* Parameter: 	int & f_nportIndex :
* Parameter: 	vector<CString> & f_csFileName :
* Parameter: 	int * f_nNumImages :
* Returns:   	int
*************************************************************/
int CDBWrapper::getTotalNumImages(
    const char *f_cstudyName,
    const int& f_nportIndex,
    vector<CString> &f_csFileName,
    int *f_nNumImages)
{
    PD_TRACE(CDBWrapper::getTotalNumImages);

    int l_nNumImges = 0;
    int l_nstatus = E_ERROR;
    CvfcDBMgr l_vfc_mgr;
    l_nstatus = l_vfc_mgr.protTraverse(f_cstudyName, f_nportIndex,
    f_csFileName, l_nNumImges);

    if (E_NO_ERROR != l_nstatus) {
        CString l_cserrmsg (_T("Protocol traverse error"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }

    *f_nNumImages =  l_nNumImges;
    return(E_NO_ERROR);
}

////////////////////////    End Of File     /////////////////////////////////
