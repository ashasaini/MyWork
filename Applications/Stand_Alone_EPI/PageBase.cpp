// PageBase.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "PageBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageBase property page

IMPLEMENT_DYNCREATE(CPageBase, CPropertyPage)

CPageBase::CPageBase(UINT IDD/*, CWnd* pParent *//*=NULL*/)
    : CPropertyPage(IDD/*, pParent*/)

//CPageBase::CPageBase() : CPropertyPage(CPageBase::IDD)
{
    //{{AFX_DATA_INIT(CPageBase)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CPageBase::CPageBase()
{
    //
}

CPageBase::~CPageBase()
{
}

void CPageBase::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPageBase)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageBase, CPropertyPage)
    //{{AFX_MSG_MAP(CPageBase)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageBase message handlers
#define     DEF_CLR_DIALOG_BACK_COLOR  RGB( 9,86,123 )

HBRUSH CPageBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
    HBRUSH m_brDlgBrush;
    //
    COLORREF m_clrBackColor = DEF_CLR_DIALOG_BACK_COLOR;
    m_brDlgBrush    = CreateSolidBrush(m_clrBackColor);

    //
    //
    if(CTLCOLOR_DLG == nCtlColor) {
        return m_brDlgBrush;
    }

    if(CTLCOLOR_BTN == nCtlColor) {
        pDC->SetBkMode(TRANSPARENT);
        //
        pDC->SetBkColor(m_clrBackColor);
        //
        return m_brDlgBrush;
    }

    if(CTLCOLOR_BTN == nCtlColor) {
        //pDC->SetBkMode(TRANSPARENT);
        pDC->SetBkColor(m_clrBackColor);
        return m_brDlgBrush;
    }

    // TODO: Change any attributes of the DC here
    // TODO: Return a different brush if the default is not desired
    return hbr;
}
