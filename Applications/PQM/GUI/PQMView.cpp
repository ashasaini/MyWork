//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-779
//TMSC-REDMINE-774

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMView.cpp
 *  Overview: Implementation of CPQMView class.
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
#include "stdafx.h"
#include "PQMView.h"
#include <PDataParser/INIReader.h>
#include <tami/vfStudy/vf_appcodes.h>

//Patni-PJS/2010Nov15/Added/V2.0/IR-175
#include <libStudyManager/constants.h>
//Patni/KSS/2011Mar31/Added/TMSC-REDMINE-773 + TMSC-REDMINE-1486-Part1
#include <libStudyManager/Mutex/ScopedLock.h>
//Patni/KSS/2011Apr06/Added/TMSC-REDMINE-779_part-1
#include <libStudyManager/destroy.h>
#include <libStudyManager/WinAPIAdapter.h>
#include <libStudyManager/CFW/CFWReceiveMsg.h>
#include <libStudyManager/CFW/CFWReplyMsg.h>
#include <PQM/AMBCommonStructures.h>

#include "PQM.h"
#include "PQMCommentDlg.h"
#include "PQMCFADlg.h"
#include "PQMSetScanOffsetDlg.h"

#include "PQMAnatomyDlg.h"
#include "Sink.h"

#include "PQMCoilInfoDlg.h"
#include "PQMGateInfoDlg.h"
#include "MRSPreScanDlg.h"

#include "PQMManualPreScanDlg.h"
#include "InterfaceForPQMManualPreScanDlg.h"
#include "QueueDialog.h"

#include "PatientOrientationDialog.h"
#include "PqmdBdtCautionDialog.h"
#include "PQMInfoSeqDlg.h"

//for coil- Himanshu
#include "CoilSelWin.h"
#include "CoilSelectionUtility.h"

#include "PQMUtility.h"
#include "DPSManager.h"
#include "PQMDebugWinDlg.h"
#include "PQMWirelessGating.h"
#include "CDS/COMPASS_Location_Debug.h"
#include "CDS/COMPASS_SettingDialog.h"

#include "PQMCDSController.h" //Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

#include "DEGLCommunicator.h" //Patni-Sribanta/2010Jun10/Added/MVC7933

#include "SFTMenuHandler.h" //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

#include "PasswordDlg.h" //+Patni-Sudhir 2011Mar-09/Added/IR-185

//Patni-PJS/2011Feb1/Modify/IR-97
#include "WFDAMsgReceiver.h"
#include "UIButtonUtility.h"

#include "DelayTimeDlg.h"
#include "CoilChannelMode.h"
#include "PQMImageUtility.h"


//Patni-PJS/2010Nov15/Added/V2.0/IR-175

using namespace MR::ACQ::SM::COMMON;

//Patni/KSS/2011Mar31/Added/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UTIL::GetCurProcessIdString;
using MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg;


const int START_VOICEMENU_ITEM       =  3000;
const int START_VOICEMENU_ITEM_MAX   =  3099;
const int START_LANGUAGEMENU_ITEM    =  4000;
const int START_LANGUAGEMENU_ITEM_MAX = 4099;

//Patni-Hemant/2010May25/Modified/MaFT/MVC007915
const int g_laterality_menuitems = 3;


//+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
//#define SAR_DLG_TOP     0
//#define SAR_DLG_BOTTOM  646
//#define SAR_DLG_LEFT    631
//#define SAR_DLG_RIGHT   939
//-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
extern Data_decl_list  g_sequence_param_name_list;

CString sa = _T("SA");

//+Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement
CString rc = /*_T("RC")*/RECVR_COIL;
//-Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement

CString trsc = _T("TRSC");
CString contrast = _T("CONTRASTNMAE");

extern CPQMApp theApp;

CString status[] = {L"WAIT", L"CURRENT", L"FAILED", L"DONE"};
/////////////////////////////////////////////////////////////////////////////
// CPQMView


//Modified by Hemant On 12/24/2008 1:43:13 PM , FOR CR from TMSC
const CPoint g_child_window_offset(258, 581);


//+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
//const int __recv_coil_id = 1094909952 ; //COILKEY;
//-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement

//Added by Hemant On 2/11/2009 3:04:19 PM
#define WSE_OPEN_CLOSE					0x0001		//binary 0001
#define WSE_OPENED_FOR_LOCATOR			0x0002		//binary 0010
#define WSE_OPENED_FOR_SCAN_EDIT		0x0004		//binary 0100

//Added by Hemant On 2/11/2009 3:04:19 PM
//#define MEMORY_INVOKATION_STARTS		0x0001		//binary 0001
//#define MEMORY_INVOKATION_ENDS			0x0002		//binary 0010
//#define MEMORY_PRESENT					0x0004		//binary 0100

//+Patni-HEMANT/ADDED On 3/6/2009 9:16:36 PM/ PSP#232
#define PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS			0x0001
#define PQM_PROCONALLOW_SELECTIONS_PROCON_BUTTONS		0x0002
#define PQM_CAN_PROCESS_PROCON_BUTTONS					0x0004	//+Patni-HEMANT/ADDED On 27/04/2009 6:52:10 PM/ PSP#269
//-Patni-HEMANT/ ADDED On3/6/2009 9:17:49 PM/


//+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
#define PQM_PROCESSING_NONE		            0x000
#define PQM_PROCESSING_QUEUE_NEXT           0x001
//-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1


//Patni-ARD/2009Jul25/Modified/IR
const CString MULTI_OID_DELIMITER = '\\';



//Patni-Hemant/2009Nov12/Added/IR#90/Bottom Buttons
const COLORREF g_default_mask = RGB(0, 255, 0);


/////////////////////////////////////////////////////////////////////////////
// CPQMView construction/destruction
CPQMView* g_MainView = NULL;	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections


//+Patni-Hemant+ARD/2009Nov06/Modified/IR 85 Auto Transfer // Incorporating Offshore Comments


//************************************Method Header************************************
// Method Name  : CPatientOrientationHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline CPatientOrientationHandler::CPatientOrientationHandler(
    CPQMView* pqm_view
): m_pqm_view(pqm_view)
{
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationHandler::OnOrienationChangedFor(
    const OrientationType orientation_type
)const
{

    LPCTSTR FUNC_NAME = _T("CPatientOrientationHandler::OnOrienationChangedFor");

    if (NULL == m_pqm_view) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));
        return ;
    }

    m_pqm_view->OnOrienationChangedFor(orientation_type);
}



/////////////////////////////////////////////////////////////////////////////
// CPQMView

IMPLEMENT_DYNCREATE(CPQMView, CFormView)

BEGIN_MESSAGE_MAP(CPQMView, CFormView)
    //{{AFX_MSG_MAP(CPQMView)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!

    ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)

    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()

    ON_BN_CLICKED(IDC_SAR_SCAN_ANATOMY_RGN_BUTTON, OnClickScanAnatomy)	//scan anatomy button...new GUI : by Hemant

    ON_BN_CLICKED(IDC_BUTTON_ANATOMY, OnClickAnatomyButton)	//Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
    //+Patni-SS/2009Apr07/Modi/PSP1#12
    ON_BN_CLICKED(IDC_DELETE_BUTTON, PQMMessageMapDelete)
    ON_BN_CLICKED(IDC_DUPLICATE_BUTTON, PQMMessageMapDuplicate)
    //-Patni-SS/2009Apr07/Modi/PSP1#12

    ON_BN_CLICKED(IDC_SCAN_EDIT_BUTTON, OnClickButtonSequenceEditior)
    ON_COMMAND(ID_COMMENT, OnClickButtonComment)


    ON_BN_CLICKED(IDC_LOCATOR_BUTTON, OnClickButtonScanPlan)

    //scan related buttons..
    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    //ON_BN_CLICKED(IDC_SCAN_START, OnClickButtonScanStart)
    ON_BN_CLICKED(IDC_SCAN_START, OnClickScanStart)
    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    ON_BN_CLICKED(IDC_PAUSE_RESUME, OnClickButtonScanPauseResume)

    ON_BN_CLICKED(IDC_ABORT, OnClickButtonScanAbort)

    ON_COMMAND(ID_MENUITEM_AUTO_PLAN, OnClickMenuitemAutoPlan)
    ON_COMMAND(ID_MENUITEM_PLAN_SCAN, OnClickMenuitemPlanScan)
    ON_COMMAND(ID_MENUITEM_SKIP_RGN, OnClickMenuitemSkipRgn)

    //-------------------------------------------------------------------------
    //Both the followings do the same.. : need to check..
    ON_COMMAND(ID_MENUITEM_ALL_COV_RGN, OnClickMenuitemAllCovRgn)
    ON_COMMAND(ID_MENUITEM_BATCH_COIL_AND_SAR_RGN, OnClickMenuitemBatchCoilAndSarRgn)
    //-------------------------------------------------------------------------

    ON_COMMAND(ID_MENUITEM_SPEEDERMAP_AUTOVOICE, OnClickMenuitemSpeedermapAutovoice)
    // + Patni-Sribanta/2010Jun09/Modified/MVC8032
    //ON_COMMAND(ID_MENUITEM_MULTI_COV_CAUTION, OnClickMenuitemMultiCovCaution)

    ON_COMMAND(ID_MENUITEM_GATE_MODE_ECG, OnClickMenuitemGateModeEcg)
    ON_COMMAND(ID_MENUITEM_GATE_MODE_PERIPHERAL, OnClickMenuitemGateModePeripheral)
    ON_COMMAND(ID_MENUITEM_GATE_MODE_RESPIRATORY, OnClickMenuitemGateModeRespiratory)

    //For electrode menu..
    ON_COMMAND(ID_MENUITEM_ELECTRODE_I, OnClickMenuitemElectrodeI)
    ON_COMMAND(ID_MENUITEM_ELECTRODE_II, OnClickMenuitemElectrodeIi)
    ON_COMMAND(ID_MENUITEM_ELECTRODE_III, OnClickMenuitemElectrodeIii)
    ON_COMMAND(ID_MENUITEM_ELECTRODE_AVF, OnClickMenuitemElectrodeAvf)
    ON_COMMAND(ID_MENUITEM_ELECTRODE_AVL, OnClickMenuitemElectrodeAvl)
    ON_COMMAND(ID_MENUITEM_ELECTRODE_AVR, OnClickMenuitemElectrodeAvr)
    ON_COMMAND(ID_WIRELESS_GATING_MENU, OnClickMenuitemWirelessGating)

    ON_COMMAND(ID_MENUITEM_PREP_WB_COIL, OnClickMenuitemPrepWbCoil)
    ON_COMMAND(ID_MENUITEM_SHIMMING_WB_COIL, OnClickMenuitemShimmingWbCoil)
    ON_COMMAND(ID_MENUITEM_CENTER_FREQ_CTRL, OnClickMenuitemCenterFreqCtrl)
    ON_COMMAND(ID_MENUITEM_APC_RETRY, OnClickMenuitemApcRetry)
    ON_COMMAND(ID_MENUITEM_APC_PAUSE, OnClickMenuitemApcPause)
    //Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator
    ON_COMMAND(ID_MENUITEM_PLNDUP_CONFIG, OnClickPlanDupConfig)
    //Patni-RJA/2010Feb26/Added/IR-86, PQM Debug Window
    ON_COMMAND(ID_UTILITY_DEBUGINFO, OnClickDebugInfo)
    ON_COMMAND(ID_UTILITY_SARMANAGER, OnClickMenuitemSARManager)

    //+Patni-SS/2009Apr07/Modi/PSP1#12
    ON_COMMAND(ID_SET_ALL, PQMMessageMapSetAll)
    //ON_COMMAND(ID_SET_ALL, OnClickButtonSetAll)
    //-Patni-SS/2009Apr07/Modi/PSP1#12

    ON_COMMAND(ID_PROTOCOL_AUTO_SCAN, OnClickButtonAutoScan)

    ON_COMMAND(ID_PROTOCOL_REFERROI, OnClickMenuItemReferROI)  //Patni-Manishkumar/2009Jul30/Added/MVC003833

    //+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
    ON_COMMAND(ID_PROTOCOLMENU_BREATHHOLD, OnClickMenuBreadthHold)
    ON_COMMAND(ID_PROTOCOLMENU_AUTOTRANSFER, OnClickMenuAutoTransfer)
    //Patni-RUP/2010Jul26/Commented/TMSC-REDMINE-466
    //ON_COMMAND(ID_PROTOCOLMENU_MOVECOUCH, OnClickMenuMoveCouch)
    //-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    //+Patni-RUP/2010Jul23/Modified/TMSC-REDMINE-466
    //ON_COMMAND(ID_PROTOCOLMENU_NO_MOVE, OnClickMenuNoMove)
    //ON_COMMAND(ID_PROTOCOLMENU_CENTER_OF_THE_PLAN, OnClickMenuCenterOfPlan)
    //ON_COMMAND(ID_PROTOCOLMENU_POSITION_OF_REFERENCE_IMAGE, OnClickMenuPositionOfReferenceImage)
    ON_COMMAND(ID_MOVECOUCHMENU_CENTER_OF_THE_PLAN, OnClickMenuCenterOfPlan)
    ON_COMMAND(ID_MOVECOUCHMENU_POSITION_OF_REFERENCE_IMAGE, OnClickMenuPositionOfReferenceImage)
    //-Patni-RUP/2010Jul23/Modified/TMSC-REDMINE-466
    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    ON_COMMAND(ID_PROTOCOLMENU_PLANDUPLICATE, OnClickButtonPlanDuplicate)//Ashish changes for Plan duplicate

    ON_BN_CLICKED(ID_PROTOCOLMENU_EDITDELAY, OnClickButtonEditDelay)
    ON_BN_CLICKED(ID_PLAN_RESET, OnClickButtonPlanReset)

    //Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    ON_MESSAGE(WM_UPDATE_DEBUG_WINDOW, OnUpdateDebugWindow)

    ON_MESSAGE(WM_INVOKE_COIL_INFO, OnInvokeCoilInfo)

    ON_MESSAGE(WM_INVOKE_GATE_DIALOG, OnInvokeGateDialog)

    ON_MESSAGE(WM_CMD_SARMANAGER_START, ReConnectSARManager)

    ON_MESSAGE(WM_CMD_OPEN, OnCmdOpen)
    ON_MESSAGE(WM_CMD_CLOSE, OnCmdClose)
    ON_MESSAGE(WM_CMD_SHOW, OnCmdShow)
    ON_MESSAGE(WM_CMD_HIDE, OnCmdHide)
    ON_MESSAGE(WM_CMD_SCAN_START, OnCmdScanStart)
    ON_MESSAGE(WM_CMD_FILE_NOT_EXIST, OnCmdFileNotExist)	//Patni-HAR/2010Apr21/Added/MVC007534
    ON_MESSAGE(WM_CMD_GL_SR_SHOW, OnCmdGLSRShow)


    ON_WM_DESTROY()
    //+Patni-SS/COMMENTED-2009May13/JFT End Exam Requirement
    //ON_BN_CLICKED(IDC_CLOSE, OnClickButtonPqmclose)
    ON_BN_CLICKED(IDC_END_EXQAM, PQMMessageMapEndExam)
    //-Patni-SS/COMMENTED-2009May13/JFT End Exam Requirement

    ON_BN_CLICKED(IDC_SCAN_OFFSET_BUTTON, OnClickScanOffset)
    ON_BN_CLICKED(IDC_PASS_BUTTON , OnClickPasButton)

    ON_BN_CLICKED(IDC_UTILITY_BUTTON , OnClickUtilityButton)
    ON_BN_CLICKED(IDC_COIL_BUTTON , OnClickCoilPasButton)

    ON_COMMAND_RANGE(ID_MENUITEM_N_N, ID_MENUITEM_M1_M1, OnClickAutoVoiceMenu)

    ON_COMMAND(ID_MENUITEM_INFO_SEQ, OnClickMenuitemInfoSeq)
    ON_COMMAND(ID_MENUITEM_INFO_COIL, OnClickMenuitemInfoCoil)
    ON_COMMAND(ID_MENUITEM_INFO_GATE, OnClickMenuitemInfoGate)
    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    //ON_COMMAND(ID_MENUITEM_MANUAL_PRESCAN, OnClickMenuitemManualPrescan)

    //ON_COMMAND(ID_MOVING_TO_LOCATOR_IMAGE, OnClickMenuitemLocatorImage) //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    //ON_COMMAND_RANGE(START_VOICEMENU_ITEM, START_VOICEMENU_ITEM_MAX, OnClickVoiceMenuItem)
    ON_COMMAND_RANGE(START_VOICEMENU_ITEM, START_VOICEMENU_ITEM_MAX, PQMMessageMapVoiceMenuItem)
    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    ON_COMMAND_RANGE(START_LANGUAGEMENU_ITEM, START_LANGUAGEMENU_ITEM_MAX, OnClickLanguageMenuItem)

    //SM3_Mangesh_Start
    ON_COMMAND_RANGE(ID_CONTRAST_POPUP_MENU, ID_CONTRAST_POPUP_MENU_MAX, OnClickContrastAgentMenu)
    ON_COMMAND_RANGE(ID_MENU_SCAN_ANATOMY, ID_MENU_SCAN_ANATOMY_MAX, OnClickScanAnatomyMenu)
    //SM3_Mangesh_End
    ON_BN_CLICKED(IDC_INJECT_TIME_BUTTON, OnButtonInject) // added for inject time button


    ON_MESSAGE(WM_CMD_CFA_SHOW, OnCFAShow)
    ON_MESSAGE(WM_CMD_SHOW_CFA_DIALOG, OnShowCFADialog)  //Patni-MN/2009Mar13/Added/PSP1#203
    ON_MESSAGE(WM_CMD_ABORT_FAIL, OnAbortFail)  //Patni-SS/2009Mar31/Added/PSP1#XX
    ON_MESSAGE(WM_CMD_ABORT_WAIT, OnAbortWait)  //Patni-SS/2009Mar31/Added/PSP1#XX
    ON_MESSAGE(WM_CMD_DBS_NOT_IN_SYNC, OnDataBaseNotInSync) //Patni-AMT/2009Mar21/Added/PSP1#001

    ON_MESSAGE(WM_RECONNECT_TO_PROCON, OnReConnectToProcon)  //+Patni-HEMANT/ADDED On 3/31/2009 3:09:37 PM/ PSP/ IDS#202, IDS#192

    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    ON_BN_CLICKED(IDC_QUEUE_AND_NEXT, OnOnQueueAndNext)
    ON_BN_CLICKED(IDC_QUEUE_AND_EXIT, OnQueueAndExit)
    ON_BN_CLICKED(IDC_NEXT_AND_COPY, OnNextAndCopy)
    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

    //+Patni-KSS/2010June16/ADDED/IR-155
    ON_BN_CLICKED(IDC_CANCEL, OnCancel)
    ON_BN_CLICKED(IDC_RESET, OnReset)
    //-Patni-KSS/2010June16/ADDED/IR-155

    //Patni-HEMANT/2009Sept05/Added/ MVC#002493
    ON_MESSAGE(WM_HANDLE_IRS_MESSAGE, OnHandleIRSMessages)
    //Patni-Manishkumar/2009Sept15/Added/MVC004760/Function to set wait cursor
    ON_MESSAGE(WM_HANDLE_WAITCURSOR_MESSAGE, OnSettingWaitCursor)
    ON_MESSAGE(WM_HANDLE_WAITCURSOR_MESSAGE_PQM_BUTTONS, OnSettingWaitCursorForPqmButtons)

    //+Patni-Ravindra Acharya/2010July23/Added/IR-153
    ON_MESSAGE(WM_CFSHIFT_CFA_READY, OnCFAReady)

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    ON_COMMAND(ID_UTILITY_MENU_AS_COMPASS_SETTING, OnASCOMPASSSettings)
    ON_COMMAND(ID_ENABLE_AS_COMPASS, OnOffAsCompass)
    ON_COMMAND(ID_ENABLE_CDS_DEBUG, OnOffCDSDebug)

    ON_MESSAGE(WM_CDS_COIL_CONFIRMATION, OpenCoilConfirmationDialog)
    ON_MESSAGE(WM_CDS_LOCATION_DEBUG, OpenLocationDebugDialog)
    ON_MESSAGE(WM_CDS_APPLY_CMPS_COIL_CUR_PROT, UpdateCompassCoilToCurrentProtocol)
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Sribanta/2011Feb07/Added/REDMINE-1026
    ON_MESSAGE(WM_PQM2COILSEL_CONNECT_COIL, GetConnectCoilPostMessage)
    ON_MESSAGE(WM_MSG_CORRECT_COIL, OnClickCoilPasButtonPostMessage)
    ON_MESSAGE(WM_PROCESS_MRS_MSG, ProcessMRSMessagesPostMessage)
    //-Patni-Sribanta/2011Feb07/Added/REDMINE-1026

    ON_MESSAGE(WM_INVOKE_PROBE_DIALOG, InvokeProbeDialogPostMessage)
    ON_MESSAGE(WM_INVOKE_AMB, InvokeAMB)

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    ON_COMMAND_RANGE(ID_LATERALITY_MENU_MIN , ID_LATERALITY_MENU_MAX, OnLaterality)
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    ON_COMMAND_RANGE(ID_SFT_MENU_MIN, ID_SFT_MENU_MAX, OnSFTMenuCommand)
    ON_COMMAND_RANGE(ID_TOOL_MENU_MIN, ID_TOOL_MENU_MAX, OnSFTMenuCommand)
    ON_MESSAGE(WM_OPEN_MANUAL_PRESCAN, OnClickMenuitemManualPrescan)
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    //+Patni-KSS/2010June25/ADDED/IR-156_2
    ON_COMMAND(ID_PROTOCOLMENU_APPEND, PQMMessageMapAppend)
    ON_COMMAND(ID_PROTOCOLMENU_DELETE, PQMMessageMapDelete)
    //-Patni-KSS/2010June25/ADDED/IR-156_2
    //+Patni-Ajay/2010July30/Added/61850/REDMINE-443
    ON_COMMAND_RANGE(ID_PLANDUPLICATE_MIN, ID_PLANDUPLICATE_MAX, OnClickPlanDuplicateMenu)
    //-Patni-Ajay/2010July30/Added/61850/REDMINE-443

    //Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    ON_COMMAND(ID_CARDIAC_LOCATOR, OnClickMenuCardiacLocator)
    ON_MESSAGE(WM_REFRESH_GUI, OnRefreshGUI)
    ON_COMMAND(ID_NEURO_LOCATOR, OnClickMenuNeuroLocator)


    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
    ON_COMMAND_RANGE(ID_PLAN_MASTER_BIAS_MIN, ID_PLAN_MASTER_BIAS_MAX, OnClickMasterOnPlanMark)


    ON_COMMAND(ID_PROTOCOLMENU_AMB, OnAMB)
	ON_COMMAND(ID_MENU_TIPREP, OnTiPrep) //Anulekha

    ON_WM_TIMER()

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//************************************Method Header**************************************

//Method Name:  CPQMView

//Author:  PATNI/SVP

//Purpose: Constructor

//**************************************************************************************/
CPQMView::CPQMView()
    : CFormView(CPQMView::IDD),
      m_lastacqman_handler_msg(PQM_MSG_INVALID_MESSAGE),
      m_connector_state(TRUE),
      m_current_scan_opration(CurrentScanOpration_Invalid),
      m_blink_status(NONBLINKING),
      m_apc_pause_enum(APC_EnableFromPQM),
      m_acquman_status(NOT_CONNECTED),
      m_pqm_gui_processing(PQM_PROCESSING_NONE),		//Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
      m_connection_point(NULL),
      m_cp_container(NULL),
      m_iunknown(NULL),
      m_sink(NULL),
      m_shim_done_flag(0),
      m_mem_passworddlg(NULL), //Patni-Sudhir 2011Mar-09/Added/IR-185
      m_info_seq_dlg(NULL),
      m_bLocatorAlreadyEnable(false),
      m_bEnableLocatorOnScanEdit(false),
      m_is_locator_open(false),
      m_strCoilSelect(_T("")),
      //Patni-PJS/2010Nov15/Added/V2.0/IR-175
      m_av_language(WCS::EMPTY_STRING),
      m_xOffset(20),                  //Patni-ARD/2010Apr08/Modified/JaFT# IR-100
      m_yOffset(20),                  //Patni-ARD/2010Apr08/Modified/JaFT# IR-100
      m_check_load_study(false),
      m_procon_ui_status(PQM_PROCONALLOW_SELECTIONS_PROCON_BUTTONS),

      m_is_coil_window_opened_before_swithcing_page(false),
      m_is_cfa_window_opened_before_swithcing_page(false),
      m_is_comment_window_opened_before_swithcing_page(false),
      m_is_mem_passworddlg_window_opened_before_swithcing_page(false), //Patni-Sudhir 2011Mar-09/Added/IR-185
      m_is_seqinfo_window_opened_before_swithcing_page(false),
      m_is_gateinfo_window_opened_before_swithcing_page(false),
      m_is_sar_window_opened_before_swithcing_page(false),
      m_is_scanoffset_window_opened_before_swithcing_page(false),
      m_is_manualprescan_window_opened_before_swithcing_page(false),
      m_is_mrs_window_opened_before_swithcing_page(false),
      m_is_coilinfo_opened_before_swithcing_page(false),
      m_is_delay_time_window_opened_before_swithcing_page(false),
      m_cds_controller(NULL),
      m_obj_coil_dlg(NULL),		//Added by Hemant On 1/19/2009 4:39:52 PM, Fixing 753
      m_is_palnduplicate_window_opened_before_swithcing_page(false),
      m_is_pqmdebugwin_window_opened_before_swithcing_page(false),
      m_dynamic_scan_on(false),
      m_acquman_ready_for_next_request(false),
      m_queuedialog(NULL),
      m_queuedialoghandler(NULL),
      m_dbdtcautiondialog(NULL),
      m_patientorientationdialog(NULL),
      m_patientorientationhandler(NULL),
      m_pqmmanualprescandlg(NULL),
      m_mrsprescandlg(NULL),
      m_mrsprescandlg_handler(NULL),
      m_is_procon_connected(false),
      m_head(NULL),
      m_coil_trestle_head(NULL),
      dlgCoilSelWin(NULL),
      m_axislocatordialog_interface(NULL),
      m_axis_locator_position(0),
      m_sr_invoked(false),
      m_store_mode(false),
      m_cfadlg(NULL),
      m_pqm_closed_flag(true), // true for PQM is closed and false for PQM is in open state
      m_iswait_cursor(false),
      m_iswait_cursor_for_pqm_buttons(false),
      m_is_scanning(false),
      m_scan_lock_acquired(false),
      m_org_cursor(NULL),
      m_ismemoryalive(false),
      m_memory_invokation_flag(0),
      m_is_wse_open(false),
      m_pqm_sequences(NULL),
      m_msg_seq_limit(FALSE),
      m_scan_index(-1),
      m_pqm_max_protocol(0),
      m_list(NULL),
      m_rmc_scan_running(false),
      b_connection_established(false),
      m_comment_dlg(NULL),
      m_interfaceclassforscancommentdlg(NULL),
      m_flag_abort(FALSE),
      m_current_flag(false),
      m_apcflag(0),				//Note - by Hemant: was set by menu dialog before.. need to remove out the dependency..
      m_shim_state_flag(VFC_SHIM_FSY_WATER),
      m_sr_param(_T("")),
      m_obj_gate_dlg(NULL),
      m_anatomy_dlg(NULL),
      m_setscanoffsetdlg(NULL),
      m_studyloid(_T("")),
      m_is_mrs_prot(false),//Patni-PJS/2010July02/Added/V1.30#TMSC-REDMINE-316
      m_wait_cursor_counter(0),       //Patni-HAR/2010feb26/Added/MSA0251-00219
      m_mismatch_error_flag(FALSE),   //Patni-HAR/2010Feb24/Added/MSA0251-00187
      m_GUIselAcqOrder(-1),
      m_bpost_process_flag(FALSE),
      m_mrs_process_tool(false),  //Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
      m_hMapFile(NULL),		//Patni-Sudhir 2011Mar-09/Added/IR-185
      m_wfdaimplementer(NULL),//Patni-PJS/2011Feb1/Modify/IR-97
      m_delay_time_dlg(NULL),
      m_pas_reg_flag(-1),
      m_inject_timer_time(0),
      m_dw_start_count(0),
      m_coil_channel_mode(NULL),
      m_system_enable_channel(0),
      m_limited_channel(0),
      m_use_coil_channel_mode(FALSE),
      m_inscan_refclose_msg_sent(false),
      m_is_end_exam_clicked(false),
      m_selectedprotocols(NULL),
      m_timer_pager_area(PAGER_TIMER_INTERVAL),
      m_dw_start_count_for_pager(0),
      m_sar_connect_status(false),
      m_sar_control_license(FALSE),
      m_amb_controller(),
      m_is_amb_planning(false),
      m_is_cfa_ok_pressed(true),
      m_is_waiting_for_scan_sarctrl(false)

{


    //Patni-PJS/2010Sep8/Added/TMSC-REDMINE-685
    //m_apc_pause_enum = APC_EnableFromPQM;
    //Patni-PJS/2010Mar12/Added/DeFT# MCM0229-00178
    m_curr_seq = new CSequence();

    //+Patni-SS/2009Apr07/Modi/PSP1#12
    if (!m_pqmchildthread.CreateChildThread(this)) {
        // error message to throgh
    }

    CPQMLogMgr::GetInstance()->SetPqm(this);

    m_wait_cursor = LoadCursor(NULL, IDC_WAIT);

    m_pqm_g.SetPQMMode(K_IMAGING);

    m_pqm_sequences = new CIPCComManager();

    m_list = new CSeqList();

    m_interface_for_pqmmanualprescandlg = new InterfaceForPQMManualPreScanDlg(this) ;

    for (int i = 0; i < 2; i++) {
        m_coil_liner_head[i] = NULL;
    }

    for (i = 0; i < 7; i++) {
        m_coil_couch_head[i] = NULL;
    }

    m_selection_utiliy = new CCoilSelectionUtility;
    m_contrast_agent = new CContrastAgentData; //SM3_Mangesh

    //Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
    for (int count = 0; count < (SAR_REGIONS - 2) ; count++) {
        m_scan_anatomy[count] = new CScanAnatomyData; //SM3_Mangesh
    }

    m_sft_menuhandler = new CSFTMenuHandler(this) ;
    m_coil_channel_mode = new CCoilChannelMode(this);
}



//************************************Method Header************************************

//Method Name:  ~CPQMView

//Author:  PATNI/SVP

//Purpose: Destructor

//**************************************************************************************/
CPQMView::~CPQMView(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::~CPQMView");

    //+Patni-HAR/2010Apr19/Modified/JaFT# MVC007125
#if MPLUS_V3

    if (RegisterUnregisterProcMgr(FALSE)) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("ProcessManager Unregistration successfull"));

    } else {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("ProcessManager Unregistration Failed"));
    }

#endif
    //-Patni-HAR/2010Apr19/Modified/

    //Code Review
    CDEGLCommunicator::DeleteCommunicatorIntance();

    m_acquisition->OnPQMExit();

    //Patni-ARD/2010Apr19/Deleted/JeFT# IR-100 Code Review IR100_CDR_02

    DEL_PTR(m_sink)

    if (SUCCEEDED(m_result)) {
        CoUninitialize();
    }

    DEL_PTR(m_info_seq_dlg)

    DEL_PTR(m_mem_passworddlg); //Patni-Sudhir 2011Mar-09/Added/IR-185

    DEL_PTR(m_comment_dlg)

    //Added by Hemant On 11/14/2008 11:55:59 AM
    DEL_PTR(m_interfaceclassforscancommentdlg)

    DEL_PTR(m_cfadlg)


    // - GUIControl

    if (m_bkbrush.m_hObject)
        m_bkbrush.DeleteObject();

    DEL_PTR(m_patientorientationhandler)

    DEL_PTR(m_patientorientationdialog)

    DEL_PTR(m_queuedialog)

    DEL_PTR(m_queuedialoghandler)

    DEL_PTR(m_interface_for_pqmmanualprescandlg)

    DEL_PTR(m_pqmmanualprescandlg)

    //Added by Hemant On 7/23/2008 4:43:20 PM
    //for MRS prescan dialog..
    DEL_PTR(m_mrsprescandlg_handler)

    DEL_PTR(m_mrsprescandlg)

    if (m_wait_cursor) {
        DestroyCursor(m_wait_cursor);
    }

    if (m_org_cursor) {
        DestroyCursor(m_org_cursor);
    }

    //for Coil...
    //
    DEL_PTR(dlgCoilSelWin)

    DEL_PTR(m_axislocatordialog_interface)

    DEL_PTR(m_selection_utiliy)

    DEL_PTR(m_delay_time_dlg)


    if (m_contrast_agent) {
        m_contrast_agent->RemoveAll();
        DEL_PTR(m_contrast_agent)
    }

    if (m_scan_anatomy) {

        //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
        //for (int count = 0; count < (SAR_REGIONS - 1); count++) {
        for (int count = 0; count < (SAR_REGIONS - 2); count++) {
            //-Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131

            //Modified by Hemant On 12/10/2008 3:33:40 PM
            //m_scan_anatomy[count]->RemoveAt(m_scan_anatomy[count]->FindIndex(count));
            if (m_scan_anatomy[count]) {
                m_scan_anatomy[count]->RemoveAll();
                DEL_PTR(m_scan_anatomy[count])
            }
        }
    }

    //Patni-Manishkumar/2009Aug12/Modified/MVC002704
    m_bEnableLocatorOnScanEdit = false;
    ClearSelectedAcqoderListForWSE();


    //Patni-PJS/2010Mar12/Added/DeFT# MCM0229-00178
    DEL_PTR(m_curr_seq)

    //Patni-Hemant/2010Apr5/Added/ACE-Phase#3/CDS Requirement
    DEL_PTR(m_cds_controller)

    //+Patni-HAR/2010Jun07/Added/Memory Leak
    //+Patni-PJS/2010Oct07/MVC009095
    if (m_coil_trestle_head) {
        while (m_coil_trestle_head->next) {
            m_coil_trestle_head = m_coil_trestle_head->next;
            FREE_PTR(m_coil_trestle_head->prev->item)
            FREE_PTR(m_coil_trestle_head->prev)
        }

        FREE_PTR(m_coil_trestle_head->item)
        FREE_PTR(m_coil_trestle_head)
    }

    for (int i = 0; i < 7; i++) {
        if (m_coil_couch_head[i]) {
            while (m_coil_couch_head[i]->next) {
                m_coil_couch_head[i] = m_coil_couch_head[i]->next;
                FREE_PTR(m_coil_couch_head[i]->prev->item)
                FREE_PTR(m_coil_couch_head[i]->prev)
            }

            FREE_PTR(m_coil_couch_head[i]->item)
            FREE_PTR(m_coil_couch_head[i])
        }
    }

    for (i = 0; i < 2; i++) {

        if (m_coil_liner_head[i]) {
            while (m_coil_liner_head[i]->next) {
                m_coil_liner_head[i] = m_coil_liner_head[i]->next;
                FREE_PTR(m_coil_liner_head[i]->prev->item)
                FREE_PTR(m_coil_liner_head[i]->prev)
            }

            FREE_PTR(m_coil_liner_head[i]->item)
            FREE_PTR(m_coil_liner_head[i])
        }
    }

    //-Patni-PJS/2010Oct07/MVC009095
    //-Patni-HAR/2010Jun07/Added/Memory Leak

    //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    DEL_PTR(m_sft_menuhandler)
    //Patni-Sudhir 2011Mar-09/Added/IR-185
    DeleteWDFAMsgReceiver();
    DEL_PTR(m_wfdaimplementer);//Patni-PJS/2011Feb1/Modify/IR-97
    CDPSManager::Destroy();

    CloseHandle(m_hMapFile);

    // //+Patni-SS/2001Aug29/CodeMoved/Redmine#2018/ As per TMSC suggestion


}


//+Patni-Sribanta/2010Nov10/Added/IR-170
//****************************Method Header************************************
//Method Name   :CheckPASselectionFile()
//Author        :PATNI/Sribanta
//Purpose       :This function checks whether the PAS selection file exists or\
//				 not. If it doesn't exists then the function returns false.If \
//				 the file exists and it has read access then it return true.
//*****************************************************************************
CString CPQMView::CheckPASselectionFile()const
{

    CString FUNC_NAME = _T("CPQMView::CheckPASselectionFile.");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString l_pas_path = _T("");

    char* l_buf = getenv("MRMP_USR_TMP_DIR");
    char l_path[512] = {0};
    strcpy(l_path, l_buf);

    const char* l_pas_selection_file = "\\PAS_Selection.txt";
    strcat(l_path, l_pas_selection_file);

    if (!WNP_access(l_path, 00)) {

        FILE* fp = NULL;

        if ((fp = fopen(l_path, "r")) != NULL) {

            char  l_filecontent_buff[256] = {0};
            memset(l_filecontent_buff, NULL, sizeof(l_filecontent_buff));
            fgets(l_filecontent_buff, sizeof(l_filecontent_buff), fp);

            l_pas_path = l_filecontent_buff;
            fclose(fp);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME ,
                      _T("Failed to read  PAS_Selection.txt file."));
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Pas Selection File does not exists"));
    }

    return l_pas_path;
}
//-Patni-Sribanta/2010Nov10/Added/IR-170


//+Patni-MSN 2009May-26/Un-Commented/PSP1#371

//+Patni-Manishkumar 2009April-22/Added/PSP1#371
//************************************Method Header************************************
// Method Name  : ChangeViewOfModelessDialogs()
// Author       : PATNI/ Manishkumar
// Purpose      : To  disable/enable the modeless dialogs
//***********************************************************************************
//Patni-AJS+Hemant/2011Jan31/Modified/REDMINE-1226
void CPQMView::ChangeViewOfModelessDialogs(const bool f_bCheck)
{
    //+Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
    static int l_counter = 0;

    if (!f_bCheck) {
        l_counter++ ;

        if (l_counter != 1) {
            return ;
        }

    } else {
        l_counter-- ;

        if (l_counter != 0) {
            return ;
        }
    }

    //-Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open


    //+Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163
    if (m_comment_dlg && ::IsWindow(m_comment_dlg->m_hWnd)) {
        m_comment_dlg->ChangeViewOfCoilListDlg(f_bCheck);
        m_comment_dlg->EnableWindow(f_bCheck);
    }

    //-Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163

    if (m_info_seq_dlg && ::IsWindow(m_info_seq_dlg->m_hWnd))
        m_info_seq_dlg->EnableWindow(f_bCheck);

    if (m_patientorientationdialog &&::IsWindow(m_patientorientationdialog->m_hWnd))
        m_patientorientationdialog->EnableWindow(f_bCheck);

    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    //	if(m_queuedialog && ::IsWindow(m_queuedialog->m_hWnd))  //Patni-Manishkumar 2009April-27/Modified/PSP1#371
    //	   m_queuedialog->EnableWindow(f_bCheck);
    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

    //+Patni-Manishkumar 2009May-04/Added/PSP1#51
    if (m_mrsprescandlg && ::IsWindow(m_mrsprescandlg->m_hWnd))
        m_mrsprescandlg->EnableWindow(f_bCheck);

    if (m_pqmmanualprescandlg && ::IsWindow(m_pqmmanualprescandlg->m_hWnd))
        m_pqmmanualprescandlg->EnableWindow(f_bCheck);

    //-Patni-Manishkumar 2009May-04/Added/PSP1#51

    if (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd)) {
        dlgCoilSelWin->ChangeViewOfCoilListDlg(f_bCheck);
        dlgCoilSelWin->EnableWindow(f_bCheck);
    }

    //+Patni-AJS+Hemant/2011Feb07/Added/REDMINE-1226
    if (::IsWindow(m_pqmdebugdlg.m_hWnd)) {
        m_pqmdebugdlg.EnableWindow(f_bCheck);
    }

    if (::IsWindow(m_pqmplandup_dlg.m_hWnd)) {
        m_pqmplandup_dlg.EnableWindow(f_bCheck);
    }

    //-Patni-AJS+Hemant/2011Feb07/Added/REDMINE-1226

}
//-Patni-Manishkumar 2009April-22/Added/PSP1#371


// + Patni- Sribanta/2010Jun10/Added/MVC7933
//************************************Method Header************************************
// Method Name  : SendDeleteReserveDEGL
// Author       : PATNI/ Sribanta
// Purpose      : Sending DeleteReserve message to DEGL
//***********************************************************************************
void CPQMView::SendDeleteReserveDEGL(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SendDeleteReserveDEGL");

    if (!CDEGLCommunicator::GetInstance()->SendDeleteReserve(m_studyloid)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("SendDeleteReserve failed"));
    }
}



//************************************Method Header************************************
// Method Name  : SendDeleteReserveDEGL
// Author       : PATNI/ Sribanta
// Purpose      : Sending DeleteReserve message to DEGL
//***********************************************************************************

void CPQMView::SendDeleteCompleteDEGL(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SendDeleteCompleteDEGL");

    if (!CDEGLCommunicator::GetInstance()->SendDeleteComplete(m_studyloid)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("SendDeleteComplete failed"));
    }
}
// - Patni- Sribanta/2010Jun10/Added/MVC7933


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
void CPQMView::HandleSFTMenuCommand(
    UINT f_cmd_id
)const
{
    m_sft_menuhandler->HandleSFTMenuCommand(f_cmd_id);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnOffCDSDebug(
)const
{
    if (m_cds_controller) {
        m_cds_controller->OnOffCDSDebug();
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//Patni-Hemant/2011Apr20/Modified/TMSC-REDMINE-1671
void CPQMView::DetachSFTToolmenu()const
{

    m_sft_menuhandler->DetachSFTToolmenu();
}
//Patni-Hemant/2011Apr20/Modified/TMSC-REDMINE-1671
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
void CPQMView::AttachSFTMenu(
    PopupMenuEx* f_menu
)const
{
    //+Patni-Hemant/2010July21/Modified/IR-152_Part 3
    if ((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(VK_SHIFT) < 0)) {
        m_sft_menuhandler->AttachSFTMenu(f_menu);
    }

    //-Patni-Hemant/2010July21/Modified/IR-152_Part 3

    m_sft_menuhandler->AttachToolMenu(f_menu);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Ajay/2010July28/TMSC-REDMINE-443
//************************************Method Header************************************
// Method Name  : AttachPlanDuplicateMenu
// Author       : PATNI/ Ajay
// Purpose      : Function to Enable/Disable OK Button
//***********************************************************************************
void CPQMView::AttachPlanDuplicateMenu(
    const int f_protocol_index,
    PopupMenuEx* f_protocol_menu
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::AttachPlanDuplicateMenu");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));

    if (!f_protocol_menu || !IsValidSequenceIndex(f_protocol_index)) {

        return ;
    }

    const CString l_plan_duplicate_string = CPQMUtility::GetMultiLingualString(_T("ID_PLAN_DUPLICATE"));

    CString l_sub_menu_string = _T("");

    const int l_menu_cnt = f_protocol_menu->GetMenuItemCount();

    for (int l_index = 0; l_index < l_menu_cnt; l_index++) {

        f_protocol_menu->GetMenuString(l_index, l_sub_menu_string, MF_BYPOSITION);

        if (l_sub_menu_string == l_plan_duplicate_string) {

            PopupMenuEx* l_plan_duplicate_menu = (PopupMenuEx*)(f_protocol_menu->GetSubMenu(l_index));
            CreatePlanDuplicatePopupMenu(l_plan_duplicate_menu, f_protocol_index);

            break ;
        }
    }

    if (!CanPlanAndDuplicateforIndex(f_protocol_index)) {
        f_protocol_menu->EnableMenuItem(ID_PROTOCOLMENU_PLANDUPLICATE, MF_BYCOMMAND | MF_GRAYED);

    } else if (CanPlanAndDuplicateforIndex(f_protocol_index)) {
        if (GetWaitCursor() == false) {
            f_protocol_menu->EnableMenuItem(ID_PROTOCOLMENU_PLANDUPLICATE, MF_BYCOMMAND);

        } else  {
            f_protocol_menu->EnableMenuItem(ID_PROTOCOLMENU_PLANDUPLICATE, MF_BYCOMMAND | MF_GRAYED);
        }
    }
}
//-Patni-Ajay/2010July28/TMSC-REDMINE-443


//+Patni-Hemant/2010May26/Added/MaFT/TMSC-REDMINE-MVC007927
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::UpdateCurrentSequence(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateCurrentSequence") ;
    PQM_TRACE_SCOPED(FUNC_NAME);
    //-Redmine-2520

    if (m_curr_seq == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_curr_seq is NULL."));
        return ;
    }

    const int l_first_wait_prot = m_pqm_listctrl.GetFirstWaitProtocolIndex() ;

    if (!IsValidSequenceIndex(l_first_wait_prot)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("First wait protocol not valid"));
        return ;
    }

    *m_curr_seq = *((CSequence*)m_pqm_listctrl.GetItemData(l_first_wait_prot));
}
//-Patni-Hemant/2010May26/Added/MaFT/TMSC-REDMINE-MVC007927


//+Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
//**************************************************************************
//Method Name   :  ChangeScanTime()
//Author        :  PATNI/PJS
//Purpose       :  This function is used to update scan time on the ListCtrl
//				   for the protocol which is currently being scanned
//**************************************************************************
void CPQMView::ChangeScanTime(
)
{

    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    CString FUNC_NAME(_T("CPQMView::ChangeScanTime"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));

    const int l_scan_index = GetScanIndex() ;//change scan time for the current protocol

    if (!IsValidSequenceIndex(l_scan_index)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("invalid sequence index"));
        return;
    }

    CSequence* l_pseq = (CSequence*) m_pqm_listctrl.GetItemData(l_scan_index);

    if (l_pseq == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pseq pointer is NULL"));
        return;
    }

    VARIANT* pData = new VARIANT;

    if (pData == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pData pointer is NULL"));
        return;
    }

    if (m_acquisition == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_acquisition pointer is NULL"));

        //REDMINE-781
        DEL_PTR(pData);
        return;
    }

    //Get scan time for protocol in the current state.
    HRESULT l_result = m_acquisition->GetScanTimeForCurrProt(pData);

    if (l_result == S_FALSE) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("failed to get scan time for current protocol"));

        //REDMINE-781
        DEL_PTR(pData);
        return;
    }

    CPASParamStore* l_pparam = l_pseq->GetCPASCOMSequence()->GetSequenceParam();

    if (l_pparam == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pparam pointer is NULL"));

        //REDMINE-781
        DEL_PTR(pData);
        return;
    }

    //Set the updated scan time in the PASParam and update the scan time in the ListCtrl.
    //+Patni-DKH/2010Sep06/Modified/MVC008741,IR-161 Reopen
    //l_pparam->SetInt(_T("SCANTIME") , (int)pData->llVal);
    float l_fscan_time(static_cast<float>(pData->llVal));
    l_pparam->SetFloat(_T("SCANTIME") , l_fscan_time);
    //-Patni-DKH/2010Sep06/Modified/MVC008741,IR-161 Reopen
    int l_scan_item[] = {l_scan_index};
    m_pqm_listctrl.UpdateSequences(l_scan_item, 1);

    DEL_PTR(pData);
    //-Patni-HAR-NP/2010Aug11/Modified/IR-161
    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
}
//-Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178


//+Patni-PJS/2010Feb01/Added/MVC004689
//************************************Method Header**********************************
// Method Name  : BlinkPage
// Author       : PATNI/PJS
// Purpose      : This function is used to start or stop blink depending upon mode
//***********************************************************************************
void CPQMView::BlinkPage(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::BlinkPage") ;

    if (m_pqm_g.GetPQMMode() != K_IMAGING) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Not in Acquire page ==> return"));
        return;
    }

    CString l_param = DPS_PAGETAB_ACQUIRE;

    //+MEITEC/2010Jun29/Added/JaFT#MVC008392
    CString l_log_str(_T(""));
    l_log_str.Format(_T("[Blink Status : %d]"), m_blink_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    //- MEITEC/2010Apr07/Modified/Ph-3# PROPOSAL_06

    CString l_msg = _T("");
    BOOL l_bsend_msg_to_dps = FALSE;
    const int l_status = CPQMUtility::GetInstance()->GetPQMStatus();

    //- MEITEC/2010Apr07/Modified/Ph-3# PROPOSAL_06
    if ((m_blink_status == NONBLINKING) && (l_status == STATE_INACTIVE)) {

        l_msg = DPS_MSGID_REQ_START_BLINK;
        l_param = l_param + CString(_T("/1"));
        m_blink_status = BLINKING;
        l_bsend_msg_to_dps = TRUE;

    } else if ((m_blink_status == BLINKING) && (l_status == STATE_ACTIVE)) {

        l_msg = DPS_MSGID_REQ_STOP_BLINK;
        m_blink_status = NONBLINKING;
        l_bsend_msg_to_dps = TRUE;
        //+Patni-PJS/2010Jun24/Added/JaFT#MVC008406

    } else if ((m_blink_status == BLINKING) && (l_status == STATE_INACTIVE)) {

        l_msg = DPS_MSGID_REQ_START_BLINK;
        l_param = l_param + CString(_T("/1"));
        l_bsend_msg_to_dps = TRUE;
        //-Patni-PJS/2010Jun24/Added/JaFT#MVC008406

    } else {

        l_log_str.Format(_T("BLINK_STATUS=%d  PQM_Status: %s"), m_blink_status,
                         CPQMUtility::GetInstance()->GetPQMStatusString());


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        l_bsend_msg_to_dps = FALSE;
    }

    if (TRUE == l_bsend_msg_to_dps) {

        l_log_str.Format(_T("[l_bsend_msg_to_dps = TRUE] [BLINK_STATUS : %d] [PQM_Status : %s]"),
                         m_blink_status, CPQMUtility::GetInstance()->GetPQMStatusString());

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
        //-MEITEC/2010Jun29/Added/JaFT#MVC008392

        CDPSManager::GetInstance()->SendASyncMsg(DPS_PAGER, l_msg, l_param);
    }

    //-Patni-PJS/2010Feb19/Added/DeFT# MVC004689 Code review
    //-Patni-AMT/2010Feb25/Modified/Ph-3# PROPOSAL_06
}


//****************************Method Header************************************
//Method Name   :UpdatePagerAreaFor
//Author        :PATNI/HEMANT
//Purpose       :
//*****************************************************************************
//+Patni-PJS/2010Jan19/Modified/MVC006828
//+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
void CPQMView::UpdatePagerAreaFor()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::UpdatePagerAreaFor");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));


    //+ MEITEC/2010Jun23/Modified/MVC008324_2
    try {
        //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK

        VARIANT* pdata = new VARIANT;

        if (pdata == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("pdata is NULL"));
            return;
        }

        //+Patni-HAR/2010Jun23/Modified/MVC008324_2
        CString l_parameter(_T(""));
        pdata->byref = &l_parameter;
        m_acquisition->GetCurrentSarInfoToDisplay(&pdata) ;
        UpdatePagerArea(_T("FW_ACQINFO_LOW"), l_parameter, _T(""));
        //-Patni-HAR/2010Jun23/Modified/MVC008324_2

        pdata->byref = NULL;

        DEL_PTR(pdata)

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Exception"));
        throw;
    }

    //- MEITEC/2010Jun23/Modified/MVC008324_2
}
//- Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)


//+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
//************************************Method Header************************************
// Method Name  : UpdateSarInfo
// Author       : PATNI/ PJS
// Purpose      : To retrieve the coil name of selected protocol.
//***********************************************************************************
void CPQMView::UpdateSarInfo()
{
    //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateSarInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));

    const int index = GetSelectedSeqIndex();

    //Patni-AMT/2010Apr16/Modified/JaFT# PROPOSAL6_CDR_08
    if (index == -1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("SelectedSeqIndex is returning -1. Invalid index"));
        return ;
    }

    //+Patni-MP & NP/2010Jul10/Modified/MVC008324-2

    if (m_acquisition != NULL) {
        m_acquisition->SetProtocolIndex(index);

    } else {
        return;
    }

    //+Patni-MP & NP/2010Jul10/Modified/MVC008324-2

    VARIANT* pdata = new VARIANT;

    if (S_OK == m_acquisition->GetSarInfoPtr(pdata)) {
        sarInfo_t* l_ptr_sarInfo = (sarInfo_t*)pdata->byref;
        CSequence* l_pseq = GetSelectedSequence(index);

        if ((NULL != l_pseq) && (NULL != l_ptr_sarInfo)) {
            m_pqm_listctrl.InvalidateItemsForStatus(index);
        }
    }

    DEL_PTR(pdata)
    //-Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
}
//- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699

//************************************Method Header************************************
// Method Name  : DisplayDbDtCautionDialog
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMView::DisplayDbDtCautionDialog(
)
{


    m_dbdtcautiondialog = new CPqmdBdtCautionDialog(this);

    ChangeViewOfModelessDialogs(false);
    m_dbdtcautiondialog->DoModal();
    ChangeViewOfModelessDialogs(true);

    DEL_PTR(m_dbdtcautiondialog)

}

//************************************Method Header************************************
// Method Name  : DisplayOffset
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMView::DisplayOffset()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayOffset");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_cfadlg) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_cfadlg pointer is NULL"));

        return;

    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    m_cfadlg->DisplayOffset();
}


void CPQMView::DisplayCoilInfo(
)
{
    PostMessage(WM_INVOKE_COIL_INFO, 0, 0);
}


//************************************Method Header************************************
// Method Name  : ApplyCoilToProtocols
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyCoilToProtocols(
)
{
    //as per old code :
    //if Batch setting is on
    //- apply anatomy to first selected item and the remaining all items.
    //else
    // - apply anatomy to only selected items.

    //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
    if (m_selection_utiliy && m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS) {

        if (m_cds_controller) {
            m_cds_controller->CDSApplyCoiltoProtocols();
        }

        return ;
    }

    //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
    int* selected_items = NULL ;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items);

    if (!selected_items || !count) {

        //        ASSERT(FALSE);
        return ;
    }

    SetWaitCursor(true) ;

    if (IsBatchSettingOn()) {

        //batch setting is on so apply anatomy settings to all the items below the first selected item (including first selected item)

        int currently_sel_item = selected_items[0] ;

        DEL_PTR_ARY(selected_items)

        count = (m_pqm_listctrl.GetItemCount() - currently_sel_item) ;

        selected_items = new int[count] ;

        int counter = 0;

        for (int item = currently_sel_item; counter < count; item++)
            selected_items[counter++] = item;
    }

    ApplyCoilToProtocols(selected_items, count);

    DEL_PTR_ARY(selected_items)

    SetWaitCursor(false) ;
}

//****************************Method Header************************************
//Method Name   : SameStudyOpened
//Author        : PATNI
//Purpose       : Send Hide PQM message to page if requested for same study to open
//*****************************************************************************
bool CPQMView::SameStudyOpened(
)
{
    //+Patni-ARD/2010Feb18/Modified/Ph-3# PROPOSAL_06

    LPCTSTR FUNC_NAME = _T("CPQMView::SameStudyOpened");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (CPQMUtility::GetInstance()->GetPQMParentName()) {

        //+Patni-HAR/2010May21/Modified/MaFT#MVC007934
        //m_pqm_closed_flag = true;
        //ShowSR();
        //m_sr_invoked = false;
        //HidePQM();
        //SendNotifyToSR();

        if (m_pqm_g.GetPQMMode() != K_IMAGING) {

            HidePQM();
            ShowSR();
        }

        SendNotifyToSR();
        m_sr_invoked = false;
        //-Patni-ARD/2010Feb18/Modified/Ph-3# PROPOSAL_06
        //Patni-HAR/2010May21/Modified/MaFT#MVC007934
    }

    return true;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsAcqumanReadyForNextRequest(
)const
{
    //changes done by Sarjeet for Scan Start button Enable/Disable 08/01/2008
    // If Acqman is not connected and not in ready state than it should return false
    return (m_acquman_status == CONNECTED && m_acquman_ready_for_next_request == ACQMAN_READY_FOR_NEXT_REQ) ;
}

void CPQMView::DisplayGateDialog(
)
{
    PostMessage(WM_INVOKE_GATE_DIALOG, 0, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CPQMView message handlers


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnDialogClosedForParameterEditing(
    int item_index,
    const bool prev_auto_scan_flag_status
)
{

    if (prev_auto_scan_flag_status) {

        //now before making it on to prev auto scan state check weather its not
        //the first wait now.. and prev should be current or wait..
        //if prev is fail or done , we can not make it auto scan..

        //now sondier that 0th item can not be set for auto scan : obviously..
        if (item_index > 0) {

            CSequence* prev_seq = (CSequence*) m_pqm_listctrl.GetItemData(item_index - 1);

            if (prev_seq) {

                //now if prev item's state is fail or done then skip the folloings
                switch (prev_seq->GetAcqStatus()) {

                    case PR_ACQ_STATUS_FAIL :
                    case PR_ACQ_STATUS_SUCCESS :
                        break ;

                    case PR_ACQ_STATUS_CURRENT :
                    case PR_ACQ_STATUS_WAIT : {

                        if (item_index != m_pqm_listctrl.GetWSEEditingProtocolIndex())
                            ApplyAutoScanToItems(&item_index, 1, prev_auto_scan_flag_status); //make it on as it was on previously : explicitly..
                    }
                    break ;
                }
            }
        }
    }
}


//+Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
//************************************Method Header************************************
// Method Name  : EnableLocatorButton()
// Author       : PATNI/ Manish
// Purpose      : Enable locator button - Called through IRS Listner
//***********************************************************************************
void CPQMView::EnableLocatorButton()
{
    //+Patni-Hemant/2009Dec03/Added/MVC006052
    m_bEnableLocatorOnScanEdit = true ;
    EnableDisableScanEditOrPlanButtons();
    //-Patni-Hemant/2009Dec03/Added/MVC006052
}

//-Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue


//+Patni-Manishkumar/2009Aug12/Modified/MVC002704
//************************************Method Header************************************
// Method Name  : OnFlagOfLocatorEnable()
// Author       : PATNI/ Mnaishkumar
// Purpose      : Set the flag value true
//***********************************************************************************
void CPQMView::OnFlagOfLocatorEnable()
{
    m_bEnableLocatorOnScanEdit = true;
}
//-Patni-Manishkumar/2009Aug12/Modified/MVC002704

//+Patni-Hemant/2009Nov16/Added/IR#90/SFT Menu
//************************************Method Header************************************
// Method Name  : CanPerfornManualPrescan
// Author       : PATNI/ HEMANT
// Purpose      : Function returns true when manual prescan can be performed.
//                Otherwise returns false
//***********************************************************************************
bool CPQMView::CanPerfornManualPrescan(
)const
{
    return !((m_pqmmanualprescandlg && m_pqmmanualprescandlg->IsWindowVisible())
             || (m_pqm_g.GetPQMMode() != K_IMAGING)
             || IsAMB()
            ) ;
}
//-Patni-Hemant/2009Nov16/Added/IR#90/SFT Menu


//Patni-DKH/2010Jan28/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   : StartPQMDebug
//Author        : PATNI\DKH
//Purpose       : To start the debug of selected option
//**************************************************************************
void CPQMView::StartPQMDebug(const debug_option f_en_debug_option, const UINT f_buttonmask)const
{
    m_acquisition->StartPQMDebug(f_en_debug_option, f_buttonmask);
}
//**************************************************************************
//Method Name   : UpdateDebugWindow
//Author        : PATNI\DKH
//Purpose       : To Update the debug window with given debug string
//**************************************************************************
void CPQMView::UpdateDebugWindow(const CString& f_csDebugdata)
{
    if (NULL != m_pqmdebugdlg.m_hWnd) {

        //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
        //m_pqmdebugdlg.UpdateDebugData(f_csDebugdata);
        m_pqmdebugdlg.InsertDebugStr(f_csDebugdata);
        g_MainView->PostMessage(WM_UPDATE_DEBUG_WINDOW, 0, 0);
        //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    }
}

//Patni-DKH/2010Jan28/Added/PH#3#/IR86/DebugWindow


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelSetCenterPos
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::CoilSelSetCenterPos(
)const
{

    if (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd)) {

        dlgCoilSelWin->CoilSelSetCenterPos();
        return ;
    }

    //	ASSERT(FALSE);
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : IsCDSDebugON
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsCDSDebugON()const
{

    if (m_cds_controller) {
        return m_cds_controller->IsCDSDebugON();
    }

    return false ;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnAxisLocatorDialogClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnAxisLocatorDialogClosed(
    const bool ok_pressed /*= true*/
)
{
    OnSelectionStatusChanged();

    //+Patni-Hemant/2010Apr7/Added/ACE-Phase#3/CDS Requirement
    if (m_selection_utiliy && (m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS)) {
        Invalidate(true);
        m_pqm_listctrl.Invalidate(true);
        m_pqm_listctrl.UpdateWindow();
        UpdateWindow();
    }

    m_acquisition->CoilSelectionClosed(ok_pressed) ;

    if (ok_pressed && m_amb_controller.IsAMBInvoked() && !(m_selection_utiliy && (m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS))) {
        OnClickScanStart();
    }

    //-Patni-Hemant/2010Apr7/Added/ACE-Phase#3/CDS Requirement

    //now inform ipc that coil selecion has been closed with Ok/Cancel
}
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : SelectCurrentProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SelectCurrentProtocol(
)
{
    if (IsValidSequenceIndex(GetScanIndex())) {
        m_pqm_listctrl.SelectOnlyItem(GetScanIndex());
    }
}
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement


//************************************Method Header************************************
// Method Name  : OpenCoilSelectionDialogForCDS
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OpenCoilSelectionDialogForCDS(
)
{
    OpenCoilSelectionDialog(true);
}

//************************************Method Header************************************
// Method Name  : SendUpdateCurrentExamData
// Author       : PATNI/Dhanesh
// Purpose      : Sending update current exam data message to DEGL
//***********************************************************************************
void CPQMView::SendUpdateCurrentExamData(
)
{
    if (!CDEGLCommunicator::GetInstance()->SendCurrentExamUpdate(m_srInfo.m_strPatID, m_srInfo.m_strStudyID)) {
        PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::SendUpdateCurrentExamData"),
                  _T("SendUpdateCurrentExamData failed"));
    }
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ShowHideOpenedWindows(
    CDialog* f_dialog,
    bool& f_window_flag_to_use,
    const bool f_show /*= true*/
)const
{

    if (f_dialog) {

        if (::IsWindow(f_dialog->m_hWnd)) {


            if (!f_show && !f_window_flag_to_use) {


                f_window_flag_to_use = f_dialog->IsWindowVisible() ? true : false;

            }

            f_dialog->ShowWindow((f_show && f_window_flag_to_use) ? SW_SHOW : SW_HIDE);

            if (f_show) {
                f_window_flag_to_use = false;
            }
        }
    }
}

//************************************Method Header************************************
// Method Name  : IsBatchSettingOn
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsBatchSettingOn(
) const //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_17
{
    return m_pqm_g.GetBatchSettingFlag() ;
}

//************************************Method Header************************************
// Method Name  : SetBatchSettingOnOff
// Author       :
// Purpose      :
//***********************************************************************************

void CPQMView::SetBatchSettingOnOff(
    const bool f_onoff
)
{

    m_pqm_g.SetBatchSettingFlag(f_onoff);

    //we have changed the batch settings...
    m_acquisition->SetBatchForSARNCoil(f_onoff);
}

//************************************Method Header************************************
// Method Name  : ApplyCoilToProtocols
// Author       :
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyCoilToProtocols(
    const int* const selected_items,
    const int size
)
{
    //Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK

    LPCTSTR l_func_name = _T("CPQMView::ApplyCoilToProtocols");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        //Patni-ARD/2010Apr29/Added/JaFT# MCM0229-00178_CDR_06 + code review

        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    if (NULL == selected_items) {

        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("selected_items pointer is NULL"));


        return;
    }

    coilSelectListStruct_t coilSelectListStructObj;
    coilSelectListStructObj.m_listInfo = *(m_selection_utiliy->GetterToPqmListCoil());
    coilSelectListStructObj.m_listCoordinateListInfo = *(m_selection_utiliy->GetPqmCordinateListInfo());

    VARIANT* pData1 = new VARIANT;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == pData1) {
        //Patni-ARD/2010Apr29/Added/JaFT# MCM0229-00178_CDR_06 + code review

        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("pData1 pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    pData1->byref = &coilSelectListStructObj;

    coilSelect_t l_coil_info2 = m_selection_utiliy->GetToPqmSelectCoil();
    VARIANT* pData2 = new VARIANT;

    //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    if (NULL == pData2) {
        PQM_TRACE(USER_FUNC_MARKER, l_func_name, _T("pData2 pointer is NULL"));

        DEL_PTR(pData1)
        return;
    }

    //-Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK


    pData2->byref = &l_coil_info2;
    const int recv_coil_id = __recv_coil_id ; //COILKEY;
    CString recv_coil_label = _T("") ;
    CSequence* l_seq = NULL ;
    int l_acq_order = -1;

    if (m_use_coil_channel_mode) { // 21-Oct-11
        l_seq = (CSequence*)m_pqm_listctrl.GetItemData(selected_items[0]);

        if (NULL == l_seq) {
            DEL_PTR(pData1);
            DEL_PTR(pData2);
            PQM_TRACE(USER_FUNC_MARKER, l_func_name, _T("l_seq is NULL"));
            return;
        }

        l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

        if ((l_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) && (m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS)) {

            if (m_use_coil_channel_mode && S_FALSE == m_acquisition->SetChannelModeSelection(l_acq_order, dlgCoilSelWin->GetSelectedChannelMode(), dlgCoilSelWin->GetSelectedChannelMode() ? m_system_enable_channel : m_limited_channel)) { //IR-193

                PQM_TRACE(USER_FUNC_MARKER, l_func_name, _T("Selected channel mode is not set"));
            }

        }
    }

    //-IR-193
    l_seq = NULL;
    l_acq_order = -1;

    for (int index = 0; index < size ; index++) {

        l_seq = (CSequence*)m_pqm_listctrl.GetItemData(selected_items[index]);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, l_func_name,  _T("l_seq is NULL"));
            continue;
        }

        //+Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement
        //We will allow to change wse editing coil.
        //
        //        //+Patni-HEMANT/ADDED On 4/2/2009 3:50:48 PM/ PSP / Internal Bug Fix
        //        //
        //        //skip apply coil to Non wait protocol
        //
        //		if (l_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT)
        //			continue ;

        //        //skip apply coil to protocol currently edited by WSE
        //        if (m_is_wse_open && (protocol_index == m_pqm_listctrl.GetWSEEditingProtocolIndex()))
        //            continue ;

        //		 //
        //        //-Patni-HEMANT/ ADDED On4/2/2009 3:50:51 PM/ PSP / Internal Bug Fix


        if (l_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) {

            if ((GetScanIndex() == selected_items[index]) &&
                (m_selection_utiliy && m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS)
               ) {
            } else {
                continue ;
            }
        }

        //-Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement


        l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

        m_acquisition->SendConnectedCoilListToIPC(pData1, l_acq_order);
        m_acquisition->SendSelectedCoilListToIPC(pData2, l_acq_order);


        //now apply settings to local seq structure..
        //
        if (recv_coil_label.IsEmpty())
            recv_coil_label = GetCoilStringFor(l_acq_order) ;

        l_seq->SetValue(rc, recv_coil_label);

        CPqmPrMode& prmode = GetPrModeOfSelectedSequence(selected_items[index]);
        prmode.m_rcv_coil_id = recv_coil_id;
        prmode.m_recv_coil_label = recv_coil_label;

        if (m_use_coil_channel_mode) { // 21-Oct-11
            //Below is required if user has selected Normal mode and selected coils are more then Limited channels

            if (S_FALSE == m_acquisition->SetChannelModeSelection(l_acq_order, dlgCoilSelWin->GetSelectedChannelMode(), dlgCoilSelWin->GetSelectedChannelMode() ? m_system_enable_channel : m_limited_channel)) { //IR-193


                PQM_TRACE(USER_FUNC_MARKER, l_func_name, _T("Selected channel mode is not set"));
            }
        }
    }

    //+Patni-HAR/2009April15/Modified/Memory Leak

    DEL_PTR(pData1)

    DEL_PTR(pData2)

    //-Patni-HAR/2009April15/Modified/Memory Leak
    //+Patni-HEMANT/MODIFIED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
    UpdateTRSCFromGivenIndex(selected_items[0], CPQMView::TRSC_R);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::DisplaySelectedCoil(
    const int idx
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::DisplaySelectedCoil");


    PQM_TRACE(USER_FUNC_MARKER,
              FUNC_NAME,
              FUNC_NAME);

    //Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    //CString str_tmp = _T("---");
    CString str_tmp = _T("");

    if (idx != -1) {
        CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(idx);

        if (NULL != l_seq) {

            str_tmp = l_seq->GetPrMode().m_recv_coil_label;
        }
    }

    m_coil_info_static.SetWindowText(str_tmp);
    m_strCoilSelect = str_tmp; //Patni-Manish/2009July01/Added/JFT#GUIRenewal-Added value in variable
}


//************************************Method Header************************************
// Method Name  : IsValidSequenceIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsValidSequenceIndex(
    const int index
)const
{
    return (m_pqm_listctrl.IsValidSequenceIndex(index) ? true : false);
}


//****************************Method Header************************************
//Module         : IsWaitCursor
//Author         : PATNI/
//Purpose        :
//*****************************************************************************
bool CPQMView::IsWaitCursor(
)const
{
    return m_iswait_cursor;
}


//************************************Method Header************************************
// Method Name  : IsWindowVisible()
// Author       : PATNI/Sribanta
// Purpose      : This function will return TRUE if PQM is visible.
//***********************************************************************************
BOOL  CPQMView::IsWindowVisible()const
{

    DWORD dwStyle = ::GetWindowLong(GetParentFrame()->m_hWnd, GWL_STYLE);
    return ((dwStyle & WS_VISIBLE) == WS_VISIBLE) ;
}


void CPQMView::AMBApplyAutoScanToItems()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::AMBApplyAutoScanToItems") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    int* l_arr_items = NULL;
    int l_count = 0;
    BOOL l_is_auto_scan_on = FALSE;

    m_acquisition->GetAMBAutoScanItems(&l_arr_items, &l_count, &l_is_auto_scan_on);

    int l_index = -1;

    for (int i = 0; i < l_count; i++) {

        l_index = GetProtocolIndexFromAcqorder(l_arr_items[i]) ;
        GetSequenceInformation(l_index);

        m_pqm_listctrl.UpdateSequences(&l_index, 1);
    }


    DEL_PTR_ARY(l_arr_items);
}


//************************************Method Header************************************
// Method Name  : ApplyAutoScanToItems
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyAutoScanToItems(
    const int* const selected_items,
    const int size,
    const bool auto_scan_on /*= true*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyAutoScanToItems") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (selected_items == NULL) {

        PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME,
                  _T("selected_items pointer is NULL"));

        return;
    }

    if (size != 0) {

        VARIANT* pdata = new VARIANT;

        if (pdata == NULL) {

            PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME,
                      _T("pdata pointer is NULL"));

            return;
        }

        int l_scan_index = -1;
        int holdvalue = -1;
        bool current_auto_scan_status = true ;
        CSequence* pseq = NULL ;

        for (int index = 0; index < size; index++) {

            pseq = (CSequence*) m_pqm_listctrl.GetItemData(selected_items[index]);

            if (NULL == pseq) {

                PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME,
                          _T("pseq is NULL"));

                continue ;
            }

            holdvalue = pseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

            current_auto_scan_status = !((holdvalue & VFC_HOLD_PRE_SCAN) == VFC_HOLD_PRE_SCAN) ;

            if (current_auto_scan_status == auto_scan_on)
                continue ;

            if (auto_scan_on)
                holdvalue &= (~VFC_HOLD_PRE_SCAN);

            else
                holdvalue |= (VFC_HOLD_PRE_SCAN);

            pdata->wReserved1 = HOLD_VALUE;
            pdata->intVal = holdvalue;

            l_scan_index = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

            if (S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_scan_index)) {

                pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("HOLD"), holdvalue);

            } else {

                PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                          _T("Trace of UpdateGUIControlsInVarfieldDB"));
            }

            m_pqm_listctrl.InvalidateItemsForScanTime(selected_items[index]);
        }

        ChangeContentForModelessInfoDialogs();
        DEL_PTR(pdata);
    }
}

//+Patni-KSS/2010June25/ADDED/IR-156_2
//************************************Method Header************************************
// Method Name  : EnableDisableDeleteButton
// Author       : PATNI/KSS
// Purpose      : Check the Delete button is enable/disable
//***********************************************************************************
BOOL CPQMView::EnableDisableDeleteButton(    //MEITEC/2010Jul22/Modified/Change type from bool to BOOL
)const
{
    //bool flagDelete = false;
    //flagDelete = GetDlgItem(IDC_DELETE_BUTTON)->IsWindowEnabled();
    return m_delete_button.IsWindowEnabled();
}
//-Patni-KSS/2010June25/ADDED/IR-156_2

//+Patni-Hemant/2009Sept01/Added/MVC#004545
//************************************Method Header************************************
// Method Name  : ParentImageRemovedFromCurrentProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ParentImageRemovedFromCurrentProtocol(
)
{
    const int l_current_protocol_index = m_scan_index;

    if (IsValidSequenceIndex(l_current_protocol_index)) {

        CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_current_protocol_index);

        if (l_seq) {

            VARIANT* l_data = new VARIANT;
            l_data->wReserved1 = PLAN_STATUS;
            m_acquisition->GetControlValueFromDB(l_data, GetAcqOderForGivenProtocolIndex(l_current_protocol_index));

            l_seq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("PLANSTATUS"), l_data->intVal);

            if (((l_data->intVal & VFC_PLN_STATUS_GLOC) == VFC_PLN_STATUS_GLOC)) {
                l_seq->SetWSEEditingEnum(WSE_Planned);

            } else {
                l_seq->SetWSEEditingEnum(WSE_None);
            }

            m_pqm_listctrl.InvalidateItemsForPLANColumn(l_current_protocol_index);

            DEL_PTR(l_data)
        }
    }
}
//-Patni-Hemant/2009Sept01/Added/MVC#004545


//+ Patni-PJS/2009Jul29/Modified/MVC0003961
//************************************Method Header************************************
// Method Name  : UpdateCouchVal
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
void CPQMView::UpdateCouchVal(
)
{
    int selected_index = GetSelectedSeqIndex();

    if (selected_index != -1) {

        VARIANT* pVariant = new VARIANT;

        if (pVariant) {

            m_acquisition->GetCouchPos(pVariant);
            CSequence* pSeq = (CSequence*) m_pqm_listctrl.GetItemData(selected_index);

            if (pSeq) {

                pSeq->GetCPASCOMSequence()->GetSequenceParam()->SetFloat(_T("COUCH"), pVariant->fltVal);
                m_pqm_listctrl.InvalidateItemsForStatus(selected_index);
            }
        }

        DEL_PTR(pVariant);
    }

}
//- Patni-PJS/2009Jul29/Modified/MVC0003961

//+Patni-HEMANT/ADDED On 4/23/2009 10:55:10 AM/ ACE / ITC-2 Defect Fix
void CPQMView::SelectAndHighlightFirstWaitProtocol()
{
    m_pqm_listctrl.SelectOnlyItem(m_pqm_listctrl.GetFirstWaitProtocolIndex());
}
//-Patni-HEMANT/ADDED On 4/23/2009 10:55:10 AM/ ACE / ITC-2 Defect Fix


//+Patni-HEMANT/ADDED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
//************************************Method Header************************************
// Method Name  : UpdateTRSCFromGivenIndex
// Author       : PATNI/ HEMANT
// Purpose      : update trsc from given index for given trsc enum type
//***********************************************************************************
bool CPQMView::UpdateFullTRSCFromGivenIndex(
    const int f_list_index
)
{

    UpdateTRSCFromGivenIndex(f_list_index, CPQMView::TRSC_T);
    UpdateTRSCFromGivenIndex(f_list_index, CPQMView::TRSC_R);
    UpdateTRSCFromGivenIndex(f_list_index, CPQMView::TRSC_S);
    UpdateTRSCFromGivenIndex(f_list_index, CPQMView::TRSC_C);
    return true ;
}
//-Patni-HEMANT/ADDED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanStartAutoScan(
)
{
    //+NFD001-Autoshimming Auto scan not started problem
    const int first_wait_protocol_index = m_pqm_listctrl.GetFirstWaitProtocolIndex();
    const int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem() ;

    if (IsScanning() || !IsValidSequenceIndex(first_wait_protocol_index) || !IsValidSequenceIndex(first_selected_item)) {
        return false;
    }

    if (first_selected_item == first_wait_protocol_index) {


        //Note: If delay time dialog is open, do not start Auto Scan
        bool l_can_auto_scan =  true;

        if (GetCoilDlgStatus() || GetEditDelayDlgStatus()) {
            l_can_auto_scan = false;
        }

        if (!l_can_auto_scan) {

            //OnDialogClosedForParameterEditing(first_selected_item, false, false);
            RemoevAutoScanFromFirstWaitItem();
        }

        return l_can_auto_scan ;
    }

    //NFD001-Autoshimming Auto scan not started problem root cause is
    //CanStartAutoScan() default return is false because of that Auto scan is not started
    //to resolve this problem we have modified this method implementation so in error case only it will return false.
    //Patni-Himanshu/Mvc006364
    //-Patni-PJS/2011Mar07/Modified/TMSC-REDMINE-1529
    //return false;
    return true;

    //-NFD001-Autoshimming Auto scan not started problem
}

//************************************Method Header****************************
//Method Name: GetIndexForAutoScanProt()
//Author:
//Purpose:
//*****************************************************************************

int CPQMView::GetIndexForAutoScanProt(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetIndexForAutoScanProt");
    int l_firstwait_protocol = -1;
    GetIndexOfFirstWaitStateProtocol(l_firstwait_protocol);

    if (IsValidSequenceIndex(l_firstwait_protocol)) {

        if (!CanStartAutoScan() || GetMismatchErrorFlag()
            || !SetScanProtocolInfoFromGUI()) {
            l_firstwait_protocol = -1;
        }

    } else {
        l_firstwait_protocol = -1;
    }

    CString l_log_str = _T("");
    l_log_str.Format(_T("Auto_scan List Index = %d"), l_firstwait_protocol);
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_log_str);

    return l_firstwait_protocol;
}


//************************************Method Header****************************
//Method Name: SendNotifyToMemory()
//Author:  PATNI/SG
//Purpose: Invoke Memory in Store Mode
//*****************************************************************************
void CPQMView::SendNotifyToMemory(
)const
{

    CDPSManager::GetInstance()->SendASyncMsg(
        CPQMUtility::GetInstance()->GetPQMParentName() + CString(_T(".Memory")),
        _T("GL_Store"), _T(""));

}


//************************************Method Header**************************************
//Method Name:  HideSR
//Author:  PATNI/SG
//Purpose: sending message to DPS to hide  SR
//**************************************************************************************/
void CPQMView::HideSR()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::HideSR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPQMUtility::GetInstance()->SetPQMStatus(STATE_INACTIVE);

    CDPSManager::GetInstance()->SendASyncMsg(_T("Schedule"), DPS_MSGID_REQ_HIDEGL, _T("SRGL"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SRGL"));

}


//*****************************************************************************
// Function Name	: SwitchPage
// Author       	: PATNI\SG
// Purpose      	: Switch page from acquire to schedule and visa-versa
//*****************************************************************************
void CPQMView::SwitchPage(
    const CString& f_page
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SwitchPage");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("Trace") + f_page);

    CDPSManager::GetInstance()->SendASyncMsg(DPS_PAGER,
            DPS_MSGID_REQ_SWITCHPAGE, f_page);

}

//Patni-ARD/2010Feb18/Commented/Ph-3# PROPOSAL_06- MakePageSensitive commented


//+Patni-AMT/2010Feb05/Modified/Ph-3# PROPOSAL_04
//****************************Method Header************************************
//Method Name   : ShowPQM
//Author        : PATNI
//Purpose       : Show PQM on schedule Page
//*****************************************************************************
BOOL CPQMView::ShowPQM()
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ShowPQM");


    PQM_TRACE_SCOPED(FUNC_NAME);

    EnableWindow(TRUE);
    CPQMUtility::GetInstance()->SetPQMStatus(STATE_INACTIVE);// READY

    //+Patni-RSG/PJS/2009Sept2009/ADDED/PQM Hang
    g_MainView->GetPQMListCtrl().DeleteAllItems();
    g_MainView->GetPQMListCtrl().Invalidate(false);
    g_MainView->GetPQMListCtrl().UpdateWindow();
    g_MainView->CleanUp();
    //-Patni-RSG/PJS/2009Sept2009/ADDED/PQM Hang


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              DPS_MSGID_REQ_SHOWGL);

    return CDPSManager::GetInstance()->SendASyncMsgToParentPage(
               DPS_MSGID_REQ_SHOWGL, _T("PQM/PQM.Document"));
}
//-Patni-AMT/2010Feb05/Modified/Ph-3# PROPOSAL_04

/****************************Method Header************************************
//Module         : SendScanMessageToSR
//Author         : PATNI/SG
//Purpose        : Send GL_SR_ScanEnd/GL_SR_ScanStart notification message to SR
//*****************************************************************************/
//Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979
void CPQMView::SendScanMessageToSR(const CString& f_message, bool f_sync)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SendScanMessageToSR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979
    //send sync message
    if (f_sync) {

        CCFWReceiveMsg l_re_msg ;

        BOOL l_ret = CDPSManager::GetInstance()->SendSyncMsg(_T("Schedule.SRGL"),
                     f_message,  &l_re_msg , GetSRParamInfo(true));

        if (FALSE == l_ret) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Sending scan message to SR failed."));
            return;
        }


        CString l_parameter = l_re_msg.GetParameter();

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_parameter);

        //processing according to the type of message sent
        //for GL_SR_ScanStart
        if (f_message == DPS_MSGID_REQ_SCANSTART) {
            //update only pps info
            SetSRParam(l_parameter, _T(""), true);
        }


    } else {
        //send async message
        CDPSManager::GetInstance()->SendASyncMsg(_T("Schedule.SRGL"),
                f_message, GetSRParamInfo(true));
    }

    //-Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_message);

}

//****************************Method Header************************************
//Module         : NotifyReceivedMessage
//Author         : PATNI/GP
//Purpose        : Notify Received Message to notifier
//*****************************************************************************
void CPQMView::NotifyReceivedMessage(
    const pqm_msg_tag f_msg
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::NotifyReceivedMessage");
    PQM_TRACE_SCOPED(FUNC_NAME);

    SetLastAcqmanHandlderMsg(f_msg);

    //now change the ctrl state for given msg..
    UpdateStatesForCtrls(f_msg);
}


//****************************Method Header************************************
//Module         : ApplyReceiverCoilForNewProtocols
//Author         : PATNI/GP
//Purpose        : Apply Receiver Coil setting for new Protocols
//*****************************************************************************
void CPQMView::ApplyReceiverCoilForNewProtocols(
    const int last_index,
    const unsigned long recv_coil_id,
    const CString& recv_coil_label,
    const CString& str_trsc
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyReceiverCoilForNewProtocols");
    WCHAR buff[20] = {0};
    CString str_recv_coil = _itow(recv_coil_id, buff, 10);

    CSequence* pseq = NULL ;

    for (int i = last_index; i < m_pqm_listctrl.GetItemCount(); i++) {

        pseq = (CSequence*)m_pqm_listctrl.GetItemData(i);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("pseq is NULL"));
            continue;
        }

        pseq->SetValue(rc, str_recv_coil);

        CPqmPrMode& prmode = GetPrModeOfSelectedSequence(i);

        if (IsBatchSettingOn() || (prmode.m_recv_coil_label.IsEmpty())) {

            prmode.m_rcv_coil_id =  recv_coil_id;
            prmode.m_recv_coil_label = recv_coil_label;


            m_pqm_listctrl.DisplayTRSC(i, str_trsc);
        }

    }
}


//****************************Method Header************************************
//Module         : UpdateProtocolStatus
//Author         : PATNI/GP
//Purpose        : Update Protocol Status as per scanning input from IPC
//*****************************************************************************
void CPQMView::UpdateProtocolStatus(
    const int selected_index,
    const int status
)
{
    //+ Patni-MSN/2009May15/Added/JFT - Optimization

    CSequence* l_pseq = GetSelectedSequence(selected_index);

    if (!l_pseq)
        return;

    l_pseq->SetAcqStatus(status);
    //- Patni-MSN/2009May15/Added/JFT - Optimization

    //Added by Hemant On 7/24/2008 12:20:20 PM
    m_pqm_listctrl.InvalidateItemsForStatus(selected_index);
}


//****************************Method Header************************************
//Module         : ApplyPatientOrientationForNewProtocols
//Author         : PATNI/GP
//Purpose        : Apply Patient Orientation Settings for new protocols as per
//                 previous last index of list
//*****************************************************************************
void CPQMView::ApplyPatientOrientationForNewProtocols(
    const int last_index,
    CPqmPrMode prMode
)
{

    for (int i = last_index + 1; i < m_pqm_listctrl.GetItemCount(); i++) {

        CPqmPrMode& lprmode = GetPrModeOfSelectedSequence(i);

        lprmode.m_pat_ori.end_in = prMode.m_pat_ori.end_in;
        lprmode.m_pat_ori.side_up = prMode.m_pat_ori.side_up;
        lprmode.m_view_dir = prMode.m_view_dir;


    }

}


//****************************Method Header************************************
//Module         : ApplyAnatomyForNewProtocols
//Author         : PATNI/GP
//Purpose        : Apply Anatomy settings for newly added Protocols
//*****************************************************************************
void CPQMView::ApplyAnatomyForNewProtocols(
    const int last_index,
    const int iAnatomy,
    const CString& str_trsc
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyAnatomyForNewProtocols");
    CString stranatomy(_T(""));
    WCHAR buff[20] = {0};	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
    stranatomy = _itow(iAnatomy, buff, 10);

    //Added by Hemant On 1/16/2009 3:56:09 PM
    CPqmPrMode prmode_last_protocol = GetPrModeOfSelectedSequence(last_index);


    bool l_same_body = true;
    bool l_scan_anatomy_present = false;

    for (int i = last_index + 1; i < m_pqm_listctrl.GetItemCount(); i++) {

        CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(i);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("pseq is NULL"));
            continue;
        }

        pseq->SetValue(sa, stranatomy);

        CPqmPrMode& prmode = GetPrModeOfSelectedSequence(i);

        //+Patni-Hemant/2010Apr30/Added/JaFT/MVC007367
        l_scan_anatomy_present = !(prmode.m_str_scan_anatomy.m_scan_anatomy_vf < 0
                                   || prmode.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty()) ;
        //-Patni-Hemant/2010Apr30/Added/JaFT/MVC007367

        l_same_body = true;

        if (IsBatchSettingOn() ||
            !((prmode.m_anatomy >= SAR_REGION(ABDOMEN))
              && (prmode.m_anatomy < SAR_REGIONS))) {

            ApplyDefaultScanAnatomyFor(i, true);

            prmode.m_anatomy =  iAnatomy;


            m_pqm_listctrl.DisplayTRSC(i, str_trsc);

            //Added by Hemant On 7/21/2008 11:18:10 AM
            m_pqm_listctrl.InvalidateItemsForSarRegion(i);

            //+Patni-Hemant/2010June14/Added/V1.20#MVC008302
            l_same_body = (prmode.m_str_scan_anatomy.m_scan_anatomy_name.CompareNoCase(
                               prmode_last_protocol.m_str_scan_anatomy.m_scan_anatomy_name) == 0) ;
            //-Patni-Hemant/2010June14/Added/V1.20#MVC008302

            //this protocol according to prev scan anatomy
            prmode.m_str_scan_anatomy = prmode_last_protocol.m_str_scan_anatomy ;
        }

        //+Patni-Hemant/2010Apr30/Added/JaFT/MVC007367
        //Patni-Hemant/2010June14/Added/V1.20#MVC008302
        if (!l_scan_anatomy_present || !l_same_body) {

            if (!IsBatchSettingOn()) {
                ApplyDefaultScanAnatomyFor(i, true);
            }

            ApplyScanAnatomy(&i, 1, -1, prmode.m_str_scan_anatomy.GetLaterality(), false, &prmode.m_str_scan_anatomy);
        }

        //-Patni-Hemant/2010Apr30/Added/JaFT/MVC007367



    }
}

//****************************Method Header************************************

//Module         : SendLOID()

//Author         : PATNI/JS

//Purpose        : Send study LOID to IPC through COM

//*****************************************************************************
//Patni-HAR/2010Apr21/Modified/MVC007534
//Patni-Sribanta/2010Nov10/Modified/IR-170
int CPQMView::SendLOID(BSTR f_strpatientloid, BSTR f_strstudyloid, BSTR f_pas_path) //Patni-MN/2009Mar03/Modified/PSP1#138/Changed the function signature
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SendLOID");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              (_T("LOID = ") + (CString)f_strstudyloid));

    m_sr_invoked = true;
    EnableDisableStartupButtons(m_sr_invoked);

    //+REDMINE-781  //No need...
    /*
    CString rrinterval = _T("RR Interval : ");
    rrinterval += _T("");

    CString heartbeat = _T("  Heart Beat : ");
    heartbeat += _T("");

    rrinterval += heartbeat;

    PQM_TRACE(SYSTEM_FUNC_MARKER,
              FUNC_NAME,
              rrinterval);
    */
    //-REDMINE-781


    //Patni-Sribanta/2010Nov10/Modified/IR-170
    HRESULT l_hr = m_acquisition->ReceiveLOID(f_strpatientloid, f_strstudyloid, f_pas_path);

    //Patni-HAR/2010Apr21/Modified/MVC007534
    if (R_TRUE == l_hr) {

        //when same loid is not opened, reset the flag for CFA
        SetShimDoneFlag(CFA_DLG_NORMAL);

        //-Patni-HEMANT/ADDED On 3/19/2009 6:04:22 PM/ PSP1/ internal defect
        //Patni-DKH/2010Jan15/Added/IR-87,Plan Duplicator
        //Patni-AMT/2010Apr22/Modified/JaFT# IR87_CDR_13-Code Review
        if (S_FALSE == m_acquisition->InitPlanDuplicateSettings()) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Plan Duplicate initialization failed."));
        }

        //Patni-DKH/2010Feb01/Added/PH#3#/FBI
        m_studyloid = f_strstudyloid ;


        m_acquisition->SendSAEInfo();

        return TRUE;

    } else {

        //Patni-HAR/2010Apr21/Modified/MVC007534
        return l_hr;
    }

    return TRUE;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::EnableDisableScanRelatedButtonsForSelectedProtocolStatus(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::EnableDisableScanRelatedButtonsForSelectedProtocolStatus");
    PQM_TRACE_SCOPED(FUNC_NAME);

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    int* selected_items = NULL;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected);

    if ((0 == count) || (NULL == selected_items)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("count is ZERO or selected_items is NULL"));
    }

    bool enable_scan_buttons = (count == 1) ? true : false;

    LPCTSTR l_strEmpty = _T("---");

    if (IsScanning() || iscurrent_selected || isdone_selected || isfail_selected) {
        enable_scan_buttons = false ;

        //+Patni-Manish/2009June08/Added/JFT#250/Enable COIL Button
        if (isdone_selected || isfail_selected || iscurrent_selected)
            m_coil_button.EnableWindow(false);

        else if (IsAcqumanReadyForNextRequest() && IsScanning()) {
            //+Patni-Manish/2009July16/Added/JFT#GUIRenewal-Fill Color on COIL Button
            m_coil_button.EnableWindow(IsAMBInvoked() ? false : true);

            if (m_strCoilSelect.Compare(l_strEmpty) == 0 || m_strCoilSelect.IsEmpty()) {

                if (!ReturnCoilOfPreviousSeq())
                    //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
                    //SetColorButton(&m_coil_button, RGB(245,195,75));
                    ApplyWarningFont(&m_coil_button);

                //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

            } else {
                BUTTON_COLORS_t stColors_o2;
                m_end_exam_button.GetButtonColors(stColors_o2);
                m_coil_button.SetButtonColors(stColors_o2);
            }

            //-Patni-Manish/2009July16/Added/JFT#GUIRenewal-Fill Color on COIL Button

        } else { //+Patni-Manish/2009Jul6/Added/IDS#650-Enable COIL button on schedule page
            if (m_pqm_g.GetPQMMode() == K_IMAGING) {
                m_coil_button.EnableWindow(false);

            } else {  //For Schedule Page
                m_coil_button.EnableWindow(true);

                if (m_strCoilSelect.Compare(l_strEmpty) == 0 || m_strCoilSelect.IsEmpty()) {
                    if (!ReturnCoilOfPreviousSeq())
                        //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
                        //SetColorButton(&m_coil_button, RGB(245,195,75));
                        ApplyWarningFont(&m_coil_button);

                    //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

                } else {
                    BUTTON_COLORS_t stColors_o2;
                    m_end_exam_button.GetButtonColors(stColors_o2);
                    m_coil_button.SetButtonColors(stColors_o2);
                }
            }
        }  //-Patni-Manish/2009Jul6/Added/IDS#650-Enable COIL button on schedule page

    } else {
        if (IsAcqumanReadyForNextRequest() && count != -1) {
            //+Patni-Manish/2009July01/Added/JFT#GUIRenewal-Fill Color on COIL Button
            if (IsNextProtocolAutoScanAfterStatusDone() && (-1 == CheckAndGetWaitingForAutoScanStartAcqOrder())) {
                enable_scan_buttons = false ;
                m_coil_button.EnableWindow(false);

            } else {
                m_coil_button.EnableWindow(IsAMBInvoked() ? false : true);
            }

            if (m_strCoilSelect.Compare(l_strEmpty) == 0 || m_strCoilSelect.IsEmpty()) {

                if (!ReturnCoilOfPreviousSeq())
                    //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
                    //SetColorButton(&m_coil_button, RGB(245,195,75));
                    ApplyWarningFont(&m_coil_button);

                //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

            } else {

                BUTTON_COLORS_t stColors_o2;
                m_end_exam_button.GetButtonColors(stColors_o2);
                m_coil_button.SetButtonColors(stColors_o2);
            }

            //-Patni-Manish/2009July16/Added/JFT#GUIRenewal-Fill Color on COIL Button

        } else {  //+Patni-Manish/2009Jul16/Added/IDS#650-Enable COIL button on schedule page
            if (m_pqm_g.GetPQMMode() == K_IMAGING) {
                m_coil_button.EnableWindow(false);

            } else if (count != -1) {  //For schedule page
                m_coil_button.EnableWindow(true);

                if (m_strCoilSelect.Compare(l_strEmpty) == 0 || m_strCoilSelect.IsEmpty()) {
                    if (!ReturnCoilOfPreviousSeq())
                        //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
                        //SetColorButton(&m_coil_button, RGB(245,195,75));
                        ApplyWarningFont(&m_coil_button);

                    //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

                } else {

                    BUTTON_COLORS_t stColors_o2;
                    m_end_exam_button.GetButtonColors(stColors_o2);
                    m_coil_button.SetButtonColors(stColors_o2);
                }
            }
        }  //-Patni-Manish/2009Jul6/Added/IDS#650-Enable COIL button on schedule page

        if (count == 0) {
            m_coil_button.EnableWindow(false);
        }

    }

    if (enable_scan_buttons && (count == 1)) {

        //check the status of selected item..
        if (m_pqm_listctrl.GetFirstWaitProtocolIndex() != selected_items[0]) {
            enable_scan_buttons = false ;
        }
    }

    //added by Hemant - Dynamic scan
    enable_scan_buttons = m_dynamic_scan_on | enable_scan_buttons && !GetAMBPlanScanStartCheck();

    //Added by Hemant On 12/5/2008 6:41:00 PM
    //now when first wait is selected, and coil window is opened
    //you can not scan start..
    if (iswait_selected && GetCoilDlgStatus()) {
        enable_scan_buttons = false ;
    }

    if (m_rmc_scan_running) {
        enable_scan_buttons = false ;
    }


    if (m_is_wse_open && (!m_dynamic_scan_on)) {

        EnableDisableScanRelatedButtons(enable_scan_buttons &&
                                        (m_pqm_listctrl.GetWSEEditingProtocolIndex() != m_pqm_listctrl.GetFirstWaitProtocolIndex())
                                       );

    } else {

        EnableDisableScanRelatedButtons(enable_scan_buttons);
    }

    //for delete button...
    BOOL enable_window = (BOOL)(((count > 0) && !(iscurrent_selected || isdone_selected || isfail_selected))) ;

    //consider the editing of coil..
    if (dlgCoilSelWin && :: IsWindow(dlgCoilSelWin->m_hWnd)) {
        enable_window = enable_window & (!dlgCoilSelWin->IsWindowVisible()) ;
    }

    BOOL l_bflag = FALSE;

    if (count == 1) {
        if (selected_items[0] == m_pqm_listctrl.GetWSEEditingProtocolIndex()) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selection Count = 0. WSE Editing Protocol is also selected."));
            l_bflag = TRUE;
        }

    } else {
        for (int l_nindex = 0; l_nindex < count; l_nindex++) {

            if (selected_items[l_nindex] == m_pqm_listctrl.GetWSEEditingProtocolIndex()) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selection Count > 0. WSE Editing Protocol is also selected."));

                l_bflag = TRUE;
                break;
            }
        }
    }

    const bool l_is_gui_processing = CanPerformGUIOperation() && (!GetIsAMBPlanning() && !IsAMBInvoked());

    enable_window = enable_window & (BOOL) l_is_gui_processing ;


    if (!l_bflag) {

        if (m_delete_button.IsWindowEnabled() != enable_window) {

            SetIconForButton(m_delete_button, enable_window ? IDI_DELETE_BUTTON_IMAGE : -1);
            m_delete_button.EnableWindow(enable_window);
        }

    } else {

        SetIconForButton(m_delete_button, -1);
        m_delete_button.EnableWindow(false);
    }

    enable_window = (bool)((count > 0)) & l_is_gui_processing ;

    if (m_duplicate_button.IsWindowEnabled() != enable_window) {

        SetIconForButton(m_duplicate_button, enable_window ? IDI_ADD_BUTTON_IMAGE : -1);
        m_duplicate_button.EnableWindow(enable_window);

    }

    DEL_PTR_ARY(selected_items);

}

//************************************Method Header************************************
// Method Name  : EnableDisableLocatorButton()
// Author       : PATNI/ ASHISH
// Purpose      :
//***********************************************************************************

void CPQMView::EnableDisableLocatorButton(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::EnableDisableLocatorButton");

    if (NULL == GetDlgItem(IDC_LOCATOR_BUTTON)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetDlgItem(IDC_LOCATOR_BUTTON) pointer is NULL"));
        return ;
    }

    m_locator_button.EnableWindow(false);
}


//************************************Method Header************************************
// Method Name  : CanModifyWithCommentDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanModifyWithCommentDialog(
)
{
    const int sel_index = GetSelectedSeqIndex();

    if (IsValidSequenceIndex(sel_index)) {
        return (sel_index != GetScanIndex());
    }

    return false;
}

//************************************Method Header************************************
// Method Name  : InsertNewSequencesOnGUI
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::InsertNewSequencesOnGUI(
    const bool f_calculatecoil_data /*= false*/,
    bool f_select_item /*= false*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::InsertNewSequencesOnGUI");

    if (m_list) {

        const int total_seq_displayed = m_pqm_listctrl.GetItemCount();
        const int total_seq_present = m_list->GetCount();

        if (total_seq_present < total_seq_displayed) {
            return ;
        }

        CString recv_coil_label = _T("");
        POSITION pos = NULL ;
        CSequence* pseq = NULL ;
        bool l_select_this_item = f_select_item ;

        for (int seq_index = (total_seq_displayed) ; seq_index < total_seq_present ; seq_index ++) {

            pos = m_list->FindIndex(seq_index);

            //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
            if (NULL == pos) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Cannot find the sequence in the list"));

                continue;
            }

            //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

            pseq = m_list->GetAt(pos);

            if (pseq) {

                //Patni-Hemant/2010Jun01/Added/MaFT/TMSC-REDMINE-MVC007915/Bug Fix
                ApplyConfigSettingForProt(pseq);

                //+Patni-HEMANT/ADDED On 4/26/2009 PM/ PSP/ Internal defect
                l_select_this_item = f_select_item ;

                if (l_select_this_item) {

                    l_select_this_item = ((pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT) &&
                                          (seq_index >= total_seq_displayed)) ;
                    f_select_item = !l_select_this_item ;
                }

                m_pqm_listctrl.InsertProtocolAt(pseq, -1, l_select_this_item);
                //+Patni-HEMANT/ADDED On 4/26/2009 PM/ PSP/ Internal defect

                recv_coil_label = GetCoilStringFor(pseq);
                pseq->SetValue(rc, recv_coil_label);
                CPqmPrMode& prmode = GetPrModeOfSelectedSequence(seq_index);
                prmode.m_rcv_coil_id = __recv_coil_id;
                prmode.m_recv_coil_label = recv_coil_label;

                //+Patni-Hemant/2010Jun01/Added/MaFT/TMSC-REDMINE-MVC007915/Bug Fix
                if (pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT) {
                    ValdiateBodyPartData(pseq, seq_index);
                }

                //Patni-Hemant/2010Jun15/Added/V1.20/MVC8201/Contrast Agent
                ValidateContrastAgent(pseq) ;

                //-Patni-Hemant/2010Jun01/Added/MaFT/TMSC-REDMINE-MVC007915/Bug Fix

                if (m_ismemoryalive && WSE_Planned != pseq->GetWSEEditingEnum()) {
                    UpdateScanOffsetInRange(seq_index);
                }

                if (m_use_coil_channel_mode && ((pseq->GetAcqStatus() != PR_ACQ_STATUS_SUCCESS) && (pseq->GetAcqStatus() != PR_ACQ_STATUS_FAIL))) {
                    if (CheckCoilForcibleMaxMode(seq_index)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Forcible Max mode is TRUE"));
                        int l_mode = 1;
                        int l_acq_order = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

                        if (S_FALSE == m_acquisition->SetChannelModeSelection(l_acq_order, l_mode, l_mode ? m_system_enable_channel : m_limited_channel)) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selected channel mode is not set"));
                        }

                    } else {
                        if (!f_calculatecoil_data)
                            SetDefaultChannelMode(pseq, seq_index);
                    }
                }
            }
        }

        m_acquisition->CalcSARControlAndPutVFForAll();
        RemoevAutoScanFromFirstWaitItem(total_seq_displayed);

        //update the total remaining time..
        DisplayTotalRemainingTimeOnPQM();

        //+Patni-HEMANT/ADDED On 5/8/2009 6:04:06 PM/ JFT/ Internal Defect Fix for JFT
        UpdateOrientationCombosOnSelectionChanged(total_seq_displayed, false);
    }
}

//************************************Method Header**************************************

//Method Name:  CleanUp

//Author:  PATNI/SVP

//Purpose: Delete protocols from list

//**************************************************************************************/
void CPQMView::CleanUp(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CleanUp");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_list) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    UTIL::DeleteAndClearMFCList<CSequence>(m_list);


}

//************************************Method Header**************************************

//Method Name:  DisplayTotalRemainingTimeOnPQM

//Author:  PATNI/SVP

//Purpose: Displays total time of remaining protocols on PQM window.

//**************************************************************************************/
void CPQMView::DisplayTotalRemainingTimeOnPQM(
)
{
    //Note : by Hemant
    //This function is currently called from CPQMListCtrl::DeleteSelectedProtocols(),
    //and after calling InsertProtocolAt() from whereever its being called..
    //and also from protocol status changed..( PQM_MSG_STATUS_CURRENT, PQM_MSG_STATUS_DONE,PQM_MSG_ABORT_WAIT , PQM_MSG_ABORT_FAIL

    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayTotalRemainingTimeOnPQM");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_list) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));
        return ;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    POSITION pos = m_list->GetHeadPosition();

    CSequence* pseq = NULL ;
    time_t total_time = 0;

    while (pos) {
        pseq = m_list->GetNext(pos);

        if (pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT) {

            total_time += _wtoi(pseq->GetCPASCOMSequence()->
                                GetSequenceParam()->GetString(_T("SCANTIME")));
        }
    }

    unsigned long sec = total_time;
    unsigned long hr  = sec / 3600;
    sec = sec % 3600;
    unsigned long min = sec / 60;
    sec = sec % 60;

    CString str_total_scan_time;

    if (hr > 0) {
        str_total_scan_time.Format(_T("%2d:%02d:%02d"), hr, min, sec);

    } else {
        str_total_scan_time.Format(_T("%02d:%02d"), min, sec);
    }

    //Patni-Hemant/2009Nov18/Modified/IR#90/Bottom Buttons
    m_remaining_time_static.SetWindowText(str_total_scan_time);
}


//************************************Method Header**************************************
//Method Name:  InvokeMemory
//Author:  PATNI/HR
//Purpose: Invoke Memory Application
//**************************************************************************************/
BOOL CPQMView::InvokeMemory(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::InvokeMemory");
    PQM_TRACE_SCOPED(FUNC_NAME);

    try {
        //Added by Hemant On 2/25/2009 11:48:51 AM, , defect PSP#180
        //Patni-HAR/2010Jun28/Commented/V1.20#MVC008324_0
        //SetWaitCursor(true, false);

        //Added by Hemant On 2/27/2009 7:30:30 PM, , defect PSP#180

        if (m_is_end_exam_clicked) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("End exam button is clicked."));
            return FALSE;
        }

        if (m_is_wse_open) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Locator or WSE button is clicked."));
            return FALSE;
        }

        m_memory_invokation_flag |= MEMORY_INVOKATION_STARTS;

        //Patni-ARD/2010Mar24/Modified/DeFT# IR-100 remove IPCConfig.ini
        BOOL l_ret = CDPSManager::GetInstance()->SendASyncMsgToParentPage(
                         _T("FW_LoadGL"), _T("Memory/MemoryGUI/972/649/965/500"),
                         _T("1/0"));

        //Patni-Hemant/2011Mar30/Added/REDMINE-1575
        OnSelectionStatusChanged();

        SetFlagRelatedCtrlsStates();

        return l_ret;

    } catch (...) {


        //TMSC/2009Nov12/Modified/Code Changes
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Exception occured"));

        throw;

        //TMSC/2009Nov12/Modified/Code Changes
        return FALSE;
    }


}

//****************************Method Header************************************
//Module         : ShowSR
//Author         : PATNI/MN
//Purpose        : To show SR Application
//*****************************************************************************
void CPQMView::ShowSR()const
{
    CDPSManager::GetInstance()->SendASyncMsg(_T("Schedule"), DPS_MSGID_REQ_SHOWGL, _T("SRGL/SRGL"));
}


void CPQMView::OnCFAOperationClick(const bool f_cfa_retry /*= true*/)
{
    m_pqmchildthread.PostMessageToThread(ON_CFA_OPERATION, (WPARAM) f_cfa_retry , NULL);
}

void CPQMView::DoCFAOperation(
    const bool f_cfa_retry /*= true*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::DoCFARetry");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VARIANT data;
    data.byref = NULL ;

    if (S_OK != m_acquisition->GetScanMutex(&data)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));
        return;
    }

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* l_scan_mutex = (MR::ACQ::SM::COMMON::MUTEX::CThreadMutex*) data.byref;
    data.byref = NULL ;

    CScopedLock l_scoped_lock(l_scan_mutex);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

    if (m_cfadlg) {
        if (f_cfa_retry) {
            m_cfadlg->DoCFARetry();

        } else {
            m_cfadlg->DoCFAApply();
        }
    }
}



//****************************Method Header************************************
//Module         : StartCFAPrescan

//Author         : PATNI/MSN

//Purpose        : Starts CFA prescan
//*****************************************************************************
void CPQMView::StartCFAPrescan()
{
    //Patni-PJS/2010Aug10/Added/TMSC-REDMINE-557
    LPCTSTR FUNC_NAME = _T("CPQMView::StartCFAPrescan");

    SetIsScanning(true);
    const int index = GetPQMListCtrl().GetFirstSelectedItem();
    SetScanIndex(index);

    TCHAR l_scanindex[NUM_100] = {0};//Patni-DKH/2009Aug24/Modified/cpp test corrections
    wsprintf(l_scanindex, _T("%d"), m_scan_index);

    //Patni-PJS/2010Aug10/Modified/TMSC-REDMINE-557
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_scanindex);

    m_acquisition->SetReceiverCoilID(GetPrModeOfSelectedSequence(index).m_rcv_coil_id);

    //+Patni-PJS/2010Aug10/Modified/TMSC-REDMINE-557
    if (S_FALSE == m_acquisition->StartCFAPrescan(m_scan_index)) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("CFA Prescan failed"));
        SetScanIndex(-1);
        SetIsScanning(false);
        SetFlagRelatedCtrlsStates();
    }

    //-Patni-PJS/2010Aug10/Modified/TMSC-REDMINE-557
}

//****************************Method Header************************************
//Module         : DisplayAPCStartLabel

//Author         : PATNI/MSN

//Purpose        : Displays APC Start or Scan Start label according to apcflag
//*****************************************************************************
void CPQMView::DisplayAPCStartLabel(const bool apcflag)
{


    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayAPCStartLabel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //-------------------------------------------------------------------------
    //Modified by Hemant On 1/23/2009 11:24:07 AM
    ////Modified by Hemant
    //if(apcflag == true) {
    if (apcflag && GetAPCPauseChecked()) {
        m_scan_start_button.SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_APC_START")));
        m_dynamic_scan_on = true ;
        //m_scan_start_button.EnableWindow(true);
        //    m_apc_start_flag = true;

    } else {
        m_scan_start_button.SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SCAN_START")));
        //m_scan_start_button.EnableWindow(false);
        //    m_apc_start_flag = false;
    }

    m_scan_start_button.EnableWindow(apcflag);

    //Added by Hemant On 1/23/2009 11:10:35 AM
    //after selecting APC Pause from menu and once we click on scan start, the status should be as it is for
    //APC pause, but after scan start, GUI should uncheck GUI side APC Pause flag, though its set at IPC side.
    //this behavior is therein vantage...

    //Conside vantage...
    //1) check APC Pause,it will prompt APC Start to user, and menu item for APC Pause will be disabled and unchecked
    //2) Now press ABORT.
    //3) Again press Scan start,
    //4) now as APC Pause Menuitem is unchecheked, user will be promt for Scan Start not APC Start
    //5) If you again abort , click on APC Pause of Menu, and scan start, it will give you APC Start ..
    // SetAPCPauseChecked(false); //Patni-Rajeesh/2010Dec10/Commented/REDMINE-1095
    //-------------------------------------------------------------------------
}

//****************************Method Header************************************
//Module         : ToggleAPCPauseMenu
//Author         : PATNI/MSN
//Purpose        : Toggles the APC Pause Menu
//*****************************************************************************
void CPQMView::ToggleAPCPauseMenu()
{

    OnClickMenuitemApcPause();
}


//************************************Method Header**************************************
//Method Name:  InvokeRefProbe
//Author:  PATNI/MSN
//Purpose: Invoke Ref Probe Application
//**************************************************************************************/
BOOL CPQMView::InvokeRefProbe(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::InvokeRefProbe");

    try {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

        return CDPSManager::GetInstance()->SendASyncMsgToParentPage(_T("FW_LoadGL"),
                _T("RefProbe/RefProbe.Document/300/300/700/500"));

    } catch (...) {


        //TMSC/2009Nov12/Modified/Code Changes
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Exception occured"));
        throw;
        //TMSC/2009Nov12/Modified/Code Changes
        return FALSE;
    }


}

//************************************Method Header************************************
// Method Name  : OnClickButtonBreathHold
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonBreathHold(
    const int row_index
)
{
    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    LPCTSTR FUNC_NAME(_T("CPQMView::OnClickButtonBreathHold"));

    if (!m_iswait_cursor) {

        int* selected_items = NULL ;
        int size = 0 ;

        if (m_pqm_listctrl.IsRowSelected(m_pqm_listctrl.m_hWnd, row_index))			//if given row is selected, then do for multiple selection..
            size = m_pqm_listctrl.GetselectedItems(&selected_items);

        else {
            //else onlt apply to given item..
            size = 1 ;
            selected_items = new int [size];
            selected_items[0] = row_index;
        }

        //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
        if (NULL == selected_items) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("selected_items pointer is NULL"));

            return ;
        }

        //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

        if (!CanCheckUnCheckFor(selected_items, size)) {

            //+Patni-HEMANT/ADDED On 8/14/2009 12:36:24 PM/ Code Review
            /*
            if (selected_items) {
                delete []selected_items;
                selected_items = NULL ;
            }
            */
            DEL_PTR_ARY(selected_items);
            //-Patni-HEMANT/ADDED On 8/14/2009 12:36:24 PM/ Code Review

            return ;
        }

        //Added by Hemant On 12/19/2008 9:22:29 PM
        //if we have clicked dynamically On seq, then don't proceed.
        {

            CSequence* seq_clicked = (CSequence*) m_pqm_listctrl.GetItemData(row_index);

            if (seq_clicked) {

                if (!seq_clicked->CanChangeModeOptionBreathHold()) {

                    DEL_PTR_ARY(selected_items);
                    return ;
                }
            }
        }

        if (size != 0) {

            VARIANT* pdata = new VARIANT;

            CSequence* seq_clicked = (CSequence*) m_pqm_listctrl.GetItemData(row_index);
            int holdvalue = seq_clicked->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

            bool on_off_flag = ((holdvalue & VFC_HOLD_COVERAGE) == VFC_HOLD_COVERAGE) ? true : false ;

            SetWaitCursor(true);

            POSITION pos = NULL;
            CSequence* pseq = NULL ;
            int l_scan_index = -1;

            for (int index = 0; index < size; index++) {

                pseq = (CSequence*) m_pqm_listctrl.GetItemData(selected_items[index]);

                if (NULL == pseq) {

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("pseq is NULL"));

                    continue;
                }

                holdvalue = pseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

                //+Patni-HAR/2010Feb17/Added/MSA0251-00166
                if (TRUE == pseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("DYN"))) {
                    continue;
                }

                //-Patni-HAR/2010Feb17/Added/MSA0251-00166

                // if flag is same as dominant item, then we need to togle it, else its already toggled..
                if (((holdvalue & VFC_HOLD_COVERAGE) == VFC_HOLD_COVERAGE) != on_off_flag)
                    continue ;

                if (on_off_flag) {
                    holdvalue &= (~VFC_HOLD_COVERAGE);

                } else {
                    holdvalue |= (VFC_HOLD_COVERAGE);
                }

                //Added by Hemant On 12/19/2008 9:23:54 PM , do not perform for dynamically On seq
                if (!seq_clicked->CanChangeModeOptionBreathHold())
                    continue ;

                pdata->wReserved1 = HOLD_VALUE;
                pdata->intVal = holdvalue;

                l_scan_index = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

                if (S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_scan_index)) {

                    pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("HOLD"), holdvalue);

                } else {

                    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                              _T("Trace of UpdateGUIControlsInVarfieldDB"));

                }

                m_pqm_listctrl.InvalidateItemsForModeOption(selected_items[index], BREATH_HOLD_OPTION);
            }

            SetWaitCursor(false);
            ChangeContentForModelessInfoDialogs();

            DEL_PTR(pdata);
            DEL_PTR_ARY(selected_items);
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM is Busy !!!"));
    }

}


//**************************************************************************************/
void CPQMView::OnDblclkListParameter(
    const int row_index
)
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(row_index);

    //Added by Hemant On 7/23/2008 4:26:29 PM
    //+ Patni-PJS/2009Jun18/Modify/IDS#607
    if (!m_is_wse_open && m_scanedit_button.IsWindowEnabled()) {

        //+Patni-Hemant/2009Nov11/Modified/IR#90 / RSNA
        if (m_locator_button.IsWindowEnabled() && (K_IMAGING == m_pqm_g.GetPQMMode())) { // Redmine-2365
            OnClickButtonScanPlan();

        } else {
            OnClickButtonSequenceEditior();
        }

        //-Patni-Hemant/2009Nov11/Modified/IR#90 / RSNA
    }

    //- Patni-PJS/2009Jun18/Modify/IDS#607
}

//************************************Method Header************************************
// Method Name  : OnClickButtonSarRegion
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonSarRegion(
    const int row_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonSarRegion");
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_ANATOMY_SELECTION);

    if (!m_iswait_cursor) {

        if (!CanChangeSar())
            return ;

        if (row_index != -1) {

            //REDMINE-781
            const CPqmPrMode& prmode = GetPrModeOfSelectedSequence(row_index);

            m_anatomy_dlg = new CPQMAnatomyDlg(m_window_rect, prmode.m_anatomy, this);

            m_anatomy_dlg->SetVariablesForParameterEditing(row_index, OnDialogOpenedForParameterEditing(row_index));

            ChangeViewOfModelessDialogs(false);

            const int return_value = m_anatomy_dlg->DoModal();

            ChangeViewOfModelessDialogs(true);

            //action after closing dialog..
            if (return_value == IDOK) {

                //Patni-HEMANT/MODIFIED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
                UpdateTRSCFromGivenIndex(row_index, CPQMView::TRSC_S);
            }

            ChangeContentForModelessInfoDialogs();

            DEL_PTR(m_anatomy_dlg)

        } else {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("PQM is Busy !!!"));
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnToggleAutoVoice(
    const int row_index
)
{
    if (!IsValidSequenceIndex(row_index)) {
        return;
    }

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(row_index);

    UINT auto_voice_menuitem_id = ID_MENUITEM_N_N;

    if (NONE_NONE == l_seq->GetAutoVoiceOption()) {
        auto_voice_menuitem_id = (NONE_NONE == l_seq->GetPrevAutoVoiceOption()) ? ID_MENUITEM_N_B1 : GetMenuIdFromAutoVoiceOption(l_seq->GetPrevAutoVoiceOption()) ;

    } else {
        auto_voice_menuitem_id = ID_MENUITEM_N_N ;
    }

    ApplyAutoVoice(row_index, auto_voice_menuitem_id);
}


//************************************Method Header************************************
// Method Name  : OnMessageFromAcqmanMsgHandler
// Author       : PATNI/ HEMANT
// Purpose      : this function is to be called from AcqmanMsgHandler after procssing..
//***********************************************************************************
void CPQMView::OnMessageFromAcqmanMsgHandler(
    const pqm_msg_tag Msg
)
{
    //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    LPCTSTR FUNC_NAME = _T("CPQMView::OnMessageFromAcqmanMsgHandler");
    PQM_TRACE_SCOPED(FUNC_NAME);
    bool set_pause_state = true ;

    bool is_scan_related_msg = false ;		//set this to true when the message is for pause resume related updation..

    switch (Msg) {
            // + SM4 Himanshu CORRECT_COIL 15 Jan 2009
        case PQM_MSG_CORRECT_COIL:
            //+Patni-Sribanta/2011Feb09/Modified/REDMINE-1026
            //OnClickCoilPasButton();
            PostMessage(WM_MSG_CORRECT_COIL, 0, 0);
            //-Patni-Sribanta/2011Feb09/Modified/REDMINE-1026
            break;
            // - SM4 Himanshu CORRECT_COIL 15 Jan 2009

        case PQM_MSG_UPDATE_WAIT_FOR_AUTO_SCAN_ICON:

            UpdateCFARetry();
            m_amb_controller.UpdateDelayTimeState();
            UpdateOKBtnStatusofWirelssDlg();
            break;

        case PQM_MSG_SCAN_PAUSE:
            m_current_scan_opration = CurrentScanOpration_ScanPause;
            is_scan_related_msg = true;
            set_pause_state = false;
            break;

        case PQM_MSG_SCAN_RESUME:
            m_current_scan_opration = CurrentScanOpration_ScanResume;
            is_scan_related_msg = true;

            break;

        case PQM_MSG_ACQMAN_DISCONNECTED:
            m_acquman_status = NOT_CONNECTED ;
            m_acquman_ready_for_next_request = 	ACQMAN_NOT_READY_FOR_NEXT_REQ;


            EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
            UpdateCFARetry();

            if (dlgCoilSelWin) {
                dlgCoilSelWin->EnableDisableDialogState(TRUE);
            }

            UpdateOKBtnStatusofWirelssDlg();

            if (m_amb_controller.IsAMBInvoked()) {
                m_amb_controller.EnabledisableAMBScanRelatedButtons(IsAcqumanReadyForNextRequest());
            }

            SetWaitCursor(false, false);

            break;

        case PQM_MSG_ACQINFO_ERROR:

            if (dlgCoilSelWin) {
                dlgCoilSelWin->EnableDisableDialogState(TRUE);
            }

            break;

        case PQM_MSG_ACQMAN_CONNECTED:

            m_acquman_status = CONNECTED ;

            if (m_acquman_ready_for_next_request == ACQMAN_NOT_READY_FOR_NEXT_REQ) {
                if (GetPQMGlobal().GetPQMMode() == K_IMAGING) {
                    SendStatusReqToAcqman();
                }
            }

            EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
            UpdateCFARetry();
            UpdateOKBtnStatusofWirelssDlg();

            if (m_amb_controller.IsAMBInvoked()) {
                m_amb_controller.EnabledisableAMBScanRelatedButtons(IsAcqumanReadyForNextRequest());
            }

            break;

        case PQM_MSG_ABORT_WAIT : 		//scan is aborted for item , so auto scan to be off if there..item would be first wait..

            //Added by Hemant On 12/19/2008 8:09:36 PM
            //There should be wait cursor between scan start to status = current or status = abort_wait
            SetWaitCursor(false, false);
            //01-07-09 Added by Sarjeet for MRS scan
            ProcessMRSMessages(Msg); // Send this message to MRS dialog if it is open than it should be closed

            if (dlgCoilSelWin) {
                dlgCoilSelWin->EnableDisableDialogState(TRUE);
            }

        case PQM_MSG_ABORT_FAIL : {		//the item was scanned is failed now.. so make auto scan off for next item , if there..next item would be first wait

            RemoevAutoScanFromFirstWaitItem();
            //Patni-Hemant/2010Mar05/Added/MSA00251-00246
            m_current_scan_opration = CurrentScanOpration_Invalid;

            set_pause_state = true ;
            is_scan_related_msg = true ;

            //enable the close button,,
            //(GetDlgItem(IDC_CLOSE))->EnableWindow(true);
            SetFlagRelatedCtrlsStates();

            //update the total remaining time..
            DisplayTotalRemainingTimeOnPQM();

            //OnSelectionStatusChanged();

            //added by Hemant - for dynamic scan..
            m_dynamic_scan_on = false ;

            //+Patni-HEMANT/ADDED On 9/13/2009 2:02:25 PM/ MVC004681
            if (PQM_MSG_ABORT_FAIL == Msg) {
                SetCanAPCPause(APC_DisableFromPQM);
                SetAPCPauseChecked(false);//Patni-MRP/2010Dec30/Added/REDMINE-1095-reopen issue
            }

            //-Patni-HEMANT/ADDED On 9/13/2009 2:02:25 PM/ MVC004681

            //01-07-09 Added by Sarjeet for MRS scan
            ProcessMRSMessages(Msg); // Send this message to MRS dialog if it is open than it should be closed

            if (dlgCoilSelWin) {
                dlgCoilSelWin->EnableDisableDialogState(TRUE);
            }

        }
        break ;

        case PQM_MSG_STATUS_DONE : {

            //added by Hemant - for dynamic scan..
            m_dynamic_scan_on = false ;

            //Patni-Hemant/2010Mar05/Added/MSA00251-00246
            m_current_scan_opration = CurrentScanOpration_Invalid;

            //Patni-HEMANT/ADDED On 9/23/2009 1:34:02 PM/ MVC004681
            SetCanAPCPause(APC_DisableFromPQM);

            SetAPCPauseChecked(false);//Patni-MRP/2010Dec30/Added/REDMINE-1095-reopen issue
        }

        case PQM_MSG_SCAN_START :
        case PQM_MSG_RESUME_DONE :

            set_pause_state = true ;
            is_scan_related_msg = true ;

            if (IsScanning()) {
                m_current_scan_opration = CurrentScanOpration_ScanResume;
                m_pqm_listctrl.InvalidateItemsForStatus(GetScanIndex());

                if (PQM_MSG_STATUS_DONE == Msg) {
                    //update the total remaining time..
                    DisplayTotalRemainingTimeOnPQM();

                    //OnSelectionStatusChanged();
                    SetFlagRelatedCtrlsStates();
                }

            } else {
                set_pause_state = true ;
                m_pause_resume_button.EnableWindow(false);
            }

            break ;

        case PQM_MSG_PAUSE_DONE :

            is_scan_related_msg = true ;

            if (IsScanning()) {
                set_pause_state = false ;

                m_current_scan_opration = CurrentScanOpration_ScanPause;
                m_pqm_listctrl.InvalidateItemsForStatus(GetScanIndex());

            } else {
                set_pause_state = true ;
                m_pause_resume_button.EnableWindow(false);
            }

            break ;

        case PQM_MSG_STATUS_CURRENT :	//scan started.. update the butotn states..
            set_pause_state = true ;
            is_scan_related_msg = true ;

            //update the total remaining time..
            DisplayTotalRemainingTimeOnPQM();

            //Added by Hemant On 12/19/2008 8:09:36 PM
            //There should be wait cursor between scan start to status = current or status = abort_wait
            SetWaitCursor(false, false);
            m_pqm_listctrl.SelectOnlyItem(GetScanIndex());
            m_pqm_listctrl.EnsureVisible(GetScanIndex(), true);

            break ;

        case PQM_MSG_MRS_SCAN :
        case PQM_MSG_MRS_LOCALSHIM_BEGIN :
        case PQM_MSG_MRS_LOCALSHIM_DONE :
        case PQM_MSG_MRS_F0_BEGIN :
        case PQM_MSG_MRS_F0_DONE :
        case PQM_MSG_MRS_REF_BEGIN :
        case PQM_MSG_MRS_REF_DONE :
        case PQM_MSG_MRS_WSAT_BEGIN :
        case PQM_MSG_MRS_WSAT_DONE :
        case PQM_MSG_MRS_PRESCAN_DONE :
            //+Patni-Sribanta/2011Feb10/Modified/REDMINE-1026
            //ProcessMRSMessages(Msg);
            g_MainView->PostMessage(WM_PROCESS_MRS_MSG, (WPARAM)Msg, 0);
            //-Patni-Sribanta/2011Feb10/Modified/REDMINE-1026
            break ;

        case PQM_MSG_PROCON_QUEUE_NEXT:
        case PQM_MSG_PROCON_QUEUE_EXIT:
        case PQM_MSG_PROCON_NEXT_COPY:
        case PQM_MSG_PROCON_RESET:
        case PQM_MSG_PROCON_CMD_EXIT: // new message added for procon serever
        case PQM_MSG_PROCON_CONNECTED:
        case PQM_MSG_PROCON_NOT_CONNECTED:
        case PQM_MSG_PROCON_QUEUE_DONE:

        case PQM_MSG_PROCON_CON_ALLOW_SELECTIONS :		//+Patni-HEMANT/ADDED On 3/6/2009 9:12:24 PM/ #232
        case PQM_MSG_PROCON_CON_INHIBIT_SELECTIONS :	//+Patni-HEMANT/ADDED On 3/6/2009 9:12:24 PM/ #232

        case PQM_MSG_CONNECT_FROM_MAIN_THREAD : //+Patni-HEMANT/ADDED On 3/31/2009 2:59:02 PM/ PSP/ IDS#202, IDS#192
        case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_SHOW:
        case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN:
        case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_CLOSE:
        case PQM_MSG_PROCON_INSCAN_SWITCH:
        case PQM_MSG_PROCON_ERR:
            ProcessProConMessages(Msg);
            break;


        case AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL:
            OnAutoScanStart();
            break;


            //added by Hemant - dynamic scan
        case PQM_MSG_SCAN_START_BREATH_HOLD :
            m_dynamic_scan_on = true ;

        case PQM_MSG_APC_RETRY_WAIT:
            is_scan_related_msg = true ;

            break ;

            //Added by Hemant On 12/19/2008 8:42:47 PM

            //+Patni-SS/ADDED,MODDI/2009-May28/JFT#215,#216,#217
        case PQM_MSG_IRS_SCANSTART :

            //Patni-HEMANT/2009Sept05/Added/ MVC#002493
            //Now code is written in the handler of this message
            //to process in GUI thread only.
            PostMessage(WM_HANDLE_IRS_MESSAGE, (WPARAM)Msg, 0);

            break;

        case PQM_MSG_IRS_SCANPAUSE:

            //Patni-HEMANT/2009Sept05/Added/ MVC#002493
            //Now code is written in the handler of this message
            //to process in GUI thread only.
            PostMessage(WM_HANDLE_IRS_MESSAGE, (WPARAM)Msg, 0);


            break;

        case PQM_MSG_IRS_SCANABORT:

            //Patni-HEMANT/2009Sept05/Added/ MVC#002493
            //Now code is written in the handler of this message
            //to process in GUI thread only.
            PostMessage(WM_HANDLE_IRS_MESSAGE, (WPARAM)Msg, 0);

            break;

            //+Patni-MP/2009Apr1/Commented/ACE-1/MCM0213-00167
        case PQM_MSG_INVOKE_LOCATOR:
            //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        {
            CScopedLock l_scoped_lock(&m_deletion_cs);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering m_deletion_cs"));
            //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

            //PAnti-SS/MODDIFYED/2009June07/JFT#166
            int topmostwaitnotautoscanprotocol = m_pqm_listctrl.GetFirstWaitProtocolForAutoPlan();

            //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
            CString l_str = _T("");
            l_str.Format(_T("First Wait-NonAutoScan Protocol Index is : %d"), topmostwaitnotautoscanprotocol);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
            //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

            if (topmostwaitnotautoscanprotocol != -1) {
                m_pqm_listctrl.SelectOnlyItem(topmostwaitnotautoscanprotocol);
                EnableDisableScanEditOrPlanButtons();
                OnScanEditOrPlan(PQM_PROCON_SCAN_PLAN, true);
            }

            //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Leaving m_deletion_cs"));
        }
        //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

        break;

        //-Patni-MP/2009Apr1/Commented/ACE-1/MCM0213-00167
        case PQM_MSG_START_SCAN_FIRST_WAIT:

            int l_target_pos;
            m_acquisition->GetTargetProtInformation(&l_target_pos);
            g_MainView->ApplyAutoScanToItems(&l_target_pos, 1, true);
            SetWaitCursor(true);
            ScanStart();
            break;
    }

    ///IRS evnt Handling Nitin
    if (is_scan_related_msg) {

        PostMessage(WM_REFRESH_GUI, 0, 0);
        m_cfadlg->ProcessShimmingMsg(Msg);

        m_pqm_listctrl.InvalidateItemsForStatus(GetScanIndex()); //
        //change the pause_resume butotn image..
        TogglePauseResumeButtonAppearence(set_pause_state);
    }
}


bool CPQMView::CanPerformModeOptionOperations(
)const
{

    return CanPerformEditingForVFFile() ;
}


bool CPQMView::CanPerformEditingForVFFile(
)const
{
    return (!(IsWaitCursor() || m_is_wse_open));
}

//************************************Method Header************************************
// Method Name  : CreateContrastAgentPopupMenu
// Author       : PATNI/
// Purpose      : Creates Contrast Agent pop up menu
//***********************************************************************************
bool CPQMView::CreateContrastAgentPopupMenu(
    PopupMenuEx* contrastmenu,
    const int row_index
)const
{
    if (!IsValidSequenceIndex(row_index)) {

        //        ASSERT(FALSE);
        return false ;
    }

    const int total_contrast_agents = m_contrast_agent->GetCount() ;

    if (total_contrast_agents <= 0)
        return false ;

    for (int index = 0; index < (int)contrastmenu->GetMenuItemCount() ; index ++) {
        contrastmenu->RemoveMenu(index, MF_BYPOSITION);
    }

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(row_index);
    CString contrst_agentstr = l_seq->GetPrMode().m_str_contrast;

    int l_menuid = ID_CONTRAST_POPUP_MENU;
    ContrastAgent contrastagent ;

    POSITION pos = m_contrast_agent->GetHeadPosition();

    // Replace & with && in contrast agent string to display ampersand(&).
    contrst_agentstr.Replace(_T("&"), _T("&&"));

    for (int contrast_agent_index = 0; contrast_agent_index < total_contrast_agents ; contrast_agent_index ++) {

        contrastagent = m_contrast_agent->GetNext(pos) ;

        // Replace & with && in contrast agent string to display ampersand(&).
        contrastagent.m_strname.Replace(_T("&"), _T("&&"));

        contrastmenu->InsertMenu(-1, MF_BYPOSITION, l_menuid, contrastagent.m_strname);

        //+Patni-ARD/2010Apr08/Modified/JaFT# IR-100

        //+Patni-Hemant/2010Jun15/Modified/V1.20/MVC8201/Contrast Agent
        if ( //(current_contrast_agent == contrastagent.m_index) &&
            (contrst_agentstr.CompareNoCase(contrastagent.m_strname) == 0)
            && contrst_agentstr.CompareNoCase(_T("")) != 0) {
            //-Patni-Hemant/2010Jun15/Modified/V1.20/MVC8201/Contrast Agent

            contrastmenu->CheckMenuItem(l_menuid, MF_BYCOMMAND | MF_CHECKED);
        }

        //-Patni-ARD/2010Apr08/Modified/JaFT# IR-100
        l_menuid ++ ;
    }

    return true ;

}

//****************************Method Header************************************
//Method Name   : IsScanning
//Author        : MSN
//Purpose       : Return if scanning or not
//*****************************************************************************
bool CPQMView::IsScanning()const
{
    return m_is_scanning;
}

bool CPQMView::IsRMCRunning() const
{
    return m_rmc_scan_running;
}


void CPQMView::UpdateHoldValue()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateHoldValue");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_list) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));

        return ;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    VARIANT* pdata = new VARIANT;
    pdata->wReserved1 = HOLD_VALUE;


    CSequence* pseq = NULL ;

    for (int index = 0; index < m_list->GetCount(); index++) {

        pseq = (CSequence*)m_pqm_listctrl.GetItemData(index);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("pseq is NULL "));
            continue;
        }

        pdata->intVal = pseq->GetCPASCOMSequence()->GetSequenceParam()->
                        GetInt(_T("HOLD"));

        m_acquisition->UpdateGUIControlsInVarfieldDB(
            pdata, _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"))));

    }

    DEL_PTR(pdata)
}


//***************************Method Header*************************************

//Method Name    :PQMManualPrescanClose

//Author         :PATNI/JS

//Purpose        :

//*****************************************************************************

void CPQMView::PQMManualPrescanClose(
)const
{
    //we should call PQMManualPrescanClose when scanning with manual prescan..
    //if scan is not going on then no need to do this action...
    if (IsScanning())
        m_acquisition->PQMManualPrescanClose();

}


//+Patni-AMT/2009Jul06/Modified/ Code Review and TMSC comments
//***************************Method Header*************************************
//Method Name    :PQMManualPrescanApply
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CPQMView::PQMManualPrescanApply(
    VARIANT* f_pdata
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::PQMManualPrescanApply");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_acquisition->PQMManualPrescanApply(f_pdata);
}

//***************************Method Header*************************************
//Method Name    :CheckASCompassOnOff
//Author         :IGate/SM
//Purpose        :Check if AS-Compass is On/Off During Manual PreScan
//*****************************************************************************
BOOL CPQMView::CheckASCompassOnOff(int f_scan_index)
{

    //If AS-Compass is ON and Manual Prescan is Done,Then Aborted the SCAN

    LPCTSTR FUNC_NAME = _T("CPQMView::CheckASCompassOnOff");

    CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(f_scan_index);

    if (NULL == pseq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("pseq is NULL"));
        return FALSE;
    }

    const BOOL l_ascompass_flag = pseq->GetASCompassFlag();

    if (l_ascompass_flag == TRUE) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("AS-Compass is ON"));
        return TRUE;
    }

    return FALSE;
}
//************************************Method Header************************************
// Method Name  : OnSelectionStatusChanged
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnSelectionStatusChanged(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnSelectionStatusChanged");
    PQM_TRACE_SCOPED(FUNC_NAME);

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    bool iswse_eidted_protocol_selected = false;
    int* selected_items = NULL;
    int count = 0;
    m_bEnableLocatorOnScanEdit = false;

    try {
        bool is_mulple_selection = false;
        int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem(&is_mulple_selection);

        if (!IsValidSequenceIndex(first_selected_item)) {

            DisplayScanOffsetOnPQM(-1);
            DisplaySelectedCoil(-1);

            //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
            SetIconForButton(m_scan_offset_button, -1);
            m_scan_offset_button.EnableWindow(false);

            m_coil_button.EnableWindow(false);

        } else { //if(!is_mulple_selection)

            count = m_pqm_listctrl.GetselectedItems(&selected_items,
                                                    &iswait_selected,
                                                    &iscurrent_selected,
                                                    &isdone_selected,
                                                    &isfail_selected,
                                                    NULL,
                                                    &iswse_eidted_protocol_selected);

            //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
            if ((count <= 0) || (NULL == selected_items)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("selected_items pointer is NULL"));
                return;
            }

            //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
            first_selected_item = selected_items[0] ;

            if (!IsValidSequenceIndex(first_selected_item)) {
                DEL_PTR_ARY(selected_items);
                return;
            }

            CheckPqmPqIsGlocOk(); //Patni-MSN-MN/2009May04/Code Review/Changes as per vantage code

            BOOL enable_buttons = (!m_is_wse_open && !IsAMBInvoked() && (!is_mulple_selection && (first_selected_item >= 0) && (!iscurrent_selected && !isdone_selected && !isfail_selected && !iswse_eidted_protocol_selected)));
            DisplayScanOffsetOnPQM(first_selected_item);		//always show the scan offset for first..

            int l_icon_id = -1 ;
            CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(first_selected_item);

            if (NULL == l_seq) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
                return;
            }


            if (m_pqm_g.GetPQMMode() != K_IMAGING) {
                enable_buttons = false;
                CUIButtonUtility::GetInstance()->SetEnableDisableButtonColor(m_scan_offset_button, false);

            } else {
                if (enable_buttons) {
                    l_icon_id = IDI_SCANOFFSET_BUTTON_IMAGE ;
                    CUIButtonUtility::GetInstance()->SetEnableDisableButtonColor(m_scan_offset_button, (l_seq->GetScanOffset().z != 0.f));
                }
            }

            SetIconForButton(m_scan_offset_button, l_icon_id);
            m_scan_offset_button.EnableWindow(enable_buttons);

            //+Patni-ARD/2009July21/Modified/JFT# defect 177
            if (m_acquisition->GetHasParent(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER")))
                && l_seq->GetWSEEditingEnum() == WSE_Planned
               ) {

                //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
                SetIconForButton(m_scan_offset_button, -1);
                m_scan_offset_button.EnableWindow(false);
            }

            //-Patni-ARD/2009July21/Modified/JFT# defect 177

            enable_buttons = (!m_is_wse_open
                              && ((first_selected_item >= 0)
                                  && (!iscurrent_selected && !isdone_selected && !isfail_selected && !iswse_eidted_protocol_selected)));

            if (m_coil_button.IsWindowEnabled() != enable_buttons) {
                //+Patni-Manish/2009June08/Added/JFT#250/Enable COIL Button
                if (isdone_selected || isfail_selected || iscurrent_selected || IsAMBInvoked()) {
                    m_coil_button.EnableWindow(false);

                } else if (IsAcqumanReadyForNextRequest()) {
                    m_coil_button.EnableWindow(true);

                } else {
                    //+Patni-Manish/2009Jul3/Added/IDS#650-Enable COIL button on schedule page
                    if (m_pqm_g.GetPQMMode() == K_IMAGING) {
                        m_coil_button.EnableWindow(false);

                    } else {
                        m_coil_button.EnableWindow(true);
                    }

                    //-Patni-Manish/2009Jul3/Added/IDS#650-Enable COIL button on schedule page
                }
            }

            if (isdone_selected || isfail_selected || iscurrent_selected || IsAMBInvoked()) {
                m_coil_button.EnableWindow(false);

            } else if (IsAcqumanReadyForNextRequest()) {
                m_coil_button.EnableWindow(true);

            } else {
                m_coil_button.EnableWindow((m_pqm_g.GetPQMMode() != K_IMAGING));
            }

            //-Patni-Manish/2009June08/Added/JFT#250/Enable COIL Button

            DisplaySelectedCoil(first_selected_item);	//always show the coil for first selected...
        }

        EnableDisableScanEditOrPlanButtons();

        ChangeContentForModelessInfoDialogs();

        EnableDisableScanRelatedButtonsForSelectedProtocolStatus();

        int protocol_index;

        if (IsLastProtocolProcessed()) {

            protocol_index = m_list->GetCount() - 1;
            is_mulple_selection = false;

        } else {

            protocol_index = first_selected_item;

        }

        UpdateOrientationCombosOnSelectionChanged(protocol_index, is_mulple_selection);
        EnableDisableScanAnatomy();

        //this in inserted here, be'us the queue and next buttons should be enabled when logic has changed and there are new seq items inserted
        EnableDisableQueueDialogButtons();

        //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
        SetIconForButton(m_utility_button, m_sr_invoked ? IDI_UTILITY_BUTTON_IMAGE : -1);
        m_utility_button.EnableWindow(m_sr_invoked);

        //Patni-Hemant/2009Nov16/Added/IR#90/Bottom Buttons
        UpdateSarAnatomyButton();

        UpdateEndExamButtonStatus();
        UpdateOKBtnStatusofWirelssDlg();
        UpdateCFARetry();

    } catch (CException* e) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Exception 'e' found in OnSelectionStatusChanged..."));

        TCHAR l_excp[1024] = {0};
        e->GetErrorMessage(l_excp, 1024);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_excp);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, GetPQMFlagsString());
        throw;

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unhandled Exception in OnSelectionStatusChanged..."));

#define BOOLYESNO(b) (b ? _T("yes") : _T("No"))

        CString l_log_string = _T("");
        l_log_string.Format(_T("Total Selected Items: %d"), count);
        l_log_string = l_log_string + _T("  iswait_selected = ") + BOOLYESNO(iswait_selected);
        l_log_string = l_log_string + _T("  iscurrent_selected = ") + BOOLYESNO(iscurrent_selected);
        l_log_string = l_log_string + _T("  isdone_selected = ") + BOOLYESNO(isdone_selected);
        l_log_string = l_log_string + _T("  isfail_selected = ") + BOOLYESNO(isfail_selected);
        l_log_string = l_log_string + _T("  iswse_eidted_protocol_selected = ") + BOOLYESNO(iswse_eidted_protocol_selected);

        l_log_string = l_log_string + _T("\n");

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, GetPQMFlagsString());
#undef BOOLYESNO

        throw;
    }

    DEL_PTR_ARY(selected_items);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::DeleteProtocolWithIndex(
    CSequence* l_seq,
    const int index
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::DeleteProtocolWithIndex");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-HAR/2010Jan29/Added/DeFT#MVC006364
    if (GetScanIndex() == index) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Can not delete scanning protocol"));
        return false;
    }

    //-Patni-HAR/2010Jan29/Added/DeFT#MVC006364

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == l_seq) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_seq pointer is NULL"));
        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    const int l_acqorder = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
    CString l_str;
    l_str.Format(_T("Deleting Protocol (Index, Acqorder), (%d, %d)"), index, l_acqorder);

    CString SeqLID;
    l_seq->GetCPASCOMSequence()->GetSequenceID(SeqLID);

    if (S_FALSE == m_acquisition->DeleteSequence(SeqLID.AllocSysString()))
        return false;

    //Patni-HAR/2010Sep20/Modified/Rename function from SendIndex() to DeleteProtocolFromVarField()

    l_seq->SetSARInfoPtrForSeq(NULL);

    if (S_FALSE == m_acquisition->DeleteProtocolFromVarField(l_acqorder)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DeleteProtocolFromVarField Failed...Error Occured..."));
        return false ;
    }

    return true ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMView::DuplicateSequenceforIndex(
    CSequence* seq_to_duplicate,
    const int index_being_duplicated,
    const int new_index_for_duplicated_protocol,
    const bool drag_dropped, /*= false*/
    const int f_group_id /* = 0*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::DuplicateSequenceforIndex");
    PQM_TRACE_SCOPED(FUNC_NAME);

    try {

        if (new_index_for_duplicated_protocol < 0) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("new_index_for_duplicated_protocol is less Than ZERO"));
            return -1 ;
        }

        if (drag_dropped) {

            int l_drag_index = index_being_duplicated;

            if (S_OK == m_acquisition->CheckSlaveProtocolForDuplicate(&l_drag_index, 1)) {

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    CPQMUtility::GetMultiLingualString(_T("IDS_WARN_AMB_SEQ_COPY")),
                    MPlus::EH::Information, _T("PQM"), _T("PQM"));

                return -1 ;
            }
        }


        CSequence* duplicated_seq = new CSequence(seq_to_duplicate);

        if (!duplicated_seq) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could Not Create the Duplicated Sequence"));
            return -1;
        }

        CString l_str;
        l_str.Format(_T("Function Params : index_being_duplicated = %d, Acq_Order_index_being_duplicated = %d, new_index_for_duplicated_protocol= %d"),
                     index_being_duplicated,
                     GetAcqOderForGivenProtocolIndex(index_being_duplicated),
                     new_index_for_duplicated_protocol);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

        // Set blank sequce id, this will be set at IPC side after sequence duplicated. // sarjeet 06/04/2008
        duplicated_seq->GetCPASCOMSequence()->SetSequenceID(" ");
        duplicated_seq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("HOLD"), 1);

        int new_index_for_duplicated_protocol_for_IPC = (new_index_for_duplicated_protocol >= m_list->GetCount()) ? 0 : new_index_for_duplicated_protocol ;
        //new_index_for_duplicated_protocol_for_IPC =

        //Meghana --Check if the sequence to be duplicated is "CURRENT"
        const int status = duplicated_seq->GetAcqStatus();

        if ((status == PR_ACQ_STATUS_CURRENT) && drag_dropped) {
            new_index_for_duplicated_protocol_for_IPC = new_index_for_duplicated_protocol;
        }

        //+Patni-RSG/2009Oct14/Commented/MVC005134
        /*else if(status == PR_ACQ_STATUS_CURRENT)
        {
        	new_index_for_duplicated_protocol_for_IPC = 0;
        }*/
        //-Patni-RSG/2009Oct14/Commented/MVC005134


        int acq_order = m_acquisition->DuplicateProtocol(
                            index_being_duplicated,
                            new_index_for_duplicated_protocol_for_IPC, f_group_id);

        if (acq_order == -1) {
            DEL_PTR(duplicated_seq)
            return -1;
        }

        // SM4 Defect#60 AMIT+Himanshu
        if (status == PR_ACQ_STATUS_CURRENT) {
            new_index_for_duplicated_protocol_for_IPC = (new_index_for_duplicated_protocol >= m_list->GetCount()) ? 0 : new_index_for_duplicated_protocol ;
        }

        VARIANT* pData = new VARIANT;
        pData->byref = (PVOID)duplicated_seq->GetCPASCOMSequence();

        if (acq_order == -1) {

            DEL_PTR(duplicated_seq)
            DEL_PTR(pData)
            return -1;
        }

        //+Patni-ARD/2009Jun19/Added/JFT# DBServer access issue
        CString str_acq_order(_T(""));
        str_acq_order.Format(_T("%d"), acq_order);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_acq_order);

        duplicated_seq->GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("ACQORDER"), str_acq_order);
        //-Patni-ARD/2009Jun19/Added/JFT# DBServer access issue

        //+Patni-AMT/2010Apr16/Modified/JaFT# PROPOSAL1_CDR_02
        CString new_index_string("");
        new_index_string.Format(_T("%d"), new_index_for_duplicated_protocol_for_IPC);
        BSTR l_new_index_str = reinterpret_cast<BSTR>(new_index_string.AllocSysString());
        HRESULT l_ret_dup_seq = m_acquisition->DuplicateSequence(pData, l_new_index_str, index_being_duplicated);
        SysFreeString(l_new_index_str);

        DEL_PTR(pData);

        if (S_FALSE == l_ret_dup_seq) {

            DEL_PTR(duplicated_seq);
            return -1;
        }

        m_list->InsertAfter(m_list->FindIndex(new_index_for_duplicated_protocol - 1), duplicated_seq);

        //now insert it to list...
        m_pqm_listctrl.InsertProtocolAt(duplicated_seq, new_index_for_duplicated_protocol, false);

        //Patni-HEMANT/2010Nov15/Added/V1.30/Redmine-971
        GetSequenceInformation(new_index_for_duplicated_protocol);
        m_pqm_listctrl.UpdateSequences(&new_index_for_duplicated_protocol, 1);

        //update the total remaining time..
        DisplayTotalRemainingTimeOnPQM();

        //Patni-HEMANT/ADDED On 7/1/2009 1:02:45 PM/ ACE-2 / WFDA
        m_acquisition->OnPQMDuplicateProtocol(index_being_duplicated, new_index_for_duplicated_protocol);

        ValdiateBodyPartData(duplicated_seq, new_index_for_duplicated_protocol);

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION Occured..."));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, GetPQMFlagsString());

        SetMismatchErrorFlag((TYPE_FALSE == GetGUIListProtocolInfo()) ? TRUE : FALSE);

        throw;
    }

    return new_index_for_duplicated_protocol;
}
//************************************Method Header************************************
// Method Name  : MoveAMBProtocolToIndex
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMView::MoveAMBProtocolToIndex(const int protocol_index_to_move, const int target_index)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::MoveProtocolToIndex");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString l_log_str(_T(""));
    l_log_str.Format(_T("[drag_index = %d] [target_index = %d] [scan_index = %d]"), protocol_index_to_move, target_index, GetScanIndex());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    if (NULL == m_list) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));

        return false;
    }

    CString selected_str(_T(""));
    selected_str.Format(_T("%d"), protocol_index_to_move);
    ChangeOrder(selected_str, target_index);

    if (S_FALSE == m_acquisition->AMBReArrange(protocol_index_to_move, target_index)) {
        return false;
    }

    POSITION position_to_move = m_list->FindIndex(protocol_index_to_move) ;

    if (NULL == position_to_move) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot find the protocol in the list"));
        return false;
    }

    CSequence* sequnece_to_process = m_list->GetAt(position_to_move) ;

    m_pqm_listctrl.DeleteItem(protocol_index_to_move);
    m_list->RemoveAt(position_to_move);

    if ((target_index) > 0) {
        m_list->InsertAfter(m_list->FindIndex(target_index - 1), sequnece_to_process);

    } else {
        m_list->InsertBefore(m_list->FindIndex(target_index), sequnece_to_process);
    }

    m_pqm_listctrl.InsertProtocolAt(sequnece_to_process, target_index);

    DisplayTotalRemainingTimeOnPQM();
    RemoevAutoScanFromFirstWaitItem();

    return true ;
}

//************************************Method Header************************************
// Method Name  : MoveProtocolToIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::MoveProtocolToIndex(
    const int protocol_index_to_move,
    const int target_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::MoveProtocolToIndex");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //steps 1) delete the item from the list first..
    //+Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246
    CString l_log_str(_T(""));
    l_log_str.Format(_T("[drag_index = %d] [target_index = %d] [scan_index = %d]"), protocol_index_to_move, target_index, GetScanIndex());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    //-Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_list) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));

        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections


    //call IPC side first... everything with zero based index..
    CString selected_str(_T(""));   //Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
    selected_str.Format(_T("%d"), protocol_index_to_move);
    ChangeOrder(selected_str, target_index);

    if (S_FALSE == m_acquisition->ReArrange(protocol_index_to_move,
                                            target_index)) {
        return false;
    }

    //here..we will change the sar and coil of the moved protocol according to the data of the prev protocol with dropped index..

    POSITION position_to_move = m_list->FindIndex(protocol_index_to_move) ;

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == position_to_move) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot find the protocol in the list"));
        return false;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    CSequence* sequnece_to_process = m_list->GetAt(position_to_move) ;

    if (IsBatchSettingOn()) {

        CPqmPrMode& prmode_to_process = sequnece_to_process->GetPrMode();

        //If perv index is wait , take its settings,
        //else if first wait protocl is there, take its settings otherwise remain as it is..
        //There will always be first wait.. be'us rearranging is done for wait protocols..

        int prev_index = target_index;

        if (protocol_index_to_move > target_index)
            prev_index -= 1;

        //get the actual settings to be applied from..
        int other_setting_index = m_pqm_listctrl.GetFirstWaitProtocolIndex(true);

        if (IsValidSequenceIndex(prev_index)) {

            POSITION pos = m_list->FindIndex(prev_index);
            CSequence* prev_seq = (CSequence*)(m_list->GetAt(pos));

            //if prev index protocol is wait, then get its index, else prev index (default = first wait)
            other_setting_index = (prev_seq->GetAcqStatus() == PR_ACQ_STATUS_WAIT)  ? prev_index : other_setting_index;
        }

        POSITION dropped_pos = m_list->FindIndex(other_setting_index);

        CSequence* sequnece_to_target = m_list->GetAt(dropped_pos);
        CPqmPrMode l_drop_prmode = sequnece_to_target->GetPrMode();

        WCHAR buff[20] = {0}; //Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
        sequnece_to_process->SetValue(rc, _itow(l_drop_prmode.m_rcv_coil_id, buff, 10));	//setting the value in seq
        prmode_to_process.m_rcv_coil_id = l_drop_prmode.m_rcv_coil_id;		//setting the same in prmode..
        prmode_to_process.m_recv_coil_label = l_drop_prmode.m_recv_coil_label;	//apply lable also..

        sequnece_to_process->SetValue(sa, _itow(l_drop_prmode.m_anatomy, buff, 10));		//setting the value in seq.
        prmode_to_process.m_anatomy = l_drop_prmode.m_anatomy;	//setting the same in prmode..

        //apply same scan anatomy to this prmode..
        prmode_to_process.m_str_scan_anatomy = l_drop_prmode.m_str_scan_anatomy;

        //********information for patient orientation
        //******* setting TRSC
        sequnece_to_process->SetValue(trsc, l_drop_prmode.m_str_trsc);
        prmode_to_process.m_str_trsc = l_drop_prmode.m_str_trsc;	//setting the same in prmode..

        //+Patni-HAR/2010Jan14/Added/DeFT#MVC006737

        //+Patni-Hemant/2010Jun8/Added/MaFT/MVC008201
        prmode_to_process.m_contrast = l_drop_prmode.m_contrast ;
        prmode_to_process.m_str_contrast = l_drop_prmode.m_str_contrast ;
        //-Patni-Hemant/2010Jun8/Added/MaFT/MVC008201

        //Patni-Hemant/2010Jun18/Added/MaFT/MVC008201/Update
        sequnece_to_process->GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("CONTRASTNAME"), prmode_to_process.m_contrast);

    } else {
        CPqmPrMode& prmode_to_process = sequnece_to_process->GetPrMode();

        int prev_index = target_index;

        if (protocol_index_to_move > target_index)
            prev_index -= 1;

        //get the actual settings to be applied from..

        //There will always be first wait.. be'us rearranging is done for wait protocols..
        int other_setting_index = m_pqm_listctrl.GetFirstWaitProtocolIndex(true) ;	//by default

        if (IsValidSequenceIndex(prev_index)) {

            POSITION pos = m_list->FindIndex(prev_index);
            CSequence* prev_seq = (CSequence*)(m_list->GetAt(pos));

            //if prev index protocol is wait, then get its index, else prev index (default = first wait)
            other_setting_index = (prev_seq->GetAcqStatus() == PR_ACQ_STATUS_WAIT)  ? prev_index : other_setting_index;
        }

        POSITION dropped_pos = m_list->FindIndex(other_setting_index);

        CSequence* sequnece_to_target = m_list->GetAt(dropped_pos);
        CPqmPrMode l_drop_prmode = sequnece_to_target->GetPrMode();

        if (prmode_to_process.m_recv_coil_label.IsEmpty()) {

            if (!l_drop_prmode.m_recv_coil_label.IsEmpty()) {

                WCHAR buff[20] = {0};
                sequnece_to_process->SetValue(rc, _itow(l_drop_prmode.m_rcv_coil_id, buff, 10));	//setting the value in seq
                prmode_to_process.m_rcv_coil_id = l_drop_prmode.m_rcv_coil_id;		//setting the same in prmode..
                prmode_to_process.m_recv_coil_label = l_drop_prmode.m_recv_coil_label;	//apply lable also..
            }
        }

        if (prmode_to_process.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty()) {

            if (!l_drop_prmode.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty()) {

                WCHAR buff[20] = {0};
                sequnece_to_process->SetValue(sa, _itow(l_drop_prmode.m_anatomy, buff, 10));  //setting the value in seq.
                prmode_to_process.m_anatomy = l_drop_prmode.m_anatomy;	//setting the same in prmode..

                //apply same scan anatomy to this prmode..
                prmode_to_process.m_str_scan_anatomy = l_drop_prmode.m_str_scan_anatomy;
            }
        }

        //+Patni-Hemant/2010Jun8/Added/MaFT/MVC008201
        prmode_to_process.m_contrast = l_drop_prmode.m_contrast ;
        prmode_to_process.m_str_contrast = l_drop_prmode.m_str_contrast ;
        //-Patni-Hemant/2010Jun8/Added/MaFT/MVC008201

        //Patni-Hemant/2010Jun18/Added/MaFT/MVC008201/Update
        sequnece_to_process->GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("CONTRASTNAME"), prmode_to_process.m_contrast);
    }

    //-Patni-HAR/2010Jan14/Added/DeFT#MVC006737
    m_pqm_listctrl.DeleteItem(protocol_index_to_move);
    m_list->RemoveAt(position_to_move);

    if ((target_index) > 0) {
        m_list->InsertAfter(m_list->FindIndex(target_index - 1), sequnece_to_process);

    } else {
        m_list->InsertBefore(m_list->FindIndex(target_index), sequnece_to_process);
    }

    m_pqm_listctrl.InsertProtocolAt(sequnece_to_process, target_index);


    //update the total remaining time..
    DisplayTotalRemainingTimeOnPQM();
    RemoevAutoScanFromFirstWaitItem();

    return true ;
}
//************************************Method Header************************************
// Method Name  : AMBPerformSlaveDuplicate
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::AMBPerformSlaveDuplicate(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::AMBPerformSlaveDuplicate");
    PQM_TRACE_SCOPED(FUNC_NAME);


    VARIANT pData;
    GetAcquisition()->GetAMBMasterProtocols(&pData);
    ambSlave_t* l_slave_list = (ambSlave_t*)pData.byref;
    const int l_slave_count = l_slave_list->m_count;

    int* selected_items_acq_order = l_slave_list->m_master_acqorders_to_duplicate;

    if ((NULL == selected_items_acq_order) || (l_slave_count <= 0)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T(" selected_items_acq_order is NULL OR l_slave_count <=0 "));

    } else {

        int* selected_items = new int [l_slave_count];

        for (int j = 0; j < l_slave_count ; j++) {
            selected_items[j] = GetProtocolIndexFromAcqorder(selected_items_acq_order[j]);
        }

        int l_max_master_pos = -1;

        for (int i = 0; i < l_slave_count ; i++) {
            if (selected_items[i] > l_max_master_pos) {
                l_max_master_pos = selected_items[i];
            }
        }

        int l_duplciated_slaves = 0;
        int l_duplicated_index = 0;

        for (i = 0; i < l_slave_count ; i++) {

            if (IsValidSequenceIndex(selected_items[i])) {

                l_duplicated_index = m_pqm_listctrl.DuplicateSequenceforIndex(selected_items[i], l_max_master_pos + l_duplciated_slaves + 1);

                if (l_duplicated_index > -1) {
                    l_slave_list->m_duplicated_slave_acqorders[i] = GetAcqOderForGivenProtocolIndex(l_duplicated_index);

                    if (i == 0) {
                        ApplyAutoScanToItems(&l_duplicated_index, 1, true);
                    }

                    l_duplciated_slaves++ ;

                    m_pqm_listctrl.EnsureVisible(l_duplicated_index, FALSE);

                } else {
                    l_slave_list->m_duplicated_slave_acqorders[i] = -1;
                    break;
                }
            }
        }

        DEL_PTR_ARY(selected_items);
    }

    HANDLE slave_duplicate_event_handle = NULL;

    if (NULL != (slave_duplicate_event_handle = OpenEvent(EVENT_ALL_ACCESS, FALSE, PQM_AMB_SLAVE_DUPLICATE))) {
        SetEvent(slave_duplicate_event_handle);
        CloseHandle(slave_duplicate_event_handle);
    }
}

//************************************Method Header************************************
// Method Name  : AMBSlaveDuplicate
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::AMBSlaveDuplicate()
{

    m_pqmchildthread.PostMessageToThread(ON_AMB_DUPLCIATE, NULL, NULL);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonDuplicate(

    //Patni-KSS/2010Jul16/ADDED/IR-156_2
    const bool f_append /*= false*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonDuplicate");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!m_iswait_cursor) {

        SetWaitCursor(true);
        EnableDisableScanRelatedButtonsForSelectedProtocolStatus();

        //Patni-KSS/2010Jul16/Modified/IR-156_2

        int* l_selected_items = NULL ;
        int l_count = m_pqm_listctrl.GetselectedItems(&l_selected_items);

        if (S_OK == m_acquisition->CheckSlaveProtocolForDuplicate(l_selected_items, l_count)) {

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPQMUtility::GetMultiLingualString(_T("IDS_WARN_AMB_SEQ_COPY")),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));
        }


        m_pqm_listctrl.DuplicateSelectedItems(f_append);

        OnSelectionStatusChanged();

        //Patni-ARD/2009September8/Added/Prototype 2
        //+Patni-HAR/2010Feb24/Modified/MSA0251-00187
        //+Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1

        //REDMINE_781
        /*
        if (TYPE_FALSE == GetGUIListProtocolInfo()) {
            SetMismatchErrorFlag(TRUE);
            //-Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1

        } else {
            SetMismatchErrorFlag(FALSE);
        }
        */
        SetMismatchErrorFlag((TYPE_FALSE == GetGUIListProtocolInfo()) ? TRUE : FALSE);

        //-Patni-HAR/2010Feb24/Modified/MSA0251-00187


        m_tiprep_app_cntl.SendRefreshMsgToTiPrep();
        SetWaitCursor(false);
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsPausedScanning(
)const
{
    return (m_current_scan_opration == CurrentScanOpration_ScanPause) ;
}

void CPQMView::MrsScanStartUiCB(
    SMRSScan_s* mrsscan_values
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::MrsScanStartUiCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));

    VARIANT* pData = new VARIANT;
    pData->byref = mrsscan_values ;
    //Patni-PJS/2010July02/Added/V1.30#TMSC-REDMINE-316
    m_is_mrs_prot = false;
    m_acquisition->MrsScanStartUiCB(pData) ;

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }
}



void CPQMView::MrsScanAbort(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::MrsScanAbort");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));

    //Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
    //OnClickButtonScanAbort();
    //Patni-PJS/2010July02/Added/V1.30#TMSC-REDMINE-316
    m_is_mrs_prot = false;
    AbortScan();
}


void CPQMView::MrsScanClose(
)
{
    m_is_mrs_prot = false;
    m_acquisition->MrsScanClose();
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::RemoevAutoScanFromFirstWaitItem(
    const int f_first_wait
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::RemoevAutoScanFromFirstWaitItem");
    PQM_TRACE_SCOPED(FUNC_NAME);

    const int first_wait_protocol_index = m_pqm_listctrl.GetFirstWaitProtocolIndex();

    if (f_first_wait != -1) {
        if (first_wait_protocol_index != f_first_wait) {
            return;
        }
    }

    if (!(IsValidSequenceIndex(first_wait_protocol_index)))   {

        CString l_str;
        l_str.Format(_T("first_wait_protocol_index %d"), first_wait_protocol_index);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

        return;
    }

    if (IsValidSequenceIndex(first_wait_protocol_index - 1)) {

        CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(first_wait_protocol_index - 1);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL"));
            return;
        }

        if (pseq->GetAcqStatus() == PR_ACQ_STATUS_CURRENT) {

            //that is first_wait_protocol_index is the first protocol after auto scan...
            //and thus no need to disable the auto scan for this..
            return ;
        }
    }

    const int l_waiting_for_autoscanstart_prot_acq_order = CheckAndGetWaitingForAutoScanStartAcqOrder();

    if (-1 != l_waiting_for_autoscanstart_prot_acq_order &&
        l_waiting_for_autoscanstart_prot_acq_order == GetAcqOderForGivenProtocolIndex(first_wait_protocol_index)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Waiting for Auto Scan Start TRUE."));
        return;
    }

    MakeAutoScanOffForIfNecessary(first_wait_protocol_index);
    EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanEnableAqumanRelatedMenuItems(
)const
{
    return (!IsScanning() && (m_pqm_g.GetPQMMode() == K_IMAGING));
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SendMessageToProcon(
    const ProconMessageEnum procon_message,
    const bool perform_gui_action /*= true*/ ,
    const bool send_message_to_procon /*= true*/,
    const int f_acq_order,			//Added by Ashish On 2/10/2009 3:52:02 PM
    const int f_maskValue					//Patni-Ajay/2010July23/Added/TMSC-REDMINE-443

)
{
    //+Patni-Sribanta/2010Sep15/Added/MVC008802
    LPCTSTR FUNC_NAME = _T("CPQMView::SendMessageToProcon");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    //+Patni-Sribanta/2010Sep15/Added/MVC008802

    if (perform_gui_action) {

        switch (procon_message) {

            case PQM_PROCON_CANCEL :

                //Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
                if (m_queuedialog) {

                    const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();


                    if (-1 != current_protocol_index_edited_by_wse) {
                        m_pqm_listctrl.SelectOnlyItem(current_protocol_index_edited_by_wse);

                        m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(current_protocol_index_edited_by_wse);
                        m_pqm_listctrl.ResetWSEEditingProtocolIndex();
                    }

                    if (GetIsAMBPlanning()) {
                        int l_count = m_selected_acqoder_list_for_wse.GetSize();

                        for (int i = 0 ; i < l_count ; i++) {
                            m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(GetListForGivenProtocolAcqOder(m_selected_acqoder_list_for_wse.GetAt(i)));
                        }
                    }

                    OnWSEOpenClosed(false);
                }

                break ;

                //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
            case PQM_PROCON_NEXT_COPY:
            case PQM_PROCON_QUEUE_NEXT: {

                ProcessAllowOrInhibitSelectionForProconButtons(false);

                //Now ask to IPC regarding next wse editing index...
                const int next_protocol_index_edited_by_wse = GetNextWSEEditProt();
                //REDMINE-781
                m_pqm_listctrl.SetWSEEditingProtocolIndex(next_protocol_index_edited_by_wse);
                //

                //+Patni-HEMANT/ADDED On 3/16/2009 2:40:53 PM/ ACE1/ MCM0213-00004 (STM45-02655)- Pqm  List Defect Handling(595)
                //highlight the last edited item
                m_pqm_listctrl.SelectOnlyItem(next_protocol_index_edited_by_wse);

                //Now according to vantage remove auto scan from currently selected protocol for wse editing
                //
                if (next_protocol_index_edited_by_wse != -1) {

                    ApplyAutoScanToItems(&next_protocol_index_edited_by_wse, 1, false);		//make it off explicitly..
                }


                m_pqm_listctrl.EnsureVisible(next_protocol_index_edited_by_wse, FALSE);

                if (next_protocol_index_edited_by_wse == -1) {

                    EnableDisableQueueDialogButtons();
                    return ;
                }

                //now if, this index is the last index of editing, we can not do the queue and next and other next buttons..
                EnableDisableQueueDialogButtons();
                m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(next_protocol_index_edited_by_wse);

                //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
                RemoveGUIProcessingBit(PQM_PROCESSING_QUEUE_NEXT);
                OnSelectionStatusChanged();
                //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

                break ;
            }

            //Added by Hemant On 12/22/2008 11:31:15 AM
            case PQM_PROCON_QUEUE_EXIT : {

                OnWSEOpenClosed(false);
            }
            break ;
        }
    }

    //+Patni-HEMANT/ADDED On 3/10/2009 7:04:35 PM/ PSP#232
    //This function is called when we click on procon communication related button,
    //on click of any button, show the hotu glass on that button.
    //
    if (send_message_to_procon && !perform_gui_action && m_is_wse_open) {
        ProcessAllowOrInhibitSelectionForProconButtons(false);
    }

    //-Patni-HEMANT/ ADDED On3/10/2009 7:20:36 PM/ PSP#232

    if (send_message_to_procon) {

        //+Patni-HEMANT/ADDED On 6/24/2009 12:17:58 PM/ ACE-2 / WFDA
        //
        //		//Modified by Ashish On 2/10/2009 3:53:00 PM ,w e require to send acqorder for plan duplicate
        //		////please check we have inserted some returns before..
        //		//m_acquisition->ProconReqUiCB((int)procon_message);
        //		m_acquisition->ProconReqUiCB((int)procon_message, f_acq_order);

        //+Patni-Ajay/2010July23/Modified/TMSC-REDMINE-443
        //if (m_acquisition->ProconReqUiCB((int)procon_message, f_acq_order) != E_NO_ERROR) {

        //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        switch (procon_message) {
            case PQM_PROCON_QUEUE_NEXT:
            case PQM_PROCON_NEXT_COPY:
                AddNewGUIProcessingBit(PQM_PROCESSING_QUEUE_NEXT);
                break;

            case PQM_PROCON_RESET:
                ProcessAllowOrInhibitSelectionForProconButtons(false, true);
                break;
        }

        //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

        //+Patni-PJS/2011Feb1/Modify/IR-97
        if (!CanProcessProconMessage((int)procon_message)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("CanProcessProconMessage failed"));
            m_queuedialog->SetWaitCursor(false);
            return;
        }

        //-Patni-PJS/2011Feb1/Modify/IR-97

        if (m_acquisition->ProconReqUiCB((int)procon_message , f_acq_order , f_maskValue) != E_NO_ERROR) {
            //Patni-Ajay/2010July23/Modified/TMSC-REDMINE-443

            //if the IPC return is false then remove the hour glass and return /
            //            ASSERT(FALSE);
            //SendMessageToProcon(PQM_PROCON_CANCEL, true , false);
            //ProcessAllowOrInhibitSelectionForProconButtons(true);
            WSEEditingCompleted(WSE_Editing_Operation_GUISave);
            OnWSEOpenClosed(false);
        }

        //
        //-Patni-HEMANT/ADDED On 6/24/2009 12:17:58 PM/ ACE-2 / WFDA

        //Patni-HEMANT/ADDED On 8/26/2009 7:33:53 PM/ IR90
        RemoveWndQueueMessages();
    }
}

//+Patni-PJS/2011Feb1/Modify/IR-97
//************************************Method Header************************************
// Method Name  : NewSequencesAdded
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::NewSequencesAdded(
    const int f_count
) const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SendDeleteCompleteDEGL");
    CString l_f_count ;
    l_f_count.Format(_T("Count is %d "), f_count);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  l_f_count);

    if (m_wfdaimplementer) {

        return m_wfdaimplementer->NewSequencesAdded(f_count);
    }

    ASSERT(FALSE);
    return false;
}

//************************************Method Header************************************
// Method Name  : DeleteWFDASequence
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::DeleteWFDASequence(CSequence* const f_seq)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::DeleteWFDASequence");

    if (NULL == f_seq) {
        //Patni-PJS/2011May2/Modified/IR-97
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_seq pointer is NULL"));
        return false;
    }

    CString l_string_acq_order = f_seq->GetCPASCOMSequence()->
                                 GetSequenceParam()->GetString(_T("ACQORDER"));
    int l_acq_order = _ttoi(l_string_acq_order);

    CString SeqLID;
    f_seq->GetCPASCOMSequence()->GetSequenceID(SeqLID);

    if (S_FALSE == m_acquisition->DeleteSequence(SeqLID.AllocSysString()))
        return false;

    if (S_FALSE == m_acquisition->SendIndex(l_acq_order))
        return false ;

    return true ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::ProcessScanRelatedMsgForWFDA(
    const int f_acq_order,
    const pqm_msg_tag f_msg_tag
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ProcessScanRelatedMsgForWFDA");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace"));

    if (f_acq_order >= 0 && m_wfdaimplementer) {

        WFDA_Protocol_Status l_wfda_protocol_status = WFDA_Protocol_Status_Normal;

        switch (f_msg_tag) {

            case PQM_MSG_SCAN_START:
                l_wfda_protocol_status = WFDA_Protocol_ScanStart;
                break ;

            case PQM_MSG_ABORT_FAIL:
                l_wfda_protocol_status = WFDA_Protocol_ScanAbort;
                break ;

            case PQM_MSG_STATUS_DONE:
                l_wfda_protocol_status = WFDA_Protocol_ScanDone;
                break ;
        }

        if (l_wfda_protocol_status != WFDA_Protocol_Status_Normal) {
            return m_wfdaimplementer->ScanStatusChanged(f_acq_order, l_wfda_protocol_status);
        }
    }

    return false;
}

bool CPQMView::IsProtocolInscan(
    const int f_acq_order
) const
{
    if (m_wfdaimplementer) {
        return m_wfdaimplementer->IsProtocolInscan(f_acq_order);
    }

    return false;
}

bool CPQMView::CheckPlnStatus(const int f_acq_order)
{

    if (m_acquisition) {
        if (S_FALSE == m_acquisition->CheckPlnStatus(f_acq_order)) {
            return false;
        }
    }

    return true;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::OnScanEditStatusChangedForCurrentProtocol(
    const int f_index,
    const WFDA_Protocol_Status f_wfda_protocol_status
)const
{

    if (m_pqm_g.GetPQMMode() != K_IMAGING) {
        return true ;
    }

    if (f_index >= 0) {

        if (m_wfdaimplementer) {
            CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(f_index);

            if (!l_seq) {
                //Trace
                return false;
            }

            CString l_string_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"));
            int l_acq_order = _ttoi(l_string_acq_order);
            return m_wfdaimplementer->OnScanEditStatusChangedForProtocol(l_acq_order, f_wfda_protocol_status);
        }
    }

    return false;
}

//************************************Method Header************************************
// Method Name  : CanProcessProconMessage
// Author       : PATNI/ HEMANT
// Purpose      : we can not do Scan edit for inscan kind of seq in schedule mode so return false
//				  when processing for those. else return true.
//***********************************************************************************
bool CPQMView::CanProcessProconMessage(
    const int f_message_code	//this code may get changed according to WFDA
)
{
    //+Patni/2011Mar24/Modified/WFDA_Offshore

    const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();

    if (!IsValidSequenceIndex(current_protocol_index_edited_by_wse)) {
        return false ;
    }

    const int l_acq_order = GetAcqOderForGivenProtocolIndex(current_protocol_index_edited_by_wse) ;

    int l_new_code = f_message_code;

    if (m_pqm_g.GetPQMMode() != K_IMAGING) {
        return true ;

    } else {

        //only acquire can process..so we are in acquire..
        switch (f_message_code) {
                //Patni-PJS/2010Dec10/Modified/IR-97


            case PQM_PROCON_CANCEL: {
                if (OnScanEditStatusChangedForCurrentProtocol(current_protocol_index_edited_by_wse, WFDA_Protocol_WSEEditing_Cancel)) {
                    m_inscan_refclose_msg_sent = true;
                }
            }
            break ;


            default : {
                ASSERT(FALSE);
                return true ;
            }
        }
    }

    //-Patni/2011Mar24/Modified/WFDA_Offshore

    //return true when we haven't any message for processing..
    return (l_new_code != PQM_PROCON_NO_EVENT) ;
}
//-Patni-PJS/2011Feb1/Modify/IR-97

//+Patni-PJS/2011Feb21/Added/IR-97
void CPQMView::InscanScanStart(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::InscanScanStart");
    int l_index = GetScanIndex();

    if (l_index < 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Invalid scan Index"));
        return;
    }

    CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(l_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
        return;
    }


    CString l_string_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"));
    const int l_acq = _ttoi(l_string_acq_order);

    if (false == g_MainView->ProcessScanRelatedMsgForWFDA(l_acq, PQM_MSG_SCAN_START)) {
        SetWaitCursor(false);
        m_acquisition->SetIsValidWFDAScan(FALSE);
        return;
    }

    m_acquisition->SetIsValidWFDAScan(TRUE);

}
//-Patni-PJS/2011Feb21/Added/IR-97

//************************************Method Header************************************
// Method Name  : ApplyContrastAgentSettings
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyContrastAgentSettings(
    const int row_index /*= -1*/ ,
    const int contrast_agent_index /*= 0*/,
    const bool f_bContrastAgentMenu /*= false*/   //+Patni-Abhishek/2010Dec14/Addeded/IR-180
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyContrastAgentSettings");

    if (!m_iswait_cursor) {

        int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();
        ASSERT(row_index != -1);

        if (row_index != -1)
            first_selected_item = row_index ;

        //int count = m_pqm_listctrl.GetselectedItems(&selected_items);

        int count = (m_pqm_listctrl.GetItemCount() - first_selected_item) ;
        int* selected_items = new int[count] ;
        int counter = 0 ;

        for (int item = first_selected_item; counter < count; item++)
            selected_items[counter++] = item;

        //	if(!CanCheckUnCheckFor(selected_items, count))
        //			return ;

        CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(first_selected_item);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("pseq is NULL"));
            DEL_PTR_ARY(selected_items);
            return ;
        }

        //+Patni-PJS/2010Aug24/Modified/TMSC-REDMINE-599

        /*if (pseq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) {
            //Patni-ARD/2010Apr29/Added/JaFT# MCM0229-00178_CDR_06 + code review + memory leak
            DEL_PTR_ARY(selected_items);
            return ;
        }*/
        if (m_scan_index != -1 && (pseq->GetAcqStatus() != PR_ACQ_STATUS_WAIT)) {
            //Patni-ARD/2010Apr29/Added/JaFT# MCM0229-00178_CDR_06 + code review + memory leak
            DEL_PTR_ARY(selected_items);
            return ;
        }

        //-Patni-PJS/2010Aug24/Modified/TMSC-REDMINE-599
        //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
        //No need to refresh GUI button status so changing the call
        //SetWaitCursor(true);
        SetWaitCursor(true, false);
        //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

        //+Patni-Abhishek/2010Dec14/Modified/IR-180
        ApplyContrastSettingsToItems(selected_items, count, contrast_agent_index, f_bContrastAgentMenu);

        if (GetSeqInfoDlgStatus()) {

            m_info_seq_dlg->DisplaySeqInformation();
        }

        //-Patni-Abhishek/2010Dec14/Modified/IR-180

        //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
        //No need to refresh GUI button status so changing the call
        //SetWaitCursor(false);
        SetWaitCursor(false, false);
        //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

        //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
        DEL_PTR_ARY(selected_items);
        //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnOrienationChangedFor(
    const OrientationType orientation_type
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnOrienationChangedFor");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!m_patientorientationdialog) {

        //        ASSERT(FALSE);
        return;
    }

    if (m_pqm_listctrl.GetSelectedCount() == 0)
        return ;

    int orientation_value = -1 ;

    switch (orientation_type) {

        case OriType_PatientInsertDir:

            switch (m_patientorientationdialog->GetPatientOrientationOption1()) {

                case Patient_FeetFirst :
                    orientation_value = VFC_PATORI_HEAD_FIRST ;
                    break ;

                case Patient_HeadFirst :
                    orientation_value = VFC_PATORI_FEET_FIRST ;
                    break ;
            }

            break ;

            //+Patni-HAR/2009April01/Added/Defect#341
        case OriType_PatientRotationDir:


            //+Patni-Hemant/2009Nov16/Modified/IR#90/Patient Orientation
            //
            switch (m_patientorientationdialog->GetPatientOrientationOption2()) {
                case PatientRotation_SUPINE:
                    orientation_value = VFC_PATORI_SUPINE;
                    break;

                case PatientRotation_LEFT_UP:
                    orientation_value = VFC_PATORI_LEFT_UP;
                    break;

                case PatientRotation_PRONE:
                    orientation_value = VFC_PATORI_PRONE;
                    break;

                case PatientRotation_RIGHT_UP:
                    orientation_value = VFC_PATORI_RIGHT_UP;
                    break;
            }

            //
            //-Patni-Hemant/2009Nov16/Modified/IR#90/Patient Orientation

            //orientation_value = (int) m_patientorientationdialog->GetPatientOrientationOption2() ;
            break ;

        case OriType_PatientViewDir:

            switch (m_patientorientationdialog->GetPatientOrientationOption3()) {
                case PatientView_FROM_HEAD:
                    orientation_value = VFC_VIEW_FROM_HEAD;
                    break;

                case PatientView_FROM_FEET:
                    orientation_value = VFC_VIEW_FROM_FEET;
                    break;
            }

            //orientation_value = (int) m_patientorientationdialog->GetPatientOrientationOption3() ;
            //-Patni-HAR/2009April01/Added/Defect#341
            break ;
    }

    if (orientation_value == -1) {

        //        ASSERT(FALSE);
        return ;
    }

    SetPatientOrientationFor(orientation_type, orientation_value) ;

    for (int i = 0; i < m_pqm_listctrl.GetItemCount() ; i++) {
        UpdateScanOffsetInRange(i);
    }
}


//************************************Method Header************************************
// Method Name  : OnToggleMoveCouchOption
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when user clicks on Move Couch button for the f_row_index
//                Movecouch is applied as per f_row_index state (selected or not selected)
//                Move couch will be previous value of f_row_index if current value is OFF
//                Otherwise it will be OFF
//***********************************************************************************
void CPQMView::OnToggleMoveCouchOption(
    const int f_row_index
)
{
    if (!IsValidSequenceIndex(f_row_index) || m_iswait_cursor) {
        return ;
    }

    CSequence* l_seq_clicked = (CSequence*) m_pqm_listctrl.GetItemData(f_row_index);

    MOVECOUCH_METHOD l_new_movecouch_method = NOMOVE;

    if (l_new_movecouch_method == l_seq_clicked->GetMoveCouchMethodOption()) {
        l_new_movecouch_method = (NOMOVE == l_seq_clicked->GetPrevMoveCouchMethodOption()) ? CENTREPLAN : l_seq_clicked->GetPrevMoveCouchMethodOption();
    }

    UpdateMoveCouchMethod(l_new_movecouch_method, f_row_index);
}



//+Patni-Hemant/2010Mar15/Added/ACE-Phase#3/CDS Requirement
void CPQMView::OpenCoilSelectionDialog(
    const bool f_cds_mode /*= false*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OpenCoilSelectionDialog");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ ACE / Defect Fix
    //following will dispaly the error message when coil database is not found
    if (!m_head) {
        //Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_head is NULL"));

        //Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPQMUtility::GetMultiLingualString(_T("IDS_CS_ERR_NO_COIL_DB")),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));

        return ;
    }

    if (!f_cds_mode && IsNextProtocolAutoScanAfterStatusDone() && (-1 == CheckAndGetWaitingForAutoScanStartAcqOrder())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not open coil dialog as next protocol in autoscan."));
        return;
    }

    //-Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ ACE / Defect Fix

    if (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Coil selection dialog is already invoked."));
        dlgCoilSelWin->ShowWindow(SW_SHOW);
        return ;
    }

    const int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem() ;

    if (first_selected_item == -1) {
        //Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("first_selected_item == -1"));
        return ;
    }

    CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(first_selected_item);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
        return ;
    }

    //+Patni-Manish/2009Jun8/Added/JFT#250/Disable COIL button
    if (!IsAcqumanReadyForNextRequest()) {
        //+Patni-Manish/2009Jul3/Added/IDS#650-Enable COIL button on schedule page
        if (m_pqm_g.GetPQMMode() == K_IMAGING) {
            m_scan_start_button.EnableWindow(false);
            m_coil_button.EnableWindow(false);
            return;

        } else {
            m_coil_button.EnableWindow(true);
        }

        //-Patni-Manish/2009Jul3/Added/IDS#650-Enable COIL button on schedule page
    }

    //-Patni-Manish/2009Jun8/Added/JFT#250/Disable COIL button

    //+Patni-Manishkumar-July29-MVC004029
    //+Patni-Hemant/2010Mar15/Modified/ACE-Phase#3/CDS Requirement
    //if (!CanChangeSar()) {
    if (!f_cds_mode && !CanChangeSar()) {
        //-Patni-Hemant/2010Mar15/Modified/ACE-Phase#3/CDS Requirement

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CanChangeSar returning from function"));
        return;
    }

    //-Patni-Manishkumar-July29-MVC004029

    //new coil updated..- HIMANSHU - AMIT san..

    CRect rect_for_utility_button ;
    m_coil_button.GetWindowRect(&rect_for_utility_button);

    //+Patni-Hemant/2010Apr5/Added/ACE-Phase#3/CDS Requirement
    if (m_selection_utiliy) {

        //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_16
        m_selection_utiliy->SetCoilSelMode(f_cds_mode ? COIL_SEL_CDS : COIL_SEL_NORMAL);
    }

    //-Patni-Hemant/2010Apr5/Added/ACE-Phase#3/CDS Requirement

    const UINT dialog_id = (m_axis_locator_position == 0) ? IDD_COIL_SELECTION_WINDOW_LEFT : IDD_COIL_SELECTION_WINDOW_RIGHT ;

    if (!dlgCoilSelWin) {

        m_selection_utiliy->SetPQMMode(m_pqm_g.GetPQMMode()) ;
        m_axislocatordialog_interface = new CAxisLocatorDialogInterface(this) ;

        dlgCoilSelWin = new CCoilSelWin(

            m_selection_utiliy,
            m_axislocatordialog_interface,
            dialog_id,
            NULL,
            (m_axis_locator_position == 0) ? COUCH_POSITION_LEFT : COUCH_POSITION_RIGHT

        );

        CCoilSelectionUtility* l_utility = dlgCoilSelWin->GetSelectionUtility();
        l_utility->SetHead(m_head);
        l_utility->SetTrestleHead(m_coil_trestle_head);

        for (int index = 0; index < 7 ; index++) {
            l_utility->SetCouchHead(m_coil_couch_head[index], index);
        }

        for (index = 0; index < 2 ; index++) {
            l_utility->SetLinearHead(m_coil_liner_head[index], index);
        }
    }

    //+TMSC-Tanoue/2010Sep03/MVC008324
    dlgCoilSelWin->SetWindowCaption(GetCoildDialogStringFor(l_seq));
    //-TMSC-Tanoue/2010Sep03/MVC008324

    BOOL l_create_flag = dlgCoilSelWin->Create(dialog_id, this);

    //+Patni-Hemant/2010Mar24/Modified/ACE-Phase#3/CDS Requirement
    //dlgCoilSelWin->ShowWindow(SW_SHOW);
    if (IsWindowVisible()) {
        dlgCoilSelWin->ShowWindow(SW_SHOW);

    } else {
        m_is_coil_window_opened_before_swithcing_page = true ;
    }

    m_acquisition->SetActiveDialog(IS_COIL_SELECTION_DIALOG, true);
    //-Patni-Hemant/2010Mar24/Modified/ACE-Phase#3/CDS Requirement

    dlgCoilSelWin->UpdateWindow();

    const int l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
    m_acquisition->SendMessageToAcqManForCoil(m_pqm_g.GetPQMMode(), l_acq_order);

    if (l_create_flag) {
        m_acquisition->SendCoilSelReadyToIPC(l_acq_order);
    }

    EnableDisableScanRelatedButtonsForSelectedProtocolStatus();

}
//-Patni-Hemant/2010Mar15/Added/ACE-Phase#3/CDS Requirement


//**************************************************************************
//Method Name   :  UpdateCurrentScanMasterSlave()
//Author        :  PATNI/AKR
//Purpose       :  This function will update master slave group id and flag
//**************************************************************************
void CPQMView::UpdateCurrentScanMasterSlave()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateCurrentScanMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    int l_protocol_index_to_update = -1;
    m_acquisition->GetProtocolIndexToUpdate(&l_protocol_index_to_update);

    UpdateMasterSlaveCtrls(&l_protocol_index_to_update, 1);

}
//**************************************************************************
//Method Name   :  OnDeleteUpdateMasterSlaveOnGUI()
//Author        :  PATNI/AKR
//Purpose       :  This function will update master slave group id and flag on Delete
//**************************************************************************
void CPQMView::OnDeleteUpdateMasterSlaveOnGUI()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnDeleteUpdateMasterSlaveOnGUI");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_item_count = m_pqm_listctrl.GetItemCount();
    int* l_items = new int[l_item_count];

    if (NULL == l_items) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_items is NULL."));
        return;
    }

    for (int l_index = 0; l_index < l_item_count; l_index++) {
        l_items[l_index] = l_index;
    }

    UpdateMasterSlaveCtrls(l_items, l_item_count);
    DEL_PTR_ARY(l_items);
}
//**************************************************************************
//Method Name   :  UpdateGUICtrlAutoCopyMasterSlave()
//Author        :  PATNI/Varun
//Purpose       :  This function is used to update PQM GUI each after completing AutoCopy functionality
//**************************************************************************
void CPQMView::UpdateGUICtrlAutoCopyMasterSlave()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateGUICtrlAutoCopyMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_current_prot_index = m_pqm_listctrl.GetWSEEditingProtocolIndex();

    VARIANT l_pdata;
    l_pdata.intVal = l_current_prot_index;

    HRESULT l_result = m_acquisition->GetProtocolInfoForAutoCopy(&l_pdata);

    if (S_OK != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetProtocolInfoForAutoCopy failed."));
        return;
    }

    int l_protocol_count = l_pdata.intVal;
    int* l_slave_protocols_index = (int*)l_pdata.byref;

    if (!l_protocol_count || !l_slave_protocols_index) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("no slave protocol."));
        return;
    }

    for (int l_index = 0; l_index < l_protocol_count; l_index++) {	// all slaves

        l_current_prot_index = l_slave_protocols_index[l_index];

        if (!IsValidSequenceIndex(l_current_prot_index)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("invalid sequence index"));
            break;
        }

        GetSequenceInformation(l_current_prot_index);
        m_pqm_listctrl.UpdateSequences(&l_current_prot_index, 1);
    }

    //Deallocate memory of slaves protocol
    DEL_PTR_ARY(l_slave_protocols_index);
}
//**************************************************************************
//Method Name   :  UpdateGroupNum()
//Author        :  PATNI/Varun
//Purpose       :  This function is used to update group num tag
//**************************************************************************
void CPQMView::UpdateGroupNum()
{
    m_acquisition->UpdateGroupNum();
}

//**************************************************************************
//Method Name   :  AllocateGroupIdForDuplicateProtocols()
//Author        :  PATNI/Varun
//Purpose       :  This function is used to allocate group id for duplicated protocol
//**************************************************************************
int* CPQMView::AllocateGroupIdForDuplicateProtocols(const int* f_selected_items, const int f_count)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::AllocateGroupIdForDuplicateProtocols");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VARIANT l_pdata;

    l_pdata.byref = (void*)f_selected_items;
    l_pdata.wReserved1 = f_count;
    m_acquisition->AllocateGroupIdForDuplicateProtocols(&l_pdata);

    int* l_new_groupids = (int*)l_pdata.byref;
    return l_new_groupids;
}
//**************************************************************************
//Method Name   :  OnDeleteUpdateMasterSlave()
//Author        :  PATNI/Varun
//Purpose       :  This function is used to update group id
//**************************************************************************
int CPQMView::OnDeleteUpdateMasterSlave(const int* f_selected_items, const int f_count)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnDeleteUpdateMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VARIANT l_pdata;

    l_pdata.byref = (void*)f_selected_items;
    l_pdata.wReserved1 = f_count;
    m_acquisition->OnDeleteUpdateMasterSlave(&l_pdata);
    int l_can_update_group_num = l_pdata.boolVal;
    return l_can_update_group_num;
}
//****************************Method Header************************************
//Module         : ShowTimerButton()
//Author         : PATNI
//Purpose        : This function will just show timer if timer thread is already present.
//*****************************************************************************
void CPQMView::ShowTimerButton()
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::ShowTimerButton"), _T("Trace"));

    if (m_dw_start_count) {
        m_inject_timer_button.ShowWindow(SW_SHOW);
    }
}
//****************************Method Header************************************
//Module         : DisplayTimer()
//Author         : PATNI
//Purpose        : This function is called when timer display msg in recieved and it
//				   will create a CTimer to start timer on inject button.
//*****************************************************************************
void CPQMView::DisplayTimer()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayTimer");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int l_min_value = m_inject_timer_time / NUM_60;
    const int l_sec_value = m_inject_timer_time % NUM_60;

    CString l_str_inject_time(_T(""));
    l_str_inject_time.Format(_T("%02d:%02d"), l_min_value, l_sec_value);

    m_inject_timer_button.SetWindowText(l_str_inject_time);

    const int l_timer_interval = INJECT_TIMER_INTERVAL;
    m_dw_start_count = GetTickCount() ;

    SetTimer(WM_CMD_INJECT_TIMER, l_timer_interval, NULL);
    m_inject_timer_button.ShowWindow(SW_SHOW);
}

//****************************Method Header************************************
//Module         : CalculateTimeDiff()
//Author         : PATNI
//Purpose        : This function is called at the time of reloading when injected time is present at root level
//				   It will get the time difference of current time and injected time from IPC.
//*****************************************************************************
void CPQMView::CalculateTimeDiff()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CalculateTimeDiff");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMView::CalculateTimeDiff"));

    VARIANT l_pdata;

    m_acquisition->CalculateTimeDiff(&l_pdata);

    m_inject_timer_time = l_pdata.intVal;
}


//************************************Method Header************************************
// Method Name  : GetProtectedStatus
// Author       : IGATE/SS
// Purpose      : Read protected tag value
//***********************************************************************************
BOOL CPQMView::GetProtectedStatus(BSTR bStr)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetProtectedStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMView::GetProtectedStatus"));

    if (S_OK == m_acquisition->GetProtectedStatus(bStr)) {
        return TRUE;

    } else {
        return FALSE;
    }
}

//************************************Method Header************************************
// Method Name  : GetCurrentDelayTime
// Author       : PATNI-AKR/NFD002_Demo_CR-3
// Purpose      :
//***********************************************************************************
CString CPQMView::GetCurrentDelayTime()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetCurrentDelayTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CString l_delay_time(_T(""));
    CSequence* l_pseq = GetSelectedSequence(GetSelectedSeqIndex());

    if (l_pseq) {
        l_delay_time = l_pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("DELAYTIME"));
    }

    return l_delay_time;
}
//************************************Method Header************************************
// Method Name  : SaveDelayTime
// Author       : PATNI-AKR/NFD002_Demo_CR-3
// Purpose      : save delay time in list ctrl and VfDB
//***********************************************************************************
void CPQMView::SaveDelayTime(const CString& f_delay_time)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SaveDelayTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("Trace"));

    const int l_selected_index = GetSelectedSeqIndex();
    const int l_scan_index = GetScanIndex();

    if (l_selected_index == l_scan_index) {

        CString l_err_msg =  CPQMUtility::GetMultiLingualString(_T("IDS_DELAY_TIME_CANNOT_SET"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_err_msg, MPlus::EH::Information, _T("PQM"), _T("PQM"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delay time cannot be set for current protocol..!!"));
        return;
    }

    VARIANT l_pdata ;

    l_pdata.intVal = GetAcqOderForGivenProtocolIndex(l_selected_index);
    l_pdata.wReserved1 = m_delay_time_dlg->ConvertTimeStrToInt(f_delay_time);

    if (S_FALSE == m_acquisition->SetDelayTimeToVf(&l_pdata)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("failed to SetDelayTimeToVf...return!!"));
        return;
    }

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_selected_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL...return!!"));
        return;
    }

    l_seq->SetDelayTimeString(f_delay_time);
    l_seq->GetCPASCOMSequence()->GetSequenceParam()->SetString(DELAYTIME , f_delay_time);
    //Update listctrl
    m_pqm_listctrl.InvalidateItemsForDelayTime(&l_selected_index, 1);
}
//************************************Method Header************************************
// Method Name  : GetEditDelayDlgStatus
// Author       : PATNI-AKR/NFD002_Demo_CR-3
// Purpose      : Get the status of Delay Dialog
//***********************************************************************************
BOOL CPQMView::GetEditDelayDlgStatus()
{
    if ((m_delay_time_dlg && ::IsWindow(m_delay_time_dlg->m_hWnd))) {
        return m_delay_time_dlg->IsWindowVisible();
    }

    return FALSE;
}

//**************************************************************************
//Method Name   :  UpdateSelectedSeqMasterSlave()
//Author        :  PATNI/AKR
//Purpose       :  This function will update master slave group id and flag for selectd protocol
//**************************************************************************
void CPQMView::UpdateSelectedSeqMasterSlave()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateSelectedSeqMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    int l_selected_seq_index = GetSelectedSeqIndex();

    UpdateMasterSlaveCtrls(&l_selected_seq_index, 1);

}

//**************************************************************************
//Method Name   :  UpdateOKBtnStatusofWirelssDlg()
//Author        :  PATNI
//Purpose       :
//**************************************************************************
void CPQMView::UpdateOKBtnStatusofWirelssDlg()
{

    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateOKBtnStatusofWirelssDlg");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (IsNextProtocolAutoScanAfterStatusDone()) {
        return;
    }

    m_wirelessgating_cntlr.UpdateOKBtnStatus();

}

//**************************************************************************
//Method Name   :  UpdateWaitForAutoScanIcon()
//Author        :  PATNI/NILESH
//Purpose       :  This function will update Wait For Auto Scan Icon.
//**************************************************************************
void CPQMView::UpdateWaitForAutoScanIcon()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateWaitForAutoScanIcon");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    int l_first_wait_seq_index = -1;
    GetIndexOfFirstWaitStateProtocol(l_first_wait_seq_index);

    m_pqm_listctrl.InvalidateItemsForStatus(l_first_wait_seq_index);

}

void CPQMView::UpdateCFARetry()
{

    if (m_cfadlg) {
        m_cfadlg->UpdateCFARetry();
    }
}
//************************************Method Header************************************
// Method Name  : UpdatePQMGUIForSAROver
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
void CPQMView::UpdatePQMGUIForSAROver()
{
    m_pqm_listctrl.InvalidateItemsForSARControl();

    if (GetSeqInfoDlgStatus()) {
        m_info_seq_dlg->DisplaySeqInformation();
    }
}

//************************************Method Header**********************************
// Method Name  : StartTimerForPager
// Author       : PATNI/
// Purpose      :
//***********************************************************************************
void CPQMView::StartTimerForPager()
{

    if (S_OK != m_acquisition->GetMaxTimeForPagerTimer(&m_max_time_for_pager)) {
        m_max_time_for_pager = 0;
        return;
    }

    m_dw_start_count_for_pager = GetTickCount() ;
    KillTimer(WM_CMD_PAGER_TIME_UPDATE);
    SetTimer(WM_CMD_PAGER_TIME_UPDATE, m_timer_pager_area, NULL);
}
//************************************Method Header**********************************
// Method Name  : StopTimerForPager
// Author       : PATNI/
// Purpose      :
//***********************************************************************************
void CPQMView::StopTimerForPager()
{
    m_max_time_for_pager = 0;
    m_dw_start_count_for_pager = 0;
    KillTimer(WM_CMD_PAGER_TIME_UPDATE);
}

/****************************Method Header************************************
//Method Name   :HandleSARControlConnectionStatus
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::HandleSARControlConnectionStatus(const bool f_sar_mgr_connection_status)
{
    m_sar_connect_status = f_sar_mgr_connection_status;

    if (SARCTRL_LICENCE_ON != m_sar_control_license) {
        return;
    }

    if (!f_sar_mgr_connection_status) {
        StopTimerForPager();
        CloseWSE();
        ResetAMBPlanProtocols();
    }

    UpdatePQMGUIForSAROver();
    EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
    EnableDisableScanEditOrPlanButtons();
}

//************************************MethodHeader************************************
// Method Name  : GetSARControlLicenseAtStartUp()
// Author       : iGate/Jayanthi
// Purpose      :
//***********************************************************************************
BOOL CPQMView::GetSARControlLicenseAtStartUp()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetSARControlLicenseAtStartUp");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return (S_OK == m_acquisition->GetSARControlLicense()) ? TRUE : FALSE;
}
//************************************Method Header************************************
// Method Name  : OnClickButtonDelete
// Author       : PATNI/ HEMANT
// Purpose      : deleting the protocols..
//***********************************************************************************
void CPQMView::OnClickButtonDelete(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonDelete");

    if (!m_iswait_cursor) {

        //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol Deletion Start"));

        SetWaitCursor(true);

        //+Patni-Hemant-MP/2011Mar25/Modified/TMSC-REDMINE-1486-Part1
        {
            CScopedLock l_scoped_lock(&m_deletion_cs);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering m_deletion_cs"));

            EnableDisableScanRelatedButtonsForSelectedProtocolStatus();

            if (EnableDisableDeleteButton() || IsAMBInvoked()) {

                //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
                __int64 l_freq = 0, l_stop = 0, l_start = 0;
                CPQMLogMgr::GetInstance()->PrintTime(_T("PQM_Deletion_Start"));
                QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif
                //- Patni-RJ/2009Mar29/Added/PSP- Performance code

                m_pqm_listctrl.DeleteSelectedProtocols();

            } else {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not Delete Selected Protocols - Delete Button Disabled"));
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Leaving m_deletion_cs"));
        }
        //-Patni-Hemant-MP/2011Mar25/Modified/TMSC-REDMINE-1486-Part1

        //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
        QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);

        CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, _T("OnClickButtonDelete"));
        CPQMLogMgr::GetInstance()->PrintTime(_T("PQM_Deletion_End"));
        CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, _T("OnClickButtonDelete"), TRUE);
#endif
        //- Patni-RJ/2009Mar29/Added/PSP- Performance code

        //also update the selection status..
        OnSelectionStatusChanged();

        SetMismatchErrorFlag((TYPE_FALSE == GetGUIListProtocolInfo()) ? TRUE : FALSE);

        SetWaitCursor(false);

        m_tiprep_app_cntl.SendRefreshMsgToTiPrep();
        //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol Deletion End"));
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonSetAll(
)
{

    int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (first_selected_item < 0) {
        return ;
    }

    int count = (m_pqm_listctrl.GetItemCount() - first_selected_item) ;
    int* selected_items = new int[count ] ;
    int counter = 0 ;

    for (int item = first_selected_item; counter < count; item++)
        selected_items[counter++] = item;


    if (selected_items && counter > 0) {

        CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(first_selected_item);
        const int holdvalue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

        // for auto transfer
        //Patni-Hemant+ARD/2009Nov06/Modified/IR 85 Auto Transfer // Incorporating Offshore Comments
        const int l_auto_transfer_value =  l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("APOST"));
        ApplyHoldValueAndAutoTranfferTo(selected_items, count, holdvalue, l_seq->GetMoveCouchMethodOption(), l_auto_transfer_value);
    }

    DEL_PTR_ARY(selected_items)
}

//************************************Method Header**************************************

//Method Name:  OnClickButtonScanResume

//Author:  PATNI/GP

//Purpose: Resume Scan

//*************************************************************************************
void CPQMView::OnClickButtonScanResume(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonScanResume");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!m_pause_resume_button.IsWindowEnabled()) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Pause-Resume is disabled"));
        return ;
    }

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_RESUME);

    m_acquisition->ResumeScan();

    m_current_scan_opration = CurrentScanOpration_ScanResume;
}

//************************************Method Header**************************************
//Method Name:  OnClickVoiceMenuItem
//Author:  PATNI/DJ
//Purpose: This is a command handler for handling all voice menu language option.
//**************************************************************************************
//+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
void CPQMView::OnClickVoiceMenuItem(
    const UINT nID
) const
//-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_ID_MENU_VOICE);

    if ((nID >= START_VOICEMENU_ITEM) && (nID <= START_VOICEMENU_ITEM_MAX)) {

        int voiceid = 0;

        switch (nID) {

            case START_VOICEMENU_ITEM : {
                voiceid = PQM_AV_VOICE_1;
            }
            break ;

            case(START_VOICEMENU_ITEM +1): {
                voiceid = PQM_AV_VOICE_2;
            }
            break ;

            case(START_VOICEMENU_ITEM +2): {
                voiceid = PQM_AV_VOICE_3;	// Fix Himanshu
            }
            break ;

            case(START_VOICEMENU_ITEM +3): {

                voiceid = PQM_AV_VOICE_4;
            }
            break ;

            case(START_VOICEMENU_ITEM +4): {
                voiceid = PQM_AV_VOICE_5;
            }
            break ;
        }

        if (voiceid == 0) {
            return ;
        }

        GetAcquisition()->ConnectVoiceManager(voiceid);
    }
}

//************************************Method Header**************************************


//Method Name:  OnClickButtonScanStart

//Author:  PATNI/SVP

//Purpose: Starts scan.

//******************************************************************************
void CPQMView::OnClickButtonScanStart(
    const bool f_is_irs_msg /*= false*/
)
{
    //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
    if (!f_is_irs_msg && (S_OK == m_acquisition->RemoteCheckActiveFlag()) && !m_amb_controller.IsAMBInvoked()) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");

        m_scan_lock_acquired = false;
        //Patni-PJS/2010May21/Added/MaFT#TMSC-REDMINE-343

        RemoevAutoScanFromFirstWaitItem();
        SetWaitCursor(false);

        return;
    }

    if (!GetAMBPlanScanStartCheck())
        ScanStart();
}

void CPQMView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMView)
    //IDC_LIST_PARAMETER is now  IDC_PQM_SEQ_LISTCTRL: by Hemant..
    DDX_Control(pDX, IDC_PQM_SEQ_LISTCTRL   , m_pqm_listctrl);

    DDX_Control(pDX, IDC_END_EXQAM			, m_end_exam_button);
    DDX_Control(pDX, IDC_ABORT				, m_abort_button);
    DDX_Control(pDX, IDC_CLOSE				, m_close_button);


    DDX_Control(pDX, IDC_PAUSE_RESUME		, m_pause_resume_button);
    DDX_Control(pDX, IDC_SCAN_START			, m_scan_start_button);
    DDX_Control(pDX, IDC_UTILITY_BUTTON		, m_utility_button);

    DDX_Control(pDX, IDC_LOCATOR_BUTTON		, m_locator_button);
    DDX_Control(pDX, IDC_INJECT_TIME_BUTTON	, m_injecttime_button);
    DDX_Control(pDX, IDC_SCAN_EDIT_BUTTON	, m_scanedit_button);
    DDX_Control(pDX, IDC_PASS_BUTTON		, m_pass_button);
    DDX_Control(pDX, IDC_SCAN_OFFSET_BUTTON , m_scan_offset_button);
    DDX_Control(pDX, IDC_COIL_BUTTON		, m_coil_button);
    DDX_Control(pDX, IDC_DELETE_BUTTON		, m_delete_button);
    DDX_Control(pDX, IDC_DUPLICATE_BUTTON   , m_duplicate_button);
    DDX_Control(pDX, IDC_SCAN_OFFSET_STATIC	, m_scan_offset_static);
    DDX_Control(pDX, IDC_COIL_INFO_STATIC	, m_coil_info_static);
    //Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons
    //DDX_Control(pDX, IDC_SCAN_ANATOMY_STATIC	, m_scan_anatomy_static);

    DDX_Control(pDX, IDC_SAR_SCAN_ANATOMY_RGN_BUTTON	, m_scan_anatomy_button);
    DDX_Control(pDX, IDC_REMAINING_TIME_STATIC	, m_remaining_time_static);

    DDX_Control(pDX, IDC_BUTTON_ANATOMY	, m_scan_sar_button); //Manish - Prototype

    DDX_Control(pDX, IDC_SAR_ANATOMY_STATIC2	, m_sar_anatomy_static); //Manish - Prototype


    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    DDX_Control(pDX, IDC_QUEUE_AND_NEXT , m_queue_and_next);
    DDX_Control(pDX, IDC_QUEUE_AND_EXIT, m_queue_and_exit);
    DDX_Control(pDX, IDC_NEXT_AND_COPY, m_next_and_copy);
    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

    //+Patni-KSS/2010June16/ADDED/IR-155
    DDX_Control(pDX, IDC_CANCEL, m_close);
    //-Patni-KSS/2010June16/ADDED/IR-155

    DDX_Control(pDX, IDC_RESET, m_reset);
    DDX_Control(pDX, IDC_INJECT_TIMER_BUTTON, m_inject_timer_button);

    //}}AFX_DATA_MAP
}


//************************************Method Header**************************************

//Method Name:  OnInitialUpdate

//Author:  PATNI/SVP

//Purpose: To initialize dialog box controls.

//**************************************************************************************/
void CPQMView::OnInitialUpdate(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnInitialUpdate");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CFormView::OnInitialUpdate();
    ResizeParentToFit(FALSE);
    // Parent window style.
    ModifyStyle(NULL, WS_CLIPCHILDREN);
    // child window style
    m_pqm_listctrl.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scan_offset_static.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_coil_info_static.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scan_anatomy_static.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_remaining_time_static.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_end_exam_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_abort_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_close_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_pause_resume_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scan_start_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_utility_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_pass_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scanedit_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_injecttime_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_locator_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_duplicate_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_delete_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scan_offset_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_coil_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scan_anatomy_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_scan_sar_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_queue_and_next.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_queue_and_exit.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_next_and_copy.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_reset.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_close.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);
    m_inject_timer_button.ModifyStyle(NULL, WS_CHILD | WS_CLIPSIBLINGS);

    // + SM4 Register App AMIT
    if (!((CPQMApp*)AfxGetApp())->IsRunEmbadded()
        && !((CPQMApp*)AfxGetApp())->IsRunAutomated()
       ) {
        _exit(0);
    }

    // - SM4 Register App AMIT

    g_MainView = (CPQMView*)this;

    //Patni-ARD/2010Jan6/Added/DeFT# IR 100
    m_check_load_study = false;

    ////COM //////////////////////////////////////////////////////////////////////
    m_result = CoInitialize(NULL);

    if (FAILED(m_result)) {

        CString string = _T("");
        string.LoadString(IDS_ERR_DLLFAILED);
        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(string, ERROR_LOG_CRUCIAL1,
                                 _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        l_pqm_mgr->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DLLFAILED")),
                                _T("PQMView.cpp"),
                                __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);

        AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE, 0, 0);
        return;
    }

    m_result = m_acquisition.CoCreateInstance(CLSID_PqmIpcCom);



    if (FAILED(m_result)) {

        CString string = _T("");
        string.LoadString(IDS_ERR_DLLFAILED);
        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(string,
                                 ERROR_LOG_CRUCIAL1 , _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        l_pqm_mgr->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DLLFAILED")),
                                _T("PQMView.cpp"),
                                __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);

        AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE, 0, 0);
        return;
    }

    m_result = m_acquisition->QueryInterface(IID_IConnectionPointContainer,
               (void**) & m_cp_container);

    if (FAILED(m_result)) {

        CString string = _T("");
        string.LoadString(IDS_ERR_DLLFAILED);
        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(string,
                                 ERROR_LOG_CRUCIAL1 , _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        l_pqm_mgr->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DLLFAILED")),
                                _T("PQMView.cpp"),
                                __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);
        AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE, 0, 0);
        return;
    }

    m_result = m_cp_container->FindConnectionPoint(IID__IPqmIpcComEvents,
               &m_connection_point);

    if (FAILED(m_result)) {

        CString string = _T("");
        string.LoadString(IDS_ERR_DLLFAILED);

        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(string,
                                 ERROR_LOG_CRUCIAL1 , _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        l_pqm_mgr->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DLLFAILED")),
                                _T("PQMView.cpp"), __LINE__,
                                ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);

        AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE, 0, 0);
        return;
    }

    m_cp_container->Release();
    m_sink = new CSink(this);

    m_result = m_sink->QueryInterface(IID_IUnknown, (void**) & m_iunknown);

    m_result = m_connection_point->Advise(m_iunknown, &dwAdvise);

    b_connection_established = (FAILED(m_result)) ? false : true;

    InitializePQMListCtrl();
    SetInitialButtonState();

    //Added by Hemant On 6/4/2008 5:42:07 PM
    SetButtonAppearance();
    m_bkbrush.CreateSolidBrush(RGB(0, 0, 0));

    //save the main window rect.. as it will be changed and will show the dimensions of OLEFramewindow..: check..
    GetWindowRect(&m_window_rect);
    GetClientRect(&m_client_rect);
    m_window_rect.OffsetRect(g_child_window_offset);
    OnSelectionStatusChanged();

    SetMultilingual();//Language Usha;

    //+ UT_JIT@WIDE_SCREEN_CHECK
    //Patni-ARD/2010Mar24/Commented/DeFT# IR-100 remove IPCConfig.ini
    //Added by Hemant On 12/11/2008 12:51:22 PM
    //it was happening in contructor,but now here as it required first arg
    //which is ready in this function..
    m_cfadlg = new CPQMCFADlg(m_window_rect, this);

    //+Patni-HAR-HM/2009July22/Added/MVC#2659
    OpenQueueDialog(false);
    //Patni-Hemant/2010May20/Commented/MaFT/PatientOrientation#1
    //OpenPatientOrientationDialog(false);
    //-Patni-HAR-HM/2009July22/Added/MVC#2659

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    m_cds_controller = new CPQMCDSController(this);

    if (m_cds_controller) {
        m_cds_controller->InitCDSController();
    }

    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

    m_wirelessgating_cntlr.SetPQMView(this);

    m_amb_controller.InitializeAMBController(this, m_window_rect);
    CreateWFDAMsgReceiver();

    InitToolTip();
    InitUiButtonUtility();
	//CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"),_T("GL_Pager_GetPageInfo"));
}


//************************************Method Header**************************************

//Method Name:  PostNcDestroy

//Author:  PATNI/GP

//Purpose:

//*****************************************************************************
void CPQMView::PostNcDestroy(
)
{
    CleanUp();
    CFormView::PostNcDestroy();
}
//************************************Method Header************************************
// Method Name  : OnEraseBkgnd
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CBrush* Old_brush = pDC->SelectObject(&m_bkbrush);

    CRect client_rect;
    GetClientRect(&client_rect);
    client_rect.bottom = m_client_rect.bottom ;

    pDC->Rectangle(&client_rect);
    pDC->SelectObject(Old_brush);

    client_rect = m_client_rect;

    CRect rect_for_utility_button ;
    m_utility_button.GetWindowRect(&rect_for_utility_button);

    CRect rect_for_duplicate_button ;

    //+Patni-HEMANT/Modified On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    //GetDlgItem(IDC_DUPLICATE_BUTTON)->GetWindowRect(&rect_for_duplicate_button);
    m_pqm_listctrl.GetWindowRect(&rect_for_duplicate_button);
    //-Patni-HEMANT/Modified On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

    CRect frame_rect;
    frame_rect.UnionRect(&rect_for_utility_button, &rect_for_duplicate_button);
    frame_rect.InflateRect(7, 7);

    ScreenToClient(&frame_rect);

    //Patni-HEMANT/COMMENTED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    //	frame_rect.left = client_rect.left + 2 ;

    frame_rect.bottom = client_rect.bottom - 2 ;

    //Patni-HEMANT/COMMENTED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    //	frame_rect.right = rect_for_duplicate_button.right + 2 ;

    CBrush frame_brush(RGB(15, 18, 29));
    CPen frame_rect_pen(PS_SOLID, 1 , RGB(52, 107, 175));
    Old_brush = pDC->SelectObject(&frame_brush);

    CPen* Old_pen = pDC->SelectObject(&frame_rect_pen);
    pDC->Rectangle(&frame_rect);

    pDC->SelectObject(Old_brush);
    pDC->SelectObject(Old_pen);

    //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    CRect l_coil_text_rect;
    m_coil_info_static.GetWindowRect(&l_coil_text_rect);
    ScreenToClient(&l_coil_text_rect);

    CPen pen(PS_SOLID, 1, RGB(52, 107, 175));

    Old_pen = (CPen*)pDC->SelectObject(&pen);
    Old_brush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
    l_coil_text_rect.InflateRect(4, 1);
    pDC->Rectangle(&l_coil_text_rect);

    pDC->SelectObject(Old_pen);
    pDC->SelectObject(Old_brush);
    //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

    return TRUE;
}

//************************************Method Header************************************
// Method Name  : OnClickScanOffset
// Author       : PATNI/ HEMANT
// Purpose      : The fucntionlaity of this function was called through the PQMMenuDlg::OnClickMenuitemSetScanOffset()
//				  I have just copied the code and modified according to this view..
//***********************************************************************************
void CPQMView::OnClickScanOffset(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickScanOffset");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Added by Hemant On 11/27/2008 11:39:53 AM
    //we are not allowed to enter scan offset in schedule mode..
    if ((m_pqm_g.GetPQMMode() != K_IMAGING) || (NULL == m_patientorientationdialog)) {
        return ;
    }

    bool l_is_multiple_sel = false;
    const int selected_seq_index = m_pqm_listctrl.GetFirstSelectedItem(&l_is_multiple_sel);

    if (l_is_multiple_sel || !IsValidSequenceIndex(selected_seq_index)) {
        return;
    }

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(selected_seq_index);

    Vector_t l_current_scan_offset, l_min_value, l_max_value;
    GetMinMaxValueForScanOffset(l_seq, GetAcqOderForGivenProtocolIndex(selected_seq_index), l_current_scan_offset, l_min_value, l_max_value);

    //Patni-Hemant/2009Nov24/Modified/IR#90/Scan Offset
    //+Patni-ARD/2010Jan21/Modified/DeFT# Code Review Comment
    m_setscanoffsetdlg = new CPQMSetScanOffsetDlg(
        (m_axis_locator_position == 0) ? COUCH_POSITION_LEFT : COUCH_POSITION_RIGHT,
        l_current_scan_offset, l_min_value, l_max_value,
        m_patientorientationdialog->GetPatientOrientationOption1(),
        m_patientorientationdialog->GetPatientOrientationOption2(),
        this);

    //action before scan offset
    const bool auto_scan_on = OnDialogOpenedForParameterEditing(selected_seq_index);

    //+Patni-MSN 2009May-26/Un-Commented/PSP1#371
    //+Patni-Manishkumar 2009April-22/Added/PSP1#371
    ChangeViewOfModelessDialogs(false);

    if (IDOK == m_setscanoffsetdlg->DoModal()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan Offset Dialog Closed"));

        //We will update Scan Offset for Valid first selected Item Only
        const int selected_seq_index = m_pqm_listctrl.GetFirstSelectedItem();

        if (IsValidSequenceIndex(selected_seq_index)) {

            l_seq = (CSequence*) m_pqm_listctrl.GetItemData(selected_seq_index);

            //Update the data...
            UpdateScanOffsetFor(l_seq, m_setscanoffsetdlg->GetScanOffsetVector(), TRUE);

            g_MainView->GetSequenceInformation(selected_seq_index);

            if (m_pqm_listctrl.GetWSEEditingProtocolIndex() == selected_seq_index)  {

                CPASParamStore* l_pparam = l_seq->GetCPASCOMSequence()->GetSequenceParam();

                if ((l_pparam->GetInt(_T("PLANSTATUS")) & VFC_PLN_STATUS_GLOC) == VFC_PLN_STATUS_GLOC) {
                    l_seq->SetWSEEditingEnum(WSE_Planned_AND_Current);

                } else {
                    l_seq->SetWSEEditingEnum(WSE_Current);
                }
            }

            g_MainView->GetPQMListCtrl().UpdateSequences(&selected_seq_index, 1);
            OnSelectionStatusChanged();
        }
    }

    ChangeViewOfModelessDialogs(true);
    //-Patni-Manishkumar 2009April-22/Added/PSP1#371
    //-Patni-MSN 2009May-26/Un-Commented/PSP1#371
    //action after scan offset
    OnDialogClosedForParameterEditing(selected_seq_index, auto_scan_on);

    //Added by Hemant On 1/8/2009 5:38:38 PM
    DEL_PTR(m_setscanoffsetdlg)

}

//************************************Method Header************************************
// Method Name  : OnClickPasButton
// Author       : PATNI/ HEMANT
// Purpose      : The fucntionlaity of this function was called through the PQMMenuDlg::OnClickMenuitemPasGetmem()
//				  I have just copied the code and modified according to this view..
//***********************************************************************************
void CPQMView::OnClickPasButton(
)
{

    //PQMMenuDlg::OnClickMenuitemPasGetmem() function , the view's function were accesed using the
    //view's pointer stored as m_parent in PQMMenuDlg

    //+Patni-Sudhir 2011Mar-09/Modified/IR-185
    if (CanInvokeMemory()) {

        if (!InvokeMemory()) {
            CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
            l_pqm_mgr->WriteEventLog(IDS_ERR_CLIENT_CONNECTION,
                                     ERROR_LOG_CRUCIAL1 , _T("PQMMenuDlg.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82
        }
    }

    //-Patni-Sudhir 2011Mar-09/Modified/IR-185
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickUtilityButton(
)
{

    PopupMenuEx context_menu ;
    context_menu.LoadMenu(IDR_UTILITY_MENU);
    PopupMenuEx* context_sub_menu = (PopupMenuEx*)context_menu.GetSubMenu(0);

    if (NULL == context_sub_menu) {
        return ;
    }

    PopupMenuEx m_voice_popup, test_menu;
    CreateTestMenu(&test_menu);
    CreateVoiceMenu(&m_voice_popup, &test_menu);

    if (m_voice_popup.m_hMenu) {

        CString l_chOutput = CPQMUtility::GetMultiLingualString(_T("Voice"));

        if (!l_chOutput.IsEmpty()) {

            context_sub_menu->ModifyMenu(ID_UTILITY_VOICE, MF_POPUP | MF_BYCOMMAND , (UINT) m_voice_popup.m_hMenu, l_chOutput);
        }

        //Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
        //When Scanning, disable voice menus
        if ((m_pqm_g.GetPQMMode() != K_IMAGING) || IsScanning() || m_rmc_scan_running || IsAMB()) {
            context_sub_menu->EnableMenuItem((UINT) m_voice_popup.m_hMenu, MF_BYCOMMAND | MF_GRAYED);
        }
    }

    LPCTSTR font_name = _T("Trebuchet MS") ;
    const int font_size = 10;
    context_menu.SetFont(font_name, font_size);

    SetStatesForUtilityMenuItems(context_sub_menu);

    CRect rect_for_utility_button ;
    m_utility_button.GetWindowRect(&rect_for_utility_button);

    context_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                rect_for_utility_button.left,
                                rect_for_utility_button.bottom,
                                this, 0, 0);

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickCoilPasButton(
)
{
    OpenCoilSelectionDialog();
}


//************************************Method Header**************************************

//Method Name:  OnClickMenuitemInfoSeq

//Author:  PATNI/SVP

//Purpose: Invokes sequence information dialog window.

//**************************************************************************************/
void CPQMView::OnClickMenuitemInfoSeq(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_INFORMATION_POPUP_SEQ_INFO);


    //+Patni-HEMANT/ADDED On 3/6/2009 8:39:26 PM/PSP #202
    //return when seq info is already open
    if (m_info_seq_dlg && ::IsWindow(m_info_seq_dlg->m_hWnd) && m_info_seq_dlg->IsWindowVisible()) {
        return ;
    }

    //-Patni-HEMANT/ ADDED On3/6/2009 8:41:36 PM/PSP #202


    //Added by Hemant On 7/10/2008 9:53:55 PM
    if (!m_info_seq_dlg) {
        m_info_seq_dlg = new CPQMInfoSeqDlg(this);
    }

    //+Patni-HEMANT/ADDED On 3/6/2009 8:43:31 PM/ PSP#202
    //Create the modeless dialog..
    //It is also commented out in CPQMInfoSeqDlg::CPQMInfoSeqDlg() constructor
    //as it should be here..
    m_info_seq_dlg->Create(IDD_INFO_SEQ, this);

    m_info_seq_dlg->ShowWindow(SW_SHOW);
}


//************************************Method Header**************************************

//Method Name:  OnClickMenuitemInfoCoil

//Author:  PATNI/SVP

//Purpose: Invokes connecting coil information dialog window.

//**************************************************************************************/
void CPQMView::OnClickMenuitemInfoCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuitemInfoCoil");
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_INFORMATION_POPUP_COIL_INFO);

    const int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem() ;

    if (first_selected_item == -1)
        return ;

    CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(first_selected_item);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("l_seq is NULL"));
        return;
    }

    const int l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

    m_acquisition->GetConnectedCoilInformationForGUIDisplay(true);

    m_acquisition->SendMessageToAcqManForCoil(m_pqm_g.GetPQMMode(), l_acq_order);

}



//************************************Method Header**************************************

//Method Name:  OnClickMenuitemInfoGate

//Author:  PATNI/SVP

//Purpose: Invokes gate information dialog window.

//******************************************************************************
void CPQMView::OnClickMenuitemInfoGate()
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_INFORMATION_POPUP_GATE_INFO);
    GetAcquisition()->SendAcqInfoForGate();

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonComment(
)
{
    //+Patni-SS/ADDED/2009-May-22/ PQM Crash on Comment dialog
    if (GetCommentDlgStatus())
        return ;

    if (!m_comment_dlg) {

        //Modified by Hemant On 11/14/2008 12:34:35 PM
        //m_comment_dlg = new CPQMCommentDlg(this);
        m_interfaceclassforscancommentdlg = new CInterfaceClassForScanCommentDlg(this);

        //+Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163
        //m_comment_dlg = new CPQMCommentDlg(m_interfaceclassforscancommentdlg, this);
        m_comment_dlg = new CPQMCommentDlg(m_interfaceclassforscancommentdlg, &m_commentdata, this);
        //-Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163

        //
        //End Of addition By Hemant On11/14/2008 12:34:55 PM
    }

    //+Patni-SS/ADDED/2009-May-22/ PQM Crash on Comment dialog
    m_comment_dlg->Create(IDD_COMMENT, this);
    m_comment_dlg->ShowWindow(SW_SHOW);
    m_comment_dlg->DisplayCommentInformation();
}

//************************************Method Header************************************
// Method Name  : OnClickButtonScanPauseResume
// Author       : PATNI/ HEMANT
// Purpose      : Just the handler function for the pause resume button..: internaly
//				  it simply calls the PQM functions according to status of pause or resume.
//
//***********************************************************************************
void CPQMView::OnClickButtonScanPauseResume(
)
{
    CString FUNC_NAME = _T("CPQMView::OnClickButtonScanPauseResume");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
    if (S_OK == m_acquisition->RemoteCheckActiveFlag()) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PAUSE_PQM_REMOTE_CONSOLE,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PAUSE_PQM_REMOTE_CONSOLE")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");

        return;
    }

    //-Patni-PJS/2010Apr28/Added/JaFT# Redmine-90

    if (!m_pause_resume_button.IsWindowEnabled()) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Pause-Resume is disabled"));
        return ;
    }

    switch (m_current_scan_opration)  {

        case CurrentScanOpration_ScanStart :
        case CurrentScanOpration_ScanResume : {

            //you can only pause it.. when it is in starting mode..or its being resumed.. again..
            //
            //calling the PQM function...for pausing the scan..
            //OnClickButtonScanPause();
            m_current_scan_opration = CurrentScanOpration_Invalid;
            m_pqmchildthread.PostMessageToScanThread(ON_SCAN_PAUSE, NULL, NULL);

            return ;

        }

        case CurrentScanOpration_ScanPause : {

            //you can only resume it.. when current mode is pause..
            //calling the PQM function...for resuming the scan..
            //+Patni-Sribanta/2011Feb09/Modified/REDMINE-1026
            //OnClickButtonScanResume();
            m_current_scan_opration = CurrentScanOpration_Invalid;
            m_pqmchildthread.PostMessageToScanThread(ON_SCAN_RESUME, NULL, NULL);
            //-Patni-Sribanta/2011Feb09/Modified/REDMINE-1026
            return ;
        }

        default: {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_current_scan_opration is Invalid"));
            break;
        }
    }
}


//************************************Method Header**************************************

//Method Name:  OnClickButtonScanPlan

//Author:  PATNI/GP

//Purpose: Scan Plan

//*************************************************************************************
void CPQMView::OnClickButtonScanPlan(
)
{
    //+ Patni-PJS/2009Jun19/Modify/IDS#624,625
    //m_bLocatorAlreadyEnable = true; //Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
    //Added by Hemant On 7/23/2008 4:24:13 PM
    OnScanEditOrPlan(PQM_PROCON_SCAN_PLAN);
    return ;
}


/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CPQMView::OnCancelEditSrvr()
{
    GetDocument()->OnDeactivateUI(FALSE);
}



//************************************Method Header**************************************
//Method Name:  OnClickButtonScanAbort
//Author:  PATNI/SVP
//Purpose: Aborts scan.
//***************************************************************************
void CPQMView::OnClickButtonScanAbort(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonScanAbort");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_ABORT);

    if (S_OK == m_acquisition->RemoteCheckActiveFlag()) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_ABORT_PQM_REMOTE_CONSOLE,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_ABORT_PQM_REMOTE_CONSOLE")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");
        return;
    }

    if (m_mrsprescandlg && ::IsWindow(m_mrsprescandlg->m_hWnd)) {
        m_mrsprescandlg->OnAbortButton();
        return;
    }

    AbortScan();
}

//************************************Method Header**************************************

//Method Name:  OnClickButtonSequenceEditior

//Author:  PATNI/SVP

//Purpose: Invokes scan editior dialog window.

//******************************************************************************
void CPQMView::OnClickButtonSequenceEditior(
)
{
    //Added by Hemant On 7/23/2008 4:24:05 PM
    OnScanEditOrPlan(PQM_PROCON_SCAN_EDIT);
    return ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemAutoPlan(
)
{
    if (m_acquisition) {
        const bool l_auto_plan_flag = !(m_pqm_g.GetAutoPlanFlag());
        m_pqm_g.SetAutoPlanFlag(l_auto_plan_flag);
        m_acquisition->SetAutoPlan(l_auto_plan_flag);
    }
}


//************************************Method Header************************************
// Method Name  : OnClickMenuitemPlanScan
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemPlanScan(
)
{
    if (m_acquisition) {
        const bool l_plan_scan_flag = !(m_pqm_g.GetPlanScanFlag());
        m_pqm_g.SetPlanScanFlag(l_plan_scan_flag);
        m_acquisition->SetPlanAndScan(l_plan_scan_flag);
    }
}

//************************************Method Header************************************
// Method Name  : OnClickMenuitemSkipRgn
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemSkipRgn(
)
{
    if (m_acquisition) {
        const bool l_skip_rgn_flag = !(m_pqm_g.GetSkipRgnFlag());
        m_pqm_g.SetSkipRgnFlag(l_skip_rgn_flag);
        m_acquisition->SetSkipRgn(l_skip_rgn_flag);
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemAllCovRgn(
)
{
    if (m_acquisition) {
        const bool l_all_cov_rgn_flag = !(m_pqm_g.GetAllCoverageRgnFlag());
        m_pqm_g.SetAllCoverageRgnFlag(l_all_cov_rgn_flag);
        m_acquisition->SetAllCoverageRgn(l_all_cov_rgn_flag);
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemSpeedermapAutovoice(
)
{
    //+Patni-Hemant/2010Jun7/Added/MaFT/MVC008166
    //m_pqm_g.SetSpeedermapAutoVoiceFlag(! m_pqm_g.GetSpeedermapAutoVoiceFlag());
    if (m_acquisition) {
        const bool l_SpeedermapAutoVoiceFlag = !(m_pqm_g.GetSpeedermapAutoVoiceFlag());
        m_pqm_g.SetSpeedermapAutoVoiceFlag(l_SpeedermapAutoVoiceFlag);
        m_acquisition->SetAvSpdrMap(l_SpeedermapAutoVoiceFlag);
    }

    //-Patni-Hemant/2010Jun7/Added/MaFT/MVC008166
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemBatchCoilAndSarRgn(
)
{
    SetBatchSettingOnOff(!(m_pqm_g.GetBatchSettingFlag()));
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemGateModeEcg(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuitemGateModeEcg");
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ECG);

    if (GetAcquisition()) {

        if (S_FALSE == GetAcquisition()->SendGateModeInfo(VFC_GAT_METHOD_BIT_ECG)) {

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to Send GateMode Method"));
        }
    }
}

//************************************Method Header************************************
// Method Name  : OnClickMenuitemGateModePeripheral
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemGateModePeripheral(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuitemGateModePeripheral");
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_PERIPHERAL);

    if (GetAcquisition()) {

        if (S_FALSE == GetAcquisition()->SendGateModeInfo(VFC_GAT_METHOD_BIT_PERIPHERAL)) {

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to Send GateMode Method"));
        }
    }
}

//************************************Method Header************************************
// Method Name  : OnClickMenuitemGateModeRespiratory
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemGateModeRespiratory(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuitemGateModeRespiratory");
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_RESPIRATORY);

    if (GetAcquisition()) {

        if (S_FALSE == GetAcquisition()->SendGateModeInfo(VFC_GAT_METHOD_BIT_RESPIRATORY)) {

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to Send GateMode Method"));
        }
    }
}

//************************************Method Header************************************
// Method Name  : OnClickMenuitemWirelessGating
// Author       :
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemWirelessGating(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuitemWirelessGating");
    m_wirelessgating_cntlr.OnClickMenuitemWirelessGating();
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemElectrodeI(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ELECTRODE_I);

    m_acquisition->SendElectrodeType(ELECTRODE_I);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemElectrodeIi(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ELECTRODE_II);

    m_acquisition->SendElectrodeType(ELECTRODE_II);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemElectrodeIii(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ELECTRODE_III);

    m_acquisition->SendElectrodeType(ELECTRODE_III);
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemElectrodeAvf(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ELECTRODE_AVF);

    m_acquisition->SendElectrodeType(ELECTRODE_AVF);

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemElectrodeAvl()
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ELECTRODE_AVL);

    m_acquisition->SendElectrodeType(ELECTRODE_AVL);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemElectrodeAvr(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_POPUP_ELECTRODE_AVR);

    m_acquisition->SendElectrodeType(ELECTRODE_AVR);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemPrepWbCoil(
)
{
    if (m_acquisition) {
        const bool l_prepwb_flag = !(m_pqm_g.GetPrepWbCoilFlag());
        m_pqm_g.SetPrepWbCoilFlag(l_prepwb_flag);
        m_acquisition->SetPrepWbCoil(l_prepwb_flag);
    }
}

//************************************Method Header************************************
// Method Name  : OnClickMenuitemShimmingWbCoil
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemShimmingWbCoil(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(
        OP_IDM_UTILITY_POPUP_SHIMMING_WB_COIL);

    if (m_acquisition) {

        const bool enable = !(m_pqm_g.GetShimmWbCoilFlag());
        m_pqm_g.SetShimmWbCoilFlag(enable);
        GetAcquisition()->SetShimmWBCoil(enable);

    }
}


//************************************Method Header************************************
// Method Name  : OnClickMenuitemCenterFreqCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemCenterFreqCtrl(
)
{

    if (GetShimDoneFlag() == CFA_DLG_SHIMMING_SEQUENCE ||
        GetShimDoneFlag() == CFA_DLG_SHIMMING_DONE) {

        SetShimDoneFlag(CFA_DLG_SHIMMING_DONE);

    } else {

        SetShimDoneFlag(CFA_DLG_NORMAL);
    }

    //+Patni-Manishkumar 2009April-22/Added/PSP1#371
    ChangeViewOfModelessDialogs(false);

    //Patni-PJS/Modified/1Aug2009/Internal defect solved
    m_cfadlg->ResetPendingCFAMessage();
    m_cfadlg->DoModal();

    ChangeViewOfModelessDialogs(true);
    //-Patni-MSN 2009May-26/Un-Commented/PSP1#371
    //-Patni-Manishkumar 2009April-22/Added/PSP1#371
}


//************************************Method Header************************************
// Method Name  : OnClickMenuitemApcRetry
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemApcRetry(
)
{
    //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
    if (S_OK == m_acquisition->RemoteCheckActiveFlag()) {


        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");

        return;
    }

    //-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_APC_RETRY);
    StartRFLPrescan();
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuitemApcPause(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_APC_PAUSE);


    if (GetAcquisition()) {

        //+Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
        bool flag = GetAPCPauseChecked();
        SetAPCPauseChecked(! flag);
        //GetAcquisition()->SetAPCHoldValue(! flag);
        //-Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
    }
}

//************************************Method Header**************************************
//Method Name:  OnCmdOpen
//Author:  PATNI/GP
//Purpose: Invoke GUI and get its information
//**************************************************************************************/
void CPQMView::OnCmdOpen(
    WPARAM wParam,
    LPARAM lParam
)
{

    //Patni-AMT/2010Feb25/Modified/Ph-3# PROPOSAL_06-Code Review
    LPCTSTR FUNC_NAME = _T("CPQMView::OnCmdOpen");
    PQM_TRACE_SCOPED(FUNC_NAME);

    try {

        if (!GetConnectorState()) {
            const CString& temp_string =  CPQMUtility::GetMultiLingualString(
                                              _T("IDS_CONNECTIN_OF_PQM_TO_DPS_ERROR"));

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(temp_string, MPlus::EH::Crucial, APP_NAME, APP_NAME);
            _exit(0);
            return ;
        }

        CPQMUtility::GetInstance()->SetPQMStatus(STATE_READY);
        const CString l_mode = (BSTR)wParam;

        CDPSManager::GetInstance()->SendASyncMsgToParentPage(
            DPS_MSGID_REQ_REPORTSTATE, CPQMUtility::GetInstance()->GetPQMStatusString(),
            GetCurProcessIdString());


#if MPLUS_V3
        //Patni-PJS/2010Apr12/Added/JaFT# MVC007125

        if (RegisterUnregisterProcMgr(TRUE)) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("PQM Registration to Process Manager Successful."));

        } else {
            CPQMLogMgr::GetInstance()->WriteEventLog(_T("PQM Registration to Process Manager Failed."),
                    SYSTEM_FUNC_MARKER, FUNC_NAME, __LINE__, true); // MEITEC/ADDED/2010May19/IR-82
        }

#endif

        const int l_nmode = (l_mode == _T("Acquire")) ? K_IMAGING : K_SCHEDULING ;

        //+Patni-Hemant/2010Jun7/Commented/MaFT/MVC008198
        //        bool l_ret = false;
        //        l_ret = InitializeIPC( l_nmode );
        //-Patni-Hemant/2010Jun7/Commented/MaFT/MVC008198

        //+Patni-ARD/2010Apr27/Added/MaFT#Redmine 190
        GetPQMGlobal().SetPQMMode(l_nmode);
        CPQMLogMgr::GetInstance()->SetModeString(l_nmode);
        GetAcquisition()->SetPQMMode(l_nmode);
        theApp.SetVoiceMenuMode(l_nmode);
        //-Patni-ARD/2010Apr27/Added/MaFT#Redmine 190

        //Patni-Hemant/2010Jun7/Added/MaFT/MVC008198
        bool l_ret = InitializeIPC(l_nmode);

        //Patni/2010Dec03/Added/V2.0/IR-173-Part2
        m_pqm_listctrl.Dictinstance();

        if (false == l_ret) {
            CDPSManager::GetInstance()->SendASyncMsgToParentPage(_T("FW_RequestClose"), _T("PQM"));
        }

        if (true == l_ret) {
            if (CPQMUtility::GetInstance()->GetPQMParentName() == _T("Schedule")) {
                ShowSR();
                g_MainView->HideBigBar();
            }
        }

        //-Patni-ARD/2010Mar29/Modified/Ph-3# IR-100 remove ICPConfig.ini
        SetInitialButtonState();

        //+Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel,MVC004997
        if (!UpdateCoilSystemSetting()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to get sys setting for coil-Use default 16 (8*2) channels"));
        }

        //-Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel,MVC004997


        if (true == l_ret) {
            CoilSelectionGlobal_t* l_coil_selection_global = m_selection_utiliy->GetCoilSelectionGlobal();
            int l_system_enable_channel = l_coil_selection_global->coil.channelPerVFEP * l_coil_selection_global->coil.VAPChannels;
            m_use_coil_channel_mode = CCoilChannelMode::ValidateMaxCoilChannelFile(l_system_enable_channel, m_system_enable_channel, m_limited_channel);

            if (m_use_coil_channel_mode) {

                if (S_FALSE == m_acquisition->SetNumChannelsforChannelMode(m_use_coil_channel_mode, m_system_enable_channel, m_limited_channel)) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in setting system enable channel and limited channel"));
                }
            }
        }

        UpdatePagerArea(_T("FW_ACQINFO_CLR"), _T("1"), _T(""));

    } catch (...) {
        DWORD dw = GetLastError();
        CString l_eventmsg(_T(""));
        l_eventmsg.Format(_T("The last error = %d"), dw);
        //TMSC/2009Nov12/Modified/Code Changes
        CPQMLogMgr::GetInstance()->WriteEventLog(
            l_eventmsg, ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        throw;
        //TMSC/2009Nov12/Modified/Code Changes
    }
}

//************************************Method Header**************************************
//Method Name:  OnCmdClose
//Author:  PATNI/GP
//Purpose: Close GUI application
//**************************************************************************************/
void CPQMView::OnCmdClose(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnCmdClose");


    PQM_TRACE_SCOPED(FUNC_NAME);

    CloseAllModalDialogs();

    m_acquisition->OnPQMExit();


    //+Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
    if (m_pqmdebugdlg.m_hWnd != NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Debug Window destroyed"));
        m_pqmdebugdlg.DestroyWindow();
    }

    //-Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow#

    CPQMUtility::GetInstance()->SetPQMStatus(STATE_READY);

    CDPSManager::GetInstance()->ReplyMsgAtLast(*reinterpret_cast<CCFWReceiveMsg*>(wParam),
            reinterpret_cast<CCFWReplyMsg*>(lParam), _T("S_OK"));

    m_tiprep_app_cntl.CloseConnectionToTiPrep();

}

//************************************Method Header**************************************
//Method Name 	: OnCmdShow
//Author		: PATNI/GP
//Purpose		: Display GUI application
//**************************************************************************************/
void CPQMView::OnCmdShow(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnCmdShow");

    //Patni-ARD/2010Jan29/Added/DeFT# MVC4446
    m_is_locator_open = false;

    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    //Patni-DKH/2010Sep24/Modified/PQM Crash in V1.20
    try {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME,
                  _T("Trace of OnCmdShow"));

        OnSelectionStatusChanged();

        ShowHideSubDialogs(true) ;
        //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
        //To be called when view is shown
        //ShowHideOpenedWindows(true);
        //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

        //+Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
        if (m_acquisition) {
            m_acquisition->OnShowHidePQM(TRUE);
        }

        //-Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect

        GetParentFrame()->ShowWindow(SW_SHOW);

        //Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
        ShowHideOpenedWindows(true);
        //Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
        UpdateActiveDlgStatusToIPC();


        //Patni-Hemant/2010May27/Commented/MaFT/TMSC-REDMINE-MVC007915
        //m_database_type = GetDataBaseType();



        CPQMUtility::GetInstance()->SetPQMStatus(STATE_ACTIVE);

        //Patni-PJS/2010Feb19/Added/DeFT# MVC004689 Code review
        BlinkPage();

        DisplayTotalRemainingTimeOnPQM();

        //+Patni-MN/2009Mar09/Added/PSP1#203
        if (m_is_cfa_window_opened_before_swithcing_page) {
            //After switching from acquire page the CFA dialog needs to be displayed
            if (GetCFADlgPtr() && !::IsWindow(GetCFADlgPtr()->m_hWnd)) {
                PostMessage(WM_CMD_SHOW_CFA_DIALOG, 0, 0); //Patni-MN/2009Mar13/Added/PSP1#203
            }
        }

        //-Patni-MN/2009Mar09/Added/PSP1#203






        if (!this->GetPQMCloseFlag()) {
            this->HideSR();
            return;
        }

        if (CDPSManager::GetInstance()->HaveToShowSR()) {
            this->ShowSR();
        }

    } catch (...) {
        DWORD dw = GetLastError();
        //TMSC/2009Nov12/Modified/Code Changes

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS,
                  FUNC_NAME,
                  _T("Unknown Exception occured"));
        throw;
        //TMSC/2009Nov12/Modified/Code Changes
    }
}

//************************************Method Header**************************************
//Method Name:  OnCmdHide
//Author:  PATNI/GP
//Purpose: Hide GUI application
//**************************************************************************************/
void CPQMView::OnCmdHide(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnCmdHide");
    PQM_TRACE_SCOPED(FUNC_NAME);


    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);


    try {
        if (CDPSManager::GetInstance()->HaveToShowSR()) {
            this->ShowSR();
        }

        //+Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
        if (m_acquisition) {
            m_acquisition->OnShowHidePQM(FALSE);
        }

        //-Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
        GetParentFrame()->ShowWindow(SW_HIDE);
        ShowHideOpenedWindows(false);

        CPQMUtility::GetInstance()->SetPQMStatus(STATE_INACTIVE);

        ShowHideSubDialogs(false) ;
        DisplayTotalRemainingTimeOnPQM();

    } catch (...) {


        //TMSC/2009Nov12/Modified/Code Changes
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Exception occured"));
        throw;
        //TMSC/2009Nov12/Modified/Code Changes
    }


}


//************************************Method Header**************************************

//Method Name:  OnCmdScanStart

//Author:  PATNI/GP

//Purpose: Hide GUI application
//**************************************************************************************/
void CPQMView::OnCmdScanStart(
    WPARAM wParam,
    LPARAM lParam
)
{
    //Modified by Hemant On 3/5/2009 2:49:40 PM, Removing warning
    //SetAcqumanReadyForNextRequest(wParam);
    SetAcqumanReadyForNextRequest(wParam ? true : false);
}



//************************************Method Header**************************************

//Method Name:  OnDestroy

//Author:  PATNI/GP

//Purpose: Memory deallocation
//**************************************************************************************/
void CPQMView::OnDestroy(
)
{
    try {
        m_tooltip_font.DeleteObject();
        CFormView::OnDestroy();

        DEL_PTR(m_sink)

        DestroySubDilaogsOnPQMHide();

    } catch (...) {
        DestroyWindow();
        PostQuitMessage(WM_QUIT);
    }
}

//****************************Method Header************************************
//Module         : OnClickButtonPqmclose
//Author         : PATNI/MN
//Purpose        : To Close the PQM Application
//*****************************************************************************
void CPQMView::OnClickButtonPqmclose(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonPqmclose");
    PQM_TRACE_SCOPED(FUNC_NAME);

    VARIANT data;
    data.byref = NULL ;

    if (S_OK != m_acquisition->GetStudyLock(&data)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetStudyLockMutex Failed"));
        return;
    }

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* l_studylock_mutex = (MR::ACQ::SM::COMMON::MUTEX::CThreadMutex*) data.byref;
    data.byref = NULL ;

    CScopedLock l_scoped_lock_irs(l_studylock_mutex);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetStudyLockMutex is acquired"));

    if (S_OK != m_acquisition->GetScanMutex(&data)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));
        m_scan_lock_acquired = false;
        return;
    }

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* l_scan_mutex = (MR::ACQ::SM::COMMON::MUTEX::CThreadMutex*) data.byref;
    data.byref = NULL ;

    CScopedLock l_scoped_lock_scan(l_scan_mutex);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

    if (m_is_end_exam_clicked) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("End Exam button is already clicked"));
        return;
    }

    m_is_end_exam_clicked = true;


    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_ID_APP_CLOSE);

    if (m_scan_lock_acquired) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_scan_lock_acquired is acquired"));
        return;
    }

    if (IsScanning()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan Is Running. Exception..."));
        return;
    }

    //Patni-Manishkumar/2009Aug12/Modified/MVC002704
    m_bEnableLocatorOnScanEdit = false;

    //Added by Hemant On 2/27/2009 7:25:38 PM, , defect PSP#180
    //Patni-Hemant/2011Mar30/Modified/REDMINE-1575
    if (GetMemoryAliveFlag())
        return ;

    if (m_patientorientationdialog)
        m_patientorientationdialog->ShowWindow(SW_HIDE);

    ResetAutoScanTimer();
    StopTimerForPager();

    //don;t perform GUI action but send the mesage to procon..
    SendMessageToProcon(PQM_PROCON_CLOSE, false, true);

    //Patni-MSN/2009May15/Added/JFT - IR 59 - Scan ID, SAR, dBdt Clear
    //Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
    UpdatePagerArea(_T("FW_ACQINFO_CLR"), _T("1"), _T(""));

    if (CPQMUtility::GetInstance()->GetPQMParentName()) {

        m_pqm_closed_flag = true;

        //Added by Hemant On 8/22/2008 8:13:37 PM - explicitly deleting all the data from
        //GUI and sequences list// its also happening in "RE_FW_RequestHideGL" , but still...
        g_MainView->GetPQMListCtrl().DeleteAllItems();

        // Patni-SS/ADDED/2009-May-27/IDS#449
        g_MainView->GetPQMListCtrl().Invalidate(false);
        g_MainView->GetPQMListCtrl().UpdateWindow();
        //-Patni-SS/ADDED/2009-May-27/IDS#449

        g_MainView->CleanUp();
        GetAcquisition()->OnPQMCloseButton();


        if (m_pqm_g.GetPQMMode() != K_IMAGING) {

            HidePQM();
            ShowSR();
        }

        SendNotifyToSR();
        m_sr_invoked = false;
        //-Patni-HAR/2010May21/Modified/MaFT#MVC007934

        m_is_end_exam_clicked = true;

    } else {
        CPQMUtility::CloseUtility();
        AfxGetMainWnd()->SendMessage(WM_CLOSE);
    }

    m_tiprep_app_cntl.CloseConnectionToTiPrep();
}

//+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
//****************************Method Header************************************
//Module         : OnClickButtonEndExam
//Author         : Patni/Manish
//Purpose        : Calling close functionality
//*****************************************************************************
void CPQMView::OnClickButtonEndExam()
{
    OnClickButtonPqmclose();
}
//-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal


//****************************Method Header************************************
//Module         : OnButtonInject
//Author         : PATNI/
//Purpose        :
//*****************************************************************************
void CPQMView::OnButtonInject(
)
{

    g_MainView->StopInjectTimerThread();
    m_acquisition->PutInjectTimeInDB();
}




//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
//************************************Method Header************************************
// Method Name  : PQMMessageMapVoiceMenuItem
// Author       : PATNI/Sribanta
// Purpose      : For Voice Menu item click
//***********************************************************************************
void CPQMView::PQMMessageMapVoiceMenuItem(UINT nID)
{
    m_pqmchildthread.PostMessageToThread(ON_VOICE_MENUITEM_CLICK, (WPARAM)nID, NULL);
}
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026

void CPQMView::OnClickLanguageMenuItem(
    UINT nID
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_ID_MENU_VOICE);

    wchar_t buffer1[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("GP_DATA"), buffer1, MAX_PATH);

    CString path = buffer1 ;
    CString voice_path = path + _T("\\Voice\\");

    CFileFind sdir;
    int langnum = sdir.FindFile(CString(voice_path + _T("*.*")));

    FILE* fp = NULL;
    wchar_t bufferlang[MAX_PATH] = {0};
    TCHAR   buf[NUM_1024] = {0};
    struct _stat stbuf;

    //PATNI-SS/Added/20090904/MVC004368
    CString lang(_T("English"));

    //Patni-AMT/2009Sep07/MOdified/MVC004368 code review
    if (::GetEnvironmentVariable(_T("MRMP_LANG"), bufferlang, MAX_PATH)) {
        lang  = bufferlang;
    }

    bool filepresent = false;
    int start_voice_menu_item = START_LANGUAGEMENU_ITEM;
    CString langdirname(_T(""));
    CString string(_T(""));
    CString filename(_T(""));
    const CString str(_T(""));

    while (langnum) {
        langnum = sdir.FindNextFile();

        if (sdir.IsDirectory() && !sdir.IsDots()) {

            filepresent = false;

            langdirname = sdir.GetFileName();

            filename = _T("");
            // get the entire file path along with the file name
            filename.Format(_T("%s%s\\label.%s"), voice_path, langdirname, lang);

            // open the file specified by FileName
            fp = _wfopen(filename, _T("r"));

            if (fp != NULL) {
                TCHAR label[NUM_100] = {0};
                fwscanf(fp, _T("%s"), label);

                //+Patni-PJS/2010Nov15/Added/V2.0/IR-175
                if ((UINT)start_voice_menu_item == nID) {
                    filepresent = true;
                    m_av_language = langdirname;
                    SetAvLanguage(m_av_language);
                }

                //-Patni-PJS/2010Nov15/Added/V2.0/IR-175
                start_voice_menu_item++ ;

                fclose(fp);
            }

            if (filepresent) {

                for (int voiceid = 0; voiceid < PQM_AV_VOICE_5; voiceid++) {

                    wsprintf(buf, L"%s\\Voice\\voice%d.wav", path, voiceid + 1);

                    TCHAR   lpexistingfilename[NUM_1024] = {0};	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
                    TCHAR   lpnewfilename[NUM_1024] = {0};	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
                    wsprintf(lpexistingfilename, L"%s\\Voice\\%s\\voice%d.wav", path, langdirname, voiceid + NUM_1);

                    if (_wstat(lpexistingfilename, &stbuf) == 0) {
                        wsprintf(lpnewfilename, L"%s\\Voice\\voice%d.wav", path, voiceid + NUM_1);

                        if (m_acquisition->FileCopy(lpexistingfilename, lpnewfilename)) {

                            CPQMLogMgr::GetInstance()->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_COPY_VOICE_FILE_ERROR")),
                                                                    _T("PQMMenuDlg.cpp"),
                                                                    __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);
                        }

                    } else {

                        string = _T("");
                        string.Format(_T("%s %d.wav %s %s %s"), CPQMUtility::GetMultiLingualString(_T("IDS_VOICE_STRING")),
                                      voiceid + NUM_1,
                                      CPQMUtility::GetMultiLingualString(_T("IDS_FILE_NOT_PRESENT")),
                                      str,
                                      CPQMUtility::GetMultiLingualString(_T("IDS_DIRECTORY_STRING")));

                        CPQMLogMgr::GetInstance()->DisplayError(string, _T("PQMMenuDlg.cpp"),
                                                                __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);

                        wsprintf(lpnewfilename, L"%s\\Voice\\voice%d.wav", path, voiceid + NUM_1);
                        wsprintf(lpexistingfilename, L"%s\\Voice\\%s\\voice%d.wav", path, _T("japanese"), voiceid + NUM_1);

                        if (m_acquisition->FileCopy(lpexistingfilename, lpnewfilename)) {
                            CPQMLogMgr::GetInstance()->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_COPY_VOICE_FILE_ERROR")),
                                                                    _T("PQMMenuDlg.cpp"),
                                                                    __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);
                        }
                    }
                }

            }
        }
    }

    //Patni-PJS/2009Sep23/Added/MVC4941

    m_acquisition->InitializeTicks();
}


//+Patni-HAR/2010Apr21/Added/MVC007534
//************************************Method Header****************************
//Method Name	: OnCmdFileNotExist
//Author		: PATNI / HAR
//Purpose		:
//*****************************************************************************
void CPQMView::OnCmdFileNotExist(
    WPARAM wParam,
    LPARAM lParam
)
{
    PQMMessageMapEndExam();
}
//-Patni-HAR/2010Apr21/Added/MVC007534

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonAutoScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonAutoScan");

    if (!m_iswait_cursor) {

        bool isfirstwait_selected = false ;

        int* selected_items = NULL ;
        int size = m_pqm_listctrl.GetselectedItems(&selected_items, NULL, NULL, NULL, NULL, &isfirstwait_selected);

        //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
        if (NULL == selected_items) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("selected_items pointer is NULL"));

            return;
        }

        //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

        if (!CanCheckUnCheckFor(selected_items, size)) {

            DEL_PTR_ARY(selected_items);

            return ;
        }

        if (isfirstwait_selected) {

            DEL_PTR_ARY(selected_items);
            return ;
        }

        if (size != 0) {

            //toggle the auto scan for selected items..

            POSITION pos = m_list->FindIndex(selected_items[0]);
            CSequence* pseq = m_list->GetAt(pos);

            int holdvalue = pseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

            //if VFC_HOLD_PRE_SCAN is set then auto scan is 'off' and if VFC_HOLD_PRE_SCAN is not there then its on..,
            //after that toglle the status..for all selected..
            bool auto_scan_on = ((holdvalue & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) ;
            auto_scan_on = !auto_scan_on ;		//toglle it..

            ApplyAutoScanToItems(selected_items, size, auto_scan_on);

            DEL_PTR_ARY(selected_items);
            //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
        }
    }
}


//+Patni//Manish//2009May20//CR-77
//************************************Method Header************************************
// Method Name  : OnClickMenuItemReferROI()
// Author       : PATNI/ MANISH
// Purpose      : Event on Menu item "Refer ROI"
//***********************************************************************************
void CPQMView::OnClickMenuItemReferROI()
{
    int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (IsValidSequenceIndex(first_selected_item)) {

        //Patni-ARD/2010Feb4/Modified/DeFT# MVC4446
        SendMessageToProcon(PQM_PROCON_PLAN_REFER_ROI, true, true, GetAcqOderForGivenProtocolIndex(first_selected_item));
    }
}
//-Patni//Manish//2009May20//CR-77


//+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
//************************************Method Header************************************
// Method Name  : OnClickMenuBreadthHold
// Author       : Patni/ Manish
// Purpose      : Event on clicking BreadthHold Menu Item
//***********************************************************************************
void CPQMView::OnClickMenuBreadthHold()
{
    if (!m_iswait_cursor) {
        //Patni-HEMANT/ ADDED On8/14/2009 12:34:56 PM/ Code Review
        PerformRightClickModeOption(BREATH_HOLD_OPTION);
    }
}
//-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal

//+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
//************************************Method Header************************************
// Method Name  : OnClickMenuAutoTransfer
// Author       : Patni/ Manish
// Purpose      : Event on clicking Auto Transfer Menu Item
//***********************************************************************************
void CPQMView::OnClickMenuAutoTransfer()
{
    if (!m_iswait_cursor) {
        //Patni-HEMANT/ ADDED On8/14/2009 12:34:56 PM/ Code Review
        PerformRightClickModeOption(AUTO_TRANSFER_OPTION);
    }
}
//-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal

//************************************Method Header************************************
// Method Name  : OnClickMenuCenterOfPlan
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when C0 is selected from the move couch menu.
//***********************************************************************************
void CPQMView::OnClickMenuCenterOfPlan()
{
    OnMoveCouchMethodMenuClick(CENTREPLAN);
}

//************************************Method Header************************************
// Method Name  : OnClickMenuPositionOfReferenceImage
// Author       : PATNI/HEMANT
// Purpose      : This function is called when C1 is selected from the move couch menu.
//***********************************************************************************
void CPQMView::OnClickMenuPositionOfReferenceImage()
{
    OnMoveCouchMethodMenuClick(PARENTREFPOS);
}


//************************************Method Header************************************
// Method Name  : OnClickScanAnatomy
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickScanAnatomy(
)
{
    CRect rect_for_utility_button ;
    m_scan_anatomy_button.GetWindowRect(&rect_for_utility_button);

    if (CanChangeScanAnatomy() != -1) {

        CSequence* current_seq = (CSequence*) m_pqm_listctrl.GetItemData(m_pqm_listctrl.GetFirstSelectedItem());

        if (current_seq) {

            const CPqmPrMode& prmode = current_seq->GetPrMode() ;

            PopupMenuEx context_sub_menu;
            context_sub_menu.CreatePopupMenu();

            //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
            CArray<PopupMenuEx*, PopupMenuEx*> l_sub_menu_list;
            CreateScanAnatomyPopupMenu(&context_sub_menu, prmode.m_anatomy, prmode.m_str_scan_anatomy, &l_sub_menu_list);
            //-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137

            LPCTSTR font_name = _T("Trebuchet MS") ;
            const int font_size = 10;
            context_sub_menu.SetFont(font_name, font_size);
            context_sub_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect_for_utility_button.left,
                                            rect_for_utility_button.bottom, this, 0);


            //+Patni/KSS/2011Apr06/Modified/TMSC-REDMINE-779_part-1
            UTIL::DeleteAndClearMFCArray<PopupMenuEx>(&l_sub_menu_list);
            //-Patni/KSS/2011Apr06/Modified/TMSC-REDMINE-779_part-1
        }
    }
}

//+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
//************************************Method Header**************************************
//Method Name:  OnClickAnatomyButton
//Author:  Patni/Manish
//Purpose: Called on clicking anatomy button.
//******************************************************************************
void CPQMView::OnClickAnatomyButton()
{
    const int l_nIndex = m_pqm_listctrl.GetFirstSelectedItem();
    OnClickButtonSarRegion(l_nIndex);

    //+Patni-Hemant/2009Nov16/Added/IR#90/Bottom Buttons
    //
    //
    //    //+Patni-Manish/2009July16/Commented/JFT#-GUIRenewal/Reverse the color of SAR Button
    //	if(-1 != l_nBitmapResourceID) {
    //		BUTTON_COLORS_t stColors_o2;
    //		m_end_exam_button.GetButtonColors(stColors_o2);
    //		m_scan_sar_button.SetButtonColors(stColors_o2);
    //	}
    //	//-Patni-Manish/2009July16/Added/JFT#-GUIRenewal/Reverse the color of SAR Button
    //
    //-Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons

    //Patni-Hemant/2009DEC02/Modified/IR#90/MVC006006
    SetBitmapForButton(m_scan_sar_button, m_pqm_listctrl.GetBitmapResourceIDForSARRegion(l_nIndex) , RGB(255, 255, 255));
}

//************************************Method Header************************************
// Method Name  : OnClickButtonPlanDuplicate
// Author       : PATNI/ Ashish
// Purpose      : Send plan duplicate Message To Procon
//***********************************************************************************
void CPQMView::OnClickButtonPlanDuplicate(
)
{
    const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();

    if ((current_protocol_index_edited_by_wse == -1) || !IsValidSequenceIndex(current_protocol_index_edited_by_wse))
        return ;

    int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (IsValidSequenceIndex(first_selected_item)) {

        SendMessageToProcon(PQM_PROCON_PLAN_DUPLICATE, false, true, GetAcqOderForGivenProtocolIndex(first_selected_item));

    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::OnSetCursor(
    CWnd* pWnd,
    UINT nHitTest,
    UINT message
)
{
    HANDLE l_hSFTMenuProcess = NULL;
    l_hSFTMenuProcess = m_sft_menuhandler->GetSFTMenuProcessHandle();

    if (m_iswait_cursor) {

        m_org_cursor = ::SetCursor(m_wait_cursor);
        return TRUE ;

    } else if (m_iswait_cursor_for_pqm_buttons) {
        POINT l_cursor_pt ;
        GetCursorPos(&l_cursor_pt);
        CRect l_rect;
        m_scanedit_button.GetWindowRect(&l_rect);

        if (l_rect.PtInRect(CPoint(l_cursor_pt.x, l_cursor_pt.y))) {
            ::SetCursor(m_wait_cursor);
            return TRUE;
        }

        m_locator_button.GetWindowRect(&l_rect);

        if (l_rect.PtInRect(CPoint(l_cursor_pt.x, l_cursor_pt.y))) {
            ::SetCursor(m_wait_cursor);
            return TRUE;
        }

        m_scan_start_button.GetWindowRect(&l_rect);

        if (l_rect.PtInRect(CPoint(l_cursor_pt.x, l_cursor_pt.y))) {
            ::SetCursor(m_wait_cursor);
            return TRUE;
        }

    } else if (l_hSFTMenuProcess) {
        if (IsSFTMenuProcessAlive(m_sft_menuhandler->m_SFtMenuName) == true) {

            m_iswait_cursor = true;
            m_org_cursor = ::SetCursor(m_wait_cursor);
            SetWaitCursorForModelessDlg(true);
            return TRUE;

        } else
            m_sft_menuhandler->SetSFTMenuProcessHandle(NULL);

        SetWaitCursorForModelessDlg(false);

    }

    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    else {

        if (m_queuedialog && m_queuedialog->GetWaitCursor()) {

            POINT pt ;
            GetCursorPos(&pt);

            //queue buttons should show wait cursor..
            if (m_queuedialog->IsQueueDialogButton(pt)) {

                ::SetCursor(m_wait_cursor);
                return TRUE ;

            } else {
                POINT pt1 ;
                GetCursorPos(&pt1);
                CRect l_rect ;
                GetDlgItem(IDC_LOCATOR_BUTTON)->GetWindowRect(&l_rect) ;

                if (l_rect.PtInRect(CPoint(pt1.x, pt1.y))) {
                    ::SetCursor(m_wait_cursor);
                    return TRUE;
                }
            }
        }
    }

    return CFormView::OnSetCursor(pWnd, nHitTest, message);
    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

}


//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
void CPQMView::OnClickPlanDuplicateMenu(
    UINT f_cmd_id
)
{
    const int l_menu_item_clicked = f_cmd_id - ID_PLANDUPLICATE_MIN ;

    if (l_menu_item_clicked < 0) {
        return ;
    }

    POSITION l_pos = m_preset_item.FindIndex(l_menu_item_clicked);
    PresetItem l_presetitem = m_preset_item.GetAt(l_pos);

    //const TCHAR* l_cstr = NULL;
    //l_cstr = l_presetitem.mask_value.GetBuffer(l_presetitem.mask_value.GetLength());
    const int l_mask_value = _ttoi((LPCTSTR)l_presetitem.mask_value);

    const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();

    if ((current_protocol_index_edited_by_wse == -1) || !IsValidSequenceIndex(current_protocol_index_edited_by_wse))
        return ;

    SendMessageToProcon(PQM_PROCON_PLAN_DUPLICATE, false, true, GetAcqOderForGivenProtocolIndex(m_pqm_listctrl.GetFirstSelectedItem()) , l_mask_value);
}


//Added by Hemant On 23_01_09, for CFA dialog show from IPC call
//
LRESULT CPQMView::OnCFAShow(
    WPARAM wParam,
    LPARAM lParam
)
{

    //+Patni-MN/2009Mar09/Added/PSP1#203//Checked if PQM window is opened
    if (!IsWindowVisible()) {
        //Patni-PJS/2010Feb01/Added/MVC004689
        BlinkPage();	//Patni-PJS/2010Feb19/Commented/DeFT# MVC004689 Code review
        m_is_cfa_window_opened_before_swithcing_page = true ;

    } else {
        //+Patni-MN/2009Mar13/Added/PSP1#196//To check if dialog is already opened.
        if (!::IsWindow(GetCFADlgPtr()->m_hWnd) && !GetCFADlgPtr()->IsWindowVisible()) {
            //+Patni-MSN 2009May-26/Un-Commented/PSP1#371
            //+Patni-Manishkumar 2009May-04/Added/JFT#51
            ChangeViewOfModelessDialogs(false);
            //Patni-PJS/Modified/1Aug2009/Internal defect solved
            //Patni-PJS/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
            //m_cfadlg->ResetPendingCFAMessage();
            GetAcquisition()->SetActiveDialog(IS_CFA_DIALOG, true); //sheril
            GetCFADlgPtr()->DoModal();
            GetAcquisition()->SetActiveDialog(IS_CFA_DIALOG, false); //sheril

            ChangeViewOfModelessDialogs(true);
            //-Patni-Manishkumar 2009May-04/Added/JFT#51
            //-Patni-MSN 2009May-26/Un-Commented/PSP1#371

        } else {
            //Patni-MN/2009Mar13/Added/PSP1#196//Disable the "OK" button of CFA dialog opened through utitlity menu.
            GetCFADlgPtr()->GetDlgItem(IDOK)->EnableWindow(false);

            //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
            GetCFADlgPtr()->ShowHideDialogCtrls();
            //-Patni-HEMANT/ADDED On 3/17/2009 4:29:00 PM/ PSP / ACEProper CFA
        }

        //-Patni-MN/2009Mar13/Added/PSP1#196
    }

    //-Patni-MN/2009Mar09/Added/PSP1#203

    return 0 ;
}



//+Patni-MN/2009Mar13/Added/PSP1#203
LRESULT CPQMView::OnShowCFADialog(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnShowCFADialog");

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    CPQMCFADlg* l_PQMCFADlg = GetCFADlgPtr();

    if (NULL == l_PQMCFADlg) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_PQMCFADlg pointer is NULL"));

        return 0;

    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    if ((l_PQMCFADlg->GetPendingCFAMessage() == PQM_MSG_CFA_PRESCAN_ABORT)) {

        //do not open the dialog when you have PQM_MSG_CFA_PRESCAN_ABORT message pending..
        //also rest the pending message for CFA
        l_PQMCFADlg->ResetPendingCFAMessage();

        return 0;
    }

    //-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130

    //+Patni-MSN 2009May-26/Un-Commented/PSP1#371
    //+Patni-Manishkumar 2009May-04/Added/JFT#51
    ChangeViewOfModelessDialogs(false);

    GetAcquisition()->SetActiveDialog(IS_CFA_DIALOG, true);
    l_PQMCFADlg->DoModal();
    GetAcquisition()->SetActiveDialog(IS_CFA_DIALOG, false);

    ChangeViewOfModelessDialogs(true);
    //-Patni-Manishkumar 2009May-04/Added/JFT#51
    //-Patni-MSN 2009May-26/Un-Commented/PSP1#371

    return 0 ;
}
//-Patni-MN/2009Mar13/Added/PSP1#203


//Patni-SS/2009Mar31/Added/PSP1#XX
LRESULT CPQMView::OnAbortFail(
    WPARAM wParam,
    LPARAM lParam
)
{
    OnMessageFromAcqmanMsgHandler(PQM_MSG_ABORT_FAIL);
    return TRUE;
}


//Patni-SS/2009Mar31/Added/PSP1#XX
LRESULT CPQMView::OnAbortWait(
    WPARAM wParam,
    LPARAM lParam
)
{
    OnMessageFromAcqmanMsgHandler(PQM_MSG_ABORT_WAIT);
    return TRUE;
}

//+Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ PSP/ IDS#202, IDS#192
//
LRESULT CPQMView::OnReConnectToProcon(
    WPARAM wParam,
    LPARAM lParam
)
{
    m_acquisition->ConnectToProconFromGUI();

    //EnableDisableScanEditOrPlanButtons();
    return 0;
}
//-Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ PSP/ IDS#202, IDS#192

//************************************Method Header*****************************
// Method Name  : OnDataBaseNotInSync
// Author       : PATNI/ AMT
// Purpose      : This method will display error message to notify user that
//                  databases are not in synchronization
//******************************************************************************
LRESULT CPQMView::OnDataBaseNotInSync(
    WPARAM wParam,
    LPARAM lParam
)
{
    //Patni-Sribanta/2010Nov23/Added/REDMINE-772
    if (!wParam) {


        CString l_app_name = _T("");
        l_app_name.LoadString(IDS_ERROR_APP_NAME);
        l_app_name = l_app_name.IsEmpty() ? l_app_name : _T("PQM");

        //Please add this message to Resource file. IDS_ERR_DB_NOT_SYNC
        //l_err_msg.LoadString(IDS_ERR_DB_NOT_SYNC);     // Added By KT/ACE-2/15-06-2009/Multilingualisation

        //REDMINE-781 / Return value not used..
        /*MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e)*/

        CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
            CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DB_NOT_SYNC")),
            l_app_name,
            SCAN_WARN_INFORMATION,
            SCAN_WARN_SYSMODAL,        // MEITEC/2010Jul14/Modified/MVC008324_2
            SCAN_WARN_OKONLYBUTTON
        );
        //Patni-MP/2010Jul02/Added/MVC008324_2
    }

    PQMMessageMapEndExam();

    return 0;
}
//-Patni-AMT/2009Mar21/Added/PSP1#001

//+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnOnQueueAndNext(
)
{

    //+Patni-HEMANT/Modified On 10/30/2009 / QA Testing Defect#2
    if (m_queuedialog && !m_queuedialog->GetWaitCursor()) {
        m_queuedialoghandler->SendMessageToProcon(PQM_PROCON_QUEUE_NEXT);
    }

    //-Patni-HEMANT/Modified On 10/30/2009 / QA Testing Defect#2
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnQueueAndExit(
)
{
    //+Patni-HEMANT/Modified On 10/30/2009 / QA Testing Defect#2
    if (m_queuedialog && !m_queuedialog->GetWaitCursor()) {
        m_queuedialoghandler->SendMessageToProcon(PQM_PROCON_QUEUE_EXIT);
    }

    //-Patni-HEMANT/Modified On 10/30/2009 / QA Testing Defect#2
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnNextAndCopy(
)
{
    //+Patni-HEMANT/Modified On 10/30/2009 / QA Testing Defect#2
    if (m_queuedialog && !m_queuedialog->GetWaitCursor()) {
        m_queuedialoghandler->SendMessageToProcon(PQM_PROCON_NEXT_COPY);
    }

    //-Patni-HEMANT/Modified On 10/30/2009 / QA Testing Defect#2
}
//-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

//+Patni-HEMANT/2009Sept05/Added/ MVC#002493
//************************************Method Header************************************
// Method Name  : RemoveWndQueueMessages
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
LRESULT CPQMView::OnHandleIRSMessages(
    WPARAM wParam,
    LPARAM lParam
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnHandleIRSMessages") ;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_pqmchildthread.PostMessageToScanThread(ON_IRS_SCAN_MSG, wParam, lParam);
    return 0;
}
void CPQMView::OnHandleIRSMessageOnScanThread(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnHandleIRSMessageOnScanThread") ;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (IsWaitCursor()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IsWaitCursor() is true"));
        return ;
    }

    if (!m_acquisition) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_acquisition is NULL"));
        return ;
    }

    VARIANT data;
    data.byref = NULL ;

    if (S_OK != m_acquisition->GetScanMutex(&data)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));
        return ;
    }

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* l_scan_mutex = (MR::ACQ::SM::COMMON::MUTEX::CThreadMutex*) data.byref;
    data.byref = NULL ;

    CScopedLock l_scoped_lock(l_scan_mutex);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));
    pqm_msg_tag l_pqm_msg = (pqm_msg_tag)wParam;
    bool l_canScan = true;							//+Patni-Ravindra Acharya/2010Sep13/Added/MVC008951

    switch (l_pqm_msg) {

        case PQM_MSG_IRS_SCANSTART : {

            //+Patni-Ravindra Acharya/2010Sep13/Added/MVC008951 & Redmine 717

            // Check if Scan Offset Dialog Box,As Compass Setting Dialog, Anatomy Dialog or PQM Coil Info Dialog is Open
            if ((m_setscanoffsetdlg) || (m_cds_controller->GetCOMPASS_SettingDlg()) || (m_anatomy_dlg) || (GetCoilDlgStatus())) {
                l_canScan = false ;
            }

            //-Patni-Ravindra Acharya/2010Sep27/Added/MVC008951 & Redmine 717
            //-Patni-Ravindra Acharya/2010Sep13/Added/MVC008951
            //+Patni-PJS/ADDED On 9/13/2009 12:34:39 PM/ MVC004692 - fixed for reopen
            ////+Patni-MP/2009Sep02/Modified/Defect Fix MVC004574
            //if((IsAcqumanReadyForNextRequest())&&(!IsScanning())){

            CString l_log_str(_T(""));
            l_log_str.Format(_T("[m_is_cfa_window_opened_before_swithcing_page : %d] \
			[CheckCFADialogStatus() : %d]"),
                             m_is_cfa_window_opened_before_swithcing_page, CheckCFADialogStatus());


            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

            if ((IsAcqumanReadyForNextRequest()) && (!IsScanning() || m_dynamic_scan_on)
                && GetSARControlStatusForScanRelatedButtons()
                //Patni-HAR/2010Jun24/Added/V1.20#MVC008392
                && !m_bpost_process_flag && (!IsWaitCursor()) /*&& (!CheckModalDialogStatus())*/
                && (!m_is_cfa_window_opened_before_swithcing_page && (!CheckCFADialogStatus()))) {//Patni-PJS/2010Jan19/Added/MVC006827
                ////+Patni-MP/2009Aug20/Modified/Defect Fix MVC004379
                //-Patni-PJS/ADDED On 9/13/2009 12:34:39 PM/ MVC004692 - fixed for reopen

                //Patni-HAR/2010Feb10/Added/ To IRS Message Process
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("In if Before retrieving CSequence *"));

                //Patni-HAR/2010Feb12/Commented/GetFirstWaitProtocolIndex() return only wait
                //protocols. Here we have to check for current protocols also in case of Breathe
                // Hold scan, Dynamic scan etc. So use GetIndexOfFirstWaitStateProtocol() which returns
                //first wait protocol or current protocol index

                //+Patni-HAR/2010Feb04/Added/MVC006364
                //CSequence *l_seq = (CSequence *)m_pqm_listctrl.GetItemData(m_pqm_listctrl.GetFirstWaitProtocolIndex(true));

                //+Patni-HAR/2010Feb12/Added/MSA0251-00160
                int FirstWaitProtocol = -1;
                GetIndexOfFirstWaitStateProtocol(FirstWaitProtocol);
                CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(FirstWaitProtocol);

                //-Patni-HAR/2010Feb12/Added/MSA0251-00160
                if (l_seq == NULL || (l_seq && l_seq->GetChildThreadStatus() != CT_NONE)) {

                    //Patni-HAR/2010Feb10/Added/ To IRS Message Process

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("l_seq == NULL || l_seq->GetChildThreadStatus() != CT_NONE"));
                    break;
                }

                //-Patni-HAR/2010Feb04/Added/MVC006364

                //bool l_canScan = true;					//+Patni-Ravindra Acharya/2010Sep13/Commented/MVC008951

                if (m_is_wse_open) {
                    const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();

                    //Patni-NP/2009Dec22/Modified/Defect Fix MVC006361
                    if (current_protocol_index_edited_by_wse == m_pqm_listctrl.GetFirstWaitProtocolIndex() && (!m_dynamic_scan_on)) {
                        //Patni-HAR/2010Feb10/Added/ To IRS Message Process

                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("Before making l_canScan = false"));

                        l_canScan = false;
                    }
                }

                if (l_canScan) {

                    //Patni-HEMANT/ADDED On 9/13/2009 4:21:31 PM/ IDS issue
                    //pressing irs sends this message in case of breath hold or
                    //APC statr, we should not move focus.
                    if (!IsScanning()) {

                        m_pqm_listctrl.SelectOnlyItem(m_pqm_listctrl.GetFirstWaitProtocolIndex());
                    }

                    //-Patni-MP/2009Mar30/Added/ACE-1 Defect Fix MCM0213-00168
                    //Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
                    if (m_amb_controller.IsAMBInvoked()) {
                        m_amb_controller.RecAmbScanStartMsg(true);

                    } else {
                        OnClickButtonScanStart(true);
                    }



                }

                //-Patni-MP/2009Aug20/Modified/Defect Fix MVC004379
                //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316

            } else if (m_mrsprescandlg && m_mrsprescandlg->GetSafeHwnd()) {


                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Posting Message to Start MRS Prescan"));

                m_mrsprescandlg->PostMessage(WM_CMD_START_MRS_PRESCAN);
            }

            //-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
            //-Patni-MP/2009Sep02/Modified/Defect Fix MVC004574

        }
        break ;

        case PQM_MSG_IRS_SCANPAUSE:

            if (m_pause_resume_button.IsWindowEnabled()) {
                //+Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
                switch (m_current_scan_opration)  {

                    case CurrentScanOpration_ScanStart :
                    case CurrentScanOpration_ScanResume :
                        OnClickButtonScanPause();
                        break ;

                    case CurrentScanOpration_ScanPause :
                        OnClickButtonScanResume();
                        break;
                }

                //-Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
            }

            break ;

        case PQM_MSG_IRS_SCANABORT:

            //+Patni-PJS/2010Aug26/Modified/V1.30/TMSC-REDMINE-316/Re-open/Code review

            /*if (m_abort_button.IsWindowEnabled()) {
                //Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
                //OnClickButtonScanAbort();
                AbortScan();
                //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316

            } else if (m_mrsprescandlg && m_mrsprescandlg->GetSafeHwnd()) {
                PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::OnHandleIRSMessages"),
                          _T("Posting Message to Abort MRS Prescan"));
                m_mrsprescandlg->PostMessage(WM_CMD_ABORT_MRS_PRESCAN);
            }*/
            //-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
            if (m_mrsprescandlg && m_mrsprescandlg->GetSafeHwnd()) {


                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Posting Message to Abort MRS Prescan"));

                m_mrsprescandlg->PostMessage(WM_CMD_ABORT_MRS_PRESCAN);

            } else if (m_abort_button.IsWindowEnabled()) {

                //Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
                //OnClickButtonScanAbort();
                AbortScan();
                //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
            }

            //-Patni-PJS/2010Aug26/Modified/V1.30/TMSC-REDMINE-316/Re-open/Code review
            break ;

        default :
        {}
    }


}


//****************************Method Header************************************
//Module         : OnSettingWaitCursorForPqmButtons
//Author         : iGate
//Purpose        :
//*****************************************************************************
LRESULT CPQMView::OnSettingWaitCursorForPqmButtons(WPARAM wParam, LPARAM lParam)
{
    SetWaitCursorForPQMButtons(wParam ? true : false);
    return 0;
}

//-Patni-HEMANT/2009Sept05/Added/ MVC#002493


//+Patni-Manishkumar/2009Sept15/Added/MVC004760/Function to set wait cursor
//************************************Method Header************************************
// Method Name  : OnSettingWaitCursor
// Author       : PATNI/ Manishkumar
// Purpose      : Function to set wait cursor
//***********************************************************************************
LRESULT CPQMView::OnSettingWaitCursor(WPARAM wParam, LPARAM lParam)
{
    //+Patni-HAR-SS/2010Feb26/Added/MSA0251-00219
    //SetWaitCursor(true);
    // + Patni-Sribanta/Modified/2010August04/ Defect ID-14
    //SetWaitCursor(wParam ? true : false);
    SetWaitCursor(wParam ? true : false, false);
    // + Patni-Sribanta/Modified/2010August04/ Defect ID-14
    //-Patni-HAR-SS/2010Feb26/Added/MSA0251-00219

    //Patni/2010Sep30/Added/V1.30/REDMINE-823
    UpdateOrientationCombosOnSelectionChanged(m_pqm_listctrl.GetFirstSelectedItem());

    UpdateEndExamButtonStatus();
    UpdateOKBtnStatusofWirelssDlg();
    UpdateCFARetry();

    return 0;
}
//-Patni-Manishkumar/2009Sept15/Added/MVC004760/Function to set wait cursor

//************************************Method Header************************************
// Method Name  : OnCFAReady
// Author       : PATNI/ Ravindra Acharya
// Purpose      : Function to Enable/Disable OK Button
//***********************************************************************************
//+Patni-Ravindra Acharya/2010July23/Added/IR-153
LRESULT CPQMView::OnCFAReady(WPARAM wParam, LPARAM lParam)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnCFAReady");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (CheckCFADialogStatus()) {

        //Patni-Ravindra Acharya/2010July30/Added/IR-153
        m_cfadlg->OnCFAReady() ;
    }

    return 0 ;
}
//-Patni-Ravindra Acharya/2010July23/Added/IR-153


//+Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator
//**************************************************************************
//Method Name   : OnClickPlanDupConfig
//Author        : PATNI\DKH
//Purpose       : To handle on click event of "Plan Duplicate config" menu
//**************************************************************************
void CPQMView::OnClickPlanDupConfig()
{
    if (!(::IsWindow(m_pqmplandup_dlg.m_hWnd))) {

        m_pqmplandup_dlg.Create(IDD_PLANDUP_DIALOG, this);
    }

    m_pqmplandup_dlg.ShowWindow(TRUE);
}
//-Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator


//+Patni-MSN/2010Feb26/Added/IR-86, PQM Debug Window
//**************************************************************************
//Method Name   : OnClickDebugInfo
//Author        : PATNI\RJA
//Purpose       : To handle on click event of "DebugInfo" menu
//**************************************************************************
void CPQMView::OnClickDebugInfo()
{
    //Patni-MSN/2010Mar11/Modified/IR-86, PQM Debug Window
    if (m_pqmdebugdlg.m_hWnd != NULL) {
        m_pqmdebugdlg.ShowWindow(TRUE);

    } else {
        m_pqmdebugdlg.Create(IDD_PQMDEBUG_DIALOG, this);
        m_pqmdebugdlg.ShowWindow(TRUE);
    }
}
//-Patni-MSN/2010Feb26/Added/IR-86, PQM Debug Window

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnASCOMPASSSettings
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnASCOMPASSSettings(
)
{
    if (m_cds_controller) {
        m_cds_controller->OnASCOMPASSSettings();
    }
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement



//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffAsCompass
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnOffAsCompass(
)
{
    if (m_cds_controller) {
        m_cds_controller->OnOffAsCompass();
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
//************************************Method Header************************************
// Method Name  : OnLaterality
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnLaterality(
    UINT f_scananatomy_menuitem_id
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnLaterality") ;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("CPQMView::OnLaterality"));

    int l_scan_anatomy = -1;
    LateralityEnum l_laterality = Laterality_Invalid;
    GetScanAnatomyAndLateralityIndex(f_scananatomy_menuitem_id, l_scan_anatomy, l_laterality);

    //Here update the scan anatomy and laterality index in database
    if ((l_scan_anatomy != -1) && (l_laterality != -1)) {
        SetScanAnatomy(l_scan_anatomy, l_laterality);
        //Patni-PJS/2010Jul14/Added/MVC008265
        ChangeContentForModelessInfoDialogs();

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("OnLaterality produces Invalid indexes"));
    }
}
//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

LRESULT CPQMView::OnRefreshGUI(WPARAM wParam, LPARAM lParam)
{

    OnSelectionStatusChanged();
    return 0;
}

LRESULT CPQMView::OnInvokeCoilInfo(WPARAM wParam, LPARAM lParam)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayCoilInfo");

    //+Patni-AMT/2010Jan29/Modified/Ph-3# PROPOSAL_01-MEM_LEAK
    VARIANT* pdata = new VARIANT;

    //+Patni-Manishkumar/2009Oct07/Added/MVC004983- PROPOSAL_01-MEM_LEAK
    if (pdata == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("VARIANT is NULL"));
        return 0;
    }

    //-Patni-Manishkumar/2009Oct07/Added/MVC004983 PROPOSAL_01-MEM_LEAK

    m_acquisition->GetConnectedCoilListForDisplayFromIPC(pdata);

    if (pdata->byref) {

        //Modified by Hemant On 1/19/2009 4:36:12 PM , Fixing 753
        m_obj_coil_dlg = new CPQMCoilInfoDlg((ConnectedCoilList*) pdata->byref , this);

        if (m_obj_coil_dlg) {
            //+Patni-MSN 2009May-26/Un-Commented/PSP1#371
            //+Patni-Manishkumar 2009April-22/Added/PSP1#371
            ChangeViewOfModelessDialogs(false);

            m_obj_coil_dlg->DoModal();

            ChangeViewOfModelessDialogs(true);
            //-Patni-Manishkumar 2009April-22/Added/PSP1#371
            //-Patni-MSN 2009May-26/Un-Commented/PSP1#371
            DEL_PTR(m_obj_coil_dlg)

        } else {  //-Patni-Manishkumar/2009Oct07/Added/MVC004983

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("CPQMCoilInfoDlg object is NULL"));
        }

        //-Patni-Manishkumar/2009Oct07/Added/MVC004983

        pdata->byref = NULL ;
    }

    DEL_PTR(pdata)
    //-Patni-AMT/2010Jan29/Modified/Ph-3# PROPOSAL_01-MEM_LEAK

    return 0;
}


LRESULT CPQMView::OnInvokeGateDialog(
    WPARAM wParam,
    LPARAM lParam
)
{

    VARIANT* l_pdata = new VARIANT;
    GetAcquisition()->GetGateInfoList(l_pdata);

    CStringArray* l_gateInfoList = (CStringArray*)l_pdata->byref;

    m_obj_gate_dlg = new CPQMGateInfoDlg(l_gateInfoList, this);

    ChangeViewOfModelessDialogs(false);
    m_obj_gate_dlg->DoModal();
    ChangeViewOfModelessDialogs(true);

    DEL_PTR(m_obj_gate_dlg)

    l_pdata->byref = NULL ;
    DEL_PTR(l_pdata)

    return 0;
}

//************************************Method Header************************************
// Method Name  : OnToolTipText
// Author       : PATNI
// Purpose      :
//***********************************************************************************
BOOL CPQMView::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT* l_pttt = (TOOLTIPTEXT*) pNMHDR;

    if (l_pttt) {

        static CToolTipCtrl* l_pToolTip = NULL;
        CToolTipCtrl* l_ptt = AfxGetThreadState()->m_pToolTip;
        l_ptt->ModifyStyle(0, TTS_NOPREFIX);


        if (l_ptt != l_pToolTip) {

            l_ptt->SetTipBkColor(RGB(40, 106, 175));
            l_ptt->SetTipTextColor(RGB(255, 255, 255));
            l_ptt->SetFont(&m_tooltip_font, FALSE);
            l_pToolTip = l_ptt;
        }

        if (l_ptt->GetFont() != &m_tooltip_font) {

            l_ptt->SetTipBkColor(RGB(40, 106, 175));
            l_ptt->SetTipTextColor(RGB(255, 255, 255));
            //Patni-ND/2012May12/Added /SR-009
            l_ptt->SetFont(&m_tooltip_font, TRUE);
        }

        CDC* l_tooltip_dc = l_ptt->GetDC();

        CFont* old_font = l_tooltip_dc->SelectObject(&m_tooltip_font);

        CRect calc_rect(0, 0, 0, 0);
        CString l_str_tooltip = GetToolTipText(((*l_pttt).hdr).idFrom, l_tooltip_dc, calc_rect);

        if (l_str_tooltip.IsEmpty()) {
            return FALSE;
        }

        if (calc_rect.IsRectEmpty()) {
            l_tooltip_dc->DrawText(l_str_tooltip, &calc_rect, DT_CALCRECT);
        }

        l_tooltip_dc->SelectObject(old_font);
        l_tooltip_dc->DeleteDC();

        l_ptt->SetMaxTipWidth(calc_rect.Width() + 2);
        ::lstrcpy(l_pttt->szText, (LPCTSTR) l_str_tooltip);
    }

    *pResult = 0;
    return TRUE;
}


//************************************Method Header************************************
// Method Name  : OnCmdGLSRShow
// Author       : PATNI/Dhanesh
// Purpose      : Post message handler for GL_SR_Show message
//***********************************************************************************
void CPQMView::OnCmdGLSRShow(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnCmdGLSRShow");

    SrParamInfo* l_srInfo = GetSRParamInfoObj();
    CString l_study_loid = l_srInfo->m_strStudyID;
    CString l_patient_loid = l_srInfo->m_strPatID;
    //-Patni-PJS/2010Nov4/Modified/REDMINE-859/Error Scenario Fixed

    PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME,
              _T("Patient LOID : ") + l_patient_loid + _T(" Study LOID : ") + l_study_loid);

    m_is_end_exam_clicked = false;

    if (l_patient_loid.IsEmpty() || l_study_loid.IsEmpty()) {


        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPQMUtility::GetMultiLingualString(_T("IDS_FAILED_TO_LOAD_STUDY")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));

        PostMessage(WM_CMD_FILE_NOT_EXIST);
        return;
    }

    //-Patni-PJS/2010Nov4/Added/REDMINE-859/Error Scenario Fixed

    //+Patni-AMT/2010Feb05/Modified/Ph-3# PROPOSAL_04 & PROPOSAL_06
    //+Patni-HAR/2010May21/Modified/MaFT#MVC007934
    if (GetPQMGlobal().GetPQMMode() == K_SCHEDULING) {
        //Patni-DKH/2010May20/Added/Phase3#MVC007934
        SetPQMCloseFlag(false);
        HideSR();
    }

    ShowPQM();
    //-Patni-HAR/2010May21/Modified/MaFT#MVC007934
    //-Patni-AMT/2010Feb05/Modified/Ph-3# PROPOSAL_04 & PROPOSAL_06


    //+Patni-Sribanta/2010Nov10/Modified/IR-170
    //+Patni-Sudhir/2010Dec09/Modified/IR-170

    CString l_pas_path = (!m_srInfo.m_strPASFilename.IsEmpty()) ? m_srInfo.m_strPASFilename : CheckPASselectionFile() ;
    //-Patni-Sudhir/2010Dec09/Modified/IR-170

    //Patni-HAR/2010Apr21/Modified/MVC007534
    BOOL l_file_status = SendLOID(l_patient_loid.AllocSysString(),
                                  l_study_loid.AllocSysString(),
                                  l_pas_path.AllocSysString());

    //-Patni-Sribanta/2010Nov10/Modified/IR-170

    //+Patni-HAR/2010Apr21/Added/MVC007534
    if (R_FILE_NOT_FOUND == l_file_status) {
        PostMessage(WM_CMD_FILE_NOT_EXIST);
    }

    //-Patni-HAR/2010Apr21/Added/MVC007534

    //Patni-ARD/2010Jan6/Added/DeFT# IR 100
    SetCheckLoadStudy(true);

    //+Patni-SS/2009Apr21/Modified/PSP#271
    if (R_SAME_FILE_EXIST == l_file_status) {//Patni-HAR/2010Apr21/Added/MVC007534
        SetPQMCloseFlag(false);

        //+Patni-Hemant/2009Sept01/Added/Move_Couch and duplicate Error\IDS#826
        //g_MainView->SetScanIndex(0); // To Initialize the Scan Index to Zero
        SetScanIndex(-1);
        //-Patni-Hemant/2009Sept01/Added/Move_Couch and duplicate Error\IDS#826
    }

    //+Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681
    //+Patni-PJS/2010Sep8/Modified/TMSC-REDMINE-685
    /*else {
        g_MainView->SetCanAPCPause(APC_EnableFromPQM);
    }*/
    //-Patni-PJS/2010Sep8/Modified/TMSC-REDMINE-685
    //Patni-ARD/2010May06/Modified/IR-100
    SetDefaultSettings();

    //-Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681

    //-Patni-SS/2009Apr21/Modified/PSP#271
}

//****************************Method Header************************************
//Module         : OnTimer()
//Author         : PATNI\AKR
//Purpose        :
//*****************************************************************************
void CPQMView::OnTimer(UINT nIDEvent)
{
    switch (nIDEvent) {
        case WM_CMD_INJECT_TIMER: {
            DWORD l_dw_tick_count = GetTickCount();
            int l_inject_timer_time = m_inject_timer_time + (l_dw_tick_count - m_dw_start_count) / 1000;

            if (MAX_TIMER_COUNT < l_inject_timer_time) {
                KillTimer(WM_CMD_INJECT_TIMER);
                m_inject_timer_button.ShowWindow(SW_HIDE);
                m_dw_start_count = 0;
                m_inject_timer_time = 0;
                return;
            }

            const int l_min_value = l_inject_timer_time / NUM_60;

            const int l_sec_value = l_inject_timer_time % NUM_60;

            CString l_str_inject_time(_T(""));

            l_str_inject_time.Format(_T("%02d:%02d"), l_min_value, l_sec_value);

            CString l_str_inject_time_old;

            m_inject_timer_button.GetWindowText(l_str_inject_time_old);

            if (l_str_inject_time_old.Compare(l_str_inject_time) != 0) {
                m_inject_timer_button.SetWindowText(l_str_inject_time);
            }
        }
        break;

        case WM_CMD_PAGER_TIME_UPDATE: {
            if (!IsScanning()) {
                m_acquisition->CalcSARControlAndPutVFForAll();
            }

            if (m_max_time_for_pager < static_cast<int>((GetTickCount() - m_dw_start_count_for_pager) / 1000)) {
                StopTimerForPager();
            }
        }
        break;
    }
}
//************************************Method Header************************************
// Method Name  : OnClickButtonEditDelay
// Author       : PATNI-AKR/NFD002_Demo_CR-3
// Purpose      : Show edit delay dialog
//***********************************************************************************
void CPQMView::OnClickButtonEditDelay()
{
    LPCTSTR FUNC_NAME =  _T("CPQMView::OnClickButtonEditDelay");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_delay_time_dlg) {
        m_delay_time_dlg = new CDelayTimeDlg(this);

        if (NULL == m_delay_time_dlg) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("m_delay_time_dlg is NULL, failed to allocate memory."));
        }
    }

    ChangeViewOfModelessDialogs(false);

    m_delay_time_dlg->DoModal();

    ChangeViewOfModelessDialogs(true);

    //Note:Delay time should be checked for both Ok and Close button
    ExecuteAutoScanStart();
}

//************************************Method Header************************************
// Method Name  : OnClickButtonPlanReset
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickButtonPlanReset()
{
    const int l_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (IsValidSequenceIndex(l_selected_item)) {
        m_acquisition->ChangeSlaveToNormalProtocol(l_selected_item);
    }

    UpdateSelectedSeqMasterSlave();
}

//************************************Method Header************************************
// Method Name  : OnClickMasterOnPlanMark
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMasterOnPlanMark(UINT f_master_menu_id)
{

    const int l_menu_item_clicked = f_master_menu_id - ID_PLAN_MASTER_BIAS_MIN ;

    if (l_menu_item_clicked < 0) {
        return ;
    }

    CString l_master_clicked = m_master_list_for_plan_mark[l_menu_item_clicked];
    int l_master_index_num = (_ttoi(l_master_clicked) / m_pqm_listctrl.GetBiasValueForProtocolNumber()) - 1; //Index is zero based


    const int l_current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();
    const int l_selected_protocol_index = m_pqm_listctrl.GetFirstSelectedItem();

    if ((l_current_protocol_index_edited_by_wse == l_selected_protocol_index)) {
        SendMessageToProcon(PQM_PROCON_PLAN_DYNAMIC, false, true, GetAcqOderForGivenProtocolIndex(l_master_index_num));

    } else {
        VARIANT l_pdata;
        l_pdata.wReserved1 = l_master_index_num;
        l_pdata.wReserved2 = l_selected_protocol_index;
        m_acquisition->PerformAutoCopyForMasterSlave(&l_pdata);

        //Update group id, master flag and hold value
        UpdateSelectedSeqMasterSlave();

        //update other parameters
        GetSequenceInformation(l_selected_protocol_index);
        m_pqm_listctrl.UpdateSequences(&l_selected_protocol_index, 1);
        OnSelectionStatusChanged();
    }
}

//************************************Method Header**********************************
// Method Name  : OnClickMenuitemSARManager
// Author       : PATNI/Jayanthi
// Purpose      : Method used to show SAR Manager application
//***********************************************************************************
void CPQMView::OnClickMenuitemSARManager()
{
    CDPSManager::GetInstance()->SendASyncMsg(SAR_MANAGER, SHOW_SAR_MANAGER, _T(""));
}

//*********************************************************************
//Method Name   :   ReConnectSARManager
//Author        :   PATNI-AKR
//Purpose       :
//*********************************************************************
LRESULT CPQMView::ReConnectSARManager(WPARAM wParam, LPARAM lParam)
{
    if (m_acquisition) {
        m_acquisition->ReConnectSARManager();
    }

    return 0;
}
//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
//**************************************************************************
//Method Name   : OnUpdateDebugWindow
//Author        : PATNI\Sribanta
//Purpose       : Updates the debug window
//**************************************************************************
void CPQMView::OnUpdateDebugWindow(WPARAM wParam, LPARAM lParam)
{

    m_pqmdebugdlg.UpdateDebugWnd();
}
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026

bool CPQMView::IsLocatorEnabled()
{
    return m_locator_button.IsWindowEnabled() > 0;
}

void CPQMView::OnClickScanStart()
{
    if (m_amb_controller.IsAMBInvoked())
        m_amb_controller.RecAmbScanStartMsg();

    else
        PQMMessageScanStart();
}

//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
void CPQMView::PQMMessageScanStart(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::PQMMessageScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_scan_lock_acquired) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Already Processing ScanStart..."));
        return ;
    }

    if (m_iswait_cursor_for_pqm_buttons) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_iswait_cursor_for_pqm_buttons is true, return!!"));
        return ;
    }

    if (!m_is_scanning) {

        m_pqm_listctrl.CheckAndEndDragging();
        SetWaitCursor(true);
    }

    m_scan_lock_acquired = true;
    m_scan_start_button.EnableWindow(false);
    m_pqmchildthread.PostMessageToScanThread(ON_SCAN_START, NULL, NULL);
}
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026


bool CPQMView::ShouldDisableCoilGateInfo(
) const
{

    return (!CanEnableAqumanRelatedMenuItems()
            || !IsAcqumanReadyForNextRequest()
            || (m_rmc_scan_running)
            || IsAMB()
           );
}
/****************************Method Header************************************
//Method Name   :SetWaitCursorForModelessDlg
//Author        :iGATE\Annamalai
//Purpose       :
//*****************************************************************************/
void CPQMView::SetWaitCursorForModelessDlg(
    const bool f_display_waitcursor
)
{
    if (m_comment_dlg && ::IsWindow(m_comment_dlg->m_hWnd)) {
        m_comment_dlg->SetWaitCursor(f_display_waitcursor);
    }

    if (m_pqmmanualprescandlg && ::IsWindow(m_pqmmanualprescandlg->m_hWnd)) {
        m_pqmmanualprescandlg->SetWaitCursor(f_display_waitcursor);
    }

    if (m_mrsprescandlg && ::IsWindow(m_mrsprescandlg->m_hWnd)) {
        m_mrsprescandlg->SetWaitCursor(f_display_waitcursor);
    }

    if (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd)) {
        dlgCoilSelWin->SetWaitCursor(f_display_waitcursor);
    }

    if (m_patientorientationdialog &&::IsWindow(m_patientorientationdialog->m_hWnd)) {
        m_patientorientationdialog ->SetWaitCursor(f_display_waitcursor);
    }

    if (m_info_seq_dlg && ::IsWindow(m_info_seq_dlg->m_hWnd)) {
        m_info_seq_dlg->SetWaitCursor(f_display_waitcursor);
    }

    if (::IsWindow(m_pqmdebugdlg.m_hWnd)) {
        m_pqmdebugdlg.SetWaitCursor(f_display_waitcursor);
    }

    if (::IsWindow(m_pqmplandup_dlg.m_hWnd)) {
        m_pqmplandup_dlg.SetWaitCursor(f_display_waitcursor);
    }
}

void CPQMView::GetMinMaxValueForScanOffset(
    CSequence* f_seq,
    const int f_acq_order,
    Vector_t& f_current_offset,
    Vector_t& f_min,
    Vector_t& f_max
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetMinMaxValueForScanOffset");

    f_max.x = (float)GetXOffsetLimit();
    f_min.x = -f_max.x;
    f_max.y = (float)GetYOffsetLimit();
    f_min.y = -f_max.y;

    f_min.z = (float)(-10.0);
    f_max.z = (float)(10.0);

    int l_in_side_limit = -1, l_out_side_limit = -1, l_enable = 0;
    GetAcquisition()->GetInsideLimit(f_acq_order, &l_in_side_limit, &l_out_side_limit, &l_enable);

    VARIANT* pData = new VARIANT;
    f32vec3_t* l_objvec = NULL;

    if (NULL == pData) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pData pointer is NULL"));
        return ;
    }

    g_MainView->GetAcquisition()->GetPatientScanoffset(f_acq_order, &pData);
    l_objvec = (f32vec3_t*) pData->byref;

    f_current_offset.x = l_objvec->x;
    f_current_offset.y = l_objvec->y;
    f_current_offset.z = l_objvec->z;

    ValidateScanOffsetWithinRange(f_current_offset.x, f_current_offset.y, f_current_offset.z);

    if (l_enable > 0) {
        f_min.z = -(l_in_side_limit / 10.0f);
        f_max.z = l_out_side_limit / 10.0f;
    }

    DEL_PTR(pData->byref)
    DEL_PTR(pData)

}

void CPQMView::UpdateScanOffsetFor(
    CSequence* f_seq,
    const Vector_t& f_vect_scanoffset,
    const BOOL f_set_mode
)
{
    if (NULL == f_seq) {
        return ;
    }

    const int l_acq_order = _ttoi(f_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

    if (l_acq_order >= 0) {

        GetAcquisition()->SetScanOffsetValue(l_acq_order ,
                                             f_vect_scanoffset.x, f_vect_scanoffset.y, f_vect_scanoffset.z, f_set_mode);
    }
}

//************************************Method Header************************************
// Method Name  : CheckAndGetWaitingForAutoScanStartAcqOrder
// Author       : PATNI/Nilesh
// Purpose      :
//***********************************************************************************
int CPQMView::CheckAndGetWaitingForAutoScanStartAcqOrder()
{
    int l_waiting_for_auto_scan_acq_order = -1;

    if (S_FALSE != m_acquisition->CheckAndGetWaitingForAutoScanStartAcqOrder(&l_waiting_for_auto_scan_acq_order)) {
        return l_waiting_for_auto_scan_acq_order;
    }

    return -1;
}


//************************************Method Header************************************
// Method Name  : CreatePlanMasterSlavePopupMenu
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
void CPQMView::CreatePlanMasterSlavePopupMenu(
    PopupMenuEx* f_plan_master_slave_menu,
    const int f_protocol_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CreatePlanMasterSlavePopupMenu");

    if (!f_plan_master_slave_menu) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_plan_master_slave_menu pointer is NULL"));
        return ;
    }

    m_master_list_for_plan_mark.clear();
    VARIANT l_pdata;
    l_pdata.wReserved1 = f_protocol_index;
    l_pdata.byref = &m_master_list_for_plan_mark;
    m_acquisition->GetMasterListForPlanMark(&l_pdata);

    const int l_master_list_size = m_master_list_for_plan_mark.size() - 1;
    CString l_bias_num_str(_T(""));
    int l_menu_id = ID_PLAN_MASTER_BIAS_MIN;

    CString l_master_of_selected_slave_str(_T(""));

    if (-1 != l_pdata.wReserved1) {
        l_master_of_selected_slave_str = m_master_list_for_plan_mark[l_master_list_size];
    }

    const int l_start_index_of_menu = 2;//First two are for reset and seperator

    for (int l_index = l_start_index_of_menu; l_index < l_master_list_size + l_start_index_of_menu; l_index++) {
        l_bias_num_str = m_master_list_for_plan_mark[l_index - l_start_index_of_menu];
        f_plan_master_slave_menu->InsertMenu(l_index, MF_BYPOSITION, l_menu_id, l_bias_num_str);

        if (l_bias_num_str == l_master_of_selected_slave_str) {
            f_plan_master_slave_menu->CheckMenuItem(l_menu_id, MF_BYCOMMAND | MF_CHECKED);
        }

        if (GetWaitCursor() == true) {
            f_plan_master_slave_menu->EnableMenuItem(l_menu_id, MF_BYCOMMAND | MF_GRAYED);

        }

        l_menu_id++;
    }
}
//************************************Method Header************************************
// Method Name  : CheckForScanIndexAndMarkForDelete
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
bool CPQMView::CheckForScanIndexAndMarkForDelete(const int* f_selected_items, const int f_count)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CheckForScanIndexAndMarkForDelete");
    bool l_scan_index_selected = false;

    if (NULL == f_selected_items || !f_count) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_selected_items  or f_count is 0"));
        return l_scan_index_selected;
    }

    CScopedLock l_scoped_lock(&m_sync_scan_and_delete_cs);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering m_sync_scan_and_delete_cs"));

    for (int l_index = 0; l_index < f_count; l_index++) {
        if (GetScanIndex() == f_selected_items[l_index]) {
            l_scan_index_selected = true;
            break;
        }
    }

    if (l_scan_index_selected) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Scanning Index selected for deletion..Leaving m_sync_scan_and_delete_cs"));

        return l_scan_index_selected;
    }

    m_pqm_listctrl.SetChildStatusInSequence(0, f_selected_items, f_count, CT_DELETE);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Leaving m_sync_scan_and_delete_cs"));
    return l_scan_index_selected;
}

//************************************Method Header************************************
// Method Name  : GetGUIListProtocolInfo
// Author       : PATNI/ ARD
// Purpose      : Collect information for all sequences on GUI List and send it for
//                check on IPC side.
//***********************************************************************************
//Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
/*bool*/result_r CPQMView::GetGUIListProtocolInfo(
    const int f_start_index /*= -1*/,
    const int f_end_index /*= -1*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetGUIListProtocolInfo") ;

    //+Patni-HAR/2010May12/Added/Code Review-MVC007927
    //In case there is no item in list ctrl then no need to call GetItemData()
    if (0 == m_pqm_listctrl.GetItemCount()) {
        //Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
        //return true;
        return TYPE_TRUE;
    }

    //-Patni-HAR/2010May12/Added/Code Review-MVC007927


    //Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
    result_r l_result = TYPE_FALSE;


    try {

        CString l_series_loid(_T(""));

        CSequence* l_seq = NULL;
        ProtocoInfo* l_protocol_info = NULL ;

        protocol_Info_Vec l_protocol_info_vec;

        int l_start = (-1 == f_start_index) ? 0 : f_start_index ;
        int l_end = (-1 == f_end_index) ? m_pqm_listctrl.GetItemCount() : f_end_index ;

        int l_list_position = l_start + 1;

        for (int l_index = l_start; l_index < l_end; l_index++) {
            l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_index);

            if (l_seq != NULL) {
                l_seq->GetCPASCOMSequence()->GetSequenceID(l_series_loid);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("COMM")));

                l_protocol_info = new ProtocoInfo(
                    l_series_loid,
                    l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER")),
                    l_list_position++,
                    l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("COMM")),
                    l_seq->GetAcqStatus()
                );

                if (l_protocol_info)
                    l_protocol_info_vec.push_back(l_protocol_info);

            } else {
                CString str;
                str.Format(_T("Invalid Protocol Index (%d) Found"), l_index);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
            }
        }

        //-Patni-HAR/2010May12/Added/Code Review-MVC007927
        //VARIANT* l_pProtocolInfo = new VARIANT;
        VARIANT* l_pProtocolInfo = new VARIANT;

        if (l_pProtocolInfo) {
            l_pProtocolInfo->byref = &l_protocol_info_vec;

            if ((-1 != f_start_index) || (-1 != f_end_index)) {
                l_result = (m_acquisition->AMBSlaveMatchSequenceParam(l_pProtocolInfo, l_start, l_end) == S_FALSE) ? TYPE_FALSE : TYPE_TRUE;

            } else {
                l_result = (m_acquisition->GetGUIListProtocolInfo(l_pProtocolInfo, l_end - l_start) == S_FALSE) ? TYPE_FALSE : TYPE_TRUE;
            }
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (S_FALSE == l_result) ? _T("Set l_result to false") : _T("Set l_result to true"));
        UTIL::DeleteAndClearContainer(&l_protocol_info_vec);

        if (l_pProtocolInfo) {
            l_pProtocolInfo->byref = NULL;
            DEL_PTR(l_pProtocolInfo)
        }

    } catch (...) {
        //Trace log

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("ErrorManager exception occurred"));

        OnDataBaseNotInSync(NULL, NULL);

        //Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
        l_result = TYPE_ERR_MANAGER_EXCP;
    }

    //-Patni-HAR/2010Feb24/Modified/MSA0251-00187
    //-Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1

    //Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
    //return l_bSequenseParamSame;
    return l_result;
}

//************************************Method Header************************************
// Method Name  : AMBRearrange
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::AMBRearrange()
{
    int l_protocol_index_to_move = 0;
    int l_target_index = 0;
    m_acquisition->GetAMBRearrangeIndexes(&l_protocol_index_to_move, &l_target_index);
    MoveAMBProtocolToIndex(l_protocol_index_to_move - 1, l_target_index - 1);
}


bool CPQMView::InsertAutoInsertProtocol(
    const pqm_msg_tag& f_msg
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::InsertAutoInsertProtocol");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (NULL == m_pqm_sequences) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_sequences pointer is NULL"));
        return false;
    }

    auto_insert l_mode = AUTO_MAP;

    switch (f_msg) {

        case PQM_MSG_INSERT_SHIM_PROTOCOL:
            l_mode = AUTO_SHIM;
            break;

        case PQM_MSG_INSERT_MAP_PROTOCOL:
            l_mode = AUTO_MAP;
            break;

        default :
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown input for function"));
            return false;
    }

    try {
        SequenceList l_sequence_list ;

        VARIANT pData;
        pData.wReserved1 = l_mode;
        pData.byref = &l_sequence_list;

        int l_auto_insert_position = -1;
        int l_target_sequence_position = -1;

        int is_shim_required = 0, is_map_required = 0;

        m_acquisition->GetAutoInsertedSeqDataToDisplay(&pData, &l_auto_insert_position, &l_target_sequence_position, &is_shim_required, &is_map_required);

        if (NULL == pData.byref) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could Not found sequence data for auto insert"));
            return false;
        }

        POSITION l_position = l_sequence_list.GetHeadPosition();
        CPASCOMSequence* l_pascomseq = l_sequence_list.GetNext(l_position) ;
        CSequence* l_pseq =  new CSequence(l_pascomseq);
        ApplyConfigSettingForProt(l_pseq);

        //Inserting protocol on data list
        if (l_auto_insert_position >= m_list->GetCount()) {
            l_position = m_list->FindIndex(m_list->GetCount() - 1);
            m_list->InsertAfter(l_position, l_pseq);

        } else {
            l_position = m_list->FindIndex(l_auto_insert_position);
            m_list->InsertBefore(l_position, l_pseq);
        }

        //Inserting protocol on list ctrl
        m_pqm_listctrl.InsertProtocolAt(l_pseq, l_auto_insert_position, true);

        //applying auto scan to Target and Auto Insert protocols
        if ((AUTO_SHIM == l_mode) || ((!is_shim_required && is_map_required) && (AUTO_MAP == l_mode))) {
            const int l_total_items = 2;
            const int l_auto_scan_pos[l_total_items] = { l_auto_insert_position, l_target_sequence_position} ;
            g_MainView->ApplyAutoScanToItems(l_auto_scan_pos, l_total_items, true);

        } else {
            g_MainView->ApplyAutoScanToItems(&l_auto_insert_position, 1, true);
        }

        UpdateCompleteSequenceFromVF(l_auto_insert_position);
        UpdateCoilDataInSeq(l_pseq);
        DisplayTotalRemainingTimeOnPQM();

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Found while adding AutoInrset protocol on GUI"));
        throw;
    }

    return true;
}


//************************************Method Header****************************
// Method Name  : CloseWSE()
// Author       : PATNI /
// Purpose      :
//*****************************************************************************
void CPQMView::CloseWSE()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CloseWSE");

    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_is_wse_open) {
        //Get Editing protocol
        const int next_protocol_index_edited_by_wse = GetNextWSEEditProt();
        //Set WSE Editing protocol, since at the time of Q&N WSE status is already set to none
        m_pqm_listctrl.SetWSEEditingProtocolIndex(next_protocol_index_edited_by_wse);
        g_MainView->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
    }
}

// + SM4 Defect#737 Himanshu 20090130
//************************************Method Header****************************
// Method Name  : GetSequenceInformation()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CPQMView::GetSequenceInformation(
    const int f_seq_index
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetSequenceInformation");

    //+Patni-AMT/2010Jan29/Modified/Ph-3# PROPOSAL_01-MEM_LEAK - Code Review
    PQM_TRACE_SCOPED(FUNC_NAME);

    VARIANT* pData      = new VARIANT;

    CSequence* pSeq = (CSequence*) m_pqm_listctrl.GetItemData(f_seq_index);

    //+Patni-PJS/2011Jan24/Added/MaFT/MVC009566
    if (!pSeq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CSequence object pSep is NULL"));
        return;
    }

    //-Patni-PJS/2011Jan24/Added/MaFT/MVC009566
    const CString l_string_scan_index = pSeq->GetCPASCOMSequence()->GetSequenceParam()->
                                        GetString(_T("ACQORDER"));
    const int l_scan_index = _ttoi(l_string_scan_index);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("AcqOrder = ") + l_string_scan_index);

    //Patni-PJS/2011Jan24/Added/MaFT/MVC009566
    try {
        if (l_scan_index != -1) {

            //Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-MVC007915
            const int l_prev_anatomy = pSeq->GetPrMode().m_anatomy ;

            pData->byref = pSeq->GetCPASCOMSequence();
            m_acquisition->GetSequenceInformation(pData, l_scan_index);
            pSeq->SetCPASCOMSequence((CPASCOMSequence*)pData->byref);

            pSeq->InitializeParam(); //initializing the other parameters using newly received params

            //Patni-Hemant/2010Jun9/Added/MaFT/MVC008238
            UpdateCoilDataInSeq(pSeq);
            //+Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-MVC007915
            //
            //		//+Patni-PJS/2010May25/Modifed+Added/TMSC-REDMINE-234
            //		CPqmPrMode prmode = pSeq->GetPrMode();
            //		SScanAantomy scan_anatomy_data ;
            //		if (GetScanAnatomyDataFor(prmode.m_anatomy, 0, scan_anatomy_data)) {
            //
            //			//prmode.m_str_scan_anatomy = scan_anatomy_data;
            //			if(prmode.m_str_scan_anatomy.m_scan_anatomy_name != scan_anatomy_data.m_scan_anatomy_name){
            //				ApplyDefaultScanAnatomyFor(l_scan_index);
            //			}
            //		}
            //		m_scan_anatomy_button.SetWindowText(prmode.m_str_scan_anatomy.m_scan_anatomy_name);
            //		//-Patni-PJS/2010May25/Modifed+Added/TMSC-REDMINE-234

            if (l_prev_anatomy != pSeq->GetPrMode().m_anatomy) {

                //apply defautl scan anantomy for changed SAR

                //Patni-Hemant/2010Jun10/Modified/MaFT/MVC007915/Internal defect found
                ApplyDefaultScanAnatomyFor(/*l_scan_index*/f_seq_index);
            }

            //
            //-Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-MVC007915

            pSeq->EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption();//changing the encoded values for auto voice..
        }

        //+Patni-AMT/2010Jan29/Modified/cpp test corrections
        if (pData) {
            pData->byref = NULL;
            DEL_PTR(pData)
        }

        //+Patni-PJS/2011Jan24/Added/MVC009566

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Exception occurred"));

        if (pData) {
            pData->byref = NULL;
            DEL_PTR(pData)
        }

        throw;
    }

    //-Patni-PJS/2011Jan24/Added/MVC009566
    //-Patni-AMT/2010Jan29/Modified/cpp test corrections

    //-Patni-AMT/2010Jan29/Modified/Ph-3# PROPOSAL_01-MEM_LEAK - Code Review
}
// - SM4 Defect#737 Himanshu 20090130


//************************************Method Header**************************************

//Method Name:  GetIndexOfFirstWaitStateProtocol

//Author:  PATNI/SVP

//Purpose: Gets index of first wait protocol in list of protocol.

//**************************************************************************************/
void CPQMView::GetIndexOfFirstWaitStateProtocol(
    int& idx_wait_prot
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetIndexOfFirstWaitStateProtocol");
    idx_wait_prot = -1;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_list) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CSequence* pseq = NULL ;
    int index = 0;
    POSITION pos = m_list->GetHeadPosition();

    while (pos) {
        pseq = m_list->GetNext(pos);

        if (pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT ||
            pseq->GetAcqStatus() == PR_ACQ_STATUS_CURRENT) {

            SetGUISelAcqOrder(pseq);    //Patni-ARD/2009June17/Added/JFT# Acqorder checking between IPC & GUI
            idx_wait_prot = index;
            break;
        }

        index++;
    }
}

//************************************Method Header**************************************

//Method Name:  GetSelectedSeqIndex

//Author:  PATNI/SVP

//Purpose: Returns selected sequence index.

//**************************************************************************************/
int CPQMView::GetSelectedSeqIndex(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetSelectedSeqIndex");

    int* selected_items = NULL ;
    const int count = m_pqm_listctrl.GetselectedItems(&selected_items);

    if (selected_items == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("selected_items pointer is NULL"));

        return -1;
    }

    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    if (count == 1) {

        const int l_selected_items = selected_items[0];
        DEL_PTR_ARY(selected_items);

        return l_selected_items;
    }

    //+ Patni-PJS/2009Jul23/Modify/IDS#441,MVC002493
    if (count == 0) {
        DEL_PTR_ARY(selected_items);

        return 0 ;
    }

    //- Patni-PJS/2009Jul23/Modify/IDS#441,MVC002493

    DEL_PTR_ARY(selected_items);

    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

    return -1;
}

//************************************Method Header****************************
// Method Name  : GetShowCoilList()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CPQMView::GetShowCoilList(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetShowCoilList");

    VARIANT* pData = new VARIANT;
    m_acquisition->GetShowCoilList(pData);

    if (NULL == pData) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pData pointer is NULL"));
        return;
    }

    VftCoilListInfo_t* l_show_coil = (VftCoilListInfo_t*)pData->byref;

    m_selection_utiliy->SetShowCoilList(l_show_coil);

    m_acquisition->GetCoordinateList(pData);
    m_selection_utiliy->SetCoordinateList((VftCoilCoordinateListInfo_t*)pData->byref);

    m_selection_utiliy->ProcessShowCoilMsg(l_show_coil);

    if (m_pqm_g.GetPQMMode() == K_SCHEDULING)
        dlgCoilSelWin->DisplayCoil();

    DEL_PTR(pData)
}

//************************************Method Header****************************
// Method Name  : GetSelectCoilList()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CPQMView::GetSelectCoilList(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetSelectCoilList");

    //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    VARIANT* pData = new VARIANT;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == pData) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pData pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    m_acquisition->GetSelectCoilList(pData);
    coilSelect_t* l_select_coil = (coilSelect_t*)pData->byref;
    m_selection_utiliy->SetSelectCoilList(l_select_coil);
    m_selection_utiliy->ProcessSelectCoilMsg(l_select_coil);

    DEL_PTR(pData)
    //-Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
}

//****************************Method Header************************************
//Method Name   : SetScanProtocolToIPC()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
BOOL CPQMView::SetScanProtocolInfoFromGUI(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetScanProtocolInfoFromGUI");
    BOOL l_result = FALSE;

    int FirstWaitProtocol = -1;
    GetIndexOfFirstWaitStateProtocol(FirstWaitProtocol);

    if (IsValidSequenceIndex(FirstWaitProtocol)) {

        CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(FirstWaitProtocol);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("l_seq is NULL"));
            return l_result;
        }

        //+Patni-HAR/2010Mar06/Added/MSA00251-00246
        if (l_seq->GetChildThreadStatus() == CT_DELETE) {
            return FALSE;
        }

        CString l_strScanID = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("COMM"));
        CString l_series_loid(_T(""));
        l_seq->GetCPASCOMSequence()->GetSequenceID(l_series_loid);

        //+Patni-HAR/2010Mar06/Modified/MSA00251-00246
        const int l_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER"));
        ProtocoInfo l_protocol_info(l_series_loid, l_acq_order, /*l_scan_index*/FirstWaitProtocol + 1, l_strScanID, l_seq->GetAcqStatus());

        VARIANT* pData = new VARIANT;
        pData->byref = &l_protocol_info;

        l_result = (S_FALSE == m_acquisition->SetScanProtocolInfoFromGUI(pData)) ? FALSE : TRUE ;

        DEL_PTR(pData)
    }

    return l_result;
}
//-Patni-HAR/2010Feb24/Added/MSA0251-00187


void CPQMView::SetDefaultSettings(
)
{

    const bool initial_pqmglobal_state = true;

    //+Patni-ARD/2010Mar18/Added/DeFT# IR 100 Default Utility menu items
    m_pqm_g.ReSetUtilityMenuDefaultItems();

    m_acquisition->SetAutoPlan(m_pqm_g.GetAutoPlanFlag());
    m_acquisition->SetPlanAndScan(m_pqm_g.GetPlanScanFlag());
    m_acquisition->SetPrepWbCoil(m_pqm_g.GetPrepWbCoilFlag());
    m_acquisition->SetShimmWBCoil(m_pqm_g.GetShimmWbCoilFlag());

    m_acquisition->SetSkipRgn(m_pqm_g.GetSkipRgnFlag());
    m_acquisition->SetAllCoverageRgn(m_pqm_g.GetAllCoverageRgnFlag());
    m_acquisition->SetBatchForSARNCoil(m_pqm_g.GetBatchSettingFlag());
    // + Patni-Sribanta/2010Jun09/Modified/MVC8032
    //m_acquisition->SetMultiCovCaution( m_pqm_g.GetMultiCovCautionFlag());
    //-Patni-ARD/2010Mar18/Added/DeFT# IR 100 Default Utility menu items


    SetAPCPauseChecked(!initial_pqmglobal_state);

    //Patni-Hemant/2011Jan11/Commented/REDMINE-1095_Reopen
    //GetAcquisition()->SetAPCHoldValue(!initial_pqmglobal_state);

    //+Patni-Hemant/2010Apr9/Added/ACE-Phase#3/CDS Requirement
    if (m_cds_controller) {
        m_cds_controller->InitializeCDSParametersWithConfigDefault();
    }

    //-Patni-Hemant/2010Apr9/Added/ACE-Phase#3/CDS Requirement

    m_wirelessgating_cntlr.InitializeWirelessParametersWithConfigDefault() ;

    if (S_OK == m_acquisition->IsWireLessSystem()) {
        m_pqm_g.SetWirelessSystemTypeFlag(true);
    }

}

//****************************Method Header************************************
//Module         : SetWaitCursorForPQMButtons
//Author         : iGate
//Purpose        :
//*****************************************************************************
bool CPQMView::SetWaitCursorForPQMButtons(const bool f_wait_flag)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetWaitCursorForPQMButtons");

    CString l_str(_T(""));
    l_str.Format(_T("Before Cursor: %d"), m_iswait_cursor_for_pqm_buttons);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
    m_iswait_cursor_for_pqm_buttons = f_wait_flag;
    l_str.Format(_T("After Cursor: %d"), m_iswait_cursor_for_pqm_buttons);

    SendMessage(WM_SETCURSOR, 0, 0);

    if (!m_iswait_cursor_for_pqm_buttons) {
        ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
        RemoveWndQueueMessages();
    }

    return m_iswait_cursor_for_pqm_buttons;
}

//********************************************************************************************
//Method Name   :  IsSFTMenuProcessAlive()
//Author        :  iGATE /SP
//Purpose       :  This function will check SFTMenuProcess is alive or not.
//********************************************************************************************
bool CPQMView::IsSFTMenuProcessAlive(const CString& f_process_name) const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::IsSFTMenuProcessAlive");


    bool l_bRetVal = false;
    CString	l_csprocess = _T("");
    TCHAR l_szProcessName[MAX_PATH] = {_T('\0')};

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("IsSFTMenuProcess  is Running or not"));


    // Get the list of process identifiers.
    DWORD l_dwaProcesses[1024] = {0}, l_dwcbNeeded = 0;
    EnumProcesses(l_dwaProcesses, sizeof(l_dwaProcesses), &l_dwcbNeeded);

    // Calculate how many process identifiers were returned.
    const DWORD l_dwcProcesses = l_dwcbNeeded / sizeof(DWORD);

    const int PQM_MAX_COUNT = 2;
    const DWORD TIMEOUT = 40 * 1000;
    int l_pqm_found_count = 0;


    for (DWORD l_ncount = 0; l_ncount < l_dwcProcesses; l_ncount++) {
        _tcscpy(l_szProcessName, _T(""));
        HANDLE l_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, l_dwaProcesses[l_ncount]);

        if (NULL == l_hProcess) {
            continue;
        }

        // Get the process name.
        GetModuleBaseName(l_hProcess, NULL, l_szProcessName, MAX_PATH);
        l_csprocess = l_szProcessName;

        if (l_csprocess.CompareNoCase(f_process_name) == 0) {
            PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME, f_process_name + _T("is alive"));

            CloseHandle(l_hProcess);
            return l_bRetVal = true;

        }

        CloseHandle(l_hProcess);
    }

    return l_bRetVal;
}
//****************************Method Header************************************
//Module         : SetWaitCursor
//Author         : PATNI/
//Purpose        :
//*****************************************************************************
bool CPQMView::SetWaitCursor(
    const bool wait_flag,
    const bool f_update_status /*= true*/			//Added by Hemant
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetWaitCursor");

    //+Patni-HAR/2010feb26/Added/MSA0251-00219
    CString l_str(_T(""));
    l_str.Format(_T("[Before][Cursor Counter: %d][Cursor Flag: %d]"), m_wait_cursor_counter, wait_flag);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

    if (true == wait_flag) {
        m_iswait_cursor = wait_flag;
        m_wait_cursor_counter++;

    } else {
        if (m_wait_cursor_counter > 0)m_wait_cursor_counter--;

        m_iswait_cursor = (m_wait_cursor_counter == 0) ? wait_flag : m_iswait_cursor;
    }

    l_str.Format(_T("[After][Cursor Counter: %d]\t[Cursor Flag: %d]"), m_wait_cursor_counter, m_iswait_cursor);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
    //-Patni-HAR/2010feb26/Added/MSA0251-00219

    //Added by Hemant On 8/22/2008 1:45:47 PM
    SendMessage(WM_SETCURSOR, 0, 0);

    //Added by Hemant On 12/19/2008 8:12:08 PM
    if (f_update_status) {
        //
        //Added by Hemant On 8/9/2008 3:28:00 PM .. also update the wait cursor related dlg items..
        //EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
        OnSelectionStatusChanged();
    }

    if (!m_iswait_cursor) {
        //we have done the processing and restored the cursor.. now remove all the LButtondown messges from message queue..

        //Patni-HEMANT/ADDED On 8/26/2009 7:33:53 PM/ IR90
        RemoveWndQueueMessages();
    }

    //+Patni-HEMANT/2009Mar016/Added/ACE1/Closing menus when hour glass is shown
    if (m_iswait_cursor) {
        SetCapture();
        ReleaseCapture();
    }

    return m_iswait_cursor;

}

//************************************Method Header************************************
// Method Name  : SetSendSeqListFor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetSendSeqListFor(
    int* const f_selected_items,
    int count
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetSendSeqListFor");

    if (!count) {

        //Meghana --defect fix
        m_acquisition->SendSequencesToMemory(NULL);
        return ;
    }

    // + Patni HAR / 2009 Jul 16 / Added / With Series defect
    VARIANT* pData = new VARIANT;
    m_acquisition->GetStudyPath(pData);
    const CString l_study_path = pData->bstrVal;

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_study_path);

    DEL_PTR(pData)

    BOOL l_amb_slave_found = FALSE;
    m_acquisition->PqmPqAMBSlaveLost(f_selected_items, &count, &l_amb_slave_found);

    if (TRUE == l_amb_slave_found) {
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPQMUtility::GetMultiLingualString(_T("IDS_WARN_AMB_PAS_REGI")),
                MPlus::EH::Information, APP_NAME, APP_NAME);
    }

    if (count <= 0) {
        m_acquisition->SendSequencesToMemory(NULL);
        return;
    }


    CSequence* pseq_for_memory = NULL ;

    for (int i = 0; i < count; i++) {
        // - Patni HAR / 2009 Jul 16 / Added / With Series defect

        pseq_for_memory = new CSequence((CSequence*)m_pqm_listctrl.GetItemData(f_selected_items[i]));

        if (NULL == pseq_for_memory) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq_for_memory is NULL"));
            continue;
        }

        pseq_for_memory->GetCPASCOMSequence()->SetSequenceID(
            pseq_for_memory->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"))
        );

        pseq_for_memory->GetCPASCOMSequence()->SetSequencePath(l_study_path);

        m_pqm_sequences->SetSndSequence(pseq_for_memory);

        //commented as seq was not getting transffered from PQM toMemory..
        //according to USha san, we have commented this.. there will be leak but will be resolved later..
        /*if(pseq_for_memory)
            delete pseq_for_memory;
        pseq_for_memory = NULL ;*/

    }

    VARIANT* pdata = new VARIANT;
    //  pdata->byref = &m_pqm_sequences->m_snd_seq_list;

    //Patni-Rajendra/2011Mar10/Modified/Redmine-783
    pdata->byref = &m_pqm_sequences->GetSendSeqList();
    m_acquisition->SendSequencesToMemory(pdata);

    //m_pqm_sequences->m_snd_seq_list.RemoveAll();		//mememory leak here.. delete all teh CSequence* from this..

    //Patni-Rajendra/2011Mar10/Modified/Redmine-783
    UTIL::DeleteAndClearMFCList<CPASCOMSequence>(&m_pqm_sequences->GetSendSeqList());

    //Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    DEL_PTR(pData)

}

//************************************Method Header************************************
// Method Name  : SetAnatomy
// Author       : PATNI/ HEMANT
// Purpose      : This function will apply anatomy to all the given array of items.
//***********************************************************************************
bool CPQMView::SetAnatomy(
    const int* const seq_items,
    const int seq_count,
    const int iAnatomy
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetAnatomy");

    if (NULL == seq_items) {
        return false;
    }

    WCHAR buff[20] = {0};	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
    const CString stranatomy = _itow(iAnatomy, buff, 10);

    VARIANT* pdata = new VARIANT;
    pdata->wReserved1 = SAR_REG;

    CSequence* pseq = NULL ;

    for (int index = 0; index < seq_count; index++) {

        pseq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[index]);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL"));
            continue;
        }

        if (PR_ACQ_STATUS_WAIT != pseq->GetAcqStatus()) {
            return false;
        }

        //-Patni//AJS-MP/2010Sep29/Added/MVC009119 defect

        CPqmPrMode& prmode = pseq->GetPrMode() ;

        if (prmode.m_anatomy == iAnatomy)
            continue ;		//if anatomy is already there, don't apply ..

        prmode.m_anatomy =  iAnatomy;

        //SetPrModeOfSelectedSequence(item_index, prmode);

        //+Patni-MEGHANA /2009APR01/Modify/ACE-1/ MCM0213-00131
        ApplyDefaultScanAnatomyFor(seq_items[index]);
        pseq->SetValue(sa, stranatomy);

        pdata->intVal = iAnatomy;

        //Modified by Hemant On 1/28/2009 4:10:17 PM
        if (S_OK == GetAcquisition()->UpdateGUIControlsInVarfieldDB(pdata,/*item_index*/GetAcqOderForGivenProtocolIndex(seq_items[index]))) {

            pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("SA"), iAnatomy);

        } else {
            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("Trace of UpdateGUIControlsInVarfieldDB"));
        }

        m_pqm_listctrl.InvalidateItemsForSarRegion(seq_items[index]);

    }

    m_acquisition->CalcSARControlAndPutVFForAll();

    EnableDisableScanAnatomy();

    //Patni-Hemant/2009Nov16/Added/IR#90/Bottom Buttons
    UpdateSarAnatomyButton();

    DEL_PTR(pdata)

    return true ;
}

//************************************Method Header************************************
// Method Name  : SetAnatomy
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetAnatomy(
    const int sel_seq_idx ,
    const int iAnatomy
)
{
    //Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(sel_seq_idx);

    //as per old code :
    //if Batch setting is on
    //- apply anatomy to first selected item and the remaining all items.
    //else
    // - apply anatomy to only selected items.

    int* selected_items = NULL ;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items);

    if (!selected_items || !count) {
        return ;
    }

    if (IsBatchSettingOn()) {

        //batch setting is on so apply anatomy settings to all the items below the first selected item (including first selected item)

        const int currently_sel_item = selected_items[0] ;
        count = (m_pqm_listctrl.GetItemCount() - currently_sel_item) ;

        DEL_PTR_ARY(selected_items)

        selected_items = new int[count] ;

        int counter = 0;

        for (int item = currently_sel_item; counter < count; item++)
            selected_items[counter++] = item;
    }

    SetAnatomy(selected_items, count, iAnatomy);

    //Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(selected_items);
}



//+Patni-ARD/2009May25/Added/GL_SR_SQSaved issue
/****************************Method Header************************************
//Module         : GetSRParamInfo
//Author         : PATNI/ARD
//Purpose        : Get the SR param and create new SR param with series loid
//*****************************************************************************/
CString CPQMView::GetSRParamInfo(
    const bool f_SeriesType
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetSRParamInfo") ;

    CString l_series_appended = _T("");

    if (f_SeriesType) {

        //Patni-AMT/2009Jun24/Modified/JFT# SR_PQM message TMSC comment ',' added
        l_series_appended.Format(_T("[%s,%s,[%s,%s],%s]"), m_srInfo.m_strPatID,
                                 m_srInfo.m_strStudyID,
                                 GetSequqnceID(),
                                 m_srInfo.m_strPSPOID,
                                 m_srInfo.m_strDBName);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_series_appended);

    } else {

        VARIANT* pdata = new VARIANT;
        m_acquisition->GetNewSeriesLoids(pdata);

        //+Patni-ARD/2009Jun10/Modified/JFT# PQM SR Communication GL_SR_SQSaved
        CStringArray* l_seriesarray = (CStringArray*)pdata->byref;
        CString l_serieslist = _T("");

        if (l_seriesarray != NULL && l_seriesarray->GetSize() > 0) {

            for (int i = 0; i < l_seriesarray->GetSize(); i++) {
                //Patni-ARD/2009Jul25/Modified/IR
                l_serieslist += l_seriesarray->GetAt(i) + MULTI_OID_DELIMITER;
            }

            //Patni-ARD/2009Jul25/Modified/IR
            l_serieslist.TrimRight(MULTI_OID_DELIMITER);

            l_series_appended.Format(_T("[%s,%s,[%s,%s],%s]"), m_srInfo.m_strPatID,
                                     m_srInfo.m_strStudyID, l_serieslist, m_srInfo.m_strPSPOID,
                                     m_srInfo.m_strDBName);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_series_appended);
            //-Patni-ARD/2009Jun10/Modified/JFT# PQM SR Communication GL_SR_SQSaved

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("*** SeriesLOID Data Null - GL_SR_SQSaved not send ****"));
            l_series_appended = _T("");
        }

        DEL_PTR(pdata)

    }

    //-Patni-ARD/2010Feb18/Modified/Ph-3# PROPOSAL_06-Log Correction
    return l_series_appended;
}

void CPQMView::GetCoilStringForCurrentProtocol(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetCoilStringForCurrentProtocol") ;

    CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(m_scan_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
        return ;
    }

    const int l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

    const CString recv_coil_label = GetCoilStringFor(l_acq_order);

    if (!recv_coil_label.IsEmpty()) {

        l_seq->SetValue(rc, recv_coil_label);

        CPqmPrMode& prmode = GetPrModeOfSelectedSequence(m_scan_index);
        prmode.m_rcv_coil_id = __recv_coil_id;
        prmode.m_recv_coil_label = recv_coil_label;

        m_coil_info_static.SetWindowText(recv_coil_label);
    }
}

//****************************Method Header************************************
//Module         : GetSequenceParamList
//Author         : PATNI/DJ
//Purpose        : Get the serialized sequence parameter list from PQM IPC.
//*****************************************************************************
void CPQMView::GetSequenceParamList(
)const
{

    VARIANT* pData = new VARIANT;

    g_sequence_param_name_list.RemoveAll();

    m_acquisition->GetSeqParamNameList(pData);

    g_sequence_param_name_list.AddTail((Data_decl_list*)pData->byref);

    DEL_PTR(pData)
}

CString CPQMView::GetCoilStringFor(
    const int f_acq_order
) const //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_17
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetCoilStringFor");

    //+Patni-AMT/2010Jan29/Modified/Ph-3# PROPOSAL_01-MEM_LEAK
    CString l_coil_name = _T("");
    VARIANT* pData      = new VARIANT;

    if (pData == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pData for getting new coil name is NULL"));
        return l_coil_name;
    }

    pData->byref = &l_coil_name;
    m_acquisition->GetCoilNameFromAcqOrder(f_acq_order , &pData) ;

    pData->byref = NULL;
    DEL_PTR(pData)

    return l_coil_name ;
    //-Patni-AMT/2010Jan29/Modified/Ph-3# PROPOSAL_01-MEM_LEAK
}

/************************************Method Header**************************************
//Method Name:  GetSequenceList
//Author:  PATNI/LK - JS
//Purpose:
//**************************************************************************************/
void CPQMView::GetSequenceList(
    const int f_firstseq
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetSequenceList");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_pqm_sequences) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_sequences pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    VARIANT* pData = new VARIANT;

    int l_mode = -1;
    m_acquisition->GetSeqDataofDisplaySequences(pData, l_mode, f_firstseq);


    //  m_pqm_sequences->m_rec_seq_list.AddTail((SequenceList*) pData->byref);

    //Patni-Rajendra/2011Mar10/Modified/Redmine-783
    m_pqm_sequences->GetRecSeqList().AddTail((SequenceList*) pData->byref);

    //Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    DEL_PTR(pData)
}


//****************************Method Header************************************
//Module         : GetSequenceList
//Author         : PATNI/DJ
//Purpose        : Get the serialized sequence list from PQM IPC.
//*****************************************************************************
void CPQMView::GetSequenceList(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetSequenceList");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_pqm_sequences) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_sequences pointer is NULL"));


        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    VARIANT* pData = new VARIANT;
    int mode = -1;
    m_acquisition->GetSeqData(pData, mode);

    //  m_pqm_sequences->m_rec_seq_list.AddTail((SequenceList*) pData->byref);

    //Patni-Rajendra/2011Mar10/Modified/Redmine-783
    m_pqm_sequences->GetRecSeqList().AddTail((SequenceList*) pData->byref);

    DEL_PTR(pData)

    //-Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK
}


//Nitin Coil
CString CPQMView::GetCoilStringForTransmiter(
    const int f_acq_order
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetCoilStringForTransmiter");

    //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    CString l_coil_name = _T("");
    VARIANT* pData = new VARIANT;

    if (pData == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pData for getting new coil name is NULL"));
        return l_coil_name;
    }

    pData->byref = &l_coil_name;
    m_acquisition->GetCoilNameFromTransmiter(f_acq_order, &pData) ;

    pData->byref = NULL;
    DEL_PTR(pData)

    return l_coil_name ;
    //-Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
}
//Nitin Coil

//Nitin Coil
CString CPQMView::GetCoilStringForReceiver(
    const int f_acq_order
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetCoilStringForReceiver");

    //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    CString l_coil_name = _T("");
    VARIANT* pData      = new VARIANT;

    if (pData == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pData for getting new coil name is NULL"));
        return l_coil_name;
    }

    pData->byref = &l_coil_name;
    m_acquisition->GetCoilNameFromReceiver(f_acq_order , &pData) ;

    pData->byref = NULL;
    DEL_PTR(pData)

    return l_coil_name ;
    //-Patni-AMT/2010Jan29/Added/Ph-3#
}
//Nitin Coil

/****************************Method Header************************************
//Module         : SetBtnSensitive
//Author         : PATNI/SG
//Purpose        : Set the Button State Sensitive/Insensitive
//*****************************************************************************/
void CPQMView::SetBtnSensitive(const int f_id, const bool f_state)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetBtnSensitive");

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    CWnd* l_dlgitem = GetDlgItem(f_id);

    if (NULL == l_dlgitem) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetDlgItem(f_id) pointer is NULL"));

        return ;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    if ((f_id == IDC_SCAN_START) && !IsAcqumanReadyForNextRequest()) {
        l_dlgitem->EnableWindow(false);

    } else {
        l_dlgitem->EnableWindow(f_state);
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetFlagRelatedCtrlsStates(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetFlagRelatedCtrlsStates");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
    //+Patni-Hemant/2011Mar30/Added/REDMINE-1575
    try {
        SetIconForButton(m_pass_button, (!GetMemoryAliveFlag() && !m_is_wse_open && !IsAMB()) ?  IDI_PASS_BUTTON_IMAGE : -1);
        m_pass_button.EnableWindow(!GetMemoryAliveFlag() && !m_is_wse_open && !IsAMB());

        //+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
        //m_close_button.EnableWindow(!GetMemoryAliveFlag() && !m_is_wse_open && !IsScanning());

        UpdateEndExamButtonStatus();
        //-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
        //-Patni-Hemant/2011Mar30/Added/REDMINE-1575

        EnableDisableScanEditOrPlanButtons();

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred..."));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, GetPQMFlagsString());
        throw;
    }
}


void CPQMView::UpdateEndExamButtonStatus(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateEndExamButtonStatus");

    try {
        bool l_can_semsetive_end_exam = !IsAMB() && (!GetMemoryAliveFlag() && !m_is_wse_open && !IsScanning() && !GetPostProcessStatus() && (m_autolocator_app_cntl.GetAutoLocatorProcessID() == -1));

        if (l_can_semsetive_end_exam && (-1 == GetScanIndex())) {

            const int l_first_wait_prot = m_pqm_listctrl.GetFirstWaitProtocolIndex();

            if (IsValidSequenceIndex(l_first_wait_prot)) {
                CSequence* l_seq = reinterpret_cast<CSequence*>(m_pqm_listctrl.GetItemData(l_first_wait_prot));

                if (NULL != l_seq) {
                    l_can_semsetive_end_exam = !((l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD")) & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN);

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq pointer is NULL"));
                }



                if (!l_can_semsetive_end_exam && -1 != CheckAndGetWaitingForAutoScanStartAcqOrder()) {
                    l_can_semsetive_end_exam = true;
                }
            }
        }

        SetBtnSensitive(IDC_END_EXQAM, l_can_semsetive_end_exam);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred..."));
        throw;
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::GetCurrentScanCommentAndScanID(
    CString* f_scan_id /*= NULL*/,
    CString* f_scan_comment /*= NULL*/
)
{

    if (f_scan_id || f_scan_comment) {

        CSequence* pSeq = GetSelectedSequence(GetSelectedSeqIndex());

        if (!pSeq)
            return false;

        CPASParamStore* seq_para =  pSeq->GetCPASCOMSequence()->GetSequenceParam();

        if (seq_para) {

            if (f_scan_id)
                *f_scan_id = seq_para->GetString(_T("COMM"));

            if (f_scan_comment)
                *f_scan_comment = seq_para->GetString(_T("MEMO"));
        }
    }

    return true ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetCurrentScanCommentAndScanID(
    CString* f_scan_id /*= NULL*/,
    CString* f_scan_comment /*= NULL*/,
    const int seq_index /*= -1*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetCurrentScanCommentAndScanID");

    if (f_scan_id || f_scan_comment) {


        const int selected_index = (seq_index != -1) ? seq_index : GetSelectedSeqIndex();
        CSequence* pSeq = (CSequence*) m_pqm_listctrl.GetItemData(selected_index);

        //+Patni-PJS/Modified/2009Nov11/MSA0248-0033/Code Review
        if (!pSeq) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("pSeq is NULL"));
            return;
        }

        CPASCOMSequence* l_pascom_seq = pSeq->GetCPASCOMSequence();

        if (l_pascom_seq == NULL) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("l_pascom_seq is NULL"));
            return;
        }

        //-Patni-PJS/Modified/2009Nov11/MSA0248-0033/Code Review

        CPASParamStore* seq_para = l_pascom_seq->GetSequenceParam();

        if (seq_para) {

            //+Patni-PJS/Added/2009Nov11/MSA0248-0033
            CString l_str_loid = _T("");
            l_pascom_seq->GetSequenceID(l_str_loid);
            BSTR l_bstr_loid = (BSTR)l_str_loid.AllocSysString();
            //-Patni-PJS/Added/2009Nov11/MSA0248-0033

            const int l_scan_index = _ttoi(seq_para->GetString(_T("ACQORDER")));
            int l_result = 0;

            VARIANT* pdata = new VARIANT;

            if (f_scan_id) {

                BSTR l_pstr = (*f_scan_id).AllocSysString();
                pdata->wReserved1 = SCAN_ID;
                pdata->byref = l_pstr;
                //+Patni-MP/Modified/2009Mar12/DefectNo#PSP239
                l_result = GetAcquisition()->UpdateGUIControlsInVarfieldDB(pdata,
                           l_scan_index);//Nitin Changes for Rename

                if (0 == l_result) {
                    seq_para->SetString(_T("COMM"), *f_scan_id);
                }

                //+Patni-RSG/Added/2009Nov25/MSA0248-0033
                if (pSeq->GetAcqStatus() == PR_ACQ_STATUS_SUCCESS || pSeq->GetAcqStatus() == PR_ACQ_STATUS_FAIL) {
                    l_result = GetAcquisition()->UpdateProtocolDataInSeries(pdata, l_bstr_loid, l_scan_index);

                } else {
                    l_result = GetAcquisition()->UpdateGUIControlsInDB(pdata, l_bstr_loid);
                }

                //-Patni-RSG/Added/2009Nov25/MSA0248-0033

                if (0 == l_result) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("Failed to update Scan id in local DB"));
                }

                //-Patni-PJS/Added/2009Nov11/MSA0248-0033
            }

            if (f_scan_comment) {

                BSTR l_pstr = (*f_scan_comment).AllocSysString();
                pdata->wReserved1 = SCAN_COMMENT;
                pdata->byref = l_pstr;

                //+Patni-MP/Modified/2009Mar12/DefectNo#PSP239
                l_result = GetAcquisition()->UpdateGUIControlsInVarfieldDB(pdata,
                           l_scan_index);		//Nitin Changes for Rename

                if (0 == l_result) {
                    seq_para->SetString(_T("MEMO"), *f_scan_comment);
                }

                //+Patni-RSG/Added/2009Nov25/MSA0248-0033
                if (pSeq->GetAcqStatus() == PR_ACQ_STATUS_SUCCESS || pSeq->GetAcqStatus() == PR_ACQ_STATUS_FAIL) {
                    l_result = GetAcquisition()->UpdateProtocolDataInSeries(pdata, l_bstr_loid, l_scan_index);

                } else {
                    l_result = GetAcquisition()->UpdateGUIControlsInDB(pdata, l_bstr_loid);
                }

                //-Patni-RSG/Added/2009Nov25/MSA0248-0033

                if (l_result == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("Failed to update Scan comment in local DB"));
                }

                //-Patni-PJS/Added/2009Nov11/MSA0248-0033

            }

            if (pdata) {

                pdata->byref = NULL ;
                DEL_PTR(pdata)
            }

            //+Patni-MP/Modified/2009Mar12/DefectNo#PSP239
            if (l_result == 0) {
                m_pqm_listctrl.UpdateSequences(&selected_index, 1);
            }

            //-Patni-MP/Modified/2009Mar12/DefectNo#PSP239
        }
    }

    //Added by Hemant On 12/5/2008 9:10:44 PM
    OnProtocolCommentChanged();
    return ;
}


//************************************Method Header************************************
// Method Name  : GetSequenceInfo
// Author       : PATNI/ Nitin
// Purpose      :
//***********************************************************************************
CString CPQMView::GetSequenceInfo()const /*= NULL*/
{
    CString  f_skip_prescan = _T("");
    int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (IsValidSequenceIndex(first_selected_item)) {

        //+Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1
        VARIANT* pData = new VARIANT;

        if (S_OK == m_acquisition->GetScanAgent(
                GetAcqOderForGivenProtocolIndex(first_selected_item), pData)
           ) {

            f_skip_prescan = *((CString*)pData->byref);
        }

        DEL_PTR(pData);
        //-Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1
    }

    return f_skip_prescan ;
}


//************************************Method Header************************************
// Method Name  : SetSequenceInfo
// Author       : PATNI/ Nitin
// Purpose      :
//***********************************************************************************

//+Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
void CPQMView::SetSequenceInfo(
    const CString& f_seq_info
)const
{

    const int first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (IsValidSequenceIndex(first_selected_item)) {

        VARIANT* pdata = new VARIANT;
        BSTR l_pstr = (f_seq_info).AllocSysString();
        pdata->wReserved1 = SKIP_PRESCAN;
        pdata->byref = l_pstr;

        const int l_result = GetAcquisition()->UpdateGUIControlsInVarfieldDB(pdata,
                             GetAcqOderForGivenProtocolIndex(first_selected_item));

        if (l_result == 0) {

            CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(first_selected_item);
            CPASParamStore* seq_para =  l_seq->GetCPASCOMSequence()->GetSequenceParam();
            seq_para->SetString(_T("SKIP_PRESCAN"), f_seq_info);
        }

        if (pdata) {
            pdata->byref = NULL;
            DEL_PTR(pdata)
        }

        ::SysFreeString(l_pstr);
    }
}
//-Patni/Nitin/ACE2/ACE2-Skip Precan Requirement

//+Patni-ARD/2009May25/Modify/GL_SR_SQSaved issue
/****************************Method Header************************************
//Module         : GetSRParam
//Author         : PATNI/SG
//Purpose        : Get the SR param value
//*****************************************************************************/
CString CPQMView::GetSRParam()const
{
    return m_sr_param;
}
//+Patni-ARD/2009May25/Modify/GL_SR_SQSaved issue
/****************************Method Header************************************
//Module         : SetSRParam
//Author         : PATNI/SG
//Purpose        : Set the SR param value
//*****************************************************************************/
//Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979
BOOL CPQMView::SetSRParam(const CString& f_param,
                          const CString& f_PASFilename,
                          bool f_update_pps)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetSRParam");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-ARD/2009May25/Modify/GL_SR_SQSaved issue
    m_sr_param = f_param;

    if (!f_update_pps) {
        m_srInfo.Clear();
    }


    //+Patni-PJS/2010Oct27/Modified/REDMINE-859
    try {

        //+Patni-PJS/2010Nov4/Modified/REDMINE-859/Error Scenario Fixed
        const int l_token_pos_ploid = f_param.Find(',');

        //Extracting patient loid
        if (-1 == l_token_pos_ploid) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("Invalid SR Param string"));
            return FALSE;
        }

        CString l_patient_loid = f_param.Left(l_token_pos_ploid);

        if ('[' != l_patient_loid.GetAt(0)) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("Invalid SR Param string"));
            return FALSE;
        }

        l_patient_loid = l_patient_loid.Right(l_patient_loid.GetLength() - 1);

        //Extracting study loid.
        CString l_temp_srparam = f_param.Mid(l_token_pos_ploid + 1 , f_param.GetLength());
        int l_token_pos_sloid = l_temp_srparam.Find(',');

        if (-1 == l_token_pos_sloid) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("Invalid SR Param string"));
            return FALSE;
        }

        CString l_study_loid = l_temp_srparam.Left(l_token_pos_sloid);
        //Extracting series_loid
        l_temp_srparam = l_temp_srparam.Mid(l_token_pos_sloid + 1 , f_param.GetLength());
        int l_token_series = l_temp_srparam.Find(',');

        if (-1 == l_token_series) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("Invalid SR Param string"));
            return FALSE;
        }

        CString l_series_loid = l_temp_srparam.Left(l_token_series);
        CString l_series = l_series_loid.Right(l_series_loid.GetLength() - 1);

        //Extracting ppsInfo
        l_temp_srparam = l_temp_srparam.Mid(l_token_series + 1 , f_param.GetLength());
        const int l_token_pos_pps_info = l_temp_srparam.Find(',');

        if (-1 == l_token_pos_pps_info) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("Invalid SR Param string"));
            return FALSE;
        }

        CString l_pps_info = l_temp_srparam.Left(l_token_pos_pps_info);
        l_pps_info = l_pps_info.Left(l_pps_info.GetLength() - 1);
        l_temp_srparam = l_temp_srparam.Mid(l_token_pos_pps_info + 1 , f_param.GetLength());

        int l_token_pos_dbname = l_temp_srparam.Find(']');

        if (-1 == l_token_pos_dbname) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("Invalid SR Param string"));
            return FALSE;
        }

        CString l_db_name = l_temp_srparam.Left(l_token_pos_dbname);
        l_db_name = l_db_name.Left(l_pps_info.GetLength() - 1);
        m_srInfo.m_strPSPOID = l_pps_info;

        if (!f_update_pps) {
            m_srInfo.m_strPatID = l_patient_loid;
            m_srInfo.m_strStudyID = l_study_loid;
            m_srInfo.m_strDBName = l_db_name;
            m_srInfo.m_strPASFilename = f_PASFilename;
        }

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME,
                  _T("Patient Loid: ") + m_srInfo.m_strPatID + _T(" Study Loid: ") + m_srInfo.m_strStudyID
                  + _T(" Series Loid: ") + l_series
                  + _T(" PPS Loid: ") + m_srInfo.m_strPSPOID
                  + _T(" DB Name: ") + m_srInfo.m_strDBName);

        if (l_patient_loid.IsEmpty() || l_study_loid.IsEmpty() || l_pps_info.IsEmpty() || l_db_name.IsEmpty()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid SSrParamInfo parameter"));
            return FALSE;
        }

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception in CPQMView::SetSRParam"));
        return FALSE;
    }

    //Patni-PJS/2010Oct27/Modified/REDMINE-859
    return TRUE;
    //-Patni-ARD/2009May25/Modify/GL_SR_SQSaved issue
}

//+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
//************************************Method Header************************************
// Method Name  : IsMRSProcessToolStarted
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
bool CPQMView::IsMRSProcessToolStarted(
)const
{
    return m_mrs_process_tool;
}

//************************************Method Header************************************
// Method Name  : SetMRSProcessToolStarted
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
void CPQMView::SetMRSProcessToolStarted(
    const bool f_mrs_process_tool_started
)
{
    m_mrs_process_tool = f_mrs_process_tool_started;
}
//-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480

//Patni-PJS/2010Mar10/Modified/MSA0251-00243
int CPQMView::GetMemInvocationProcessSt()const
{
    return m_mem_invocation_status;
}

void CPQMView::SetMemInvocationProcessSt(const int f_mem_invocation_status)
{
    m_mem_invocation_status = f_mem_invocation_status;
}
//Patni-PJS/2010Mar10/Modified/MSA0251-00243

//****************************Method Header************************************
//Method Name   : GetMismatchErrorFlag()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
BOOL CPQMView::GetMismatchErrorFlag(
)const
{
    return m_mismatch_error_flag;
}


//****************************Method Header************************************
//Method Name   : SetMismatchErrorFlag()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CPQMView::SetMismatchErrorFlag(
    const BOOL f_mismatch_error_flag
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetMismatchErrorFlag") ;
    m_mismatch_error_flag = f_mismatch_error_flag;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_mismatch_error_flag ? _T("SetMismatchErrorFlag to TRUE") : _T("SetMismatchErrorFlag to FALSE"));
}



//***************************Method Header*************************************

//Method Name    :GetMemorymode()

//Author         :PATNI/SG

//Purpose        :

//*****************************************************************************
BOOL CPQMView::GetMemorymode(
)const
{
    return m_store_mode;
}


//***************************Method Header*************************************
//Method Name    :SetMemoryMode()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
void CPQMView::SetMemoryMode(
    const bool f_memory_mode
)
{
    m_store_mode = f_memory_mode;
}

//************************************Method Header**************************************
//Method Name:  GetScanMode
//Author:  PATNI/MSN
//Purpose: Get Scan Mode
//**************************************************************************************/
bool CPQMView::GetPQMCloseFlag()const
{
    return m_pqm_closed_flag;
}

//************************************Method Header**************************************
//Method Name:  SetPQMCloseFlag
//Author:  PATNI/MSN
//Purpose: Set PQM Close Flag
//**************************************************************************************/
void CPQMView::SetPQMCloseFlag(const bool f_pqm_closed_flag)
{
    m_pqm_closed_flag = f_pqm_closed_flag;
}

//+Patni-HEMANT/ADDED On 3/27/2009 1:58:14 PM/ PSP#311
bool CPQMView::GetMemoryAliveFlag(
)const
{
    //Patni-Hemant/2011Mar30/Added/REDMINE-1575
    //Patni-Hemant/2011Apr20/Modified/IR185 A-PAS re-open
    return (m_ismemoryalive
            || (MEMORY_INVOKATION_STARTS == (m_memory_invokation_flag & MEMORY_INVOKATION_STARTS))
            || (MEMORY_PASSWORD_DIALOG_OPEN == (m_memory_invokation_flag & MEMORY_PASSWORD_DIALOG_OPEN))
           ) ;
}
//-Patni-HEMANT/ADDED On 3/27/2009 1:58:14 PM/ PSP#311


/****************************Method Header************************************
//Module         : SetMemoryAliveFlag
//Author         : PATNI/SG
//Purpose        : Set the Flag if Memory is alive
//*****************************************************************************/
void CPQMView::SetMemoryAliveFlag(const bool f_memory_alive)
{

    //+Patni-Hemant/2011Mar30/Modified/REDMINE-1575
    //+Patni-Hemant/2011Mar30/Added/REDMINE-1575
    m_ismemoryalive = f_memory_alive;
    m_memory_invokation_flag = 0;		//No tmemory present..

    SetFlagRelatedCtrlsStates();
    //-Patni-Hemant/2011Mar30/Modified/REDMINE-1575

    if (m_ismemoryalive) {
        m_memory_invokation_flag |= (MEMORY_INVOKATION_ENDS | MEMORY_PRESENT) ;
        SetWaitCursor(false);
    }
}

bool CPQMView::IsNextProtocolAutoScanAfterStatusDone() const
{

    switch (g_MainView->GetLastAcqmanHandlderMsg()) {

        case PQM_MSG_STATUS_DONE :
        case AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL:
        case PQM_MSG_CAN_AUTO_SCAN_START:
        case PQM_MSG_POST_PROCESS_END:
        case PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END:
        case PQM_MSG_CFA_PRESCAN_END:

            const int l_first_wait_prot = m_pqm_listctrl.GetFirstWaitProtocolIndex();

            if (IsValidSequenceIndex(l_first_wait_prot)) {
                CSequence* l_seq = (CSequence*)(m_pqm_listctrl.GetItemData(l_first_wait_prot));
                bool auto_scan_on = ((l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD")) & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) ;

                if (auto_scan_on) {
                    return true;
                }
            }
    }

    return false;
}

/****************************Method Header************************************
//Module         : IsFirstWaitProtocolEditedbyWSE
//Author         : PATNI
//Purpose        : Returns true if WSE is invoked for first top wait protocol
//*****************************************************************************/
bool CPQMView::IsFirstWaitProtocolEditedbyWSE(
) const
{
    return ((m_pqm_listctrl.GetFirstSelectedItem() == m_pqm_listctrl.GetFirstWaitProtocolIndex()) &&
            (m_pqm_listctrl.GetFirstSelectedItem() == m_pqm_listctrl.GetWSEEditingProtocolIndex())) ;
}

bool CPQMView::CanEnableCFARetry()const
{
    return (IsAcqumanReadyForNextRequest() &&
            !IsWaitCursor() &&
            (-1 == GetScanIndex()) &&
            (IsValidSequenceIndex(m_pqm_listctrl.GetFirstSelectedItem())) &&
            !m_rmc_scan_running &&
            !GetPostProcessStatus() &&
            !GetIsAMBPlanning() &&
            !IsAMBInvoked());
}

//****************************Method Header************************************
//Module         : GetScanIndex
//Author         : PATNI/MSN
//Purpose        : Getter method to get the Scan Index
//*****************************************************************************
int CPQMView::GetScanIndex(
)const
{
    return m_scan_index;
}


// + LOKESH 6 June 2007
//****************************Method Header************************************
//Module         : SetScanIndex
//Author         : PATNI/LK
//Purpose        : Setter method to Set the Scan Index
//*****************************************************************************
void CPQMView::SetScanIndex(
    const int f_scanindex
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetScanIndex");

    CString l_scanindex(_T(""));	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
    l_scanindex.Format(L"Scan Index = %d", f_scanindex);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_scanindex);

    m_scan_index = f_scanindex;
}
// - LOKESH 6 June 2007


//************************************Method Header****************************

//Method Name   :GetShimDoneFlag()

//Author        :PATNI/HAR

//Purpose       :Get Shimming Done Flag

//*****************************************************************************
int CPQMView::GetShimDoneFlag(
)const
{
    return m_shim_done_flag;
}


//************************************Method Header****************************

//Method Name   :SetShimDoneFlag()

//Author        :PATNI/HAR

//Purpose       :Set Shimming Done Flag

//*****************************************************************************
void CPQMView::SetShimDoneFlag(
    const int f_shim_done_flag
)
{
    m_shim_done_flag = f_shim_done_flag;
}

//************************************Method Header****************************

//Method Name   :GetShimStateFlag()

//Author        :PATNI/HAR

//Purpose       :

//*****************************************************************************
int CPQMView::GetShimStateFlag(
)const
{
    return m_shim_state_flag;
}




//************************************Method Header****************************

//Method Name   :SetShimStateFlag()

//Author        :PATNI/HAR

//Purpose       :

//*****************************************************************************
void CPQMView::SetShimStateFlag(
    const int f_shim_state_flag
)
{
    m_shim_state_flag = f_shim_state_flag;
}


BOOL CPQMView::GetPostProcessStatus() const
{

    return (S_OK == m_acquisition->GetPostProcessFlag());
}

CPqmChildThread& CPQMView::GetPqmChildThread()
{

    return m_pqmchildthread;
}



//+Patni-PJS/2010Nov4/Added/REDMINE-859/Error Scenario Fixed
//************************************Method Header**************************************
// Method Name  : GetSRParamInfoObj
// Author       : PATNI/ AJS
// Purpose      : This function is used to get the object of SRParamInfo memeber variable
//***************************************************************************************
SrParamInfo* CPQMView::GetSRParamInfoObj()
{
    return &m_srInfo;
}
//-Patni-PJS/2010Nov4/Modified/REDMINE-859/Error Scenario Fixed


//**************************************************************************************************
//Method Name   :  GetTempSequence()
//Author        :  PATNI/PJS
//Purpose       :  This function returns the current sequnece which is under scan and not stored in
//				   scratch pad. This provide sequence to be duplicated when protocol is in current
//				   state and avoid to duplicate the protocol from scratch pad.
//**************************************************************************************************
CSequence* CPQMView::GetCurrentSequence(
)const
{

    return m_curr_seq;
}

//+Patni-ARD/2010Jan29/Added/DeFT# MVC4446
//************************************Method Header**********************************
// Method Name  : GetIsLocatorOpen
// Author       : PATNI/ARD
// Purpose      : return true if locator is opened
//***********************************************************************************
bool CPQMView::GetIsLocatorOpen()const
{
    return m_is_locator_open;
}
//-Patni-ARD/2010Jan29/Added/DeFT# MVC4446

//************************************Method Header**********************************
// Method Name  : GetWaitCursor
// Author       : Igate/SM
// Purpose      : Return true if wait cursor is set in Queue Dialog (Q&E buttons etc.)
//***********************************************************************************
bool CPQMView::GetWaitCursor()const
{
    if (m_queuedialog != NULL) {
        return m_queuedialog->GetWaitCursor();
    }

    return false;
}

//**************************************************************************
//Method Name   : Getstudyloid
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
CString CPQMView::Getstudyloid()const
{
    return m_studyloid;
}

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetMagneticCenterPos
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
float CPQMView::GetMagneticCenterPos(
)const
{

    if (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd)) {

        VARIANT* pData = new VARIANT;

        m_acquisition->GetMagneticCenterPos(&pData);

        const float f_center_pos = (pData->fltVal) ;

        if (pData) {
            pData->byref = NULL ;
            DEL_PTR(pData)
        }

        return f_center_pos ;
    }

    return 0.0 ;

}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

//************************************Method Header************************************
// Method Name  : GetPQMCDSControllerInterface
// Author       : PATNI/ HEMANT
// Purpose      : Returns the interface for cds contorller
//***********************************************************************************
/*CPQMCDSControllerInterface*/CPQMCDSController* CPQMView::GetPQMCDSControllerInterface(
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_17
{
    //return (/*CPQMCDSControllerInterface*/CPQMCDSController*)(m_cds_controller);
    return m_cds_controller ;
}





// + Patni-Sribanta/Added/2010Jun24/IR-156_2

//************************************Method Header************************************
// Method Name  : GetHoveredProtocolIdent
// Author       : PATNI/ Sribanta
// Purpose      : Get the scan Ident for the protocol on which the mouse is hovered.
//***********************************************************************************

bool CPQMView::GetHoveredProtocolIdent(
    CString* f_scan_ident /*= NULL*/,
    const int f_scan_index
)const
{

    if (IsValidSequenceIndex(f_scan_index) && (f_scan_ident))  {

        CSequence* pSeq = GetSelectedSequence(f_scan_index);

        if (!pSeq)
            return false;

        CPASParamStore* seq_para =  pSeq->GetCPASCOMSequence()->GetSequenceParam();

        if (seq_para) {
            if (f_scan_ident)
                *f_scan_ident = seq_para->GetString(_T("IDENT"));

        }
    }

    return true ;
}

//-Patni-Sribanta/Added/2010Jun24/IR-156_2


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMView::GetAcqOderForGivenProtocolIndex(
    const int f_protocol_list_index
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetAcqOderForGivenProtocolIndex");

    if (!IsValidSequenceIndex(f_protocol_list_index)) {
        return -1;
    }

    CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(f_protocol_list_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
        return -1;
    }

    return _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
}


int CPQMView::GetProtocolIndexFromAcqorder(
    const int f_protocol_acqorder
)const
{

    for (int i = 0; i < m_pqm_listctrl.GetItemCount() ; i++) {
        if (f_protocol_acqorder == GetAcqOderForGivenProtocolIndex(i)) {
            return i ;
        }
    }

    return -1;
}

//+Patni-NP/2009Sept15/Added/MVC#004766
//************************************Method Header************************************
// Method Name  : GetDBDtMode
// Author       : PATNI/ NP
// Purpose      :
//***********************************************************************************
int CPQMView::GetDBDtMode(const int f_acq_order)const
{
    //REDMINE_781
    /*
    int* l_current_dbdt_mode = new int;
    //+Patni-NP/2009Sept22/Added/MVC#004914
    *l_current_dbdt_mode = NULL;

    if (m_acquisition) {
        m_acquisition->GetDBDTCurrentMode(f_acq_order , l_current_dbdt_mode);
    }

    //-Patni-NP/2009Sept22/Added/MVC#004914
    return *l_current_dbdt_mode;
    */

    int l_current_dbdt_mode = 0;

    if (m_acquisition) {
        m_acquisition->GetDBDTCurrentMode(f_acq_order , &l_current_dbdt_mode);
    }

    return l_current_dbdt_mode;

}
//-Patni-NP/2009Sept15/Added/MVC#004766


//****************************Method Header************************************
//Module         : GetSelectedSequence
//Author         : PATNI/DJ
//Purpose        : Return the reference for selected sequence.
//*****************************************************************************
CSequence* CPQMView::GetSelectedSequence(
    const int index
)const
{
    return (CSequence*) m_pqm_listctrl.GetItemData(index);
}




//****************************Method Header************************************
//Module         : GetInfoLableArray
//Author         : PATNI/GP
//Purpose        : Get long lable array
//*****************************************************************************
CStringArray& CPQMView::GetInfoLableArray(
)
{
    return m_info_long_lable;
}


//****************************Method Header************************************
//Module         :
//Author         : PATNI/EG+AR
//Purpose        : Update Protocol Status as per scanning input from IPC
//*****************************************************************************
long CPQMView::GetMaxProtocolLimit(
)
{
    m_acquisition->GetMaxProtocolLimit(&m_pqm_max_protocol);
    return m_pqm_max_protocol;
}

//****************************Method Header************************************
//Module         : GetTagArray
//Author         : PATNI/DJ
//Purpose        : Return the reference of Sequence tag array.
//*****************************************************************************
CStringArray& CPQMView::GetTagArray(
)
{
    return m_tag_arr;
}


//****************************Method Header************************************
//Module         : GetPrModeOfSelectedSequence
//Author         : PATNI/DJ
//Purpose        : Return the PR Mode of selected Sequence.
//*****************************************************************************
CPqmPrMode& CPQMView::GetPrModeOfSelectedSequence(
    const int index
)const
{
    return ((CSequence*) m_pqm_listctrl.GetItemData(index))->GetPrMode();
}

//****************************Method Header************************************
//Module         : GetPQMListCtrl
//Author         : PATNI/DJ
//Purpose        : Return the reference of PQM List control.
//*****************************************************************************
CPQMListCtrl&   CPQMView::GetPQMListCtrl(
)
{
    return m_pqm_listctrl;
}


//****************************Method Header************************************

//Module         : GetHeaderArray

//Author         : PATNI/GP

//Purpose        : Get list header Array

//*****************************************************************************
CStringArray& CPQMView::GetHeaderArray(
)
{
    return m_str_arr;
}

//************************************Method Header**************************************

//Method Name:  GetPQMG

//Author:  PATNI/SVP

//Purpose: Returns PQMG object.

//**************************************************************************************/
CPQMGlobal& CPQMView::GetPQMGlobal(
)
{

    return m_pqm_g;
}

//****************************Method Header************************************
//Module         : GetPQMSequence
//Author         : PATNI/MSN
//Purpose        : Getter method to get the PQM sequence
//*****************************************************************************
CIPCComManager* const CPQMView::GetPQMSequence(
)
{
    return m_pqm_sequences;
}

//****************************Method Header************************************
//Module         : GetSeqList
//Author         : PATNI/MSN
//Purpose        : Getter method to get the Sequence List
//*****************************************************************************
CSeqList* const CPQMView::GetSeqList(
)
{
    return m_list;
}

//****************************Method Header************************************
//Module         : GetAcquisition
//Author         : PATNI/MSN
//Purpose        : Getter method to get the Com Ptr
//*****************************************************************************
IPqmIpcCom* CPQMView::GetAcquisition()const //TMSC-Tanoue/2010Sep03/MVC008324
{
    return m_acquisition.p;
}

//****************************Method Header************************************
//Module         : GetMsgSeqLimit
//Author         : PATNI/MSN
//Purpose        : Getter method to get the Sequence Limit
//*****************************************************************************
BOOL const CPQMView::GetMsgSeqLimit(
)
{
    return m_msg_seq_limit;
}


//+Patni-HEMANT/ADDED On 8/12/2009 10:48:36 AM/ Restructure
void CPQMView::GetManualPreScanDefaultData(
)const
{
    if (m_pqmmanualprescandlg) {
        m_pqmmanualprescandlg->GetManualPreScanDefaultData();
    }
}
//-Patni-HEMANT/ADDED On 8/12/2009 10:48:36 AM/ Restructure


//***************************Method Header*************************************
//Method Name    :GetManualPreScanDefaultData
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
BOOL CPQMView::GetManualPreScanDefaultData(
    VARIANT** f_pdata
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetManualPreScanDefaultData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace"));

    // GetManualPrescanDefaultData() reads multiple fields from study file and
    //  returns its value. If atleast one field failed to read from study
    //	this function will return false. For 1st scan, we do not have FSY value
    //	hence it will fail for 1st scan everytime. Hence no need to check for
    //	return value.
    m_acquisition->GetManualPreScanDefaultData(f_pdata);
    return E_NO_ERROR;
}
//-Patni-AMT/2009Jul06/Modified/ Code Review and TMSC comments

/************************************Method Header**************************************
//Method Name:  GetCFADlgPtr
//Author:  PATNI
//Purpose: Get CFA dlg pointer
//**************************************************************************************/
CPQMCFADlg* CPQMView::GetCFADlgPtr(
)const
{
    return m_cfadlg;
}


//************************************Method Header************************************
// Method Name  : GetXOffsetLimit
// Author       : PATNI/ ARD
// Purpose      : returns the X offset limit value read from config file
//***********************************************************************************
int CPQMView::GetXOffsetLimit()const
{
    return m_xOffset;
}
//************************************Method Header************************************
// Method Name  : GetYOffsetLimit
// Author       : PATNI/ ARD
// Purpose      : returns the Y offset limit value read from config file
//***********************************************************************************
int CPQMView::GetYOffsetLimit()const
{
    return m_yOffset;
}


//Patni-RAJ/2010Jan18/Added/PH#3#/IR-86/DebugWindow/Return the object of CPQMDebugWinDlg
//**************************************************************************
//Method Name   : GetPqmDebugWinDlg
//Author        : PATNI\RAJ
//Purpose       : This function return the object of CPQMDebugWinDlg
//**************************************************************************
CPQMDebugWinDlg& CPQMView::GetPqmDebugWinDlg()
{
    return m_pqmdebugdlg;
}

//**************************************************************************
//Method Name   : GetSeletedProtocols
//Author        : PATNI\RAJ
//Purpose       : To get the list of selected protocol index.
//**************************************************************************
int* CPQMView::GetSeletedProtocols()
{

    GetPQMListCtrl().GetselectedItems(&m_selectedprotocols);
    return  m_selectedprotocols;
}

//+Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   : GetTotalProtocols
//Author        : PATNI\RAJ
//Purpose       : To get the count of total selected protocols
//**************************************************************************
int CPQMView::GetTotalProtocols()
{
    return  GetPQMListCtrl().GetSelectedCount();
}
//-Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow



//+ Patni-MSN/2010Mar17/Modified/RMC - Phase3_10
//****************************Method Header************************************
//Method Name   : SetRMCScanRunning()
//Author        : PATNI\MSN
//Purpose       :
//*****************************************************************************
void CPQMView::SetRMCScanRunning(const bool f_state)
{
    m_rmc_scan_running = f_state;

    UpdateCFARetry();

    UpdateOKBtnStatusofWirelssDlg();
}
//- Patni-MSN/2010Mar17/Modified/RMC - Phase3_10


//+Patni-HAR/2010Jan29/Modified/DeFT#MVC006364
//****************************Method Header************************************
//Method Name   : SetChildThreadStatusToIPC()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CPQMView::SetChildThreadStatusToIPC(
    int* f_pData,
    const int f_count,
    const int f_child_thread_status)const
{
    m_acquisition->SetChildThreadStatusToIPC(f_pData, f_count, f_child_thread_status);
}
//-Patni-HAR/2010Jan29/Modified/DeFT#MVC006364

//************************************Method Header************************************
// Method Name  : SetDBDtCancelFlag
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMView::SetDBDtCancelFlag(const bool f_dbdt_flag)const
{
    m_acquisition->SetDBDtCancelFlag(f_dbdt_flag);
}


void CPQMView::SetDynamicScan(
    const bool f_dynamic_scan
)
{
    m_dynamic_scan_on = f_dynamic_scan;
}

bool CPQMView::GetDynamicScan()const
{
    return m_dynamic_scan_on;
}

//+Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681
//************************************Method Header************************************
// Method Name  : SetCanAPCPause
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetCanAPCPause(
    const APC_Scan_Enum f_apc_pause_enum
)
{
    m_apc_pause_enum = f_apc_pause_enum;
}
//-Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681

//+Patni-DKH/2010Jan29/Added/PH#3#/FBI
//**************************************************************************
//Method Name   :
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
void CPQMView::SetPqmPmDelay(const int f_delay1, const int f_delay2)const
{
    m_acquisition->SetPqmPmDelay(f_delay1, f_delay2);
}

//-Patni-DKH/2010Jan29/Added/PH#3#/FBI

//+Patni-HAR/2010Jun28/Added/V1.20#MVC008392
void CPQMView::SetCFAWindowOpenedBeforeSwPage(
    const bool f_flag
)
{
    m_is_cfa_window_opened_before_swithcing_page = false;
}
//-Patni-HAR/2010Jun28/Added/V1.20#MVC008392




//************************************Method Header************************************
// Method Name  : SetSendSeqList
// Author       : PATNI/ HEMANT
// Purpose      : Set the send sequence list to the local member of
//				  CPQMIPCHelper class.
//***********************************************************************************
void CPQMView::SetSendSeqList(
)
{
    int* selected_items = NULL ;
    const int count = m_pqm_listctrl.GetselectedItems(&selected_items);

    if (count && selected_items)
        SetSendSeqListFor(selected_items, count);

    //Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(selected_items);
}


//****************************Method Header************************************
//Module         : SetAbortFlag
//Author         : PATNI/MSN
//Purpose        : Setter method to set the Abort Flag
//*****************************************************************************
void CPQMView::SetAbortFlag(
    const bool abortflag
)
{
    m_flag_abort = abortflag;

}


//****************************Method Header************************************
//Method Name   : SetIsScanning
//Author        : MSN
//Purpose       : Set if scanning or not
//*****************************************************************************
void CPQMView::SetIsScanning(const bool f_scanning)
{
    m_is_scanning = f_scanning;
}


//***************************Method Header*************************************

//Method Name    :SetPrescanHook

//Author         :PATNI/JS

//Purpose        :

//*****************************************************************************
void CPQMView::SetPrescanHook(
    const BOOL f_prescan_hook
)const
{
    //if(!IsScanning()) // Commented by Sarjeet for Defect # 861 set pre scan hook should set at every time.
    m_acquisition->SetPrescanHook(f_prescan_hook);

}


//***************************Method Header*************************************

//Method Name    :SetManualPreScanApplyBtnState

//Author         :PATNI/JS

//Purpose        :

//*****************************************************************************
void CPQMView::SetManualPreScanApplyBtnState(
    const BOOL f_button_state
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetManualPreScanApplyBtnState");

    if (NULL == m_pqmmanualprescandlg) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqmmanualprescandlg pointer is NULL"));
        return ;
    }

    m_pqmmanualprescandlg->SetManualPreScanApplyBtnState(f_button_state);
}

//************************************Method Header************************************
// Method Name  : SetPASButtonState
// Author       : PATNI/JS
// Purpose      :
//***********************************************************************************
//+ UT_JS@For pas button disable when map drive not found
void CPQMView::SetPASButtonState(
    const BOOL f_pas_button_state
)
{
    //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
    SetIconForButton(m_pass_button, f_pas_button_state ? IDI_PASS_BUTTON_IMAGE : -1);
    m_pass_button.EnableWindow(f_pas_button_state);
}
//- UT_JS@For pas button disable when map drive not found





//+Patni-PJS/2010Jan19/Added/MVC006827
void CPQMView::SetPostProcessFlag(const BOOL f_bpost_process_flag)
{
    m_bpost_process_flag = f_bpost_process_flag;
}
//-Patni-PJS/2010Jan19/Added/MVC006827

//************************************Method Header************************************
// Method Name  : SetCheckLoadStudy
// Author       : PATNI/ ARD
// Purpose      : Set the flag only if study is loading.
//***********************************************************************************
void CPQMView::SetCheckLoadStudy(const bool f_check_load_study)
{
    m_check_load_study = f_check_load_study;
}

//+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
bool CPQMView::CanPerformGUIOperation(
) const
{
    return (m_pqm_gui_processing == PQM_PROCESSING_NONE);
}
//-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
//************************************Method Header************************************
// Method Name  : GetCoilChannelMode
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
CCoilChannelMode* CPQMView::GetCoilChannelMode()
{
    return m_coil_channel_mode;
}
/************************************Method Header************************************
// Method Name  : GetCoilChannelMode
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************/
BOOL CPQMView::GetIsChannelModeApplicable()
{
    return m_use_coil_channel_mode;
}

//************************************Method Header************************************
// Method Name  : GetSystemChannels
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
int CPQMView::GetSystemChannels()
{
    return m_system_enable_channel;
}

//************************************Method Header************************************
// Method Name  : SetSystemChannels
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
void CPQMView::SetSystemChannels(const int f_system_enable_channel)
{
    m_system_enable_channel = f_system_enable_channel;
}
//************************************Method Header************************************
// Method Name  : GetLimitedChannels
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
int CPQMView::GetLimitedChannels()
{
    return m_limited_channel;
}
//************************************Method Header************************************
// Method Name  : SetConnectorState
// Author       : iGate-Patni
// Purpose      :
//***********************************************************************************
void CPQMView::SetConnectorState(
    const BOOL f_connector_state
)
{
    m_connector_state = f_connector_state;
}
//+Redmine-2187
//************************************Method Header************************************
// Method Name  : GetMulitScanDynamic_License
// Author       : iGate-Patni/Abhishek
// Purpose      : This function will retrive MultiScan Dynamic License from IPCLIB
//				  Against Redmine 2187
//***********************************************************************************
BOOL CPQMView::GetMulitScanDynamic_License()const//Redmine-2187
{
    return m_acquisition->GetMulitScanDynamic_License();
}
//-Redmine-2187

void CPQMView::SetLastAcqmanHandlderMsg(
    const pqm_msg_tag f_Msg
)
{
    m_lastacqman_handler_msg = f_Msg ;
}

pqm_msg_tag CPQMView::GetLastAcqmanHandlderMsg(
)const
{
    return m_lastacqman_handler_msg;
}


//************************************Method Header************************************
// Method Name  : CanInvokeMemory()
// Author       : PATNI/ SUDHIR
// Purpose      : checks the status of shift and ctrl key and write APPLICATION mode
//
//***********************************************************************************
//+Patni-Sudhir 2011Mar-09/Added/IR-185
bool CPQMView::CanInvokeMemory(
)
{

    bool l_can_invoke_memory = true;

    //+Patni-Hemant/2011Apr20/Modified/IR185 A-PAS re-open
    m_memory_invokation_flag |= MEMORY_PASSWORD_DIALOG_OPEN;
    SetFlagRelatedCtrlsStates();

    if ((GetAsyncKeyState(VK_SHIFT) < 0) && (GetAsyncKeyState(VK_CONTROL) < 0)) {
        //delete previous instance, if present
        DEL_PTR(m_mem_passworddlg);

        //create new instance of mem password dialog
        m_mem_passworddlg = new CPasswordDlg();

        ChangeViewOfModelessDialogs(false);

        //open mem password dialog and check return value
        l_can_invoke_memory = m_mem_passworddlg->OpenPasswordDlg() ;

        ChangeViewOfModelessDialogs(true);

        if (l_can_invoke_memory)
            UpdateMemoryInvocationMode(_T("APPLICATION"));

        DEL_PTR(m_mem_passworddlg);

    } else {
        UpdateMemoryInvocationMode(_T("NORMAL_MODE"));
    }

    m_memory_invokation_flag &= ~(MEMORY_PASSWORD_DIALOG_OPEN);
    SetFlagRelatedCtrlsStates();

    //-Patni-Hemant/2011Apr20/Modified/IR185 A-PAS re-open

    return l_can_invoke_memory;
}
//-Patni-Sudhir 2011Mar-09/Added/IR-185


//+Patni-Sudhir 2011Mar-09/Added/IR-185
//************************************Method Header************************************
//+Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
// Method Name  : UpdateMemoryInvocationMode()
// Author       : PATNI/ SUDHIR
// Purpose      : function writes text APPLICATION to shared file mapping object
//				  for invoke memory in APPLICATION mode
//***********************************************************************************
bool CPQMView::UpdateMemoryInvocationMode(
    TCHAR	szMemMode[]
)
{

    MemModeData* pMemModeData = NULL;
    TCHAR	m_MappingObjName[] = TEXT("MEMAPPMODE");

    if (m_hMapFile == NULL) {
        m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,     // use paging file
                                       NULL,					// default security
                                       PAGE_READWRITE,          // read/write access
                                       0,                       // maximum object size (high-order DWORD)
                                       sizeof(MemModeData),                // maximum object size (low-order DWORD)
                                       m_MappingObjName);                 // name of mapping object

        if (m_hMapFile == NULL) {
            return false;
        }
    }

    pMemModeData = (MemModeData*) MapViewOfFile(
                       m_hMapFile,   // handle to map object
                       FILE_MAP_ALL_ACCESS, // read/write permission
                       0, 0, 0);

    if (pMemModeData == NULL) {
        CloseHandle(m_hMapFile);
        return false;
    }

    CopyMemory((PVOID)pMemModeData->mem_mode, szMemMode, (_tcslen(szMemMode) * sizeof(TCHAR)));
    UnmapViewOfFile(pMemModeData);

    return true;
}
//-Patni-Sudhir 2011Mar-09/Added/IR-185
//-Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open

void CPQMView::InitUiButtonUtility(
)
{
    BUTTON_COLORS_t btnColor;
    m_scan_offset_button.GetButtonColors(btnColor);
    CUIButtonUtility::GetInstance()->InitUIButtonUtility(btnColor);
}


void CPQMView::InitToolTip(
)
{
    EnableToolTips(TRUE);

    m_tooltip_font.CreateFont(18,
                              0,
                              0,
                              0,
                              FW_NORMAL,
                              FALSE,
                              FALSE,
                              0,
                              ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS,
                              DEFAULT_QUALITY,
                              DEFAULT_PITCH | FF_SWISS,
                              _T("Arial")
                             );
}

void CPQMView::UpdateScanOffsetInRange(const int f_index)
{


    if (IsValidSequenceIndex(f_index)) {

        CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(f_index);
        Vector_t l_current_scan_offset, l_min_value, l_max_value;
        GetMinMaxValueForScanOffset(l_seq, GetAcqOderForGivenProtocolIndex(f_index), l_current_scan_offset, l_min_value, l_max_value);

        UpdateScanOffsetFor(l_seq, l_current_scan_offset);
    }
}


//+Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681
//************************************Method Header************************************
// Method Name  : CanAPCPause
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
APC_Scan_Enum CPQMView::CanAPCPause(
)const
{
    return m_apc_pause_enum;
}
//-Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681


//************************************Method Header************************************
// Method Name  : OpenCoilConfirmationDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
LRESULT CPQMView::OpenCoilConfirmationDialog(
    WPARAM wParam, LPARAM lParam
)
{

    if (m_cds_controller) {
        m_cds_controller->DisplayCoilSelforCOMPASSConfirmation();
    }

    return 0;
}


//************************************Method Header************************************
// Method Name  : OpenLocationDebugDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
LRESULT CPQMView::OpenLocationDebugDialog(
    WPARAM wParam, LPARAM lParam
)
{
    if (m_cds_controller) {
        m_cds_controller->OpenCompassDebug();
    }

    return 0;
}


LRESULT CPQMView::UpdateCompassCoilToCurrentProtocol(
    WPARAM wParam, LPARAM lParam
)
{
    if (GetPQMCDSControllerInterface()) {
        GetPQMCDSControllerInterface()->UpdateCompassCoilToCurrentProtocol();
    }

    return 0;
}

//+Patni-Sribanta/2011Feb07/Added/REDMINE-1026
//************************************Method Header****************************
// Method Name  : GetConnectCoilPostMessage()
// Author       : PATNI / Sribanta
// Purpose      :
//*****************************************************************************
LRESULT CPQMView::GetConnectCoilPostMessage(
    WPARAM wParam,
    LPARAM lParam
)
{
    GetConnectCoil();
    BlinkPage();
    return 0 ;
}
//-Patni-Sribanta/2011Feb07/Added/REDMINE-1026


//+Patni-Sribanta/2011Feb07/Added/REDMINE-1026
//************************************Method Header****************************
// Method Name  : OnClickCoilPasButtonPostMessage()
// Author       : PATNI / Sribanta
// Purpose      :
//*****************************************************************************
LRESULT CPQMView::OnClickCoilPasButtonPostMessage(
    WPARAM wParam,
    LPARAM lParam
)
{
    OnClickCoilPasButton();
    return 0 ;
}
//-Patni-Sribanta/2011Feb07/Added/REDMINE-1026


//************************************Method Header****************************
// Method Name  : InvokeProbeDialog()
// Author       : PATNI / SAGARIKA MOHANTY
// Purpose      :
//*****************************************************************************
LRESULT CPQMView::InvokeProbeDialogPostMessage(
    WPARAM wParam,
    LPARAM lParam
)
{

    if (m_acquisition->ProconReqUiCB((int)PQM_PROCON_INVOKE_PROBE_DIALOG, 0, 0) != E_NO_ERROR) {
        return 0;
    }

    return 0 ;
}

LRESULT CPQMView::InvokeAMB(WPARAM wParam, LPARAM lParam)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::InvokeAMB") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (g_MainView->AMBCheckIsAnyDialogOpen()) {

        ResetAMBPlanProtocols();
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not Invoke AMB Dialog. Other dialgos are open"));
        return 0;
    }

    m_amb_controller.OnAMBScanMenu();
    m_amb_controller.SetIswaitforAMBDlgOpen(false);

    return 0 ;
}


//+Patni-Sribanta/2011Feb07/Added/REDMINE-1026
//************************************Method Header****************************
// Method Name  : ProcessMRSMessagesPostMessage()
// Author       : PATNI / Sribanta
// Purpose      :
//*****************************************************************************
LRESULT CPQMView::ProcessMRSMessagesPostMessage(
    WPARAM wParam,
    LPARAM lParam
)
{
    ProcessMRSMessages((pqm_msg_tag)wParam);
    return 0 ;
}
//-Patni-Sribanta/2011Feb07/Added/REDMINE-1026



//+Patni-KSS/2010June16/ADDED/IR-155
//************************************Method Header************************************
// Method Name  : OnCancel
// Author       : PATNI/KSS
// Purpose      : Cancel the WSE Dialog
//***********************************************************************************
void CPQMView::OnCancel(
)
{
    if (m_queuedialog && !m_queuedialog->GetWaitCursor()) {

        ResetAMBPlanProtocols();
        m_queuedialoghandler->SendMessageToProcon(PQM_PROCON_CANCEL);
    }
}
//-Patni-KSS/2010June16/ADDED/IR-155

void CPQMView::OnReset(
) const
{
    if (m_queuedialog && !m_queuedialog->GetWaitCursor()) {
        m_queuedialoghandler->SendMessageToProcon(PQM_PROCON_RESET);
    }
}



//+Patni-KSS/2010June25/ADDED/IR-156_2
//************************************Method Header************************************
// Method Name  : PQMMessageMapAppend
// Author       : PATNI/KSS
// Purpose      : For Append Menu On ContextMenu
//***********************************************************************************
void CPQMView::PQMMessageMapAppend(
)const
{
    //PQMMessageMapDuplicate();
    m_pqmchildthread.PostMessageToThread(ON_DUPLICATE_APPEND, NULL, NULL);
}
//-Patni-KSS/2010June25/ADDED/IR-156_2



//*********************************Method Header***********************************
// Method Name  : HidePQM
// Author       : PATNI/ARD
// Purpose      : Hide PQM
//*********************************************************************************
BOOL CPQMView::HidePQM()const
{

    return CDPSManager::GetInstance()->SendASyncMsgToParentPage(
               DPS_MSGID_REQ_HIDEGL, PQM_APPLICATION);

}




//************************************Method Header************************************
// Method Name  : OnClickButtonAutoTransfer
// Author       : PATNI/ HEMANT
// Purpose      : On Off Auto Transfer flag in MPlus and Varfield DB for multiple selected protocol
//                On OFF is to be done according to given protocol list index
//***********************************************************************************
void CPQMView::OnClickButtonAutoTransfer(
    const int row_index
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonAutoTransfer");

    if (!m_iswait_cursor) {

        int* selected_items = NULL ;
        int size = 0 ;

        if (m_pqm_listctrl.IsRowSelected(m_pqm_listctrl.m_hWnd, row_index))	{		//if given row is selected, then do for multiple selection..
            size = m_pqm_listctrl.GetselectedItems(&selected_items);

        } else {

            //else onlt apply to given item..
            size = 1 ;
            selected_items = new int [size];
            selected_items[0] = row_index;
        }

        if (!size || !selected_items) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("selected_items pointer or size is zero"));

            return ;
        }

        if (!CanCheckUnCheckFor(selected_items, size)) {

            DEL_PTR_ARY(selected_items)
            return ;
        }

        SetWaitCursor(true);

        CSequence* seq_clicked = (CSequence*) m_pqm_listctrl.GetItemData(row_index);

        //toggle it once and set it to all ...
        const int flag_auto_transfer = !(seq_clicked->GetAutoTransferFlag()) ;

        int l_scan_index = -1;
        CString seqlid(_T(""));

        VARIANT* pdata = new VARIANT;
        CSequence* pseq = NULL ;

        for (int index = 0; index < size; index++) {

            pseq = GetSelectedSequence(selected_items[index]);
            //+ Patni-MSN/2009May15/Added/JFT - Optimization

            if (pseq) {

                // if flag is same as dominant item, then do not operate for this item..
                int current_auto_transfer_flag = pseq->GetAutoTransferFlag();

                if (current_auto_transfer_flag == flag_auto_transfer)
                    continue ;

                pseq->SetAutoTransferFlag(flag_auto_transfer);

                pdata->wReserved1 = AUTO_TRANSFER;
                pdata->intVal = flag_auto_transfer;

                //Patni-Hemant+ARD/2009Nov04/Modified/IR 85 Auto Transfer
                l_scan_index = GetAcqOderForGivenProtocolIndex(selected_items[index]);

                //+Patni-ARD/2009Oct28/Modified/IR 85 Auto Transfer
                pseq->GetCPASCOMSequence()->GetSequenceID(seqlid);

                //Patni-Hemant+ARD/2009Nov04/Modified/IR 85 Auto Transfer
                if (S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_scan_index)) {

                    //Patni-Hemant+ARD/2009Nov06/Modified/IR 85 Auto Transfer // Incorporating Offshore Comments
                    pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("APOST"), flag_auto_transfer);

                } else {

                    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                              _T("UpdateGUIControlsInVarfieldDB unable to write to DB"));
                }

                if (S_OK == m_acquisition->UpdateGUIControlsInDB(pdata, _bstr_t(seqlid))) {

                    //done during  vf updation
                    //pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("APOST"), flag_auto_transfer ? true : false);

                } else {

                    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                              _T("UpdateGUIControlsInDB unable to write to DB"));

                }

                m_pqm_listctrl.InvalidateItemsForModeOption(selected_items[index], AUTO_TRANSFER_OPTION);
            }

            ChangeContentForModelessInfoDialogs();
        }

        SetWaitCursor(false);

        DEL_PTR_ARY(selected_items)
        DEL_PTR(pdata)
    }

    //-Patni-ARD/2009Oct28/Modified/IR 85 Auto Transfer
}

//************************************Method Header************************************
// Method Name  : OnClickContrastAgentMenu
// Author       : PATNI/Mangesh
// Purpose      : This is a command handler for handling all Contrast Agent menu .
//***********************************************************************************
void CPQMView::OnClickContrastAgentMenu(
    const UINT f_contrast_menuitem_id
)
{
    OnContrastAgent(f_contrast_menuitem_id - ID_CONTRAST_POPUP_MENU);
}

//************************************Method Header**********************************
// Method Name  : OnClickScanAnatomyMenu
// Author       : PATNI/Mangesh
// Purpose      : This is a command handler for handling all Scan Anatomy menu .
//***********************************************************************************
void CPQMView::OnClickScanAnatomyMenu(
    const UINT f_scananatomy_menuitem_id
)
{
    //m_context_sub_menu.GetMenuString(f_scananatomy_menuitem_id, l_strmenu_item,MF_BYCOMMAND);

    //m_scan_anatomy_button.SetWindowText(l_strmenu_item);


    if (!m_iswait_cursor) {

        SetWaitCursor(true);
        SetScanAnatomy(f_scananatomy_menuitem_id - ID_MENU_SCAN_ANATOMY);
        SetWaitCursor(false);
    }
}

bool CPQMView::CanPlanAndDuplicateforIndex(
    const int f_seq_index
)const
{
    return m_is_wse_open ? (IsValidSequenceIndex(f_seq_index)) : false;
}


//***************************Method Header*************************************

//Method Name    :llickMenuitemManualPrescan

//Author         :PATNI/JS

//Purpose        :

//*****************************************************************************
void CPQMView::OnClickMenuitemManualPrescan(
    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    WPARAM wParam,
    LPARAM lParam
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
)
{
    if (m_pqmmanualprescandlg && m_pqmmanualprescandlg->IsWindowVisible())
        return ;

    //AfxMessageBox(_T("Manual Prescan Dialog Box"));

    if (!m_pqmmanualprescandlg)
        m_pqmmanualprescandlg = new CPQMManualPreScanDlg(m_interface_for_pqmmanualprescandlg);

    m_pqmmanualprescandlg->Create(IDD_PQMMANUALPRESCAN_DIALOG, this) ;

    m_pqmmanualprescandlg->ShowWindow(SW_SHOW) ;

}

//************************************Method Header************************************
// Method Name  : CheckpositionofReferenceimage
// Author       : PATNI/Manish
// Purpose      :
//***********************************************************************************
UINT CPQMView::CheckpositionofReferenceimage()const
{
    return ((m_pqm_g.GetPQMMode() == K_IMAGING) &&  m_pqm_g.GetLocatorMoveFlag()) ? MF_CHECKED : MF_UNCHECKED ;
}


BOOL CPQMView::PreCreateWindow(CREATESTRUCT& cs)
{

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    // WS_CLIPCHILDREN style exclude the child wondow area when you redraw parent window.
    cs.style |= WS_CLIPCHILDREN;

    if (!CFormView::PreCreateWindow(cs)) {
        return FALSE;
    }

    return TRUE;
}


/************************************Method Header**************************************

//Method Name:  PreTranslateMessage

//Author:  PATNI/SP

//Purpose: To translate window messages for accelerator table

//**************************************************************************************/
BOOL CPQMView::PreTranslateMessage(
    MSG* pMsg
)
{
    if (m_iswait_cursor) {

        if ((pMsg->message == WM_KEYUP) ||
            (pMsg->message == WM_KEYDOWN) ||
            (pMsg->message == WM_LBUTTONDOWN) ||
            (pMsg->message == WM_RBUTTONDOWN) ||

            //Added by Hemant On 2/6/2009 4:41:21 PM, defect #873
            (pMsg->message == WM_LBUTTONDBLCLK) ||
            (pMsg->message == WM_RBUTTONDBLCLK)
            //End Of addition By Hemant On2/6/2009 4:41:32 PM, defect #873

           )

            return TRUE ;
    }

    if (pMsg->wParam == VK_ESCAPE) {
        pMsg->wParam = NULL;
    }

    if (FALSE == CanProcessViewMsg(pMsg)) {
        pMsg->wParam = NULL;
    }

    // To arrest Alt + F4 Message
    if (pMsg->wParam == VK_F4 && (pMsg->wParam & VK_MENU)) {
        pMsg->wParam = NULL;
    }

    if ((pMsg->message == WM_KEYDOWN) && ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_SPACE))) {
        if ((m_utility_button.GetSafeHwnd() == pMsg->hwnd) || (m_scan_sar_button.GetSafeHwnd() == pMsg->hwnd)) {
            pMsg->message = NULL;
        }
    }


    return CFormView::PreTranslateMessage(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
// CPQMView diagnostics

#ifdef _DEBUG
void CPQMView::AssertValid() const
{
    CFormView::AssertValid();
}

void CPQMView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

// non-debug version is inline
CPQMDoc* CPQMView::GetDocument()
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPQMDoc)));
    return (CPQMDoc*)m_pDocument;
}
#endif //_DEBUG


//+Patni-ARD+AMT/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
//************************************Method Header************************************
// Method Name  : ValidateScanOffsetWithinRange
// Author       : PATNI/ ARD+AMT
// Purpose      : Validate the offset beyond the range and provide correct offset in
//                the range if it is going out of boundry at the time of display
//***********************************************************************************
void CPQMView::ValidateScanOffsetWithinRange(
    float& f_scan_offset_x,
    float& f_scan_offset_y,
    const float f_scan_offset_z
)
{
#define SET_OFF_LIMIT(so,lolimit,hilimit) if( so < lolimit) {\
        so=lolimit; \
    } else if (so > hilimit) { \
        so=hilimit; \
    }

    //+Patni-ARD/2010Jan6/Modified/DeFT# IR 100
    SET_OFF_LIMIT(f_scan_offset_x, -m_xOffset * 1.0f, m_xOffset * 1.0f);
    SET_OFF_LIMIT(f_scan_offset_y, -m_yOffset * 1.0f, m_yOffset * 1.0f);
    //-Patni-ARD/2010Jan6/Modified/DeFT# IR 100

    float   l_min_z = 0.0;
    float   l_max_z = 0.0;

    GetMinMaxZValue(l_min_z, l_max_z);
    //Patni-PJS/2010Sep16/Added/MVC008989
    //SET_OFF_LIMIT(f_scan_offset_z, l_min_z, l_max_z);

#undef SET_OFF_LIMIT
}
//-Patni-ARD+AMT/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087

//+Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel
//************************************Method Header****************************
// Method Name  : UpdateCoilSystemSetting
// Author       : PATNI/AMT
// Purpose      : Update System setting as per sm.cf file from IPC
//*****************************************************************************
bool CPQMView::UpdateCoilSystemSetting()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateCoilSystemSetting") ;

    VARIANT* l_data = new VARIANT;

    if (m_acquisition->GetCoilSysSetting(l_data) != S_OK) {


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get Coil System data from IPC"));

        DEL_PTR(l_data)

        return false;
    }

    coil_sys_info_t* l_coil_sys_setting = (coil_sys_info_t*) l_data->byref;

    if (l_coil_sys_setting != NULL) {

        m_selection_utiliy->SetCoilChannels(l_coil_sys_setting->VAP_channels,
                                            l_coil_sys_setting->channelPerVFEP);
        CString l_log_ms;
        l_log_ms.Format(_T("NumChannels=%d/VFEPChannels=%d"), l_coil_sys_setting->VAP_channels,
                        l_coil_sys_setting->channelPerVFEP);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_ms);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_coil_sys_setting NULL from IPC"));
    }

    DEL_PTR(l_data)
    return true;
}
//-Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel

//+Patni-Hemant/2009Nov16/Added/IR#90/Bottom Buttons
//************************************Method Header************************************
// Method Name  : UpdateSarAnatomyButton
// Author       : PATNI/ HEMANT
// Purpose      : This function updates the Region static text color and updates the
//                Region (SAR) image on IDC_BUTTON_ANATOMY button for first selected protocol
//***********************************************************************************
void CPQMView::UpdateSarAnatomyButton(
)
{
    const int l_first_selected_item = m_pqm_listctrl.GetFirstSelectedItem() ;

    //+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
    if (l_first_selected_item == -1) {
        m_scan_sar_button.EnableWindow(false);
        SetBitmapForAntomyButton(NULL);

    } else {
        m_scan_sar_button.EnableWindow(IsAMBInvoked() ? false : true);

        const int l_nBitmapResourceID = m_pqm_listctrl.GetBitmapResourceIDForSARRegion(l_first_selected_item);

        //+Patni-Manish/2009July01/Added/JFT#GUIRenewal-Add Color on SAR Button
        if (-1 == l_nBitmapResourceID) {
            //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
            //SetColorButton(&m_scan_sar_button, RGB(245,195,75));
            //Patni-Hemant/2009DEC02/Commented/IR#90/MVC006006
            //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
            SetBitmapForAntomyButton(NULL);

        } else {
            SetBitmapForAntomyButton(l_nBitmapResourceID);
            //Patni-Hemant/2009DEC02/Commented/IR#90/MVC006006
        }

        //-Patni-Manish/2009July01/Added/JFT#GUIRenewal-Add Color on SAR Button
    }

    //-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
}
//-Patni-Hemant/2009Nov16/Added/IR#90/Bottom Buttons

//************************************Method Header************************************
// Method Name  : ReturnCoilOfPreviousSeq
// Author       : PATNI/ Manish
// Purpose      : To retrieve the coil name of selected protocol.
//***********************************************************************************
bool CPQMView::ReturnCoilOfPreviousSeq()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ReturnCoilOfPreviousSeq");

    const int l_nLastSequenceInList = m_pqm_listctrl.GetFirstSelectedItem();

    //-1 when protocol coil has 1 and previous protocol has 0 and onstatuschanged called from OnItemChanged
    if (l_nLastSequenceInList < 0) {
        return true;
    }

    if (NULL != m_list) {

        const POSITION pos = m_list->FindIndex(l_nLastSequenceInList);

        //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
        if (NULL == pos) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Cannot find the sequence in the list"));

            return false;
        }

        //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

        CSequence* pseq = m_list->GetAt(pos);

        if (NULL != pseq) {
            return (!(GetCoilStringFor(pseq).IsEmpty())) ;
        }
    }

    return false;
}
//-Patni-Manish/2009July16/Added/JFT#GUIRenewal-To Change color of COIL,ANATOMY button

//************************************Method Header**************************************
//Method Name:  HideBigBar
//Author:  PATNI/JS
//Purpose: sending message to DPS to hide  bigbar
//**************************************************************************************/
BOOL CPQMView::HideBigBar()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::HideBigBar");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Hide Bigbar"));
    CPQMUtility::GetInstance()->SetPQMStatus(STATE_INACTIVE);


    return CDPSManager::GetInstance()->SendASyncMsgToParentPage(
               DPS_MSGID_REQ_HIDEGL, _T("BIGBAR"));


}

//****************************Method Header************************************
//Method Name   :CreateVoiceMenu
//Author        :PATNI/DJ
//Purpose       :Dynamic voice menu creation
//*****************************************************************************
bool CPQMView::CreateVoiceMenu(
    PopupMenuEx* voice_menu,
    PopupMenuEx* test_menu
)const
{
    //PATNI-SS/Added/20090828/MVC004368
    CString lang(_T("English")) ;// Default value ;

    wchar_t bufferlang[MAX_PATH] = {0};	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections

    //PATNI-SS/Modified/20090828/MVC004368
    if (::GetEnvironmentVariable(_T("MRMP_LANG"), bufferlang, MAX_PATH)) {
        lang  = bufferlang;

    } else {
        //PATNI-MP/added/20100514/IR-82
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("MRMP_LANG environment variable is not set."),
            ERROR_LOG_WARNING1,
            _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        //PATNI-MP/added/20100514/IR-82
    }

    wchar_t buffer[MAX_PATH] = {0};

    //Patni-PJS/2009Sep11/Modified/MVC4368
    ::GetEnvironmentVariable(_T("GP_DATA"), buffer, MAX_PATH);
    CString data_path  = buffer;

    voice_menu->CreatePopupMenu();

    CFileFind sDir;
    int datanum = sDir.FindFile(data_path + _T("\\*.*"));

    CString dirName = _T("");
    bool ret = false;

    while (datanum) {
        datanum = sDir.FindNextFile();

        if (sDir.IsDirectory() && (!sDir.IsDots())) {
            dirName = sDir.GetFileName();

            if (dirName.Compare(_T("Voice")) == 0) {
                ret = true;
                break;
            }
        }
    }

    voice_menu->AppendMenu(MF_POPUP, (UINT) test_menu->m_hMenu,
                           CPQMUtility::GetMultiLingualString(_T("IDS_TEST_STRING")));

    CString voice_path = data_path + _T("\\Voice\\");
    CString searchKey = voice_path + _T("*.*");



    int langnum = sDir.FindFile(searchKey);
    int lang_menu_item = START_LANGUAGEMENU_ITEM ;

    int langcnt = 0;
    CString choutput = _T("");

    while (langnum) {
        langnum = sDir.FindNextFile();

        if (sDir.IsDirectory() && !sDir.IsDots()) {

            dirName = sDir.GetFileName();

            // + Patni-Ajay/2010July21/Modified/REDMINE-478
            choutput = CPQMUtility::GetMultiLingualString(dirName);

            //Patni-PJS/2010Nov29/Added/V2.0/IR-175/Re-open
            if (!choutput.IsEmpty()) {

                //+Patni-PJS/2010Nov15/Added/V2.0/IR-175
                voice_menu->AppendMenu(MF_STRING, lang_menu_item, choutput);

                //Patni-PJS/2010Nov29/Added/V2.0/IR-175/Re-open
                if (!dirName.CompareNoCase(m_av_language) && !m_av_language.IsEmpty()) {
                    voice_menu->CheckMenuItem(lang_menu_item, MF_BYCOMMAND | MF_CHECKED);
                }

                lang_menu_item++;
                //-Patni-PJS/2010Nov15/Added/V2.0/IR-175
            }

            // + Patni-Ajay/2010July21/Modified/REDMINE-478
        }

        langcnt++;
    }

    return ret;
}


bool CPQMView::CreateTestMenu(
    PopupMenuEx* test_menu
)const
{
    if (test_menu) {

        int start_voice_menu_item = START_VOICEMENU_ITEM ;
        test_menu->CreatePopupMenu();
        test_menu->AppendMenu(MF_STRING, start_voice_menu_item++, CPQMUtility::GetMultiLingualString(_T("Breath1")));
        test_menu->AppendMenu(MF_STRING, start_voice_menu_item++, CPQMUtility::GetMultiLingualString(_T("Breath2")));
        test_menu->AppendMenu(MF_STRING, start_voice_menu_item++, CPQMUtility::GetMultiLingualString(_T("Breath3")));
        test_menu->AppendMenu(MF_STRING, start_voice_menu_item++, CPQMUtility::GetMultiLingualString(_T("Motion")));
        test_menu->AppendMenu(MF_STRING, start_voice_menu_item++, CPQMUtility::GetMultiLingualString(_T("Relax")));

        return true ;
    }

    return false ;
}

//+Patni-Ajay/2010July27/TMSC-REDMINE-443
//************************************Method Header************************************
// Method Name  : CreatePlanDuplicatePopupMenu
// Author       : PATNI/ Ajay
// Purpose      :
//***********************************************************************************
bool CPQMView::CreatePlanDuplicatePopupMenu(
    PopupMenuEx* f_plan_duplicate_menu,
    const int f_protocol_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CreatePlanDuplicatePopupMenu");

    if (!f_plan_duplicate_menu) {
        return false;

    }

    const int l_total_preset_items = m_preset_item.GetCount();

    if (0 >= l_total_preset_items) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_total_preset_items is less or equal to ZERO"));
        return true ;
    }

    const bool f_enable_menu_items = CanPlanAndDuplicateforIndex(f_protocol_index) ;

    int l_menuid = ID_PLANDUPLICATE_MIN ;

    POSITION pos = m_preset_item.GetHeadPosition();

    for (int preset_item_index = 1; preset_item_index <= l_total_preset_items ; preset_item_index ++) {
        PresetItem l_presetitem = m_preset_item.GetNext(pos) ;

        if (!l_presetitem.preset_id.IsEmpty()) {

            f_plan_duplicate_menu->InsertMenu(preset_item_index, MF_BYPOSITION, l_menuid,
                                              CPQMUtility::GetMultiLingualString(l_presetitem.preset_id));

            if (!f_enable_menu_items) {
                f_plan_duplicate_menu->EnableMenuItem(l_menuid, MF_BYCOMMAND | MF_GRAYED);

            } else if (f_enable_menu_items) {
                if (GetWaitCursor() == true) {
                    f_plan_duplicate_menu->EnableMenuItem(l_menuid, MF_BYCOMMAND | MF_GRAYED);

                } else {
                    f_plan_duplicate_menu->EnableMenuItem(l_menuid, MF_BYCOMMAND);
                }
            }

            l_menuid ++ ;
        }
    }

    return true ;

}
//-Patni-Ajay/2010July27/TMSC-REDMINE-443


//+Patni-ARD/2010Jan6/Added/DeFT# IR 100
//************************************Method Header************************************
// Method Name  : ReadConfigInfo
// Author       : PATNI/ ARD
// Purpose      : Read the configuration parameters from pqmConfig.ini and set them for future use.
//***********************************************************************************
//+Patni-ARD/2010Mar29/Modified/Ph-3# IR-100 remove ICPConfig.ini
bool CPQMView::ReadConfigInfo(SIPCConfigInfo* f_sIPCConfigInfo)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ReadConfigInfo") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
    if (f_sIPCConfigInfo && m_cds_controller) {
        m_cds_controller->InitializeCDSParametersWithDefault(f_sIPCConfigInfo);
    }

    //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement

    CString l_config_path(_T(""));

    if (!CPQMUtility::GetConfigFilePath(l_config_path)) {
        m_pqm_listctrl.SetBiasValueForProtocolNumber(f_sIPCConfigInfo->series_bias);
        return false;
    }

    CIniReader l_ini_reader(l_config_path);

    CString l_str_key_value(_T(""));

    // Menu Option
    CString l_section = _T("Menu option");

    if (l_ini_reader.SectionExists(l_section)) {

        //+Patni-AMT/2010Apr07/Added/JaFT# IR-100 Code Review
#define GET_KEY_BOOL_VAL(s) CPQMUtility::GetBoolKeyValue( l_ini_reader, s, l_section)

        //Set Utility menu staus.
        //+Patni-AMT/2010Apr07/Modified/JaFT# IR-100 code review optimization
        //+Patni-ARD/2010Mar18/Modified/DeFT# IR 100 Default Utility menu items
        //+Patni-ARD/2010Jan20/Added/DeFT#6 IR 100

        bool l_data = false;

        //+Patni-Hemant/2010Jun7/Added/MaFT/MVC008166
        if (m_pqm_g.GetPQMMode() != K_IMAGING) {
            l_data = false;

            m_pqm_g.SetShimmWbCoilFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.shimming_WB_coil = l_data;

            m_pqm_g.SetPrepWbCoilFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.prep_WB_coil = l_data;

            m_pqm_g.SetSkipRgnFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.skip_rgn = l_data;

            m_pqm_g.SetAllCoverageRgnFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.all_cov_rgn = l_data;

            m_pqm_g.SetAutoPlanFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.auto_plan = l_data;

            m_pqm_g.SetPlanScanFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.plan_scan = l_data;

            m_pqm_g.SetSpeedermapAutoVoiceFlag(l_data);
            //f_sIPCConfigInfo->av_SPDR_Map = l_data ;
            f_sIPCConfigInfo->s_DefaultUtilityMenu.speedermap_auto_voice = l_data;

            m_pqm_g.SetMultiCovCautionFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.multi_cov_caution = l_data;

        } else {

            l_data = GET_KEY_BOOL_VAL(_T("shimmingWBcoil"));
            m_pqm_g.SetShimmWbCoilFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.shimming_WB_coil = l_data;

            l_data = GET_KEY_BOOL_VAL(_T("prepWBcoil"));
            m_pqm_g.SetPrepWbCoilFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.prep_WB_coil = l_data;


            l_data = GET_KEY_BOOL_VAL(_T("skipRGN"));
            m_pqm_g.SetSkipRgnFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.skip_rgn = l_data;

            l_data = GET_KEY_BOOL_VAL(_T("allRGN"));
            m_pqm_g.SetAllCoverageRgnFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.all_cov_rgn = l_data;

            l_data = GET_KEY_BOOL_VAL(_T("autoPlan"));
            m_pqm_g.SetAutoPlanFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.auto_plan = l_data;

            l_data = GET_KEY_BOOL_VAL(_T("planAndScan"));
            m_pqm_g.SetPlanScanFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.plan_scan = l_data;

            l_data = GET_KEY_BOOL_VAL(_T("avSPDRMap"));
            m_pqm_g.SetSpeedermapAutoVoiceFlag(l_data);
            //f_sIPCConfigInfo->av_SPDR_Map = l_data ;
            f_sIPCConfigInfo->s_DefaultUtilityMenu.speedermap_auto_voice = l_data;

            l_data = GET_KEY_BOOL_VAL(_T("coverageMsg"));
            m_pqm_g.SetMultiCovCautionFlag(l_data);
            f_sIPCConfigInfo->s_DefaultUtilityMenu.multi_cov_caution = l_data;

        }

        //-Patni-Hemant/2010Jun7/Modified/MaFT/MVC008166

        l_data = GET_KEY_BOOL_VAL(_T("SetAllCoilSAR"));
        m_pqm_g.SetBatchSettingFlag(l_data);
        f_sIPCConfigInfo->s_DefaultUtilityMenu.batch_setting = l_data;

        // + Patni-Sribanta/2010Jun09/Modified/MVC8032
        /*
        l_data = GET_KEY_BOOL_VAL( _T("coverageMsg") );
        m_pqm_g.SetMultiCovCautionFlag( l_data );
        f_sIPCConfigInfo->s_DefaultUtilityMenu.multi_cov_caution = l_data;
        */
        // - Patni-Sribanta/2010Jun09/Modified/MVC8032
        m_pqm_g.SetUtilityMenuDefaultItems(f_sIPCConfigInfo);
        //-Patni-ARD/2010Mar18/Modified/DeFT# IR 100 Default Utility menu items

        //+Patni-Hemant/2010Jun7/Commented/MaFT/MVC008166
        //        l_data = GET_KEY_BOOL_VAL( _T("avSPDRMap") );
        //        m_pqm_g.SetSpeedermapAutoVoiceFlag(l_data);
        //        f_sIPCConfigInfo->av_SPDR_Map = l_data ;
        //-Patni-Hemant/2010Jun7/Commented/MaFT/MVC008166

        f_sIPCConfigInfo->offset_Vector_Reset = GET_KEY_BOOL_VAL(_T("OffsetVecReset"));

        // Set contrastAgent
        l_str_key_value = l_ini_reader.GetKeyValue(_T("contrastAgent"), l_section);
        l_str_key_value.TrimLeft();
        l_str_key_value.TrimRight();

        if (!l_str_key_value.IsEmpty()) {

            wchar_t l_sep[] = _T(",");
            wchar_t* l_tok = NULL;
            f_sIPCConfigInfo->contrast_agent_list.RemoveAll();
            const int l_len = l_str_key_value.GetLength() + 1;
            wchar_t* l_wdata = new wchar_t[l_len];

            if (NULL != l_wdata) {

                memset(l_wdata, 0, sizeof(wchar_t) * l_len);
                wcscpy(l_wdata, l_str_key_value);
                l_tok = wcstok(l_wdata, l_sep);

                CString l_str_contrast_name = _T("");

                while (l_tok != NULL) {
                    l_str_contrast_name = l_tok ;
                    f_sIPCConfigInfo->contrast_agent_list.Add(l_str_contrast_name);
                    l_tok = wcstok(NULL, l_sep);
                }

                DEL_PTR_ARY(l_wdata)

            } else {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("error:Out of memory. Cannot Read contrast agent from config file"));
            }

            l_wdata = NULL;
        }

        //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
        f_sIPCConfigInfo->m_tool_menu = GET_KEY_BOOL_VAL(_T("ToolMenu"));

        if (m_sft_menuhandler) {
            m_sft_menuhandler->SetToolMenuAvaibility(f_sIPCConfigInfo->m_tool_menu);
        }

        //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

    } else {

        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Menu option section does not exist in PQMConfig.ini"),
                ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    l_section = _T("Auto Voice");

    if (l_ini_reader.SectionExists(l_section)) {

#define SetValidInRange( min, max, input, output, def ) if( input < min || input > max ) {output = def;} \
    else {output = input;}

        // Set avSPDRMapDelay
        l_str_key_value = l_ini_reader.GetKeyValue(_T("avSPDRMapDelay"), l_section);
        SetValidInRange(0, 20000, _wtoi(l_str_key_value), f_sIPCConfigInfo->av_SPDR_Map_Delay, 0);

        // Set avVolume
        l_str_key_value = l_ini_reader.GetKeyValue(_T("avVolume"), l_section);
        SetValidInRange(0, 100, _wtoi(l_str_key_value), f_sIPCConfigInfo->av_Volume, 80);

        // Set avInterval
        l_str_key_value = l_ini_reader.GetKeyValue(_T("avInterval"), l_section);
        SetValidInRange(0, 15000, _wtoi(l_str_key_value), f_sIPCConfigInfo->av_Interval, 3000);

        // Set avDynamicDelay
        l_str_key_value = l_ini_reader.GetKeyValue(_T("avDynamicDelay"), l_section);
        SetValidInRange(0, 3000, _wtoi(l_str_key_value), f_sIPCConfigInfo->av_Dynamic_Delay, 500);

        // Set avTP1Delay
        l_str_key_value = l_ini_reader.GetKeyValue(_T("avTP1Delay"), l_section);
        SetValidInRange(0, 3000, _wtoi(l_str_key_value), f_sIPCConfigInfo->av_TP1_Delay, 500);

        //+Patni-PJS/2010Nov15/Added/V2.0/IR-175
        // Set avLanguage
        l_str_key_value = l_ini_reader.GetKeyValue(_T("language"), l_section);
        l_str_key_value.TrimLeft();
        l_str_key_value.TrimRight();
        InitializeAvLanguage(l_str_key_value, f_sIPCConfigInfo);
        //-Patni-PJS/2010Nov15/Added/V2.0/IR-175

#undef SetValidInRange

    } else {

        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Auto Voice section does not exist in PQMConfig.ini"),
                ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    l_section = _T("Scan Offset");

    if (l_ini_reader.SectionExists(l_section)) {

#define SetValidBoundary( min, max, input, output ) if( input < min ) {output = min;} \
    else if( input > max ) {output = max;} \
    else {output = input;}

        // Set xOffset
        l_str_key_value = l_ini_reader.GetKeyValue(_T("xOffset"), l_section);

        //+Patni-ARD/2010Apr08/Modified/JaFT# IR-100
        if (!l_str_key_value.IsEmpty()) {
            SetValidBoundary(10, 25, _wtoi(l_str_key_value), m_xOffset);
        }

        //Patni-ARD/2010Mar24/Added/Ph-3# Debug Info
        f_sIPCConfigInfo->limit_offset_x = m_xOffset;

        // Set yOffset
        l_str_key_value = l_ini_reader.GetKeyValue(_T("yOffset"), l_section);

        if (!l_str_key_value.IsEmpty()) {
            SetValidBoundary(10, 25, _wtoi(l_str_key_value), m_yOffset);
        }

        //-Patni-ARD/2010Apr08/Modified/JaFT# IR-100
        //Patni-ARD/2010Mar24/Added/Ph-3# Debug Info
        f_sIPCConfigInfo->limit_offset_y = m_yOffset;

#undef SetValidBoundary

    } else {
        //section does not exist
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Scan Offset section does not exist in PQMConfig.ini"),
                ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    l_section = _T("IPC");

    if (l_ini_reader.SectionExists(l_section)) {

        // Set hiSpeedCouch
        f_sIPCConfigInfo->hi_Speed_couch = GET_KEY_BOOL_VAL(_T("HiSpeedCouch"));

        // Set referenceImage
        l_str_key_value = l_ini_reader.GetKeyValue(_T("referenceImage"), l_section);

        //+Patni-ARD/2010Apr09/Modified/JaFT# IR-100
        if (!l_str_key_value.IsEmpty()) {
            f_sIPCConfigInfo->reference_Image = _wtoi(l_str_key_value);
        }

        //-Patni-ARD/2010Apr09/Modified/JaFT# IR-100

        l_str_key_value = l_ini_reader.GetKeyValue(_T("ReconAfterRefView"), l_section);
        f_sIPCConfigInfo->recon_after_ref_view = true;

        if (!l_str_key_value.IsEmpty()) {
            f_sIPCConfigInfo->recon_after_ref_view = (_wtoi(l_str_key_value) > 0);
        }

    } else {
        //section does not exist
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("IPC section does not exist in PQMConfig.ini"),
                ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    //+Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
    if (l_ini_reader.SectionExists(_T("JETSCAN"))) {
        l_str_key_value = l_ini_reader.GetKeyValue(_T("JETChannel_V"), _T("JETSCAN"));

        //+Patni-ARD/2010Apr09/Modified/JaFT# IR-100
        if (!l_str_key_value.IsEmpty()) {
            f_sIPCConfigInfo->jet_channel_v = _ttoi(l_str_key_value);

        } else {

            CString l_err_str(_T(""));
            l_err_str.LoadString(IDS_JETCHANNEL_V_ILLEGAL_VALUE_FOUND);

            CPQMLogMgr::GetInstance()->WriteEventLog(
                IDS_JETCHANNEL_V_ILLEGAL_VALUE_FOUND,
                ERROR_LOG_CRUCIAL1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        }

        //-Patni-ARD/2010Apr09/Modified/JaFT# IR-100

    } else {
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("JETSCAN section does not exist in PQMConfig.ini"),
            ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    l_section = _T("TIMER");

    if (l_ini_reader.SectionExists(l_section)) {
        l_str_key_value = l_ini_reader.GetKeyValue(_T("timer_pager_area"), _T("TIMER"));

        if (!l_str_key_value.IsEmpty()) {
            l_str_key_value.TrimLeft();
            l_str_key_value.TrimRight();

            f_sIPCConfigInfo->timer_pager_area = _ttoi(l_str_key_value);
            m_timer_pager_area = f_sIPCConfigInfo->timer_pager_area;

        } else {
            f_sIPCConfigInfo->timer_pager_area = m_timer_pager_area = PAGER_TIMER_INTERVAL;

            CPQMLogMgr::GetInstance()->WriteEventLog(
                _T("Pager Timer value is not present in TIMER section of PQMConfig.ini"),
                ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__);
        }

    } else {

        f_sIPCConfigInfo->timer_pager_area = m_timer_pager_area = PAGER_TIMER_INTERVAL;

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("TIMER section does not exist in PQMConfig.ini"),
            ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__);

    }

    if (l_ini_reader.SectionExists(_T("STUDY"))) {
        l_str_key_value = l_ini_reader.GetKeyValue(_T("Series Bias"), _T("STUDY"));

        if (!l_str_key_value.IsEmpty()) {
            f_sIPCConfigInfo->series_bias = _ttoi(l_str_key_value);
        }

    } else {

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("STUDY section does not exist in PQMConfig.ini"),
            ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    m_pqm_listctrl.SetBiasValueForProtocolNumber(f_sIPCConfigInfo->series_bias);

    l_section = _T("COIL");

    if (l_ini_reader.SectionExists(_T("COIL"))) {
        //+Patni-Ravindra Acharya/2010May24/Modified/IR-147
        l_str_key_value = l_ini_reader.GetKeyValue(_T("AxisLocator"), l_section);

        //-Patni-Ravindra Acharya/2010May24/Modified/IR-147
        if (l_str_key_value.CompareNoCase(_T("Right")) == 0) {
            m_axis_locator_position = 1;

        } else {
            m_axis_locator_position = 0;
        }

#define SetValidInRange1( min, max, input, output, def ) if( input <= min || input >= max ) {output = def;} \
    else {output = input;}
#define SetValidInRange2( min, max, input, output, def ) if( input <= min || input > max ) {output = def;} \
    else {output = input;}
#define SetValidInRange3( min, max, input, output, def ) if( input < min || input > max ) {output = def;} \
    else {output = input;}

        // Set gui_sec_size_start
        l_str_key_value = l_ini_reader.GetKeyValue(_T("gui_sec_size_start"), l_section);
        SetValidInRange1(0.f, 15.f, _tcstod(l_str_key_value, 0), f_sIPCConfigInfo->m_coilselconfigparams.gui_sec_size_start, 1.f);

        // Set gui_sec_size_end
        l_str_key_value = l_ini_reader.GetKeyValue(_T("gui_sec_size_end"), l_section);
        SetValidInRange2(f_sIPCConfigInfo->m_coilselconfigparams.gui_sec_size_start, 15.f, _tcstod(l_str_key_value, 0), f_sIPCConfigInfo->m_coilselconfigparams.gui_sec_size_end, 2.f);

        // Set gui_sec_step
        l_str_key_value = l_ini_reader.GetKeyValue(_T("gui_sec_step"), l_section);
        SetValidInRange2(0.f, 1.f, _tcstod(l_str_key_value, 0), f_sIPCConfigInfo->m_coilselconfigparams.gui_sec_step, 0.2f);

        // Set couch_select_range
        l_str_key_value = l_ini_reader.GetKeyValue(_T("couch_select_range"), l_section);
        SetValidInRange3(2.f, 15.f, _tcstod(l_str_key_value, 0), f_sIPCConfigInfo->m_coilselconfigparams.couch_select_range, 3.8f);

    } else {
        m_axis_locator_position = 0;

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("COIL section does not exist in PQMConfig.ini"),
            ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

#undef SetValidInRange3
#undef SetValidInRange2
#undef SetValidInRange1

    if (l_ini_reader.SectionExists(_T("SOCKET"))) {

        l_str_key_value = l_ini_reader.GetKeyValue(_T("Acqman IP"), _T("SOCKET"));

        //Patni-ARD/2010Apr09/Modified/JaFT# IR-100
        if (!l_str_key_value.IsEmpty())
            f_sIPCConfigInfo->acqman_ip = l_str_key_value;

        l_str_key_value = l_ini_reader.GetKeyValue(_T("SarManager IP"), _T("SOCKET"));


        if (!l_str_key_value.IsEmpty())
            f_sIPCConfigInfo->sarmanager_ip = l_str_key_value;

    } else {

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("SOCKET section does not exist in PQMConfig.ini"),
            ERROR_LOG_WARNING1, _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

    }

    //-Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
#undef GET_KEY_BOOL_VAL

    //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
    //Now reading CDS related parameters

    //+ MEITEC/2010Aug10/Modified/REDMINE-144
    //+Patni-Hemant/2010July26/Modified/CDS_Defect_1
    //Read In both pages
    //if ((m_pqm_g.GetPQMMode() == K_IMAGING) && m_cds_controller) {

    m_wirelessgating_cntlr.ReadWirelessParamfromConfig(&l_ini_reader);

    return m_cds_controller->ReadCDSParams(&l_ini_reader, f_sIPCConfigInfo);
    //}
    //-Patni-Hemant/2010July26/Modified/CDS_Defect_1


    //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
    // return true;
    //-Patni-ARD/2010Mar29/Modified/Ph-3# IR-100 remove ICPConfig.ini
    //- MEITEC/2010Aug10/Modified/REDMINE-144
}



//+Patni-HEMANT+PJS/2010Jun11/Modified/MVC008236
//****************************Method Header************************************
//Method Name   : OnCmdMsg()
//Author        : PATNI\PJS
//Purpose       : This function use to handle the update of command user
//				  interface
//*****************************************************************************
BOOL CPQMView::OnCmdMsg(
    const UINT nID,
    const int nCode,
    void* pExtra,
    AFX_CMDHANDLERINFO* pHandlerInfo
)
{
    if (m_iswait_cursor) {
        return TRUE;
    }

    return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
//-Patni-HEMANT+PJS/2010Jun11/Modified/MVC008236


//+Patni-MSN-MN/2009Nov04/Code Review/Changes as per vantage code
//************************************Method Header****************************
// Method Name  : CheckPqmPqIsGlocOk
// Author       : PATNI/MN-MSN
// Purpose      : Call PqmPqIsGlocOk() to check the enable/disable of locator button
//*****************************************************************************
void CPQMView::CheckPqmPqIsGlocOk()const
{
    m_acquisition->GetPqmPqIsGlocOk();
}
//-Patni-MSN-MN/2009May04/Code Review/Changes as per vantage code


//************************************Method Header****************************
// Method Name  : SendStatusReqToAcqman
// Author       : PATNI
// Purpose      : Send FW_StatusQuery msg to Acqman GL
//*****************************************************************************
void CPQMView::SendStatusReqToAcqman()const
{

    CDPSManager::GetInstance()->SendASyncMsg(_T("AcqManGL"),
            _T("FW_StatusQuery"), _T(""));
}

// + Himanshu SM2

//************************************Method Header****************************
// Method Name  : CreateCoilListsForSelectionCoil()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
bool CPQMView::CreateCoilListsForSelectionCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CreateCoilListsForSelectionCoil");

    DllT*						l_dll_tmp = NULL;
    CoilSelectionCoil_t*		 l_coil_select = NULL;
    CoilSelectionCoilButton_t*	l_btn = NULL, *l_sub_btn = NULL;
    BOOL						l_insert = FALSE;
    int 						i = 0;	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
    UINT						l_uid = 1u;//Patni-DKH/2009Aug24/Modified/cpp test corrections

    for (DllT* l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_select = (CoilSelectionCoil_t*) l_dll->item;

        if (!l_coil_select->enable) {
            continue;
        }

        l_btn = new CoilSelectionCoilButton_t;
        //if(l_btn == NULL){
        //	DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
        //	CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);
        //	return;
        //}
        memset(l_btn, 0, sizeof(CoilSelectionCoilButton_t));

        l_btn->coil = l_coil_select;
        l_btn->menu_id = l_uid++;
        l_insert = FALSE;

        // Search for G port
        if (l_coil_select->port  != CDB_PORT_G && l_coil_select->port
            != CDB_PORT_D) {

            for (l_dll_tmp = m_coil_trestle_head; l_dll_tmp != NULL;
                 l_dll_tmp = l_dll_tmp->next) {

                l_sub_btn = (CoilSelectionCoilButton_t*)l_dll_tmp->item;

                if (IS_COIL_SAME(l_btn->coil->coil_id , l_sub_btn->coil->coil_id)) {

                    int i = 0;

                    l_sub_btn->subMenu = dllAppendNode(l_sub_btn->subMenu,
                                                       l_btn, 0, NULL);

                    DLL_FIND_HEAD(l_sub_btn->subMenu);
                    l_insert = TRUE;

                    if (l_sub_btn->label == NULL) {
                        l_sub_btn->label = strdup(l_sub_btn->coil->label);

                        if (l_sub_btn->label == NULL) {

                            //CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);

                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                      _T("l_sub_btn->label pointer is NULL"));
                            return false;
                        }
                    }

                    for (i = 0; i < (int)strlen(l_sub_btn->label); i++) {

                        if (l_sub_btn->label[i] != l_btn->coil->label[i]) {

                            l_sub_btn->label[i] = '\0';
                            break;
                        }
                    }

                    break;
                }
            }

            if (!l_insert) {
                dllAppendNode(m_coil_trestle_head, l_btn, 0, &m_coil_trestle_head);
                DLL_FIND_HEAD(m_coil_trestle_head);
            }

        } else if (l_coil_select->port  == CDB_PORT_G) {
            for (i = 0; i < 7/*MAX_COUCH_PORT*/; i++) {

                CoilSelectionCoilButton_t*	l_slave = NULL;

                l_slave = new CoilSelectionCoilButton_t;

                if (l_slave == NULL) {

                    //DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
                    //CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);
                    return false;
                }

                memset(l_slave, 0, sizeof(CoilSelectionCoilButton_t));
                memcpy(l_slave, l_btn, sizeof(CoilSelectionCoilButton_t));

                //+Patni-AMT/2009Nov09/Modified/DeFT/MCM0229-00154
                for (l_dll_tmp = m_coil_couch_head[i]; l_dll_tmp != NULL;
                     l_dll_tmp = l_dll_tmp->next) {
                    l_sub_btn = (CoilSelectionCoilButton_t*)l_dll_tmp->item;

                    if (IS_COIL_SAME(l_btn->coil->coil_id, l_sub_btn->coil->coil_id)) {
                        int j = 0;
                        l_sub_btn->subMenu = dllAppendNode(l_sub_btn->subMenu, l_slave,
                                                           0, NULL);
                        DLL_FIND_HEAD(l_sub_btn->subMenu);
                        l_insert = TRUE;

                        if (l_sub_btn->label == NULL) {
                            l_sub_btn->label = strdup(l_sub_btn->coil->label);

                            //if (btn2_p->label == NULL) {
                            //   DB_PRINTF("",("%s:%d malloc error\n",
                            //        __FILE__,__LINE__));
                            //    CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);
                            //} //if(btn2_p->label == NULL){
                        } //if ( l_sub_btn->label == NULL ){

                        for (j = 0; j < (int) strlen(l_sub_btn->label); j++) {
                            if (l_sub_btn->label[j] != l_btn->coil->label[j]) {
                                l_sub_btn->label[j] = '\0';
                                break;
                            }
                        }

                        break;
                    } // if ( IS_SAME_COIL
                } // for

                if (!l_insert) {
                    dllAppendNode(m_coil_couch_head[i], l_slave, 0, &m_coil_couch_head[i]);
                    DLL_FIND_HEAD(m_coil_couch_head[i]);
                }

                //+Patni-AMT/2009Nov09/Modified/DeFT/MCM0229-00154
            }

        } else if (l_coil_select->port == CDB_PORT_D) {

            if (l_coil_select->channel == CDB_CHANNEL_A) {

                i = 0;

            } else if (l_coil_select->channel == CDB_CHANNEL_B) {

                i = 1;

            } else {
                i = 2;
            }

            if (i != 2) {

                CoilSelectionCoilButton_t*	l_slave = NULL;
                l_slave = new CoilSelectionCoilButton_t;

                if (l_slave == NULL) {

                    //DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
                    //CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);
                    return false;
                }

                memset(l_slave, 0, sizeof(CoilSelectionCoilButton_t));
                memcpy(l_slave , l_btn, sizeof(CoilSelectionCoilButton_t));

                dllAppendNode(m_coil_liner_head[i], l_slave , 0, &m_coil_liner_head[i]);
                DLL_FIND_HEAD(m_coil_liner_head[i]);
            }
        }

    }

    // Call function to create a menu for 3 different ports.


    for (int l_coilindex = 0; l_coilindex < 7; l_coilindex++) {
        CSCoilDisplay(m_coil_couch_head[l_coilindex], l_coilindex);
    }

    for (; l_coilindex < 9; l_coilindex++) {
        CSCoilDisplay(m_coil_liner_head[l_coilindex - MAX_COUCH_PORT], l_coilindex);
    }

    CSCoilDisplay(m_coil_trestle_head, l_coilindex);

    // - Himanshu SM2
    return true;
}


void CPQMView::CSCoilDisplay(
    DllT* f_head,
    const int f_no
)const
{
    DllT*                        l_sub_dll  = NULL;
    CoilSelectionCoilButton_t*   l_btn      = NULL;
    CoilSelectionCoilButton_t*   l_sub_btn  = NULL;

    for (DllT* l_dll = f_head; l_dll != NULL; l_dll = l_dll->next) {
        l_btn = (CoilSelectionCoilButton_t*)l_dll->item;
        l_btn->no = f_no;

        //Sub menu processing
        if (l_btn->subMenu != NULL) {

            for (l_sub_dll = l_btn->subMenu; l_sub_dll != NULL;
                 l_sub_dll = l_sub_dll->next) {

                l_sub_btn = (CoilSelectionCoilButton_t*)l_sub_dll->item;
                l_sub_btn->no = f_no;
            }
        }
    }
}


//************************************Method Header**********************************
// Method Name  : InitializeAvLanguage
// Author       : PATNI/ PJS
// Purpose      : Initialize auto voice language member variable.
//***********************************************************************************
void CPQMView::InitializeAvLanguage(
    const CString& f_av_lang, const SIPCConfigInfo* f_sIPCConfigInfo
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::InitializeAvLanguage");

    if (f_av_lang.IsEmpty()) {
        //Trace Log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Auto voice language is empty in INI file"));

        //f_sIPCConfigInfo->av_Language = f_av_lang;

        m_av_language = f_av_lang;
        return;
    }

    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("GP_DATA"), buffer, MAX_PATH);

    CString l_voice_path = buffer;
    l_voice_path = l_voice_path + _T("\\Voice\\") + f_av_lang;

    CFileFind l_sDir;

    if (l_sDir.FindFile(l_voice_path)) {

        //f_sIPCConfigInfo->av_Language = f_av_lang;
        m_av_language = f_av_lang;
    }
}
//-Patni-PJS/2010Nov15/Added/V2.0/IR-175


//************************************Method Header************************************
// Method Name  : CanChangeScanAnatomy
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMView::CanChangeScanAnatomy(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CanChangeScanAnatomy");

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    int* selected_items = NULL;
    bool iswse_eidted_protocol_selected = false;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected, NULL, &iswse_eidted_protocol_selected);

    //+ Patni-PJS/2009Dec31/Added+Modified/MVC006364
    if (!selected_items) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("selected_items pointer is NULL"));
        return -1;

    }

    //- Patni-PJS/2009Dec31/Added+Modified/MVC006364

    //Modified by Hemant On 11/12/2008 4:37:02 PM
    //if(m_is_wse_open || m_iswait_cursor || iscurrent_selected || isfail_selected || isdone_selected || iswse_eidted_protocol_selected) {
    if (m_is_wse_open || iscurrent_selected || isfail_selected || isdone_selected || iswse_eidted_protocol_selected) {
        DEL_PTR_ARY(selected_items)
        return -1 ;
    }

    bool sar_valid = false ;
    int sar_anatomy = -1;

    if (count) {

        sar_valid = true ;

        CSequence* current_seq = NULL ;

        for (int index = 0; index < count; index++) {

            current_seq = (CSequence*) m_pqm_listctrl.GetItemData(selected_items[index]);

            if (!current_seq) {
                sar_valid = false ;
                break ;
            }

            const CPqmPrMode& prmode = current_seq->GetPrMode() ;

            if (sar_anatomy == -1) {

                sar_anatomy = prmode.m_anatomy ;

            } else {

                //anatomy is different from prev item..so break..
                if (prmode.m_anatomy != sar_anatomy) {
                    sar_valid = false ;
                    break ;
                }
            }
        }
    }

    DEL_PTR_ARY(selected_items)

    //Modified by Hemant On 12/9/2008 1:24:18 PM
    //if sar anatomy is not Ok then also disable..
    //return (sar_valid ? sar_anatomy : -1);
    return ((sar_valid && ((sar_anatomy >= SAR_REGION(ABDOMEN)) && (sar_anatomy < SAR_REGIONS))) ? sar_anatomy : -1);
}

//************************************Method Header************************************
// Method Name  : InitializePQMListCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::InitializePQMListCtrl(
)
{

    CRect rect;
    CWnd* wnd = GetDlgItem(IDC_PQM_SEQ_LISTCTRL);
    wnd->GetWindowRect(&rect);
    ScreenToClient(&rect);
    wnd->DestroyWindow();

    m_pqm_listctrl.CreateListCtrl(IDC_PQM_SEQ_LISTCTRL, rect, this);
    m_pqm_listctrl.SetSequenceListPtr(m_list);

    //This line was called from where this function is being called now..
    m_pqm_listctrl.Init();

    return TRUE ;
}


//************************************Method Header************************************
// Method Name  : ApplyWarningFont
// Author       : PATNI/ HEMANT
// Purpose      : Applies the default warning text color to given button if second
//                argument is not given
//                otherwise warning text color will be given second parameter
//***********************************************************************************
void CPQMView::ApplyWarningFont(
    ButtonEx* f_button,
    const COLORREF f_warning_text_color /*= g_no_data_warning_color*/
)const
{
    if (NULL != f_button) {
        BUTTON_COLORS_t l_button_colors;
        f_button->GetButtonColors(l_button_colors);

        l_button_colors.crPrimaryCaptionNormal = f_warning_text_color;
        f_button->SetButtonColors(l_button_colors);
    }
}


//************************************Method Header************************************
// Method Name  : CanChangeSar
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanChangeSar(
)
{
    if (!CanPerformEditingForVFFile())
        return false;

    int* selected_items = NULL ;

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    bool iswse_eidted_protocol_selected = false;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected, NULL, &iswse_eidted_protocol_selected);

    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(selected_items);
    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

    if (!count || iscurrent_selected || isdone_selected || isfail_selected || iswse_eidted_protocol_selected)
        return false ;

    return true ;
}

//************************************Method Header************************************
// Method Name  : EnableDisableScanAnatomy
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::EnableDisableScanAnatomy(
)
{
    const BOOL enable_scan_anatomy = (!IsAMBInvoked() && (CanChangeScanAnatomy() != -1));

    if (m_scan_anatomy_button.IsWindowEnabled() != enable_scan_anatomy) {

        m_scan_anatomy_button.EnableWindow(enable_scan_anatomy);

        //Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons
        //SetBitmapForAntomyButton(enable_scan_anatomy); 	//Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
    }

    const int sel_seq_idx = m_pqm_listctrl.GetFirstSelectedItem();

    if (!IsValidSequenceIndex(sel_seq_idx)) {

        m_scan_anatomy_button.SetWindowText(_T(""));
        return ;
    }

    CSequence* current_seq = (CSequence*) m_pqm_listctrl.GetItemData(sel_seq_idx);

    CString l_scananatomy_str = current_seq->GetPrMode().m_str_scan_anatomy.m_scan_anatomy_name;
    const CString l_lat_shortname = current_seq->GetLateralityShortStr() ;

    if (!l_lat_shortname.IsEmpty()) {
        l_scananatomy_str = l_scananatomy_str + _T(":") + l_lat_shortname ;
    }

    m_scan_anatomy_button.SetWindowText(l_scananatomy_str);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::EnableDisableScanEditOrPlanButtons()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::EnableDisableScanEditOrPlanButtons");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int* selected_items = NULL;

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    bool iswse_eidted_protocol_selected = false;

    int count = 0;

    //+Patni-Manish/2009Jun12/Added/JFT#IR-23/Enable LOCATOR on envoking WSE.
    //Patni-Hemant/2011Mar30/Modified/REDMINE-1575
    try {
        if (GetMemoryAliveFlag() || !m_is_procon_connected || IsAMBInvoked()) {
            if (m_pqm_g.GetPQMMode() == K_IMAGING) {

                m_locator_button.EnableWindow(false);
            }

            //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
            SetIconForButton(m_scanedit_button, -1);
            m_scanedit_button.EnableWindow(false);
            return ;
        }

        //+ Patni-PJS/2009Jun19/Modify/IDS#624,625
        if (m_is_wse_open) {

            //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
            SetIconForButton(m_scanedit_button, -1);
            m_scanedit_button.EnableWindow(false);

            count = m_pqm_listctrl.GetselectedItems(&selected_items, NULL, NULL, NULL, NULL, NULL, &iswse_eidted_protocol_selected);

            if ((count > 1) && m_bLocatorAlreadyEnable)
                m_is_locator_open = true;

            if ((count == 1) && (1 == iswse_eidted_protocol_selected) && IsOneScanDone() && m_bEnableLocatorOnScanEdit && selected_items) {

                if (m_bLocatorAlreadyEnable) {
                    m_locator_button.EnableWindow(false);

                    //Patni-ARD/2010Jan29/Added/DeFT# MVC4446
                    m_is_locator_open = true;

                } else {

                    //+Patni-Manishkumar/2009Aug12/Modified/MVC002704
                    if (true == m_bEnableLocatorOnScanEdit) {
                        m_locator_button.EnableWindow(true);
                    }

                    //-Patni-Manishkumar/2009Aug12/Modified/MVC002704
                }

            } else {

                if (m_pqm_g.GetPQMMode() == K_IMAGING) {
                    m_locator_button.EnableWindow(false);
                }
            }

            DEL_PTR_ARY(selected_items);

            return;
        }

        //-Patni-PJS/2009Jun19/Modify/IDS#624,625

        //-Patni-Manish/2009Jun12/Added/JFT#IR-23/Enable LOCATOR on envoking WSE.

        count = m_pqm_listctrl.GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected);

        if (!count || !selected_items) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("selected_items count is 0"));

            //process for aquire page only..
            if ((m_pqm_g.GetPQMMode() == K_IMAGING))	{

                //for locator button, atleast one scan should be done..
                m_locator_button.EnableWindow(false);
            }

            //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
            SetIconForButton(m_scanedit_button, -1);
            m_scanedit_button.EnableWindow(false);

        } else {

            //regarding scan edit and scan plan...
            BOOL canenable_scan_edit_plan = (!(iscurrent_selected || isdone_selected || isfail_selected));

            if ((m_pqm_g.GetPQMMode() == K_IMAGING) && !GetSARControlStatusForScanEditRelatedButtons()) {
                canenable_scan_edit_plan = false;
            }

            if (m_scanedit_button.IsWindowEnabled() != canenable_scan_edit_plan) {

                //+Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
                SetIconForButton(m_scanedit_button, canenable_scan_edit_plan ? IDI_SCANEDIT_BUTTON_IMAGE : -1);
                m_scanedit_button.EnableWindow(canenable_scan_edit_plan);
            }

            //+Patni-Hemant/2009Dec03/Modified/MVC006052
            if ((m_pqm_g.GetPQMMode() == K_IMAGING))	{

                m_locator_button.EnableWindow(canenable_scan_edit_plan && IsOneScanDone() && m_bEnableLocatorOnScanEdit);
            }

            //-Patni-Hemant/2009Dec03/Modified/MVC006052
        }

        //Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred..."));

#define BOOLYESNO(b) (b ? _T("yes") : _T("No"))

        CString l_log_string = _T("");
        l_log_string.Format(_T("Total Selected Items: %d"), count);
        l_log_string = l_log_string + _T("  iswait_selected = ") + BOOLYESNO(iswait_selected);
        l_log_string = l_log_string + _T("  iscurrent_selected = ") + BOOLYESNO(iscurrent_selected);
        l_log_string = l_log_string + _T("  isdone_selected = ") + BOOLYESNO(isdone_selected);
        l_log_string = l_log_string + _T("  isfail_selected = ") + BOOLYESNO(isfail_selected);
        l_log_string = l_log_string + _T("  iswse_eidted_protocol_selected = ") + BOOLYESNO(iswse_eidted_protocol_selected);

        l_log_string = l_log_string + _T("\n");
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, GetPQMFlagsString());
#undef BOOLYESNO

        throw;
    }

    DEL_PTR_ARY(selected_items);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::EnableDisableQueueDialogButtons(
)
{
    //Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
    LPCTSTR FUNC_NAME = _T("CPQMView::EnableDisableQueueDialogButtons");

    //+Patni-Sribanta/2010Sep15/Added/MVC008802
    CString l_log_str(_T(""));
    l_log_str.Format(_T("m_is_wse_open : %s, m_procon_ui_status : %d"), m_is_wse_open ? _T("TRUE") : _T("FALSE"), m_procon_ui_status);
    PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::EnableDisableQueueDialogButtons"), l_log_str);
    //-Patni-Sribanta/2010Sep15/Added/MVC008802

    //Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    if (!m_queuedialog || !m_is_wse_open) {
        return ;
    }

    //+Patni-HEMANT/ADDED On 27/04/2009 6:52:10 PM/ PSP#269
    if ((m_procon_ui_status & PQM_CAN_PROCESS_PROCON_BUTTONS) != PQM_CAN_PROCESS_PROCON_BUTTONS) {

        //Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_procon_ui_status is not PQM_CAN_PROCESS_PROCON_BUTTONS"));

        return ;
    }


    //+Patni-HEMANT/MODIFIED On 27/04/2009 6:52:10 PM/ PSP#269
    //
    ////+Patni-HEMANT/ADDED On 3/6/2009 9:14:30 PM/ PSP #232
    ////inhibit procon buttons to be clicked..
    ////else part need not to be written, the remaining code is else part..
    //if(m_procon_ui_status == PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS) {
    //
    //	m_queuedialog->EnableDisableAllButtons(false);
    //	return ;
    //}
    ////-Patni-HEMANT/ADDED On 3/6/2009 9:14:30 PM/ PSP #232
    if ((m_procon_ui_status & PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS) == PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS) {

        //Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_procon_ui_status is PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS"));

        return ;
    }

    //-Patni-HEMANT/MODIFIED On 27/04/2009 6:52:10 PM/ PSP#269


    const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();
    //+Patni-Sribanta/2010Sep15/Added/MVC008802
    l_log_str.Format(_T("current_protocol_index_edited_by_wse: %d "), current_protocol_index_edited_by_wse);
    PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::EnableDisableQueueDialogButtons"), l_log_str);
    //+Patni-Sribanta/2010Sep15/Added/MVC008802

    if (current_protocol_index_edited_by_wse != -1) {

        //now enable disable the queue dialog.. buttons..
        //cancel has been pressed.. so disable all the
        //m_queuedialog->EnableDisableAllButtons(true);


        //enable till second last item..

        //Modified by Hemant On 12/21/2008 5:07:02 PM, we have different logic now..
        //bool can_enable_next_buttons = (current_protocol_index_edited_by_wse < (total_list_items-1)) ;
        bool can_enable_next_buttons = false ;
        bool l_is_enable_queueandexit = true;

        if (m_is_amb_planning) {
            int l_edited_item_count = m_selected_acqoder_list_for_wse.GetSize();
            int l_last_editing_item_list_index =  GetListForGivenProtocolAcqOder(m_selected_acqoder_list_for_wse.GetAt(l_edited_item_count - 1));
            bool l_is_last_index = (l_last_editing_item_list_index == current_protocol_index_edited_by_wse) ? true : false;

            l_is_enable_queueandexit = l_is_last_index;
            can_enable_next_buttons  = !(l_is_last_index);

        } else {

            if (m_selected_acqoder_list_for_wse.GetSize() == 1)
                can_enable_next_buttons = (current_protocol_index_edited_by_wse < (m_pqm_listctrl.GetItemCount() - 1)) ;

            else if ((m_selected_acqoder_list_for_wse.GetSize() > 1)) {
                can_enable_next_buttons = IsAnyProtocolPendingForWSEEditing() ;

            } else {
                can_enable_next_buttons = false ;
            }
        }

        //+Patni-Sribanta/2010Sep15/Added/MVC008802
        l_log_str.Format(_T("can_enable_next_buttons: %d "), can_enable_next_buttons);
        PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::EnableDisableQueueDialogButtons"), l_log_str);
        //+Patni-Sribanta/2010Sep15/Added/MVC008802

        m_queuedialog->EnableDisableButton(PQM_PROCON_QUEUE_NEXT, can_enable_next_buttons);
        m_queuedialog->EnableDisableButton(PQM_PROCON_NEXT_COPY, can_enable_next_buttons);

        //+Patni-HEMANT/ADDED On 3/6/2009 9:14:30 PM/ PSP #232
        //Enable remaining buttons..
        //we can first enable all buttons and then disable remaining but in doing so buttons will flicker
        //Thus doing it seperately..
        m_queuedialog->EnableDisableButton(PQM_PROCON_QUEUE_EXIT, l_is_enable_queueandexit);
        m_queuedialog->EnableDisableButton(PQM_PROCON_RESET, true);
        m_queuedialog->EnableDisableButton(PQM_PROCON_CANCEL, true);
        //-Patni-HEMANT/ ADDED On3/6/2009 9:45:07 PM/ PSP #232

    }
}

//************************************Method Header************************************
// Method Name  : TogglePauseResumeButtonAppearence
// Author       : PATNI/ HEMANT
// Purpose      : The fucntion wil chaneg the appearence of the button accoridng to ther
//				  flag given
//				  the button can only be pause / resume...and appearence will be
//				  according to the bitmap speciafied in this fucntion..
//***********************************************************************************
void CPQMView::TogglePauseResumeButtonAppearence(
    const bool set_pause_state /*= true*/
)
{
    SetBitmapForButton(m_pause_resume_button, set_pause_state ? IDB_PAUSE_BUTTON_IMAGE : IDB_RESUME_BUTTON_IMAGE);
    m_pause_resume_button.SetWindowText(_T(""));// I have put the text for this butotn in the rc file..
}

//************************************Method Header************************************
// Method Name  : EnableDisableScanRelatedButtons
// Author       : PATNI/ HEMANT
// Purpose      : this function enables / disables the scan related buttons according to
//				  passed variable..
//***********************************************************************************
void CPQMView::EnableDisableScanRelatedButtons(
    const bool enable /*= true*/,
    const bool apply_forcefully /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::EnableDisableScanRelatedButtons");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_pqm_g.GetPQMMode() != K_IMAGING) {
        return;
    }

    if (IsAcqumanReadyForNextRequest()) {

        const bool l_can_scan_enable_as_amb = (!IsScanning() && IsAMBInvoked() && !m_amb_controller.IsAMBScanStarted());
        m_scan_start_button.EnableWindow(enable && !m_scan_lock_acquired && GetSARControlStatusForScanRelatedButtons() && !l_can_scan_enable_as_amb);

    } else {
        //acquman is not ready so scan start is disabled..
        m_scan_start_button.EnableWindow(false);
        m_coil_button.EnableWindow(false);//Patni-Manish/2009Jun8/Added/JFT#250-Disable COIL button
    }


    if (apply_forcefully) {

        //consider enabling disabling of following buttons when scan is running..
        m_pause_resume_button.EnableWindow(enable);

    }

    BOOL l_can_enable_abort_button = (IsScanning() && !GetAbortFlag()) ;

    if (l_can_enable_abort_button) {

        if ((m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS) &&
            (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd) && dlgCoilSelWin->IsWindowVisible())) {

            l_can_enable_abort_button = FALSE;
        }
    }

    m_abort_button.EnableWindow(l_can_enable_abort_button);
    UpdateOtherButtonState();

}

//************************************Method Header************************************
// Method Name  : UpdateStatesForCtrls
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::UpdateStatesForCtrls(
    const pqm_msg_tag f_msg
)
{

    //Note : CControlStateMgr mgr has all teh states for ctr for various messages...
    //just get the state for each ctrl (ctrl you want) here.. and call the SetState
    //with appropriate args..
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateStatesForCtrls");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //CScopedLock l_scoped_lock(&m_deletion_cs); // Added for Redmine-2085
    SetStateForCtrl(&m_end_exam_button, f_msg);

    //Patni-Hemant/2011Mar30/Modified/REDMINE-1575
    if (!GetMemoryAliveFlag()) {
        SetStateForCtrl(&m_close_button, f_msg);

    } else {
        //+Patni-SS/COMMENTED-2009May13/JFT EndExam Requirement
        //m_close_button.EnableWindow(FALSE);
        //m_end_exam_button.EnableWindow(FALSE);
        SetBtnSensitive(IDC_END_EXQAM, FALSE);
    }


    //-Patni-SS/COMMENTED-2009May13/JFT EndExam Requirement

    if (m_pqm_g.GetPQMMode() == K_IMAGING) {

        SetStateForCtrl(&m_abort_button, f_msg);
        SetStateForCtrl(&m_pause_resume_button, f_msg);

        if (!(IsAMBInvoked() && (PQM_MSG_ABORT_WAIT == f_msg ||  PQM_MSG_ABORT_FAIL == f_msg)))
            SetStateForCtrl(&m_scan_start_button, f_msg);

        UpdateOtherButtonState();
    }

    SetStateForCtrl(&m_utility_button, f_msg);

    SetStateForCtrl(&m_inject_timer_button, f_msg);

    SetStateForCtrl(&m_injecttime_button, f_msg);
    SetStateForCtrl(&m_scan_offset_button, f_msg);
    SetStateForCtrl(&m_coil_button, f_msg);

    SetStateForCtrl(&m_delete_button, f_msg);
    SetStateForCtrl(&m_duplicate_button, f_msg);
    SetStateForCtrl(&m_scan_offset_static, f_msg);
    SetStateForCtrl(&m_coil_info_static, f_msg);

    SetStateForCtrl(&m_scan_anatomy_button, f_msg);

    m_amb_controller.UpdateStatesForCtrls(f_msg);

    SetFlagRelatedCtrlsStates();

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    SetStateForCtrlForCFADialog(f_msg);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ChangeContentForModelessInfoDialogs()
{
    if (GetSeqInfoDlgStatus()) {

        m_info_seq_dlg->DisplaySeqInformation();
    }

    if (GetCommentDlgStatus()) {

        m_comment_dlg->DisplayCommentInformation();
    }

    if (GetCoilDlgStatus()) {

        ChangeCoilDialogCaptionAppropriately();
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanCheckUnCheckFor(
    const int* const seq_items,
    const int seq_count
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::CanCheckUnCheckFor");

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == seq_items) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("seq_items pointer is NULL"));

        return false;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    CSequence* pseq = NULL ;

    for (int index = 0; index < seq_count; index++) {

        if (seq_items[index] < 0) {
            return false;
        }

        pseq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[index]);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL "));
            continue;
        }

        if (pseq->GetAcqStatus() != PR_ACQ_STATUS_WAIT)
            return false ;

    }

    return true;
}

//************************************Method Header************************************
// Method Name  : OpenQueueDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OpenQueueDialog(
    const bool f_show /*= true*/ 	//+Patni-HAR-HM/2009July22/Modified/MVC#2659
)
{

    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    if (!m_queuedialog) {

        m_queuedialoghandler = new CQueueDialogHandler(this);
        m_queuedialog = new CQueueDialog(m_queuedialoghandler);

        m_queuedialog->SetQueueButton(PQM_PROCON_QUEUE_NEXT, &m_queue_and_next);
        m_queuedialog->SetQueueButton(PQM_PROCON_QUEUE_EXIT, &m_queue_and_exit);
        m_queuedialog->SetQueueButton(PQM_PROCON_NEXT_COPY, &m_next_and_copy);

        //Patni-KSS/2010June16/ADDED/IR-155
        m_queuedialog->SetQueueButton(PQM_PROCON_CANCEL, &m_close);
        m_queuedialog->SetQueueButton(PQM_PROCON_RESET, &m_reset);


    }

    //initially, all button should be disabled..
    m_queuedialog->EnableDisableAllButtons(false);
    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
}

//************************************Method Header************************************
// Method Name  : OpenPatientOrientationDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OpenPatientOrientationDialog(
    const bool f_show /*= true*/ 	//+Patni-HAR-HM/2009July22/Modified/MVC#2659
)
{

    //CPoint offset = g_child_window_offset ;
    //CPoint offset(257,677);
    //+Patni-ARD/2010Apr07/Commented/JaFT# IR-100 remove IPCConfig.ini
    if (f_show) {
        if (!m_patientorientationdialog) {
            m_patientorientationhandler = new CPatientOrientationHandler(this) ;
            //CWnd* parent = CWnd::FromHandle(m_hWnd);
            m_patientorientationdialog = new CPatientOrientationDialog(m_patientorientationhandler, this, false , (m_axis_locator_position == 0) ? COUCH_POSITION_LEFT : COUCH_POSITION_RIGHT);
            m_patientorientationdialog->OnOffOrientationChangeFlag(false);

            m_patientorientationdialog->Create(IDD_PATIENT_ORIENTATION_DIALOG, this);

            //added by Hemant on 19/11/08 when re loading with
            //series we should also refresh its ctrls..
            OnSelectionStatusChanged();
        }

        //commented by Hemant ,done when doing setwindowpos
        //m_patientorientationdialog->ShowWindow(SW_SHOW);

        CRect main_window_rect;
        GetWindowRect(&main_window_rect);

        CRect dialog_rect ;
        m_patientorientationdialog->GetWindowRect(&dialog_rect);

        const int temp_width = dialog_rect.Width() ;
        const int temp_height = dialog_rect.Height();

        dialog_rect.top = 679 ;
        dialog_rect.left = 7 ;
        dialog_rect.bottom = dialog_rect.top + temp_height ;
        dialog_rect.right = dialog_rect.left + temp_width ;

        //+Patni-HAR-HM/2009July22/Modified/MVC#2659
        UINT flags = f_show ? (SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER) : (SWP_NOSIZE | SWP_NOZORDER);

        //Modified by Hemant On 11/27/2008 11:15:51 AM
        //m_patientorientationdialog->SetWindowPos(&wndTop, dialog_rect.left, dialog_rect.top , temp_width, temp_height, SWP_NOSIZE | SWP_NOZORDER );
        m_patientorientationdialog->SetWindowPos(&wndTop, dialog_rect.left, dialog_rect.top , temp_width, temp_height, flags); 	//+Patni-HAR-HM/2009July22/Modified/MVC#2659

    }

    //-Patni-ARD/2010Apr07/Commented/JaFT# IR-100 remove IPCConfig.ini
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::UpdateOrientationCombosOnSelectionChanged(
    const int selected_item,
    const bool multiple_selection /*= false*/
)const
{
    //+Patni-HAR-HM/2009July22/Modified/MVC#2659
    if (!m_patientorientationdialog) {
        return ;
    }

    m_patientorientationdialog->OnOffOrientationChangeFlag(false);

    //-Patni-HAR-HM/2009July22/Modified/MVC#2659
    if ((m_pqm_listctrl.GetItemCount() == 0)) {

        m_patientorientationdialog->UpdateOrientationAccordingto(PatientInsertDirection_Invalid, PatientRotationDirection_Invalid, PatientViewDirection_Invalid);
        return ;
    }



    if (IsValidSequenceIndex(selected_item) && !m_is_wse_open) {

        //item should be of wait state..only
        CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(selected_item);

        if ((selected_item == 0) && (l_seq->GetAcqStatus() == PR_ACQ_STATUS_WAIT) && !m_iswait_cursor)
            //-Patni-Hemant/2010Apr21/Added/JaFT/Patient Orientation Buf Fix
            m_patientorientationdialog->OnOffOrientationChangeFlag(true);


        if ((m_patientorientationdialog->GetPatientOrientationOption1() != -1) &&
            (m_patientorientationdialog->GetPatientOrientationOption2() != -1) &&
            (m_patientorientationdialog->GetPatientOrientationOption3() != -1)
           ) {

            CPqmPrMode pr_mode = l_seq->GetPrMode();

            m_patientorientationdialog->UpdateOrientationAccordingto(
                (PatientInsertDirection)GetPatientOrientationOptionFor(OriType_PatientInsertDir, pr_mode.m_pat_ori.end_in),
                (PatientRotationDirection) GetPatientOrientationOptionFor(OriType_PatientRotationDir, pr_mode.m_pat_ori.side_up),
                (PatientViewDirection) GetPatientOrientationOptionFor(OriType_PatientViewDir, pr_mode.m_view_dir)
            );
        }
    }

    //ther is no need to change the combo items according to selection...as there will always be selection for all items
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OpenMRSPreScanDialog(
)
{
    if (!g_MainView->IsWindowVisible()) {
        m_is_mrs_window_opened_before_swithcing_page = true ;
        return ;
    }

    if (!m_mrsprescandlg_handler) {
        m_mrsprescandlg_handler = new CMRSPreScanDlgHandler(this);
        m_mrsprescandlg = new CMRSPreScanDlg(m_mrsprescandlg_handler);
    }

    //Patni-HAR/2009Mar05/Commented/PSP1- MRS PreScan Dialog Assertion
    if (m_mrsprescandlg /*&& !m_mrsprescandlg->IsWindowVisible()*/) {

        if (!m_mrsprescandlg->GetSafeHwnd()) {
            SMRSScan_s mrsscan_values;

            if (!GetDefaultMRSPrescanValue(mrsscan_values)) {

                return ;
            }

            m_mrsprescandlg->SetMRSscanValues(mrsscan_values);
            m_mrsprescandlg->SetMrsLicensePresent(GetMrsBrstLicense());
            m_mrsprescandlg->Create(IDD_MRSPRESCANDLG_DIALOG, this);

        } else {
            m_mrsprescandlg->SetFocus();
        }

    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ProcessMRSMessages(
    const pqm_msg_tag f_msg
)
{
    if (f_msg == PQM_MSG_MRS_SCAN) {
        //Patni-PJS/2010July02/Added/V1.30#TMSC-REDMINE-316
        m_is_mrs_prot = true;
        OpenMRSPreScanDialog() ;

    } else if (m_mrsprescandlg && m_mrsprescandlg->m_hWnd) {
        //Patni-PJS/2010July02/Added/V1.30#TMSC-REDMINE-316
        m_is_mrs_prot = false;
        m_mrsprescandlg->ProcessMRSMessages(f_msg);
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ Sarjeet
// Purpose      :
//***********************************************************************************
void CPQMView::ProcessProConMessages(
    const pqm_msg_tag f_msg
)
{
    // new message added for procon serever
    LPCTSTR FUNC_NAME = _T("CPQMView::ProcessProConMessages");
    PQM_TRACE_SCOPED(FUNC_NAME);

    switch (f_msg) {

        case PQM_MSG_PROCON_ERR: {
            ResetAMBPlanProtocols();
        }
        break;

        case PQM_MSG_PROCON_CMD_EXIT:
        case PQM_MSG_PROCON_NOT_CONNECTED: {


            m_is_procon_connected = (PQM_MSG_PROCON_CMD_EXIT == f_msg) ;

            int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();
            CString l_str;
            l_str.Format(_T("current_protocol_index_edited_by_wse = %d"), current_protocol_index_edited_by_wse);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

            if ((IsValidSequenceIndex(current_protocol_index_edited_by_wse)) && !m_inscan_refclose_msg_sent) {
                OnScanEditStatusChangedForCurrentProtocol(current_protocol_index_edited_by_wse, WFDA_Protocol_WSEEditing_Cancel);
            }

            if (m_is_wse_open)	//close the current working of procon..
                SendMessageToProcon(PQM_PROCON_CANCEL, true , false);

            if (IsValidSequenceIndex(current_protocol_index_edited_by_wse)) {
                m_pqm_listctrl.InvalidateItemsForPLANColumn(current_protocol_index_edited_by_wse);
            }

            if (m_amb_controller.GetIsCanResetAMBprotocolsOnWseExit())
                ResetAMBPlanProtocols();

            m_amb_controller.SetIsCanResetAMBprotocolsOnWseExit(true);
            SetIsAMBPlanning(false);



        }
        break ;

        case PQM_MSG_PROCON_CONNECTED:
            m_is_procon_connected = true ;

            break ;

        case PQM_MSG_PROCON_QUEUE_NEXT:

            if (m_is_wse_open) {

                //+Patni-Hemant/2010May21/Commented/MaFT/MVC007927
                ////Added by Hemant On 2/9/2009 7:05:22 PM
                //WSEEditingCompleted(WSE_Editing_Operation_GUISave);
                //-Patni-Hemant/2010May21/Commented/MaFT/MVC007927

                SendMessageToProcon(PQM_PROCON_QUEUE_NEXT, true , false);
            }

            break ;

        case PQM_MSG_PROCON_QUEUE_EXIT:

            if (m_is_wse_open) {

                //+Patni-Hemant/2010May21/Commented/MaFT/MVC007927
                //
                ////+Patni-HEMANT/ADDED On 3/16/2009 2:40:53 PM/ ACE1/ MCM0213-00004 (STM45-02655)- Pqm  List Defect Handling(595)
                ////highlight the last edited item
                //const int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();
                //m_pqm_listctrl.SelectOnlyItem(current_protocol_index_edited_by_wse);
                ////-Patni-HEMANT/ADDED On 3/16/2009 2:40:53 PM/ ACE1/ MCM0213-00004 (STM45-02655)- Pqm  List Defect Handling(595)
                //
                //
                ////Added by Hemant On 2/9/2009 7:05:22 PM
                //WSEEditingCompleted(WSE_Editing_Operation_GUISave);
                //-Patni-Hemant/2010May21/Commented/MaFT/MVC007927

                //+Patni-HEMANT/COMMENTED On 3/12/2009 5:41:08 PM/ PSP#232
                //Note : Now when closing WSE using [Queue And Exit] or [Cancel] button,
                //we wil lbe getting PQM_MSG_PROCON_CMD_EXIT message.
                //when PQM_MSG_PROCON_QUEUE_EXIT message is coming from IPC, we just need
                //to save the GUI changes here, we have already saved the data on process of
                //PQM_MSG_PROCON_QUEUE_DONE at GUI side.
                //so we will not be processing PQM_PROCON_QUEUE_EXIT only for saving GUI changes,
                //for closing purpose, we will have PQM_MSG_PROCON_CMD_EXIT message at GUI side
                //
                //SendMessageToProcon(PQM_PROCON_QUEUE_EXIT, true , false);
                //
                //-Patni-HEMANT/ ADDED On3/12/2009 5:41:13 PM/ PSP#232

                //+Patni-Hemant/2010May21/Commented/MaFT/MVC007927
                m_pqm_listctrl.SelectOnlyItem(m_pqm_listctrl.GetLastPlannedProtocol());
                //Patni-PANKAJ(PJS)/2010May28/Added/MaFT/MVC008019
                ChangeContentForModelessInfoDialogs();
                m_pqm_listctrl.ResetLastPlannedProtocol();
                //-Patni-Hemant/2010May21/Commented/MaFT/MVC007927

                if (m_is_amb_planning) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invoking AMB Dialog..."));

                    m_amb_controller.SetIsCanResetAMBprotocolsOnWseExit(false);
                    m_amb_controller.SetIswaitforAMBDlgOpen(true);
                    PostMessage(WM_INVOKE_AMB, 0, 0);

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_is_amb_planning = FALSE"));
                }
            }

            break ;

        case PQM_MSG_PROCON_NEXT_COPY:

            if (m_is_wse_open)
                SendMessageToProcon(PQM_PROCON_NEXT_COPY, true , false);

            break ;

        case PQM_MSG_PROCON_RESET:

            if (m_is_wse_open)
                SendMessageToProcon(PQM_PROCON_RESET, true , false);

            break ;

        case PQM_MSG_PROCON_QUEUE_DONE: {
            // To DO for breath hold and Scan time

            //Modified by Hemant On 2/9/2009 7:07:54 PM
            //WSEEditingCompleted();
            int current_protocol_index_edited_by_wse = m_pqm_listctrl.GetWSEEditingProtocolIndex();

            if (OnScanEditStatusChangedForCurrentProtocol(current_protocol_index_edited_by_wse, WFDA_Protocol_WSEEditing_Save)) {
                m_inscan_refclose_msg_sent = true;
            }

            WSEEditingCompleted(WSE_Editing_Operation_SaveOnly);

        }
        break;

        //+Patni-HEMANT/ADDED On 3/6/2009 9:12:24 PM/ #232
        case PQM_MSG_PROCON_CON_ALLOW_SELECTIONS : {

            //+Patni-HEMANT/ADDED On 27/04/2009 6:52:10 PM/ PSP#269
            m_procon_ui_status |= PQM_CAN_PROCESS_PROCON_BUTTONS;

            ProcessAllowOrInhibitSelectionForProconButtons(true);
        }
        break ;

        case PQM_MSG_PROCON_CON_INHIBIT_SELECTIONS : {

            ProcessAllowOrInhibitSelectionForProconButtons(false);
        }
        break ;

        //-Patni-HEMANT/ADDED On 3/6/2009 9:12:24 PM/ #232
        case PQM_MSG_PROCON_INSCAN_SWITCH: {
            OnScanEditStatusChangedForCurrentProtocol(m_pqm_listctrl.GetWSEEditingProtocolIndex(), WFDA_Protocol_Inscan_Switch);
        }
        break;

        case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN: {

            OnScanEditStatusChangedForCurrentProtocol(m_pqm_listctrl.GetWSEEditingProtocolIndex(), WFDA_Protocol_WSEEditing_Start);

        }
        break;

        case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_SHOW: {

            OnScanEditStatusChangedForCurrentProtocol(m_pqm_listctrl.GetWSEEditingProtocolIndex(), WFDA_Protocol_BlockSetting_Show);

        }
        break;

        case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_CLOSE: {

            OnScanEditStatusChangedForCurrentProtocol(m_pqm_listctrl.GetWSEEditingProtocolIndex(), WFDA_Protocol_WSEEditing_Cancel);
        }
        break;

        //+Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ PSP/ IDS#202, IDS#192
        case PQM_MSG_CONNECT_FROM_MAIN_THREAD : {

            //m_acquisition->ConnectToProconFromGUI();
            //PostMessage(WM_RECONNECT_TO_PROCON,0,0);
            SendMessage(WM_RECONNECT_TO_PROCON, 0, 0);
        }
        break ;
        //-Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ PSP/ IDS#202, IDS#192

    }

    EnableDisableScanEditOrPlanButtons();

    // TO DO:- Locator and ScanEdit button's Sensitivity has change as per message

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::OnDialogOpenedForParameterEditing(
    const int item_index
)
{

    if (!IsValidSequenceIndex(item_index)) {
        return false ;
    }

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(item_index);

    //if the hold is not inlcuding VFC_HOLD_PRE_SCAN then the auto scan is on..
    bool auto_scan_on = ((l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD")) & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) ;

    if (auto_scan_on) {

        ApplyAutoScanToItems(&item_index, 1, false);		//make it off explicitly..
    }

    return auto_scan_on ;
}


void CPQMView::OnScanEditOrPlan(const ProconMessageEnum procon_message, const bool f_is_auto_plan /*= false*/)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::OnScanEditOrPlan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_is_wse_open && (m_procon_ui_status & PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inhibit selection is true"));
        return;
    }

    m_pqmchildthread.PostMessageToThread(ON_SCAN_EDIT_OR_PLAN, (WPARAM)procon_message, (LPARAM) f_is_auto_plan);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::StartScanEditOrPlan(
    const ProconMessageEnum procon_message,
    const bool f_is_auto_plan /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::StartScanEditOrPlan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              (PQM_PROCON_SCAN_PLAN == procon_message) ? _T("Trace : PQM_PROCON_SCAN_PLAN") : _T("Trace : PQM_PROCON_SCAN_EDIT"));

    if ((PQM_PROCON_SCAN_PLAN == procon_message) && m_bLocatorAlreadyEnable) {

        SetIsAMBPlanning(false);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Occured. Locator button is Disabled."));
        return ;
    }

    if ((PQM_PROCON_SCAN_EDIT == procon_message) && !m_scanedit_button.IsWindowEnabled()) {

        SetIsAMBPlanning(false);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Occured. WSE button is Disabled."));
        return ;
    }

    if (m_procon_ui_status & PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS) {

        SetIsAMBPlanning(false);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inhibit selection is true"));
        return;
    }

    if (m_iswait_cursor_for_pqm_buttons) {

        SetIsAMBPlanning(false);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_iswait_cursor_for_pqm_buttons is true, return!"));
        return;
    }

    //+Patni-SS/COMMENTED/2009June05/IR-23
    if (/*m_is_wse_open || */!m_is_procon_connected) {	//already open ?
        SetIsAMBPlanning(false);
        return ;
    }

    if (m_acquisition) {

        VARIANT data;
        data.byref = NULL ;

        if (S_OK != m_acquisition->GetScanMutex(&data)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));
            SetIsAMBPlanning(false);
            return;
        }

        MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* l_scan_mutex = (MR::ACQ::SM::COMMON::MUTEX::CThreadMutex*) data.byref;
        data.byref = NULL ;

        CScopedLock l_scoped_lock(l_scan_mutex);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

        if ((PQM_PROCON_SCAN_PLAN == procon_message) && m_bLocatorAlreadyEnable) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Locator already invoked"));
            SetIsAMBPlanning(false);
            return ;
        }

        bool iswait_selected = false ;
        bool iscurrent_selected = false ;
        bool isdone_selected = false ;
        bool isfail_selected = false ;

        int* selected_items = NULL ;
        int count = 0 ;

        if (f_is_auto_plan) {
            const int topmostwaitnotautoscanprotocol = m_pqm_listctrl.GetFirstWaitProtocolForAutoPlan();

            if (!IsValidSequenceIndex(topmostwaitnotautoscanprotocol)) {

                CString l_str = _T("");
                l_str.Format(_T("Actual First Wait-NonAutoScan Protocol Index is : %d"), topmostwaitnotautoscanprotocol);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                SetIsAMBPlanning(false);
                return;
            }

            selected_items = new int[1];
            selected_items[0] = topmostwaitnotautoscanprotocol;
            count = 1;

            m_pqm_listctrl.SelectOnlyItem(topmostwaitnotautoscanprotocol);

        } else {
            count = m_pqm_listctrl.GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected);
        }

        //+ Patni-PJS/2009Dec31/Added+Modified/MVC006364
        if (!selected_items) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("selected_items pointer is NULL"));
            SetIsAMBPlanning(false);
            return;
        }

        //- Patni-PJS/2009Dec31/Added+Modified/MVC006364
        //Patni-Hemant/2011Mar30/Modified/REDMINE-1575
        if (GetMemoryAliveFlag() || (!selected_items || count <= 0) || (iscurrent_selected || isdone_selected || isfail_selected)) {

            //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
            DEL_PTR_ARY(selected_items);
            //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
            SetIsAMBPlanning(false);
            return ;
        }

        //Patni-HEMANT/2009Sept05/Added/ MVC#002493
        //making this flag available when using differnt thread..
        m_is_wse_open = true;

        //set the top most index as wse editing index...
        m_pqm_listctrl.SetWSEEditingProtocolIndex(selected_items[0]);
        m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(selected_items[0]);

        //+Patni-PJS/2011Feb1/Modify/IR-97
        if (!CanProcessProconMessage((int)procon_message)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanProcessProconMessage failed"));
            m_is_wse_open = false;
            m_pqm_listctrl.ResetWSEEditingProtocolIndex();
            m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(selected_items[0]);
            SetIsAMBPlanning(false);
            return;
        }

        //-Patni-PJS/2011Feb1/Modify/IR-97

        //-------------------------------------------------------------------------
        //Now according to vantage remove auto scan from currently selected protocol for wse editing
        //
        // Defect # 614
        //Added by Hemant On 1/15/2009 3:17:37 PM
        if (selected_items[0] != -1) {

            ApplyAutoScanToItems(selected_items, 1, false);		//make it off explicitly..
        }

        //-------------------------------------------------------------------------

        //Added by Hemant On 12/20/2008 2:48:25 PM
        //don't send normal indexes but send the acq orders
        ClearSelectedAcqoderListForWSE();

        //+Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
        CString l_editing_items = _T("");
        l_editing_items.Format(_T("Total Items to Edit = %d "), count);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_editing_items);
        //-Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1

        for (int index = 0; index < count ; index++) {

            //+Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
            l_editing_items.Format(_T("WSE Selected Item Index = %d "), selected_items[index]);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_editing_items);
            //-Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1

            CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(selected_items[index]);

            if (NULL == l_seq) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL "));
                continue;
            }

            if (GetIsAMBPlanning()) {
                if (WSE_Planned_AND_Current == l_seq->GetWSEEditingEnum() || WSE_Planned == l_seq->GetWSEEditingEnum())
                    l_seq->SetAMBEditingEnum(AMB_PLANNED);

                else
                    l_seq->SetAMBEditingEnum(AMB_PLAN);

            } else
                l_seq->SetAMBEditingEnum(AMB_NONE);

            m_pqm_listctrl.InvalidateItemsForPLANColumn(selected_items[index]);

            selected_items[index]  = GetAcqOderForGivenProtocolIndex(selected_items[index]);

            //also store in member variable list
            m_selected_acqoder_list_for_wse.Add(selected_items[index]) ;

            //+Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
            l_editing_items.Format(_T("WSE Selected Item Acq_order = %d "), selected_items[index]);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_editing_items);
            //-Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1

        }

        //+Patni-HEMANT/ADDED On 27/04/2009 6:52:10 PM/ PSP#269
        m_procon_ui_status = PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS ;

        //Modified by Hemant On 2/11/2009 3:16:15 PM
        //WSE is opening indicated by first parameter,
        //Which button was clicked, indicated by second parameter
        //OnWSEOpenClosed();
        OnWSEOpenClosed(true, procon_message);

        //PqmSeverConfigToProCon (button type , first selected item , selected item array , selected item count)...
        //Note this functionwill delete the selected_items array..
        //+Patni-RSG Modified and Added /2009Jun15/JFT-324
        const status_t res =  m_acquisition->PqmSeverConfigToProCon((int)procon_message, selected_items[0] , selected_items, count);

        //Patni-ARD/2010Apr29/Added/JaFT# MCM0229-00178_CDR_06 + code review + memory leak
        DEL_PTR_ARY(selected_items);

        if (res == E_ERROR) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("PqmSeverConfigToProCon Failed..."));

            //+Patni-RSG/ADDED/2009June20
            m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(m_pqm_listctrl.GetWSEEditingProtocolIndex());
            m_pqm_listctrl.ResetWSEEditingProtocolIndex();
            //+Patni-RSG/ADDED/2009June20

            OnWSEOpenClosed(false);
            ResetAMBPlanProtocols();
        }

        m_inscan_refclose_msg_sent = false;
        //-Patni-RSG Modified and Added /2009Jun15/JFT-324

        return ;
    }

    //    ASSERT(FALSE);
    return ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//Modified by Hemant On 2/11/2009 3:15:34 PM , changing the method prototype..
//second arg will be used when first arg is true..
//void CPQMView::OnWSEOpenClosed(
//	bool wse_open /*= true*/
void CPQMView::OnWSEOpenClosed(
    const bool wse_open /*= true*/ ,
    const ProconMessageEnum procon_message /*= PQM_PROCON_SCAN_EDIT*/
)
{
    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
    LPCTSTR FUNC_NAME = _T("CPQMView::OnWSEOpenClosed");
    PQM_TRACE_SCOPED(FUNC_NAME);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , wse_open ? _T("WSE Open") : _T("WSE Closed"));
    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2

    m_is_wse_open = wse_open;		//scan edit and plan buttons can be enabled now..
    //+ Patni-PJS/2009Jun19/Added/IDS#624,625
    m_bLocatorAlreadyEnable = (procon_message == PQM_PROCON_SCAN_PLAN) ? true : false;
    EnableDisableScanEditOrPlanButtons();

    //Patni-ARD/2010Jan29/Added/DeFT# MVC4446
    if (!wse_open) {
        m_is_locator_open = wse_open;
        m_bLocatorAlreadyEnable = wse_open;
    }

    //EnableWindow(!wse_open);

    //(GetDlgItem(IDC_CLOSE))->EnableWindow(!wse_open);
    //m_pass_button.EnableWindow(!wse_open);
    SetFlagRelatedCtrlsStates();

    //now enable disable the queue dialog.. buttons..
    //Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    if (m_queuedialog) {

        //+Patni-HEMANT/MODIFIED On 3/7/2009 3:41:19 PM/ PSP#232
        //Previously, when scan edit was clicked,we were enabling all the buttons
        //and when WSE was closed, we were disabling the butons,
        //But now, when WSE is opened by clicking Scan edit, don't enable the buttons
        //But when WSe is closed, diable the buttons
        ////cancel has been pressed.. so disable all the
        //m_queuedialog->EnableDisableAllButtons(wse_open);
        if (!wse_open) {
            m_queuedialog->EnableDisableAllButtons(false);
        }

        //The following functiuons dispalyes the hour glass only on queue dialog
        //1)this will show hour glass when scan edit is clicked,
        //when wse is opened, we wil get allow_selection message
        //that time we will remove he hour glass there..
        //2) when wse is editing a paramater, we wil lget inhibit_selection
        //from procon, that time we will again show hour glass.ON getting allow_seleciton
        //we will remove hour glass.
        //3)when queue and next or any other button is clicked, we will show hour glass.
        //On receiving allow_selection, we will remove hour glass.
        //
        //if(wse_open)

        //+Patni-HEMANT/ADDED On 3/10/2009 8:02:50 PM/ PSP#232
        //allow procon buttons to be clicked..
        //m_procon_ui_status = PQM_PROCONALLOW_SELECTIONS_PROCON_BUTTONS ;
        //DisplayHourglassOnProconRelatedButtons(false);
        ProcessAllowOrInhibitSelectionForProconButtons(!wse_open, false);
        DisplayHourglassOnProconRelatedButtons(wse_open);
        //
        //-Patni-HEMANT/ MODIFIED On3/7/2009 3:41:27 PM/ PSP#232

        //+Patni-HEMANT/COMMENTED On 27/04/2009 6:52:10 PM/ PSP#269
        ////enable disable other queue dialog items..
        //EnableDisableQueueDialogButtons();
        //-Patni-HEMANT/COMMENTED On 27/04/2009 6:52:10 PM/ PSP#269
    }

    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    RemoveGUIProcessingBit(PQM_PROCESSING_QUEUE_NEXT);
    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

    OnSelectionStatusChanged();
}

//************************************Method Header************************************
// Method Name  : IsOneScanDone
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsOneScanDone(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::IsOneScanDone");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_list) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_list pointer is NULL"));

        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    POSITION pos = m_list->GetHeadPosition();
    CSequence* pseq = NULL ;

    while (pos) {
        pseq = m_list->GetNext(pos);

        switch (pseq->GetAcqStatus()) {

            case PR_ACQ_STATUS_SUCCESS :	//we have first found done before first wait..so return true..
                return true ;

            case PR_ACQ_STATUS_WAIT :
                return false;		//if wait is found before fail/ current/ done.. then we can not have done before this..
        }
    }

    return false ;
}

//************************************Method Header****************************
// Method Name  : ShowHideSubDialogs()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CPQMView::ShowHideSubDialogs(
    const bool f_show
)
{

    if (f_show) {

        //+Patni-HEMANT/Commented On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
        //        //Added by Hemant On 7/22/2008 6:55:40 PM
        //        if(!m_queuedialog)	//only one time..
        //            OpenQueueDialog(f_show); 	//+Patni-HAR-HM/2009July22/Modified/MVC#2659
        //        else
        //            m_queuedialog->ShowWindow(SW_SHOW);
        //-Patni-HEMANT/Commented On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90

        if (!m_patientorientationdialog)	//only one time..
            OpenPatientOrientationDialog(f_show); 	//+Patni-HAR-HM/2009July22/Modified/MVC#2659

        else
            m_patientorientationdialog->ShowWindow(SW_SHOW);

    } else {


        //Added by Hemant On 7/22/2008 6:20:48 PM
        if (m_patientorientationdialog)
            m_patientorientationdialog->ShowWindow(SW_HIDE);

        //+Patni-HEMANT/Commented On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
        //        //Added by Hemant On 7/22/2008 6:20:52 PM
        //        if(m_queuedialog)
        //            m_queuedialog->ShowWindow(SW_HIDE);
        //-Patni-HEMANT/Commented On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    }
}

//************************************Method Header************************************
// Method Name  : DestroySubDilaogsOnPQMHide
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::DestroySubDilaogsOnPQMHide(
)
{
    DEL_PTR(m_comment_dlg)

    //Added by Hemant On 11/14/2008 11:56:42 AM
    DEL_PTR(m_interfaceclassforscancommentdlg)

    DEL_PTR(m_info_seq_dlg)

    DEL_PTR(dlgCoilSelWin)

    //Patni-SS/ADDED/2009May13/JFT#Optimization
    DEL_PTR(m_pqmmanualprescandlg)


    DEL_PTR(m_delay_time_dlg)

    if (::IsWindow(m_pqmplandup_dlg.m_hWnd)) {
        m_pqmplandup_dlg.DestroyWindow();
    }
}

bool CPQMView::CreateScanAnatomyPopupMenu(
    PopupMenuEx* f_scan_anatomy_menu,
    const int f_sar_index,
    const SScanAantomy& f_scan_anatomy,
    CArray<PopupMenuEx*, PopupMenuEx*>* f_sub_menu_list
)const
//-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
{

    LPCTSTR FUNC_NAME = _T("CPQMView::CreateScanAnatomyPopupMenu");

    //+++Added SM3_Mangesh_Start
    //For checking initial limits on Array of list .Since List index starts from 0;
    if (f_sar_index <= 0) {
        return false;
    }

    //+++Added SM3_Mangesh_End

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_scan_anatomy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_scan_anatomy pointer is NULL"));
        return false;
    }//-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CScanAnatomyData* scan_anatomy_list = m_scan_anatomy[f_sar_index - 1] ;

    //Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
    bool l_already_checked = false ;

    //Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    PopupMenuEx* l_laterality_menu = NULL ;

    int l_menuid = ID_MENU_SCAN_ANATOMY;

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    int l_laterality_menu_index = ID_LATERALITY_MENU_MIN ;
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137


    SScanAantomy scan_anatomy ;
    POSITION pos = scan_anatomy_list->GetHeadPosition();

    while (pos) {

        scan_anatomy = scan_anatomy_list->GetNext(pos);

        //+Patni-Hemant/2010Apr26/Modified/JaFT/IR-137
        /*
        		f_scan_anatomy_menu->InsertMenu(-1, MF_BYPOSITION, l_menuid, scan_anatomy.m_scan_anatomy_name);

                //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
                //if (f_scan_anatomy == scan_anatomy.m_scan_anatomy_vf) {
                if ((f_scan_anatomy.m_scan_anatomy_name.CompareNoCase(scan_anatomy.m_scan_anatomy_name) == 0)
                    && (f_scan_anatomy.m_scan_anatomy_vf == scan_anatomy.m_scan_anatomy_vf)
                    && !l_already_checked
                   ) {
                    l_already_checked = true ;
                    //-Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131

                    f_scan_anatomy_menu->CheckMenuItem(l_menuid, MF_BYCOMMAND | MF_CHECKED);
                }
        */
        //const UNIT l_flag = (scan_anatomy.GetLiteralityFlag()) ? MF_POPUP | MF_BYPOSITION : MF_BYPOSITION ;
        //f_scan_anatomy_menu->InsertMenu(-1, l_flag , l_menuid, scan_anatomy.m_scan_anatomy_name);

        l_laterality_menu = NULL ;

        if (scan_anatomy.GetLiteralityFlag()) {
            l_laterality_menu = new PopupMenuEx ;

            l_laterality_menu->CreatePopupMenu();
            l_laterality_menu->InsertMenu(-1, MF_BYPOSITION, l_laterality_menu_index++, g_L_None_MenuName);
            l_laterality_menu->InsertMenu(-1, MF_BYPOSITION, l_laterality_menu_index++, g_L_Left_MenuName);
            l_laterality_menu->InsertMenu(-1, MF_BYPOSITION, l_laterality_menu_index++, g_L_Right_MenuName);


            //+Patni-KSS+Hemant/2010Aug27/Added/ITT-068/Redmine-691
            /*
            int l_check_item = l_laterality_menu_index;

            switch (f_scan_anatomy.GetLaterality()) {
            case Laterality_None:
            	l_check_item--;
            case Laterality_Left:
            	l_check_item--;
            case Laterality_Right:
            	l_check_item--;

            	break;
            default :
            	l_check_item = -1;
            }

            if (l_check_item != l_laterality_menu_index) {
            	l_laterality_menu->CheckMenuItem(l_check_item, MF_BYCOMMAND | MF_CHECKED);
            }
            */
            //-Patni-KSS+Hemant/2010Aug27/Added/ITT-068/Redmine-691


            f_sub_menu_list->Add(l_laterality_menu);
        }

        //+Patni-Hemant/2010May25/Added/MaFT/TMSC-REDMINE-MVC007915
        else {
            l_laterality_menu_index += g_laterality_menuitems;
        }

        //-Patni-Hemant/2010May25/Added/MaFT/TMSC-REDMINE-MVC007915

        if (l_laterality_menu) {
            f_scan_anatomy_menu->AppendMenu(MF_POPUP, (UINT)(l_laterality_menu)->m_hMenu, scan_anatomy.m_scan_anatomy_name);

        } else {
            f_scan_anatomy_menu->InsertMenu(-1, MF_BYPOSITION, l_menuid, scan_anatomy.m_scan_anatomy_name);
        }

        //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
        //if (f_scan_anatomy == scan_anatomy.m_scan_anatomy_vf) {
        if ((f_scan_anatomy.m_scan_anatomy_name.CompareNoCase(scan_anatomy.m_scan_anatomy_name) == 0)
            && (f_scan_anatomy.m_scan_anatomy_vf == scan_anatomy.m_scan_anatomy_vf)
            && !l_already_checked
           ) {
            l_already_checked = true ;
            //-Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131

            //+Patni-KSS+Hemant/2010Aug27/Added/ITT-068/Redmine-691
            if (l_laterality_menu) {
                int l_check_item = l_laterality_menu_index;

                switch (f_scan_anatomy.GetLaterality()) {
                    case Laterality_None:
                        l_check_item--;

                    case Laterality_Left:
                        l_check_item--;

                    case Laterality_Right:
                        l_check_item--;

                        break;

                    default :
                        l_check_item = -1;
                }

                if (l_check_item != -1) {
                    l_laterality_menu->CheckMenuItem(l_check_item, MF_BYCOMMAND | MF_CHECKED);
                }
            }

            //-Patni-KSS+Hemant/2010Aug27/Added/ITT-068/Redmine-691

            f_scan_anatomy_menu->CheckMenuItem(l_menuid - ID_MENU_SCAN_ANATOMY, MF_BYPOSITION | MF_CHECKED);
        }

        //-Patni-Hemant/2010Apr26/Modified/JaFT/IR-137

        l_menuid ++ ;
    }

    return true ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ShowHideOpenedWindows(
    const bool f_show /*= true*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ShowHideOpenedWindows");
    PQM_TRACE_SCOPED(FUNC_NAME);

    ShowHideOpenedWindows(m_comment_dlg , m_is_comment_window_opened_before_swithcing_page , f_show);

    //+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    if (m_comment_dlg) {
        m_comment_dlg->ShowHideOpenedWindowsOnPageChanged(f_show);
    }

    //-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

    ShowHideOpenedWindows(m_mem_passworddlg , m_is_mem_passworddlg_window_opened_before_swithcing_page, f_show); //Patni-Sudhir 2011Mar-09/Added/IR-185

    ShowHideOpenedWindows(m_info_seq_dlg , m_is_seqinfo_window_opened_before_swithcing_page, f_show);
    ShowHideOpenedWindows(m_obj_gate_dlg , m_is_gateinfo_window_opened_before_swithcing_page, f_show);
    ShowHideOpenedWindows(dlgCoilSelWin , m_is_coil_window_opened_before_swithcing_page, f_show);

    ShowHideOpenedWindows(m_delay_time_dlg , m_is_delay_time_window_opened_before_swithcing_page , f_show);

    if (dlgCoilSelWin)
        dlgCoilSelWin->ShowHideOpenedWindowsOnPageChanged(f_show);

    //Now for modal dialogs..we are doing them late be'us that should be on top
    ShowHideOpenedWindows(m_cfadlg , m_is_cfa_window_opened_before_swithcing_page, f_show);
    ShowHideOpenedWindows(m_anatomy_dlg , m_is_sar_window_opened_before_swithcing_page, f_show);
    ShowHideOpenedWindows(m_setscanoffsetdlg , m_is_scanoffset_window_opened_before_swithcing_page, f_show);

    ShowHideOpenedWindows(m_pqmmanualprescandlg , m_is_manualprescan_window_opened_before_swithcing_page, f_show);

    //+Patni-HEMANT/2010Jun30/Added/V1.30#TMSC-REDMINE-316
    //ShowHideOpenedWindows(m_mrsprescandlg , m_is_mrs_window_opened_before_swithcing_page, f_show);
    //Patni-PJS/2010July02/Added/V1.30#TMSC-REDMINE-316
    if (f_show && m_is_mrs_prot/*m_is_mrs_window_opened_before_swithcing_page*/ && (!m_mrsprescandlg || !::IsWindow(m_mrsprescandlg->m_hWnd))) {
        OpenMRSPreScanDialog();

    } else {
        ShowHideOpenedWindows(m_mrsprescandlg , m_is_mrs_window_opened_before_swithcing_page, f_show);
    }

    //-Patni-HEMANT/2010Jun30/Added/V1.30#TMSC-REDMINE-316

    //Added by Hemant On 1/19/2009 4:39:20 PM , Fixing 753
    ShowHideOpenedWindows(m_obj_coil_dlg , m_is_coilinfo_opened_before_swithcing_page, f_show);
    //Patni-DKH/2010Feb02/Added/PH#3#/FBI
    ShowHideOpenedWindows(&m_pqmplandup_dlg, m_is_palnduplicate_window_opened_before_swithcing_page , f_show);
    //Patni-DKH/2010Feb02/Added/PH#3#/IR86-DebugWindow
    ShowHideOpenedWindows(&m_pqmdebugdlg, m_is_pqmdebugwin_window_opened_before_swithcing_page , f_show);

    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    if (m_cds_controller) {
        m_cds_controller->ShowHideOpenedWindows(f_show);
    }

    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

    m_amb_controller.ShowHideOpenedWindows(f_show);

    m_wirelessgating_cntlr.ShowHideOpenedWindows(f_show);
}


#if MPLUS_V3
//+Patni-PJS/2010Apr12/Added/JaFT# MVC007125

//****************************Method Header************************************
//Method Name   :GetStatusMessage
//Author        :PATNI/ASHISH SM3
//Purpose       :Convert the MPLUS_PM_OPERATION_RESPONSE_e status message in a corresponding string message.
//*****************************************************************************

CString CPQMView::GetStatusMessage(MPlus::PM::MPLUS_PM_OPERATION_RESPONSE_e f_res)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetStatusMessage");

    PQM_TRACE(SYSTEM_FUNC_MARKER,
              FUNC_NAME, _T("Trace of GetStatusMessage"));

    CString csStatus(_T(""));	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections

    switch (f_res) {
        case MPlus::PM::MPlusPM_Unknown_Error:
            csStatus = _T("MPlusPM_Unknown_Error");
            break;

        case MPlus::PM::MPlusPM_Success:
            csStatus = _T("MPlusPM_Success");
            break;

        case MPlus::PM::MPlusPM_ResourceAlreadyReleased:
            csStatus = _T("MPlusPM_ResourceAlreadyReleased");
            break;

        case MPlus::PM::MPlusPM_EmptyResourceInfo:
            csStatus = _T("MPlusPM_EmptyResourceInfo");
            break;

        case MPlus::PM::MPlusPM_Error_InvalidProcessID:
            csStatus = _T("MPlusPM_Error_InvalidProcessID");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessGroupIDNotFound:
            csStatus = _T("MPlusPM_Error_ProcessGroupIDNotFound");
            break;

        case MPlus::PM::MPlusPM_Error_InvalidProcessName:
            csStatus = _T("MPlusPM_Error_InvalidProcessName");
            break;

        case MPlus::PM::MPlusPM_Error_InvalidResourceID:
            csStatus = _T("MPlusPM_Error_InvalidResourceID");
            break;

        case MPlus::PM::MPlusPM_Error_InvalidResourceInfo:
            csStatus = _T("MPlusPM_Error_InvalidResourceInfo");
            break;

        case MPlus::PM::MPlusPM_Error_InvalidResourceUpdateType:
            csStatus = _T("MPlusPM_Error_InvalidResourceUpdateType");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessMonitoringStartFailed:
            csStatus = _T("MPlusPM_Error_ProcessMonitoringStartFailed");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessMonitoringStopFailed:
            csStatus = _T("MPlusPM_Error_ProcessMonitoringStopFailed");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessAlreadyRegistered:
            csStatus = _T("MPlusPM_Error_ProcessAlreadyRegistered");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessNotRegistered:
            csStatus = _T("MPlusPM_Error_ProcessNotRegistered");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessGroupTerminateFailed:
            csStatus = _T("MPlusPM_Error_ProcessGroupTerminateFailed");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessGroupStartFailed:
            csStatus = _T("MPlusPM_Error_ProcessGroupStartFailed");
            break;

        case MPlus::PM::MPlusPM_Error_InitializationFailed:
            csStatus = _T("MPlusPM_Error_InitializationFailed");
            break;

        case MPlus::PM::MPlusPM_Error_TerminateFailed:
            csStatus = _T("MPlusPM_Error_TerminateFailed");
            break;

        case MPlus::PM::MPlusPM_Error_RMCtrlDllLoadFailed:
            csStatus = _T("MPlusPM_Error_RMCtrlDllLoadFailed");
            break;

        case MPlus::PM::MPlusPM_Error_RMCtrlDllUnloadFailed:
            csStatus = _T("MPlusPM_Error_RMCtrlDllUnloadFailed");
            break;

        case MPlus::PM::MPlusPM_Error_RMCtrlDllResourceUpdateFailed:
            csStatus = _T("MPlusPM_Error_RMCtrlDllResourceUpdateFailed");
            break;

        case MPlus::PM::MPlusPM_Error_RMCtrlDllResourceReleaseFailed:
            csStatus = _T("MPlusPM_Error_RMCtrlDllResourceReleaseFailed");
            break;

        case MPlus::PM::MPlusPM_Error_PMCannotBeRegistered:
            csStatus = _T("MPlusPM_Error_PMCannotBeRegistered");
            break;

        case MPlus::PM::MPlusPM_Error_ProcessNameMismatch:
            csStatus = _T("MPlusPM_Error_ProcessNameMismatch");
            break;

        case MPlus::PM::MPlusPM_Error_MemoryError:
            csStatus = _T("MPlusPM_Error_MemoryError");
            break;

        case MPlus::PM::MPlusPM_Error_SpecifiedResourceNotRegistered:
            csStatus = _T("MPlusPM_Error_SpecifiedResourceNotRegistered");
            break;

        case MPlus::PM::MPlusPM_ProcessNameMismatch:
            csStatus = _T("MPlusPM_ProcessNameMismatch");
            break;

        default:
            csStatus = _T("Invalid return");
            break;
    }

    return csStatus;

}

//****************************Method Header************************************
//Method Name   :GetProcessNameFromProcessID
//Author        :PATNI/ASHISH SM3
//Purpose       :Get the process name for the process id passed to this function.
//*****************************************************************************

bool CPQMView::GetProcessNameFromProcessID(DWORD f_dwProcessID_i, CString& f_csProcessName_o)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetProcessNameFromProcessID");
    PQM_TRACE_SCOPED(FUNC_NAME);

    HANDLE hProcessHandle = 0;

    try {
        // Get the process name
        hProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                     FALSE,
                                     f_dwProcessID_i);

        if (0 != hProcessHandle) {

            HMODULE hModule = 0;
            DWORD dwNumOfBytesNeeded = 0;

            // Get all the module handle for the specified Process.
            if (EnumProcessModules(hProcessHandle,  &hModule,  sizeof(hModule), &dwNumOfBytesNeeded)) {

                // Get the module name
                LPTSTR lptszActualProcessName = new TCHAR[MAX_PATH];

                if (0 == lptszActualProcessName) {
                    CloseHandle(hProcessHandle);
                    hProcessHandle = 0;
                    return false;
                }

                if (0 == GetModuleBaseName(hProcessHandle, hModule, lptszActualProcessName, MAX_PATH)) {
                    CloseHandle(hProcessHandle);
                    hProcessHandle = 0;

                    DEL_PTR_ARY(lptszActualProcessName)

                    return false;
                }

                f_csProcessName_o = lptszActualProcessName;

                DEL_PTR_ARY(lptszActualProcessName)

                CloseHandle(hProcessHandle);
                hProcessHandle = 0;
                return true;

            } else {
                CloseHandle(hProcessHandle);
                hProcessHandle = 0;
                return false;
            }

        } else {
            return false;
        }

    } catch (...) {

        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Unknown exception occured while getting Process ID."),
                SYSTEM_FUNC_MARKER, _T("CPQMView::GetProcessNameFromProcessID"), __LINE__, true); // Patni-KSS/ADDED/2010May19/IR-82


        if (0 != hProcessHandle) {
            CloseHandle(hProcessHandle);
            hProcessHandle = 0;
        }

        //TMSC/2009Nov12/Modified/Code Changes
        throw;

        return false;
    }
}

//****************************Method Header************************************
//Method Name   :RegisterToPM
//Author        :PATNI/ASHISH SM3
//Purpose       :Registration and Unregistration to Process Manager.
//*****************************************************************************
BOOL CPQMView::RegisterUnregisterProcMgr(
    const BOOL f_reg_proc_mgr
)
//-Patni-PJS/2010Apr12/Added/JaFT# MVC007125
{
    LPCTSTR FUNC_NAME = _T("CPQMView::RegisterUnregisterProcMgr");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Inside RegisterUnregisterProcMgr"));

    //AUTO REGISTER TO PM
    try {
        HRESULT hRes = ::CoInitialize(NULL);

        if (FAILED(hRes)) {
            CPQMLogMgr::GetInstance()->WriteEventLog(_T("COM Initialization failed."),
                    SYSTEM_FUNC_MARKER, FUNC_NAME, __LINE__, true); // Patni-KSS/ADDED/2010May19/IR-82

            //Patni-PJS/2010Apr12/Added/JaFT# MVC007125
            return FALSE;
        }

        MPlus::PM::ProcessManager*  pm = new MPlus::PM::ProcessManager();

        if (NULL == pm) {
            CPQMLogMgr::GetInstance()->WriteEventLog(_T("pm pointer is NULL"),
                    SYSTEM_FUNC_MARKER, FUNC_NAME, __LINE__, true); // Patni-KSS/ADDED/2010May19/IR-82

            //Patni-PJS/2010Apr12/Added/JaFT# MVC007125
            return FALSE;
        }

        CString csProcessName(_T(""));	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections

        DWORD dwProcessID = GetCurrentProcessId();

        if (!GetProcessNameFromProcessID(dwProcessID, csProcessName)) {

            DEL_PTR(pm)

            //Patni-PJS/2010Apr12/Added/JaFT# MVC007125
            return FALSE;
        }

        CString csProcessID(_T(""));	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections
        csProcessID.Format(_T("%d"), dwProcessID);
        //+Patni-NBP/2009Mar03/Added/PSP1#204
        //+Patni-HAR/2010May10/Modified/MVC7125_1
        //CString csProcessGroup((CPQMUtility::GetInstance()->GetPQMParentName()));
        CString csProcessGroup(_T("GENERAL"));
        //-Patni-HAR/2010May10/Modified/MVC7125
        //+Patni-NBP/2009Mar03/Added/PSP1#204
        //+Patni-PJS/2010Apr12/Added/JaFT# MVC007125
        MPlus::PM::MPLUS_PM_OPERATION_RESPONSE_e res;

        LPTSTR response = NULL;

        if (f_reg_proc_mgr == TRUE) {
            res = pm->RegisterProcessInfo(
                      csProcessGroup, dwProcessID, csProcessName, response);

        } else {
            res = pm->ReleaseResourceInfo(
                      csProcessGroup, dwProcessID, csProcessName, response);
        }

        //-Patni-PJS/2010Apr12/Added/JaFT# MVC007125

        if (MPlus::PM::MPlusPM_Success != res) {
            CPQMLogMgr::GetInstance()->WriteEventLog(_T("Unknown Error occurred."),
                    SYSTEM_FUNC_MARKER, FUNC_NAME, __LINE__, true); // Patni-KSS/ADDED/2010May19/IR-82

            //+Patni-PJS/2010Apr12/Added/JaFT# MVC007125
            DEL_PTR(pm)

            return FALSE;
            //-Patni-PJS/2010Apr12/Added/JaFT# MVC007125

        }

        DEL_PTR(pm)

        ::CoUninitialize();

    } catch (...) {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Unknown exception occured during register process."),
                SYSTEM_FUNC_MARKER, FUNC_NAME, __LINE__, true); // Patni-KSS/ADDED/2010May19/IR-82

        //TMSC/2009Nov12/Modified/Code Changes
        throw;
        //Patni-PJS/2010Apr12/Added/JaFT# MVC007125
        return FALSE;
    }

    //Patni-PJS/2010Apr12/Added/JaFT# MVC007125
    return TRUE;
}

#endif

//************************************Method Header************************************
// Method Name  : ApplyDefaultScanAnatomyFor
// Author       : PATNI/
// Purpose      : Applies default scan anatomy settings
//***********************************************************************************
bool CPQMView::ApplyDefaultScanAnatomyFor(

    //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131

    //CPqmPrMode& prmode
    int f_seq_index,
    const bool f_gui_only /*= false*/
)
{

    //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
    if (IsValidSequenceIndex(f_seq_index)) {

        //Patni - ACE1 - SCAN anatomy
        CPqmPrMode& prmode = GetPrModeOfSelectedSequence(f_seq_index);

        //+Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
        //
        //        //+Patni-HEMANT/ADDED On 9/11/2009 12:19:48 PM/ defect found during MVC003979
        //        if (f_gui_only) {
        //
        //            SScanAantomy scan_anatomy_data ;
        //
        //            if (GetScanAnatomyDataFor(prmode.m_anatomy, 0, scan_anatomy_data)) {
        //
        //                prmode.m_str_scan_anatomy = scan_anatomy_data;
        //            }
        //
        //        } else {
        //			ApplyScanAnatomy(&f_seq_index, 1, 0);
        //        }
        //        //-Patni-HEMANT/ADDED On 9/11/2009 12:19:48 PM/ defect found during MVC003979

        SScanAantomy scan_anatomy_data ;

        if (!GetScanAnatomyDataFor(prmode.m_anatomy, 0, scan_anatomy_data)) {
            return false;
        }

        if (f_gui_only) {

            prmode.m_str_scan_anatomy = scan_anatomy_data;

            //+Patni-Hemant/2010Apr30/Added/JaFT/MVC007367
            if (prmode.m_str_scan_anatomy.GetLiteralityFlag()) {
                prmode.m_str_scan_anatomy.SetLaterality(Laterality_None);
            }

            //-Patni-Hemant/2010Apr30/Added/JaFT/MVC007367

        } else {

            ApplyScanAnatomy(&f_seq_index, 1, 0, scan_anatomy_data.GetLiteralityFlag() ? Laterality_None : Laterality_Invalid);
        }

        //-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137

        return true ;
    }

    return false ;
    //-Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131

}

bool CPQMView::ApplyScanAnatomy(
    const int* const seq_items,
    const int seq_count,
    const int f_scananatomy_menu_index,
    const LateralityEnum f_laterality,

    //+Patni-Hemant/2010Apr30/Added/JaFT/MVC007367
    const bool f_check_for_same, /*= true*/
    SScanAantomy* f_scananatomy /*= NULL*/
    //-Patni-Hemant/2010Apr30/Added/JaFT/MVC007367
)
//-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyScanAnatomy");

    if (!seq_items) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("seq_items is NULL "));
        return false ;
    }

    CSequence* first_seq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[0]);

    if (NULL == first_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("first_seq is NULL "));
        return false ;
    }

    SScanAantomy scan_anatomy_data ;

    //+Patni-Hemant/2010Apr30/Added/JaFT/MVC007367
    if (f_scananatomy) {
        scan_anatomy_data = *f_scananatomy;
    }

    //-Patni-Hemant/2010Apr30/Added/JaFT/MVC007367

    //+Patni-Hemant/2010Apr30/Modified/JaFT/MVC007367
    if (first_seq && !f_scananatomy) {
        //-Patni-Hemant/2010Apr30/Modified/JaFT/MVC007367

        CPqmPrMode prmode = first_seq->GetPrMode() ;

        if (!GetScanAnatomyDataFor(prmode.m_anatomy, f_scananatomy_menu_index, scan_anatomy_data)) {

            return false ;
        }
    }

    //+Patni-MEGHANA /2009Mar17/ADDED/ACE-1/ MCM0213-00131
    VARIANT* pData = new VARIANT;
    pData->wReserved1 = BODY_PART;
    BSTR l_bodypart_bstr = NULL;//Patni-DKH/2009Aug24/Modified/cpp test corrections
    CString l_BodyPartName = scan_anatomy_data.m_scan_anatomy_name;
    l_bodypart_bstr = l_BodyPartName.AllocSysString();
    pData->byref = l_bodypart_bstr;
    //-Patni-MEGHANA /2009Mar17/ADDED/ACE-1/ MCM0213-00131

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    VARIANT* pData_2 = new VARIANT;
    pData_2->wReserved1 = LATERALITY;
    pData_2->intVal = (int)f_laterality;
    scan_anatomy_data.SetLaterality(f_laterality);
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
    int l_first_item_anatomy_value = -1;
    int l_scan_index = -1;

    for (int index = 0; index < seq_count; index++) {

        CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[index]);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL "));
            continue;
        }

        //REDMINE-781
        CPqmPrMode& prmode = pseq->GetPrMode() ;

        //+Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-MVC007915
        if (l_first_item_anatomy_value == -1) {

            l_first_item_anatomy_value = prmode.m_anatomy;
        }

        //-Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-MVC007915

        //+Patni-Hemant/2010Apr30/Modified/JaFT/MVC007367
        //if (prmode.m_str_scan_anatomy == scan_anatomy_data)
        if (f_check_for_same && (prmode.m_str_scan_anatomy == scan_anatomy_data))
            //-Patni-Hemant/2010Apr30/Modified/JaFT/MVC007367
            continue ;		//if anatomy is already there, don't apply ..


        //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
        //according to Vantage, if next item is of different sar return
        if ((l_first_item_anatomy_value != -1) &&
            (l_first_item_anatomy_value != prmode.m_anatomy)) {

            //+Patni-Hemant/2010May7/Modified/JaFT/IR-137/TMSC-REDMINE-265
            //            //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
            //            ::SysFreeString(l_bodypart_bstr);
            //            l_bodypart_bstr = NULL;
            //
            //            if (pData) {
            //                delete pData;
            //                pData = NULL;
            //            }
            //
            //            //-Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
            //            return true;

            break ;
            //-Patni-Hemant/2010May7/Modified/JaFT/IR-137/TMSC-REDMINE-265
        }

        //+Patni-Hemant/2010May27/Commented/MaFT/TMSC-REDMINE-MVC007915
        //        if (l_first_item_anatomy_value == -1) {
        //
        //            l_first_item_anatomy_value = prmode.m_anatomy;
        //        }
        //-Patni-Hemant/2010May27/Commented/MaFT/TMSC-REDMINE-MVC007915

        //
        //-Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
        //+Patni-MEGHANA /2009Mar17/ADDED/ACE-1/ MCM0213-00131
        l_scan_index = GetAcqOderForGivenProtocolIndex(seq_items[index]);


        //+Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
        //if (S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pData, l_scan_index)) {
        if ((S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pData, l_scan_index))
            && (S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pData_2, l_scan_index))) {
            //-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137

            prmode.m_str_scan_anatomy =  scan_anatomy_data;

            //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
            prmode.m_str_scan_anatomy.SetLaterality(f_laterality);
            //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

            //REDMINE-781
            //SetPrModeOfSelectedSequence(seq_items[index], prmode);

        } else {

            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("Trace of ApplyScanAnatomy"));

        }

        //-Patni-MEGHANA /2009Mar17/ADDED/ACE-1/ MCM0213-00131
    }

    //+Patni-MEGHANA /2009Mar17/ADDED/ACE-1/ MCM0213-00131
    //Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    ::SysFreeString(l_bodypart_bstr);
    l_bodypart_bstr = NULL;

    if (pData) {

        pData->byref = NULL ;
        DEL_PTR(pData)
    }

    //-Patni-MEGHANA /2009Mar17/ADDED/ACE-1/ MCM0213-00131

    //Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    DEL_PTR(pData_2)

    //+Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
    //m_scan_anatomy_button.SetWindowText(scan_anatomy_data.m_scan_anatomy_name);
    EnableDisableScanAnatomy();
    //-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137

    return true ;
}
//************************************Method Header************************************
// Method Name  : AMBAddShimMap
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::AMBAddShimMap()
{
    LPCTSTR FUNC_NAME  = _T("CPQMView::AMBAddShimMap") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    VARIANT pData;
    GetAcquisition()->GetAMBShimMapArray(&pData);

    AMB_shim_map_struct_t* l_amb_shim_map_list = (AMB_shim_map_struct_t*)pData.byref;

    //Shim
    SequenceList* l_shim_list = (SequenceList*)l_amb_shim_map_list->m_seq_list;
    int l_auto_insert_position = -1;

    POSITION l_position ;
    POSITION l_gui_lst_position ;

    if (l_amb_shim_map_list->m_list_count > 0)
        l_position = l_shim_list->GetHeadPosition();

    for (int i = 0 ; i < l_amb_shim_map_list->m_list_count ; i++) {

        CPASCOMSequence* l_pascomseq = l_shim_list->GetNext(l_position) ;
        CSequence* l_pseq = new CSequence(l_pascomseq);

        ApplyConfigSettingForProt(l_pseq);

        l_auto_insert_position  = l_amb_shim_map_list->m_seq_pos[i] - 1;

        //Inserting protocol on data list
        if (l_auto_insert_position >= m_list->GetCount()) {
            l_gui_lst_position = m_list->FindIndex(m_list->GetCount() - 1);
            m_list->InsertAfter(l_gui_lst_position, l_pseq);

        } else {
            l_gui_lst_position = m_list->FindIndex(l_auto_insert_position);
            m_list->InsertBefore(l_gui_lst_position, l_pseq);
        }

        //Inserting protocol on list ctrl
        m_pqm_listctrl.InsertProtocolAt(l_pseq, l_auto_insert_position, true);

        //applying auto scan to Target Or Auto Insert protocols
        g_MainView->ApplyAutoScanToItems(&l_auto_insert_position, 1, true);

        UpdateCoilDataInSeq(l_pseq);
        DisplayTotalRemainingTimeOnPQM();
    }
}
//************************************Method Header************************************
// Method Name  : AMBUpdateSlaveprotocols
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::AMBUpdateSlaveprotocols()
{
    LPCTSTR FUNC_NAME  = _T("CPQMView::AMBUpdateSlaveprotocols") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    VARIANT pData;
    GetAcquisition()->GetAMBMasterProtocols(&pData);

    ambSlave_t* l_slave_list = (ambSlave_t*)pData.byref;

    VARIANT l_pdata ;

    CString l_scan_id ;
    l_pdata.wReserved1 = SCAN_ID;
    l_pdata.byref = &l_scan_id;

    int l_protocol_index  = -1;
    CSequence* pseq = NULL ;

    CString l_log_str;

    for (int i = 0 ; i < l_slave_list->m_count ; i++) {

        m_acquisition->GetControlValueFromDB(&l_pdata, l_slave_list->m_duplicated_slave_acqorders[i]);
        l_protocol_index = GetProtocolIndexFromAcqorder(l_slave_list->m_duplicated_slave_acqorders[i]);

        l_log_str.Format(_T("Slave Acqorder=%d, Slave Index=%d"), l_slave_list->m_duplicated_slave_acqorders[i], l_protocol_index);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , l_log_str);

        if (IsValidSequenceIndex(l_protocol_index)) {

            pseq = (CSequence*)m_pqm_listctrl.GetItemData(l_protocol_index);

            if (NULL == pseq) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL "));
                continue;
            }

            pseq->GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("COMM"), l_scan_id);
            pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("AMB-MODE-FLAG"), VFC_AMB_MODE_SLAVE);
            pseq->GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("DELAYTIME"), _T(""));

            m_pqm_listctrl.UpdateSequences(&l_protocol_index, 1);

        }
    }
}

void CPQMView::WSEEditingCompleted(
    const WSE_Editing_Operation f_wse_editing_operation
)
{
    LPCTSTR FUNC_NAME  = _T("CPQMView::WSEEditingCompleted") ;
    PQM_TRACE_SCOPED(FUNC_NAME);
    //End Of addition By Hemant On2/9/2009 7:00:03 PM

    const int tepm_wse_editing_protocol_index = m_pqm_listctrl.GetWSEEditingProtocolIndex() ;
    //const int tepm_wse_editing_protocol_index = m_pqm_listctrl.GetLastPlannedProtocol();

    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
    CString l_str = _T("");
    l_str.Format(_T("WSE Editing Completed for index: %d"), tepm_wse_editing_protocol_index);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , l_str);
    //Note:new editing can start after editing is completed for current protocol.
    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2


    if (tepm_wse_editing_protocol_index != -1) {

        CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(tepm_wse_editing_protocol_index);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL"));
            return;
        }

        const int l_acq_order = GetAcqOderForGivenProtocolIndex(tepm_wse_editing_protocol_index);

        if (CanSaveProtocol(f_wse_editing_operation)) {
            pseq->SetWSEEditingEnum(pseq->GetWSEEditingEnum(), true);
            UpdateCompleteSequenceFromVF(tepm_wse_editing_protocol_index);

            DisplayTotalRemainingTimeOnPQM();

            //added by Hemant - Changing the content of all Open dialogs after WSE editing completion
            ChangeContentForModelessInfoDialogs();
            //
            //End Of addition By Hemant On2/9/2009 7:05:15 PM
            ///////////////////////////

            //+Patni-Hemant/2010May21/Commented/MaFT/MVC007927
            m_pqm_listctrl.SetLastPlannedProtocol(tepm_wse_editing_protocol_index);
            //-Patni-Hemant/2010May21/Commented/MaFT/MVC007927

            if (m_is_amb_planning) {
                pseq->SetAMBEditingEnum(AMB_PLANNED);
            }

            m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(tepm_wse_editing_protocol_index);
        }

        //Added by Hemant On 2/9/2009 7:05:22 PM
        if (CanGUISaveProtocol(f_wse_editing_operation)) {

            VARIANT* pData = new VARIANT;

            //MRPdefect615
            pData->wReserved1 = PLAN_STATUS;
            m_acquisition->GetControlValueFromDB(pData, l_acq_order);
            int l_pln_status = pData->intVal;

            //if(((l_pln_status & VFC_PLN_STATUS_GLOC) == VFC_PLN_STATUS_GLOC ) && ((l_pln_status & VFC_PLN_STATUS_PROEDIT) == VFC_PLN_STATUS_PROEDIT )){
            if (((l_pln_status & VFC_PLN_STATUS_GLOC) == VFC_PLN_STATUS_GLOC)) {
                m_pqm_listctrl.MakeCurrentWSEEditingProtocolToPlanned();

            }

            //MRPdefect615

            //make current as planned and then reset current..
            //Modified by Hemant On 2/4/2009 11:43:34 AM , Defect #870, 871
            //m_pqm_listctrl.SetNextWSEEditingProtocolIndex(-1);
            m_pqm_listctrl.ResetWSEEditingProtocolIndex();

            //Added by Hemant On 2/9/2009 7:05:22 PM, refreshing WSE editing column
            m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(tepm_wse_editing_protocol_index);

            DEL_PTR(pData)
            ////added by Hemant - Changing the content of all Open dialogs after WSE editing completion
            //ChangeContentForModelessInfoDialogs();
        }

        //Added by Hemant On 2/9/2009 7:08:25 PM , moved from IFs..
    }

}

void CPQMView::UpdateCompleteSequenceFromVF(
    const int f_list_index
)
{

    LPCTSTR FUNC_NAME  = _T("CPQMView::UpdateCompleteSequenceFromVF") ;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("Trace"));

    if (!IsValidSequenceIndex(f_list_index))
        return;

    const int l_acq_order = GetAcqOderForGivenProtocolIndex(f_list_index);
    CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(f_list_index);

    if (NULL == pseq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL"));
        return;
    }

    VARIANT pData;
    pData.wReserved1 = HOLD_VALUE;
    m_acquisition->GetControlValueFromDB(&pData, l_acq_order);
    pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("HOLD"), pData.intVal);
    pseq->SetCanChangeModeOptionBreathHold(!(pData.wReserved2));

    pData.wReserved1 = SCAN_TIME;
    m_acquisition->GetControlValueFromDB(&pData, l_acq_order);
    pseq->GetCPASCOMSequence()->GetSequenceParam()->SetFloat(_T("SCANTIME") , (float)pData.llVal);

    GetSequenceInformation(f_list_index);

    if (GetIsAMBPlanning()) {
        if (IsProtocolSelectedForAMBPlan(f_list_index) && (m_pqm_listctrl.GetFirstWaitProtocolIndex() == f_list_index)) {
            MakeAutoScanOffForIfNecessary(f_list_index);
        }
    }

    int selected_items[] = {f_list_index};
    m_pqm_listctrl.UpdateSequences(selected_items, 1);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanSaveProtocol(
    const CPQMView::WSE_Editing_Operation f_wse_editing_operation
)const
{

    switch (f_wse_editing_operation) {

        case CPQMView::WSE_Editing_Operation_SaveOnly :
        case CPQMView::WSE_Editing_Operation_GUISave_SaveOnly:
            return true ;
    }

    return false ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::CanGUISaveProtocol(
    const CPQMView::WSE_Editing_Operation f_wse_editing_operation
)const
{

    switch (f_wse_editing_operation) {

        case CPQMView::WSE_Editing_Operation_GUISave :
        case CPQMView::WSE_Editing_Operation_GUISave_SaveOnly:
            return true ;
    }

    return false ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ProcessAllowOrInhibitSelectionForProconButtons(
    const bool f_process_allow_selection,
    const bool process_hour_glass /*= true*/
)
{
    //+Patni-Sribanta/2010Sep15/Added/MVC008802
    LPCTSTR FUNC_NAME = _T("CPQMView::ProcessAllowOrInhibitSelectionForProconButtons");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    //+Patni-Sribanta/2010Sep15/Added/MVC008802

    const bool is_inhibit_message = !f_process_allow_selection ;

    //+Patni-HEMANT/ADDED On 27/04/2009 6:52:10 PM/ PSP#269
    //m_procon_ui_status = (is_inhibit_message ? PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS : PQM_PROCONALLOW_SELECTIONS_PROCON_BUTTONS) ;
    m_procon_ui_status |= (is_inhibit_message ? PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS : PQM_PROCONALLOW_SELECTIONS_PROCON_BUTTONS) ;
    m_procon_ui_status &= ~(is_inhibit_message ? PQM_PROCONALLOW_SELECTIONS_PROCON_BUTTONS : PQM_INHIBIT_SELECTIONS_PROCON_BUTTONS);
    //-Patni-HEMANT/ADDED On 27/04/2009 6:52:10 PM/ PSP#269

    //+Patni-Sribanta/2010Sep15/Added/MVC008802
    CString l_log_str(_T(""));
    l_log_str.Format(_T("m_procon_ui_status: %d "), m_procon_ui_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    //+Patni-Sribanta/2010Sep15/Added/MVC008802

    if (process_hour_glass) {
        DisplayHourglassOnProconRelatedButtons(is_inhibit_message);
    }

    //enable disable other queue dialog items..
    EnableDisableQueueDialogButtons();
}
//-Patni-HEMANT/ADDED On 3/10/2009 7:01:11 PM/ PSP#232

//+Patni-HEMANT/ADDED On 3/10/2009 7:01:11 PM/ PSP#232
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::DisplayHourglassOnProconRelatedButtons(
    const bool hour_glass_show
)
{
    //+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
    if (m_queuedialog) {

        m_queuedialog->SetWaitCursor(hour_glass_show);

        //Patni-HEMANT/2010Nov08/Modified/TMSC-REDMINE-936(QA)
        SendMessage(WM_SETCURSOR, 0, 0);
    }

    //-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
}

//************************************Method Header************************************
// Method Name  : OnAutoScanStart
// Author       : PATNI/
// Purpose      : To scan start
//***********************************************************************************
void CPQMView::OnAutoScanStart()
{
    const int l_first_wait = m_pqm_listctrl.GetFirstWaitProtocolIndex();

    if (!IsScanning() && IsValidSequenceIndex(l_first_wait) && !IsProtocolSelectedForAMBPlan(l_first_wait)) {
        if (m_amb_controller.IsAMBInvoked())
            m_amb_controller.RecAmbScanStartMsg();

        else
            PQMMessageScanStart();

    } else {
        RemoevAutoScanFromFirstWaitItem();
    }
}

void CPQMView::ResetAMBPlanProtocols()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ResetAMBPlanProtocols");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_amb_controller.SetIswaitforAMBDlgOpen(false);
    SetIsAMBPlanning(false);
    int l_size = 	m_selected_acqoder_list_for_wse.GetSize();

    for (int i = 0 ; i < l_size ; i++) {
        int l_list_index  =  GetListForGivenProtocolAcqOder(m_selected_acqoder_list_for_wse.GetAt(i));
        CSequence* l_seq = (CSequence*)GetPQMListCtrl().GetItemData(l_list_index);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("l_seq is NULL"));
            continue;
        }

        l_seq->SetAMBEditingEnum(AMB_NONE);
        m_pqm_listctrl.InvalidateItemsForWSEEditingColumn(l_list_index);
    }
}


//************************************Method Header************************************
// Method Name  : OnAMBAutoScanStartforFirstSlaveProtocol
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::OnAMBAutoScanStartforFirstSlaveProtocol()
{
    int FirstWaitProtocol = -1;
    GetIndexOfFirstWaitStateProtocol(FirstWaitProtocol);

    int l_count =   0;
    m_acquisition->GetAMBSlaveCount(&l_count);

    if (TYPE_FALSE == GetGUIListProtocolInfo(FirstWaitProtocol + 1 , FirstWaitProtocol + l_count)) {
        SetMismatchErrorFlag(TRUE);
        return ;
    }

    OnAutoScanStart();
}



void CPQMView::PQMMessageMapDelete()const
{
    m_pqmchildthread.PostMessageToThread(ON_DELETE, NULL, NULL);
}
//************************************Method Header************************************
// Method Name  : CheckAMBMenuClickConditions
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMView::CheckAMBMenuClickConditions()
{
    int* l_selected_items = NULL ;
    BOOL l_return = FALSE;
    int l_size = m_pqm_listctrl.GetselectedItems(&l_selected_items);
    m_acquisition->CheckAMBMenuClickConditions(l_selected_items, l_size, &l_return);
    DEL_PTR_ARY(l_selected_items);
    return l_return;

}
//************************************Method Header************************************
// Method Name  : OnAMB
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::OnAMB()
{
    if (CheckAMBMenuClickConditions()) {
        SetIsAMBPlanning(true);
        m_amb_controller.SetIswaitforAMBDlgOpen(false);
        int* selected_items = NULL ;
        int seq_count = m_pqm_listctrl.GetselectedItems(&selected_items);

        if (!selected_items || !seq_count) {
            return ;
        }

        UpdateMoveCouchMethodFor(selected_items, seq_count, PARENTREFPOS);
        OnScanEditOrPlan(PQM_PROCON_SCAN_PLAN);
    }

}
//************************************Method Header************************************
// Method Name  : OnAMB
// Author       : iGATE-Adv Cardiac
// Purpose      :
//***********************************************************************************
void CPQMView::OnTiPrep()
{
    if (1) { /* any condition in which tiprep should not be performed needs to be checked here*/
       // SetIsAMBPlanning(true);
        //m_amb_controller.SetIswaitforAMBDlgOpen(false);
        int* selected_items = NULL ;
        int seq_count = m_pqm_listctrl.GetselectedItems(&selected_items);

        if (!selected_items || !seq_count) {
            return ;
        }
		if (m_tiprep_app_cntl.CanTiPrepInvoke())
		{
			m_tiprep_app_cntl.InvokeTiPrep(TRUE);
		}

    }

}

//************************************Method Header************************************
// Method Name  : PQMMessageMapEndExam
// Author       : PATNI/ Raghavendra K M
// Purpose      :
//***********************************************************************************
void CPQMView::PQMMessageMapEndExam()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::PQMMessageMapEndExam");
    PQM_TRACE_SCOPED(FUNC_NAME);

    DestroySubDilaogsOnPQMHide();
    m_pqmchildthread.PostMessageToThread(ON_END_EXAM, NULL, NULL);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickAutoVoiceMenu(
    const UINT auto_voice_menuitem_id
)
{
    ApplyAutoVoice(m_pqm_listctrl.GetRightClickCell().y, auto_voice_menuitem_id);
}

//************************************Method Header**************************************
//Method Name:  OnClickButtonScanPause
//Author:  PATNI/SVP
//Purpose: Pauses scan.
//******************************************************************************
void CPQMView::OnClickButtonScanPause(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickButtonScanPause");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!m_pause_resume_button.IsWindowEnabled()) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Pause-Resume is disabled"));
        return ;
    }

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_PAUSE);

    m_acquisition->PauseScan();

    m_current_scan_opration = CurrentScanOpration_ScanPause;
}

//+Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
//*****************************************************************************
//Method Name   : AbortScan
//Author        : Patni/PJS
//Purpose       : This function execute Abort functionality
//*****************************************************************************
void CPQMView::AbortScan(
)
{

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_ABORT);

    if ((m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS) &&
        (dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd) && dlgCoilSelWin->IsWindowVisible())) {
        return ;
    }



    //////////////////////////////////////////////////////////////////////////

    if (m_amb_controller.IsAMBInvoked()) {
        m_acquisition->AMBScanAbort();
    }

    else
        m_acquisition->AbortScan();


    if (m_pause_resume_button.IsWindowEnabled()) {
        m_pause_resume_button.EnableWindow(false);
    }

    m_abort_button.EnableWindow(false);
    SetAbortFlag(true);

    UpdateOKBtnStatusofWirelssDlg();
    UpdateOtherButtonState();
}
//*****************************************************************************
//Method Name   : AMBSlaveScanStart
//Author        : iGATE
//Purpose       :
//*****************************************************************************
void CPQMView::AMBSlaveScanStart()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::AMBSlaveScanStart");
    PQM_TRACE_SCOPED(FUNC_NAME);

    try {

        m_delete_button.EnableWindow(FALSE);

        int FirstWaitProtocol = -1;
        GetIndexOfFirstWaitStateProtocol(FirstWaitProtocol);

        CString l_log_str ;
        l_log_str.Format(_T("First Wait Protocol Index for Scan Start : %d"), FirstWaitProtocol);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        if (!IsValidSequenceIndex(FirstWaitProtocol)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));

            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return ;
        }

        CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(FirstWaitProtocol);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return ;
        }

        //Delete operation will continue after sending ACQ_INFO.

        CScopedLock l_scoped_lock(&m_sync_scan_and_delete_cs);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering m_sync_scan_and_delete_cs"));

        if (CT_DELETE == l_seq->GetChildThreadStatus()) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("CT_DELETE flag is on, can not scan start..Leaving m_sync_scan_and_delete_cs"));

            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return;
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Leaving m_sync_scan_and_delete_cs"));

        VARIANT data;
        data.byref = NULL ;

        if (S_OK != m_acquisition->GetScanMutex(&data)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));
            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return;
        }

        int l_proc_count = 0;

        if (S_OK == m_acquisition->GetRunningProcCount(&l_proc_count) && l_proc_count) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Running Scan Procedure count is NOT zero"));
            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return;
        }

        CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_START);

        if (GetMismatchErrorFlag() && !m_dynamic_scan_on) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Scan cannot start because Mismatch Error flag is TRUE"));

            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPQMUtility::GetMultiLingualString(_T("IDS_PQM_LIST_STUDY_PROTOCOL_MISMATCH")),
                _T("PQM"), MPlus::EH::Information, MPlus::EH::SysModal, MPlus::EH::Ok);

            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return;
        }

        m_bpost_process_flag = FALSE;

        m_flag_abort = false;
        m_scan_start_button.EnableWindow(FALSE);

        SetIconForButton(m_delete_button, -1);
        m_delete_button.EnableWindow(FALSE);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delete Button Disabled"));

        const CPqmPrMode& l_pqmprmode = GetPrModeOfSelectedSequence(FirstWaitProtocol);

        m_acquisition->SetReceiverCoilID(l_pqmprmode.m_rcv_coil_id);


        if (m_dynamic_scan_on) {
            m_dynamic_scan_on = false ;
        }

        m_is_scanning = true;
        SetScanIndex(FirstWaitProtocol);
        m_scan_lock_acquired = false;

        if (m_curr_seq == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_curr_seq is NULL. Exception..."));
            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return ;
        }

        *m_curr_seq = *l_seq;
        const int l_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER"));

        CString l_strScanID = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("COMM"));

        CString l_series_loid(_T(""));
        l_seq->GetCPASCOMSequence()->GetSequenceID(l_series_loid);

        const int l_acq_status = l_seq->GetAcqStatus();
#if 1
        ProtocoInfo l_protocol_info(l_series_loid, l_acq_order, m_scan_index + 1, l_strScanID, l_acq_status);
#else
        ProtocoInfo l_protocol_info;

        l_protocol_info.series_loid    = l_seried_loid;
        l_protocol_info.acq_order      = l_acq_order;
        l_protocol_info.pqm_list_index = m_scan_index + 1;
        l_protocol_info.scan_id        = l_strScanID;
        l_protocol_info.acq_status	   = l_acq_status;
#endif


        m_pqm_listctrl.UpdateProtocolStatus(GetScanIndex(), PR_ACQ_STATUS_CURRENT); //Set slave protocol status current.
        DisplayTotalRemainingTimeOnPQM();

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION Occured..."));
        SetMismatchErrorFlag((TYPE_FALSE == GetGUIListProtocolInfo()) ? TRUE : FALSE);

        throw;
    }
}
//*****************************************************************************
//Method Name   : ScanStart
//Author        : Patni/PJS
//Purpose       : This function execute scan functionality
//*****************************************************************************
void CPQMView::ScanStart(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ScanStart");
    PQM_TRACE_SCOPED(FUNC_NAME);

    try {
        m_delete_button.EnableWindow(FALSE);

        int FirstWaitProtocol = -1;
        GetIndexOfFirstWaitStateProtocol(FirstWaitProtocol);

        CString l_log_str ;
        l_log_str.Format(_T("First Wait Protocol Index for Scan Start : %d"), FirstWaitProtocol);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        if (!IsValidSequenceIndex(FirstWaitProtocol)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));

            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return ;
        }

        CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(FirstWaitProtocol);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return ;
        }

        if (!IsAcqumanReadyForNextRequest()) {

            m_scan_start_button.EnableWindow(false);
            m_coil_button.EnableWindow(false);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("AcqMan is not ready for next request"));

            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return ;
        }


        //Delete operation will continue after sending ACQ_INFO.

        CScopedLock l_scoped_lock(&m_sync_scan_and_delete_cs);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering m_sync_scan_and_delete_cs"));

        if (CT_DELETE == l_seq->GetChildThreadStatus()) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("CT_DELETE flag is on, can not scan start..Leaving m_sync_scan_and_delete_cs"));

            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return;
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Leaving m_sync_scan_and_delete_cs"));



        VARIANT data;
        data.byref = NULL ;

        if (S_OK != m_acquisition->GetScanMutex(&data)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WaitForScanMutex Failed"));
            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return;
        }


        MR::ACQ::SM::COMMON::MUTEX::CThreadMutex* l_scan_mutex = (MR::ACQ::SM::COMMON::MUTEX::CThreadMutex*) data.byref;
        data.byref = NULL ;

        CScopedLock l_scoped_lock_scan(l_scan_mutex);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

        if (PR_ACQ_STATUS_FAIL == l_seq->GetAcqStatus()) {
            m_scan_lock_acquired = false;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trying to scan aborted protocol"));
            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);
            return;
        }

        if (!m_dynamic_scan_on && IsScanning()) {

            m_scan_lock_acquired = false;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan is Already Running"));

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return;
        }

        int l_proc_count = 0;

        if (S_OK == m_acquisition->GetRunningProcCount(&l_proc_count) && l_proc_count) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Running Scan Procedure count is NOT zero"));
            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            if (m_amb_controller.IsAMBInvoked()) {
                m_amb_controller.AmbScanStateSetting();
            }

            return;
        }

        CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_START);

        if (GetMismatchErrorFlag() && !m_dynamic_scan_on) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Scan cannot start because Mismatch Error flag is TRUE"));

            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPQMUtility::GetMultiLingualString(_T("IDS_PQM_LIST_STUDY_PROTOCOL_MISMATCH")),
                _T("PQM"), MPlus::EH::Information, MPlus::EH::SysModal, MPlus::EH::Ok);

            m_scan_lock_acquired = false;

            RemoevAutoScanFromFirstWaitItem();
            SetWaitCursor(false);

            return;
        }

        m_bpost_process_flag = FALSE;

        m_flag_abort = false;
        m_scan_start_button.EnableWindow(FALSE);

        SetIconForButton(m_delete_button, -1);
        m_delete_button.EnableWindow(FALSE);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delete Button Disabled"));

        const CPqmPrMode& l_pqmprmode = GetPrModeOfSelectedSequence(FirstWaitProtocol);

        m_acquisition->SetReceiverCoilID(l_pqmprmode.m_rcv_coil_id);


        if (m_dynamic_scan_on) {
            m_dynamic_scan_on = false ;
        }

        m_is_scanning = true;
        SetScanIndex(FirstWaitProtocol);
        m_scan_lock_acquired = false;

        if (m_curr_seq == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_curr_seq is NULL. Exception..."));
            m_scan_lock_acquired = false;
            SetWaitCursor(false);
            return ;
        }

        *m_curr_seq = *l_seq;
        const int l_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER"));

        CString l_strScanID = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("COMM"));

        CString l_series_loid(_T(""));
        l_seq->GetCPASCOMSequence()->GetSequenceID(l_series_loid);

        const int l_acq_status = l_seq->GetAcqStatus();
#if 1
        ProtocoInfo l_protocol_info(l_series_loid, l_acq_order, m_scan_index + 1, l_strScanID, l_acq_status);
#else
        ProtocoInfo l_protocol_info;

        l_protocol_info.series_loid    = l_seried_loid;
        l_protocol_info.acq_order      = l_acq_order;
        l_protocol_info.pqm_list_index = m_scan_index + 1;
        l_protocol_info.scan_id        = l_strScanID;
        l_protocol_info.acq_status	   = l_acq_status;
#endif

        UpdateGUIOnScanStart();

        VARIANT* pData = new VARIANT;
        pData->byref = &l_protocol_info;
        m_acquisition->StartScan(pData, m_scan_index + 1);

        if (pData) {
            pData->byref = NULL;
            DEL_PTR(pData)
        }


    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION Occured..."));
        SetMismatchErrorFlag((TYPE_FALSE == GetGUIListProtocolInfo()) ? TRUE : FALSE);

        throw;
    }
}
//-Patni-PJS/2010Apr28/Added/JaFT# Redmine-90


//************************************Method Header****************************
// Method Name  : GetShowCoilList()
// Author       : PATNI /
// Purpose      :
//*****************************************************************************
void CPQMView::EnableDisableStartupButtons(
    bool l_enable
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::EnableDisableStartupButtons");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == GetDlgItem(IDC_UTILITY_BUTTON)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetDlgItem(IDC_UTILITY_BUTTON) pointer is NULL"));
        return;

    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //Patni-HAR/2010Jan18/Added/DeFT#MVC006380
    SetIconForButton(m_utility_button, IDI_UTILITY_BUTTON_IMAGE);
    m_utility_button.EnableWindow(l_enable);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == GetDlgItem(IDC_PASS_BUTTON)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetDlgItem(IDC_PASS_BUTTON) pointer is NULL"));
        return;

    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //Patni-HAR/2010Jan18/Modified/DeFT#MVC006380
    if (IsAMB())
        l_enable = false;

    SetIconForButton(m_pass_button, l_enable ? IDI_PASS_BUTTON_IMAGE : -1);
    m_pass_button.EnableWindow(l_enable);
}

/****************************Method Header************************************
//Module         : InitializeIPC
//Author         : PATNI/MSN
//Purpose        : Initializes IPC com
//*****************************************************************************/
//+Patni-ARD/2010Mar29/Modified/Ph-3# IR-100 remove ICPConfig.ini
bool CPQMView::InitializeIPC(const int f_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::InitializeIPC");

    //+Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
    SIPCConfigInfo* l_sIPCConfigInfo = new SIPCConfigInfo();
    bool l_is_read_config_success = ReadConfigInfo(l_sIPCConfigInfo);

    if (!l_is_read_config_success) {

        CString temp_string =  CPQMUtility::GetMultiLingualString(_T("IDS_CONFIG_ERROR"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(temp_string, MPlus::EH::Crucial, APP_NAME, APP_NAME);
        CPQMLogMgr::GetInstance()->WriteEventLog(temp_string, ERROR_LOG_CRUCIAL1, _T("PQMView.cpp"), __LINE__);
        return false;
    }

    VARIANT* f_config_data  = new VARIANT;
    f_config_data->byref    = l_sIPCConfigInfo;

    if (!InitialisePqmCOM(f_mode, f_config_data)) {
        //+Patni-DKH/2009Aug24/Modified/cpp test corrections
        if (NULL == m_connection_point) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("m_connection_point pointer is NULL"));

            CoUninitialize();

            if (NULL != l_sIPCConfigInfo) {
                if (f_config_data) {
                    f_config_data->byref = NULL ;
                    DEL_PTR(f_config_data)
                }

                DEL_PTR(l_sIPCConfigInfo)
            }

            return false;
        }

        //-Patni-DKH/2009Aug24/Modified/cpp test corrections

        m_result = m_connection_point->Unadvise(dwAdvise);

        if (FAILED(S_FALSE)) {

            CString string = _T("");
            string.LoadString(IDS_ERR_DLLFAILED);

            CPQMLogMgr::GetInstance()->DisplayError(CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DLLFAILED")),
                                                    _T("PQMView.cpp"),
                                                    __LINE__, ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);

            CPQMLogMgr::GetInstance()->WriteEventLog(string,
                    ERROR_LOG_WARNING1  , _T("PQMView.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        }

        CoUninitialize();

        if (NULL != l_sIPCConfigInfo) {
            if (f_config_data) {
                f_config_data->byref = NULL ;
                DEL_PTR(f_config_data)
            }

            DEL_PTR(l_sIPCConfigInfo)
        }

        return false;
    }

    if (NULL != l_sIPCConfigInfo) {

        //IPC has updated few params (i.e. Lable size) required for COIL.
        m_selection_utiliy->SetCSConfigParams(l_sIPCConfigInfo->m_coilselconfigparams);

        GetContrastAgentName(l_sIPCConfigInfo);

        //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
        if (m_cds_controller) {
            m_cds_controller->InitializeCDSParameters(l_sIPCConfigInfo);
        }

        //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement

        if (f_config_data) {
            f_config_data->byref = NULL ;
            DEL_PTR(f_config_data)
        }

        DEL_PTR(l_sIPCConfigInfo)
    }

    //-Patni-ARD/2010Mar29/Modified/Ph-3# IR-100

    //This function will retrive
    //the sequence parameter info from IPC.
    GetSequenceParamList();

    GetScanAnatomyData();

    //Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
    GetPresetItemData();

    //Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    GetScanCommentXMLData();

    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    //+Patni-Hemant/2011Apr20/Modified/TMSC-REDMINE-1671
    m_sft_menuhandler->GenerateSFTMenuFromXMLData();
    m_sft_menuhandler->GenerateToolMenuFromXMLData();
    //-Patni-Hemant/2011Apr20/Modified/TMSC-REDMINE-1671
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

    //Added by Hemant, set the global flags and call the IPC related fucntions..
    SetDefaultSettings();

    m_sar_control_license = GetSARControlLicenseAtStartUp();

    return true;
    //-Patni-ARD/2010Mar29/Modified/Ph-3# IR-100 remove ICPConfig.ini
}


/****************************Method Header************************************
//Module         : SendNotifyToSR
//Author         : PATNI/SG
//Purpose        : Send close PQM notification message to SR
//*****************************************************************************/
void CPQMView::SendNotifyToSR()const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SendNotifyToSR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (K_IMAGING != m_pqm_g.GetPQMMode()) {
        return;
    }

    CDPSManager::GetInstance()->SendASyncMsg(_T("Schedule.SRGL"),
            DPS_MSGID_REQ_CLOSEPQM, DPS_PAGENAME_ACQUIRE);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DPS_MSGID_REQ_CLOSEPQM);

}

//************************************Method Header**************************************
//Method Name:  InitialisePqmCOM
//Author:  PATNI/HAR
//Purpose: Initialise PQM COM
//*************************************************************************************
//Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
bool CPQMView::InitialisePqmCOM(
    const int f_mode, VARIANT* f_config_data
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::InitialisePqmCOM");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
    if (m_acquisition->Initialize(f_mode, f_config_data) == S_FALSE) {
        return false;

    } else {

        VARIANT* pData = new VARIANT;
        m_acquisition->GetCoilSelectionData(pData, -1);
        m_head = (DllT*)pData->byref;

        //Divide master coillist (m_head) into its sub componenets( G, L A port)
        CreateCoilListsForSelectionCoil();

        //+Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK
        pData->byref = NULL;
        DEL_PTR(pData)
        //-Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK

        return true;
    }
}

//****************************Method Header************************************
//Module         : ChangeOrder()
//Author         : PATNI/PJP
//Purpose        : Send series LOID to IPC through COM
//*****************************************************************************
void CPQMView::ChangeOrder(
    const CString& seqorder,
    const int dropindex
)const
{
    BSTR bstr = seqorder.AllocSysString();
    m_acquisition->ReceiveSeriesOrder(bstr, dropindex);
    ::SysFreeString(bstr);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************

void CPQMView::OnContrastAgent(
    const int contrast_agent_index
)
{
    if (!m_iswait_cursor) {

        int first_selected_item = m_pqm_listctrl.GetRightClickCell().y ;

        if (IsValidSequenceIndex(first_selected_item)) {
            ApplyContrastAgentSettings(first_selected_item, contrast_agent_index, true);	//for first selected only..will need to check..
        }
    }
}


//+Patni-Hemant+ARD/2009Nov06/Modified/IR 85 Auto Transfer
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyHoldValueAndAutoTranfferTo(
    const int* const seq_items,
    const int seq_count,
    const int holdvalue,
    const MOVECOUCH_METHOD f_move_couch_method,
    const int auto_transfer_value
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyHoldValueAndAutoTranfferTo");

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    if ((NULL == seq_items) || (seq_count <= 0)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("seq_items pointer is NULL or seq_count is less than ZERO"));
        return;
    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections



    VARIANT* pdata = new VARIANT;

    CString seqlid(_T(""));
    CSequence* pseq = NULL ;

    for (int index = 0; index < seq_count; index ++) {

        if (!IsValidSequenceIndex(seq_items[index])) {
            continue;
        }

        pseq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[index]);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL "));
            continue;
        }

        if (pseq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) {
            DEL_PTR(pdata)
            return ;		//we will not process further.. as we should not do for non wait protocols..
        }


        SetMoveCouchForSeq(pseq,  f_move_couch_method,
                           (pseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("DYN"))) ?  holdvalue | VFC_HOLD_COVERAGE : holdvalue
                          );

        pdata->wReserved1 = AUTO_TRANSFER;
        pdata->intVal = auto_transfer_value;

        if (S_OK == m_acquisition->UpdateGUIControlsInVarfieldDB(pdata,
                _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"))))
           ) {

            //we will update gui when VF database is OK
            pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("APOST"), auto_transfer_value);
            pseq->SetAutoTransferFlag(auto_transfer_value);

        } else {
            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("UpdateGUIControlsInVarfieldDB Failed for AUTO_TRANSFER"));
        }

        pseq->GetCPASCOMSequence()->GetSequenceID(seqlid);

        if (S_OK != m_acquisition->UpdateGUIControlsInDB(pdata, _bstr_t(seqlid))) {

            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("UpdateGUIControlsInDB Failed for AUTO_TRANSFER"));
        }

        m_pqm_listctrl.InvalidateItemsForModeOption(seq_items[index]);
        m_pqm_listctrl.InvalidateItemsForScanTime(seq_items[index]);
    }

    DEL_PTR(pdata)
}
//-Patni-Hemant+ARD/2009Nov06/Modified/IR 85 Auto Transfer
//-Patni-Hemant+ARD/2009Nov06/Modified/IR 85 Auto Transfer // Incorporating Offshore Comments


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyAutoVoice(
    const int f_focus_item,
    const UINT f_menu_id
)
{
    int* l_selected_items = NULL ;
    int l_size = 0;

    if (!m_pqm_listctrl.GetItemsForAutoVoiceAndMoveCouch(f_focus_item, &l_selected_items, l_size)) {

        DEL_PTR_ARY(l_selected_items);
        return ;
    }

    SetWaitCursor(true, false);
    ApplyAutoVoiceToSelectedItems(l_selected_items, l_size, f_menu_id);
    SetWaitCursor(false, false);

    DEL_PTR_ARY(l_selected_items);
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyAutoVoiceToSelectedItems(
    const int* const selected_items,
    const int count,
    const UINT auto_voice_menuitem_id
)
{
    AUTOVOICE_PRESCAN_OPTION l_prescan_aut_voi_optn = NONE;
    AUTOVOICE_OPTIONS selected_auto_voice_option = GetAutoVoiceOptionForID(auto_voice_menuitem_id, l_prescan_aut_voi_optn) ;

    if (selected_auto_voice_option == AUTOVOICE_OPTIONS_INVALID) {
        return ;
    }

    CString str_auto_voice(_T(""));
    str_auto_voice.LoadString(auto_voice_menuitem_id);

    int auto_voice_exe_code = 0;
    DecodeAutoVoiceStringToNumber(str_auto_voice, &auto_voice_exe_code);
    SetAutoVoiceCodeToItems(selected_items, count, auto_voice_exe_code, selected_auto_voice_option, l_prescan_aut_voi_optn);

    if (GetSeqInfoDlgStatus()) {
        m_info_seq_dlg->DisplaySeqInformation();
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ApplyContrastSettingsToItems(
    const int* const seq_items,
    const int seq_count,
    const int contrast_agent_index,
    const bool f_bContrastAgentMenu // //+Patni-Abhishek/2010Dec14/Added/IR-180
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::ApplyContrastSettingsToItems");

    if (!seq_count || (seq_count <= 0)) {
        return;
    }

    int l_scan_index = -1;
    ContrastAgent contrastagent;

    if (!GetContrastAgentFromList(contrastagent, contrast_agent_index)) {
        contrastagent.m_index = 0 ;	//no contrast agent..
        contrastagent.m_strname = _T("");
    }

    //+Patni-Abhishek/2010Dec14/Added/IR-180
    if (0 == contrast_agent_index && !f_bContrastAgentMenu) {

        CSequence* pseq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[0]);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL "));
            return;
        }

        const CPqmPrMode& prmode = pseq->GetPrMode() ;

        if (prmode.m_Prev_Contrast != 0) {
            contrastagent.m_index = prmode.m_Prev_Contrast;	//no contrast agent..
            contrastagent.m_strname = prmode.m_Prev_Str_Contrast;
        }

    }

    //+Patni-Abhishek/2010Dec14/Added/IR-180

    //bugfix
    VARIANT* l_pdata = new VARIANT;
    l_pdata->wReserved1 = CONTRAST_AGENT;
    CString* pStr = new CString(L"TESTING");
    l_pdata->byref = pStr;

    //Patni-PJS/2010Aug24/Added/TMSC-REDMINE-599
    bool l_is_contrast_agnt_updt = false;
    CString seqlid = _T(""), contrast_agent_string = _T("") ;
    CSequence* pseq = NULL ;
    CPASParamStore* l_seq_para = NULL ;

    for (int counter = 0; counter < seq_count ; counter++) {

        pseq = (CSequence*)m_pqm_listctrl.GetItemData(seq_items[counter]);

        if (NULL == pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL "));
            continue;
        }

        CPqmPrMode& prmode = pseq->GetPrMode() ;

        if ((prmode.m_contrast == contrastagent.m_index)
            &&
            (prmode.m_str_contrast.CompareNoCase(contrastagent.m_strname) == 0)
           )
            continue ;		//do not applly if already its there..

        //get the acq order..
        l_scan_index = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

        l_seq_para = pseq->GetCPASCOMSequence()->GetSequenceParam();

        //+Patni-HEMANT/MODIFIED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
        //The defualt string in CSequence::CSequence() is "".
        //CString contrast_agent_string = _T(" ");		//no contrast agent
        contrast_agent_string = _T("");		//no contrast agent
        //-Patni-HEMANT/MODIFIED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
        l_pdata->byref = _T(" ");		//no contrast agent

        if (contrastagent.m_index) {

            contrast_agent_string = contrastagent.m_strname ;
            //added SM4_Mangesh
            BSTR bstrcontrast_agent = contrast_agent_string.AllocSysString();
            l_pdata->byref = bstrcontrast_agent ;
            l_pdata->wReserved3 = contrastagent.m_index ;

        }


        GetAcquisition()->UpdateGUIControlsInVarfieldDB(l_pdata, l_scan_index);

        //+Patni-PJS/2010Aug24/Added/TMSC-REDMINE-599
        if (PR_ACQ_STATUS_WAIT != pseq->GetAcqStatus()) {

            seqlid = _T("") ;
            pseq->GetCPASCOMSequence()->GetSequenceID(seqlid);

            if (S_OK == m_acquisition->UpdateProtocolDataInSeries(l_pdata, seqlid.AllocSysString(), l_scan_index)) {
                if (!l_is_contrast_agnt_updt) {

                    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                        CPQMUtility::GetMultiLingualString(_T("IDS_INFORM_COMMENT_DONE")),
                        MPlus::EH::Information, _T("PQM"), _T("PQM"));
                    l_is_contrast_agnt_updt = true;
                }

            } else {
                PQM_TRACE(GUI_CONTROLS_MARKER,
                          FUNC_NAME,
                          _T("Fail to update Contrast agent in Local DB"));
            }
        }

        l_seq_para->SetString(_T("CONTRASTNAME"), contrast_agent_string);

        //CPqmPrMode prmode = GetPrModeOfSelectedSequence(index);

        //+Patni-Hemant/2010Jun15/Added/V1.20#MVC008201
        //prmode.m_contrast = contrastagent.m_index ;
        //Patni-Abhishek/2010Dec14/Modified/IR-180
        if (contrast_agent_index == -1) {
            prmode.m_Prev_Contrast = prmode.m_contrast;
            prmode.m_Prev_Str_Contrast = prmode.m_str_contrast;
        }

        prmode.m_contrast = contrastagent.m_index;
        //-Patni-Hemant/2010Jun15/Added/V1.20#MVC008201

        prmode.m_str_contrast = contrast_agent_string ;

        //+Patni-Abhishek/2010Dec14/Added/IR-180
        if (contrast_agent_index != -1) {
            prmode.m_Prev_Contrast = prmode.m_contrast;
            prmode.m_Prev_Str_Contrast = prmode.m_str_contrast;
        }

        //+Patni-Abhishek/2010Dec14/Added/IR-180


        m_pqm_listctrl.InvalidateItemsForModeOption(seq_items[counter], CONTRAST_AGENT_OPTION);
    }

    UpdateTRSCFromGivenIndex(seq_items[0], CPQMView::TRSC_C);
    //-Patni-HEMANT/MODIFIED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect


    DEL_PTR(pStr)
    DEL_PTR(l_pdata)
}

//************************************Method Header**************************************
//Method Name:  DecodeAutoVoiceStringToNumber
//Author:  PATNI/SVP
//Purpose: Create auto voice code.

//**************************************************************************************/
void CPQMView::DecodeAutoVoiceStringToNumber(
    const CString& str_auto_voice,
    int* auto_voice_exe_code
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::DecodeAutoVoiceStringToNumber");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == auto_voice_exe_code) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("auto_voice_exe_code pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    *auto_voice_exe_code = 0;

    for (int i = 0; i < NUM_VOICE; i++) {

        //Creates pre scan voice code
        if (str_auto_voice.Left(2) == auto_voice_code[i]) {

            *auto_voice_exe_code |= i << 4;
        }

        //Creates scan voice code
        if (str_auto_voice.Right(2) == auto_voice_code[i]) {

            *auto_voice_exe_code |= i;
        }
    }
}

//************************************Method Header**************************************
//Method Name:  EncodeAutoVoiceNumberToString
//Author:  PATNI/SVP
//Purpose: Encode auto voice number to string
//**************************************************************************************/
void CPQMView::EncodeAutoVoiceNumberToString(
    const int auto_voice_exe_code,
    CString& str_auto_voice
)const
{
    str_auto_voice =  auto_voice_code[auto_voice_exe_code >> 4];
    str_auto_voice += auto_voice_code[auto_voice_exe_code & 0x0F];
}

//************************************Method Header**************************************
//Method Name:  DisplayScanOffsetOnPQM
//Author:  PATNI/SVP
//Purpose: Displays Scan Offset value on PQM window.
//**************************************************************************************/
void CPQMView::DisplayScanOffsetOnPQM(
    const int idx
)
{
    //Patni-AMT/2010Apr16/Added/JaFT# PROPOSAL1_CDR_01
    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayScanOffsetOnPQM");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CWnd* l_scan_offset_ctrl = GetDlgItem(IDC_SCAN_OFFSET_STATIC);

    if (NULL == l_scan_offset_ctrl) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetDlgItem(IDC_SCAN_OFFSET_STATIC) pointer is NULL"));
        return;
    }

    CString str_tmp = _T("(---)cm");

    if (idx != -1) {

        VARIANT* pData = new VARIANT;

        if (NULL == pData) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pData pointer is NULL"));
            return;
        }

        if (S_OK != m_acquisition->GetPatientScanoffset(GetAcqOderForGivenProtocolIndex(idx), &pData)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetPatientScanoffset failed to get Patient scan offset"));
            DEL_PTR(pData);
            return;
        }

        f32vec3_t* l_objvec = (f32vec3_t*) pData->byref;

        if (NULL == l_objvec) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_objvec pointer is NULL"));
            DEL_PTR(pData)
            return;
        }

        ValidateScanOffsetWithinRange(l_objvec->x, l_objvec->y, l_objvec->z);

        str_tmp.Format(_T("(%1.1f)cm"), l_objvec->z);

        DEL_PTR(pData->byref)
        DEL_PTR(pData)
    }

    l_scan_offset_ctrl->SetWindowText(str_tmp);

    //-Patni-ARD+AMT/2009Dec03/Modified/DeFT# MVC005787+MSA0248-00087
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::OnProtocolCommentChanged()
{

    if (GetSeqInfoDlgStatus()) {

        m_info_seq_dlg->DisplaySeqInformation();
    }

    if (GetCoilDlgStatus()) {

        ChangeCoilDialogCaptionAppropriately();
    }

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ChangeCoilDialogCaptionAppropriately()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ChangeCoilDialogCaptionAppropriately");
    const int l_first_sel_item = m_pqm_listctrl.GetFirstSelectedItem();

    if (IsValidSequenceIndex(l_first_sel_item)) {

        CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(l_first_sel_item);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL "));
            return;
        }

        dlgCoilSelWin->SetWindowCaption(GetCoildDialogStringFor(l_seq), true);
    }
}


//****************************Method Header************************************
//Module         : StartRFLPrescan
//Author         : PATNI/MSN
//Purpose        : Starts RFL prescan
//*****************************************************************************
void CPQMView::StartRFLPrescan()
{
    //+Patni-PJS/2010Aug09/Modified/MVC008333 code review
    LPCTSTR FUNC_NAME = _T("CPQMView::StartRFLPrescan");
    PQM_TRACE_SCOPED(FUNC_NAME);
    //-Patni-PJS/2010Aug09/Modified/MVC008333 code review

    int firstwaitprotocol = -1;
    GetIndexOfFirstWaitStateProtocol(firstwaitprotocol);

    //+Patni-HEMANT/ADDED On 9/11/2009 3:42:57 PM/ Code review
    if (firstwaitprotocol == -1) {
        return ;
    }

    //-Patni-HEMANT/ADDED On 9/11/2009 3:42:57 PM/ Code review

    SetScanIndex(firstwaitprotocol);

    //Patni-Hemant/2009Dec1/Added/MVC005971
    SetAbortFlag(false);

    TCHAR l_scanindex[NUM_100] = {0};//Patni-DKH/2009Aug24/Modified/cpp test corrections
    wsprintf(l_scanindex, _T("%d"), m_scan_index);

    //Patni-PJS/2010Aug09/Modified/MVC008333 code review
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_scanindex);

    if (firstwaitprotocol > 0) {

        //Patni-HEMANT/ADDED On 9/11/2009 3:42:57 PM/ internal error found
        m_is_scanning = true;

        //+Patni-PJS/2010Aug09/Added/MVC008333

        if (m_curr_seq == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_curr_seq is NULL."));
            return ;
        }

        CSequence* l_seq = (CSequence*)m_pqm_listctrl.GetItemData(firstwaitprotocol);

        if (NULL == l_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL."));
            return ;
        }

        *m_curr_seq = *l_seq;
        //-Patni-PJS/2010Aug09/Added/MVC008333

        //+Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
        if (S_FALSE == m_acquisition->StartRFLPrescan(m_scan_index)) {

            m_is_scanning = false;
            SetAbortFlag(true);
            SetScanIndex(-1);
        }

        //-Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::MakeAutoScanOffForIfNecessary(
    const int item_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::MakeAutoScanOffForIfNecessary");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!IsValidSequenceIndex(item_index)) {
        CString str ;
        str.Format(_T("item_index is Not Valid : item_index = %d"), item_index);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

        return;
    }

    //this msg comes when scan terminates and status becomes wait..
    //For this message, we must check for auto scan status for first wait protocol index ..
    //the first wait items status is not changed to curent..


    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(item_index);

    if (l_seq == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
        return;
    }

    //get the status for this seq ..
    int holdvalue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

    //if the hold is not inlcuding VFC_HOLD_PRE_SCAN then the auto scan is on..
    const bool auto_scan_on = ((holdvalue & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) ;

    if (auto_scan_on) {

        ApplyAutoScanToItems(&item_index, 1, false);		//make it off explicitly..
    }

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::ClearSelectedAcqoderListForWSE(
)
{
    m_selected_acqoder_list_for_wse.RemoveAll();
}

//************************************Method Header************************************
// Method Name  : IsAnyProtocolPendingForWSEEditing
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::IsAnyProtocolPendingForWSEEditing(
)const
{
    //Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
    LPCTSTR FUNC_NAME = _T("CPQMView::IsAnyProtocolPendingForWSEEditing");

    const int total_editing_protocol_selected = m_selected_acqoder_list_for_wse.GetSize() ;

    if (total_editing_protocol_selected) {

        const int l_acq_order = GetAcqOderForGivenProtocolIndex(m_pqm_listctrl.GetWSEEditingProtocolIndex());

        //Now we have found current acq order being edited..
        //if there is any missing acq oder protocol after this in list saved,
        //return true..

        int protocol_index_in_list = -1;
        CSequence* l_seq = NULL ;

        //we are not going to check last element as there will not be any element after last element
        for (int index = 0; index < total_editing_protocol_selected - 1 ; index++) {

            //Added by Hemant On 1/22/2009 8:52:01 PM
            //lets say, in future we allow delete to be perfoemd while editing,
            //we must check if this is not the valid index, the next would not be valid
            if (!IsValidSequenceIndex(index))
                return false ;

            if (m_selected_acqoder_list_for_wse.GetAt(index) == l_acq_order) {

                //-------------------------------------------------------------------------
                //Modified by Hemant On 1/22/2009 8:51:20 PM
                //
                //Now , if next protocol to be edited is re-arranged to up and scanned, then..
                //find the next protocol in list which is of WAIT state.
                //again we will serach only till [one less item of list] that is (total_editing_protocol_selected-1)

                //return true;

                index++;

                for (; index < total_editing_protocol_selected ; index++) {

                    //note : we are editing only for stored acq order list..
                    //so here get the list index from acq order first..

                    protocol_index_in_list = GetListForGivenProtocolAcqOder(m_selected_acqoder_list_for_wse.GetAt(index));

                    if (protocol_index_in_list == -1)
                        continue ;

                    //lets say, in future we allow delete to be performed while editing,
                    //we must check if this is not the valid index, the next would not be valid
                    if (!IsValidSequenceIndex(protocol_index_in_list))
                        return false ;

                    l_seq = (CSequence*)m_pqm_listctrl.GetItemData(protocol_index_in_list);

                    if (NULL == l_seq) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
                        continue;
                    }

                    if (l_seq->GetAcqStatus() == PR_ACQ_STATUS_WAIT) {

                        //we have found a protocol in list , which is WAIT...

                        //+Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1
                        CString l_str = _T("");
                        l_str.Format(_T("Next WSE Protocol index would be : %d"), protocol_index_in_list);
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                        //-Patni-Hemant-MP/2011Mar28/Added/TMSC-REDMINE-1486-Part1

                        return true ;
                    }

                }

                //here we have not found any WAIT protocol which is to be edited next and also in save list
                //so we can not do QUEUE_AND_NEXT
                return false ;
                //
                //End Of addition By Hemant On1/22/2009 8:54:35 PM
                //-------------------------------------------------------------------------
            }
        }
    }

    //we don't have found pending wse editable protocol in acq oder list after current.
    return false ;		//no protocol for editing is avialble..
}


//+Patni-HEMANT/ADDED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
//************************************Method Header************************************
// Method Name  : UpdateTRSCFromGivenIndex
// Author       : PATNI/ HEMANT
// Purpose      : update trsc from given index for given trsc enum type
//***********************************************************************************
bool CPQMView::UpdateTRSCFromGivenIndex(
    const int f_list_index,
    const CPQMView::TRSC_Enum f_trsc_enum_to_update
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateTRSCFromGivenIndex");

    //If given list index itself is not valid, then return
    if (!IsValidSequenceIndex(f_list_index)) {
        //        ASSERT(FALSE);
        return false;
    }

    const int trsc_index = (f_trsc_enum_to_update - TRSC_T) ;

    const int total_list_items = m_pqm_listctrl.GetItemCount() ;

    char trsc_symbol_for_prev = _T('');

    CSequence* prev_seq = NULL ;

    for (int index = f_list_index ; index < total_list_items ; index++) {

        //if we are processing first list item, then it should be always '-'
        if (index == 0) {

            CSequence* current_seq = (CSequence*)m_pqm_listctrl.GetItemData(index);

            if (current_seq) {
                CPqmPrMode& current_prmode = current_seq->GetPrMode() ;
                current_prmode.m_str_trsc.SetAt(trsc_index, '-');
                continue ;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("current_seq is NULL "));
            }
        }

        //now process item which is not the first in list

        //if the current item's data mismatch with the prev item
        //then alter the diaply symbol for current.
        //if prev item's symbol is '-' then curent item's symbol
        //will be '*'.
        //if the current item's data doesn't mismatch with the prev item
        //retain the symbol of prev item

        CPqmPrMode& current_prmode = ((CSequence*)m_pqm_listctrl.GetItemData(index))->GetPrMode() ;

        prev_seq = (CSequence*)m_pqm_listctrl.GetItemData(index - 1);

        if (NULL == prev_seq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("prev_seq is NULL "));
            continue;
        }

        //REDMINE-781
        const CPqmPrMode& prev_prmode = prev_seq->GetPrMode() ;


        //get trsc symbol for prev item for given trsc_index
        trsc_symbol_for_prev = (char)prev_prmode.m_str_trsc.GetAt(trsc_index);

        if (AreDataSameForGivenTwoIndex(index - 1, index , f_trsc_enum_to_update)) {

            current_prmode.m_str_trsc.SetAt(trsc_index, trsc_symbol_for_prev);

        } else {

            current_prmode.m_str_trsc.SetAt(trsc_index, (trsc_symbol_for_prev == '-') ? '*' : '-');
        }

        m_pqm_listctrl.DisplayTRSC(index, current_prmode.m_str_trsc);
    }

    return true ;
}
//-Patni-HEMANT/ADDED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect

//************************************Method Header************************************
// Method Name  : AreDataSameForGivenTwoIndex
// Author       : PATNI/ HEMANT
// Purpose      : check for data mismatch for goven trsc enum type
//***********************************************************************************
bool CPQMView::AreDataSameForGivenTwoIndex(
    const int f_list_prev,
    const int f_list_current,
    const CPQMView::TRSC_Enum f_trsc_enum_to_update
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::AreDataSameForGivenTwoIndex");

    //If given list index itself is not valid, then return
    if (!IsValidSequenceIndex(f_list_prev)  ||
        !IsValidSequenceIndex(f_list_current)
       ) {
        //        ASSERT(FALSE);
        return false;
    }

    //now according to f_trsc_enum_to_update , we will target theh data
    //for comparision

    CSequence* prev_seq = (CSequence*)m_pqm_listctrl.GetItemData(f_list_prev);
    CSequence* current_seq = (CSequence*)m_pqm_listctrl.GetItemData(f_list_current);

    if ((NULL == prev_seq)  || (NULL == current_seq)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("prev_seq or current_seq is NULL"));
        return false;
    }

    //REDMINE-781
    const CPqmPrMode& prev_prmode = prev_seq->GetPrMode() ;


    //REDMINE-781
    const CPqmPrMode& current_prmode = current_seq->GetPrMode() ;


    bool l_are_data_same = false ;

    switch (f_trsc_enum_to_update) {

        case TRSC_T : {

            l_are_data_same = (prev_seq->GetAutoTransferFlag() == current_seq->GetAutoTransferFlag());
        }
        break ;

        case TRSC_R : {

            l_are_data_same = (prev_prmode.m_recv_coil_label.CompareNoCase(current_prmode.m_recv_coil_label) == 0) ;

        }
        break ;

        case TRSC_S : {

            l_are_data_same = (prev_prmode.m_anatomy == current_prmode.m_anatomy) ;

            break ;
        }

        case TRSC_C : {

            l_are_data_same  = ((prev_prmode.m_contrast == current_prmode.m_contrast)
                                &&
                                (prev_prmode.m_str_contrast.CompareNoCase(current_prmode.m_str_contrast) == 0)
                               ) ;
        }
        break ;

        default :
            return l_are_data_same ;
    }


    return l_are_data_same ;
}
//-Patni-HEMANT/ADDED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect

//+Patni-SS/2009Apr07/Modi/PSP1#12
void CPQMView::PQMMessageMapDuplicate()const
{

    m_pqmchildthread.PostMessageToThread(ON_DUPLICATE, NULL, NULL);

}

void CPQMView::PQMMessageMapSetAll()const
{
    m_pqmchildthread.PostMessageToThread(ON_SETALL, NULL, NULL);

}
//-Patni-SS/2009Apr07/Modi/PSP1#12


//+Patni-HEMANT/ADDED On 8/14/2009 12:28:15 PM/ Code Review
void CPQMView::PerformRightClickModeOption(
    const int f_option
)
{
    if (!CanPerformModeOptionOperations())
        return ;

    const int l_first_selected_item = m_pqm_listctrl.GetFirstSelectedItem();

    switch (f_option) {

        case AUTO_TRANSFER_OPTION:
            OnClickButtonAutoTransfer(l_first_selected_item);
            break ;

        case BREATH_HOLD_OPTION:
            OnClickButtonBreathHold(l_first_selected_item);
            break ;
    }
}
//-Patni-HEMANT/ADDED On 8/14/2009 12:28:15 PM/ Code Review


//+Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90
//************************************Method Header************************************
// Method Name  : RemoveWndQueueMessages
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::RemoveWndQueueMessages(
)const
{
    MSG l_msg = { 0 };

    while (PeekMessage(&l_msg, m_hWnd, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE));

    while (PeekMessage(&l_msg, m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE));

}
//-Patni-HEMANT/ADDED On 8/21/2009 5:45:40 PM/ ACE+JFT / IR90


//+Patni-Sribanta/2011Apr29/Added/REDMINE-1678
//************************************Method Header************************************
// Method Name  : UpdateScanOffsetForDisplay
// Author       : PATNI/Hemant+Sribanta
// Purpose      :
//***********************************************************************************
void CPQMView::UpdateScanOffsetForDisplay(
    const int f_prot_index
) const
{

    if (!IsValidSequenceIndex(f_prot_index)) {
        return ;
    }

    CSequence* l_pseq = (CSequence*) m_pqm_listctrl.GetItemData(f_prot_index);
    VARIANT* pData = new VARIANT;

    GetAcquisition()->GetPatientScanoffset(GetAcqOderForGivenProtocolIndex(f_prot_index), &pData);
    f32vec3_t* l_objvec = (f32vec3_t*) pData->byref;

    if (NULL == l_objvec) {
        return ;
    }

    Vector_t f_scanoffset;
    f_scanoffset.x = l_objvec->x;
    f_scanoffset.y = l_objvec->y;
    f_scanoffset.z = l_objvec->z;

    l_pseq->SetScanOffset(f_scanoffset);


    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData);
    }

    //memory for l_objvec is assigned at IPC side.
    //so need to delete
    DEL_PTR(l_objvec);
}
//-Patni-Sribanta/2011Apr29/Added/REDMINE-1678


//+Patni-Hemant/2010Jun01/Added/MaFT/TMSC-REDMINE-MVC007915/Bug Fix
void CPQMView::ApplyConfigSettingForProt(
    CSequence* f_seq
)const
{
    if (!f_seq) {
        return;
    }

    SScanAantomy l_sscanaantomy_to_return ;
    CPqmPrMode& l_prmode_seq = f_seq->GetPrMode();

    if (((l_prmode_seq.m_anatomy >= SAR_REGION(ABDOMEN)) && (l_prmode_seq.m_anatomy < SAR_REGIONS))
        &&
        (!l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty())
       ) {

        LateralityEnum l_laterality = l_prmode_seq.m_str_scan_anatomy.GetLaterality();

        GetBodyPartData(l_prmode_seq.m_anatomy, l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_name, l_sscanaantomy_to_return);
        l_prmode_seq.m_str_scan_anatomy = l_sscanaantomy_to_return ;

        l_prmode_seq.m_str_scan_anatomy.SetLaterality(l_laterality);
    }
}

void CPQMView::ValdiateBodyPartData(
    CSequence* f_seq,
    const int f_seq_index
)
{
    if (!f_seq) {
        return;
    }

    CPqmPrMode& l_prmode_seq = f_seq->GetPrMode();

    if ((l_prmode_seq.m_anatomy >= SAR_REGION(ABDOMEN)) && (l_prmode_seq.m_anatomy < SAR_REGIONS)
        &&
        (l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_vf < 0 || l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty())
       ) {

        ApplyDefaultScanAnatomyFor(f_seq_index, true);
        ApplyScanAnatomy(&f_seq_index, 1, -1, l_prmode_seq.m_str_scan_anatomy.GetLaterality(), false, &l_prmode_seq.m_str_scan_anatomy);
    }


    if (!l_prmode_seq.m_str_scan_anatomy.GetLiteralityFlag()
        && (l_prmode_seq.m_str_scan_anatomy.GetLaterality() != Laterality_Invalid)
       ) {
        l_prmode_seq.m_str_scan_anatomy.SetLaterality(Laterality_Invalid);

        if (!(l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_vf < 0 || l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty())) {
            ApplyScanAnatomy(&f_seq_index, 1, -1, l_prmode_seq.m_str_scan_anatomy.GetLaterality(), false, &l_prmode_seq.m_str_scan_anatomy);
        }

    } else if (l_prmode_seq.m_str_scan_anatomy.GetLiteralityFlag()
               && (l_prmode_seq.m_str_scan_anatomy.GetLaterality() == Laterality_Invalid)
              ) {
        l_prmode_seq.m_str_scan_anatomy.SetLaterality(Laterality_None);

        if (!(l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_vf < 0 || l_prmode_seq.m_str_scan_anatomy.m_scan_anatomy_name.IsEmpty())) {
            ApplyScanAnatomy(&f_seq_index, 1, -1, l_prmode_seq.m_str_scan_anatomy.GetLaterality(), false, &l_prmode_seq.m_str_scan_anatomy);
        }
    }
}

//-Patni-Hemant/2010Jun01/Added/MaFT/TMSC-REDMINE-MVC007915/Bug Fix
//+Patni-Hemant/2010Jun9/Added/MaFT/MVC008238
void CPQMView::UpdateCoilDataInSeq(
    CSequence* f_seq
)const
{
    if (!f_seq) {

        ASSERT(FALSE);
        return ;
    }

    const CString recv_coil_label = GetCoilStringFor(f_seq) ;

    f_seq->SetValue(rc, recv_coil_label);

    CPqmPrMode& prmode = f_seq->GetPrMode();

    prmode.m_rcv_coil_id = __recv_coil_id;

    prmode.m_recv_coil_label = recv_coil_label;
}
//-Patni-Hemant/2010Jun9/Added/MaFT/MVC008238

void CPQMView::ValidateContrastAgent(
    CSequence* f_seq
)const
{

    if (!f_seq || (PR_ACQ_STATUS_WAIT != f_seq->GetAcqStatus())) {
        return;
    }

    bool l_contrast_agent_found = false;
    CPqmPrMode& prmode = f_seq->GetPrMode() ;

    const int total_contrast_agents = m_contrast_agent->GetCount() ;

    if (total_contrast_agents > 0) {

        POSITION pos = m_contrast_agent->GetHeadPosition();

        const CString contrst_agentstr = prmode.m_str_contrast;

        for (int contrast_agent_index = 0; contrast_agent_index < total_contrast_agents ; contrast_agent_index ++) {

            ContrastAgent l_contrastagent = m_contrast_agent->GetNext(pos) ;

            if (contrst_agentstr.CompareNoCase(l_contrastagent.m_strname) == 0) {
                l_contrast_agent_found = true;
                break ;
            }
        }
    }

    if (!l_contrast_agent_found) {
        prmode.m_contrast = false;
        prmode.m_str_contrast = _T("");

        VARIANT* l_pdata = new VARIANT;
        l_pdata->wReserved1 = CONTRAST_AGENT;
        l_pdata->byref = _T("") ;
        l_pdata->wReserved3 = 0;

        CPASParamStore* l_seq_para = f_seq->GetCPASCOMSequence()->GetSequenceParam();

        const int l_acq_order = _ttoi(l_seq_para->GetString(_T("ACQORDER")));

        CString seqlid;
        f_seq->GetCPASCOMSequence()->GetSequenceID(seqlid);

        GetAcquisition()->UpdateGUIControlsInVarfieldDB(l_pdata, l_acq_order);

        l_seq_para->SetString(_T("CONTRASTNAME"), prmode.m_str_contrast);

        //REDMINE-781
        DEL_PTR(l_pdata);
    }
}
//-Patni-Hemant/2010Jun15/Added/V1.20#MVC008201


//+Patni-HAR/2010Jun24/Added/V1.20#MVC008392
//*****************************************************************************
//Method Name   : CheckCFADialogStatus
//Author        : Patni / HAR
//Purpose       :
//*****************************************************************************
BOOL CPQMView::CheckCFADialogStatus(
)const
{
    return ((m_cfadlg && ::IsWindow(m_cfadlg->m_hWnd)) ? TRUE : FALSE);
}

BOOL CPQMView::CheckCoilDialogStatus(
)const
{
    return ((dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd) && dlgCoilSelWin->IsWindowVisible()) ? TRUE : FALSE);
}
//-Patni-HAR/2010Jun24/Added/V1.20#MVC008392

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
void CPQMView::OnSFTMenuCommand(
    const UINT f_cmd_id
)
{
    m_pqmchildthread.PostMessageToThread(ON_SFT_MENU_COMMAND, f_cmd_id, NULL);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//**************************************************************************
//Method Name   :  UpdateMasterSlaveCtrls()
//Author        :  PATNI/AKR
//Purpose       :  This function will update master slave group id and flag
//**************************************************************************
void CPQMView::UpdateMasterSlaveCtrls(int* f_seq_indexes, const int f_seq_count)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateMasterSlaveCtrls");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!f_seq_count || !f_seq_indexes) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("no protocol."));
        return;
    }

    VARIANT l_pdata;
    CSequence* l_pseq = NULL;
    int l_acq_order = 0;

    for (int l_index = 0 ; l_index < f_seq_count; l_index++) {

        int l_current_prot_index = f_seq_indexes[l_index];

        if (!IsValidSequenceIndex(l_current_prot_index)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("invalid sequence index"));
            break;
        }

        l_pseq = (CSequence*)m_pqm_listctrl.GetItemData(l_current_prot_index);

        if (NULL == l_pseq) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pseq is NULL "));
            continue;
        }

        l_acq_order = GetAcqOderForGivenProtocolIndex(l_current_prot_index);

        l_pdata.wReserved1 = MASTER_SLAVE_HOLD;

        if (S_OK == m_acquisition->GetControlValueFromDB(&l_pdata, l_acq_order)) {

            l_pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("GROUPID") , l_pdata.intVal);
            l_pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("MASTERFLAG") , l_pdata.wReserved1);
            l_pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("HOLD") , l_pdata.wReserved2);
            l_pseq->SetCanChangeModeOptionBreathHold(!(l_pdata.wReserved3));
        }

    }

    m_pqm_listctrl.Invalidate(true);
    m_pqm_listctrl.UpdateWindow();
}
//**************************************************************************
//Method Name   :  UpdateGUICtrlMasterSlave()
//Author        :  PATNI/AKR
//Purpose       :
//**************************************************************************
void CPQMView::UpdateGUICtrlMasterSlave()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateGUICtrlMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VARIANT l_pdata;

    int l_current_prot_index = m_pqm_listctrl.GetWSEEditingProtocolIndex();
    l_pdata.intVal = l_current_prot_index;

    HRESULT l_result = m_acquisition->GetProtocolInfoForAutoCopy(&l_pdata);

    if (S_OK != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetProtocolInfoForAutoCopy failed."));
        return;
    }

    int l_protocol_count = l_pdata.intVal;
    int* l_slave_protocols_index = (int*)l_pdata.byref;

    if (!l_protocol_count || !l_slave_protocols_index) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("no slave protocol."));
        return	;
    }

    UpdateMasterSlaveCtrls(l_slave_protocols_index, l_protocol_count);
    DEL_PTR_ARY(l_slave_protocols_index);

}
//************************************Method Header************************************
// Method Name  : ExecuteAutoScanStart
// Author       : PATNI-AKR/NFD002_Demo_CR-3
// Purpose      : Starts AutoScan
//***********************************************************************************
void CPQMView::ExecuteAutoScanStart()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::ExecuteAutoScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    int l_first_wait_index = m_pqm_listctrl.GetFirstWaitProtocolIndex();
    int l_selected_index = GetSelectedSeqIndex();

    //1.Scan should not start while scanning, 2. Delay dlg is modal, so cannot change the selection after opening
    if (IsScanning() || !IsValidSequenceIndex(l_first_wait_index) || l_selected_index != l_first_wait_index) {
        return ;
    }

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_selected_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq is NULL"));
        return;
    }

    int l_holdvalue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

    if ((l_holdvalue & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) {
        m_acquisition->ExecuteAutoScanStart();
    }
}
//****************************Method Header************************************
//Module         : StopInjectTimerThread()
//Author         : PATNI
//Purpose        : This function will stop the thread created for time display on inject button.
//*****************************************************************************
void CPQMView::StopInjectTimerThread()
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CPQMView::StopInjectTimerThread"),
              _T("CPQMView::StopInjectTimerThread"));

    KillTimer(WM_CMD_INJECT_TIMER);
    m_inject_timer_button.ShowWindow(SW_HIDE);
    m_inject_timer_time = 0;
    m_dw_start_count = 0;
}


//+Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
void CPQMView::UpdateActiveDlgStatusToIPC(
)
{
    if (m_acquisition) {
        m_acquisition->SetActiveDialog(IS_COIL_SELECTION_DIALOG, GetCoilDlgStatus());
    }
}
//-Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226

void CPQMView::UpdateGUIOnScanStart()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateGUIOnScanStart");
    PQM_TRACE_SCOPED(FUNC_NAME);
    m_current_scan_opration = CurrentScanOpration_ScanStart;
    EnableDisableScanRelatedButtons(false, false);

    if (GetCommentDlgStatus()) {
        m_comment_dlg->DisplayCommentInformation();
    }


    UpdateCFARetry();
    UpdateOKBtnStatusofWirelssDlg();
    EnableDisableScanEditOrPlanButtons();
    EnableDisableScanAnatomy();
}


void CPQMView::CreateWFDAMsgReceiver()
{

    LPCTSTR FUNC_NAME = _T("CPQMView::CreateWFDAMsgReceiver");

    m_wfdaimplementer = new CWFDAImplementer(this);

    if (m_wfdaimplementer) {

        CWFDAMsgReceiver::CreateWFDAMsgReceiver(m_wfdaimplementer->GetWFDAHandlerInterface());
        CDPSManager::GetInstance()->Attach(CWFDAMsgReceiver::GetWFDAMsgReceiver());

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_wfdaimplementer pointer is NULL"));
    }
}

//+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
//************************************Method Header************************************
// Method Name  : AddNewGUIProcessingBit
// Author       : PATNI/Hemant
// Purpose      : To set processing bit
//***********************************************************************************
inline void CPQMView::AddNewGUIProcessingBit(
    const UINT f_bit
)
{
    m_pqm_gui_processing |= f_bit;
}

//************************************Method Header************************************
// Method Name  : RemoveGUIProcessingBit
// Author       : PATNI/Hemant
// Purpose      : To reset processing bit
//***********************************************************************************
inline void CPQMView::RemoveGUIProcessingBit(
    const UINT f_bit
)
{
    if (m_pqm_gui_processing & f_bit) {
        m_pqm_gui_processing ^= f_bit;
    }
}
//-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1



bool CPQMView::ShouldDisableAPCFunc(
    const int f_first_sel_item
) const
{

    return ((m_pqm_g.GetPQMMode() != K_IMAGING) ||
            IsScanning() ||
            (m_pqm_listctrl.GetItemCount() <= 0) ||
            (f_first_sel_item != m_pqm_listctrl.GetFirstWaitProtocolIndex()) ||
            (!IsValidSequenceIndex(f_first_sel_item)) ||
            (m_rmc_scan_running) ||
            IsFirstWaitProtocolEditedbyWSE() ||
            IsAMB()
           ) ;
}

bool CPQMView::ShouldDisableGATEFunc(
) const
{
    return ((m_pqm_g.GetPQMMode() != K_IMAGING)
            || IsScanning()
            || (m_rmc_scan_running)
            || IsAMB()
           ) ;
}
bool CPQMView::ShouldDisableLeadType(
) const
{
    return ((m_pqm_g.GetPQMMode() != K_IMAGING)
            || IsScanning()
            || (m_rmc_scan_running)
            || IsAMB()
           ) ;
}



//************************************Method Header************************************
// Method Name  : OnMoveCouchMethodMenuClick
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when Move Couch Menu item is clicked
//                f_move_couch_method can be PARENTREFPOS or CENTREPLAN
//                This function will apply move couch as per parameter to
//                appropriate protocols.
//***********************************************************************************
bool CPQMView::OnMoveCouchMethodMenuClick(
    const MOVECOUCH_METHOD f_move_couch_method
)
{

    const CPoint l_cell_index = m_pqm_listctrl.GetRightClickCell();

    if (!IsValidSequenceIndex(l_cell_index.y) || m_iswait_cursor) {
        return false;
    }

    CSequence* l_seq_clicked = (CSequence*) m_pqm_listctrl.GetItemData(l_cell_index.y);
    const MOVECOUCH_METHOD l_new_movecouch_method = GetNewMethodForMoveCouch(l_seq_clicked->GetMoveCouchMethodOption(), f_move_couch_method);

    UpdateMoveCouchMethod(l_new_movecouch_method, l_cell_index.y);

    return true;
}


//************************************Method Header************************************
// Method Name  : UpdateMoveCouchMethod
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when Move Couch Menu item is clicked or when
//                move couch button itself is clicked.
//                f_move_couch_method is final method to apply
//                f_move_couch_method  can be PARENTREFPOS or CENTREPLAN or NOMOVE
//                This function will apply move couch as per parameter to
//                appropriate protocols.
//***********************************************************************************
bool CPQMView::UpdateMoveCouchMethod(
    const MOVECOUCH_METHOD f_move_couch_method,
    const int f_focus_item
)
{
    if ((K_IMAGING != GetPQMGlobal().GetPQMMode())) {
        return false;
    }

    BOOL l_enable = FALSE;
    int l_in_side_limit = 0, l_out_side_limit = 0;
    g_MainView->GetAcquisition()->GetInsideLimit(g_MainView->GetAcqOderForGivenProtocolIndex(f_focus_item), &l_in_side_limit,
            &l_out_side_limit, &l_enable);

    if (!l_enable) {
        return false;
    }


    int* l_selected_items = NULL ;
    int l_size = 0;

    if (!m_pqm_listctrl.GetItemsForAutoVoiceAndMoveCouch(f_focus_item, &l_selected_items, l_size, MOVE_COUCH_OPTION)) {

        DEL_PTR_ARY(l_selected_items);
        return false;
    }

    SetWaitCursor(true, false);
    UpdateMoveCouchMethodFor(l_selected_items, l_size, f_move_couch_method);

    if (GetSeqInfoDlgStatus()) {
        m_info_seq_dlg->DisplaySeqInformation();
    }

    SetWaitCursor(false, false);

    DEL_PTR_ARY(l_selected_items);

    return true;
}


//************************************Method Header************************************
// Method Name  : UpdateMoveCouchMethodFor
// Author       : PATNI/ HEMANT
// Purpose      : This function will apply f_move_couch_method move couch method to
//                f_selected_items protocols.
//***********************************************************************************
void CPQMView::UpdateMoveCouchMethodFor(
    int* f_selected_items,
    const int l_size,
    const MOVECOUCH_METHOD f_new_move_couch_method
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::UpdateMoveCouchMethodFor");

    if ((NULL == f_selected_items) || (l_size <= 0)) {
        return ;
    }

    CSequence* l_seq = NULL ;
    int l_new_holdvalue = 0;

    for (int l_index = 0; l_index < l_size; l_index++) {

        const int l_item_to_apply = f_selected_items[l_index];
        l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_item_to_apply) ;

        if (NULL == l_seq) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq pointer is NULL"));
            continue;
        }

        if (l_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) {
            continue;
        }

        if (GetIsAMBPlanning() && (NOMOVE != l_seq->GetMoveCouchMethodOption())) {
            continue;
        }

        l_new_holdvalue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

        if (NOMOVE == f_new_move_couch_method) {
            l_new_holdvalue &= (~VFC_HOLD_COUCH);

        } else {
            l_new_holdvalue |= (VFC_HOLD_COUCH);
        }

        SetMoveCouchForSeq(l_seq, f_new_move_couch_method, l_new_holdvalue);

        m_pqm_listctrl.InvalidateItemsForModeOption(l_item_to_apply, MOVE_COUCH_OPTION);
    }
}


//************************************Method Header************************************
// Method Name  : GetNewMethodForMoveCouch
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when move couch menu item is clicked.
//                It returns move couch method to apply as per input parameters.
//                If f_move_couch_method_old and f_move_couch_method_new(user click) methods
//                are same then we should make new method as NOMOVE,
//                otherwise we should apply f_move_couch_method_new method
//***********************************************************************************
MOVECOUCH_METHOD CPQMView::GetNewMethodForMoveCouch(
    const MOVECOUCH_METHOD f_move_couch_method_old,
    const MOVECOUCH_METHOD f_move_couch_method_new
) const
{
    return (f_move_couch_method_old == f_move_couch_method_new) ? NOMOVE : f_move_couch_method_new ;
}


BOOL CPQMView::CanProcessViewMsg(
    const MSG* const pMsg
) const
{
    if (pMsg) {

        switch (pMsg->message) {
            case WM_KEYDOWN:

                if (pMsg->wParam == VK_SPACE) {

                    return CanProcessButtonClick(::GetDlgCtrlID(pMsg->hwnd)) ;
                }
        }
    }

    return TRUE;
}

BOOL CPQMView::CanProcessButtonClick(
    const UINT f_id
) const
{

    switch (f_id) {

        case IDC_SCAN_START:
        case IDC_DELETE_BUTTON:
        case IDC_DUPLICATE_BUTTON:
        case IDC_PAUSE_RESUME:
        case IDC_ABORT:
        case IDC_QUEUE_AND_NEXT:
        case IDC_QUEUE_AND_EXIT:
        case IDC_NEXT_AND_COPY:
        case IDC_CANCEL:
        case IDC_END_EXQAM:
        case IDC_LOCATOR_BUTTON:
        case IDC_SCAN_EDIT_BUTTON:
        case IDC_RESET:
            return ((CWnd*)GetDlgItem(f_id))->IsWindowEnabled() ;
    }

    return TRUE ;
}

//************************************Method Header************************************
// Method Name  : CheckCoilForcibleMaxMode
// Author       : PATNI/ Pavan
// Purpose      : Check forcible max mode
//***********************************************************************************
BOOL CPQMView::CheckCoilForcibleMaxMode(int f_selected_index)
{
    LPCTSTR FUNC_NAME = _T("CCoilChannelMode::CheckCoilForcibleMaxMode");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CSequence* l_seq = NULL;

    int l_mode = -1;
    int l_acq_order = -1;

    l_seq = (CSequence*)m_pqm_listctrl.GetItemData(f_selected_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selected sequence pointer is NULL"));
        return FALSE;
    }

    l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

    VARIANT* pdata = new VARIANT;

    if (S_FALSE != m_acquisition->GetChannelModeSelection(l_acq_order, pdata)) {
        l_mode = pdata->intVal;

        if (1 == l_mode) { // this is not the case of forcible MAX so let it return from here
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Channel mode is already set to MAX mode, No need to check forcible mode"));
            DEL_PTR(pdata);
            return FALSE;
        }

        else { // If channel mode is normal mode

            if (S_FALSE != m_acquisition->GetNumSignalfromSelectedSection(l_acq_order, pdata)) { // Review comment-11-Jan-11
                if (-1 == pdata->intVal) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid channel mode value from VFC"));
                    DEL_PTR(pdata);
                    return FALSE;
                }

                int l_numsignal = pdata->intVal;
                DEL_PTR(pdata);

                if (l_numsignal > m_limited_channel)
                    return TRUE;

                else
                    return FALSE;
            }
        }

    }

    DEL_PTR(pdata);


    return FALSE;
}
/*************************************************************************************
//Module: CloseAllModalDialogs
//Author: PATNI/Sribanta
//Purpose: Close all the modal dialog opened by PQM application.
//**************************************************************************************/
void CPQMView::CloseAllModalDialogs()const
{

    CCOMPASS_SettingDialog* l_compas_setting_dialog = m_cds_controller->GetCOMPASS_SettingDlg();
    CCOMPASS_Location_Debug* l_compas_location_debug_dialog = m_cds_controller->GetCompasLocationDebugDialog();
    CPQMWirelessGating* l_wireless_gating_dialog = m_wirelessgating_cntlr.GetWirelessGatingDialog();

    if (l_compas_setting_dialog && ::IsWindow(l_compas_setting_dialog->m_hWnd)) {
        l_compas_setting_dialog->EndDialog(IDCANCEL);
    }

    if (l_compas_location_debug_dialog && ::IsWindow(l_compas_location_debug_dialog->m_hWnd)) {
        l_compas_location_debug_dialog->EndDialog(IDCANCEL);
    }

    if (m_dbdtcautiondialog && ::IsWindow(m_dbdtcautiondialog->m_hWnd)) {
        m_dbdtcautiondialog->EndDialog(IDCANCEL);
    }

    if (m_anatomy_dlg && ::IsWindow(m_anatomy_dlg->m_hWnd)) {
        m_anatomy_dlg->EndDialog(IDCANCEL);
    }

    if (m_obj_coil_dlg && ::IsWindow(m_obj_coil_dlg->m_hWnd)) {
        m_obj_coil_dlg->EndDialog(IDCANCEL);
    }

    if (m_setscanoffsetdlg && ::IsWindow(m_setscanoffsetdlg->m_hWnd)) {
        m_setscanoffsetdlg->EndDialog(IDCANCEL);
    }

    if (m_cfadlg && ::IsWindow(m_cfadlg->m_hWnd)) {
        m_cfadlg->EndDialog(IDCANCEL);
    }

    if (m_obj_gate_dlg && ::IsWindow(m_obj_gate_dlg->m_hWnd)) {
        m_obj_gate_dlg->EndDialog(IDCANCEL);
    }

    if (m_delay_time_dlg && ::IsWindow(m_delay_time_dlg->m_hWnd)) {
        m_delay_time_dlg->EndDialog(IDCANCEL);
    }

    if (l_wireless_gating_dialog && ::IsWindow(l_wireless_gating_dialog->m_hWnd)) {
        l_wireless_gating_dialog->EndDialog(IDCANCEL);
    }

}


/*************************************************************************************
//Module: ResetAutoScanTimer
//Author: PATNI/AKR
//Purpose: Reset Auto Scan Start timer and remove auto scan from first wait protocol
//**************************************************************************************/
void CPQMView::ResetAutoScanTimer()
{
    g_MainView->StopInjectTimerThread();
    m_acquisition->ResetAutoScanTimer();
    RemoevAutoScanFromFirstWaitItem();
}
/*************************************************************************************
//Module: DeleteWDFAMsgReceiver
//Author: PATNI/
//Purpose:
//**************************************************************************************/
void CPQMView::DeleteWDFAMsgReceiver() // Redmine-2275
{
    if (CWFDAMsgReceiver::GetWFDAMsgReceiver()) {

        CDPSManager::GetInstance()->Detach(CWFDAMsgReceiver::GetWFDAMsgReceiver());
        CWFDAMsgReceiver::DeleteWFDAMsgReceiver();
    }
}


//+Patni-Hemant/Added-2009May17/JFT#169
int CPQMView::GetPatientOrientationOptionFor(
    const OrientationType f_orientation_type,
    const int orientation_value
)const
{

    switch (f_orientation_type) {

        case OriType_PatientInsertDir :

            switch (orientation_value) {
                case  VFC_PATORI_HEAD_FIRST :
                    return (int) Patient_FeetFirst;

                case VFC_PATORI_FEET_FIRST :
                    return (int) Patient_HeadFirst;
            }

            break ;

            //+Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
        case OriType_PatientRotationDir :

            switch (orientation_value) {

                case VFC_PATORI_SUPINE:
                    return (int) PatientRotation_SUPINE ;

                case VFC_PATORI_PRONE:
                    return (int) PatientRotation_PRONE ;

                case VFC_PATORI_RIGHT_UP:
                    return (int) PatientRotation_RIGHT_UP ;

                case VFC_PATORI_LEFT_UP:
                    return (int) PatientRotation_LEFT_UP ;
            }

            break ;
            //-Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation

        case OriType_PatientViewDir :

            switch (orientation_value) {

                case VFC_VIEW_FROM_HEAD:
                    return (int)PatientView_FROM_HEAD ;
                    break;

                case VFC_VIEW_FROM_FEET:
                    return (int) PatientView_FROM_FEET ;
                    break;
            }

            break ;
    }

    return -1 ;
}
//-Patni-Hemant/Added-2009May17/JFT#169

//+Patni-HEMANT/2009Mar27/MODIFIED/ACE-1/ MCM0213-00131
//+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
bool CPQMView::GetBodyPartData(
    const int f_anatomy,
    const CString& f_anatomyString,
    SScanAantomy& f_sscanaantomy_to_return
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetBodyPartData");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_scan_anatomy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_scan_anatomy pointer is NULL"));

        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CScanAnatomyData* scananatomydata = m_scan_anatomy[f_anatomy - 1] ;

    if (scananatomydata) {

        POSITION pos = scananatomydata->GetHeadPosition();
        SScanAantomy l_scan_anatomy ;

        while (pos) {

            l_scan_anatomy = scananatomydata->GetNext(pos);

            if (l_scan_anatomy.m_scan_anatomy_name.CompareNoCase(f_anatomyString) == 0) {

                f_sscanaantomy_to_return = l_scan_anatomy ;
                return true ;
            }
        }
    }

    return false;
}
//-Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
//-Patni-HEMANT/2009Mar27/MODIFIED/ACE-1/ MCM0213-00131


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CPQMView::GetSarString(
    const int sar_anatomy
)const
{

    CString sar_anatomy_string = _T("");

    switch (sar_anatomy) {

        case HEAD:
            sar_anatomy_string = _T("HEAD");
            break ;

        case C_SPINE:
            sar_anatomy_string = _T("C_SPINE");
            break ;

        case TL_SPINE:
            sar_anatomy_string = _T("TL_SPINE");
            break ;

        case CHEST:
            sar_anatomy_string = _T("CHEST");
            break ;

        case ABDOMEN:
            sar_anatomy_string = _T("ABDOMEN");
            break ;

        case PELVIS:
            sar_anatomy_string = _T("PELVIS");
            break ;

        case SHOULDER:
            sar_anatomy_string = _T("SHOULDER");
            break ;

        case EXTREMITY:
            sar_anatomy_string = _T("EXTREMITY");
            break ;

            //case EXTREMITY2:
            //	sar_anatomy_string = _T("EXTREMITY2");
            //	break ;

        case HAND:
            sar_anatomy_string = _T("HAND");
            break ;

        case ANKLE:
            sar_anatomy_string = _T("ANKLE");
            break ;
    }

    return sar_anatomy_string ;
}


//************************************Method Header************************************
// Method Name  : GetAutoVoiceOptionForID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
AUTOVOICE_OPTIONS CPQMView::GetAutoVoiceOptionForID(
    const UINT menu_item_id,
    AUTOVOICE_PRESCAN_OPTION& f_prescan_aut_voi_optn
)
{
    //+Patni-PJS/2010Jan19/Added+Modified/Phase 3 MVC004363
    switch (menu_item_id) {

        case ID_MENUITEM_N_N :
            f_prescan_aut_voi_optn = NONE;
            return NONE_NONE ;

        case ID_MENUITEM_N_B1 :
            f_prescan_aut_voi_optn = NONE;
            return NONE_B1 ;

        case ID_MENUITEM_N_B2 :
            f_prescan_aut_voi_optn = NONE;
            return NONE_B2 ;

        case ID_MENUITEM_N_B3 :
            f_prescan_aut_voi_optn = NONE;
            return NONE_B3 ;

        case ID_MENUITEM_N_M1 :
            f_prescan_aut_voi_optn = NONE;
            return NONE_MOTION ;

        case ID_MENUITEM_B1_N :
            f_prescan_aut_voi_optn = B1;
            return B1_NONE;

        case ID_MENUITEM_B1_B1 :
            f_prescan_aut_voi_optn = B1;
            return B1_B1;

        case ID_MENUITEM_B1_B2 :
            f_prescan_aut_voi_optn = B1;
            return B1_B2;

        case ID_MENUITEM_B1_B3 :
            f_prescan_aut_voi_optn = B1;
            return B1_B3;

        case ID_MENUITEM_B1_M1 :
            f_prescan_aut_voi_optn = B1;
            return B1_MOTION;

        case ID_MENUITEM_B2_N :
            f_prescan_aut_voi_optn = B2;
            return B2_NONE;

        case ID_MENUITEM_B2_B1 :
            f_prescan_aut_voi_optn = B2;
            return B2_B1;

        case ID_MENUITEM_B2_B2 :
            f_prescan_aut_voi_optn = B2;
            return B2_B2;

        case ID_MENUITEM_B2_B3 :
            f_prescan_aut_voi_optn = B2;
            return B2_B3;

        case ID_MENUITEM_B2_M1 :
            f_prescan_aut_voi_optn = B2;
            return B2_MOTION;

        case ID_MENUITEM_B3_N :
            f_prescan_aut_voi_optn = B3;
            return B3_NONE;

        case ID_MENUITEM_B3_B1 :
            f_prescan_aut_voi_optn = B3;
            return B3_B1;

        case ID_MENUITEM_B3_B2 :
            f_prescan_aut_voi_optn = B3;
            return B3_B2;

        case ID_MENUITEM_B3_B3 :
            f_prescan_aut_voi_optn = B3;
            return B3_B3;

        case ID_MENUITEM_B3_M1 :
            f_prescan_aut_voi_optn = B3;
            return B3_MOTION;

        case ID_MENUITEM_M1_N :
            f_prescan_aut_voi_optn = MOTION;
            return MOTION_NONE ;

        case ID_MENUITEM_M1_B1 :
            f_prescan_aut_voi_optn = MOTION;
            return MOTION_B1;

        case ID_MENUITEM_M1_B2 :
            f_prescan_aut_voi_optn = MOTION;
            return MOTION_B2;

        case ID_MENUITEM_M1_B3 :
            f_prescan_aut_voi_optn = MOTION;
            return MOTION_B3;

        case ID_MENUITEM_M1_M1 :
            f_prescan_aut_voi_optn = MOTION;
            return MOTION_MOTION;
    }

    //-Patni-PJS/2010Jan19/Added/Phase 3 MVC004363

    //    ASSERT(FALSE);	//we should not reach here..
    return AUTOVOICE_OPTIONS_INVALID;
}


//+Patni-Abhishek/2010Dec14/Modified/IR-180
//************************************Method Header**************************************
// Method Name  : GetMenuIdFromAutoVoiceOption
// Author       : PATNI/ Abhishek
// Purpose      : This function is used to get the ID of the menu based on auto voice option
//***************************************************************************************
UINT CPQMView::GetMenuIdFromAutoVoiceOption(
    const AUTOVOICE_OPTIONS a_AutoVoiceOption
)
{
    switch (a_AutoVoiceOption) {

        case NONE_NONE :

            return ID_MENUITEM_N_N;

        case NONE_B1 :

            return ID_MENUITEM_N_B1;

        case NONE_B2 :

            return ID_MENUITEM_N_B2;

        case NONE_B3 :

            return ID_MENUITEM_N_B3;

        case NONE_MOTION :

            return ID_MENUITEM_N_M1;

        case B1_NONE :

            return ID_MENUITEM_B1_N;

        case B1_B1 :

            return ID_MENUITEM_B1_B1;

        case B1_B2 :

            return ID_MENUITEM_B1_B2;

        case B1_B3 :

            return ID_MENUITEM_B1_B3;

        case B1_MOTION :

            return ID_MENUITEM_B1_M1;

        case B2_NONE :

            return ID_MENUITEM_B2_N;

        case B2_B1 :

            return ID_MENUITEM_B2_B1;

        case B2_B2 :

            return ID_MENUITEM_B2_B2;

        case B2_B3 :

            return ID_MENUITEM_B2_B3;

        case B2_MOTION :

            return ID_MENUITEM_B2_M1;

        case B3_NONE :

            return ID_MENUITEM_B3_N;

        case B3_B1 :

            return ID_MENUITEM_B3_B1;

        case B3_B2 :

            return ID_MENUITEM_B3_B2;

        case B3_B3 :

            return ID_MENUITEM_B3_B3;

        case B3_MOTION :

            return ID_MENUITEM_B3_M1;

        case MOTION_NONE :

            return ID_MENUITEM_M1_N;

        case MOTION_B1 :

            return ID_MENUITEM_M1_B1;

        case MOTION_B2 :

            return ID_MENUITEM_M1_B2;

        case MOTION_B3 :

            return ID_MENUITEM_M1_B3;

        case MOTION_MOTION :

            return ID_MENUITEM_M1_M1;

        default:

            return ID_MENUITEM_N_N;
    }

    return ID_MENUITEM_N_N;
}
//-Patni-Abhishek/2010Dec14/Modified/IR-180

//************************************Method Header************************************
// Method Name  : GetScanAnatomyDataFor
// Author       : PATNI/
// Purpose      : Returns scan anatomy data
//***********************************************************************************
bool CPQMView::GetScanAnatomyDataFor(
    const int f_sar_index,
    const int f_scan_item_listindex,
    SScanAantomy& scan_anatomy_data
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetScanAnatomyDataFor");

    //+++Added SM3_Mangesh_Start
    //For checking initial limits on Array of list .Since List index starts from 0;
    if (f_sar_index <= 0) {
        return false;
    }

    //+++Added SM3_Mangesh_End

    //+Patni-HEMANT/ADDED On 4/9/2009 7:18:41 PM/ ACE MCM0213-00131
    //
    //If we haven't found any data for scan anatomy, apply the sar anatomy
    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_scan_anatomy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_scan_anatomy pointer is NULL"));

        return false;
    }//-Patni-DKH/2009Aug24/Modified/cpp test corrections

    if (m_scan_anatomy[f_sar_index - 1]->GetCount() == 0) {

        scan_anatomy_data.m_scan_anatomy_vf = f_sar_index;
        scan_anatomy_data.m_scan_anatomy_name = GetSarString(f_sar_index);
        return true ;
    }

    if (f_scan_item_listindex < m_scan_anatomy[f_sar_index - 1]->GetCount()) {
        //
        //-Patni-HEMANT/ ADDED On4/9/2009 7:18:45 PM/ ACE1 MCM0213-00131

        POSITION pos = m_scan_anatomy[f_sar_index - 1]->FindIndex(f_scan_item_listindex);

        if (pos) {

            scan_anatomy_data = m_scan_anatomy[f_sar_index - 1]->GetAt(pos);
            return true ;
        }
    }

    return false ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMView::GetNextWSEEditProt(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetNextWSEEditProt");

    VARIANT* pData = new VARIANT;

    //+Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    if (pData == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("VARIANT pData is NULL"));
        return -1;
    }

    //-Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK

    pData->intVal = -1;
    m_acquisition->GetCurrentWSEEditProt(pData);

    if (pData->intVal != -1) {

        //we have found the acq oder.. so get the protocol index for acq oder

        POSITION pos = m_list->GetHeadPosition();
        int return_index = -1 ;
        int l_scan_index = -1;

        if (pos) {

            return_index = 0;

            while (pos) {
                CSequence* pseq = m_list->GetNext(pos);

                l_scan_index = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

                if (l_scan_index == pData->intVal) {

                    //Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
                    DEL_PTR(pData)

                    return return_index;
                }

                return_index++ ;
            }
        }
    }

    //Patni-AMT/2010Jan29/Added/Ph-3# PROPOSAL_01-MEM_LEAK
    DEL_PTR(pData)

    return -1 ;
}

//************************************Method Header****************************
// Method Name  : GetConnectCoil()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CPQMView::GetConnectCoil(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::GetConnectCoil");

    VARIANT* pData = new VARIANT;
    m_acquisition->GetConnectCoilInfo(pData);
    VftCoilListInfo_t* l_connect_coil = (VftCoilListInfo_t*)pData->byref;

    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    m_selection_utiliy->SetConnectCoil(l_connect_coil);

    dlgCoilSelWin->DisplayCoil();

    //Patni-HAR/2009April15/Added/Memory Leak
    DEL_PTR(pData)
}


//+Patni-ARD/2010Jan21/Added/DeFT# Code Review Comment
//************************************Method Header************************************
// Method Name  : GetMinMaxZValue
// Author       : PATNI/ARD
// Purpose      : Get Min and Max Z offset value
//***********************************************************************************
void CPQMView::GetMinMaxZValue(
    float& f_min_z,
    float& f_max_z
)
{

    const int l_index = GetSelectedSeqIndex();

    if (!IsValidSequenceIndex(l_index)) {
        return ;
    }

    int l_in_side_limit = -1, l_out_side_limit = -1;
    BOOL l_enable = -1;

    m_acquisition->GetInsideLimit(g_MainView->GetAcqOderForGivenProtocolIndex(l_index), &l_in_side_limit,
                                  &l_out_side_limit, &l_enable);

    if (l_enable == TRUE) {

        f_min_z = -(l_in_side_limit / 10.0f);
        f_max_z = l_out_side_limit / 10.0f;

    } else {
        f_min_z = (float)(-10.0);
        f_max_z = (float)(10.0);
    }
}
//-Patni-ARD/2010Jan21/Added/DeFT# Code Review Comment


//+Patni-SS/2009Nov16/Added/IR#90/SFT Menu
//************************************Method Header************************************
// Method Name  : GetStudyfileName
// Author       : PATNI/ SS
// Purpose      : Get Study file name and path (.\..\RunXXX.XXX).
//***********************************************************************************
void CPQMView::GetStudyfileName(
    CString& f_studyfilepathname
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetStudyfileName");

    VARIANT* pData = new VARIANT;
    m_acquisition->GetStudyPath(pData);

    f_studyfilepathname = pData->bstrVal;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , f_studyfilepathname);

    DEL_PTR(pData)
}


//************************************Method Header************************************
// Method Name  : GetContrastAgentName
// Author       : PATNI/ARD
// Purpose      : Read Contrast Agent Name from config file
//***********************************************************************************
//+Patni-ARD/2010Jan6/Modified/DeFT# IR 100
void CPQMView::GetContrastAgentName(SIPCConfigInfo* f_sIPCConfigInfo
                                   )const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetContrastAgentName");

    PQM_TRACE(USER_FUNC_MARKER,
              FUNC_NAME,
              _T("Trace of GetContrastAgentName"));

    ContrastAgent l_contrast_agent;

    const int l_contrast_agent_list_count = f_sIPCConfigInfo->contrast_agent_list.GetSize();

    for (int l_index = 0; l_index < l_contrast_agent_list_count; l_index++) {
        //Patni-ARD/2010Apr27/Modified/JaFT# IR100_CDR_04
        l_contrast_agent.m_index    = l_index + 1;
        l_contrast_agent.m_strname  = f_sIPCConfigInfo->contrast_agent_list.GetAt(l_index);

        m_contrast_agent->AddTail(l_contrast_agent);
    }
}
//-Patni-ARD/2010Jan6/Modified/DeFT# IR 100


//************************************Method Header************************************
// Method Name  : GetScanAnatomyData
// Author       : PATNI/Mangesh
// Purpose      : Read Scan Anatomy from config file
//***********************************************************************************
void CPQMView::GetScanAnatomyData(
)
{
    VARIANT* pData = new VARIANT;
    //pData->pparray = m_scan_anatomy;
    //m_scan_anatomy->RemoveAll();
    pData->byref = &m_scan_anatomy[0] ;
    GetAcquisition()->GetScanAnatomy(&pData);
    //m_scan_anatomy = (CScanAnatomyData*)pData->byref;

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }

}

//************************************Method Header************************************
// Method Name  : GetContrastAgentFromList
// Author       : PATNI/
// Purpose      : Retrives required contrast agent from the list
//***********************************************************************************
bool CPQMView::GetContrastAgentFromList(
    ContrastAgent& contrastagent,
    const int index
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMView::GetContrastAgentFromList");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_contrast_agent) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_contrast_agent pointer is NULL"));

        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    const int total_contrast_agents = m_contrast_agent->GetCount() ;

    if (!((total_contrast_agents) && (index >= 0 && index < total_contrast_agents)))
        return false ;

    contrastagent = m_contrast_agent->GetAt(m_contrast_agent->FindIndex(index));

    return true ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMView::GetListForGivenProtocolAcqOder(
    const int f_protocol_AcqOder_index
)const
{
    POSITION pos = m_list->GetHeadPosition();

    if (!pos)
        return -1 ;

    int protocol_list_index = 0;

    int l_acq_order = -1;
    CSequence* l_seq = NULL ;

    while (pos) {

        l_seq = m_list->GetNext(pos);

        if (l_seq && l_seq->GetCPASCOMSequence() && l_seq->GetCPASCOMSequence()->GetSequenceParam()) {

            l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

            if (f_protocol_AcqOder_index == l_acq_order) {
                //acqorder match , so return the protocol_list index
                return protocol_list_index;
            }
        }

        protocol_list_index++ ;
    }

    return -1 ;

}

//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
//************************************Method Header************************************
// Method Name  : GetScanAnatomyAndLateralityIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::GetScanAnatomyAndLateralityIndex(
    const int l_laterality_menu_index,
    int& l_scan_anatomy,
    LateralityEnum& l_laterality
)const
{
    l_scan_anatomy = -1 ;
    l_laterality = Laterality_Invalid ;

    if ((l_laterality_menu_index >= ID_LATERALITY_MENU_MIN) &&
        (l_laterality_menu_index <= ID_LATERALITY_MENU_MAX)) {

        const int l_laterality_menu = l_laterality_menu_index - ID_LATERALITY_MENU_MIN ;

        //+Patni-Hemant/2010May25/Modified/MaFT/MVC007915
        //l_scan_anatomy = (l_laterality_menu /3);
        //l_laterality = (LateralityEnum)(l_laterality_menu % 3) ;
        l_scan_anatomy = (l_laterality_menu / g_laterality_menuitems);
        l_laterality = (LateralityEnum)(l_laterality_menu % g_laterality_menuitems) ;
        //+Patni-Hemant/2010May25/Modified/MaFT/MVC007915
    }

}
//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
//************************************Method Header************************************
// Method Name  : GetScanCommentXMLData
// Author       : PATNI/ AJS
// Purpose      :
//*************************************************************************************
void CPQMView::GetScanCommentXMLData(
)
{
    VARIANT* pData = new VARIANT;


    pData->byref = &m_commentdata;
    GetAcquisition()->GetScanCommentXMLData(&pData);

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163



//************************************Method Header************************************
// Method Name  : SetButtonAppearance
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::SetButtonAppearance(
)
{

    SetButtonImages();
    SetButtonCurve();
    SetButtonFont();
    SetColorForResetButton();

    BUTTON_COLORS_t stColors_o ;
    m_scan_start_button.GetButtonColors(stColors_o);
    stColors_o.crPrimaryCaptionNormal = RGB(245, 195, 75);
    m_scan_start_button.SetButtonColors(stColors_o);

    m_inject_timer_button.SetButtonColors(stColors_o);

    //This is the default button appearnce for the dialogs of PQM..
    CCustomStatic::SetDefaultTextColor(g_pqmdialog_static_ctrl_text_color);
    CCustomStatic::SetDefaultFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
    //+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal

    //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    //	CCustomStatic* static_ctrl[4] = { &m_scan_offset_static , &m_coil_info_static , &m_scan_anatomy_static , &m_sar_anatomy_static};
    //	for(int static_ctrl_index = 0; static_ctrl_index < 4; static_ctrl_index++ ){
    CCustomStatic* static_ctrl[2] = { &m_scan_offset_static , &m_coil_info_static};

    for (int static_ctrl_index = 0; static_ctrl_index < 2; static_ctrl_index++) {
        //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

        //-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal

        static_ctrl[static_ctrl_index]->SetColorData(g_pqm_static_bkcolor, g_pqm_static_textcolor);
        static_ctrl[static_ctrl_index]->SetFontAttributes(pqm_static_font_name, pqm_static_font_size);
        static_ctrl[static_ctrl_index]->UpdateFont();
    }

    m_remaining_time_static.SetColorData(g_pqm_static_bkcolor, g_pqmdialog_static_ctrl_text_color);

    //Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    //m_remaining_time_static.SetFontAttributes(_T("Arial"), 140);
    m_remaining_time_static.SetFontAttributes(_T("Time New Roman"), 175);

    m_remaining_time_static.UpdateFont();


    //+Patni-Hemant/2009Nov17/Added/IR#90/Bottom Buttons
    m_sar_anatomy_static.SetColorData(g_pqm_static_bkcolor, g_pqm_static_textcolor);
    m_sar_anatomy_static.SetFontAttributes(_T("Verdana Bold"), 90);
    m_sar_anatomy_static.UpdateFont();
    //-Patni-Hemant/2009Nov17/Added/IR#90/Bottom Buttons

    return TRUE ;
}

//************************************Method Header************************************
// Method Name  : SetButtonImages
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::SetButtonImages(
)
{
    SetBitmapForButton(m_close_button, IDB_CLOSE_BUTTON_IMAGE);
    m_close_button.SetWindowText(_T(""));

    SetBitmapForButton(m_injecttime_button, IDB_INJECTTIME_BUTTON_IMAGE);
    m_injecttime_button.SetWindowText(_T(""));

    SetBitmapForButton(m_pause_resume_button, IDB_PAUSE_BUTTON_IMAGE);
    m_pause_resume_button.SetWindowText(_T(""));

    //Patni-Hemant/2009Nov12/Modified/IR#90 / PQM Queue Top Buttons
    //SetBitmapForButton(m_utility_button, IDB_UTILITY_BUTTON_IMAGE,false);
    //+Patni-HAR/2009Jan8/Added/MVC006380
    SetIconForButton(m_utility_button, IDI_UTILITY_BUTTON_IMAGE);

    m_utility_button.SetWindowText(_T(""));

    //Patni-Hemant/2009Nov12/Modified/IR#90 / PQM Queue Top Buttons
    //SetBitmapForButton(m_pass_button, IDB_PASS_BUTTON_IMAGE);
    //+Patni-HAR/2009Jan8/Added/MVC006380
    SetIconForButton(m_pass_button, IDI_PASS_BUTTON_IMAGE);

    m_pass_button.SetWindowText(_T(""));


    //Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    //SetBitmapForButton(m_delete_button, IDB_DELETE_BUTTON_IMAGE);
    //+Patni-HAR/2009Jan8/Added/MVC006380
    SetIconForButton(m_delete_button, IDI_DELETE_BUTTON_IMAGE);

    m_delete_button.SetWindowText(_T(""));

    //Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    //SetBitmapForButton(m_duplicate_button, IDB_ADD_BUTTON_IMAGE);
    //+Patni-HAR/2009Jan8/Added/MVC006380
    SetIconForButton(m_duplicate_button, IDI_ADD_BUTTON_IMAGE);

    m_duplicate_button.SetWindowText(_T(""));

    //Patni-Hemant/2009Nov12/Modified/IR#90
    //SetBitmapForButton(m_locator_button, IDB_LOCATOR_BUTTON_IMAGE, false);
    SetBitmapForButton(m_locator_button, IDB_LOCATOR_BUTTON_IMAGE);

    m_locator_button.SetWindowText(_T(""));

    //Patni-Hemant/2009Nov12/Modified/IR#90 / PQM Queue Top Buttons
    //SetBitmapForButton(m_scanedit_button, IDB_SCANEDIT_BUTTON_IMAGE,false);
    //+Patni-HAR/2009Jan8/Added/MVC006380
    SetIconForButton(m_scanedit_button, IDI_SCANEDIT_BUTTON_IMAGE);
    m_scanedit_button.SetWindowText(_T(""));

    //Patni-Hemant/2009Nov12/Modified/IR#90
    //SetBitmapForButton(m_scan_anatomy_button, IDB_SCAN_ANATOMY_BITMAP, false);
    SetBitmapForButton(m_scan_anatomy_button, IDB_SCAN_ANATOMY_BITMAP, g_default_mask, false);

    m_scan_anatomy_button.SetWindowText(_T(""));

    CRect button_rect;
    m_scan_anatomy_button.GetClientRect(&button_rect);

    m_scan_anatomy_button.SetPicturePos(button_rect.Width() - 25, -2);

    //+Patni-HAR/2009Jan8/Added/MVC006380
    SetIconForButton(m_scan_offset_button, IDI_SCANOFFSET_BUTTON_IMAGE);

    return TRUE ;

}


//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::SetButtonFont(
)
{

    LPCTSTR button_font_name = _T("MS Reference Sans Serif") ;
    const int l_scan_button_size = 16 ;

    m_end_exam_button.SetPrimaryTextFont(button_font_name, l_scan_button_size);
    m_end_exam_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_abort_button.SetPrimaryTextFont(button_font_name, l_scan_button_size);
    m_abort_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_close_button.SetPrimaryTextFont(button_font_name, l_scan_button_size);
    m_close_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_pause_resume_button.SetPrimaryTextFont(button_font_name, l_scan_button_size);
    m_pause_resume_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_scan_start_button.SetPrimaryTextFont(button_font_name, l_scan_button_size);
    m_scan_start_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    button_font_name = _T("MS Reference Sans Serif") ;
    const int l_non_scan_btnsize = 8 ;
    const bool bold_flag = true ;

    m_utility_button.SetPrimaryTextFont(button_font_name, l_non_scan_btnsize, bold_flag);
    m_utility_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_RIGHT_VCENTER);
    m_pass_button.SetPrimaryTextFont(button_font_name, l_non_scan_btnsize, bold_flag);
    m_pass_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    //+Patni-Hemant/2009Nov12/Commented/IR#90/Bottom Buttons
    //m_scan_offset_button.SetPrimaryTextFont(button_font_name, l_non_scan_btnsize, bold_flag);
    //m_scan_offset_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    //-Patni-Hemant/2009Nov12/Commented/IR#90/Bottom Buttons

    m_coil_button.SetPrimaryTextFont(button_font_name, l_non_scan_btnsize, bold_flag);
    m_coil_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_scan_anatomy_button.SetPrimaryTextFont(button_font_name, l_non_scan_btnsize, bold_flag);
    m_scan_anatomy_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_LEFT_VCENTER);


    return TRUE ;
}


//************************************Method Header************************************
// Method Name  : SetButtonCurve
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::SetButtonCurve(
)
{
    const CURVE_SIZE_e curve_type_to_set = CURVE_SMALL_3 ;	//the other option is onlt CURVE_LARGE_5
    m_end_exam_button.SetCurveType(curve_type_to_set);
    m_abort_button.SetCurveType(curve_type_to_set);
    m_close_button.SetCurveType(curve_type_to_set);
    m_pause_resume_button.SetCurveType(curve_type_to_set);
    m_scan_start_button.SetCurveType(curve_type_to_set);

    m_utility_button.SetCurveType(curve_type_to_set);
    m_pass_button.SetCurveType(curve_type_to_set);

    m_scanedit_button.SetCurveType(curve_type_to_set);
    m_locator_button.SetCurveType(curve_type_to_set);

    m_duplicate_button.SetCurveType(curve_type_to_set);
    m_delete_button.SetCurveType(curve_type_to_set);
    m_scan_offset_button.SetCurveType(curve_type_to_set);
    m_coil_button.SetCurveType(curve_type_to_set);
    m_scan_anatomy_button.SetCurveType(curve_type_to_set);

    //Patni-Hemant/2009Nov16/Added/IR#90/Bottom Buttons
    m_scan_sar_button.SetCurveType(curve_type_to_set);

    return TRUE ;
}


//************************************Method Header************************************
// Method Name  : SetStatesForUtilityMenuItems
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetStatesForUtilityMenuItems(
    PopupMenuEx* utility_menu
)
{
    //REDMINE-781
    if (NULL == utility_menu) {
        return ;
    }


    //for auto plan..
    UINT flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) && m_pqm_g.GetAutoPlanFlag()) ? MF_CHECKED : MF_UNCHECKED ;

    //+Patni-ARD/2009Oct5/Modified/Defect Fix# MVC5149
#ifdef PHASE1
    utility_menu->CheckMenuItem(ID_MENUITEM_AUTO_PLAN, MF_BYCOMMAND | MF_UNCHECKED);
    utility_menu->EnableMenuItem(ID_MENUITEM_AUTO_PLAN, MF_BYCOMMAND | MF_GRAYED);
#else
    utility_menu->CheckMenuItem(ID_MENUITEM_AUTO_PLAN, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_AUTO_PLAN, MF_BYCOMMAND | MF_GRAYED);

#endif
    //-Patni-ARD/2009Oct5/Modified/Defect Fix# MVC5149

    //for plan scan..
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) &&  m_pqm_g.GetPlanScanFlag()) ? MF_CHECKED : MF_UNCHECKED;

    //+Patni-ARD/2009Oct13/Modified/Defect Fix# MSA0248-00034
    utility_menu->CheckMenuItem(ID_MENUITEM_PLAN_SCAN, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_PLAN_SCAN, MF_BYCOMMAND | MF_GRAYED);

    //-Patni-ARD/2009Oct13/Modified/Defect Fix# MSA0248-00034

    //for skip rgn..
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) &&  m_pqm_g.GetSkipRgnFlag()) ? MF_CHECKED : MF_UNCHECKED;
    utility_menu->CheckMenuItem(ID_MENUITEM_SKIP_RGN, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_SKIP_RGN, MF_BYCOMMAND | MF_GRAYED);

    //for All Coverage RgnFlag
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) && m_pqm_g.GetAllCoverageRgnFlag()) ? MF_CHECKED : MF_UNCHECKED;
    utility_menu->CheckMenuItem(ID_MENUITEM_ALL_COV_RGN, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_ALL_COV_RGN, MF_BYCOMMAND | MF_GRAYED);


    //for Speeder map AutoVoiceFlag
    if ((m_pqm_g.GetPQMMode() != K_IMAGING)) {
        utility_menu->DeleteMenu(ID_MENUITEM_SPEEDERMAP_AUTOVOICE, MF_BYCOMMAND);

    } else {

        flag = (m_pqm_g.GetSpeedermapAutoVoiceFlag()) ? MF_CHECKED : MF_UNCHECKED;
        utility_menu->CheckMenuItem(ID_MENUITEM_SPEEDERMAP_AUTOVOICE, MF_BYCOMMAND | flag);
    }

    if (IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_SPEEDERMAP_AUTOVOICE, MF_BYCOMMAND | MF_GRAYED);

    //for Multi Coverage Caution Flag
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) && m_pqm_g.GetMultiCovCautionFlag()) ? MF_CHECKED : MF_UNCHECKED ;
    utility_menu->CheckMenuItem(ID_MENUITEM_MULTI_COV_CAUTION, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_MULTI_COV_CAUTION, MF_BYCOMMAND | MF_GRAYED);

    //for Batch Setting Flag
    flag = m_pqm_g.GetBatchSettingFlag() ? MF_CHECKED : MF_UNCHECKED ;
    utility_menu->CheckMenuItem(ID_MENUITEM_BATCH_COIL_AND_SAR_RGN, MF_BYCOMMAND | flag);

    if (IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_BATCH_COIL_AND_SAR_RGN, MF_BYCOMMAND | MF_GRAYED);

    //for Prep Wb Coil
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) && m_pqm_g.GetPrepWbCoilFlag()) ? MF_CHECKED : MF_UNCHECKED ;
    utility_menu->CheckMenuItem(ID_MENUITEM_PREP_WB_COIL, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_PREP_WB_COIL, MF_BYCOMMAND | MF_GRAYED);

    //for Shimm WbCoil
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) && m_pqm_g.GetShimmWbCoilFlag()) ? MF_CHECKED : MF_UNCHECKED ;
    utility_menu->CheckMenuItem(ID_MENUITEM_SHIMMING_WB_COIL, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_SHIMMING_WB_COIL, MF_BYCOMMAND | MF_GRAYED);

    //for Locator Image Move
    flag = ((m_pqm_g.GetPQMMode() == K_IMAGING) &&  m_pqm_g.GetLocatorMoveFlag()) ? MF_CHECKED : MF_UNCHECKED ;
    utility_menu->CheckMenuItem(ID_MOVING_TO_LOCATOR_IMAGE, MF_BYCOMMAND | flag);

    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MOVING_TO_LOCATOR_IMAGE, MF_BYCOMMAND | MF_GRAYED);

    //Added by Hemant On 11/27/2008 11:38:54 AM
    if (m_pqm_g.GetPQMMode() != K_IMAGING || IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_MENUITEM_CENTER_FREQ_CTRL, MF_BYCOMMAND | MF_GRAYED);

    if ((m_pqmmanualprescandlg && m_pqmmanualprescandlg->IsWindowVisible()) || IsAMBInvoked()) {
        utility_menu->EnableMenuItem(ID_MENUITEM_MANUAL_PRESCAN, MF_BYCOMMAND | MF_GRAYED);
    }

    if (IsAMBInvoked())
        utility_menu->EnableMenuItem(ID_UTILITY_MENU_AS_COMPASS_SETTING, MF_BYCOMMAND | MF_GRAYED);


    if (!m_pqm_g.GetWirelessSystemTypeFlag() || IsAMBInvoked()) {

        utility_menu->RemoveMenu(ID_WIRELESS_GATING_MENU, MF_BYCOMMAND);
    }

    if (ShouldDisableGATEFunc()) {

        utility_menu->EnableMenuItem(ID_MENUITEM_GATE_MODE_ECG, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_GATE_MODE_PERIPHERAL, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_GATE_MODE_RESPIRATORY, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_MANUAL_PRESCAN, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_WIRELESS_GATING_MENU, MF_BYCOMMAND | MF_GRAYED);
    }

    //Patni-MSN/2010Mar11/Added/IR-86, PQM Debug Window

    if (!((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(VK_SHIFT) < 0))) {
        utility_menu->RemoveMenu(utility_menu->GetMenuItemCount() - 2, MF_BYPOSITION);
    }

    if (!((GetAsyncKeyState(VK_CONTROL) < 0) && (GetAsyncKeyState(VK_SHIFT) < 0))) {
        utility_menu->RemoveMenu(utility_menu->GetMenuItemCount() - 1, MF_BYPOSITION);
    }


    //+Patni-Hemant/2011Jan11/Added/REDMINE-1095_Reopen
    // disable LeadType Menus when acqman is not connected
    //+Patni-Manish/2010Jan12/Added/MVC006524/Disable Electrode Menu
    if (!IsAcqumanReadyForNextRequest() || IsScanning() || ShouldDisableLeadType() || IsAMBInvoked()) {
        utility_menu->EnableMenuItem(ID_MENUITEM_ELECTRODE_I, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_ELECTRODE_II, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_ELECTRODE_III, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_ELECTRODE_AVR, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_ELECTRODE_AVL, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_ELECTRODE_AVF, MF_BYCOMMAND | MF_GRAYED);
    }

    //-Patni-Manish/2010Jan12/Added/MVC006524/Disable Electrode Menu
    //-Patni-Hemant/2011Jan11/Added/REDMINE-1095_Reopen

    if (!IsAcqumanReadyForNextRequest() || IsAMBInvoked()) {

        //Patni-Hemant/2011Jan11/Added/REDMINE-1095_Reopen
        //disable APC Pause when acqman is not connected
        utility_menu->EnableMenuItem(ID_MENUITEM_APC_PAUSE, MF_BYCOMMAND | MF_GRAYED);

        utility_menu->EnableMenuItem(ID_MENUITEM_APC_RETRY, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_GATE_MODE_ECG, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_GATE_MODE_PERIPHERAL, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_GATE_MODE_RESPIRATORY, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_MENUITEM_CENTER_FREQ_CTRL, MF_BYCOMMAND | MF_GRAYED);
        utility_menu->EnableMenuItem(ID_WIRELESS_GATING_MENU, MF_BYCOMMAND | MF_GRAYED);

    } else {

        //followings are the enable disable conditions for APC_Pause, APC_Retry..
        //bothe items are disabled when no protocol is present..
        bool enable_apc_pause = true;
        bool enable_apc_retry = true;

        //+Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681
        int l_first_selected_item = m_pqm_listctrl.GetFirstSelectedItem() ;

        if (ShouldDisableAPCFunc(l_first_selected_item)) {

            enable_apc_pause = false;
            enable_apc_retry = false;

        } else {

            CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_first_selected_item);

            if (l_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) {

                enable_apc_pause = false;
                enable_apc_retry = false;

            } else {

                if (CanAPCPause() == APC_EnableFromPQM) {
                    enable_apc_pause = true;
                    enable_apc_retry = false;

                } else {

                    //check weather prev seq has differnt SAR or coil..then enable apc pause ..
                    //else enable apc retry
                    if (!IsValidSequenceIndex(l_first_selected_item - 1)) {

                        enable_apc_pause = true;
                        enable_apc_retry = false;

                    } else {

                        CSequence* l_seq_prev = (CSequence*) m_pqm_listctrl.GetItemData(l_first_selected_item - 1);
                        CPqmPrMode l_prmode_prev = l_seq_prev->GetPrMode();
                        CPqmPrMode l_prmode_first_wait = l_seq->GetPrMode();

                        if ((l_prmode_prev.m_anatomy != l_prmode_first_wait.m_anatomy) ||
                            (l_prmode_prev.m_recv_coil_label.CompareNoCase(l_prmode_first_wait.m_recv_coil_label) != 0)
                           ) {

                            enable_apc_pause = true;
                            enable_apc_retry = false;

                        } else {
                            enable_apc_pause = false;
                            enable_apc_retry = true;
                        }
                    }
                }
            }
        }

        if (!enable_apc_pause) {
            //when its gray ,don't show it checked , as per vantage..
            utility_menu->EnableMenuItem(ID_MENUITEM_APC_PAUSE, MF_BYCOMMAND | MF_GRAYED);

        } else {
            flag = GetAPCPauseChecked() ? MF_CHECKED : MF_UNCHECKED ;
            utility_menu->CheckMenuItem(ID_MENUITEM_APC_PAUSE, MF_BYCOMMAND | flag);
        }

        if (!enable_apc_retry)
            utility_menu->EnableMenuItem(ID_MENUITEM_APC_RETRY, MF_BYCOMMAND | MF_GRAYED);

        //-Patni-HEMANT/ADDED On 9/12/2009 8:56:37 PM/ MVC004681
    }

    if (m_pqm_g.GetPQMMode() != K_SCHEDULING && SARCTRL_LICENCE_ON == GetSARControlLicenseAtStartUp()) {

        const long l_button_state = MF_BYCOMMAND | m_sar_connect_status ? 0 : MF_GRAYED;
        utility_menu->EnableMenuItem(ID_UTILITY_SARMANAGER, l_button_state);

    } else { // Remove menu in case of schedule mode.
        utility_menu->RemoveMenu(ID_UTILITY_SARMANAGER, MF_BYCOMMAND);
    }

    //+Patni-Hemant/2011Jan11/Code Deletion/REDMINE-1095_Reopen
    // It is already done above now
    ////-Patni-Rajeesh/ADDED On 31/03/2010 MVC006751
    //if (m_pqm_g.GetPQMMode() == K_SCHEDULING) {
    //    utility_menu->EnableMenuItem(ID_MENUITEM_APC_PAUSE, MF_BYCOMMAND | MF_GRAYED);
    //}
    //-Patni-Hemant/2011Jan11/Code Deletion/REDMINE-1095_Reopen
}


//************************************Method Header************************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::SetBitmapForButton(
    ButtonEx& button_object,
    const UINT bitmap_resource_id,
    const COLORREF f_mask_color  /* =RGB(0,255,0)*/,   //Patni-Hemant/2009Nov12/Added/ IR#90 / PQM Queue Top Buttons
    const bool center_bitmap /*= true*/
)const
{
    return CUIButtonUtility::GetInstance()->SetBitmapForButton(button_object, bitmap_resource_id, f_mask_color, center_bitmap);
}


//+Patni-HAR/2009Jan8/Added/MVC006380
//************************************Method Header**********************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HAR - HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::SetIconForButton(
    ButtonEx& f_button_object,
    const UINT bitmap_resource_id,
    const bool f_center_bitmap /*= true*/
)
{
    return CUIButtonUtility::GetInstance()->SetIconForButton(f_button_object, bitmap_resource_id, f_center_bitmap);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetAutoVoiceCodeToItems(
    const int* const seq_items,
    const int seq_count,
    const int auto_voice_exe_code,
    const AUTOVOICE_OPTIONS selected_auto_voice_option,
    const AUTOVOICE_PRESCAN_OPTION l_prescan_aut_voi_optn
)
{

    LPCTSTR FUNC_NAME = _T("CPQMView::SetAutoVoiceCodeToItems");

    if (NULL == seq_items) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("seq_items pointer is NULL"));
        return ;
    }

    VARIANT* pdata = new VARIANT;

    if (NULL == pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pdata pointer is NULL"));
        return ;
    }

    pdata->wReserved1 = AUTO_VOICE;
    pdata->intVal = auto_voice_exe_code;

    bool l_set_prev = (NONE_NONE == selected_auto_voice_option);

    for (int counter = 0; counter < seq_count ; counter++) {

        const int l_seq_index = seq_items[counter];
        CSequence* pseq = GetSelectedSequence(l_seq_index);

        if ((NULL == pseq) || (PR_ACQ_STATUS_WAIT != pseq->GetAcqStatus())) {
            continue;
        }

        if (S_OK != m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, GetAcqOderForGivenProtocolIndex(l_seq_index))) {
            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("Trace of UpdateGUIControlsInVarfieldDB"));
            continue;
        }

        pseq->SetAutoVoiceCode(auto_voice_exe_code);

        if (l_set_prev) {
            pseq->SetPrevAutoVoiceOption(pseq->GetAutoVoiceOption());
        }

        pseq->SetAutoVoiceOption(selected_auto_voice_option);
        pseq->SetPrescanAutoVoiceOptn(l_prescan_aut_voi_optn);

        m_pqm_listctrl.InvalidateItemsForModeOption(l_seq_index, AUTO_VOICE_OPTION);
    }

    DEL_PTR(pdata)
}


//************************************Method Header************************************
// Method Name  : SetPatientOrientationFor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetPatientOrientationFor(
    const OrientationType orientation_type,
    const int orientation_value
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetPatientOrientationFor");

    if (orientation_value == -1) {

        //        ASSERT(FALSE);
        return ;
    }

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    bool iswse_eidted_protocol_selected = false;
    int* selected_items = NULL;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected, NULL, &iswse_eidted_protocol_selected);

    if (count <= 0 || iswse_eidted_protocol_selected) {
        //        ASSERT(FALSE);

        DEL_PTR_ARY(selected_items)

        return ;
    }

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == selected_items) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("selected_items pointer is NULL"));

        return ;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    const int seq_item_index = selected_items[0];
    ASSERT(seq_item_index == 0);	//it should be first item only..

    VARIANT* pdata = new VARIANT;
    CString seqlid(_T(""));	//Patni-PP/2009Jun02/Modified/SU14 CPP test initialization corrections

    BSTR l_bstr_direction(_T(""));
    CString str_direction(_T(""));
    CSequence* l_pseq = NULL ;
    int l_acq_order = -1;

    for (int index = seq_item_index; index < m_pqm_listctrl.GetItemCount() ; index++) {

        l_pseq = (CSequence*) m_pqm_listctrl.GetItemData(index);

        if ((!l_pseq) || (l_pseq->GetAcqStatus() != PR_ACQ_STATUS_WAIT)) {

            //            ASSERT(FALSE);
            continue ;
        }

        CPqmPrMode& pr_mode = GetPrModeOfSelectedSequence(index);

        l_pseq->GetCPASCOMSequence()->GetSequenceID(seqlid);

        //Patni-HAR/2010feb25/Added/MSA00251-00213
        l_acq_order = GetAcqOderForGivenProtocolIndex(index);

        switch (orientation_type) {

            case OriType_PatientInsertDir:
            case OriType_PatientRotationDir: {

                if (orientation_type == OriType_PatientInsertDir)
                    pr_mode.m_pat_ori.end_in = orientation_value ;

                else
                    pr_mode.m_pat_ori.side_up = orientation_value;

                str_direction.Format(_T("%d%s%d"), pr_mode.m_pat_ori.end_in, _T(","), pr_mode.m_pat_ori.side_up);
                l_bstr_direction = str_direction.AllocSysString();

                pdata->wReserved1 = PATIENT_ORIENTATION;
                pdata->byref = l_bstr_direction;


                //+Patni-HAR/2010feb25/Modified/MSA00251-00213
                m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_acq_order);


                //+Patni-Hemant/2010Jun15/Added/V1.20/MVC008326
                CString l_patient_ori;
                l_patient_ori.Format(_T("%d,%d"), pr_mode.m_pat_ori.end_in, pr_mode.m_pat_ori.side_up);
                l_pseq->GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("PORI"), l_patient_ori);
                //-Patni-Hemant/2010Jun15/Added/V1.20/MVC008326
            }
            break ;

            case OriType_PatientViewDir: {

                pdata->wReserved1 = VIEW_DIRECTION;
                pdata->intVal = orientation_value;

                pr_mode.m_view_dir = orientation_value;

                //+Patni-HAR/2010feb25/Modified/MSA00251-00213
                m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_acq_order);

                l_pseq = (CSequence*)m_pqm_listctrl.GetItemData(index);

                if (NULL == l_pseq) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pseq is NULL "));
                    break;
                }

                l_pseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("VDIR"), orientation_value);

                //+Patni-HAR/2010feb25/Modified/MSA00251-00213
                m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_acq_order);


            }
            break ;
        }

    }

    //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-391
    if (GetSeqInfoDlgStatus()) {

        m_info_seq_dlg->DisplaySeqInformation();
    }

    //-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-391

    //Patni-AMT/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
    DisplayScanOffsetOnPQM(seq_item_index);

    DEL_PTR(pdata)

    DEL_PTR_ARY(selected_items)

}

// update all text in language settings
void CPQMView::SetMultilingual()
{
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_END_EXQAM"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_END_EXQAM);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SCAN_START"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_SCAN_START);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    /*l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PASS_BUTTON"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_PASS_BUTTON);
    if (_tcscmp(l_chOutput ,_T("")) != 0)
    {::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;*/


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ABORT"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_ABORT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    /*	l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_LOCATOR_BUTTON"));
    	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_LOCATOR_BUTTON);
    	if (_tcscmp(l_chOutput ,_T("")) != 0)
    	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    	l_chOutput = _T("");
    	l_buttonUserHwnd = NULL;
    */

    /*	l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SCAN_EDIT_BUTTON"));
    	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_SCAN_EDIT_BUTTON);
    	if (_tcscmp(l_chOutput ,_T("")) != 0)
    	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    	l_chOutput = _T("");
    	l_buttonUserHwnd = NULL;
    */

    //+Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons
    //
    //    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SCAN_OFFSET_BUTTON"));
    //	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_SCAN_OFFSET_BUTTON);
    //	if (_tcscmp(l_chOutput ,_T("")) != 0)
    //	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    //	l_chOutput = _T("");
    //	l_buttonUserHwnd = NULL;

    //-Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COIL_BUTTON"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_COIL_BUTTON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COIL_INFO_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_COIL_INFO_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    //+Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons
    //
    //	l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SCAN_ANATOMY_STATIC"));
    //	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_SCAN_ANATOMY_STATIC);
    //	if (_tcscmp(l_chOutput ,_T("")) != 0)
    //	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    //	l_chOutput = _T("");
    //	l_buttonUserHwnd = NULL;
    //
    //-Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons

    /*	l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_DELETE_BUTTON"));
    	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_DELETE_BUTTON);
    	if (_tcscmp(l_chOutput ,_T("")) != 0)
    	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    	l_chOutput = _T("");
    	l_buttonUserHwnd = NULL;
    */
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_DUPLICATE_BUTTON"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_DUPLICATE_BUTTON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    //+Patni-Hemant/2009Nov24/Commented/IR#90/Bottom Buttons
    //l_chOutput = _T("");
    //l_buttonUserHwnd = NULL;

    //l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_REMAINING_TIME_STATIC"));
    //l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_REMAINING_TIME_STATIC);
    //-Patni-Hemant/2009Nov24/Commented/IR#90/Bottom Buttons

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_SAR_ANATOMY_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_SAR_ANATOMY_STATIC2);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


}


//************************************Method Header**********************************
// Method Name  : SetMoveCouchForSeq
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMView::SetMoveCouchForSeq(CSequence* f_protseq, const MOVECOUCH_METHOD f_movecouchmethod, const int f_hold_val)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetMoveCouchMethod");

    if (NULL == f_protseq) {
        return false;
    }

    const int l_seq_acqorder = _ttoi(f_protseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

    VARIANT* pdata = new VARIANT;

    if (f_hold_val != f_protseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"))) {

        pdata->wReserved1 = HOLD_VALUE;
        pdata->wReserved2 = MOVE_COUCH;
        pdata->intVal = f_hold_val;

        //Updating Holdvalue in Database
        if (S_OK != m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_seq_acqorder)) {

            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("Trace of UpdateGUIControlsInVarfieldDB"));

            DEL_PTR(pdata);
            return false;
        }

        //Updating Holdvalue in GUI sequence
        f_protseq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("HOLD"), f_hold_val);
    }

    if (f_protseq->GetMoveCouchMethodOption() != f_movecouchmethod) {

        pdata->wReserved1 = MOVE_COUCH_METHOD;
        pdata->wReserved2 = 0;
        pdata->intVal = f_movecouchmethod;

        //Updating Move Couch Method in Database
        if (S_OK != m_acquisition->UpdateGUIControlsInVarfieldDB(pdata, l_seq_acqorder)) {

            PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                      _T("Trace of UpdateGUIControlsInVarfieldDB"));

            DEL_PTR(pdata);
            return false;
        }

        //Updating Move Couch Method in GUI sequence
        if (f_movecouchmethod == NOMOVE) {
            f_protseq->SetPrevMoveCouchMethodOption(f_protseq->GetMoveCouchMethodOption());
        }

        f_protseq->SetMoveCouchMethodOption(f_movecouchmethod);
    }

    DEL_PTR(pdata);
    return true;
}


//+Patni-PJS/2010Nov15/Added/V2.0/IR-175
//************************************Method Header**********************************
// Method Name  : SetAvLanguage
// Author       : PATNI/ PJS
// Purpose      : Set Auto voice language in pqmconfig.ini
//***********************************************************************************
void CPQMView::SetAvLanguage(
    const CString& f_av_lang
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetAvLanguage");

    if (f_av_lang.IsEmpty()) {
        //Trace Log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Auto voice language is empty in INI file"));
        return;
    }

    CString l_config_path(WCS::EMPTY_STRING);

    if (!CPQMUtility::GetConfigFilePath(l_config_path)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get config file path"));
        return;
    }

    CIniReader l_ini_reader(l_config_path);

    if (!l_ini_reader.SetKey(f_av_lang, _T("language"), _T("Auto Voice"))) {
        //Trace Log.
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to set set key val for language key in Auto voice"));
    }
}

//****************************Method Header********************
//Method Name   : SetState
//Author        : PATNI/GP
//Purpose       : Set state of button
//*************************************************************
void CPQMView::SetState(
    CWnd* wnd, const PQMControlState control_state
)const
{
    switch (control_state) {
        case STATE_ENABLE:
            wnd->EnableWindow(true);
            break;

        case STATE_DISABLE:
            wnd->EnableWindow(false);
            break;

        case STATE_VISIBLE:
            wnd->ShowWindow(true);
            break;

        case STATE_INVISIBLE:
            wnd->ShowWindow(false);
            break;

        case STATE_VISIBLE_ENABLE:
            wnd->ShowWindow(true);
            wnd->EnableWindow(true);
            break;

        case STATE_VISIBLE_DISABLE:
            wnd->ShowWindow(true);
            wnd->EnableWindow(false);
            break;

        case STATE_DEFAULT:
        default:
            break;
    };

}

//+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
void CPQMView::SetStateForCtrlForCFADialog(
    const pqm_msg_tag f_message_to_process
)const
{
    //Note: we have explicitly habdled PQM_MSG_CFA_PRESCAN_END and PQM_MSG_CFA_PRESCAN_ABORT at
    //AcqmanMessageHandler.cpp sid.e it needs further processing

    LPCTSTR FUNC_NAME = _T("CPQMView::SetStateForCtrlForCFADialog");
    PQM_TRACE_SCOPED(FUNC_NAME);

    switch (f_message_to_process) {
        case PQM_MSG_SCAN_START:
        case PQM_MSG_STATUS_DONE:
        case PQM_MSG_ABORT_WAIT:
        case PQM_MSG_APC_RETRY_WAIT:
        case PQM_MSG_PAUSE_DONE:
        case PQM_MSG_RESUME_DONE:
        case PQM_MSG_ERROR:

            //+Patni-DKH/2009Aug24/Modified/cpp test corrections
            CPQMCFADlg* l_PQMCFADlg = GetCFADlgPtr();

            if (NULL == l_PQMCFADlg) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("l_PQMCFADlg pointer is NULL"));
                return ;

            }

            //-Patni-DKH/2009Aug24/Modified/cpp test corrections

            l_PQMCFADlg->ProcessShimmingMsg(f_message_to_process);

            break ;
    }

    return ;
}
//****************************Method Header************************************
//Module         : SetDefaultChannelMode
//Author         : PATNI/Pavan
//Purpose        : Sets the default channel mode if Maxcoilchannel.ini file present
//*****************************************************************************
void CPQMView::SetDefaultChannelMode(CSequence* f_pseq, int f_seq_index)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::SetDefaultChannelMode");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int l_mode = -1;
    int l_acq_order = _ttoi(f_pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
    POSITION pos = NULL ;
    CSequence* pseq_prev = NULL ;

    pos = m_list->FindIndex(f_seq_index - 1);

    if (pos) {
        pseq_prev = m_list->GetAt(pos);
    }

    int l_acq_order_prev_prot = -1;

    if (NULL != pseq_prev) {
        l_acq_order_prev_prot = _ttoi(pseq_prev->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
    }

    CString l_msg = _T("");

    if (IsBatchSettingOn()) {
        int size = m_pqm_listctrl.GetItemCount();

        if (f_seq_index > 0) {
            CPqmPrMode l_prmode = GetPrModeOfSelectedSequence(f_seq_index - 1);

            VARIANT* l_pdata = new VARIANT;

            if (S_FALSE != m_acquisition->GetChannelModeSelection(l_acq_order_prev_prot, l_pdata)) { // Get Channel Mode of previous protocol // Review comment-11-Jan-11
                l_mode = l_pdata->intVal;
            }

            DEL_PTR(l_pdata);

            if ((-1 == l_mode) && (!l_prmode.m_recv_coil_label.IsEmpty())) { // If coil is present but Channel mode is not present, then channel mode will be normal
                l_mode = CHANNEL_MODE_NORMAL;
                l_msg.Format(_T("Channel Mode = %d, system channels to be used %d"), l_mode, l_mode ? m_system_enable_channel : m_limited_channel);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

                if (S_FALSE == m_acquisition->SetChannelModeSelection(l_acq_order, l_mode, l_mode ? m_system_enable_channel : m_limited_channel)) { // Review comment-11-Jan-11
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selected channel mode is not set"));
                }

                return;
            }

            if ((-1 != l_mode) && (!l_prmode.m_recv_coil_label.IsEmpty())) {
                // If coil and channel mode both present
                l_msg.Format(_T("Channel Mode = %d, system channels to be used %d"), l_mode, l_mode ? m_system_enable_channel : m_limited_channel);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

                if (S_FALSE == m_acquisition->SetChannelModeSelection(l_acq_order, l_mode, l_mode ? m_system_enable_channel : m_limited_channel)) { // Review comment-11-Jan-11
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selected channel mode is not set"));
                }

                return;
            }
        }
    }

    CPqmPrMode l_prmode = GetPrModeOfSelectedSequence(f_seq_index);

    VARIANT* l_pdata = new VARIANT;

    if (S_FALSE != m_acquisition->GetChannelModeSelection(f_seq_index, l_pdata)) { // Get Channel Mode of transferred protocol
        l_mode = l_pdata->intVal;
    }

    DEL_PTR(l_pdata);

    if ((-1 == l_mode) && (!l_prmode.m_recv_coil_label.IsEmpty())) { // If coil is present but Channel mode is not present, then channel mode will be normal
        l_mode = CHANNEL_MODE_NORMAL;

        if (S_FALSE == m_acquisition->SetChannelModeSelection(l_acq_order, l_mode, l_mode ? m_system_enable_channel : m_limited_channel)) { // Review comment-11-Jan-11
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selected channel mode is not set"));
        }

        l_msg.Format(_T("Channel Mode = %d, system channels to be used %d"), l_mode, l_mode ? m_system_enable_channel : m_limited_channel);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
    }

    if ((-1 != l_mode) && (!l_prmode.m_recv_coil_label.IsEmpty())) { // If previous protocol' coil is not present, then

        if (!m_acquisition->SetChannelModeSelection(l_acq_order, l_mode, l_mode ? m_system_enable_channel : m_limited_channel)) { // Review comment-11-Jan-11
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selected channel mode is not set"));
        }

        l_msg.Format(_T("Channel Mode = %d, system channels to be used %d"), l_mode, l_mode ? m_system_enable_channel : m_limited_channel);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);
        return;
    }

}

void CPQMView::SetScanAnatomy(
    const int f_scananatomy_menu_index,
    const LateralityEnum f_laterality /*= Laterality_Invalid*/
)
//-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
{
    //as per old code :
    //if Batch setting is on
    //- apply anatomy to first selected item and the remaining all items.
    //else
    // - apply anatomy to only selected items.

    int* selected_items = NULL ;
    int count = m_pqm_listctrl.GetselectedItems(&selected_items);

    if (!selected_items || !count) {

        return ;
    }

    //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131
    //
    if (IsBatchSettingOn()) {

        //batch setting is on so apply anatomy settings to all the items below the first selected item (including first selected item)
        int currently_sel_item = selected_items[0] ;

        DEL_PTR_ARY(selected_items)

        count = (m_pqm_listctrl.GetItemCount() - currently_sel_item) ;
        selected_items = new int[count] ;

        int counter = 0;

        for (int item = currently_sel_item; counter < count; item++)
            selected_items[counter++] = item;
    }

    //+Patni-MEGHANA /2009Mar17/Modify/ACE-1/ MCM0213-00131

    //+Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
    //ApplyScanAnatomy(selected_items, count, f_scananatomy_menu_index);
    ApplyScanAnatomy(selected_items, count, f_scananatomy_menu_index, f_laterality);
    //-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137

    DEL_PTR_ARY(selected_items)
}


//****************************Method Header************************************
//Module         : SetInitialButtonState
//Author         : PATNI/GP
//Purpose        : Set initial Buttons State depending upon mode
//*****************************************************************************
void CPQMView::SetInitialButtonState(
)
{
    //Added by Hemant On 7/24/2008 12:22:26 PM
    SetFlagRelatedCtrlsStates();

    if (m_pqm_g.GetPQMMode() != K_IMAGING) {

        m_scan_start_button.ShowWindow(false);

        m_pause_resume_button.ShowWindow(false);
        m_abort_button.ShowWindow(false);

        //we can not have locator button in schedule page..
        m_locator_button.ShowWindow(false);

    } else {

        EnableDisableScanRelatedButtons(false, true);
    }

    //
    //End Of addition By Hemant On7/24/2008 12:22:31 PM
    //-------------------------------------------------------------------------

    EnableDisableStartupButtons(false) ;
}


//+Patni-ARD/2010Feb18/Commented/Ph-3# PROPOSAL_06
// HideMemory() & ClosePQM() delete as not used in GUI code.




//+Patni-Manish/2009July16/Added/JFT#GUIRenewal-To Change color of COIL,ANATOMY button
//************************************Method Header************************************
// Method Name  : SetColorButton
// Author       : PATNI/ Manish
// Purpose      : To change color of COIL/ANATOMY Button
//***********************************************************************************
void CPQMView::SetColorButton(ButtonEx* pbtn, const COLORREF cColor)const
{
    if (NULL != pbtn) {
        BUTTON_COLORS_t btnColor;
        pbtn->GetButtonColors(btnColor);
        btnColor.crBrushEdgeStart = cColor;
        btnColor.crBrushEdgeEnd	  = cColor;
        btnColor.crBottomEdge = cColor;
        btnColor.crBottomEdgeShadow = RGB(113, 77, 0);
        btnColor.crbrushCircleStart = cColor;
        btnColor.crbrushCircleEnd = cColor;
        btnColor.crBottomEdgePushed = cColor;
        btnColor.crUpperEdgeShadow = cColor;
        btnColor.crFocusRect = cColor;
        btnColor.crBrushFaceStart = cColor;
        btnColor.crBrushFaceEnd = RGB(113, 77, 0);
        pbtn->SetButtonColors(btnColor);

    } else
        return;

}
//************************************Method Header************************************
// Method Name  : SetColorForResetButton
// Author       : PATNI/ Sribanta
// Purpose      : To change color of Reset Button
//***********************************************************************************
void CPQMView::SetColorForResetButton()
{


    const COLORREF cColor = RESET_BTN_BACKGROUNDCOLOR;
    BUTTON_COLORS_t btnColor;
    m_reset.GetButtonColors(btnColor);
    btnColor.crBrushEdgeStart = cColor;
    btnColor.crBrushEdgeEnd	  = cColor;
    btnColor.crBottomEdge = cColor;
    btnColor.crBottomEdgeShadow = RESET_BTN_BOTTOMEDGESHADOWCOLOR;
    btnColor.crbrushCircleStart = cColor;
    btnColor.crbrushCircleEnd = cColor;
    btnColor.crBottomEdgePushed = cColor;
    btnColor.crUpperEdgeShadow = RESET_BTN_UPPEREDGESHADOWCOLOR;
    btnColor.crFocusRect = cColor;
    btnColor.crBrushFaceStart = cColor;
    btnColor.crBrushFaceEnd = RESET_BTN_BOTTOMEDGESHADOWCOLOR;
    btnColor.crPressedCenter = RESET_BTN_BOTTOMEDGESHADOWCOLOR;
    btnColor.crPressedSurround = cColor;
    btnColor.crFocusRect = RESET_BTN_FOCUSRECTCOLOR;
    m_reset.SetButtonColors(btnColor);
}


//************************************Method Header**************************************
//Method Name:  SetBitmapForAntomyButton
//Author:  Patni/Manish
//Purpose: Called from button event of list control.
//******************************************************************************
void CPQMView::SetBitmapForAntomyButton(const int f_nBitmapID)   ///Called from LButtonDown event of list control.
{
    //+Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons
    LPCTSTR FUNC_NAME = _T("CPQMView::SetBitmapForAntomyButton");

    try {
        if (f_nBitmapID <= 0) {

            m_sar_anatomy_static.SetTextColor(g_no_data_warning_color);
            m_sar_anatomy_static.UpdateFont();
            SetBitmapForButton(m_scan_sar_button, IDB_SAR_REGION_UNSELECT, RGB(255, 255, 255));

        } else {
            //Patni-Hemant/2009DEC02/Modified/IR#90/MVC006006
            SetBitmapForButton(m_scan_sar_button, f_nBitmapID, RGB(255, 255, 255));

            m_sar_anatomy_static.SetTextColor(g_pqm_static_textcolor);
            m_sar_anatomy_static.UpdateFont();

        }

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred..."));
        throw;
    }

    //
    //-Patni-Hemant/2009Nov16/Modified/IR#90/Bottom Buttons

}
//-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal


//****************************Method Header************************************
//Module         : GetShimmingForCFA
//Author         : PATNI/MSN
//Purpose        : Get shim flag for CFA
//*****************************************************************************
inline bool CPQMView::GetAPCPauseChecked(
)const
{
    return m_apcflag;
}

//****************************Method Header************************************
//Module         : SetAPCPauseChecked
//Author         : PATNI/MSN
//Purpose        :
//*****************************************************************************
inline void CPQMView::SetAPCPauseChecked(
    const bool apcflag
)
{
    m_apcflag = apcflag;

    //Patni-Hemant/2011Jan11/Added/REDMINE-1095_Reopen
    GetAcquisition()->SetAPCHoldValue(m_apcflag);
}



CString CPQMView::GetCoilStringFor(
    CSequence* f_seq
) const //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_17
{

    return (f_seq == NULL) ? _T("") : GetCoilStringFor(_ttoi(f_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"))))  ;
}



/****************************Method Header************************************
//Module         : GetSequqnceID
//Author         : PATNI/SG
//Purpose        : Get the SequenseID
//*****************************************************************************/
CString CPQMView::GetSequqnceID()const
{

    CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(GetScanIndex());
    CString SeqLID = _T("");

    //Patni-PJS+HEMANT/COMMENTED On 9/15/2009 10:21:21 PM/ MVC2336
    if (l_seq != NULL) {

        l_seq->GetCPASCOMSequence()->GetSequenceID(SeqLID);
    }

    return SeqLID;
}
//-Patni-ARD/2009May25/Added/GL_SR_SQSaved issue



bool CPQMView::GetDefaultMRSPrescanValue(
    SMRSScan_s& mrsscan_values
)const
{
    //+Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK

    VARIANT* pData = new VARIANT;
    pData->byref = &mrsscan_values;

    const bool l_ret_value = m_acquisition->GetMRSDefaultValue(&pData) > 0 ? true : false;

    DEL_PTR(pData)

    return l_ret_value;
    //-Patni-AMT/2010Jan28/Added/Ph-3# PROPOSAL_01-MEM_LEAK

}

bool CPQMView::GetMrsBrstLicense()
{
    return (S_OK == m_acquisition->GetMRS_BRST_License()) ? true : false;
}

//+Patni-Ajay/2010July26/Added/REDMINE-443
//************************************Method Header************************************
// Method Name  : GetPresetItemData
// Author       : PATNI/ Ajay
// Purpose      :
//*************************************************************************************
void CPQMView::GetPresetItemData(
)
{
    VARIANT* pData = new VARIANT;

    pData->byref = &m_preset_item;
    GetAcquisition()->GetPresetItem(&pData);

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }

}
//-Patni-Ajay/2010July26/Added/REDMINE-443



//****************************Method Header************************************
//Module         : GetCommentDlgStatus
//Author         : PATNI/GP
//Purpose        : Get Comment Dialog Status
//*****************************************************************************
bool CPQMView::GetCommentDlgStatus(
)const
{
    if ((m_comment_dlg && ::IsWindow(m_comment_dlg->m_hWnd))) {

        return (m_comment_dlg->IsWindowVisible() ? true : false) ;
    }

    return false;
}


//************************************Method Header**************************************

//Method Name:  GetSeqInfoDlgStatus

//Author:  PATNI/SVP

//Purpose: Returns seq info dialog status.

//**************************************************************************************/
BOOL CPQMView::GetSeqInfoDlgStatus(
)const
{

    //+Patni-Hemant/2010May20/Added/MaFT/Code Review
    if (m_info_seq_dlg && ::IsWindow(m_info_seq_dlg->m_hWnd)) {

        return m_info_seq_dlg->IsWindowVisible();
    }

    return FALSE;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMView::GetCoilDlgStatus(
)const
{
    if ((dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd))) {

        return dlgCoilSelWin->IsWindowVisible();
    }

    return FALSE;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CPQMView::GetCoildDialogStringFor(
    CSequence* f_seq
)
{
    if (!f_seq) {

        return _T("");
    }

    CPASParamStore* seq_para =  f_seq->GetCPASCOMSequence()->GetSequenceParam();

    //Modified by Hemant on 23_01_09
    //Now getting the number from the listctrl
    //this will be according to bias..
    //protocol_number.Format( _T("%d : "), first_selected_item +1 ) ;
    const CString protocol_number = m_pqm_listctrl.GetCellString(CPoint(m_pqm_listctrl.GetProtocolNumberCol(), m_pqm_listctrl.GetFirstSelectedItem()));

    return protocol_number + _T(" : ") + seq_para->GetString(_T("COMM"));
}


//****************************Method Header************************************
//Module         : GetAbortFlag
//Author         : PATNI/MSN
//Purpose        : Getter method to get the Abort Flag
//*****************************************************************************
inline bool CPQMView::GetAbortFlag(
)const
{
    return m_flag_abort;
}


//+Patni-ARD/2009June17/Added/JFT#  Acqorder checking between IPC & GUI
//************************************Method Header**************************************
//Method Name: SetGUISelAcqOrder
//Author:  PATNI/ARD
//Purpose: Sets the acqorder of first wait protocol in list of protocol.
//**************************************************************************************/
void CPQMView::SetGUISelAcqOrder(CSequence* f_seq)
{
    if (f_seq != NULL) {

        m_GUIselAcqOrder = _ttoi(f_seq->GetCPASCOMSequence()->
                                 GetSequenceParam()->GetString(_T("ACQORDER")));
    }
}
//-Patni-ARD/2009June17/Added/JFT# Acqorder checking between IPC & GUI




//****************************Method Header************************************
//Module         : SetPrModeOfSelectedSequence
//Author         : PATNI/DJ
//Purpose        : Set the PR Mode for selected sequence.
//*****************************************************************************
void CPQMView::SetPrModeOfSelectedSequence(
    const int index,
    CPqmPrMode pr_mod
)
{
    ((CSequence*) m_pqm_listctrl.GetItemData(index))->SetPrMode(pr_mod);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetAcqumanReadyForNextRequest(
    const bool f_ready
)
{
    m_acquman_ready_for_next_request = f_ready ;

    // Added by Sajeet ++ 08/01/2008
    if (m_acquman_status == NOT_CONNECTED && f_ready) {
        // connect to acqman if it is not connected, coz we have received ready message from acqman
        m_acquisition->ConnectToAcqMan();
        m_acquisition->SendSAEInfo();
    }

    //Added by Hemant On 12/9/2008 3:09:18 PM
    EnableDisableScanRelatedButtonsForSelectedProtocolStatus();

}


//-Patni-HAR/2009Jan8/Added/MVC006380

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMView::SetStateForCtrl(
    CWnd* wnd,
    const pqm_msg_tag f_msg
)
{
    if (wnd) {

        SetState(wnd, CControlStateMgr::GetState(f_msg, wnd->GetDlgCtrlID()));
    }
}
//************************************Method Header************************************
// Method Name  : GetConnectorState()
// Author       : iGate-Patni
// Purpose      :
//***********************************************************************************
inline BOOL CPQMView::GetConnectorState(
)const
{
    return m_connector_state;

}

// + GUIControl


//-------------------------------------------------------------------------



#if 0
#define		DBDT_MODE1	1
#define		DBDT_MODE2	2
#define		DBDT_NORMAL	0
#define		DBDT_NORMAL_LABEL	"---"
#define		DBDT_MODE1_LABEL	"1st"
#define		DBDT_MODE2_LABEL	"2nd"
status_t	PqmLabeldBdt(void* data, int prot, char* label)
{
    flt32_t*		dBdtValue ;
    int		value , dBdt_mode , mode ;
    PqmPrData_t* prObj;

    DB_FUNC_ENTER("[PqmLabeldBdt]:");
    mode = DBDT_NORMAL ;

    if (data != NULL) {
        dBdtValue = (flt32_t*)data;
        value = (int) * (dBdtValue + 1) ;

        if (value == 0.0)
            value = (int) * (dBdtValue) ;

        prObj = PqmPqGetItemFromProt(prot);

        if (prObj != NULL) {
            dBdt_mode = PqmGetdBdtMode(prObj);

            if (dBdt_mode == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED)
                mode =  DBDT_MODE1;

            else  if (dBdt_mode == VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED)
                mode =  DBDT_MODE2;
        }

        if (mode == DBDT_NORMAL)
            sprintf(label, DBDT_NORMAL_LABEL);

        else if (mode ==  DBDT_MODE1)
            sprintf(label, DBDT_MODE1_LABEL);

        else if (mode ==  DBDT_MODE2)
            sprintf(label, DBDT_MODE2_LABEL);

#if	0
        else {
            if (value == 0.0)
                sprintf(label, "---(%d)", mode);

            else
                sprintf(label, "% 3d(%d)", value, mode);
        }

#endif

        DB_PRINTF("L", ("PORT[%d], Mode=[%d],dBdt_Value=[%f][%f],label=%s \n",
                        prot, mode, *dBdtValue, *(dBdtValue + 1) , label));

    } else
        sprintf(label, DBDT_NORMAL_LABEL);


    DB_FUNC_EXIT();
    return      E_NO_ERROR;

}
#endif

//************************************Method Header************************************
// Method Name  : OnClickMenuCardiacLocator
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuCardiacLocator(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuCardiacLocator");


    if (FALSE == m_autolocator_app_cntl.InvokeAutoLocatorApplication(AUTOLOCATOR_CARDIAC, true)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to Load CALA application"));
    }

}

void CPQMView::SetRegistFlag(
    const BOOL f_reg_flag
)
{
    m_pas_reg_flag = f_reg_flag;
}
//************************************Method Header************************************
// Method Name  : OnClickMenuCardiacLocator
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
void CPQMView::OnClickMenuNeuroLocator(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnClickMenuNeuroLocator");

    if (FALSE == m_autolocator_app_cntl.InvokeAutoLocatorApplication(AUTOLOCATOR_NEURO, true)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to Load HALA application"));
    }

}


BOOL CPQMView::GetRegistFlag(
)const
{
    return m_pas_reg_flag;
}
CAutoLocatorAppController& CPQMView::GetAutoLocatorController()
{
    return m_autolocator_app_cntl;
}
CTiPrepAppController & CPQMView::GetTiPrepController()
{
    return m_tiprep_app_cntl;
}
//************************************Method Header************************************
// Method Name  : PQMMessageUpdateSarInfo
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMView::PQMMessageUpdateSarInfo()const
{
    m_pqmchildthread.PostMessageToThread(ON_UPDATE_SARINFO, NULL, NULL);
}

//************************************Method Header************************************
// Method Name  : UpdateSarInfo
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CPQMView::UpdateSarInfoOnPage()const
{
    if (m_acquisition != NULL) {
        m_acquisition->UpdateSARInfo();
    }
}
//****************************Method Header************************************
//Method Name   :UpdatePagerArea
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
//- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
void CPQMView::UpdatePagerArea(const CString& f_message, const CString& f_parameter, const CString& f_custom)const
{
    //+Patni-PJS/2010Jan19/Added/MVC006828
    if (m_pqm_g.GetPQMMode() != K_IMAGING) {
        return;
    }

    //-Patni-PJS/2010Jan19/Added/MVC006828

    //Patni-AMT/2010Feb25/Modified/Ph-3# PROPOSAL_06, MVC006916
    CDPSManager::GetInstance()->SendASyncMsg(DPS_PAGER, f_message, f_parameter,
            f_custom);

}
/****************************Method Header************************************
//Method Name   :IsLastProtocolProcessed
//Author        :PATNI/Raghavendra K M
//Purpose       :To get the last protocol status whether status is success or fail
//*****************************************************************************/

bool CPQMView::IsLastProtocolProcessed()
{

    LPCTSTR FUNC_NAME = _T("CPQMView::IsLastProtocolProcessed");

    if (!IsValidSequenceIndex(m_list->GetCount() - 1)) {
        return false;
    }

    CSequence* pseque = (CSequence*)m_pqm_listctrl.GetItemData(m_list->GetCount() - 1);

    if ((NULL != pseque) && ((PR_ACQ_STATUS_SUCCESS == pseque->GetAcqStatus()) || (PR_ACQ_STATUS_FAIL == pseque->GetAcqStatus()))) {
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseque is NULL "));
        return false;
    }

}



//************************************Method Header************************************
// Method Name  : GetStringForScaneOffset
// Author       : PATNI
// Purpose      : To calculate scan offset value
//***********************************************************************************
const CString CPQMView::GetStringForScaneOffset(
    CDC* f_dc,
    CRect& f_tooltip_text_rect
) const
{

    const int l_first_sel_item = m_pqm_listctrl.GetFirstSelectedItem();

    CString l_str_tooltip = CPQMUtility::GetMultiLingualString("IDS_PQM_SCAN_OFFSET_BUTTON") ;

    if (!IsValidSequenceIndex(l_first_sel_item)) {

        return CPQMImageUtility::GetToolTipValueFor(l_str_tooltip, _T(""), f_dc, f_tooltip_text_rect);
    }

    VARIANT* pData = new VARIANT;
    GetAcquisition()->GetPatientScanoffset(GetAcqOderForGivenProtocolIndex(l_first_sel_item), &pData);
    f32vec3_t* l_objvec = (f32vec3_t*) pData->byref;

    if (l_objvec == NULL) {
        return CPQMImageUtility::GetToolTipValueFor(l_str_tooltip, _T(""), f_dc, f_tooltip_text_rect);
    }

    CString l_param = _T(""), l_temp = _T("");
    l_temp.Format(_T("X: %2.1f cm"), l_objvec->x);
    l_param += l_temp + _T("\n");
    l_temp.Format(_T("Y: %2.1f cm"), l_objvec->y);
    l_param += l_temp;

    l_str_tooltip = CPQMImageUtility::GetToolTipValueFor(l_str_tooltip, l_param, f_dc, f_tooltip_text_rect);


    pData->byref = NULL ;
    DEL_PTR(pData);
    DEL_PTR(l_objvec);

    return l_str_tooltip;
}

//************************************Method Header************************************
// Method Name  : GetToolTipText
// Author       : PATNI
// Purpose      :
//***********************************************************************************
const CString CPQMView::GetToolTipText(
    const UINT f_nID,
    CDC* f_dc,
    CRect& f_tooltip_text_rect
) const
{

    if (f_nID == (UINT)m_delete_button.m_hWnd) {

        return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_DELETE_PROTOCOLS"));

    } else if (f_nID == (UINT)m_duplicate_button.m_hWnd) {

        return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_DUPLICATE_PROTOCOLS"));

    } else if (f_nID == (UINT)m_scanedit_button.m_hWnd) {

        return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SEQUENCE_EDIT_BUTTON"));

    } else if (f_nID == (UINT)m_pass_button.m_hWnd) {
        return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PASS_BUTTON"));

    } else if (f_nID == (UINT)m_utility_button.m_hWnd) {
        return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_UTILITY_MENU_BUTTON"));

    } else if (f_nID == (UINT)m_pause_resume_button.m_hWnd) {

        if (m_pause_resume_button.IsWindowEnabled()) {

            if (IsPausedScanning()) {

                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_RESUME_BUTTON"));

            } else {
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PAUSE_BUTTON"));
            }

        } else {
            return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PAUSE_RESUME_BUTTON"));
        }

    } else if (f_nID == (UINT)m_scan_offset_button.m_hWnd) {

        return GetStringForScaneOffset(f_dc, f_tooltip_text_rect);
    }

    return _T("");

}


/****************************Method Header************************************
//Method Name   :GetPQMFlagsString
//Author        :PATNI/
//Purpose       :
//*****************************************************************************/
CString CPQMView::GetPQMFlagsString()const
{

    CString l_log_string = _T("");

#define BOOLYESNO(b) (b ? _T("yes") : _T("No"))

    l_log_string.Format(_T("m_scan_index : %d"), m_scan_index) ;
    l_log_string += _T("\n");
    l_log_string = l_log_string + _T("m_is_scanning = ") + BOOLYESNO(m_is_scanning) + _T("\n");
    l_log_string = l_log_string + _T("m_scan_lock_acquired = ") + BOOLYESNO(m_scan_lock_acquired) + _T("\n");
    l_log_string = l_log_string + _T("m_iswait_cursor = ") + BOOLYESNO(m_iswait_cursor) + _T("\n");

    l_log_string.Format(_T("m_wait_cursor_counter : %d"), m_wait_cursor_counter) ;
    l_log_string += _T("\n");

    l_log_string = l_log_string + _T("m_ismemoryalive = ") + BOOLYESNO(m_ismemoryalive) + _T("\n");
    l_log_string = l_log_string + _T("m_is_wse_open = ") + BOOLYESNO(m_is_wse_open) + _T("\n");
    l_log_string = l_log_string + _T("m_is_locator_open = ") + BOOLYESNO(m_is_locator_open) + _T("\n") ;

    l_log_string = l_log_string + _T("m_pqm_closed_flag = ") + BOOLYESNO(m_pqm_closed_flag) + _T("\n") ;
    l_log_string = l_log_string + _T("m_pqm_max_protocol = ") + BOOLYESNO(m_pqm_max_protocol) + _T("\n") ;
    l_log_string = l_log_string + _T("m_rmc_scan_running = ") + BOOLYESNO(m_rmc_scan_running) + _T("\n") ;
    l_log_string = l_log_string + _T("m_flag_abort = ") + BOOLYESNO(m_flag_abort) + _T("\n") ;
    l_log_string = l_log_string + _T("m_is_mrs_prot = ") + BOOLYESNO(m_is_mrs_prot) + _T("\n") ;

#undef BOOLYESNO

    return l_log_string;
}


/****************************Method Header************************************
//Method Name   :RecAmbEditExit
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
void CPQMView::RecAmbEditExit()
{
    DisplayTotalRemainingTimeOnPQM();
    m_amb_controller.RecAmbEditExit();
}
/****************************Method Header************************************
//Method Name   :RecAMBPqmScanMsg
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
void CPQMView::RecAMBPqmScanMsg()
{

    m_amb_controller.RecAMBPqmScanMsg();
}
/****************************Method Header************************************
//Method Name   :RecAmbScanWait
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
void CPQMView::RecAmbScanWait()
{

    m_amb_controller.RecAmbScanWait();
}

/****************************Method Header************************************
//Method Name   :RecAmbScanStartError
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
void CPQMView::RecAmbScanStartError()
{

    m_amb_controller.RecAmbScanStartError();
}
/****************************Method Header************************************
//Method Name   :GetIsWSEOpen
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
bool CPQMView::GetIsWSEOpen()const
{

    return m_is_wse_open;
}
/****************************Method Header************************************
//Method Name   :AMBScanStart
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
bool CPQMView::AMBScanStart()
{

    return m_amb_controller.AMBScanStartThread();
}
/****************************Method Header************************************
//Method Name   :AMBScanStartThread
//Author        :iGATE/
//Purpose       :
//*****************************************************************************/
bool CPQMView::AMBScanStartThread()
{

    return m_amb_controller.AMBScanStartThread();
}

/****************************Method Header************************************
//Method Name   :UpdateAMBStatusFail
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
void CPQMView::UpdateAMBStatusFail()
{

    const int l_index = m_acquisition->GetAMBIndexToUdpate();

    if (l_index == GetScanIndex()) {
        //ReceiveMessage(PQM_MSG_ABORT_FAIL);
    } else {
        g_MainView->GetPQMListCtrl().UpdateProtocolStatus(l_index, PR_ACQ_STATUS_FAIL);

        OnSelectionStatusChanged();
    }

    DisplayTotalRemainingTimeOnPQM();
}

/****************************Method Header************************************
//Method Name   :DeleteAMBSlaveIndex
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
void CPQMView::DeleteAMBSlaveIndex()
{

    const int l_index = m_acquisition->GetAMBIndexToUdpate();

    if (IsValidSequenceIndex(l_index)) {

        m_pqm_listctrl.SelectOnlyItem(l_index);
        OnClickButtonDelete();
    }
}
/****************************Method Header************************************
//Method Name   :UpdateOtherButtonState
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
void CPQMView::UpdateOtherButtonState()
{

    m_amb_controller.UpdateAMBAbortButtonState(m_abort_button.IsWindowEnabled());
    m_amb_controller.UpdateAMBScanButtonState(m_scan_start_button.IsWindowEnabled());

}

/****************************Method Header************************************
//Method Name   :SetIsAMBPlanning
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
void CPQMView::SetIsAMBPlanning(const bool f_is_amb_planning)
{
    CString FUNC_NAME = _T("CPQMView::SetIsAMBPlanning.");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_is_amb_planning ? _T("AMB Planning Started.") : _T("AMB Planning Ends."));

    m_is_amb_planning = f_is_amb_planning;

    m_acquisition->SetIsAMBPlanning(m_is_amb_planning);
    EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
}

/****************************Method Header************************************
//Method Name   :GetIsAMBPlanning
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
bool CPQMView::GetIsAMBPlanning()const
{
    return m_is_amb_planning;
}

/****************************Method Header************************************
//Method Name   :IsProtocolSelectedForAMBPlan
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
bool CPQMView::IsProtocolSelectedForAMBPlan(const int f_check_list_index) const
{
    if (m_amb_controller.IsAMBInvoked() || !m_is_amb_planning) {
        return false;
    }

    for (int i = 0 ; i < m_selected_acqoder_list_for_wse.GetSize() ; i++) {
        int l_list_index =  GetListForGivenProtocolAcqOder(m_selected_acqoder_list_for_wse.GetAt(i));

        if (l_list_index == f_check_list_index) {
            return true;
        }
    }

    return false;
}

/****************************Method Header************************************
//Method Name   :AMBGetSelectedItems
//Author        :iGATE
//Purpose       :
//*****************************************************************************/
void CPQMView::AMBGetSelectedItems(int** f_selected_items, int& f_size)
{
    f_size = m_selected_acqoder_list_for_wse.GetSize();
    (*f_selected_items) = new int [f_size] ;

    for (int i = 0 ; i < f_size ; i++) {
        (*f_selected_items)[i]  = 	GetListForGivenProtocolAcqOder(m_selected_acqoder_list_for_wse.GetAt(i));
    }

}

//************************************Method Header************************************
// Method Name  : AMBParentImageRemovedfromProtocol
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMView::AMBParentImageRemovedfromProtocol(
)
{
    int l_protocol_acq_order = -1;
    m_acquisition->GetIndexForParentImageRemoved(&l_protocol_acq_order);

    if (IsValidSequenceIndex(l_protocol_acq_order)) {

        int l_list_index = GetListForGivenProtocolAcqOder(l_protocol_acq_order);
        CSequence* l_seq = (CSequence*) m_pqm_listctrl.GetItemData(l_list_index);

        if (l_seq) {

            VARIANT* l_data = new VARIANT;
            l_data->wReserved1 = PLAN_STATUS;
            m_acquisition->GetControlValueFromDB(l_data, l_protocol_acq_order);

            l_seq->GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("PLANSTATUS"), l_data->intVal);

            if (((l_data->intVal & VFC_PLN_STATUS_GLOC) == VFC_PLN_STATUS_GLOC)) {
                l_seq->SetWSEEditingEnum(WSE_Planned);
                l_seq->SetAMBEditingEnum(AMB_PLANNED);

            } else {
                l_seq->SetWSEEditingEnum(WSE_None);
                l_seq->SetAMBEditingEnum(AMB_NONE);
            }

            m_pqm_listctrl.InvalidateItemsForPLANColumn(l_list_index);

            DEL_PTR(l_data)
        }
    }
}

//************************************Method Header************************************
// Method Name  : AMBCheckIsAnyDialogOpen
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMView::AMBCheckIsAnyDialogOpen(
)const
{
    CCOMPASS_SettingDialog* l_compas_setting_dialog = m_cds_controller->GetCOMPASS_SettingDlg();
    CCOMPASS_Location_Debug* l_compas_location_debug_dialog = m_cds_controller->GetCompasLocationDebugDialog();
    CPQMWirelessGating* l_wireless_gating_dialog = m_wirelessgating_cntlr.GetWirelessGatingDialog();

    if (((dlgCoilSelWin && ::IsWindow(dlgCoilSelWin->m_hWnd))  && (m_selection_utiliy->GetCoilSelMode() != COIL_SEL_CDS)) ||
        (m_obj_gate_dlg && ::IsWindow(m_obj_gate_dlg->m_hWnd)) ||
        (m_delay_time_dlg && ::IsWindow(m_delay_time_dlg->m_hWnd)) ||
        (m_cfadlg && ::IsWindow(m_cfadlg->m_hWnd)) ||
        (m_mrsprescandlg && ::IsWindow(m_mrsprescandlg->m_hWnd)) ||
        (m_anatomy_dlg && ::IsWindow(m_anatomy_dlg->m_hWnd)) ||
        (m_dbdtcautiondialog && ::IsWindow(m_dbdtcautiondialog->m_hWnd)) ||
        (m_obj_coil_dlg && ::IsWindow(m_obj_coil_dlg->m_hWnd)) ||
        (m_pqmmanualprescandlg && ::IsWindow(m_pqmmanualprescandlg->m_hWnd)) ||
        (m_setscanoffsetdlg && ::IsWindow(m_setscanoffsetdlg->m_hWnd)) ||
        (l_compas_setting_dialog && ::IsWindow(l_compas_setting_dialog->m_hWnd)) ||
        (l_compas_location_debug_dialog && ::IsWindow(l_compas_location_debug_dialog->m_hWnd)) ||
        (l_wireless_gating_dialog && ::IsWindow(l_wireless_gating_dialog->m_hWnd)) ||
        (m_mem_passworddlg && ::IsWindow(m_mem_passworddlg->m_hWnd)) ||
        (::IsWindow(m_pqmplandup_dlg.m_hWnd))) {

        return true;
    }

    return false;
}


//************************************Method Header************************************
// Method Name  : GetAMBPlanScanStartCheck
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMView::GetAMBPlanScanStartCheck()const
{
    bool l_amb_plan_check = false;

    if (m_scan_index == -1) {
        l_amb_plan_check  = GetIsAMBPlanning() ? true : false;
    }

    return l_amb_plan_check;
}

//************************************Method Header************************************
// Method Name  : GetAPCRetry
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMView::GetAPCRetry()const
{
    BOOL f_is_abf_retry = FALSE;
    m_acquisition->GetAPCRetry(&f_is_abf_retry);
    return f_is_abf_retry;
}
/****************************Method Header************************************
//Method Name   :GetSARControlStatusForScanEditRelatedButtons
//Author        :iGate
//Purpose       :
//*****************************************************************************/
bool CPQMView::GetSARControlStatusForScanEditRelatedButtons() const
{
    return (SARCTRL_LICENCE_ON == m_sar_control_license) ? m_sar_connect_status : true;
}
/****************************Method Header************************************
//Method Name   :GetSARControlStatusForScanRelatedButtons
//Author        :iGate
//Purpose       :
//*****************************************************************************/
bool CPQMView::GetSARControlStatusForScanRelatedButtons() const
{
    return (SARCTRL_LICENCE_ON == m_sar_control_license) ? (m_dynamic_scan_on ? true : m_sar_connect_status) : true;
}

/****************************Method Header************************************
//Method Name   :IsAMBInvoked
//Author        :iGate
//Purpose       :
//*****************************************************************************/
bool CPQMView::IsAMBInvoked()const
{
    return m_amb_controller.IsAMBInvoked();
}

/****************************Method Header************************************
//Method Name   :IsAMB
//Author        :iGate
//Purpose       :
//*****************************************************************************/
bool CPQMView::IsAMB()const
{
    return (IsAMBInvoked() || GetIsAMBPlanning() || m_amb_controller.GetIswaitforAMBDlgOpen());
}

/****************************Method Header************************************
//Method Name   :OnAMBClosed
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::OnAMBClosed()
{
    LPCTSTR FUNC_NAME = _T("CPQMView::OnAMBClosed");
    PQM_TRACE_SCOPED(FUNC_NAME);

    EnableDisableScanEditOrPlanButtons();
    SetFlagRelatedCtrlsStates();
    OnSelectionStatusChanged();
}

/****************************Method Header************************************
//Method Name   :GetSARControlLicense
//Author        :iGate
//Purpose       :
//*****************************************************************************/
BOOL CPQMView::GetSARControlLicense()
{
    return m_sar_control_license;
}

/****************************Method Header************************************
//Method Name   :GetSARControlLicense
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::GetSarInfoPtrForIndex(CSequence* f_seq, int f_seq_index)
{
    VARIANT l_pdata;

    l_pdata.intVal = f_seq_index;

    if (S_OK == m_acquisition->GetSarInfoPtrForIndex(&l_pdata)) {
        sarInfo_t* l_ptr_sarInfo = (sarInfo_t*)l_pdata.byref;
        f_seq->SetSARInfoPtrForSeq(l_ptr_sarInfo);
    }
}

/****************************Method Header************************************
//Method Name   :GetWaitTimeSARControlFlag
//Author        :iGate
//Purpose       :
//*****************************************************************************/
bool CPQMView::GetWaitTimeSARControlFlag()const
{
    return m_is_waiting_for_scan_sarctrl;
}

/****************************Method Header************************************
//Method Name   :SetWaitTimeSARControlFlag
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::SetWaitTimeSARControlFlag(const bool f_is_waiting_for_scan_sarctrl)
{
    m_is_waiting_for_scan_sarctrl = f_is_waiting_for_scan_sarctrl;
}

/****************************Method Header************************************
//Method Name   :SetIsCFAokpressed
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::SetIsCFAokpressed(const bool f_is_cfa_ok_pressed)
{
    m_is_cfa_ok_pressed = f_is_cfa_ok_pressed;
}

/****************************Method Header************************************
//Method Name   :GetIsCFAokpressed
//Author        :iGate
//Purpose       :
//*****************************************************************************/
bool CPQMView::GetIsCFAokpressed()const
{
    return m_is_cfa_ok_pressed;
}

/****************************Method Header************************************
//Method Name   :DisplayWaitTimeonAcquirePage
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::DisplayWaitTimeonAcquirePage() const
{
    LPCTSTR FUNC_NAME = _T("CPQMView::DisplayWaitTimeonAcquirePage");
    PQM_TRACE_SCOPED(FUNC_NAME);

    const int l_scan_index = GetScanIndex();
    CSequence* l_pseq = GetSelectedSequence(l_scan_index);

    if (NULL == l_pseq) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pseq is NULL "));
        return;

    }

    sarInfo_p l_sar_info = l_pseq ->GetSARInfoPtrForSeq();

    if (NULL == l_sar_info) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_sar_info pointer is NULL"));
        return;
    }

    const int l_wait_time = l_sar_info->sarControl.waitTime;

    CString l_str_wait_time(_T(""));

    l_str_wait_time.Format(_T("%02d:%02d"), (l_wait_time % 3600) / 60, l_wait_time % 60);

    CString l_param(_T(""));

    l_param.Format(_T("%s /%s/-%s/%s/%d/%d"), _T("Waiting"), _T("#TIME#"), l_str_wait_time,
                   _T("s"), 10, 1);

    CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"), _T("FW_DisplayStringAndTime"), l_param);

    CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"), _T("FW_StartAcqBtnTimer"), _T(""));

    CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"), _T("FW_ShowStringAndTime"), _T(""));


}

/****************************Method Header************************************
//Method Name   :ClearDPSTimer
//Author        :iGate
//Purpose       :
//*****************************************************************************/
void CPQMView::ClearDPSTimer() const
{

    CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"), _T("FW_StopAcqBtnTimer"), _T(""));
    CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"), _T("FW_HideStringAndTime"), _T(""));
}

