//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescanrfl.h
 *  Overview: Structure of CPreScanRFL class.
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

#ifndef __SM_IPCLIB_PRESCANRFL_H__
#define __SM_IPCLIB_PRESCANRFL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "PreScan.h"

class CPreScanRFL : public CPreScan
{
public:
    explicit CPreScanRFL(CScanProcedure* scan_proc);
    virtual ~CPreScanRFL();
};

#endif // #ifndef __SM_IPCLIB_PRESCANRFL_H__ 
