//Redmine-783
//Redmine-774
//Redmine-782
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
#include <ErrorDisp/ErrorDisplay.h>
#include <PQM/PQMLogMgr.h>
//Patni-Rajendra/2011Mar31/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <PQM/Resource.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"


//Patni-Rajendra/2011Mar31/Added/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;
using namespace  MPlus::EH;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPQMLogMgr* CPQMLogMgr::m_pqm_log_mgr = NULL;

const CString SUB_SYSTEM_ACQUIRE  = _T("Acquire");
const CString SUB_SYSTEM_SCHEDULE = _T("Schedule");

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
    DEL_PTR(m_error_log_inst);

    if (m_ope_log_inst != NULL) {
        m_ope_log_inst->DestroyInstance();
    }

    DEL_PTR(m_trace_log_inst);
}

//********************************Method Header********************************
//Method Name   :InitTraceLog()
//Author        :PATNI/MSN
//Purpose       :Initializing the TraceLog Path
//*****************************************************************************
BOOL CPQMLogMgr::InitTraceLog(
    int f_mode
)
{

    if (m_trace_log_inst == NULL) {
        CString  l_traceconfigpath = m_trace_config_path + _T("\\MRTraceConfig.ini");
        CString l_tracelog_path(_T(""));

        if (f_mode == K_SCHEDULING) {
            l_tracelog_path = m_tracelogpath + _T("\\SchedulePQMTraceLog.txt");

        } else if (f_mode == K_IMAGING) {

            l_tracelog_path = m_tracelogpath + _T("\\AcquirePQMTraceLog.txt");
        }

        m_trace_log_inst = new CMRTraceLog(m_tracelogflag ? true : false ,
                                           DEFAULT_TRACE_MARKER,
                                           l_traceconfigpath,
                                           l_tracelog_path);

        return TRUE;

    } else {
        return false;
    }
}

//********************************Method Header********************************
//Method Name   :InitEventLog()
//Author        :PATNI/MSN
//Purpose       : Initializing the event log Path
//*****************************************************************************
BOOL CPQMLogMgr::InitEventLog(
    int f_mode
)
{

    if (m_error_log_inst == NULL) {
        CString l_eventlog_path(_T(""));

        if (f_mode == K_SCHEDULING) {
            l_eventlog_path = m_eventlogpath + _T("\\SchedulePQMEventLog.txt");

        } else if (f_mode == K_IMAGING) {
            l_eventlog_path = m_eventlogpath + _T("\\AcquirePQMEventLog.txt");
        }

        //		AfxMessageBox(l_eventlog_path,MB_SYSTEMMODAL);
        m_error_log_inst = new CMRErrorLog(m_eventlogflag, l_eventlog_path);
        return TRUE;

    } else {
        return FALSE;
    }
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

    if (m_trace_log_inst != NULL) {
        //Patni-Rajendra/2011Mar31/Modified/REDMINE-1559
        CScopedLock l_scoped_lock(&m_trace_lock);

#ifdef DETAIL_LOG

        m_trace_log_inst->LogTrace(domain_name, marker_val,
                                   func_name, log_text);

#else

        if (!detail_trace_log) {

            m_trace_log_inst->LogTrace(domain_name, marker_val,
                                       func_name, log_text);

        }

#endif


    } else {

        return FALSE;
    }

    return TRUE;
}

//*************************************************************************
//Method Name   :   WriteEventLog
//Author        :   PATNI/AM
//Purpose       :   Writes EventLog
//*************************************************************************
BOOL CPQMLogMgr::WriteEventLog(
    CString err_msg,
    int error_level,
    CString app_location,
    int error_line_no,
    bool detail_error_log

)
{
    //+Patni-HAR/2010Mar10/Modified/MSA0251-00278
#if 0 //+Patni-MRP/2010Apr28/Modified/IR-82
    WriteTraceLog(DOMAIN_NAME, USER_FUNC_MARKER, app_location, err_msg);
#else
    MRERROR_LOG_DATA error_data;
    error_data.error_level = (MRERROR_LOG_LEVEL)error_level;

    error_data.error_message = err_msg;
    error_data.error_source = APP_NAME;
    error_data.error_source_file_name = app_location;
    error_data.l_line_number = error_line_no;//Patni-KSS/2010May19/ADDED/IR-82
    error_data.user_name = USER_NAME;

    //+Patni-MRP/2010Apr28/Modified/IR-82
    if (m_error_log_inst != NULL) {
        //Patni-Rajendra/2011Mar31/Modified/REDMINE-1559
        CScopedLock l_scoped_lock(&m_error_lock);

#ifdef DETAIL_LOG
        m_error_log_inst->WriteMessageToErrorLog(error_data);


#else

        if (!detail_error_log) {

            m_error_log_inst->WriteMessageToErrorLog(error_data);

        }

#endif

    } else {
        return FALSE;
    }

#endif
    return TRUE;
    //-Patni-HAR/2010Mar10/Modified/MSA0251-00278
}

//*************************************************************************
//Method Name   :   DisplayError
//Author        :   PATNI/RB
//Purpose       :   Display Error
//*************************************************************************
BOOL CPQMLogMgr::DisplayError(
    CString error_message,
    CString file_name,
    long line_number,
    DWORD error_level,
    DWORD message_id,
    CString event_source,
    bool detail_error_log,
    bool f_use_application_name_as_finename /*= true*/		//+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(detail_error_log);

    //+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    if (f_use_application_name_as_finename) {

        file_name.LoadString(IDS_ERROR_APP_NAME);
        event_source = m_mode_str;
    }

    //-Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    return CPQMLogMgr::DisplayMessageInErrorView(error_message,
            MPlus::EH::Information, file_name, event_source, f_use_application_name_as_finename);

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

    if (m_ope_log_inst != NULL) {
        m_ope_log_inst->ToWriteLog(SUBSYSTEM_ID, COMPONENT_ID, operation_id);

    } else {
        return FALSE;
    }

    return TRUE;
}

//********************************Method Header********************************
//Method Name   :DisplayMessageInErrorView()
//Author        :PATNI/LK
//Purpose       : Display Error message in view
//*****************************************************************************
bool CPQMLogMgr::DisplayMessageInErrorView(
    CString f_errormessage,
    DWORD f_errorlevel,
    CString f_errsrc_filename,
    CString f_errsource,
    bool f_use_application_name_as_finename /*= true*/		//+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
)
{


    if (f_use_application_name_as_finename) {

        f_errsrc_filename.LoadString(IDS_ERROR_APP_NAME);
        f_errsource = m_mode_str;

    }

    //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
    return ErrorDisplay::Instance().DisplayMessageInErrorView(f_errormessage,
            MPlus::EH::MPLUSERRORLEVEL_e(f_errorlevel),
            f_errsource,
            f_errsrc_filename
                                                             );

}



//********************************Method Header********************************
//Method Name   :DisplayMessageinErrorDialog()
//Author        :PATNI/LK
//Purpose       : Display error message in a dialog
//*****************************************************************************
int CPQMLogMgr::DisplayMessageinErrorDialog(
    MRERROR_LOG_DATA error_log_data,  MPlus::EH::MPLUSERRORLEVEL_e f_error_level /*= MPlus::EH::Information*/
)
{
    return ErrorDisplay::Instance().DisplayMessageInErrorDialog(
               error_log_data.error_message,
               f_error_level,
               MPlus::EH::SysModal,
               MPlus::EH::Ok,	// SM4 Himanshu 15 Jan 2009
               error_log_data.error_source
           );

}
//********************************Method Header********************************
//Method Name   :DisplayMessageinWarningDialog()
//Author        :PATNI/LK
//Purpose       :Display warning message in dialog
//*****************************************************************************
int CPQMLogMgr::DisplayMessageinWarningDialog(
    CString f_warn_message,
    CString f_warn_source,
    int f_warn_level,
    int f_warn_dialogtype,
    int f_warn_buttonstype
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
//********************************Method Header********************************
//Method Name   :DisplayScanError()
//Author        :PATNI/LK
//Purpose       :Display Error messages tht occur during scanning
//*****************************************************************************
void CPQMLogMgr::DisplayScanError(
    int f_error_id,
    CString f_error_id_str,
    int f_errorlevel,
    CString f_errsource_filename,
    bool f_use_application_name_as_finename /*= true*/		//+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
)
{
    LPCTSTR FUNC_NAME = _T("CPQMLogMgr::DisplayScanError");
    //    HINSTANCE curr_instance;
    CString l_errormessage = L"";
    //
    //    #ifdef _DEBUG
    //          curr_instance = GetModuleHandle(_T("IPCCOMd.dll"));
    //    #else
    //          curr_instance = GetModuleHandle(_T("IPCCOM.dll"));
    //    #endif
    //
    //    AfxSetResourceHandle(curr_instance);

    l_errormessage.LoadString(f_error_id);

    CString temp_string =  CPqmUtilities::GetMultiLingualString(f_error_id_str);    // Added By KT/ACE-2/15-06-2009/Multilingualisation


    if (l_errormessage.GetLength() <= 0) {

        //Patni-DKH/2010Jan06/Added/TMSC-REDMINE-1098-Part2
        l_errormessage = f_error_id_str ;
    }

    CString l_errsource(_T(""));
    l_errsource.LoadString(IDS_ERROR_APP_NAME);

    //+Patni-MP/modified/5/20/2010/IR-82
    CPQMLogMgr::GetInstance()->WriteEventLog(l_errormessage,
            f_errorlevel,
            f_errsource_filename, __LINE__ //Patni-KSS/2010May19/ADDED/IR-82
                                            );
    //-Patni-MP/modified/5/20/2010/IR-82

    //+Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement
    if (f_use_application_name_as_finename) {

        f_errsource_filename = l_errsource ;
        l_errsource = m_mode_str;
    }

    //-Patni-HEMANT/ADDED On 7/13/2009 12:15:44 PM/ ACE-2 / Error Display Requirement

    //     AfxSetResourceHandle(AfxGetInstanceHandle());


    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
        temp_string,						// Added By KT/ACE-2/15-06-2009/Multilingualisation
        f_errorlevel,
        l_errsource,
        f_errsource_filename
    );

    //l_errormessage.ReleaseBuffer();
    l_errormessage.Empty();
}

//+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
bool CPQMLogMgr::TracePerformance(
    __int64 f_Start,
    __int64 f_Stop,
    __int64 f_freq,
    CString f_msg
)
{

    m_file = _tfopen(_T("C:\\MRMPlus\\Log\\Trace\\SMPerformanceLog.txt"), _T("a+"));

    if (m_file) {
        unsigned long l_timediff = (unsigned long)(((f_Stop - f_Start) * 1000000) / f_freq);

        long l_microsec = l_timediff % 1000;
        long l_millisec = l_timediff / 1000 ;
        long l_sec = l_millisec / 1000;
        l_millisec = l_millisec - l_sec * 1000;
        long l_minute = l_sec / 60;
        l_sec = l_sec - l_minute * 60;

        long l_hr = l_minute / 60;
        l_minute = l_minute - l_hr * 60;

        fwprintf(m_file, _T("\r\n### TASK [%-80s] PERF. TIME : [%2ld:%2ld:%2ld:  %3ld:%3ld] ###"),
                 f_msg.GetBuffer(0), l_hr, l_minute, l_sec, l_millisec, l_microsec);
        //  fflush(m_file);
        fclose(m_file);
        m_file = NULL;

    }

    return true;

}


void CPQMLogMgr::TracePerformanceTime(
    unsigned long f_perf_time,
    CString f_log_message,
    bool f_operation_end
)
{


    m_file = _tfopen(_T("C:\\MRMPlus\\Log\\Trace\\SMMemoryPerformanceLog.txt"), _T("a+"));

    if (m_file) {

        long l_microsec = f_perf_time % 1000;
        long l_millisec = f_perf_time / 1000 ;
        long l_sec = l_millisec / 1000;
        long l_millisec = l_millisec - l_sec * 1000;
        long l_minute = l_sec / 60;
        l_sec = l_sec - l_minute * 60;

        long l_hr = l_minute / 60;
        l_minute = l_minute - l_hr * 60;

        if (f_operation_end) {
            fwprintf(m_file, _T("\n\nTASK [%s] \n PERF. TIME : [%2ld:%2ld:%2ld:%3ld:%3ld]\n\n***************************************************************************\n"),
                     f_log_message.GetBuffer(0), l_hr, l_minute, l_sec, l_millisec, l_microsec);

        } else {
            fwprintf(m_file, _T("\n\nTASK [%s] \n PERF. TIME : [%2ld:%2ld:%2ld:%3ld:%3ld] "),
                     f_log_message.GetBuffer(0), l_hr, l_minute, l_sec, l_millisec, l_microsec);
        }

        //  fflush(m_file);
        fclose(m_file);
        m_file = NULL;

    }

}

void CPQMLogMgr::PrintTime(CString f_msg)
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
//+ Patni-RJ/2009Apr02/Added/PSP- Performance code

//********************************Method Header********************************
//Method Name   :SetTraceLogUseCssdk()
//Author        :PATNI/MSN
//Purpose       :Sets the Trace Log Cssdk flag
//*****************************************************************************
bool CPQMLogMgr::SetTraceLogUseCssdk(int tracelogflag)
{
    m_tracelogflag = tracelogflag;
    return true;
}

//********************************Method Header********************************
//Method Name   :SetEventLogUseCssdk()
//Author        :PATNI/MSN
//Purpose       :Sets the Event Log Cssdk flag
//*****************************************************************************
void CPQMLogMgr::SetEventLogUseCssdk(int eventlogflag)
{
    m_eventlogflag = eventlogflag;
}

//********************************Method Header********************************
//Method Name   :SetTraceLogPath()
//Author        :PATNI/MSN
//Purpose       :Sets the Trace Log path
//*****************************************************************************
bool CPQMLogMgr::SetTraceLogPath(CString tracelogpath)
{
    m_tracelogpath = tracelogpath;
    return true;
}

//********************************Method Header********************************
//Method Name   :SetEventLogPath()
//Author        :PATNI/MSN
//Purpose       :Sets the Event Log path
//*****************************************************************************
void CPQMLogMgr::SetEventLogPath(CString eventlogpath)
{
    m_eventlogpath = eventlogpath;
}

//********************************Method Header********************************
//Method Name   :SetTraceConfigFilePath()
//Author        :PATNI/MSN
//Purpose       : Setting the config file path
//*****************************************************************************
void CPQMLogMgr::SetTraceConfigFilePath(CString trace_config_path)
{
    m_trace_config_path = trace_config_path;
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
CPQMLogMgr::CPQMLogMgr() :
    m_error_log_inst(NULL),
    m_ope_log_inst(NULL),
    m_trace_log_inst(NULL),
    m_trace_config_path(_T("")),
    m_tracelogpath(_T("")),
    m_eventlogpath(_T("")),
    m_tracelogflag(0),
    m_eventlogflag(0)
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //   PreLogOperation();

    m_ope_log_inst = &COperationLog::Instance();

}