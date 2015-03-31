// ACGLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdvCardiacGL.h"
#include "ACGLDlg.h"
#include "ACGAppController.h"
#include "DPSComManager.h"
#include "CommonDefinitions.h"
#include "AdvCardiacGLView.h"
#include "ConfirmationDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 CConfirmationDlg l_dlg;
/////////////////////////////////////////////////////////////////////////////
// CACGLDlg dialog


CACGLDlg::CACGLDlg(CWnd* pParent /*=NULL*/)
: DialogEx(CACGLDlg::IDD, pParent)
{
    toggle = false;
    width = 0;
    height = 0;
    //{{AFX_DATA_INIT(CACGLDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_APP_ICON);
}


void CACGLDlg::DoDataExchange(CDataExchange* pDX)
{
    DialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CACGLDlg)
    DDX_Control(pDX, IDC_BUTTON_15, m_ACGL_15);
    DDX_Control(pDX, IDC_BUTTON_14, m_ACGL_14);
    DDX_Control(pDX, IDC_BUTTON_13, m_ACGL_13);
    DDX_Control(pDX, IDC_BUTTON_12, m_ACGL_12);
    DDX_Control(pDX, IDC_BUTTON_11, m_ACGL_11);
    DDX_Control(pDX, IDC_BUTTON_10, m_ACGL_10);
    DDX_Control(pDX, IDC_BUTTON_9, m_ACGL_9);
    DDX_Control(pDX, IDC_BUTTON_8, m_ACGL_8);
    DDX_Control(pDX, IDC_BUTTON_7, m_ACGL_7);
    DDX_Control(pDX, IDC_BUTTON_6, m_ACGL_6);
    DDX_Control(pDX, IDC_BUTTON_5, m_ACGL_5);
    DDX_Control(pDX, IDC_BUTTON_4, m_ACGL_4);
    DDX_Control(pDX, IDC_BUTTON_3, m_ACGL_3);
    DDX_Control(pDX, IDC_BUTTON_2, m_ACGL_2);
    DDX_Control(pDX, IDC_BUTTON_1, m_ACGL_1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACGLDlg, DialogEx)
//{{AFX_MSG_MAP(CACGLDlg)
ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
ON_BN_CLICKED(IDC_BUTTON_5, OnButton5)
ON_BN_CLICKED(IDC_BUTTON_6, OnButton6)
ON_BN_CLICKED(IDC_BUTTON_7, OnButton7)
ON_BN_CLICKED(IDC_BUTTON_8, OnButton8)
ON_BN_CLICKED(IDC_BUTTON_9, OnButton9)
ON_BN_CLICKED(IDC_BUTTON_10, OnButton10)
ON_BN_CLICKED(IDC_BUTTON_11, OnButton11)
ON_BN_CLICKED(IDC_BUTTON_12, OnButton12)
ON_BN_CLICKED(IDC_BUTTON_13, OnButton13)
ON_BN_CLICKED(IDC_BUTTON_14, OnButton14)
ON_BN_CLICKED(IDC_BUTTON_15, OnButton15)
ON_WM_CTLCOLOR()
ON_WM_CLOSE()
ON_COMMAND(ID_BUTTON_DOCK, OnButtonDock)
	ON_COMMAND(ID_BUTTON_MAXIMISE, OnButtonMaximise)
ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACGLDlg message handlers

void CACGLDlg::OnButton1() 
{
    int l_nToolNum = 0;
	CString l_csToolName;
	m_ACGL_1.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);

}

void CACGLDlg::OnButton2() 
{
    int l_nToolNum = 1;
	CString l_csToolName;
	m_ACGL_2.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);}

void CACGLDlg::OnButton3() 
{
    int l_nToolNum = 2;
	CString l_csToolName;
	m_ACGL_3.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton4() 
{
    int l_nToolNum = 3;
	CString l_csToolName;
	m_ACGL_4.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton5() 
{
    int l_nToolNum = 4;
	CString l_csToolName;
	m_ACGL_5.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton6() 
{
    int l_nToolNum = 5;
	CString l_csToolName;
	m_ACGL_6.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton7() 
{
    int l_nToolNum = 6;
	CString l_csToolName;
	m_ACGL_7.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton8() 
{
    int l_nToolNum = 7;
	CString l_csToolName;
	m_ACGL_8.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton9() 
{
    int l_nToolNum = 8;
	CString l_csToolName;
	m_ACGL_9.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton10() 
{
    int l_nToolNum = 9;
	CString l_csToolName;
	m_ACGL_10.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton11() 
{
    int l_nToolNum = 10;
	CString l_csToolName;
	m_ACGL_11.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton12() 
{
    int l_nToolNum = 11;
	CString l_csToolName;
	m_ACGL_12.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton13() 
{
    int l_nToolNum = 12;
	CString l_csToolName;
	m_ACGL_13.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton14() 
{
    int l_nToolNum = 13;
	CString l_csToolName;
	m_ACGL_14.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

void CACGLDlg::OnButton15() 
{
    int l_nToolNum = 14;
	CString l_csToolName;
	m_ACGL_15.GetWindowText(l_csToolName);
	InvokeApp(l_nToolNum,l_csToolName);
}

HBRUSH CACGLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    
    // TODO: Change any attributes of the DC here
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return m_ACGL_brush;
    } else {
    /*CWnd * wnd = m_FirstToolBar.GetOwner();
    CDC* dc = wnd->GetDC();
    dc->SetBkColor(BKCOLOR);
    dc->SetBkMode(OPAQUE);
        return m_ACGL_brush;*/
        //((CWnd*)( &m_FirstToolBar))->Setcolo(BKCOLOR);
        //if )
        //
    }
    
    // TODO: Return a different brush if the default is not desired
    return hbr;
}

BOOL CACGLDlg::OnInitDialog() 
{
    DialogEx::OnInitDialog();
    
	
	
    // TODO: Add extra initialization here
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_ACGL_brush.CreateSolidBrush(l_window_color.crClientColor);
    
    SetRibbonHeight(0);//KJ
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    DisplayToolButtons();
    SetACGLDlgPosition();
    
    CDPSComManager *commgr = CDPSComManager::GetInstance();
    CString parent = commgr->GetParentName();
    if (parent.CompareNoCase(_T(""))==0) {
        CString csparent= PAGER;
        commgr->SetParentName(_bstr_t(csparent))		;
        commgr->InitializeServer();
    }

	extern CAdvCardiacGLApp theApp;
    theApp.m_pMainWnd->ShowWindow(SW_HIDE);
    
    if(!m_FirstToolBar.CreateEx(this, /*TBSTYLE_FLAT,*/ WS_CHILD | 
        WS_VISIBLE | CBRS_TOP | /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | 
        /*CBRS_FLYBY |*/ CBRS_SIZE_DYNAMIC) || (!m_FirstToolBar.LoadToolBar(IDR_TOOLBAR1)))
    {
        EndDialog (0);
        //app exit
    }
 //   m_FirstToolBar.MoveWindow()
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,0);
    
    
    m_hIcon = AfxGetApp()->LoadIcon(IDI_APP_ICON);
		  SetIcon(m_hIcon, TRUE);			// Set big icon
          SetIcon(m_hIcon, FALSE);		// Set small icon
          //	m_FirstToolBar.SetWindowPos(0,5,5,30,30,0);
          // commgr->InitializeServer();
          
          // EXCEPTION: OCX Property Pages should return FALSE
          CRect rect;
          GetClientRect(&rect);
          width = rect.Width();
          height = rect.Height();
          
          ACGAppController::GetInstance()->AddZOrderLayer();
          ACGAppController::GetInstance()->RegisterWindowZOrder(m_hWnd);
          
          return TRUE;  // return TRUE unless you set the focus to a control
}

void CACGLDlg::OnClose() 
{
    // TODO: Add your message handler code here and/or call default
   
	INT_PTR nRet = IDOK;
	int i = 0;
		  if (ACGAppController::GetInstance()->IsAnyToolRunning()) {
			  nRet = l_dlg.DoModal();
		  }
		  
		  
		  switch ( nRet )
		  {
		  case IDOK:
			  
			  //	PostQuitMessage(WM_DESTROY);
			  //	DialogEx::OnClose();
			  //for (i = 0; i <= ACGAppController::GetInstance()->isExitClcked; i++)
			  ACGAppController::GetInstance()->isExitClcked = true;
			  ACGAppController::GetInstance()->SendCloseToTools();
			  ACGAppController::GetInstance()->CloseTool();
			  
			  break;
		  case IDCANCEL:
			  return;
			  break;
		  default:
			  return;
			  break;
		  };    
}

BOOL CACGLDlg::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class    
    return DialogEx::PreTranslateMessage(pMsg);
}

void CACGLDlg::SetACGLDlgPosition()
{
    RECT desktop_rect;
    CRect acgldlg_rect;
    GetDesktopWindow()->GetWindowRect(&desktop_rect);
    GetWindowRect(&acgldlg_rect);
    long x_pos = (desktop_rect.left + 10);
    long y_pos = (desktop_rect.bottom - acgldlg_rect.Height() - 50);
    
    MoveWindow(x_pos, y_pos, acgldlg_rect.Width(), acgldlg_rect.Height());    
}

void CACGLDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    
    lpwndpos->flags = lpwndpos->flags | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOSENDCHANGING;
}

void CACGLDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    //AGCL_TRACE(CACGLDlg::OnActivate);
    DialogEx::OnActivate(nState, pWndOther, bMinimized);
	if(WA_INACTIVE != nState) {
		if (l_dlg && l_dlg.GetSafeHwnd()) {}else { //AN DEFect fix: Confirmation dialog hidden behind main dialog
    (ACGAppController::GetInstance())->ChangeWindowZOrder(this->GetSafeHwnd());
	}
	}
    
}

int CACGLDlg::DisplayToolButtons()
{    
    try{
        vector<KeyValuePair> l_KeyValuePairs;
        ACGAppController::GetInstance()->GetConfigToolSettingsForDisplay(l_KeyValuePairs);
        
        KeyValuePair l_OneKeyValuePair;
        size_type count = l_KeyValuePairs.size();
        string l_csToolName;
        
        // HICON l_icon;
        
        if(NUMBER_OF_TOOLS < count){
            CString l_csmsg(_T("Number of tools in config file exceeds total no. of buttons on the GUI"));
            CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            count = NUMBER_OF_TOOLS;
        }
        
        for(size_type index = 0; index < count ; ++index) {
            
            l_OneKeyValuePair = l_KeyValuePairs[index];
            l_csToolName = l_OneKeyValuePair.first;     
            
            CString caption(l_csToolName.c_str());
            
            switch(index){
                
            case 0:
                m_ACGL_1.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_1.SetButtonType(TUSBUTTON_ROUND_ALL); 
                m_ACGL_1.ShowWindow(SW_SHOW);
                m_ACGL_1.EnableWindow(TRUE);
                m_ACGL_1.SetWindowText(caption);
                // l_icon = AfxGetApp()->LoadIcon(IDI_ICON1);
                // m_ACGL_1.SetIcon(l_icon);            
                break;
                
            case 1:
                m_ACGL_2.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_2.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_2.ShowWindow(SW_SHOW);
                m_ACGL_2.EnableWindow(TRUE);
                m_ACGL_2.SetWindowText(caption);
                break;
                
            case 2:
                m_ACGL_3.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_3.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_3.ShowWindow(SW_SHOW);
                m_ACGL_3.EnableWindow(TRUE);
                m_ACGL_3.SetWindowText(caption);
                break;
                
            case 3:
                m_ACGL_4.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_4.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_4.ShowWindow(SW_SHOW);
                m_ACGL_4.EnableWindow(TRUE);
                m_ACGL_4.SetWindowText(caption);
                break;
                
            case 4:
                m_ACGL_5.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_5.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_5.ShowWindow(SW_SHOW);
                m_ACGL_5.EnableWindow(TRUE);
                m_ACGL_5.SetWindowText(caption);
                break;
                
            case 5:
                m_ACGL_6.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_6.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_6.ShowWindow(SW_SHOW);
                m_ACGL_6.EnableWindow(TRUE);
                m_ACGL_6.SetWindowText(caption);
                break;
                
            case 6:
                m_ACGL_7.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_7.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_7.ShowWindow(SW_SHOW);
                m_ACGL_7.EnableWindow(TRUE);
                m_ACGL_7.SetWindowText(caption);
                break;
                
            case 7:
                m_ACGL_8.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_8.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_8.ShowWindow(SW_SHOW);
                m_ACGL_8.EnableWindow(TRUE);
                m_ACGL_8.SetWindowText(caption);
                break;
                
            case 8:
                m_ACGL_9.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_9.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_9.ShowWindow(SW_SHOW);
                m_ACGL_9.EnableWindow(TRUE);
                m_ACGL_9.SetWindowText(caption);            
                break;
                
            case 9:
                m_ACGL_10.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_10.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_10.ShowWindow(SW_SHOW);
                m_ACGL_10.EnableWindow(TRUE);
                m_ACGL_10.SetWindowText(caption);
                break;
                
            case 10:
                m_ACGL_11.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_11.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_11.ShowWindow(SW_SHOW);
                m_ACGL_11.EnableWindow(TRUE);
                m_ACGL_11.SetWindowText(caption);
                break;
                
            case 11:
                m_ACGL_12.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_12.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_12.ShowWindow(SW_SHOW);
                m_ACGL_12.EnableWindow(TRUE);
                m_ACGL_12.SetWindowText(caption);
                break;
                
            case 12:
                m_ACGL_13.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_13.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_13.ShowWindow(SW_SHOW);
                m_ACGL_13.EnableWindow(TRUE);
                m_ACGL_13.SetWindowText(caption);
                break;
                
            case 13:
                m_ACGL_14.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_14.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_14.ShowWindow(SW_SHOW);
                m_ACGL_14.EnableWindow(TRUE);
                m_ACGL_14.SetWindowText(caption);
                break;
                
            case 14:
                m_ACGL_15.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
                m_ACGL_15.SetButtonType(TUSBUTTON_ROUND_ALL);
                m_ACGL_15.ShowWindow(SW_SHOW);
                m_ACGL_15.EnableWindow(TRUE);
                m_ACGL_15.SetWindowText(caption);
                break;   
                
            default:
                
                CString l_csmsg(_T("Default case executed in DisplayToolButtons"));
                CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                break;
            }        
        }
}catch(...){
    CString l_csmsg(_T("Exception occurred in DisplayToolButtons"));
    CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    return(E_ERROR);
}
return(E_NO_ERROR);
}

void CACGLDlg::OnButtonDock() 
{  
	SetWindowPos(NULL,0,0,85,75,SWP_NOMOVE);
    //if (false == toggle)
    //    SetWindowPos(NULL,0,0,85,75,SWP_NOMOVE);
   // else
     //   SetWindowPos(NULL,0,0,width,height,SWP_NOMOVE);
    //toggle = !toggle;
    // TODO: Add your command handler code here    

}

HCURSOR CACGLDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

BOOL CACGLDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    // TODO: Add your specialized code here and/or call the base class
    LPNMHDR pnmh = (LPNMHDR) lParam;
    if(pnmh->hwndFrom == m_FirstToolBar.m_hWnd)
    {int i =0;
    LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW) lParam;
    CRect rect;
    m_FirstToolBar.GetClientRect(rect);
    FillRect(lpNMCustomDraw->nmcd.hdc, rect, 
        /*	(HBRUSH)GetStockObject(BLACK_BRUSH)*/m_ACGL_brush);	
        
    }     
    return CDialog::OnNotify(wParam, lParam, pResult);
}

void CACGLDlg::OnButtonMaximise() 
{
	SetWindowPos(NULL,0,0,width,height,SWP_NOMOVE);
	// TODO: Add your command handler code here
	
}
int CACGLDlg::InvokeApp(int f_ntoolno,CString & cstoolname)
{
    CString l_csPageScene;
    ACGAppController::GetInstance()->SetToolNumber(f_ntoolno);   
    ACGAppController::GetInstance()->SetToolName(cstoolname);
    CString l_csRequestedPageScene(_T(""));
    ACGAppController::GetInstance()->GetPageScene(l_csRequestedPageScene); 
    ACGAppController::GetInstance()->SetButtonPressedStatus(1,f_ntoolno);
    ACGAppController::GetInstance()->SwitchPageScene(l_csRequestedPageScene);
    ACGAppController::GetInstance()->InvokeApplication();        
	return 0;
}
///////////////////////////////// END OF FILE /////////////////////////////////
