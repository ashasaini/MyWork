// KtReconView.cpp : implementation of the CKtReconView class
//

#include "stdafx.h"
#include "KtRecon.h"
#include "KtReconDoc.h"
#include "KtReconView.h"
#include "KtICMMgr.h"
#include "KtReconLogHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKtReconView
CKtReconView *g_MainView = NULL;

IMPLEMENT_DYNCREATE(CKtReconView, CFormView)

BEGIN_MESSAGE_MAP(CKtReconView, CFormView)
        ON_MESSAGE(WM_INIT_KTRECON,OnInitKtRecon)
        ON_MESSAGE(WM_CMD_CLOSE,OnCmdClose)
        ON_MESSAGE(WM_CMD_SHOW,OnCmdShow)
        ON_MESSAGE(WM_CMD_HIDE,OnCmdHide)
		ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKtReconView construction/destruction

CKtReconView::CKtReconView()
	: CFormView(CKtReconView::IDD)
{
	m_ktReconDlg = NULL;
}

CKtReconView::~CKtReconView()
{
	CKtDPSCommMgr *l_server = NULL;
    l_server = CKtDPSCommMgr::GetInstance();
	if (l_server != NULL) 
    {
        l_server->DestroyServer();
        l_server->DeleteInstance();
        l_server = NULL;
    }
    CKtReconLogHelper::DeleteInstance();	//TEST
}

void CKtReconView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CKtReconView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CKtReconView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
    g_MainView = (CKtReconView*)this;

}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CKtReconView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CKtReconView diagnostics

#ifdef _DEBUG
void CKtReconView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKtReconView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKtReconDoc* CKtReconView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKtReconDoc)));
	return (CKtReconDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKtReconView message handlers


LRESULT CKtReconView::OnInitKtRecon(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
//    AfxMessageBox(_T("OnInitKtRecon"));
    if(NULL == m_ktReconDlg){
        m_ktReconDlg = new CKtReconDlg(AfxGetMainWnd());
    }
    
    CKtReconLogHelper::CreateInstance(BSTR(lParam));	//TEST

    CKtDPSCommMgr *l_server = NULL;
    l_server = CKtDPSCommMgr::GetInstance();
    
    if (l_server != NULL) 
    {
        if (l_server->InitializeServer()) 
        {
            l_server->SetKtReconStatus(STATE_READY); 
            IMplusMessage *l_msg = NULL;
            
            CString l_sparent(_T(""));
            l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
            l_server->Connect(l_sparent);
            l_msg = l_server->CreateMessage(FW_ReportState,1,l_server->GetParentName());
            if (l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            l_server->Disconnect();
        }
        else
        {
            l_server->DestroyServer();
            l_server->DeleteInstance();
            l_server = NULL;
			//log error
        }
       
        //Register KtRecon Window Z-Order --

		m_KtReconController = new CKtReconController();
		if (m_KtReconController == NULL) 
		{
			//log error

		}
		m_ktReconDlg->SetController(m_KtReconController);
		m_KtReconController->SetDlg(m_ktReconDlg);
		StartGUI();
		int l_status = m_KtReconController->GetStudyLoidFromPager();
        if(0 != l_status){
			CString l_error_display = _T("");
			l_error_display = _T("Error Occured in GetSelectedImages");
		    CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
		
            //log error
            //pending return required?
            //should error dlg be displayed here ?
            //if yes then display error E_IMAGE_MTRX_COMM_FAILED
        }
		 
    }

    return 1;
}

extern CKtReconApp theApp;
LRESULT CKtReconView::OnCmdClose(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

	if(m_ktReconDlg)
	{
		m_ktReconDlg->ShowWindow(SW_HIDE);
		m_ktReconDlg->DestroyWindow();
		delete m_ktReconDlg;
		m_ktReconDlg = NULL;

	}
	theApp.m_pMainWnd->PostMessage(WM_CLOSE);
    return 1;
}


LRESULT CKtReconView::OnCmdShow(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    CFrameWnd* l_wnd = GetParentFrame();

    if(NULL != l_wnd && m_ktReconDlg && m_ktReconDlg->GetSafeHwnd()) {
        l_wnd->ShowWindow(SW_HIDE);    //Hiding the View
        m_ktReconDlg->ShowWindow(SW_SHOW); //Showing the Dialog
    }
    int status = NULL;
    status = (int )(wParam);
    
    if(NULL != status) {
        CString l_csstat(_T(""));
        l_csstat.Format(_T("Init status = %d"), status);
    }
    if((NULL != status)) { 
        // when KtRecon is to be shown after being hidden
    }else{
        //m_ktReconDlg->SetDlgPos();
    }
//     tagWINDOWPLACEMENT lWndPlacement ; 
//     ZeroMemory(&lWndPlacement, sizeof(WINDOWPLACEMENT));
//     
//     GetParentFrame()->GetWindowPlacement(&lWndPlacement);
//     lWndPlacement.rcNormalPosition = CRect( 500, 
//         200, 
//         500+400, 
//         200+350);
//     
//     GetParentFrame()->ShowWindow(SW_SHOW);
// 	GetParentFrame()->SetWindowPlacement(&lWndPlacement);
// 
    return 1;
}


LRESULT CKtReconView::OnCmdHide(WPARAM wParam,LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);


    GetParentFrame()->ShowWindow(SW_HIDE);
	 if(m_ktReconDlg && m_ktReconDlg->GetSafeHwnd() ) {
        m_ktReconDlg->ShowWindow(SW_HIDE); //Showing the Dialog
    }
    return 1;
}
BOOL CKtReconView::RegisterWindowZOrder(HWND hWnd_i)
{
    BOOL l_result = FALSE;
    
    CKtDPSCommMgr *l_server = CKtDPSCommMgr::GetInstance();
    
    if( l_server != NULL ){
        
        IMplusMessage *l_msg = NULL;

        CString l_sparent(_T(""));
        l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
        l_server->Connect(l_sparent);

        //l_server->Connect(CString(l_server->GetParentName()));
        
        l_msg = l_server->CreateMessage(FW_RegisterWindowZOrder,1,l_server->GetParentName()); 
        
        CString l_parameter = _T("");
        l_parameter.Format(_T("%ld/%s"), hWnd_i,_T("Z_ORDER_FLOAT_GL_LAYER"));
        l_parameter += _T("/");
        l_parameter += _T("Top");
        if(l_msg != NULL) {
        l_msg->SetParameter( _bstr_t(l_parameter));
        if (!l_server->SendMsg(l_msg) ) {
            l_result = FALSE;
        }
        
        DEL_MSG(l_msg);
        l_msg = NULL;
        }
		l_server->Disconnect();
    }		
    return l_result;	
}

BOOL CKtReconView::ChangeWindowZOrder(HWND hWnd_i)
{
    
    BOOL l_result = FALSE;
    
    CKtDPSCommMgr *l_server = CKtDPSCommMgr::GetInstance();
    
    if( l_server != NULL ){
        
        IMplusMessage *l_msg = NULL;
        CString l_sparent(_T(""));
        l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
        l_server->Connect(l_sparent);
        //l_server->Connect(CString(l_server->GetParentName()));
        
        l_msg = l_server->CreateMessage(FW_ChangeWindowZOrder,1,l_server->GetParentName()); 
        
        CString l_parameter = _T("");
        l_parameter.Format(_T("%ld/%s"), hWnd_i,_T("Z_ORDER_FLOAT_GL_LAYER"));
        l_parameter += _T("/");
        l_parameter += _T("Top");
        
        l_msg->SetParameter( _bstr_t(l_parameter));
        
        if (!l_server->SendMsg(l_msg) ) {
            l_result = FALSE;
        }
        
        DEL_MSG(l_msg);
        l_msg = NULL;
        l_server->Disconnect();              
    }		
    
    return l_result;	
}

void CKtReconView::AddZOrderLayer()
{
    try {
        CKtDPSCommMgr *pqm = NULL;
        pqm = CKtDPSCommMgr::GetInstance();
        
        if(pqm != NULL) {
            IMplusMessage *l_msg = NULL;
            CString l_parameter = _T("");
            l_parameter += _T("Z_ORDER_PANEL_GL_LAYER");
            l_parameter += _T("/");
            l_parameter += _T("Z_ORDER_ISEL_LAYER");//Add the layer after Z_ORDER_ISEL_LAYER
            BSTR l_bstr_parameter = _bstr_t(l_parameter);
            pqm->Connect(CString(pqm->GetParentName()));
            l_msg = pqm->CreateMessage(FW_AddZOrderLayer, 1, pqm->GetParentName(), l_bstr_parameter);
            
            if(l_msg != NULL) {
                pqm->SendMsg(l_msg);
                DEL_MSG(l_msg);
            }
        } else {
            CString l_cserrmsg = (_T("CKtDPSCommMgr NULL pointer"));
            CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            //log error//recheck the message to be written to the log file
        }
    } catch(...) {
		CString l_error_display = _T("");
		l_error_display = _T("Exception occured in AddZOrderLayer");
		    CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);

        //CKtReconLogHelper::WriteToErrorLog(_T("AddZOrderLayer(): Exception occured"), __LINE__, __FILE__);
        //log error
    }
}

BOOL CKtReconView::UnRegisterWindowZOrder(HWND hWnd_i)
{
    try{
        //KTRECON_TRACE(CTiPrepView::UnRegisterWindowZOrder);
        BOOL l_result = FALSE;
        CKtDPSCommMgr *l_server = CKtDPSCommMgr::GetInstance();
        
        if(l_server != NULL) {
            IMplusMessage *l_msg = NULL;
            BSTR l_bsparent = NULL;
            l_bsparent = l_server->GetParentName();
            
            if(NULL != l_bsparent) { 
                l_server->Connect(CString(l_bsparent)); 
                l_msg = l_server->CreateMessage(FW_UnregisterWindowZOrder, 1, l_server->GetParentName());
                CString l_parameter = _T("");
                l_parameter.Format(_T("%ld/%s"), hWnd_i, _T("Z_ORDER_FLOAT_GL_LAYER"));
                
                l_msg->SetParameter(_bstr_t(l_parameter));
                
                if(!l_server->SendMsg(l_msg)) {
                    l_result = FALSE;
                }
                
                DEL_MSG(l_msg);
                l_msg = NULL;
                l_server->Disconnect();
            } else {
                CString l_cserr(_T("DPSComm Parent is NULL "));
                CKtReconLogHelper::WriteToErrorLog(l_cserr, __LINE__);
                //log error//recheck the message to be written to the log file
            }
        } else { 
            CString l_cserr(_T("CDPSComManager is NULL "));
            CKtReconLogHelper::WriteToErrorLog(l_cserr, __LINE__);
            //log error//recheck the message to be written to the log file
            return FALSE;
        }
        
        return l_result;
    }catch(...) {
		CString l_error_display = _T("");
		l_error_display = _T("Exception occured in UnRegisterWindowZOrder");
		    CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);

        //CKtReconLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        //log error
        return FALSE;
    }
}


CKtReconView * CKtReconView::GetView()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
    CView * pView = pFrame->GetActiveView();
    
    if(!pView) {
        return NULL;
    }
    
    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if(! pView->IsKindOf(RUNTIME_CLASS(CKtReconView))) {
        return NULL;
    }
    
    return (CKtReconView *) pView;
}
/////////////////////////////////END OF FILE /////////////////////////////////////

int CKtReconView::StartGUI()
{
	if (m_ktReconDlg && m_ktReconDlg->GetSafeHwnd()) return E_NO_ERROR;
	m_ktReconDlg->Create(IDD_DIALOG_KTRECON, AfxGetMainWnd());
	m_ktReconDlg->ShowWindow(SW_SHOW);
//if (m_KtReconController)m_KtReconController->SetWaitCursor(true);
	//Register KtRecon Window Z-Order ++   
	//AddZOrderLayer();
	//RegisterWindowZOrder(m_ktReconDlg->GetSafeHwnd());
	return 0;
}
