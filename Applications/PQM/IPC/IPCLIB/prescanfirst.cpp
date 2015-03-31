//Redmine-778
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescanfirst.cpp
 *  Overview: Implementation of CPreScanFirst class.
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
#include "PreScanFirst.h"
#include <PQM/PQMLogMgr.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//*******************************Method Header*********************************

//Method Name    : CPreScanFirst()

//Author         : PATNI/MRP

//Purpose        : Constructor

//*****************************************************************************
CPreScanFirst::CPreScanFirst(
    CScanProcedure* scan_proc
): CPreScan(
        scan_proc
    )
{
    LPCTSTR FUNC_NAME = _T("CPreScanFirst::CPreScanFirst");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPreScanFirst::CPreScanFirst"));

    AddRequest(ACQ_PRESCAN_TXCOIL);
    AddRequest(ACQ_PRESCAN_RXCOIL);
    AddRequest(ACQ_PRESCAN_RFLEVEL);
    AddRequest(ACQ_PRESCAN_CFA);
    AddRequest(ACQ_PRESCAN_RXGAIN);

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_FSY,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_CLINICAL));

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_RFL,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_CLINICAL));

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_RGN,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_CLINICAL));
}

//*******************************Method Header*********************************

//Method Name    : ~CPreScanFirst()

//Author         : PATNI/MRP

//Purpose        : Destructor

//*****************************************************************************
CPreScanFirst::~CPreScanFirst(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScanFirst::~CPreScanFirst");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPreScanFirst::~CPreScanFirst"));
}
