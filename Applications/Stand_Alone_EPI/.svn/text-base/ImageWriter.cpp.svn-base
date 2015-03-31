// ImageWriter.cpp: implementation of the CImageWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "ImageWriter.h"
#include <WINDOWS.H>
//#include "win32a.inc"
#include "EPILogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageWriter::CImageWriter()
{
}

CImageWriter::~CImageWriter()
{
}

int CImageWriter::createFolders(int l_nProtocolNo,
                                int l_nSubProtNo,
                                int l_nSliceGrpNo,
                                int l_nSliceNo,
                                int l_nImageNo, CString &f_csFilePath)
{
    CString l_csFilePath(_T("C:/EPI_Data"));
    CString l_csTemp;
    int l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csFilePath = l_csFilePath + "/";
    l_csFilePath = l_csFilePath + m_csStudyName;
    l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csTemp.Format("/1.%d", l_nProtocolNo);
    l_csFilePath = l_csFilePath + l_csTemp;
    l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csTemp.Format("/2.%d", l_nSubProtNo);
    l_csFilePath = l_csFilePath + l_csTemp;
    l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csTemp.Format("/3.%d", l_nSliceGrpNo);
    l_csFilePath = l_csFilePath + l_csTemp;
    l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csTemp.Format("/5.%d", l_nSliceNo);
    l_csFilePath = l_csFilePath + l_csTemp;
    l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csTemp.Format("/6.%d", l_nImageNo);
    l_csFilePath = l_csFilePath + l_csTemp;
    l_nStatus = CreateDirectory(l_csFilePath, NULL);
    l_csTemp.Format("/");
    f_csFilePath = l_csFilePath + l_csTemp;
    //f_csFilePath = l_csFilePath + f_csFilePath;
    return E_NO_ERROR;
}
