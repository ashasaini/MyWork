//Redmine-783
//Redmine-781_CPP_TestTool
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: GateMode.h
 *  Overview: Structure of CGateMode class.
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
#ifndef __SM_IPCLIB_GATEMODE_H__
#define __SM_IPCLIB_GATEMODE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"
#include "LibPLTreeWrapper.h"

class CPqm;
class CPqmPmAcqman;
class CScanProcedure;
class CVarFieldHandle;


class CGateMode : public CScanTransaction
{
public:

    explicit CGateMode(CScanProcedure* f_scan_proc);
    virtual ~CGateMode();

    status_t AcqSetInfoGate(AM_AcqManGateInfo_t* f_gate);
    virtual bool PrepareRequest();

    void SetGateModeMethod(const int f_gate_method);

    inline void SetGateModeMethodOnscan(const bool f_gate_method_onscan) {
        m_gate_method_onscan = f_gate_method_onscan;
    }

private:

    CPqm* m_pqm_ptr;
    CPqmPmAcqman* m_acqman_ptr;

    int m_gate_method;
    bool m_gate_method_onscan;

    int GateModeCallback(const int f_type, AM_AcqManToClient_t* f_data);
    bool ErrGateMode(const int f_type, const int f_eror_id, AM_AcqManToClient_t* f_response);

    virtual void ProcessResponse(
        AM_ClientMessageHeader_t* f_header,
        AM_AcqManToClient_t* f_data);

    inline bool GetGateModeMethodOnscan() const {
        return m_gate_method_onscan;
    }
};

#endif // #ifndef __SM_IPCLIB_GATEMODE_H__ 
