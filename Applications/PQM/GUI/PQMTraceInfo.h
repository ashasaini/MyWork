//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMTraceInfo.h
 *  Overview: Structure of CPQMTraceInfo class.
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
#ifndef __SM_PQM_TRACE_INFO_H__
#define __SM_PQM_TRACE_INFO_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <PDataTypes/TraceInfo.h>

class CPQMTraceInfo : public CTraceInfo
{
public:
    static CPQMTraceInfo* GetInstance();

    virtual ~CPQMTraceInfo();
    virtual void LogTrace();
    void LogTrace(TRACE_LEVEL level, LPCTSTR trace_msg, ...);

private:
    static CPQMTraceInfo* m_traceinfo;
    FILE* fp;

    CPQMTraceInfo();
    CPQMTraceInfo(TRACE_LEVEL level, LPCTSTR trace_msg, ...);
};
#endif // #ifndef __SM_PQM_TRACE_INFO_H__
