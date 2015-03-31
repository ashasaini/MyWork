//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMLogMgr.h
 *  Overview: Structure of CPQMLogMgr class.
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
#ifndef __SM_PQM_LOG_MGR_H__
#define __SM_PQM_LOG_MGR_H__

#if _MSC_VER > 1000
#pragma once
#endif


#include <LogLibrary/MRTraceLog.h>
#include <LogLibrary/MRErrorLog.h>
#include <libStudyManager/Log/LogMgr.h>
#include <libStudyManager/NonCopyable.h>

using namespace MR::TRACELOG;
using namespace MR::ERRORLOG;


class CPQMView;


// ! This class is used for logging details of events,operations and traces

class CPQMLogMgr  : public MR::ACQ::SM::COMMON::LOG::ILogMgr,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMLogMgr>
{

public:

    static CPQMLogMgr* GetInstance();
    virtual ~CPQMLogMgr();

    BOOL WriteTraceLog(LPCTSTR domain_name, unsigned long marker_val,
                       LPCTSTR func_name, LPCTSTR log_text, bool detail_trace_log = false);

    BOOL WriteEventLog(const CString& err_msg, const int error_level,
                       const CString& app_location, const int error_line_no, const bool detail_error_log = false
                      )const;// Patni-KSS/ADDED/2010May19/IR-82

    BOOL DisplayError(const CString& error_message, CString file_name,
                      const UINT line_number, const int error_level,
                      const DWORD message_id , CString event_source, const bool detail_error_log = false,
                      const bool f_use_application_name_as_finename = true)const ;

    BOOL WriteOpeLog(WORD operation_id);

    void DisplayScanError(
        const int f_error_id,
        const CString& f_errmsg,		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        const int f_errorlevel,
        CString f_errsource_filename,
        const bool f_use_application_name_as_finename = true
    )const;
    bool DisplayMessageInErrorView(
        const CString& f_errormessage,
        const DWORD f_errorlevel,
        CString f_errsrc_filename,
        CString f_errsource,
        const bool f_use_application_name_as_finename = true
    )const;

#ifdef __SM_PERFORMANCE_CHECK__
    bool TracePerformance(
        __int64 f_Start,
        __int64 f_Stop,
        __int64 f_freq,
        CString f_msg,
        BOOL f_operation_end = FALSE
    );
    void PrintTime(const CString& f_msg)const;

#endif

    static int DisplayMessageinWarningDialog(
        const CString& f_warn_message,
        const CString& f_warn_source,
        const int f_warn_level,
        const int f_warn_dialogtype,
        const int f_warn_buttonstype
    );

    bool SetPqm(CPQMView* pqm);
    void SetModeString(const int f_mode);


private:

    CString m_mode_str;
    static CPQMLogMgr* m_pqm_log_mgr;

#ifdef __SM_PERFORMANCE_CHECK__
    FILE* m_file;
#endif

    CPQMView* m_pqm;
    CPQMLogMgr();
};

//Patni-PJS/2011May2/Modified/IR-97
#define PQM_TRACE(m,s1,s2) (CPQMLogMgr::GetInstance()->WriteTraceLog(_T("MRI_PQM"),m,s1,s2))


class CPQMScopedTrace
{
public:
    explicit CPQMScopedTrace(const CString& f_func_name);
    virtual ~CPQMScopedTrace();

private:
    CString m_func_name;
};

#define PQM_TRACE_SCOPED(func_name)       CPQMScopedTrace l_trace_obj(func_name);


#endif // #ifndef __SM_PQM_LOG_MGR_H__
