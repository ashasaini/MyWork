// PDLogHelper.cpp: implementation of the CPDLogHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDLogHelper.h"
#include "PDataParser/INIReader.h"
#include "Common.h"
#include "Shlwapi.h"
#include "dataTypes.h"
#include "PDTrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef _WIN64
#define CONST3264(a) (a##i64)
#else
#define CONST3264(a)  (a##L)
#endif
const int ERROR_ID = 0XCD010101;

CMRErrorLog *CPDLogHelper::m_errorlog_instance = NULL;
CMRTraceLog *CPDLogHelper::m_tracelog_instance = NULL;
CPDLogHelper* CPDLogHelper::m_pPDLogger = NULL;
CString CPDLogHelper::m_config_path = "";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/**
*  Represents application name.
*/
const CString TRACE_SECTION_NAME = "TRACE_SETTINGS";
const CString ERROR_SECTION_NAME = "ERROR_SETTINGS";
const CString APPLICATION_NAME = "PD_Tool";


/**
* Constructor.
* This function is used for initialization of member variables.
*/
CPDLogHelper::CPDLogHelper()
{
    m_config_path = getenv(MRMP_CONFIG);
    m_config_path += PD_TOOL;
}

CPDLogHelper::~CPDLogHelper()
{
    //
}

//////////////      Instance Creation       ////////////////////////

/*************************************************************
* Method:    	CreateInstance
* Description: 	This function creates logger instance for this
*                singleton class.
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int CPDLogHelper::CreateInstance(void)
{
    if(NULL == m_pPDLogger){
        m_pPDLogger = new CPDLogHelper();
        
        if (NULL == m_pPDLogger){
            CPDErrorMsgDlg l_errmsg_dlg;
            CString l_cserrmsg (_T("Loghelper Creation Failed."));
            l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
            l_errmsg_dlg.DoModal();
            return(E_ERROR);
        }else{
            if (E_NO_ERROR == CreateTraceLogInstance()){
                if (E_NO_ERROR == CreateErrorLogInstance()){
                    return(E_NO_ERROR);
                }else{
                    return(E_ERROR);
                }
            }else{
                return(E_ERROR);
            }
        }
        
        return(E_NO_ERROR);
    }else{
        
        return(E_NO_ERROR);
    }
}

/*************************************************************
* Method:    	CreateErrorLogInstance
* Description: 	This function creates error log instance.
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int CPDLogHelper::CreateErrorLogInstance(void)
{
    if (NULL == m_errorlog_instance){
        bool l_berror_cssdk = FALSE;
        CString l_csPDerrorlog_path = "";
        CString l_csini_path ="";
        CString l_cs_inierrorlog_path="";
        CString l_cssdk="";
        if (NULL == m_errorlog_instance){
            
            if(PathFileExists(m_config_path)){
                l_csini_path = m_config_path;
                l_csini_path += PD_INI_FILE;//Patni-VP/2012Feb09/Added/Design Implementation
                
                CIniReader l_ini_reader(l_csini_path);
                
                l_csPDerrorlog_path = getenv(MRMP_LOGDIR);
                l_cs_inierrorlog_path = l_ini_reader.GetKeyValue(PD_ERROR_FILE_PATH,
                    ERROR_SECTION_NAME);
                l_csPDerrorlog_path += l_cs_inierrorlog_path;
                
                if(PathFileExists(l_csPDerrorlog_path)){
                    l_csPDerrorlog_path += PD_ERROR_LOG_FILE;
                } else{
                    CPDErrorMsgDlg l_errmsg_dlg;
                    CString l_cserrmsg (_T("Error log file path does not exist."));
                    l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
                    l_errmsg_dlg.DoModal();
                    return(E_ERROR);
                }
                
                l_cssdk = l_ini_reader.GetKeyValue(PD_ERROR_CSSDK, ERROR_SECTION_NAME);
                
                if(_ttoi(l_cssdk)){
                    l_berror_cssdk = TRUE;
                }else{
                    l_berror_cssdk = FALSE;
                }
            }else{
                CPDErrorMsgDlg l_errmsg_dlg;
                CString l_cserrmsg (_T("PD config file path does not exist."));
                l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
                l_errmsg_dlg.DoModal();
                return(E_ERROR);
            }
            
            m_errorlog_instance = new CMRErrorLog(l_berror_cssdk,l_csPDerrorlog_path);
            
            if (NULL == m_errorlog_instance){
                CPDErrorMsgDlg l_errmsg_dlg;
                CString l_cserrmsg (_T("ErrorLogger Creation Failed."));
                l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
                l_errmsg_dlg.DoModal();
                return(E_ERROR);
            } else {
                //dummy
            }
        } else{
            return(E_NO_ERROR);
        }
    }
    
    return(E_NO_ERROR);
}


/*************************************************************
* Method:    	CreateTraceLogInstance
* Description:  This function creates Trace log instance.
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int CPDLogHelper::CreateTraceLogInstance(void)
{
    if(NULL != m_tracelog_instance){
        return(E_NO_ERROR);
    }
    if(FALSE == PathFileExists(m_config_path)){
        CPDErrorMsgDlg l_errmsg_dlg;
        CString l_cserrmsg (_T("PD config file path does not exist."));
        l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
        l_errmsg_dlg.DoModal();
        return E_ERROR;
    }
    //      if(NULL != m_tracelog_instance){
    //          return(E_NO_ERROR);
    //      }
    // if(NULL == m_tracelog_instance){
    CString l_csPDtracelog_path = "";
    CString l_config_file = "";
    bool l_btrace_cssdk = FALSE;
    long m_nlmarker_val = 0L;
    CString l_ini_path="";
    CString l_cssdk, l_log_marker, l_cstrace_path;
    int l_trace_level = 0;
    TCHAR *p = NULL;
    
    // if(NULL == m_tracelog_instance){
    // if(PathFileExists(m_config_path)){
    l_ini_path = m_config_path;
    l_ini_path += PD_INI_FILE;
    l_config_file += m_config_path;
    l_config_file += PD_TRACECONFIG_INI_FILE;
    
    
    if(PathFileExists(l_ini_path)){
        
        CIniReader m_ini_reader(l_ini_path);
        l_cssdk = m_ini_reader.GetKeyValue(PD_TRACE_CSSDK, TRACE_SECTION_NAME);
        l_trace_level = _ttoi(m_ini_reader.GetKeyValue(PD_TRACE_LEVEL,
            TRACE_SECTION_NAME));
        
        if(0 < l_trace_level){
            l_csPDtracelog_path = getenv(MRMP_LOGDIR);
            l_cstrace_path = m_ini_reader.GetKeyValue(PD_TRACE_FILE_PATH,
                TRACE_SECTION_NAME);
            l_csPDtracelog_path += l_cstrace_path;
            
            if(PathFileExists(l_csPDtracelog_path)){
                l_csPDtracelog_path += PD_TRACE_LOG_FILE;
            }else{
                CPDErrorMsgDlg l_errmsg_dlg;
                CString l_cserrmsg (_T("Trace log file path does not exist."));
                l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
                l_errmsg_dlg.DoModal();
                return(E_ERROR);
            }
            
            l_log_marker = m_ini_reader.GetKeyValue(PD_MARKER, TRACE_SECTION_NAME);
            
            
            m_nlmarker_val = _tcstoul(l_log_marker, &p, NUMBER_BASE);
            
            if(_ttoi(l_cssdk)){
                l_btrace_cssdk = TRUE;
            }else{
                l_btrace_cssdk = FALSE;
            }
        }
    } else {
        //Need some handling
    }
    //             }else{
    //                 CPDErrorMsgDlg l_errmsg_dlg;
    //                 CString l_cserrmsg (_T("PD config file path does not exist."));
    //                 l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
    //                 l_errmsg_dlg.DoModal();
    //                 return E_ERROR;
    //             }
    
    m_tracelog_instance = new CMRTraceLog(l_btrace_cssdk,m_nlmarker_val,
        l_config_file,l_csPDtracelog_path);
    if (NULL == m_tracelog_instance ){
        CPDErrorMsgDlg l_errmsg_dlg;
        CString l_cserrmsg (_T("TraceLogger Creation Failed."));
        l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
        l_errmsg_dlg.DoModal();
        return(E_ERROR);
    }
    //         }else{
    //             return(E_NO_ERROR);
    //         }
    //}
    return(E_NO_ERROR);
}


/*************************************************************
* Method:    	WriteToErrorLog
* Description: 	This function is used to trace error log information.
* Parameter: 	CString & f_smsg :  Error message.
* Parameter: 	int f_Line_No :     Line number at which error occurs.
* Parameter: 	Msg_Option f_nmsgoption :
* Returns:   	void
*************************************************************/
void CPDLogHelper::WriteToErrorLog(CString &f_smsg, 
                                   int f_Line_No,
                                   Msg_Option f_nmsgoption
                                   ){
    try
    {
        DWORD n_size = CONST3264(20);
        TCHAR   sz_user_name[BUFFER_SIZE_64] = L"";         // AS buffer size was 50
        CString m_user_name;
        BOOL l_bresult = FALSE;
        CString l_null_str = _T("");
        CMRErrorLog l_errorlog_instance(1, l_null_str);
        MRERROR_LOG_DATA error_log_data;
        CPDErrorMsgDlg l_errmsg_dlg;
        
        l_bresult = GetUserName(sz_user_name, &n_size);
        m_user_name = sz_user_name;
        
        if ( (NULL == m_errorlog_instance) || (ERRORVIEW_DISPLAY == f_nmsgoption) ){
            error_log_data.error_level = (MRERROR_LOG_LEVEL) ERROR_LOG_INFORMATION1;
            error_log_data.error_id = (DWORD)ERROR_ID;
            error_log_data.l_line_number = f_Line_No;
            error_log_data.error_message = SysAllocString(f_smsg);
            error_log_data.error_source = SysAllocString(APPLICATION_NAME);
            
            error_log_data.error_source_file_name = SysAllocString(_T("PlanDuplicator"));
            error_log_data.user_name = SysAllocString(m_user_name);
            error_log_data.hwndApp = (CWnd *)GetActiveWindow();
            l_bresult = l_errorlog_instance.DisplayMessageInErrorView(error_log_data);
        }else{
            
            error_log_data.error_level = (MRERROR_LOG_LEVEL) ERROR_LOG_INFORMATION1;
            error_log_data.error_id = (DWORD)ERROR_ID;
            error_log_data.l_line_number = f_Line_No;
            error_log_data.error_message = SysAllocString(f_smsg);
            error_log_data.error_source = SysAllocString(APPLICATION_NAME);
            
            error_log_data.error_source_file_name = SysAllocString(_T("PlanDuplicator"));
            error_log_data.user_name = SysAllocString(m_user_name);
            error_log_data.hwndApp = (CWnd *)((AfxGetMainWnd())->m_hWnd);
            
            if(ERROR_LOGONLY == f_nmsgoption){
                l_bresult = m_errorlog_instance->WriteMessageToErrorLog(error_log_data);
            } else if(ERRORDIALOG_DISPLAY == f_nmsgoption){
                l_bresult = m_errorlog_instance->DisplayMessageAndWriteToLog(error_log_data);
            } else if(CUSTOM_ERRORDIALOG_DISPLAY == f_nmsgoption){
                
                l_errmsg_dlg.SetErrorMessage(f_smsg);
                l_errmsg_dlg.DoModal();
                l_bresult = m_errorlog_instance->WriteMessageToErrorLog(error_log_data);
            } else{
                l_bresult = m_errorlog_instance->WriteMessageToErrorLog(error_log_data);
            }
            
            if (FALSE == l_bresult){
                CPDErrorMsgDlg l_errmsg_dlg;
                CString l_cserrmsg (_T("Failed to log the error message"));
                l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
                l_errmsg_dlg.DoModal();
            }else{
                //
            }
        }
    }
    catch(...)
    {
        //
    }
}

/*************************************************************
* Method:    	WriteToTraceLog
* Description: 	This function is used to trace Trace log
*               information.
* Parameter: 	LPCTSTR f_domain_name :  Tracing the Logs
*               through CSSDK Logging or Custom Logging.
* Parameter: 	unsigned long f_marker_value :  Indicates the
*               hexadecimal value whose corresponding string name
*               to be logged in the Trace Log.
* Parameter: 	LPCTSTR f_log_text :     Message.
* Parameter: 	LPCTSTR f_func_name :   Name of the function.
* Returns:   	void
*************************************************************/
void CPDLogHelper::WriteToTraceLog(LPCTSTR f_domain_name,unsigned long f_marker_value,
                                   LPCTSTR f_log_text,LPCTSTR f_func_name
                                   )
{
    
    if (m_tracelog_instance){
        m_tracelog_instance->LogTrace(f_domain_name,f_marker_value,
            f_func_name,f_log_text);
    }else{
        //dummy
    }
}

//////////////////////////      End of File     ////////////////////////////////////