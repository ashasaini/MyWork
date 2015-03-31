// AdvCardiacGLView.cpp : implementation of the CAdvCardiacGLView class
//

#include "stdafx.h"
#include "AdvCardiacGL.h"
#include "AdvCardiacGLDoc.h"
#include "AdvCardiacGLView.h"
#include "ACGAppController.h"
#include "DPSComManager.h"
#include "CommonDefinitions.h"
#include "ACGLDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CAdvCardiacGLView *g_MainView = NULL;

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLView

IMPLEMENT_DYNCREATE(CAdvCardiacGLView, CFormView)

BEGIN_MESSAGE_MAP(CAdvCardiacGLView, CFormView)
//{{AFX_MSG_MAP(CAdvCardiacGLView)
ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
ON_MESSAGE(WM_INIT_TOOL, OnInitAdvCardiacGL)
ON_MESSAGE(WM_CMD_CLOSE, OnCmdClose)
ON_MESSAGE(WM_CMD_SHOW, OnCmdShow)
ON_MESSAGE(WM_CMD_HIDE, OnCmdHide)
ON_WM_SHOWWINDOW()
ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLView construction/destruction

CAdvCardiacGLView::CAdvCardiacGLView()
: CFormView(CAdvCardiacGLView::IDD)
{
	m_maindlg = NULL;
    //{{AFX_DATA_INIT(CAdvCardiacGLView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // TODO: add construction code here
}

CAdvCardiacGLView::~CAdvCardiacGLView()
{
}

void CAdvCardiacGLView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAdvCardiacGLView)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BOOL CAdvCardiacGLView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    
    return CFormView::PreCreateWindow(cs);
}

void CAdvCardiacGLView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();
    g_MainView = (CAdvCardiacGLView*)this;
    
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CAdvCardiacGLView::OnCancelEditSrvr()
{
    GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLView diagnostics

#ifdef _DEBUG
void CAdvCardiacGLView::AssertValid() const
{
    CFormView::AssertValid();
}

void CAdvCardiacGLView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

CAdvCardiacGLDoc* CAdvCardiacGLView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAdvCardiacGLDoc)));
    return (CAdvCardiacGLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLView message handlers


extern CAdvCardiacGLApp theApp;

LRESULT CAdvCardiacGLView::OnInitAdvCardiacGL(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    CACGLLogHelper::CreateInstance(BSTR(lParam));
    
    int l_nStatus = E_NO_ERROR;
    
    l_nStatus = ACGAppController::GetInstance()->ReadToolConfigSettingsFromFile();
    if(E_NO_ERROR != l_nStatus){
        CString l_csStr(_T("ReadToolConfigSettingsFromFile returned error"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        //pending
    }
    
    CDPSComManager *l_server = NULL;
    
    l_server = CDPSComManager::GetInstance();    
    if (l_server != NULL) 
    {
        if (l_server->InitializeServer()) 
        {
            l_server->SetACGLStatus(STATE_READY); 
            IMplusMessage *l_msg = NULL;
            
            CString l_sparent(_T(""));
            l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
            l_server->Connect(l_sparent);
            //l_server->Connect(CString(l_server->GetParentName()));
            l_msg = l_server->CreateMessage(FW_ReportState,1,l_server->GetParentName());
            if (l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            l_server->Disconnect();
        } else {
            l_server->DestroyServer();
            l_server->DeleteInstance();
            l_server = NULL;
			CString l_csStr(_T("Failed to Initialise DPS communication. Please exit and try again"));
            CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
			//
        }
        l_nStatus = StartGUI();//ACGAppController::GetInstance()->StartGUI();
        if(E_NO_ERROR != l_nStatus){
            CString l_csStr(_T("Application failed to start"));
            CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
			//AN:Exit the tool??
        }
        
       (ACGAppController::GetInstance()->SetDlg( m_maindlg ));
        
        this->ShowWindow(SW_HIDE);
        
      //  if (m_maindlg){
//            ACGAppController::GetInstance()->AddZOrderLayer();
          //  ACGAppController::GetInstance()->RegisterWindowZOrder(m_maindlg->GetSafeHwnd());
      //  }
    }    
    
    return 1;
}


LRESULT CAdvCardiacGLView::OnCmdClose(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    //	exit (0);//for now
    if (m_maindlg && m_maindlg->GetSafeHwnd()) {
        m_maindlg->DestroyWindow();
        delete m_maindlg;
        m_maindlg = NULL;
    }
    PostQuitMessage(WM_DESTROY);
    return 1;
}


LRESULT CAdvCardiacGLView::OnCmdShow(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    CFrameWnd* l_wnd = GetParentFrame();
	
    
    if((NULL != l_wnd) && (m_maindlg != NULL) && (m_maindlg->GetSafeHwnd())) {
        l_wnd->ShowWindow(SW_HIDE);    //Hiding the View
        m_maindlg->ShowWindow(SW_SHOW); //Showing the Dialog
    }
	
    int *status = NULL;
    status = (int *)(wParam);    
    if((NULL != status) && (1 == *status)) { // when AdvCardiacGL is to be shown after being hidden
		
        
    }else{  //Invocation time
		
		//  m_maindlg->SetACGLDlgPosition(); //Display at standar position        
    }   
	
    return 1;
}


LRESULT CAdvCardiacGLView::OnCmdHide(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
	
    if (m_maindlg && m_maindlg->GetSafeHwnd()) {
        m_maindlg->ShowWindow(SW_HIDE) ;
    }
    
    GetParentFrame()->ShowWindow(SW_HIDE);
    return 1;
}
CAdvCardiacGLView * CAdvCardiacGLView::GetView()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
    CView * pView = pFrame->GetActiveView();
    
    if(!pView) {
        return NULL;
    }
    
    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if(! pView->IsKindOf(RUNTIME_CLASS(CAdvCardiacGLView))) {
        return NULL;
    }
    
    return (CAdvCardiacGLView *) pView;
}


int CAdvCardiacGLView::StartGUI()
{
    try{
		if (m_maindlg && m_maindlg->GetSafeHwnd()) return E_NO_ERROR;
		if (m_maindlg) {
			delete m_maindlg; m_maindlg = NULL;
		}
		 m_maindlg = new CACGLDlg();
    
    if(NULL == m_maindlg){
        CString l_csStr(_T("Memory Allocation failed in ACGAppController()"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
		return E_ERROR;
    }
    
        if(NULL != m_maindlg){
            BOOL l_bStatus = m_maindlg->Create(IDD_ACGL_DIALOG,AfxGetMainWnd()/*GetDesktopWindow()*/);
            
            if(TRUE != l_bStatus){
                CString l_csmsg(_T("Failed to create dialog"));
                CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                return(E_ERROR);
            }
            
            l_bStatus = m_maindlg->ShowWindow(SW_SHOW);
            
        }else{   
            CString l_csmsg(_T("Dialog handle NULL"));
            CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return(E_ERROR);  
        }
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in StartGUI"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
///////////////////////////////// END OF FILE /////////////////////////////////


