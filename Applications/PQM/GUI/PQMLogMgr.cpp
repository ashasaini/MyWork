//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-777
//TMSC-REDMINE-774

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMLogMgr.cpp
 *  Overview: Implementation of CPQMLogMgr class.
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
#include "stdafx.h"
#include "PQMLogMgr.h"
#include <PQM/resource.h>
#include "PQMView.h"

using namespace MPlus::EH;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const CString SUB_SYSTEM_ACQUIRE  = _T("Acquire");
const CString SUB_SYSTEM_SCHEDULE = _T("Schedule");

CPQMLogMgr* CPQMLogMgr::m_pqm_log_mgr = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



//*************************************************************************
//Method Name   :   GetInstance
//Author        :   PATNI/AM
//Purpose       :   Returns the single Instance of CPQMLogMgr class
//*************************************************************************
CPQMLogMgr* CPQMLogMgr::GetInstance()
{

    if (m_pqm_log_mgr == NULL) {
        m_pqm_log_mgr = new CPQMLogMgr;
    }

    return m_pqm_log_mgr;
}

//*************************************************************************
//Method Name   :   ~CPQMLogMgr
//Author        :   PATNI/AM
//Purpose       :   Destroys the object (destructor)
//*************************************************************************
CPQMLogMgr::~CPQMLogMgr()
{
}


//*************************************************************************
//Method Name   :   WriteTraceLog
//Author        :   PATNI/AM
//Purpose       :   Writes TraceLog
//*************************************************************************
BOOL CPQMLogMgr::WriteTraceLog(
    LPCTSTR domain_name,
    unsigned long marker_val,
    LPCTSTR func_name,
    LPCTSTR log_text,
    bool detail_trace_log
)
{
    //+ MEITEC/2010Sep03/MVC008324
    try {
        if (m_pqm != NULL) {
            if (m_pqm->GetAcquisition() != NULL) {
                m_pqm->GetAcquisition()->WriteTraceLog(
                    domain_name, marker_val,
                    func_name, log_text, detail_trace_log);
                return TRUE;
            }
        }


    } catch (...) {
        FILE* fp = fopen("/MRMPlus/log/Trace/PQM_LogErr.txt", "at");

        if (fp) {
            SYSTEMTIME systime = {0};
            GetLocalTime(&systime);
            fprintf(fp, "%02d:%02d:%02d.%03d : PQMLogMgr::WriteTraceLog failed.\n",
                    systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
            fflush(fp);
            fclose(fp);
        }

    }

    return FALSE;

    //- MEITEC/2010Sep03/MVC008324
}

//*************************************************************************
//Method Name   :   WriteEventLog
//Author        :   PATNI/AM
//Purpose       :   Writes EventLog
//*************************************************************************
BOOL CPQMLogMgr::WriteEventLog(
    const CString& err_msg,
    const int error_level,
    const CString& app_location,
    const int error_line_no,// Patni-KSS/ADDED/2010May19/IR-82
    const bool detail_error_log
)const
{


    if (m_pqm != NULL) {
        if (m_pqm->GetAcquisition() != NULL) {
            BSTR errMsg = err_msg.AllocSysString();
            BSTR appLocation = app_location.AllocSysString();
            m_pqm->GetAcquisition()->WriteEventLog(
                errMsg,
                error_level,
                appLocation,
                error_line_no,	//Patni-MP/2010May24/Modified/IR-82
                detail_error_log);
            SysFreeString(errMsg);
            SysFreeString(appLocation);

            return TRUE;
        }
    }

    return FALSE;
}


//*************************************************************************
//Method Name   :   DisplayError
//Author        :   PATNI/RB
//Purpose       :   Display Error
//*************************************************************************
BOOL CPQMLogMgr::DisplayError(
    const CString& error_message,
    CString file_name,
    const UINT line_number,
    const int error_level,
    const DWORD message_id,
    CString event_source,
    const bool detail_error_log,
    const bool f_use_application_name_as_finename /*= true*/		//+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
)const
{

    //+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    if (f_use_application_name_as_finename) {
        file_name = APP_NAME;
        event_source = m_mode_str;
    }

    //-Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement

    if (m_pqm != NULL) {
        if (m_pqm->GetAcquisition() != NULL) {
            BSTR errMsg = error_message.AllocSysString();
            BSTR filename = file_name.AllocSysString();
            BSTR eventSource = event_source.AllocSysString();
            m_pqm->GetAcquisition()->PQMDisplayError(
                errMsg,
                filename,
                line_number,
                error_level,
                message_id,
                eventSource,
                detail_error_log);
            SysFreeString(errMsg);
            SysFreeString(filename);
            SysFreeString(eventSource);

            return TRUE;
        }
    }

    return FALSE;
}


//*************************************************************************
//Method Name   :   WriteOpeLog
//Author        :   PATNI/AM
//Purpose       :   Writes OperationLog
//*************************************************************************
BOOL CPQMLogMgr::WriteOpeLog(
    WORD operation_id
)
{

    if (m_pqm != NULL) {
        if (m_pqm->GetAcquisition() != NULL) {
            m_pqm->GetAcquisition()->WriteOpeLog(operation_id);
            return TRUE;
        }
    }

    return FALSE;
}


//********************************Method Header********************************
//Method Name   :DisplayScanError()
//Author        :PATNI/LK
//Purpose       :Display Error messages tht occur during scanning
//*****************************************************************************
void CPQMLogMgr::DisplayScanError(
    const int f_error_id,
    const CString& f_errmsg,										// Added By KT/ACE-2/15-06-2009/Multilingualisation
    const int f_errorlevel,
    CString f_errsource_filename,
    const bool f_use_application_name_as_finename /*= true*/		//+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
)const
{
    //+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    if (f_use_application_name_as_finename) {
        f_errsource_filename = APP_NAME;
    }

    //-Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement

    if (m_pqm != NULL) {
        if (m_pqm->GetAcquisition() != NULL) {
            m_pqm->GetAcquisition()->PQMDisplayScanError(
                f_error_id,
                (_bstr_t)f_errmsg ,						// Added By KT/ACE-2/31-07-2009/Multilingualisation
                f_errorlevel,
                (_bstr_t)f_errsource_filename);			// Added By KT/ACE-2/31-07-2009/Memory leak
            return;
        }
    }

}


//********************************Method Header********************************
//Method Name   :DisplayMessageInErrorView()
//Author        :PATNI/LK
//Purpose       : Display Error message in view
//*****************************************************************************
bool CPQMLogMgr::DisplayMessageInErrorView(
    const CString& f_errormessage,
    const DWORD f_errorlevel,
    CString f_errsrc_filename,
    CString f_errsource,
    const bool f_use_application_name_as_finename /*= true*/		//+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
)const
{

    //+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    if (f_use_application_name_as_finename) {
        f_errsrc_filename = APP_NAME;
        f_errsource = m_mode_str;
    }

    //-Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement

    //+Patni-MP/MODIFIED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    return ErrorDisplay::Instance().DisplayMessageInErrorView(f_errormessage,
            MPlus::EH::MPLUSERRORLEVEL_e(f_errorlevel),
            f_errsource,
            f_errsrc_filename
                                                             );
}


//+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
bool CPQMLogMgr::TracePerformance(
    __int64 f_Start,
    __int64 f_Stop,
    __int64 f_freq,
    CString f_msg,
    BOOL f_operation_end
)
{


    m_file = _tfopen(_T("C:\\MRMPlus\\Log\\Trace\\SMPerformanceLog.txt"), _T("a+"));

    if (m_file) {

        if (f_operation_end) {

            fwprintf(m_file, _T("\r\n\r\n **************************************************************************************** \r\n\r\n"));

        } else {

            unsigned long l_timediff = (unsigned long)(((f_Stop - f_Start) * 1000000) / f_freq);

            long  l_microsec = l_timediff % 1000;
            long l_millisec = l_timediff / 1000 ;
            long l_sec = l_millisec / 1000;
            l_millisec = l_millisec - l_sec * 1000;
            long l_minute = l_sec / 60;
            l_sec = l_sec - l_minute * 60;

            l_hr = l_minute / 60;
            l_minute = l_minute - l_hr * 60;

            fwprintf(m_file, _T("\r\n### TASK [%-80s] PERF. TIME : [%2ld:%2ld:%2ld:  %3ld:%3ld] ###"),
                     f_msg.GetBuffer(0), l_hr, l_minute, l_sec, l_millisec, l_microsec);

        }

        //  fflush(m_file);
        fclose(m_file);
        m_file = NULL;

    }

    return true;

}

void CPQMLogMgr::PrintTime(const CString& f_msg)
{

    m_file = _tfopen(_T("C:\\MRMPlus\\Log\\Trace\\SMPerformanceLog.txt"), _T("a+"));
    SYSTEMTIME l_time;
    GetLocalTime(&l_time);

    if (m_file) {

        fwprintf(m_file, _T("\r\n###{%-80s} System Time [%2ld:%2ld:%2ld:  %3ld]"), f_msg.GetBuffer(0), l_time.wHour, l_time.wMinute, l_time.wSecond, l_time.wMilliseconds);
        fclose(m_file);
        m_file = NULL;

    }
}
#endif
//- Patni-RJ/2009Mar29/Added/PSP- Performance code


//********************************Method Header********************************
//Method Name   :DisplayScanError()
//Author        :PATNI/LK
//Purpose       :Display Error messages tht occur during scanning
//*****************************************************************************
int CPQMLogMgr::DisplayMessageinWarningDialog(
    const CString& f_warn_message,
    const CString& f_warn_source,
    const int f_warn_level,
    const int f_warn_dialogtype,
    const int f_warn_buttonstype
)
{
    return ErrorDisplay::Instance().DisplayMessageInErrorDialog(
               f_warn_message,
               MPLUSERRORLEVEL_e(f_warn_level),//MPlus::EH::Crucial,
               MPLUSERRORDISPLAYMODE_e(f_warn_dialogtype), //MPlus::EH::SysModal,
               MPLUSERRORDIALOGBUTTONS_e(f_warn_buttonstype),//MPlus::EH::OkCancel,
               f_warn_source
           );
}


//*************************************************************************
//Method Name   :   WriteTraceLog
//Author        :   PATNI/AM
//Purpose       :   Writes TraceLog
//*************************************************************************
bool CPQMLogMgr::SetPqm(
    CPQMView* pqm
)
{
    if (m_pqm == NULL) {
        m_pqm = pqm;
        return true;
    }

    return false;

}

//********************************Method Header********************************
//Method Name   :SetModeString()
//Author        :Hemant
//Purpose       :Setting the mode string
//*****************************************************************************
void CPQMLogMgr::SetModeString(
    const int f_mode
)
{

    switch (f_mode) {
        case K_IMAGING :
            m_mode_str = SUB_SYSTEM_ACQUIRE ;
            break;

        case K_SCHEDULING:
            m_mode_str = SUB_SYSTEM_SCHEDULE ;
            break;

        default:
            m_mode_str = SUB_SYSTEM_ACQUIRE ;
    }
}

//*************************************************************************
//Method Name   :   CPQMLogMgr
//Author        :   PATNI/AM
//Purpose       :   Builds the object (constructor)
//*************************************************************************
CPQMLogMgr::CPQMLogMgr(
) :
    //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
    m_file(NULL),
#endif
    //- Patni-RJ/2009Mar29/Added/PSP- Performance code
    m_pqm(NULL)
{

}


//+Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
CPQMScopedTrace::CPQMScopedTrace(
    const CString& f_func_name
): m_func_name(f_func_name)
{

    if (!m_func_name.IsEmpty()) {


        PQM_TRACE(GUI_FUNCTION_MARKER_IN, m_func_name, m_func_name);
    }
}


CPQMScopedTrace::~CPQMScopedTrace(
)
{
    if (!m_func_name.IsEmpty()) {


        PQM_TRACE(GUI_FUNCTION_MARKER_OUT, m_func_name, m_func_name);
    }
}
//-Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
