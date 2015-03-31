//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescancoil.h
 *  Overview: Structure of CPreScanCoil class.
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

#ifndef __SM_IPCLIB_PRESCANCOIL_H__
#define __SM_IPCLIB_PRESCANCOIL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "PreScan.h"

class CPreScanCoil : public CPreScan
{
public:
    explicit CPreScanCoil(CScanProcedure* scan_proc);
    virtual ~CPreScanCoil();
};
#endif // #ifndef __SM_IPCLIB_PRESCANCOIL_H__ 