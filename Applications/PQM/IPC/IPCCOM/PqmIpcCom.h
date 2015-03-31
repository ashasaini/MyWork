/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmIpcCom.h
 *  Overview: Structure of CPqmIpcCom class.
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
#ifndef __PQMIPCCOM_H__
#define __PQMIPCCOM_H__

#include <PQM/PQMCommanMessages.h>
#include <PQM/PQMLogMgr.h>
#include "IPCComCP.h"
#include "Resource.h"

class CPQMIPCManager;

/////////////////////////////////////////////////////////////////////////////
// CPqmIpcCom
class ATL_NO_VTABLE CPqmIpcCom :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CPqmIpcCom, &CLSID_PqmIpcCom>,
    public IConnectionPointContainerImpl<CPqmIpcCom>,
    public IDispatchImpl<IPqmIpcCom, &IID_IPqmIpcCom, &LIBID_IPCCOMLib>,
    public CProxy_IPqmIpcComEvents< CPqmIpcCom >
{
public:
    CPqmIpcCom();
    ~CPqmIpcCom();
    DECLARE_REGISTRY_RESOURCEID(IDR_PQMIPCCOM)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CPqmIpcCom)
    COM_INTERFACE_ENTRY(IPqmIpcCom)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    END_COM_MAP()
    BEGIN_CONNECTION_POINT_MAP(CPqmIpcCom)
    CONNECTION_POINT_ENTRY(IID__IPqmIpcComEvents)
    END_CONNECTION_POINT_MAP()

    // IPqmIpcCom
public:
    STDMETHOD(OnPQMExit)();
    STDMETHOD(GetCoilNameFromAcqOrder)(int f_acq_order, VARIANT**  coilnamefrom);
    //Nitin coil
    STDMETHOD(GetCoilNameFromTransmiter)(int f_acq_order, VARIANT** coilnamefrom);
    STDMETHOD(GetCoilNameFromReceiver)(int f_acq_order, VARIANT** coilnamefrom);

    //Nitin coil
    //Patni-ARD/2010Apr08/Commented/JaFT# IR-100 code review
    //SM3_Mangesh_Start
    //STDMETHOD(GetContrastAgent)(VARIANT** pData);
    STDMETHOD(GetScanAnatomy)(VARIANT** pData);
    //SM3_Mangesh_End

    //+Patni-Ajay/2010July23/Added/PlanDuplicate/REDMINE-443
    STDMETHOD(GetPresetItem)(VARIANT** pData);
    //+Patni-Ajay/2010July23/Added/PlanDuplicate/REDMINE-443
    STDMETHOD(SetScanOffsetValue)(int index, float f_scanoffset_x, float f_scanoffset_y, float f_scanoffset_z, const BOOL f_set_mdoe);

    //Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    STDMETHOD(GetScanCommentXMLData)(VARIANT** pData);
public:
	STDMETHOD(CanTiPrepEnable)();
	STDMETHOD(IsProtocolforTiPrep)(int f_acq_order);
	STDMETHOD(GetTiPrepLicense)(const int f_tiprep_type);
	STDMETHOD(GetGDCSeriesLOIDForProtocol)(int * f_prot_idx, VARIANT * f_seriesloid);
    STDMETHOD(GetAPCRetry)(BOOL* f_is_apc_retry);
    STDMETHOD(CheckAMBMenuClickConditions)(int* f_selected_items, int count, BOOL* f_return);
    STDMETHOD(GetIndexForParentImageRemoved)(int* f_acq_order);
    STDMETHOD(SetIsAMBPlanning)(const  BOOL f_is_amb_planning);
    STDMETHOD(PqmPqAMBSlaveLost)(int* f_selected_list , int* f_selected_count, BOOL* f_slave_exist);
    STDMETHOD(AMBSlaveMatchSequenceParam)(VARIANT* f_pData, const int f_start_index, const int f_end_index);
    STDMETHOD(GetAMBSlaveCount)(int* f_count);
    STDMETHOD(CheckSlaveProtocolForDuplicate)(int* f_selected_items, int f_count);
    STDMETHOD(GetAMBIndexToUdpate)();
    STDMETHOD(GetAMBShimMapArray)(VARIANT* f_data);
    STDMETHOD(GetAMBLicense)();
    STDMETHOD(GetAMBMasterProtocols)(VARIANT* f_data);
    STDMETHOD(GetAMBAutoScanItems)(int** f_arr_items, int* f_count, BOOL* l_is_auto_scan_on);
    STDMETHOD(AMBReArrange)(int f_drag_index, int f_drop_index);
    STDMETHOD(GetAMBRearrangeIndexes)(int* f_rotocol_index_to_move, int* f_target_index);
    STDMETHOD(AMBScanStart)(VARIANT* data, BOOL* f_is_scan_start);
    STDMETHOD(GetAmbPlan)(VARIANT* data);
    STDMETHOD(AMBScanCancel)(VARIANT* data);
    STDMETHOD(AMBScanAbort)();
    STDMETHOD(GetAMBStatusData)(int* f_selected_items, int count, VARIANT* data);
    STDMETHOD(GetScanMutex)(VARIANT* f_mutex);
    STDMETHOD(GetStudyLock)(VARIANT* f_mutex);
    //Patni-PJS/2011Feb21/Added/IR-97
    STDMETHOD(SetIsValidWFDAScan)(BOOL f_is_valid_wfda_scan);
    STDMETHOD(SetAutoLocatorRunning)(BOOL f_cala_appln_runng);
    STDMETHOD(GetProtIndexAndWorkingMode)(int f_acq_order, VARIANT* pData);
    STDMETHOD(GetProtIndexAndWorkingModeForDoneProt)(VARIANT* pData);
    STDMETHOD(IsValidSequenceToInvokeAutoLocator)(const int f_acq_order, const int f_autolocatortype);
    //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    STDMETHOD(InvokeMRSProcessTool)(const int f_acq_order);
    STDMETHOD(GetMRSLicense)();
    STDMETHOD(GetMRS_BRST_License)();
    STDMETHOD(GetPostProcessFlag)();
    STDMETHOD(IsSelectedSeqMRS)(const int f_acq_order);
    //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    STDMETHOD(GetToolMenuXMLData)(VARIANT** pData);
    STDMETHOD(GetSFTParamValue)(VARIANT** f_param, int f_acq_order);
    STDMETHOD(GetSFTMenuXMLData)(VARIANT** pData);
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    STDMETHOD(SetAvSpdrMap)(BOOL f_flag_val);
    //Patni-PJS/2010Apr22/Added/JaFT# MVC007277
    STDMETHOD(GetCouchMethodForProt)(int f_acq_order, VARIANT* f_pData);
    STDMETHOD(SetScanProtocolInfoFromGUI)(VARIANT* pData); //Patni-HAR/2010Feb24/Added/MSA0251-00187
    //Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
    STDMETHOD(GetScanTimeForCurrProt)(VARIANT* f_pdata);
    STDMETHODIMP SetFirstWaitProtForAutoScan(const int f_acqorder);
    STDMETHOD(SetChildThreadStatusToIPC)(int* f_pData, int f_count, int f_child_thread_status);
    STDMETHOD(GetASCompassFlag)(int f_acqorder, BOOL* f_compass_flag);
    STDMETHOD(SetCDSCheckCoil)(int f_check);
    STDMETHOD(CoilSelectionClosed)(BOOL f_ok_pressed);
    STDMETHOD(CDSDebugClosed)(VARIANT** f_compass_debug_param, BOOL f_set);
    STDMETHOD(OnOffAsCompass)(int f_protocol, BOOL f_OnOff);
    STDMETHOD(OnOffCDSDebug)(BOOL f_OnOff);
    STDMETHOD(GetASCOMPASSParameter)(VARIANT**  f_cdscompasssetting_param);
    STDMETHOD(SetASCOMPASSParameter)(VARIANT**  f_cdscompasssetting_param);
    STDMETHOD(GetCDSDebugParam)(VARIANT**  f_compass_debug_param);
    STDMETHOD(GetMagneticCenterPos)(VARIANT** f_data);
    STDMETHOD(UpdateProtocolDataInSeries)(VARIANT* pData, BSTR f_seriesid, int f_prot_index); //+Patni-RSG/Added/2009Nov25/MSA0248-0033
    STDMETHOD(GetCoilSysSetting)(VARIANT* f_data); //Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel
    STDMETHOD(GetGUIListProtocolInfo)(VARIANT* pdata, int f_count_gui_list);
    STDMETHOD(RecvCallBack)(VARIANT* messageParam, VARIANT* messageRet, VARIANT* pCustom);
    STDMETHOD(OnPQMDuplicateProtocol)(int f_index_to_duplicate, int f_duplicat);
    STDMETHOD(OnPQMDeleteProtocol)(int f_protocol_index);
    STDMETHOD(GetStudyPath)(VARIANT* f_study_path);
    //Patni-HAR/2010Jun23/Modified/MVC008324_2
    STDMETHOD(GetCurrentSarInfoToDisplay)(VARIANT** pdata);
    STDMETHOD(ConnectToProconFromGUI)();
    STDMETHOD(GetSequenceInformation)(VARIANT* f_data, int f_acq_order);
    STDMETHOD(CheckNextProtocol)(int l_protocol_no, BOOL f_call_frm_gui = FALSE);
    STDMETHOD(ConnectToAcqMan)();
    STDMETHOD(SendAcqInfoForGate)();
    STDMETHOD(GetGateInfoList)(VARIANT* pData);
    STDMETHOD(GetCurrentWSEEditProt)(VARIANT* f_data);
    STDMETHOD(GetControlValueFromDB)(VARIANT* f_pdata, int f_scan_index);
    STDMETHOD(GetConnectedCoilInformationForGUIDisplay)(BOOL f_flag);
    STDMETHOD(GetConnectedCoilListForDisplayFromIPC)(VARIANT* f_data);
    STDMETHOD(SetBatchForSARNCoil)(BOOL f_flag);
    STDMETHOD(SetMoveLocatorMode)(BOOL f_flag);
    STDMETHOD(SetDBDtCancelFlag)(BOOL f_dbdt_flag);
    STDMETHOD(GetInsideLimit)(int f_index, int* f_in_side_limit, int* f_out_side_limit, BOOL* f_enable);
    STDMETHOD(SendConnectedCoilListToIPC)(VARIANT* pData, int f_protocol);
    STDMETHOD(SendSelectedCoilListToIPC)(VARIANT* pData, int f_protocol);
    STDMETHOD(GetSelectCoilList)(VARIANT* f_data);
    STDMETHOD(GetShowCoilList)(VARIANT* f_data);
    STDMETHOD(GetCoordinateList)(VARIANT* f_data);
    STDMETHOD(SendCoilSelReadyToIPC)(int f_index);
    STDMETHOD(GetConnectCoilInfo)(VARIANT* f_data);
    STDMETHOD(SendMessageToAcqManForCoil)(int f_mode, int f_acq_order);
    STDMETHOD(GetCoilSelectionData)(VARIANT* f_pdata, int f_mode);
    STDMETHOD(SetAutoView)(int);
    STDMETHOD(SetPlanAndScan)(int);
    STDMETHOD(SetAutoPlan)(int);
    STDMETHOD(SetSkipRgn)(int);
    STDMETHOD(SetAllCoverageRgn)(int);
    STDMETHOD(SetPrepWbCoil)(int);
    STDMETHOD(PqmSeverConfigToProCon)(int, int, int*, int);
    // MEITEC/2010Aug06/Added/REDMINE-443
    STDMETHOD(ProconReqUiCB)(int, int , int);
    STDMETHOD(MrsScanStartUiCB)(VARIANT*);
    STDMETHOD(GetMRSDefaultValue)(VARIANT**);
    STDMETHOD(GetManualPreScanDefaultData)(VARIANT** f_pdata);
    STDMETHOD(PQMManualPrescanApply)(VARIANT* f_pdata);
    STDMETHOD(PQMManualPrescanClose)();
    STDMETHOD(SetPrescanHook)(BOOL f_prescan_hook);
    STDMETHOD(SetShimFSYFlag)(int f_shimfsy_flag);
    STDMETHOD(GetSeqDataofDisplaySequences)(VARIANT* pdata, int f_mode, int f_firstseq);
    STDMETHOD(SetShimmWBCoil)(BOOL f_shim_wb_coil);
    STDMETHOD(ReArrange)(int f_drag_index, int f_drop_index);
    STDMETHOD(DuplicateSequenceInVarfield)(int f_pos_list);
    STDMETHOD(DuplicateProtocol)(int f_from_pos, int f_to_pos, int f_group_id);
    //Patni-HAR/2010Sep20/Modified/Rename function from SendIndex() to DeleteProtocolFromVarField()
    //Patni-PJS/2011Jan29/Modified/IR-97
    STDMETHOD(SendIndex)(int f_index);
    STDMETHOD(DeleteProtocolFromVarField)(int f_index);
    STDMETHOD(GetDBDTCurrentMode)(int f_index , int*); 	   //Patni-NP/2009Sep15/Added/MVC004766
    STDMETHOD(SetPQMMode)(/*[in]*/ int mode);
    STDMETHOD(SetExeAutoVoice)(int);
    STDMETHOD(SendSequencesToMemory)(VARIANT* pData);
    //	STDMETHOD(PQMDisplayScanError)(/*[in]*/ int f_errorid, /*[in]*/ int f_errorlevel, /*[in]*/ BSTR f_errorsource);				  // Added By KT/ACE-2/15-06-2009/Multilingualisation
    STDMETHOD(PQMDisplayScanError)(/*[in]*/ int f_errorid, BSTR f_errstr,/*[in]*/ int f_errorlevel, /*[in]*/ BSTR f_errorsource); // Added By KT/ACE-2/15-06-2009/Multilingualisation
    STDMETHOD(GetSeriesCount)(int* seriescnt);
    STDMETHOD(SetAnatomy)(int f_anatomy_id);
    STDMETHOD(SetReceiverCoilID)(long f_receiver_coilid);
    // + Patni-Sribanta/2010Jun09/Modified/MVC8032
    //STDMETHOD(SetMultiCovCaution)(BOOL f_multicovcaution);
    STDMETHOD(UpdateGUIControlsInDB)(VARIANT* pData, BSTR f_seriesid);
    STDMETHOD(GetAPCHoldValue)(/*[out,retval]*/ BOOL* holdvalue);
    STDMETHOD(SetAPCHoldValue)(/*[in]*/ BOOL holdvalue);
    STDMETHOD(ChangeShimCenterFrequency)(/*[in]*/ double fsyvalue);
    STDMETHOD(StartCFAPrescan)(/*[in]*/ int scanindex);
    STDMETHOD(StartRFLPrescan)(/*[in]*/int scanindex);
    STDMETHOD(PutInjectTimeInDB)();
    STDMETHOD(SendGateModeInfo)(int f_gate_mode);
    STDMETHOD(PQMDisplayError)(BSTR error_msg, BSTR file_name, UINT line_Number, int error_level, DWORD msg_id, BSTR event_source, int f_detail_error_log);
    STDMETHOD(WriteEventLog)(BSTR err_msg, int error_level, BSTR code_line, int error_line, int detail_error_log);	//Patni-MP/2010May24/Modified/IR-82
    STDMETHOD(WriteOpeLog)(WORD opeation_id);
    STDMETHOD(WriteTraceLog)(LPCTSTR domain_name, unsigned long marker_val, LPCTSTR func_name, LPCTSTR log_text, int detail_trace_log);
    STDMETHOD(GetMaxProtocolLimit)(long* MaxProtLimit);
    STDMETHOD(DeleteSequence)(BSTR bstr);
    STDMETHOD(GetSeqParamNameList)(VARIANT* pData);
    STDMETHOD(DuplicateSequence)(VARIANT* pData, BSTR bstr, const int f_index_duplicating_prot);
    STDMETHOD(GetSeqData)(VARIANT* pdata, int mode);
    //Patni-PJS/2010Nov23/TMSC-REDMINE-774
    //STDMETHOD(GetCoilData)(VARIANT* pData);
    STDMETHOD(OnPQMCloseButton)();
    STDMETHOD(UpdateComment)(BSTR bstr, BSTR scancomment, BSTR scanid);
    STDMETHOD(SetAutoVoice)(BOOL flag);
    STDMETHOD(ConnectVoiceManager)(int voice_param);
    STDMETHOD(ReceiveSeriesOrder)(BSTR bStr, int dropindex);
    STDMETHOD(ReceiveSeriesAll)(BSTR bStr);
    STDMETHOD(ReceiveLOID)(BSTR f_strpatientloid, BSTR f_strstudyloid, BSTR f_pas_path); //Patni-Sribanta/2010Nov10/Modified/IR-170
    STDMETHOD(GetColorDataEx)(VARIANT* data);
    STDMETHOD(UpdateGUIControlsInVarfieldDB)(VARIANT* data, int f_scan_index);
    STDMETHOD(SendElectrodeType)(int);
    STDMETHOD(SetActiveDialog)(int f_nDialogType, BOOL f_bSet);//ashish SM3
    //+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
    STDMETHOD(GetSarInfoPtr)(VARIANT* pData);
    STDMETHOD(SetProtocolIndex)(int f_index);
    //- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
    STDMETHOD(FileCopy)(BSTR f_existingfilename, BSTR f_newfilename); //Patni-ARD/20090309/Added/ACE-1# handling of stabilization
    // Initialize of PQM libarary
    //Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
    STDMETHOD(Initialize)(int f_mode, VARIANT* f_config_data);

    // Start scan
    //Patni-ARD/2009Sept01/Modified/IR GUI IPC check SEQ
    STDMETHOD(StartScan)(VARIANT* f_pVariant, int);

    // Abort scan
    STDMETHOD(AbortScan)();

    // Abort MRS scan
    STDMETHOD(MrsScanClose)();

    // Pause scan
    STDMETHOD(PauseScan)();

    // Resume scan
    STDMETHOD(ResumeScan)();

    //+ Patni-PJS/2009Jul29/Modified/MVC0003961
    STDMETHODIMP GetCouchPos(VARIANT* f_pData);
    //- Patni-PJS/2009Jul29/Modified/MVC0003961

    //+Patni-Sribanta/2011Jan06/Commented/TMSC-REDMINE-1026
    ////Patni-PJS/2009Sep23/Added/MVC4941
    STDMETHOD(InitializeTicks)();
    //-Patni-Sribanta/2011Jan06/Commented/TMSC-REDMINE-1026

    //Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved
    STDMETHOD(GetNewSeriesLoids)(VARIANT* f_newseriesloids);
    static bool WrapperRaiseEvent(pqm_msg_tag Msg, void* ptrObj);
    //Patni-ARD/2009June17/Added/JFT# Acqorder checking between IPC & GUI
    STDMETHODIMP GetIPCTopMostWaitAcqOrder(int* f_acqorder);

    //Patni-ARD/2009July22/Added/JFT# 177
    STDMETHODIMP GetHasParent(int f_acq_order);

    //Patni-Manishkumar/2009Jul29/Modified/MVC003316
    STDMETHODIMP GetTotalSequeunces(VARIANT* f_pVariant);

    //Patni-Manishkumar/2009Oct12/ADDED/MVC004921/Loc.enable
    STDMETHOD(GetPqmPqIsGlocOk)();

    //Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
    STDMETHODIMP GetEnableDisableAutoTransfer();

    //Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
    STDMETHODIMP GetPatientScanoffset(int f_acq_order, VARIANT**  f_scanoffset);

    bool RaiseEvent(pqm_msg_tag Msg);

    //Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
    STDMETHODIMP RemoteCheckActiveFlag();

    //+Patni-DKH/2010Jan29/Added/PH#3#/IR86/DebugWindow#
    bool RaiseDisplayEvent(BSTR f_debugdata);
    static bool WrapperRaiseDisplayEvent(BSTR f_debugdata, void* ptrObj);
    STDMETHODIMP StartPQMDebug(int f_debugOption, UINT f_buttonmask) ;
    STDMETHODIMP PqmDebugStatusLogOpen();

    STDMETHODIMP SelectListandTotalCnt(int* f_selected_list, int f_size);//Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow
    //-Patni-DKH/2010Jan29/Added/PH#3#/IR86/DebugWindow
    //+Patni-DKH/2010Jan12/Added/IR-87
    STDMETHODIMP InitPlanDuplicateSettings();
    STDMETHODIMP ReadPlanDuplicateSettings(VARIANT** f_data);
    STDMETHODIMP WritePlanDuplicateMask(long f_imask , BOOL f_bsaveflag);
    //-Patni-DKH/2010Jan12/Added/IR-87
    //Patni-DKH/2010Jan29/Added/PH#3#/FBI
    STDMETHODIMP SetPqmPmDelay(int f_delay1, int f_delay2);
    //Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
    STDMETHODIMP OnShowHidePQM(BOOL f_show_flag);

    //+Patni-PJS/2011Jan29/Modified/WFDA/IR-97
    STDMETHODIMP IsProtocolInScan(int f_acq_order);
    STDMETHODIMP CheckPlnStatus(int f_acq_order);
    //-Patni-PJS/2011Jan29/Modified/WFDA/IR-97

    //Patni-PJS/2011Jan29/Modified/WFDA/IR-97
    STDMETHODIMP GetProtocolInformation(VARIANT* pData, int f_acq_order);
    //Patni-DKH/2010Jun24/Added/NFD001AutoMap
    STDMETHODIMP GetAutoInsertedSeqDataToDisplay(VARIANT* pdata, int* f_insertedmappos, int* f_target_pos, int* const f_is_shim_required, int* const f_is_map_required);
    STDMETHODIMP GetTargetProtInformation(int* f_target_pos);

    //Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1
    STDMETHODIMP GetScanAgent(const int f_protocol_index, VARIANT* f_scan_agent);
    //Patni-Sudhir/2011Jan13/Added/Redmine-434
    STDMETHODIMP GetNRFilterStringForProtocol(const int f_acq_order, const int f_NRFLG, VARIANT* f_NRVal);
    STDMETHODIMP GetEMTFilterStringForProtocol(const int f_acq_order, VARIANT* f_EMTVal);
    STDMETHODIMP GetAutoLocatorPlannedStatus(const int f_acq_order, int* f_cala_planned_status);
    STDMETHODIMP GetAutoLocatorLicense(const int f_autolocator_type);
    STDMETHOD(GetProtocolInfoForAutoCopy)(VARIANT* pdata);
    STDMETHOD(UpdateGroupNum)();
    STDMETHOD(AllocateGroupIdForDuplicateProtocols)(VARIANT* f_pdata);
    STDMETHOD(OnDeleteUpdateMasterSlave)(VARIANT* f_pdata);
    STDMETHOD(ExecuteAutoScanStart)();
    STDMETHOD(CalculateTimeDiff)(VARIANT* f_pdata);
    STDMETHODIMP GetProtectedStatus(BSTR bstr);
    STDMETHOD(SetDelayTimeToVf)(VARIANT* f_pdata);
    STDMETHOD(GetMulitScanDynamic_License)();//Remine-2187
    STDMETHODIMP SetChannelModeSelection(const int f_selected_item, BOOL f_selectedchannel_mode, const int f_system_channel);
    STDMETHODIMP GetChannelModeSelection(const int f_selected_item, VARIANT* f_pdata);
    STDMETHODIMP GetNumSignalfromSelectedSection(const int f_selected_item, VARIANT* f_pdata);
    STDMETHODIMP SetNumChannels(const int f_prot, const int f_channel);
    STDMETHODIMP SetNumChannelsforChannelMode(const BOOL f_use_coil_channel, const int f_system_enable_channel, const int f_limited_channel); // 20-Oct-11

    STDMETHODIMP IsWireLessSystem();
    STDMETHODIMP SendWirelessGateParam(VARIANT* f_pdata);
    STDMETHODIMP GetWirelessGatingSelections(VARIANT* f_data);
    STDMETHOD(SetWirelessParamtoProt)(VARIANT**  f_wireless_param);
    STDMETHOD(CheckAndGetWaitingForAutoScanStartAcqOrder)(int* f_waiting_for_auto_scan_acq_order);
    STDMETHOD(GetMasterPositionForSlave)(const int f_slave_index, int* f_master_position);
    STDMETHOD(ChangeSlaveToNormalProtocol)(const int f_slave_index);
    STDMETHOD(GetProtocolIndexToUpdate)(int* const f_protocol_index);
    STDMETHOD(GetMasterListForPlanMark)(VARIANT* f_pdata);
    STDMETHOD(PerformAutoCopyForMasterSlave)(VARIANT* const f_pdata);
    STDMETHOD(ResetAutoScanTimer)();
    STDMETHODIMP UpdateSARInfo();
    STDMETHOD(GetRunningProcCount)(int* f_proc_count);
    STDMETHOD(SendSAEInfo)();
    STDMETHOD(GetShimState)(int* f_shim_state);

    STDMETHOD(ReConnectSARManager)();
    STDMETHOD(HandleAbortFailForProtocol)();
    STDMETHOD(CalcSARControlAndPutVFForAll)();
    STDMETHOD(GetSARControlLicense());
    STDMETHOD(GetMaxTimeForPagerTimer(int* f_fax_pager_timer_time));
    STDMETHOD(GetSarInfoPtrForIndex)(VARIANT* f_pdata);

private:
    CPQMIPCManager* m_ptrManager;
};

#endif //__PQMIPCCOM_H__
