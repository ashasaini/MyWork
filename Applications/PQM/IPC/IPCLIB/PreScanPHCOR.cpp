/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PreScanPHCOR.cpp
 *  Overview: Implementation of CPreScanPHCOR class.
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
#include "PreScanPHCOR.h"
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

//Method Name    : CPreScanPHCOR()

//Author         : PATNI/MRP

//Purpose        : Constructor

//*****************************************************************************
CPreScanPHCOR::CPreScanPHCOR(
    CScanProcedure* scan_proc
): CPreScan(
        scan_proc
    )
{
    AddRequest(ACQ_PRESCAN_RFLEVEL);
    AddRequest(ACQ_PRESCAN_CFA);
    AddRequest(ACQ_PRESCAN_RXGAIN);

    AddFlag(
        new CPSAcqSetFlag(
            CPSAcqSetFlag::K_PR_PRESCAN_FSY,
            CPSAcqSetFlag::PQM_VFC_PRESCAN_USE_PREVIOUS));

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

//Method Name    : ~CPreScanPHCOR()

//Author         : PATNI/MRP

//Purpose        : Destructor

//*****************************************************************************
CPreScanPHCOR::~CPreScanPHCOR()
{

}
