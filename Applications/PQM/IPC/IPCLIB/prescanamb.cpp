//Redmine-778
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescanamb.cpp
 *  Overview: Implementation of CPreScanAMB class.
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
#include "PreScanAMB.h"
#include <PQM/PQMLogMgr.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//*******************************Method Header*********************************

//Method Name    : CPreScanAMB()

//Author         : PATNI/MRP

//Purpose        : Constructor

//*****************************************************************************
CPreScanAMB::CPreScanAMB(
    CScanProcedure* scan_proc
): CPreScan(
        scan_proc
    )
{

    LPCTSTR FUNC_NAME = _T("CPreScanAMB::CPreScanAMB:");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPreScanAMB::CPreScanAMB:"));

    AddRequest(ACQ_PRESCAN_RFLEVEL);
    AddRequest(ACQ_PRESCAN_CFA);
    AddRequest(ACQ_PRESCAN_RXGAIN);

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_FSY,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_USE_VARFIELD));

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_RFL,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_USE_VARFIELD));

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_RGN,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_USE_VARFIELD));
}

//*******************************Method Header*********************************

//Method Name    : ~CPreScanAMB()

//Author         : PATNI/MRP

//Purpose        : Destructor

//*****************************************************************************
CPreScanAMB::~CPreScanAMB(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScanAMB::~CPreScanAMB");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPreScanAMB::~CPreScanAMB"));

}
