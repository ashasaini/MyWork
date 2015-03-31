/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: ACGL
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "ACGLLogHelper.h"
#include "MainFrm.h"
#include "IpFrame.h"
#include "PDataParser/INIReader.h"
#include "CommonDefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMRErrorLog *CACGLLogHelper::m_errorlog_instance = NULL;
CMRTraceLog *CACGLLogHelper::m_tracelog_instance = NULL;
CACGLLogHelper* CACGLLogHelper::m_pACGLLogger = NULL;
CString CACGLLogHelper::m_config_path = "";

/**
*  Represents application name.
*/
const CString APPLICATION_NAME = "AdvCardiacGL";


/**
* Constructor.
* This function is used for initialization of member variables.
*/
CACGLLogHelper::CACGLLogHelper()
{
    m_config_path = getenv(MRMP_CONFIG);
    m_config_path += _T("\\AdvCardiacGL");
}

/*************************************************************
* Method:       CreateInstance
* Description:  This function creates logger instance for this
*               singleton class.
* Returns:      bool
*************************************************************/
bool CACGLLogHelper::CreateInstance(BSTR lParam)
{
    if(NULL == m_pACGLLogger) {
        m_pACGLLogger = new CACGLLogHelper();

        if(!m_pACGLLogger) {
            return false;
        } else {
            if(CreateTraceLogInstance(lParam)) {
                if(CreateErrorLogInstance(lParam)) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        return true;
    }

    return true;
}

/*************************************************************
* Method:       CreateErrorLogInstance
* Description:  This function creates error log instance.
* Returns:      bool
*************************************************************/
bool CACGLLogHelper::CreateErrorLogInstance(BSTR lParam)
{
    CString l_ini_path = "";
    bool l_error_cssdk = false;
    CString l_cssdk, l_error_path;

    if(PathFileExists(m_config_path)) {
        l_ini_path += m_config_path;
        l_ini_path += ACGL_INI_FILE;
        int l_trace_level = 0;
        CIniReader m_ini_reader(l_ini_path);
        l_cssdk = m_ini_reader.GetKeyValue(ACGL_ERROR_CSSDK, APPLICATION_NAME);
        l_error_path = m_ini_reader.GetKeyValue(ACGL_ERROR_FILE_PATH,
                                                APPLICATION_NAME);

        if(PathFileExists(l_error_path)) {
            l_error_path += "\\";
            l_error_path += lParam; 
            l_error_path += ACGL_ERROR_LOG_FILE;
        } else {
            return false;
        }

        if(_ttoi(l_cssdk)) {
            l_error_cssdk = true;
        } else {
            l_error_cssdk = false;
        }
    }

    if(NULL == m_errorlog_instance) {
        m_errorlog_instance = new CMRErrorLog(
            l_error_cssdk,
            l_error_path,
			APPEND_LOG
        );

        if(!m_errorlog_instance) {
            return false;
        }
    }

    return true;
};

/*************************************************************
* Method:       CreateTraceLogInstance
* Description:  This function creates Trace log instance.
* Returns:      bool
*************************************************************/
bool CACGLLogHelper::CreateTraceLogInstance(BSTR lParam)
{
    CString l_ini_path = "";
    CString l_config_file = "";

    if(PathFileExists(m_config_path)) {
        l_ini_path += m_config_path;
        l_ini_path += ACGL_INI_FILE;
        l_config_file += getenv(MRMP_CONFIG);//m_config_path;
		l_config_file+= _T("\\Trace");
        l_config_file += _T("\\MRTraceConfig.ini");//ACGL_TRACECONFIG_INI_FILE;
    } else {
        return false;
    }

    bool l_trace_cssdk = false;
    long m_marker_val = 0L;
    CString l_cssdk, l_log_marker, l_trace_path;

    if(PathFileExists(l_ini_path)) {
        int l_trace_level = 0;
        CIniReader m_ini_reader(l_ini_path);
        l_cssdk = m_ini_reader.GetKeyValue(ACGL_TRACE_CSSDK, APPLICATION_NAME);
        l_trace_level = _ttoi(m_ini_reader.GetKeyValue(ACGL_TRACE_LEVEL,
                              APPLICATION_NAME));

        if(l_trace_level) {
            l_trace_path = m_ini_reader.GetKeyValue(ACGL_TRACE_FILE_PATH,
                                                    APPLICATION_NAME);
            TCHAR *p = NULL;

            if(PathFileExists(l_trace_path)) {
                l_trace_path += "\\";
                l_trace_path += lParam;
                l_trace_path += ACGL_TRACE_LOG_FILE;
            } else {
                //
            }

            l_log_marker = m_ini_reader.GetKeyValue(ACGL_MARKER, APPLICATION_NAME);
            m_marker_val = _tcstoul(l_log_marker, &p, NUMBER_BASE);

            if(_ttoi(l_cssdk)) {
                l_trace_cssdk = true;
            } else {
                l_trace_cssdk = false;
            }
        }
    } else {
        return false;
    }

    m_tracelog_instance = new CMRTraceLog(
        l_trace_cssdk,
        m_marker_val,
        l_config_file,
        l_trace_path
    );

    if(!m_tracelog_instance) {
        return false;
    }

    return true;
};

/*************************************************************
* Method:       WriteToErrorLog
* Description:  This function is used to trace error log information.
* Parameter:    CString f_smsg
*               Error message.
* Parameter:    int f_Line_No
*               Line number at which error occurs.
* Parameter:    int f_nmsgoption
* Returns:      void
*************************************************************/
void CACGLLogHelper::WriteToErrorLog(CString f_smsg,
                                       int f_Line_No, CString f_csFile_Name,
                                       int f_nmsgoption)
{
    try {
        DWORD n_size = 20L;
        TCHAR   sz_user_name[50] = L"";
        CString m_user_name;
        BOOL l_bresult = FALSE;
        l_bresult = GetUserName(sz_user_name, &n_size);
        m_user_name = sz_user_name;

        if(NULL == m_errorlog_instance || (ERRORVIEW_DISPLAY == f_nmsgoption)) { 
            CString l_null_str = _T("");
            CMRErrorLog l_errorlog_instance(1, l_null_str);
            MRERROR_LOG_DATA error_log_data;
            error_log_data.error_level = (MRERROR_LOG_LEVEL) ERROR_LOG_INFORMATION1;
            error_log_data.error_id = (DWORD)0XCD010101;
            error_log_data.l_line_number = f_Line_No;
            error_log_data.error_message = (f_smsg);
            error_log_data.error_source = (APPLICATION_NAME);
            error_log_data.error_source_file_name = f_csFile_Name;
            error_log_data.user_name = (m_user_name);
            error_log_data.hwndApp = (CWnd *)GetActiveWindow();
            l_bresult = l_errorlog_instance.DisplayMessageInErrorView(error_log_data);
        } else {
            MRERROR_LOG_DATA error_log_data;
            error_log_data.error_level = (MRERROR_LOG_LEVEL) ERROR_LOG_INFORMATION1;
            error_log_data.error_id = (DWORD)0XCD010101;
            error_log_data.l_line_number = f_Line_No;
            error_log_data.error_message = (f_smsg);
            error_log_data.error_source = (APPLICATION_NAME);
            error_log_data.error_source_file_name = f_csFile_Name;
            error_log_data.user_name = (m_user_name);
            error_log_data.hwndApp = (CWnd *)(((CInPlaceFrame *)AfxGetMainWnd())->m_hWnd);
            l_bresult = m_errorlog_instance->WriteMessageToErrorLog(error_log_data);
        }
    } catch(...) {
        //
    }
}

/*************************************************************
* Method:       WriteToTraceLog
* Description:  This function is used to trace Trace log information.
* Parameter:    LPCTSTR f_domain_name
*               Tracing the Logs through CSSDK Logging or Custom Logging.
* Parameter:    unsigned long f_marker_value
*               Indicates the hexadecimal value whose corresponding
*               string name to be logged in the Trace Log.
* Parameter:    LPCTSTR f_log_text
*               Message.
* Parameter:    LPCTSTR f_func_name
*               Name of the function.
* Returns:      void
*************************************************************/
void CACGLLogHelper::WriteToTraceLog(LPCTSTR f_domain_name,
                                       unsigned long f_marker_value,
                                       LPCTSTR f_log_text,
                                       LPCTSTR f_func_name
                                      )
{
    CString l_func_name;

    if(m_tracelog_instance) {
        m_tracelog_instance->LogTrace(
            f_domain_name,
            f_marker_value,
            f_func_name,
            f_log_text
        );
    }
}

/*************************************************************
* Method:       DeleteInstance
* Description:  This function is used
* Returns:      void
*************************************************************/
void CACGLLogHelper::DeleteInstance()
{
    if(NULL != m_errorlog_instance) {
        delete m_errorlog_instance;
        m_errorlog_instance = NULL;
    }

    if(NULL != m_tracelog_instance) {
        delete m_tracelog_instance;
        m_tracelog_instance = NULL;
    }

    if(NULL != m_pACGLLogger) {
        delete m_pACGLLogger;
        m_pACGLLogger = NULL;
    }
}
///////////////////////////////// END OF FILE /////////////////////////////////