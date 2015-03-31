/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: ACGL
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "DPSComManager.h"
#include "IpFrame.h"
#include "AdvCardiacGLView.h"
#include "CommonDefinitions.h"
#include "ACGAppController.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
extern CAdvCardiacGLView *g_MainView;
CDPSComManager* CDPSComManager::m_dps_com_manager = NULL;
IMplusConnector* CDPSComManager::m_dpsconnector = NULL;
bool waiting = false;
/**
* Pointer to CInPlaceFrame class.
*/
extern CInPlaceFrame *g_frame;

/**
* Pointer to CACGLView class.
*/
extern CAdvCardiacGLView *g_MainView;

/**
* Pointer to CACGLApp class.
*/
extern CAdvCardiacGLApp theApp;

/**
* Parameter used for CFW communication.
*/
unsigned _int64 Uid = 0;

/**
* Constructor.
* This function is used for initialization of member variables.
*/
CDPSComManager::CDPSComManager() : m_parent_name(NULL)
{
}

/**
* Destructor.
* This function is used for releasing the allocated memory.
*/
CDPSComManager::~CDPSComManager()
{
    //    ACGL_TRACE(CDPSComManager::~CDPSComManager);
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       Initialize
* Description:  This function is used to create new connector
*               for CFW communication.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::Initialize()
{
    ACGL_TRACE(CDPSComManager::Initialize);    
    try{        
        if(NULL == m_dpsconnector) {
            m_dpsconnector = NEW_CONNECTOR();
        }
        
        if(NULL == m_dpsconnector) {
            CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to create Connector."),_T("CDPSComManager::Initialize"));
            return FALSE;
        } else {
            CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Connector created."),_T("CDPSComManager::Initialize"));
            return TRUE;
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::Initialize"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       GetInstance
* Description:  This function is used to create instance of
*               CDPSComManager class.
* Returns:      CDPSComManager*
*               Instance of CDPSComManager class.
*************************************************************/
CDPSComManager* CDPSComManager::GetInstance()
{
    ACGL_TRACE(CDPSComManager::GetInstance);
    
    try{
        if(NULL == m_dps_com_manager) {
            m_dps_com_manager = new CDPSComManager();
            
            if(NULL == m_dps_com_manager) {
                CString l_cserr = _T("Memory allocation failed.");
                CACGLLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            }
        }
        
        return m_dps_com_manager;
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::GetInstance"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        return NULL;
    }
}

/*************************************************************
* Method:       Terminate
* Description:  This function is used to delete the connector.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::Terminate()
{
    ACGL_TRACE(CDPSComManager::Terminate);
    BOOL bret = TRUE;
    
    try {
        if(m_dpsconnector != NULL) {
            DEL_CONNECTOR((IMplusConnector*)m_dpsconnector);
            m_dpsconnector = NULL;
            
        }
        
        SysFreeString(m_parent_name);
        m_parent_name = NULL;
    } catch(...) {
        throw;
        bret = FALSE;
        CString l_csmsg(_T("Exception occured in CDPSComManager::Terminate"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    }
    
    return bret;
}

/*************************************************************
* Method:       Connect
* Description:  This function is used to connect to channel
*               using CFW communication.
* Parameter:    CString & f_target
*               The target channel that application want to connect.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::Connect(CString& f_target)
{
    ACGL_TRACE(CDPSComManager::Connect);
    BOOL l_ret_val = FALSE;
    
    try {
        //We need to trim the string on the left and right to remove any
        //trailing/leading white spaces,tabs an new lines.
        f_target.TrimLeft();
        f_target.TrimRight();
        _bstr_t l_target_channel(f_target);
        
        if(m_dpsconnector != NULL) {
            if(l_target_channel != _bstr_t(_T(""))) {
                HRESULT hr = m_dpsconnector->Connect(l_target_channel);
                
                if(SUCCEEDED(hr)) {
                    l_ret_val = TRUE;
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Connection with target establish."),                        _T("CDPSComManager::Connect"));
                } else {
                    l_ret_val = FALSE;
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Failed to connect with target."),
                        _T("CDPSComManager::Connect"));
                }
            }
        }
    } catch(...) {
        CACGLLogHelper::WriteToErrorLog(_T("Exception occurred during DPSComm Connect"), __LINE__, __FILE__);
        l_ret_val = FALSE;
    }
    
    return l_ret_val;
}

/*************************************************************
* Method:       SendMsg
* Description:  This function is used to send the message from
*               the client to the server.
* Parameter:    IMplusMessage * f_message_send
*               Message to be send.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::SendMsg(IMplusMessage *f_message_send)
{
    ACGL_TRACE(CDPSComManager::SendMsg);
    BOOL l_ret_val = FALSE;
    
    try {
        //Send the message asynchronously. This can be changed
        //later on if required to send synchronously.
        if(NULL != f_message_send) {
            if(m_dpsconnector != NULL) {
                HRESULT hr = m_dpsconnector->SendASync(f_message_send);
                
                if(SUCCEEDED(hr)) {
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Message Sent."),_T("CDPSComManager::SendMsg"));
                    l_ret_val = TRUE;
                } else {
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Failed to sent message."),_T("CDPSComManager::SendMsg"));
                    l_ret_val = FALSE;
                }
            }
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::SendMsg"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_ret_val = FALSE;
    }
    
    return l_ret_val;
}
BOOL CDPSComManager::SendSyncMsg(IMplusMessage *f_message_send,IMplusMessage *f_message_recv,DWORD timeout)
{
    ACGL_TRACE(CDPSComManager::SendMsg);
    BOOL l_ret_val = FALSE;
    
    try {
        //Send the message asynchronously. This can be changed
        //later on if required to send synchronously.
        if(NULL != f_message_send) {
            if(m_dpsconnector != NULL) {
                HRESULT hr = m_dpsconnector->SendSync(f_message_send,f_message_recv,timeout);
                
                if(SUCCEEDED(hr)) {
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Message Sent."),_T("CDPSComManager::SendMsg"));
                    l_ret_val = TRUE;
                } else {
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Failed to sent message."),_T("CDPSComManager::SendMsg"));
                    l_ret_val = FALSE;
                }
            }
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::SendMsg"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_ret_val = FALSE;
    }
    
    return l_ret_val;
}
/*************************************************************
* Method:       Disconnect
* Description:  This function is used to disconnect a target
*               channel that has been connected.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::Disconnect()
{
    ACGL_TRACE(CDPSComManager::Disconnect);
    BOOL l_ret_val = FALSE;
    
    try {
        if(m_dpsconnector != NULL) {
            HRESULT hr = m_dpsconnector->DisConnect();
            
            if(SUCCEEDED(hr)) {
                CACGLLogHelper::WriteToTraceLog
                    (_T("MRI_ACGL"), MRTraceMarker1, _T("Disconnected with target."),
                    _T("CDPSComManager::Disconnect"));
                l_ret_val = TRUE;
            } else {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Failed to disconnect with target."),_T("CDPSComManager::Disconnect"));
                l_ret_val = FALSE;
            }
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::Disconnect"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_ret_val = FALSE;
    }
    
    return l_ret_val;
}

/*************************************************************
* Method:       InitializeListener
* Description:  This function is used to initialize the MSMQ and
*               register callback function "f_func" for receive
*               message.
* Parameter:    CString & f_target
*               Channel name of ACGL application
* Parameter:    MPLUS_MESSAGE_CALLBACK f_func
*               Valid callback function for processing the message.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::InitializeListener(CString& f_target, 
                                        MPLUS_MESSAGE_CALLBACK f_func)
{
    ACGL_TRACE(CDPSComManager::InitializeListener);
    BOOL l_ret_val = FALSE;
    
    try {
        //We need to trim the string on the left and right to remove any
        //trailing/leading white spaces,tabs an new lines. Need to verify
        //if this is really desired by the client.
        f_target.TrimLeft();
        f_target.TrimRight();
        _bstr_t l_target_channel(f_target);
        
        if(m_dpsconnector != NULL) {
            if(l_target_channel != _bstr_t("") && NULL != f_func) {
                HRESULT hr = m_dpsconnector->InitListener
                    (l_target_channel, f_func, (void **)&m_dpsconnector);
                
                if(SUCCEEDED(hr)) {
                    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Initialisation Success."),_T("CDPSComManager::InitializeListener"));
                    l_ret_val = TRUE;
                }
            } else {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Initialisation Failed."),_T("CDPSComManager::InitializeListener"));
                l_ret_val = FALSE;
            }
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::InitializeListener"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_ret_val = FALSE;
    }
    
    return l_ret_val;
}

/*************************************************************
* Method:       DestroyListener
* Description:  This function is used to to kill (destroy) the
*               server(connection).Need to pass an "" string to
*               unlisten on the previously created channel.
* Parameter:    MPLUS_MESSAGE_CALLBACK f_func
*               Valid callback function for processing the message.
* Parameter:    PVOID f_argument
*               User defined parameter that will be passed to the
*               callback function.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::DestroyListener(MPLUS_MESSAGE_CALLBACK f_func, PVOID f_argument)
{
    ACGL_TRACE(CDPSComManager::DestroyListener);
    BOOL l_ret_val = FALSE;
    
    try {
        if(m_dpsconnector != NULL) {
            HRESULT hr = m_dpsconnector->InitListener("", f_func, f_argument);
            
            if(SUCCEEDED(hr)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Destroy Listener Suceess."),_T("CDPSComManager::DestroyListener"));
                l_ret_val = TRUE;
            } else {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Destroy Listener Failed."),_T("CDPSComManager::DestroyListener"));
                l_ret_val = FALSE;
            }
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::DestroyListener"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_ret_val = FALSE;
    }    
    
    return l_ret_val;
}

/*************************************************************
* Method:       RecvCallBack
* Description:  This is a Callback function which is called when
*               message is received on the channel.
*               This function is used for processing the
*               message received.
* Parameter:    IMplusMessage * messageParam
* Parameter:    IMplusMessage * messageRet
* Parameter:    PVOID pCustom
* Returns:      BOOL
*************************************************************/
bool gl_tool_hide = false;
BOOL CDPSComManager::RecvCallBack(IMplusMessage* messageParam,
                                  IMplusMessage* messageRet,
                                  PVOID pCustom)
{
    ACGL_TRACE(CDPSComManager::RecvCallBack);
    BOOL l_result = TRUE;
    //AfxMessageBox(_T("in recv call back"));
    if(NULL == messageParam || NULL == messageRet || NULL == pCustom) {
        return FALSE;
    }
    
    try {
        _bstr_t l_bstr_sender = messageParam->GetFromAddress();
        _bstr_t l_str_command = messageParam->GetCommand();
        _bstr_t l_strparam = messageParam->GetParameter();
        _bstr_t l_custom = messageParam->GetCustom();
        CString l_csmsg = __T("Recieved Message: ");
        l_csmsg += (LPCTSTR)(l_str_command);
        IMplusMessage* l_msg = NULL;
        CString l_sender;
        l_sender.Format(_T("%s"), (LPCTSTR)l_bstr_sender);
        CDPSComManager *l_server = NULL;
        l_server = GetInstance();
        
        if(NULL == l_server) {
            return FALSE;
        }
        
        Uid = messageParam->GetUID();
        
        if(l_str_command == _bstr_t(_T("FW_CheckState"))) {
            l_msg = l_server->CreateMessage(RE_FW_CheckState, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_FW_CheckState."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("FW_Close"))) {
            //Here we need to send msg to view class to close the dialog
            if(g_frame == NULL) {
                theApp.m_pMainWnd->PostMessage(WM_CLOSE);
            } else {
                g_MainView->PostMessage(WM_CMD_CLOSE, 0, (LPARAM)(LPCTSTR)l_bstr_sender);
            }
            
            l_server->SetACGLStatus(1/*STATE_READY*/);
            l_msg = l_server->CreateMessage(RE_FW_Close, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_FW_Close."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("FW_Show"))) {
        /*if(g_frame == NULL) {
        theApp.m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
        } else {*/
            //AfxMessageBox(_T("FW_Show recvd"));
            g_MainView->PostMessage(WM_CMD_SHOW, WPARAM(0), (LPARAM)(LPCTSTR)l_bstr_sender); 
            //   }
            
            //  l_server->SetACGLStatus(STATE_ACTIVE);
            l_msg = l_server->CreateMessage(RE_FW_Show, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_FW_Show."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("FW_Hide"))) {
            //AfxMessageBox(_T("FW_Hide recvd"));
            if(g_frame == NULL) {
                //                theApp.m_pMainWnd->ShowWindow(SW_HIDE);
            } else {
				MessageBox(NULL,_T("FW_Hide"),NULL,NULL);
                g_MainView->PostMessage(WM_CMD_HIDE, 0, (LPARAM)(LPCTSTR)l_bstr_sender);
            }
            
//            l_server->SetACGLStatus(STATE_INACTIVE);
            l_msg = l_server->CreateMessage(RE_FW_Hide, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_FW_Hide."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("RE_FW_ReportState"))) {
            //
        } else if(l_str_command == _bstr_t(_T("RE_FW_RequestHideGL"))) {
            //AfxMessageBox(_T("RE_FW_RequestHideGL recvd"));
            //
        } else if(l_str_command == _bstr_t(_T("GL_Tool_Show"))) {

//            AfxMessageBox(_T("GL_Tool_Show recvd"));
			if (ACGAppController::GetInstance()->isExitClcked)
			{
				ACGAppController::GetInstance()->CloseTool();  //AN/defect fix :if mtb button is clicked while tool is running,then need to click twice close button
			} else {
				gl_tool_hide = false;
            int init_flag = 0;
            waiting = true;
            BSTR l_csstatus =l_server->GetACGLStatusString();
            if(0 == CString(l_csstatus).CompareNoCase(_T("STATE_INACTIVE"))) {
                init_flag = 1;
            } else {
                init_flag = 0;
            }
            if (l_csstatus) {
                ::SysFreeString(l_csstatus);
                l_csstatus = NULL;
            }
            if(g_frame == NULL) {
                //theApp.m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
            } else {
                g_MainView->PostMessage(WM_CMD_SHOW, (WPARAM)(init_flag) , (LPARAM)(LPCTSTR)l_bstr_sender);
            }
            
            //            l_server->SetACGLpStatus(STATE_ACTIVE);
            l_msg = l_server->CreateMessage(RE_GL_Tool_Show, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_GL_Tool_Show."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
			ACGAppController::GetInstance()->QueryActiveDisplayPage();
			}
        } else if(l_str_command == _bstr_t(_T("GL_Tool_Hide"))) {
			if (ACGAppController::GetInstance()->isExitClcked)ACGAppController::GetInstance()->CloseTool(); else {

//            AfxMessageBox(_T("GL_Tool_Hide recvd"));
			waiting = true;
			gl_tool_hide = true;
            if(g_frame == NULL) {
                //                theApp.m_pMainWnd->ShowWindow(SW_HIDE);
            } else {
               // g_MainView->PostMessage(WM_CMD_HIDE, 0, (LPARAM)(LPCTSTR)l_bstr_sender);
				HWND pwd  = NULL;
				if ((CAdvCardiacGLView::GetView())->m_maindlg)
					pwd = (CAdvCardiacGLView::GetView())->m_maindlg->GetSafeHwnd();
//                ACGAppController::GetInstance()->ChangeWindowZOrder(pwd);                                                
            }
            
            //            l_server->SetACGLStatus(STATE_INACTIVE);
            l_msg = l_server->CreateMessage(RE_GL_Tool_Hide, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_GL_Tool_Hide."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
            ACGAppController::GetInstance()->QueryActiveDisplayPage();
			}
        } else if(_bstr_t(_T("CFW_OBSERVE")) == l_str_command) {
            //             ROI_CMD* stCmd = new ROI_CMD;
            //             CString csParam = CString(static_cast<LPCTSTR>(messageParam->GetParameter()));
            //             CString csObservedModel = csParam;
            //             int nPos = csObservedModel.ReverseFind(_T(':'));
            //             
            //             if((nPos < 0) || (nPos >= csObservedModel.GetLength())) {
            //                 return false;
            //             }
            //             
            //             CString csObservedModelName =
            //                 csObservedModel.Right(csObservedModel.GetLength()
            //                 - nPos - 1);
            //             CString csRoiStrokeModelName(_T("Roi Stroke Model"));
            //             csRoiStrokeModelName = csObservedModelName.Left(
            //                 csRoiStrokeModelName.GetLength());
            //             
            //             // respond to stroke model notify message
            //             if(0 == csRoiStrokeModelName.Compare(_T("Roi Stroke Model"))) {
            //                 g_MainView->PostMessage(WM_CMD_DRAW_ROI, NULL, (LPARAM) stCmd);
            //             }
        } else if(l_str_command == _bstr_t(_T("RE_FW_RegisterMsg"))) {
            OutputDebugString(_T("Registered_Iselector"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
        } else if(l_str_command == _bstr_t(_T("RE_FW_UnRegisterMsg"))) {
            OutputDebugString(_T("UnRegistered_Iselector"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
        } else if(l_str_command == _bstr_t(_T("RE_FW_EnableButton"))) {
            OutputDebugString(_T("received RE_FW_EnableButton"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
        } else if (l_str_command == _bstr_t(_T("RE_FW_SubscribeMsg"))) {
            
        } else if (l_str_command == _bstr_t(_T("RE_FW_SwitchSceneBranch"))) {
            //AfxMessageBox(_T("RE_FW_SwitchSceneBranch recvd"));
            ACGAppController::GetInstance()->InvokeApplication();   
        } else if (l_str_command == _bstr_t(_T("RE_FW_QueryCurrentSceneBranch"))) {
            //AfxMessageBox(_T("RE_FW_QueryCurrentSceneBranch recvd"));
            //get current scene
            CString l_csCurrentPageScene(_T(""));
            l_csCurrentPageScene = (LPCTSTR)l_strparam;
            
            //show or hide tools in current scene
            if (waiting ) {
                ACGAppController::GetInstance()->ShowHideCardiacToolsByScene(l_csCurrentPageScene);
            } else {
                
            }
        } else if (l_str_command == _bstr_t(_T("FW_RequestClose"))) {
			IMplusMessage *l_msg = NEW_MSG();;
			l_msg->SetCommand(_T("FW_RequestClose"));
			l_msg->SetParameter(l_strparam);
			l_msg->SetCustom(_T(""));
			CString sender = l_server->GetParentName();;
			l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));  
			l_server->Connect(sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Fail to sent message RE_GL_Tool_Hide."),                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
			DEL_MSG(l_msg);
			CString toolname = CString(static_cast<LPCTSTR>(l_strparam));
			ACGAppController::GetInstance()->UpdatedInvokedToolList(toolname);
		} else if (l_str_command == _bstr_t(_T("RE_FW_QueryActivePage"))) {
				CString l_csCurrentPage(_T(""));
				CString parentpage = l_server->GetParentName();;
            l_csCurrentPage = (LPCTSTR)l_strparam;
			if (l_csCurrentPage.CompareNoCase(parentpage) == 0) 
			{
				ACGAppController::GetInstance()->QueryActivePage();

			} else {
				g_MainView->PostMessage(WM_CMD_HIDE, (WPARAM)(NULL) , (LPARAM)(LPCTSTR)l_bstr_sender);
			}
		}
        
        if(l_msg) {
            DEL_MSG(l_msg);
            l_msg = NULL;
        }
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::RecvCallBack"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_result = FALSE;
    }
    
    return l_result;
}

/*************************************************************
* Method:       SetParentName
* Description:  This function is used to set the parent channel
*               name of the ACGL Application.
* Parameter:    BSTR f_parent_name
*               Parent channel name which invokes ACGL.
* Returns:      void
*************************************************************/
void CDPSComManager::SetParentName(BSTR f_parent_name)
{
    ACGL_TRACE(CDPSComManager::SetParentName);
    try{
        m_parent_name = SysAllocString(f_parent_name);
        CString child_name ;
        child_name.LoadString(AFX_IDS_APP_TITLE);
        m_channel_name = CString(m_parent_name) + "." + child_name;
//        AfxMessageBox(m_channel_name);;
        
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::SetParentName"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       GetParentName
* Description:  This function returns the parent channel name
*               of the ACGL Application.
* Returns:      BSTR
*               Parent channel name which invokes ACGL.
*************************************************************/
BSTR CDPSComManager::GetParentName()
{
    ACGL_TRACE(CDPSComManager::GetParentName);
    return m_parent_name;
}

/*************************************************************
* Method:       SetACGLStatus
* Description:  This function is used to set status of the
*               ACGL Application.
* Parameter:    int f_status
*               Status of ACGL Application.
* Returns:      void
*************************************************************/
void CDPSComManager::SetACGLStatus(int f_status)
{
    ACGL_TRACE(CDPSComManager::SetACGLStatus);
    m_status = f_status;
}

/*************************************************************
* Method:       GetACGLStatusString
* Description:  This function returns the status of the ACGL
*               Application.
* Returns:      BSTR
*               Status of ACGL Application.
*************************************************************/
BSTR CDPSComManager::GetACGLStatusString()
{
    ACGL_TRACE(CDPSComManager::GetACGLStatusString);
    
    try{
        BSTR status_string = L"";
        
        //         if(m_status == STATE_READY) {
        //             status_string = SysAllocString(L"STATE_READY");
        //         } else if(m_status == STATE_ACTIVE) {
        //             status_string = SysAllocString(L"STATE_ACTIVE");
        //         } else if(m_status == STATE_INACTIVE) {
        //             status_string = SysAllocString(L"STATE_INACTIVE");
        //         }
        
        return status_string;
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::GetACGLStatusString"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        return NULL;
    }
}

/*************************************************************
* Method:       InitializeServer
* Description:  This function is used to initialize the MSMQ
*               and register callback function for receive message.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::InitializeServer()
{
        ACGL_TRACE(CDPSComManager::InitializeServer);
    
    BOOL l_status = FALSE;
    
    try{
        
        if(Initialize()) {
            if(InitializeListener
                (m_channel_name, (MPLUS_MESSAGE_CALLBACK) CDPSComManager::RecvCallBack)) {
                CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, _T("Server Initialiased."),                    _T("CDPSComManager::InitializeServer"));
                l_status = TRUE;
            }
        }
        
        return l_status;
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::InitializeServer"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        l_status = FALSE;
    }
    
    return l_status;
}

/*************************************************************
* Method:       CreateMessage
* Description:  This function is used to Constructs message
*               using acgl_message_t.
* Parameter:    acgl_message_t f_msg_id
*               Message ID.
* Parameter:    int f_status
*               Status of message.
* Parameter:    BSTR f_bstrSender
*               Channel name of the sender of message.
* Parameter:    BSTR f_parameter
*               Parameter.
* Parameter:    BSTR f_custom
*               Custom.
* Returns:      IMplusMessage*
*               Pointer to the message created
*************************************************************/
IMplusMessage* CDPSComManager::CreateMessage(
                                             acgl_message_t f_msg_id,
                                             int f_status,
                                             BSTR f_bstrSender,
                                             BSTR f_parameter,
                                             BSTR f_custom)
                                             
{
    UNREFERENCED_PARAMETER(f_bstrSender);
    UNREFERENCED_PARAMETER(f_status);

    ACGL_TRACE(CDPSComManager::CreateMessage);
    try{
        IMplusMessage *l_msg = NEW_MSG();
        
        CString l_csmsg(_T(""));
        
        if(NULL == l_msg) {
            return NULL;
        }
        if (f_msg_id == FW_LOAD_GL)
        {
            l_csmsg = _T("FW_LoadGL");
            l_msg->SetCommand(_bstr_t(_T("FW_LoadGL")));
            l_msg->SetCustom(f_custom);
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(f_parameter);
            // l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_ReportState) {
            l_msg->SetCommand(_bstr_t(_T("FW_ReportState")));
            l_csmsg = _T("FW_ReportState");
            DWORD dwProcessId = GetCurrentProcessId();
            CString csProcessId = _T("");
            csProcessId.Format(_T("%ld"), dwProcessId);
            l_msg->SetCustom(_bstr_t(csProcessId));
            l_msg->SetSender(_bstr_t(m_channel_name));
            BSTR tmpstatus = NULL;
            tmpstatus = GetACGLStatusString();
            l_msg->SetParameter(_bstr_t(tmpstatus));
            if (tmpstatus) {::SysFreeString(tmpstatus);}
        } else if(f_msg_id == RE_FW_CheckState) {
            l_csmsg = _T("RE_FW_CheckState");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_CheckState")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            BSTR tmpstatus = NULL;
            tmpstatus = GetACGLStatusString();
            l_msg->SetParameter(_bstr_t(_bstr_t(tmpstatus)));
            if (tmpstatus) {::SysFreeString(tmpstatus);}
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == RE_FW_Close) {
            l_csmsg = _T("RE_FW_Close");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_Close")));
            l_msg->SetCustom(_T(""));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == RE_FW_Show) {
            l_csmsg = _T("RE_FW_Show");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_Show")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == RE_GL_Tool_Show) {
            l_csmsg = _T("RE_GL_Tool_Show");
            l_msg->SetCommand(_bstr_t(_T("RE_GL_Tool_Show")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == RE_FW_Hide) {
            l_csmsg = _T("RE_FW_Hide");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_Hide")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == RE_GL_Tool_Hide) {
            l_csmsg = _T("RE_GL_Tool_Hide");
            l_msg->SetCommand(_bstr_t(_T("RE_GL_Tool_Hide")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == FW_RequestClose) {
            l_csmsg = _T("FW_RequestClose");
            CString app_name;
            app_name.LoadString(AFX_IDS_APP_TITLE);
            l_msg->SetCommand(_bstr_t(_T("FW_RequestClose")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(app_name));
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == FW_RequestHideGL) {
            l_csmsg = _T("FW_RequestHideGL");
            l_msg->SetCommand(_bstr_t(_T("FW_RequestHideGL")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(f_parameter);
            OutputDebugString(f_parameter);
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == FW_RequestShowGL) {
            l_csmsg = _T("FW_Show");
            l_msg->SetCommand(_bstr_t(_T("FW_Show"))); //Time being solution ..
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(f_parameter);
            OutputDebugString(f_parameter);
            l_msg->SetReUID(Uid);     //DPS-6
        } else if(f_msg_id == FW_RegisterWindowZOrder) {
            l_csmsg = _T("FW_RegisterWindowZOrder");
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetCommand(_bstr_t(_T("FW_RegisterWindowZOrder")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_ChangeWindowZOrder) {
            l_csmsg = _T("FW_ChangeWindowZOrder");
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetCommand(_bstr_t(_T("FW_ChangeWindowZOrder")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_AddZOrderLayer) {   
            l_csmsg = _T("FW_AddZOrderLayer");
            l_msg->SetCommand(_bstr_t(_T("FW_AddZOrderLayer")));
            l_msg->SetParameter(f_parameter);  //6633 test value
            l_msg->SetReUID(Uid);
        }else if(f_msg_id == FW_UnregisterWindowZOrder) {   
            l_csmsg = _T("FW_UnregisterWindowZOrder");
            l_msg->SetCommand(_bstr_t(_T("FW_UnregisterWindowZOrder")));
            l_msg->SetParameter(f_parameter);  //6633 test value
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_RegisterMsg) {
            CString l_parameter ;
            l_csmsg = _T("FW_RegisterMsg");
            l_parameter.LoadString(AFX_IDS_APP_TITLE);
            l_parameter += _T("/ToolBox/Film");
            l_msg->SetCommand(_bstr_t(_T("FW_RegisterMsg")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(l_parameter));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_UnRegisterMsg) {
            CString l_parameter ;
            l_csmsg = _T("FW_UnRegisterMsg");
            l_parameter.LoadString(AFX_IDS_APP_TITLE);
            l_parameter += _T("/Film");
            l_msg->SetCommand(_bstr_t(_T("FW_UnRegisterMsg")));
            l_msg->SetCustom(_bstr_t(_T("")));
            l_msg->SetSender(_bstr_t(m_channel_name));
            l_msg->SetParameter(_bstr_t(l_parameter));
            l_msg->SetReUID(Uid);
        }  else if(f_msg_id == FW_Disable_Process_Btn) {
            l_csmsg = _T("FW_EnableButton");
            l_msg->SetCommand(_bstr_t(_T("FW_EnableButton")));
            l_msg->SetParameter(f_parameter);
            l_msg->SetCustom(f_custom);
            l_msg->SetReUID(Uid);
        }
        
        
    //    CACGLLogHelper::WriteToTraceLog(_T("MRI_ACGL"), MRTraceMarker1, l_csmsg, _T("CDPSComManager::CreateMessage"));
        return l_msg;
    } catch(...) {
        CString l_csStr(_T("Exception occured in CDPSComManager::CreateMessage"));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        return NULL;
    }
}

/*************************************************************
* Method:       DeleteInstance
* Description:  This function is used to release the allocated
*               memory of the instance of CDPSComManager class.
* Returns:      void
*************************************************************/
void CDPSComManager::DeleteInstance()
{
    ACGL_TRACE(CDPSComManager::DeleteInstance);
    
    if(m_dps_com_manager) {
        delete m_dps_com_manager;
        m_dps_com_manager = NULL;
    }
}

/*************************************************************
* Method:       DestroyServer
* Description:  This function is used to destroy the channel
*               of communication from ACGL to Pager.
*               It overrides this function from the base class.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::DestroyServer()
{
    ACGL_TRACE(CDPSComManager::DestroyServer);
    BOOL l_status = FALSE;
    
    if(DestroyListener((MPLUS_MESSAGE_CALLBACK) CDPSComManager::RecvCallBack, (void **)&m_dpsconnector)) {
        if(Terminate()) {
            l_status = TRUE;
        }
    }
    
    return l_status;
}

/*************************************************************
* Method:       GetChannelName
* Description:  This function returns the channel name of the
*               ACGL Application.
* Returns:      CString
*               Channel name of ACGL Application.
*************************************************************/
CString CDPSComManager::GetChannelName()
{
    ACGL_TRACE(CDPSComManager::GetChannelName);
    return m_channel_name;
}
///////////////////////////////// END OF FILE /////////////////////////////////