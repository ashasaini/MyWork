/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMTraceInfo.cpp
 *  Overview: Implementation of CPQMTraceInfo class.
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
#include "PQMTraceInfo.h"
#include "pqm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BOOL trace_log_flag = TRUE;

CPQMTraceInfo* CPQMTraceInfo::m_traceinfo = NULL;

//************************************Method Header**************************************
//Method Name   :  GetInstance
//Author        :  PATNI
//Purpose       :  Creates CPQMTraceInfo instance if it is not created before.
//**************************************************************************************/
CPQMTraceInfo* CPQMTraceInfo::GetInstance()
{
    if (!m_traceinfo) {
        m_traceinfo = new CPQMTraceInfo();

        if (m_traceinfo) {
            return m_traceinfo;

        } else {
            return NULL;
        }
    }

    return m_traceinfo;
}


//****************************Method Header************************************

//Method Name   :~CPQMTraceInfo

//Author        :PATNI/ASG

//Purpose       :Destructor

//*****************************************************************************
CPQMTraceInfo::~CPQMTraceInfo(
)
{
    if (fp) {
        fclose(fp);
    }
}

//****************************Method Header************************************

//Method Name   :LogTrace

//Author        :PATNI

//Purpose       :Initialize level and trace message

//*****************************************************************************
void CPQMTraceInfo::LogTrace(TRACE_LEVEL level,
                             LPCTSTR trace_msg, ...
                            )
{
    CTraceInfo::CTraceInfo(level, trace_msg);
    LogTrace();
}

//****************************Method Header************************************

//Method Name   :LogTrace

//Author        :PATNI

//Purpose       :Writing to the file

//*****************************************************************************
void CPQMTraceInfo::LogTrace()
{
    //  TRACE_LEVEL trace_level = (CTraceInfo::TRACE_LEVEL) CPQMApp ::m_trace_level;

    //    if ( GetLevel()>= trace_level) {
    //        time_t ltime;
    //      time(&ltime);
    //
    //       CPTString str, time_str;
    //      time_str.Format(_T("%s"), _tctime(&ltime));
    //      time_str.TrimRight(_T('\n'));

    //      str.Format(_T("PQM [%s]: %s"), (LPCUSTR)time_str, GetTrace());

    //     _ftprintf(fp, _T("%s\n"), (LPCUSTR)str);
    //  }
}
//****************************Method Header************************************

//Method Name   :CPQMTraceInfo

//Author        :PATNI

//Purpose       :Default Constructor

//*****************************************************************************
CPQMTraceInfo::CPQMTraceInfo()
{
    //  CString trace_file_name = CPQMApp ::m_trace_file;
    //  fp = _tfopen(trace_file_name, _T("a+"));
}

//****************************Method Header************************************

//Method Name   :CPQMTraceInfo

//Author        :PATNI

//Purpose       :Parameterised Constructor

//*****************************************************************************
CPQMTraceInfo::CPQMTraceInfo(
    TRACE_LEVEL level,
    LPCTSTR trace_msg, ...
): CTraceInfo(
        level,
        trace_msg
    )
{
    if (trace_msg) {
        va_list argList;
        va_start(argList, trace_msg);
        FormatT(trace_msg, argList);
    }
}
