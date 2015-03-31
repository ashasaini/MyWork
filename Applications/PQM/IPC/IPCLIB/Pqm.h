//Redmine-783
//Redmine-774
//Redmine-777
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Pqm.h
 *  Overview: Structure of CPqm class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6
                                                            requirements & Formatted
 *   3.0          Patni/LK         2008/5/10 12:00:00     Restructured after TA
                                                            review
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQM_H__
#define __SM_IPCLIB_PQM_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include <tami/libms/gdcModeDecision.h>
#include <tami/libms/harmonizationModeDecision.h>
#include <tami/libinit/Init.h>
#include <tami/libSAR/sar.h>
#include <libStudyManager/NonCopyable.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
//All this header files are related with PASSCOM and DICTIONARY LIBRARY.

#include "ILicenseInfo.h"
#include "IObjectManager.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "DataManager.h"    // Vantage CoilDatabse support @ AMT
#include "PqmProcon.h"
//Patni-PJS/2010May09/Added/JaFT# IR-141
#include "PqmAuditLog.h"
#include "PqmShimSharedData.h"
#include "CoilSelectionTool.h"
//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
#include "pqmCDS.h"
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

#include "AutoInsertProcedure.h"
#include "AutoLocatorParameterSetter.h"
#include "TiPrepUpdater.h"

#include "VarFieldHandle.h"
#include "PqmPrMode.h"
#include "pqmstudy.h"
#include "PqmProtocol.h"
#include "GateParamSetter.h"
#include "DefaultPGUIDXmlDataManager.h"


using namespace MR::ACQ::SM::COMMON;

class CPQMIPCManager;
// MEITEC/2010Apr23/Added/IR86
class CPQMDebugInfo;
class CPqmPmAcqman;
class CCouchConfig;
class CSharedMemory;

class CPASCOMSequence;
class CColor;
class CColorSetting;
class CPQMStudy;
class CCoilData;
class CCoil;
class CPASCOMInterface;
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//class CCoilDB;
class CIRSListner;
class CPQMStudyMgr;
class CPQMSAR;
class CPqmSpeeder;
class CPQMdBdt;
class CCFShiftCheck;
class CCoilChannelMode;
class CpqmAMB;
class CPqmPmSARManager;

//+Patni-HEMANT/ADDED On 6/12/2009 1:02:11 PM/ ACE-2 / WFDA
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
class CWFDAImplementer;
class CWFDAHandlerInterface;
*/
//-Patni-PJS/2010Feb2/Modified/IR-97
//-Patni-HEMANT/ADDED On 6/12/2009 1:02:11 PM/ ACE-2 / WFDA

class CPlanDuplicateSettings;
class CMasterSlave;
class CAutoLocatorPostProc;
class CAutoLocatorParameterSetter;
//class CAutoMapManager;
//class CAutoShimManager;
//This class is the main controller class for the PQM application. It holds the major
//responsibility of  reading all the initial information like the couch information
//and the study information from the database. The couch information will be read
//using Mplus Config. It also contains the CPQMPmAcqman, which manages all the
//communication with Acqman. It is also the main class which will expose the
//required methods to the COM interface for communication with the PQM GUI.

extern Data_decl_list  g_sequence_param_name_list;

class CPqm : IObjectManager, ILicenseInfo
    , private MR::ACQ::SM::COMMON::NonCopyable<CPqm>
{
private:
    BOOL m_cala_appln_running;
public:


    explicit CPqm(int f_mode);

    ~CPqm();


    BOOL GetScanMutex(VARIANT** pData);
    BOOL GetStudyLock(VARIANT** pData);
    void WriteToUI(const pqm_msg_tag Msg)const;
    bool PqmPmIsActive(const int type)const;
    bool IsDialogActive(const int f_dialog_type) const;
    bool IsSaturnCoilSystem()const ;
    bool DeleteAllGateIteams();
    bool AddToGateList(const CString& f_gate_info);
    bool RaiseInstancesAuditEvent();
    bool IsActiveDialog();
    void ProlimAutoPlan(const int);
    //Patni-KSS/2010Oct08/Added/V1.35#IR-167
    bool ProcessProbeMsgForIRS(const pqm_msg_tag f_msg)const;
    //Patni-Sribanta/2010Nov15/Added/IR-170 //IR-170_Impact_Scenario
    bool ShowSequenceOnGUI(const int* f_acq_order, const CString& f_pas_path, const int f_actual_seq_transfered, const int f_total_seq = 1) ;
    void WriteDebugDataToUI(CString f_csDebugData);
    void UpdateSARInfoOnPage();
    bool InitializePqm(/*CString study_uid*/);
    bool IsAcqmanValid() const;
    int  ConnectProCon();
    bool ScanSeries(VARIANT* f_pData, const int f_pos);
    bool LoadStudyLOID(LPCTSTR f_patientloid, LPCTSTR f_studyloid, LPCTSTR f_pas_path);  // Load DBSA study file
    BOOL DeleteAll(BSTR bStr);
    bool UpdateComment(BSTR bStr, BSTR scancomment);
    void SendSequencesToMemory(VARIANT* pdata);
    void SendCoilSelectionReady(const int f_index);
    void CheckNextProtocol(const int f_protocol_no, const BOOL f_call_frm_gui = FALSE);
    void SendAcqInfoForGate()const ;
    int  FileCopy(BSTR f_existingfilename, BSTR f_newfilename);
    BOOL IsCFShiftSupportedSystem()const;
    bool RaiseOrderRecordAuditEvent();
    //Patni-PJS/2010Feb2/Modified/IR-97
    //void ProcessScanRelatedMsgForWFDA(CPqmProtocol* f_protocol, pqm_msg_tag f_msg_tag);

    void SeriesScanDone() const;

    bool InitPlanDupSettingsFile();
    bool DeleteProtocol(const int f_protocol_index);
    bool DuplicateProtocol(const int f_index_to_duplicate, const int f_duplicate_index);
    void StartPQMDebug(const int f_idebugOption, const UINT f_buttonmask);
    void OnOffCDSDebug(const BOOL f_OnOff);
    bool OnOffAsCompass(const int f_protocol, const bool f_OnOff)const ;
    bool CDSDebugClosed(VARIANT** f_compass_debug_param, bool f_set);
    void CoilSelectionClosed(const BOOL f_ok_pressed) ;
    bool PqmDebugStatusLogOpen();

    bool IsSameStudy() const;
    void AddCoilToList(CString coilDesc);
    void DeleteAllCoilData();
    bool IsSkipRgn() const;
    bool IsMatchSequenceParam(CPqmProtocol* f_protocol, ProtocoInfo* f_protocol_info);
    void LogProtocolInfo(CPqmProtocol* f_protocol, ProtocoInfo* f_list_info);
    void SeriesPrescanDone() const;

    void AllocateGroupIdForDuplicateProtocols(VARIANT* f_pdata);
    void OnDeleteUpdateMasterSlave(VARIANT* f_pdata);
    void UpdateGroupNum();
    int IsMasterSlaveProtocol(const CPqmProtocol* f_protocol) const;
    bool IsWireLessSystem()const;
    void CheckAndGetWaitingForAutoScanStartAcqOrder(int* f_waiting_for_auto_scan_acq_order);
    void ChangeSlaveToNormalProtocol(const int f_slave_index);
    void PerformAutoCopyForMasterSlave(VARIANT* const f_pdata);
    bool CheckScanEditForInscan()const;
    void GetProtocolInfoForAutoCopy(VARIANT* f_pdata);
    int GetEditingProtocolGroupID();
    void GetMasterPositionForSlave(const int f_slave_index, int* f_master_position)const;
    void GetProtocolIndexToUpdate(int* const f_protocol_index);
    void GetMasterListForPlanMark(VARIANT* f_pdata);


    CPQMStudy* const GetStudy()const;
    CPqmPmAcqman* const GetAcqManager()const;
    CPASCOMInterface* GetPASCOMInterface() const;
    CDataManager* const GetDataManager();
    CPASCOMInterface* GetPascomInterface();
    CPQMStudyMgr* const GetStudyManagerPtr() const;
    CPqmSpeeder* GetSpeederPtr();

    CMasterSlave* GetMasterSlavePtr() const;
    CPqmPmSARManager* const GetSARManager();
    int GetVAPChannels() const;
    int GetNumChannels() const;

    CpqmAMB* GetAMB();
    int GetChannelPerVFEP() const;
    bool_t GetVAP() const;
    float GetVFEPProcRatio() const;
    CPqmProcon*	 const GetPqmProCon()const;
    int GetIRSScoketFD() const;
    int  GetProcessing() const;
    BOOL GetPostProcessFlag() const;
    CCouchConfig* const GetCouchConfigPtr() const;
    bool GetHiSpeedCouch() const;
    int GetNoOfSequence() const;
    UINT GetAcqmanSocketID() const;
    int GetCouchpac() const;
    int GetXOffsetLimit() const;
    int GetYOffsetLimit() const;
    study_id_t GetAnotherPqmStudy() const;
    int GetChannel_mask() const;
    CpqmCDS* GetCDS();
    CPQMSAR* const GetSAR() const;
    CIRSListner* const GetIRSListner() const;
    void DeleteIRSListnerInstance();
    //Patni-PJS/2010Feb2/Modified/IR-97
    //CWFDAImplementer* GetWFDAImplementer();
    CCoilSelectionTool* const GetCoilSelectionTool()const;
    CPQMdBdt* GetdBdt();
    int GetAvInterval()const ;
    CString GetRisDictionary() const ;
    CString GetRisName() const ;
    CPlanDuplicateSettings* GetPlanDuplicateSettings() const;
    CColorSetting* const GetColorInfo();
    VftCoilListInfo_t* const GetShowCoilInfo() const;
    VftCoilCoordinateListInfo_t* const GetCoordinateList() const;

    coilSelect_t GetSelectCoil() const;
    void GetInsideLimit(int f_index, int& f_in_side_limit, int& f_out_side_limit, BOOL& enable);
    ConnectedCoilList* GetConnectedCoilList();
    CStringArray* const GetGateInfoList()const ;
    CString GetCurrentSarInfo() const;
    int GetHasParent(const int f_acq_order) const;
    sarInfo_t* GetSarInfoPtr();
    sarInfo_t* GetSarInfoPtrForIndex(const int f_seq_index) const;
    int GetNoOfSequencesTransferred() const;
    int GetSFTMode() const;
    void GetSFTParamValue(VARIANT** f_param, const int f_acq_order);
    void GetMagneticCenterPosForCoilSel(VARIANT**  f_center_pos) const;
    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_18
    void GetCDSDebugParam(VARIANT** f_compass_debug_param) const;
    BOOL GetASCompassFlag(const int f_acqorder, BOOL* f_compass_flag) const;

    CAutoInsertProcedure* GetAutoInsertProc();

    int GetAutoView() const;
    CPqmProtocol* GetProtocolForIndex(const int f_index);
    float GetCouchLimitAPC() const;
    float GetDefaultRGN() const;
    bool GetDRKSreconMode() const;
    bool_t GetNRFilter_License()const ;
    bool_t GetEMTONE_License()const;
    bool GetOffsetVecReset() const;
    int  GetSysType() const;
    int GetAvSpdrMapDelay()const ;
    CString GetSysName() const;
    CString GetAcqmanIP() const;
    BOOL GetMrsLicense() const; // For MRS license
    BOOL GetMRS_BRST_License() const ;
    bool GetButtonState(const int button) const;
    float GetRxGainOffset() const;
    SequenceList& GetSendAcquireSeqList();
    SequenceList& GetSendScheduleSeqList();
    CPQMStudyMgr* GetDbsaStudyMgr() const;

    bool  CanDisplayCFAGraph(CPqmProtocol* const f_protocol) const;
    void ProtocolEditingCompleted(CPqmProtocol* const f_protocol) const;

    CString GetStudyUid() const;
    void    SetStudyUid(const CString& f_study_uid);

    CString GetStudyUidPath() const;
    void    SetStudyUidPath(const CString& f_study_uid_path);

    CPQMIPCManager* const GetManagerPtr();
    void  SetManagerPtr(CPQMIPCManager* manager);

    void  GetASCOMPASSParameter(VARIANT** f_cdscompasssetting_param)const ;
    void  SetASCOMPASSParameter(VARIANT** f_cdscompasssetting_param);

    int   GetMode() const;
    void  SetMode(const int mode);

    BOOL  GetAutoPlan()const ;
    void  SetAutoPlan(const int auto_plan);

    int   GetPlanAndScan() const;
    void  SetPlanAndScan(const int);

    bool  GetIsProconConnected()const;
    void  SetIsProconConnected(const bool);

    float GetCouchPos() const;
    void  SetCouchPos(const float f_couch_val);

    BOOL GetShimmWBCoil();
    void SetShimmWBCoil(const BOOL shimming_wb_coil);

    BOOL GetPrepWbCoil()const ;
    void SetPrepWbCoil(const BOOL f_prep_scan_use_wb);

    bool GetAllCoilSAR() const;
    void SetAllCoilSAR(const bool);

    char* GetSystemType();
    void  SetSystemType(char* f_systemtype);

    gdcMode_t	GetGDCMode() const;
    void SetGDCMode(const gdcMode_t&	f_gdcmode);

    harmonizationMode_t	GetHarmonizationMode();
    void SetHarmonizationMode(const harmonizationMode_t&	f_mode);

    BOOL GetAvSpdrMap()const ;
    void SetAvSpdrMap(const BOOL av_spdr_map);

    bool GetAllCoverageRgn() const;
    void SetAllCoverageRgn(const bool);

    int GetFirstWaitProtForAutoScan() const;
    void SetFirstWaitProtForAutoScan(const int f_auto_scan_flag);

    bool GetGUICoilInformationDisplay() const;
    void SetGUICoilInformationDisplay(const bool f_flag);

    CCFShiftCheck* const GetCFShiftCheck() const;
    CCoilChannelMode* const GetCoilChannelMode() const;
    CGateParamSetter* const GetGateParamObj() ;
    const CGateParamSetter* const GetGateParamObj() const;

    void SetPostProcessFlag(const BOOL f_post_process_flag);
    void SetUsername(const CString& f_usr);
    void SetCurrentSarInfo(const CString& f_strsarinfo);
    void SetRefProbeNotAlive(const bool f_refprobe_alive_flag);
    void SetWaitStatus(CPASCOMSequence* pSeq);
    void SetAutoVoice(const BOOL auto_voice);
    void SetSelectedCoilKey(const unsigned long f_selected_coil_key);
    void SetSkipRgn(const bool);
    void SetConfigInfo(SIPCConfigInfo* f_sIPCConfiginfo);
    void SetCDSCheckCoilFlag(const int f_flag) ;
    void SetActiveDialog(const int f_nDialogType , const BOOL f_bSet);
    void SetMoveLocatorMode(const bool f_mode);
    void SetDBDtCancelFlag(const BOOL f_dbdt_flag);
    BOOL SetScanProtocolInfoFromGUI(VARIANT* f_data);
    void SetProtocolIndex(const int f_index);
    void SetPauseButtonLabel(const UINT button_status);
    void SetSensitive(const UINT tag, const bool flag);
    void SetStartLabelForAPC(const UINT button_label);

    void SetAutoLocatorRunning(const BOOL f_cala_applnRunning);
    BOOL IsCalaApplnRunning()const;

    void GetRefParentInfo(CPqmProtocol* f_protocol, CPqmPrMode* f_parent_info);
    BOOL GetAutoLocatorLicense(const AUTOLOCATOR_TYPE f_autolocator_type)const;
    BOOL SetAutoLocatorOffsetParameters(CPqmProtocol* f_protocol);
    BOOL GetSARControlLicense() const;
    BOOL IsValidSequenceToInvokeAutoLocator(const int f_acq_order, const AUTOLOCATOR_TYPE f_autolocator_type) const;
    bool CheckForAutoLocator(CPqmProtocol* f_protocol, const bool f_Autolocator_after_acquisition = true);
    BOOL IsAutoLocatorRunning();
    BOOL CheckAutoLocatorScanConditions(CPqmProtocol* f_protocol);
    //-Patni-PJS/2011Feb15/Added/IR-186
    bool_t GetMulitScanDynamic_License()const ;//Redmine-2187
    BOOL UpdateSARInfo();
    CDefaultPGUIDXmlDataManager* GetDefaultPGUIDDataManager();
    BOOL IsPGUIDDataXmlFilePresent();

    BOOL GetAMBStatusData(int* f_selected_items, int count, VARIANT**  data) const ;
    void GetAMBPlan(VARIANT**  data)const  ;
    void AMBScanStart(VARIANT**  data)const  ;
    BOOL GetAMBLicense()const ;
    bool AMBSlaveMatchSequenceParam(VARIANT* f_pData, const int f_start_index, const int f_end_index);
    BOOL IsSARControlLimitFileExist();
    void SetIsAMBPlanning(const bool f_is_amb_planning);
    bool GetIsAMBPlanning()const;
	int UpdateTIinStudy(int * f_prot_idx, float * f_ti_val); //Anulekha
	int GetGDCSeriesLOIDForProtocol(int protocol, VARIANT *seriesloid);//Anulekha
    BOOL GetTiPrepLicense(const TIPREP_TYPE f_tiprep_type)const;
    bool IsProtocolforTiPrep(int f_acq_order);
    BOOL CanTiPrepEnable();

protected:

    bool InitiaseUIMsgMap();

private:

    CMasterSlave*		m_master_slave;			//CMasterSlave class pointer
    CAutoInsertProcedure m_autoinsert_proc;

    CPQMStudyMgr* m_dbsa_study_mgr;       // PQM Study Mgr Pointer
    int           m_sft_mode;
    bool          m_offset_vector_reset;
    SequenceList  m_snd_acquire_seq_list;
    SequenceList  m_snd_schedule_seq_list;
    CPqmAuditLog       m_pqm_audit_log;
    int		           m_xOffset;
    int		           m_yOffset;
    CString            m_acqman_ip;
    //Patni-PJS/2010Feb2/Modified/IR-97
    //CWFDAImplementer*  m_wfdaimplementer;
    CpqmCDS            m_pqmcds ;
    int                m_total_seq;
    int                m_no_of_sequence;
    CMemFile           memFile; //used for storing the serilized sequence list.
    bool               m_is_same_study;
    BOOL               m_post_process_flag;
    float              m_couch_pos;

    sarInfo_t*         m_ptr_sarInfo;
    int                m_protocol_index;
    coilSelect_t       m_select_coil;
    VftCoilCoordinateListInfo_t*       m_CoilListcoordinate;
    VftCoilListInfo_t* m_show_coil;
    BOOL			   m_mrs_license;
    int                m_pqm_autoview;   // added in SM2 for Autoview
    int                m_plan_scan;   // added in SM2 for Plan and scan
    CString            m_study_uid;                // Study UID
    SequenceList       m_rec_seq_list;
    CPqmPmAcqman*      m_acq_manager;        // Acq Manager pointer
    CCouchConfig*      m_couchconfig;        // Couch Config class pointer
    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilDB*          m_coilDB;                  // Coil DB class pointer
    CColorSetting*     m_colorinfo;
    CPQMStudy*         m_pqm_study;             // PQM Study class pointer
    CPQMIPCManager*    m_pqmipcmanager;    // PQM IPC Manager class pointer
    CSharedMemory*     m_sharedmem;         // Shared memory class pointer
    CCFShiftCheck*     m_cfshift_check;
    CCoilChannelMode*   m_coilchannelmode;
    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilData*       m_coildata;              // Coil Data Pointer
    CColor*            m_color;                // Coil Data Pointer
    CPqmProcon*		   m_pqm_procon;
    int                m_mode;                         // Mode
    bool               m_skip_rgn;                     //
    int                m_systype;                      // System type
    int                m_processing_ui;                //
    int                m_selected_pos;                 // Selected position
    float              m_couch_limit_apc;            // Couch limit APC
    bool               m_drks_recon_mode;
    int                m_couch_pac;                    // mm of couch movement offset
    int                m_pqm_display_mode;             // List display mode
    //Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
    CString			   m_sys_name;
    int				   m_gate_mode;

    CpqmAMB* m_pamb;
    // 01 : hide comment area
    // 10 : hide status area
    // 11 : hide both area
    CPQMSAR*           m_pqmSAR;
    CPqmSpeeder*       m_pqmSpeeder;
    CString            m_study_uid_path;           // Study UID Path
    CPASCOMInterface*  m_pascom;
    BOOL               m_coverage_msg;
    BOOL               m_shimm_wb_coil;
    BOOL               m_prep_wb_coil;
    bool               m_all_coverage_rgn;
    float              m_default_rgn;
    float              m_rx_gain_offset;

    //+Patni-AMT/2010Apr19/Modified/JaFT# MVC7300_CDR_03
    CString            m_ris_dictionary;
    CString            m_ris_name;
    //-Patni-AMT/2010Apr19/Modified/JaFT# MVC7300_CDR_03

    BOOL                m_auto_voice;
    BOOL                m_start_voice;
    long                m_av_interval;
    BOOL                m_av_spdr_map;
    long                m_av_spdr_map_delay;
    int                 m_freeze_level;
    BOOL                m_auto_plan;
    unsigned long       m_selected_coil_key;
    bool                m_is_refprobe_alive;
    study_id_t          m_anotherpqmstudy;
    CIRSListner*        m_irs_listner;
    int					m_after_auto_scan;			/* Automatic plan after running automatic imaging */

    int m_no_of_sequence_transferred;//MRP
    // + Vantage CoilDB support @ AMT
    CDataManager*       m_data_mgr;
    int			        m_channel_mask;	    // V9.0
    int			        m_num_Channels;	    // Maxumum Coil element
    int			        m_VAP_Channels;	    // number of VAP unit
    int			        m_channel_Per_VFEP;
    float		        m_vfep_Proc_Ratio;	// usec/point/ch
    bool_t				m_vap;				//Used in CheckDiskSpace()
    CCoilSelectionTool* m_coil_selection_tool;
    // - Vantage CoilDB support @ AMT
    bool                m_move_locator_mode;
    bool                m_hi_speed_couch;
    bool_t              m_dBdtlicense;
    bool_t              m_dBdt2002_1_license;
    bool_t              m_dBdt2002_2_license;
    bool_t              m_SAR02_01_License;
    bool_t              m_SAR02_02_License;
    BOOL                m_MRS_BRST_license;
    bool_t              m_IEC_Edition;
    /*License Check*/
    //+ Patni-NP/2009Aug17/Added/MVC003302
    bool_t              m_NoiseReduction_license;
    //- Patni-NP/2009Jun17/Added/MVC003302
    //Patni-NP/2009Aug21/Added/License Check
    bool_t              m_Mrapv2002_license;
    //Patni-Abhishek/Redmine-2187
    bool_t              m_muliscan_dyn_license;
    bool_t              m_EMTONE_license;
    CPQMdBdt*           m_pqmdBdt;
    char*               m_systemtype;

    //Ashish SM3
    bool				m_is_active_scanoffset_dialog;
    bool				m_is_active_cfa_dialog;
    bool				m_is_active_coilinfo_dialog;
    bool				m_is_active_gateinfo_dialog;
    bool				m_is_active_coilselection_dialog;
    //Ashish SM3
    bool				m_is_active_amb_dialog;
    BOOL				m_dbdt_cancel_flag;
    bool				m_is_procon_connected;
    CStringArray*		m_gate_list;
    gdcMode_t			m_gdcmode;
    harmonizationMode_t	m_harmonizationmode;			/* V9.20 èkëﬁëŒâû */
    bool				m_setAllCoilSAR;

    // AMT@ConnectedCoilList
    ConnectedCoilList	m_connect_coil_list;
    bool				m_request_coil_info_display;
    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
    CString				m_current_sarInfo;

    //+Patni-RSG/2009Aug/Added/ACE-PerformenceCode
    int*				m_recieve_request_arr;
    int					m_recieved_request_no;
    int					m_vf_add_count;

#ifdef __SM_PERFORMANCE_CHECK__
    unsigned long		m_get_single_seq_time;
    unsigned long		m_receive_sequence_time;
#endif
    //-Patni-RSG/2009Aug/Added/ACE-PerformenceCode

    //Patni-AMT/2009Oct06/Added/Log correction
    pqm_msg_tag_lit_t	m_pqm_msg_tag_lit_map[PQM_MSG_LAST_MSG];
    //Patni-DKH/2010Jan12/Added/IR-87
    CPlanDuplicateSettings* m_planduplicateSettings;
    //dhanesh IR-86
    CPQMDebugInfo*				m_debugInfo;
    CAutoLocatorPostProc*		m_autolocator_postproc;
    CAutoLocatorParameterSetter* m_autolocator_parametersetter;
	CTiPrepUpdater *m_tiprep_updater;//Anulekha
    CGateParamSetter m_gate_param_setter;

    //Patni-HAR/2010Feb03/Added/MVC006364
    int				m_autoprot_acq_order;
    BOOL				m_CALA_license;
    BOOL				m_HALA_license;
    BOOL			m_amb_licence;

    BOOL            m_Tiprep_license;//KJ/2014

    CDefaultPGUIDXmlDataManager*	m_defpguid_xml_data;
    bool							m_is_pguidxml_file;

    bool m_is_amb_planning;

    CPqmPmSARManager*   m_sar_manager;
    BOOL				m_sarcontrol_license;
    int					m_max_wait_time_for_auto_scan;


    void InitializeCouchConfig();
    bool InitializeSMInfo();
    BOOL ReadPqmAppConfigFile();
    bool LoadCoilInfo();                // Load coil information
    bool LoadColorInfo();
    //Patni-Sribanta/2010Nov18/Added/REDMINE-772
    void IntializeShimPrepProbeData() const;
    void ResetShimDataFromFile();
    void ReSetReceiverCoil(CPASCOMSequence* f_pascomseq);
    void ReSetScanAnatomy(CPASCOMSequence* f_pascomseq);
    void SerializeSequenceList();
    void SerializeSequenceParamList();
    void SendSequencesToGUI();
    void ReceiveSequences();
    void ReceiveSequences(CString f_sequence_path, const CString& f_acq_order);
    void UpdateScanTime(COleDateTime scandate_time);
    void UpdateCoilData() const;
    void SelectSeries(const int series_id, const bool series_state);
    void ShiftOffsetVector(const int protocol, const int mode, const int direction, const float amount);
    void InitializeRis() ;
    void WcharToChar(char* des, WCHAR* buff);
    bool StartPrescan(const int seriesuid);
    bool NewSequencesAdded(const int f_count);
    void CheckForValidLicense();
    void CheckForTiPrepLicense();
    bool IsProtocolForTiPrep(); 


    bool_t GetIECEdition()const ;
    long   GetTotalSequence() const;
    bool_t GetMrapv2002_License()const ;
    //Patni-PJS/2010Feb2/Modified/IR-97
    //CWFDAHandlerInterface* GetWFDAHandlerInterface();
    CPQMDebugInfo* GetDebugInfoPtr();
    BOOL   GetDBDtCancelFlag()const;
    BOOL   GetAutoVoice() const;
    unsigned long GetSelectedCoilKey()const ;
    bool   GetRefProbeNotAlive()const ;
    bool   GetMoveLocatorMode() const;
    bool_t GetdBdt2002_1_License() const ; // For dBdt license


    bool_t GetdBdtLicense()const ;
    void   SetdBdtLicense(const bool_t f_license);

    BOOL   GetCoverageMsg();
    void   SetCoverageMsg(BOOL coverage_msg);

    BOOL   GetStartVoice()const;
    void   SetStartVoice(const BOOL start_voice);

    int    GetFreezeLevel()const ;
    void   SetFreezeLevel(const int freeze_level);

    int    GetAfterAutoScan() const;
    void   SetAfteAutoScan(const int f_after_auto_scan);

    bool_t GetdBdt2002_2_License()const ; // For dBdt license
    void   SetdBdt2002_2_License(const bool_t f_license); // For dBdt license

    bool_t GetSAR02_01_License()const ; // For dBdt license
    void   SetSAR02_01_License(const bool_t f_license); // For dBdt license

    bool_t GetSAR02_02_License()const ;
    void   SetSAR02_02_License(const bool_t f_license); // For dBdt license

    void SetHiSpeedCouch(const bool f_hi_speed_couch);
    void SetNoOfSequence(const int total_sequences);
    void SetSpeederPtr(CPqmSpeeder* f_speeder);
    void SetSysType(const int sys_type);
    void SetProcessing(const int processing);
    void SetCouchLimitAPC(const float couch_limit_apc);
    void SetDRKSreconMode(const bool drks_recon_mode);
    void SetDefaultRGN(const float default_rgn);
    void SetRxGainOffset(const float rx_gain_offset);
    void SetRisDictionary(const CString& f_ris_dictionary);
    void SetRisName(const CString&  f_ris_name);
    void SetAvInterval(const int av_interval);
    void SetAvSpdrMapDelay(const int av_spdr_map_delay);
    void SetMrsLicense(const int);  // For MRS license
    void SetStudyDetails(const CString& f_patient_id, const CString& f_study_id);
    void SetIsSameStudy(const bool f_same_study);
    void SetSelectedPosition(const int position);
    bool SetSeriesOrder(BSTR bStr, const int dropindex);
    void SetGateVal(int32_t* const f_current_gate_val);

};

#endif // #ifndef __SM_IPCLIB_PQM_H__ 
