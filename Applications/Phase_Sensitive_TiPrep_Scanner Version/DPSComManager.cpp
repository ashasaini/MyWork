/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "DPSComManager.h"
#include "IpFrame.h"
#include "TiPrepView.h"
#include "TiPrep.h"
#include "ICMMgr.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepController.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDPSComManager* CDPSComManager::m_dps_com_manager = NULL;
IMplusConnector* CDPSComManager::m_dpsconnector = NULL;

/**
* Pointer to CInPlaceFrame class.
*/
extern CInPlaceFrame *g_frame;

/**
* Pointer to CTiPrepView class.
*/
extern CTiPrepView *g_MainView;

/**
* Pointer to CTiPrepApp class.
*/
extern CTiPrepApp theApp;

/**
* Parameter used for CFW communication.
*/
unsigned _int64 Uid = 0;

const CString EXCEPTION_DPSCOMMANAGER = L"Exception occurred in DPSCommanager";

/**
* Constructor.
* This function is used for initialization of member variables.
*/
CDPSComManager::CDPSComManager() : m_parent_name(NULL)
{
    m_channel_name.Empty();
    m_status = STATE_INACTIVE;
    m_dpsconnector = NULL;
    m_dps_com_manager = NULL;
	m_branchboss = _T("");
}

/**
* Destructor.
* This function is used for releasing the allocated memory.
*/
CDPSComManager::~CDPSComManager()
{
    /*TIPREP_TRACE(CDPSComManager::~CDPSComManager);*/
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
    TIPREP_TRACE(CDPSComManager::Initialize,MRTraceMarker3);
    
    try{
        
        if(NULL == m_dpsconnector) {
            m_dpsconnector = NEW_CONNECTOR();
        }
        
        if(NULL == m_dpsconnector) {
            CTiPrepLogHelper::WriteToTraceLog
                (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to create Connector."),
                _T("CDPSComManager::Initialize"));
            return FALSE;
        } else {
            CTiPrepLogHelper::WriteToTraceLog
                (_T("MRI_TIPREP"), MRTraceMarker1, _T("Connector created."),
                _T("CDPSComManager::Initialize"));
            return TRUE;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
    /*TIPREP_TRACE(CDPSComManager::GetInstance);*/
    
    try{
        if(NULL == m_dps_com_manager) {
            m_dps_com_manager = new CDPSComManager();
            
            if(NULL == m_dps_com_manager) {
                CString l_cserr = _T("Memory allocation failed.");
                CTiPrepLogHelper::WriteToErrorLog
                    (l_cserr, __LINE__, __FILE__);
            }
        }
        
        return m_dps_com_manager;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
    TIPREP_TRACE(CDPSComManager::Terminate,MRTraceMarker3);
    BOOL bret = TRUE;
    
    try {
        if(m_dpsconnector != NULL) {
            DEL_CONNECTOR((IMplusConnector*)m_dpsconnector);
            m_dpsconnector = NULL;
            CTiPrepLogHelper::WriteToTraceLog
                (_T("MRI_TIPREP"), MRTraceMarker1, _T("Connector deleted."),
                _T("CDPSComManager::Terminate"));
        }
        
        SysFreeString(m_parent_name);
        m_parent_name = NULL;
    } catch(...) {
        throw;
        bret = FALSE;
        CTiPrepLogHelper::WriteToTraceLog
            (_T("MRI_TIPREP"), MRTraceMarker1, _T("Failed to delete connector."),
            _T("CDPSComManager::Terminate"));
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
    TIPREP_TRACE(CDPSComManager::Connect,MRTraceMarker3);
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
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker1, _T("Connection with target establish."),
                        _T("CDPSComManager::Connect"));
                } else {
                    l_ret_val = FALSE;
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker1, _T("Failed to connect with target."),
                        _T("CDPSComManager::Connect"));
                }
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred during DPSComm Connect"), __LINE__, __FILE__);
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
    TIPREP_TRACE(CDPSComManager::SendMsg,MRTraceMarker3);
    BOOL l_ret_val = FALSE;
    IMplusMessage *returnmsg = NEW_MSG();
    try {
        //Send the message asynchronously. This can be changed
        //later on if required to send synchronously.
        if(NULL != f_message_send && f_message_send->GetCommand() != _bstr_t("")) {
            if(m_dpsconnector != NULL) {
				  HRESULT hr = m_dpsconnector->SendASync(f_message_send);
              //  HRESULT hr = m_dpsconnector->SendSync(f_message_send,returnmsg);
                
                if(SUCCEEDED(hr)) {
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker1, _T("Message Sent."),
                        _T("CDPSComManager::SendMsg"));
                    l_ret_val = TRUE;
                } else {
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker1, _T("Failed to sent message."),
                        _T("CDPSComManager::SendMsg"));
                    l_ret_val = FALSE;
                }
            }
        }
	//	RecvCallBack(returnmsg,f_message_send,NULL);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
    TIPREP_TRACE(CDPSComManager::Disconnect,MRTraceMarker3);
    BOOL l_ret_val = FALSE;
    
    try {
        if(m_dpsconnector != NULL) {
            HRESULT hr = m_dpsconnector->DisConnect();
            
            if(SUCCEEDED(hr)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Disconnected with target."),
                    _T("CDPSComManager::Disconnect"));
                l_ret_val = TRUE;
            } else {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Failed to disconnect with target."),
                    _T("CDPSComManager::Disconnect"));
                l_ret_val = FALSE;
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
*               Channel name of TiPrep application
* Parameter:    MPLUS_MESSAGE_CALLBACK f_func
*               Valid callback function for processing the message.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::InitializeListener(CString& f_target, 
                                        MPLUS_MESSAGE_CALLBACK f_func)
{
    TIPREP_TRACE(CDPSComManager::InitializeListener,MRTraceMarker3);
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
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker1, _T("Initialisation Success."),
                        _T("CDPSComManager::InitializeListener"));
                    l_ret_val = TRUE;
                }
            } else {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Initialisation Failed."),
                    _T("CDPSComManager::InitializeListener"));
                l_ret_val = FALSE;
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
    TIPREP_TRACE(CDPSComManager::DestroyListener,MRTraceMarker3);
    BOOL l_ret_val = FALSE;
    
    try {
        if(m_dpsconnector != NULL) {
            HRESULT hr = m_dpsconnector->InitListener("", f_func, f_argument);
            
            if(SUCCEEDED(hr)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Destroy Listener Suceess."),
                    _T("CDPSComManager::DestroyListener"));
                l_ret_val = TRUE;
            } else {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Destroy Listener Failed."),
                    _T("CDPSComManager::DestroyListener"));
                l_ret_val = FALSE;
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
BOOL CDPSComManager::RecvCallBack(IMplusMessage* messageParam,
                                  IMplusMessage* messageRet,
                                  PVOID pCustom)
{
    TIPREP_TRACE(CDPSComManager::RecvCallBack,MRTraceMarker3);
    BOOL l_result = TRUE;
    
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
        CTiPrepLogHelper::WriteToTraceLog
            (_T("MRI_TIPREP"), MRTraceMarker1, l_csmsg, _T("CDPSComManager::RecvCallBack"));
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
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to sent message RE_FW_CheckState."),
                    _T("CDPSComManager::RecvCallBack"));
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
            
            l_server->SetTiPrepStatus(STATE_READY);
            l_msg = l_server->CreateMessage(RE_FW_Close, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to sent message RE_FW_Close."),
                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("FW_Show"))) {
            if(g_frame == NULL) {
                theApp.m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
            } else {
                g_MainView->PostMessage
                    (WM_CMD_SHOW, WPARAM(0), (LPARAM)(LPCTSTR)l_bstr_sender); 
            }
            
            l_server->SetTiPrepStatus(STATE_ACTIVE);
            l_msg = l_server->CreateMessage(RE_FW_Show, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to sent message RE_FW_Show."),
                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("FW_Hide"))) {
            if(g_frame == NULL) {
                theApp.m_pMainWnd->ShowWindow(SW_HIDE);
            } else {
                g_MainView->PostMessage(WM_CMD_HIDE, 0, (LPARAM)(LPCTSTR)l_bstr_sender);
            }
            
            l_server->SetTiPrepStatus(STATE_INACTIVE);
            l_msg = l_server->CreateMessage(RE_FW_Hide, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to sent message RE_FW_Hide."),
                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("RE_FW_ReportState"))) {
            //
        } else if(l_str_command == _bstr_t(_T("RE_FW_RequestHideGL"))) {
            //
        } else if(l_str_command == _bstr_t(_T("GL_Tool_Show"))) {
            int init_flag = 0;
            BSTR l_csstatus =l_server->GetTiPrepStatusString();
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
                theApp.m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
            } else {
                g_MainView->PostMessage(WM_CMD_SHOW, (WPARAM)(init_flag) , (LPARAM)(LPCTSTR)l_bstr_sender);
            }
            
            l_server->SetTiPrepStatus(STATE_ACTIVE);
            l_msg = l_server->CreateMessage(RE_GL_Tool_Show, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to sent message RE_GL_Tool_Show."),
                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(l_str_command == _bstr_t(_T("GL_Tool_Hide"))) {
            if(g_frame == NULL) {
                theApp.m_pMainWnd->ShowWindow(SW_HIDE);
            } else {
                g_MainView->PostMessage(WM_CMD_HIDE, 0, (LPARAM)(LPCTSTR)l_bstr_sender);
            }
            
            l_server->SetTiPrepStatus(STATE_INACTIVE);
            l_msg = l_server->CreateMessage(RE_GL_Tool_Hide, 1, l_bstr_sender);
            l_server->Connect(l_sender);
            
            if(!l_server->SendMsg(l_msg)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Fail to sent message RE_GL_Tool_Hide."),
                    _T("CDPSComManager::RecvCallBack"));
                l_result = FALSE;
            }
            
            l_server->Disconnect();
        } else if(_bstr_t(_T("CFW_OBSERVE")) == l_str_command) {
            ROI_CMD* stCmd = new ROI_CMD;
            CString csParam = CString(static_cast<LPCTSTR>(messageParam->GetParameter()));
            CString csObservedModel = csParam;
            int nPos = csObservedModel.ReverseFind(_T(':'));
            
            if((nPos < 0) || (nPos >= csObservedModel.GetLength())) {
                return false;
            }
            
            CString csObservedModelName =
                csObservedModel.Right(csObservedModel.GetLength()
                - nPos - 1);
            CString csRoiStrokeModelName(_T("Roi Stroke Model"));
            csRoiStrokeModelName = csObservedModelName.Left(
                csRoiStrokeModelName.GetLength());
            
            // respond to stroke model notify message
            if(0 == csRoiStrokeModelName.Compare(_T("Roi Stroke Model"))) {
                g_MainView->PostMessage(WM_CMD_DRAW_ROI, NULL, (LPARAM) stCmd);
            }
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
        } else if(l_str_command == _bstr_t(_T("RE_GL_IS_Send_IC_Name"))) {
            CICMMgr l_mgr;
            int b_status = E_FAIL;
            CString csParam = CString(static_cast<LPCTSTR>(l_strparam));
            b_status = l_mgr.GetLOIDSelectedImages(csParam);
            if(E_NO_ERROR != b_status){
                g_MainView->ClearRepeatationTimeTextBox();
            }
            g_MainView->PostMessage(WM_CMD_GET_SELIMG, (WPARAM)(b_status), (LPARAM)(LPCTSTR) l_bstr_sender);     //MPower 043/Sep-2012/AN/Added
            OutputDebugString(_T("Msg Received from IC - Selected Images"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
        } else if(l_str_command == _bstr_t(_T("GL_IS_Send_CONTROLLER_IC_Name"))) {
            int b_status = E_FAIL;
            CICMMgr l_mgr;
            CString csParam = CString(static_cast<LPCTSTR>(l_strparam));
            b_status = l_mgr.GetLOIDSelectedImages(csParam);
            if(E_NO_ERROR != b_status){
                g_MainView->ClearRepeatationTimeTextBox();
                //vilas_31Oct..in case of error I_PROPER_IMG no image is selected
                //so skip further handling. same might be possible for other errors
            }
            g_MainView->PostMessage(WM_CMD_GET_SELIMG, (WPARAM)(b_status), (LPARAM)(LPCTSTR) l_bstr_sender); 
            OutputDebugString(_T("Msg Received from IC"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
        } else if(l_str_command == _bstr_t(_T("RE_FW_EnableButton"))) {
            OutputDebugString(_T("received RE_FW_EnableButton"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
			//AN Scanner
        } else if (l_str_command == _bstr_t(_T("RE_GL_Pager_GetPageInfo"))) {
			//g_MainView->
			CString csParam = CString(static_cast<LPCTSTR>(l_strparam));
			CTiPrepController::GetInstance()->ExtractPagerInfo(csParam);
		} else if (l_str_command == _bstr_t(_T("RE_REGISTER_TIPREP_TO_PQM"))) {
			CString custom = CString(static_cast<LPCTSTR>(l_custom));
			if (custom.CompareNoCase(_T("TRUE")) == 0) {
				m_dps_com_manager->m_branchboss = CString(static_cast<LPCTSTR>(m_dps_com_manager->GetParentName()));
				
			}
			CString param = CString(static_cast<LPCTSTR>(l_strparam));
			if (param.CompareNoCase(_T("S_OK")) == 0)
				CTiPrepController::GetInstance()->isConnectedToPQM = E_CONNECTED;
		} else if ( l_str_command == _bstr_t( _T("PQM_CLOSE_CONN"))) {
			CTiPrepController::GetInstance()->isConnectedToPQM = E_NO_CONNECTION;
			CWnd * wnd = CTiPrepView::GetView()->GetCTiPrepDlgInstance();
			wnd->GetDlgItem(IDC_BUTTON_APPLYTI)->EnableWindow(FALSE);
		} else if ( l_str_command == _bstr_t(_T("RE_CAN_TIPREP_UPDATE"))) {
			CString param = CString(static_cast<LPCTSTR>(l_strparam));
			if (param.CompareNoCase(_T("TRUE")) == 0) {
				CTiPrepController::GetInstance()->UpdateTi(true);
			} else {
				//	CTiPrepController::GetInstance()->UpdateTi(false); // Only to show an error message
			}
			//AN scanner
        }else if ( l_str_command == _bstr_t(_T("REFRESH_TIPREP"))) {
            CTiPrepController::GetInstance()->RefreshApplyTiGUI();
        }
        
        if(l_msg) {
            DEL_MSG(l_msg);
            l_msg = NULL;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
        l_result = FALSE;
    }
    
    return l_result;
}

/*************************************************************
* Method:       SetParentName
* Description:  This function is used to set the parent channel
*               name of the TiPrep Application.
* Parameter:    BSTR f_parent_name
*               Parent channel name which invokes TiPrep.
* Returns:      void
*************************************************************/
void CDPSComManager::SetParentName(BSTR f_parent_name)
{
    TIPREP_TRACE(CDPSComManager::SetParentName,MRTraceMarker3);
    try{
        m_parent_name = SysAllocString(f_parent_name);
        CString child_name ;
        child_name.LoadString(AFX_IDS_APP_TITLE);
        m_channel_name = CString(m_parent_name) + "." + child_name;
		m_branchboss = CString(m_parent_name) + _T(".MPlusClinAppToolBox"); //For invocation from wakeuptool //AN scanner
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       GetParentName
* Description:  This function returns the parent channel name
*               of the TiPrep Application.
* Returns:      BSTR
*               Parent channel name which invokes TiPrep.
*************************************************************/
BSTR CDPSComManager::GetParentName()
{
    TIPREP_TRACE(CDPSComManager::GetParentName,MRTraceMarker8);
    return m_parent_name;
}

/*************************************************************
* Method:       SetTiPrepStatus
* Description:  This function is used to set status of the
*               TiPrep Application.
* Parameter:    int f_status
*               Status of TiPrep Application.
* Returns:      void
*************************************************************/
void CDPSComManager::SetTiPrepStatus(int f_status)
{
    TIPREP_TRACE(CDPSComManager::SetTiPrepStatus,MRTraceMarker8);
    m_status = f_status;
}

/*************************************************************
* Method:       GetTiPrepStatusString
* Description:  This function returns the status of the TiPrep
*               Application.
* Returns:      BSTR
*               Status of TiPrep Application.
*************************************************************/
BSTR CDPSComManager::GetTiPrepStatusString()
{
    TIPREP_TRACE(CDPSComManager::GetTiPrepStatusString,MRTraceMarker3);
    
    try{
        BSTR status_string = L"";
        
        if(m_status == STATE_READY) {
            status_string = SysAllocString(L"STATE_READY");
        } else if(m_status == STATE_ACTIVE) {
            status_string = SysAllocString(L"STATE_ACTIVE");
        } else if(m_status == STATE_INACTIVE) {
            status_string = SysAllocString(L"STATE_INACTIVE");
        }
        
        return status_string;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
    TIPREP_TRACE(CDPSComManager::InitializeServer,MRTraceMarker3);
    
    BOOL l_status = FALSE;
    
    try{
        
        if(Initialize()) {
            if(InitializeListener
                (m_channel_name, (MPLUS_MESSAGE_CALLBACK) CDPSComManager::RecvCallBack)) {
                CTiPrepLogHelper::WriteToTraceLog
                    (_T("MRI_TIPREP"), MRTraceMarker1, _T("Server Initialiased."),
                    _T("CDPSComManager::InitializeServer"));
                l_status = TRUE;
            }
        }
        
        return l_status;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
        l_status = FALSE;
    }
    
    return l_status;
}

/*************************************************************
* Method:       CreateMessage
* Description:  This function is used to Constructs message
*               using tiprep_message_t.
* Parameter:    tiprep_message_t f_msg_id
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
                                             tiprep_message_t f_msg_id,
                                             int f_status,
                                             BSTR f_bstrSender,
                                             BSTR f_parameter,
                                             BSTR f_custom)
                                             
{
    TIPREP_TRACE(CDPSComManager::CreateMessage,MRTraceMarker3);
    try{
        IMplusMessage *l_msg = NEW_MSG();
        
        CString l_csmsg(_T(""));
        
        if(NULL == l_msg) {
            return NULL;
        }
        
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));

        if(f_msg_id == FW_ReportState) {
            l_msg->SetCommand(_bstr_t(_T("FW_ReportState")));
            l_csmsg = _T("FW_ReportState");
            DWORD dwProcessId = GetCurrentProcessId();
            CString csProcessId = _T("");
            csProcessId.Format(_T("%ld"), dwProcessId);
            l_msg->SetCustom(_bstr_t(csProcessId));
            BSTR tmpstatus = NULL;
            tmpstatus = GetTiPrepStatusString();
            l_msg->SetParameter(_bstr_t(tmpstatus));
            if (tmpstatus) {::SysFreeString(tmpstatus);}
        } else if(f_msg_id == RE_FW_CheckState) {
            l_csmsg = _T("RE_FW_CheckState");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_CheckState")));
            BSTR tmpstatus = NULL;
            tmpstatus = GetTiPrepStatusString();
            l_msg->SetParameter(_bstr_t(_bstr_t(tmpstatus)));
             if (tmpstatus) {::SysFreeString(tmpstatus);}
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == RE_FW_Close) {
            l_csmsg = _T("RE_FW_Close");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_Close")));
            l_msg->SetCustom(_T(""));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == RE_FW_Show) {
            l_csmsg = _T("RE_FW_Show");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_Show")));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == RE_GL_Tool_Show) {
            l_csmsg = _T("RE_GL_Tool_Show");
            l_msg->SetCommand(_bstr_t(_T("RE_GL_Tool_Show")));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == RE_FW_Hide) {
            l_csmsg = _T("RE_FW_Hide");
            l_msg->SetCommand(_bstr_t(_T("RE_FW_Hide")));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == RE_GL_Tool_Hide) {
            l_csmsg = _T("RE_GL_Tool_Hide");
            l_msg->SetCommand(_bstr_t(_T("RE_GL_Tool_Hide")));
            l_msg->SetParameter(_bstr_t(_T("S_OK")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_RequestClose) {
            l_csmsg = _T("FW_RequestClose");
            CString app_name;
            app_name.LoadString(AFX_IDS_APP_TITLE);
            l_msg->SetCommand(_bstr_t(_T("FW_RequestClose")));
            l_msg->SetParameter(_bstr_t(app_name));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_RequestHideGL) {
            l_csmsg = _T("FW_RequestHideGL");
            l_msg->SetCommand(_bstr_t(_T("FW_RequestHideGL")));
            l_msg->SetParameter(f_parameter);
            OutputDebugString(f_parameter);
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_RequestShowGL) {
            l_csmsg = _T("FW_Show");
            l_msg->SetCommand(_bstr_t(_T("FW_Show"))); //Time being solution ..
            l_msg->SetParameter(f_parameter);
            OutputDebugString(f_parameter);
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_RegisterWindowZOrder) {
            l_csmsg = _T("FW_RegisterWindowZOrder");
            l_msg->SetCommand(_bstr_t(_T("FW_RegisterWindowZOrder")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_ChangeWindowZOrder) {
            l_csmsg = _T("FW_ChangeWindowZOrder");
            l_msg->SetCommand(_bstr_t(_T("FW_ChangeWindowZOrder")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_RegisterMsg) {
            CString l_parameter ;
            l_csmsg = _T("FW_RegisterMsg");
            l_parameter.LoadString(AFX_IDS_APP_TITLE);
            l_parameter += _T("/ToolBox/Film");
            l_msg->SetCommand(_bstr_t(_T("FW_RegisterMsg")));
            l_msg->SetParameter(_bstr_t(l_parameter));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_UnRegisterMsg) {
            CString l_parameter ;
            l_csmsg = _T("FW_UnRegisterMsg");
            l_parameter.LoadString(AFX_IDS_APP_TITLE);
            l_parameter += _T("/Film");
            l_msg->SetCommand(_bstr_t(_T("FW_UnRegisterMsg")));
            l_msg->SetParameter(_bstr_t(l_parameter));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == GL_IS_Get_Selected_IC_Name) {
            l_csmsg = _T("GL_IS_Get_Selected_IC_Name");
            l_msg->SetCommand(_bstr_t(_T("GL_IS_Get_Selected_IC_Name")));
            l_msg->SetParameter(_bstr_t(_T("")));
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_Disable_Process_Btn) {
            l_csmsg = _T("FW_EnableButton");
            l_msg->SetCommand(_bstr_t(_T("FW_EnableButton")));
            l_msg->SetParameter(f_parameter);
            l_msg->SetCustom(f_custom);
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == GL_IS_Get_Select_Series_IC_Name) {   //Select all images in series
            l_csmsg = _T("GL_IS_Get_Select_Series_IC_Name");  
            l_msg->SetCommand(_bstr_t(_T("GL_IS_Get_Select_Series_IC_Name")));
            l_msg->SetParameter(f_parameter);  //6633 test value
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == GL_IS_Refresh_Image_Matrix) {   //refresh isel
            l_csmsg = _T("GL_IS_Refresh_Image_Matrix");
            l_msg->SetCommand(_bstr_t(_T("GL_IS_Refresh_Image_Matrix")));
            l_msg->SetParameter(_bstr_t(_T("")));  //6633 test value
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_AddZOrderLayer) {   
            l_csmsg = _T("FW_AddZOrderLayer");
            l_msg->SetCommand(_bstr_t(_T("FW_AddZOrderLayer")));
            l_msg->SetParameter(f_parameter);  //6633 test value
            l_msg->SetReUID(Uid);
        } else if(f_msg_id == FW_UnregisterWindowZOrder) {   
            l_csmsg = _T("FW_UnregisterWindowZOrder");
            l_msg->SetCommand(_bstr_t(_T("FW_UnregisterWindowZOrder")));
            l_msg->SetParameter(f_parameter);  //6633 test value
            l_msg->SetReUID(Uid);
        } else {
		//	l_msg->SetCommand(_bstr_t(_T("")));
			DEL_MSG(l_msg);
			return NULL;
		}
        
        CTiPrepLogHelper::WriteToTraceLog
            (_T("MRI_TIPREP"), MRTraceMarker1, l_csmsg, _T("CDPSComManager::CreateMessage"));
        return l_msg;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_DPSCOMMANAGER, __LINE__, __FILE__);
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
    /*TIPREP_TRACE(CDPSComManager::DeleteInstance);*/
    
    if(m_dps_com_manager) {
        delete m_dps_com_manager;
        m_dps_com_manager = NULL;
    }
}

/*************************************************************
* Method:       DestroyServer
* Description:  This function is used to destroy the channel
*               of communication from TiPrep to Pager.
*               It overrides this function from the base class.
* Returns:      BOOL
*************************************************************/
BOOL CDPSComManager::DestroyServer()
{
    TIPREP_TRACE(CDPSComManager::DestroyServer,MRTraceMarker3);
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
*               TiPrep Application.
* Returns:      CString
*               Channel name of TiPrep Application.
*************************************************************/
CString CDPSComManager::GetChannelName()
{
    TIPREP_TRACE(CDPSComManager::GetChannelName,MRTraceMarker8);
    return m_channel_name;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////   