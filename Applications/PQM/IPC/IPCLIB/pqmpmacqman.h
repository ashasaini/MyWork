//Redmine-783
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmpmacqman.h
 *  Overview: Structure of CPqmPmAcqman class.
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
#ifndef __SM_IPCLIB_PQMPMACQMAN_H__
#define __SM_IPCLIB_PQMPMACQMAN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/CoilDefinitions.h>	// Added by ClassView
#include "PQMMessageStructures.h"

class CSocketMsg;
class CPqm;
class CPqmProtocol;
class CScanProcedure;
class CPQMIPCManager;
class CPQMSocket;
class CScanTransaction;
class CPqmPrMode;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345
class CAcqManNotifier;
//class CVoiceManager;

// This class will be a manager for Acqman. It will have all the information
// required to locate the Acqman process. It will be the class through which
// the communication will happen with Acqman. This class will contain an object
// of ImplusConnector which will be used for initializing the channels, connecting,
// disconnecting etc. The communication will happen through channels using Mplus CFW.

// The channel name for communication with Acqman will be picked up from a
// configuration file. This class will also translate the message (ScanTransactions)
// that needs to be sent from PQM IPC to Acqman in the ImplusMessage format.

// It will also have the callback function in the Mplus required format to
// listen to the response received from Acqman. It can also send Asynchronous
// as well synchronous messages. This class will also contain the CScanProcedure.

// This CScanProcedure will contain the CScanTransaction objects.

//! The class is used to handle all the requests sent to Acqman.
//! It is also responsible for delegating the response sent by Acqman
//! to the respective Scan Transactions
/*! class CPqmPmAcqman */

class CPqmPmAcqman : private MR::ACQ::SM::COMMON::NonCopyable<CPqmPmAcqman>
{
    friend class CPqm;
    typedef std::map<int, CScanTransaction*> CScanTransactionMap;

public:

    void OnAcqmanMsg(const CSocketMsg& msg);
    void OnAcqmanDisconnect();
    void AutoScanStart();
    void ScanStart(CPqmProtocol* protocol, const BOOL retry);
    void AcqmanAbort();
    int  AcqDeleteLockFile();
    BOOL SendMessage(CScanTransaction* scantransaction, bool synchronous = false);
    bool ConnectToAcqman();
    void AutoScanRetry(CPqmProtocol* f_pqm_protocol = NULL);

    //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    void pqmAcqResetCopySourceProt(CVarFieldHandle* const vf_handle, CPqmProtocol* l_pstartobj);

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    status_t PqmAcqAutoCoilSet(CPqmProtocol* f_cds_protocol);

    BOOL CanAutoScanStart(const int f_topprotocol)const;

    void CheckDelayAndScanStartForFirstWaitProtocol(CPqmProtocol* f_top_protocol);
    void AutoScanTimerReset();
    void AcqmanAbortFromnMainThread();
    void AcqmanAbortFromScanThread();

    CString  GetStr(const int func)const;
    CPqm*    GetPqm()const;
    int      GetFunctionBit(const int function)const;
    CPQMSocket* GetPQMSocket()const;
    CScanProcedure* GetScanProcedure()const;
    CPqmProtocol* GetProtocol()const;
    CPqmPrMode*   GetProtocolModeLastScan()const;
    PqmSaturnCoil_t*    GetLastSaturnCoil();
    PqmSaturnRcvCoil_t* GetLastSaturnCoilTbl();


    CPqmProtocol* GetDelayStartObj()const;
    void SetDelayStartObj(CPqmProtocol* delay_start_obj);

    //+Patni-PJS-AP/2009Sep21/Added/MVC004818, MVC004938
    BOOL GetIsContinueScan() const;
    void SetIsContinueScan(const BOOL f_bcontinue_scan);
    //-Patni-PJS-AP/2009Sep21/Added/MVC004818, MVC004938

    //+Patni-HAR/2009Aug20/Added/Memory Leak Removal
    CScanTransaction* GetLastTransaction()const;
    void SetLastTransaction(CScanTransaction* f_scan_trans);

    void SetInjectTimePresent(const bool f_inject_time_present);
    bool GetInjectTimePresent() const;

    bool GetIsScanRetryFlag() const;

    int GetWaitingForAutoScanStartAcqOrder();

    void SetProtocolModeLastScan(CPqmPrMode* last_scan);
    bool CheckScanTimeLimitation();
    bool CalcSARControlAndCheckSAR(CPqmProtocol* f_current_protocol, bool f_scan_retry = false, const bool f_is_prescan_done = false);
protected:

    explicit CPqmPmAcqman(CPqm* pqmptr);
    virtual ~CPqmPmAcqman();

private:

    static CPqmPmAcqman*	m_acq_self_object;
    static int				m_timer_time;
    static int				m_delay_time;

    CAcqManNotifier*         m_acqmannotifier;
    CPqmProtocol*            m_current_protocol;
    CPqmProtocol*            m_delay_start_obj;
    CPqmPrMode*              m_last_scan;
    CPqm*                    m_pqmptr;
    CScanProcedure*          m_scan_procedure;
    CScanTransaction*        m_last_trans;
    CPQMSocket*              m_acqman_socket;
    char*                    m_scan_lockfile;
    CScanTransactionMap     m_map_uid_scan_transactions;
    // CProbeManager           *m_refprobe;
    // + V9.25 coil support@AMT
    PqmSaturnCoil_t		    m_last_saturn_coil;	    // Saturn Coil
    PqmSaturnRcvCoil_t	    m_last_saturn_tbl;	    // Saturn CoilDBtbl
    // - V9.25 coil support@AMT
    int						m_after_autoScan;
    BOOL					m_continue_scan;//Pankaj S

    int						m_waiting_for_auto_scan_acq_order;
    bool					m_inject_time_present;
    bool                    m_scan_retry;

    bool AddScanTransaction(int request_uid, CScanTransaction* scan_transaction);
    bool ConvertClientMsgToAscii(const AM_ClientMessageHeader_t& header, const AM_ClientToAcqMan_unicode_t& client_data, AM_ClientToAcqMan_t* client_data_ascii);
    void LogHeaderAndData(const AM_ClientMessageHeader_t&, const AM_ClientToAcqMan_unicode_t&);
    void LogHeaderAndData(const AM_ClientMessageHeader_t& f_received_header, const AM_AcqManToClient_t& f_received_body);
    int  AcqMakeLockFile();
    int  TlGetTmpPath(char* f_path);
    char* AcqGetLockFileName();
    //Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    status_t PqmCheckDisk(CPqmProtocol* f_protocol);
    bool CanAutoScanStartForWaitingProtocol();

    bool GetWirelessFilterAndMode(const int f_acq_order, int32_t& f_wireless_filter, int32_t& f_wireless_mode) const;

    CScanTransaction* GetScanTransaction(const int request_uid);
    CString GetStatusString(const int status);
    //Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
    UINT GetPQMSocketID() const;


    int  GetAfterAutoScan() const;
    void SetAfterAutoScan(const int f_after_autoScan);
    // + V9.25 coil support@AMT


    bool SetLastSaturnCoilTbl(PqmSaturnRcvCoil_t*  f_last_saturn_coil_tbl);
    bool SetLastSaturnCoil(PqmSaturnCoil_t* f_last_saturn_coil);
    // - V9.25 coil support@AMT


    bool CheckDelayTimeForTopProtocol(const CPqmProtocol* f_top_protocol);
    bool WaitForAutoScan(const CPqmProtocol* f_protocol);
    void AutoScanTimerSetUp(int f_time_out);
    static void WaitAutoStartTimerOutProc(int sig);
    int GetEarlyScanTimeFromIni();
    void AutoScanStartForFirstWaitProtocol(CPqmProtocol* f_protocol);
    void GetAutoSepIniFilePath(CString& f_str_filename);
    void CheckWhetherToWaitForAutoLocatorPlannedProt(CPqmProtocol* f_protocol);
    bool GetProtocolMaxScanTimeFlag(const int f_current_protocol, int32_t* f_temp_max_scan_time_flag) const;
    bool GetProtocolScanTimeData(const int f_current_protocol, ProtocolScanTimeData_t* f_protScanTime_data) const;
    bool CalcSARControlForAllAndCheckSAR(CPqmProtocol* f_current_protocol, bool f_scan_retry, const bool f_is_prescan_done = false);


};

#endif // #ifndef __SM_IPCLIB_PQMPMACQMAN_H__ 
