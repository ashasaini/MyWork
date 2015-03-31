// InfoView.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "InfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormInfo

IMPLEMENT_DYNCREATE(CFormInfo, CFormView)

CFormInfo::CFormInfo()
    : CFormView(CFormInfo::IDD)
{
    //{{AFX_DATA_INIT(CFormInfo)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CFormInfo::~CFormInfo()
{
}

void CFormInfo::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFormInfo)
    DDX_Control(pDX, IDC_STATIC_IMAGE_INFO, m_ctrlImgInfo);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormInfo, CFormView)
    //{{AFX_MSG_MAP(CFormInfo)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormInfo diagnostics

#ifdef _DEBUG
void CFormInfo::AssertValid() const
{
    CFormView::AssertValid();
}

void CFormInfo::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormInfo message handlers
int CFormInfo::setErrorMessage(CString &f_csErrorMsg)
{
    m_ctrlImgInfo.SetWindowText(f_csErrorMsg);
    return 0;
}
