//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescan.h
 *  Overview: Structure of CPreScan class.
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

#ifndef __SM_IPCLIB_PRESCAN_H__
#define __SM_IPCLIB_PRESCAN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"
#include "AcqSetFlag.h"

class CPqm;
class CPqmProtocol;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CPreScan : public CScanTransaction
{
public:

    //Constructor / Destructor
    explicit CPreScan(CScanProcedure* scan_procedure);
    virtual ~CPreScan();

    bool StartRFLPrescan(const int f_scan_id);
    bool RetryCFA(const int f_scan_id, const CPqmProtocol* f_protocol);
    void PQMManualPrescanApply(VARIANT* f_pdata);
    void PQMManualPrescanClose();
    //Patni+MP/2009Jun22/Modified/ACE-2/Missing Vantage Skip Prescan
    int  PreScanCheck();
    bool AcqProbingPreScanStart(const int f_scan_id, const CPqmProtocol* f_protocol);

    status_t AcqProbingScanStart(const CPqmProtocol* obj);
    status_t AcqProbingScanAbort();

    virtual bool PrepareRequest();

    //Patni+MP/2009Jun22/Modified/ACE-2/Missing Vantage Skip Prescan
    int  SetAgentAsScan(CPqmProtocol* f_curr_protocol);

protected:

    void AddRequest(AM_PreScanKind_t req);
    void AddFlag(CPSAcqSetFlag* flag);
    void CFARedraw(const bool redraw);
    void CFAReady()const;


    CPSAcqSetFlagVec& GetFlags();

    //Patni+MP/2009Jun22/Modified/ACE-2/Missing Vantage Skip Prescan
    void SetAgentRG(CPqmProtocol* f_current_protocol, char* data);
    void SetAgentCF(char* data);
    bool SetRequest();
    bool SetAction();


private:

    std::vector<AM_PreScanKind_t> m_request;
    CPSAcqSetFlagVec m_flag;


    BOOL ExecuteMrsPrescan();
    //Patni-AKR/2010Aug13/Added/NFD001
    void UpdateWaterFatSection(CPqmProtocol* f_curr_protocol, CPqm* f_pqmptr);
    bool ErrPrescan(const int type, const int error_id, AM_AcqManToClient_t* body);
    void PreScanDone(const int status, float couch_pos, const int couch_status);
    int  PreScanCallback(const int type, AM_AcqManToClient_t* data);
    void ProbeActionComplete() const;
    void SendMessagetoProbe(const int f_code) const;

    bool SendPreScan();

    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    void PqmAcqShimmingDone(CVarFieldHandle* const  f_vf_handle,
                            CPqmProtocol*			f_pqmprotocol,
                            const float				f_couchPos);

    status_t MaintainMotionProbingStudy(char* f_file, CPqmProtocol* f_protocol); //Patni-MSN/2009Mar02/Modified/RMC - Phase3-7 reported by TMSC
    virtual void ProcessResponse(AM_ClientMessageHeader_t* hdr, AM_AcqManToClient_t* body);


    CString GetErrorMessage(const int status, const int type);
    int     GetRequestCount()const;
    int     GetFlagCount()const;
    AM_PreScanKind_t GetRequest(const int index)const ;
    CPSAcqSetFlag*   GetFlag(const int index)const ;
    BOOL GetManualPreScanDefaultData(VARIANT** f_data);

    void SetPrescan(VARIANT* f_pdata);
};

#endif // #ifndef __SM_IPCLIB_PRESCAN_H__ 
