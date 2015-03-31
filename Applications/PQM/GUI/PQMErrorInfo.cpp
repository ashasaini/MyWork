/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMErrorInfo.cpp
 *  Overview: Implementation of CPQMErrorInfo class.
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
#include "PqmErrorInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//****************************Method Header************************************

//Method Name   :CPqmErrorInfo

//Author        :PATNI/JCM

//Purpose       :Default Constructor

//*****************************************************************************
CPqmErrorInfo::CPqmErrorInfo(
    ERR_SEVERITY severity,
    CErrorInfo::ERR_TYPE type,
    LPCTSTR file_name,
    unsigned int line_num
) : CErrorInfo(severity, type, file_name, line_num)
{

    ERR_SEVERITY svr = GetErrorSev();
    CString msg;
    msg.LoadString(GetErrorID());
    CString e_file_name = GetFileName();
    unsigned int e_line_num = GetLineNo();

    MPlus::EH::CErrorItem error_item(
        svr,
        msg,
        e_file_name,
        e_line_num,
        ERR_ID,
        APPLICATION,
        NULL,
        NULL,
        0);

    MPlus::EH::CErrorStatus m_error_status;

    m_error_status.SetErrorItem(error_item);
    //MPlus::EH::CErrorStatus error_status;
    //CString event_source = error_status.GetErrorItem(0).GetEventSource();
    CString event_source = m_error_status.GetErrorItem(0).GetEventSource();
    CString log_message(L"");

    log_message.Format(
        L"%s %s  %s  %d",
        L"The Errormessage is :",
        msg,
        e_file_name,
        e_line_num);

    MPlus::EvLog::CLogger error_log;

    error_log.Log(log_message, svr, L"PQM-IPC");

    CString e_message = msg;
    CString csview_field(L"PQM-IPC");

    MPlus::EH:: ErrorDisplay::Instance().
    DisplayMessageInErrorView(
        e_message,
        (MPlus::EH::MPLUSERRORLEVEL_e)0 ,
        csview_field);
}

//****************************Method Header************************************

//Method Name   :CPqmErrorInfo

//Author        :PATNI/JCM

//Purpose       :Parameter constructor with error description read from string
//               table, filename and line number
//*****************************************************************************
CPqmErrorInfo::CPqmErrorInfo(
    int error_id,
    CErrorInfo::ERR_SEVERITY severity,
    CErrorInfo::ERR_TYPE type,
    LPCTSTR file_name,
    unsigned int line_num
): CErrorInfo(error_id, severity, type, file_name, line_num
                 )
{
    ERR_SEVERITY svr = GetErrorSev();
    CString msg;
    msg.LoadString(GetErrorID());
    CString e_file_name = GetFileName();
    unsigned int e_line_num = GetLineNo();

    MPlus::EH::CErrorItem error_item(
        svr,
        msg,
        e_file_name,
        e_line_num,
        ERR_ID,
        APPLICATION,
        NULL,
        NULL,
        0);

    MPlus::EH::CErrorStatus m_error_status;

    m_error_status.SetErrorItem(error_item);
    //MPlus::EH::CErrorStatus error_status;
    //CString event_source = error_status.GetErrorItem(0).GetEventSource();
    CString event_source = m_error_status.GetErrorItem(0).GetEventSource();
    CString log_message(L"");

    log_message.Format(
        L"%s %s  %s  %d",
        L"The Errormessage is :",
        msg,
        e_file_name,
        e_line_num);

    MPlus::EvLog::CLogger error_log;

    error_log.Log(log_message, svr, L"PQM-IPC");

    CString e_message = msg;
    CString csview_field(L"PQM-IPC");

    MPlus::EH:: ErrorDisplay::Instance().
    DisplayMessageInErrorView(
        e_message,
        (MPlus::EH::MPLUSERRORLEVEL_e)0 ,
        csview_field);
}

//****************************Method Header************************************

//Method Name   :LogError

//Author        :PATNI/JCM

//Purpose       :This method logs the error into windows Application Log,
//               log file of Mplus Log
//*****************************************************************************
bool CPqmErrorInfo::LogError(
)
{
    return 0;
}

//****************************Method Header************************************

//Method Name   :Copy

//Author        :PATNI/JCM

//Purpose       :

//*****************************************************************************
void CPqmErrorInfo::Copy(
    const CPqmErrorInfo& pqm_error_info
)
{
}

//****************************Method Header************************************

//Method Name   :~CPqmErrorInfo

//Author        :PATNI/JCM

//Purpose       :Destructor

//*****************************************************************************
CPqmErrorInfo::~CPqmErrorInfo(
)
{

}

//****************************Method Header************************************

//Method Name   :ProcessError

//Author        :PATNI/JCM

//Purpose       :This method shows the error dialog to the user

//*****************************************************************************
void CPqmErrorInfo::ProcessError(
    int display_seconds
)
{
    MPlus::EH::CErrorStatus m_error_status;

    m_error_status.GetErrorItem(0).DisplayToUser(display_seconds, false);
}

