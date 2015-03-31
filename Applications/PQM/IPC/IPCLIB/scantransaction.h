//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: scantransaction.h
 *  Overview: Structure of CScanTransaction class.
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

#ifndef __SM_IPCLIB_SCAN_TRANSACTION_H__
#define __SM_IPCLIB_SCAN_TRANSACTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/CoilDefinitions.h>
#include "PQMMessageStructures.h"

class CScanProcedure;
class CPqmProtocol;

class CScanTransaction : private MR::ACQ::SM::COMMON::NonCopyable<CScanTransaction>
{
public:
    //Constructor / Destructor
    explicit CScanTransaction(CScanProcedure* scan_proc);
    virtual ~CScanTransaction();

    bool SendControl(const int control_kind);
    int  AcqCheckSaturnCoil(VftCoilListInfo_t* f_coil_connect, PqmSaturnCoil_t* f_saturn_coil,
                            bool_t f_change_under_prot);
    int  AcquisitionStatusCallback(int type, AM_ClientMessageHeader_t* header, AM_AcqManToClient_t* data);

    int  CheckBreastImaging(const int f_recv_coil_id, const int f_patori_side_up, const int f_patori_end_in) const;
    status_t CheckBreastImagingInSaturn(
        //+Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
        //PqmSaturnRcvCoil_t *f_coil_t,
        VftCoilListInfo_t* f_coil_t,
        //-Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
        VftPatori_t* f_pat_ori);
    // - V9.25 coil support@AMT


    virtual bool SendMessage();
    virtual void ProcessResponse(AM_ClientMessageHeader_t* hdr, AM_AcqManToClient_t* body) = 0;
    virtual bool PrepareRequest() = 0;


    virtual AM_ClientMessageHeader_t GetHeader()const;
    void SetHeader(const AM_ClientMessageHeader_t& header);

    virtual AM_ClientToAcqMan_unicode_t GetData()const;
    void SetData(const AM_ClientToAcqMan_unicode_t& body);

    CScanProcedure* GetScanProcedure()const;

protected:
    int m_lastfunction;
    AM_ClientToAcqMan_unicode_t m_data;
    AM_ClientMessageHeader_t    m_header;

    DWORD   m_check_bit;
    CString m_check_str;
    float   m_couch_position;
    int     m_couch_status;
    int     m_acq_info_retry;
    double  m_fsyvalue;

    CScanProcedure* m_scan_procedure;

    virtual bool PostProcess(const float couch_position, const int couch_status,
                             const int lastfunction);

    BOOL CheckMovingCouch();
    virtual bool PreScanGo(const int f_last_function);

    //+Patni-Hemant/2009Mar03/Added/ACE-1/Venus Coil
    void PQMAcqPortString(PqmCoil_t* f_coil_p, char* f_port_string);
    //-Patni-Hemant/2009Mar03/Added/ACE-1/Venus Coil

    //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    bool CDSCheckShimCtrlLevel(CPqmProtocol* f_current_protocol);
    //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

private:

    bool PSGAcqInfoLevel()const;
    bool PSGRFInterLockLevel()const;
    bool PSGMoveCouchLevel();
    bool PSGShimCtrlLevel()const;
    bool PSGUnknownLevel()const;
    bool AcqSendMoveCouch(const int f_function, const float f_position)const;
    void AbortScan()const;    //Patni-HAR/2010Mar10/Added/MSA0251-00279
    void DisplayMoveCouchMessageinWarningDialog(const int f_errorid, const CString& f_errmsg,
            const int buttontype, const float f_offset);
    void AcquisitionWaitForStart(AM_AcqManToClient_t* data);
    void AcqChangeCoilList(VftCoilListInfo_t* f_coil_connect, int f_connect_flg[MAXIMUM_COIL_IN_PORT]);


    double GetCFValue()const;
    void   SetCFValue(const double fsyvalue);

    int GetCheckBit()const;
    //+Patni-Hemant/2010Mar16/Modified/ACE-Phase#3/CDS Requirement
    //bool SetCheckBit();
    bool SetCheckBit(bool& f_process_prescango);
    //-Patni-Hemant/2010Mar16/Modified/ACE-Phase#3/CDS Requirement

};
#endif // #ifndef __SM_IPCLIB_SCAN_TRANSACTION_H__ 

