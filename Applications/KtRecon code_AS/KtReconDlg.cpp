// KtReconDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KtRecon.h"
#include "KtReconView.h"
#include "KtReconDlg.h"
#include "KtReconErrors.h"
#include "ErrorDlg.h"
#include "KtReconLogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKtReconDlg dialog

CKtReconDlg::CKtReconDlg(CWnd* pParent /*=NULL*/)
: DialogEx(CKtReconDlg::IDD, pParent),m_ncurrscannedrowno(-1)
{
    //{{AFX_DATA_INIT(CKtReconDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    
    //     if(m_KtReconController)
    //     {
    //         delete m_KtReconController;
//     m_KtReconController = NULL;
    //     }
    //     
//     if(NULL == m_KtReconController){
      //  m_KtReconController = new CKtReconController();
//         if(NULL == m_KtReconController){
//             CString l_error_string(_T("CKtReconDlg() :m_KtReconController is NULL"));
//             CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            // CKtReconLogHelper::WriteToErrorLog(_T("CKtReconDlg():m_KtReconController NULL"),__LINE__, __FILE__);
//         } else {
//             m_KtReconController->SetDlg(this);
//         }
//     }  
}

CKtReconDlg::~CKtReconDlg()
{
    try{
        if(NULL != m_KtReconController){
            delete m_KtReconController;
            m_KtReconController = NULL;
        }           
    }catch(...)
    {
        CString l_error_string(_T("Exception occured in ~CKtReconDlg "));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        
    }
}

void CKtReconDlg::DoDataExchange(CDataExchange* pDX)
{
    DialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CKtReconDlg)
    DDX_Control(pDX, IDC_STATIC_PROTOCOL_DISPLAY_FRAME, m_scannedProtDisplay);
    DDX_Control(pDX, IDC_STATIC_DATA_OPERATION_FRAME, m_operations);
    
    DDX_Control(pDX, IDC_BUTTON_GET_DATA, m_getDataButton);
    DDX_Control(pDX, IDC_BUTTON_KTRECON, m_ktReconButton);
    DDX_Control(pDX, IDC_BUTTON_VIEW, m_viewerButton);
    DDX_Control(pDX, IDC_BUTTON_EXIT, m_exitButton);
    
    DDX_Control(pDX, IDC_LIST_SCANNED_PROTOCOL, m_scannedProtocolListbox);
    DDX_Control(pDX, IDC_STATIC_SCANNED_PROT, m_lbScannedProt);
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKtReconDlg, DialogEx)
//{{AFX_MSG_MAP(CKtReconDlg)
ON_WM_PAINT()
ON_WM_CTLCOLOR()
ON_WM_WINDOWPOSCHANGING()
ON_WM_ACTIVATE()
ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
ON_BN_CLICKED(IDC_BUTTON_GET_DATA, OnButtonGetData)
ON_BN_CLICKED(IDC_BUTTON_KTRECON, OnButtonKtrecon)
ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
ON_NOTIFY(NM_CLICK, IDC_LIST_SCANNED_PROTOCOL, OnClickListScannedProtocol)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKtReconDlg message handlers

void CKtReconDlg::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
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
        //CPopupDlg::OnPaint();
    }
    
    // Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CKtReconDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    
    // TODO: Change any attributes of the DC here
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return m_list_brush;
    } else {
        //
    }
    
    // TODO: Return a different brush if the default is not desired
    return hbr;	return hbr;
}

void CKtReconDlg::OnButtonExit() 
{
    // TODO: Add your control notification handler code here
  
    if(NULL != m_KtReconController){
        m_KtReconController->OnExit();
    } else {
		//log error message
	}
}

void CKtReconDlg::initializeControls()
{
    m_scannedProtDisplay.SetFrameColor(FRAME_BKCOLOR);
    m_scannedProtDisplay.SetFrameColor(FRAME_BKCOLOR);
    
    m_operations.SetFrameColor(FRAME_BKCOLOR);
    m_operations.SetFrameColor(FRAME_BKCOLOR);
    
    m_getDataButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_getDataButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_ktReconButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_ktReconButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_viewerButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_viewerButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_exitButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_exitButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_scannedProtocolListbox.SetFont(_T("Arial"), TEXT_SIZE, true );
    //insert columns in list control
    m_scannedProtocolListbox.InsertColumn(0,_T("Scanned Protocol"),LVCFMT_LEFT,120);
    m_scannedProtocolListbox.SetColumnRange(0,120,120);    
}

BOOL CKtReconDlg::OnInitDialog() 
{
    DialogEx::OnInitDialog();
    try {
    WINDOW_COLORS_t l_window_color;
//	AfxGetApp()->DoWaitCursor(1);
//	m_KtReconController->SetWaitCursor(true);
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    initializeControls();     
    RECT desktop_rect;
    CRect tidlg_rect;
    GetDesktopWindow()->GetWindowRect(&desktop_rect);
    GetWindowRect(&tidlg_rect);
    //    long x_pos = (desktop_rect.right - tidlg_rect.Width() - 15);
    //    long y_pos = desktop_rect.top + 15;
    SetRibbonHeight(0);    
    m_scannedProtocolListbox.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);  
//	LoadScannedListBox();
	CKtReconView::GetView()->AddZOrderLayer();
	CKtReconView::GetView()->RegisterWindowZOrder(this->GetSafeHwnd());
	} catch (...)
	{
        CString l_error_string(_T("OnInitDialog() :Exception"));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);

	}
    return TRUE;  
}
void CKtReconDlg::SetDlgPos()
{
    RECT desktop_rect;
    CRect tidlg_rect;
    GetDesktopWindow()->GetWindowRect(&desktop_rect);
    GetWindowRect(&tidlg_rect);
    long x_pos = (desktop_rect.right - tidlg_rect.Width() - 15);
    long y_pos = desktop_rect.top + 15;
    MoveWindow(x_pos, y_pos, tidlg_rect.Width(), tidlg_rect.Height());
}

void CKtReconDlg::OnButtonGetData() 
{
	
	GetDlgItem(IDC_BUTTON_GET_DATA)->EnableWindow(FALSE);//+Mpower/20130812/PN/added //disable button
    if(NULL == m_KtReconController){
        CString l_error_string(_T("CKtReconDlg() :m_KtReconController is NULL"));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //log error
        int l_nStatus = E_COMON_ERROR;
        DisplayErr(l_nStatus);
        return;
    }
	m_KtReconController ->SetWaitCursor(true); //+Mpower/20130812/PN/added
    int l_nStatus = m_KtReconController->OnGetData();
    if(E_NO_ERROR != l_nStatus){
        DisplayErr(l_nStatus);
    }
	//+Mpower/20130812/PN/added
	m_KtReconController ->SetWaitCursor(false);
	GetDlgItem(IDC_BUTTON_GET_DATA)->EnableWindow(TRUE);//button enable 

	//-Mpower/20130812/PN/added 
}   

void CKtReconDlg::OnButtonKtrecon() 
{
    GetDlgItem(IDC_BUTTON_KTRECON)->EnableWindow(FALSE);//+Mpower/20130812/PN/added //disable button
    if(NULL == m_KtReconController){
        CString l_error_string(_T("CKtReconDlg() :m_KtReconController is NULL"));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //log error
        int l_nStatus = E_COMON_ERROR;
        DisplayErr(l_nStatus);
        return;
    }
	m_KtReconController ->SetWaitCursor(true); //+Mpower/20130812/PN/added
    int l_nStatus =  m_KtReconController->OnKtRecon();
    if(E_NO_ERROR != l_nStatus){
        DisplayErr(l_nStatus);
    }
	//+Mpower/20130812/PN/added
	m_KtReconController ->SetWaitCursor(false); 
	GetDlgItem(IDC_BUTTON_KTRECON)->EnableWindow(TRUE);//button enable 
	//-Mpower/20130812/PN/added
}

void CKtReconDlg::OnButtonView() 
{
    GetDlgItem(IDC_BUTTON_VIEW)->EnableWindow(FALSE);//+Mpower/20130812/PN/added //disable button
    if(NULL == m_KtReconController){
        CString l_error_string(_T("CKtReconDlg() :m_KtReconController is NULL"));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //log error
        int l_nStatus = E_COMON_ERROR;
        DisplayErr(l_nStatus);
        return;
    }
	m_KtReconController ->SetWaitCursor(true); //+Mpower/20130812/PN/added
    int l_nStatus = m_KtReconController->OnView();
    if(E_NO_ERROR != l_nStatus){
        DisplayErr(l_nStatus);
    }
	//+Mpower/20130812/PN/added
	m_KtReconController ->SetWaitCursor(false); 
	GetDlgItem(IDC_BUTTON_VIEW)->EnableWindow(TRUE);//button enable 
	//-Mpower/20130812/PN/added
}

void CKtReconDlg::OnClickListScannedProtocol(NMHDR* pNMHDR, LRESULT* pResult) 
{
    
    if ((NULL == pNMHDR) || (NULL == pResult)){
        CString l_cserrmsg = _T("OnClickListScannedProtocol() :pNMHDR , pResult is NULL ");
        CKtReconLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__); //AN
    }else{
        NMLISTVIEW* l_plist = (NMLISTVIEW*)pNMHDR;
        m_ncurrscannedrowno = l_plist->iItem;
	CString csprotno  =	m_scannedProtocolListbox.GetItemText(m_ncurrscannedrowno, 0);
	int protno = _ttol(csprotno);
	protno = protno - protno%1000 - 1; //VFC protocol index
	if (m_KtReconController) m_KtReconController->SetProtocolNumber(protno);
	//	CString scanro = m_ncurrscannedrowno;
	//	CKtReconLogHelper::WriteToErrorLog(scanro,__LINE__,__FILE__,ERRORVIEW_DISPLAY);
        *pResult = 0;
    }
    
    *pResult = 0;
}

void CKtReconDlg::FillProtocolListBox( LVITEM &lv )
{
    try{
        m_scannedProtocolListbox.InsertItem(&lv);        
    }catch(...)
    {
        
    }
}

void CKtReconDlg::DisplayErr(int &f_nmsg_code)
{
    CErrorDlg *dlg = NULL;
    CWnd * parent = NULL;
    HWND pHwnd = NULL;
    //dlg = new CErrorDlg(this);
    parent = this;
    pHwnd = this->GetSafeHwnd();
    
    
    dlg = new CErrorDlg(parent);
    if (!dlg) 
    {
        CKtReconLogHelper::WriteToErrorLog(_T("DisplayErr Mem allocation failed"),__LINE__,__FILE__);
        return;
    }
    dlg->SetHWnd(pHwnd);
    dlg->SetErrorValue(f_nmsg_code);
    dlg->ShowErrorMessage();
}

//+Mpower/20130812/PN/added
/*************************************************************
* Method:       OnSetCursor handles WM_SETCURSOR message for setting Wait cursor when necessary
* Description:  This function is used
* Parameter:    CWnd * pWnd :
* Parameter:    UINT nHitTest :
* Parameter:    UINT message :
* Returns:      BOOL
*************************************************************/
BOOL CKtReconDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    
    if(m_KtReconController->IsWaitCursor()) {
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
        return TRUE;
    } else {
        // ReleaseCapture();
    }
    return TRUE; //check again
//    return CPopupDlg::OnSetCursor(pWnd, nHitTest, message);
}
//-Mpower/20130812/PN/added
/////////////////////////////////END OF FILE /////////////////////////////////////

void CKtReconDlg::SetController(CKtReconController *control)
{
	if (control == NULL)
	{
		//log error 
		return;
	}
m_KtReconController = control;
}

int CKtReconDlg::LoadScannedListBox()
{
	if (	m_KtReconController && m_KtReconController->m_csstudyloid.CompareNoCase(_T("")) != 0) {

	 int l_nTotalProtocolCount = 0;
        int l_nTotalSeriesCount = 0;
        vector <CString> l_csSeriesLoidVector;
        vector <long> l_csProtocolNumberVector;
        vector <long> l_csSeriesNumberVector;
        CString csSourceProt (_T(""));

        int l_nStatus = m_KtReconController->GetDataForDisplayingScannedProtocol(
                                                        l_csProtocolNumberVector, 
                                                        l_nTotalProtocolCount
                                                        );

        if(E_NO_ERROR != l_nStatus){
            CString l_cserrmsg = (_T("GetDataForDisplayingScannedProtocol Failed."));
			CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
            l_nTotalProtocolCount = 0;
            l_nTotalSeriesCount = 0;
            //log error
           DisplayErr(l_nStatus);
            
            return(FALSE);            
        }
		 if(l_nTotalProtocolCount > 0) {
            CString strItem = _T("");
            LVITEM lv;
            for(int l_nCnt_i = 0; l_nCnt_i < l_nTotalProtocolCount; l_nCnt_i++) {
                strItem.Format(_T("%d"), l_csProtocolNumberVector.at(l_nCnt_i));
                lv.iItem = l_nCnt_i;
                lv.iSubItem = 0;
                lv.pszText = strItem.GetBuffer(strItem.GetLength());
                lv.mask = LVIF_TEXT;
                FillProtocolListBox(lv);
            }
        }else{
            //log error //recheck the message to be written to the log file
			
           
                l_nStatus = KTRECON_NO_SCANNED_PROTOCOL;
                DisplayErr(l_nStatus);
            
		//	CString l_error_display = _T("");
			//l_error_display = _T("There is no scaned protocol");
		   // CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
            return(E_ERROR);
        }

      
        l_csProtocolNumberVector.clear();
		
	}
	return E_NO_ERROR;
}
void CKtReconDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    lpwndpos->flags = lpwndpos->flags | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOSENDCHANGING;
}
void CKtReconDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    //KTRECON_TRACE(CTiPrepDlg::OnActivate);
    DialogEx::OnActivate(nState, pWndOther, bMinimized);
	if(WA_INACTIVE != nState) {
		(CKtReconView::GetView())->ChangeWindowZOrder(this->GetSafeHwnd());
	}
   
}