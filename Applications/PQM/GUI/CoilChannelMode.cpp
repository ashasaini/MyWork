// CoilChannelMode.cpp: implementation of the CCoilChannelMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoilChannelMode.h"
#include "PQMView.h"
#include "PQMUtility.h"
#include "CoilSelectionUtility.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoilChannelMode::CCoilChannelMode(CPQMView* f_pqm_view) :
    m_pqm_view(f_pqm_view),
    m_limited_channel(0)
{

}

CCoilChannelMode::~CCoilChannelMode()
{

}


//************************************Method Header************************************
// Method Name  : ValidateMaxCoilChannelFile
// Author       : PATNI/ Pavan
// Purpose      : Validate File and Data
//***********************************************************************************
BOOL CCoilChannelMode::ValidateMaxCoilChannelFile(const int f_cf_system_enable_channel, int& f_system_enable_channel, int& f_limited_channel)
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::ValidateMaxCoilChannelFile");
    PQM_TRACE_SCOPED(FUNC_NAME);

    BOOL l_bRet = TRUE;

    if (!ReadMaxCoilChannelFile(f_cf_system_enable_channel, f_system_enable_channel, f_limited_channel)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("error in reading MaxCoilChannel.ini"));
        return FALSE;
    }

    if (!ValidateMaxCoilChannelValues(f_cf_system_enable_channel, f_system_enable_channel, f_limited_channel)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Values in MaxCoilChannel.ini file are not in range"));
        return FALSE;
    }

    return l_bRet;
}
//************************************Method Header************************************
// Method Name  : ValidateMaxCoilChannelFile
// Author       : PATNI/ Pavan
// Purpose      : Validate File and Data
//***********************************************************************************
// TODO:: this function must move in separate class
BOOL CCoilChannelMode::ReadMaxCoilChannelFile(const int f_cf_system_enable_channel, int& f_system_enable_channel, int& f_limited_channel)
{

    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::ReadMaxCoilChannelFile");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString l_max_coil_filepath = _T("");
    CString l_file_name = _T("MaxCoilChannel.ini");

    if (!GetConfigFilePath(l_max_coil_filepath, l_file_name)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("MaxCoilChannel.ini file not present"));
        return FALSE;
    }


    FILE* fp = NULL;
    char  l_filecontent[MAX_PATH] = {0};
    TCHAR l_buff[MAX_PATH] = {0} ;
    BOOL bret = FALSE;

    fp = _wfopen(l_max_coil_filepath, _T("r"));

    if (NULL == fp) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("error in reading MaxCoilChannel.ini file"));
        return FALSE;
    }

    while (!feof(fp)) { // 21-Oct-11
        // Read file contents
        memset(l_filecontent, NULL, sizeof(l_filecontent));
        fgets(l_filecontent, sizeof(l_filecontent), fp);

        CString l_contents = _T("");
        CString l_system_enable_channel = _T("");
        CString l_limited_channel = _T("");

        mbstowcs(l_buff, l_filecontent, MAX_PATH);
        l_contents.Format(_T("%s"), l_buff);
        int index = l_contents.ReverseFind('=');

        l_system_enable_channel = l_contents.Mid(0, index);

        l_limited_channel = l_contents.Mid(index + 1);

        f_system_enable_channel = _ttoi(l_system_enable_channel);
        f_limited_channel = _ttoi(l_limited_channel);

        CString msg = _T("");
        msg.Format(_T("System type = %d, System enable channel= %d, Limited Channel = %d"), f_cf_system_enable_channel, f_system_enable_channel, f_limited_channel);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, msg);

        if (!ValidateMaxCoilChannelValues(f_cf_system_enable_channel, f_system_enable_channel, f_limited_channel)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid entry"));

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Values in MaxCoilChannel.ini file are in range"));
            bret = TRUE;
            break;
        }
    }

    if (NULL != fp) {
        fclose(fp);
        fp = NULL;
    }

    return bret;
}

//************************************Method Header************************************
// Method Name  : ValidateMaxCoilChannelValues
// Author       : PATNI/ Pavan
// Purpose      : Validate File and Data
//***********************************************************************************
// TODO:: this function must move in separate class
BOOL CCoilChannelMode::ValidateMaxCoilChannelValues(const int f_cf_system_enable_channel, const int f_system_enable_channel_keyvalue, const int f_limited_channel_keyvalue)
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::ValidateMaxCoilChannelValues");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString l_msg = _T("");
    l_msg.Format(_T("System Enable channel = %d, system Enable channel Key = %d, limited channel key = %d"), f_cf_system_enable_channel, f_system_enable_channel_keyvalue, f_limited_channel_keyvalue);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

    if (f_limited_channel_keyvalue < 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Limited channel values"));
        return FALSE;
    }

    if ((f_cf_system_enable_channel == f_system_enable_channel_keyvalue) && ((f_limited_channel_keyvalue < f_cf_system_enable_channel) && (f_limited_channel_keyvalue > 1))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Values are matching"));
        return TRUE;

    } else {
        return FALSE;
    }

    return TRUE;
}

//************************************Method Header************************************
// Method Name  : GetConfigFilePath
// Author       : PATNI/ Pavan
// Purpose      : check the existence of configuration file and get the path if successful.
//***********************************************************************************
BOOL CCoilChannelMode::GetConfigFilePath(CString& f_config_file_path, CString f_file_name)
{
    wchar_t l_buffer[MAX_PATH] = {0};

    DWORD l_ret = ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);

    if (0 == l_ret) {
        _tcscpy(l_buffer, _T("C:\\MRMPlus\\config\\MRConfig"));
    }

    f_config_file_path  = l_buffer;
    f_config_file_path += L"\\";
    f_config_file_path += L"PQM\\";
    f_config_file_path += f_file_name;


    CFileFind l_file_find;

    if (!l_file_find.FindFile(f_config_file_path)) {

        CPQMLogMgr::GetInstance()->WriteEventLog(f_config_file_path + _T(" does not exist."),
                ERROR_LOG_WARNING1, _T("PQMUtility.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        return FALSE;
    }

    return TRUE;

}
