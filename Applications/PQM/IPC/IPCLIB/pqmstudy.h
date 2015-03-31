//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmstudy.h
 *  Overview: Structure of CPQMStudy class.
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
//This class will contain all the study related information like the patient name etc.
//It will also hold the map of the CSeriesPQM objects
#ifndef __SM_IPCLIB_PQMSTUDY_H__
#define __SM_IPCLIB_PQMSTUDY_H__

#if _MSC_VER > 1000
#pragma once
#endif
extern "C" {
#include <tami/libvfToUi/vfToUiprotoVf.h>
#include <tami/libSAR/sar.h>
}
//Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838
//+Patni-Rajendra/2011Mar29/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/Mutex.h>
#include <libStudyManager/Mutex/ThreadMutex.h>
#include <libStudyManager/Mutex/MutexFactory.h>
//-Patni-Rajendra/2011Mar29/Added/TMSC-REDMINE-773
#include <libStudyManager/NonCopyable.h>
#include <pdet/pdet.h>
#include <PASCOMLIB/PASCOMSequence.h> //Patni-AMT/2009Apr22/Added/PSP1#001
#include <PQM/CoilDefinitions.h>    // V9.25 Coil Support@AMT

#include "ShimmingRequestStructures.h"
#include "pqmmessagestructures.h"


typedef enum  {
    OTHER = 0,
    SP_CREATE_MODE_WSE,
    SP_CREATE_MODE_WSE_AMB,
    SP_CREATE_MODE_PRIVATE_PROLIM,
    SP_CREATE_MODE_PRIVATE_PROLIM_MSD,
    SP_CREATE_MODE_WSE_MRS
}
SCRATCHPAD_CREATION_MODE;

class CPqmProtocol;
class CPqmPrMode;
class CPASParamStore;
class CPASCOMSequence;// SM4 Defect#737 Himanshu 20090130
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345
//+Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
class CPqm;
class CAutoInsertProtocolStudy;
//-Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
class CMasterSlave;


#define STUDY_FOLDER _T("study")
#define OFFSET_RATE	100.0

// MEITEC/2010Apr23/Added/IR86
typedef std::vector<CPqmProtocol*> protocolvec;

//MEITEC watanabe ADDED 10/01/14 //.pdb_header file copy
#define FNAME_PDBHEADER      _T(".pdb_header")
#define ENGINE_CONNECT_MODE  _T("CONNECT")
#define ENV_ENGINE_HOST      "ENGINE_HOST"
#define ENV_ENGINE           "ENGINE"
//MEITEC watanabe ADDED 10/01/14 //.pdb_header file copy

class CPQMStudy : private MR::ACQ::SM::COMMON::NonCopyable<CPQMStudy>
{

public:
	bool IsProtocolForTiPrep(int f_acq_order);
	int GetGDCSeriesLOIDForProtocol(int protocol, VARIANT *seriesloid);

    // Default CPQMStudy constructor and destructor
    explicit CPQMStudy(CPqm* pqm);
    virtual ~CPQMStudy();

    int AMBReArrange(const int f_drag_indx, const int f_drop_index);

    //Patni-PJS/2010Dec10/Modified/IR-97
    status_t CheckPlnStatus(const int f_protocol)const;
    //Patni-PJS/2011Feb21/Added/IR-97
    BOOL IsValidWFDAScan();
    BOOL UpdateGUIControlsInVarfieldDB(VARIANT* f_pdata, const int f_scan_index);
    CPqmProtocol* ListCopyProt(CVarFieldHandle* const f_vf_handle, const int f_to_prot, const int f_to_pos);
    int  ReArrange(const int f_drag_indx, const int f_drop_index);
    bool CopyScanModeFrom(CVarFieldHandle* const f_vfd, const int from_pos, const int	to_pos,
                          const bool_t	copyContrast);

    bool AMBCopyScanModeFrom(CVarFieldHandle*	const f_vfd,
                             const int		from_protocol,
                             const int		to_protocol,
                             const bool_t	copyContrast);

    //void PqmUiSetScanOffset(CPqmProtocol*        f_obj);
    BOOL PqmUiSetScanOffset(CPqmProtocol*        f_obj , VARIANT**  f_scanoffset);
    void PqmUiConvertVector(VftPatori_t*	patori, bool_t		toMachine,
                            f32vec3_t*	fr, f32vec3_t*	to);

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    bool OnOffAsCompass(const int f_protocol, const bool f_OnOff);
    int  PutSaturnCoil(CVarFieldHandle* const f_vf_handle, const int f_prot,
                       PqmSaturnCoil_t* f_saturn_Coil);
    status_t PutConnectedCoil(CVarFieldHandle* const f_vf_handle, const int          f_prot,
                              CPqmProtocol* f_pr_obj);
    status_t PutRecvrCoil(CVarFieldHandle* const f_vf_handle, const int          f_prot,
                          CPqmProtocol* f_pr_obj);
    status_t SpeederMAPExist(const int prot)const;
    //TMSC-Tanoue/2010Aug25/Modified/MVC008345. Use CVarFieldHandle
    int PutdBdtMode(CVarFieldHandle* const vf_handle , const int  port, int32_t mode , flt32_t* value)const;
    void CloseStudy();
    //+Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838
    bool CreateOrUpdateVFStudyFile(CString f_patient_loid, CString f_study_loid, CString& f_vf_study_name);
    bool CreateStudyFile(const CString& f_patient_loid, const CString& f_study_loid);
    BOOL IsHasImage(CPqmProtocol* curr_protocol)const;
    void PutInjectTimeInDB()const;
    // duplicating the sequence
    int  DuplicateProtocol(int f_from_pos, int f_to_pos, const int f_group_id = 0);
    void DuplicateSequence(int* f_pos_list);
    // for deleting any sequence
    void DeleteProt(const int f_index);
    int  ReaddBdtOpMode(CPqmProtocol* f_protocol, int& f_dbdt_op_mode, flt32_t& f_curr_dbdt_value)const;
    bool CheckAllForSequenseParam(VARIANT* f_pData, const int f_count_gui_list);
    BOOL WriteToShimData()const;
    //Patni-Hemant/08Sep2010/Added/IR-152 Reopen
    bool CreateScratchPadForSFTTool(const int f_acq_order, CString* f_tmp_file,
                                    const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);
    int  MaintainShimming(const CString& f_file);

    status_t PqmPmSaveProtocol(CPqmProtocol* f_protocol, char* f_tmp_file);

    status_t PqmPmMaintainScratchPad(CPqmProtocol* f_protocol, char* tmp_file,
                                     const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);
    void AddPrescanTagsForNextProtocol(CVarFieldHandle* const f_vf_read_handle, CVarFieldHandle* const f_vf_write_handle,
                                       const int f_current_protocol, const bool f_write_scratchpad = false,
                                       const bool f_apply_to_current_only = false)const;
    bool_t PqmPrCheckSlicePosition(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_obj);
    int PutSaturnCoilDegeneracy(const int f_prot, PqmSaturnCoil_t* f_saturn_coil);
    status_t PqmSetWBCoilCDS(CVarFieldHandle* const f_vf_handle, int f_protocol,
                             PqmSaturnCoil_t* f_saturnCoil, PqmCoil_t* f_coil_p);

    bool     IsProtocolInScan(const int f_protocol_acq_order);
    status_t SpeederSetDynamic(const int prot, int* CoilTbl);
    int32_t  SpeederCoilMode(const int prot)const;
    status_t SpeederClearMapPath(const int seqProt)const;
    status_t SpeederSetMapPath(const int seqProt, const int mapProt)const;
    status_t SpeederSetMapType(const int prot, int type)const;
    status_t SpeederResetDynamic(CVarFieldHandle* const vf_handle, CPqmProtocol* f_current_protocol /*PqmPrData_t *obj*/)const;  //IR-161
    status_t PqmPrMaintainGdc(CVarFieldHandle* const f_vf_handle, const int f_prot);
    //Patni-Sribanta/2010Oct25/Modified/IR-170
    char*   ConvertCHAR(CString f_input) const;
    int  ReadTotalSlicesForShimming(VfPathElem_t* f_vfpath_elem)const;
    int  ShimPositionRead(SShimStudy_t* f_shimstudy_struct, position_t* f_position_struct)const;
    BOOL ShimCheckAMB(VfPathElem_t* f_vfpath_elem)const;
    BOOL ShimDataKindCheck(VfPathElem_t* f_processpathp, const int f_processed_nodes)const;
    BOOL ShimReqPutOffset(int f_typeformat_depth, VfPathElem_t* f_vfpath_element, const int f_adjust_processed)const;
    BOOL ShimGetZ2OffSet(int* f_z2_offset)const ;
    BOOL ShimReqPutFsyVal(VfPathElem_t* f_vfpath_element, int f_typeformatdepth, const int f_adjust_processed)const ;
    status_t PutSaturnCoil2(int prot, int			    numSection,	/* number of Section */
                            VftSectionRecvrCoil_t*   RcvCoil,   /* Recver Coil */
                            VftSectionSignal_t*    	Signal,   /* Signal */
                            char*    CoilName  /* Coil label name  */);

    bool IsMatchAcqStatusOfVarField(ProtocoInfo* f_protocol_info);
    //Patni-Sribanta/2010Oct25/Modified/IR-170
    BOOL LoadStudy(const CString& f_study_name, const int& f_mode, study_id_t& f_study_id_t, bool& f_same_study); //+Patni-Sribanta/2010Nov18/Modified/REDMINE-772
    BOOL FillProtocolVector(const CString& f_pas_path);
    //+Patni-RSG/2009Aug07/ACE Performence Code
    //BOOL WriteToStudy(CString f_sequence_path, int f_acqorder);
    BOOL WriteToStudy(CString f_sequence_path, int* f_acqorder, const int f_seq_count);
    BOOL ReadShimValuesFromFile(int* f_scannumber, float* f_couchpos, float* f_homepos, float* f_txtunepos, float* f_rflpos, int* f_shimstate, int* f_shimsfyflag)const;
    int  ReadMaxValueFromSubProtocol(const int f_protocol, const int f_tag_name, f32vec3_t& f_curr_value)const;
    int  ReadRealtimeSAR(const int f_acq_order, std::vector<f32vec3_t>* f_tag_values)const;
    int  ReadProtocolLevelTagValue(const int f_acq_order, const int f_tag_name, f32vec3_t& f_var_field_value)const;
    int  ReadSubProtocolLevelTagValue(const int f_acq_order, const int f_tag_name, std::vector<f32vec3_t>* f_var_field_value)const;
    //+Patni-MP/2009Mar16/Added/ACE-1/MCM0213-00181
    int  DeleteParentIndexFplanning(const int	f_protocol)const;
    void PostProcessMk(CPqmProtocol* f_curObj);
    void NoiseReductionProcessCheck(const int f_protocolnumber);
    void EMTONE_LicenseCheck(const int f_protocolnumber);
    void CopyCurrentProtocol(const int f_protocol);
    //Patni/2011Jan31/Added/V1.35/MVC009666
    void DelPCVAPMode(const int f_protocol) const ;
    int  DRKSCheck(CPqmProtocol* f_current_protocol);
    BOOL ProtocolShimmingStudy();
    //Patni-PJS/2010Jun17/Added/MVC008293 CR
    status_t ClearFieldsOnScanStart(const int f_protocol);
    void SearchScanMode(CVarFieldHandle* const f_handle, const int flag);
    void InitPatientPath(CPqmProtocol* f_prot);
    BOOL IsHasLocatorImage(const int prot);
    BOOL StudyCheckPlan(const CString& file_name);
    //Patni-SS/2010July6/Added/NFD001-AutoMap
    void UpdateSpectrumData(CPqmProtocol* f_curpqmprotocol);
    int PutShimmingOffset(CVarFieldHandle* const f_vf_handle, VfPathElem_t* f_path, const int f_depth, PqmShim_t* f_shim)const;
    //Patni-MP/2010Oct19/Added/NFD001_AT_6
    bool CopySpectrumDataFromShimFile(CPqmProtocol* f_pqmprotocol)const;
    bool Check1stScan()const;
    int  MaintainStudy(CString f_file, CPqmProtocol* f_current_protocol, const int f_mode);
    status_t PutRflParamsSAR(CVarFieldHandle* const f_vf_handle, const int f_prot)const;
    int  IsProtocolJETEnable(const int f_prot)const;
    void pqmAcqOverErrLog(char* f_coilname);
    int  MaintainCfStudy(CPqmProtocol* f_protocol, wchar_t* f_study_name); // + SM4 Himanshu CFA Fix

    //Patni-DKH/2011Feb23/Added/TMSC-REDMINE-1470
    int CheckNextAutoProt(CPqmProtocol* f_prot);
    void CheckMRSValueInVF();
    void CheckMRSValueInVF(const CPqmProtocol* f_protocol)const;
    bool UpdateIntensityCorrStatus(const int f_protocol, int f_intensity_corr_status_value);
    bool UpdateIntensityCorrMethod(const int f_protocol, int f_intensity_corr_method_value);
    bool UpdateIntensityCorrFlag(const int f_protocol, int f_intensity_corr_flag_value);
    bool CheckIntensityCorrFlag(const int f_protocol) const;
    bool CheckIntensityCorrMapStatus(const int f_protocol)const;
    void CopyPCVAPmodetoMRSScratchPad(const int f_acq_order, const CString& f_file);
    BOOL UpdateSARInfo();



    void SetRMSAEStatusForStudy(const bool f_status);
    bool GetRMSAEStatusForStudy() const;

    int GetTargetProtocolsDegeneracyFlag(const int f_index)const;

    void SetBreathHoldValue(CPqmProtocol* f_protocol, CVarFieldHandle* const f_vf_handle);
    status_t DisableMoveCouchMethodFlagFromScratchPad(CVarFieldHandle* const f_vf_handle);
    status_t SetMasterRGNValueToSlave(const int f_slave_acq_order, const int f_master_acq_order);
    void SetGroupIDToVF(CPqmProtocol* f_protocol, int f_group_id);
    void SetGroupNumToVF(int f_group_num);
    CString GetDelayTimeFromVf(const int f_protocol);
    void SetNewDelayTimeToVf(int f_delay_time, const int f_protocol);
    void CalculateTimeDiff(VARIANT* f_pdata);
    bool CheckForInjectedTime(int& f_injected_time_diff);
    void SetMasterFlagToVF(CPqmProtocol* f_protocol, BOOL f_master_flag, bool f_can_delete_tag = false);
    bool CheckPixelAndRGNCorrFlag(const int f_current_protocol);
    bool IsTagPresentInFile(CVarFieldHandle* f_vf_handle, int f_tag, int f_type, const int f_acq_order);
    bool SendSAEInfo();
    BOOL     ClearSequenceList(SequenceList& f_seq_list);
    //Patni-SS/2010July6/Added/NFD001-AutoMap
    CAutoInsertProtocolStudy* GetAutoMapShimmStudy();
    int32_t GetTotalSAE(CPqmProtocol* f_pqm_protocol);
    CPqmProtocol* GetLastCompletedProtocol();
    CVarFieldHandle* GetVfDBHandle();
    CPqmProtocol* GetProtocol(const int protocol_number)const;
    CPqmProtocol* GetItemFromPosition(const int position)const;
    CPqmProtocol* GetItemFromProtocol(const int f_protocol)const;
    CPqm* GetPqm()const; // Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
    CString GetStudyName()const;
    int GetRumNum() const;
    int GetSiteId() const;
    //+TMSC-Tanoue/2010Aug25/MVC008345
    //Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* GetStudyLock() {
        return &m_cs;
    }
    //-TMSC-Tanoue/2010Aug25/MVC008345

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* GetScanMutex() {
        return &m_scan_mutex;
    }
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* GetSeqTransferMutex() {
        return &m_seq_transfer_mutex;
    }

    study_id_t GetStudyID()const;
    int GetShimFlag(VfPathElem_t* f_vfpath_element)const;
    int GetShimmingOffset(PqmShim_t* f_shim)const;
    int GetImageDataKind(const int protocol, const int proc)const;
    int GetTopWaitPos(CPqmProtocol** result_acqSeries)const;
    CPqmProtocol* GetCurrentProtocol()const;
    float GetWeight() const;
    float GetHeight()const;
    int   GetGender()const;
    // MEITEC/2010Apr23/Added/IR86
    int   GetAllItems()const;
    int   GetProtCountFromVec()const;
    protocolvec GetProtocolvector()const;
    BOOL GetFirstScan()const;
    //Patni-DKH/2010Sep13/Added/TMSC-REDMINE-666
    void GetAllProtAcqStatus();
    CString GetCoilNameFromAcqOrder(const int f_protocol_no)const;
    CString GetReceiverCoilName(const int f_acq_order)const; //Nitin Coil
    BOOL GetControlValueFromStudy(VARIANT* f_pdata, const int f_scan_index)const;
    int GetScanAgent(const int f_protocol_index, CString& f_scan_agent);
    //Patni-Sudhir/2011Jan13/Added/Redmine-434
    int GetNRFilterStringForProtocol(const int f_acq_order, const int f_NRFLG, VARIANT* f_NRVal)const;
    int GetEMTFilterStringForProtocol(const int f_acq_order, VARIANT* f_EMTVal)const;
    CPqmProtocol* GetFirstProtocol();
    CPqmProtocol* GetNextItemFromPos(CPqmProtocol* f_obj);
    //Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07
    CPqmProtocol* GetPrevItemFromPos(CPqmProtocol* f_obj)const;
    CPqmProtocol* GetNextItem(CPqmProtocol* protocol)const;
    CPqmProtocol* GetNextWaitProtocol(CPqmProtocol* protocol)const;
    BOOL GetSubProtValues(int prot, int sub_prot, bitfield_t* mask,   ...);
    BOOL GetProtocolItemValues(CVarFieldHandle* const f_vf_handle, CPqmProtocol* pr_obj_p);
    int MinimumChannelCheck(const int f_num_Section, const int f_coil_num, VftSectionRecvrCoil_t f_recvr_Coil[], const VftSectionSignal_t f_signal_Coil[])const;
    CString GetPGUIDForProtocol(const int f_protocol_acq_order);
    //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    CString GetSequnceDirectory(const int f_acq_order);
    BOOL GetASCompassFlag(const int f_acqorder, BOOL* f_compass_flag)const;
    CPqmProtocol* GetProtocolForIndex(const int f_index)const;
    //Patni-Sribanta/2010June25/Added/IR-156_2
    CString GetIDENTValueFor(const int f_acq_order)const;
    void GetSeriesLOIDUnderProtocol(const int f_prot_index, std::vector<char*>* f_vect_series_loid);
    CPqmProtocol* GetTopWaitProtocol()const;
    CPqmProtocol* GetTopWaitOrCurrentProtocol() const;
    CString GetDBDTValueFromStudy(const int f_protocol)const;
    void GetParentInfo(int prot, CPqmPrMode* pqm_pr_mode);
    bool GetIsStudyReopened() const;
    BOOL GetShimEnableFlagFromVF(const int f_protocol)const;
    bool GetShimPathFromVF(const int f_protocol, VfPathElem_t* f_shim_path)const;
    CPqmProtocol* GetLastDone()const;
    void GetNewRflSAR(const int f_protocol_id)const;
    void GetValueFromVfSAR(CPqmProtocol* f_protocol)const;
    int  GetPreviousAgent(CPqmProtocol* protocol, int nos, float* rgn);
    //Patni-Sribanta/2010Aug11/Added/TMSC-REDMINE-557
    CPqmProtocol* GetCFStudyProt()const;


    BOOL GetProtocolValues(CVarFieldHandle* const f_vf_handle, int prot, bitfield_t*  mask, ...);
    status_t SetProtocolValues(CVarFieldHandle* const f_vf_handle, int prot, bitfield_t*  mask, ...);

    //Patni-MP&Sribanta/2011Apr08/Added/REDMINE-1610
    CString GetMappedIDForProcessedNode(const int f_acq_order, const int f_processed_node_index)const;
    int GetMRSValueFromVF(const CPqmProtocol* f_protocol)const;
    void SetMrsValueInVF(int f_Value, const CPqmProtocol* f_protocol)const;

    BOOL GetHasImageInStudy()const ;
    void SetHasImageInStudy(const BOOL study_has_image);

    int  GetCurrentHighestDbdt() const;
    void SetCurrentHighestDbdt(const int f_current_hgst_dbdt);

    CString GetStudyLoid()const;
    void    SetStudyLoid(const CString& f_study_loid);

    CString GetStudyPath()const;
    void    SetStudyPath(const CString& f_study_path);

    status_t GetTxCoilInSaturn(PqmSaturnRcvCoil_t* f_coil,  PqmCoil_t**	f_coil_p);
    bool     SetTxCoilInSaturn(CVarFieldHandle* const f_vf_handle, CPqmProtocol*        f_obj,
                               PqmSaturnRcvCoil_t*  f_coil);

    CPASCOMSequence* GetPascomSequenceObject(VARIANT* pData, const int f_acq_order);
    CPASCOMSequence* SetPascomSequenceObject(CPASCOMSequence* f_pascom_sequence, const int f_acq_order);

    char*    GetScanDateTime(const int f_protocol);
    status_t SetScanDateTime(char* f_scan_time);

    int      GetCurrentHighestSar() const;
    void     SetCurrentHighestSar(const int f_current_hgst_sar);

    void SetAutoMapProtocolsDegeneracyFlag(int f_degeneracy_flag, const int f_index)const;
    //Patni-PJS/2011Feb21/Added/IR-97
    void SetIsValidWFDAScan(BOOL f_is_valid_wfda_scan);
    void SetAcqStatus(CPqmProtocol* curr_protocol, const int acq_status);
    bool SetScanOffsetValue(const int index, const float f_scanoffset_x, const float f_scanoffset_y,
                            const float f_scanoffset_z, const BOOL f_set_mode = FALSE);
    void SetShimmingInfo(int f_protocolnum, BOOL f_isactiveshim);
    void SetScanModeOfHead();
    void SetCurrentSarDbdt(CPqmProtocol* f_pqm_prot, const bool f_everyprotocol_flag = false);
    void SetScanAnatomy(int f_scan_anatomy);
    void SetReceiverCoilID(long f_receiver_coilid);
    int  SetMRSExecutation(const int f_pro_index, int f_f0_value,
                           int f_ref_value, int f_wsat_value)const;
    SetstudyReopened(const bool f_istudyreopened);
    void SetSeriesLOIDReference(const int f_protocolnum, CString f_series_loid);
    void SetRGNCoverageMode(const int port, const int mode);
    PqmShim_t* SetShimmingOffset(const int f_behavior, const int f_protocol)const;
    BOOL SetProtocolValue(CVarFieldHandle* const f_vf_handle, CPqmProtocol*    f_obj, const int f_tag,
                          void* f_data, const int f_size, const BOOL  f_remake_item,
                          const int f_remake_part);
    bool SetIntensityFlagInStudy(bool f_value, CPqmProtocol* f_protocol = NULL);
    int  SetWBCoilSaturn(CVarFieldHandle* const f_vf_handle, const int             f_protocol,
                         PqmSaturnCoil_t* f_saturn_coil, PqmCoil_t*       f_coil,
                         const char            f_port);
    void SetNewSAR(const int f_protocol_id);
    void SetFirstScan(const BOOL first_scan);
    status_t SetProbeThresHold(const int  f_protocol);
    BOOL SetProtValues(CVarFieldHandle* const vf_handle, int prot, bitfield_t* mask, ...);
    void SetWaitToCurrent(CPqmProtocol* protocol);
    void SetDefaultRGNValue(const int node);
    void SetDelayTimeForProtocol(const int f_acq_order, const int f_delay_time);
    void CheckAutoLocLicensenSetStatus(CPqmProtocol* l_protocol);
    status_t CreatScratchPadFromStudy(const int f_protocol_acq_order, const char* const f_scratchpad, const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);

    status_t MaintainScratchPadForPrivateProlim(const int f_acq_order, const char* tmp_file, const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);
    void MaintainScratchPadforMRS(const int f_acq_order, char* tmp_file, const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);

    void RecalculateSAR(CPqmProtocol* f_protocol)const;
    BOOL GetTopWaitGateProt(CPqmProtocol** result_acqSeries)const;
    status_t DeleteCALAFields(const int f_acq_order);
    status_t DeleteHALAFields(const int f_acq_order);
    status_t GetTotalSAEValueFromVarfield(CPqmProtocol* f_protocol, int32_t f_total_sae[]);

    void CheckCFAValueinScratchPad(CPqmProtocol* f_current_prot, const int f_cfa_retry);
    void GetSequenceArchitecture(const int f_protocol, CString& f_seq_architecture)const;
    CString GetSequenceName(const int f_acq_order)const;
    //Patni-HEMANT/ADDED On 5/28/2009 2:56:26 PM/ ACE-2 / WFDA
    int  GetInscanFlagForProtocolIndex(const int f_protocol_acq_order);
    CString GetDefaultPGUID(const int f_inscantype);
    void SetPGUIDForProtocol(const int f_protocol_acq_order, CString& f_pguid);
    void UpdateInscanFlag(const CPqmProtocol* f_pqmProtocol);
    bool IsEditedInAcquirePage(const int f_protocol)const;
    int BodyPartGetVF(const int f_protocol_index, CString& f_body_part_value);
    int  CheckImgGr(CVarFieldHandle* const f_vf_handle, const int f_protocol)const; //AMB
    int  ResetOffset(CVarFieldHandle* const f_vf_handle, int f_protocol)const; //AMB
    int  AddProtocol(CVarFieldHandle* const f_vf_handle, const int f_to_prot, int f_to_pos, const bool_t f_copyContrast, CPqmProtocol** f_obj);

    bool CheckSlaveProtocolForDuplicate(int* f_selected_items, int f_count);
    void UpdateSARControlInfoOnProtocolUpdate(CPqmProtocol* f_protocol_obj);

    int GetRTSARTime(const int& f_protocol, SYSTEMTIME& f_rtsar_time)const;

    status_t RestoreSavedTagsAfterScratchpad(const int f_acq_order);
    status_t SaveTagsBeforeScratchpad(const int f_acq_order);
private:

    typedef std::vector<CPqmProtocol*> protocolvec;
    CPqm*           m_pqm_ptr;
    int             m_wait_pos;
    CString         m_study_loid;
    CString         m_study_name;
    CVarFieldHandle* m_vfdb_handle; //TMSC-Tanoue/2010Aug25/MVC008345
    study_id_t      m_study_id;
    float           m_weight;
    BOOL            m_first_scan;
    BOOL            m_has_image;
    long            m_total_time;
    int             m_prot_count;
    int             m_gender;
    protocolvec     m_protocol_vec;
    int             m_mode;
    float           m_height;
    CString         m_study_path;
    //Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_cs;

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_scan_mutex;

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    //Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
    mutable MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_protocol_vec_mutex;
    mutable MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_seq_transfer_mutex;

    //Patni-PJS/2009Oct29/Added/IR-92
    int				m_current_hgst_dbdt;
    int				m_current_hgst_sar;

    //+Patni-DKH/2010May03/Added/Phase3#IR105
    bool			m_studyreopen;
    //Patni/SS/2010July10/Added/NFD001-AutoMap

    //Patni-PJS/2011Feb21/Added/IR-97
    BOOL m_is_valid_wfda_scan;
    bool	m_rm_sae_status_for_study;

    class CPQMChildstudy : private MR::ACQ::SM::COMMON::NonCopyable<CPQMChildstudy>
    {
    public:
        explicit CPQMChildstudy(CPQMStudy* f_study) {
            m_study = f_study;
        }
        CPQMStudy* m_study;
    };


    int  Direction(const float x, const float y, const float z)const;
    bool WriteStudyLOIDinVarfield();


    //MEITEC watanabe ADDED 10/01/14 //.pdb_header file copy
    void PdbFileCopy();
    void RenumberPositionsInVector();
    void PutDBDT_NormalMode(CVarFieldHandle* const f_maintain_vf_handle, const int f_protocol_num)const;
    void DeleteHoldMode(const int f_protocol)const;
    void ConvCouchOffset(CPqmProtocol* protocol);

    int  CopyError(const int f_protocol, char* f_msg, char* f_file_name, const int f_line_no)const;
    int  ClearFields(CVarFieldHandle* const f_vf_handle, const int f_protocol)const;
    int  ResetProtInfo(CVarFieldHandle* const f_vf_handle, const int f_prot, void* f_data);
    status_t ListCopyCurrent(CVarFieldHandle* const f_vf_handle, const int f_pos, int* f_to_pos);
    status_t ListCopyItem(/*VfDBHandle_t f_vf_handle,*/const int f_from_pos, const int f_to_pos);
    void InitPatientPathfromCopy(CVarFieldHandle* const f_vf_handle, CPqmProtocol* l_obj, CPqmProtocol* l_fromobj);
    void ClearFieldsForCopy(CVarFieldHandle* const f_vf_handle, const int f_prot);
    int  VarfieldInitOffset(CVarFieldHandle* const f_vf_handle, VfPathElem_t* f_path)const;
    int  AddProtocolObject(CVarFieldHandle* const f_vf_handle, const int f_protocol, const int f_to_pos = 0);
    CPqmProtocol* CreateProtocolObject(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_protocol_object,
                                       const int f_protocol);

    BOOL CompareContrastAgent(CVarFieldHandle* const f_vf_handle, const int f_protocol1,
                              const int f_protocol2);

    int CreateScratchPad(const int f_protocol_index, const char* f_tmp_file, const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);

    //+Patni/SS/2010July10/Added/NFD001-AutoMap
    void CopyScanMode(CVarFieldHandle* const f_vf_handle, const int f_to_pos, const int f_copyContrast);

    //+Patni-Sribanta/2010Nov18/Added/REDMINE-772
    void CopyBackCouchInfoForlastDoneProtocol();
    BOOL ReadPatientInfoFromVarField(const CString& f_study_name);
    //-Patni-Sribanta/2010Nov18/Added/REDMINE-772

    //+Patni-Sribanta/2010Oct25/Added/IR-170
    bool LoadPasSequences(const CString& f_pas_path)const;
    bool CheckPasValidity(const CString& f_pas_path) const;
    bool CheckPasLicense(CString f_pas_path) const;
    bool LoadPasAndDisplay(const CString& f_pas_path)const;
    //-Patni-Sribanta/2010Oct25/Added/IR-170

    BOOL CheckProtocolsInDB(int& f_varfield_protocol_count)const;
    BOOL DisplayProtocolMismatchMessage(const BOOL f_message_display_flag)const;
    void FillObjectParam(int& f_protocol_index, CPqmProtocol& f_pqm_protocol,
                         CPASParamStore& f_pasparam_object);
    bool OnCreateVFStudyFile(CString f_file_name);
    status_t IsValidCopySrcProtNum(const int f_prot_num)const;
    //Patni-PJS/2010June1/Modified/MVC007434 CR

    //+Patni-PJS/2010Mar24/Added/IR-140
    status_t PopulateGUIControls(const int f_acq_order);
    status_t RestoreGUICtrlValInVarfield(const int f_acq_order, const bool f_is_plan_dynamic_executed = false);
    //-Patni-PJS/2010Mar24/Added/IR-140
    //+Patni-ARD/2010Mar10/Added/Ph-3# IR-100 code review
    void PrepPlanInit()const;
    void ProbePlanInit()const;
    //-Patni-ARD/2010Mar10/Added/Ph-3# IR-100 code review
    void InitializeVFPath(VfPathElem_t* f_vf_path)const;
    //+Patni-MP/2009July13/Added/ACE-2/Release 20th June
    bool CheckPostProcessing(const int f_protocol_index);
    bool PutPostscript(const int f_protocol_index, char* script)const;

    bool WritePrescanTagsForProtocol(CVarFieldHandle* f_vf_handle, VfFieldSpec_t* f_vfReq,
                                     const int f_total_elements,
                                     const int f_start_pos, const int f_end_pos,
                                     const bool f_write_to_scratchpad)const;

    BOOL ClearProtocolVector();
    //Patni-MP/2009Mar12/Added/ACE1-JET SCAN requirement

    void	 PqmPrStoreFile(char*	store_file, char*	to_file, char*	comment);
    status_t PqmPrCountNode(CVarFieldHandle* const vf_handle, VfPathElem_t*	nodePath,
                            const int		depth, int*		numNode);
    void	 PqmPrCheckVisualGr(char*		file_name, char*	 to_file, const int	prot,
                                char*	file, const int	line);
    void PqmPrCheckSliceNumber(char*	file_name, char*	to_file, const int	prot,
                               char*	file, const int	line);
    status_t PqmPrSetPatientWeight(const char* file);
    BOOL CheckForMultiCoverageScan(CPqmProtocol* protocol)const;

    bool IsThisFlagInScanKind(const int f_inscan_flag_value)const;
    CPqmProtocol* pqmAcqTopProtocol();
    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    bool SaveLaterality(const int& f_protocol_index, const int& f_laterality);
    void MoveProtObjInVector(const int f_from_pos, const int f_to_pos);


    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    static status_t pqmPmInitCouchAndSourceProt(CVarFieldHandle* const vf_handle, int	prot);
    //+Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    static status_t PqmPmMemoryResetProtInfo(CVarFieldHandle* const vf_handle, const int  f_protocol,
            void* f_mode, void* f_data);
    //-Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927

    void DeleteCFASpectrum(const int f_protocol)const;
    bool UpdateNumchannelforCoilChannelMode(const BOOL f_coil_channel_mode, CPqmProtocol* f_pqm_prot);
    void SetScanOffsetModeFlag(CPqmProtocol* const f_protocol, const int f_value) const;
    status_t UpdateRTSAEValue(CPqmProtocol* f_protocol);
    status_t PutTotalSAEValueToVarfield(CPqmProtocol* f_protocol, int32_t f_total_sae[]);


    static status_t GetProtocolFromStudy(VfDBHandle_t f_db_handle, int f_protocol, void* f_data);

    int  GetInitNumber(const int tag)const;
    BOOL GetShimDefault(PqmShim_t* f_shim)const;
    int  GetParentIndexFplanning(const int protocol)const;
    status_t GetProbeData(float* f_max, float*   f_min)const;
    void GetPasParamObject(CPASParamStore& l_object_param, int f_protocol_index);

    int  GetINTValues(const unsigned long f_vftype, const unsigned long f_vfname,
                      const int f_depth, const int f_protocol_index)const;

    //+Patni-NP/2009Sep21/Commented/MVC004865
    int GetLONGValues(const unsigned long f_vftype, const unsigned long f_vfname,
                      const int f_depth, const int f_protocol_index)const;

    float GetFLOATValues(const unsigned long f_vftype, const unsigned long f_vfname,
                         const int f_depth, const int f_protocol_index)const;

    CString GetSTRValues(const unsigned long f_vftype, const unsigned long f_vfname,
                         const int f_depth, const int f_protocol_index)const;

    CString GetVectorValues(const unsigned long f_vftype, const unsigned long f_vfname,
                            const int f_depth, const int f_protocol_index)const;
    bool GetLicenseStringForPath(CString& f_pas_path) const;
    bitfield_t* GetActiveNodeBranches(VfPathElem_t* path, int* tot_node_branch, unsigned int& node_name);

    int MaintainBaseTempToCFStudy(const CString& f_file, const CPqmProtocol* f_current_protocol) const;

    // Retrieving protocols from the study file
    CPqmProtocol* GetLastProtocol()const;
    CPqmProtocol* Get1stScanDone()const;
    int  GetProtocolCountFromDB()const;//MRP
    int  GetSaturnCoil(CVarFieldHandle* const f_vf_handle, const int f_protocol, PqmSaturnCoil_t* f_saturn_coil)const;
    int  GetConnectedCoil(CVarFieldHandle* const f_vf_handle, const int f_protocol, CPqmProtocol* f_protocol_object);

    bool GetLaterality(const int& f_protocol_index, int& f_laterality_value)const;
    //+Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
    CString GetReuseShimValue(const int f_protocol)const;
    CString GetReuseMapValue(const int f_protocol)const;
    bool GetMapPathFromVF(const int f_protocol, VfPathElem_t* f_map_path)const;
    //-Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285

    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    int BodyPartSetVF(const int f_protocol_index, CString f_body_part_value);
    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    void SetNewRflSARForProtocol(SequenceList& f_sequence_list);
    BOOL SetProtocolObject(CVarFieldHandle* const f_vf_handle, CPqmProtocol* pr_obj_p);
    void SetTopWaitPos(const int waitpos);
    int  SetScanDateTime(long f_time);
    BOOL SetAnatomyInDB(int anatomy, const int protocol);
    void SetPreScanValue(const int f_acq_order);
    void SetSiteId(const int site_id);
    void SetRumNum(const int rum_num);
    void SetupShimming(CVarFieldHandle* const f_vf_handle);
    void SetStudyName(const CString& f_study_file_name);
    status_t SetPVCoilTbl(const int		port, int*		rcvCoilTbl, const int		n);
    int  SetAgentValueVF(const int f_protocol_index, const CString& f_scan_agent_value);

    BOOL GetMasterFlagfromVF(const int f_acq_order);
    int GetGroupIDfromVF(const int f_protocol_acq_order);
    int GetGroupNumFromVF();
    BOOL CheckMulitScanDynamic_License(CPqmProtocol* f_obj, const int f_protocol);//Redmine-2187
    bool CheckInscanPguidPrefix(const CString& f_pguid, const int l_inscanflag)const;
    status_t DeleteInscanPGUID(const int f_acq_order)const;
    void UpdateScratchPadForSARControl(const char* f_tmp_file, const SCRATCHPAD_CREATION_MODE f_sp_creation_mode);
    void UpdateFixedOpeModeForSARControl(CPqmProtocol* f_protocol_obj);
    const CString GetSarControlParamStoreSarInfoFor(CPqmProtocol* f_protocol_obj,
            const sarType_t f_sar_control_type, const bool f_is_scan_completed) const;
};
#endif // #ifndef __SM_IPCLIB_PQMSUTDY_H__ 
