//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: interlock.h
 *  Overview: Structure of CInterLock class.
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
#ifndef __SM_IPCLIB_INTERLOCK_H__
#define __SM_IPCLIB_INTERLOCK_H__
#if _MSC_VER > 1000
#pragma once
#endif

#include <afxtempl.h>
#include "Coil.h"
#include "ScanTransaction.h"
#include "InterlockData.h"
#include "PQMMessageStructures.h"

class CScanProcedure;
class CPqmPmAcqman;
class CStudy;

class CInterLock : public CScanTransaction
{
public:

    explicit CInterLock(CScanProcedure* scan_proc);
    virtual ~CInterLock();
    BOOL GetInterLock(const int receiver_coilId, const int anatomy, const float weight, float*	inter_lock)const;
private:

    bool ErrReport(const int type, const int errorId,
                   const CString& err_msg,	AM_AcqManToClient_t* const body);

    int  BeforePrescan(const int type, AM_AcqManToClient_t* body);


    virtual void ProcessResponse(AM_ClientMessageHeader_t* hdr, AM_AcqManToClient_t* body);
    virtual bool PrepareRequest();



    int  GetLockValue(const int f_receiver_coil_id, const int f_weight_class);
    int  GetLockAnatomy(const int f_anatomy, const int f_weight_class);


};

#endif // #ifndef __SM_IPCLIB_INTERLOCK_H__ 
