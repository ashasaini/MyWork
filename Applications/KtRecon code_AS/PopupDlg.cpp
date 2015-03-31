// PopupDlg.cpp: implementation of the CPopupDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PopupDlg.h"
#include "KtReconTrace.h"
#include "KtReconLogHelper.h"
#include "KtReconView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CPopupDlg
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CPopupDlg::CPopupDlg(UINT IDD, CWnd* pParent /*=NULL*/)
    : DialogEx(IDD, pParent)
{
    //KTRECON_TRACE(CPopupDlg::CPopupDlg);
    //{{AFX_DATA_INIT(CPopupDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_pLabel_Font.CreatePointFont(100, _T("Arial"));
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CPopupDlg::DoDataExchange(CDataExchange* pDX)
{
    //KTRECON_TRACE(CPopupDlg::DoDataExchange);
    DialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPopupDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupDlg, DialogEx)
ON_WM_ACTIVATE()
    //{{AFX_MSG_MAP(CPopupDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg message handlers

/*************************************************************
* Method:       OnCancel
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPopupDlg::OnCancel()
{
    //KTRECON_TRACE(CPopupDlg::OnCancel);
    DialogEx::OnCancel();
}

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CPopupDlg::OnInitDialog()
{
    DialogEx::OnInitDialog();
    HWND hwndOwner;

    if((hwndOwner = GetParent()->GetSafeHwnd()) == NULL) {
        hwndOwner = GetDesktopWindow()->GetSafeHwnd();
    }

    CRect rc, rcDlg, rcOwner;
    ::GetWindowRect(hwndOwner, &rcOwner);
    GetWindowRect(&rcDlg);
    CopyRect(&rc, &rcOwner);
    OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
    OffsetRect(&rc, -rc.left, -rc.top);
    OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
    MoveWindow(rcOwner.left + (rc.right / 2),
               rcOwner.top + (rc.bottom / 3), rcDlg.Width(), rcDlg.Height());
    return TRUE;  
}

//****************************Method Header***************************
//Method Name   : CInPlaceFrame
//Purpose       : Destructor
//********************************************************************
CPopupDlg::~CPopupDlg()
{
    //
}

/*************************************************************
* Method:    	PreTranslateMessage
* Description: 	This function is used 
* Parameter: 	MSG * pMsg :
* Returns:   	BOOL
*************************************************************/
BOOL CPopupDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
            return TRUE;
        }
    }

    return DialogEx::PreTranslateMessage(pMsg);
}

/*************************************************************
* Method:    	OnActivate
* Description: 	This function is used 
* Parameter: 	UINT nState :
* Parameter: 	CWnd * pWndOther :
* Parameter: 	BOOL bMinimized :
* Returns:   	void
*************************************************************/
void CPopupDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    //KTRECON_TRACE(CTiPrepDlg::OnActivate);
    DialogEx::OnActivate(nState, pWndOther, bMinimized);
//    (CKtReconView::GetView())->ChangeWindowZOrder(this->GetSafeHwnd());
   
}