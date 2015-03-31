//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: acqinfo.h
 *  Overview: Structure of CAcqInfo class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2007/8/10 12:00:00     code restructured
                                                             after TA comments
 *****************************************************************************/
#ifndef __SM_IPCLIB_ACQINFO_H__
#define __SM_IPCLIB_ACQINFO_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libAcq/mCouchInfo.h>
#include "ScanTransaction.h"


class CPqm;
class CCoil;

// This class is responsible for Sending/Receiving AcqInfo related messages to/from AcqMan.
class CAcqInfo : public CScanTransaction
{
public:
    //Constructor / Destrcutor
    explicit CAcqInfo(CScanProcedure* f_scan_proc);
    virtual ~CAcqInfo();
    virtual bool PrepareRequest();

    //+Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
    //-Patni-ARD/2010Feb16/Added/CDS code review
    static status_t GetMovingBedInSaturn(VftCoilListInfo_t* f_coillist);
    //-Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement

    void SetAcqInfoMode(const int f_acqinfo_mode);

protected:

private:
    int m_acqinfo_mode;

    int  AcqInfoCallback(const int f_type, AM_AcqManToClient_t* const f_data);

    bool ErrAcqInfo(const int f_type, const int f_eror_id, const CString& f_errmsg,
                    AM_AcqManToClient_t* const f_response);

    int  CheckCoil(AM_AcqManToClient_t* const f_data, const int f_tx_coil_key,
                   const int f_rcv_coil_key, CCoil** f_new_coil)const;

    void         PrepareAcqInfo();
    status_t	 AcqMovingPVcheck(CPqmProtocol* const	curObj, const float couchPos , mCouchInfo_t*	CouchInfo);
    virtual void ProcessResponse(AM_ClientMessageHeader_t* f_hdr, AM_AcqManToClient_t* f_body);

    //+Patni/AKR/2010Aug12/Added/NFD001
    int AutoMapAutoShimming(CPqmProtocol* const f_protocol, CPqm* f_pqm, AM_AcqManToClient_t* const f_data)const;
    //+Patni/AKR/2010Aug12/Added/NFD001

    int  SetInfoCoil(AM_AcqManToClient_t* const f_data);

};

#endif // #ifndef __SM_IPCLIB_ACQINFO_H__
