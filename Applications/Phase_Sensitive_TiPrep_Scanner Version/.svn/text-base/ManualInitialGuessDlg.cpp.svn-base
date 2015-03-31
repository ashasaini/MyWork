// ManualInitialGuessDlg.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "ManualInitialGuessDlg.h"
#include "TiPrepView.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "ErrorDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualInitialGuessDlg dialog


/*************************************************************
* Method:    	CManualInitialGuessDlg
* Description: 	This function is constructor
* Parameter: 	CWnd * pParent :
* Returns:   	
*************************************************************/
CManualInitialGuessDlg::CManualInitialGuessDlg(CWnd* pParent /*=NULL*/)
    : CPopupDlg(CManualInitialGuessDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CManualInitialGuessDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
	    m_dAFactor = 0.0;
		m_dBFactor = 0.0;
		m_dMFactor = 0.0;
		m_dNFactor = 0.0;
        m_nEqnFlag = 0;
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}
CManualInitialGuessDlg& CManualInitialGuessDlg::operator = (CManualInitialGuessDlg& f_manualinitialguess)
{
    if(this == &f_manualinitialguess) {
        return(*this);
    } 
	m_nEqnFlag = f_manualinitialguess.m_nEqnFlag;
	m_dAFactor = f_manualinitialguess.m_dAFactor;
	m_dBFactor = f_manualinitialguess.m_dBFactor;
	m_dMFactor = f_manualinitialguess.m_dMFactor;
	m_dNFactor = f_manualinitialguess.m_dNFactor;
    return (*this);
}


void CManualInitialGuessDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CManualInitialGuessDlg)
    DDX_Control(pDX, IDCANCEL, m_buttonExit);
    DDX_Control(pDX, IDC_BUTTON_INITIAL_GUESS_APPLY, m_buttonManualApply);
    DDX_Control(pDX, IDC_STATIC_A_FCTOR, m_lblAFactor);
    DDX_Control(pDX, IDC_STATIC_B_FACTOR, m_lblBFactor);
    DDX_Control(pDX, IDC_STATIC_M_FACTOR, m_lblMFactor);
    DDX_Control(pDX, IDC_STATIC_N_FACTOR, m_lblNFactor);
    DDX_Control(pDX, IDC_EDIT_A_FACTOR, m_editAFactor);
    DDX_Control(pDX, IDC_EDIT_B_FACTOR, m_editBFactor);
    DDX_Control(pDX, IDC_EDIT_M_FACTOR, m_editMFactor);
    DDX_Control(pDX, IDC_EDIT_N_FACTOR, m_editNFactor);
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualInitialGuessDlg, CPopupDlg)
    //{{AFX_MSG_MAP(CManualInitialGuessDlg)
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_INITIAL_GUESS_APPLY, OnButtonInitialGuessApply)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualInitialGuessDlg message handlers

/*************************************************************
* Method:    	OnInitDialog
* Description: 	This function is used 
* Returns:   	BOOL
*************************************************************/
BOOL CManualInitialGuessDlg::OnInitDialog()
{
    TIPREP_TRACE(CManualInitialGuessDlg::OnInitDialog,MRTraceMarker9);
    CPopupDlg::OnInitDialog();
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    SetRibbonHeight(0);
    InitializeControls();
    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
    return TRUE;  
}

/*************************************************************
* Method:       OnCtlColor
* Description:  This function is used
* Parameter:    CDC * pDC
* Parameter:    CWnd * pWnd
* Parameter:    UINT nCtlColor
* Returns:      HBRUSH
*************************************************************/
HBRUSH CManualInitialGuessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPopupDlg::OnCtlColor(pDC, pWnd, nCtlColor);

    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return (HBRUSH)m_list_brush;
    } else {
        //
    }

    return hbr;
}

/*************************************************************
* Method:       OnPaint
* Description:  This function is used
* Returns:      void
*************************************************************/
void CManualInitialGuessDlg::OnPaint()
{
    //CPaintDC dc(this); // device context for painting

    if(IsIconic()) {
        CPaintDC dc(this); // device context for painting
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CPaintDC dc(this); // device context for painting
        //CPopupDlg::OnPaint();
    }
}

/*************************************************************
* Method:    	OnButtonInitialGuessApply
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualInitialGuessDlg::OnButtonInitialGuessApply()
{
//     CString l_temp_str;
//     BOOL l_bStatus = FALSE;

    try{
        int l_status = ValidateInitialValues();
        if(l_status != E_NO_ERROR) {
            return;
        }
        
        int l_nStatus = CTiPrepView::GetView()->performCurveFitForInitialGuessValues(m_dAFactor,
            m_dBFactor,
            m_dMFactor,
            m_dNFactor,m_nEqnFlag);
        if(E_NO_ERROR != l_nStatus){
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Failed to Curve fit"),__LINE__,__FILE__);
            return;
        }
//         l_nStatus = (CTiPrepView::GetView())->OnApply(m_nEqnFlag);
//         if (E_NO_ERROR != l_nStatus){
//             CTiPrepLogHelper::WriteToErrorLog
//                 (_T("Failed Curve fit"),__LINE__,__FILE__);
//             return;
//         }
    }
    catch(...)
    {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in OnButtonInitialGuessApply()"),__LINE__,__FILE__);
    }
}

/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualInitialGuessDlg::OnCancel()
{
    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       InitializeControls
* Description:  This function is used to initialize dialog controls.
* Returns:      void
*************************************************************/
void CManualInitialGuessDlg::InitializeControls()
{
    TIPREP_TRACE(CManualInitialGuessDlg::InitializeControls,MRTraceMarker9);
    CRect l_rcHideBtn;
    CRect l_rcMovedCtrl;
    double l_dAFactor = 0.0;
    double l_dBFactor = 0.0;
    double l_dMFactor = 0.0;
    double l_dNFactor = 0.0;
    CString l_temp_str;
    m_editAFactor.SetFont(_T("Arial"), 10, true);
    m_editBFactor.SetFont(_T("Arial"), 10, true);
    m_editMFactor.SetFont(_T("Arial"), 10, true);
    m_editNFactor.SetFont(_T("Arial"), 10, true);
    m_buttonManualApply.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_buttonManualApply.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_buttonExit.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_lblAFactor.SetFont(&m_fontPhaseCorrection, true);
    m_lblBFactor.SetFont(&m_fontPhaseCorrection, true);
    m_lblMFactor.SetFont(&m_fontPhaseCorrection, true);
    m_lblNFactor.SetFont(&m_fontPhaseCorrection, true);
    CTiPrepView::GetView()->getInitialGuessValues(l_dAFactor,
            l_dBFactor,
            l_dMFactor,
            l_dNFactor);
    //l_dAFactor = l_dAFactor + 0.05;
    l_temp_str.Format(_T("%0.1f"), l_dAFactor /*+ 0.05*/);
    m_editAFactor.SetWindowText(l_temp_str);
    //l_dBFactor = l_dBFactor + 0.05;
    l_temp_str.Format(_T("%0.1f"), l_dBFactor/* + 0.05*/);
    m_editBFactor.SetWindowText(l_temp_str);

    if(0 == m_nEqnFlag || 1 == m_nEqnFlag || 3 == m_nEqnFlag) {
        m_lblMFactor.ShowWindow(FALSE);
        m_editMFactor.ShowWindow(FALSE);
        m_lblNFactor.ShowWindow(FALSE);
        m_editNFactor.ShowWindow(FALSE);
        m_editBFactor.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        int l_nRectTop = l_rcHideBtn.bottom + 20;
        m_buttonManualApply.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonManualApply.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_buttonExit.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonExit.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        SetWindowPos(&wndTop, l_rcMovedCtrl.left + 900 , l_rcMovedCtrl.right + 200 , 300, 200, SWP_NOOWNERZORDER);
    } else if(2 == m_nEqnFlag) {
        m_lblNFactor.ShowWindow(FALSE);
        m_editNFactor.ShowWindow(FALSE);
        m_editMFactor.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        int l_nRectTop = l_rcHideBtn.bottom + 20;
        m_buttonManualApply.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonManualApply.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_buttonExit.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonExit.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        SetWindowPos(&wndTop, l_rcMovedCtrl.left + 900 , l_rcMovedCtrl.right + 200 , 300, 250, SWP_NOOWNERZORDER);
//        l_dMFactor = l_dMFactor + 0.05;
        l_temp_str.Format(_T("%0.1f"), l_dMFactor /*+ 0.05*/);
        m_editMFactor.SetWindowText(l_temp_str);
    } else if(4 == m_nEqnFlag) {
        m_lblMFactor.ShowWindow(FALSE);
        m_editMFactor.ShowWindow(FALSE);
        m_editBFactor.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        int l_nRectTop = l_rcHideBtn.bottom + 20;
        m_editNFactor.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_editNFactor.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_lblNFactor.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_lblNFactor.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_editNFactor.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        l_nRectTop = l_rcHideBtn.bottom + 20;
        m_buttonManualApply.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonManualApply.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_buttonExit.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonExit.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        SetWindowPos(&wndTop, l_rcMovedCtrl.left + 900 , l_rcMovedCtrl.right + 200 , 300, 250, SWP_NOOWNERZORDER);
//        l_dNFactor = l_dNFactor + 0.05;
        l_temp_str.Format(_T("%0.1f"), l_dNFactor /*+ 0.05*/);
        m_editNFactor.SetWindowText(l_temp_str);
    }
}

/*************************************************************
* Method:    	ValidateValues
* Description: 	This function is used 
* Parameter: 	CString f_csValue :
* Parameter: 	int f_nCondition :
* Parameter: 	double & f_dValue :
* Returns:   	BOOL
*************************************************************/
BOOL CManualInitialGuessDlg::ValidateValues(CString &f_csValue, int f_nCondition, double &f_dValue)
{
    try{
        char val[100];
        int l_nDotCnt = 0;
        
        if(f_csValue.IsEmpty()) {
            return FALSE;
        }
        
        wcstombs(val, f_csValue, 100);
        int l_nstrlength = f_csValue.GetLength();
        
        for(int l_ni = 0; l_ni < l_nstrlength; l_ni++) {
            if(1 != f_nCondition) {
                if('-' == val[l_ni] && l_ni == 0) {
                    l_ni++;
                }
            }
            
            if('.' == val[l_ni]) {
                l_nDotCnt++;
                l_ni++;
            }
            
            if(FALSE == isdigit(val[l_ni])) {
                return FALSE;
            }
        }
        
        if(1 < l_nDotCnt) {
            return FALSE;
        }
        
        f_dValue = atof(val);
        return TRUE;
        
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in manual initial guess"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:    	getEqnFlag
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CManualInitialGuessDlg::getEqnFlag() const
{
    return m_nEqnFlag;
}

/*************************************************************
* Method:    	setEqnFlag
* Description: 	This function is used 
* Parameter: 	int val :
* Returns:   	void
*************************************************************/
void CManualInitialGuessDlg::setEqnFlag(int val)
{
    m_nEqnFlag = val;
}

/*************************************************************
* Method:    	SetDlgPos
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualInitialGuessDlg::SetDlgPos()
{
    RECT desktop_rect;
    CRect tidlg_rect;
    GetDesktopWindow()->GetWindowRect(&desktop_rect);
    GetWindowRect(&tidlg_rect);
    long x_pos = (desktop_rect.right - tidlg_rect.Width() - 15);
    long y_pos = desktop_rect.top + 15;//(desktop_rect.bottom / 2) - y_size / 2;
    MoveWindow(x_pos, y_pos, tidlg_rect.Width(), tidlg_rect.Height());
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

BOOL CManualInitialGuessDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        ValidateInitialValues();	//AS/Added
        return TRUE;
    }
    else if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE){
        return TRUE;
    }	
	return CPopupDlg::PreTranslateMessage(pMsg);
}
int CManualInitialGuessDlg::ValidateInitialValues() 
{

    CString l_temp_str;
    BOOL l_bStatus = FALSE;
    m_editAFactor.GetWindowText(l_temp_str);
    l_bStatus = ValidateValues(l_temp_str, 0, m_dAFactor);
    
    if(TRUE != l_bStatus) {
        CErrorDlg dlg;
        CString l_csTemp(_T("Please enter valid values for A"));
        dlg.SetErrorMessage(l_csTemp);
        dlg.DoModal();
        return E_ERROR;
    }
    
    m_editBFactor.GetWindowText(l_temp_str);
    l_bStatus = ValidateValues(l_temp_str, 1, m_dBFactor);
    
    if(TRUE != l_bStatus) {
        CErrorDlg dlg;
        CString l_csTemp(_T("Please enter valid values for T1(ms)"));
        dlg.SetErrorMessage(l_csTemp);
        dlg.DoModal();
        return E_ERROR;
    }
    
    if(2 == m_nEqnFlag) {
        m_editMFactor.GetWindowText(l_temp_str);
        l_bStatus = ValidateValues(l_temp_str, 2, m_dMFactor);
        
        if(TRUE != l_bStatus) {
            CErrorDlg dlg;
            CString l_csTemp(_T("Please enter valid values for B"));
            dlg.SetErrorMessage(l_csTemp);
            dlg.DoModal();
            return E_ERROR;
        }
    } else if(4 == m_nEqnFlag) {
        m_editNFactor.GetWindowText(l_temp_str);
        l_bStatus = ValidateValues(l_temp_str, 4, m_dNFactor);
        
        if(TRUE != l_bStatus) {
            CErrorDlg dlg;
            CString l_csTemp(_T("Please enter valid values for N"));
            dlg.SetErrorMessage(l_csTemp);
            dlg.DoModal();
            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}

CManualInitialGuessDlg::~CManualInitialGuessDlg()
{
	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in manual initial guess"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}
}
