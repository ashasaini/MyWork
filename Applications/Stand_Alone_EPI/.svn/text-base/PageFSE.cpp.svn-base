// PageTwo.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "PageFSE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageFSE property page

IMPLEMENT_DYNCREATE(CPageFSE, CPageBase/*CPropertyPage*/)

CPageFSE::CPageFSE() : CPageBase/*CPropertyPage*/(CPageFSE::IDD)
{
    //{{AFX_DATA_INIT(CPageFSE)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CPageFSE::~CPageFSE()
{
    //
}

void CPageFSE::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPageFSE)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageFSE, CPropertyPage)
    //{{AFX_MSG_MAP(CPageFSE)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageFSE message handlers

BOOL CPageFSE::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPageFSE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
//  HBRUSH hbr = CPageBase/*CPropertyPage*/::OnCtlColor(pDC, pWnd, nCtlColor);
    // TODO: Change any attributes of the DC here
    // TODO: Return a different brush if the default is not desired
    return hbr;
}
