/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrepLogHelper.h"
#include "MainFrm.h"
#include "IpFrame.h"
#include "PDataParser/INIReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMRErrorLog *CTiPrepLogHelper::m_errorlog_instance = NULL;
CMRTraceLog *CTiPrepLogHelper::m_tracelog_instance = NULL;
CTiPrepLogHelper* CTiPrepLogHelper::m_pTiPrepLogger = NULL;
CString CTiPrepLogHelper::m_config_path = "";
/*
bool IsNumeric(const CString &data)  
{
    for(int i=0; i < data.GetLength(); i++)
    {
		//+Patni-PH/2010Feb09/Modified/SU16 IR124 Part2
        if((data[i] >= _T('0') && data[i] <= _T('9')) || (data[i] == _T('.'))) 
        {
			continue;
        }
        else
        {
            return false;
        }
		//-Patni-PH/2010Feb09/Modified/SU16 IR124 Part2
    }
    return true;
}
int GetLogFileCount(const CString &file_name)  
{
    int i_file_start = file_name.ReverseFind(_T('\\'));
    CString str_file_start = _T("");
    
    if(i_file_start != -1)
    {
        str_file_start = file_name.Right(file_name.GetLength() - i_file_start - 1);
    }

    int file_ext = file_name.ReverseFind(_T('.'));
    CString str_file_ext = _T("");
    if(file_ext != -1)
    {
        str_file_ext = file_name.Right(file_name.GetLength()- file_ext - 1);
    }

    CString path_str(file_name);
    CString sFileType (str_file_start);
    sFileType += _T("*");
    path_str.Replace(str_file_start, sFileType);

    int file_count = 0;

    WIN32_FIND_DATA find_data = {0};
    HANDLE hFile_handle = FindFirstFile(path_str, &find_data);

    do 
    {
        if(INVALID_HANDLE_VALUE == hFile_handle)
        {
            break;
        }

        CString find_file_name = find_data.cFileName;
        int nExt = find_file_name.ReverseFind(_T('.'));

        if(nExt != -1)
        {
            CString sExt = find_file_name.Right(find_file_name.GetLength() - nExt -1);
            if(IsNumeric(sExt))
            {
                 file_count++;
            }
        }

    } while (FindNextFile(hFile_handle, &find_data));

    return file_count;
}
void SetBackUpLogFiles(const CString &file_name) 
{
	try{

		CString file_path = _T("");
		file_path = file_name;
		double curr_file_size = 0;
		if(PathFileExists(file_name))
		{
			CFile temp_file;
			LARGE_INTEGER lni;
			temp_file.Open(file_name,CFile::modeNoTruncate| CFile::modeRead | CFile::shareDenyNone);
			GetFileSizeEx((HANDLE)temp_file.m_hFile,&lni);
			curr_file_size = (int)lni.QuadPart;
			//curr_file_size = temp_file.GetLength();
			temp_file.Close();
		}
    
		int curr_file_cnt = GetLogFileCount(file_name);
		curr_file_size  = ((curr_file_size/1024)/1024);
  
		//Check if current file size overflow the File_Size_Limit
		if(curr_file_size >= 128)
		{
			//Check if current file count overflow the File_Count_Limit
			//then, delete max index file
			if(curr_file_cnt >= 2)
			{
				CString del_file_path(file_path);
				char buff[10];
				itoa(curr_file_cnt-1, buff, 10);
				CString str_file_id(_T("."));
				str_file_id += buff;
				del_file_path += str_file_id;
    
				if(PathFileExists(del_file_path))
				{
					if(!DeleteFile(del_file_path))
					{
						//Error
					}
				}
			}

			//Reset log file names
			for(int file_id = curr_file_cnt; file_id >=0; file_id-- )
			{
				CString new_file_name(file_path);
				CString old_file_name(file_path);
				char buff[10];
				itoa(file_id, buff, 10);
				CString str_file_id(_T("."));
				str_file_id += buff;
				new_file_name += str_file_id;

				if(file_id == 0)
				{
					old_file_name = file_path;
				}
				else
				{
					itoa(file_id - 1, buff, 10);
					str_file_id = _T(".");
					str_file_id += buff;
					old_file_name += str_file_id;
				}
            
				if(PathFileExists(old_file_name))
				{
					if(!MoveFileEx(old_file_name, new_file_name, MOVEFILE_REPLACE_EXISTING))
					{
						//Error
					}
				}
			}
		}
	}catch (CMemoryException* e) {
		e->Delete();
	} catch (...)
	{
//		MessageBox(NULL,NULL,NULL,NULL);
	}
}

/**
*  Represents application name.
*/
const CString APPLICATION_NAME = "TiPrep";


/**
* Constructor.
* This function is used for initialization of member variables.
*/
CTiPrepLogHelper::CTiPrepLogHelper()
{
    m_config_path = getenv(MRMP_CONFIG);
    m_config_path += _T("\\TiPrep");
}

/*************************************************************
* Method:       CreateInstance
* Description:  This function creates logger instance for this
*               singleton class.
* Returns:      bool
*************************************************************/
bool CTiPrepLogHelper::CreateInstance(BSTR lParam)
{
    if(NULL == m_pTiPrepLogger) {
        m_pTiPrepLogger = new CTiPrepLogHelper();

        if(!m_pTiPrepLogger) {
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
bool CTiPrepLogHelper::CreateErrorLogInstance(BSTR lParam)
{
    CString l_ini_path = "";
    bool l_error_cssdk = false;
    CString l_cssdk, l_error_path;

    if(PathFileExists(m_config_path)) {
        l_ini_path += m_config_path;
        l_ini_path += TIPREP_INI_FILE;
        int l_trace_level = 0;
        CIniReader m_ini_reader(l_ini_path);
        l_cssdk = m_ini_reader.GetKeyValue(TIPREP_ERROR_CSSDK, APPLICATION_NAME);
        l_error_path = m_ini_reader.GetKeyValue(TIPREP_ERROR_FILE_PATH,
                                                APPLICATION_NAME);

        if(PathFileExists(l_error_path)) {
            l_error_path += "\\";
            l_error_path += lParam; 
            l_error_path += TIPREP_ERROR_LOG_FILE;
        } else {
            return false;
        }

        if(_ttoi(l_cssdk)) {
            l_error_cssdk = true;
        } else {
            l_error_cssdk = false;
        }
    }
//     SetBackUpLogFiles(l_error_path);
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
bool CTiPrepLogHelper::CreateTraceLogInstance(BSTR lParam)
{
    CString l_ini_path = "";
    CString l_config_file = "";

    if(PathFileExists(m_config_path)) {
        l_ini_path += m_config_path;
        l_ini_path += TIPREP_INI_FILE;
        l_config_file += getenv(MRMP_CONFIG);//m_config_path;
		l_config_file+= _T("\\Trace");
        l_config_file += _T("\\MRTraceConfig.ini");//TIPREP_TRACECONFIG_INI_FILE;
    } else {
        return false;
    }

    bool l_trace_cssdk = false;
    long m_marker_val = 0L;
    CString l_cssdk, l_log_marker, l_trace_path;

    if(PathFileExists(l_ini_path)) {
        int l_trace_level = 0;
        CIniReader m_ini_reader(l_ini_path);
        l_cssdk = m_ini_reader.GetKeyValue(TIPREP_TRACE_CSSDK, APPLICATION_NAME);
        l_trace_level = _ttoi(m_ini_reader.GetKeyValue(TIPREP_TRACE_LEVEL,
                              APPLICATION_NAME));

        if(l_trace_level) {
            l_trace_path = m_ini_reader.GetKeyValue(TIPREP_TRACE_FILE_PATH,
                                                    APPLICATION_NAME);
            TCHAR *p = NULL;

            if(PathFileExists(l_trace_path)) {
                l_trace_path += "\\";
                l_trace_path += lParam;
                l_trace_path += TIPREP_TRACE_LOG_FILE;
            } else {
                //
            }

            l_log_marker = m_ini_reader.GetKeyValue(TIPREP_MARKER, APPLICATION_NAME);
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
//SetBackUpLogFiles(l_trace_path);
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
void CTiPrepLogHelper::WriteToErrorLog(CString f_smsg,
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
void CTiPrepLogHelper::WriteToTraceLog(LPCTSTR f_domain_name,
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
void CTiPrepLogHelper::DeleteInstance()
{
    if(NULL != m_errorlog_instance) {
        delete m_errorlog_instance;
        m_errorlog_instance = NULL;
    }

    if(NULL != m_tracelog_instance) {
        delete m_tracelog_instance;
        m_tracelog_instance = NULL;
    }

    if(NULL != m_pTiPrepLogger) {
        delete m_pTiPrepLogger;
        m_pTiPrepLogger = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
