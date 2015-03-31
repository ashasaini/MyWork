// ConfirmationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdvCardiacGL.h"
#include "ConfirmationDlg.h"
#include "AdvCardiacGLView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmationDlg dialog


CConfirmationDlg::CConfirmationDlg(CWnd* pParent /*=NULL*/)
	: DialogEx(CConfirmationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfirmationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfirmationDlg::DoDataExchange(CDataExchange* pDX)
{
	DialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmationDlg)
	DDX_Control(pDX, IDC_STATIC_LABEL, m_label);
	DDX_Control(pDX, IDOK, m_ConfirmOk);
    DDX_Control(pDX, IDCANCEL, m_ConfirmCancel);
	//}}AFX_DATA_MAP
}
void CConfirmationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    /*TIPREP_TRACE(CTiPrepDlg::OnActivate,MRTraceMarker9);*/
    DialogEx::OnActivate(nState, pWndOther, bMinimized);
	if(WA_INACTIVE != nState) {
    ACGAppController::GetInstance()->ChangeWindowZOrder(this->GetSafeHwnd());
	}
}

BEGIN_MESSAGE_MAP(CConfirmationDlg, DialogEx)
	//{{AFX_MSG_MAP(CConfirmationDlg)
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmationDlg message handlers

void CConfirmationDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	DialogEx::OnOK();
}

void CConfirmationDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
    try {
         ACGAppController::GetInstance()->UnRegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CString l_csStr(_T("Excxeption occuered in confirmation dlg"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
    }
	DialogEx::OnCancel();
}
///////////////////////////////// END OF FILE /////////////////////////////////

HBRUSH CConfirmationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: Return a different brush if the default is not desired
	HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    
    // TODO: Change any attributes of the DC here
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return m_ConfirmationDlg_brush;
    } else {
    
    }
    // TODO: Return a different brush if the default is not desired
    return hbr;
}
BOOL CConfirmationDlg::OnInitDialog() 
{
	DialogEx::OnInitDialog();
    
    // TODO: Add extra initialization here
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_ConfirmationDlg_brush.CreateSolidBrush(l_window_color.crClientColor);
    
    SetRibbonHeight(0);

	m_ConfirmOk.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_ConfirmCancel.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
	CFont  l_font;
    m_label.SetFont(&l_font, true);
    
     ACGAppController::GetInstance()->AddZOrderLayer();
     ACGAppController::GetInstance()->RegisterWindowZOrder(m_hWnd);

	ACGAppController::GetInstance()->ChangeWindowZOrder(this->GetSafeHwnd());
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
