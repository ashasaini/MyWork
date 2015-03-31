// ImgFileHandler.cpp: implementation of the CImgFileHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "ImgFileHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImgFileHandler::CImgFileHandler()
{
}

CImgFileHandler::~CImgFileHandler()
{
}

CString CImgFileHandler::getStudyName() const
{
    return m_csStudyName;
}

void CImgFileHandler::setStudyName(CString f_csStudyFileName)
{
    int l_nTotalLen = f_csStudyFileName.GetLength();
    int l_nCount = f_csStudyFileName.ReverseFind(_T('\\'));
    CString l_csStudyFileName = f_csStudyFileName.Right(l_nTotalLen - (l_nCount + 1));
    m_csStudyName = l_csStudyFileName;
}

