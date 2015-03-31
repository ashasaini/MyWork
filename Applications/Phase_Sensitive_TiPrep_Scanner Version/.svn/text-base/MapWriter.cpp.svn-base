// MapWriter.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "MapWriter.h"
#include "TiPrepLogHelper.h"
#include <IO.H>
#include <FCNTL.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWriter

CMapWriter::CMapWriter()
{
    //
}

CMapWriter::~CMapWriter()
{
    //
}


BEGIN_MESSAGE_MAP(CMapWriter, CWnd)
//{{AFX_MSG_MAP(CMapWriter)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:    	WriteToFile
* Description: 	This function is used 
* Parameter: 	CString f_csFileName :
* Parameter: 	void * f_dImgBuf :
* Parameter: 	unsigned long f_nImgSize :
* Returns:   	int
*************************************************************/
int CMapWriter::WriteToFile(CString &f_csFileName, void *f_dImgBuf, unsigned long f_nImgSize)
{
    try{
    if(f_nImgSize <= 0) {
        CTiPrepLogHelper::WriteToErrorLog(_T("WriteToFile(): Image size <= 0"), __LINE__, __FILE__);
        return(E_NO_DATA_FOUND_MAP);
    }
    
    if(NULL == f_dImgBuf) {
        CTiPrepLogHelper::WriteToErrorLog(_T("WriteToFile(): Function parameter NULL error"), __LINE__, __FILE__);
        return(E_ERROR_NULL_FUNCPARAMS);
    }
    
    if(f_csFileName.IsEmpty() == TRUE) {
        CTiPrepLogHelper::WriteToErrorLog(_T("WriteToFile(): Filename is empty"), __LINE__, __FILE__);
        return(E_ERROR_NULL_FUNCPARAMS);
    }
    
    char charptr[100] = {'\0'};
    wcstombs(charptr, f_csFileName, 100);
    int fd = open(charptr, (O_WRONLY | O_BINARY | O_TRUNC | O_CREAT));
    
    if(fd < 0) {
        CTiPrepLogHelper::WriteToErrorLog(_T("WriteToFile(): Failed to open file"), __LINE__, __FILE__);
        return(E_FILE_WRITE);
    }
    
    unsigned long l_nSizeWritten;
    l_nSizeWritten = write(fd, f_dImgBuf, f_nImgSize);
    
    if(l_nSizeWritten != f_nImgSize) {
        CString l_csTemp(_T(""));
        l_csTemp.Format(_T("WriteToFile(): could not write data"));
        CTiPrepLogHelper::WriteToErrorLog(_T("WriteToFile(): Failed to open file"), __LINE__, __FILE__);
        l_csTemp.Format(_T("WriteToFile(): size written is %d and actual size is %d"), l_nSizeWritten, f_nImgSize);
        CTiPrepLogHelper::WriteToErrorLog(_T("WriteToFile(): Failed to open file"), __LINE__, __FILE__);
    }
    
    close(fd);
    return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in WriteToFile()"),__LINE__,__FILE__);
        return(E_ERROR);
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
