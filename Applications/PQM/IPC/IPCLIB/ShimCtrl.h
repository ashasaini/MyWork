//Redmine-783
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ShimCtrl.h
 *  Overview: Structure of CShimCtrl class.
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
#ifndef __SM_IPCLIB_SHIMCTRL_H__
#define __SM_IPCLIB_SHIMCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"

class CInterLock;
class CScanTransaction;
class CPqmProtocol;

static PqmShim_t m_shim;

class CShimCtrl : public CScanTransaction
{
public:
    //Constrcutor / Destrcutor
    explicit CShimCtrl(CScanProcedure* scan_proc);
    virtual ~CShimCtrl();

    bool SendShimOffset(PqmShim_t* f_shim);

private:

    // Adjust the shim and sends CFA prescan msg to AcqMan and updates GUI
    void ShimmingAdjust()const;
    void ProcessResponse(AM_ClientMessageHeader_t* hdr, AM_AcqManToClient_t* body);
    bool PrepareRequest();
    bool SendShimFrequency(const double f_fsy_value);
    int  BeforePrescan(const int type, AM_AcqManToClient_t* body);
    bool ErrReport(const int type, const int errorId, const CString& err_msg, AM_AcqManToClient_t* body); // Added By KT/ACE-2/15-06-2009/Multilingualisation

    //Redundant - Definition Commented
    // void CloseShimming();
    // void ShimmingTimerSetup();

};

#endif // #ifndef __SM_IPCLIB_SHIMCTRL_H__
