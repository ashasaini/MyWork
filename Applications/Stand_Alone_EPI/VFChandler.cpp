// VFChandler.cpp: implementation of the CVFChandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EPI.h"
#include "VFChandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVFChandler::CVFChandler(): m_vfd(NULL), m_csStudyFileName(_T(""))
{
}

CVFChandler::~CVFChandler()
{
}

CString CVFChandler::StudyFileName() const
{
    return m_csStudyFileName;
}

void CVFChandler::StudyFileName(CString val)
{
    m_csStudyFileName = val;
}

int CVFChandler::OpenDBFile()
{
    return 0;
}

int CVFChandler::CloseDBFile()
{
    m_vfd = NULL;
    return 0;
}
