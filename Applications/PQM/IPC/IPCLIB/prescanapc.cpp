//Redmine-778
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescanapc.cpp
 *  Overview: Implementation of CPreScanAPC class.
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
#include "PreScanAPC.h"
#include <PQM/PQMLogMgr.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//*******************************Method Header*********************************

//Method Name    : CPreScanAPC()

//Author         : PATNI/MRP

//Purpose        : Constructor

//*****************************************************************************
CPreScanAPC::CPreScanAPC(
    CScanProcedure* scan_procedure
): CPreScan(
        scan_procedure
    )
{
    LPCTSTR FUNC_NAME = _T("CPreScanAPC::CPreScanAPC");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPreScanAPC::CPreScanAPC:"));

    AddRequest(ACQ_PRESCAN_RFLEVEL);

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_FSY,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_USE_PREVIOUS));

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

//Method Name    : ~CPreScanAPC()

//Author         : PATNI/MRP

//Purpose        : Destructor

//*****************************************************************************
CPreScanAPC::~CPreScanAPC(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScanAPC::~CPreScanAPC");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPreScanAPC::~CPreScanAPC"));
}
