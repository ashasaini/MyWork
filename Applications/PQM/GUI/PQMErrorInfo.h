//Redmine-775
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMErrorInfo.h
 *  Overview: Structure of CPQMErrorInfo class.
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
#ifndef __SM_PQM_ERROR_INFO_H__
#define __SM_PQM_ERROR_INFO_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <libStudyManager/NonCopyable.h>
#include "ErrorInfo.h"
//TMSC-REDMINE-774
#include "PqmGuiConstants.h"


#define APPLICATION L"PQM-GUI"

class CPqmErrorInfo : public CErrorInfo,
    private MR::ACQ::SM::COMMON::NonCopyable<CPqmErrorInfo>
{
public:
    CPqmErrorInfo(
        ERR_SEVERITY severity,
        ERR_TYPE type,
        LPCTSTR file_name,
        unsigned int line_num);

    CPqmErrorInfo(
        int error_id,
        ERR_SEVERITY severity,
        ERR_TYPE type,
        LPCTSTR file_name,
        unsigned int line_num);

    ~CPqmErrorInfo();

    void ProcessError(int display_seconds = 0);

    virtual bool LogError();

private:
    void Copy(const CPqmErrorInfo& pqm_error_info);
};

#define PQM_ERROR1(typ)             LOG_ERROR1(CPqmErrorInfo, typ)
#define PQM_ERROR2(eid, sev, typ)   LOG_ERROR2(CPqmErrorInfo, eid, sev, typ)

#endif // #ifndef __SM_PQM_ERROR_INFO_H__
