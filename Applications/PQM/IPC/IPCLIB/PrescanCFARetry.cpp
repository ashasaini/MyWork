//Redmine-778
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescancfa.cpp
 *  Overview: Implementation of CPreScanCFA class.
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
#include "PrescanCFARetry.h"
#include <PQM/PQMLogMgr.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//*******************************Method Header*********************************

//Method Name    : CPreScanCFA()

//Author         : PATNI/MRP

//Purpose        : Constructor

//*****************************************************************************
CPrescanCFARetry::CPrescanCFARetry(CScanProcedure* scan_proc
                                  ): CPreScan(
                                          scan_proc)
{

    LPCTSTR FUNC_NAME = _T("CPrescanCFARetry::CPrescanCFARetry");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPrescanCFARetry::CPrescanCFARetry"));

    AddRequest(ACQ_PRESCAN_CFA);

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_FSY,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_CLINICAL));

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_RFL,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_USE_PREVIOUS));

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_RGN,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_CLINICAL));
}
//*******************************Method Header*********************************

//Method Name    : ~CPreScanCFA()

//Author         : PATNI/MRP

//Purpose        : Destructor

//*****************************************************************************
CPrescanCFARetry::~CPrescanCFARetry()
{
    LPCTSTR FUNC_NAME = _T("CPrescanCFARetry::~CPrescanCFARetry");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPrescanCFARetry::~CPrescanCFARetry"));
}
