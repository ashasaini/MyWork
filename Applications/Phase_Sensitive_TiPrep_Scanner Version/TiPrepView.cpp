/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrep.h"
#include "TiPrepDoc.h"
#include "TiPrepView.h"
#include "TiPrepLogHelper.h"
#include "TiPrepTrace.h"
#include "PhaseCorrection.h"
#include "SocketMgr.h" 
#include "ProcessExec.h"
#include "vfcDBMgr.h" 
#include "IniFileReader.h"
#include <direct.h>
#include "CommonCalculations.h"
#include "TiPrepController.h"
#include "NodeCopy.h"
#include "TiPrepChildThread.h"
#include "TiPrepProgressDlg.h"
#include "ApplyTi.h"
#include <MPlusDicomTag/MPlusMRAppCode.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
#define SCALING_FACTOR 1000 
/**
* Pointer to CTiPrepView class.
*/
CTiPrepView *g_MainView = NULL;
CTiPrepDlg * m_prep_dlg = NULL;

/**
* Used to store the modules under Process Tab which needs to be enable/disable.
*/
char *ProcessBtn[] = {
    "PSflowBtn",     // 0
        "MPR1Btn",       // 1
        "PerfusionBtn",  // 2
        "FBIBtn",        // 3
        "FusionBtn",     // 4
        "VolumeCalcBtn", // 5
        "MR3DBtn",       //6
        "CompositeBtn",  //7
        "MRSBtn",        //8
        "fMRIBtn",       //9
        "DynamicAnalBtn",//10
        "CardiacBtn"     //11
};


/**
* Used to store the modules under Tool Tab which needs to be enable/disable.
*/
char *ROIToolBtn[] = {
    "ROIToolsBtn",       // 0
        "ROITools2Btn",      // 1
        "ApplyViewBtn",      // 2
        "CommentRegBtn"      // 3
};

/////////////////////////////////////////////////////////////////////////////
// CTiPrepView

IMPLEMENT_DYNCREATE(CTiPrepView, CFormView)

BEGIN_MESSAGE_MAP(CTiPrepView, CFormView)
//{{AFX_MSG_MAP(CTiPrepView)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
ON_MESSAGE(WM_CMD_GET_SELIMG, OnCmdGetSelImg)   
ON_MESSAGE(WM_INIT_TIPREP, OnInitTiPrep)
ON_MESSAGE(WM_CMD_CLOSE, OnCmdClose)
ON_MESSAGE(WM_CMD_SHOW, OnCmdShow)
ON_MESSAGE(WM_CMD_HIDE, OnCmdHide)
ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
ON_MESSAGE(WM_CMD_DRAW_ROI, OnCmdDrawROIComplete)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()


const CString EXCEPTION_VIEW = L"Exception occurred in TiPrep View";


/**
* Constructor.
* This function is used for initialization of member variables.
*/
CTiPrepView::CTiPrepView()
: CFormView(CTiPrepView::IDD),
m_is_init_done(FALSE),
m_is_wait_request_isel(false),
m_is_wait_cursor(false),
m_max_counter(0),  
m_socketComm(NULL), 
m_pexec(NULL),
child_app(NULL), 
m_nInitialGuessFlag(0),
m_nisApplyDone(0) ,
m_T1Map(NULL),
IsT1MapSelected(false),
m_event_img_select(EVENT_INIT),
m_nEquFlag(0),
m_ExtraPtsINTDVD(1)
{
    //{{AFX_DATA_INIT(CTiPrepView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_prep_dlg = NULL;
    m_DisplayData = new CDisplayData();
}

/**
* Destructor.
* This function is used for releasing the allocated memory.
*/
CTiPrepView::~CTiPrepView()
{
    /*TIPREP_TRACE(CTiPrepView::~CTiPrepView)*/
        CDPSComManager *l_server = NULL;
    l_server = CDPSComManager::GetInstance();
    
    if(l_server != NULL) {
        l_server->DestroyServer();
        l_server->DeleteInstance();
        l_server = NULL;
    }
    
    if(NULL != m_DisplayData) {
        delete m_DisplayData;
        m_DisplayData = NULL;
    }
    
    if(NULL != m_T1Map) {   
        delete m_T1Map;
        m_T1Map = NULL;
    }
    
    if(NULL != m_pexec) {
        m_pexec->ExitProc();
        delete m_pexec;
        m_pexec = NULL;
    }
    
    CTiPrepLogHelper::DeleteInstance(); 
    
    CTiPrepController::DeleteInstance();
}

/*************************************************************
* Method:    	CTiPrepView
* Description: 	This function is used 
* Parameter: 	CTiPrepView & f_view :
* Returns:   	
*************************************************************/
CTiPrepView::CTiPrepView(CTiPrepView & f_view) : CFormView(CTiPrepView::IDD)
{
    TIPREP_TRACE(CTiPrepView::CTiPrepView,MRTraceMarker9);
    
    if(NULL != f_view.m_pexec) {
        if(m_pexec) {
            delete m_pexec;
            m_pexec = NULL;
        }
        
        m_pexec = new CProcessExec(*(f_view.m_pexec));
    }
    
    if(NULL != f_view.m_socketComm) {
        if(m_socketComm) {
            delete m_socketComm;
            m_socketComm = NULL;
        }
        m_socketComm = new CSocketMgr(*(f_view.m_socketComm));
    }
	child_app = f_view.child_app;
	m_is_init_done = f_view.m_is_init_done;
	m_is_wait_request_isel = f_view.m_is_wait_request_isel;
	m_is_wait_cursor = f_view.m_is_wait_cursor;
	m_nEquFlag = f_view.m_nEquFlag;
	m_event_img_select = f_view.m_event_img_select;
	m_max_counter = f_view.m_max_counter;
	IsT1MapSelected = f_view.IsT1MapSelected;
	m_pexec = f_view.m_pexec;
	m_socketComm = f_view.m_socketComm;
	m_lineFitData = f_view.m_lineFitData;
	m_nInitialGuessFlag = f_view.m_nInitialGuessFlag;
	m_nisApplyDone = f_view.m_nisApplyDone;
}

/*************************************************************
* Method:    	operator=
* Description: 	This function is used 
* Parameter: 	CTiPrepView & f_view :
* Returns:   	CTiPrepView&
*************************************************************/
CTiPrepView& CTiPrepView::operator = (CTiPrepView& f_view)
{
    TIPREP_TRACE(CTiPrepView::CTiPrepView,MRTraceMarker9);
    
    if(this == &f_view) {
        return(*this);
    }
    
    if(NULL != f_view.m_pexec) {
        if(m_pexec) {
            delete m_pexec;
            m_pexec = NULL;
        }
        
        m_pexec = new CProcessExec(*(f_view.m_pexec));
    }
    
    if(f_view.m_socketComm) {
        m_socketComm = f_view.m_socketComm;
    }
    
    return (*this);
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CTiPrepView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTiPrepView)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

/*************************************************************
* Method:       PreCreateWindow
* Description:  This function is used
* Parameter:    CREATESTRUCT & cs
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CFormView::PreCreateWindow(cs);
}

/*************************************************************
* Method:       OnInitialUpdate
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();
    g_MainView = (CTiPrepView*)this;
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
/*************************************************************
* Method:       OnCancelEditSrvr
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepView::OnCancelEditSrvr()
{
    GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CTiPrepView diagnostics

#ifdef _DEBUG
/*************************************************************
* Method:       AssertValid
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepView::AssertValid() const
{
    CFormView::AssertValid();
}

/*************************************************************
* Method:       Dump
* Description:  This function is used
* Parameter:    CDumpContext & dc
* Returns:      void
*************************************************************/
void CTiPrepView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

/*************************************************************
* Method:       GetDocument
* Description:  This function is used
* Returns:      CTiPrepDoc*
*************************************************************/
CTiPrepDoc* CTiPrepView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTiPrepDoc)));
    return (CTiPrepDoc*)m_pDocument;
}
#endif //_DEBUG

/*************************************************************
* Method:       GetView
* Description:  This function is used
* Returns:      CTiPrepView *
*************************************************************/
CTiPrepView * CTiPrepView::GetView()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
    CView * pView = pFrame->GetActiveView();
    
    if(!pView) {
        return NULL;
    }
    
    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if(! pView->IsKindOf(RUNTIME_CLASS(CTiPrepView))) {
        return NULL;
    }
    
    return (CTiPrepView *) pView;
}

/*************************************************************
* Method:       OnInitTiPrep
* Description:  This function is used for initialization.
*               CFW initialization.
*               CosVis channel initialization for drawing ROI.
* Parameter:    WPARAM wParam
* Parameter:    LPARAM lParam
* Returns:      LRESULT
*************************************************************/
LRESULT CTiPrepView::OnInitTiPrep(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    CTiPrepLogHelper::CreateInstance(BSTR(lParam));
    TIPREP_TRACE(CTiPrepView::OnInitTiPrep,MRTraceMarker3);
    m_prep_dlg = new CTiPrepDlg(AfxGetMainWnd());
    
    if(NULL == m_prep_dlg) {
        CString l_cserrmsg = (_T("Memory allocation Failed."));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return 0;
    }
    
    m_nisApplyDone = 0;
    CDPSComManager *l_server = NULL;
    l_server = CDPSComManager::GetInstance();
    
    if(l_server != NULL) {
        l_server->SetParentName(BSTR(lParam));
        BSTR l_bsparent = NULL;
        l_bsparent = l_server->GetParentName();
        CROIHandler *ROI_ptr = CROIHandler::GetInstance();
        
        if(l_server->InitializeServer()) {
            l_server->SetTiPrepStatus(STATE_READY);
            IMplusMessage *l_msg = NULL;
            
            if(NULL != l_bsparent) { 
                l_server->Connect(CString(l_bsparent)); 
                l_msg = l_server->CreateMessage(FW_ReportState, 1, l_bsparent);
                
                if(l_msg != NULL) {
                    l_server->SendMsg(l_msg);
                    DEL_MSG(l_msg);
                    l_msg = NULL;
                }
                
                l_server->Disconnect();
            } else {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("Could not connect to Parent. Parent Name not Set "), __LINE__, __FILE__);
            }
            
            RequestShowISelector(TRUE);
            //RegisterISelector();
            //Initialize DBServer
            CLocalDBMgr localdb;
            localdb.InitTest();
            GetSelectedImages();
            SetStateProcessTools(_T("false"));
            SetStateROITools(_T("false"));
            
            if(NULL == ROI_ptr) {
                CString l_csErrMsg(_T("CROIHandler NULL Pointer"));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                //return;
            } else {
                l_bsparent = l_server->GetParentName(); 
                
                if(NULL != l_bsparent) { 
                    BOOL l_flag = ROI_ptr->InitializeCosVisChannel(CString(l_bsparent));
                    
                    if(l_flag) {
                        HRESULT l_result = ROI_ptr->InitializeObserver(l_server->GetChannelName());
                        
                        if(S_OK == l_result) {
                            BOOL l_resultROI = ROI_ptr->AutoSaveDB(FALSE);
                        } else {
                            CString l_csErrMsg(_T("Observer initialization failed"));
                            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                        }
                    } else {
                        CString l_csErrMsg(_T("CosVisChannel intialization failed"));
                        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    }
                } else {
                    CString l_csErrMsg(_T("DPSComm Manager Parent is NULL"));
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                }
            }
        }
        
        CIniFileReader l_objIniReader;
        int l_nAppOption = 0;
        int l_nStatus = l_objIniReader.GetT1MapOption(l_nAppOption);
        if(E_NO_ERROR != l_nStatus){    //no need to return from here //pending    
            CString l_csErrMsg(_T("OnInitTiPrep() GetT1MapOption() returned error"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
        m_prep_dlg->SetAppOption(l_nAppOption);
        //m_prep_dlg->Create(IDD_TIPREP_DIALOG, AfxGetMainWnd());
		m_prep_dlg->Create(IDD_TIPREP_DIALOG,/* GetDesktopWindow()*/AfxGetMainWnd());
        InitSocket();
        //Register TiPrep Window Z-Order ++
        RegisterWindowZOrder(m_prep_dlg->GetSafeHwnd());
		CTiPrepController::GetInstance()->GetPagerInfo(); //AN scanner
    } else {
        CString l_cserrmsg = (_T("CDPSComManager Instance creation Failed"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return 0;
    }
    
    vector<CString> l_csloids;
    
    if(NULL != m_DisplayData) {
        m_DisplayData->getSelImgLOID(l_csloids);
    }
    
    int l_nstatus = getDataForm(l_csloids, l_csloids.size());
    if (E_NO_ERROR != l_nstatus)
    {
        CString l_cserrmsg = (_T("Failed to get data form from study file.(VFC reading)"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        
    }
    long l_nNumImgSel = 0;
    int l_nStatus = getNumImgSelectedISel(l_nNumImgSel);
    
    if(l_nStatus == E_NO_ERROR && l_nNumImgSel > 1) {
        l_nstatus = GetRepeatationTimeFromTag();
        if(E_NO_ERROR != l_nstatus){
            CString l_cserrmsg = (_T("Failed to get repetation time from study file.(VFC reading)"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            float l_tr_value = 0.0;
            m_prep_dlg->SetRepeatationTime(l_tr_value);
        }
    }

    BOOL f_nLicense = FALSE;
    CTiPrepController::GetInstance()->GetApplyTiLicense(f_nLicense);
    if(TRUE == f_nLicense){
        if(m_prep_dlg){
            m_prep_dlg->DisableApplyTiButton();
        }
    }

    return 1;
}

/*************************************************************
* Method:       OnCmdClose
* Description:  This function is a message handler used to free
*               the allocated memory.
* Parameter:    WPARAM wParam
*               Message parameter.
* Parameter:    LPARAM lParam
*               Message parameter.
* Returns:      LRESULT
*************************************************************/
LRESULT CTiPrepView::OnCmdClose(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    TIPREP_TRACE(CTiPrepView::OnCmdClose,MRTraceMarker9);
    
    if(NULL == m_prep_dlg) {
        return 0;
    }
    
    //Destroy the Dialog
    m_prep_dlg->DestroyWindow();
    delete m_prep_dlg;
    m_prep_dlg = NULL;
    //Here we have to free resources if any.
    return 1;
}

/*************************************************************
* Method:       OnCmdShow
* Description:  This function is a message handler used to show
*               TiPrep window.
* Parameter:    WPARAM wParam
*               Message parameter.
* Parameter:    LPARAM lParam
*               Message parameter.
* Returns:      LRESULT
*************************************************************/
LRESULT CTiPrepView::OnCmdShow(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    TIPREP_TRACE(CTiPrepView::OnCmdShow,MRTraceMarker9);
    CFrameWnd* l_wnd = GetParentFrame();
    
    if(NULL != l_wnd && m_prep_dlg != NULL) {
        l_wnd->ShowWindow(SW_HIDE);    //Hiding the View
        m_prep_dlg->ShowWindow(SW_SHOW); //Showing the Dialog
    } else { 
        CString l_cserrmsg = (_T("GetParentFrame Failed"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        //return 0;
    }
    
    int status = NULL; 
    status = (int)(wParam);
    
    if(/*(NULL != status) &&*/ (1 == status)) { // when TiPrep is to be shown after being hidden
        CROIHandler *ROI_ptr = CROIHandler::GetInstance();
        BOOL l_isroi_disp = FALSE;
        
        if(NULL == ROI_ptr) {
            CString l_cserrmsg = (_T("CROIHandler pointer NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
           // return 0;
        }else{
			Sleep(6);
            ROI_ptr->DeleteROI();
			//	Sleep(5);
				 //ROI_ptr->DeleteROI();
			           // vector<CString> l_csImgInfo;
//int l_status = m_DisplayData->getSelImgLOID(l_csImgInfo);
//if (l_csImgInfo.size() > 0) {
            for(int l_ni = 0; l_ni < NUMBEROFROI; l_ni++) {
                l_isroi_disp = m_prep_dlg->GetROIDisplayCheck(l_ni);
                BOOL l_resultROI = ROI_ptr->DrawROI(l_ni, l_isroi_disp);
            
                if(l_resultROI) {
                    l_resultROI = ROI_ptr->MouseMoveROI(TRUE);
                }
                }
         //   }
       }
        RequestShowISelector(TRUE);   
    } else { //Invocation time
        m_prep_dlg->SetDlgPos(); 
    }
    
    if(m_socketComm) {
        m_socketComm->Send("SHOW");
    } else { 
        CString l_cserrmsg = (_T("m_socketComm pointer NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
    }
    
    return 1;
}

/*************************************************************
* Method:       OnCmdHide
* Description:  This function is a message handler used to hide
*               TiPrep window.
* Parameter:    WPARAM wParam
*               Message parameter.
* Parameter:    LPARAM lParam
*               Message parameter.
* Returns:      LRESULT
*************************************************************/
LRESULT CTiPrepView::OnCmdHide(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    CFrameWnd* l_wnd = GetParentFrame();
    
    if(NULL != l_wnd && m_prep_dlg != NULL) {
        l_wnd->ShowWindow(SW_HIDE);    //Hiding the View
        m_prep_dlg->ShowWindow(SW_HIDE); //Hiding the Dialog
    } else { 
        CString l_cserrmsg = (_T("GetParentFrame Failed"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
       // return 0;
    }
    
    if(m_socketComm) {
        m_socketComm->Send("HIDE");
    } else { 
        CString l_cserrmsg = (_T("m_socketComm pointer NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
       // return 0;
    }
    
    if(SavePreviousROIstate() == FALSE) { //KJ
        CTiPrepLogHelper::WriteToErrorLog
            (_T("OnCmdHide(): SavePreviousROIstate() returned error"), __LINE__, __FILE__); //AS/added
    }
    
    CROIHandler::GetInstance()->DeleteROI();
    return 1;
}

/*************************************************************
* Method:       RegisterWindowZOrder
* Description:  This function is used to send FW_RegisterWindowZOrder
*               to parent page.
* Parameter:    HWND hWnd_i
*               Handle to window.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::RegisterWindowZOrder(HWND hWnd_i)
{
    TIPREP_TRACE(CTiPrepView::RegisterWindowZOrder,MRTraceMarker9);
    BOOL l_result = FALSE;
    CDPSComManager *l_server = CDPSComManager::GetInstance();
    
    if(l_server != NULL) {
        IMplusMessage *l_msg = NULL;
        BSTR l_bsparent = NULL;
        l_bsparent = l_server->GetParentName();
        
        if(NULL != l_bsparent) { 
            l_server->Connect(CString(l_bsparent)); 
            l_msg = l_server->CreateMessage(FW_RegisterWindowZOrder, 1, l_server->GetParentName());
            CString l_parameter = _T("");
            l_parameter.Format(_T("%ld/%s"), hWnd_i, _T("Z_ORDER_FLOAT_GL_LAYER"));
            l_parameter += _T("/");
            l_parameter += _T("Top");
            l_msg->SetParameter(_bstr_t(l_parameter));
            
			if(l_msg != NULL) {
				if(!l_server->SendMsg(l_msg)) {
                l_result = FALSE;
				}
            }
            if(l_msg != NULL) {
				DEL_MSG(l_msg);
				l_msg = NULL;
			}
            l_server->Disconnect();
        } else {
            CString l_cserr(_T("DPSComm Parent is NULL "));
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        }
    } else { 
        CString l_cserr(_T("CDPSComManager is NULL "));
        CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        return FALSE;
    }
    
    return l_result;
}

/*************************************************************
* Method:       ChangeWindowZOrder
* Description:  This function is used to send FW_ChangeWindowZOrder
*               to parent page.
* Parameter:    HWND hWnd_i
*               Handle to window.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::ChangeWindowZOrder(HWND hWnd_i)
{
//    TIPREP_TRACE(CTiPrepView::ChangeWindowZOrder,MRTraceMarker9);
    BOOL l_result = FALSE;
    CDPSComManager *l_server = CDPSComManager::GetInstance();
    
    if(l_server != NULL) {
        IMplusMessage *l_msg = NULL;
        BSTR l_bsparent = NULL;
        l_bsparent = l_server->GetParentName();
        
        if(NULL != l_bsparent) { 
            l_server->Connect(CString(l_bsparent)); 
            l_msg = l_server->CreateMessage(FW_ChangeWindowZOrder, 1, l_server->GetParentName());
            CString l_parameter = _T("");
            l_parameter.Format(_T("%ld/%s"), hWnd_i, _T("Z_ORDER_FLOAT_GL_LAYER"));
            l_parameter += _T("/");
            l_parameter += _T("Top");
            l_msg->SetParameter(_bstr_t(l_parameter));
            if(l_msg != NULL) {
            if(!l_server->SendMsg(l_msg)) {
                l_result = FALSE;
            }
			}
            if(l_msg != NULL) {
            DEL_MSG(l_msg);
            l_msg = NULL;
			}
            l_server->Disconnect();
        } else {
            CString l_cserr(_T("DPSComm Parent is NULL "));
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        }
    } else { //KJ-JULY
        CString l_cserr(_T("CDPSComManager is NULL "));
        CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        return FALSE;
    }
    
    return l_result;
}

/*************************************************************
* Method:       OnCmdDrawROIComplete
* Description:  This function is a message handler used to redraw ROI.
* Parameter:    WPARAM wParam
*               Message parameter.
* Parameter:    LPARAM lParam
*               Message parameter.
* Returns:      void
*************************************************************/
void CTiPrepView::OnCmdDrawROIComplete(WPARAM wParam, LPARAM lParam)
{
    TIPREP_TRACE(CTiPrepView::OnCmdDrawROIComplete,MRTraceMarker9)
        UNREFERENCED_PARAMETER(wParam);
    ROI_CMD *message = (ROI_CMD*) lParam;
    
    if(NULL == message) { 
        CTiPrepLogHelper::WriteToErrorLog
            (_T("OnCmdDrawROIComplete(): message is NULL"), __LINE__, __FILE__);
        return;
    }
    
    CString csSender = message->csSender;
    CString csCommand = message->csCommand;
    CString csParam = message->csParameter;
    _bstr_t custom = message->Custom;
    CString l_csmsg = csSender + _T(" ") + csCommand +  _T(" ") + csParam  + _T(" ");
    l_csmsg += CString(BSTR(custom));
    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), MRTraceMarker3, l_csmsg , _T("CTiPrepView::OnCmdDrawROIComplete")); 
    delete message;
    CROIHandler *l_ROIhandler = NULL;
    l_ROIhandler = CROIHandler::GetInstance();
    
    if(NULL == l_ROIhandler) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("OnCmdDrawROIComplete(): l_ROIhandler is NULL"), __LINE__, __FILE__);
        return;
    }
    
    l_ROIhandler->DrawROIComplete(csSender, csCommand, csParam, custom);
    
    if(message) {
        delete message;
        message = NULL;
    }
}

/*************************************************************
* Method:       RequestShowISelector
* Description:  This function is used to show/hide ISelector Window.
* Parameter:    BOOL bShow
*               Depending upon this value, ISelector Window is
*               shown/hidden.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::RequestShowISelector(BOOL bShow)
{
    TIPREP_TRACE(CTiPrepView::RequestShowISelector,MRTraceMarker3)
        
        try {
        //BOOL l_result = FALSE;
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RequestShowISelector(): l_server is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
        
        IMplusMessage *l_msg = NULL;
        tiprep_message_t l_msgid = bShow ? FW_RequestShowGL : FW_RequestHideGL;
        CString l_target;
        BSTR l_bsparent = NULL;
        l_bsparent = l_server->GetParentName();
        
        if(NULL != l_bsparent) {
            l_target = l_bsparent;
            l_target += ".";
            l_target += _T("iselector");   
            // Time being sending msg to iselector directly instead from controller.
            OutputDebugString(l_target);
            CString l_parameter(_T(""));
            
            if(l_msgid == FW_RequestHideGL) {
                l_parameter = _T("iselector");
                OutputDebugString(_T("Hide"));
            } else if(l_msgid == FW_RequestShowGL) {
                l_parameter = _T("770*311+2+843"); 
                OutputDebugString(_T("Show"));
            } else {

			}
            
            l_server->Connect(l_target);
            l_msg = l_server->CreateMessage(l_msgid, 1, _bstr_t(l_target), _bstr_t(l_parameter));
            
            if(l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            
            l_server->Disconnect();
        } else {
            CString l_cserr(_T("DPS Parent NULL .Cannot connect"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        }
    } catch(...) {
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       RegisterISelector
* Description:  This function is used to register with ISelector
*               by sending FW_RegisterMsg message.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::RegisterISelector()
{
    TIPREP_TRACE(CTiPrepView::RegisterISelector,MRTraceMarker3)
        
        try {
        //BOOL l_result = FALSE;
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RegisterISelector(): l_server is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("Controller");
        OutputDebugString(l_target);
        l_server->Connect(l_target);
        l_msg = l_server->CreateMessage(FW_RegisterMsg, 1, _bstr_t(l_target));
        
        if(l_msg != NULL) {
            l_server->SendMsg(l_msg);
            DEL_MSG(l_msg);
            l_msg = NULL;
        }
        
        l_server->Disconnect();
    } catch(...) {
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       GetSelectedImages
* Description:  This function is used to get the LOID information
*               of images selected by the user on the ISelector.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::GetSelectedImages()
{
    TIPREP_TRACE(CTiPrepView::GetSelectedImages,MRTraceMarker3)
        
        try {
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetSelectedImages(): l_server is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target.Empty();
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("iselector");
        l_server->Connect(l_target);
        l_msg = l_server->CreateMessage(GL_IS_Get_Selected_IC_Name, 1, _bstr_t(l_target));
        
        if(l_msg != NULL) {
            l_server->SendMsg(l_msg);
            DEL_MSG(l_msg);
            l_msg = NULL;
        } else {
            return FALSE;
        }
        
        l_server->Disconnect();
        m_is_wait_request_isel = true;
    } catch(...) {
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       SetStateProcessTools
* Description:  This function is used to enable/disable buttons
*               of some modules in the Process Tab.
* Parameter:    BSTR f_custom
*               Depending upon this value, buttons are enabled/disabled.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::SetStateProcessTools(BSTR f_custom)
{
    TIPREP_TRACE(CTiPrepView::SetStateProcessTools,MRTraceMarker3)
        
        try {
        int i = 0;
        //BOOL l_result = FALSE;
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SetStateProcessTools(): l_server is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("Process");
        
        for(i = 0; i < sizeof(ProcessBtn) / sizeof(ProcessBtn[0]); i++) {
            l_server->Connect(l_target);
            CString l_Str(ProcessBtn[i]);
            l_msg = l_server->CreateMessage(FW_Disable_Process_Btn, 1, _bstr_t(l_target), _bstr_t(l_Str), f_custom);
            
            if(l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            
            l_server->Disconnect();
        }
    } catch(...) {
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       SetStateROITools
* Description:  This function is used to enable/disable buttons
*               of some modules in the Tool Tab.
* Parameter:    BSTR f_custom
*               Depending upon this value, buttons are enabled/disabled.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepView::SetStateROITools(BSTR f_custom)
{
    TIPREP_TRACE(CTiPrepView::SetStateROITools,MRTraceMarker3)
        
        try {
        int i = 0;
        //BOOL l_result = FALSE;
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SetStateROITools(): l_server is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("MPlusClinAppToolBox");
        
        for(i = 0; i < sizeof(ROIToolBtn) / sizeof(ROIToolBtn[0]); i++) {
            l_server->Connect(l_target);
            CString l_Str(ROIToolBtn[i]);
            l_msg = l_server->CreateMessage(FW_Disable_Process_Btn, 1, _bstr_t(l_target), _bstr_t(l_Str), f_custom);
            
            if(l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            
            l_server->Disconnect();
        }
    } catch(...) {
        return FALSE;
    }
    
    return TRUE;
}
/*************************************************************
* Method:       GetROIInstance
* Description:  This function is used
* Returns:      CROIHandler*
*************************************************************/
CROIHandler* CTiPrepView::GetROIInstance()
{
    return CROIHandler::GetInstance();
}

/*************************************************************
* Method:       GetCTiPrepDlgInstance
* Description:  This function is used
* Returns:      CTiPrepDlg*
*************************************************************/
CTiPrepDlg* CTiPrepView::GetCTiPrepDlgInstance()
{
    return m_prep_dlg;
}

/*************************************************************
* Method:       OnCmdGetSelImg
* Description:  This function is invoked on recieving selected
*               image information from ISelector
* Parameter:    WPARAM wParam
*               Images selected are valid or not.
* Parameter:    LPARAM lParam
* Returns:      void
*************************************************************/
void CTiPrepView::OnCmdGetSelImg(WPARAM wParam, LPARAM lParam)
{
    TIPREP_TRACE(CTiPrepView::OnCmdGetSelImg,MRTraceMarker3);
    int  status = E_NO_ERROR;
    status = (int)(wParam);
    int ret_status = 0;
    int dlg_code = 0; //Default main dialog
    
    if(FALSE == m_is_init_done) { //During Invocation of TOOL
        CROIHandler *ROI_ptr = NULL;
        ROI_ptr = CROIHandler::GetInstance();
        CString l_csProtocolName(_T(""));
        
        if(NULL != ROI_ptr) {
            vector<CString> l_csImgInfo;
            CString l_csProcessName(_T(""));
            int l_status = m_DisplayData->getSelImgLOID(l_csImgInfo);
            
            if(E_NO_ERROR != l_status) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("OnCmdGetSelImg(): getSelImgLOID returned error"), __LINE__, __FILE__);
            }
            
            CLocalDBMgr localdb;
            unsigned short l_image_width = 0;
            unsigned short l_image_height = 0;
            
            if(0 < l_csImgInfo.size()) { 
				CString f_csStudyName;
                CString f_csCompletepath;//sb
               ret_status =  localdb.GetProtocolNameFromImageLevelDB(l_csImgInfo.at(0), l_csProtocolName,f_csStudyName,f_csCompletepath);//sb
                if (ret_status == E_NO_ERROR)
                {

                
              /*  ret_status = localdb.TagReadingForHeightWidthImage(l_csImgInfo.at(0),                                      
                    &l_image_width,
                    &l_image_height);*/
                }
            }
            
            BOOL l_bresultROI = TRUE;
            if(IsT1MapSelected == false) {
                l_bresultROI= ROI_ptr->GetsaveROIParams(l_csProtocolName, _T("AVG"),l_image_height,l_image_width);
            } else {
                l_bresultROI = ROI_ptr->GetsaveROIParams(l_csProtocolName, _T("MAP"),l_image_height,l_image_width);
            }

            if(FALSE == l_bresultROI){
                CTiPrepLogHelper::WriteToErrorLog(_T("Exception occuredwhile reading ROI parameters"), __LINE__, __FILE__);
            }
            
            ROI_ptr->SetProtocolNameForSetROIParams(l_csProtocolName);

            for(int l_ni = 0; l_ni < NUMBEROFROI; l_ni++) {
                BOOL isshow = FALSE;
                if (l_ni == 0) isshow = TRUE;
                BOOL l_resultROI = ROI_ptr->DrawROI(l_ni,isshow);
                
                if(l_resultROI) {
                    l_resultROI = ROI_ptr->MouseMoveROI(TRUE);
                }
            }
        }
        
        if(m_DisplayData) {
            vector<CString> imgLOIDS;
            int l_status2 = m_DisplayData->getSelImgLOID(imgLOIDS);
            
            if(E_NO_ERROR != l_status2) {
                CTiPrepLogHelper::WriteToErrorLog(_T("OnCmdGetSelImg(): getSelImgLOID returned error"), __LINE__, __FILE__);
            }
            
            if(imgLOIDS.size() > 0) {
                DisplaySeriesNumber(imgLOIDS.at(0));
            }
        }
        
        
        m_is_init_done = TRUE;
    } else {  //Reselect Button was clicked
        //int l_ntotalimgs = 0;
        //CLocalDBMgr localdb;
        CString l_csseries_name;
        //CString l_csimgloid;
        //vector<CString> l_loid;
        CString lcsdataForm(_T(""));
        CString l_csseries_number(_T(""));
        CString l_csDataForm;
        switch(m_event_img_select) {
        case EVENT_AUTOPHASE:
            m_prep_dlg->m_image_data_type_control.GetWindowText(l_csDataForm);
            CTiPrepController::GetInstance()->setDataFormPhaseCorrect(l_csDataForm);
            ret_status =  CTiPrepController::GetInstance()->GetSelImgInfoFromIsel(m_max_counter, EVENT_AUTOPHASE/*, l_csseries_name*/);
            break;
        case EVENT_POLARITY_CORRECTION:
            dlg_code = POLARITY_CORR_DLG; //AN DEFIX Added
            ret_status =CTiPrepController::GetInstance()->GetSelImgInfoFromIsel(m_max_counter, EVENT_POLARITY_CORRECTION/*, l_csseries_name*/);
            
            break;
        case EVENT_ADD_OFFSET:
            dlg_code = ADD_OFFSET_DLG;
            ret_status =CTiPrepController::GetInstance()->GetSelImgInfoFromIsel(m_max_counter, EVENT_ADD_OFFSET/*, l_csseries_name*/);
            
            break;
        case EVENT_MG_SELECT:
            dlg_code = PHASE_CORRECTION_DLG;
            ret_status =CTiPrepController::GetInstance()->GetSelImgInfoFromIsel(m_max_counter, EVENT_MG_SELECT/*, l_csseries_name*/);
            break;
        case EVENT_RE_SELECT:
            dlg_code = PHASE_CORRECTION_DLG;
            ret_status = CTiPrepController::GetInstance()->GetSelImgInfoFromIsel(m_max_counter, EVENT_RE_SELECT/*, l_csseries_name*/);
            break;
        case EVENT_IMG_SELECT:
            dlg_code = PHASE_CORRECTION_DLG;
            ret_status = CTiPrepController::GetInstance()->GetSelImgInfoFromIsel(m_max_counter, EVENT_IMG_SELECT/*, l_csseries_name*/);
            break;
        case EVENT_RESELECT:
            
            if(/*(NULL == status) ||*/ (E_NO_ERROR == status)) {
                //Images are valid
                CString l_csProtocolName(_T(""));
                CROIHandler *ROI_ptr = NULL;
                ROI_ptr = CROIHandler::GetInstance();
                
                if(m_prep_dlg->IsReselectWait()) {
                  ret_status =   m_prep_dlg->OnRecvReselectDone();
                    vector<CString> csImgLOID;
                    
                    if(m_DisplayData) { 
                        if(m_DisplayData->getSelImgLOID(csImgLOID) != E_NO_ERROR) {
                            CTiPrepLogHelper::WriteToErrorLog(_T("OnCmdGetSelImg(): getSelImgLOID returned error"), __LINE__, __FILE__);
                        }
                    }
                    
                    if(csImgLOID.size() > 0) {
                        DisplaySeriesNumber(csImgLOID.at(0));
                    }
                    
                }
            } else {
				CString l_csProtocolName (_T("")),mode (_T(""));
				BOOL l_falg = false;
				if (IsT1MapSelected)
				{
					mode = _T("MAP");

				} else {
					mode = _T("AVG");
				} 
				l_falg = (CTiPrepView::GetView())->GetROIInstance()->RedrawROI(true,l_csProtocolName,mode);
			}
            
            break;
        default:
            break;
        }
    }
    
    //AN/Modified/End
    SetWaitCursor(false);
    m_is_wait_request_isel = false;
    
    if(E_NO_ERROR != status) {
        CString l_cserror(_T(""));
        int l_nErrVal = status;
        
        if(l_nErrVal /*&& FALSE == l_cserror.IsEmpty()*/) {
            if(NULL != m_prep_dlg->GetSafeHwnd()) {
                DisplayErr(l_nErrVal,dlg_code); //AN DEFIX Added
            } else {
                CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
            }
        }
        ClearRepeatationTimeTextBox();            
    } else if (ret_status != E_NO_ERROR)
    {
        ClearRepeatationTimeTextBox();            
        DisplayErr(ret_status);
    } else {
		 vector <CString> l_csImageLoid;
        l_csImageLoid.clear();
	    //AN start
	if (m_event_img_select == EVENT_RESELECT || m_event_img_select == EVENT_INIT) {
		  if(/*(NULL == status) ||*/ (E_NO_ERROR == status)) {
			  int nImageNum = 0;
			    if(m_DisplayData) { 
                        if(m_DisplayData->getSelImgLOID(l_csImageLoid) != E_NO_ERROR) {
                            CTiPrepLogHelper::WriteToErrorLog(_T("OnCmdGetSelImg(): getSelImgLOID returned error"), __LINE__, __FILE__);
                        }
						nImageNum = l_csImageLoid.size();
                    }
	 int set_status =  getDataForm(l_csImageLoid, nImageNum); 
            if(nImageNum > 1) {
                int l_reading_status = (CTiPrepView::GetView())->GetRepeatationTimeFromTag();        
                if(E_NO_ERROR != l_reading_status){
                    CString l_cserrmsg = (_T("Failed to get repetation time from study file.(VFC reading)"));
                    CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                    (CTiPrepView::GetView())->ClearRepeatationTimeTextBox();
                    ClearRepeatationTimeTextBox();            
        DisplayErr(l_reading_status);
                }
                
            }
		  }
	}
    }
    
}
//MPower 043/Sep-2012/AN/Added :Start

/*************************************************************
* Method:       SetWaitCursor
* Description:  This function sets Wait cursor
* Parameter:    bool flag
*               indicating whether to display Wait cursor or not
* Returns:      void
*************************************************************/
void CTiPrepView::SetWaitCursor(bool flag)
{
//    TIPREP_TRACE(CTiPrepView::SetWaitCursor);
    m_is_wait_cursor = flag;
    
    if(NULL == m_prep_dlg) {
        CTiPrepLogHelper::WriteToErrorLog(_T("SetWaitCursor(): m_prep_dlg is NULL"), __LINE__, __FILE__);
    }else{    
        m_prep_dlg->SendMessage(WM_SETCURSOR, 0, 0);
    }
}

/*************************************************************
* Method:       IsWaitCursor
* Description:  This function returns whether current cursor
*               is Wait Cursor
* Returns:      bool
*************************************************************/
bool CTiPrepView::IsWaitCursor()
{
    return m_is_wait_cursor;
}

/*************************************************************
* Method:       IsWaitIsel
* Description:  This function returns whether TiPrep is waiting
*               for return message from ISelector during
*               reselection / invocation.
* Returns:      bool
*************************************************************/
bool CTiPrepView::IsWaitIsel()
{
    return m_is_wait_request_isel;
}

/*************************************************************
* Method:       OnApply
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::OnApply(int f_nEqnSelectFlag)
{
    try {
        int l_nstatus = E_NO_ERROR;
        LONG TotalAnnularPoints = 0;
        double l_trValue = 0.0;
        int TotalNewPtsforXaxis = 0;
        CExtraPointsGen *l_CExtraPoint = NULL;

        if(NULL == m_prep_dlg) { 
            CString l_csErrMsg(_T("m_prep_dlg Pointer is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        m_prep_dlg->GUIOnIdle();
        m_prep_dlg->GUIOnApply();
        
        
        //Important to clear graph window before any memory manipulations
        m_prep_dlg->ClearGraphCtl();
        if((BST_UNCHECKED == m_prep_dlg->m_checkDispROI1Data.GetCheck())
            && (BST_UNCHECKED == m_prep_dlg->m_checkDispROI2Data.GetCheck())
            && (BST_UNCHECKED == m_prep_dlg->m_checkDispROI3Data.GetCheck())){
            CString l_cserror(_T(""));
            l_cserror.Format(_T("SaveRoiDataToFile() : No ROI has been selected\n"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
            //int l_nErrCode = E_NO_ROI_SELECTED;
            m_prep_dlg->ClearGraphCtl();
            m_prep_dlg->GUIOnIdle();
            return(E_NO_ROI_SELECTED);
        }
        CString l_temp_str = _T(""), l_temp_errStr = _T(""), l_temp_t1val = _T("");
        CString l_error = _T("");
        long l_Img_selected = 0;
        unlink("E:\\DataLogForTiPrep.txt");
        unlink("E:\\FitData.txt");
        unlink("E:\\OrgDataFromDataClass.txt");
        m_nEquFlag = f_nEqnSelectFlag;
        
        if(NULL == m_DisplayData) {
            CString l_csErrMsg(_T("NULL Pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            m_prep_dlg->ClearGraphCtl();
            m_prep_dlg->GUIOnIdle();
            return E_ERROR;
        }
        
        //release current buffers
        m_DisplayData->DeAllocateMemory();//free all heavy data buffers
        m_DisplayData->getSelImgNum(&l_Img_selected);
        
        if(1 >= l_Img_selected) {
           // CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            m_prep_dlg->ClearGraphCtl();
            m_prep_dlg->GUIOnIdle();
            return E_APPLY_IMAGE_NOT_PROPER;
        }
        
        if(true == IsT1MapSelected) {
            CString l_csStudyFileName = _T("");
            
            if(E_NO_ERROR == GetStudyNameFromDB(l_csStudyFileName)) {
                m_DisplayData->SetStudyFileName(l_csStudyFileName);
            } else {
                //Log error as getting study name failed
                CTiPrepLogHelper::WriteToErrorLog(_T("GetStudyNameFromDB failed"), __LINE__);
                m_prep_dlg->GUIOnIdle();
                return E_DB_ERROR;
            }
        }

        if(SavePreviousROIstate() == FALSE) {   
            CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): SavePreviousROIstate() returned error"), __LINE__, __FILE__);
        }
        
        m_DisplayData->setTimeStampData(l_Img_selected);
        
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if(false == m_prep_dlg->m_tigraphctrl.SetNumberImgSelInGraph(l_Img_selected)) {
            
            CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            m_prep_dlg->ClearGraphCtl();
            m_prep_dlg->GUIOnIdle();
            return E_ERROR;
        }
        
        //CLocalDBMgr localdb;
        vector<CString> l_csloid;
        vector<CString> l_csNewLOIDs;
        l_nstatus = m_DisplayData->getSelImgLOID(l_csloid);
        
        if(E_NO_ERROR != l_nstatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnApply(): getSelImgLOID returned error"), __LINE__, __FILE__);
            m_prep_dlg->GUIOnIdle();
            return(E_ERROR);
        }
        
        int filtersz = 0;
        int l_nseries_status = 0;
        
        if(IsT1MapSelected && m_prep_dlg->IsAvgFilterOn()) {
            l_nstatus =  m_prep_dlg->GetFilterSize(filtersz);
            
            if(l_nstatus != E_NO_ERROR) {
                m_prep_dlg->GUIOnIdle();
                return l_nstatus; //Important, validation message not displayed inside function,need to display in caling function
            }
        }
        
        if(IsT1MapSelected && m_prep_dlg->IsAvgFilterOn() && m_prep_dlg->IsCreateFilteredSeries()) {
            //l_nseries_status = localdb.CreateSeriesCopy(l_csloid, l_csNewLOIDs);
            l_nseries_status = CTiPrepController::GetInstance()->CreateSeries(l_csloid, l_csNewLOIDs);
        }
        
        if(false == IsT1MapSelected) {                                                                          //MAP DEPENDANCY HERE
            l_nstatus = m_DisplayData->setROIData();
        } else {
            ClearMapFiles();
            l_nstatus = m_DisplayData->setAnnularMapData(m_prep_dlg->IsAvgFilterOn(), filtersz, m_prep_dlg->IsCreateFilteredSeries(), l_csNewLOIDs); //AN /AVG Filter/Modified
        }
        
        if(IsT1MapSelected && m_prep_dlg->IsAvgFilterOn() && m_prep_dlg->IsCreateFilteredSeries() && l_nseries_status == E_NO_ERROR) {
            RefreshIsel();
        }
        
        //AN /End
        if(E_NO_ERROR != l_nstatus) {
            m_prep_dlg->ClearGraphCtl();
            
          /*  if(E_ERR_IMG_LOAD_FAIL != l_nstatus && I_ROI_OUT_RGN != l_nstatus) {
                l_error.Format(_T("Unable to load Image ROI data."));
                CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            }*/
            
            m_DisplayData->DeAllocateMemory();//free all heavy data buffers
            m_prep_dlg->GUIOnIdle();
            return l_nstatus; //return function status which has its error code
        }
        
        
        CString l_csNoise_Correction_Val(_T(""));
        l_nstatus = m_DisplayData->setNoiseROIDataFromRoi();
        
        if(E_NO_ERROR != l_nstatus) {
            m_prep_dlg->ClearGraphCtl();
            
            /*if(E_ERR_IMG_LOAD_FAIL != l_nstatus && I_ROI_OUT_RGN != l_nstatus) {
                l_error.Format(_T("Unable to load Image Noise ROI data."));
                // CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__, ERRORVIEW_DISPLAY);
            }*/
            
            m_DisplayData->DeAllocateMemory();
            m_prep_dlg->GUIOnIdle();
            return l_nstatus;
        }
        
        if(m_prep_dlg->m_checkOffsetCorrection.GetCheck() == BST_CHECKED) {
            BOOL l_roi[MAX_NOISE_ROI] = {FALSE, FALSE}; 
            l_roi[0] = m_prep_dlg->m_checkNoiseROI1Data.GetCheck();
            l_roi[1] = m_prep_dlg->m_checkNoiseROI2Data.GetCheck();
            l_nstatus = m_DisplayData->calcNoiseROIMean(l_roi, l_csNoise_Correction_Val); 
            
            if(E_NO_ERROR != l_nstatus) {
                CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): calcNoiseROIMean() returned an error"), __LINE__, __FILE__);
            }
            
            m_prep_dlg->m_NoiseCorrectionVal.SetWindowText(l_csNoise_Correction_Val);
            l_nstatus = m_DisplayData->performNoiseCorrection(); 
            
            if(E_NO_ERROR != l_nstatus) {
                CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): performNoiseCorrection() returned an error"), __LINE__, __FILE__);
            }
            
            //            m_DisplayData->setMinMaxSigData();
            BOOL l_bstatus = m_DisplayData->setROI_Inv_Data(l_Img_selected);
            
            if(TRUE != l_bstatus) { //AS/revw/Added
                m_prep_dlg->ClearGraphCtl();
                CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): setROI_Inv_Data() returned an error"), __LINE__, __FILE__);
                m_DisplayData->DeAllocateMemory();//free all heavy data buffers
                m_prep_dlg->GUIOnIdle();
                return(E_ERROR);
            }
        } else {
            l_csNoise_Correction_Val.Format(_T("---.---"));
            m_prep_dlg->m_NoiseCorrectionVal.SetWindowText(l_csNoise_Correction_Val);
        }
        
        //Get Total Number of ROIS or Total Number of POints
        if(true == IsT1MapSelected) {                                                                          //MAP DEPENDANCY HERE
            TotalAnnularPoints  = m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();
        } else {
            TotalAnnularPoints = MAX_NUM_ROI;
        }
        
        if(TotalAnnularPoints <= 0) { 
            m_prep_dlg->ClearGraphCtl();
            l_error.Format(_T("TotalAnnularPoints are Zero."));
            // CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__, ERRORVIEW_DISPLAY);
            m_DisplayData->DeAllocateMemory();
            m_prep_dlg->GUIOnIdle();
            return E_NO_DATA_FOUND_MAP;
        }
        
        int *l_nTime_Stamp = m_DisplayData->Time_stamp();
        
        if(NULL == l_nTime_Stamp) {
            m_prep_dlg->ClearGraphCtl();
            l_error.Format(_T("l_nTime_Stamp is NULL."));
            CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            m_DisplayData->DeAllocateMemory();
            m_prep_dlg->GUIOnIdle();
            return E_ERROR;
        }
        
        int l_nEqnFlag = 0;
        l_nstatus = getSelectedEqn(l_nEqnFlag);
        
        if(f_nEqnSelectFlag == 0 || f_nEqnSelectFlag == 3 || f_nEqnSelectFlag == 4) {
            l_trValue = 0.0;
        } else {
            m_prep_dlg->m_RepTime.GetWindowText(l_temp_str);
            
            if(TRUE != m_prep_dlg->ValidateOffset(l_temp_str)) {
                m_prep_dlg->ClearGraphCtl();
                
                m_prep_dlg->m_RepTime.SetWindowText(_T(""));
                m_DisplayData->DeAllocateMemory();
                m_prep_dlg->GUIOnIdle();
                return I_VALID_TR;
            }
            
            char *l_cpC = NULL;
            l_cpC = new char[150];
            
            if(NULL == l_cpC) {
                m_prep_dlg->ClearGraphCtl();
                CString l_cserrmsg(_T("Memory Allocation Failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                m_DisplayData->DeAllocateMemory();
                m_prep_dlg->GUIOnIdle();
                return E_ERROR;
            }
            
            wcstombs(l_cpC, l_temp_str, 100);
            l_trValue = atof(l_cpC);
            
            if(NULL != l_cpC) {
                delete []l_cpC;
                l_cpC = NULL;
            }
        }
        
        double **l_ROI_Org_Data = m_DisplayData->getROIOrgData();
        double **l_Roi_Inv_Data = m_DisplayData->getROIIvnData();
        double **l_Roi_Inv_Rev_Data = m_DisplayData->getROIMinRevData();
        double **l_Roi_Fit_Data = m_DisplayData->getROIFitData();
        double **l_Roi_Fit_Rev_Data = m_DisplayData->getROIFitMinRevData();
        double **l_Noise_ROI_Org_Data = m_DisplayData->getNoiseROIOrgData();    
        
        if(l_ROI_Org_Data == NULL ||
            l_Roi_Inv_Data == NULL ||
            l_Roi_Inv_Rev_Data == NULL ||
            l_Roi_Fit_Data == NULL ||
            l_Roi_Fit_Rev_Data == NULL ||
            l_Noise_ROI_Org_Data == NULL) {
            m_prep_dlg->ClearGraphCtl();
            l_error.Format(_T("Display data Buffers are NULL."));
            CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            m_DisplayData->DeAllocateMemory();
            m_prep_dlg->GUIOnIdle();
            return E_ERROR;
        }
        
        double *l_Roi_Ti_Values = new double[TotalAnnularPoints];
        double *l_Roi_Ti_Rev_Values = new double[TotalAnnularPoints];
        double *l_Roi_Fit_Err_Values = new double[TotalAnnularPoints];
        double *l_Roi_Fit_Rev_Err_Values = new double[TotalAnnularPoints];
        double *l_Roi_T1_values = new double[TotalAnnularPoints];
        double *l_Roi_T1_Min_Rev_Values = new double[TotalAnnularPoints];
        
        if(NULL == l_Roi_Ti_Values ||
            NULL == l_Roi_Ti_Rev_Values ||
            NULL == l_Roi_Fit_Err_Values ||
            NULL == l_Roi_Fit_Rev_Err_Values ||
            NULL == l_Roi_T1_values ||
            NULL == l_Roi_T1_Min_Rev_Values) {
            
            
            if(NULL != l_Roi_Ti_Values) {
                delete [] l_Roi_Ti_Values;
                l_Roi_Ti_Values  = NULL;
            }
            
            if(NULL != l_Roi_Ti_Rev_Values) {
                delete [] l_Roi_Ti_Rev_Values;
                l_Roi_Ti_Rev_Values = NULL;
            }
            
            if(NULL != l_Roi_Fit_Err_Values) {
                delete [] l_Roi_Fit_Err_Values;
                l_Roi_Fit_Err_Values = NULL;
            }
            
            if(NULL != l_Roi_Fit_Rev_Err_Values) {
                delete [] l_Roi_Fit_Rev_Err_Values;
                l_Roi_Fit_Rev_Err_Values = NULL;
            }
            
            if(NULL != l_Roi_T1_values) {
                delete [] l_Roi_T1_values;
                l_Roi_T1_values = NULL;
            }
            
            if(NULL != l_Roi_T1_Min_Rev_Values) {
                delete [] l_Roi_T1_Min_Rev_Values;
                l_Roi_T1_Min_Rev_Values = NULL;
            }
            
            l_ROI_Org_Data = NULL;
            l_Noise_ROI_Org_Data = NULL;
            l_Roi_Inv_Data = NULL;
            l_Roi_Inv_Rev_Data = NULL;
            l_Roi_Fit_Data = NULL;
            l_Roi_Fit_Rev_Data = NULL;
            m_prep_dlg->ClearGraphCtl();
            l_error.Format(_T("Display data Buffers are NULL."));
            CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            m_DisplayData->DeAllocateMemory();
            m_prep_dlg->GUIOnIdle();
            return E_ERROR;
        }
        
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //PreProcessing Before Extra Point Generation
        //to switch off extra point generation just make l_CExtraPoint = NULL before this step
        //this preprocessing is common for Both NORMAL calc and MINREV calc
        int statusEPG = E_NO_ERROR;
        l_CExtraPoint = NULL;
        
        if (l_Img_selected <= 50){
            m_ExtraPtsINTDVD = 5;
            l_CExtraPoint = new CExtraPointsGen(m_ExtraPtsINTDVD);//KJ
        } else {
		    m_ExtraPtsINTDVD = 1; //AN/Revw/Reset to 1  as this is a member variable, consider performing 100 images after perfoming 20 images, member will be 5 if not reset
		}
        
        if(l_CExtraPoint != NULL) {
            TotalNewPtsforXaxis = l_Img_selected;
            statusEPG = l_CExtraPoint->ModeSetting(/*IsT1MapSelected,*/TotalAnnularPoints);
            
            if(statusEPG == E_NO_ERROR) {
                statusEPG = l_CExtraPoint->DivideIntervalAndAllocateNewBuffers(TotalNewPtsforXaxis, l_nTime_Stamp);
                
                if(statusEPG == E_NO_ERROR) {
                    statusEPG = l_CExtraPoint->GenerateBuffgenX(l_nTime_Stamp);
                    
                    if(statusEPG != E_NO_ERROR) {
                        TotalNewPtsforXaxis = 0;
                        CString l_csErrMsg = (_T("GenerateBuffgenX failed"));                        
                        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__,__FILE__);
                        l_error = (_T("Graph Smoothening Failed"));
                        CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
                        //set flag to indicate extra point generation failed
                    }
                } else {
                    TotalNewPtsforXaxis = 0;
                    CString l_csErrMsg =(_T("DivideIntervalAndAllocateNewBuffers failed"));                    
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__,__FILE__);
                    l_error = (_T("Graph Smoothening Failed"));
                    CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
                    //set flag to indicate extra point generation failed
                }
            } else {
                CString l_csErrMsg =(_T("ModeSetting failed"));                
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__,__FILE__);
                l_error = (_T("Graph Smoothening Failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
                TotalNewPtsforXaxis = 0;
                //set flag to indicate extra point generation failed
            }
        } else {
            CString l_csErrMsg =(_T("l_CExtraPoint is NULL"));            
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__,__FILE__);
            l_error = (_T("Graph Smoothening Failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
            TotalNewPtsforXaxis = 0;
            //set flag to indicate extra point generation failed
        }
		   //DWORD dwError;
        
          // if(!SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS  )){
             //  dwError = GetLastError();
             //  MessageBox(_T("unable to set high priority"),NULL,NULL);
          // } 
		                  int thread_count = 2;
							time_t t1 = time(NULL);
           if(true == IsT1MapSelected) {//------------------------------------------------------MAP MODE

               int l_nroi = 0;
               int numpoints_thread = 0;
//	if (TotalAnnularPoints > 50000 && l_Img_selected > 80)
//	{
//		thread_count = 3; //Performance benefit for 3 threads is only for very large data
//	}
				CIniFileReader l_objIniReader;;
				l_objIniReader.GetNumThreads(thread_count);

               HANDLE * handle_array = NULL;
               handle_array = new HANDLE[thread_count];

               CLineFitStrategy * linefit = NULL;
               linefit = new CLineFitStrategy[thread_count]; //Need to set manual guess parameters from m_line_fit data
     
               CTiPrepChildThread * child_threads = NULL;
               child_threads = new CTiPrepChildThread[thread_count];

               if(NULL == child_threads || linefit == NULL || handle_array == NULL){
                   if (linefit) {delete [] linefit; linefit = NULL;}
                   if (handle_array) {delete [] handle_array; handle_array = NULL;}
                   if(child_threads) {delete [] child_threads;child_threads = NULL;}
                   
                   if(NULL != l_Roi_Ti_Values) {delete [] l_Roi_Ti_Values;l_Roi_Ti_Values  = NULL;}
                   if(NULL != l_Roi_Ti_Rev_Values) {delete [] l_Roi_Ti_Rev_Values;l_Roi_Ti_Rev_Values = NULL;}
                   if(NULL != l_Roi_Fit_Err_Values) {delete [] l_Roi_Fit_Err_Values;l_Roi_Fit_Err_Values = NULL;}
                   if(NULL != l_Roi_Fit_Rev_Err_Values) {delete [] l_Roi_Fit_Rev_Err_Values;l_Roi_Fit_Rev_Err_Values = NULL;}
                   if(NULL != l_Roi_T1_values) {delete [] l_Roi_T1_values;l_Roi_T1_values = NULL;}
                   if(NULL != l_Roi_T1_Min_Rev_Values) {delete [] l_Roi_T1_Min_Rev_Values;l_Roi_T1_Min_Rev_Values = NULL;}
                   
                   if(l_CExtraPoint) {delete l_CExtraPoint; l_CExtraPoint = NULL;}
                   m_prep_dlg->ClearGraphCtl();
                   l_error.Format(_T("Display data Buffers are NULL."));
                   CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
                   m_DisplayData->DeAllocateMemory();
                   m_prep_dlg->GUIOnIdle();
                   return E_ERROR;
               }

               for (int l_nt = 0; l_nt < thread_count; l_nt++) {
                   numpoints_thread = TotalAnnularPoints/ thread_count;
                   if (l_nt == thread_count - 1) {
                       numpoints_thread = TotalAnnularPoints - (l_nt)*(TotalAnnularPoints/ thread_count);
                   }
                   if_error(l_nstatus = child_threads[l_nt].StartThread(
                       l_Roi_Ti_Values,
                       l_Roi_T1_values,
                       l_nroi,
                       l_Roi_Inv_Data,
                       l_Roi_Fit_Data,
                       l_Roi_Fit_Err_Values,
                       l_Img_selected,
                       l_nTime_Stamp,l_trValue,
                       f_nEqnSelectFlag,
                       l_CExtraPoint,
                       m_nInitialGuessFlag,
                       numpoints_thread,handle_array[l_nt],&linefit[l_nt],
                       l_Roi_Ti_Rev_Values,   
                       l_Roi_T1_Min_Rev_Values,
                       l_Roi_Inv_Rev_Data,
                       l_Roi_Fit_Rev_Data,
                       (l_Roi_Fit_Rev_Err_Values ),
                       m_prep_dlg->GetSafeHwnd())) {
                       //break;
                   }
				 //  if (TotalAnnularPoints > 30000)
				//   SetThreadPriority(handle_array[l_nt],REALTIME_PRIORITY_CLASS    );
                 /*  if (l_nt == 0)
                       SetThreadPriority(handle_array[l_nt],THREAD_PRIORITY_HIGHEST );
                   else if (l_nt == 1 || l_nt == 2)
                       SetThreadPriority(handle_array[l_nt],THREAD_PRIORITY_ABOVE_NORMAL  );
				   else
						SetThreadPriority(handle_array[l_nt],THREAD_PRIORITY_ABOVE_NORMAL  );*/
                   l_nroi += numpoints_thread;                
               }//thread creation for loop ends           

               
               CTiPrepProgressDlg *ProgressDlg = NULL;
               ProgressDlg = new CTiPrepProgressDlg(m_prep_dlg);
               ProgressDlg->SetThreadHandles(handle_array);
               ProgressDlg->SetRange(TotalAnnularPoints);
               ProgressDlg->SetTotalThreadsRunning(thread_count);
               m_prep_dlg->SetProgressDlg(ProgressDlg);
               ProgressDlg->DoModal();
               
               WaitForMultipleObjects(thread_count, handle_array, true, INFINITE);
			   //AN/Revw/right place to delete ProgressDlg pointer
			   if (ProgressDlg) {
			   delete ProgressDlg;
			   ProgressDlg = NULL;
			   }
               m_prep_dlg->SetProgressDlg(NULL);
			   time_t t2 = time(NULL);
			   int diff = difftime(t2,t1);
			   CString msg;
			   msg.Format(_T("time taken is : %d\n"),diff);
			   CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"),MRTraceMarker20,msg,_T("CTiPrepView::OnApply()"));

               
               
             //  if(!SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS  )){
              //     dwError = GetLastError();
                 //  MessageBox(NULL,_T("unable to set high priority"),NULL);
              // } 
               if (linefit) {delete [] linefit; linefit = NULL;}
               if (handle_array) {delete [] handle_array; handle_array = NULL;}
               if(child_threads) {delete [] child_threads;child_threads = NULL;}
           }else{//--------------------------------------------------------------------------------AVG MODE
               m_lineFitData.AllocTimeStamp(l_nTime_Stamp,l_Img_selected);
               m_lineFitData.AllocTmpBuffers(l_Img_selected,f_nEqnSelectFlag);

		        for(int l_nroi = 0; l_nroi < TotalAnnularPoints; l_nroi++) {
		            l_nstatus = 0;
		            if_error(l_nstatus = m_lineFitData.CalculateTi_NonLinear(l_Roi_Ti_Values, 
		                l_Roi_T1_values,
		                l_nroi,
		                l_Roi_Inv_Data[l_nroi],
		                l_Roi_Fit_Data[l_nroi],
		                (l_Roi_Fit_Err_Values + l_nroi),
		                l_Img_selected,
		                l_nTime_Stamp,
		                l_trValue,
		                f_nEqnSelectFlag,
		                l_CExtraPoint,
		                0, m_nInitialGuessFlag)) {
		                //break;
		            }
		            if_error(l_nstatus = m_lineFitData.CalculateTi_NonLinear(l_Roi_Ti_Rev_Values,   
		                l_Roi_T1_Min_Rev_Values,
		                l_nroi,
		                l_Roi_Inv_Rev_Data[l_nroi],
		                l_Roi_Fit_Rev_Data[l_nroi],
		                (l_Roi_Fit_Rev_Err_Values + l_nroi),
		                l_Img_selected,
		                l_nTime_Stamp,
		                l_trValue,
		                f_nEqnSelectFlag,
		                l_CExtraPoint,
		                1, m_nInitialGuessFlag)) {
		                //break;
		            }
		        }
                m_lineFitData.DeAllocTimeStampData();
                m_lineFitData.DeAllocTmpBuffers(f_nEqnSelectFlag);
           }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if((l_CExtraPoint != NULL) && (TotalNewPtsforXaxis > 0) && (l_CExtraPoint->m_FailureFlag == 0)) {
            //clear x and y buffer from displaydata class allocate with new size
            //and get the copy of the new generated buffers
            int l_nMemStatus = m_DisplayData->DeallocateBufferXY();
            if(E_NO_ERROR != l_nMemStatus){
                //set flag to indicate extra point generation failed
            }
            l_nMemStatus = m_DisplayData->AllocateBufferXY(TotalNewPtsforXaxis);

            if(E_NO_ERROR != l_nMemStatus){
                //set flag to indicate extra point generation failed
            }
            l_Roi_Fit_Data = NULL;
            l_Roi_Fit_Rev_Data = NULL;
            l_nTime_Stamp = NULL;
            l_nTime_Stamp = m_DisplayData->Time_stamp();
            l_Roi_Fit_Data = m_DisplayData->getROIFitData();
            l_Roi_Fit_Rev_Data = m_DisplayData->getROIFitMinRevData();
            
            if(NULL == l_nTime_Stamp || NULL == l_Roi_Fit_Data || NULL == l_Roi_Fit_Rev_Data) {
                if(NULL != l_Roi_Ti_Values) {delete [] l_Roi_Ti_Values;l_Roi_Ti_Values  = NULL;}
                if(NULL != l_Roi_Ti_Rev_Values) {delete [] l_Roi_Ti_Rev_Values;l_Roi_Ti_Rev_Values = NULL;}
                if(NULL != l_Roi_Fit_Err_Values) {delete [] l_Roi_Fit_Err_Values;l_Roi_Fit_Err_Values = NULL;}
                if(NULL != l_Roi_Fit_Rev_Err_Values) {delete [] l_Roi_Fit_Rev_Err_Values;l_Roi_Fit_Rev_Err_Values = NULL;}
                if(NULL != l_Roi_T1_values) {delete [] l_Roi_T1_values;l_Roi_T1_values = NULL;}
                if(NULL != l_Roi_T1_Min_Rev_Values) {delete [] l_Roi_T1_Min_Rev_Values;l_Roi_T1_Min_Rev_Values = NULL;}
                
                l_ROI_Org_Data = NULL;
                l_Noise_ROI_Org_Data = NULL;
                l_Roi_Inv_Data = NULL;
                l_Roi_Inv_Rev_Data = NULL;
                l_Roi_Fit_Data = NULL;
                l_Roi_Fit_Rev_Data = NULL;
                
                if(l_CExtraPoint) {delete l_CExtraPoint; l_CExtraPoint = NULL;}
                
                
                m_prep_dlg->ClearGraphCtl();
                l_error = (_T("Memory Issue:Extra Point data Buffers are NULL."));
                CTiPrepLogHelper::WriteToErrorLog(l_error, __LINE__, __FILE__);
                m_DisplayData->DeAllocateMemory();
                m_prep_dlg->GUIOnIdle();
                return E_ERROR;
            } else {
                l_CExtraPoint->GetCopyOfNewBuffers(l_nTime_Stamp, l_Roi_Fit_Data, l_Roi_Fit_Rev_Data);
            }
        }
        
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        m_DisplayData->setTiValues(l_Roi_Ti_Values, l_Roi_Ti_Rev_Values);
        m_DisplayData->setErrorValues(l_Roi_Fit_Err_Values, l_Roi_Fit_Rev_Err_Values);
        m_DisplayData->setT1Values(l_Roi_T1_values, l_Roi_T1_Min_Rev_Values);
        int l_Ymax = 0;
        int l_Ymin = 0;
        int l_Xmax = 0;
        int l_Xmin = 0;
        BOOL l_bRoi[MAX_LINE_ROI];
        BOOL l_bDispGrap[3];
        BOOL l_bRevMin[MAX_NUM_ROI];
        //CTiPrepDlg *l_prep_dlg;
        //l_prep_dlg = GetCTiPrepDlgInstance();
        l_bRoi[ROI_1] = m_prep_dlg->m_checkDispROI1Data.GetCheck();
        l_bRoi[ROI_2] = m_prep_dlg->m_checkDispROI2Data.GetCheck();
        l_bRoi[ROI_3] = m_prep_dlg->m_checkDispROI3Data.GetCheck();
        l_bRoi[NOISE_ROI_1] = m_prep_dlg->m_checkNoiseROI1Data.GetCheck();
        l_bRoi[NOISE_ROI_2] = m_prep_dlg->m_checkNoiseROI2Data.GetCheck();  
        l_bDispGrap[0] = m_prep_dlg->m_checkOrgData.GetCheck();
        l_bDispGrap[1] = m_prep_dlg->m_checkInvData.GetCheck();
        l_bDispGrap[2] = m_prep_dlg->m_checkFitData.GetCheck();
        l_bRevMin[0] = m_prep_dlg->m_checkRevDispROI1Data.GetCheck();
        l_bRevMin[1] = m_prep_dlg->m_checkRevDispROI2Data.GetCheck();
        l_bRevMin[2] = m_prep_dlg->m_checkRevDispROI3Data.GetCheck();
        long TotalXPoints = 0;
        
        if((l_CExtraPoint != NULL) && (TotalNewPtsforXaxis > 0) && (l_CExtraPoint->m_FailureFlag == 0)) {
            TotalXPoints = TotalNewPtsforXaxis;
        } else {
            TotalXPoints = l_Img_selected;
        }
        
        if(TotalXPoints <= 0) {
            TotalXPoints = l_Img_selected;
        }
        
        if(IsT1MapSelected == true) {
            BOOL l_bNoiseRoi[MAX_NOISE_ROI];
            l_bNoiseRoi[0] = m_prep_dlg->m_checkNoiseROI1Data.GetCheck();
            l_bNoiseRoi[1] = m_prep_dlg->m_checkNoiseROI2Data.GetCheck();
            //AN/Do it here
            int l_nIndx = 0;
            for(l_nIndx = 0; l_nIndx < TotalAnnularPoints; l_nIndx++) {
                if(-1 != l_Roi_T1_values[l_nIndx]) {
                    
                    l_nstatus = m_DisplayData->getMinMaxForYaxisMapMode(TotalXPoints, l_bRoi[0], l_bNoiseRoi, l_bDispGrap, l_bRevMin, l_Ymax, l_Ymin,l_nIndx,FALSE);
                    
                    if(E_IMPROPER_INDEX == l_nstatus){
                        continue; //go for next index                           
                    }else if(E_NO_ERROR == l_nstatus){
                        break;
                    }else{
                        CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): Failed to get Min Max value for Y axis"), __LINE__, __FILE__);
                        // no need to return because default min max(-10 to 10) value will be considered
                       
                    }                   
                }              
            }
            
            if(l_nIndx < TotalAnnularPoints){
                m_DisplayData->SetSelectedIndex(l_nIndx);
            }else{  // to be safe
                l_nIndx = l_nIndx - 1;
                m_DisplayData->SetSelectedIndex(l_nIndx);
            }
            CString l_cstemp(_T(""));
            int x = 0, y = 0; 
            m_T1Map->GetMouseXY(l_nIndx, x, y);
            m_prep_dlg->m_lbl_pix_pos.ShowWindow(SW_SHOW);
            m_prep_dlg->DisplayPixPos(x, y);
            
        } else {
            l_nstatus = m_DisplayData->getMinMaxForYaxis(TotalXPoints, l_bRoi, l_bDispGrap, l_bRevMin, l_Ymax, l_Ymin);
            if(E_NO_ERROR != l_nstatus) {   
                CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): Failed to get Min Max value for Y axis"), __LINE__, __FILE__);
                // no need to return because default min max(-10 to 10) value will be considered
            }
        }
        
        l_nstatus = m_DisplayData->getMinMaxForXaxis(TotalXPoints, l_Xmax, l_Xmin);
        
        if(E_NO_ERROR != l_nstatus) {   
            CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): Failed to get Min Max value for X axis"), __LINE__, __FILE__);
        } 

        l_nstatus = m_prep_dlg->m_tigraphctrl.SetGraphData(TotalXPoints,
            m_DisplayData->Time_stamp(),   //l_nTime_Stamp,//AS/modified/defect fix 
            m_DisplayData->getROIOrgData(),  //l_ROI_Org_Data,
            m_DisplayData->getROIIvnData(),  //l_Roi_Inv_Data,
            m_DisplayData->getROIFitData(),  //l_Roi_Fit_Data,
            m_DisplayData->getROIFitMinRevData(),    //l_Roi_Fit_Rev_Data,
            m_DisplayData->getROIMinRevData(),//l_Roi_Inv_Rev_Data,
            m_DisplayData->getROITiValues(),//l_Roi_Ti_Values,
            m_DisplayData->getROITiMinRevValues(),//l_Roi_Ti_Rev_Values,
            m_DisplayData->getNoiseROIOrgData(),//l_Noise_ROI_Org_Data,    
            l_Ymax, l_Ymin, l_Xmax, l_Xmin,
            TotalAnnularPoints,m_DisplayData->getROIT1Values(),
            m_DisplayData->getROIT1MinRevValues());        
        
        if(E_NO_ERROR != l_nstatus) {   
            CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): Failed to set data for graph"), __LINE__, __FILE__);
        }
        
        
        if(true == IsT1MapSelected) {   
            m_prep_dlg->displayMapModeData();
        } else {
            m_prep_dlg->displayData();
        }
        
        m_prep_dlg->displayNoiseData(); 
        m_prep_dlg->m_flag = TRUE; 
        //++++++++++++++++++++++++++++++++++++Study Wise FOlder for Map Files++++++++++++++++++++++++++++++++++++++++
        if(true == IsT1MapSelected) {
            if(E_NO_ERROR != CreateMapFiles(l_Roi_T1_values, l_Roi_T1_Min_Rev_Values)) {
                CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): CreateMapFiles Failed"), __LINE__);
            } else {
            }
        }
        
        if(NULL != l_Roi_Ti_Values) {
            delete [] l_Roi_Ti_Values;
            delete [] l_Roi_Ti_Rev_Values;
            delete [] l_Roi_Fit_Err_Values;
            delete [] l_Roi_Fit_Rev_Err_Values;
            delete [] l_Roi_T1_values;
            delete [] l_Roi_T1_Min_Rev_Values;
            l_Roi_Inv_Data = NULL;
            l_Roi_Inv_Rev_Data = NULL;
            l_Roi_Fit_Data = NULL;
            l_Roi_Fit_Rev_Data = NULL;
            l_Roi_Ti_Values  = NULL;
            l_Roi_Ti_Rev_Values = NULL;
            l_Roi_Fit_Err_Values = NULL;
            l_Roi_Fit_Rev_Err_Values = NULL;
            l_Roi_T1_values = NULL;
            l_Roi_T1_Min_Rev_Values = NULL;
        }
        
        if(true == IsT1MapSelected) {
            if(E_NO_ERROR != InvokeT1Map()) { 
                CTiPrepLogHelper::WriteToErrorLog(_T("OnApply(): InvokeT1Map Failed"), __LINE__, __FILE__);
            }
        }
        
        
        if(NULL != l_CExtraPoint) {
            delete l_CExtraPoint;
            l_CExtraPoint = NULL;
        }
        
        m_nisApplyDone = 1;
        m_prep_dlg->GUIOnIdle();
    } catch(CMemoryException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        m_prep_dlg->ClearGraphCtl();
        m_DisplayData->DeAllocateMemory();
        m_prep_dlg->GUIOnIdle();
        return E_ERROR;
    } catch(CFileException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        m_prep_dlg->ClearGraphCtl();
        m_DisplayData->DeAllocateMemory();
        m_prep_dlg->GUIOnIdle();
        return E_ERROR;
    } catch(CException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        m_prep_dlg->ClearGraphCtl();
        m_DisplayData->DeAllocateMemory();
        m_prep_dlg->GUIOnIdle();
        return E_ERROR;
    } catch(...) {
        CString l_cserrmsg = _T("Exception Occurred on click of Apply");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        m_prep_dlg->ClearGraphCtl();
        m_DisplayData->DeAllocateMemory();
        m_prep_dlg->GUIOnIdle();
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

void CTiPrepView::ClearMapFiles()
{
    CString l_csStudyFileName = _T("");
    m_DisplayData->GetStudyFileName(l_csStudyFileName);
    char destFldr[100] = {'\0'};
    strcpy(destFldr, "c:/MrMplus/Data/TiPrep");
    
    if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)/*ENOENT != _mkdir(destFldr)*/) {
        char temp[50];
        //set normal attributes for "TiPrep" folder destFldr
        CString l_csTiPrepFolder(_T(""));
        l_csTiPrepFolder.Format(_T("%S"),destFldr);
        if(FALSE == SetFileAttributes(l_csTiPrepFolder, FILE_ATTRIBUTE_NORMAL)) {
            CTiPrepLogHelper::WriteToErrorLog(_T("ClearFiles(): TiPrep SetFileAttributes Failed"), __LINE__);
        }
        
        if(l_csStudyFileName.IsEmpty() == FALSE) {
            wcstombs(temp, (LPCTSTR)l_csStudyFileName, 50);
            strcat(destFldr , "/");
            strcat(destFldr , temp);
            //set normal attributes for for "StudyFile" folder destFldr
            CString l_csStudyFileFolder(_T(""));
            l_csStudyFileFolder.Format(_T("%S"),destFldr);
            if(FALSE == SetFileAttributes(l_csStudyFileFolder, FILE_ATTRIBUTE_NORMAL)) {
                CTiPrepLogHelper::WriteToErrorLog(_T("ClearFiles(): StudyFile folder SetFileAttributes Failed"), __LINE__);
            }
        }
    }
    
    CString l_cpFileName(_T(""));
    l_cpFileName += destFldr;
    l_cpFileName += "/";
    l_cpFileName += "T1Map";
    
    if(FALSE == SetFileAttributes(l_cpFileName, FILE_ATTRIBUTE_NORMAL)) {
        CTiPrepLogHelper::WriteToErrorLog(_T("ClearMapFiles(): T1MAP_FILEPATH SetFileAttributes Failed"), __LINE__);
    }
    
    if(!(DeleteFile(l_cpFileName))) {     // failed to delete
        CString l_csErrMsg(_T(""));
        l_csErrMsg.Format(_T("Failed to delete file : %S.File may not be updated"), l_cpFileName);
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
    } else { // succeeded
        CString l_csErrMsg(_T(""));
        l_csErrMsg.Format(_T("File %s deleted successfully"), l_cpFileName);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csErrMsg);
    }
    
    l_cpFileName = _T("");
    l_cpFileName += destFldr;
    l_cpFileName += "/";
    l_cpFileName += "T1Back";
    
    if(FALSE == SetFileAttributes(l_cpFileName, FILE_ATTRIBUTE_NORMAL)) {
        CTiPrepLogHelper::WriteToErrorLog(_T("ClearFiles(): T1BACK_FILEPATH SetFileAttributes Failed"), __LINE__);
    }
    
    if(!(DeleteFile(l_cpFileName))) {     // failed to delete
        CString l_csErrMsg(_T(""));
        l_csErrMsg.Format(_T("Failed to delete file : %s.File may not be updated"), l_cpFileName);
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
    } else { // succeeded
        CString l_csErrMsg(_T(""));
        l_csErrMsg.Format(_T("File %s deleted successfully"), l_cpFileName);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csErrMsg);
    }
    
    l_cpFileName = _T("");
    l_cpFileName += destFldr;
    l_cpFileName += "/";
    l_cpFileName += "T1MinMap";
    
    if(FALSE == SetFileAttributes(l_cpFileName, FILE_ATTRIBUTE_NORMAL)) {
        CTiPrepLogHelper::WriteToErrorLog(_T("ClearFiles(): T1MINMAP_FILEPATH SetFileAttributes Failed"), __LINE__);
    }
    
    if(!(DeleteFile(l_cpFileName))) {     // failed to delete
        CString l_csErrMsg(_T(""));
        l_csErrMsg.Format(_T("Failed to delete file : %s.File may not be updated"), l_cpFileName);
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
    } else { // succeeded
        CString l_csErrMsg(_T(""));
        l_csErrMsg.Format(_T("File %s deleted successfully"), l_cpFileName);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csErrMsg);
    }
}     

/*************************************************************
* Method:       OnEquationOne
* Description:  This function is used
* Parameter:    int f_nEquFlag
* Returns:      int
*************************************************************/
int CTiPrepView::OnEquationOne(int f_nEquFlag)
{
    try{
        if(NULL == m_prep_dlg) { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnEquationOne(): m_prep_dlg Is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        int status = 0;
        if(m_prep_dlg->m_flag) {
            
            m_nEquFlag = f_nEquFlag;
            status= OnApply(f_nEquFlag);
            
            if(status != E_NO_ERROR) { 
                CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationOne(): OnApply Failed"), __LINE__, __FILE__);
                return status;
            }        
        } else {
            m_nEquFlag = f_nEquFlag;
            //Need to press Apply Manually
        }
        
        return E_NO_ERROR;
    }      catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
}

/*************************************************************
* Method:       OnEquationTwo
* Description:  This function is used
* Parameter:    int f_nEquFlag
* Returns:      int
*************************************************************/
int CTiPrepView::OnEquationTwo(int f_nEquFlag)
{
    try{
        CString l_temp_str = _T("");
        CString l_error = _T("");
        
        if(NULL == m_prep_dlg) {
            CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationTwo(): m_prep_dlg Is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_prep_dlg->m_RepTime.GetWindowText(l_temp_str);
        BOOL l_status = m_prep_dlg->ValidateOffset(l_temp_str); 
        
        if(!l_status) {
            m_prep_dlg->ClearGraphCtl();
            m_prep_dlg->m_ROI1T1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_ROI2T1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_ROI3T1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_ROIMeanT1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_RepTime.SetWindowText(_T("")); 
            
            return I_VALID_TR;
        }
        int status = 0;
        if(m_prep_dlg->m_flag) {
            
            m_nEquFlag = f_nEquFlag;
            status = OnApply(f_nEquFlag);
            if(E_NO_ERROR != status) { 
                CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationTwo(): OnApply Failed"), __LINE__, __FILE__);
                return status;
            }
            
            
        } else {
            m_nEquFlag = f_nEquFlag;
        }
        
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       OnEquationThree
* Description:  This function is used
* Parameter:    int f_nEquFlag
* Returns:      int
*************************************************************/
int CTiPrepView::OnEquationThree(int f_nEquFlag)
{
    try{
        CString l_temp_str = _T("");
        CString l_error = _T("");
        
        if(NULL == m_prep_dlg) { 
            CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationThree(): m_prep_dlg Is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_prep_dlg->m_RepTime.GetWindowText(l_temp_str);
        BOOL l_status = m_prep_dlg->ValidateOffset(l_temp_str); 
        
        if(!l_status) {
            m_prep_dlg->ClearGraphCtl();
            m_prep_dlg->m_ROI1T1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_ROI2T1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_ROI3T1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_ROIMeanT1Val.SetWindowText(l_temp_str);
            m_prep_dlg->m_RepTime.SetWindowText(_T("")); 
            
            return I_VALID_TR;
        }
        
        if(m_prep_dlg->m_flag) {
            
            m_nEquFlag = f_nEquFlag;
            int status = OnApply(f_nEquFlag);
            if(E_NO_ERROR != status ) { 
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("OnEquationThree(): OnApply Failed"), __LINE__, __FILE__);
                return status;
            }
            
            
        } else {
            m_nEquFlag = f_nEquFlag;
        }
        
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       OnEquationFour
* Description:  This function is used
* Parameter:    int f_nEquFlag
* Returns:      int
*************************************************************/
int CTiPrepView::OnEquationFour(int f_nEquFlag)
{
    try{
        if(NULL == m_prep_dlg) { 
            CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationFour(): m_prep_dlg Is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if(m_prep_dlg->m_flag) {
            
            m_nEquFlag = f_nEquFlag;
            int status = 0;
            status = OnApply(f_nEquFlag);
            if(E_NO_ERROR != status) { 
                CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationFour(): OnApply Failed"), __LINE__, __FILE__);
                return status;
            }
        } else {
            m_nEquFlag = f_nEquFlag;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       OnEquationFive
* Description:  This function is used
* Parameter:    int f_nEquFlag
* Returns:      int
*************************************************************/
int CTiPrepView::OnEquationFive(int f_nEquFlag)
{
    try{
        if(NULL == m_prep_dlg) { 
            CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationFive(): m_prep_dlg Is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if(m_prep_dlg->m_flag) {
            
            m_nEquFlag = f_nEquFlag;
            int status = 0;
            status = OnApply(f_nEquFlag);
            if(E_NO_ERROR != status) { 
                CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationFive(): OnApply Failed"), __LINE__, __FILE__);
                return status;
            }
        } else {
            m_nEquFlag = f_nEquFlag;
        }
        
        return E_NO_ERROR;
    }    catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       OnReselect
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::OnReselect()
{
    m_ExtraPtsINTDVD = 1;
    SetWaitCursor(true);
    
    try {
		vector<CString> loids;
	//	SetImageLOID(0,loids);
        if(SavePreviousROIstate() == FALSE) { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnReselect(): SavePreviousROIstate Failed"), __LINE__, __FILE__);
        }
        SetImageLOID(0,loids);

        m_nisApplyDone = 0;
        

        if(m_prep_dlg) {
            m_prep_dlg->ClearGraphCtl();
        }
        
        vector <CString> l_csImageLoid;
        l_csImageLoid.clear();
        
        if(NULL == m_DisplayData) {
            CTiPrepLogHelper::WriteToErrorLog(
                _T("OnReselect(): m_DisplayData is NULL"), __LINE__, __FILE__);
        } else {
            m_DisplayData->DeAllocateMemory();      
        }
        
        //         }
        if(NULL == m_prep_dlg) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnReselect(): m_prep_dlg is NULL"), __LINE__, __FILE__);
        } else {
            m_prep_dlg->m_flag = FALSE;
            m_prep_dlg->m_is_reselect_wait = true;
        }
        
        SetIselEvent(EVENT_RESELECT);
        BOOL l_bselImg = GetSelectedImages();
        
        if(FALSE == l_bselImg) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnReselect(): GetSelectedImages Failed"), __LINE__, __FILE__);
            (CTiPrepView::GetView())->ClearRepeatationTimeTextBox();
            return E_ERROR;
        }
        
    } catch(CMemoryException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        (CTiPrepView::GetView())->ClearRepeatationTimeTextBox();
        return E_ERROR;
    } catch(CFileException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        (CTiPrepView::GetView())->ClearRepeatationTimeTextBox();
        return E_ERROR;
    } catch(CException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        (CTiPrepView::GetView())->ClearRepeatationTimeTextBox();
        return E_ERROR;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occured on Reselct ");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        GetROIInstance()->DeleteROI();
        (CTiPrepView::GetView())->ClearRepeatationTimeTextBox();
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       OnExit
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::OnExit()
{
    CDPSComManager *l_server = NULL;
    CROIHandler*l_crhptr = NULL;
    
    try {
        
        
        if(NULL == m_DisplayData) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnExit(): m_DisplayData is NULL"), __LINE__, __FILE__);
        }else{        
            m_DisplayData->DeAllocateMemory();  
        }
        l_crhptr = GetROIInstance();
        if(NULL != l_crhptr){
            BOOL l_falg = l_crhptr->DeleteROI();
            if(!l_falg) {
                CString l_csErrMsg(_T("Unable to delete ROI."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            }
        }else{
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnExit(): GetROIInstance() is NULL"), __LINE__, __FILE__);
        }
        
        
        l_server = CDPSComManager::GetInstance();
        CString l_branch_boss(_T(""));
        if(l_server != NULL) {
            l_branch_boss = CDPSComManager::GetInstance()->GetBranchBoss();//CString(l_server->GetParentName()) + "." + _T("MPlusClinAppToolBox");        //AN scanner
            IMplusMessage *l_msg = NULL;
            CString l_target = _T("");
            l_target = l_server->GetParentName();
            l_target += ".";
            l_target += _T("Controller");
            OutputDebugString(_T("Parent name :"));
            OutputDebugString(l_target);
            l_server->Connect(l_target);
            l_msg = l_server->CreateMessage(FW_UnRegisterMsg, 1, _bstr_t(l_target));
            
            if(l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            
            l_server->Disconnect();
        } else {
            CTiPrepLogHelper::WriteToErrorLog(_T("OnExit(): l_server is NULL"), __LINE__, __FILE__);
        }
        if(NULL!= l_crhptr){
            BOOL l_resultROI = FALSE;
            l_resultROI = l_crhptr->AutoSaveDB(TRUE);
            if(!l_resultROI) {
                CString l_csErrMsg(_T("Unable to save data in database."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            }
        }
        
        
        SetStateProcessTools(_T("true"));
        SetStateROITools(_T("true"));
		CTiPrepController::GetInstance()->SendTiPrepCloseToPQM(); //AN scanner
        if(l_server != NULL) {
            IMplusMessage *l_msg = NULL;
            l_msg = NULL;
            l_server->Connect(l_branch_boss);
            l_msg = l_server->CreateMessage(FW_RequestClose, 1, l_server->GetParentName());
            
            if(l_msg != NULL) {
                l_server->SendMsg(l_msg);
                DEL_MSG(l_msg);
                l_msg = NULL;
            }
            
            l_server->Disconnect();
        }
        
        SendMsg("EXIT");
        CSocketMgr::DeleteInstance(); 
        
    } catch(CMemoryException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        if(l_crhptr) {l_crhptr->DeleteROI();}
        return E_ERROR;
    } catch(CFileException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        if(l_crhptr) {l_crhptr->DeleteROI();}
        return E_ERROR;
    } catch(CException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        if(l_crhptr) {l_crhptr->DeleteROI();}
        return E_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred on click of Exit"), __LINE__, __FILE__);
        if(l_crhptr) {l_crhptr->DeleteROI();}
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	SetImageLOID
* Description: 	This function is used 
* Parameter: 	long f_no_of_images_selected :
* Parameter: 	vector <CString> & f_image_loid :
* Returns:   	int
*************************************************************/
int CTiPrepView::SetImageLOID(long f_no_of_images_selected,
                              /*In*/vector <CString> & f_image_loid)
{
    try{
        if(NULL == m_DisplayData) {
            CTiPrepLogHelper::WriteToErrorLog(_T("SetImageLOID(): m_DisplayData is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        int l_nStatus = m_DisplayData->setImgInfo(f_no_of_images_selected, f_image_loid);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog(_T("SetImageLOID(): setImgInfo() returned error"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        m_DisplayData->setTimeStampData(f_no_of_images_selected);
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       GetSelectedSeriesImages
* Description:  This function is used
* Parameter:    CString & series_loid
* Returns:      int
*************************************************************/
int CTiPrepView::GetSelectedSeriesImages(CString &series_loid)
{
    TIPREP_TRACE(CTiPrepView::GetSelectedSeriesImages,MRTraceMarker3)
        
        try {
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetSelectedSeriesImages() : CDPSComManager is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target.Empty();
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("iselector");
        l_server->Connect(l_target);
        l_msg = l_server->CreateMessage
            (GL_IS_Get_Select_Series_IC_Name, 1, _bstr_t(l_target), _bstr_t(series_loid));
        
        if(l_msg != NULL) {
            l_server->SendMsg(l_msg);
            DEL_MSG(l_msg);
            l_msg = NULL;
        } else {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetSelectedSeriesImages() : l_msg is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_server->Disconnect();
        m_is_wait_request_isel = true;
    } catch(...) {
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}
/*************************************************************
* Method:       RefreshIsel
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepView::RefreshIsel()
{
    TIPREP_TRACE(CTiPrepView::RefreshIsel,MRTraceMarker3)
        
        try {
        CDPSComManager *l_server = NULL;
        l_server = CDPSComManager::GetInstance();
        
        if(NULL == l_server) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RefreshIsel() : CDPSComManager is NULL"), __LINE__, __FILE__);
            return ;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target.Empty();
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("iselector");
        l_server->Connect(l_target);
        l_msg = l_server->CreateMessage(GL_IS_Refresh_Image_Matrix, 1, _bstr_t(l_target));
        
        if(l_msg != NULL) {
            l_server->SendMsg(l_msg);
            DEL_MSG(l_msg);
            l_msg = NULL;
        } else {//KJ-JULY
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RefreshIsel() : l_msg is NULL"), __LINE__, __FILE__);
            return ;
        }
        
        l_server->Disconnect();
        m_is_wait_request_isel = true;
    } catch(...) {
        return ;
    }
    
    return ;
}

/*************************************************************
* Method:       onReconDone
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::onReconDone()
{
    return 0 ;
}

/*************************************************************
* Method:       getSelectedEqn
* Description:  This function is used
* Parameter:    int & f_nEqnFlag
* Returns:      int
*************************************************************/
int CTiPrepView::getSelectedEqn(int &f_nEqnFlag)
{
    if(m_prep_dlg->m_radioEqu1.GetCheck()) {
        f_nEqnFlag = 0;
    } else if(m_prep_dlg->m_radioEqu2.GetCheck()) {
        f_nEqnFlag = 1;
    } else if(m_prep_dlg->m_radioEqu3.GetCheck()) {
        f_nEqnFlag = 2;
    } else if(m_prep_dlg->m_radioEqu4.GetCheck()) {
        f_nEqnFlag = 3;
    } else if(m_prep_dlg->m_radioEqu5.GetCheck()) {
        f_nEqnFlag = 4;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       SetT1MapFlag
* Description:  This function is used
* Parameter:    bool flg
* Returns:      void
*************************************************************/
void CTiPrepView::SetT1MapFlag(bool flg)
{
    IsT1MapSelected = flg;
}
/*************************************************************
* Method:       GetT1MapFlag
* Description:  This function is used
* Returns:      bool
*************************************************************/
bool CTiPrepView::GetT1MapFlag()
{
    return(IsT1MapSelected);
}
/*************************************************************
* Method:       SavePreviousROIstate
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::SavePreviousROIstate()
{
    try{
        vector<CString> l_csImgInfo;
        
        if(NULL == m_DisplayData) { 
            CTiPrepLogHelper::WriteToErrorLog(_T("SavePreviousROIstate(): m_DisplayData found NULL"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        int l_nStatus = m_DisplayData->getSelImgLOID(l_csImgInfo);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog(_T("SavePreviousROIstate(): getSelImgLOID returned error"), __LINE__, __FILE__);
			l_csImgInfo.clear();
            return(E_ERROR);
        }
        
        BOOL l_resultROI = TRUE;
        CLocalDBMgr localdb;
        CString l_csProtocolName = _T(""), l_csProcessName = _T("");
        CString f_csStudyName;
        CString f_csCompletepath;//sb
        int status = 0;
        if(0 < l_csImgInfo.size()) {
            status = localdb.GetProtocolNameFromImageLevelDB(l_csImgInfo.at(0), l_csProtocolName,f_csStudyName,f_csCompletepath); 
            if_no_error(status) {
                if(IsT1MapSelected == false) {
					CString l_csmode(_T("AVG"));
                    l_resultROI = GetROIInstance()->SaveUpdateROIParamsInFile(l_csProtocolName, l_csmode);
                } else if(IsT1MapSelected == true) {
					CString l_csmode(_T("MAP"));
                    l_resultROI = GetROIInstance()->SaveUpdateROIParamsInFile(l_csProtocolName, l_csmode);
                }
            }
        }
        l_csImgInfo.clear();
        return(l_resultROI);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       SendMsg
* Description:  This function is used
* Parameter:    char * f_msg
* Returns:      int
*************************************************************/
int CTiPrepView::SendMsg(char * f_msg)
{
    try{
        if(NULL == m_socketComm) {
            //log error
            CString l_cserr(_T("socket mgr is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_socketComm->Send(f_msg);
        
        if(strcmp("EXIT", f_msg) == 0) {
            child_app = NULL;
        }
        
        return 0;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       OnReceive
* Description:  This function is used
* Parameter:    char * f_msg
* Parameter:    unsigned int f_nlength
* Returns:      int
*************************************************************/
int CTiPrepView::OnReceive(char * f_msg, unsigned int f_nlength)
{
    try{
        char * msg_type = NULL;
        int x = 0, y = 0;
        
        if(f_msg) {
            msg_type = strtok(f_msg, ":");
            
            if(!msg_type) {
                return 1;
            }
            
            if(strncmp(msg_type, "PIX_POS", strlen("PIX_POS")) == 0) {
                char * val = NULL;
                val = strtok(NULL, ":");
                
                if(val) {
                    sscanf(val, "%d,%d", &x, &y);
                    m_prep_dlg->DisplayPixPos(x, y);
                    int l_nSelectedIndex = 0;
                    
                    if(NULL == m_T1Map) {
                        CTiPrepLogHelper::WriteToErrorLog
                            (_T("OnReceive(): m_T1Map found NULL error"), __LINE__, __FILE__);
                        return E_ERROR;
                    }
                    
                    int l_nStatus = m_T1Map->FindSelectedIndex(x, y, &l_nSelectedIndex);
                    
                    if(E_NO_ERROR != l_nStatus) {
                        CTiPrepLogHelper::WriteToErrorLog
                            (_T("OnReceive(): FindSelectedIndex returned error"), __LINE__, __FILE__);
                        return(l_nStatus);
                    }
                    
                    m_DisplayData->SetSelectedIndex(l_nSelectedIndex);
                    RefreshGraphAndDataOnGui();
                } else {
                }
            } else if(strcmp(msg_type, "EXIT") == 0) {
                child_app = NULL;
            } else if (strcmp(msg_type,"SETWND") == 0) { 
                if (m_pexec) 
                {
                    m_pexec->FindChildHWND(child_app);
                    if (child_app != NULL) {
                        AddZOrderLayer();
                        RegisterWindowZOrder(child_app);
                    }
                } 
            } else if(strcmp(msg_type, "") == 0) { //whatver message
            }
        }
        
        //do processing here
        return 0;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       InitSocket
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::InitSocket()
{
    try{
        if(!m_socketComm) {
            m_socketComm = CSocketMgr::GetInstance();    //new CSocketMgr();
        }
        
        if(!m_socketComm) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("InitSocket(): Memoery allocation error"), __LINE__, __FILE__);
            return 1;
        }
        
        m_socketComm->CreateSocket();
        return 0;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return 1;
    }
}
/*************************************************************
* Method:       InvokeT1Map
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepView::InvokeT1Map()
{
    try{
        TIPREP_TRACE(CPDController::InvokeT1Map,MRTraceMarker3);
        
        if(NULL == m_pexec) {
            m_pexec = new CProcessExec();
        }
        
        if(NULL == m_pexec) {
            CString l_cserrmsg(_T("InvokeT1Map Memory allocation error"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        if(NULL == m_prep_dlg){
            CString l_cserrmsg = _T("m_prep_dlg instance is NULL");
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        //const char * l_csExe_name = T1MAP_EXE_PATH;
        const char * l_csProc_name = T1MAP_EXE_NAME;
        bool l_flag = false;
        int l_nstatus = 0;
        l_nstatus = m_pexec->GetProcessList(l_csProc_name, l_flag);
        
        if(E_NO_ERROR != l_nstatus) {
            CString l_csErrMsg(_T("InvokeT1Map(): Error in getting process list ."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return(l_nstatus);
        }
        
        if(true == l_flag) {
            //process already running
            
            //SendMsg("UPDATE");//AS/commented
            if(m_prep_dlg->m_checkRevDispROI1Data.GetCheck()) {//AS/added
                CTiPrepView::GetView()->SendMsg("MINUPDATE");
            } else {
                CTiPrepView::GetView()->SendMsg("UPDATE");
            }
            return E_NO_ERROR;
        } else {
            if(m_T1Map == NULL) {
                CString l_cserrmsg = _T("T1Map instance is NULL");
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                return(E_ERROR);
            }
            
            int l_nheight = m_T1Map->GetHeightForT1Map();
            int l_nwidth = m_T1Map->GetWidhtForT1Map();
            //AS/added for folder handling/start
            CString l_csStudyFileName(_T(""));
            
            if(NULL != m_DisplayData) {
                m_DisplayData->GetStudyFileName(l_csStudyFileName);
                
                if(l_csStudyFileName.IsEmpty() == TRUE) {
                    CString l_cserrmsg = _T("StudyFileName found empty");
                    CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                    return(E_ERROR);
                }
            } else {
                CString l_cserrmsg = _T("m_DisplayData instance is NULL");
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                return(E_ERROR);
            }
            
            char MapName[20] = {'\0'};
            char BackName[20] = {'\0'};
            char MinName[20] = {'\0'};
            strcpy(MapName, "T1Map");
            strcpy(BackName, "T1Back");
            strcpy(MinName, "T1MinMap");
            CString l_csProcessNm(_T(""));
            l_csProcessNm = _T(T1MAP_EXE_PATH_PRODUCT);
            CString l_csPortNumber(_T(""));
            CString l_csReverseMinFlag(_T(""));
            l_csPortNumber.Format(_T("%d"),m_socketComm->GetPort());   
            
            if(BST_CHECKED == m_prep_dlg->m_checkRevDispROI1Data.GetCheck()){
                l_csReverseMinFlag.Format(_T("ON"));
            }else{
                l_csReverseMinFlag.Format(_T("OFF"));
            }
            
            CString l_csParams(_T(""));
            l_csParams.Format(_T("-S%s -B%S -M%S -N%S -H%d -W%d -R%s -P%s"), l_csStudyFileName, BackName, MapName, MinName, l_nheight, l_nwidth,l_csReverseMinFlag,l_csPortNumber); //AS/modified for folder handling
            child_app = NULL;
            l_nstatus = m_pexec->CreateProc(l_csProcessNm, l_csParams, child_app);//exec the exe from bin
            if(E_NO_ERROR != l_nstatus) {
                l_csProcessNm.Empty();
                l_csProcessNm = _T(T1MAP_EXE_PATH);
                int l_nstatus2 = 0;
                child_app = NULL;
                l_nstatus2 = m_pexec->CreateProc(l_csProcessNm, l_csParams, child_app);
                    if(E_NO_ERROR != l_nstatus2) {
                    CString l_cserrmsg = _T("Create Process failed for application :");
                    l_cserrmsg += l_csProcessNm;
                    CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                    return(l_nstatus);
                }
            }
            
            // ::SetWindowPos(child_app, m_prep_dlg->GetSafeHwnd(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS); //AN not required
            //         AddZOrderLayer();
            //         RegisterWindowZOrder(child_app);
            
            //         if(child_app && m_socketComm) { //Need to ensure always the handle is valid only till the window is open
            //  ::SendMessage(child_app, WM_PORT_NO, (WPARAM)m_socketComm->GetPort(), NULL);
            //         }
            
            //         if(m_prep_dlg && m_prep_dlg->m_checkRevDispROI1Data.GetCheck()) {
            //         }
        }
        
        //if successful creation
        return 0;
}catch(...) {
    CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
    return 1;
}
}

//AN/End

/*************************************************************
* Method:       RoiCopyHandler
* Description:  This function is used
* Parameter:    vector<CString> & f_csSeriesLoidVector
* Parameter:    vector<long> & f_nSeriesNumberVector
* Parameter:    int & f_nTotalScannedProt
* Returns:      int
*************************************************************/
int CTiPrepView::RoiCopyHandler(vector<CString> & f_csSeriesLoidVector,
                                vector<long> & f_nSeriesNumberVector,
                                vector<long> & f_nProtocolNumberVector,
                                 CString & f_source_series,
                                int & f_nTotalProtocolCount,
                                int & f_nTotalSeriesCount)
{
    try{
        vector<CString> l_csImgInfo;
        CString l_csSeriesLOID = _T("");
        CString l_csStudyLOID = _T("");
        vector<CString> l_csAcqVec;
        int l_nstatus = E_NO_ERROR;
        LPCTSTR *l_SeriesLOIDArray = NULL;
        long l_nLastProtocolNumber = 0;
        bool protocolFound = true;
        
        if(NULL == m_DisplayData) {
            CString l_csErrMsg(_T("Data Unavailable!"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        if(NULL != m_DisplayData) {
            l_nstatus = m_DisplayData->getSelImgLOID(l_csImgInfo);
            
            if(E_NO_ERROR != l_nstatus) { //AS/added
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("RoiCopyHandler(): getSelImgLOID returned error"), __LINE__, __FILE__);
                return(E_ERROR);
            }
        }
        
        CLocalDBMgr localdb;
        localdb.GetParentLOID(l_csImgInfo.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
        localdb.GetParentLOID(l_csSeriesLOID, LOID_TYPE_SERIES, l_csStudyLOID);
        //AN source protocol
        long current_Series = 0;
        int status = 0;
        status = localdb.GetSeriesNumberFromDB(l_csSeriesLOID,&current_Series);
        if (status != E_NO_ERROR)
        {
            return status;
        }
        int proc_num = 0;
        proc_num = current_Series%1000;
        current_Series -= proc_num;
        f_source_series.Format(_T("%d"),(current_Series /*- proc_num*/));
        
        l_nstatus = localdb.getProtAcqOrderFromDB
            (l_csStudyLOID, &f_nTotalSeriesCount, l_csAcqVec, &l_SeriesLOIDArray);
        
        if(E_NO_ERROR != l_nstatus || l_SeriesLOIDArray == NULL ) {
            CString l_csErrMsg(_T("Getting AcqOrder Failed!"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            if(NULL != l_SeriesLOIDArray) {
                delete [] l_SeriesLOIDArray;
                l_SeriesLOIDArray = NULL;
            }
            return E_ERROR;
        }
        
        for(int l_ni = 0; l_ni < f_nTotalSeriesCount; l_ni++) {
            CString l_csSeriesLD(l_SeriesLOIDArray[l_ni]);
            f_csSeriesLoidVector.push_back(l_csSeriesLD);
            long l_nSeriesLOID = 0;
            l_nSeriesLOID = _ttol(l_csSeriesLD);
            long l_nProtocolNumber = 0;
            l_nstatus = localdb.GetSeriesNumberFromDB(l_csSeriesLD, &l_nProtocolNumber); 
            
            if(E_NO_ERROR != l_nstatus) {
                if(NULL != l_SeriesLOIDArray) {
                    delete [] l_SeriesLOIDArray;
                    l_SeriesLOIDArray = NULL;
                }
                
                CString l_csErrMsg(_T("getProtocolNumberFromDB Failed!"));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return E_ERROR;
            }
            
            f_nSeriesNumberVector.push_back(l_nProtocolNumber);
            
            if((abs(l_nProtocolNumber - l_nLastProtocolNumber) < 1000) || //AN Modified
                abs( l_nProtocolNumber - current_Series) < 1000){ 
                protocolFound = false;
            } else {
                protocolFound = true;
                f_nTotalProtocolCount++;
                f_nProtocolNumberVector.push_back(l_nProtocolNumber);
                l_nLastProtocolNumber = l_nProtocolNumber;
            }
        }
        
        if(NULL != l_SeriesLOIDArray) {
            delete [] l_SeriesLOIDArray;
            l_SeriesLOIDArray = NULL;
        }
        
        l_csImgInfo.clear();
        l_csAcqVec.clear();
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       TiCopyHandler
* Description:  This function is used
* Parameter:    vector<CString> & f_csSeriesLoidVector
* Parameter:    vector<long> & f_nSeriesNumberVector
* Parameter:    int & f_nTotalScannedProt
* Returns:      int
*************************************************************/
int CTiPrepView::TiCopyHandler( 
                               std::vector<int> & f_nTiValueVector,
                               std::vector<CString> & f_csProtSequenceVector,
                                std::vector<int> & f_nProtIndexVector,
                                std::vector<long> & f_nProtocolNumberVector,
                                int & f_nTotalProtocolCount,
                                int & f_nTotalSeriesCount,
                                CString &f_csStudyPath)
{
    LPCTSTR *l_SeriesLOIDArray = NULL;

    try{
        std::vector<CString> l_csImgInfo;
        CString l_csSeriesLOID = _T("");
        CString l_csStudyLOID = _T("");
        std::vector<CString> l_csAcqVec;
        int l_nstatus = E_NO_ERROR;
        long l_nLastProtocolNumber = 0;
        bool protocolFound = true;
        
        if(NULL == m_DisplayData) {
            CString l_csErrMsg(_T("Data Unavailable!"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        if(NULL != m_DisplayData) {
            l_nstatus = m_DisplayData->getSelImgLOID(l_csImgInfo);
            
            if(E_NO_ERROR != l_nstatus) { //AS/added
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("TiCopyHandler(): getSelImgLOID returned error"), __LINE__, __FILE__);
                return(E_ERROR);
            }
        }
        
        CLocalDBMgr localdb;
        localdb.GetParentLOID(l_csImgInfo.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
        localdb.GetParentLOID(l_csSeriesLOID, LOID_TYPE_SERIES, l_csStudyLOID);
        localdb.getStudyPathFromDB(l_csStudyLOID,f_csStudyPath);
        
        l_nstatus = localdb.getProtAcqOrderFromDB
            (l_csStudyLOID, &f_nTotalSeriesCount, l_csAcqVec, &l_SeriesLOIDArray);
        
        if(E_NO_ERROR != l_nstatus || l_SeriesLOIDArray == NULL ) {
            CString l_csErrMsg(_T("Getting AcqOrder Failed!"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            if(NULL != l_SeriesLOIDArray) {
                delete [] l_SeriesLOIDArray;
                l_SeriesLOIDArray = NULL;
            }
            return E_ERROR;
        }
        
        int l_nAlreadyScannedProtNum = 0;
        //----------------------------------------------
        for(int l_ni = 0; l_ni < f_nTotalSeriesCount; l_ni++) {
            CString l_csSeriesLD(l_SeriesLOIDArray[l_ni]);
            long l_AcqStatus = MRC_ACQ_STATUS_WAIT;//MRC_ACQ_STATUS_WAIT 0 MRC_ACQ_STATUS_SUCCESS 1 MRC_ACQ_STATUS_FAIL 2 MRC_ACQ_STATUS_CURRENT 3
            
            localdb.GetAcqStatusFromDB(l_csSeriesLD,&l_AcqStatus);
            if(l_AcqStatus == MRC_ACQ_STATUS_SUCCESS || 
                l_AcqStatus == MRC_ACQ_STATUS_FAIL ||
                l_AcqStatus == MRC_ACQ_STATUS_CURRENT){
                l_nAlreadyScannedProtNum++;
            }
        }
        
        int l_nIncrementNum = 0;

        //------------------------------------
        for(l_ni = 0; l_ni < f_nTotalSeriesCount; l_ni++) {
            CString l_csSeriesLD(l_SeriesLOIDArray[l_ni]);
            long l_AcqStatus = MRC_ACQ_STATUS_SUCCESS;//MRC_ACQ_STATUS_WAIT 0 MRC_ACQ_STATUS_SUCCESS 1 MRC_ACQ_STATUS_FAIL 2 MRC_ACQ_STATUS_CURRENT 3
            localdb.GetAcqStatusFromDB(l_csSeriesLD,&l_AcqStatus);

            if(l_AcqStatus == MRC_ACQ_STATUS_WAIT){
                int  l_nProtocolIndex = 0;
                l_nstatus = localdb.GetProtocolIndexFromSeriesLevelDB(l_csSeriesLD, l_nProtocolIndex);
                CString l_csSequence(_T(""));
                localdb.GetProtocolSequenceFromSeriesLevelDB(l_csSeriesLD,l_csSequence);
                if(E_NO_ERROR != l_nstatus) {                    
                    CString l_csErrMsg(_T("GetProtocolIndexFromSeriesLevelDB Failed!"));
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    continue;
                }
                l_nIncrementNum++;
                long l_nProtocolNumber  = (l_nAlreadyScannedProtNum + l_nIncrementNum)*1000;
                
                f_nProtIndexVector.push_back(l_nProtocolIndex);                 
                f_nTotalProtocolCount++;
                f_nProtocolNumberVector.push_back(l_nProtocolNumber);
                f_csProtSequenceVector.push_back(l_csSequence);
                int l_nTi = 0;
                getVfcTI(f_csStudyPath,l_nProtocolIndex,&l_nTi);//dont return on error
                f_nTiValueVector.push_back(l_nTi);
            }
        }
        
        if(NULL != l_SeriesLOIDArray) {
            delete [] l_SeriesLOIDArray;
            l_SeriesLOIDArray = NULL;
        }
        
        l_csImgInfo.clear();
        l_csAcqVec.clear();
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        if(NULL != l_SeriesLOIDArray) {
            delete [] l_SeriesLOIDArray;
            l_SeriesLOIDArray = NULL;
        }
        return E_ERROR;
    }
}
//************************************
// Method:    GetNoiseOffsetVal
// FullName:  CTiPrepView::GetNoiseOffsetVal
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: double & f_nNoiseOffsetVal
//************************************
int CTiPrepView::GetNoiseOffsetVal(double &f_nNoiseOffsetVal)
{
    try{
        if(NULL == m_DisplayData) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetNoiseOffsetVal(): m_DisplayData is NULL"), __LINE__, __FILE__); 
            return E_ERROR;
        }
        
        f_nNoiseOffsetVal = m_DisplayData->getNoiseOffsetVal();
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CTiPrepView::GetSelSeriesName(int f_nCheckStatus)
{
    try{
        vector<CString> l_csImgInfo;
        CString l_csProtocolName = _T("");
        
        if(NULL == m_DisplayData) { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetSelSeriesName() :m_DisplayData is NULL."), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        int l_nstatus = m_DisplayData->getSelImgLOID(l_csImgInfo);
        
        if(E_NO_ERROR != l_nstatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetSelSeriesName(): getSelImgLOID returned error"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        CLocalDBMgr localdb;
        CString f_csStudyName;
        unsigned short l_image_width = 0;
        unsigned short l_image_height = 0;
        int status = 0;
        if(0 < l_csImgInfo.size()) {
            CString l_csTempStr(_T(""));
            CString f_csCompletepath;
            status = localdb.GetProtocolNameFromImageLevelDB(l_csImgInfo.at(0), l_csProtocolName,f_csStudyName,f_csCompletepath);//sb
            if (status != E_NO_ERROR)
            {
                CTiPrepLogHelper::WriteToErrorLog(_T("GetSelSeriesName(): GetProtocolNameFromImageLevelDB() return fail status"),__LINE__,__FILE__);
              
            }
           /* localdb.TagReadingForHeightWidthImage(l_csImgInfo.at(0),                                      
                &l_image_width,
                &l_image_height);
            */ //KJ not used currently
            BOOL l_bresultROI = FALSE;
			CString mode (_T(""));

            if(f_nCheckStatus == BST_CHECKED) {
				mode = _T("MAP");
                l_bresultROI = GetROIInstance()->GetsaveROIParams
                    (l_csProtocolName, _T("MAP"),l_image_height,l_image_width); 
            } else if(f_nCheckStatus == BST_UNCHECKED) {
				mode = _T("AVG");
                l_bresultROI = GetROIInstance()->GetsaveROIParams
                    (l_csProtocolName, _T("AVG"),l_image_height,l_image_width);
            }
            
            if(FALSE == l_bresultROI){
                CTiPrepLogHelper::WriteToErrorLog(_T("Exception occured while reading ROI parameters"));
            }

            GetROIInstance()->SetProtocolNameForSetROIParams(l_csProtocolName);
		
            //l_resultROI = GetROIInstance()->SetRoiPosition();
			GetROIInstance()->RedrawROI(false,l_csProtocolName,mode);
        }//	else {//Anulekha
		//	GetROIInstance()->DeleteROI();
       // }	
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GetDisplayVals
* Description: 	This function Retrieves values required for display on screen
* Parameter: 	double * * f_pfTiVals :
* Parameter: 	double * * f_pfErrVals :
* Parameter: 	double * * f_pfT1Vals :
* Parameter: 	double * * f_pfTiRevVals :
* Parameter: 	double * * f_pfErrRevVals :
* Parameter: 	double * * f_pfT1RevVals :
* Returns:   	int
*************************************************************/
int CTiPrepView::GetDisplayVals(double ** f_pfTiVals, double ** f_pfErrVals,
                                double ** f_pfT1Vals, double ** f_pfTiRevVals,
                                double ** f_pfErrRevVals, double ** f_pfT1RevVals)
{
    try{
        if(NULL == m_DisplayData) { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetImgdata() :m_DisplayDatais NULL."), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        *f_pfTiVals = m_DisplayData->getROITiValues();
        *f_pfErrVals = m_DisplayData->getROIFitErr();
        *f_pfT1Vals = m_DisplayData->getROIT1Values();
        *f_pfTiRevVals = m_DisplayData->getROITiMinRevValues();
        *f_pfErrRevVals = m_DisplayData->getROIFitMinRevErr();
        *f_pfT1RevVals = m_DisplayData->getROIT1MinRevValues();
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GetImgdata
* Description: 	This function is used 
* Parameter: 	double * * & f_pOrgData :
* Parameter: 	double * * & f_pInvData :
* Parameter: 	double * * & f_pFitData :
* Parameter: 	long & f_nTotalImgCnt :
* Returns:   	int
*************************************************************/
int CTiPrepView::GetImgdata(double **& f_pOrgData , double **& f_pInvData,
                            double **& f_pFitData, long & f_nTotalImgCnt)
{
    try{
        if(m_DisplayData == NULL) { //KJ-JULY
            f_pOrgData = NULL;
            f_pInvData = NULL;
            f_pFitData = NULL;
            f_nTotalImgCnt = 0;
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetImgdata() :m_DisplayDatais NULL."), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        f_pOrgData = m_DisplayData->getROIOrgData();
        f_pInvData = m_DisplayData->getROIIvnData();
        f_pFitData = m_DisplayData->getROIFitData();
        m_DisplayData->getNumImagesSelectedOnIsel(&f_nTotalImgCnt); 
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	vfcDataGetGDCKind
* Description: 	This function is used 
* Parameter: 	CString f_csImgLoid :
* Returns:   	int
*************************************************************/
int CTiPrepView::vfcDataGetGDCKind(CString f_csImgLoid)
{
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	vfcDataGetDataForm
* Description: 	This function is used 
* Parameter: 	CString & f_csDataType :
* Parameter: 	vector<CString> & f_img_loid :
* Returns:   	int
*************************************************************/
int CTiPrepView::vfcDataGetDataForm(CString &f_csDataType, vector<CString> &f_img_loid)
{
    try{
        CString l_protocol_name, l_intermediate_string, l_study_file_name(_T("")), l_csProcessName;
        int l_process_no = 0;
        int l_protocol_no = 0;
        int l_nDataType = 0;
        int l_tag_reading_status = 0;
        CvfcDBMgr l_vfcDb;
        //vector<CString> l_img_loid;
        
        if(f_img_loid.size() <= 0) { 
            CTiPrepLogHelper::WriteToErrorLog(_T("vfcDataGetDataForm() image loids is empty"), __LINE__, __FILE__);
            return I_PROPER_IMG;
        }

        CLocalDBMgr l_localdb ;
        CString l_csSeriesLOID(_T(""));
        int l_nStatus = l_localdb.GetParentLOID(f_img_loid.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Unable to get file path from LocalDB."), ERROR_LOGONLY);
            return(E_DB_ERROR);
        }
        
        l_nStatus = l_localdb.GetProtocolNameFromSeriesLevelDB(l_csSeriesLOID, l_protocol_name, l_csProcessName);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Unable to get protocol name from LOcalDB."), ERROR_LOGONLY);
            return(E_DB_ERROR);
        }
        
        l_intermediate_string.Format(_T("%s"), l_csProcessName);
        int l_count_special_char = l_intermediate_string.Find(_T(':'));
        l_study_file_name = l_intermediate_string.Left(l_count_special_char);
        int l_total_length = l_csProcessName.GetLength();
        l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
        l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
        l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
        l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        char l_intermediate_char[10];
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        l_protocol_no = atoi(l_intermediate_char);
        l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
        l_total_length = l_csProcessName.GetLength();
        l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
        l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
        l_intermediate_string = l_intermediate_string.Right(l_count_special_char);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        l_process_no = atoi(l_intermediate_char);

        VfDBHandle_t l_vfd;
        int l_vfc_file_open_status = l_vfcDb.OpenDBFile(l_study_file_name,l_vfd);
        
        if(E_NO_ERROR != l_vfc_file_open_status) {
//             CTiPrepLogHelper::WriteToErrorLog
//                 (_T("Failed to Open vfc File."),__LINE__, __FILE__);
            return(E_VF_OPEN);
        }
        
        l_tag_reading_status = l_vfcDb.getImageDataForm
            (l_protocol_no, l_process_no, l_nDataType, f_csDataType,l_vfd);
        
        if(E_NO_ERROR != l_tag_reading_status) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Unable to get TR value"),__LINE__, __FILE__);
            l_vfcDb.CloseDBFile(l_vfd);
            return E_Vf_GET_ERR;
        }
        
        if(NULL == m_prep_dlg) {
            l_vfcDb.CloseDBFile(l_vfd);
            CTiPrepLogHelper::WriteToErrorLog(_T("TiPrep Dlg pointer NULL "), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_nStatus = l_vfcDb.CloseDBFile(l_vfd);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Unable to close study file in VFC"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:    	vfcDataGetDataType
* Description: 	This function is used 
* Parameter: 	CString f_csImgLoid :
* Returns:   	int
*************************************************************/
int CTiPrepView::vfcDataGetDataType(CString f_csImgLoid)
{
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	HideROI
* Description: 	This function is used 
* Parameter: 	int f_nROINo :
* Parameter: 	int f_nChecked :
* Returns:   	BOOL
*************************************************************/
BOOL CTiPrepView::HideROI(int f_nROINo, int f_nChecked)
{
    BOOL l_bHideflag = GetROIInstance()->HideROI(f_nROINo, f_nChecked);
    return l_bHideflag;
}

/*************************************************************
* Method:    	GetLocalDBImgData
* Description: 	This function is used 
* Parameter: 	CString f_csImageLoid :
* Parameter: 	void * * f_pImgData :
* Parameter: 	int * f_pnsize :
* Parameter: 	float * f_pfScale :
* Parameter: 	int * f_pnTime_stamp :
* Parameter: 	unsigned short * f_nlData_Type :
* Returns:   	int
*************************************************************/
/*int CTiPrepView::GetLocalDBImgData(CString f_csImageLoid, void **f_pImgData,
                                   int *f_pnsize, float *f_pfScale,
                                   int *f_pnTime_stamp, unsigned short * f_nlData_Type,int &f_nImgNo)
{
    try{
        int status = E_NO_ERROR;
        CLocalDBMgr localdb;
        status = localdb.getImgDataFromDB(f_csImageLoid, f_pImgData,
            f_pnsize, f_pfScale,
            f_pnTime_stamp, f_nlData_Type,f_nImgNo);
        return status;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}
*/
/*************************************************************
* Method:    	GetLocalDBProtocolNameAtImageLevel
* Description: 	This function is used 
* Parameter: 	CString f_csImageLoid :
* Parameter: 	CString & f_csProtocolName :
* Returns:   	int
*************************************************************/
/*
int CTiPrepView::GetLocalDBProtocolNameAtImageLevel(CString f_csImageLoid,
        CString & f_csProtocolName,CString &f_csStudyName)
{
    try{
        int l_nStatus = E_NO_ERROR;
        CLocalDBMgr localdb;
        CString f_csCompletepath;//sb
        l_nStatus = localdb.GetProtocolNameFromImageLevelDB(f_csImageLoid, f_csStudyName,
            f_csProtocolName,f_csCompletepath);//sb
        return l_nStatus;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}
*/
/*************************************************************
* Method:    	GetLocalDBProtocolNameAtSeriesLevel
* Description: 	This function is used 
* Parameter: 	CString f_csSeriesLoid :
* Parameter: 	CString & f_csProtocolName :
* Returns:   	int
*************************************************************/
int CTiPrepView::GetLocalDBProtocolNameAtSeriesLevel(CString &f_csSeriesLoid,
                                                     CString &f_csProtocolName)
{
    try{
        int l_nStatus = E_NO_ERROR;
        CString l_csProcessName;//Shweta
        CLocalDBMgr localdb;
        l_nStatus = localdb.GetProtocolNameFromSeriesLevelDB(f_csSeriesLoid, f_csProtocolName, l_csProcessName); //Shweta
        return l_nStatus;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	OnSetMap
* Description: 	This function is used 
* Parameter: 	byte * roi_map :
* Parameter: 	int height :
* Parameter: 	int width :
* Returns:   	int
*************************************************************/
int CTiPrepView::OnSetMap(byte * roi_map, int height, int width)
{
    try{
        if(m_T1Map == NULL) {
            m_T1Map = new CT1Map();
            
            if(NULL == m_T1Map) {
                CTiPrepLogHelper::WriteToErrorLog(_T("CT1Map instantiation failed"), __LINE__, __FILE__);
                m_T1Map = NULL;
                return E_ERROR;
            }
        }
        
        return m_T1Map->SetMap(roi_map, height, width);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	RefreshGraphAndDataOnGui
* Description: 	This function is used 
* Returns:   	bool
*************************************************************/
bool CTiPrepView::RefreshGraphAndDataOnGui()
{
    try{
        if((NULL == m_DisplayData) || (NULL == m_prep_dlg)) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RefreshGraphAndDataOnGui(): m_DisplayData or m_prep_dlg found NULL"), __LINE__, __FILE__);
            return(false);
        }
        if (!m_prep_dlg->m_flag)return false; //AN/Added
        int l_nstatus = E_NO_ERROR;
        long l_Img_selected = 0;
        m_DisplayData->getSelImgNum(&l_Img_selected);
        BOOL l_bRoi[MAX_LINE_ROI];
        BOOL l_bDispGrap[3];
        BOOL l_bRevMin[3];
        l_bRoi[ROI_1] = m_prep_dlg->m_checkDispROI1Data.GetCheck();
        l_bRoi[ROI_2] = m_prep_dlg->m_checkDispROI2Data.GetCheck();
        l_bRoi[ROI_3] = m_prep_dlg->m_checkDispROI3Data.GetCheck();
        l_bRoi[NOISE_ROI_1] = m_prep_dlg->m_checkNoiseROI1Data.GetCheck();
        l_bRoi[NOISE_ROI_2] = m_prep_dlg->m_checkNoiseROI2Data.GetCheck();  
        l_bDispGrap[0] = m_prep_dlg->m_checkOrgData.GetCheck();
        l_bDispGrap[1] = m_prep_dlg->m_checkInvData.GetCheck();
        l_bDispGrap[2] = m_prep_dlg->m_checkFitData.GetCheck();
        l_bRevMin[0] = m_prep_dlg->m_checkRevDispROI1Data.GetCheck();
        l_bRevMin[1] = m_prep_dlg->m_checkRevDispROI2Data.GetCheck();
        l_bRevMin[2] = m_prep_dlg->m_checkRevDispROI3Data.GetCheck();
        int l_Ymax = 0;
        int l_Ymin = 0;
        int l_Xmax = 0;
        int l_Xmin = 0;
        
        if(IsT1MapSelected == true) {
            BOOL l_bNoiseRoi[MAX_NOISE_ROI];
            l_bNoiseRoi[0] = m_prep_dlg->m_checkNoiseROI1Data.GetCheck();
            l_bNoiseRoi[1] = m_prep_dlg->m_checkNoiseROI2Data.GetCheck();
            LONG TotalAnnularPoints = 0;
            TotalAnnularPoints = m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();

            int l_nSelectedIndex = 0;
            l_nSelectedIndex = m_DisplayData->GetSelectedIndex();
            l_nstatus = m_DisplayData->getMinMaxForYaxisMapMode(l_Img_selected, l_bRoi[0], l_bNoiseRoi, l_bDispGrap, l_bRevMin, l_Ymax, l_Ymin,l_nSelectedIndex,TRUE);
            //no need to check status for E_IMPROPER_INDEX as we dont need to set selected index here
        } else {
            l_nstatus = m_DisplayData->getMinMaxForYaxis(l_Img_selected, l_bRoi, l_bDispGrap, l_bRevMin, l_Ymax, l_Ymin);
            if(E_NO_ERROR != l_nstatus) {   
                CTiPrepLogHelper::WriteToErrorLog(_T("RefreshGraphAndDataOnGui(): Failed to get Min Max value for Y axis"), __LINE__, __FILE__);
            }
        }
        
        l_nstatus = m_DisplayData->getMinMaxForXaxis(l_Img_selected, l_Xmax, l_Xmin);
        
        if(E_NO_ERROR != l_nstatus) {   
            CTiPrepLogHelper::WriteToErrorLog(_T("RefreshGraphAndDataOnGui(): Failed to get Min Max value for X axis"), __LINE__, __FILE__);
        }
        
        int *l_nTime_Stamp = m_DisplayData->Time_stamp();
        double **l_ROI_Org_Data = m_DisplayData->getROIOrgData();
        double **l_Roi_Inv_Data = m_DisplayData->getROIIvnData();
        double **l_Roi_Inv_Rev_Data = m_DisplayData->getROIMinRevData();
        double **l_Roi_Fit_Data = m_DisplayData->getROIFitData();
        double **l_Roi_Fit_Rev_Data = m_DisplayData->getROIFitMinRevData();
        double **l_Noise_ROI_Org_Data = m_DisplayData->getNoiseROIOrgData();
        double *l_Roi_Ti_Values = m_DisplayData->getROITiValues();
        double *l_Roi_Ti_Rev_Values = m_DisplayData->getROITiMinRevValues();
        long l_nNumOfPoints = m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();
        double *l_Roi_T1_Values = m_DisplayData->getROIT1Values();
        double *l_Roi_T1_Rev_Values = m_DisplayData->getROIT1MinRevValues();
        if(l_nTime_Stamp == NULL ||
            l_ROI_Org_Data == NULL ||
            l_Roi_Inv_Data == NULL ||
            l_Roi_Inv_Rev_Data == NULL ||
            l_Roi_Fit_Data == NULL ||
            l_Roi_Fit_Rev_Data == NULL ||
            l_Noise_ROI_Org_Data == NULL ||
            l_Roi_Ti_Values == NULL ||
            l_Roi_Ti_Rev_Values == NULL ||
            l_nNumOfPoints <= 0 || l_Roi_T1_Values == NULL || l_Roi_T1_Rev_Values == NULL) { //KJ-JULY
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RefreshGraphAndDataOnGui(): Data Pointers are NULL"), __LINE__, __FILE__);
            return false;
        }
        
        l_nstatus = m_prep_dlg->m_tigraphctrl.SetGraphData(
            l_Img_selected, l_nTime_Stamp, l_ROI_Org_Data, l_Roi_Inv_Data,
            l_Roi_Fit_Data, l_Roi_Fit_Rev_Data, l_Roi_Inv_Rev_Data, l_Roi_Ti_Values, l_Roi_Ti_Rev_Values,
            l_Noise_ROI_Org_Data,
            l_Ymax, l_Ymin, l_Xmax, l_Xmin, l_nNumOfPoints,l_Roi_T1_Values,l_Roi_T1_Rev_Values);
        
        if(E_NO_ERROR != l_nstatus) {   
            CTiPrepLogHelper::WriteToErrorLog
                (_T("RefreshGraphAndDataOnGui(): Failed to set data for graph"), __LINE__, __FILE__);
        }
        
        if(true == IsT1MapSelected) {   
            m_prep_dlg->displayMapModeData();
        } else {
            m_prep_dlg->displayData();
        }
        
        m_prep_dlg->displayNoiseData();
        return(true);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return false;
    }
}

/*************************************************************
* Method:    	GetRepeatationTimeFromTag
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepView::GetRepeatationTimeFromTag()
{
    try{
        CString l_protocol_name, l_intermediate_string, l_study_file_name(_T(""));
        int l_process_no = 0;
        int l_protocol_no = 0;
        float l_gating_period = 0;
        int l_tag_reading_status = 0;
        CvfcDBMgr l_vfcDb;
        CLocalDBMgr l_localdb ;
        vector<CString> l_img_loid;
        CString l_csProcessName;
        
        if(NULL != m_DisplayData) {
            int l_nstatus = m_DisplayData->getSelImgLOID(l_img_loid);
            
            if(E_NO_ERROR != l_nstatus) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("GetRepeatationTimeFromTag(): getSelImgLOID returned error"), __LINE__, __FILE__);
                return(E_ERROR);
            }
        }
        
        CString l_csSeriesLOID(_T(""));
        int l_nStatus = l_localdb.GetParentLOID
            (l_img_loid.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Unable to get Parent LOID from LocalDB."), ERROR_LOGONLY);
            return(E_ERROR);
        }
        
        CLocalDBMgr localdb;
        l_nStatus = localdb.GetProtocolNameFromSeriesLevelDB
            (l_csSeriesLOID, l_protocol_name, l_csProcessName);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get protocol."), ERROR_LOGONLY);
            return(E_ERROR);
        }
        
        l_intermediate_string.Format(_T("%s"), l_csProcessName);
        int l_count_special_char = l_intermediate_string.Find(_T(':'));
        l_study_file_name = l_intermediate_string.Left(l_count_special_char);
        int l_total_length = l_csProcessName.GetLength();
        l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
        l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
        l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
        l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        char l_intermediate_char[10];
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        l_protocol_no = atoi(l_intermediate_char);
        l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
        l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
        l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
        l_intermediate_string = l_intermediate_string.Right(l_count_special_char + 1);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        l_process_no = atoi(l_intermediate_char);
        VfDBHandle_t  l_vfd;
        int l_vfc_file_open_status = l_vfcDb.OpenDBFile(l_study_file_name,l_vfd);
        
        if(E_NO_ERROR != l_vfc_file_open_status) {
            /*CTiPrepLogHelper::WriteToErrorLog(_T("OpenDBFile returned error"), ERROR_LOGONLY);*/
            return(E_ERROR);
        }
        
        l_tag_reading_status = l_vfcDb.GetGatingPeriod(l_protocol_no, l_process_no, l_gating_period,l_vfd);
        
        if(E_NO_ERROR != l_tag_reading_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get TR value"), __LINE__, __FILE__);
            l_vfcDb.CloseDBFile(l_vfd);
            return E_ERROR;
        }
        
        if(NULL == m_prep_dlg) {
            l_vfcDb.CloseDBFile(l_vfd);
            CTiPrepLogHelper::WriteToErrorLog(_T("TiPrep Dlg pointer NULL "), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        float l_tr_value = l_gating_period * SCALING_FACTOR ;
        m_prep_dlg->SetRepeatationTime(l_tr_value);
        l_nStatus = l_vfcDb.CloseDBFile(l_vfd);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Unable to close study file in VFC"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GetRoiDataAndDisplayROI
* Description: 	This function is used 
* Returns:   	bool
*************************************************************/
bool CTiPrepView::GetRoiDataAndDisplayROI()
{
    try{
        CROIHandler *ROI_ptr = NULL;
        ROI_ptr = CROIHandler::GetInstance();
        CString l_csProtocolName(_T(""));
        BOOL l_resultROI = TRUE;
        
        if(NULL == ROI_ptr) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetRoiDataAndDisplayROI(): Null pointer"), __LINE__, __FILE__);
            return(false);
        }
        
        vector<CString> l_csImgInfo;
        CString l_csProcessName(_T(""));
        
        if(NULL != m_DisplayData) {
            int l_nstatus = m_DisplayData->getSelImgLOID(l_csImgInfo);
            
            if(E_NO_ERROR != l_nstatus) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("GetRoiDataAndDisplayROI(): getSelImgLOID returned error"), __LINE__, __FILE__);
                return(E_ERROR);
            }
        }
        
        CLocalDBMgr localdb;
        CString f_csStudyName;
        unsigned short l_image_width = 0;
        unsigned short l_image_height = 0;
        CString f_csCompletepath;//sb
        int status = 0;
        if(0 < l_csImgInfo.size()) {
           status =  localdb.GetProtocolNameFromImageLevelDB(l_csImgInfo.at(0), l_csProtocolName,f_csStudyName,f_csCompletepath);//sb
         if (status != E_NO_ERROR)
             CTiPrepLogHelper::WriteToErrorLog(_T("GetRoiDataAndDisplayROI() : GetProtocolNameFromImageLevelDB() return fail status"),__LINE__,__FILE__);
          /*  localdb.TagReadingForHeightWidthImage(l_csImgInfo.at(0),                                      
                &l_image_width,
                &l_image_height);*/ //KJ /Not used
        }
        
        CString mode (_T(""));
        if(true == IsT1MapSelected) {
			mode = _T("MAP");
            l_resultROI = ROI_ptr->GetsaveROIParams(l_csProtocolName, _T("MAP"),l_image_height,l_image_width);
        } else {
			mode = _T("AVG");
            l_resultROI = ROI_ptr->GetsaveROIParams(l_csProtocolName, _T("AVG"),l_image_height,l_image_width);
        }
        
        ROI_ptr->SetProtocolNameForSetROIParams(l_csProtocolName); 

        if(TRUE != l_resultROI) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetRoiDataAndDisplayROI(): failed to GetsaveROIParams"), __LINE__, __FILE__);
            return(false);
        }
        
       // l_resultROI = GetROIInstance()->SetRoiPosition();
		l_resultROI = GetROIInstance()->RedrawROI(false,l_csProtocolName,mode);
        
        if(l_resultROI == FALSE) { //KJ-JULY
            CTiPrepLogHelper::WriteToErrorLog
                (_T("GetRoiDataAndDisplayROI(): l_resultROI is FALSE"), __LINE__, __FILE__);
            return(false);
        }
        
        return(true);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return false;
    }
}

/*************************************************************
* Method:    	OnROICopy
* Description: 	This function is used 
* Parameter: 	CString f_csSeriesLoid :
* Returns:   	int
*************************************************************/
int CTiPrepView::OnROICopy(CString &f_csSeriesLoid)
{
    try{
        CString l_csProtName = _T("");
        int l_nstatus = GetLocalDBProtocolNameAtSeriesLevel(f_csSeriesLoid, l_csProtName);
        
        if(E_NO_ERROR != l_nstatus) {
            CString l_csErrMsg(_T(" OnROICopy():getProtocolNumberFromDB Failed!"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_GETTING_PROTOCOL_NUM_AT_SERIES_LEVEL;
        }
        
        BOOL l_resultROI = 0;
        if(IsT1MapSelected == false) {
			CString l_csmode(_T("AVG"));
            l_resultROI = GetROIInstance()->SaveUpdateROIParamsInFile(l_csProtName, l_csmode);
        } else if(IsT1MapSelected == true) {
			CString l_csmode(_T("MAP"));
            l_resultROI = GetROIInstance()->SaveUpdateROIParamsInFile(l_csProtName, l_csmode);
        }
        
        if(TRUE != l_resultROI){
            CString l_csErrMsg(_T("OnROICopy():SaveUpdateROIParamsInFile Failed!"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_FILE_WRITE;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	DisplaySeriesNumber
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepView::DisplaySeriesNumber()
{
    try{
        vector<CString> l_csloids;
        
        if(m_DisplayData) {
            int l_nstatus = m_DisplayData->getSelImgLOID(l_csloids);
            
            if(E_NO_ERROR != l_nstatus) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("DisplaySeriesNumber(): getSelImgLOID returned error"), __LINE__, __FILE__);
            }
            
            if(l_csloids.size() > 0) {
                DisplaySeriesNumber(l_csloids.at(0));
            } else { //KJ-JULY
             //   CTiPrepLogHelper::WriteToErrorLog
                 //   (_T("DisplaySeriesNumber(): l_csloids.size() is zero"), __LINE__, __FILE__);
            }
        } else { //KJ-JULY
            CTiPrepLogHelper::WriteToErrorLog
                (_T("DisplaySeriesNumber(): m_DisplayData is NULL"), __LINE__, __FILE__);
        }
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
    }
}
/*************************************************************
* Method:    	DisplaySeriesNumber
* Description: 	This function is used 
* Parameter: 	CString & imgLOID :
* Parameter: 	int f_nSelDlg :
* Returns:   	void
*************************************************************/
void CTiPrepView::DisplaySeriesNumber(CString &imgLOID,  int f_nSelDlg)
{
    try{
        TIPREP_TRACE(CTiPrepView::DisplaySeriesNumber,MRTraceMarker9);
        CString l_csseries_number(_T(""));
        long series_no = 0;
        
        CString l_csSriesLoid(_T(""));
        int status = 0;
        CLocalDBMgr localdb;
        localdb.GetParentLOID(imgLOID, LOID_TYPE_IMAGE, l_csSriesLoid);
        
        if(l_csSriesLoid.IsEmpty()) {
            CTiPrepLogHelper::WriteToErrorLog(_T("DisplaySeriesNumber() Parent Series LOID not found"), __LINE__, __FILE__);
            return ;
        }
        
        status = localdb.GetSeriesNumberFromDB(l_csSriesLoid, &series_no);
        
        if(status != E_NO_ERROR) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Failed to get Series number from Database"), __LINE__, __FILE__);
            return ;
        }
        
        l_csseries_number.Format(_T("%d"), series_no);
        
        if(m_prep_dlg && m_prep_dlg->GetSafeHwnd()) {
            m_prep_dlg->DisplaySeriesName(l_csseries_number, f_nSelDlg); 
        } else {
            //CString lcsmsg (_T("dlg is not yet invoked"));
            //CTiPrepLogHelper::WriteToErrorLog(lcsmsg,__LINE__,__FILE__);
        }
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	AddZOrderLayer
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepView::AddZOrderLayer()
{
    try {
        CDPSComManager *pqm = NULL;
        pqm = CDPSComManager::GetInstance();
        
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
            CString l_cserrmsg = (_T("CDPSComManager NULL pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("AddZOrderLayer(): Exception occured"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	UpdateImageBuffer
* Description: 	This function is used 
* Parameter: 	CString & f_csimgLoid :
* Parameter: 	void * imgBuff :
* Parameter: 	unsigned int imgSz :
* Returns:   	int
*************************************************************/
int CTiPrepView::UpdateImageBuffer(CString &f_csimgLoid, void *imgBuff, unsigned int imgSz)
{
    try{
        CLocalDBMgr localdb;
        
        if(NULL == imgBuff || imgSz <= 0) { //KJ-JULY
            CString l_cserrmsg = (_T("imgBuff NULL or imgSz is Zero"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        return  localdb.UpdateImageBuffer(f_csimgLoid, imgBuff, imgSz);
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	getNumImgSelectedISel
* Description: 	This function is used 
* Parameter: 	long & f_nSelImgNo :
* Returns:   	int
*************************************************************/
int CTiPrepView::getNumImgSelectedISel(long &f_nSelImgNo)
{
    try{
        if(m_DisplayData != NULL) {
            m_DisplayData->getSelImgNum(&f_nSelImgNo);
        } else {
            return E_ERROR;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	SaveRoiDataToFile
* Description: 	This function is used 
* Parameter: 	BOOL f_ROI :
* Returns:   	bool
*************************************************************/
bool CTiPrepView::SaveRoiDataToFile(BOOL f_ROI[])
{
    CStdioFile fp;
    try{
    CString l_csfilepath(_T("c:/MrMplus/Data/TiPrep/TiPrep_ROIVal.csv"));
    CFileException e;
    
    
    if(NULL != m_prep_dlg) {
        if(FALSE == m_prep_dlg->m_flag) {
            CString l_cserror(_T(""));
            l_cserror.Format(_T("SaveRoiDataToFile() : Error : Apply not performed.No data found for writing\n"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
            int l_nErrCode = E_NO_DATA_FOUND;
            DisplayErr(l_nErrCode, TIPREP_DLG); //ITs ok, last function call direct from button click
            return(false);
        }
    }
    
    if((FALSE == f_ROI[ROI_1]) && (FALSE == f_ROI[ROI_2]) && (FALSE == f_ROI[ROI_3])) {
        CString l_cserror(_T(""));
        l_cserror.Format(_T("SaveRoiDataToFile() : No ROI has been selected\n"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
        int l_nErrCode = E_NO_ROI_SELECTED;
        DisplayErr(l_nErrCode, TIPREP_DLG);
        return(false);
    }
    
    int l_nSelectedIndex = 0;   //these variables are used in an if below hence declared here
    int l_nNumOfPoints = 0;
    
    char destFldr[100] = {'\0'};
    strcpy(destFldr, "c:/MrMplus/Data/TiPrep");    
    if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)) {
        //folder present
    }else{
        CTiPrepLogHelper::WriteToErrorLog
            (_T("SaveRoiDataToFile():c:/MrMplus/Data/TiPrep  path not found"), __LINE__);
        int  l_nErrCode = E_FILE_WRITE;
        DisplayErr(l_nErrCode, TIPREP_DLG);
        return E_ERROR;
    }
    if(true == IsT1MapSelected) {
        l_nNumOfPoints = m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();
        
        if(l_nNumOfPoints <= 0) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SaveRoiDataToFile(): Selected index out of range error"), __LINE__, __FILE__);
            int l_nErrCode = E_NO_DATA_FOUND_MAP;
            DisplayErr(l_nErrCode, TIPREP_DLG);
            return(false);
        }
        
        l_nSelectedIndex = m_DisplayData->GetSelectedIndex();
        
        if((l_nSelectedIndex < 0) || (l_nSelectedIndex >= l_nNumOfPoints)) { //AN Modified
            CString l_csval(_T(""));
            l_csval.Format(_T("Selected Index: %d"),l_nSelectedIndex);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker10, l_csval, _T("SaveRoiDataToFile(): Selected index out of range error"));
            int l_nErrCode = E_SELECT_VALID_MAP_POINT;
            DisplayErr(l_nErrCode, TIPREP_DLG);
            return(false);
        }
    }
    
    if(!fp.Open(l_csfilepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite , &e)) {
        CString l_cserror(_T(""));
        l_cserror.Format(_T("SaveRoiDataToFile() : Could not open \"%s\" for writing\n"), l_csfilepath);
        CTiPrepLogHelper::WriteToErrorLog
            (l_cserror, __LINE__, __FILE__);
        int  l_nErrCode = E_FILE_WRITE;
        DisplayErr(l_nErrCode, TIPREP_DLG);
        return(false);
    }
    
    fp.SeekToEnd();
    long l_nTotalImgCnt = 0;
    double ** l_pOrgData = NULL;
    double ** l_pInvData = NULL;
    double ** l_pFitData = NULL;
    GetImgdata(l_pOrgData, l_pInvData, l_pFitData, l_nTotalImgCnt);
    
    if(NULL == l_pOrgData || NULL == l_pInvData || NULL == l_pFitData) {
        CString l_cserror(_T(""));
        l_cserror.Format(_T("SaveRoiDataToFile() : Data not available\n"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
        fp.Close();
        return(false);
    }
    
    CString l_csRow(_T(""));
    double l_fImgOrgData = 0.0f;
    double l_fImgInvData = 0.0f;
    double l_fImgFitData = 0.0f;
    
    if(true == IsT1MapSelected) {   //indicates map mode
        int x = 0, y = 0;
        
        if(NULL == m_DisplayData) {
            CString l_cserror(_T(""));
            l_cserror.Format(_T("SaveRoiDataToFile() : m_DisplayData found NULL\n"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
            fp.Close();
            return(false);
        }
        
        if(NULL == m_T1Map) {
            CString l_cserror(_T(""));
            l_cserror.Format(_T("SaveRoiDataToFile() : m_T1Map found NULL\n"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);
            fp.Close();
            return(false);
        }
        
        m_T1Map->GetMouseXY(l_nSelectedIndex, x, y);
        CString l_cstemp_str(_T(""));
        l_cstemp_str.Format(_T("\"%s\",\"[%d,%d]\"\n"), COLUMN_HEADER1_MAPMODE, x, y);
        fp.WriteString(l_cstemp_str);
        fp.SeekToEnd();
        fp.WriteString(COLUMN_HEADER2_MAPMODE);
        fp.SeekToEnd();
        
        for(int l_nImgCnt = 0; l_nImgCnt < l_nTotalImgCnt; l_nImgCnt++) {
            l_csRow.Empty();
            int l_nImgDisplayIndex = l_nImgCnt + 1;
            CString l_sImgIndex;
            l_sImgIndex.Format(_T("%d,"), l_nImgDisplayIndex);
            l_csRow += l_sImgIndex;
            l_fImgOrgData = l_pOrgData[l_nSelectedIndex][l_nImgCnt];
            l_fImgInvData = l_pInvData[l_nSelectedIndex][l_nImgCnt];
            
            if(&(l_pFitData[l_nSelectedIndex][l_nImgCnt*m_ExtraPtsINTDVD]) != NULL) { //this adjustment is done due to extra point generation
                l_fImgFitData = l_pFitData[l_nSelectedIndex][l_nImgCnt*m_ExtraPtsINTDVD];
            }
            
            CString l_csData(_T(""));
            l_csData.Format(_T("%lf,%lf,%lf,"),
                l_fImgOrgData,
                l_fImgInvData,
                l_fImgFitData);
            l_csRow += l_csData;
            int l_nstrlen = l_csRow.GetLength();
            l_csRow.Delete(l_nstrlen - 1);
            l_csRow += _T("\n");
            fp.WriteString(l_csRow);
            fp.SeekToEnd();
        }
    } else {                                        //indicates avg mode
       
        CString l_csHeader1(_T(""));
        CString l_csHeader2(_T(""));
        CString l_csTempStr(_T(""));
        bool l_bFirstWrite = true;  //purposely kept true

        for(int l_nroi = 0;l_nroi < MAX_NUM_ROI;l_nroi++){            
            if(FALSE == f_ROI[l_nroi]){ //if roi is not selected then skip
                continue;
            }
            
            if(l_nroi == 0){
                l_csTempStr.Format(_T(",,ROI%d"),(l_nroi+1));
            }else{
                if(false == l_bFirstWrite){ //i.e "Img# column has been written hence ,,, required"
                    l_csTempStr.Format(_T(",,,ROI%d"),(l_nroi+1));  //3 ,
                }else{
                    l_csTempStr.Format(_T(",,ROI%d"),(l_nroi+1));   //2 ,
                }
                
            }
            l_csHeader1 += l_csTempStr;

            l_csTempStr.Empty();    //clear this after use
            
            if(l_bFirstWrite == true){  //this is done to handle for first column which has "Img#"
                                        //other columns does not have "Img#"
                l_bFirstWrite = false;
                l_csHeader2 += _T("Img#,Original,Inverted,CurveFit");   //this should be executed only once
            }else{
                l_csHeader2 += _T(",Original,Inverted,CurveFit");
            }
              
        }
        //add "\n"
        l_csHeader1 += "\n";
        l_csHeader2 += "\n";

        fp.WriteString(l_csHeader1);
        fp.SeekToEnd();
        fp.WriteString(l_csHeader2);
        fp.SeekToEnd();
        
        for(int l_nImgCnt = 0; l_nImgCnt < l_nTotalImgCnt; l_nImgCnt++) {
            l_csRow.Empty();
            int l_nImgDisplayIndex = l_nImgCnt + 1;
            CString l_sImgIndex;
            l_sImgIndex.Format(_T("%d,"), l_nImgDisplayIndex);
            l_csRow += l_sImgIndex;
            
            for(int l_nROICnt = 0; l_nROICnt < ROICNT ; l_nROICnt++) {
                if(FALSE == f_ROI[l_nROICnt]) {
                    continue;
                }
                
                l_fImgOrgData = l_pOrgData[l_nROICnt][l_nImgCnt];
                l_fImgInvData = l_pInvData[l_nROICnt][l_nImgCnt];
                
                if(&(l_pFitData[l_nROICnt][l_nImgCnt*m_ExtraPtsINTDVD]) != NULL) { 
                    //this adjustment is done due to extra point generation
                    l_fImgFitData = l_pFitData[l_nROICnt][l_nImgCnt*m_ExtraPtsINTDVD];
                }
                
                CString l_csData(_T(""));
                l_csData.Format(_T("%lf,%lf,%lf,"),
                    l_fImgOrgData,
                    l_fImgInvData,
                    l_fImgFitData);
                l_csRow += l_csData;
            }
            
            int l_nstrlen = l_csRow.GetLength();
            l_csRow.Delete(l_nstrlen - 1);
            l_csRow += _T("\n");
            fp.WriteString(l_csRow);
            fp.SeekToEnd();
        }
    }
    
    fp.Close();
    
    }catch(...){
        CString l_cserror(_T(""));
        l_cserror.Format(_T("SaveRoiDataToFile() : Exception occurred\n"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserror, __LINE__, __FILE__);        
        fp.Close();
        return(false);
    }
    return(true);
}

/*************************************************************
* Method:    	GetStudyNameFromDB
* Description: 	This function is used 
* Parameter: 	CString & f_csStudyName :
* Returns:   	int
*************************************************************/
int CTiPrepView::GetStudyNameFromDB(CString &f_csStudyName)
{
    try{
        CString l_csTempStr = _T("");
        CString l_csProtocolName = _T("");
        vector<CString> l_csImgInfo;
        l_csImgInfo.clear();
        m_DisplayData->getSelImgLOID(l_csImgInfo);
        
        if(NULL == m_DisplayData) {
            CTiPrepLogHelper::WriteToErrorLog(_T("GetStudyNameFromDB() : m_DisplayData is NULL "),__LINE__,__FILE__);
            return E_ERROR;
        }
        
        CLocalDBMgr *l_pdb_mgr = new CLocalDBMgr();
        
        if(NULL == l_pdb_mgr) {
            CTiPrepLogHelper::WriteToErrorLog(_T("GetStudyNameFromDB() :Memory allocation failed"),__LINE__,__FILE__);
            return E_ERROR;
        }
        CString f_csStudyNameP (_T(""));
        int status = 0;
        CString f_csCompletepath;//sb
        status = l_pdb_mgr->GetProtocolNameFromImageLevelDB(l_csImgInfo.at(0), l_csProtocolName,f_csStudyNameP,f_csCompletepath);//sb
        if_no_error(status) {
            l_csTempStr.Format(_T("%s"), l_csProtocolName);
            int cnt = l_csTempStr.Find(_T(':'));
            f_csStudyName = l_csTempStr.Left(cnt);
            
            cnt = f_csStudyName.Find(_T("Run"),0);
            int totalCnt = f_csStudyName.GetLength();
            f_csStudyName = f_csStudyName.Right(totalCnt - (cnt));
            
            
            int folder_break = 0;
            folder_break = f_csStudyName.Find('/');
            if (folder_break != -1)
                f_csStudyName = f_csStudyName.Left(folder_break);
            
            totalCnt = f_csStudyName.GetLength();
            cnt = f_csStudyName.Find(_T('.'));
            if (cnt != -1)
                f_csStudyName = f_csStudyName.Left(cnt);
            
            if(NULL != l_pdb_mgr) {
                delete l_pdb_mgr;
                l_pdb_mgr = NULL;
            }
            
            return E_NO_ERROR;
        } else {
            if(NULL != l_pdb_mgr) {
                delete l_pdb_mgr;
                l_pdb_mgr = NULL;
            }
            
            if(NULL != l_pdb_mgr) {
                delete l_pdb_mgr;
                l_pdb_mgr = NULL;
            }
            return status;
        }
        
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GetSelectedIndex
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepView::GetSelectedIndex()
{
    if(NULL != m_DisplayData) {
        return(m_DisplayData->GetSelectedIndex());
    } else {
        return 0;//Zeroth Index will be selected all the time
    }
}

/*************************************************************
* Method:    	getDataForm
* Description: 	This function is used 
* Parameter: 	vector<CString> & f_csloids :
* Parameter: 	int numImgs :
* Returns:   	int
*************************************************************/
int CTiPrepView::getDataForm(vector<CString> &f_csloids, int numImgs)
{
    try{
        //long l_nNumImgSel = 0;
        CString l_csDataType(_T(""));
        
        if(/*l_nStatus == E_NO_ERROR &&*/ numImgs != 0) {
            int l_reading_status = vfcDataGetDataForm(l_csDataType, f_csloids);
            
            if(E_NO_ERROR != l_reading_status) {
                if (m_prep_dlg) m_prep_dlg->SetDataForm(_T(""));
                return l_reading_status;
            }
            
            if (m_prep_dlg) m_prep_dlg->SetDataForm(l_csDataType);
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	getInitialGuessFlag
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepView::getInitialGuessFlag() const
{
    return m_nInitialGuessFlag;
}

/*************************************************************
* Method:    	setInitialGuessFlag
* Description: 	This function is used 
* Parameter: 	const int & val :
* Returns:   	void
*************************************************************/
void CTiPrepView::setInitialGuessFlag(const int &val)
{
    m_nInitialGuessFlag = val;
}

/*************************************************************
* Method:    	setInitialGuessValues
* Description: 	This function is used 
* Parameter: 	const double & f_dAfactor :
* Parameter: 	const double & f_dBfactor :
* Parameter: 	const double & f_dMfactor :
* Parameter: 	const double & f_dNfactor :
* Returns:   	int
*************************************************************/
int CTiPrepView::performCurveFitForInitialGuessValues(const double &f_dAfactor,
                                       const double &f_dBfactor,
                                       const double &f_dMfactor,
                                       const double &f_dNfactor,int &f_nEqnFlag)
{
    int l_nStatus = m_lineFitData.setInitialGuessValues(f_dAfactor, f_dBfactor, f_dMfactor, f_dNfactor);
    if (E_NO_ERROR != l_nStatus)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("Failed save initial guess values."), __LINE__, __FILE__);
        return E_ERROR;
    }
    m_nFlags = f_nEqnFlag;
    l_nStatus = OnApply(f_nEqnFlag);
    if(E_NO_ERROR != l_nStatus) { 
        CTiPrepLogHelper::WriteToErrorLog(_T("OnEquationTwo(): OnApply Failed"), __LINE__, __FILE__);
        return l_nStatus;
    }

    return E_NO_ERROR;
}

/*************************************************************
* Method:    	getInitialGuessValues
* Description: 	This function is used 
* Parameter: 	double & f_dAfactor :
* Parameter: 	double & f_dBfactor :
* Parameter: 	double & f_dMfactor :
* Parameter: 	double & f_dNfactor :
* Returns:   	int
*************************************************************/
int CTiPrepView::getInitialGuessValues(double &f_dAfactor,
                                       double &f_dBfactor,
                                       double &f_dMfactor,
                                       double &f_dNfactor)
{
    m_lineFitData.getInitialGuessValues(f_dAfactor, f_dBfactor, f_dMfactor, f_dNfactor);
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	CreateMapFiles
* Description: 	This function is used 
* Parameter: 	double * f_Roi_T1_values :
* Parameter: 	double * f_Roi_T1_Min_Rev_Values :
* Returns:   	int
*************************************************************/
int CTiPrepView::CreateMapFiles(double* f_Roi_T1_values, double* f_Roi_T1_Min_Rev_Values)
{
    try{
        if(NULL == m_T1Map) {
            return E_ERROR;
        }
        
        char destFldr[100] = {'\0'};
        strcpy(destFldr, "c:/MrMplus/Data/TiPrep");
        
        if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)/*ENOENT != _mkdir(destFldr)*/) {
            CString l_csFldr(destFldr);
            SetFileAttributes(l_csFldr, FILE_ATTRIBUTE_NORMAL);
            char temp[50];
            CString l_csStudyFileName(_T(""));
            m_DisplayData->GetStudyFileName(l_csStudyFileName);
            
            if(l_csStudyFileName.IsEmpty() == FALSE) {
                wcstombs(temp, (LPCTSTR)l_csStudyFileName, 50);
                strcat(destFldr , "/");
                strcat(destFldr , temp);
            }
            
            if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)/*ENOENT !=_mkdir(destFldr)*/) {
                CString l_csFldr(destFldr);
                SetFileAttributes(l_csFldr, FILE_ATTRIBUTE_NORMAL);
                CString fname = destFldr ;
                fname = fname + _T("/") + _T("T1Map");//T1MAP_FILEPATH;
                
                int l_nStatus = m_T1Map->CreateT1Map(f_Roi_T1_values, fname);
                if(E_NO_ERROR != l_nStatus) { 
                    CTiPrepLogHelper::WriteToErrorLog
                        (_T("CreateMapFiles():T1MAP_FILEPATH CreateT1Map Failed"), __LINE__);				
                    return(l_nStatus);
                }
                
                fname = destFldr;
                fname = fname + _T("/") + _T("T1MinMap");//T1MINMAP_FILEPATH;
                
                l_nStatus = m_T1Map->CreateT1Map(f_Roi_T1_Min_Rev_Values, fname);
                if(E_NO_ERROR != l_nStatus) { 
                    CTiPrepLogHelper::WriteToErrorLog
                        (_T("CreateMapFiles():T1MINMAP_FILEPATH CreateT1Map Failed"), __LINE__);
                    return(l_nStatus);
                }
            } else {
                //log error
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("CreateMapFiles():CreateDirectoryA Folder Failed"), __LINE__);
                return E_ERROR;
            }
        } else {
            //log errror
            CTiPrepLogHelper::WriteToErrorLog
                (_T("CreateMapFiles():c:/MrMplus/Data/TiPrep  path not found"), __LINE__);
            return E_ERROR;
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}



/*************************************************************
* Method:    	UnRegisterWindowZOrder
* Description: 	This function is used 
* Parameter: 	HWND hWnd_i :
* Returns:   	BOOL
*************************************************************/
BOOL CTiPrepView::UnRegisterWindowZOrder(HWND hWnd_i)
{
    try{
        TIPREP_TRACE(CTiPrepView::UnRegisterWindowZOrder,MRTraceMarker9);
        BOOL l_result = FALSE;
        CDPSComManager *l_server = CDPSComManager::GetInstance();
        
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
                if(l_msg != NULL) {
					if(!l_server->SendMsg(l_msg)) {
						l_result = FALSE;
					}
					DEL_MSG(l_msg);
					l_msg = NULL;
                }
                
                l_server->Disconnect();
            } else {
                CString l_cserr(_T("DPSComm Parent is NULL "));
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__);
            }
        } else { 
            CString l_cserr(_T("CDPSComManager is NULL "));
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__);
            return FALSE;
        }
        
        return l_result;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:    	DisplayErr
* Description: 	This function is used 
* Parameter: 	int & f_nmsg_code :
* Parameter: 	int dlgcode :
* Returns:   	void
*************************************************************/
void CTiPrepView::DisplayErr(int &f_nmsg_code, int dlgcode)  
{
    SetWaitCursor(false);
    
    if(m_prep_dlg) {
        m_prep_dlg->DisplayErr(f_nmsg_code, dlgcode);    
    }
}

/*************************************************************
* Method:    	GetHeightWidthImageFromDB
* Description: 	This function is used 
* Parameter: 	unsigned short & f_image_height :
* Parameter: 	unsigned short & f_image_width :
* Returns:   	int
*************************************************************/
int CTiPrepView::GetHeightWidthImageFromDB( unsigned short &f_image_height,unsigned short&f_image_width )
{
    TIPREP_TRACE(CTiPrepView::GetHeightWidthImageFromDB,MRTraceMarker3);

    try{
        
        vector<CString> l_csImgInfo;
        CString l_csProcessName(_T(""));
        int l_status = m_DisplayData->getSelImgLOID(l_csImgInfo);
        
        
        if(E_NO_ERROR != l_status) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnCmdGetSelImg(): getSelImgLOID returned error"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        CLocalDBMgr localdb;    
        if(0 < l_csImgInfo.size()){
            localdb.TagReadingForHeightWidthImage(l_csImgInfo.at(0),                                      
                &f_image_width,
                &f_image_height);
        }
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_VIEW, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	DeAllocateMemoryOnCheckT1Map
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepView::DeAllocateMemoryOnCheckT1Map()
{
    if(m_DisplayData)
    {
        m_DisplayData->DeAllocateMemory();
    }
}


#define DEL_ARR_PTR(ptr) if (ptr){ \
    delete [] (ptr); \
    ptr = NULL;\
}

int CTiPrepView::getVfcTI(CString& StudyPath,int &f_nProtIndex,int *f_nTI)

{
    CvfcDBMgr l_vfcDb;
    VfDBHandle_t  l_vfd = NULL;
    int l_status = l_vfcDb.OpenDBFile(StudyPath,l_vfd);        
    if(E_NO_ERROR != l_status) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Failed to Open vfc File."),__LINE__, __FILE__);
        return(E_VF_OPEN);
    }
    float l_fTimeStamp = 0.0;
    float l_fTi = 0.0;
    l_status = l_vfcDb.GetVfTI((int)f_nProtIndex,l_fTi,l_vfd);
    if(E_NO_ERROR != l_status) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Failed GetVfTI."),__LINE__, __FILE__);
        l_vfcDb.CloseDBFile(l_vfd);
        return(E_Vf_GET_ERR);
    }
    *f_nTI =  (int)(l_fTi * 1000);

    l_status = l_vfcDb.CloseDBFile(l_vfd);
    if (E_NO_ERROR != l_status)
    {
        return E_ERROR;
    }
    return 0;
}

/*************************************************************
* Method:    	getVfcTimeStamp
* Description: 	This function is used 
* Parameter: 	CString & f_csImgLoid :
* Parameter: 	int & f_nImgNo :
* Parameter: 	int * f_nTime_stamp :
* Returns:   	int
*************************************************************/
int CTiPrepView::getVfcTimeStamp(CString &f_csImgLoid,int &f_nImgNo,int *f_nTime_stamp)
{
    try{
        CLocalDBMgr l_localdb;
        //vector<CString> l_csImgInfo;
        CString l_csProcessName(_T(""));
        CString f_csStudyName;
        CString f_csCompletepath;
        CString l_csProtocolName;
        CString l_intermediate_string;
        
        int l_status =  l_localdb.GetProtocolNameFromImageLevelDB(f_csImgLoid, l_csProtocolName,f_csStudyName,f_csCompletepath);
        if (l_status != E_NO_ERROR)
        {
            return l_status;
        } 
        CString l_cstemp;
        ///Run40042/Run40042.-1959:/1.3/7.1/8.0/9.0
        l_intermediate_string = f_csCompletepath;
        int l_count_special_char = l_intermediate_string.Find(_T(':'));
        CString l_csStudyFileName = l_intermediate_string.Left(l_count_special_char);///Run40042/Run40042.-1959
        
        //to get protocol no
        int l_total_length = l_intermediate_string.GetLength();
        l_count_special_char = l_intermediate_string.ReverseFind(_T(':'));
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 2));
        l_cstemp = l_intermediate_string;// /1.3/7.1/8.0/9.0
        l_count_special_char = l_intermediate_string.Find(_T('/'));
        l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        char l_intermediate_char[10];
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        int l_protocol_no = atoi(l_intermediate_char);
        
        //to get process node
        l_intermediate_string = l_cstemp;
        l_count_special_char = l_intermediate_string.Find(_T('/'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        l_cstemp = l_intermediate_string;
        l_count_special_char = l_intermediate_string.Find(_T('/'));// 7.1/8.0/9.0
        l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        int l_process_no = atoi(l_intermediate_char);
        
        //to get echo node 
        l_intermediate_string = l_cstemp;
        l_count_special_char = l_intermediate_string.Find(_T('/'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1)); 
        l_cstemp = l_intermediate_string;
        l_count_special_char = l_intermediate_string.Find(_T('/'));// 8.0/9.0
        l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        int l_echo_no = atoi(l_intermediate_char);
        
        //to get image node 
        l_intermediate_string = l_cstemp;
        l_count_special_char = l_intermediate_string.Find(_T('/'));
        l_total_length = l_intermediate_string.GetLength();// 9.0
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));        
        l_count_special_char = l_intermediate_string.Find(_T('.'));
        l_total_length = l_intermediate_string.GetLength();
        l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
        sprintf(l_intermediate_char, "%S", l_intermediate_string);
        int l_image_no = atoi(l_intermediate_char);
        CvfcDBMgr l_vfcDb;
        VfDBHandle_t  l_vfd;
        l_status = l_vfcDb.OpenDBFile(l_csStudyFileName,l_vfd);        
        if(E_NO_ERROR != l_status) {
//             CTiPrepLogHelper::WriteToErrorLog
//                 (_T("Failed to Open vfc File."),__LINE__, __FILE__);
            return(E_VF_OPEN);
        }
        float l_fTimeStamp = 0.0;
        float l_fTi = 0.0;
        l_status = l_vfcDb.GetVfTimeStamp(l_protocol_no,l_process_no,
            l_echo_no,l_image_no,l_fTi,l_fTimeStamp,l_vfd);
        if(E_NO_ERROR != l_status) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Failed to Open vfc File."),__LINE__, __FILE__);
            GenerateTimeStamp(f_nImgNo,l_fTi,l_fTimeStamp);
            //return(E_Vf_GET_ERR);
        }

        l_status = l_vfcDb.CloseDBFile(l_vfd);
        if (E_NO_ERROR != l_status)
        {
            return E_ERROR;
        }
        *f_nTime_stamp =  (int)(l_fTimeStamp * 1000);
    }catch(...){
        return E_ERR_EXCEPTION;
    }
    return E_NO_ERROR;
}
#define TIME_INTERVAL 0.02
/*************************************************************
* Method:    	GenerateTimeStamp
* Description: 	This function is used 
* Parameter: 	int img_no :
* Parameter: 	float & initTI :
* Parameter: 	float & time_stamp :
* Returns:   	int
*************************************************************/
int CTiPrepView::GenerateTimeStamp(int img_no,  float &initTI,float &time_stamp)
{
    int status = E_NO_ERROR;
    time_stamp = initTI + img_no * TIME_INTERVAL;       //CPP tool change
    return(status);     //CPP tool change
}

void CTiPrepView::ClearRepeatationTimeTextBox()
{
    if(m_prep_dlg){
        m_prep_dlg->m_RepTime.SetWindowText(_T(""));
    }
}

void CTiPrepView::enableOneButtonPC(BOOL &f_bFlag)
{
    if(m_prep_dlg){
        m_prep_dlg->EnablePhaseCorrectionButton(f_bFlag);
    }
}

int CTiPrepView::OnApply_Ti()
{
    CTiPrepController::GetInstance()->DisplayApplyTiDlg();
    return 0;
}

double* CTiPrepView::GetCurrentROIsTIValues()
{
    return(m_DisplayData->getROITiValues());
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
