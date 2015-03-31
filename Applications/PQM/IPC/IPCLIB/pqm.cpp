//REDMINE-1213_Update_29_APR
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqm.cpp
 *  Overview: Implementation of CPqm class.
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
                                                            comments
 *****************************************************************************/
#include "StdAfx.h"
#include "Pqm.h"
#include <tami/libpreScan/preScan.h>		//Patni-ARD/20090305/Added/ACE-1# handling of stabilization
#include <nasu/liblicense2/license.h>
#include <PDataParser/INIReader.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMIPCManager.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/pqmcommon.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <PQM/SFTMenuConstants.h>
#include <PQM/PQMLogMgr.h>
#include "PQMIPC.h"
#include "PqmPmAcqman.h"
#include "PQMStudyMgr.h"
#include "CouchConfig.h"
#include "SharedMemory.h"
#include "ColorSetting.h"
//+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "CoilData.h"
//#include "CoilDB.h"
//-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmCoilData.h"
#include "PASCOMInterface.h"
#include "PQMSAR.h"
#include "PqmSpeeder.h"
#include "PQMdBdt.h"
#include "scanprocedure.h"
#include "PqmPrMode.h"
#include "Pqmconfig.h"
#include "voicemanager.h"
#include "IRSListner.h"
#include "Pqmprobe.h" //Patni-Sribanta/2010Nov18/Added/REDMINE-772
//Patni-DKH/2010Jan12/Added/IR-87,Plan Duplicate
#include "PlanDuplicateSettings.h"

//Patni-HEMANT/ADDED On 6/12/2009 1:02:11 PM/ ACE-2 / WFDA
//Patni-PJS/2011Jan29/Added/IR-97
//#include "WFDAImplementer.h"

//Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "CDSCommonStructures.h"
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
#include "AutoInsertMapManager.h"
#include "AutoInsertShimManager.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "ProtocolUtil.h" //TMSC-Tanoue/2010Sep03/MVC008324
#include "CFShiftCheck.h"
#include "MasterSlave.h"
#include "CoilChannelMode.h"
#include "AutoLocatorPostProc.h"
#include "AutoLocatorParameterSetter.h"
#include "pqmAMB.h"
#include "PqmPmSARManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Patni-ARD/20090305/Added/ACE-1# handling of stabilization start
extern "C" {
    extern int AcqFileCopy(char* infile, char* outfile);
}
//Patni-ARD/20090305/Added/ACE-1# handling of stabilization End


//****************************Method Header************************************
//Method Name   :CPqm()
//Author        :PATNI/JCM
//Purpose       :Constructor
//*****************************************************************************
CPqm::CPqm(int f_mode) :
    m_sft_mode(SFT_CLINICAL),
    m_offset_vector_reset(false),
    m_dbsa_study_mgr(NULL),
    //Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
    //+Patni-ARD/2010Apr08/Modified/JaFT# IR-100
    m_xOffset(20),
    m_yOffset(20),
    m_acqman_ip(_T("127.0.0.1")),
    //m_wfdaimplementer(NULL),//Patni-PJS/2010Feb2/Modified/IR-97
    m_total_seq(0),
    m_no_of_sequence(0),
    m_is_same_study(false),
    m_post_process_flag(FALSE),//Patni-HAR/2010Mar08/Added/MSA0251-00275
    m_couch_pos(0.0),
    //+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
    m_protocol_index(0),
    m_show_coil(0),
    m_pqm_autoview(0),
    m_plan_scan(true),
    m_study_uid(_T("")),
    m_couchconfig(new CCouchConfig()),
    m_colorinfo(NULL),
    m_pqm_study(NULL),
    m_pqmipcmanager(NULL),
    m_sharedmem(NULL),
    m_cfshift_check(NULL),
    m_color(NULL),
    m_mode(f_mode),
    m_skip_rgn(true),		//Ashish SM3
    m_systype(MRT150GP),
    //-Patni-ARD/2010Apr08/Modified/JaFT# IR-100
    m_processing_ui(0),
    m_selected_pos(0),
    m_couch_limit_apc(0.0),
    m_drks_recon_mode(true),
    m_couch_pac(0),
    m_pqm_display_mode(0),
    m_sys_name(_T("")),
    m_gate_mode(-1),
    m_pqmSpeeder(new CPqmSpeeder()),
    m_study_uid_path(_T("")),
    m_pascom(NULL),
    m_coverage_msg(FALSE),
    m_shimm_wb_coil(TRUE),
    m_all_coverage_rgn(FALSE),
    m_default_rgn(0.0f),
    m_rx_gain_offset(0.0f),
    m_ris_dictionary(_T("")),
    m_ris_name(_T("")),
    m_auto_voice(false),
    m_av_interval(3000),
    m_av_spdr_map(FALSE),
    m_av_spdr_map_delay(6000),
    m_freeze_level(0),
    m_selected_coil_key(0),
    m_is_refprobe_alive(false),
    m_after_auto_scan(0),
    m_no_of_sequence_transferred(0),
    m_channel_mask(0),
    m_num_Channels(1),
    m_VAP_Channels(1),
    m_channel_Per_VFEP(1),
    m_vfep_Proc_Ratio(3.0),
    m_move_locator_mode(false),
    m_hi_speed_couch(false),
    m_mrs_license(FALSE),
    m_dBdtlicense(FALSE),
    m_dBdt2002_1_license(FALSE),
    m_dBdt2002_2_license(FALSE),
    m_SAR02_01_License(FALSE),
    m_SAR02_02_License(FALSE),
    m_MRS_BRST_license(FALSE),
    m_IEC_Edition(FALSE),
    m_NoiseReduction_license(FALSE),
    m_Mrapv2002_license(FALSE),
    m_muliscan_dyn_license(FALSE),
    m_EMTONE_license(FALSE),
    m_systemtype(0),
    m_is_active_scanoffset_dialog(false),
    m_is_active_cfa_dialog(false),
    m_is_active_coilinfo_dialog(false),
    m_is_active_gateinfo_dialog(false),
    m_is_active_coilselection_dialog(false),//Patni-PJS/2009Oct29/Added/IR-92
    m_is_procon_connected(false),
    m_gate_list(new CStringArray()),
    m_setAllCoilSAR(true),//MRP
    m_request_coil_info_display(false),
    m_current_sarInfo(_T("")),
    m_recieve_request_arr(NULL),
    m_recieved_request_no(0),
    m_vf_add_count(0),
    m_planduplicateSettings(new CPlanDuplicateSettings()),
    m_autoprot_acq_order(-1),
    m_cala_appln_running(false),
    m_CALA_license(FALSE),
    m_HALA_license(FALSE),
    m_Tiprep_license(FALSE),//KJ/2014
    m_autolocator_parametersetter(NULL),
    m_gate_param_setter(),
    m_autolocator_postproc(NULL),
    m_master_slave(NULL),
    m_defpguid_xml_data(),
    m_is_pguidxml_file(true),
    m_is_active_amb_dialog(false),
    m_amb_licence(FALSE),
    m_is_amb_planning(false),
    m_sar_manager(NULL),
    m_sarcontrol_license(FALSE),
    m_max_wait_time_for_auto_scan(MAX_TIME_FOR_AUTO_SCAN_DEFAULT),
	m_tiprep_updater(NULL)
{
    LPCTSTR FUNC_NAME = _T("CPqm::CPqm");

    m_acq_manager = new CPqmPmAcqman(this);
    m_pascom = new CPASCOMInterface(f_mode);
    m_pqm_procon = new CPqmProcon(this);
    m_pqmSAR = new CPQMSAR(this);
    m_pqmdBdt = new CPQMdBdt(f_mode, this, this);


    if (m_pqmSpeeder) {
        m_pqmSpeeder->SetPqmPtr(this);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmSpeeder pointer is NULL"));
    }

    m_irs_listner = new CIRSListner(this);

    CheckForValidLicense();

    m_IEC_Edition = VFC_DBDT_IEC_EDITION_2002;

    m_data_mgr = CDataManager::GetInstance();
    m_data_mgr->SetPQM(this) ;



    m_pamb = new CpqmAMB(m_acq_manager);

    //SM3_Mangesh_Start
    //+Patni-KT/2009Sep23/Modified/Added Try Catch
    try {
        m_vap = checkVap();

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("checkVap() crashed"));
    }

    //-Patni-KT/2009Sep23/Modified/Added Try Catch
    //SM3_Mangesh_End

    //+Patni-RUP/2009Sep14/Modified/cpp test corrections
    //    InitializeSMInfo();
    //-Patni-RUP/2009Sep14/Modified/cpp test corrections
    m_coil_selection_tool = new CCoilSelectionTool(this);

    // - Vantage CoilDB support @ AMT

    //Patni-AMT/2009Oct06/Added/Log correction
    InitiaseUIMsgMap();
    //Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator

    m_gate_param_setter.ResetGateInterval();
}

//****************************Method Header************************************
//Method Name   :~CPqm()
//Author        :PATNI/JCM
//Purpose       :Destructor
//*****************************************************************************
CPqm::~CPqm(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::~CPqm");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Destructor"));

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    DEL_PTR(m_pqm_procon);

    DEL_PTR(m_pqmSAR);

    DEL_PTR(m_pqmSpeeder);


    DEL_PTR(m_pqmdBdt);

    DEL_PTR(m_gate_list);

    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    DEL_PTR(m_acq_manager);

    DEL_PTR(m_couchconfig);

    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //DEL_PTR(m_coilDB);

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    DEL_PTR(m_colorinfo);

    DEL_PTR(m_pqm_study);

    DEL_PTR(m_dbsa_study_mgr);

    DEL_PTR(m_sharedmem);

    DEL_PTR(m_cfshift_check);
    DEL_PTR(m_coilchannelmode);
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //DEL_PTR(m_coildata);

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    DEL_PTR(m_pascom);

    DEL_PTR(m_irs_listner);

    if (NULL != m_data_mgr) {
        CDataManager::DestroyInstanc();
        m_data_mgr = NULL;
    }

    DEL_PTR(m_coil_selection_tool);

    //+Patni-HEMANT/ADDED On 6/17/2009 4:12:22 PM/ ACE-2/ WFDA
    //Patni-PJS/2010Feb2/Modified/IR-97
    //DEL_PTR(m_wfdaimplementer);

    //Patni-DKH/2010Jan12/Added/IR-87
    DEL_PTR(m_planduplicateSettings);

    DEL_PTR(m_autolocator_postproc);
    DEL_PTR(m_autolocator_parametersetter);

    DEL_PTR(m_master_slave);

    DEL_PTR(m_pamb);

    DEL_PTR(m_sar_manager);

}

//****************************Method Header************************************
//Method Name   :WriteToUI()
//Author        :PATNI/MRP
//Purpose       :
//*****************************************************************************
void CPqm::WriteToUI(
    const pqm_msg_tag Msg
)const
{
    LPCTSTR FUNC_NAME = _T("CPqm::WriteToUI");

    //+Panti-AMT/2009Oct07/Modified/Log correction
    if (Msg == PQM_MSG_INVALID_MESSAGE || Msg == PQM_MSG_LAST_MSG) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot send PQM_MSG_INVALID_MESSAGE  or PQM_MSG_LAST_MSG to UI"));
        return;
    }

    CString l_log_msg = _T("Enter-Sending message to UI = ") + m_pqm_msg_tag_lit_map[Msg].lit;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Panti-AMT/2009Oct07/Modified/Log correction

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmipcmanager) {
        m_pqmipcmanager->SendMessageToGUI(Msg);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqmipcmanager pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Panti-AMT/2009Apr07/Modified/PSP1#5 VFLOCK hang issue
    l_log_msg = _T("Exit-Sending message to UI = ") + m_pqm_msg_tag_lit_map[Msg].lit;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Panti-AMT/2009Apr07/Modified/PSP1#5 VFLOCK hang issue
}

//*******************************Method Header*********************************
//Method Name    : PqmPmIsActive()
//Author         : PATNI/MRP
//Purpose        : To be done in the next phase
//*****************************************************************************
bool CPqm::PqmPmIsActive(
    const int type
)const
{
    return IsDialogActive(type);
}


//+Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : IsDialogActive
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqm::IsDialogActive(
    const int f_dialog_type
)const 	//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_18
{

    switch (f_dialog_type) {

        case IS_SCANOFFSET_DIALOG:
            return m_is_active_scanoffset_dialog ;
            break;

        case IS_CFA_DIALOG:
            return m_is_active_cfa_dialog ;
            break;

        case IS_COIL_INFO_DIALOG:
            return m_is_active_coilinfo_dialog ;
            break;

        case IS_GATE_INFO_DIALOG:
            return m_is_active_gateinfo_dialog ;
            break;

        case IS_COIL_SELECTION_DIALOG:
            return m_is_active_coilselection_dialog ;
            break;

        case IS_AMB_DIALOG:
            return m_is_active_amb_dialog;
            break;

        default:
            break;
    }

    return false;
}
//-Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement

//********************************Method Header********************************
//Method Name   :IsSaturnCoilSystem()
//Author        :PATNI/MRP
//Purpose       :Returns the
//****************************************************************************
bool CPqm::IsSaturnCoilSystem(
)const
{
    return 	GP_SYS_ISSATURN(m_systype); // Added by Meghana San for check system type 10/7/2008 4:42:51 PM
    //	return true;    // V9.25 Coil support@AMT

}

//********************************Method Header********************************
//Method Name   : DeleteAllGateIteams()
//Author        : PATNI / JS
//Purpose       :
//*****************************************************************************
bool CPqm::DeleteAllGateIteams()
{
    LPCTSTR FUNC_NAME = _T("CPqm::DeleteAllGateIteams");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_gate_list) {
        m_gate_list->RemoveAll();
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_gate_list pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   : AddToGateList()
//Author        : PATNI / JS
//Purpose       :
//*****************************************************************************
bool CPqm::AddToGateList(
    const CString& f_gate_info
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::AddToGateList");

    if (m_gate_list) {
        m_gate_list->Add(f_gate_info);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_gate_list pointer is NULL"));
        return false;
    }
}

//+Patni-PJS/2010May09/Added/JaFT# IR-141
//*****************************************************************************
//Method Name   : RaiseInstancesAuditEvent
//Author        : Patni/PJS
//Purpose       : This function is wrapper for RaiseInstancesAuditEvent of
//                CPqmAuditLog class
//*****************************************************************************
bool CPqm::RaiseInstancesAuditEvent(
)
{
    return m_pqm_audit_log.RaiseInstancesAuditEvent();
}

//********************************Method Header********************************
//Method Name   :IsActiveDialog()
//Author        :PATNI/ASHISH SM3
//Purpose       :
//*****************************************************************************
bool CPqm::IsActiveDialog(
)
{
    if (m_is_active_cfa_dialog || m_is_active_coilinfo_dialog ||
        m_is_active_coilselection_dialog || m_is_active_gateinfo_dialog ||
        m_is_active_scanoffset_dialog || m_is_active_amb_dialog)
        return true;

    return false;
}

//********************************Method Header********************************
//Method Name   :ProlimAutoPlan()
//Author        :PATNI/ASHISH SM3
//Purpose       :
//*****************************************************************************
void CPqm::ProlimAutoPlan(
    const int f_nindex
)
{

    LPCTSTR FUNC_NAME = _T("CPqm::ProlimAutoPlan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
    //Check if any other protocol is being edited or not
    if (m_pqm_procon && m_pqm_procon->GetCurrentProtocol()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Can not Start AutoPlan -> WSE is already open"));
        return;
    }

    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2


    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-MP/2009Mar13/Commented/ACE-1/MCM0213-00167
    //CPqmProtocol* l_waitprotocol = NULL;

    //+Patni-MP/2009Dec08/Commented/MVC005121
    //l_protocol = m_pqm_study->GetItemFromPosition(f_nindex + 1);
    CPqmProtocol* l_protocol = m_pqm_study->GetItemFromProtocol(f_nindex);
    //-Patni-MP/2009Dec08/Commented/MVC005121

    if (l_protocol == NULL) {
        //+Patni-RUP/2009Aug24/Modified/cpp test corrections
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        //-Patni-RUP/2009Aug24/Modified/cpp test corrections
        return;
    }

    //-Patni-MP/2009Mar13/Commented/ACE-1/MCM0213-00167
    /* for( l_protocol = m_pqm_study->GetNextItem(l_protocol);
     	 l_protocol;
     	 l_protocol = m_pqm_study->GetNextItem(l_protocol))
     {
     	if((l_curprotocol = (CPqmProtocol*)l_protocol->GetProtocol()) == NULL)
     		continue;

     	if(l_curprotocol->GetPosition()!= f_nindex)
     		continue;

     	l_scan_number = l_curprotocol->GetPqmPrMode()->GetScanNumber();
     	break;
     }*/
    int l_scan_number = l_protocol->GetPqmPrMode()->GetScanNumber();

    if (l_scan_number == -1) return;

    CPqmProtocol* l_curprotocol = NULL;

    for (; l_protocol; l_protocol = m_pqm_study->GetNextItem(l_protocol)) {
        l_curprotocol = m_pqm_study->GetNextItem(l_protocol);

        if (l_curprotocol == NULL) {
            continue;
        }

        if ((l_curprotocol->GetAcqStatus() == C_PR_STATUS_WAIT) &&
            (l_curprotocol->GetPqmPrMode()->GetScanNumber() == l_scan_number) &&
            l_curprotocol->GetHoldScan() & VFC_HOLD_PRE_SCAN &&
            !(l_curprotocol->GetPlnStatus() & VFC_PLN_STATUS_GLOC) &&
            (l_curprotocol->GetChildThreadStatus() != CT_DELETE)	//Patni-HAR/2010Jan18/Added/DeFT#MVC006364
            //&&
            //!l_curprotocol->GetFluoro() //out of scope.
           ) {

            //+Patni-MP/2009Mar13/Commented/ACE-1/MCM0213-00167
            /*m_pqm_study->GetTopWaitPos( &l_waitprotocol );// WAIT:O FILLER_WAIT:X

            if( l_waitprotocol->GetProtocol() != NULL &&
            	!(l_waitprotocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {
            	m_after_auto_scan = 2;
            }*/
            //-Patni-MP/2009Mar13/Commented/ACE-1/MCM0213-00167

            /*
            PqmListSelectPos(l_curprotocol->GetPosition(),FALSE,0);//imp  GUI
            if ( Focusblink( TRUE ) )//no  GUI
            {
               	MTBblink( FALSE );//no GUI
            	PqmPmActionTools(K_PM_PROLIM_GLOC,obj);//no GUI

            	PqmPqGetTopWaitPos(&waitObj,NULL);// WAIT:O FILLER_WAIT:X //imp
            	if(waitObj != NULL && !(waitObj->holdScan & VFC_HOLD_PRE_SCAN))//imp
            	{
            		PqmGlobal.after_autoScan = 2;//imp

            		pqmProlimTimerSetup();// no imp GUI
            	}
            }
            */

            //+Patni-MP/2009Mar13/Commented/ACE-1/MCM0213-00167
            WriteToUI(PQM_MSG_INVOKE_LOCATOR);//ashish SM3		Auto Plan
            break;
            //-Patni-MP/2009Mar13/Commented/ACE-1/MCM0213-00167
            //Send message to GUI to select the item in List
            //Invoke Locator
        }

        //		else
        //	    	PqmPmHoldPutProcess( K_PM_PROLIM_GLOC,obj);

    }
}

//+Patni-KSS/2010Oct08/Added/V1.35#IR-167
bool CPqm::ProcessProbeMsgForIRS(
    const pqm_msg_tag f_msg
)const
{
    if (m_pqm_procon && m_pqm_procon->ProcessProbeMsgForIRS(f_msg)) {
        return true;
    }

    return false;

}
//-Patni-KSS/2010Oct08/Added/V1.35#IR-167

//+Patni-Sribanta/2010Nov15/Added/IR-170
//****************************Method Header************************************
//Method Name   :ShowSequenceToGUI()
//Author        :PATNI/Sribanta
//Purpose       :
//*****************************************************************************
bool CPqm::ShowSequenceOnGUI(
    const int* f_acq_order,
    const CString& f_pas_path,
    const int f_actual_seq_transfered,	//IR-170_Impact_Scenario
    const int f_total_seq
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ShowSequenceOnGUI");

    if ((NULL == f_acq_order) ||
        (0 >= f_total_seq) ||
        //IR-170_Impact_Scenario
        (0 >= f_actual_seq_transfered) ||
        (f_pas_path.IsEmpty())
       ) {
        return false;
    }

    CString l_tmp_acq_order(_T(""));

    //IR-170_Impact_Scenario
    SetNoOfSequence(f_actual_seq_transfered);

    for (int l_seq_index = 0; l_seq_index < f_total_seq; l_seq_index++) {

        l_tmp_acq_order.Format(_T("%d"), f_acq_order[l_seq_index]);
        ReceiveSequences(f_pas_path, l_tmp_acq_order);
    }

    SendSequencesToGUI();

    if (!NewSequencesAdded(f_total_seq)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("NewSequencesAdded failed"));
        return false;
    }

    return true;
}
//-Patni-Sribanta/2010Nov15/Added/IR-170

void CPqm::WriteDebugDataToUI(CString f_csDebugData)
{
    BSTR l_cstemp = f_csDebugData.AllocSysString();
    m_pqmipcmanager->SendDebugMessageToGUI(l_cstemp);
    SysFreeString(l_cstemp);
}

//********************************Method Header********************************
//Method Name   : UpdateSARInfoOnPage()
//Author        : PATNI/PJS

//*****************************************************************************
void CPqm::UpdateSARInfoOnPage()
{
    LPCTSTR FUNC_NAME = _T("CPqm::UpdateSARInfoOnPage");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //This message is sent to GUI thread (Which handles delete duplicate)
    //so that there is no synchronization issue while deletion/duplication and Sar updation
    WriteToUI(PQM_UPDATE_SAR_INFO);

}

//****************************Method Header************************************
//Method Name   :InitializePqm()
//Author        :PATNI/JCM
//Purpose       :This method initializations all the information required
//               for the PQM application
//*****************************************************************************
//Patni-AMT/2010Mar10/Modified/Ph-3#IR-100-StudyUID
bool CPqm::InitializePqm(
    //CString study_uid
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::InitializePqm");

    try {
        //+Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

        //+TMSC-Tanoue/2010Sep03/MVC008324
        if (pr_init_count != K_ALL_PR) {
            CString strlog(_T(""));
            strlog.Format(_T("PR count is mismatch !! [pr_count:%d][K_ALL_PR:%d]"),
                          pr_init_count, K_ALL_PR);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, strlog);
            strlog = CPqmUtilities::GetMultiLingualString(_T("IDS_NO_MATCHING_WARNING_ID"));
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(strlog,
                    MPlus::EH::Information, _T("PQM"), _T("PQM"));
            return false;
        }

        //-TMSC-Tanoue/2010Sep03/MVC008324

        //m_study_uid     = study_uid;
        //-Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
        //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        //m_coilDB        = new CCoilDB();
        m_colorinfo     = new CColorSetting();

        m_is_active_scanoffset_dialog   = false;
        m_is_active_cfa_dialog          = false;
        m_is_active_coilinfo_dialog     = false;
        m_is_active_gateinfo_dialog     = false;
        m_is_active_coilselection_dialog = false;
        m_is_active_amb_dialog			= false;

        m_dbsa_study_mgr = new CPQMStudyMgr(this);

        //Patni-AMT/2009Nov11/Deleted/MSA0248-00100-Code Review
        //+Patni-RUP/2009Sep14/Modified/cpp test corrections
        if (false == InitializeSMInfo()) {
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_READ_SMCF_FILE,
                    _T("IDS_FAIL_READ_SMCF_FILE"), SCAN_ERR_NOTIFY, _T("Pqm.cpp"));
            return false;
        }

        //-Patni-RUP/2009Sep14/Modified/cpp test corrections
#if NORMAL_COIL

        // This code is not required. Might be in future we need to support it.
        // @AMT@20080730
        if (!LoadCoilInfo()) {

            CString string = L"";
            string.LoadString(IDS_ERR_NO_COILDB);

            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_NO_COILDB,
                    _T("IDS_ERR_NO_COILDB"), SCAN_ERR_NOTIFY, L"pqm.cpp");
            return false;
        }

#endif // #if NORMAL_COIL

        //+Patni-MJC/2009Aug17/Modified/cpp test corrections
        if (m_data_mgr == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_data_mgr pointer is NULL"));
            return false;
        }

        //-Patni-MJC/2009Aug17/Modified/cpp test corrections

        // + Create databases of Vantage. @ AMT
        if (!m_data_mgr->InitializeDB()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("InitializeDB() failed."));
            //Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
            return false;
        } // - Create databases of Vantage. @ AMT

        //Handles if any  problem occurs in reading the xml file
        if (!ReadPqmAppConfigFile()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Can not read Application Config file"));
            return false;
        }

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
#if 1

        if (!LoadColorInfo()) {
            CString error_initialize = L"";
        }

#endif
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
        InitializeCouchConfig();

        //+Patni-Ravindra Acharya/Added On 25March2010//IR-133
        InitializeRis() ;
        //Patni-Ravindra Acharya/Added On 25March2010//IR-133

        m_pqm_study = new CPQMStudy(this);


        m_autolocator_postproc =  new CAutoLocatorPostProc(this);
		m_tiprep_updater  = new CTiPrepUpdater(); //Anulekha
        m_cfshift_check = new CCFShiftCheck(m_pqm_study);
        m_coilchannelmode = new CCoilChannelMode(m_pqm_study);
        m_autolocator_parametersetter = new CAutoLocatorParameterSetter(this);
        m_gate_param_setter.SetPQM(this);
        m_pascom->SetPqm(this);
        m_dbsa_study_mgr->DBSA_Init();
        m_pqmSpeeder->InitializeSpeeder();

        //Serialize sequence parameter list, this list will be used by PQM GUI.
        SerializeSequenceParamList();

        //Patni-AMT/2010Mar22/Modified/Ph-3# IR-100
        if (FALSE == CPqmUtilities::CreateCurrentScratchPad()) {
            return false;
        }

        //Patni-PJS/2010Feb2/Modified/IR-97
        //m_wfdaimplementer = new CWFDAImplementer(this);
        //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
        m_pqmcds.PqmCdsInit(this);
        //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

        const bool l_auto_insert_lincence = (LICENSE_OK == CheckLicense2(NULL, "A_MAPSHIM"));
        m_autoinsert_proc.InitAutoInsertProc(this, l_auto_insert_lincence);

        m_master_slave = new CMasterSlave(this);


        m_defpguid_xml_data = new CDefaultPGUIDXmlDataManager();

        if (m_defpguid_xml_data) {
            if (FALSE == m_defpguid_xml_data->ReadDefaultPGUIDXML()) {
                m_is_pguidxml_file = false;
            }
        }

        m_sar_manager = new CPqmPmSARManager(this);



    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CMemmoryException at CPqm::InitializePqm()"));
        e->Delete();
        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :IsAcqmanValid()
//Author        :PATNI / AC
//Purpose       :Checks for validity of Acqman manager - useful for error conditions
//*****************************************************************************
bool CPqm::IsAcqmanValid(
) const
{
    LPCTSTR FUNC_NAME = _T("CPqm::IsAcqmanValid");
    //Patni-AMT/2010Mar10/Modified/Ph-3#IR-100-Code Review
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_acq_manager) {
        return true;
    }

    return false;
}

/****************************Method Header************************************
//Method Name   :ConnectProCon()
//Author        :PATNI/SS
//Purpose       :
//*****************************************************************************/
int CPqm::ConnectProCon(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ConnectProCon");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_procon == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_procon pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-SS/2009Mar23/Added and/PSP1#272
    bool l_result = false;
    int loop_count = 1;

    if (false == (l_result = m_pqm_procon->ConnectToProcon())) {
        while (loop_count < 5) {
            Sleep(1000);

            if (true == (l_result = m_pqm_procon->ConnectToProcon())) {
                break;
            }

            loop_count++;

            if (loop_count >= 5) {
                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(
                    IDS_ERR_CAN_NOT_CONNECT_PROCON, _T("IDS_ERR_CAN_NOT_CONNECT_PROCON"), SCAN_ERR_WARNING,
                    _T("CPqm::ConnectProCon"));
            }
        }
    }

    return  l_result;

    //-Patni-SS/2009Mar23/Added and/PSP1#272

}


bool CPqm::AMBSlaveMatchSequenceParam(
    VARIANT* f_pData,
    const int f_start_index,
    const int f_end_index
)
{

    LPCTSTR FUNC_NAME = _T("CPqm::AMBSlaveMatchSequenceParam");
    PQM_SCOPED_TRACE();

    protocol_Info_Vec* l_list_protocol_info_vec = (protocol_Info_Vec*) f_pData->byref;

    CPqmProtocol* l_protocol = NULL;
    ProtocoInfo* l_protocol_info = NULL;

    int i = 0;

    for (int i_seq = f_start_index ; i_seq < f_end_index ; i_seq++) {
        l_protocol = m_pqm_study->GetItemFromPosition(i_seq + 1);

        l_protocol_info = l_list_protocol_info_vec->at(i++);

        LogProtocolInfo(l_protocol, l_protocol_info);

        if (!IsMatchSequenceParam(l_protocol, l_protocol_info)) {
            CString l_strErrMsg =  CPqmUtilities::GetMultiLingualString(_T("IDS_SCAN_NOT_PERFORM"));
            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPqmUtilities::GetMultiLingualString(l_strErrMsg), _T("PQM"),
                SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);
            WriteToUI(PQM_MSG_ABORT_WAIT);
            return false;
        }

    }

    return true;
}

//****************************Method Header************************************
//Method Name   :ScanSeries()
//Author        :PATNI/JCM
//Purpose       :This method  asks acqman to start scan
//*****************************************************************************
bool CPqm::ScanSeries(
    VARIANT* f_pData,
    const int f_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ScanSeries");

    CString str_id(_T(""));
    str_id.Format(_T("Series UID: %d"), f_pos);
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_id);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_acq_manager pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CScanProcedure* l_scan_procedure = m_acq_manager->GetScanProcedure();
    l_scan_procedure->SetIsCdsWaitCursor(false);

    if (l_scan_procedure->GetAPCRetry()) {
        l_scan_procedure->SetAPCRetry(FALSE);
    }

    if (l_scan_procedure->GetCFARetry()) {
        l_scan_procedure->SetCFARetry(PQM_CFA_NONE);
    }

    l_scan_procedure->SetScanMode(PQM_PROTOCOL_SCAN);

    bool result = false;
    CPqmProtocol* protocol = NULL;

    switch (l_scan_procedure->GetScanStart()) {
        case PQM_APC_START:
            l_scan_procedure->APCStart();
            break;

        case PQM_SCAN_RESTART:
            l_scan_procedure->AcquisitionScanRestart();
            break;

        default:

            //+Patni-MJC/2009Aug17/Modified/cpp test corrections
            if (m_pqm_study == NULL) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("m_pqm_study pointer is NULL"));
                return false;
            }

            //-Patni-MJC/2009Aug17/Modified/cpp test corrections

            //+Patni-Hemant-MP/2011Mar08/Modified/TMSC-REDMINE-1486-Part2
            const int l_protocol_pos = m_pqm_study->GetTopWaitPos(&protocol) ;

            CString str;
            str.Format(_T("TopMost Protocol for Scan is : %d"), l_protocol_pos);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

            if (l_protocol_pos < 0 || protocol == NULL) {
                //-Patni-Hemant-MP/2011Mar08/Modified/TMSC-REDMINE-1486-Part2

                //+Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
                /*
                CPQMLogMgr::GetInstance()->WriteEventLog(
                    _T("No protocol in study file with wait status !"),
                    ERROR_LOG_INFORMATION1 , _T("Pqm.cpp"));
                	*/
                //-Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179

                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_WAIT_PROTOCOLS, _T("IDS_NO_WAIT_PROTOCOLS"),
                        SCAN_ERR_NOTIFY, L"pqm.cpp");
                WriteToUI(PQM_MSG_ABORT_WAIT);
                return result;
            }

            //+Patni-Hemant/2010Feb17/Added/ACE-Phase#3/CDS Requirement
            //In Vantage9.51, pqmAcqTopProtocol is called in which there is checking for
            //current object also so in that case we should not do anything and return.
            if (m_pqm_study->GetCurrentProtocol()) {

                //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Aborting...->Protocol with current status already present"));

                WriteToUI(PQM_MSG_ABORT_WAIT);
                return result;
            }

            //-Patni-Hemant/2010Feb17/Added/ACE-Phase#3/CDS Requirement
            //+Patni-ARD/2009Sept08/Modified/Prototype 2
            ProtocoInfo* l_protocol_info = (ProtocoInfo*) f_pData->byref;
            LogProtocolInfo(protocol, l_protocol_info);

            //Patni-MSN/2009Nov04/Modified/Code Review - Method name change
            if (!IsMatchSequenceParam(protocol, l_protocol_info)) {

                //+TMSC-Tanoue/2010Mar05/MSA0251-00247
                //MRERROR_LOG_DATA l_error_log_data;

                CString l_strErrMsg =  CPqmUtilities::GetMultiLingualString(_T("IDS_SCAN_NOT_PERFORM"));
                /*
                l_error_log_data.error_message = l_strErrMsg;
                l_error_log_data.error_source = _T("PQM");
                CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data);
                */
                CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    CPqmUtilities::GetMultiLingualString(l_strErrMsg), _T("PQM"),
                    SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
                //-TMSC-Tanoue/2010Mar05/MSA0251-00247

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);

                WriteToUI(PQM_MSG_ABORT_WAIT);
                return false;
            }

            //-Patni-ARD/2009Sept08/Modified/Prototype 2
            //+Patni-Manishkumar/2Nov09/ADDED/IR88/Chk acq. status in varfield
            if (!m_pqm_study->IsMatchAcqStatusOfVarField(l_protocol_info)) {
                CString l_strErrMsg =  CPqmUtilities::GetMultiLingualString(_T("IDS_PQM_INCORRECT_PROTOCOL_SCANREQUEST"));
                MRERROR_LOG_DATA l_error_log_data;
                l_error_log_data.error_message = l_strErrMsg;
                l_error_log_data.error_source = _T("PQM");
                CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data, MPlus::EH::Information);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);

                WriteToUI(PQM_MSG_ABORT_WAIT);
                return false;
            }

            //-Patni-Manishkumar/2Nov09/ADDED/IR88/Chk acq. status in varfield

            if (protocol == NULL) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("protocol pointer is NULL"));
                return result;
            }

            CPqmPrMode* l_pqmpr_mode = protocol->GetPqmPrMode();

            if (l_pqmpr_mode == NULL) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("l_pqmpr_mode pointer is NULL"));
                return result;
            }

            l_pqmpr_mode->SetRecvCoilId(m_selected_coil_key);
            //-Patni-RUP/2009Aug24/Modified/cpp test corrections

            if ((protocol == NULL) || (protocol->GetProtocol() < 0)) {

                //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Aborting...->IDS_ERR_ILLEGAL_PROTOCOL error"));

                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(
                    IDS_ERR_ILLEGAL_PROTOCOL, _T("IDS_ERR_ILLEGAL_PROTOCOL"), SCAN_ERR_NOTIFY, _T("Pqm.cpp"));

                m_pqm_study->SetScanModeOfHead();
                //PqmUiSetSensitive(W_SCAN_START_BTN,FALSE);
                l_scan_procedure->ResetCurrentRequest();
                return result;

            } else if (protocol->GetProcessing() > 0) {

                //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Aborting...->GetProcessing() is greter than zero"));
                //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2

                //Patni-SS/2009June02/Internal defect
                WriteToUI(PQM_MSG_ABORT_WAIT);
                return result;

            } else {

                //+Patni-Hemant/2010Aug10/Commented/MVC008295
                ////+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
                ////m_acq_manager->ScanStart(protocol, FALSE);
                //if (protocol->GetASCOMPASSFlag()) {
                //    /* COMPASS is set if the information is backed up coil */
                //    m_pqmcds.PqmCdsBackUpCoil(protocol->GetSaturnCoil());
                //}
                ////-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
                //-Patni-Hemant/2010Aug10/Commented/MVC008295

                //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("calling Acqman ScanStart"));
                //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2

                m_acq_manager->ScanStart(protocol, FALSE);

            }
    }

    return result;
}

//****************************Method Header************************************
//Method Name   :LoadStudyLOID()
//Author        :PATNI/JS
//Purpose       :This method Loads the instance of CStudy
//*****************************************************************************
bool CPqm::LoadStudyLOID(
    LPCTSTR f_patientloid,
    LPCTSTR f_studyloid,
    LPCTSTR f_pas_path //Patni-Sribanta/2010Nov10/Added/IR-170
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::LoadStudyLOID");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //Patni-HAR/2010Jun28/Commented/V1.20#MVC008324_0
    //WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock

    //    PostThreadMessage(CPQMIPCManager::dThreadId,MSG_LOADSERIES,WPARAM(f_studyloid),NULL);
    //  WaitForSingleObject(CPQMIPCManager::m_ThreadEvent,INFINITE);


    if (!m_dbsa_study_mgr->LoadStudyLOID(f_studyloid, true)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("LoadStudyLOID() Failed."));
        WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        WriteToUI(PQM_MSG_LOADSTUDY_ERROR);
        return false;
    }

    CString l_study_uid = GetStudyUid();

    //self->m_pqmptr->SetStudyUid(l_study_uid);

    //Patni-Sribanta/2010Nov11/modified/IR-170
    m_post_process_flag = FALSE;
    CScanProcedure* l_scan_proc_ptr = m_acq_manager->GetScanProcedure();

    if (l_scan_proc_ptr) {
        l_scan_proc_ptr->SetPrescanHook(FALSE);
    }

    if (m_pqmSAR) {
        m_pqmSAR->ResetRfl();
    }

    if (l_scan_proc_ptr && l_scan_proc_ptr->GetScanProcProtocol()) {
        l_scan_proc_ptr->SetScanProcProtocol(NULL);
    }


    bool l_same_study = false;

    if (PQM_ERROR_NORMAL != m_pqm_study->LoadStudy(l_study_uid, m_mode, m_anotherpqmstudy, l_same_study)) {
        WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        WriteToUI(PQM_MSG_LOADSTUDY_ERROR);
        return false;

    } else {
        if ((K_IMAGING == m_mode) && (!l_same_study)) {
            m_pqm_study->SetCurrentHighestDbdt(VFC_DBDT_IEC_OPERATING_NORMAL);
            m_pqm_study->SetCurrentHighestSar(IEC_SAR_NORMAL_MODE);
            m_is_same_study = false;
            IntializeShimPrepProbeData();
            m_pqm_study->SetRMSAEStatusForStudy(false);

        } else {
            ResetShimDataFromFile();
        }
    }

    if (!m_pqm_study->FillProtocolVector(f_pas_path)) {
        WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        WriteToUI(PQM_MSG_LOADSTUDY_ERROR);
        return false;

    } else {
        CPqmProtocol* l_topwait_protocol = NULL;

        if (NULL != m_pqm_procon) {
            if (m_pqm_procon->PqmPqIsGlocOk(m_pqm_study->GetTopWaitPos(
                                                &l_topwait_protocol), true)) {
                WriteToUI(PQM_MSG_ON_FLAG_LOCATOR);
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, _T("CPqm::LoadStudyLOID"),
                      _T("CPqmProcon object NULLL"));

        }
    }

    int	l_injected_time_diff = 0;

    if ((K_IMAGING == m_mode) && true == m_pqm_study->CheckForInjectedTime(l_injected_time_diff) && GetMulitScanDynamic_License()) {
        WriteToUI(PQM_MSG_INJECT_TIMER_DISPLAY);
    }

    m_pqm_study->CheckMRSValueInVF();

    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
    WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
    WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    //    m_dbsa_study_mgr->SetLoid(f_studyloid);


    //+Patni-PJS/2010May09/Added/JaFT# IR-141
    CString l_ptn_uid = m_dbsa_study_mgr->GetPatientUID(f_patientloid);
    CString l_std_uid = m_dbsa_study_mgr->GetStudyUID(f_studyloid);
    SetStudyDetails(l_ptn_uid, l_std_uid);

    //-Patni-PJS/2010May09/Added/JaFT# IR-141
    if (!l_same_study) {
        m_gate_param_setter.ResetGateInterval();
    }

    CPqmProtocol* l_last_processed_protocol = m_pqm_study->GetLastCompletedProtocol();

    if ((!l_same_study) || ((l_same_study) && (l_last_processed_protocol == NULL))) {
        CPqmPrMode prModeReset;
        m_acq_manager->SetProtocolModeLastScan(&prModeReset);
    }

    GetAMB()->PqmAmbDeleteSlave();
    return true;
}

//****************************Method Header************************************
//Method Name   :DeleteAll()
//Author        :PATNI/PJP
//Purpose       :This method deletes the instance of CSeries
//*****************************************************************************
BOOL CPqm::DeleteAll(
    BSTR bStr
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::DeleteAll");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_dbsa_study_mgr) {
        return m_dbsa_study_mgr->DeleteAll(bStr);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_dbsa_study_mgr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   :UpdateComment()
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
bool CPqm::UpdateComment(
    BSTR bStr,
    BSTR scancomment
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::UpdateComment");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_dbsa_study_mgr) {
        m_dbsa_study_mgr->UpdateComment(bStr, scancomment);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_dbsa_study_mgr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

}

//********************************Method Header********************************
//Method Name   :SendSequencesToMemory()
//Author        :PATNI/DJ
//Purpose       :Send Sequnece list from PQM to Memory.
//*****************************************************************************
void CPqm::SendSequencesToMemory(
    VARIANT* pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SendSequencesToMemory");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    //SequenceList seq_list;
    //Meghana testing

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    SequenceList* pSeq = NULL;

    if (pdata != NULL) {
        pSeq = (SequenceList*)pdata->byref;
        //pdata->byref = seq_list;

        //m_pascom->SendSequence(m_rec_seq_list);
        m_pascom->SendSequence(*pSeq);

    } else {
        pSeq = NULL;
        m_pascom->SendSequence(*pSeq);
    }
}

//********************************Method Header********************************
//Method Name   : SendCoilSelectionReady()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
void CPqm::SendCoilSelectionReady(
    const int f_index
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SendCoilSelectionReady");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    // PENDING
    //m_pqm_study->GetTopWaitPos(&l_protocol);
    CPqmProtocol* l_protocol = m_pqm_study->GetProtocol(f_index);

    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
    if (l_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug24/Modified/cpp test corrections
    m_CoilListcoordinate = &(l_protocol->GetSaturnCoil()->CoilListcoordinate);
    m_show_coil = &(l_protocol->GetSaturnCoil()->CoilList);

    WriteToUI(PQM_MSG_PQM2COILSEL_SHOW_COIL);

    if (l_protocol->GetSaturnCoil()->numSection != 0) {

        memset(&m_select_coil, 0, sizeof(coilSelect_t));
        m_select_coil.num = l_protocol->GetSaturnCoil()->numSection;

        for (int i = 0; i < l_protocol->GetSaturnCoil()->numSection; i++) {
            m_select_coil.select[i] = l_protocol->GetSaturnCoil()->RcvCoil[i];
        }

        WriteToUI(PQM_MSG_PQM2COILSEL_SELECTE_COIL);
    }

    // PENDING
    //Send TITLE_LABEL
    //PqmPmCoilSelSetTitleLabel

    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    //COMPASS during determine the position of center field after sending
    if (m_pqmcds.GetCDSCoilSelStatus() == CDS_ON) {
        //pqmPmCoilSelSetCenterPos(pmObj);
        WriteToUI(PQM_MSG_CDS_GET_CENTER_POS);
    }

    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    // Send information to evacuation
    if (!m_coil_selection_tool->GetCoilSelectionFlagActive() &&
        !m_coil_selection_tool->GetAcqSendFlag()) {

        WriteToUI(PQM_MSG_PQM2COILSEL_CONNECT_COIL);
    }
}

//********************************Method Header********************************
//Method Name   : CheckNextProtocol()
//Author        : PATNI/MRP
//Purpose       : Checks the next protocol for Auto Scan
//*****************************************************************************
void CPqm::CheckNextProtocol(
    const int f_protocol_no,
    const BOOL f_call_frm_gui
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::CheckNextProtocol");
    CString l_print_string(_T(""));
    l_print_string.Format(_T("Flag = %d"), f_call_frm_gui);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    if (m_acq_manager != NULL) {
        m_acq_manager->GetScanProcedure()->CheckNextProtocol(f_protocol_no, f_call_frm_gui);
    }
}

//********************************Method Header********************************
//Method Name   : SendAcqInfoForGate()
//Author        : PATNI/MRP
//Purpose       : Returns the flag to display the Coil Info dialog
//*****************************************************************************
void CPqm::SendAcqInfoForGate(

)const
{
    LPCTSTR FUNC_NAME = _T("CPqm::SendAcqInfoForGate");

    if (m_acq_manager) {

        CScanProcedure* scan_procedure =  m_acq_manager->GetScanProcedure();
        CPqmProtocol* protocol = NULL;

        if ((m_pqm_study->GetTopWaitPos(&protocol) < 0) || (protocol == NULL)) {

            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_WAIT_PROTOCOLS, _T("IDS_NO_WAIT_PROTOCOLS"), SCAN_ERR_NOTIFY, L"pqm.cpp");

        } else if (!scan_procedure->GetIsScanning()) {

            if (scan_procedure) {
                scan_procedure->SetScanProcProtocol(protocol);
                scan_procedure->AcquisitionGateInfo(PQM_INFO_BIT_GATE);

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_procedure pointer is NULL"));
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Scan is going on. During Scan GateMethod cannot be sent"));
        }
    }
}

//Patni-ARD/2009Mar18/Added/ACE1 # Handling of Stabilization Start
int CPqm::FileCopy(
    BSTR f_existingfilename,
    BSTR f_newfilename
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::FileCopy");
    CString l_existingFile = f_existingfilename;
    CString l_newFileName = f_newfilename;

    int l_len = l_existingFile.GetLength();
    char* l_existing_File = new char[l_len + 1];
    memset(l_existing_File, 0, sizeof(char) * (l_len + 1));
    wcstombs(l_existing_File, l_existingFile, l_len + 1);//+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()

    l_len = l_newFileName.GetLength();
    char* l_new_FileName = new char[l_len + 1];
    memset(l_new_FileName, 0, sizeof(char) * (l_len + 1));
    wcstombs(l_new_FileName, l_newFileName, l_len + 1);//+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()

    int l_result = AcqFileCopy(l_existing_File, l_new_FileName);

    if (l_result != E_NO_ERROR) {
        CString l_errmsg = _T("");
        l_errmsg.Format(_T("AcqFileCopy(%s, %s) function FAILED.Error Value : %d."),
                        l_existingFile, l_newFileName, l_result);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errmsg);
    }

    //-Patni-DKH/2010Dec21/Modified/TMSC-REDMINE-655-Part-I

    DEL_PTR_ARY(l_existing_File);	//+Patni-HAR/2009Aug11/Added/Memory Leaks

    DEL_PTR_ARY(l_new_FileName);	//+Patni-HAR/2009Aug11/Added/Memory Leaks

    return l_result;
}
//Patni-ARD/2009Mar18/Added/ACE1 # Handling of Stabilization End

//+Patni-DKH/2010Jul23/Added/IR-153
//*****************************************************************************
//Method Name   : IsCFShiftSupportedSystem
//Author        : Patni/DKH
//Purpose       : This function is used to check the sys type
//                which support CF shift
//*****************************************************************************
//Patni-DKH/2010Aug03/Modified/IR-153-Review Comment
BOOL CPqm::IsCFShiftSupportedSystem()const
{
    LPCTSTR FUNC_NAME = _T("CPqm::IsCFShiftSupportedSystem");
    CPqm* l_pqm_ptr = m_acq_manager->GetPqm();

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    const int l_sys_type = l_pqm_ptr->GetSysType();

    if (GP_SYS_ISTITAN(l_sys_type) && !GP_SYS_ISVENUS(l_sys_type) && !GP_SYS_ISHSR(l_sys_type))  {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("This is CF Supported System"));
        return TRUE ;
    }

    return FALSE;
}

//-Patni-DKH/2010Jul23/Added/IR-153
//*****************************************************************************
//Method Name   : RaiseOrderRecordAuditEvent
//Author        : Patni/PJS
//Purpose       : This function is wrapper for RaiseOrderRecordAuditEvent of
//                CPqmAuditLog class
//*****************************************************************************
bool CPqm::RaiseOrderRecordAuditEvent(
)
{
    return m_pqm_audit_log.RaiseOrderRecordAuditEvent();
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
void CPqm::ProcessScanRelatedMsgForWFDA(
    CPqmProtocol* f_protocol,
    pqm_msg_tag f_msg_tag
)
{
    //According to QTS#36, We have to send scan start message to InScan server
    //after Prescan is completed
    CWFDAImplementer* l_wfdaimplementer = GetWFDAImplementer();

    if (f_protocol && l_wfdaimplementer) {

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
            l_wfdaimplementer->ScanStatusChanged(f_protocol, l_wfda_protocol_status);
        }
    }
}
*/
//-Patni-PJS/2010Feb2/Modified/IR-97


//****************************Method Header************************************
//Method Name   :SeriesScanDone()
//Author        :PATNI/JCM
//Purpose       :This method sends the notification regarding of scan done
//*****************************************************************************
void CPqm::SeriesScanDone(
) const
{
    LPCTSTR FUNC_NAME = _T("CPqm::SeriesScanDone");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inform PQM GUI from here"));
}


//Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator
//**************************************************************************
//Method Name   : InitPlanDupSettingsFile
//Author        : PATNI\DKH
//Purpose       : To initialize plan duplicate settings
//**************************************************************************
//Patni-AMT/2010Apr22/Modified/JaFT# IR87_CDR_13 return type changed to bool
bool CPqm::InitPlanDupSettingsFile()
{
    LPCTSTR FUNC_NAME = _T("CPQM::InitPlanDupSettingsFile");
    //Patni-AMT/2010Apr22/Modified/JaFT# IR87_CDR_13-CODING RULE_13
    CString l_studyfile = GetStudyUidPath();
    bool    l_ret = false;

    if (NULL != m_planduplicateSettings) {
        l_ret = m_planduplicateSettings->InitPlanDupSettingsFile(l_studyfile);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pointer m_planduplicateSettings is NULL"));
    }

    //Patni-AMT/2010Apr22/Modified/JaFT# IR87_CDR_13-Code review
    return l_ret;
}

//+Patni-HEMANT/ADDED On 6/12/2009 1:14:17 PM/ ACE-2 / WFDA
//************************************Method Header************************************
// Method Name  : DeleteProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqm::DeleteProtocol(
    const int f_protocol_index
)
{
    //+Patni-PJS/2010Feb2/Modified/IR-97
    /*
    if (m_wfdaimplementer) {

        return m_wfdaimplementer->DeleteProtocol(f_protocol_index);
    }
    */
    //-Patni-PJS/2010Feb2/Modified/IR-97
    ASSERT(FALSE);
    return false;
}

//************************************Method Header************************************
// Method Name  : DuplicateProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqm::DuplicateProtocol(
    const int f_index_to_duplicate,
    const int f_duplicate_index
)
{
    //+Patni-PJS/2010Feb2/Modified/IR-97
    /*
    if (m_wfdaimplementer) {

        return m_wfdaimplementer->DuplicateProtocol(f_index_to_duplicate, f_duplicate_index);
    }
    */
    //-Patni-PJS/2010Feb2/Modified/IR-97
    ASSERT(FALSE);
    return false;
}

void CPqm::StartPQMDebug(const int f_idebugOption, const UINT f_buttonmask)
{
    CPQMDebugInfo::GetInstance(this)->StartPQMDebug(f_idebugOption, f_buttonmask);
}

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqm::OnOffCDSDebug(
    const BOOL f_OnOff
)
{
    m_pqmcds.OnOffCDSDebug(f_OnOff);
}

//************************************Method Header************************************
// Method Name  : OnOffAsCompass
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqm::OnOffAsCompass(
    const int f_protocol,
    const bool f_OnOff
)const
{
    if (m_pqm_study) {
        return m_pqm_study->OnOffAsCompass(f_protocol, f_OnOff);
    }

    return false ;
}

//************************************Method Header************************************
// Method Name  : CDSDebugClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqm::CDSDebugClosed(
    VARIANT** f_compass_debug_param,
    bool f_set
)
{
    if (f_compass_debug_param && (*f_compass_debug_param)) {

        COMPASS_Debugg_Param* l_compass_debug_param =
            (COMPASS_Debugg_Param*)(*f_compass_debug_param)->byref ;

        if (f_set) {

            m_pqmcds.pqmUiDebugSetCdsCB(l_compass_debug_param);

        } else {

            m_pqmcds.pqmUiDebugCancelCdsCB(l_compass_debug_param);
        }

        return true;
    }

    ASSERT(FALSE);
    return false;
}

//************************************Method Header************************************
// Method Name  : CoilSelectionClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqm::CoilSelectionClosed(
    const BOOL f_ok_pressed
)
{
    SetActiveDialog(IS_COIL_SELECTION_DIALOG, false);

    CScanProcedure* l_scan_procedure = m_acq_manager->GetScanProcedure();

    if (l_scan_procedure) {
        l_scan_procedure->CoilSelectionClosed(f_ok_pressed);
    }
}
//-Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement

bool CPqm::PqmDebugStatusLogOpen()
{
    if (NULL != m_debugInfo) {
        return CPQMDebugInfo::GetInstance(this)->PqmDebugStatusLogOpen();
    }

    return false;
}


//************************************Method Header************************************
// Method Name  : IsSameStudy
// Author       : PATNI/ PJS
// Purpose      : Get m_same_study member variable of the class.
//***********************************************************************************
bool CPqm::IsSameStudy(
) const
{
    return m_is_same_study;
}
//-Patni-PJS/2009Oct29/Added/IR-92

//****************************Method Header************************************
//Method Name   : AddCoilToList()
//Author        : PATNI/HAR
//Purpose       : Add coil in to list
//*****************************************************************************
void CPqm::AddCoilToList(
    CString coilDesc
)
{
    // AMT@ConnectedCoilList
    m_connect_coil_list.AddTail(coilDesc);
}

//****************************Method Header************************************
//Method Name   : DeleteAllCoilData()
//Author        : PATNI/HAR
//Purpose       : It Deletes All coil
//*****************************************************************************
void CPqm::DeleteAllCoilData(
)
{
    // AMT@ConnectedCoilList
    m_connect_coil_list.RemoveAll();
}

//********************************Method Header********************************
//Method Name   :IsSkipRgn()
//Author        :PATNI/
//Purpose       :
//****************************************************************************
bool CPqm::IsSkipRgn(
) const
{
    return m_skip_rgn;
}

//+Patni-ARD/2009Sept01/Modified/IR GUI IPC check SEQ
//************************************Method Header************************************
// Method Name  : IsMatchSequqnceParam
// Author       : PATNI/ ARD
// Purpose      : Check if ScanID or AcqOrder or scantime of current scanning protocol in study file
//				  is same as that selected from PQM list
//***********************************************************************************
//Patni-ARD/2009September8/Modified/Prototype 2
//Patni-MSN/2009Nov04/Modified/Code Review - Method name change
//+Patni-KSS/2011Feb17/CPP Test tool Execution
bool CPqm::IsMatchSequenceParam(
    CPqmProtocol* f_protocol,
    ProtocoInfo* f_protocol_info
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::IsMatchSequenceParam");

    if (f_protocol_info == NULL || f_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_info or f_protocol is NULL"));
        return false;
    }

    _bstr_t l_bstr_scanID = (_bstr_t)f_protocol->GetScanID();
    CString l_study_scan_ID(_T(""));
    l_study_scan_ID.Format(_T("%s"), (LPCTSTR)l_bstr_scanID);

    ProtocoInfo l_study_info(f_protocol->GetSeriesLOID(), f_protocol->GetProtocol(),
                             f_protocol->GetPosition(), l_study_scan_ID, f_protocol->GetAcqStatus());

    bool l_is_match_seq_param = false;

    if (*f_protocol_info == l_study_info) {
        l_is_match_seq_param = true;
    }

    //+Patni-DKH/2010September8/Added/MVC008755
    //If in AcquisitionDone function  varfield status is updated to DONE and we are checking
    //for protocol mismatch before we update at GUI end
    //we can skip checking of only status to avoid unnecessary protocol mismatch error
    if ((*f_protocol_info != l_study_info) && m_acq_manager->GetScanProcedure()->GetIsScanning()) {

        if ((C_PR_STATUS_CURRENT == f_protocol_info->acq_status) || (C_PR_STATUS_CURRENT == l_study_info.acq_status)) {
            l_is_match_seq_param = true;
        }
    }

    //-Patni-DKH/2010September8/Added/MVC008755

    CString l_log_msg = _T("");
    l_log_msg.Format(_T("*f_protocol_info == l_study_info->%d"), l_is_match_seq_param);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //Patni-ARD/2009September8/Modified/Prototype 2
    return l_is_match_seq_param;
}
//-Patni-KSS/2011Feb17/CPP Test tool Execution

void CPqm::LogProtocolInfo(
    CPqmProtocol* f_protocol,
    ProtocoInfo* f_list_info
)
{
    LPCTSTR FUNC_NAME = _T("CPQM::LogProtocolInfo");

    if (f_protocol == NULL || f_list_info == NULL) return;

    CString l_strErrMsg(_T(""));
    l_strErrMsg.Format(_T("GUI List: %s"), f_list_info->ToString());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);

    bstr_t l_bstr_scanID = (_bstr_t)f_protocol->GetScanID();
    CString l_study_scan_ID(_T(""));
    l_study_scan_ID.Format(_T("%s"), (LPCTSTR)l_bstr_scanID);

    ProtocoInfo l_study_info(f_protocol->GetSeriesLOID(), f_protocol->GetProtocol(),
                             f_protocol->GetPosition(), l_study_scan_ID,
                             f_protocol->GetAcqStatus());

    l_strErrMsg.Format(_T("IPC List: %s"), l_study_info.ToString());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);

}

//****************************Method Header************************************
//Method Name   :SeriesPrescanDone()
//Author        :PATNI/JCM
//Purpose       :This method sends notification to GUI that the scanning for the
//               given series is done..
//*****************************************************************************
void CPqm::SeriesPrescanDone(
) const
{
    LPCTSTR FUNC_NAME = _T("CPqm::SeriesPrescanDone");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inform PQM GUI from here"));
}

//**************************************************************************
//Method Name   : AllocateGroupIdForDuplicateProtocols
//Author        : PATNI/AKR
//Purpose       : This function is a wrapper of CMasterSlave::AllocateGroupIdForDuplicateProtocols()
//**************************************************************************
void CPqm::AllocateGroupIdForDuplicateProtocols(VARIANT* f_pdata)
{
    m_master_slave->AllocateGroupIdForDuplicateProtocols(f_pdata);
}
//**************************************************************************
//Method Name   : OnDeleteUpdateMasterSlave
//Author        : PATNI/AKR
//Purpose       : Wrapper function of CMasterSlave::OnDeleteUpdateMasterSlave()
//**************************************************************************
void CPqm::OnDeleteUpdateMasterSlave(VARIANT* f_pdata)
{
    m_master_slave->OnDeleteUpdateMasterSlave(f_pdata);
}
//**************************************************************************
//Method Name   : UpdateGroupNum
//Author        : PATNI/AKR
//Purpose       : Wrapper function of CMasterSlave::UpdateGroupNum()
//**************************************************************************
void CPqm::UpdateGroupNum()
{
    m_master_slave->UpdateGroupNum();
}
//**************************************************************************
//Method Name   : IsMasterSlaveProtocol
//Author        : PATNI/AKR
//Purpose       : Wrapper function of CMasterSlave::IsMasterSlaveProtocol()
//**************************************************************************
int CPqm::IsMasterSlaveProtocol(const CPqmProtocol* f_protocol) const
{
    return m_master_slave->IsMasterSlaveProtocol(f_protocol);
}

//********************************Method Header********************************
//Method Name   :IsWireLessSystem()
//Author        :
//Purpose       :
//*****************************************************************************
bool CPqm::IsWireLessSystem()const
{
    return (m_gate_mode == 1);
}
//***************************Method Header*************************************/
//Method Name    : CheckAndGetWaitingForAutoScanStartAcqOrder()
//Author         : Patni/Nilesh
//Purpose        :
//*****************************************************************************/
void CPqm::CheckAndGetWaitingForAutoScanStartAcqOrder(int* f_waiting_for_auto_scan_acq_order)
{
    LPCTSTR FUNC_NAME = _T("CPqm::CheckAndGetWaitingForAutoScanStartAcqOrder");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_waiting_prot_acq_order = m_acq_manager->GetWaitingForAutoScanStartAcqOrder();

    if (-1 != l_waiting_prot_acq_order) {
        CPqmProtocol* l_top_wait_protocol = m_pqm_study->GetTopWaitProtocol();

        if (l_top_wait_protocol && (l_top_wait_protocol->GetProtocol() != l_waiting_prot_acq_order)) {
            if (!m_acq_manager->GetScanProcedure()->GetAPCRetry()) {
                m_acq_manager->AutoScanTimerReset();
                l_waiting_prot_acq_order = -1;
            }
        }
    }

    *f_waiting_for_auto_scan_acq_order = l_waiting_prot_acq_order;

}
/***************************Method Header*************************************/
//Method Name    : ChangeSlaveToNormalProtocol()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPqm::ChangeSlaveToNormalProtocol(const int f_slave_index)
{
    m_master_slave->ChangeToNormalProtocol(m_pqm_study->GetProtocolForIndex(f_slave_index), false);
}
/***************************Method Header*************************************/
//Method Name    : PerformAutoCopyForMasterSlave()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPqm::PerformAutoCopyForMasterSlave(VARIANT* const f_pdata)
{
    m_master_slave->PerformAutoCopyForMasterSlave(f_pdata);
}
//****************************Method Header************************************
//Method Name   :CheckScanEditForInscan()
//Author        :PATNI/Sribanta
//Purpose       :Check Plan status for Inscan sequence and display error.
//*****************************************************************************
bool CPqm::CheckScanEditForInscan()const
{

    LPCTSTR FUNC_NAME = _T("CPqm::CheckScanEditForInscan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckScanEditForInscan"));

    CScanProcedure* l_scanprocedure = m_acq_manager->GetScanProcedure();

    if (NULL == l_scanprocedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scanprocedure is NULL"));
        return false;
    }

    CPqmProtocol* l_current_protocol = l_scanprocedure->GetScanProcProtocol();

    if (m_pqm_study->IsProtocolInScan(l_current_protocol->GetProtocol())) {
        if (E_ERROR == m_pqm_study->CheckPlnStatus(l_current_protocol->GetProtocol())) {
            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_SCAN_EDIT_NOT_DONE, _T("IDS_ERR_SCAN_EDIT_NOT_DONE"), SCAN_ERR_NOTIFY, L"PqmPmAcqman.cpp");
            m_pqm_study->SetScanModeOfHead();
            l_scanprocedure->ResetCurrentRequest();
            WriteToUI(PQM_MSG_ABORT_WAIT);
            return false;
        }

        if (!m_pqm_study->IsEditedInAcquirePage(l_current_protocol->GetProtocol())) {
            CString l_error = CPqmUtilities::GetMultiLingualString(_T("IDS_INSCAN_PROTOCOL_NOTEDITED_IN_ACQUIRE"));
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                l_error, MPlus::EH::Information, APP_NAME, APP_NAME);
            m_pqm_study->SetScanModeOfHead();
            l_scanprocedure->ResetCurrentRequest();
            WriteToUI(PQM_MSG_ABORT_WAIT);
            return false;
        }
    }

    return true;
}
//**************************************************************************
//Method Name   :  GetProtocolInfoForAutoCopy()
//Author        :  PATNI/Varun
//Purpose       :  Get the Master's slaves protocol indexes and count
//**************************************************************************
void CPqm::GetProtocolInfoForAutoCopy(VARIANT* f_pdata)
{
    int l_master_protocol_index = f_pdata->intVal;

    if (m_master_slave && (-1 == l_master_protocol_index)) {

        l_master_protocol_index = m_master_slave->GetProtocolIndexToUpdate();
    }

    CPqmProtocol* l_master_protocol = m_pqm_study->GetProtocolForIndex(l_master_protocol_index);
    int l_slave_count = 0;
    int* l_slave_protocols_index = NULL;

    if (m_master_slave && l_master_protocol) {
        l_slave_count = m_master_slave->GetSlavesProtocolIndex(l_master_protocol->GetGroupID(), &l_slave_protocols_index);
    }

    f_pdata->intVal = l_slave_count;
    f_pdata->byref = (void*)l_slave_protocols_index;
}

//**************************************************************************
//Method Name   :  GetEditingProtocolGroupID()
//Author        :  PATNI/Amit
//Purpose       :  Get the current editing protocol's group id.
//**************************************************************************
int CPqm::GetEditingProtocolGroupID()
{
    int l_group_id = 0;
    CPqmProtocol* l_editing_protocol = m_pqm_procon->GetCurrentProtocol();

    if (NULL != l_editing_protocol) {

        l_group_id = l_editing_protocol->GetGroupID();
    }

    return l_group_id;
}
//**************************************************************************
//Method Name   :  GetMasterPositionForSlave()
//Author        :  PATNI/AKR
//Purpose       :
//**************************************************************************
void CPqm::GetMasterPositionForSlave(const int f_slave_index, int* f_master_position)const
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetMasterPositionForSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPqmProtocol* l_slave_protocol = m_pqm_study->GetProtocolForIndex(f_slave_index);

    if (l_slave_protocol) {
        CPqmProtocol* l_master_protocol = l_slave_protocol->GetMasterProtocol();

        if (l_master_protocol) {
            *f_master_position = l_master_protocol->GetPosition();
        }
    }
}
/***************************Method Header*************************************/
//Method Name    : GetProtocolIndexToUpdate()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPqm::GetProtocolIndexToUpdate(int* const f_protocol_index)
{
    *f_protocol_index = m_master_slave->GetProtocolIndexToUpdate();
}
/***************************Method Header*************************************/
//Method Name    : GetMasterListForPlanMark()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPqm::GetMasterListForPlanMark(VARIANT* f_pdata)
{
    m_master_slave->GetMasterListForPlanMark(f_pdata);
}

//****************************Method Header************************************
//Method Name   :GetStudy()
//Author        :PATNI/JCM
//Purpose       :This method gets the system magnet type
//*****************************************************************************
CPQMStudy* const CPqm::GetStudy(
)const
{
    return m_pqm_study;
}

//********************************Method Header********************************
//Method Name   : GetAcqManager()
//Author        : PATNI / HAR
//Purpose       : Return the Acq manager pointer to calling function.
//*****************************************************************************
CPqmPmAcqman* const CPqm::GetAcqManager(
)const
{
    return m_acq_manager;
}

//+Patni-MN/2009Dec02/Added/MSA0248-00032/Get the pointer of CPASCOMInterface
//************************************Method Header************************************
// Method Name  : GetPASCOMInterface
// Author       : PATNI - Manishkumar
// Purpose      : Return the CPASCOMInterface object
//***********************************************************************************
CPASCOMInterface* CPqm::GetPASCOMInterface() const
{
    return m_pascom;
}
//-Patni-MN/2009Dec02/Added/MSA0248-00032/Get the pointer of CPASCOMInterface

//********************************Method Header********************************
//Method Name   :GetDataManager()
//Author        :PATNI/AMT
//Purpose       :returns an object od CDataManager class which use to access
//                  database information.
//*****************************************************************************
//+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
CDataManager* const CPqm::GetDataManager()
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    return m_data_mgr;
}

//**************************************************************************
//Method Name   : GetPascomInterface
//Author        : PATNI/DKH
//Purpose       : Gets the PascomInterface pointer
//**************************************************************************
CPASCOMInterface* CPqm::GetPascomInterface()
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetPascomInterface");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqm::GetPascomInterface"));

    if (NULL != m_pascom) {
        return m_pascom ;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
        return NULL;
    }
}
//-Patni-DKH/2010Jun24/Added/NFD001AutoMap

//********************************Method Header********************************
//Method Name   :GetStudyManagerPtr()
//Author        :PATNI/HAR
//Purpose       :Returns Study Manager Pointer
//****************************************************************************
CPQMStudyMgr* const CPqm::GetStudyManagerPtr(
) const
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    if (m_dbsa_study_mgr != NULL) {
        return m_dbsa_study_mgr;

    } else {
        return NULL;
    }
}

CPqmSpeeder* CPqm::GetSpeederPtr()
{
    return m_pqmSpeeder;
}

//**************************************************************************
//Method Name   : GetMasterSlavePtr
//Author        : PATNI/AKR
//Purpose       : Get the master slave class pointer
//**************************************************************************
CMasterSlave* CPqm::GetMasterSlavePtr() const
{
    return m_master_slave;
}
//********************************Method Header********************************
//Method Name   : GetSARManager()
//Author        : PATNI / NILESH
//Purpose       : Return the SAR manager pointer to calling function.
//*****************************************************************************
CPqmPmSARManager* const CPqm::GetSARManager()
{
    return m_sar_manager;
}

//********************************Method Header********************************
//Method Name   :GetVAPChannels()
//Author        :PATNI/Mangesh
//Purpose       :Returns VAPChannels
//*****************************************************************************
int CPqm::GetVAPChannels(
) const
{
    return m_VAP_Channels;
}

//********************************Method Header********************************
//Method Name   :GetNumChannels()
//Author        :PATNI/AMT
//Purpose       :return channer numbers.
//*****************************************************************************
int CPqm::GetNumChannels(
) const
{
    return m_num_Channels;
}




//********************************Method Header********************************
//Method Name   :GetChannelPerVFEP()
//Author        :PATNI/Mangesh
//Purpose       :Returns ChannelPerVFEP
//*****************************************************************************
int CPqm::GetChannelPerVFEP(
) const
{
    return m_channel_Per_VFEP;
}

//********************************Method Header********************************
//Method Name   :GetVAP()
//Author        :PATNI/Mangesh
//Purpose       :Returns VAP
//*****************************************************************************
bool_t CPqm::GetVAP(
) const
{
    return m_vap;
}

//********************************Method Header********************************
//Method Name   :GetVAP()
//Author        :PATNI/Mangesh
//Purpose       :Returns VAP
//*****************************************************************************
float CPqm::GetVFEPProcRatio(
) const
{
    return m_vfep_Proc_Ratio;
}

CPqmProcon*	 const CPqm::GetPqmProCon(
)const
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    return	m_pqm_procon;
}

//+Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   : GetIRSScoketFD
//Author        : PATNI\DKH
//Purpose       : To get the IRS listener socket FD
//**************************************************************************
int CPqm::GetIRSScoketFD() const
{
    if (NULL != m_irs_listner) {
        return m_irs_listner->GetIRSSocketFD();
    }

    return -1;
}

//****************************Method Header************************************
//Method Name   : GetProcessing()
//Author        : PATNI/ MRP
//Purpose       : To get the Processing flag
//*****************************************************************************
int  CPqm::GetProcessing(
) const
{
    return  m_processing_ui;
}

//****************************Method Header************************************
//Method Name   : GetPostProcessFlag()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetPostProcessFlag(
) const
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetPostProcessFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, m_post_process_flag ? _T("TRUE") : _T("FALSE"));
    return m_post_process_flag;
}
//-Patni-HAR/2010Mar08/Added/MSA0251-00275

//****************************Method Header************************************
//Method Name   : GetCouchConfigPtr()
//Author        : PATNI/NJK
//Purpose       : This method returns the counconfig pointer for the CCouchConfig class
//*****************************************************************************
CCouchConfig* const CPqm::GetCouchConfigPtr(
) const
{
    return m_couchconfig;
}

//********************************Method Header********************************
//Method Name   :GetHiSpeedCouch()
//Author        :PATNI/MSN
//Purpose       :Returns the locator mode that is set using the PQM GUI
//				 Utility menu -> Moving to locator img couch position
//*****************************************************************************
bool CPqm::GetHiSpeedCouch(
) const
{
    return m_hi_speed_couch;
}

//Patni-SS/ADDED/2009-May-27/JFT73
int CPqm::GetNoOfSequence() const
{
    return m_no_of_sequence;
}

//**************************************************************************
//Method Name   : GetAcqmanSocketID
//Author        : PATNI\DKH
//Purpose       : To get Acq manager socket ID
//**************************************************************************
UINT CPqm::GetAcqmanSocketID() const
{
    if (NULL != m_acq_manager) {
        return m_acq_manager->GetPQMSocketID();
    }

    return 0;
}
//-Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow

//********************************Method Header********************************
//Method Name   :GetCouchpac()
//Author        :PATNI/DKH
//Purpose       :Returns the m_couch_pac
//****************************************************************************
int CPqm::GetCouchpac(
) const
{
    return m_couch_pac;
}

//+Patni-ARD/201APR14/Added/PH#3#/IR86-IR-100/DebugWindow# max offset
//****************************Method Header************************************
//Method Name   : GetXOffsetLimit()
//Author        : PATNI\ARD
//Purpose       : return x offset
//*****************************************************************************
int CPqm::GetXOffsetLimit() const
{
    return m_xOffset;
}

//****************************Method Header************************************
//Method Name   : GetXOffsetLimit()
//Author        : PATNI\ARD
//Purpose       : return y offset
//*****************************************************************************
int CPqm::GetYOffsetLimit() const
{
    return m_yOffset;
}
//-Patni-ARD/201APR14/Added/PH#3#/IR86-IR-100/DebugWindow# max offset

/****************************Method Header************************************
//Method Name   :GetAnotherPqmStudy()
//Author        :PATNI/LK
//Purpose       :Get another study
//*****************************************************************************/
study_id_t CPqm::GetAnotherPqmStudy(
) const
{
    return m_anotherpqmstudy;
}

//+ MEITEC/2010Apr23/Added/IR86
//**************************************************************************
//Method Name   :
//Author        : PATNI/RAJ
//Purpose       :
//**************************************************************************
int CPqm::GetChannel_mask(
) const
{
    return m_channel_mask;
}
//- MEITEC/2010Apr23/Added/IR86
//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetCDS
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CpqmCDS* CPqm::GetCDS(
)
{
    return &m_pqmcds ;
}
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement

CpqmAMB* CPqm::GetAMB(
)
{
    return m_pamb ;
}

//********************************Method Header********************************
//Method Name   :GetSAR()
//Author        :PATNI/MRP
//Purpose       :Returns the CPQMSAR pointer
//****************************************************************************
CPQMSAR* const CPqm::GetSAR(
) const
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    return m_pqmSAR;
}

//********************************Method Header********************************
//Method Name   :GetIRSListner()
//Author        :PATNI/HAR
//Purpose       :Returns IRSListner pointer
//****************************************************************************
CIRSListner* const CPqm::GetIRSListner(
) const
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    if (m_irs_listner != NULL) {
        return m_irs_listner;

    } else {
        return NULL;
    }
}
//********************************Method Header********************************
//Method Name   :DeleteIRSListnerInstance()
//Author        :PATNI/HAR
//Purpose       :Delete IRSListner pointer
//****************************************************************************
void CPqm::DeleteIRSListnerInstance(
)
{
    DEL_PTR(m_irs_listner);
}

//************************************Method Header************************************
// Method Name  :GetWFDAImplementer
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
CWFDAImplementer* CPqm::GetWFDAImplementer(
)
{
    return m_wfdaimplementer ;
}
*/
//-Patni-PJS/2010Feb2/Modified/IR-97

//********************************Method Header********************************
//Method Name   :GetCoilSelectionTool()
//Author        :PATNI/AMT
//Purpose       :returns an object od CCoilSelection class which use to access
//                  coil selection functionalities.
//*****************************************************************************
//+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
CCoilSelectionTool* const CPqm::GetCoilSelectionTool()const
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    return m_coil_selection_tool;
}

//********************************Method Header********************************
//Method Name   :GetdBdt()
//Author        :PATNI/MSN
//Purpose       :Returns the CPQMSAR pointer
//****************************************************************************
CPQMdBdt* CPqm::GetdBdt(
)
{
    return m_pqmdBdt;
}

//********************************Method Header********************************
//Method Name   :GetAvInterval()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetAvInterval(
)const
{
    return m_av_interval;
}

//********************************Method Header********************************
//Method Name   :GetRisDictionary()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
CString CPqm::GetRisDictionary(
)const
{
    return m_ris_dictionary;
}

//********************************Method Header********************************
//Method Name   :GetRisName()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
CString CPqm::GetRisName(
)const
{
    return m_ris_name;
}

//**************************************************************************
//Method Name   : GetPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : To get plan duplicate settings
//**************************************************************************
CPlanDuplicateSettings* CPqm::GetPlanDuplicateSettings() const
{
    return m_planduplicateSettings;
}
//Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator

//****************************Method Header************************************
//Method Name   : GetColorInfo()
//Author        : PATNI/MSN
//Purpose       : This method gets the system magnet type
//*****************************************************************************
CColorSetting* const CPqm::GetColorInfo(
)
{
    return m_colorinfo;
}

//********************************Method Header********************************
//Method Name   : GetShowCoilInfo()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
VftCoilListInfo_t* const CPqm::GetShowCoilInfo()const
{
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    return m_show_coil;
}

VftCoilCoordinateListInfo_t* const CPqm::GetCoordinateList(
) const
{

    return m_CoilListcoordinate;
}

//********************************Method Header********************************
//Method Name   : GetSelectCoil()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
coilSelect_t CPqm::GetSelectCoil() const
{
    return m_select_coil;
}

//********************************Method Header********************************
//Method Name   :GetInsideLimit()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
void CPqm::GetInsideLimit(int f_index, int& f_in_side_limit, int& f_out_side_limit, BOOL& enable)
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetInsideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_study) {
        CPqmProtocol* l_protocol = m_pqm_study->GetProtocol(f_index);

        if (l_protocol) {
            CPqmPrMode* pqm_pr_mode = l_protocol->GetPqmPrMode();

            if (pqm_pr_mode) {
                pqm_pr_mode->SetRecvCoilId(m_selected_coil_key);

                CPqmCoilData* l_coil_data = m_data_mgr->GetPQMCoilData();

                if (l_coil_data) {
                    CCoil l_coil;
                    l_coil_data->PqmCoilGetCoil(pqm_pr_mode->GetRecvCoilId(), l_coil);

                    if (IsSaturnCoilSystem()) {	/* Saturn Coil */
                        //+Patni-PJS/2010Sep16/Added/MVC008989
                        /*enable = (l_coil_data->GetInOutSideLimit(l_protocol->GetCoilDBTable(),
                                  &f_in_side_limit, &f_out_side_limit) == E_NO_ERROR
                                  &&  f_in_side_limit + f_in_side_limit > 0);*/
                        enable = (l_coil_data->GetInOutSideLimit(l_protocol->GetCoilDBTable(),
                                  &f_in_side_limit, &f_out_side_limit) == E_NO_ERROR
                                  &&  f_in_side_limit + f_out_side_limit > 0);
                        //-Patni-PJS/2010Sep16/Added/MVC008989

                    } else {
                        l_coil_data->PqmCoilGetCoil(pqm_pr_mode->GetRecvCoilId(), l_coil);
                        f_in_side_limit  = l_coil.GetInsideLimit();
                        f_out_side_limit = l_coil.GetOutsideLimit();
                        enable = (f_in_side_limit + f_out_side_limit > 0);
                    }

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
                }

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqm_pr_mode pointer is NULL"));
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_acq_manager pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   : GetConnectedCoilList()
//Author        : PATNI/AMT
//Purpose       : Returns connected coil information to GUI through IPC
//*****************************************************************************
ConnectedCoilList* CPqm::GetConnectedCoilList(
)
{
    return &m_connect_coil_list;
}

//********************************Method Header********************************
//Method Name   : GetGateInfoList()
//Author        : PATNI/MRP
//Purpose       : Returns the flag to display the Coil Info dialog
//*****************************************************************************
CStringArray* const CPqm::GetGateInfoList(
)const
{
    return m_gate_list;
}

CString CPqm::GetCurrentSarInfo() const
{
    return m_current_sarInfo;
}

//+Patni-ARD/2009July21/Added/JFT# 177
//********************************Method Header********************************
//Method Name   : GetHasParent()
//Author        : PATNI/ARD
//*****************************************************************************
int CPqm::GetHasParent(const int f_acq_order) const
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetHasParent");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_study) {
        CPqmProtocol* l_pqm_prot = m_pqm_study->GetProtocol(f_acq_order);

        if (l_pqm_prot != NULL) {
            return l_pqm_prot->GetHasParent();

        } else {
            return FALSE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}
//-Patni-ARD/2009July21/Added/JFT# 177

//+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
//********************************Method Header********************************
//Method Name   : GetSarInfoPtr()
//Author        : PATNI/PJS

//*****************************************************************************
sarInfo_t* CPqm::GetSarInfoPtr()
{
    //+TMSC-Tanoue/2010Sep03/MVC008324
    LPCTSTR FUNC_NAME = _T("CPqm::GetSarInfoPtr");

    if (!m_pqm_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study is NULL"));
        return NULL;
    }

    CPqmProtocol* l_protocol = m_pqm_study->GetItemFromPosition(m_protocol_index + 1);

    if (!l_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol is NULL"));
        return NULL;
    }

    CPQMSAR* pqm_sar = GetSAR();

    if (!pqm_sar) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqm_sar is NULL"));
        return NULL;
    }

    pqm_sar->GetSARValue(l_protocol);
    m_ptr_sarInfo = l_protocol->GetSarInfo();
    return m_ptr_sarInfo;
    //-TMSC-Tanoue/2010Sep03/MVC008324
}



//********************************Method Header********************************
//Method Name   : GetSarInfoPtrForIndex()
//Author        : iGATE

//*****************************************************************************
sarInfo_t* CPqm::GetSarInfoPtrForIndex(const int f_seq_index) const
{
    CPqmProtocol* l_protocol = m_pqm_study->GetProtocolForIndex(f_seq_index);

    if (l_protocol) {
        return l_protocol->GetSarInfo();
    }

    return NULL;
}


//********************************Method Header********************************
//Method Name   :GetNoOfSequencesTransferred()
//Author        :PATNI/MRP
//Purpose       :Returns the number of sequences tranffered
//****************************************************************************
int CPqm::GetNoOfSequencesTransferred(
) const
{
    return m_no_of_sequence_transferred;

}

//+Patni-HEMANT/ADDED On 8/11/2009 2:11:50 PM/ MVC2710
//************************************Method Header************************************
// Method Name  : GetSFTMode
// Author       : PATNI/ HEMANT
// Purpose      : returns the sft mode value
//***********************************************************************************
int CPqm::GetSFTMode(
) const
{
    return m_sft_mode;
}
//-Patni-HEMANT/ADDED On 8/11/2009 2:11:50 PM/ MVC2710

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
void CPqm::GetSFTParamValue(
    VARIANT** f_param,
    const int f_acq_order
)
{

    //$SD : <Sequence Directory>
    //$SF : -study <Study Filename>
    //$sf : <Study Filename>
    //$SP : -scratchPad <Scratchpad>
    //$sp : <Scratchpad>

    if (f_param && *f_param) {

        if (((*f_param)->byref)) {

            CString* l_param = ((CString*)((*f_param)->byref));
            CString l_copy_param = *l_param;

            if (l_copy_param.Compare(g_SD) == 0) {
                (*l_param) = m_pqm_study->GetSequnceDirectory(f_acq_order);

            } else if (l_copy_param.Compare(g_SF) == 0) {

                CString f_study_file_name = m_pqm_study->GetStudyPath() ;

                //Patni-Hemant/08Sep2010/Modified/IR-152 Reopen
                (*l_param) = g_study_str + _T(" ") + f_study_file_name;

            } else if (l_copy_param.Compare(g_sf) == 0) {

                CString f_study_file_name = m_pqm_study->GetStudyPath() ;
                (*l_param) = f_study_file_name;

            } else if (l_copy_param.Compare(g_SP) == 0) {

                //+Patni-Hemant/08Sep2010/Modified/IR-152 Reopen
                CString l_scratch_pad(_T(""));
                m_pqm_study->CreateScratchPadForSFTTool(f_acq_order, &l_scratch_pad, OTHER) ;
                (*l_param) = g_scratchPad_str + _T(" ")  + l_scratch_pad;
                //-Patni-Hemant/08Sep2010/Modified/IR-152 Reopen

            } else if (l_copy_param.Compare(g_sp) == 0) {

                //+Patni-Hemant/08Sep2010/Modified/IR-152 Reopen
                CString l_scratch_pad(_T(""));
                m_pqm_study->CreateScratchPadForSFTTool(f_acq_order, &l_scratch_pad, OTHER);

                (*l_param) = l_scratch_pad;
                //-Patni-Hemant/08Sep2010/Modified/IR-152 Reopen
            }
        }
    }
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetMagneticCenterPosForCoilSel
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqm::GetMagneticCenterPosForCoilSel(
    VARIANT**  f_center_pos
)const  //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    if (f_center_pos && (*f_center_pos)) {

        const float l_center = m_pqmcds.GetMagneticCenterPosForCoilSel();
        (*f_center_pos)->fltVal = l_center ;

        return ;
    }

    ASSERT(FALSE);
}

//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CPqm::GetCDSDebugParam(
    VARIANT**  f_compass_debug_param
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_18
{
    if (f_compass_debug_param && *f_compass_debug_param) {

        COMPASS_Debugg_Param* l_compass_debug_param =
            (COMPASS_Debugg_Param*)(*f_compass_debug_param)->byref ;

        m_pqmcds.GetCDSDebugParam(l_compass_debug_param);

        return ;
    }

    ASSERT(FALSE) ;
}

//************************************Method Header************************************
// Method Name  : GetASCompassFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPqm::GetASCompassFlag(
    const int f_acqorder,
    BOOL* f_compass_flag
) const  //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_18
{
    if (m_pqm_study) {

        return m_pqm_study->GetASCompassFlag(f_acqorder, f_compass_flag);
    }

    return FALSE ;
}
//-Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement


//**************************************************************************
//Method Name   : GetAutoMapMgr
//Author        : PATNI/AKR/NFD001-AutoMap
//Purpose       : Gets the AutoMapMgr class pointer
//**************************************************************************
CAutoInsertProcedure* CPqm::GetAutoInsertProc()
{
    return &m_autoinsert_proc;
}

//********************************Method Header********************************
//Method Name   :GetAutoView()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
int CPqm::GetAutoView(
) const
{
    return m_pqm_autoview;
}

//************************************Method Header************************************
// Method Name  : GetProtocolForIndex
// Author       : PATNI/ HEMANT
// Purpose      : returns the CPqmProtocol pointer stored at zero based index = f_index
//                If the f_index is not valid, the function should reutrn NULL.
//***********************************************************************************
CPqmProtocol* CPqm::GetProtocolForIndex(
    const int f_index
)
{
    //Note : f_index should be zero based index

    if (m_pqm_study) {

        return m_pqm_study->GetProtocolForIndex(f_index);
    }

    return NULL ;
}

//****************************Method Header************************************
//Method Name   : GetCouchLimitAPC()
//Author        : PATNI/HAR
//Purpose       : Get the couch limit of APC
//*****************************************************************************
float CPqm::GetCouchLimitAPC(
) const
{
    return m_couch_limit_apc;
}

//********************************Method Header********************************
//Method Name   :GetDefaultRGN()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
float CPqm::GetDefaultRGN(
)const
{
    return m_default_rgn;
}

//********************************Method Header********************************
//Method Name   :GetDRKSreconMode()
//Author        :PATNI/MRP
//Purpose       :To get the value for DRKS Recon Mode
//*****************************************************************************
bool CPqm::GetDRKSreconMode(
) const
{
    return m_drks_recon_mode;
}

//********************************Method Header********************************
//Method Name   :GetNRFilter_License()
//Author        :PATNI/NP
//Purpose       :Returns the Noise Reduction license
//*****************************************************************************
//+ Patni-NP/2009Aug17/Added/MVC003302
bool_t CPqm::GetNRFilter_License()const
{
    //Patni-SS/Added/20090828/IR#91
    return (bool_t)m_NoiseReduction_license;
}


//********************************Method Header********************************
//Method Name   :GetEMTONE_License()
//Author        :PATNI
//Purpose       :Returns the EMTONE license
//*****************************************************************************
bool_t CPqm::GetEMTONE_License()const
{

    return m_EMTONE_license;
}

//************************************Method Header************************************
// Method Name  : GetOffsetVecReset
// Author       : PATNI\ARD
// Purpose      : Get OffsetVecReset value
//***********************************************************************************
bool CPqm::GetOffsetVecReset() const
{
    return m_offset_vector_reset;
}
//-Patni-ARD/2010Jan6/Added/DeFT# IR 100

//****************************Method Header************************************
//Method Name   : GetSysType()
//Author        : PATNI/HAR
//Purpose       : Get the Sys Type
//*****************************************************************************
int CPqm::GetSysType(
) const
{
    return m_systype;
}

//********************************Method Header********************************
//Method Name   :GetAvSpdrMapDelay()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CPqm::GetAvSpdrMapDelay(
)const
{
    return m_av_spdr_map_delay;
}

//+Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
//**************************************************************************
//Method Name   : GetSysName
//Author        : PATNI\ARD
//Purpose       : return the system name
//**************************************************************************
CString CPqm::GetSysName() const
{
    return m_sys_name;
}
//-Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini

//+Patni-ARD/2010Mar22/Modified/Ph-3# IR-100
//****************************Method Header************************************
//Method Name   : GetAcqmanIP()
//Author        : PATNI\ARD
//Purpose       : return acqman ip address
//*****************************************************************************
CString CPqm::GetAcqmanIP() const
{
    return m_acqman_ip;
}
//-Patni-ARD/2010Mar22/Modified/Ph-3# IR-100

//****************************Method Header************************************
//Method Name   :GetMrsLicense()
//Author        :PATNI / SS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetMrsLicense(
) const
{
    return m_mrs_license;
}


BOOL CPqm::GetMRS_BRST_License(
)const
{
    return m_MRS_BRST_license;
}

//****************************Method Header************************************
//Method Name   :GetButtonState()
//Author        :PATNI/MRP
//Purpose       :Dummy method to get the state of a button from GUI
//*****************************************************************************
bool CPqm::GetButtonState(
    const int button
)const
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(button);
    return false;
}

//********************************Method Header********************************
//Method Name   :GetRxGainOffset()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
float CPqm::GetRxGainOffset(
)const
{
    return m_rx_gain_offset;
}
//****************************Method Header************************************
//Module         : GetSendAcquireSeqList
//Author         : PATNI/Sribanta
//Purpose        : Get Acquire Sequence List
//*****************************************************************************
SequenceList& CPqm::GetSendAcquireSeqList()
{
    return m_snd_acquire_seq_list;
}
//****************************Method Header************************************
//Module         : GetSendScheduleSeqList
//Author         : PATNI/Sribanta
//Purpose        : Get Schedule Sequence List
//*****************************************************************************

SequenceList& CPqm::GetSendScheduleSeqList()
{
    return m_snd_schedule_seq_list;
}

//************************************Method Header************************************
// Method Name  :GetDbsaStudyMgr
// Author       : PATNI/Sriabnta
// Purpose      :
//***********************************************************************************
CPQMStudyMgr* CPqm::GetDbsaStudyMgr(
)const
{
    return m_dbsa_study_mgr;
}


//**************************************************************************
//Method Name   : CanDisplayCFAGraph
//Author        : PATNI/AM/NFD001
//Purpose       : Get the CFAdisplay flag
//**************************************************************************
bool CPqm::CanDisplayCFAGraph(
    CPqmProtocol* const f_protocol
) const
{
    return m_autoinsert_proc.CanDisplayCFAGraph(f_protocol);
}

void CPqm::ProtocolEditingCompleted(
    CPqmProtocol* const f_protocol
) const
{

    m_autoinsert_proc.ProtocolEditingCompleted(f_protocol);
}


//****************************Method Header************************************
//Method Name   :GetStudyUid()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
CString CPqm::GetStudyUid(
) const
{
    return m_study_uid;
}

//****************************Method Header************************************
//Method Name   :SetStudyUid()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetStudyUid(
    const CString& f_study_uid
)
{
    m_study_uid = f_study_uid;
}

//****************************Method Header************************************
//Method Name   :GetStudyUidPath()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
CString CPqm::GetStudyUidPath(
) const
{
    return m_study_uid_path;
}

//****************************Method Header************************************
//Method Name   :SetStudyUidPath()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetStudyUidPath(
    const CString& f_study_uid_path
)
{
    //+Patni-AMT/2010Mar10/Modified/Ph-3#IR-100-StudyUID
    /*CString l_study_path = _T("");
    l_study_path = CPQMConfig::GetInstance()->GetStudyPath();

    if (l_study_path.GetLength()) {
        m_study_uid_path = l_study_path + f_study_uid_path;
    }*/
    m_study_uid_path = f_study_uid_path;
    //-Patni-AMT/2010Mar10/Modified/Ph-3#IR-100-StudyUID
}

//****************************Method Header************************************
//Method Name   :GetManagerPtr()
//Author        :PATNI/MRP
//Purpose       :
//*****************************************************************************
CPQMIPCManager* const CPqm::GetManagerPtr(
)
{
    return m_pqmipcmanager;
}

//****************************Method Header************************************
//Method Name   :SetManagerPtr()
//Author        :PATNI/MRP
//Purpose       :
//*****************************************************************************
void CPqm::SetManagerPtr(
    CPQMIPCManager* manager
)
{
    m_pqmipcmanager = manager;
}

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CPqm::GetASCOMPASSParameter(
    VARIANT**  f_cdscompasssetting_param
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_18
{

    if (f_cdscompasssetting_param) {

        CDSCompassSettingParams* l_cdscompasssetting_param =
            (CDSCompassSettingParams*)(*f_cdscompasssetting_param)->byref ;

        if (l_cdscompasssetting_param) {

            m_pqmcds.GetASCOMPASSParameter(l_cdscompasssetting_param);
            return ;
        }
    }

    ASSERT(FALSE) ;
}

//************************************Method Header************************************
// Method Name  : SetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CPqm::SetASCOMPASSParameter(
    VARIANT**  f_cdscompasssetting_param
)
{
    if (f_cdscompasssetting_param && (*f_cdscompasssetting_param)) {

        CDSCompassSettingParams* l_cdscompasssetting_param =
            (CDSCompassSettingParams*)(*f_cdscompasssetting_param)->byref ;

        if (l_cdscompasssetting_param) {

            m_pqmcds.SetASCOMPASSParameter(l_cdscompasssetting_param);
            return ;
        }
    }

    ASSERT(FALSE) ;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   : GetMode()
//Author        : PATNI/HAR
//Purpose       : This method give the value of the mode
//*****************************************************************************
int CPqm::GetMode(
) const
{
    return m_mode;
}

//****************************Method Header************************************
//Method Name   : SetMode()
//Author        : PATNI/HAR
//Purpose       : This method set the value of the mode
//*****************************************************************************
void CPqm::SetMode(
    const int mode
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetMode");
    m_mode = mode;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->SetMode(m_mode);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   :GetAutoPlan()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetAutoPlan(
)const
{
    return m_auto_plan;

}

//********************************Method Header********************************
//Method Name   :SetAutoPlan()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetAutoPlan(
    const int f_auto_plan
)
{
    m_auto_plan = f_auto_plan;
}

//********************************Method Header********************************
//Method Name   :GetPlanAndScan()
//Author        :PATNI/
//Purpose       :
//****************************************************************************
int CPqm::GetPlanAndScan(
) const
{
    return m_plan_scan;
}

//********************************Method Header********************************
//Method Name   :SetPlanAndScan()
//Author        :PATNI/
//Purpose       :
//****************************************************************************
void CPqm::SetPlanAndScan(
    const int f_plan_scan
)
{
    m_plan_scan = f_plan_scan;
}

//********************************Method Header********************************
//Method Name   :GetIsProconConnected()
//Author        :PATNI/ASHISH SM3
//Purpose       :
//*****************************************************************************
bool CPqm::GetIsProconConnected(
)const
{
    return m_is_procon_connected;
}

//********************************Method Header********************************
//Method Name   :SetIsProconConnected()
//Author        :PATNI/ASHISH SM3
//Purpose       :
//*****************************************************************************
void CPqm::SetIsProconConnected(
    const bool f_bconnection_status
)
{
    m_is_procon_connected = f_bconnection_status;
}

//************************************Method Header************************************
// Method Name  : GetCouchPos
// Author       : PATNI/ PJS
// Purpose      : Gets the Couch Position
//***********************************************************************************
float CPqm::GetCouchPos() const
{
    return m_couch_pos;
}

//************************************Method Header************************************
// Method Name  : SetCouchPos
// Author       : PATNI/ PJS
// Purpose      : Sets the Couch Position
//***********************************************************************************
void CPqm::SetCouchPos(const float f_couch_val)
{
    m_couch_pos = f_couch_val;
}

//********************************Method Header********************************
//Method Name   :GetShimmWBCoil()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetShimmWBCoil(
)
{
    return m_shimm_wb_coil;
}

//********************************Method Header********************************
//Method Name   :SetShimmWBCoil()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetShimmWBCoil(
    const BOOL shimming_wb_coil
)
{
    m_shimm_wb_coil = shimming_wb_coil;
}



//********************************Method Header********************************
//Method Name   :GetPrepWbCoil()
//Author        :PATNI/Ashish SM3
//Purpose       :
//****************************************************************************
int CPqm::GetPrepWbCoil(
)const
{
    return m_prep_wb_coil;
}

//********************************Method Header********************************
//Method Name   :SetPrepWbCoil()
//Author        :PATNI/Ashish SM3
//Purpose       :
//*****************************************************************************
void CPqm::SetPrepWbCoil(
    const BOOL f_prep_scan_use_wb
)
{
    m_prep_wb_coil = f_prep_scan_use_wb;
}

bool CPqm::GetAllCoilSAR(
) const
{
    return m_setAllCoilSAR;
}
void CPqm::SetAllCoilSAR(
    const bool f_setallSAR)
{
    m_setAllCoilSAR = f_setallSAR;
}

//********************************Method Header********************************
//Method Name   :GetSystemType()
//Author        :PATNI/MSN
//Purpose       :Returns the System type
//*****************************************************************************
char* CPqm::GetSystemType()
{
    return m_systemtype;
}

//********************************Method Header********************************
//Method Name   :SetSystemType()
//Author        :PATNI/MSN
//Purpose       :Sets the System type
//*****************************************************************************
void CPqm::SetSystemType(char* f_systemtype)
{
    m_systemtype = f_systemtype;
}

//********************************Method Header********************************
//Method Name   :GetGDCMode()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
gdcMode_t CPqm::GetGDCMode() const
{
    return m_gdcmode;
}

//********************************Method Header********************************
//Method Name   :SetGDCMode()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
void CPqm::SetGDCMode(
    const gdcMode_t&	f_gdcmode
)
{
    // + Himanshu OnSite
    memcpy(&m_gdcmode, &f_gdcmode, sizeof(gdcMode_t));
    //m_gdcmode = f_gdcmode;
    //  Himanshu OnSite
}

//********************************Method Header********************************
//Method Name   :GetHarmonizationMode()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
harmonizationMode_t	CPqm::GetHarmonizationMode()
{
    return m_harmonizationmode;
}

//********************************Method Header********************************
//Method Name   :SetHarmonizationMode()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
void CPqm::SetHarmonizationMode(
    const harmonizationMode_t&	f_mode
)
{
    // + Himanshu OnSite
    memcpy(&m_harmonizationmode, &f_mode, sizeof(harmonizationMode_t));
    //m_harmonizationmode = f_mode;
    // - Himanshu OnSite
}

//********************************Method Header********************************
//Method Name   :GetAvSpdrMap()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetAvSpdrMap(
)const
{
    return m_av_spdr_map;
}

//********************************Method Header********************************
//Method Name   :SetAvSpdrMap()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetAvSpdrMap(
    const BOOL av_spdr_map
)
{
    m_av_spdr_map = av_spdr_map;
}

//********************************Method Header********************************
//Method Name   :GetAllCoverageRgn()
//Author        :PATNI/
//Purpose       :
//****************************************************************************
bool CPqm::GetAllCoverageRgn(
) const
{
    return m_all_coverage_rgn;
}

//********************************Method Header********************************
//Method Name   :SetAllCoverageRgn()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetAllCoverageRgn(
    const bool f_all_rgn
)
{
    m_all_coverage_rgn = f_all_rgn;
}

//****************************Method Header************************************
//Method Name   : GetStartAutoScan()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
int CPqm::GetFirstWaitProtForAutoScan(
) const
{
    return m_autoprot_acq_order;
}

//****************************Method Header************************************
//Method Name   : SetStartAutoScan()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetFirstWaitProtForAutoScan(
    const int f_auto_scan_flag
)
{
    m_autoprot_acq_order = f_auto_scan_flag;
}

//********************************Method Header********************************
//Method Name   : GetGUICoilInformationDisplay()
//Author        : PATNI/MRP
//Purpose       : Returns the flag to display the Coil Info dialog
//*****************************************************************************
bool CPqm::GetGUICoilInformationDisplay(
) const
{
    return m_request_coil_info_display;
}

//********************************Method Header********************************
//Method Name   : SetGUICoilInformationDisplay()
//Author        : PATNI/MRP
//Purpose       : Sets the flag to display the Coil Info dialog
//*****************************************************************************
void CPqm::SetGUICoilInformationDisplay(
    const bool f_flag
)
{
    m_request_coil_info_display = f_flag;
}


//****************************Method Header************************************
//Method Name   :SetSeriesOrder()
//Author        :PATNI/Pavan
//Purpose       :Returns CFShiftCheck object
//*****************************************************************************
CCFShiftCheck* const CPqm::GetCFShiftCheck(
)const
{
    return m_cfshift_check;
}
//****************************Method Header************************************
//Method Name   :SetSeriesOrder()
//Author        :PATNI/Pavan
//Purpose       :Returns CCoilChannelMode object
//*****************************************************************************
CCoilChannelMode* const CPqm::GetCoilChannelMode() const
{
    return m_coilchannelmode;
}

//****************************Method Header************************************
//Method Name   :GetGateParamObj()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
CGateParamSetter* const CPqm::GetGateParamObj()
{
    return &m_gate_param_setter;
}

//****************************Method Header************************************
//Method Name   :GetGateParamObj()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
const CGateParamSetter* const CPqm::GetGateParamObj() const
{
    return &m_gate_param_setter;
}

//+Patni-HAR/2010Mar08/Added/MSA0251-00275
//****************************Method Header************************************
//Method Name   : SetPostProcessFlag()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetPostProcessFlag(
    const BOOL f_post_process_flag
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetPostProcessFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_post_process_flag ? _T("TRUE") : _T("FALSE"));
    m_post_process_flag = f_post_process_flag;
}


//*****************************************************************************
//Method Name   : SetUsername
//Author        : Patni/PJS
//Purpose       : This function is wrapper for SetUsername of
//                CPqmAuditLog class
//*****************************************************************************
void CPqm::SetUsername(
    const CString& f_usr
)
{
    m_pqm_audit_log.SetUsername(f_usr);
}

//+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
//********************************Method Header********************************
//Method Name   : SetCurrentSarInfo()
//Author        : PATNI/PJS

//*****************************************************************************
void CPqm::SetCurrentSarInfo(const CString& f_strsarinfo)
{
    m_current_sarInfo = f_strsarinfo;
}

/****************************Method Header************************************
//Method Name   :SetRefProbeNotAlive()
//Author        :PATNI/MSN
//Purpose       :Set if RefProbe is alive or not
//*****************************************************************************/
void CPqm::SetRefProbeNotAlive(
    const bool f_refprobe_alive_flag
)
{
    m_is_refprobe_alive = f_refprobe_alive_flag;
}

//********************************Method Header********************************
//Method Name   :SetWaitStatus()
//Author        :PATNI/DJ
//Purpose       :Change the Acquisitioon status to WAIT stat.
//*****************************************************************************
void CPqm::SetWaitStatus(
    CPASCOMSequence* pSeq
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetWaitStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPASParamStore* seq_param = pSeq->GetSequenceParam();
    seq_param->SetInt(_T("ACQSTATUS2"), 0);
    seq_param = NULL;
}

//********************************Method Header********************************
//Method Name   :SetAutoVoice()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetAutoVoice(
    const BOOL auto_voice
)
{
    m_auto_voice = auto_voice;
}

//********************************Method Header********************************
//Method Name   :SetSelectedCoilKey()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetSelectedCoilKey(
    const unsigned long f_selected_coil_key
)
{

    BITFLD_DECL(l_mask, K_ALL_PR) = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    BITFLD_INIT(l_mask, K_ALL_PR, 0);


    //    l_study = GetStudy();
    //    l_acq_manager->GetProtocol();
    m_selected_coil_key = f_selected_coil_key;
    //
    //    l_study->SetProtocolValues(l_acq_manager->GetProtocol()->GetProtocol(),
    //            l_mask,
    //            K_PR_RC_COIL,
    //            &f_selected_coil_key,
    //            sizeof(long),
    //            NULL);
}

//********************************Method Header********************************
//Method Name   :SetSkipRgn()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetSkipRgn(
    const bool f_skip_rgn
)
{
    m_skip_rgn = f_skip_rgn;
}

//+Patni-ARD/2010Jan6/Added/DeFT# IR 100
//************************************Method Header************************************
// Method Name  : SetConfigInfo
// Author       : PATNI\ARD
// Purpose      : Set the configurable information
//***********************************************************************************
void CPqm::SetConfigInfo(SIPCConfigInfo* f_sIPCConfiginfo)
{
    //+Patni-ARD/2010Mar24/Modified/DeFT# IR-100 code review
    m_av_interval       = f_sIPCConfiginfo->av_Interval;
    m_hi_speed_couch    = f_sIPCConfiginfo->hi_Speed_couch;

    //+Patni-Hemant/2010Jun7/Modified/MaFT/MVC008166
    //m_av_spdr_map       = f_sIPCConfiginfo->av_SPDR_Map;
    m_av_spdr_map       = f_sIPCConfiginfo->s_DefaultUtilityMenu.speedermap_auto_voice;
    //-Patni-Hemant/2010Jun7/Modified/MaFT/MVC008166

    m_av_spdr_map_delay = f_sIPCConfiginfo->av_SPDR_Map_Delay;

    //referenceimage
    m_offset_vector_reset = f_sIPCConfiginfo->offset_Vector_Reset;

    //+Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
    CPQMConfig::GetInstance()->SetJetChannel_V(f_sIPCConfiginfo->jet_channel_v);
    //if( NULL != m_dbsa_study_mgr )
    //	m_dbsa_study_mgr->SetSeriesBias( f_sIPCConfiginfo->series_bias );

    //-Patni-ARD/2010Mar24/Modified/DeFT# IR-100 code review

    //+Patni-ARD/2010Mar24/Added/Ph-3# Debug Info
    m_xOffset = f_sIPCConfiginfo->limit_offset_x;
    m_yOffset = f_sIPCConfiginfo->limit_offset_y;

    m_acqman_ip = f_sIPCConfiginfo->acqman_ip;
    //-Patni-ARD/2010Mar24/Added/Ph-3# Debug Info
    //-Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini

    //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    CPQMConfig::GetInstance()->LoadToolMenu(f_sIPCConfiginfo->m_tool_menu);

    m_drks_recon_mode = f_sIPCConfiginfo->recon_after_ref_view;

    m_pqmcds.SetCSConfigParams(f_sIPCConfiginfo->m_coilselconfigparams);
    f_sIPCConfiginfo->m_coilselconfigparams.m_couchLabelSize = m_pqmcds.GetCoilLableSize();

}

//+Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : SetCDSCheckCoilFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqm::SetCDSCheckCoilFlag(
    const int f_flag
)
{
    m_pqmcds.SetCDSCheckCoilFlag(f_flag);
}
//-Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement

//********************************Method Header********************************
//Method Name   :SetActiveDialog()
//Author        :PATNI/ASHISH SM3
//Purpose       :Set unset the member variable dedicated to the dialogs
//				 in PQM GUI on open & close of dialogs respectively.
//*****************************************************************************
void CPqm::SetActiveDialog(
    const int f_nDialogType, const BOOL f_bSet
)
{

    if (f_bSet) {

        switch (f_nDialogType) {

            case IS_SCANOFFSET_DIALOG:
                m_is_active_scanoffset_dialog = true;
                break;

            case IS_CFA_DIALOG:
                m_is_active_cfa_dialog = true;
                break;

            case IS_COIL_INFO_DIALOG:
                m_is_active_coilinfo_dialog = true;
                break;

            case IS_GATE_INFO_DIALOG:
                m_is_active_gateinfo_dialog = true;
                break;

            case IS_COIL_SELECTION_DIALOG:
                m_is_active_coilselection_dialog = true;
                break;

            case IS_AMB_DIALOG:
                m_is_active_amb_dialog = true;


            default:
                break;
        }

    } else {

        switch (f_nDialogType) {

            case IS_SCANOFFSET_DIALOG:
                m_is_active_scanoffset_dialog = false;
                break;

            case IS_CFA_DIALOG:
                m_is_active_cfa_dialog = false;
                break;

            case IS_COIL_INFO_DIALOG:
                m_is_active_coilinfo_dialog = false;
                break;

            case IS_GATE_INFO_DIALOG:
                m_is_active_gateinfo_dialog = false;
                break;

            case IS_COIL_SELECTION_DIALOG:
                m_is_active_coilselection_dialog = false;
                break;

            case IS_AMB_DIALOG:
                m_is_active_amb_dialog = false;
                break;

            default:
                break;
        }
    }
}

//********************************Method Header********************************
//Method Name   :SetMoveLocatorMode()
//Author        :PATNI/MSN
//Purpose       :Sets the locator mode that is set using the PQM GUI
//				 Utility menu -> Moving to locator img couch position
//*****************************************************************************
void CPqm::SetMoveLocatorMode(
    const bool f_mode
)
{
    m_move_locator_mode = f_mode;
}

//********************************Method Header********************************
//Method Name   :SetDBDtCancelFlag()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
void CPqm::SetDBDtCancelFlag(const BOOL f_dbdt_flag)
{
    m_dbdt_cancel_flag = f_dbdt_flag;
}

//+Patni-HAR/2010Feb24/Added/MSA0251-00187
//****************************Method Header************************************
//Method Name   : SetScanProtocolInfoFromGUI()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
BOOL CPqm::SetScanProtocolInfoFromGUI(
    VARIANT* f_data
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetScanProtocolInfoFromGUI");
    ProtocoInfo* l_protocol_info = (ProtocoInfo*) f_data->byref;
    CPqmProtocol* l_topprotocol = NULL;
    m_pqm_study->GetTopWaitPos(&l_topprotocol);

    if (!IsMatchSequenceParam(l_topprotocol, l_protocol_info)) {
        LogProtocolInfo(l_topprotocol, l_protocol_info);
        //+TMSC-Tanoue/2010Mar05/MSA0251-00247
        //MRERROR_LOG_DATA l_error_log_data;

        CString l_strErrMsg =  CPqmUtilities::GetMultiLingualString(_T("IDS_SCAN_NOT_PERFORM"));
        /*
        l_error_log_data.error_message = l_strErrMsg;
        l_error_log_data.error_source = _T("PQM");
        CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data);
        */
        CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
            CPqmUtilities::GetMultiLingualString(l_strErrMsg), _T("PQM"),
            SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
        //-TMSC-Tanoue/2010Mar05/MSA0251-00247
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);
        return FALSE;
    }

    return TRUE;
}
//-Patni-HAR/2010Feb24/Added/MSA0251-00187

//********************************Method Header********************************
//Method Name   : SetProtocolIndex()
//Author        : PATNI/PJS

//*****************************************************************************
void CPqm::SetProtocolIndex(const int f_index)
{
    m_protocol_index = f_index;
}
//- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699

//****************************Method Header************************************
//Method Name   : SetPauseButtonLabel()
//Author        : PATNI/AC
//Purpose       : This function is used to set the status of the Pause button
//*****************************************************************************
void CPqm::SetPauseButtonLabel(
    const UINT button_status
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(button_status);
}

//****************************Method Header************************************
//Method Name   : SetSensitive()
//Author        : PATNI/AC
//Purpose       : This function is used to set the state of a button
//*****************************************************************************
void CPqm::SetSensitive(
    const UINT tag,
    const bool flag
)
{
    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(tag);
    UNREFERENCED_PARAMETER(flag);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution
}

//****************************Method Header************************************
//Method Name   :SetStartLabelForAPC()
//Author        :PATNI/MRP
//Purpose       :Dummy function to Set Label for APC
//*****************************************************************************
void CPqm::SetStartLabelForAPC(
    const UINT button_label
)
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(button_label);
}


//************************************Method Header************************************
// Method Name  : InitiaseUIMsgMap
// Author       : PATNI/ AMT
// Purpose      : Initialize UI message map for log
//***********************************************************************************
bool CPqm::InitiaseUIMsgMap()
{
#define CASE_OPTION(i) { case i: m_pqm_msg_tag_lit_map[i].index = (pqm_msg_tag)i;\
            m_pqm_msg_tag_lit_map[i].lit=TO_STRING(i); break;}

    int i = -1;

    for (i = 0; i < PQM_MSG_LAST_MSG; i++) {
        switch (i) {
                CASE_OPTION(PQM_MSR_RCV_SEQ);
                CASE_OPTION(PQM_MSG_SEQ_RDY);
                CASE_OPTION(PQM_MSG_STATUS_CURRENT);
                CASE_OPTION(PQM_MSG_SCAN_START);
                CASE_OPTION(PQM_MSG_STATUS_DONE);
                CASE_OPTION(PQM_MSG_ABORT_FAIL);
                CASE_OPTION(PQM_MSG_ABORT_WAIT);
                CASE_OPTION(PQM_MSG_CONTROL_DONE);
                CASE_OPTION(PQM_MSG_PAUSE_DONE);
                CASE_OPTION(PQM_MSG_RESUME_DONE);

                CASE_OPTION(PQM_MSG_ERROR);
                CASE_OPTION(PQM_MSG_ERROR_INIT);
                CASE_OPTION(PQM_MSG_MAX_PROT);
                CASE_OPTION(PQM_MSG_GL_SR_SQSAVED);
                CASE_OPTION(PQM_MSG_INVOKE_MEMORY);
                CASE_OPTION(PQM_MSG_FILE_FAILED);
                CASE_OPTION(PQM_MSG_TRACELOGFLAG);
                CASE_OPTION(PQM_MSG_EVENTLOGFLAG);
                CASE_OPTION(PQM_MSG_TRACELOGPATH);
                CASE_OPTION(PQM_MSG_EVENTLOGPATH);

                CASE_OPTION(PQM_MSG_APC_START);
                CASE_OPTION(PQM_MSG_APC_END);
                CASE_OPTION(PQM_MSG_INJECT_TIME_VISIBLE);
                CASE_OPTION(PQM_MSG_INJECT_TIME_INVISIBLE);
                CASE_OPTION(PQM_MSG_SCAN_START_TRUE);
                CASE_OPTION(PQM_MSG_SCAN_START_FALSE);
                CASE_OPTION(PQM_MSG_SCAN_PAUSE_TRUE);
                CASE_OPTION(PQM_MSG_SCAN_PAUSE_FALSE);
                CASE_OPTION(PQM_MSG_SCAN_ABORT_TRUE);
                CASE_OPTION(PQM_MSG_SCAN_ABORT_FALSE);

                CASE_OPTION(PQM_MSG_ENDWAIT_CURSOR);
                CASE_OPTION(PQM_MSG_SCAN_START_BREATH_HOLD);
                CASE_OPTION(PQM_MSG_BEGINWAIT_CURSOR);
                CASE_OPTION(PQM_MSG_SCAN_BEGIN);
                CASE_OPTION(PQM_MSG_SCAN_END);
                CASE_OPTION(PQM_MSG_SCAN_PAUSE);
                CASE_OPTION(PQM_MSG_SCAN_RESUME);
                CASE_OPTION(PQM_MSG_SHIMMING_END);
                CASE_OPTION(PQM_MSG_APC_PAUSE_END);
                CASE_OPTION(PQM_MSG_CFA_PRESCAN_BEGIN);

                //+Patni-Ravindra Acharya/2010July23/Added/IR-153
                CASE_OPTION(PQM_MSG_CFA_OK_BTN_STATE);
                CASE_OPTION(PQM_MSG_CFA_PRESCAN_END);
                CASE_OPTION(PQM_MSG_CFA_PRESCAN_ABORT);
                CASE_OPTION(PQM_MSG_CFA_PRESCAN_COUCH);
                CASE_OPTION(PQM_MSG_INVOKE_REFPROBE);
                CASE_OPTION(PQM_MSG_APC_RETRY_WAIT);
                CASE_OPTION(PQM_MSG_SR_ABORT_FAIL);
                CASE_OPTION(PQM_MSG_SR_ABORT_WAIT);
                CASE_OPTION(PQM_MSG_SENDING_STARTED);
                CASE_OPTION(PQM_MSG_SENDING_COMPLETED);
                CASE_OPTION(PQM_MSG_GATE_SCAN_END);

                CASE_OPTION(PQM_MSG_SEQ_RDY_DISPLAY);
                CASE_OPTION(PQM_MSG_CFA_REDRAW_WATER);
                CASE_OPTION(PQM_MSG_CFA_REDRAW_FAT);
                // + Patni-Sribanta/2010Jun09/Modified/MVC8032
                //CASE_OPTION(PQM_MSG_MULTICOVERAGE);
                CASE_OPTION(PQM_MSG_ENABLE_APPLY_BTN_MANUAL_PRESCAN);
                CASE_OPTION(PQM_MSG_MANUAL_PRESCAN_VALUES);
                CASE_OPTION(PQM_MSG_MRS_SCAN);
                CASE_OPTION(PQM_MSG_MRS_LOCALSHIM_BEGIN);
                CASE_OPTION(PQM_MSG_MRS_LOCALSHIM_DONE);

                CASE_OPTION(PQM_MSG_MRS_F0_BEGIN);
                CASE_OPTION(PQM_MSG_MRS_F0_DONE);
                CASE_OPTION(PQM_MSG_MRS_REF_BEGIN);
                CASE_OPTION(PQM_MSG_MRS_REF_DONE);
                CASE_OPTION(PQM_MSG_MRS_WSAT_BEGIN);
                CASE_OPTION(PQM_MSG_MRS_WSAT_DONE);
                CASE_OPTION(PQM_MSG_MRS_PRESCAN_DONE);
                CASE_OPTION(PQM_MSG_LOC_BTN);
                CASE_OPTION(PQM_MSG_PROCON_ERR);
                CASE_OPTION(PQM_MSG_PROCON_CMD_EXIT);
                CASE_OPTION(PQM_MSG_CONNECT_FROM_MAIN_THREAD);

                CASE_OPTION(PQM_MSG_PROCON_CONNECTED);
                CASE_OPTION(PQM_MSG_PROCON_NOT_CONNECTED);
                CASE_OPTION(PQM_MSG_PROCON_QUEUE_NEXT);
                CASE_OPTION(PQM_MSG_PROCON_QUEUE_EXIT);
                CASE_OPTION(PQM_MSG_PROCON_NEXT_COPY);
                CASE_OPTION(PQM_MSG_PROCON_RESET);
                CASE_OPTION(PQM_MSG_PROCON_QUEUE_DONE);
                CASE_OPTION(AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL);
                CASE_OPTION(PQM_MSG_PROCON_CON_INHIBIT_SELECTIONS);
                CASE_OPTION(PQM_MSG_PROCON_CON_ALLOW_SELECTIONS);

                CASE_OPTION(PQM_MSG_MAP_DRIVE_NOT_FOUND);
                CASE_OPTION(PROMSG_START_PROBE_PLAN);
                CASE_OPTION(PQM_MSG_PQM2COILSEL_CONNECT_COIL);
                CASE_OPTION(PQM_MSG_COILINFO_SHOW);
                CASE_OPTION(PQM_MSG_PQM2COILSEL_SHOW_COIL);
                CASE_OPTION(PQM_MSG_PQM2COILSEL_SELECTE_COIL);
                CASE_OPTION(PQM_MSG_SCAN_STARTPAUSE_DISABLE);
                CASE_OPTION(PQM_MSG_SCANSTART_ENABLE);
                CASE_OPTION(PQM_MSG_SAME_UID);
                CASE_OPTION(PQM_MSG_SERIES_LOAD);

                CASE_OPTION(PQM_MSG_ENABLE_COUCHBUTTON);
                CASE_OPTION(PQM_MSG_DISABLE_COUCHBUTTON);
                CASE_OPTION(PQM_MSG_DBDT_CAUTION_DIALOG);
                CASE_OPTION(PQM_MSG_COIL_CHANGE_FOR_CURRENT_PROTOCOL);
                CASE_OPTION(PQM_MSG_GATE_DIALOG_SHOW);
                CASE_OPTION(PQM_MSG_ACQMAN_CONNECTED);
                CASE_OPTION(PQM_MSG_ACQMAN_DISCONNECTED);
                CASE_OPTION(PQM_MSG_CORRECT_COIL);
                CASE_OPTION(PQM_MSG_IRS_SCANSTART);
                CASE_OPTION(PQM_MSG_IRS_SCANPAUSE);

                CASE_OPTION(PQM_MSG_IRS_SCANABORT);
                CASE_OPTION(PQM_MSG_UPDATE_SEQUENCE_INFO);
                CASE_OPTION(PQM_MSG_SCANDONE_SHIMMING);
                CASE_OPTION(PQM_MSG_MDB_VDB_NOT_SYNC);
                CASE_OPTION(PQM_MSG_RESET_CURRENT_STATUS);
                CASE_OPTION(PQM_MSG_UPDATE_PAGER_AREA);
                CASE_OPTION(PQM_MSG_INVOKE_LOCATOR);
                CASE_OPTION(PQM_MSG_UPDATE_COUCH_VAL);
                CASE_OPTION(PQM_MSG_IRS_CFW_EF_LAST_IMAGE_DONE);
                CASE_OPTION(PQM_MSG_ON_FLAG_LOCATOR);

                CASE_OPTION(PQM_MSG_PARENT_IMAGE_DELETED_FOR_CURRENT);
                //Patni-ARD/2010Feb05/Added/Deft# MVC006963
                CASE_OPTION(PQM_MSG_APC_RETRY_WAIT_ON_DISCONNECT);
                //+Patni-Hemant/2010Mar25/Added/ACE-Phase#3/CDS Requirement
                CASE_OPTION(PQM_MSG_CDS_GET_CENTER_POS);
                CASE_OPTION(PQM_MSG_CDS_DEBUG);
                CASE_OPTION(PQM_MSG_CDS_COIL_CONFIRMATION);
                CASE_OPTION(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);
                CASE_OPTION(PQM_MSG_CDS_UPDATE_AS_CMPS_FLAG_CUR_PROT);
                CASE_OPTION(PQM_MSG_CDS_APPLY_COMPASS_COIL_TO_ALL_BELOW);
                CASE_OPTION(PQM_MSG_CDS_SELECT_CURRENT_PROTOCOL);
                //-Patni-Hemant/2010Mar25/Added/ACE-Phase#3/CDS Requirement
                //Patni/AM/2010July06/Added/NFD001-AutoMap
                CASE_OPTION(PQM_MSG_INSERT_MAP_PROTOCOL); //Used to insert automap protocol
                //Patni/AM/2010July20/Added/NFD001-AutoShim
                CASE_OPTION(PQM_MSG_INSERT_SHIM_PROTOCOL); //Used to insert autoshim protocol
                CASE_OPTION(PQM_MSG_GL_EXAMUPDATE);
                CASE_OPTION(PQM_MSG_CAN_AUTO_SCAN_START);

                CASE_OPTION(PQM_MSG_INJECT_TIMER_VISIBLE);
                CASE_OPTION(PQM_MSG_MASTER_SLAVE_UPDATE_GUI);
                CASE_OPTION(PQM_MSG_UPDATE_WAIT_FOR_AUTO_SCAN_ICON);
                CASE_OPTION(PQM_MSG_CLOSE_WSE);
                CASE_OPTION(PQM_UPDATE_SAR_INFO);
                CASE_OPTION(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN);
                CASE_OPTION(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_SHOW);
                CASE_OPTION(PQM_MSG_PROCON_INSCAN_SWITCH);
                CASE_OPTION(PQM_MSG_SEQUENCE_TRANSFER_COMPLETED);
                CASE_OPTION(PQM_MSG_UPDATE_GUI_FOR_SAR_OVER);
                CASE_OPTION(PQM_MSG_START_SAR_UPDATE_TIMER);
                CASE_OPTION(PQM_MSG_KILL_SAR_UPDATE_TIMER);
                CASE_OPTION(PQM_MSG_SARMANAGER_CONNECTED);
                CASE_OPTION(PQM_MSG_SARMANAGER_DISCONNECTED);
                CASE_OPTION(PQM_MSG_DISPLAY_WAIT_TIME);
                CASE_OPTION(PQM_MSG_DISPLAY_WAIT_TIME_END);
        }
    }

#undef CASE_OPTION

    return true;
}

//********************************Method Header********************************
//Method Name   :InitializeCouchConfig()
//Author        :PATNI/MSN
//Purpose       :Sets the locator mode that is set using the PQM GUI
//				 Utility menu -> Moving to locator img couch position
//*****************************************************************************
void CPqm::InitializeCouchConfig()
{
    LPCTSTR FUNC_NAME = _T("CPqm::InitializeCouchConfig");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_couchconfig == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_couchconfig pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    m_couchconfig->SetCouchInLimit(-2050);
    m_couchconfig->SetCouchOutLimit(0);
    m_couchconfig->SetCouchMinimumMovement(1);
    m_couchconfig->SetCouchMaximumMovement(1200);

    //+Patni-SS/20091014/Modified/TMSC review comments
    //SetcouchMoveMaximumMovement is allready set as 1200 and replaced with 50
    m_couchconfig->SetCouchSpeedRange(50);
    //-Patni-SS/20091014/Modified/TMSC review comments
    m_couchconfig->SetCouchMaximumParentOffset(150);

    //+Patni-RUP/2009/Sep10/Modify/Modified/cpp test corrections
    char* l_en_etc_path = getenv("GP_ETC_PATH");

    if (NULL == l_en_etc_path) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get value of GP_ETC_PATH environment variable"));
        return;
    }

    CString l_couch_file(l_en_etc_path);
    l_couch_file = l_couch_file + _T("/couch.cf");

    const int l_len = ((l_couch_file.GetLength()) * sizeof(TCHAR)) / sizeof(char);
    char* l_file_name = new char[l_len];

    if (l_file_name == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_file_name is null"));
        return ;
    }

    memset(l_file_name, 0, l_len);
    wcstombs(l_file_name, l_couch_file, l_len) ;

    FILE*	fp = NULL;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    char*	pt = NULL, buff[1024] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if ((fp = fopen(l_file_name, "r"))) {
        while (fgets(buff, sizeof(buff), fp)) {
            if (buff[0] == '#' || (pt = strchr(buff, ':')) == NULL)
                continue;

            *pt++ = '\0';

            if (!strcmp(buff, "Inside Limit"))
                m_couchconfig->SetCouchInLimit(atof(pt));

            else if (!strcmp(buff, "Outside Limit"))
                m_couchconfig->SetCouchOutLimit(atof(pt));

            else if (!strcmp(buff, "Minimum Move"))
                m_couchconfig->SetCouchMinimumMovement(atof(pt));

            else if (!strcmp(buff, "Maximum Move"))
                m_couchconfig->SetCouchMaximumMovement(atof(pt));

            else if (!strcmp(buff, "Slow Speed Range"))
                //Patni-SS/20091014/Modified/TMSC review comments
                m_couchconfig->SetCouchSpeedRange((double)(atof(pt)));

            else if (!strcmp(buff, "Maximum Parent Offset"))
                m_couchconfig->SetCouchMaximumParentOffset((float)(atof(pt)));

        }

        fclose(fp);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to read couch.cf file."));
    }

    DEL_PTR_ARY(l_file_name);

    //-Patni-RUP/2009/Sep10/Modify/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   :InitializeSMInfo()
//Author        :PATNI/AMT
//Purpose       :Read a file sm.cf from /gp/etc/ location and based on information
//                  it initialize member variables.
//*****************************************************************************
bool CPqm::InitializeSMInfo()
{
    LPCTSTR FUNC_NAME = _T("CPqm::InitializeSMInfo");

    //+Patni-RUP/2009/Sep10/Modified/cpp test corrections
    char* l_en_etc_path = getenv("GP_ETC_PATH");

    if (NULL == l_en_etc_path) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to read value of GP_ETC_PATH environment variable"));
        return false;
    }

    CString l_smfile(l_en_etc_path);
    l_smfile = l_smfile + _T("/sm.cf");

    const int l_len = ((l_smfile.GetLength()) * sizeof(TCHAR)) / sizeof(char);
    char* l_file_name = new char[l_len];

    if (l_file_name == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_file_name is null"));
        return false;
    }

    memset(l_file_name, 0, l_len);
    wcstombs(l_file_name, l_smfile, l_len) ;

    FILE* fp = NULL;
    Common_t l_common;

    if ((fp = fopen(l_file_name, "r")) != NULL) {

        InitReadSmConf(fp, &l_common);

        //+Patni-HEMANT/ADDED On 8/11/2009 2:11:50 PM/ MVC2710
        m_sft_mode = l_common.sft_mode;
        AppCommon_t* l_appcommon_p = NULL;
        InitGetAppCommon(&l_appcommon_p);
        m_VAP_Channels = l_appcommon_p->numChannels;
        m_channel_Per_VFEP = l_appcommon_p->channelPerVFEP;
        m_vfep_Proc_Ratio = (float)atof(l_appcommon_p->vfepProcRatio);
        int	l_channel = m_channel_Per_VFEP * m_VAP_Channels;
        m_num_Channels = l_channel;
        m_channel_mask = (1 << (l_channel - 1));
        fclose(fp);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to read sm.cf file."));


        DEL_PTR_ARY(l_file_name);

        return false;
    }

    //-Patni-RUP/2009/Sep10/Modified/cpp test corrections

    //Added by Meghana On 10/7/2008 4:44:30 PM
    m_systype = l_common.sys_type;

    //Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
    m_sys_name = l_common.sys_name;
    m_gate_mode = l_common.gate_mode;

    DEL_PTR_ARY(l_file_name);

    return true;
}

//*****************************************************************************
//Method Name   :ReadPqmAppConfigFile()
//Author        :PATNI/LK
//Purpose       :This method will read all Protocol related information from
//              PQM_AppConfig.xml placed in MRMP_Config
//*****************************************************************************
BOOL CPqm::ReadPqmAppConfigFile(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ReadPqmAppConfigFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    wchar_t l_buff[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buff, MAX_PATH);
    CString l_MR_config(l_buff);

    l_MR_config = l_MR_config + _T("\\Trace");
    CPQMLogMgr::GetInstance()->SetTraceConfigFilePath(l_MR_config);

    // setting TraceLog CSSDK values
    int l_tracelog_cssdk_value = 0;
    BOOL l_status = CPQMConfig::GetInstance()->GetTraceLogCSSDKValue(l_tracelog_cssdk_value);

    CString l_tracelog_path(_T("")), l_eventlog_path(_T(""));

    if (l_status) {
        CPQMLogMgr::GetInstance()->SetTraceLogUseCssdk(l_tracelog_cssdk_value);

    } else {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("InValid CSSDK values present for TraceLog in config file"),
                ERROR_LOG_INFORMATION1,
                _T("Pqm.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_CSSDK_VAL_FOR_TRACELOG")),
            _T("Pqm.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

        CPQMLogMgr::GetInstance()->SetTraceLogUseCssdk(0);
    }

    // setting TraceLog path
    l_status = CPQMConfig::GetInstance()->GetTraceLogPath(l_tracelog_path);

    if (l_status == INVALID_XML_FILE || l_status == INVALID_TRACE_PATH) {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Trace Path Invalid"),
                ERROR_LOG_INFORMATION1,
                _T("Pqm.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_TRACE_LOG_PATH_ERROR")),
            _T("Pqm.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);
    }

    CPQMLogMgr::GetInstance()->SetTraceLogPath(l_tracelog_path);

    // setting EventLog CSSDK values
    int l_eventlog_cssdk_value = 0;
    l_status = CPQMConfig::GetInstance()->GetEventLogCSSDKValue(l_eventlog_cssdk_value);

    if (l_status) {
        CPQMLogMgr::GetInstance()->SetEventLogUseCssdk(l_eventlog_cssdk_value);

    } else {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("InValid CSSDK values present for EventLog in config file"),
                ERROR_LOG_INFORMATION1,
                _T("Pqm.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_CSSDK_VAL_FOR_EVENTLOG")),
            _T("Pqm.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);
        CPQMLogMgr::GetInstance()->SetEventLogUseCssdk(0);
    }


    // setting EventLog path
    l_status = CPQMConfig::GetInstance()->GetEventLogPath(l_eventlog_path);

    if (l_status == INVALID_XML_FILE || l_status == INVALID_TRACE_PATH) {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Invalid EventLog path"),
                ERROR_LOG_INFORMATION1,
                _T("Pqm.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_EVENTLOG_PATH")),
            _T("Pqm.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);
    }

    CPQMLogMgr::GetInstance()->SetEventLogPath(l_eventlog_path);

    // set the mode for both trace and event
    CPQMLogMgr::GetInstance()->InitEventLog(m_mode);
    CPQMLogMgr::GetInstance()->InitTraceLog(m_mode);

    return l_status;
}

//****************************Method Header************************************
//Method Name   :LoadCoilInfo()
//Author        :PATNI/JCM
//Purpose       :This method reads the coil information from the database
//*****************************************************************************
bool CPqm::LoadCoilInfo(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::LoadCoilInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    /*if (m_coilDB == NULL) {
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                USER_FUNC_MARKER, _T("CPqm::LoadCoilInfo"),
                _T("m_coilDB pointer is NULL"));
        return false;
    }*/

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    /*if (!m_coilDB->ReadCoilDataFromDB()) {
        result = false;
    }

    m_coildata = m_coilDB->GetCoilData();
    CString str_result(_T(""));
    str_result.Format(_T("Result: %d"), result);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_result);
    */
    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    return true;
}

//****************************Method Header************************************
//Method Name   :LoadColorInfo()
//Author        :PATNI/MSN
//Purpose       :This method reads the color information from the database
//*****************************************************************************
bool CPqm::LoadColorInfo(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::LoadColorInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    bool result = true;

    CString l_trace_msg(_T(""));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_colorinfo) {
        if (m_colorinfo->ReadColorSetting()) {
            m_color = m_colorinfo->GetColorSetting();
            l_trace_msg.Format(_T("Color Settings are SUCESSFULLY loaded"));

        } else {
            m_color = NULL;
            result = false;
            l_trace_msg.Format(_T("Color Settings loading FAILURE"));
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_colorinfo pointer is NULL"));

        m_color = NULL;
        result = false;
        l_trace_msg.Format(_T("Color Settings loading FAILURE"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

    return result;
}

//+Patni-Sribanta/2010Nov18/Added/REDMINE-772
//****************************Method Header************************************
//Method Name   :IntializeShimPrepProbeData()
//Author        :PATNI/HAR-Sribanta
//Purpose       :
//*****************************************************************************
void CPqm::IntializeShimPrepProbeData(
) const
{
    CString FUNC_TRACE(_T("CPqm::IntializeShimPrepProbeData"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, FUNC_TRACE);

    float l_couchpos = PQM_COUCH_INIT_POSITION;
    CPqmShimSharedData::GetInstance()->ShimRegistCouchInfo(NULL,
            &l_couchpos, &l_couchpos);
    CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(
        &l_couchpos, &l_couchpos, &l_couchpos); //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    CPqmShimSharedData::GetInstance()->InitShim();
    WriteToUI(PQM_MSG_CFA_SET_FSY_WATER);

    m_pqm_study->WriteToShimData();
    char l_file_name[512] = {0};

    CPqmUtilities::GetPrepStudyName(l_file_name);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, CString(l_file_name));

    if ((NULL != m_pqm_procon) && m_pqm_procon->GetPQMProbe()) {
        m_pqm_procon->GetPQMProbe()->StudyPlanInit(l_file_name, 0);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, CString(l_file_name));
    CPqmUtilities::GetProbeStudyName(l_file_name);

    if ((NULL != m_pqm_procon) && m_pqm_procon->GetPQMProbe()) {
        m_pqm_procon->GetPQMProbe()->StudyPlanInit(l_file_name, 0);
    }
}

//****************************Method Header************************************
//Method Name   :ResetShimDataFromFile()
//Author        :PATNI/HAR-Sribanta
//Purpose       :
//*****************************************************************************
void CPqm::ResetShimDataFromFile(
)
{
    CString FUNC_TRACE(_T("CPqm::ResetShimDataFromFile"));
    m_is_same_study     = true;
    int l_scannumber    = -1;
    int l_shimstate     = SHIM_STATE_WAIT;
    int l_shimfsyflag   = VFC_SHIM_FSY_WATER;
    float l_couchpos    = PQM_COUCH_INIT_POSITION;
    float l_homepos     = PQM_COUCH_INIT_POSITION;
    float l_txtunepos   = PQM_COUCH_INIT_POSITION;
    float l_rflpos      = PQM_COUCH_INIT_POSITION;

    if (m_pqm_study->ReadShimValuesFromFile(&l_scannumber, &l_couchpos, &l_homepos,
                                            &l_txtunepos, &l_rflpos, &l_shimstate, &l_shimfsyflag)) {

        CPqmShimSharedData::GetInstance()->ShimRegistCouchInfo(
            &l_scannumber, &l_couchpos, &l_homepos);
        CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(
            &l_txtunepos, &l_rflpos, &l_rflpos);//Patni-NP/2010May26/Added/IR-151
        CPqmShimSharedData::GetInstance()->SetShimState(l_shimstate);
        CPqmShimSharedData::GetInstance()->SetShimFsyFlag(l_shimfsyflag);

        if (l_shimfsyflag == VFC_SHIM_FSY_FAT) {

            WriteToUI(PQM_MSG_CFA_SET_FSY_FAT);

        } else {

            WriteToUI(PQM_MSG_CFA_SET_FSY_WATER);
        }


        m_pqm_study->WriteToShimData();

        CString l_tracemsg(_T(""));
        l_tracemsg.Format(_T("Scan Number = %d \n Couch Position =%f\n, \
                                                                          Home Position =%f\n RFL Pos =%f\nTxTunePos = %f \nShim State = %d \
                                                                          \nShim FsyFlag = %d read from ShimData.txt file"), l_scannumber,
                          l_couchpos, l_homepos, l_txtunepos, l_rflpos, l_shimstate,
                          l_shimfsyflag);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, l_tracemsg);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, _T("Unable to Read Shim Data from File"));
    }
}
//-Patni-Sribanta/2010Nov18/Added/REDMINE-772

//********************************Method Header********************************
//Method Name   :ReSetReceiverCoil()
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
void CPqm::ReSetReceiverCoil(
    CPASCOMSequence* f_pascomseq
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ReSetReceiverCoil");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPASParamStore* seq_param = f_pascomseq->GetSequenceParam();
    seq_param->SetInt(_T("RC"), -1);
    seq_param = NULL;
}

//********************************Method Header********************************
//Method Name   :ReSetScanAnatomy()
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
void CPqm::ReSetScanAnatomy(
    CPASCOMSequence* f_pascomseq
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ReSetScanAnatomy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPASParamStore* seq_param = f_pascomseq->GetSequenceParam();
    seq_param->SetInt(_T("SA"), -1);
    seq_param = NULL;
}

//********************************Method Header********************************
//Method Name   :SerializeSequenceList()
//Author        :PATNI/DJ
//Purpose       :Serialize the sequence list and put it into memory file.
//*****************************************************************************
void CPqm::SerializeSequenceList(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SerializeSequenceList");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == CPQMIPCManager::GetThreadEvent()) {
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Unable to create thread event, failed to prepare sequence list."),
            ERROR_LOG_CRUCIAL1 , _T("Pqm.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
    }

    //WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock

    LPCTSTR l_loid = _T("");

    if (m_mode == K_SCHEDULING) {
        //	    PostThreadMessage(CPQMIPCManager::dThreadId,MSG_MAKESERIES,(WPARAM)&m_snd_schedule_seq_list,(LPARAM)l_loid);

        m_dbsa_study_mgr->MakeSeries(
            &m_snd_schedule_seq_list,
            l_loid);

    } else if (m_mode == K_IMAGING) {
        //	PostThreadMessage(CPQMIPCManager::dThreadId,MSG_MAKESERIES,(WPARAM)&m_snd_acquire_seq_list,(LPARAM)l_loid);

        m_dbsa_study_mgr->MakeSeries(
            &m_snd_acquire_seq_list,
            l_loid);

    }

    //WaitForSingleObject(CPQMIPCManager::m_ThreadEvent,INFINITE);
    //-Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock

    //WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
}

//********************************Method Header********************************
//Method Name   :SerializeSequenceParamList()
//Author        :PATNI/DJ
//Purpose       :Serialize the sequence parameter list and put it into memory file.
//*****************************************************************************
void CPqm::SerializeSequenceParamList(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SerializeSequenceParamList");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CArchive StoreArchive(&memFile, CArchive::store);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->ReadSequenceParameterXML(g_sequence_param_name_list);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   :SendSequencesToGUI()
//Author        :PATNI/DJ
//Purpose       :This function will serialize and send the sequences to PQM GUI.
//*****************************************************************************
void CPqm::SendSequencesToGUI(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SendSequencesToGUI");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_no_of_sequence == 0) {
        SerializeSequenceList();
        //Patni-PJS/2010Mar10/Modified/MSA0251-00243
        m_no_of_sequence_transferred = 0;
        WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    }
}

//********************************Method Header********************************
//Method Name   :ReceiveSequences()
//Author        :PATNI/DJ
//Purpose       :This function will collect sequences from Memory PASSCOM.
//*****************************************************************************
void CPqm::ReceiveSequences(
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ReceiveSequences");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    SequenceList sequence_list;
    int mode = 0;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    m_pascom->GetAllSequences(sequence_list, mode);
    POSITION pos = sequence_list.GetHeadPosition();
    CPASCOMSequence* ppascom_seq = sequence_list.GetNext(pos);

    CString str_mode = _T("");
    str_mode.Format(_T("Mode: %d, m_snd_schedule_seq_list: %d, m_snd_acquire_seq_list: %d"),
                    mode, m_snd_schedule_seq_list.GetCount(), m_snd_acquire_seq_list.GetCount());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_mode);

    if (m_no_of_sequence > 0) {
        if (mode == K_SCHEDULING) {
            m_snd_schedule_seq_list.AddTail(&sequence_list);

        } else {
            m_snd_acquire_seq_list.AddTail(&sequence_list);
        }

        m_no_of_sequence--;
    }
}

//********************************Method Header********************************
//Method Name   :ReceiveSequences()
//Author        :PATNI/DJ
//Purpose       :This function will collect sequences from Memory PASSCOM.
//*****************************************************************************
void CPqm::ReceiveSequences(
    CString f_sequence_path,
    const CString& f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::ReceiveSequences");

#ifdef __SM_PERFORMANCE_CHECK__
    __int64 l_freq = 0, l_stop = 0, l_start = 0;
    __int64 l_freq1 = 0, l_stop1 = 0, l_start1 = 0;
    unsigned long l_receive_sequence_time = 0;
    unsigned long l_flush_study_file_time = 0;
    unsigned long timediff = 0;
    unsigned long l_get_single_seq_time = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&l_start1);
#endif

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CPASCOMSequence* l_seq_temp = 0;
    long l_tempordr = _ttol((LPCTSTR)f_acq_order);
    CString l_refstr = f_sequence_path;
    int l_pos = f_sequence_path.ReverseFind(_T('\\'));
    CString l_new_str = f_sequence_path.Mid(l_pos + 1);
    int l_length = l_new_str.GetLength() + 1;
    f_sequence_path.Delete(l_pos, l_length);

    SequenceList sequence_list;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    int mode = m_pascom->GetMode();

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("P1**GetSingleSequence **** PASCOM Call ***** - Before"));
#ifdef __SM_PERFORMANCE_CHECK__
    QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif

    l_seq_temp = m_pascom->GetSingleSequence(l_refstr, l_tempordr);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_seq_temp == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_seq_temp pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
#ifdef __SM_PERFORMANCE_CHECK__
    QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);
    timediff = (unsigned long)(((l_stop - l_start) * 1000000) / l_freq);
    m_get_single_seq_time += timediff;
#endif

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("P1**GetSingleSequence **** PASCOM Call ***** - After"));

    SequenceList l_sequence_list;
    CString l_sequence_path(_T(""));

    l_seq_temp->GetSequencePath(l_sequence_path);

    l_new_str = _T(" PLANSTATUS = 0 ");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_new_str);

    CString str_mode(_T(""));
    str_mode.Format(_T("Mode: %d, m_snd_schedule_seq_list: %d, m_snd_acquire_seq_list: %d"),
                    mode,
                    m_snd_schedule_seq_list.GetCount(),
                    m_snd_acquire_seq_list.GetCount());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_mode);

    VuiAcqOrder_t l_acq_order;
    BITFLD_DECL(mask, MAX_NODES) = {0};
    BITFLD_INIT(mask, MAX_NODES, 0);

    ClibVfToUiAdapter vuiAdapter(m_pqm_study->GetVfDBHandle());

    if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_acq_order)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() Failed."));
        return ;
    }

    CString l_series_count(_T(""));
    l_series_count.Format(_T("Branches = %d"), l_acq_order.SzAcqOrder);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_series_count);

    const long l_maxprotocol_limit = CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();
    const int l_difference = l_maxprotocol_limit - ((l_acq_order.SzAcqOrder) + m_recieved_request_no);

    if (l_difference <= 0) {

        DEL_PTR(l_seq_temp);

        if (++m_recieved_request_no != m_no_of_sequence) {
            return;
        }

        l_new_str = _T("");
        l_new_str =  CPqmUtilities::GetMultiLingualString(_T("IDS_NO_SEQ_ACCEPATANCE"));

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_new_str,
                MPlus::EH::Information, _T("pqm.cpp"), _T("PQM"));

    }

    if (m_recieved_request_no != m_no_of_sequence) {
        if (m_recieve_request_arr == NULL) {
            m_recieve_request_arr = new int[m_no_of_sequence];
            memset(m_recieve_request_arr, -1, (sizeof(int))*m_no_of_sequence);
        }

        m_recieve_request_arr[m_recieved_request_no] = l_tempordr;
        m_recieved_request_no++;

        l_sequence_list.AddTail(l_seq_temp);

        if (m_no_of_sequence > 0) {
            if (mode == K_SCHEDULING) {
                m_snd_schedule_seq_list.AddTail(&l_sequence_list);

            } else {
                m_snd_acquire_seq_list.AddTail(&l_sequence_list);
            }
        }

        m_vf_add_count++;
#ifdef __SM_PERFORMANCE_CHECK__
        bool messg_logged = false;
#endif
    }

    if (m_recieved_request_no == m_no_of_sequence) {
        int l_return_val = E_ERROR;

        if (m_no_of_sequence > 0) {
            if (mode == K_SCHEDULING) {
                l_return_val = m_pqm_study->WriteToStudy(l_sequence_path, m_recieve_request_arr, m_vf_add_count);

            } else {
                l_return_val = m_pqm_study->WriteToStudy(l_sequence_path, m_recieve_request_arr, m_vf_add_count);
            }
        }

        if (l_return_val == E_ERROR) {
            if (mode == K_SCHEDULING) {
                m_snd_schedule_seq_list.RemoveAll();

            } else {
                m_snd_acquire_seq_list.RemoveAll();
            }
        }

#ifdef __SM_PERFORMANCE_CHECK__
        QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq1);
        QueryPerformanceCounter((LARGE_INTEGER*)&l_stop1);
        timediff = (unsigned long)(((l_stop1 - l_start1) * 1000000) / l_freq1);
        m_receive_sequence_time += timediff;
        CPQMLogMgr::GetInstance()->TracePerformanceTime(m_get_single_seq_time, _T("PQM:IPC::ReceiveSequences::Get Single Sequence"));
        CPQMLogMgr::GetInstance()->TracePerformanceTime(m_receive_sequence_time, _T("PQM:IPC::ReceiveSequences"), true);
        messg_logged = true;
        m_get_single_seq_time = 0;
        m_receive_sequence_time = 0;
#endif
        m_no_of_sequence = 0;
        m_recieved_request_no = 0;
        m_vf_add_count = 0;

        DEL_PTR_ARY(m_recieve_request_arr);

    }

#ifdef __SM_PERFORMANCE_CHECK__

    if (!messg_logged) {
        QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq1);
        QueryPerformanceCounter((LARGE_INTEGER*)&l_stop1);
        timediff = (unsigned long)(((l_stop1 - l_start1) * 1000000) / l_freq1);
        m_receive_sequence_time += timediff;
    }

#endif
}

//****************************Method Header************************************
//Method Name   :UpdateScanTime()
//Author        :PATNI/JCM
//Purpose       :This method sends total duration for scanning
//*****************************************************************************
void CPqm::UpdateScanTime(
    COleDateTime  scandate_time
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::UpdateScanTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, scandate_time.Format(LOCALE_NOUSEROVERRIDE));
}

//****************************Method Header************************************
//Method Name   :UpdateCoilData()
//Author        :PATNI/JCM
//Purpose       :This method will send a notification when the coil data is
//               read from the database
//*****************************************************************************
void CPqm::UpdateCoilData(
) const
{
    LPCTSTR FUNC_NAME = _T("CPqm::UpdateCoilData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inform PQM GUI from here"));
}

//****************************Method Header************************************
//Method Name   :SelectSeries()
//Author        :PATNI/JCM
//Purpose       :This method will notify PQM GUI to select or unselect
//               a particular series.
//*****************************************************************************
void CPqm::SelectSeries(
    const int series_id,
    const bool series_state
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SelectSeries");
    CString str_msg = _T("");
    str_msg.Format(_T("%d %d"), series_id, series_state);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              str_msg + _T("Inform PQM GUI from here to select/unselect the particular series"));
}

//****************************Method Header************************************
//Method Name   : ShiftOffsetVector()
//Author        : PATNI/AC
//Purpose       : This function shifts the ui vector according to the protocol, mode, direction and amount
//*****************************************************************************
void CPqm::ShiftOffsetVector(
    const int protocol,
    const int mode,
    const int direction,
    const float amount
)
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(protocol);
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(direction);
    UNREFERENCED_PARAMETER(amount);
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
}

//****************************Method Header************************************
//Method Name   :InitializeRis()
//Author        :PATNI/Ravindra Acharya
//Purpose       :This method Sets the value of risName and risDictionary
//*****************************************************************************
void CPqm::InitializeRis()
{
    //+Patni-AMT/2010Apr19/Modified/JaFT# MVC7300_CDR_01
    CString l_ris_dictionary = getenv("GP_PRO_DIR");
    l_ris_dictionary += _T("/proRISDictionary");

    SetRisDictionary(l_ris_dictionary) ;
    SetRisName(_T("ris\0")) ;
    //-Patni-AMT/2010Apr19/Modified/JaFT# MVC7300_CDR_01
}

//********************************Method Header********************************
//Method Name   :WcharToChar()
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
void CPqm::WcharToChar(
    char* des,
    WCHAR* buff
)
{
    if (buff == NULL) {
        return;
    }

    if (!des) {
        des = new char[wcslen(buff)];
        // MEM_INI AMIT 20081230
        memset(des, 0, sizeof(char) *wcslen(buff)) ;
    }

    while (*buff != '\0') {
        *des++ = (char) * buff++;
    }

    *des = '\0';
}

//********************************Method Header********************************
//Method Name   :StartPrescan()
//Author        :PATNI/MSN
//Purpose       :Starts Prescan for selected series
//*****************************************************************************
bool CPqm::StartPrescan(const int seriesuid)
{
    LPCTSTR FUNC_NAME = _T("CPqm::StartPrescan");

    CString str_id(_T(""));
    str_id.Format(_T("Series UID: %d"), seriesuid);
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_id);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmProtocol* protocol = NULL;

    bool result = false;

    if (m_pqm_study->GetTopWaitPos(&protocol) < 0 || protocol == NULL) {

        //+Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("No protocol in study file with wait status !"),
            ERROR_LOG_INFORMATION1, _T("Pqm.cpp"));
        	*/
        //-Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_WAIT_PROTOCOLS, _T("IDS_NO_WAIT_PROTOCOLS"),
                SCAN_ERR_NOTIFY, L"Pqm.cpp");

        return result;
    }

    // Please delete this function calls before merging into VSS
    // please remove unnecessary commented code
    // + SM4 DEFECT#864 AMIT + Himanshu
    //m_pqm_study->SetCoilInDB(m_selected_coil_key, protocol->GetProtocol());
    //    m_pqm_study->SetCoilInDB(COILKEY, protocol->GetProtocol());
    // below one line code has commented by Integration defect.... Saurabh
    //   m_pqm_study->SetAnatomyInDB(ANATOMY_GUI, protocol->GetProtocol());

    //  m_pqm_study->SetPreScanValue(protocol->GetProtocol()); // uncommented

    // + Values that did not upload from Binary study file
    //protocol->GetPqmPrMode()->SetRecvCoilId(COILKEY);
    protocol->GetPqmPrMode()->SetRecvCoilId(m_selected_coil_key);
    // - Values that did not upload from Binary study file

    CScanProcedure* scanprocedure = m_acq_manager->GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scanprocedure) {
        scanprocedure->SetScanProcProtocol(protocol);
        scanprocedure->SetProt(scanprocedure->GetScanProcProtocol()->GetProtocol());
        scanprocedure->SetIsScanning(true);

        if ((protocol == NULL) || (protocol->GetProtocol() < 0)) {
            m_pqm_study->SetScanModeOfHead();
            scanprocedure->ResetCurrentRequest();
            return result;

        } else if (protocol->GetProcessing() > 0) {
            return result;
        }

        return result;

    } else {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("scanprocedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//************************************Method Header************************************
// Method Name  : NewSequencesAdded
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqm::NewSequencesAdded(
    const int f_count
)
{
    //+Patni-PJS/2010Feb2/Modified/IR-97
    /*
    if (m_wfdaimplementer) {

        return m_wfdaimplementer->NewSequencesAdded(f_count);
    }
    */
    //-Patni-PJS/2010Feb2/Modified/IR-97

    ASSERT(FALSE);
    return true;
}

//****************************Method Header************************************
//Method Name   :CheckForValidLicense()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CPqm::CheckForValidLicense()
{

    LPCTSTR FUNC_NAME = _T("CPqm::CheckForValidLicense");

#define CHECK_LIC(lic) (CheckLicense2(NULL, lic) == LICENSE_OK) ? TRUE : FALSE

    m_mrs_license = ((TRUE == CHECK_LIC("MRSC")) || (TRUE == CHECK_LIC("MRSNC"))) ? TRUE : FALSE ;

    if (CHECK_LIC("DBDT_IEC") == TRUE) {
        m_dBdtlicense = TRUE;

    } else {
#if 1
        m_dBdtlicense = TRUE;
#else
        m_dBdtlicense = FALSE;
#endif

    }

    m_dBdt2002_1_license = CHECK_LIC("DBDT02_1");
    m_dBdt2002_2_license = CHECK_LIC("DBDT02_2");
    m_SAR02_01_License = CHECK_LIC("SAR02_01");
    m_SAR02_02_License = CHECK_LIC("SAR02_02");
    m_MRS_BRST_license = CHECK_LIC("MRS_BRST");
    m_NoiseReduction_license = CHECK_LIC("NoiseReduction");
    m_Mrapv2002_license = CHECK_LIC("MRAPV2002");
    m_muliscan_dyn_license = CHECK_LIC("MULTIS_DYN");
    m_EMTONE_license = CHECK_LIC("EM_TONE");
    m_CALA_license = CHECK_LIC("CRDC_PLN");
    m_HALA_license = CHECK_LIC("NEURO_PLN");

    //ini file license temprary //KJ/2014
    CheckForTiPrepLicense();//m_Tiprep_license = //CHECK_LIC("TIPREP");//In future////KJ/2014    

    m_amb_licence = CHECK_LIC("AMB");
    m_sarcontrol_license = CHECK_LIC("SARCTRL");

#undef CHECK_LIC
}

//********************************Method Header********************************
//Method Name   :GetIECEdition()
//Author        :PATNI/MSN
//Purpose       :Returns the IEC_Edition pointer
//****************************************************************************
bool_t CPqm::GetIECEdition(
)const
{
    return m_IEC_Edition;
}

//+Patni-Manishkumar/2009Jul29/Modified/MVC003316
//************************************Method Header************************************
// Method Name  : GetTotalSequence
// Author       : PATNI/ Manish
// Purpose      : Gets Total Sequence
//***********************************************************************************
long CPqm::GetTotalSequence() const
{
    return (long)m_no_of_sequence;
}
//-Patni-Manishkumar/2009Jul29/Modified/MVC003316

//+ Patni-NP/2009Aug21/Added/License Check
//********************************Method Header********************************
//Method Name   :GetMrapv2002_License()
//Author        :PATNI/NP
//Purpose       :Returns the Mrapv2002 license
//*****************************************************************************
bool_t CPqm::GetMrapv2002_License()const
{
    return m_Mrapv2002_license;
}
//- Patni-NP/2009Aug21/Added/License Check
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
CWFDAHandlerInterface* CPqm::GetWFDAHandlerInterface(
)
{
    if (GetWFDAImplementer()) {

        return GetWFDAImplementer()->GetWFDAHandlerInterface();
    }

    ASSERT(FALSE);
    return NULL ;
}
*/
//-Patni-PJS/2010Feb2/Modified/IR-97

CPQMDebugInfo* CPqm::GetDebugInfoPtr()
{
    return m_debugInfo ;
}

//********************************Method Header********************************
//Method Name   :GetDBDtCancelFlag()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetDBDtCancelFlag()const
{
    return m_dbdt_cancel_flag;
}

//********************************Method Header********************************
//Method Name   :GetAutoVoice()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetAutoVoice(
)const
{
    return m_auto_voice;
}

//********************************Method Header********************************
//Method Name   :GetSelectedCoilKey()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
unsigned long CPqm::GetSelectedCoilKey(
)const
{
    return m_selected_coil_key;
}


/****************************Method Header************************************
//Method Name   :GetRefProbeNotAlive()
//Author        :PATNI/MSN
//Purpose       :Get if RefProbe is alive or not
//*****************************************************************************/
bool CPqm::GetRefProbeNotAlive(
)const
{
    return m_is_refprobe_alive;
}

//********************************Method Header********************************
//Method Name   :GetMoveLocatorMode()
//Author        :PATNI/MSN
//Purpose       :Returns the locator mode that is set using the PQM GUI
//				 Utility menu -> Moving to locator img couch position
//*****************************************************************************
bool CPqm::GetMoveLocatorMode(
) const
{
    return m_move_locator_mode;
}

//********************************Method Header********************************
//Method Name   :GetdBdt2002_1_License()
//Author        :PATNI/MSN
//Purpose       :Returns the dBdt2002_1_License pointer
//****************************************************************************
bool_t CPqm::GetdBdt2002_1_License(
)const
{
    return m_dBdt2002_1_license;
}

//********************************Method Header********************************
//Method Name   :GetdBdtLicense()
//Author        :PATNI/MSN
//Purpose       :Returns the dBdt license
//*****************************************************************************
bool_t CPqm::GetdBdtLicense() const
{
    return m_dBdtlicense;
}

//********************************Method Header********************************
//Method Name   :SetdBdtLicense()
//Author        :PATNI/MSN
//Purpose       :Sets the dBdt license
//*****************************************************************************
void CPqm::SetdBdtLicense(const bool_t f_license)
{
    m_dBdtlicense = f_license;
}

//********************************Method Header********************************
//Method Name   :GetCoverageMsg()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CPqm::GetCoverageMsg(
)
{
    return m_coverage_msg;
}

//********************************Method Header********************************
//Method Name   :SetCoverageMsg()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CPqm::SetCoverageMsg(
    int coverage_msg
)
{
    m_coverage_msg = coverage_msg;
}

//********************************Method Header********************************
//Method Name   :GetStartVoice()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetStartVoice(
)const
{
    return m_start_voice;
}

//********************************Method Header********************************
//Method Name   :SetStartVoice()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetStartVoice(
    const BOOL start_voice
)
{
    m_start_voice = start_voice;
}

//********************************Method Header********************************
//Method Name   :GetFreezeLevel()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CPqm::GetFreezeLevel(
)const
{
    return m_freeze_level;
}

//********************************Method Header********************************
//Method Name   :SetFreezeLevel()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetFreezeLevel(
    const int freeze_level
)
{
    m_freeze_level = freeze_level;
}

//********************************Method Header********************************
//Method Name   :SetAfteAutoScan()
//Author        :PATNI/
//Purpose       :
//****************************************************************************
int CPqm::GetAfterAutoScan(
) const
{
    return m_after_auto_scan;
}

//********************************Method Header********************************
//Method Name   :SetAfteAutoScan()
//Author        :PATNI
//Purpose       :
//****************************************************************************
void CPqm::SetAfteAutoScan(
    const int f_after_auto_scan
)
{
    m_after_auto_scan = f_after_auto_scan;
}

//********************************Method Header********************************
//Method Name   :GetdBdt2002_2_License()
//Author        :PATNI/MSN
//Purpose       :Returns the dBdt license
//*****************************************************************************
bool_t CPqm::GetdBdt2002_2_License()const
{
    return m_dBdt2002_2_license;
}

//********************************Method Header********************************
//Method Name   :SetdBdtLicense()
//Author        :PATNI/MSN
//Purpose       :Sets the dBdt license
//*****************************************************************************
void CPqm::SetdBdt2002_2_License(const bool_t f_license)
{
    m_dBdt2002_2_license = f_license;
}

//********************************Method Header********************************
//Method Name   :GetSAR02_01_License()
//Author        :PATNI/MSN
//Purpose       :Returns the SAR license
//*****************************************************************************
bool_t CPqm::GetSAR02_01_License()const
{
    return m_SAR02_01_License;
}

//********************************Method Header********************************
//Method Name   :SetSAR02_01_License()
//Author        :PATNI/MSN
//Purpose       :Sets the dBdt license
//*****************************************************************************
void CPqm::SetSAR02_01_License(const bool_t f_license)
{
    m_SAR02_01_License = f_license;
}

//********************************Method Header********************************
//Method Name   :GetSAR02_02_License()
//Author        :PATNI/MSN
//Purpose       :Returns the SAR license
//*****************************************************************************
bool_t CPqm::GetSAR02_02_License()const
{
    return m_SAR02_02_License;
}

//********************************Method Header********************************
//Method Name   :SetSAR02_02_License()
//Author        :PATNI/MSN
//Purpose       :Sets the dBdt license
//*****************************************************************************
void CPqm::SetSAR02_02_License(const bool_t f_license)
{
    m_SAR02_02_License = f_license;
}

//********************************Method Header********************************
//Method Name   :SetMoveLocatorMode()
//Author        :PATNI/MSN
//Purpose       :Sets the locator mode that is set using the PQM GUI
//				 Utility menu -> Moving to locator img couch position
//*****************************************************************************
void CPqm::SetHiSpeedCouch(
    const bool f_hi_speed_couch
)
{
    m_hi_speed_couch = f_hi_speed_couch;
}

//********************************Method Header********************************
//Method Name   :SetNoOfSequence()
//Author        :PATNI/DJ
//Purpose       :This function will set receiving no. of sequence from Memory managar.
//*****************************************************************************
void CPqm::SetNoOfSequence(
    const int total_sequences
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetNoOfSequence");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_no_of_sequence == 0) {
        m_no_of_sequence = total_sequences;
        m_no_of_sequence_transferred = total_sequences;
        m_total_seq += total_sequences;

        if (m_mode == K_SCHEDULING) {

            m_pqm_study->ClearSequenceList(m_snd_schedule_seq_list);

        } else {

            m_pqm_study->ClearSequenceList(m_snd_acquire_seq_list);
        }
    }
}

void CPqm::SetSpeederPtr(CPqmSpeeder* f_speeder)
{
    m_pqmSpeeder = f_speeder;
}

//****************************Method Header************************************
//Method Name   : SetSysType()
//Author        : PATNI/HAR
//Purpose       : Set the Sys Type
//*****************************************************************************
void CPqm::SetSysType(
    const int sys_type
)
{
    m_systype = sys_type;
}

//****************************Method Header************************************
//Method Name   : SetProcessing()
//Author        : PATNI/ MRP
//Purpose       : To Set the processing flag
//*****************************************************************************
void CPqm::SetProcessing(
    const int processing
)
{
    m_processing_ui = processing;
}

//****************************Method Header************************************
//Method Name   : SetCouchLimitAPC()
//Author        : PATNI/HAR
//Purpose       : Set the couch limit of APC
//*****************************************************************************
void CPqm::SetCouchLimitAPC(
    const float couch_limit_apc
)
{
    m_couch_limit_apc = couch_limit_apc;
}

//********************************Method Header********************************
//Method Name   :SetDRKSreconMode()
//Author        :PATNI/MRP
//Purpose       :To set the value for DRKS Recon Mode
//*****************************************************************************
void CPqm::SetDRKSreconMode(
    const bool drks_recon_mode
)
{
    m_drks_recon_mode = drks_recon_mode;
}

//********************************Method Header********************************
//Method Name   :SetDefaultRGN()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetDefaultRGN(
    float default_rgn
)
{
    m_default_rgn = default_rgn;
}

//********************************Method Header********************************
//Method Name   :SetRxGainOffset()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetRxGainOffset(
    float rx_gain_offset
)
{
    m_rx_gain_offset = rx_gain_offset;
}

//********************************Method Header********************************
//Method Name   : SetRisDictionary()
//Author        : PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetRisDictionary(
    const CString& f_ris_dictionary
)
{
    m_ris_dictionary = f_ris_dictionary;
}

//********************************Method Header********************************
//Method Name   :SetRisName()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetRisName(
    const CString&  f_ris_name
)
{
    m_ris_name = f_ris_name;
}

//********************************Method Header********************************
//Method Name   :SetAvInterval()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetAvInterval(
    const int av_interval
)
{
    m_av_interval = av_interval;
}


//********************************Method Header********************************
//Method Name   :SetAvSpdrMapDelay()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CPqm::SetAvSpdrMapDelay(
    const int av_spdr_map_delay
)
{
    m_av_spdr_map_delay = av_spdr_map_delay;
}

//****************************Method Header************************************
//Method Name   :SetMrsLicense()
//Author        :PATNI / SS
//Purpose       :
//*****************************************************************************
void CPqm::SetMrsLicense(const int f_mrs_license
                        )
{
    m_mrs_license = f_mrs_license;
}

//*****************************************************************************
//Method Name   : SetStudyDetails
//Author        : Patni/PJS
//Purpose       : This function is wrapper for SetStudyDetails of
//                CPqmAuditLog class
//*****************************************************************************
void CPqm::SetStudyDetails(
    const CString& f_patient_id, const CString& f_study_id
)
{
    m_pqm_audit_log.SetStudyDetails(f_patient_id, f_study_id);
}
//-Patni-PJS/2010May09/Added/JaFT# IR-141

//+Patni-PJS/2009Oct29/Added/IR-92
//************************************Method Header************************************
// Method Name  : SetSameStudyFlag
// Author       : PATNI/ PJS
// Purpose      : Set m_same_study member variable of the class.
//***********************************************************************************
void CPqm::SetIsSameStudy(
    const bool f_same_study
)
{
    m_is_same_study = f_same_study;
}

//****************************Method Header************************************
//Method Name   : SetSelectedPosition()
//Author        : PATNI/ MRP
//Purpose       : Dummy function which will initialize the List in the GUI
//*****************************************************************************
void CPqm::SetSelectedPosition(
    const int position
)
{
    m_selected_pos = position;
}

//****************************Method Header************************************
//Method Name   :SetSeriesOrder()
//Author        :PATNI/PJP
//Purpose       :This method Rearranges the information of Series
//*****************************************************************************
bool CPqm::SetSeriesOrder(
    BSTR bStr,
    const int dropindex
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetSeriesOrder");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_dbsa_study_mgr) {
        m_dbsa_study_mgr->SetSeriesOrder(bStr, dropindex);
        return TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_dbsa_study_mgr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Method Name   :SetAutoLocatorRunning()
//Author        :PATNI/PJS
//Purpose       :
//*****************************************************************************
void CPqm::SetAutoLocatorRunning(
    const BOOL f_cala_applnRunning
)
{
    m_autolocator_postproc->SetAutoLocatorRunning(f_cala_applnRunning);
}

//****************************Method Header************************************
//Method Name   :IsCalaApplnRunning()
//Author        :PATNI/PJS
//Purpose       :
//*****************************************************************************
BOOL CPqm::IsCalaApplnRunning(
)const
{
    return m_cala_appln_running;
}




//****************************Method Header************************************
//Method Name   :GetCALARefParentInfo()
//Author        :PATNI/PJS
//Purpose       :
//*****************************************************************************
void CPqm::GetRefParentInfo(CPqmProtocol* f_protocol,
                            CPqmPrMode* f_parent_info
                           )
{
    m_autolocator_parametersetter->GetRefParentInfo(f_protocol, f_parent_info);
}

//****************************Method Header************************************
//Method Name   :GetAutoLocatorLicense()
//Author        :PATNI/PJS
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetAutoLocatorLicense(const AUTOLOCATOR_TYPE f_autolocator_type
                                )const
{
    if (AUTOLOCATOR_CARDIAC == f_autolocator_type) {
        return m_CALA_license;

    } else if (AUTOLOCATOR_NEURO == f_autolocator_type) {
        return m_HALA_license;

    } else {
        return FALSE;
    }
}
//****************************Method Header************************************
//Method Name   :SetCardiacParameters()
//Author        :PATNI/PJS
//Purpose       :
//*****************************************************************************
BOOL CPqm::SetAutoLocatorOffsetParameters(
    CPqmProtocol* f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::SetCardiacParameters");
    return m_autolocator_parametersetter->SetOffsetParameters(f_protocol);
}

//****************************Method Header************************************
//Method Name   :GetSARControlLicense()
//Author        :iGATE/Pavan
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetSARControlLicense() const
{
    return m_sarcontrol_license;
}
//+Remine-2187
//********************************Method Header********************************
//Method Name   :GetMulitScanDynamic_License()
//Author        :iGATEPatni/Abhishek
//Purpose       :Remine-2187 Returns the Dyanmic MultiScan license
//*****************************************************************************
bool_t CPqm::GetMulitScanDynamic_License()const
{
    return m_muliscan_dyn_license;
}
//-Remine-2187

BOOL CPqm::GetScanMutex(
    VARIANT** pData
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetScanMutex");
    PQM_SCOPED_TRACE();

    if (pData && *pData) {
        (*pData)->byref = (void*) m_pqm_study->GetScanMutex();
        return TRUE;
    }

    return FALSE;
}
//****************************Method Header************************************
//Method Name   : GetStudyLock()
//Author        : iGATE
//Purpose       :
//*****************************************************************************
BOOL CPqm::GetStudyLock(
    VARIANT** pData
)
{
    LPCTSTR FUNC_NAME = _T("CPqm::GetStudyLock");
    PQM_SCOPED_TRACE();

    if (pData && *pData) {
        (*pData)->byref = (void*) m_pqm_study->GetStudyLock();
        return TRUE;
    }

    return FALSE;
}

BOOL CPqm::IsValidSequenceToInvokeAutoLocator(const int f_acq_order, const AUTOLOCATOR_TYPE f_autolocator_type) const
{
    return 	m_autolocator_postproc->IsValidSequenceToInvokeAutoLocator(f_acq_order, f_autolocator_type);
}
bool CPqm::CheckForAutoLocator(CPqmProtocol* f_protocol, const bool f_Autolocator_after_acquisition)
{
    return m_autolocator_postproc->HandleAutoLocatorInvocation(f_protocol, f_Autolocator_after_acquisition);
}
BOOL CPqm::CheckAutoLocatorScanConditions(CPqmProtocol* f_protocol)
{
    return m_autolocator_postproc->CheckALProtocolScanConditions(f_protocol);
}
BOOL CPqm::IsAutoLocatorRunning()
{
    return m_autolocator_postproc->IsAutoLocatorRunning();
}
CDefaultPGUIDXmlDataManager* CPqm::GetDefaultPGUIDDataManager()
{
    return m_defpguid_xml_data ;
}
BOOL CPqm::IsPGUIDDataXmlFilePresent()
{
    return m_is_pguidxml_file;
}

//************************************Method Header************************************
// Method Name  : GetAMBStatusData
// Author       : iGATE
// Purpose      : Returns the AMB status data structure
//***********************************************************************************
BOOL CPqm::GetAMBStatusData(int* f_selected_items, int count, VARIANT**  data)const
{
    return m_pamb->CheckAmbInitialPlan(f_selected_items, count);
}
//************************************Method Header************************************
// Method Name  : GetAMBPlan
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPqm::GetAMBPlan(VARIANT**  data)const
{
    if (data) {

        amb_t* l_amb = (amb_t*)(*data)->byref;

        if (l_amb)
            m_pamb->pqmGetAMBPlan(l_amb, 0);

        ASSERT(FALSE) ;

    }
}
//************************************Method Header************************************
// Method Name  : AMBScanStart
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPqm::AMBScanStart(VARIANT**  data)const
{
    if (data) {
        amb_t* l_amb = (amb_t*)(*data)->byref;

        if (l_amb) {
            m_pamb->pqmAMBScanstart(l_amb);
        }
    }
}
//************************************Method Header************************************
// Method Name  : GetAMBLicense
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPqm::GetAMBLicense()const
{

    return m_amb_licence;
}


void CPqm::SetIsAMBPlanning(const bool f_is_amb_planning)
{
    m_is_amb_planning = f_is_amb_planning;
}
bool CPqm::GetIsAMBPlanning()const
{
    return m_is_amb_planning;
}
//************************************MethodHeader************************************
// Method Name  : UpdateSARInfo()
// Author       : PATNI/
// Purpose      :
//***********************************************************************************
BOOL CPqm::UpdateSARInfo()
{
    if (NULL != m_pqm_study) {
        m_pqm_study ->UpdateSARInfo();
    }

    return TRUE;
}

//************************************MethodHeader************************************
// Method Name  : IsSARControlLimitFileExist()
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPqm::IsSARControlLimitFileExist()
{
    LPCTSTR FUNC_NAME = _T("CPqm::IsSARControlLimitFileExist");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    CString l_file_name(_T("C:\\tmp\\SarControlLimit100.txt"));
    WIN32_FIND_DATA l_findfiledata;
    HANDLE l_hfind = NULL ;
    l_hfind = FindFirstFile(l_file_name, &l_findfiledata);


    if (INVALID_HANDLE_VALUE == l_hfind || NULL == l_hfind) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarControlLimit100.txt file not present"));
        return FALSE;
    }

    FindClose(l_hfind);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SarControlLimit100.txt file present"));
    return TRUE;
}

//************************************MethodHeader************************************
// Method Name  : UpdateTIinStudy()
// Author       : iGATE
// Purpose      :
//***********************************************************************************

int CPqm::UpdateTIinStudy(int * f_prot_idx, float * f_ti_val)
{
	if (m_tiprep_updater) {
	return m_tiprep_updater->UpdateTIinStudy(f_prot_idx,f_ti_val);
}	else {
		return 1;
	}

}
int CPqm::GetGDCSeriesLOIDForProtocol(int protocol, VARIANT *seriesloid)
{
	if (NULL != m_pqm_study) {
        m_pqm_study ->GetGDCSeriesLOIDForProtocol(protocol,seriesloid);
    }
	return 0;
}

BOOL CPqm::GetTiPrepLicense( const TIPREP_TYPE f_tiprep_type ) const
{
    if (TIPREP_SCANNER == f_tiprep_type) {
        return m_Tiprep_license;        
    } else {
        return FALSE;
    }

}

void CPqm::CheckForTiPrepLicense()
{
    #include "PDataParser/INIReader.h"
    CString l_ini_file_name(_T(""));
    l_ini_file_name = getenv("MRMP_Config");
    l_ini_file_name += _T("\\PQM");
    l_ini_file_name += _T("\\PQM_TiPrep_License.ini");
    
    if(PathFileExists(l_ini_file_name)) {
        CIniReader l_ini_file_name(l_ini_file_name);
        CString l_cspwdRead = l_ini_file_name.GetKeyValue(_T("TiPrep_License"), _T("TiPrep"));
        
        if(l_cspwdRead.CompareNoCase(_T("ON")) == 0) {
            m_Tiprep_license = TRUE;
        } else {
            m_Tiprep_license = FALSE;
        }
    }else{
        m_Tiprep_license = FALSE;
    }        

}

bool CPqm::IsProtocolforTiPrep( int f_acq_order )
{
    if (NULL != m_pqm_study) {
        return(m_pqm_study ->IsProtocolForTiPrep(f_acq_order));
    }
    return false;
}

BOOL CPqm::CanTiPrepEnable()
{
    //if Locator is running
    if(IsAutoLocatorRunning()){
        return(FALSE);
    }

    // if CALA is running
    if(IsCalaApplnRunning()){
        return(FALSE);
    }

    //if TiPrep License is not present
    if(TRUE != GetTiPrepLicense(TIPREP_SCANNER)){
        return(FALSE);
    }
    
    return(TRUE);
}

