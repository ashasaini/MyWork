//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-774

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMUtility.cpp
 *  Overview: Implementation of CPQMUtility class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include <process.h>
#include "stdafx.h"
#include "PQMUtility.h"
#include <MPlusI18n/MPlusI18nCatalog.h>
#include <PDataParser/INIReader.h>
#include <PQM/resource.h>
#include "Psapi.h"
#include "PQMView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPQMUtility* CPQMUtility::m_utility = NULL;

CString CPQMUtility::m_parent_name;
int CPQMUtility::m_status;
CString CPQMUtility::m_parent_channel;
CString CPQMUtility::m_sender_param;

class CIniReader;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//************************************Method Header**************************************
//Method Name   :  GetInstance
//Author        :  PATNI
//Purpose       :  Creates PQMUtility instance if it is not created before.
//**************************************************************************************/
CPQMUtility* CPQMUtility::GetInstance(
)
{
    if (!m_utility) {
        m_utility = new CPQMUtility();
    }

    return m_utility;
}


//********************************Method Header********************************
//Method Name   :ParseLine()
//Author        :PATNI/GP
//Purpose       :Parse two comma separated string
//*****************************************************************************
void CPQMUtility::ParseLine(
    const CString& f_strline,
    CString& f_name,
    CString& f_key,
    CString& f_value
)const
{
    int l_prevch   = 0;
    int l_curch = f_strline.Find(_T(","), l_prevch);

    if (l_curch != -1) {
        f_key = f_strline.Mid(l_prevch, l_curch - l_prevch);
        l_prevch = l_curch + 1;

        l_curch = f_strline.Find(_T(","), l_prevch);

        if (l_curch == -1) {

            //////////////
            f_value = f_strline.Mid(l_prevch,
                                    f_strline.GetLength() - l_prevch);
            //////////

        } else if (l_curch != -1) {
            f_value = f_strline.Mid(l_prevch, l_curch - l_prevch);

        } else {
        }
    } else {
        l_curch = f_strline.Find(_T(":"), l_prevch);

        if (l_curch != -1) {
            f_name = f_strline.Mid(l_prevch, l_curch - l_prevch);
        }
    }
}





//************************************Method Header**************************************

//Method Name:  ConvertDoubleToString

//Author:  PATNI/SVP

//Purpose: To convert double to string.

//**************************************************************************************/
void CPQMUtility::ConvertDoubleToString(
    const double d_val,
    CString& str ,
    const CString& format_spec
)const
{
    str.Format(format_spec, (float)d_val);
    //+Patni-ARD/2009Dec9/Added/DeFT# MSA0248-00087
    str.TrimLeft();
    str.TrimRight();
    //-Patni-ARD/2009Dec9/Added/DeFT# MSA0248-00087
}


//************************************Method Header**************************************

//Method Name:  ConvertStringToDouble

//Author:  PATNI/SVP

//Purpose: To convert string to double.

//**************************************************************************************/
double CPQMUtility::ConvertStringToDouble(
    const CString& str
) const
{

    CString extract_string = _T("");
    wchar_t* extract_string_buffer = extract_string.GetBuffer(0);

    double value_retrived = wcstod(str, &extract_string_buffer);

    extract_string = extract_string_buffer;
    extract_string.ReleaseBuffer();

    return value_retrived;
}


//********************************Method Header********************************
//Method Name   :GetScanOffsetValues()
//Author        :PATNI/GP
//Purpose       :Get Scan Offset Values
//*****************************************************************************
void CPQMUtility::GetScanOffsetValues(
    const CString& f_strline,
    CString& x_offset,
    CString& y_offset,
    CString& z_offset
)const
{
    int l_prevch   = 0;
    int l_curch = f_strline.Find(_T(","), l_prevch);

    if (l_curch != -1) {
        x_offset = f_strline.Mid(l_prevch, l_curch - l_prevch);
        l_prevch = l_curch + 1;

        l_curch = f_strline.Find(_T(","), l_prevch);

        if (l_curch == -1) {

            y_offset = f_strline.Mid(l_prevch,
                                     f_strline.GetLength() - l_prevch);

        } else if (l_curch != -1) {

            y_offset = f_strline.Mid(l_prevch, l_curch - l_prevch);

            l_prevch = l_curch + 1;

            l_curch = f_strline.Find(_T(","), l_prevch);

            if (l_curch == -1) {

                z_offset = f_strline.Mid(l_prevch,
                                         f_strline.GetLength() - l_prevch);

            } else if (l_curch != -1) {
                z_offset = f_strline.Mid(l_prevch, l_curch - l_prevch);

            } else {
            }
        } else {
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMUtility::CloseUtility(
)
{
    DEL_PTR(m_utility)
}

DWORD CPQMUtility::AsciiToString(
    LPCTSTR f_chInputChar,
    LPTSTR f_chOutputChar
)const
{

    const int l_size = 2;
    int i = 0 , j = 0;
    TCHAR tmp[l_size] = {0};

    int len = _tcslen(f_chInputChar);

    for (i = j = 0; i < len; i += l_size, j++) {
        memset(tmp, NULL, l_size);
        tmp[0] = f_chInputChar[i];
        tmp[1] = f_chInputChar[i + 1];

        f_chOutputChar[j] = (TCHAR)(_tcstol(tmp, NULL, 16));
    }

    return TRUE;
}

//****************************Method Header************************************

//Method Name   :GetPQMStatusString

//Author        :PATNI/SSA

//Purpose       :Gets the Status of the PQM Application in a String value

//*****************************************************************************
CString CPQMUtility::GetPQMStatusString(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMUtility::GetPQMStatusString");

    CString status_string = _T("");	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    try {
        if (m_status == STATE_READY) {
            status_string = _T("STATE_READY");

        } else if (m_status == STATE_ACTIVE) {
            status_string = _T("STATE_ACTIVE");

        } else if (m_status == STATE_INACTIVE) {
            status_string = _T("STATE_INACTIVE");

        } else {
        }

    } catch (...) {

        //TMSC/2009Nov12/Modified/Code Changes

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME,
                  _T("Unknown Exception Occured"));

        throw;
        //TMSC/2009Nov12/Modified/Code Changes

        //WSE_LOG_EXCEPTION();
    }

    return status_string;
}



//+Patni-AMT/2010Apr07/Added/JaFT# IR-100 Shifted from View to Utility
//************************************Method Header************************************
// Method Name  : GetConfigFilePath
// Author       : PATNI/ ARD
// Purpose      : check the existence of configuration file and get the path if successful.
//***********************************************************************************
BOOL CPQMUtility::GetConfigFilePath(CString& f_config_file_path)
{
    //+Patni-ARD/2010Jan20/Modified/DeFT# IR 100
    wchar_t l_buffer[MAX_PATH] = {0};

    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    f_config_file_path  = l_buffer;
    f_config_file_path += L"\\";
    f_config_file_path += L"PQM\\PqmConfig.INI";

    //+Patni-ARD/2010Apr19/Deleted/JeFT# IR-100 Code Review IR100_CDR_07
    /*
    BOOL l_b_is_file_exists = FALSE;
    l_b_is_file_exists = l_file_find.FindFile(f_config_file_path);

    if (!l_b_is_file_exists) {
    */
    CFileFind l_file_find;

    if (!l_file_find.FindFile(f_config_file_path)) {
        //-Patni-ARD/2010Apr19/Deleted/JeFT# IR-100 Code Review IR100_CDR_07

        CPQMLogMgr::GetInstance()->WriteEventLog(_T("PQMConfig.ini does not exist."),
                ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        return FALSE;
    }

    return TRUE;
    //-Patni-ARD/2010Jan20/Modified/DeFT# IR 100
} //-Patni-AMT/2010Apr07/Added/JaFT# IR-100 Shifted from View to Utility



//****************************Method Header************************************

//Method Name   :GetPQMStatus

//Author        :PATNI/SSA

//Purpose       :Gets the Status of the PQM Application.

//*****************************************************************************
int CPQMUtility::GetPQMStatus(
)
{
    return m_status;
}

//****************************Method Header************************************

//Method Name   :SetPQMStatus

//Author        :PATNI/SSA

//Purpose       :Sets the Status of the PQM Application.

//*****************************************************************************
void CPQMUtility::SetPQMStatus(
    const int state
)
{
    m_status = state;
}

//****************************Method Header************************************

//Method Name   :GetPQMParentName

//Author        :PATNI/SSA

//Purpose       :Gets the Parent Name of the PQM Application.

//*****************************************************************************
CString CPQMUtility::GetPQMParentName(
)
{
    return m_parent_name;
}


//****************************Method Header************************************

//Method Name   :SetPQMParentName

//Author        :PATNI/SSA

//Purpose       :Sets the Parent Name of the PQM Application.

//*****************************************************************************
void CPQMUtility::SetPQMParentName(
    const CString& parent_name
)
{
    m_parent_name = parent_name;
}



//****************************Method Header************************************

//Method Name   :GetApplicationName

//Author        :PATNI/SSA

//Purpose       :Gets the full ApplicationName.

//*****************************************************************************
CString CPQMUtility::GetApplicationName(
)
{
    CString child_name ;
    child_name.LoadString(AFX_IDS_APP_TITLE);

    //return (CString(m_parent_name) + "." + child_name);
    return(child_name);
}

// Added By KT/ACE-2/15-06-2009/Multilingualisation
//************************************Method Header************************************
// Method Name  : GetMultiLingualString
// Author       : PATNI/ KT
// Purpose      : Get String for Internationalisation
//***********************************************************************************
CString CPQMUtility::GetMultiLingualString(
    const CString& f_str
)
{
    CString err_string =  MPlus::I18n::CCatalog::instance().get(f_str);    // Added By KT/ACE-2/15-06-2009/Multilingualisation

    if (err_string.IsEmpty()) {

        ASSERT(FALSE);
        err_string = f_str ;
    }

    return err_string;
}
// Added By KT/ACE-2/15-06-2009/Multilingualisation


//+Patni-AMT/2010Apr07/Added/JaFT# IR-100
//*****************************************************************************
//Method Name   : GetBoolKeyValue
//Author        : Patni/AMT
//Purpose       : Reads the key value from file opened by f_reader under f_section
//*****************************************************************************
bool CPQMUtility::GetBoolKeyValue(
    CIniReader& f_reader,
    const CString& f_key,
    const CString& f_section
)
{
    return (_wtoi(f_reader.GetKeyValue(f_key, f_section)) == 1) ? true : false;
}
//-Patni-AMT/2010Apr07/Added/JaFT# IR-100


//+Patni-PJS/2010Feb01/Added/MVC004689 code review/PROPOSAL_05
//************************************Method Header****************************
// Method Name  : GetSenderName
// Author       : PATNI/ PJS
// Purpose      : Returns sender name eg pagename.glname(Acquire.PQM)
//*****************************************************************************
CString CPQMUtility::GetSenderName(
)
{
    CString l_app_name = _T("");
    l_app_name.LoadString(AFX_IDS_APP_TITLE);

    return m_parent_name + _T(".") + l_app_name;//l_sender_name;
}
//-Patni-PJS/2010Feb01/Added/MVC004689 code review/PROPOSAL_05


//****************************Method Header************************************
//Method Name   :SetSenderParam
//Author        :PATNI
//Purpose       :Sets Sender param

//*****************************************************************************
void CPQMUtility::SetSenderParam(const CString& f_sender_param)
{
    m_sender_param = f_sender_param;
}

//****************************Method Header************************************
//Method Name   :SetSenderChannel

//Author        :PATNI

//Purpose       :Sets sender channel

//*****************************************************************************
void CPQMUtility::SetSenderChannel(const CString& f_channel)
{
    m_parent_channel = f_channel;
}


//************************************Method Header**************************************
//Method Name   :  CPQMUtility
//Author        :  PATNI/DJ
//Purpose       :  Default Constructor
//**************************************************************************************/
CPQMUtility::CPQMUtility()
{

}

//************************************Method Header**************************************
//Method Name   :  ~CPQMUtility
//Author        :  PATNI/DJ
//Purpose       :  Destructor
//**************************************************************************************/
CPQMUtility::~CPQMUtility()
{
}

//****************************Method Header************************************

//Method Name   :GetSenderChannel

//Author        :PATNI

//Purpose       :Retrieve sender channel

//*****************************************************************************
inline CString CPQMUtility::GetSenderChannel()const
{
    return m_parent_channel;
}

//****************************Method Header************************************
//Method Name   :IsCALAApplicationAlive
//Author        :PATNI

//Purpose       :
//*****************************************************************************
unsigned int CPQMUtility::WaitForCALAApplcationProcess(
    void* f_parameter
)
{
    LPCTSTR FUNC_NAME = _T("CPQMUtility::WaitForCALAApplcationProcess");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function"));
    BOOL l_bRetVal = FALSE;
    CPQMView* l_pqm_view = (CPQMView*)f_parameter;

    if (l_pqm_view && !l_pqm_view->IsKindOf(RUNTIME_CLASS(CPQMView))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Illegal class parameter"));
    }

    l_pqm_view->GetAcquisition()->SetAutoLocatorRunning(TRUE);
    l_pqm_view->UpdateEndExamButtonStatus();

    CString l_process_id = _T("");
    l_process_id.Format(_T("CALA Process ID : %d"), l_pqm_view->GetAutoLocatorController().GetAutoLocatorProcessID());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_process_id);

    HANDLE l_autolocator_wait = OpenEvent(EVENT_ALL_ACCESS, FALSE, PQM_AUTOLOCATOR_WAIT_FOR_REPLY);

    if (NULL != l_autolocator_wait) {
        SetEvent(l_autolocator_wait);
        CloseHandle(l_autolocator_wait);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENt set as GL_REGISTER message is received."));

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENT NOT FOUND."));
    }

    // Get a handle to the process.
    HANDLE l_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, l_pqm_view->GetAutoLocatorController().GetAutoLocatorProcessID());

    if (NULL != l_hProcess) {
        WaitForSingleObject(l_hProcess, INFINITE);
        l_bRetVal = TRUE;
        CloseHandle(l_hProcess);
    }

    l_pqm_view->GetAutoLocatorController().SetAutoLocatorProcessID(-1);
    l_pqm_view->GetAcquisition()->SetAutoLocatorRunning(FALSE);
    l_pqm_view->UpdateEndExamButtonStatus();

    _endthreadex(0);
    return l_bRetVal;
}

//****************************Method Header************************************
//Method Name   :WaitForGLParamSetReady
//Author        :PATNI

//Purpose       :
//*****************************************************************************
unsigned int CPQMUtility::WaitForGLParamSetReady(
    void* f_parameter
)
{
    LPCTSTR FUNC_NAME = _T("CPQMUtility::WaitForGLParamSetReady");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function"));

    BOOL l_bRetVal = FALSE;
    CPQMView* l_pqm_view = (CPQMView*)f_parameter;

    if (l_pqm_view && !l_pqm_view->IsKindOf(RUNTIME_CLASS(CPQMView))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Illegal class parameter"));
    }

    HANDLE l_fwda_event_handle = CreateEvent(NULL, TRUE, TRUE, PQM_WFDA_FOR_PARAM_SETREADY_REPL);

    if (NULL == l_fwda_event_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error while creating event object"));
    }

    ResetEvent(l_fwda_event_handle);

    if (WAIT_TIMEOUT == WaitForSingleObject(l_fwda_event_handle, WFDA_REPLY_TIME_OUT)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Time Out Occured while receiving RE_GL_ParamSet_READY messages"));

        l_pqm_view->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
        l_pqm_view->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPQMUtility::GetMultiLingualString(_T("IDS_FAIL_TO_EDIT_INSCAN_PROTOCOL_RESET_PAGE")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));

    }

    _endthreadex(0);
    return l_bRetVal;
}