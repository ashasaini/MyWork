// InfoView.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "FormStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormStatus

IMPLEMENT_DYNCREATE(CFormStatus, CFormView)

CFormStatus::CFormStatus()
    : CFormView(CFormStatus::IDD)
{
    //{{AFX_DATA_INIT(CFormStatus)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CFormStatus::~CFormStatus()
{
}

void CFormStatus::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFormStatus)
	DDX_Control(pDX, IDC_EDIT_STATUS, m_ctrlStatus);
    DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrlCloseButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormStatus, CFormView)
    //{{AFX_MSG_MAP(CFormStatus)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormStatus diagnostics

#ifdef _DEBUG
void CFormStatus::AssertValid() const
{
    CFormView::AssertValid();
}

void CFormStatus::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormStatus message handlers
int CFormStatus::setErrorMessage(CString &f_csErrorMsg)
{
    m_ctrlCloseButton.ShowWindow(TRUE);
    m_nCnt++;
//    int nLength = m_ctrlStatus.GetWindowTextLength();
    // put the selection at the end of text
    m_ctrlStatus.SetSel(0, 0);
    // replace the selection
    f_csErrorMsg = f_csErrorMsg + "\r\n";
   m_ctrlStatus.ReplaceSel(f_csErrorMsg);
    return 0;
}

void CFormStatus::OnButtonClose()
{
    // TODO: Add your control notification handler code here
    // Delete all of the text.
    m_ctrlStatus.SetSel(0, -1);
    m_ctrlStatus.Clear();
    m_ctrlStatus.SetWindowText("");
    m_ctrlCloseButton.ShowWindow(FALSE);
    m_nCnt = 0;
}

void CFormStatus::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
//     m_ctrlStatusList.DeleteAllItems();
    m_nCnt = 0;
//     int width = 400;
//     m_ctrlStatusList.InsertColumn(0, _T("Staus"), LVCFMT_LEFT, width);
//     m_ctrlStatusList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES);	
}
