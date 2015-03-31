//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: scanprocedure.h
 *  Overview: Structure of CScanProcedure class.
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

#ifndef __SM_IPCLIB_SCANPROCEDURE_H__
#define __SM_IPCLIB_SCANPROCEDURE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <tami/libvf/varfields.h>
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <tami/vfStudy/vf_apptypes.h>
#include <tami/libSAR/sar.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <libStudyManager/NonCopyable.h>
#include "AcqSetFlag.h"

#include "Coil.h"
#include "PqmIpcLibConstants.h"
#include "AutoInsertConstant.h"


using namespace MPlus::EH;
class CPreScan;
class CScanTransaction;
class CPqm;
class CPqmProtocol;
class CPqmPrMode;
class CShimCtrl;
class CPqmProbe;
class CVarFieldHandle;
class COlpTemp; //+Patni-Ravindra Acharya/2010June22/Added/IR-153

enum SARMSG_CHECK_BIT {

    PQM_SAR_OK_BUT_COIL_CHK_BIT = 0x0001,
    PQM_SAR_OK_BUT_SAR_CHK_BIT =  0x0002,
    PQM_SAR_OK_BUT_RF_CHK_BIT = 0x0004,
    PQM_SAR_SAE_WARN_CHK_BIT = 0x0008,
};

// This class is responsible  for handling the scanning procedure. It will contain a
// vector of all the ScanTransactions. This class will take the decision about the
// next transaction that should be sent to Acqman depending on the parameter of the
// series as well as the return information got from Acqman
class CScanProcedure : private MR::ACQ::SM::COMMON::NonCopyable<CScanProcedure>
{
    friend class CPqmPmAcqman;

public:
    void AcquisitionScanPause()const;
    void AcquisitionScanResume();
    void ResetCurrentRequest();
    void AcquisitionGateInfo(const int f_mode);
    void PQMAcqSearchShim(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_curpqmprotocol,
                          const mode_autoshim shim_mode = SHIM_NORMAL_MODE, const float target_couch_pos = 0.0f);

    //+Patni-HAR/2010Feb10/Modified/CDS Requirement
    void pqmAcqCdsAbort(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_current_protocol);
    //-Patni-HAR/2010Feb10/Modified/CDS Requirement
    void AcquisitionScanAbort();
    void CheckNextProtocol(const int f_protcol_no, const BOOL f_call_frm_gui = FALSE)const; //+Patni-HAR-RG/2009June09/Modified/JFT#50

    //Patni-HAR/2010Apr13/Added/Phase#3 CDS Requirement
    void SendAcqInfoForCouch();
    //Patni-MP/2009Sep18/Added/MVC004774
    void SendAcqInfoRetry(const int f_mode);
    bool RetryCFA(const int f_scan_id, CPqmProtocol* f_protocol);
    bool StartRFLPrescan(const int f_scan_id);
    bool SendShimCtrl(PqmShim_t* f_shim);
    int  SendGateModeInfo(const int f_gate_method, Wireless_gating_t* const f_wirelessgateinfo, const bool f_gate_method_onscan = false);
    void PQMManualPrescanClose();
    void PQMManualPrescanApply(VARIANT* f_pdata);
    int  SendElectrodeType(const int);
    BOOL SendAcqInfoForCoil(CPqmProtocol* f_protocol);
    bool AddTransaction(CScanTransaction* scan_transaction);
    bool AcqProbingScanAbort();
    bool SendAcquisition();
    bool AcqProbingPreScanStart(CPqmProtocol* f_curr_protocol);
    void APCStart();
    void CoilSelectionClosed(const BOOL f_ok_pressed);
    //+Patni-MP/2009Mar28/Added/PSP Internal Defect
    void SendMessagrToUIForEndWaitCursor()const;
    //-Patni-MP/2009Mar28/Added/PSP Internal Defect

    bool WarningScan(const int f_scanwarning_code,
                     const CString&  f_warning_source,
                     const int f_scanwarning_buttonstype = SCAN_WARN_OK_CANCEL_BUTTONS,
                     int f_scanwarning_level = SCAN_WARN_INFORMATION,
                     const int f_scanwarning_dialogtype = SCAN_WARN_SYSMODAL,
                     const bool f_continue = true);

    bool WarningSAR(const int f_sarwarning_code, const CString& f_sarwarn_source,
                    const bool f_reset_current_protocol_status = true,  //+Patni-NP/2010Jan7/Added/MVC06663
                    const int f_sarwarning_buttonstype = SCAN_WARN_OK_CANCEL_BUTTONS,
                    int f_sarwarning_level = SCAN_WARN_INFORMATION,
                    int f_sarwarning_dialogtype = SCAN_WARN_APPMODAL);

    bool WarningSARAfterAcqmanResponseForPrescan(const status_t f_sts_sar, CPqmProtocol* f_curr_protocol);
    MPLUSERRORUSERRESPONSE_e DisplayMessageForSAR(const status_t f_sts_sar) ;

    void AcquisitionScanRestart();
    //Patni-Ravindra Acharya/2010June24/Added/IR-153
    COlpTemp* CreateOLPTransaction();
    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    status_t PqmAcqAutoCoilSet(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_cds_protocol);
    void pqmAcqCDSContinueCB()const ;
    void WarningPreScan(const int flg, const CString& f_errormsg, CCoil* newcoil);				// Added By KT/ACE-2/11-06-2009/Multilingualisation
    BOOL IsHasParent()const;
    int  CheckParentCouchOffset(const float f_couch_position);
    void CFAOpen(CPqmProtocol* const f_pqmprotocol); // SHIMMING Lokesh 15 March 2008
    bool IsCouchMoving()const;
    bool SendInterLock();
    int  MoveCouchProhibitionCheck(CPqmProtocol* f_protocol);

    //Patni-DKH/2010May03/Modified/Phase3#IR105
    void PQMAcqSearchRFL(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_curpqmprotocol, bool f_isreopenstudy = false);
    void PQMAcqSearchCFA(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_curpqmprotocol, bool f_isreopenstudy = false);
    BOOL CheckPlan(const CPqmProtocol* protocol);
    void PQMAcqReuseShim(CVarFieldHandle* const f_vf_handle, int	f_prot);
    bool IsCFValueChanged()const;
    void pqmPmCoilSelCdsTimeOut();
    void ScanContinue();
    void AcqSendMoveCouch(const int	f_function, const float f_position);

    static void pqmPmCoilSelCdsTimeOut(int sig);
    static void RecalcSARControlAndAutoScanStartStatic(int f_sig);
    bool ContinueScanAfterSAESet();

    const VftTimeCtrlParam_t* GetAvTimeCtrl() const;
    void SetCurrentDynamicRepeat(const int repeat);

    bool CanAbortProbeOnPrescanError() ;
    bool IsSingleProcRunning() const;
    bool CanProceedCFARetry() const;

    bool CanInvokeProbe(CString& f_error_message, bool& f_display_dialog);
    bool SendSAEInfo(const int32_t f_total_sae);

    int  GetProcessing()const;
    void SetProcessing(const int processing);

    int  GetRequest()const;
    void SetRequest(const int req);

    CPqmProtocol* GetScanProcProtocol()const;
    void SetScanProcProtocol(CPqmProtocol* protocol);

    int  GetTempMode() const;
    void SetTempMode(const int f_temp_mode) ;

    int  GetScanAbort()const;
    void SetScanAbort(const int scan_abort);

    int  GetAPCHold()const;
    void SetAPCHold(const int apc_hold);

    int  GetProbing()const;
    void SetProbing(const int probing);

    int  GetCFARetry()const;
    void SetCFARetry(const int f_cfa_retry);

    BOOL GetAPCRetry()const;
    void SetAPCRetry(const BOOL apc_retry);

    int  GetProt()const;
    void SetProt(const int prot);

    int  GetPreScanStatus()const;
    void SetPreScanStatus(const int f_prescan_status);

    bool GetIsScanning()const;
    void SetIsScanning(const bool scan);


    virtual CPSAcqSetFlagVec* GetPrevAction();
    virtual void SetPrevAction();

    bool GetCFAClinical()const ;
    void SetCFAClinical(const bool cfa_clinical);

    BOOL GetPrescanHook()const;
    void SetPrescanHook(const BOOL f_prescan_hook);

    int  GetScanStart()const;
    void SetScanStart(const int scan_start);

    int  GetScanPause()const;
    void SetScanPause(const int scan_pause);

    virtual CPSAcqSetFlagVec* GetCurAction();
    virtual void SetCurAction(CPSAcqSetFlagVec* cur_action);

    int  GetPrescan()const;
    void SetPrescan(const int prescan);

    int  GetAcqInfoRetry()const;
    void SetAcqInfoRetry(const int f_apq_info_retry);

    BOOL GetBreathHold()const;
    void SetBreathHold(const BOOL breathhold);

    bool GetWaitForVoice()const;
    void SetWaitForVoice(const bool waitforvoice);

    double GetCFValue()const;
    void   SetCFValue(const double fsyvalue);

    BOOL GetEnablePrepVoice()const;
    void SetEnablePrepVoice(const BOOL enable_prep_voice);

    BOOL GetWaitForPrepScan()const;
    void SetWaitForPrepScan(const BOOL wait_for_prep_scan);

    void SetIsCdsWaitCursor(const bool f_cdswaitcursor);

    PqmShim_t* GetPqmShimInfoptr()const;
    CScanTransaction* GetCurrentTransaction()const;
    CPqmPmAcqman* GetAcqManager()const;
    CShimCtrl* GetShimCtrl();
    CPqm*      GetPqmPtr();
    BOOL  GetManualPreScanDefaultData(VARIANT** f_pdata);
    //+saurabh RMC
    CPqmProbe* GetPqmProbe();
    bool     GetGateInfo(const int f_method);
    CString  GetScanProc(const int index)const;
    int GetAvAcqTime()const;
    int GetAvTp1Duration()const;
    int GetAvTp1Ctrl()const;
    int GetAvTp1Margin()const;


    void SetControlKind(const int control_kind);
    void SetDynamicVoice();
    bool SetPSData(CVarFieldHandle* const vf_handle, DWORD chk_bit);
    int  SetCoilName(CVarFieldHandle* const vf_handle, CPqmProtocol* protocol , CCoil* rcv_coil);
    virtual void SetCouchInfo(const int position, float couch_position, int couch_status);

    void SetIsCoilOrGateInfoExecuting(const bool f_is_coil_or_gateinfo_executing) ;
    void SetIsGateExecuting(const bool f_is_gate_executing) ;

    PQM_SCAN_MODE GetScanMode() const;
    void SetScanMode(const PQM_SCAN_MODE f_scan_mode);

    int GetProcedureCount() const;
    int GetStatusForCheckSAR(CPqmProtocol* f_current_protocol, const bool f_is_prescan_done);
    int HandleSARErrorForStatus(const int f_sar_status, const bool f_scan_retry);
    void RecalcSARControlAndAutoScanStart();
    void Set10SecOverWaitCount(const int f_10sec_over_wait_count);
    bool GetIsWaitingFlagForSARCtrl() const;
    void WaitTimeTimerReset(CPqmProtocol* f_scanning_protocol);

protected:

    static CScanProcedure* m_self_object ;

    explicit CScanProcedure(CPqmPmAcqman* acq_manager);
    ~CScanProcedure();

    virtual CPreScan* CreatePreScan(DWORD chk_bit);
    void    ScanCancelCB();

    //+Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion
    void ForceParentCB();
    void ForceCoilStatusCB();
    //-Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion

    void DeleteAllScanTransactionObjs();
    //+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
    void PqmAcqAfterCDSScanStart(CPqmProtocol* f_cds_protocol)const;
    void OnCoilSelectionCancel();
    void OnCoilSelectionOK();
    //-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
    bool GetIsCoilOrGateInfoExecuting() const;
    bool GetIsGateExecuting() const;


private:


    VftTimeCtrlParam_t  m_av_time_ctrl;     /* NULL */
    CPqm*                     m_pqm_ptr;
    CPqmPmAcqman*             m_acq_manager;
    vector<CScanTransaction*> m_acqscan_transaction;
    CPSAcqSetFlagVec*         m_prev_action;
    CPSAcqSetFlagVec*         m_current_action;
    CPqmProtocol*             m_pqm_protocol;
    CPqmPrMode*               m_last_acquisition;
    CScanTransaction*         m_curr_transaction;
    CShimCtrl*                m_shim_ctrl;

    //+Patni-Ravindra Acharya/2010June10/Added/IR-153
    COlpTemp*                 m_olp_temp ;

    bool   m_is_coil_or_gateinfo_executing;
    bool   m_is_gate_executing;
    bool   m_scanning;
    int    m_cfa_retry;
    bool   m_cfa_clinical;
    BOOL   m_apc_retry;
    bool   m_wait_for_voice;

    int    m_request;
    int    m_apc_hold;
    int    m_probing;
    int    m_prot;
    int    m_prescan;
    int    m_processing;
    int    m_acq_info_retry;
    BOOL   m_breath_hold;

    //+Patni-Ravindra Acharya/2010June09/Added/IR-153
    /* for Pre - Scan */
    int    m_temp_mode ;

    /* for Auto Voice */
    int    m_av_acq_time;      /* 0 */
    int    m_av_tp1_duration;      /* 0 */
    int    m_av_tp1_margin;        /* 0 */
    int    m_av_tp1_ctrl;      /* 0 */
    int    m_current_dynamic_repeat;

    BOOL   m_wait_for_prep_scan;
    BOOL   m_enable_prep_voice;

    int    m_control_kind;
    vector<CString> m_scan_proc;

    int    m_scan_start;
    int    m_scan_abort;
    int    m_scan_pause;
    double m_fsyvalue;
    int    m_prescan_status;
    bool   m_cfvaluechanged;
    // + SHIMMING Lokesh 15 March 2008
    PqmShim_t* m_pqmshiminfo;
    // - SHIMMING Lokesh 15 March 2008
    BOOL   m_prescan_hook;
    //+saurabh
    CPqmProbe* m_pqm_probe;
    //-saurabh
    PQM_SCAN_MODE m_scan_mode;

    int   m_sar_msg_bit;
    bool m_cdswaitcursor ;
    mutable int  m_10sec_over_wait_count;
    bool		 m_is_waiting_for_scan_sarctrl;

    int  AcqGetPVcoilIDTable(int* f_coilkey, int* f_pvcoilnum);
    void PrepDelayDelete();
    void ForceRMStatusCB();
    void pqmForceParentCB();
    int  ReadScanProc();
    int  CopyTuneInfo(float& txtunePos, float& rflPos);
    int  CopyCouchInfo(int& scanNumber, float& couchPos, float& homePos);
    void AcqModeSet(CPqmProtocol* protocol);
    void AcquisitionAPCStart();
    int  VisualPrepDelayCheck(CPqmProtocol* f_protocol);	// + SM4 Himanshu VISUAL_PREP 13 JAN 2009
    int  PqmPrGetShimmingOffset(CVarFieldHandle* const f_vf_handle, VfPathElem_t*	f_path, int32_t f_depth,
                                PqmShim_t*		f_shim);
    bool CheckToDisplayCdsInDoNotShow(CPqmProtocol* f_cds_protocol);
    virtual bool ScanStart();

    int CheckSARforSARControl(CPqmProtocol* f_protocol, const bool f_is_prescan_done) const;

    int GetShimState(int& shimState);
    int GetControlKind()const;
    //+Patni-Ravindra Acharya/2010June22/Added/IR-153
    COlpTemp* GetOlpTemp()const ;

    void SetScanProc(const CString& scan_proc, const int index);
    int  SetTPCTRLAutoVoice(CVarFieldHandle* const f_vf_handle, CPqmProtocol* protocol);
    void SetAvAcqTime(const int avacqtime);
    void SetAvTp1Ctrl(const int avtp1ctrl);
    void SetAvTp1Duration(const int avtp1duration);
    void SetAvTp1Margin(const int av_tp1_margin);
    BOOL SetPVCoil(const int  port, int* rcv_coil_tbl, const int n);
    void SetPqmShimInfo(PqmShim_t* f_pqmshim);

    void ResetSARMsgCheckBit();
    int GetSARMessageCheckBit(const int f_functionbit)const;
    bool GetSARMessageDisplayInformation(const status_t f_sts_sar,
                                         CString& f_sarwarning,
                                         int& f_sarwarning_buttonstype,
                                         int& f_sarwarning_level) const;
    const CString GetSARMessageString(const status_t f_sts_sar) const;
    const CString GetMessageStringForSARStatus(const int f_msg_code) const;
    bool GetSAROKFlagForCoilAndAnatomy(CPqmProtocol* f_current_protocol) const;
    int GetMaybeSARIDForStatus(const int f_sar_status) const;


};
#endif // #ifndef __SM_IPCLIB_SCANPROCEDURE_H__ 
