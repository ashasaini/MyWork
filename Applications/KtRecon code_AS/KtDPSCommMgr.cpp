// KtDPSCommMgr.cpp: implementation of the CKtDPSCommMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtDPSCommMgr.h"
#include "IpFrame.h"
#include "KtRecon.h"
#include "KtReconView.h"
#include "KtICMMgr.h"
#include "KtReconLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CKtDPSCommMgr* CKtDPSCommMgr::m_dps_com_manager = NULL;
IMplusConnector* CKtDPSCommMgr::m_dpsconnector = NULL;

extern CInPlaceFrame *g_frame;
extern CKtReconView *g_MainView;
extern CKtReconApp theApp;
int CKtDPSCommMgr ::exit = 0;
unsigned _int64 Uid = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtDPSCommMgr::CKtDPSCommMgr()
{
    
}

CKtDPSCommMgr::~CKtDPSCommMgr()
{
    
}

BOOL CKtDPSCommMgr::Initialize(
                               )
{
    if (NULL == m_dpsconnector) {
        m_dpsconnector = NEW_CONNECTOR();
    }
    if (NULL == m_dpsconnector)
    {   
		CString l_error_string(_T("CKtDPSCommMgr:m_dpsconnector is NULL"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
CKtDPSCommMgr* CKtDPSCommMgr::GetInstance(
                                          )
{
    
    if (NULL == m_dps_com_manager) {
        m_dps_com_manager = new CKtDPSCommMgr();
    }
    
    return m_dps_com_manager;
}

BOOL CKtDPSCommMgr::Terminate(
                              )
{
    
    BOOL bret = TRUE;
    
    try {
        if (m_dpsconnector != NULL) {
            
            DEL_CONNECTOR((IMplusConnector*)m_dpsconnector);
            m_dpsconnector = NULL;
        }
        
    } catch (...) {
        throw;
        bret = FALSE;
    }
    
    return bret;
}
BOOL CKtDPSCommMgr::Connect(
                            CString& f_target)
{
    
    BOOL l_ret_val = FALSE;
    
    try{
        
        //We need to trim the string on the left and right to remove any
        //trailing/leading white spaces,tabs an new lines. 
        f_target.TrimLeft();
        f_target.TrimRight();
        _bstr_t l_target_channel(f_target);
        
        if(m_dpsconnector != NULL)
        {
            if(l_target_channel != _bstr_t(_T("")))
                
            {
                HRESULT hr = m_dpsconnector->Connect(l_target_channel);
                
                if(SUCCEEDED(hr)){
                    l_ret_val = TRUE;
                }
            }
            
        }
        
        
    }catch(...){
		CString l_error_string(_T("Exception occured in Connect"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    return l_ret_val;
}

BOOL CKtDPSCommMgr::SendMsg(
                            IMplusMessage *f_message_send)
                            
{
    BOOL l_ret_val = FALSE;
    try{
        //Send the message asynchronously. This can be changed
        //later on if required to send synchronously.
        if(NULL != f_message_send)
        {
            if(m_dpsconnector != NULL)
            {
                HRESULT hr = m_dpsconnector->SendASync(f_message_send);
                if(SUCCEEDED(hr))
                {
                    
                    l_ret_val = TRUE;
                }
            }
            else
            {
                l_ret_val = FALSE;
            }
            
        }
    }
    catch(...)
    {
		CString l_error_string(_T("Exception occured in SendMsg"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    return l_ret_val;
}

BOOL CKtDPSCommMgr::Disconnect(
                               )
{
    
    BOOL l_ret_val = FALSE;
    try{
        
        if(m_dpsconnector != NULL)
        {
            HRESULT hr = m_dpsconnector->DisConnect();
            if(SUCCEEDED(hr)){
                l_ret_val = TRUE;
            }
        }
        else
        {
            l_ret_val = FALSE;
        }
        
    }
    catch(...)
    {
		CString l_error_string(_T("Exception occured in Disconnect"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    return l_ret_val;
}

BOOL CKtDPSCommMgr::InitializeListener(
                                       CString& f_target, 
                                       MPLUS_MESSAGE_CALLBACK f_func)
                                       
{   
    BOOL l_ret_val = FALSE;
    try{
        //We need to trim the string on the left and right to remove any
        //trailing/leading white spaces,tabs an new lines. Need to verify
        //if this is really desired by the client.
        //        AfxMessageBox(_T("Initializing Listener"));
        f_target.TrimLeft();
        f_target.TrimRight();
        _bstr_t l_target_channel(f_target);
        
        if(m_dpsconnector != NULL)
        {
            if(l_target_channel != _bstr_t("") && NULL != f_func)
            {
                HRESULT hr = m_dpsconnector->InitListener(l_target_channel, f_func, (void **)&m_dpsconnector);
                if(SUCCEEDED(hr)){
                    l_ret_val = TRUE;
                    //                    AfxMessageBox(_T("Initialization Success"));
                }
            }
        }
        else
        {
            l_ret_val = FALSE;
        }
    }catch(...){
		CString l_error_string(_T("Exception occured in InitializeListener"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    return l_ret_val;
}
BOOL CKtDPSCommMgr::DestroyListener(
                                    MPLUS_MESSAGE_CALLBACK f_func, 
                                    PVOID f_argument)
{
    BOOL l_ret_val = FALSE;
    try{
        if(m_dpsconnector != NULL)
        {
            HRESULT hr = m_dpsconnector->InitListener("",f_func,f_argument);
            if(SUCCEEDED(hr)){
                //                AfxMessageBox(_T("Listener Destroyed"));
                l_ret_val = TRUE;
            }
        }
        else
        {
            l_ret_val = FALSE;
        }
    }catch(...){
		CString l_error_string(_T("Exception occured in DestroyListener"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    return l_ret_val;
}
bool gl_tool_hide_recv = false;
BOOL CKtDPSCommMgr::RecvCallBack(
                                 IMplusMessage* messageParam,
                                 IMplusMessage* messageRet,
                                 PVOID pCustom
                                 )
{
    BOOL l_result = TRUE;
    if(NULL == messageParam || NULL == messageRet || NULL == pCustom)
        return FALSE;
    
    try 
    {  
        _bstr_t l_bstr_sender = messageParam->GetFromAddress();
        _bstr_t l_str_command = messageParam->GetCommand();
        _bstr_t l_strparam = messageParam->GetParameter();
        _bstr_t l_custom = messageParam->GetCustom();
        
        IMplusMessage* l_msg = NULL;
        
        CKtDPSCommMgr *l_server = NULL;
        l_server = GetInstance();
        
        if (NULL == l_server)
            return FALSE;
        
        Uid = messageParam->GetUID();
        
        CString l_sparent(_T(""));
        l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
        l_server->Connect(l_sparent);
        
        //        l_server->Connect(CString(l_server->GetParentName()));
        
        if ( l_str_command == _bstr_t(_T("FW_CheckState"))) {
            l_msg = l_server->CreateMessage(RE_FW_CheckState,1,l_bstr_sender); 
            if (!l_server->SendMsg(l_msg) ) {
                l_result = FALSE;
            }
            
        } else if ( l_str_command == _bstr_t(_T("FW_Close"))) {
			  //  AfxMessageBox(_T("FW_Close"));
	//			if (1 != exit) return FALSE;
            //Here we need to send msg to view class to close the dialog
           // if (g_frame == NULL) {
            //    theApp.m_pMainWnd->PostMessage(WM_CLOSE);  
           // } else {
                g_MainView->PostMessage(WM_CMD_CLOSE,0,(LPARAM)(LPCTSTR)l_bstr_sender); 
           // }
            l_server->SetKtReconStatus(STATE_READY);
            l_msg = l_server->CreateMessage(RE_FW_Close,1,l_bstr_sender); 
            if (!l_server->SendMsg(l_msg) ) {
                l_result = FALSE;
            }
            
        } else if ( l_str_command == _bstr_t(_T("FW_Show"))) {
            if (!gl_tool_hide_recv) {
            //Here we need to send msg to view class to show the dialog
            if (g_frame == NULL) {
                theApp.m_pMainWnd->ShowWindow(SW_SHOWNORMAL);                   
            } else {    
                g_MainView->PostMessage(WM_CMD_SHOW,0,(LPARAM)(LPCTSTR)l_bstr_sender);
            }
            l_server->SetKtReconStatus(STATE_ACTIVE);
            
            l_msg = l_server->CreateMessage(RE_FW_Show,1,l_bstr_sender); 
            if (!l_server->SendMsg(l_msg) ) {
                l_result = FALSE;
            }
            }
        } else if ( l_str_command == _bstr_t(_T("FW_Hide"))) {
            
            //Here we need to send msg to view class to hide the dialog
            if (g_frame == NULL) {
                theApp.m_pMainWnd->ShowWindow(SW_HIDE);             
            } else {                        
                g_MainView->PostMessage(WM_CMD_HIDE,0,(LPARAM)(LPCTSTR)l_bstr_sender); 
            }
            
            l_server->SetKtReconStatus(STATE_INACTIVE);
            
            l_msg = l_server->CreateMessage(RE_FW_Hide,1,l_bstr_sender); 
            if (!l_server->SendMsg(l_msg) ) {
                l_result = FALSE;
            }
            
        } else if ( l_str_command == _bstr_t(_T("RE_FW_ReportState"))) {
            
        } else if ( l_str_command == _bstr_t(_T("RE_FW_RequestHideGL"))) {
            
        }/*else if(l_str_command == _bstr_t(_T("RE_GL_IS_Send_IC_Name"))) {

            CString csParam = CString(static_cast<LPCTSTR>(l_strparam));
            //pending
            //inform controller that msg recvd from isel
            //pending : this needs to be changed as per the design
            //currently controller is accessed via view and dlg
            (CKtReconView::GetView())->m_KtReconController->OnRecvMsgFromIsel(csParam);
            
            OutputDebugString(_T("Msg Received from IC - Selected Images"));
            OutputDebugString(l_strparam);
            OutputDebugString(l_custom);
            OutputDebugString(l_bstr_sender);
		} */else if (l_str_command == _bstr_t(_T("RE_GL_Pager_GetPageInfo"))) {
			 CString csParam = CString(static_cast<LPCTSTR>(l_strparam));
				int index = csParam.Find(CString(m_dps_com_manager->m_parent_name));
				CString tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(" "));
				tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				CString studyloid = tempstr.Right(tempstr.GetLength() - index - 1);
			 (CKtReconView::GetView())->m_KtReconController->SetStudyFileLoid(studyloid);
			 
			 
		}else if ( l_str_command == _bstr_t(_T("GL_Tool_Hide"))) {
			gl_tool_hide_recv = true;
			g_MainView->PostMessage(WM_CMD_HIDE,0,(LPARAM)(LPCTSTR)l_bstr_sender); 
		} else if ( l_str_command == _bstr_t(_T("GL_Tool_Show"))) {
			gl_tool_hide_recv = false;
			g_MainView->PostMessage(WM_CMD_SHOW,0,(LPARAM)(LPCTSTR)l_bstr_sender); 
		}
        
        if (l_msg) 
        {
            DEL_MSG(l_msg);
            l_msg = NULL;
        }
        CKtDPSCommMgr::GetInstance()->Disconnect();
    }
    catch(...) {
		CString l_error_string(_T("Exception occured in RecvCallBack"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return FALSE;
    }
    
    return l_result;
}

void CKtDPSCommMgr::SetParentName(BSTR f_parent_name)
{
    
    m_parent_name = SysAllocString(f_parent_name);
    
    //m_parent_name.Format(_T("%s"), (LPCTSTR)f_parent_name);
    CString child_name ;
    
    child_name.LoadString(AFX_IDS_APP_TITLE);
    
    m_channel_name = CString(m_parent_name) + "." + child_name;
}

BSTR CKtDPSCommMgr::GetParentName()
{
    return m_parent_name;
}

void CKtDPSCommMgr::SetKtReconStatus(int f_status)
{
    m_status = f_status;
}

BSTR CKtDPSCommMgr::GetKtReconStatusString() 
{
    
    BSTR status_string = L"";
    
    if ( m_status == STATE_READY) {
        status_string = SysAllocString(L"STATE_READY");
    } else if( m_status == STATE_ACTIVE ) {
        status_string = SysAllocString(L"STATE_ACTIVE");
    } else if( m_status == STATE_INACTIVE) {
        status_string = SysAllocString(L"STATE_INACTIVE");
    }
    
    return status_string;
}

BOOL CKtDPSCommMgr::InitializeServer()
{
    BOOL l_status = FALSE;
    //m_channel_name = CFW_BC::CHANNEL; //Temporary
    //    AfxMessageBox(_T("InitializeServer"));
    if (Initialize())
    {
        if( InitializeListener(m_channel_name,(MPLUS_MESSAGE_CALLBACK) CKtDPSCommMgr::RecvCallBack))
            l_status = TRUE;
    }
    return l_status;
    
}

IMplusMessage* CKtDPSCommMgr::CreateMessage(
                                            tiprep_message_t f_msg_id,
                                            int f_status,
                                            BSTR f_bstrSender,
                                            BSTR f_parameter,
                                            BSTR f_custom)
                                            
{
    UNREFERENCED_PARAMETER(f_parameter);
    UNREFERENCED_PARAMETER(f_status);
    UNREFERENCED_PARAMETER(f_bstrSender);
    
    IMplusMessage *l_msg = NEW_MSG();
    
    if (NULL == l_msg)
    {
        return NULL;
    }
    
    if (f_msg_id == FW_ReportState) {
        
        l_msg->SetCommand(_bstr_t(_T("FW_ReportState")));
        DWORD dwProcessId = GetCurrentProcessId();
        CString csProcessId = _T("");
        csProcessId.Format( _T( "%ld" ), dwProcessId );
        l_msg->SetCustom(_bstr_t( csProcessId ));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(GetKtReconStatusString()));
        
    } else if (f_msg_id == RE_FW_CheckState) {
        
        l_msg->SetCommand(_bstr_t(_T("RE_FW_CheckState")));
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(GetKtReconStatusString()));
        
        l_msg->SetReUID(Uid);     //DPS-6
        
    } else if (f_msg_id == RE_FW_Close) {        
        
        l_msg->SetCommand(_bstr_t(_T("RE_FW_Close")));
        
        l_msg->SetCustom(f_custom);
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(S_OK));
        l_msg->SetReUID(Uid);
        
    } else if (f_msg_id == RE_FW_Show) {
        
        l_msg->SetCommand(_bstr_t(_T("RE_FW_Show")));
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(S_OK));
        
        l_msg->SetReUID(Uid);     //DPS-6
        
    } else if (f_msg_id == RE_FW_Hide) {
        
        
        l_msg->SetCommand(_bstr_t(_T("RE_FW_Hide")));
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(S_OK));
        
        l_msg->SetReUID(Uid);     //DPS-6
        
    } else if (f_msg_id == FW_RequestClose) {
        
        CString app_name;
        app_name.LoadString(AFX_IDS_APP_TITLE);
        l_msg->SetCommand(_bstr_t(_T("FW_RequestClose")));
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(app_name));
        
        l_msg->SetReUID(Uid);     //DPS-6
        
    } else if (f_msg_id == FW_RequestHideGL) {
        
        CString app_name;
        app_name.LoadString(AFX_IDS_APP_TITLE);
        l_msg->SetCommand(_bstr_t(_T("FW_RequestHideGL")));
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(app_name));
        
        l_msg->SetReUID(Uid);     //DPS-6
        
    } else if (f_msg_id == FW_RequestShowGL) {
        
        CString app_name;
        app_name.LoadString(AFX_IDS_APP_TITLE);
        l_msg->SetCommand(_bstr_t(_T("FW_RequestShowGL")));
        l_msg->SetCustom(_bstr_t(_T("")));
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetParameter(_bstr_t(app_name));
        
        l_msg->SetReUID(Uid);     //DPS-6
    }else if (f_msg_id == FW_RegisterWindowZOrder){
        
        l_msg->SetSender(_bstr_t(m_channel_name));
        l_msg->SetCommand( _bstr_t( _T("FW_RegisterWindowZOrder")));        
        
        l_msg->SetCustom( _bstr_t(_T("")));
        
        l_msg->SetReUID(Uid);
        
    } else if (f_msg_id == FW_ChangeWindowZOrder) {
        
        l_msg->SetSender( _bstr_t( m_channel_name) );
        
        l_msg->SetCommand( _bstr_t( _T("FW_ChangeWindowZOrder") ) );        
        
        l_msg->SetCustom( _bstr_t(_T("")));
        
        l_msg->SetReUID(Uid);
    }else if(f_msg_id == FW_AddZOrderLayer) {   
		l_msg->SetCommand(_bstr_t(_T("FW_AddZOrderLayer")));
		l_msg->SetParameter(f_parameter);  //6633 test value
		l_msg->SetReUID(Uid);
	}else if(f_msg_id == FW_UnregisterWindowZOrder) {   
		l_msg->SetCommand(_bstr_t(_T("FW_UnregisterWindowZOrder")));
		l_msg->SetParameter(f_parameter);  //6633 test value
		l_msg->SetReUID(Uid);
		}
    
    
    return l_msg;
}

void CKtDPSCommMgr::DeleteInstance()
{
    if (m_dps_com_manager)
    {
        delete m_dps_com_manager;
        m_dps_com_manager = NULL;
    }
}



BOOL CKtDPSCommMgr::DestroyServer()
{
    BOOL l_status = FALSE;
    if (DestroyListener( (MPLUS_MESSAGE_CALLBACK) CKtDPSCommMgr::RecvCallBack,(void **)&m_dpsconnector))
    {
        if (Terminate())
        {
            l_status = TRUE;
        }
    }
    return l_status;
}
/////////////////////////////////END OF FILE /////////////////////////////////////