//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-777
//Redmine-774
//Redmine-781
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMView.h
 *  Overview: Structure of CPQMView class.
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
#ifndef __SM_PQM_VIEW_H__
#define __SM_PQM_VIEW_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/ButtonEx.h>

//FOR COIL ---
#if MPLUS_V3
#include "Psapi.h"
#include <MPlusProcessManager/ProcessManager.h> //ashish SM3
#endif

//Patni/KSS/2011Mar31/Added/TMSC-REDMINE-773 & TMSC-REDMINE-1486-Part1
#include <libStudyManager/Mutex/Mutex.h>


#include <PQM/IPCCom.h>
//Patni-PJS/2011Feb1/Modify/IR-97
#include "WFDAImplementer.h"

#include "CustomStatic.h"
#include "PQMDoc.h"

#include "IPCComManager.h"
#include "PQMListCtrl.h"
#include "PQMGlobal.h"

#include "ControlStateMgr.h"
#include "InterfaceClasses.h"
#include "PqmChildThread.h"

#include "PQMPlanDupDlg.h"
#include "PQMDebugWinDlg.h"

#include "AutoLocatorAppController.h"
#include "WirelessGatingController.h"
#include "TiPrepAppController.h"

#include "PQMAMBController.h"


class CCoilSelWin;
class CPQMCommentDlg;
class CPQMInfoSeqDlg;
class CPQMCFADlg;
class CPQMManualPreScanDlg;
class CPqmdBdtCautionDialog;
class CCOMPASS_Location_Debug;

class CCoilSelectionUtility;
class CSink;
class InterfaceForPQMManualPreScanDlg;

class CPQMGateInfoDlg;
class CPQMAnatomyDlg;
class CPQMSetScanOffsetDlg;
class CPQMCoilInfoDlg;

class CQueueDialog;
class CPatientOrientationDialog;
class CMRSPreScanDlg;

class CPQMCDSController;
class CSFTMenuHandler ;

class CPasswordDlg;	//Patni-Sudhir 2011Mar-09/Added/IR-185

class CDelayTimeDlg;
class CCoilChannelMode;

enum CurrentScanOpration {

    CurrentScanOpration_ScanPause,
    CurrentScanOpration_ScanResume,
    CurrentScanOpration_ScanStart,
    CurrentScanOpration_ScanAbort,
    CurrentScanOpration_Invalid
};

enum result_r {
    TYPE_FALSE,
    TYPE_ERR_MANAGER_EXCP,
    TYPE_TRUE
};

//+Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
#define MEMORY_INVOKATION_STARTS		0x0001		//binary 0001
#define MEMORY_PASSWORD_DIALOG_OPEN		0x0002		//binary 0010
#define MEMORY_INVOKATION_ENDS			0x0004		//binary 0100
#define MEMORY_PRESENT					0x0008		//binary 1000
//-Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open


typedef struct SContrastAgent {
    CString  m_strname;
    int  m_index;
} ContrastAgent;

typedef CList<ContrastAgent, ContrastAgent> CContrastAgentData;
typedef CList<SScanAantomy, SScanAantomy> CScanAnatomyData;

typedef struct SSrParamInfo {
    CString  m_strPatID;
    CString  m_strStudyID;
    CString  m_strPSPOID;
    CString  m_strDBName;
    CString	 m_strPASFilename;

    void Clear() {
        m_strPatID = _T("");
        m_strStudyID = _T("");
        m_strPSPOID = _T("");
        m_strDBName = _T("");
        m_strPASFilename = _T("");
    }
} SrParamInfo;


//+Patni-Sudhir 2011Mar-09/Added/IR-185
typedef struct Mem_Mode_Data {
    TCHAR mem_mode[100];
} MemModeData ;
//-Patni-Sudhir 2011Mar-09/Added/IR-185


typedef CArray <CString, CString&> paramList;

enum APC_Scan_Enum { APC_EnableFromPQM , APC_DisableFromPQM };

typedef enum {
    NONBLINKING = 0,
    BLINKING
} BLINK_STATUS;

class CPQMView : public CFormView
{
public:
    CPQMView();
    virtual ~CPQMView();

    void ResetAMBPlanProtocols();
    void OnAMBAutoScanStartforFirstSlaveProtocol();
    bool AMBScanStart();
    bool AMBScanStartThread();
    void SetBatchSettingOnOff(const bool f_onoff);
    void AMBPerformSlaveDuplicate();
    void AMBSlaveDuplicate();
    void RecAmbEditExit();
    void RecAMBPqmScanMsg();
    void RecAmbScanWait();

    void RecAmbScanStartError();
    bool MoveAMBProtocolToIndex(const int protocol_index_to_move, const int target_index);
    void AMBApplyAutoScanToItems();
    void AMBUpdateSlaveprotocols();
    void AMBAddShimMap();

    void UpdateAMBStatusFail();
    void DeleteAMBSlaveIndex();
    void UpdateOtherButtonState();

    void AMBRearrange();
    void AMBSlaveScanStart();
    bool GetIsWSEOpen()const;
    int GetProtocolIndexFromAcqorder(const int f_protocol_acqorder)const;


    CString CheckPASselectionFile()const;
    void ChangeViewOfModelessDialogs(const bool f_bCheck);
    void SendDeleteReserveDEGL()const;
    void SendDeleteCompleteDEGL()const;
    void HandleSFTMenuCommand(UINT f_cmd_id)const;
    void OnOffCDSDebug()const;
    //Patni-Hemant/2011Apr20/Modified/TMSC-REDMINE-1671
    void DetachSFTToolmenu()const;
    void AttachSFTMenu(PopupMenuEx* f_menu)const;
    void AttachPlanDuplicateMenu(const int f_protocol_index, PopupMenuEx* f_protocol_menu);
    void UpdateCurrentSequence()const;
    void ChangeScanTime();
    void BlinkPage();
    void UpdatePagerAreaFor()const;
    void UpdateSarInfo();
    void DisplayDbDtCautionDialog();
    void DisplayOffset()const;
    void DisplayCoilInfo();
    void ApplyCoilToProtocols();
    // Same study is opened in both page
    bool SameStudyOpened();
    bool IsAcqumanReadyForNextRequest()const;
    void DisplayGateDialog();
    void OnDialogClosedForParameterEditing(int item_index, const bool prev_auto_scan_flag_status) ;
    void EnableLocatorButton();
    void OnFlagOfLocatorEnable();


    bool CanPerfornManualPrescan()const;
    void StartPQMDebug(const debug_option f_en_debug_option, const UINT f_buttonmask)const;
    void UpdateDebugWindow(const CString& f_csDebugdata);
    void CoilSelSetCenterPos()const;
    bool IsCDSDebugON()const;
    void OnAxisLocatorDialogClosed(const bool ok_pressed = true);
    void SelectCurrentProtocol();
    void OpenCoilSelectionDialogForCDS();
    void ShowHideOpenedWindows(CDialog* f_dialog, bool& f_window_flag_to_use, const bool f_show = true)const;
    bool IsBatchSettingOn() const ;
    void ApplyCoilToProtocols(const int* const selected_items, const int size);
    void DisplaySelectedCoil(const int idx);
    bool IsValidSequenceIndex(const int index)const;
    bool IsWaitCursor()const;
    BOOL IsWindowVisible()const;

    void ApplyAutoScanToItems(const int* const selected_items, const int size, const bool auto_scan_on = true);

    BOOL EnableDisableDeleteButton()const;    //MEITEC/2010Jul22/Modified/Change type from bool to BOOL
    void ParentImageRemovedFromCurrentProtocol();
    void UpdateCouchVal();
    void SelectAndHighlightFirstWaitProtocol();
    bool UpdateFullTRSCFromGivenIndex(const int f_list_index);
    bool CanStartAutoScan();
    int GetIndexForAutoScanProt();
    void SendNotifyToMemory()const;
    void HideSR()const;
    void SwitchPage(const CString& f_page)const;
    BOOL ShowPQM();
    void SendScanMessageToSR(const CString& f_Message, bool f_sync = false);
    void NotifyReceivedMessage(const pqm_msg_tag f_msg);
    void ApplyReceiverCoilForNewProtocols(const int last_index, const unsigned long recv_coil_id,
                                          const CString& recv_coil_label, const CString& str_trsc);
    void UpdateProtocolStatus(const int selected_index, const int status);
    void ApplyPatientOrientationForNewProtocols(const int last_index,
            CPqmPrMode prMode);
    void ApplyAnatomyForNewProtocols(const int last_index, const int iAnatomy, const CString& str_trsc);
    int SendLOID(BSTR f_strpatientloid, BSTR f_strstudyloid, BSTR f_pas_path);

    void EnableDisableScanRelatedButtonsForSelectedProtocolStatus() ;
    void EnableDisableLocatorButton(); //ashish SM3
    bool CanModifyWithCommentDialog();
    void InsertNewSequencesOnGUI(const bool f_calculatecoil_data = false, bool f_select_item = false);
    void CleanUp()const;
    void DisplayTotalRemainingTimeOnPQM();
    BOOL InvokeMemory();
    void ShowSR()const;

    void OnCFAOperationClick(const bool f_cfa_retry = true);
    void DoCFAOperation(const bool f_cfa_retry = true);

    void StartCFAPrescan();
    void DisplayAPCStartLabel(const bool flag);
    void ToggleAPCPauseMenu();
    BOOL InvokeRefProbe()const;

    void OnClickButtonBreathHold(const int row_index);
    void OnDblclkListParameter(const int row_index);
    void OnClickButtonSarRegion(const int row_index);
    void OnToggleAutoVoice(const int row_index);
    void OnMessageFromAcqmanMsgHandler(const pqm_msg_tag Msg);
    bool CanPerformModeOptionOperations()const ;
    bool CanPerformEditingForVFFile()const;
    bool CreateContrastAgentPopupMenu(PopupMenuEx* contrastmenu, const int row_index)const;
    bool IsScanning()const;
    bool IsRMCRunning() const;
    void UpdateHoldValue()const;
    void PQMManualPrescanClose()const;
    void PQMManualPrescanApply(VARIANT* f_pdata)const;
    BOOL CheckASCompassOnOff(int f_scan_index);
    void OnSelectionStatusChanged();
    bool DeleteProtocolWithIndex(CSequence* l_seq, const int index)const;

    int DuplicateSequenceforIndex(CSequence* seq_to_duplicate, const int index_being_duplicated,
                                  const int new_index_for_duplicated_protocol, const bool drag_dropped = false, const int f_group_id = 0);
    int DuplicateSequenceforIndex(CSequence* seq_to_duplicate, const int index_being_duplicated, const int new_index_for_duplicated_protocol, const bool drag_dropped = false);
    bool MoveProtocolToIndex(const int protocol_index_to_move, const int target_index) ;
    void OnClickButtonDuplicate(const bool f_append = false);
    bool IsPausedScanning()const;
    void MrsScanStartUiCB(SMRSScan_s* mrsscan_values) ;
    void MrsScanAbort();
    void MrsScanClose();

    void RemoevAutoScanFromFirstWaitItem(const int f_first_wait = -1);
    bool CanEnableAqumanRelatedMenuItems()const;
    void SendMessageToProcon(const ProconMessageEnum procon_message, const bool perform_gui_action = true, const bool send_message_to_procon = true, const int acq_order = -1 , const int f_maskValue = -1);

    //+Patni-PJS/2011Feb1/Modify/IR-97
    bool NewSequencesAdded(const int f_count) const;
    bool DeleteWFDASequence(CSequence* const f_seq);
    bool ProcessScanRelatedMsgForWFDA(const int f_acq_order, const pqm_msg_tag f_msg_tag) const;
    bool IsProtocolInscan(const int f_acq_order) const;
    bool CheckPlnStatus(const int f_acq_order);
    bool OnScanEditStatusChangedForCurrentProtocol(const int f_index, const WFDA_Protocol_Status f_wfda_protocol_status) const ;
    bool CanProcessProconMessage(const int f_message_code);
    //-Patni-PJS/2011Feb1/Modify/IR-97

    //Patni-PJS/2011Feb21/Added/IR-97
    void InscanScanStart();

    void ApplyContrastAgentSettings(const int row_index, const int contrast_agent_index = 0, const bool f_bContrastAgentMenu = false);
    void OnOrienationChangedFor(const OrientationType orientation_type);
    void OnToggleMoveCouchOption(const int f_row_index);
    void OpenCoilSelectionDialog(const bool f_cds_mode = false);

    void SendUpdateCurrentExamData();

    void UpdateCurrentScanMasterSlave();
    void OnDeleteUpdateMasterSlaveOnGUI();
    void UpdateGUICtrlAutoCopyMasterSlave();
    void UpdateGroupNum();
    int* AllocateGroupIdForDuplicateProtocols(const int* f_selected_items, const int f_count);
    int OnDeleteUpdateMasterSlave(const int* f_selected_items, const int f_count);
    void ShowTimerButton();
    void DisplayTimer();
    void CalculateTimeDiff();
    BOOL GetProtectedStatus(BSTR bStr);
    CString GetCurrentDelayTime();
    void SaveDelayTime(const CString& f_delay_time);
    BOOL GetEditDelayDlgStatus();
    void UpdateSelectedSeqMasterSlave();
    void UpdateOKBtnStatusofWirelssDlg();
    void UpdateWaitForAutoScanIcon();

    void UpdateCFARetry();
    LRESULT OnSettingWaitCursorForPqmButtons(WPARAM wParam, LPARAM lParam);
    bool SetWaitCursorForPQMButtons(bool);
    bool IsSFTMenuProcessAlive(const CString& f_process_name)const;

    void UpdatePQMGUIForSAROver();
    void StartTimerForPager();
    void StopTimerForPager();
    void HandleSARControlConnectionStatus(const bool f_sar_mgr_connection_status);
    BOOL GetSARControlLicenseAtStartUp();

    //{{AFX_MSG(CPQMView)
    afx_msg void OnClickButtonDelete();
    afx_msg void OnClickButtonSetAll();
    //}}AFX_MSG

    //Patni-Sribanta/2011Feb09/Added/REDMINE-1026
    void OnClickButtonScanResume();
    void OnClickButtonScanPause();

    //Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    void OnClickVoiceMenuItem(const UINT nID) const;
    void StartScanEditOrPlan(const ProconMessageEnum procon_message, const bool f_is_auto_plan = false);

    //Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    afx_msg void OnClickButtonScanStart(const bool f_is_irs_msg = false);

    //{{AFX_VIRTUAL(CPQMView)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnInitialUpdate(); // called first time after construct

    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

    //void calcMRSEddy(const int f_protocol_index,const BOOL f_calc_b0_b1);

    // Generated message map functions
    //{{AFX_MSG(CPQMView)
    // NOTE - the ClassWizard will add and remove member functions here.
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnClickScanOffset();
    afx_msg void OnClickPasButton();
    //afx_msg void OnClickSavePasButton();
    afx_msg void OnClickUtilityButton();
    afx_msg void OnClickCoilPasButton();
    afx_msg void OnClickMenuitemInfoSeq();
    afx_msg void OnClickMenuitemInfoCoil();
    afx_msg void OnClickMenuitemInfoGate();
    afx_msg void OnClickButtonComment();

    //previously there were two seperate buttons for the
    //pause resume functionality... it has to be iomplemented with one button.
    afx_msg void OnClickButtonScanPauseResume();
    afx_msg void OnClickButtonScanPlan();

    afx_msg void OnCancelEditSrvr();
    //Patni-Sribanta/2011Jan11/Commented/TMSC-REDMINE-1026
    //afx_msg void OnClickButtonScanStart();
    afx_msg void OnClickButtonScanAbort();

    //afx_msg void OnBegindragListParameter(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickButtonSequenceEditior();
    afx_msg void OnClickMenuitemAutoPlan();
    afx_msg void OnClickMenuitemPlanScan();
    afx_msg void OnClickMenuitemSkipRgn();
    afx_msg void OnClickMenuitemAllCovRgn();
    afx_msg void OnClickMenuitemSpeedermapAutovoice();
    //afx_msg void OnClickMenuitemMultiCovCaution();
    afx_msg void OnClickMenuitemBatchCoilAndSarRgn();
    afx_msg void OnClickMenuitemGateModeEcg();
    afx_msg void OnClickMenuitemGateModePeripheral();
    afx_msg void OnClickMenuitemGateModeRespiratory();

    afx_msg void OnClickMenuitemWirelessGating();

    afx_msg void OnClickMenuitemElectrodeI();
    afx_msg void OnClickMenuitemElectrodeIi();
    afx_msg void OnClickMenuitemElectrodeIii();
    afx_msg void OnClickMenuitemElectrodeAvf();
    afx_msg void OnClickMenuitemElectrodeAvl();
    afx_msg void OnClickMenuitemElectrodeAvr();

    afx_msg void OnClickMenuitemPrepWbCoil();
    afx_msg void OnClickMenuitemShimmingWbCoil();
    afx_msg void OnClickMenuitemCenterFreqCtrl();
    afx_msg void OnClickMenuitemApcRetry();
    afx_msg void OnClickMenuitemApcPause();

    afx_msg void OnCmdOpen(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCmdClose(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCmdShow(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCmdHide(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCmdScanStart(WPARAM wParam, LPARAM lParam);
    afx_msg void OnDestroy();
    afx_msg void OnClickButtonPqmclose();

    afx_msg void OnClickButtonEndExam();

    afx_msg void OnButtonInject();

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    //void OnClickVoiceMenuItem(UINT nID);
    afx_msg void PQMMessageMapVoiceMenuItem(UINT nID);
    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    afx_msg void OnClickLanguageMenuItem(UINT nID);

    afx_msg void OnCmdFileNotExist(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClickButtonAutoScan();

    afx_msg void OnClickMenuItemReferROI();

    afx_msg void OnClickMenuBreadthHold();
    afx_msg void OnClickMenuAutoTransfer();

    afx_msg void OnClickMenuCenterOfPlan();
    afx_msg void OnClickMenuPositionOfReferenceImage();

    afx_msg void OnClickScanAnatomy();
    afx_msg void OnClickAnatomyButton();
    afx_msg void OnClickButtonPlanDuplicate();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnClickPlanDuplicateMenu(UINT f_cmd_id);


    afx_msg LRESULT OnCFAShow(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnShowCFADialog(WPARAM wParam, LPARAM lParam);  //Patni-MN/2009Mar13/Added/PSP1#203
    afx_msg LRESULT OnAbortFail(WPARAM wParam, LPARAM lParam);  //Patni-SS/2009Mar31/Added/PSP1#XX
    afx_msg LRESULT OnAbortWait(WPARAM wParam, LPARAM lParam);  //Patni-SS/2009Mar31/Added/PSP1#XX
    afx_msg LRESULT OnReConnectToProcon(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDataBaseNotInSync(WPARAM wParam, LPARAM lParam);
    afx_msg void OnOnQueueAndNext();
    afx_msg void OnQueueAndExit();
    afx_msg void OnNextAndCopy();
    afx_msg LRESULT OnHandleIRSMessages(WPARAM wParam, LPARAM lParam);
    afx_msg void OnHandleIRSMessageOnScanThread(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSettingWaitCursor(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCFAReady(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClickPlanDupConfig();
    afx_msg void OnClickDebugInfo();
    afx_msg void OnASCOMPASSSettings();
    afx_msg void OnOffAsCompass();
    afx_msg void OnLaterality(UINT f_scananatomy_menuitem_id);
    afx_msg void OnClickMenuCardiacLocator();
    afx_msg void OnClickMenuNeuroLocator();
    afx_msg LRESULT OnRefreshGUI(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnInvokeCoilInfo(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnInvokeGateDialog(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCmdGLSRShow(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnClickButtonEditDelay();
    afx_msg void OnClickButtonPlanReset();
    afx_msg void OnClickMasterOnPlanMark(UINT f_master_menu_id);
    afx_msg void OnAMB();
	afx_msg void OnTiPrep();
    afx_msg void OnClickMenuitemSARManager();
    afx_msg LRESULT ReConnectSARManager(WPARAM wParam, LPARAM lParam);

    //}}AFX_MSG

    //+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    afx_msg void OnUpdateDebugWindow(WPARAM wParam, LPARAM lParam);

    bool IsLocatorEnabled();
    void OnClickScanStart();
    void PQMMessageScanStart();

    bool ShouldDisableCoilGateInfo() const;

    void SetWaitCursorForModelessDlg(const bool f_display_waitcursor);
    void GetMinMaxValueForScanOffset(CSequence* f_seq, const int f_acq_order, Vector_t& f_current_offset, Vector_t& f_min, Vector_t& f_max);
    void UpdateScanOffsetFor(CSequence* f_seq, const Vector_t& f_vect_scanoffset, const BOOL f_set_mode = FALSE);

    int CheckAndGetWaitingForAutoScanStartAcqOrder();
    void CreatePlanMasterSlavePopupMenu(PopupMenuEx* f_plan_master_slave_menu, const int f_protocol_index);
    bool CheckForScanIndexAndMarkForDelete(const int* f_selected_items, const int f_count);

    result_r GetGUIListProtocolInfo(const int f_start_index = -1,
                                    const int f_end_index = -1);
    bool InsertAutoInsertProtocol(const pqm_msg_tag& f_msg);

    void CloseWSE();
    void GetSequenceInformation(const int f_seq_index);
    void GetIndexOfFirstWaitStateProtocol(int& idx_wait_prot);
    int  GetSelectedSeqIndex();
    void GetShowCoilList()const;
    void GetSelectCoilList()const;
    BOOL SetScanProtocolInfoFromGUI();
    void SetDefaultSettings();
    bool SetWaitCursor(const bool wait_flag, const bool f_update_status = true);
    void SetSendSeqListFor(int* f_selected_items, int count)const;
    bool SetAnatomy(const int* const seq_items, const int seq_count, const int iAnatomy);
    void SetAnatomy(const int sel_seq_idx , const int iAnatomy);

    CString GetSRParamInfo(const bool f_isStudyAppend = false);

    void GetCoilStringForCurrentProtocol();
    void GetSequenceParamList()const;
    CString GetCoilStringFor(const int f_acq_order) const ;
    void GetSequenceList(const int f_firstseq)const;
    void GetSequenceList()const;
    CString GetCoilStringForTransmiter(const int f_acq_order)const;
    CString	GetCoilStringForReceiver(const int f_acq_order)const;

    void SetBtnSensitive(const int f_id, const bool f_state);

    void SetFlagRelatedCtrlsStates();
    void UpdateEndExamButtonStatus();

    bool GetCurrentScanCommentAndScanID(CString* f_scan_id = NULL, CString* f_scan_comment = NULL);
    void SetCurrentScanCommentAndScanID(CString* f_scan_id = NULL, CString* f_scan_comment = NULL, const int seq_index = -1) ;

    CString GetSequenceInfo()const;
    void SetSequenceInfo(const CString& f_seq_info)const;

    CString GetSRParam()const;
    BOOL SetSRParam(const CString& f_param, const CString& f_PASFilename, bool f_update_pps = false);

    //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    bool IsMRSProcessToolStarted() const;
    void SetMRSProcessToolStarted(const bool f_mrs_process_tool_started);
    //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480

    int GetMemInvocationProcessSt()const;
    void SetMemInvocationProcessSt(const int f_mem_invocation_status);

    BOOL GetMismatchErrorFlag()const;
    void SetMismatchErrorFlag(const BOOL f_mismatch_error_flag);

    BOOL GetMemorymode()const;
    void SetMemoryMode(const bool f_mode);

    bool GetPQMCloseFlag()const;
    void SetPQMCloseFlag(const bool f_pqm_closed_flag);

    bool GetMemoryAliveFlag()const ;
    void SetMemoryAliveFlag(const bool f_memory_alive);

    bool IsNextProtocolAutoScanAfterStatusDone() const;
    bool IsFirstWaitProtocolEditedbyWSE() const;
    bool CanEnableCFARetry()const;

    int GetScanIndex()const;
    void SetScanIndex(const int f_scanindex);

    int GetShimDoneFlag()const;
    void SetShimDoneFlag(const int f_shim_done_flag);

    int GetShimStateFlag()const;
    void SetShimStateFlag(const int f_shim_state_flag);

    BOOL GetPostProcessStatus() const;
    CPqmChildThread& GetPqmChildThread();
    SrParamInfo* GetSRParamInfoObj();
    CSequence* GetCurrentSequence()const;
    bool GetIsLocatorOpen()const;
    bool GetWaitCursor()const;
    CString Getstudyloid()const;
    float GetMagneticCenterPos()const;
    CPQMCDSController* GetPQMCDSControllerInterface() const;
    bool GetHoveredProtocolIdent(CString* f_scan_ident = NULL, const int f_scan_index = 0)const;
    int GetAcqOderForGivenProtocolIndex(const int f_protocol_list_index)const;
    int GetDBDtMode(const int f_acq_order)const;
    CSequence* GetSelectedSequence(const int index)const;
    CStringArray& GetInfoLableArray();
    long GetMaxProtocolLimit();
    CStringArray& GetTagArray();
    CPqmPrMode& GetPrModeOfSelectedSequence(const int index)const;
    CPQMListCtrl&   GetPQMListCtrl();
    CStringArray& GetHeaderArray();
    CPQMGlobal& GetPQMGlobal();
    CIPCComManager* const GetPQMSequence();
    CSeqList* const GetSeqList();
    IPqmIpcCom* GetAcquisition()const; //TMSC-Tanoue/2010Sep03/MVC008324
    BOOL const GetMsgSeqLimit();
    void GetManualPreScanDefaultData()const;
    BOOL GetManualPreScanDefaultData(VARIANT** f_pdata)const;
    CPQMCFADlg* GetCFADlgPtr()const;
    int  GetXOffsetLimit()const;
    int  GetYOffsetLimit()const;
    CPQMDebugWinDlg& GetPqmDebugWinDlg();
    int* GetSeletedProtocols();
    int GetTotalProtocols();

    void SetAutoScanStatus(const bool bFlag);
    void SetRMCScanRunning(const bool f_state);
    void SetChildThreadStatusToIPC(int* f_pData, const int f_count, const int f_child_thread_status)const;
    void SetDBDtCancelFlag(const bool f_dbdt_flag)const;
    void SetDynamicScan(const bool f_dynamic_scan);
    void SetCanAPCPause(const APC_Scan_Enum f_apc_pause_enum);
    void SetPqmPmDelay(const int f_delay1, const int f_delay2)const;
    void SetCFAWindowOpenedBeforeSwPage(const bool f_flag);
    void SetSendSeqList();
    void SetAbortFlag(const bool abortflag);
    void SetIsScanning(const bool f_scanning);
    void SetPrescanHook(const BOOL f_prescan_hook)const;
    void SetManualPreScanApplyBtnState(const BOOL f_button_state)const;
    void SetPASButtonState(const BOOL f_pas_button_state);

    void SetPostProcessFlag(const BOOL f_bpost_process_flag);
    void SetCheckLoadStudy(const bool f_check_load_study);

    //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    bool CanPerformGUIOperation() const;

    CCoilChannelMode* GetCoilChannelMode();
    int GetSystemChannels();
    void SetSystemChannels(const int f_system_enable_channel);
    int GetLimitedChannels();
    BOOL GetIsChannelModeApplicable();
    void SetConnectorState(const BOOL f_connector_state);
    BOOL GetMulitScanDynamic_License()const;//Redmine-2187

    void SetLastAcqmanHandlderMsg(const pqm_msg_tag f_Msg);
    pqm_msg_tag GetLastAcqmanHandlderMsg() const;

    void SetRegistFlag(const BOOL f_reg_flag);
    BOOL GetRegistFlag()const;
    CAutoLocatorAppController& GetAutoLocatorController();
	    CTiPrepAppController & GetTiPrepController();

    void PQMMessageUpdateSarInfo()const;
    void UpdateSarInfoOnPage()const;
    void UpdatePagerArea(const CString& f_message, const CString& f_parameter, const CString& f_custom)const;

    bool IsLastProtocolProcessed();

    void SetIsAMBPlanning(const bool f_is_amb_planning);
    bool GetIsAMBPlanning()const;
    bool IsProtocolSelectedForAMBPlan(const int f_check_list_index) const;
    void AMBGetSelectedItems(int** f_selected_items, int& f_size);
    BOOL CheckCFADialogStatus()const;
    void AMBParentImageRemovedfromProtocol();
    bool AMBCheckIsAnyDialogOpen()const;
    BOOL CheckCoilDialogStatus()const;
    bool GetDynamicScan()const;
    BOOL GetAPCRetry()const;
    bool IsAMBInvoked()const;
    bool IsAMB()const;
    void OnAMBClosed();
    BOOL GetSARControlLicense();
    void GetSarInfoPtrForIndex(CSequence* f_seq, int f_seq_index);
    bool GetWaitTimeSARControlFlag() const;
    void SetWaitTimeSARControlFlag(const bool f_is_waiting_for_scan_sarctrl);
    void SetIsCFAokpressed(const bool f_is_cfa_ok_pressed);
    bool GetIsCFAokpressed()const;
    void DisplayWaitTimeonAcquirePage() const;
    void ClearDPSTimer() const;


private:
    bool m_is_end_exam_clicked;
    pqm_msg_tag m_lastacqman_handler_msg;
    BOOL m_connector_state;
    CFont m_tooltip_font;
    bool m_iswait_cursor_for_pqm_buttons;

    bool m_sar_connect_status;
    BOOL m_sar_control_license;

    CAutoLocatorAppController m_autolocator_app_cntl;
	CTiPrepAppController m_tiprep_app_cntl; //ANULEKHA
    CWirelessGatingController m_wirelessgating_cntlr;

    enum WSE_Editing_Operation { WSE_Editing_Operation_SaveOnly, WSE_Editing_Operation_GUISave, WSE_Editing_Operation_GUISave_SaveOnly };
    enum TRSC_Enum { TRSC_T, TRSC_R, TRSC_S, TRSC_C };
    CurrentScanOpration m_current_scan_opration ;
    BLINK_STATUS m_blink_status;
    APC_Scan_Enum m_apc_pause_enum;
    acqman_status m_acquman_status;

    //+Patni/KSS/2011Mar31/Modified/TMSC-REDMINE-1559
    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    UINT m_pqm_gui_processing;
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_deletion_cs;
    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    //-Patni/KSS/2011Mar31/Modified/TMSC-REDMINE-1559

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_sync_scan_and_delete_cs;


    //{{AFX_DATA(CPQMView)
    enum { IDD = IDD_PQM_DIALOG };
    CPQMListCtrl	m_pqm_listctrl;

    CCustomStatic m_scan_offset_static;
    CCustomStatic m_coil_info_static;
    CCustomStatic m_scan_anatomy_static;
    CCustomStatic m_remaining_time_static;

    CCustomStatic m_sar_anatomy_static;

    ButtonEx m_end_exam_button ;
    ButtonEx m_abort_button ;
    ButtonEx m_close_button ;

    ButtonEx m_pause_resume_button ;
    ButtonEx m_scan_start_button ;

    ButtonEx m_utility_button ;
    ButtonEx m_pass_button ;

    ButtonEx m_scanedit_button ;
    ButtonEx m_injecttime_button ;
    ButtonEx m_locator_button ;
    ButtonEx m_duplicate_button ;
    ButtonEx m_delete_button ;
    ButtonEx m_scan_offset_button ;
    ButtonEx m_coil_button ;

    ButtonEx m_scan_anatomy_button;

    ButtonEx m_scan_sar_button;

    ButtonEx m_queue_and_next;
    ButtonEx m_queue_and_exit;
    ButtonEx m_next_and_copy;
    ButtonEx m_reset ;
    ButtonEx m_close;
    ButtonEx m_inject_timer_button ;

    //}}AFX_DATA

    CPqmChildThread m_pqmchildthread;
    DWORD dwAdvise;
    CComPtr<IPqmIpcCom> m_acquisition;
    HRESULT m_result;
    IConnectionPoint* m_connection_point;
    IConnectionPointContainer* m_cp_container;
    IUnknown* m_iunknown;

    CSink* m_sink;
    int m_shim_done_flag;
    CPasswordDlg* m_mem_passworddlg ; //Patni-Sudhir 2011Mar-09/Added/IR-185
    CPQMInfoSeqDlg* m_info_seq_dlg;
    bool m_bLocatorAlreadyEnable;
    bool m_bEnableLocatorOnScanEdit;
    bool m_is_locator_open;
    CString m_strCoilSelect;
    int* m_selectedprotocols;
    CString m_av_language;
    ScanCommentXMLData m_commentdata;
    CSFTMenuHandler* m_sft_menuhandler;
    int m_mem_invocation_status;
    CSequence* m_curr_seq;
    int m_xOffset;
    int m_yOffset;
    bool m_check_load_study;
    UINT m_procon_ui_status;

    bool m_is_coil_window_opened_before_swithcing_page;
    bool m_is_cfa_window_opened_before_swithcing_page;
    bool m_is_comment_window_opened_before_swithcing_page;
    bool m_is_mem_passworddlg_window_opened_before_swithcing_page; //Patni-Sudhir 2011Mar-09/Added/IR-185
    bool m_is_seqinfo_window_opened_before_swithcing_page;
    bool m_is_gateinfo_window_opened_before_swithcing_page;
    bool m_is_sar_window_opened_before_swithcing_page;
    bool m_is_scanoffset_window_opened_before_swithcing_page;
    bool m_is_manualprescan_window_opened_before_swithcing_page;
    bool m_is_mrs_window_opened_before_swithcing_page;
    bool m_is_coilinfo_opened_before_swithcing_page;
    bool m_is_delay_time_window_opened_before_swithcing_page;
    CPQMCDSController* m_cds_controller;
    CPQMCoilInfoDlg* m_obj_coil_dlg;
    bool m_is_palnduplicate_window_opened_before_swithcing_page ;
    bool m_is_pqmdebugwin_window_opened_before_swithcing_page ;
    bool m_dynamic_scan_on ;
    bool m_acquman_ready_for_next_request;

    CRect m_window_rect ;
    CRect m_client_rect ;
    CBrush m_bkbrush;
    CQueueDialog* m_queuedialog;
    CQueueDialogHandler* m_queuedialoghandler ;
    CPqmdBdtCautionDialog* m_dbdtcautiondialog;
    CPatientOrientationDialog* m_patientorientationdialog ;
    CPatientOrientationHandler* m_patientorientationhandler;

    //for manual precan - Jitendra
    CPQMManualPreScanDlg* m_pqmmanualprescandlg ;
    InterfaceForPQMManualPreScanDlg* m_interface_for_pqmmanualprescandlg;

    //for MRS prescan
    CMRSPreScanDlg* m_mrsprescandlg;
    CMRSPreScanDlgHandler* m_mrsprescandlg_handler;
    bool m_is_procon_connected;
    //For COIL
    CCoilSelectionUtility* m_selection_utiliy;
    DllT* m_head;

    // CoilSelection heads for G, L and A Port
    // NEED_MODI pls use contant for 2 and 7.
    DllT* m_coil_trestle_head;	// For G Port
    DllT* m_coil_liner_head[2];	// For L Port
    DllT* m_coil_couch_head[7];	// For A Port

    CCoilSelWin* dlgCoilSelWin;
    CAxisLocatorDialogInterface* m_axislocatordialog_interface ;
    CContrastAgentData* m_contrast_agent;
    CScanAnatomyData* m_scan_anatomy[SAR_REGIONS - 2];
    CArray<int, int&> m_selected_acqoder_list_for_wse;
    CPresetItemData m_preset_item;

    int m_axis_locator_position;
    bool m_sr_invoked;
    BOOL m_store_mode;
    CPQMCFADlg* m_cfadlg;
    bool m_pqm_closed_flag;
    bool m_iswait_cursor;
    bool m_is_scanning;
    bool m_scan_lock_acquired;
    HCURSOR m_wait_cursor, m_org_cursor ;
    bool m_ismemoryalive;
    int m_memory_invokation_flag;
    bool m_is_wse_open;
    CIPCComManager* m_pqm_sequences;
    BOOL m_msg_seq_limit;
    int m_scan_index;
    long m_pqm_max_protocol;
    CSeqList* m_list;
    CStringArray m_str_arr;
    CStringArray m_tag_arr;
    CStringArray m_info_long_lable;
    bool m_rmc_scan_running;
    bool b_connection_established;
    CPQMGlobal m_pqm_g;
    CPQMCommentDlg* m_comment_dlg;

    CPQMAMBController m_amb_controller;

    CInterfaceClassForScanCommentDlg* m_interfaceclassforscancommentdlg;
    bool m_flag_abort;
    bool m_current_flag;
    bool m_apcflag;
    int m_shim_state_flag;
    CString m_sr_param;
    CPQMGateInfoDlg* m_obj_gate_dlg;
    CPQMAnatomyDlg* m_anatomy_dlg;
    CPQMSetScanOffsetDlg* m_setscanoffsetdlg;
    CString m_studyloid;
    bool m_is_mrs_prot;
    int				m_wait_cursor_counter;
    BOOL m_mismatch_error_flag;
    int m_GUIselAcqOrder;
    BOOL m_bpost_process_flag;
    SrParamInfo m_srInfo;

    CPQMDebugWinDlg m_pqmdebugdlg ;
    CPQMPlanDupDlg m_pqmplandup_dlg ;

    CDelayTimeDlg* m_delay_time_dlg;

    //Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    bool m_mrs_process_tool;

    //+Patni-Sudhir 2011Mar-09/Added/IR-185
    HANDLE m_hMapFile;

    //Patni-PJS/2011Feb1/Modify/IR-97
    CWFDAImplementer* m_wfdaimplementer;
    BOOL m_pas_reg_flag;
    int m_inject_timer_time;
    DWORD m_dw_start_count;


    CCoilChannelMode* m_coil_channel_mode;
    BOOL m_use_coil_channel_mode;
    int m_system_enable_channel;
    int m_limited_channel;
    MasterListItemData m_master_list_for_plan_mark;
    bool m_inscan_refclose_msg_sent;
    bool m_is_amb_planning;
    bool m_is_cfa_ok_pressed;


    int m_timer_pager_area;
    int m_timer_count_for_pager;
    DWORD m_dw_start_count_for_pager;
    int	 m_max_time_for_pager;
    bool m_is_waiting_for_scan_sarctrl;

    bool CanInvokeMemory();
    //Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
    bool UpdateMemoryInvocationMode(TCHAR szMemMode[]);
    //-Patni-Sudhir 2011Mar-09/Added/IR-185

    void InitUiButtonUtility();
    void InitToolTip();
    void UpdateScanOffsetInRange(const int f_index);

    APC_Scan_Enum CanAPCPause()const ;
    afx_msg LRESULT OpenCoilConfirmationDialog(WPARAM wParam, LPARAM lParam) ;
    afx_msg LRESULT OpenLocationDebugDialog(WPARAM wParam, LPARAM lParam) ;
    afx_msg LRESULT UpdateCompassCoilToCurrentProtocol(WPARAM wParam, LPARAM lParam) ;

    //+Patni-Sribanta/2011Feb07/Added/REDMINE-1026
    afx_msg LRESULT GetConnectCoilPostMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnClickCoilPasButtonPostMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT ProcessMRSMessagesPostMessage(WPARAM wParam, LPARAM lParam);
    //-Patni-Sribanta/2011Feb07/Added/REDMINE-1026
    afx_msg LRESULT InvokeProbeDialogPostMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT InvokeAMB(WPARAM wParam, LPARAM lParam);


    void OnCancel();
    void OnReset() const;
    void PQMMessageMapAppend()const;
    BOOL HidePQM()const;
    void OnClickButtonAutoTransfer(const int row_index);
    void OnClickContrastAgentMenu(const UINT f_contrast_menuitem_id);
    void OnClickScanAnatomyMenu(const UINT f_scananatomy_menuitem_id);
    bool CanPlanAndDuplicateforIndex(const int f_seq_index)const;
    void OnClickMenuitemManualPrescan(WPARAM wParam, LPARAM lParam);
    UINT CheckpositionofReferenceimage()const;

    //{{AFX_VIRTUAL(CPQMView)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL PreTranslateMessage(MSG* pMsg);	//from PQM - by Hemant
    //}}AFX_VIRTUAL

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    void ValidateScanOffsetWithinRange(float& f_scan_offset_x,
                                       float& f_scan_offset_y, const float f_scan_offset_z);
    bool UpdateCoilSystemSetting()const;
    void UpdateSarAnatomyButton();
    bool ReturnCoilOfPreviousSeq()const;
    BOOL HideBigBar()const;

    bool CreateVoiceMenu(PopupMenuEx* voice_menu, PopupMenuEx* test_menu)const;
    bool CreateTestMenu(PopupMenuEx* test_menu)const ;
    bool CreatePlanDuplicatePopupMenu(PopupMenuEx* f_plan_duplicate_menu, const int f_protocol_index);

    void DeleteWDFAMsgReceiver();

#ifdef _DEBUG

#endif


    bool ReadConfigInfo(SIPCConfigInfo* l_sIPCConfigInfo);

    virtual BOOL OnCmdMsg(
        const UINT nID,
        const int nCode,
        void* pExtra,
        AFX_CMDHANDLERINFO* pHandlerInfo
    );

    void CheckPqmPqIsGlocOk()const;

    // for Acqman current status request
    void SendStatusReqToAcqman()const;
    //FOR COIL...

    bool CreateCoilListsForSelectionCoil();
    void CSCoilDisplay(
        DllT* f_head,
        const int f_no
    )const;

    bool GetAMBPlanScanStartCheck()const;
    BOOL CheckAMBMenuClickConditions();
    void InitializeAvLanguage(const CString& f_av_lang, const SIPCConfigInfo* f_sIPCConfigInfo);
    int CanChangeScanAnatomy();
    BOOL InitializePQMListCtrl();
    void ApplyWarningFont(ButtonEx* f_button, const COLORREF f_warning_text_color = g_no_data_warning_color)const;
    bool CanChangeSar();
    void EnableDisableScanAnatomy();
    void EnableDisableScanEditOrPlanButtons();
    void EnableDisableQueueDialogButtons();

    //The following functions are added for the PQM new GUI...
    void TogglePauseResumeButtonAppearence(const bool set_pause_state = true);
    void EnableDisableScanRelatedButtons(const bool enable = true, const bool apply_forcefully = false);
    void UpdateStatesForCtrls(const pqm_msg_tag f_msg);
    void ChangeContentForModelessInfoDialogs() ;
    bool CanCheckUnCheckFor(const int* const seq_items, const int seq_count) const;
    void OpenQueueDialog(const bool f_show = true);
    void OpenPatientOrientationDialog(const bool f_show = true);
    void UpdateOrientationCombosOnSelectionChanged(const int selected_item, const bool multiple_selection = false)const;
    void OpenMRSPreScanDialog();
    void ProcessMRSMessages(const pqm_msg_tag f_msg);
    void ProcessProConMessages(const pqm_msg_tag f_msg);
    bool OnDialogOpenedForParameterEditing(const int item_index);
    void OnScanEditOrPlan(const ProconMessageEnum procon_message, const bool f_is_auto_plan = false);
    void OnWSEOpenClosed(const bool wse_open = true, const ProconMessageEnum procon_message = PQM_PROCON_SCAN_EDIT);
    bool IsOneScanDone()const;
    void ShowHideSubDialogs(const bool f_show);
    void DestroySubDilaogsOnPQMHide();
    bool CreateScanAnatomyPopupMenu(PopupMenuEx* f_scan_anatomy_menu,
                                    const int f_sar_index,
                                    const SScanAantomy& f_scan_anatomy,
                                    CArray<PopupMenuEx*, PopupMenuEx*>* f_sub_menu_list
                                   )const ;
    void ShowHideOpenedWindows(const bool f_show = true);

#if MPLUS_V3
    CString GetStatusMessage(MPlus::PM::MPLUS_PM_OPERATION_RESPONSE_e);
    bool GetProcessNameFromProcessID(DWORD dwProcessID_i, CString& csProcessName_o)const;
    BOOL RegisterUnregisterProcMgr(const BOOL f_reg_proc_mgr);
#endif
    bool ApplyDefaultScanAnatomyFor(int f_seq_index, const bool f_gui_only = false);
    bool ApplyScanAnatomy(const int* const seq_items, const int seq_count, const int f_scananatomy_menu_index, const LateralityEnum f_laterality,
                          const bool f_check_for_same = true, SScanAantomy* f_scananatomy = NULL) ;
    void WSEEditingCompleted(const CPQMView::WSE_Editing_Operation f_wse_editing_operation);
    void UpdateCompleteSequenceFromVF(const int f_list_index);
    bool CanSaveProtocol(const WSE_Editing_Operation f_wse_editing_operation)const;
    bool CanGUISaveProtocol(const WSE_Editing_Operation f_wse_editing_operation)const;
    void ProcessAllowOrInhibitSelectionForProconButtons(const bool f_process_allow_selection, const bool process_hour_glass = true);
    void DisplayHourglassOnProconRelatedButtons(const bool hour_glass_show);

    void OnAutoScanStart();
    void PQMMessageMapDelete()const;
    void PQMMessageMapEndExam();
    void OnClickAutoVoiceMenu(const UINT auto_voice_menuitem_id);
    //Patni-Sribanta/2011Feb09/Commented/REDMINE-1026
    //void OnClickButtonScanResume();

    void AbortScan();
    void ScanStart();
    void EnableDisableStartupButtons(bool l_enable);
    bool InitializeIPC(const int f_mode);
    void SendNotifyToSR()const;
    bool InitialisePqmCOM(const int f_mode, VARIANT* f_config_data);
    void ChangeOrder(const CString& seqorder, const int dropindex)const;
    void OnContrastAgent(const int contrast_agent_index);
    void ApplyHoldValueAndAutoTranfferTo(const int* const seq_items, const int seq_count, const int holdvalue, const MOVECOUCH_METHOD f_move_couch_method, const int auto_transfer_value) ;
    void ApplyAutoVoice(const int f_focus_item, const UINT f_menu_id);
    void ApplyAutoVoiceToSelectedItems(const int* const selected_items, const int count, const UINT auto_voice_menuitem_id);
    void ApplyContrastSettingsToItems(const int* const seq_items, const int seq_count, const int contrast_agent_index, const bool f_bContrastAgentMenu);
    void DecodeAutoVoiceStringToNumber(const CString& str_auto_voice,
                                       int* auto_voice_exe_code)const;
    void EncodeAutoVoiceNumberToString(const int auto_voice_exe_code,
                                       CString& str_auto_voice)const;
    void DisplayScanOffsetOnPQM(const int idx);
    void OnProtocolCommentChanged();
    void ChangeCoilDialogCaptionAppropriately();
    void StartRFLPrescan();
    void MakeAutoScanOffForIfNecessary(const int item_index);
    void ClearSelectedAcqoderListForWSE();
    bool IsAnyProtocolPendingForWSEEditing()const;
    bool UpdateTRSCFromGivenIndex(const int f_list_index, const CPQMView::TRSC_Enum f_trsc_enum_to_update);
    bool AreDataSameForGivenTwoIndex(const int f_list_prev, const int f_list_current, const CPQMView::TRSC_Enum f_trsc_enum_to_update)const;

    void PQMMessageMapDuplicate()const;
    void PQMMessageMapSetAll()const;
    void PerformRightClickModeOption(const int f_option);
    void RemoveWndQueueMessages()const;

    //Patni-Sribanta/2011Apr29/Added/REDMINE-1678
    void UpdateScanOffsetForDisplay(const int f_prot_index) const;

    void ApplyConfigSettingForProt(CSequence* f_seq)const;
    void ValdiateBodyPartData(CSequence* f_seq, const int f_seq_index);
    void UpdateCoilDataInSeq(CSequence* f_seq)const ;
    void ValidateContrastAgent(CSequence* f_seq)const;

    void OnSFTMenuCommand(const UINT f_cmd_id);


    static DWORD WINAPI TimerProc(LPVOID lpParam);
    void UpdateMasterSlaveCtrls(int* f_seq_indexes, const int f_seq_count);
    void UpdateGUICtrlMasterSlave();
    void ExecuteAutoScanStart();
    void StopInjectTimerThread();



    //Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
    void UpdateActiveDlgStatusToIPC();

    void UpdateGUIOnScanStart();

    void CreateWFDAMsgReceiver();

    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    void AddNewGUIProcessingBit(const UINT f_bit);
    void RemoveGUIProcessingBit(const UINT f_bit);
    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

    bool ShouldDisableAPCFunc(const int f_first_sel_item) const;
    bool ShouldDisableGATEFunc() const;
    bool ShouldDisableLeadType() const;

    bool OnMoveCouchMethodMenuClick(const MOVECOUCH_METHOD f_move_couch_method);
    bool UpdateMoveCouchMethod(const MOVECOUCH_METHOD f_move_couch_method, const int f_focus_item);
    void UpdateMoveCouchMethodFor(int* f_selected_items, const int l_size, const MOVECOUCH_METHOD f_new_move_couch_method);
    MOVECOUCH_METHOD GetNewMethodForMoveCouch(const MOVECOUCH_METHOD f_move_couch_method_old, const MOVECOUCH_METHOD f_move_couch_method_new) const;

    BOOL CanProcessViewMsg(const MSG* const pMsg) const;
    BOOL CanProcessButtonClick(const UINT f_id) const;

    BOOL CheckCoilForcibleMaxMode(int f_selected_index);

    void CloseAllModalDialogs() const;
    void ResetAutoScanTimer();

    int GetPatientOrientationOptionFor(
        const OrientationType f_orientation_type,
        const int orientation_value)const ;
    bool GetBodyPartData(
        const int f_anatomy,
        const CString& f_anatomyString,
        SScanAantomy& f_sscanaantomy_to_return
    )const;
    CString GetSarString(const int sar_anatomy)const;
    AUTOVOICE_OPTIONS GetAutoVoiceOptionForID(const UINT menu_item_id, AUTOVOICE_PRESCAN_OPTION& f_prescan_aut_voi_optn) ;
    UINT GetMenuIdFromAutoVoiceOption(const AUTOVOICE_OPTIONS f_AutoVoiceOption);
    bool GetScanAnatomyDataFor(const int sar_index, const int f_scan_item_listindex, SScanAantomy& scan_anatomy_data)const;
    int GetNextWSEEditProt()const;
    void GetConnectCoil()const;
    void GetMinMaxZValue(float& f_min_z, float& f_max_z);
    void GetStudyfileName(CString&);
    void GetContrastAgentName(SIPCConfigInfo* l_sIPCConfigInfo)const;
    void GetScanAnatomyData();
    bool GetContrastAgentFromList(ContrastAgent& contrastagent, const int index)const;
    int GetListForGivenProtocolAcqOder(const int f_protocol_AcqOder_index)const;
    void GetScanAnatomyAndLateralityIndex(const int l_laterality_menu_index,
                                          int& l_scan_anatomy,
                                          LateralityEnum& l_laterality)const;
    void GetScanCommentXMLData();

    BOOL SetButtonAppearance();
    BOOL SetButtonImages();
    BOOL SetButtonFont();
    BOOL SetButtonCurve();
    void SetStatesForUtilityMenuItems(PopupMenuEx* utility_menu);
    BOOL SetBitmapForButton(ButtonEx& button_object, const UINT bitmap_resource_id, const COLORREF f_mask_color = RGB(0, 255, 0) , const bool center_bitmap = true)const;
    BOOL SetIconForButton(ButtonEx& f_button_object, const UINT bitmap_resource_id, const bool f_center_bitmap = true);
    void SetAutoVoiceCodeToItems(const int* const seq_items, const int seq_count, const int auto_voice_exe_code, const AUTOVOICE_OPTIONS selected_auto_voice_option, const AUTOVOICE_PRESCAN_OPTION l_prescan_aut_voi_optn) ;
    void SetPatientOrientationFor(const OrientationType orientation_type, const int orientation_value);
    void SetMultilingual();
    bool SetMoveCouchForSeq(CSequence* f_protseq, const MOVECOUCH_METHOD f_movecouchmethod, const int f_hold_val)const;
    void SetAvLanguage(const CString& f_av_lang)const;
    void SetState(CWnd* wnd, const PQMControlState control_state)const;
    void SetStateForCtrlForCFADialog(const pqm_msg_tag f_message_to_process)const;
    void SetScanAnatomy(const int f_scananatomy_menu_index, const LateralityEnum f_laterality = Laterality_Invalid);
    void SetInitialButtonState();
    void SetColorButton(ButtonEx* pbtn, const COLORREF cColor)const;
    void SetColorForResetButton();
    void SetBitmapForAntomyButton(const int f_nBitmapID);

    bool GetAPCPauseChecked()const;
    void SetAPCPauseChecked(const bool apcflag);

    CPQMDoc* GetDocument();
    CString GetCoilStringFor(CSequence* f_seq) const ;
    CString GetSequqnceID()const;
    bool GetDefaultMRSPrescanValue(SMRSScan_s& mrsscan_values)const ;
    bool GetMrsBrstLicense();
    void GetPresetItemData();
    bool GetCommentDlgStatus()const;
    BOOL GetSeqInfoDlgStatus()const;
    BOOL GetCoilDlgStatus()const;
    CString GetCoildDialogStringFor(CSequence* f_seq);
    bool GetAbortFlag()const;

    void SetGUISelAcqOrder(CSequence* f_seq);
    void SetPrModeOfSelectedSequence(const int index, CPqmPrMode pr_mod);
    void SetAcqumanReadyForNextRequest(const bool f_ready);
    void SetStateForCtrl(CWnd* wnd, const pqm_msg_tag f_msg);
    void SetDefaultChannelMode(CSequence* f_pseq, int f_seq_index);
    BOOL GetConnectorState()const;
    const CString GetToolTipText(const UINT nID, CDC* f_dc, CRect& f_tooltip_text_rect) const;
    const CString GetStringForScaneOffset(CDC* f_dc, CRect& f_tooltip_text_rect) const;
    CString GetPQMFlagsString()const ;
    bool GetSARControlStatusForScanEditRelatedButtons() const;
    bool GetSARControlStatusForScanRelatedButtons() const;


    DECLARE_DYNCREATE(CPQMView)
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PQMView.cpp
inline CPQMDoc* CPQMView::GetDocument()
{
    return (CPQMDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_VIEW_H__
