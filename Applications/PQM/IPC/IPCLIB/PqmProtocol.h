//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmProtocol.h
 *  Overview: Structure of CPqmProtocol class.
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
#ifndef __SM_IPCLIB_PQMPROTOCOL_H__
#define __SM_IPCLIB_PQMPROTOCOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <tami/libSAR/sar.h>

#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/constants.h> //Tanoue/MVC5849
#include <PQM/pqmcommon.h>
#include <PQM/CoilDefinitions.h>    // For Saturn Coil information
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
//Patni-Hemant/2010Feb5/Added/ACE-Phase#3/CDS Requirement
#include "pqmCDSInfo.h"
#include "PqmAutoInsert.h"

class CPqmPrMode;
class CPqmPrTRSC;

enum ProtocolKindEnum { ProtocolKind_Normal, ProtocolKind_AutoShim, ProtocolKind_AutoMap };

class CPqmProtocol : private MR::ACQ::SM::COMMON::NonCopyable<CPqmProtocol>
{
public:
    //Constrcutor / Destrcutor
    CPqmProtocol();
    virtual ~CPqmProtocol();


    bool IsProtocolCALAEnable();
    void SetProtcolCALAEnabled(const bool f_cala_enable_flg);
    AL_INVOKE_MODE GetCALAInvokeMode();
    void SetCALAInvokeMode(const AL_INVOKE_MODE f_cala_invoke_mode);
    void SetHALAInvokeMode(const AL_INVOKE_MODE f_hala_invoke_mode);
    BOOL CanInvokeAutoLocator();
    void SetCanInvokeAutoLocator(const BOOL f_invoke_cala_app);
    void SetCalaPlannedStatus(const AL_PLANNED_STATUS f_cala_planned_status);
    void SetHalaPlannedStatus(const AL_PLANNED_STATUS f_hala_planned_status);
    AL_PLANNED_STATUS GetCalaPlannedStatus();
    AL_PLANNED_STATUS GetHALAPlannedStatus();
    AUTOLOCATOR_TYPE GetAutoLocatorType();
    AL_INVOKE_MODE GetAutoLocatorInvokeMode(const AUTOLOCATOR_TYPE f_autolocator_type);

    PqmSaturnRcvCoil_t* GetCoilDBTable();
    bool SetCoilDBTable(PqmSaturnRcvCoil_t* f_saturn_rcv_coil);

    PqmSaturnCoil_t* GetSaturnCoil();
    bool SetSaturnCoil(PqmSaturnCoil_t* f_saturn_coil);

    int  GetIntermitTime() const;
    void SetIntermitTime(const int intermit_time);

    int  GetAppCode() const;
    void SetAppCode(const int app_code);

    int  GetProtocol() const;
    void SetProtocol(const int prot);

    int  GetPosition() const;
    void SetPosition(const int pos);

    int* GetImageGr();

    int  GetAcqStatus() const;
    void SetAcqStatus(const int acq_status);

    int  GetHoldScan() const;
    void SetHoldScan(const int hold_scan);

    int  GetAutoHold() const;
    void SetAutoHold(const int auto_hold);

    int  GetAutoProEdit() const;
    void SetAutoProEdit(const int auto_pro_edit);

    int  GetAutoTransfer() const;
    void SetAutoTransfer(const int auto_transfer);

    int  GetCouchOffsetFlag() const;
    void SetCouchOffsetFlag(const int couch_offset_flag);

    int  GetGatingMethod() const;
    void SetGatingMethod(const int gating_method);

    BOOL GetDynamic() const;
    void SetDynamic(const BOOL dynamic);

    int  GetIntermitFlag() const;
    void SetIntermitFlag(const int intermit_flag);

    char* GetScanID();
    void  SetScanID(const char* scan_id);

    char* GetScanComment();
    void  SetScanComment(const char* comment);

    BOOL GetSARInit() const;
    void SetSARInit(const BOOL sar_init);
    //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
    int GetProcessing() const;
    void SetProcessing(const int processing);
    //-Patni-PJS/Added/2011-Mar-11/REDMINE-1529

    int  GetShimming() const;
    void SetShimming(const int shimming);

    BOOL GetShimmingReady() const;
    void SetShimmingReady(const BOOL shimm_ready);

    BOOL GetFluoro() const;
    void SetFluoro(const BOOL fluoro);

    BOOL GetHasImage() const;
    void SetHasImage(const BOOL has_image);

    BOOL GetHasParent() const;
    void SetHasParent(const BOOL has_parent);

    int  GetPrepScan() const;
    void SetPrepScan(const int prep_scan);

    int  GetMovingCouch() const;
    void SetMovingCouch(const int moving_couch);

    int  GetAcqSplitMode() const;
    void SetAcqSplitMode(const int acq_split_mode);

    int  GetPlnAutoVoice() const;
    void SetPlnAutoVoice(const int pln_auto_voice);

    int  GetExeAutoVoice() const;
    void SetExeAutoVoice(const int exe_auto_voice);

    int const GetDataKind();
    void SetDataKind(const int data_kind);

    int  GetSpeederType() const;
    void SetSpeederType(const int speeder_type);

    int  GetSpeederMapProtocol() const;
    void SetSpeederMapProtocol(const int speeder_mapprot);

    int  GetIntensityType() const;
    void SetIntensityType(const int intensity_type);

    int  GetProbeScan() const;
    void SetProbeScan(const int probe_scan);

    int GetRetroScan() const;

    int  GetdBdtMode() const;
    void SetdBdtMode(const int dBdt_mode);

    CString GetdBdtModeString()const ;
    void SetdBdtModeString(const CString& f_dbdtstring);

    int  GetdBdtOperation() const;
    void SetdBdtOperation(const int dBdt_operation);

    int  GetNumOfCoverages() const;
    void SetNumOfCoverages(const int num_of_coverages);

    int  GetNumOfScan() const;
    void SetNumOfScan(const int num_of_scan);

    BOOL GetProlimSaveFlag() const;
    void SetProlimSaveFlag(const BOOL prolim_saveflg);

    int GetRetroMode() const;

    int  GetNumChangedReceiveCoil() const;
    void SetNumChangedReceiveCoil(const int num_changed_receive_coil);

    int* GetReceiveCoilTable() ;
    void SetReceiveCoilTable(const int index, const int receive_coil_table);

    int  GetCurrentdBdtMode() const;
    void SetCurrentdBdtMode(const int f_cur_dbdt_mode);

    int  GetdBdtMode2002() const;
    void SetdBdtMode2002(const int dBdt_mode_2002);

    int  GetdBdtControl() const;
    void SetdBdtControl(const int dBdt_control);

    int  GetLocalCF() const;
    void SetLocalCF(const int local_cf);


    int  GetProbeCorrection() const;
    void SetProbeCorrection(const int probe_correction);

    int  GetPlnStatus() const;
    void SetPlnStatus(const int pln_status);

    int GetAMBEnableFlag() const;
    void SetAMBEnableFlag(const int amb_enable_flag);

    int  GetAMBModeFlag() const;
    void SetAMBModeFlag(const int amb_mode_flag);

    time_t GetScanTime() const;
    void SetScanTime(const time_t f_scan_time);

    f32vec3_t GetOffset()const;
    void SetOffset(const int xyz = 0, const float value = 0);

    CPqmPrMode* const GetPqmPrMode()const;
    CPqmPrTRSC* const GetPqmPrTRSC()const;

    sarInfo_t* GetSarInfo() ;

    int  GetTrValue() const;
    void SetTrValue(const int f_tr);

    char* GetContrastName();
    void  SetContrastName(const char* f_contrastname);

    CString GetSeriesLOID()const ;
    void SetSeriesLOID(const CString& f_series_loid);

    sarStat_t GetSARStat()const;
    void SetSARStat(const sarStat_t f_sarStat);

    VfPathElem_t GetLocatorImages(const int f_loc_img_cnt);
    void SetLocatorImages(VfPathElem_t* f_locator_image);

    float GetdBdtValue(const int f_index)const ;
    void  SetdBdtValue(const int f_index, const float f_dbdtvalue);

    bool GetFlagWaitForVoice()const;
    void SetFlagWaitForVoice(const bool f_bFlagWaitForVoice);

    BOOL GetSeriesNumInDbFlag()const;
    void SetSeriesNumInDbFlag(const BOOL f_set_series_num_indb);

    CHILD_THREAD_STATUS GetChildThreadStatus()const ;
    void SetChildThreadStatus(const int f_child_thread_status);

    int  GetCopySourceProt()const ;
    void SetCopySourceProt(const int f_copySourceProt);

    int  GetcouchMethodFlg()const ;
    void SetcouchMethodFlg(const int f_couchMethodFlg);

    float GetRflPos()const ;
    void  SetRflPos(const float f_rflpos);

    float GetCfaPos()const ;
    void  SetCfaPos(const float f_cfapos);

    int  GetRflProt()const ;
    void SetRflProt(const int f_rflprot);

    int  GetCfaProt()const ;
    void SetCfaProt(const int f_cfaprot);

    int  GetShimProt()const ;
    void SetShimProt(const int f_shimprot);

    CpqmCDSInfo* GetCDSInfo() ;
    int  GetASCOMPASSFlag() const;
    void SetASCOMPASSFlag(const int f_as_compass_flag);

    int  GetMRSEddyAxis() const;
    void SetMRSEddyAxis(const int f_mrs_eddy_axis);

    int  GetRequireShimming() const ;
    void SetRequireShimming(const int l_require_shimmimg_flag);

    bool GetIsMapPostProcDone() const ;
    void SetIsMapPostProcDone(const bool f_is_map_postproc_done);

    int GetProtcolCorrStatus()const;
    void SetProtcolCorrStatus(const int f_intensity_corr_status);

    CPqmProtocol* GetAutoInsertedMapProtocol()const;
    void SetAutoInsertedMapProtocol(CPqmProtocol* f_protocol);

    CPqmProtocol* GetAutoInsertedShimProtocol()const;
    void SetAutoInsertedShimProtocol(CPqmProtocol* f_protocol);

    CPqmProtocol* GetTargetProtocol()const;
    void SetTargetProtocol(CPqmProtocol* f_protocol);

    void SetMapRequired(const int f_map_required);
    int GetMapRequired()const;

    void SetOriginalParameters();
    OriginalParameters* GetOriginalParameters()const;

    int GetcoilChannelMode() const;
    void SetcoilChannelMode(const int f_coil_channel_mode);
    int GetSystemChannels() const;
    void SetSystemChannels(const int f_num_channel);
    int GetMapInsertionCounter()const;
    void IncrementMapInsertionCounter(const int f_counter);

    int GetShimInsertionCounter()const;
    void IncrementShimInsertionCounter(const int f_counter);

    const ProtocolKindEnum GetProtocolKind() const;
    void SetProtocolKind(const ProtocolKindEnum f_protocol_kind);

    CString GetDelayTime() const;
    void SetDelayTime(const CString& f_delay_time);

    BOOL GetMasterFlag() const;
    void SetMasterFlag(const BOOL f_master_flag);

    int GetGroupID() const;
    void SetGroupID(const int f_group_id);

    int GetMultiSeriesDynamicFlag() const;
    void SetMultiSeriesDynamicFlag(const int f_multi_series_enable_flag);

    CPqmProtocol* GetMasterProtocol() const;
    void SetMasterProtocol(CPqmProtocol* const f_master_protocol);

    BOOL GetAutoGetIntervalFlag()const;
    void SetAutoGetIntervalFlag(const BOOL f_auto_get_interval_flag);
    bool IsProtocolHALAEnable();
    void SetProtcolHALAEnabled(const bool f_hala_enable_flg);
    AL_INVOKE_MODE GetHALAInvokeMode();
    bool GetInscanSwitchFlag()const;
    void SetInscanSwitchFlag(const bool f_inscanswitchflag);
    int GetInscanSwitchType()const;
    void SetInscanSwitchType(const int f_inscanswitchtype);

    bool GetAtuomapAutoShimAdded()const;
    void SetAtuomapAutoShimAdded(const bool f_automap_autoshim_protocol_added);

private:
    AutoInsertProtocol m_auto_target_ptr;
    ProtocolKindEnum m_protocolkind;

    int m_map_required;
    bool m_automap_autoshim_protocol_added;

    bool m_cala_enable_flg;
    bool m_hala_enable_flg;
    AL_INVOKE_MODE m_cala_invoke_mode;
    AL_INVOKE_MODE m_hala_invoke_mode;
    BOOL m_invoke_cala_app;
    AL_PLANNED_STATUS m_cala_planned_status;
    AL_PLANNED_STATUS m_hala_planned_status;

    bool m_map_post_proc_done;    //Patni-MRP/2010Aug6/Added/REDMINE-498
    //Patni-Hemant/2010Feb5/Added/ACE-Phase#3/CDS Requirement
    CpqmCDSInfo m_cds_info ;

    //Patni/AM/2010July20/Added/NFD001-AutoShim
    int m_require_shimming_flag;

    BOOL        m_is_series_num_indb;
    int         m_app_code;

    int         m_tr;
    char m_contrast_name[32];

    int         m_prot;     /* protocol number */
    //    DllT      *m_link;        /* protocol link */
    int         m_pos;      /* position */

    int         m_img_gr[NUM_2];

    int         m_acq_status;
    int         m_hold_scan;
    int         m_auto_hold;
    int         m_auto_proedit; /* Auto ProEdit Flag V5.00 */

    int         m_auto_transfer;    /* Auto Transfer flag V5.00 */

    int         m_gating_method;    /* Method of Gate */
    BOOL        m_dynamic;  /* Dynamic Scan or no */
    int         m_intermit_flag;
    int         m_intermit_time;

    time_t      m_time;

    /* For scan offset. */
    //  float   m_offset_x;     /* Scan offset. */
    //float   m_offset_y;
    //float   m_offset_z;
    f32vec3_t	m_offset;

    char    m_scan_id[MR::ACQ::SM::COMMON::SCAN_ID_LENGTH * 2 + 1]; /* scan ID. */ //Tanoue/MVC5849
    char    m_comment[MR::ACQ::SM::COMMON::SCAN_COMMENT_LENGTH * 2 + 1]; /* scan comments. */ //Tanoue/MVC5849

    CPqmPrMode*     m_pqm_pr_mode;
    CPqmPrTRSC*     m_pqm_pr_trsc;      /* Protocol cahge flag */
    //    sarInfo_t     sarInfo; // later phase
    BOOL        m_sar_init;
    //TMSC-REDMINE-1529
    int        m_processing;

    int         m_shimming; /* Shimming sequence flag */
    BOOL        m_shimm_ready;

    BOOL        m_fluoro;       /* Fluoro sequence flag */
    BOOL        m_has_image;    /* Has Images */
    BOOL        m_has_parent;   /* Has Parent */
    VfPathElem_t    m_loc_images[4];    /* Parent path */
    int     m_prep_scan;
    int     m_moving_couch;

    int     m_acq_split_mode;   /* acquisition Split mode */

    int         m_pln_auto_voice;
    int         m_exe_auto_voice;

    /// XmString     prxms[PQM_LIST_MAX];    /* XmStrings for list item. */

    int         m_data_kind;    /* SVN_PRC_DATA_KIND */
    int         m_speeder_type ;    /* V4.5 Speeder Ph1 */
    int         m_speeder_mapprot ; /* V4.5 Speeder Ph1 */
    int         m_intensity_type ;  /* V4.5 Speeder Ph1 */
    int         m_probe_scan ;      /* V5.00 MotionCorrection FLAG */
    int         m_retro_scan ;      /* V5.00 Retro Scan FLAG */
    int         m_dbdt_mode ;       /* V5.00 dBdt Mode FLAG */
    int         m_dbdt_operation ;      /* V5.00 dBdt Operaation FLAG */
    float       m_dbdt_value[NUM_2] ;       /* V5.00 dBdt Value */
    int         m_num_of_coverages ;    /* V5.00 number of COVERAGES for oneTimeBrethHold */
    int         m_num_of_scan ;     /* V5.00 number of AMCMS_SCAN_BEGIN for EZscan */
    BOOL        m_prolim_saveflg;       /* V5.01 Only save from prolim */
    int         m_retro_mode ;      /* V5.10 retro Mode FLAG */
    int         m_num_changed_receive_coil ;    /* V5.30 number of Changed Recive Coil */
    int         m_rcv_coil_tbl[PQM_MAX_MOVINGBED_SCAN] ;    /* V5.30 Recive Coil ID table */
    int         m_cur_dbdt_mode ;       /* V6.00 current dBdt Mode FLAG */
    int     m_dbdt_mode_2002 ;      /* V6.00 dBdt 2002 Mode FLAG */
    int     m_dbdt_control ;        /* V6.00 dBdt control1/control2 mode FLAG */
    int     m_local_cf ;        /* V6.25 Local CF Flag */
    int     m_couch_offset_flag;    /* V6.30 HH/CC=>FALSE  HC=>TURE */
    int     m_probe_correction; /* V6.50 Probe COrrection flag */
    int     m_pln_status;       /* V7.00 Plan Status */
    int     m_amb_enable_flag;  /* V7.10 Advanced MovingBed Mode flag */
    int     m_amb_mode_flag;    /* V7.10 Advanced MovingBed Mode flag */
    sarInfo_t*  m_sarinfo;
    sarStat_t  m_sarStat;
    CString		 m_series_loid;//MRP here

    CString		m_delay_time;
    int			m_group_id;
    BOOL		m_master_flag;
    BOOL		m_multi_series_enable_flag;
    CPqmProtocol*	m_master_protocol;

    // V9.25 Coil Support@AMT
    PqmSaturnCoil_t*    	m_saturn_coil ;	// V9.00 number of Section
    PqmSaturnRcvCoil_t*	m_coil_DB_tbl;	    // V9.00 for Saturn
    //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
    CString m_dbdtstring;
    //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
    //+Patni-HAR/2010Jan18/Added/DeFT#MVC006364
    CHILD_THREAD_STATUS m_child_thread_status;
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    float	m_rflPos;		// RFL
    float	m_cfaPos;		// CFA
    int		m_rflProt;	// RFL
    int		m_cfaProt;	// CFA
    int		m_shimProt;	//


    //Patni-HAR/2010Apr26/Added/IR-134
    int m_mrs_eddy_axis;
    //Patni-Manish//2009Apr04//Added//PSP1-352
    bool m_bFlagWaitForVoice;


    //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    int m_copySourceProt;
    int m_couchMethodFlg;

    int m_intensity_corr_status;
    int m_coil_channel_mode;
    int m_numchannels;
    BOOL m_auto_get_interval_flag;
    bool m_inscanswitchflag;
    int  m_inscantypetochange;

    void RemoveAutoInsertProtRefFromTarget(CPqmProtocol* const f_auto_insertprot);
    void RemoveTargetRefFromAutoInsertProt(CPqmProtocol* const f_targetprot);
    void RemoveCrossRefForTargetAndAutoInsert();

    void SetImageGr(const int index, const int img_gr);
    void SetRetroScan(const int retro_scan);
    void SetRetroMode(const int retro_mode);

};
#endif // #ifndef __SM_IPCLIB_PQMPROTOCOL_H__ 
