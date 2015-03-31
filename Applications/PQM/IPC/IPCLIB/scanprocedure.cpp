//REDMINE-1213_Update_03_MAY
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
 *  File Name: scanprocedure.cpp
 *  Overview: Implementation of CScanProcedure class.
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
//This class is responsible  for handling the scanning procedure. It will contain a
//vector of all the canTransactions. This class will take the decision about the
//next transaction that should be sent to Acqman depending on the parameter of the
// series as well as the return information got from Acqman

#include "StdAfx.h"
#include "scanprocedure.h"
#include <math.h>
#include <atlbase.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <ErrorDisp/ErrorDisplay.h>
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMIPCManager.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "pqm.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "PreScanAMB.h"
#include "PreScanAPC.h"
#include "PreScanCFARetry.h"
#include "PreScanCoil.h"
#include "PreScanFirst.h"
#include "PreScanModeAll.h"
#include "PreScanRFL.h"
#include "PreScanCFA.h"
#include "PreScanPHCOR.h"
#include "AcqInfo.h"
#include "Interlock.h"
#include "Scan.h"
#include "PQMXMLParser.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "coildb.h"
#include "PQMStudyMgr.h"
#include "pqmpmacqman.h"
#include "PqmProbe.h"
#include "PqmPrMode.h"
#include "GateMode.h"
#include "ShimCtrl.h"
#include "Electrode.h"	//ASHISH SM3
#include "MoveCouch.h"
#include "voicemanager.h"
#include "CoilSet.h"
#include "PqmSectionData.h"
#include "PqmCoilData.h"
#include "couchconfig.h"
#include "PqmSpeeder.h"

//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
#include "CDSUtilities.h"
#include "CDSPrescan.h"
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "SAEInfo.h"

#include "OlpTemp.h"		//+Patni-Ravindra Acharya/2010June24/Added/IR-153
#include "ShimCommon.h"
#include "pqmAMB.h"
#include "CoilChannelMode.h"

using namespace MPlus::EH;
using namespace MR::ACQ::SM::COMMON;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
CScanProcedure* CScanProcedure::m_self_object = NULL;
//-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   :AcquisitionScanPause()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcquisitionScanPause(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcquisitionScanPause");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcquisitionScanPause"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    CScanTransaction* l_curr_scan_transaction = m_acq_manager->GetLastTransaction();

    if (NULL == l_curr_scan_transaction) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_scan_transaction is NULL"));
        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        return;
    }

    if (!l_curr_scan_transaction->SendControl(ACQ_PAUSE_BUTTON)) {

        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
        //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot Connect to AcqMan"));

        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        //  ResetCurrentRequest();
        return;
    }
}

//****************************Method Header************************************
//Method Name   :AcquisitionScanResume()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcquisitionScanResume(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcquisitionScanResume");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcquisitionScanResume"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    CPqm* pqm = m_acq_manager->GetPqm();
    //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - Code Review
    CVoiceManager* voice_manager = CVoiceManager::GetInstance();
    CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();

    //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
    if (NULL == voice_manager) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("voice_manager pointer is NULL"));
        return;
    }

    //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941

    /* AUTO_VOICE */
    if (m_pqm_protocol->GetPlnAutoVoice()) {

        voice_manager->CmdDelay(pqm->GetAvInterval(),
                                GetScanProcProtocol()->GetPlnAutoVoice(),
                                TRUE);
        SetWaitForVoice(true);

        if (GetAvTp1Ctrl() && GetAvTp1Margin() >= 0) {
            //Patni-Dhanesh/2009Nov04/MOdified/MVC004746
            voice_manager->CmdTrap(voice_manager->GetAvTp1Delay(),
                                   GetAvTp1Duration(),
                                   PQM_AV_VOICE_5,
                                   GetScanProcProtocol()->GetPlnAutoVoice());
        }

    } else if (GetScanProcProtocol()->GetExeAutoVoice() & AUTO_VOICE_VALUE) {
        voice_manager->CmdDelay(pqm->GetAvInterval(),
                                (GetScanProcProtocol()->GetExeAutoVoice() & AUTO_VOICE_VALUE),
                                TRUE);
        SetWaitForVoice(true);
    }

    CScanTransaction* l_curr_scan_transaction = m_acq_manager->GetLastTransaction();

    if (NULL == l_curr_scan_transaction) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_scan_transaction is NULL"));
        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        return;
    }

    if (!l_curr_scan_transaction->SendControl(ACQ_RESUME_BUTTON)) {

        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
        //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

        //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot Connect to AcqMan"));

        l_study->SetScanModeOfHead();
        //      ResetCurrentRequest();
        //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
        return;
    }

    //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - Code Review
}

//***************************Method Header*************************************
//Method Name    :ResetCurrentRequest()
//Author         :PATNI/MRP
//Purpose        :This function resets all the values stored in the scan procedure
//*****************************************************************************
void CScanProcedure::ResetCurrentRequest(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::ResetCurrentRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ResetCurrentRequest"));

    //+Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
    CPqmPmAcqman* acqman = GetAcqManager();

    if (NULL == acqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmPmAcqman object is NULL"));
        return;
    }

    CPqm* pqm = acqman->GetPqm();

    if (NULL == pqm) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqm object is NULL"));
        return;
    }

    //-Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display

    //if ((!IsSingleProcRunning()) && (m_scanning == false)) {
    //    return;
    //}

    if (-1 != acqman->GetWaitingForAutoScanStartAcqOrder() && PQM_PROTOCOL_SCAN == m_scan_mode) {
        acqman->AutoScanTimerReset();
    }

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    for (int i = 0; i < NUM_3; i++) {
        if (!(GetScanProc(i).IsEmpty())) {
            GetScanProc(i).Empty();
        }
    }

    m_scanning = false;
    m_breath_hold = FALSE;
    m_cfa_retry = false;
    m_cfa_clinical = false;
    m_apc_retry = false;

    //Patni-Hemant/2011Jan11/Commented/REDMINE-1095_Reopen
    //We should not reset it, its already set as per GUI
    //m_apc_hold = 0;

    m_probing = 0;


    m_is_gate_executing = false;
    m_is_coil_or_gateinfo_executing = false;



    m_prot = -1;
    m_prescan = 0;
    m_acq_info_retry = 0;
    m_scan_start = PQM_SCAN_START; //Patni-HAR/2010Jan06/Added/Defect#6629

    pqm->SetSensitive(SCAN_START_BTN, true);
    pqm->SetSensitive(SCAN_ABORT_BTN, false);
    pqm->SetSensitive(SCAN_PAUSE_BTN, false);

    m_prev_action->clear();
    m_current_action->clear();

    pqm->SetStartLabelForAPC(PQM_START_BTN_NORMAL);
    pqm->SetPauseButtonLabel(PQM_PAUSE_BTN);
    //Patni-HAR/2010Sep2/Modified/TMSC-REDMINE-685
    SetEnablePrepVoice(FALSE);

    if (GetProcessing() > 0) {
        SetProcessing(0);
    }

    if (GetRequest() > 0) {
        SetRequest(0);
    }

    // + SHIMMING Lokesh 15 March 2008
    CPqmShimSharedData::GetInstance()->ShimRegistProtocol(-1);
    // - SHIMMING Lokesh 15 March 2008

    // Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
    CPqmProtocol* l_delay_start_object = m_acq_manager->GetDelayStartObj();


    if ((l_delay_start_object != NULL) && ((acqman->CanAutoScanStart(l_delay_start_object->GetProtocol())))) {

        m_acq_manager->CheckDelayAndScanStartForFirstWaitProtocol(l_delay_start_object);

    } else {

        //Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CPqmProtocol object is NULL"));
        return;
    }

    // Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
}

//****************************Method Header************************************
//Method Name   :AcquisitionGateInfo()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcquisitionGateInfo(
    const int f_mode
)
{
    ReadScanProc();
    //Patni-PJS/2010Nov10/Modified/TMSC-REDMINE-950
    CAcqInfo* l_acq_info = new CAcqInfo(this);

    AddTransaction(l_acq_info);

    l_acq_info->SetAcqInfoMode(f_mode);

    if (l_acq_info->PrepareRequest()) {

        SetIsCoilOrGateInfoExecuting(true);
    }
}

//**************************************************************************
//Method Name   : PQMAcqSearchShim
//Author        : PATNI/DKH
//Purpose       : To Search previous Shim value to be used.
//**************************************************************************
void CScanProcedure::PQMAcqSearchShim(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_curpqmprotocol, const mode_autoshim shim_mode , const float target_couch_pos)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::PQMAcqSearchShim");

    //Patni-KSS/2011Feb21/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);

    if (NULL == f_curpqmprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function PQMAcqSearchShim"));
        return;
    }

    CPqmProtocol*	l_pqmprotocol;
    l_pqmprotocol = f_curpqmprotocol;
    f_curpqmprotocol->SetShimProt(-1);

    CPqm* l_pqmptr = m_acq_manager->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmptr is NULL"));
        return;
    }

    CPQMStudy* l_pqmstudy = l_pqmptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy is NULL"));
        return;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (NULL == l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return;
    }

    int l_rflLimit = 0, l_tuneLimit = 0;

    if (l_pqmptr->IsSaturnCoilSystem()) {

        if (l_coil_data->PqmGetRflTuneInSaturn(f_curpqmprotocol->GetCoilDBTable(),
                                               &l_rflLimit,
                                               &l_tuneLimit) != E_NO_ERROR) {

            l_rflLimit = GLOBAL_RFL_LIMIT;
            l_tuneLimit = GLOBAL_TUNE_LIMIT;
        }

    } else {
        CCoil l_coil;
        CPqmPrMode* l_pqm_pr_mode = f_curpqmprotocol->GetPqmPrMode();

        if (NULL == l_pqm_pr_mode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pr_mode is NULL"));
            return;
        }

        l_coil_data->PqmCoilGetCoil(l_pqm_pr_mode ->GetRecvCoilId(), l_coil);

        if (l_coil.GetCoil()) {

            l_rflLimit = l_coil.GetRFLLimit();
            l_tuneLimit = l_coil.GetTuneLimit();

        } else {

            l_rflLimit = GLOBAL_RFL_LIMIT;
            l_tuneLimit = GLOBAL_TUNE_LIMIT;
        }
    }

    while (1) {
        //Patni-AMT/2010Feb26/Modified/Ph-3# PROPOSAL_07
        l_pqmprotocol = l_pqmstudy->GetPrevItemFromPos(l_pqmprotocol);

        if (NULL == l_pqmprotocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmprotocol is NULL"));
            break;
        }

        if (l_pqmprotocol->GetAcqStatus() != C_PR_STATUS_DONE) {
            continue;
        }

        CPqmPrMode* l_pqm_pr_mode = l_pqmprotocol->GetPqmPrMode();
        CPqmPrMode* l_curpqm_pr_mode = f_curpqmprotocol->GetPqmPrMode();

        if (NULL == l_pqm_pr_mode || NULL == l_curpqm_pr_mode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pr_mode or l_curpqm_pr_mode is NULL"));
            break;
        }

        if (l_pqm_pr_mode->GetScanNumber() != l_curpqm_pr_mode->GetScanNumber()) {
            break;
        }

        if (l_pqmptr->IsSaturnCoilSystem() &&
            (l_coil_data->PqmIsSameCoilInSaturn(f_curpqmprotocol->GetSaturnCoil(),
                                                l_pqmprotocol->GetSaturnCoil(),
                                                f_curpqmprotocol->GetCoilDBTable(),
                                                l_pqmprotocol->GetCoilDBTable()) == FALSE)) {
            break;

        } else if (!l_pqmptr->IsSaturnCoilSystem() &&
                   !IS_COIL_SAME(l_curpqm_pr_mode->GetRecvCoilId(), l_pqm_pr_mode->GetRecvCoilId())) {
            break;
        }

        //+Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-227
        //        if (l_pqmprotocol->GetShimming() != VFC_SHIM_STANDARD) {
        //            continue;
        //        }
        //-Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-227

        //Patni/AM/2010July20/Added/NFD001-AutoShim
        if (shim_mode == AUTOSHIM_MODE) {
            if (ABSF(target_couch_pos - l_pqm_pr_mode->GetCouchPos()) > (float) l_rflLimit) {
                continue;
            }

        } else if (ABSF(l_curpqm_pr_mode->GetCouchPos() - l_pqm_pr_mode->GetCouchPos()) > (float) l_rflLimit) {
            continue;
        }

        //-Patni/AM/2010July20/Added/NFD001-AutoShim

        //original--
        //  if (ABSF(l_curpqm_pr_mode->GetCouchPos() - l_pqm_pr_mode->GetCouchPos()) > (float) l_rflLimit) {
        //     continue;
        //  }

        //+Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-227
        if (l_pqmprotocol->GetShimming() == VFC_SHIM_STANDARD) {

            //protocol is shimm so CFA and Shim protocol are same
            f_curpqmprotocol->SetShimProt(l_pqmprotocol->GetProtocol());
            f_curpqmprotocol->SetCfaProt(l_pqmprotocol->GetProtocol());
            break ;

        } else {


            CString l_logstr(_T(""));
            //protocol is not shim
            //need to check weather it is having shim enabled or not
            //if shim is enable, need to check shim protocol for this match
            //if that shim protocol also in range than we have found the
            //perfect match
            const int l_cfa_prot_match = l_pqmprotocol->GetProtocol();

            l_logstr.Format(_T("l_cfa_prot_match = %d is found for CFA pos match") , l_cfa_prot_match);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_logstr);

            if (!l_pqmstudy->GetShimEnableFlagFromVF(l_cfa_prot_match)) {

                l_logstr.Format(_T("l_cfa_prot_match = %d does not have shim enable ") , l_cfa_prot_match);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_logstr);

                continue ;
            }

            //l_pqmprotocol match is having shim enabled

            VfPathElem_t	l_activeshimpath[2] = {0};

            if (!l_pqmstudy->GetShimPathFromVF(l_cfa_prot_match, l_activeshimpath)) {

                l_logstr = _T("GetShimPathFromVF Failed");
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_logstr);

                continue ;
            }

            const int l_shim_protocol_match = l_activeshimpath[0].index;

            //if l_shim_protocol_match protocol is VFC_SHIM_STANDARD and if it is also in range
            //for CFA then consider  l_shim_protocol_match for shim protocol

            CPqmProtocol* l_shim_protocol = l_pqmstudy->GetProtocol(l_shim_protocol_match);

            if (!l_shim_protocol) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_shim_protocol is NULL!!!"));

                continue ;
            }

            flt32_t l_current_couch_pos = 0.0f;

            if (AUTOSHIM_MODE == shim_mode) {
                l_current_couch_pos = ABSF(target_couch_pos - l_shim_protocol->GetPqmPrMode()->GetCouchPos());

            } else {
                l_current_couch_pos = ABSF(l_curpqm_pr_mode->GetCouchPos() - l_shim_protocol->GetPqmPrMode()->GetCouchPos());
            }



            if ((l_shim_protocol->GetShimming() != VFC_SHIM_STANDARD)
                || (l_current_couch_pos > (float) l_rflLimit)) {

                l_logstr.Format(_T("l_shim_protocol = %d is not in range") , l_shim_protocol);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_logstr);

                continue ;
            }

            l_logstr = _T("Reuse shim match found");
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_logstr);

            l_logstr.Format(_T("l_cfa_prot_match = %d, l_shim_protocol = %d, ") , l_cfa_prot_match, l_shim_protocol);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_logstr);

            f_curpqmprotocol->SetShimProt(l_shim_protocol_match);
            f_curpqmprotocol->SetCfaProt(l_cfa_prot_match);
        }

        //-Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-227


        //+Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-227
        //        f_curpqmprotocol->SetShimProt(l_pqmprotocol->GetProtocol());
        //        f_curpqmprotocol->SetCfaProt(l_pqmprotocol->GetProtocol());

        break ;
        //-Patni-Hemant/2010May27/Added/MaFT/TMSC-REDMINE-227
    }

    if (f_curpqmprotocol->GetShimProt() != -1) {

        //+Patni-Hemant/2010May27/Modified/MaFT/TMSC-REDMINE-227
        //path[0].index = f_curpqmprotocol->GetCfaProt();
        //-Patni-Hemant/2010May27/Modified/MaFT/TMSC-REDMINE-227

        // MEITEC/2010Apr26/Added/IR-86
        CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("SHIM CFA DATA COPY fromProt[%d] toProt=[%d]")
                , f_curpqmprotocol->GetCfaProt(),
                f_curpqmprotocol->GetProtocol());

        int n = 0;
        VfFieldSpec_t       vfReq[1];
        VFF_SET_ARGS(vfReq[n], SVN_EXE_SHIM_AFTER_FSY_VALUE, SVT_EXE_SHIM_AFTER_FSY_VALUE,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        n++;

        CVarFieldHandle* const vf_handle = l_pqmstudy->GetVfDBHandle();
        int numDone = 0;
        VfPathElem_t        path[1];
        path[0].subtree_name = SVN_ST_PROTOCOL;
        path[0].index = f_curpqmprotocol->GetShimProt();
        vf_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, n, (int*) &numDone);

        if (n != numDone) {
            f_curpqmprotocol->SetCfaProt(-1);
            f_curpqmprotocol->SetShimProt(-1);

            // MEITEC/2010Apr26/Added/IR-86
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("SHIM CFA DATA COPY ERROR!!!"));
        }

        n = 0;

        VfFieldSpec_t       vfReq1[1];

        if (vfReq[0].status == E_NO_ERROR) {
            VFF_SET_ARGS(vfReq1[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE,
                         vfReq[0].size, vfReq[0].buffer, VFO_REPLACE);
            n++;
        }

        if (n) {
            time_t              ts;
            VfPathElem_t        path1[1];
            path1[0].subtree_name = SVN_ST_PROTOCOL;
            path1[0].index = f_curpqmprotocol->GetProtocol();
            status_t l_status = vf_handle->PutFields(path1, SVD_ST_PROTOCOL, vfReq1, n, (int*)&numDone, &ts);

            if (E_NO_ERROR != l_status || numDone != n) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            }
        }

        if (n != numDone) {
            f_curpqmprotocol->SetCfaProt(-1);
            f_curpqmprotocol->SetShimProt(-1);

            // MEITEC/2010Apr26/Added/IR-86
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("SHIM CFA DATA COPY ERROR!!!"));
        }

        if (vfReq[0].status == E_NO_ERROR && vfReq[n].size > 0)
            FREE_PTR(vfReq[0].buffer) ;
    }

    return;
}

//+Patni-HAR/2010Feb10/Modified/CDS Requirement
//*******************************Method Header*********************************
// Method Name  : pqmAcqCdsAbort
// Author       : PATNI/ CdsAbort
// Purpose      : Aborts CDS Scan, During scanning process abort CDS
//*****************************************************************************
void CScanProcedure::pqmAcqCdsAbort(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* f_current_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::pqmAcqCdsAbort");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CScanProcedure::pqmAcqCdsAbort"));

    CPQMStudy* l_study_obj = m_pqm_ptr->GetStudy();
    CpqmCDS* l_pqm_cds = m_pqm_ptr->GetCDS();
    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    if (!l_study_obj || !l_pqm_cds || !l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_study_obj or l_pqm_cds or l_coil_data pointer is NULL"));
        return ;
    }

    f_current_protocol->GetCDSInfo()->m_cds_step	= CDS_STEP0_NONE;
    f_current_protocol->GetCDSInfo()->m_cds_rescan	= TRUE;
    AcquisitionScanAbort();
    l_study_obj->SetAcqStatus(f_current_protocol, C_PR_STATUS_WAIT);

    //Information back to the front coil CDS
    l_pqm_cds->PqmCdsReturnCoil(f_current_protocol->GetSaturnCoil());

    l_study_obj->PutSaturnCoil(f_vf_handle,
                               f_current_protocol->GetProtocol(),
                               f_current_protocol->GetSaturnCoil());

    if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_current_protocol->GetSaturnCoil(),
            f_current_protocol->GetCoilDBTable())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
    }

    l_study_obj->PutConnectedCoil(f_vf_handle,
                                  f_current_protocol->GetProtocol(),
                                  f_current_protocol);

    l_study_obj->PutRecvrCoil(f_vf_handle,
                              f_current_protocol->GetProtocol(),
                              f_current_protocol);

    //First let the protocol  update the coil data..
    //Now this will update the GUI ...
    //This will again check for batch setting and then apply accordingly.
    //This will also consider the current protocol under scan
    m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

    m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

    return;
}
//-Patni-HAR/2010Feb10/Modified/CDS Requirement

//****************************Method Header************************************
//Method Name   :AcquisitionScanAbort()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcquisitionScanAbort(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcquisitionScanAbort");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcquisitionScanAbort"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    //+ Patni-PJS/2009Aug14/Modify/MVC002490 Unit Testing
    CPqmProtocol* protocol = GetScanProcProtocol();

    if (protocol) {
        if (protocol->GetPrepScan() ||
            (protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP)) {
            m_acq_manager->GetPqm()->WriteToUI(PQM_MSG_INJECT_TIME_INVISIBLE);
            //+Patni-PJS/2010Sep2/Modified/TMSC-REDMINE-685
            SetWaitForPrepScan(FALSE);
            m_pqm_ptr->WriteToUI(PQM_MSG_SCAN_STARTPAUSE_DISABLE);
            //-Patni-PJS/2010Sep2/Modified/TMSC-REDMINE-685

        }
    }

    if (m_is_waiting_for_scan_sarctrl) {
        WaitTimeTimerReset(protocol);
    }

    //- Patni-PJS/2009Aug14/Modify/MVC002490 Unit Testing
    CScanTransaction* l_curr_scan_transaction = m_acq_manager->GetLastTransaction();

    if (NULL == l_curr_scan_transaction) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_scan_transaction is NULL"));
        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        return;
    }

    if (!l_curr_scan_transaction->SendControl(ACQ_ABORT_BUTTON)) {

        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
        //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        //      ResetCurrentRequest();
        return;
    }
}

//********************************Method Header********************************
//Method Name   : CheckNextProtocol()
//Author        : PATNI/MRP
//Purpose       : Checks the next protocol for Auto Scan
//*****************************************************************************
//+Patni-HAR-RG/2009June09/Modified/JFT#50
void CScanProcedure::CheckNextProtocol(
    const int f_protcol_no,
    const BOOL f_call_frm_gui
)const
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::CheckNextProtocol");
    CString l_print_string(_T(""));
    l_print_string.Format(_T("Flag = %d"), f_call_frm_gui);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (GetIsScanning()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Occured...It is already Scanning."));
        return;
    }

    if (GetProcessing()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetProcessing() is NOT Zero"));
        m_pqm_ptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);

        return;
    }

    CPqmProtocol*	l_prot_obj = NULL;
    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (f_protcol_no <= 0) {
        //Get the topmost protocol
        l_prot_obj = l_study->GetTopWaitProtocol();

    } else {
        //CPqmProtocol* l_current_protocol = l_study->GetProtocol(f_protcol_no);

        //l_prot_obj = l_study->GetNextItemFromPos(l_current_protocol->GetPosition());
        //l_prot_obj = l_study->GetNextItemFromPos(f_protcol_no + 1);
        CPqmProtocol* l_current_protocol = l_study->GetProtocol(f_protcol_no);
        l_prot_obj = l_study->GetNextWaitProtocol(l_current_protocol);
        //coz 0 based is got
    }

    //-Patni-HAR-RG/2009June09/Modified/JFT#50

    if (l_prot_obj != NULL &&
        l_prot_obj->GetAcqStatus() == C_PR_STATUS_WAIT &&
        !(l_prot_obj->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {

        //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
        CPQMIPCManager* l_ppqmipcmanager = m_pqm_ptr->GetManagerPtr();

        if (NULL != l_ppqmipcmanager) {
            if (l_ppqmipcmanager->RemoteCheckActiveFlag() && !m_pqm_ptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
                l_prot_obj->SetHoldScan(VFC_HOLD_PRE_SCAN);
                int l_hold_scan = l_prot_obj->GetHoldScan();
                l_study->SetProtocolValue(l_study->GetVfDBHandle(), l_prot_obj, K_PR_HOLD, &l_hold_scan,
                                          sizeof(int32_t), TRUE, NULL);
                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                        CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE")),
                        SCAN_ERR_NOTIFY, L"CScanProcedure.cpp");
                m_pqm_ptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
                return;
            }
        }

        //-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole

        /* ミラーリング時は次の撮像は行わない */
        /*if(PqmUiPCMirrorChk() && !PqmPmIsActive(K_PM_TYPE_AMB)){
            prObj->holdScan |= VFC_HOLD_PRE_SCAN;
        	PqmPqSetProtValue(PqmGlobal.pqmStudy.vfHandle,prObj,K_PR_HOLD,
        			  &prObj->holdScan,sizeof(int32_t),
        			  TRUE,PQM_STS_LIST);
            PQM_SET_WIDGET_STATUS(prObj->pos);
            PqmListSelectPos(PqmPqGetPosFromProt(prObj->prot),FALSE,0);
            PqmUiErrorDialog(PqmUiFetchLiteral("ERR_PC_MIRROR"),ERR_ERROR);
            return;
        }
        */
        //+ Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212

        CPqmPmAcqman* l_acqman = m_pqm_ptr->GetAcqManager();

        if (GetRequest() && (!f_call_frm_gui)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Before SetDelayStartObj() & Set Call Flag as FALSE"));
            l_acqman->SetDelayStartObj(l_prot_obj);

        } else {   /*  add. V7.10  List Select   */
            //PqmListSelectPos(PqmPqGetPosFromProt(prObj->prot),FALSE,0);
            //PqmPqSetVisible(PqmPqGetPosFromProt(prObj->prot));

            if (!l_acqman->CanAutoScanStart(l_prot_obj->GetProtocol())) {
                m_pqm_ptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);

            } else {
                m_acq_manager->CheckDelayAndScanStartForFirstWaitProtocol(l_prot_obj);
            }
        }

        //- Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
    }
}


//+Patni-HAR/2010Apr13/Added/Phase#3 CDS Requirement
//*******************************Method Header*********************************
// Method Name  : pqmAcqCDSContinueCB
// Author       : PATNI/ HEMANT
// Purpose      :
//*****************************************************************************
void CScanProcedure::SendAcqInfoForCouch(
)
{
    CAcqInfo* acqinfo = new CAcqInfo(this);
    AddTransaction(acqinfo);

    acqinfo->SetAcqInfoMode(PQM_INFO_BIT_COUCH);
    acqinfo->PrepareRequest();
}
//-Patni-HAR/2010Apr13/Added/Phase#3 CDS Requirement

//+Patni-MP/2009Sep18/Added/MVC004774
//********************************Method Header********************************
//Method Name   : SendAcqInfoRetry()
//Author        : PATNI/MRP
//Purpose       :
//*****************************************************************************
void CScanProcedure::SendAcqInfoRetry(const int f_mode
                                     )
{
    CAcqInfo* acqinfo = new CAcqInfo(this);
    AddTransaction(acqinfo);
    acqinfo->SetAcqInfoMode(f_mode);
    acqinfo->PrepareRequest();
}
//-Patni-MP/2009Sep18/Added/MVC004774

//*******************************Method Header*********************************
//Method Name    :RetryCFA()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
bool CScanProcedure::RetryCFA(
    int f_scan_id,
    CPqmProtocol* f_protocol
)
{
    CPreScan* l_pre_scan = new CPrescanCFARetry(this);
    AddTransaction(l_pre_scan);

    if (!l_pre_scan->RetryCFA(f_scan_id, f_protocol)) {
        return false;
    }

    return true;
}

//*******************************Method Header*********************************
//Method Name    :StartRFLPrescan()
//Author         :PATNI/MSN
//Purpose        :
//*****************************************************************************
bool CScanProcedure::StartRFLPrescan(
    int f_scan_id
)
{
    CPreScan* l_pre_scan = new CPreScanAPC(this);
    AddTransaction(l_pre_scan);

    if (!l_pre_scan->StartRFLPrescan(f_scan_id)) {
        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :SendShimCtrl()
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
bool CScanProcedure::SendShimCtrl(
    PqmShim_t* f_shim
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendShimCtrl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SendShimCtrl"));


    CScanTransaction* shimctrl = new CShimCtrl(this);
    SetPqmShimInfo(f_shim);
    AddTransaction(shimctrl);
    return shimctrl->PrepareRequest();

}


//****************************Method Header************************************
//Method Name   :SendGateModeInfo()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::SendGateModeInfo(
    const int f_gate_method,
    Wireless_gating_t* const f_wirelessgateinfo,
    const bool f_gate_method_onscan /* = false*/
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendGateModeInfo");

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_protocol == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_protocol pointer is NULL"));
        return FALSE;
    }


    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CGateMode* l_gate_mode = new CGateMode(this);

    m_pqm_ptr->GetGateParamObj()->SetWirelessGateParam(f_wirelessgateinfo);


    CString l_str_values;
    l_str_values.Format(_T("Wireless param values to be sent to Acqman: Cmd = %d, Filter = %d, Mode = %d")
                        , f_wirelessgateinfo->m_command, f_wirelessgateinfo->m_wireless_filter, f_wirelessgateinfo->m_mode);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_values);


    AddTransaction(l_gate_mode);
    l_gate_mode->SetGateModeMethod(f_gate_method);
    l_gate_mode->SetGateModeMethodOnscan(f_gate_method_onscan);

    if (!l_gate_mode->PrepareRequest()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PrepareRequest() failed."));
        return FALSE;
    }

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :PQMManualPrescanClose
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CScanProcedure::PQMManualPrescanClose(
)
{

    CPreScan* l_pre_scan = (CPreScan*)m_curr_transaction;

    //CPreScan *l_pre_scan = (CPreScan *) m_acq_manager->GetLastTransaction();
    //+ UT_JIT@MANUAL PRESCAN CLOSE CRASH
    if (l_pre_scan == NULL) {
        return;

    } else {
        l_pre_scan->PQMManualPrescanClose();
    }

    //- UT_JIT@MANUAL PRESCAN CLOSE CRASH
}

//*******************************Method Header*********************************
//Method Name    :PQMManualPrescanApply
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CScanProcedure::PQMManualPrescanApply(
    VARIANT* f_pdata
)
{
    CPreScan* l_pre_scan = (CPreScan*)m_curr_transaction;

    if (l_pre_scan) {
        l_pre_scan->PQMManualPrescanApply(f_pdata);
    }
}

int CScanProcedure::SendElectrodeType(const int f_nelectrodetype)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendElectrodeType");
    CElectrode* l_electrode = new CElectrode(this);
    AddTransaction(l_electrode);
    l_electrode->SetElectrodeType(f_nelectrodetype);

    if (!l_electrode->PrepareRequest()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PrepareRequest() failed."));
        return FALSE;
    }

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    : SendAcqInfoForCoil
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
BOOL CScanProcedure::SendAcqInfoForCoil(
    CPqmProtocol* f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendAcqInfoForCoil");
    //Patni-AMT/2009Sep09/Added/MVC004983 -  Code Review
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME, _T("m_pqm_ptr is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (!(GetIsScanning() || IsSingleProcRunning() || m_pqm_ptr->GetPostProcessFlag()) && (m_pqm_ptr->GetMode() == K_IMAGING)) {
        if (f_protocol) {
            SetScanProcProtocol(f_protocol);
        }

        CAcqInfo* acqinfo = new CAcqInfo(this);

        if (NULL == acqinfo) {
            //+Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CAcqInfo object is NULL"));
            return FALSE;
            //-Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
        }

        AddTransaction(acqinfo);

        int l_mode = PQM_INFO_BIT_COILID;

        acqinfo->SetAcqInfoMode(l_mode);

        if (!m_pqm_ptr->GetGUICoilInformationDisplay()) {
            m_pqm_ptr->GetCoilSelectionTool()->SetCoilSelectionFlagActive(TRUE);
        }

        if (!acqinfo->PrepareRequest()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PrepareRequest() failed."));
            return FALSE;
        }

    } else {
        //Patni-AMT/2009Sep09/Added/MVC004983 -  Code Review
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Returning without sending AMCMM_ACQINFO"));
        return FALSE;
    }

    SetIsCoilOrGateInfoExecuting(true);
    return TRUE;
}

//***************************Method Header*************************************
//Method Name    :AddTransaction()
//Author         :PATNI/MRP
//Purpose        :This method adds the transaction to the vector.
//*****************************************************************************
bool CScanProcedure::AddTransaction(
    CScanTransaction* scan_transaction
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AddTransaction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == scan_transaction) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_transaction is NULL"));
        return false;
    }

    m_curr_transaction = scan_transaction;

    vector<CScanTransaction*>::iterator l_itr = NULL;

    for (l_itr = m_acqscan_transaction.begin(); l_itr != m_acqscan_transaction.end(); l_itr++) {
        if ((scan_transaction != NULL) && (scan_transaction == *l_itr)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_transaction is already present in m_acqscan_transaction"));
            return true;
        }
    }

    m_acqscan_transaction.push_back(scan_transaction);

    return true;
}

//****************************Method Header************************************
//Method Name   :AcqProbingScanAbort()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
bool CScanProcedure::AcqProbingScanAbort(
)
{
    CPreScan* l_pre_scan = new CPrescanCFARetry(this);
    AddTransaction(l_pre_scan);
    //+ Patni-MP/2009Mar11/Modified/Warning Removal
    const int l_sts = l_pre_scan->AcqProbingScanAbort();

    if (l_sts <= 0) {
        return false;

    } else {
        return true;
    }

    //- Patni-MP/2009Mar11/Modified/Warning Removal
}

//*******************************Method Header*********************************
//Method Name    :SendAcquisition()
//Author         :PATNI/MRP
//Purpose        :This method send the Scan transaction to acqman
//*****************************************************************************
bool CScanProcedure::SendAcquisition(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendAcquisition");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SendAcquisition"));

    CScanTransaction* scan = new CScan(this);

    AddTransaction(scan);

    if (!scan->PrepareRequest()) {
        return false;
    }

    GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_SCAN_ABORT_TRUE);

    return true;

}

//****************************Method Header************************************
//Method Name   :AcqProbingPreScanStart()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
bool CScanProcedure::AcqProbingPreScanStart(CPqmProtocol* f_curr_protocol
                                           )
{
    //+Patni-HAR/2010Jun02/Modified/TMSC-REDMINE-111
    CPreScan* l_pre_scan = new CPrescanCFARetry(this);
    AddTransaction(l_pre_scan);
    bool l_sts = false;

    if (f_curr_protocol) {
        l_sts = l_pre_scan->AcqProbingPreScanStart(f_curr_protocol->GetProtocol(), f_curr_protocol);
    }

    //-Patni-HAR/2010Jun02/Modified/TMSC-REDMINE-111
    return l_sts;
}

//*******************************Method Header*********************************
//Method Name    :APCStart()
//Author         :PATNI/MSN
//Purpose        :Starts RFL prescan after clicking the APC Start button displayed
//                during APC Pause
//*****************************************************************************
void CScanProcedure::APCStart(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::APCStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of APCStart"));

    CPqmProtocol* l_current_protocol = GetScanProcProtocol();

    if (l_current_protocol->GetExeAutoVoice() >> 4) {
        //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("waitForPrepChange = %d"), m_wait_for_prep_scan);

        if (m_wait_for_prep_scan) {

            CVoiceManager::GetInstance()->CmdPlay((l_current_protocol->GetExeAutoVoice() >> 4));
            SetWaitForPrepScan(FALSE);

        } else {
            CVoiceManager::GetInstance()->CmdPlay((l_current_protocol->GetExeAutoVoice() >> 4), TRUE);
        }
    }

    if (!m_acq_manager->GetLastTransaction()->SendControl(ACQ_START_BUTTON)) {
        return;
    }

    GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_APC_END);
}

//+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelectionClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CScanProcedure::CoilSelectionClosed(
    const BOOL f_ok_pressed
)
{
    (f_ok_pressed == TRUE) ? OnCoilSelectionOK() : OnCoilSelectionCancel() ;
}
//-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

//+Patni-MP/2009Mar28/Added/PSP Internal Defect
void CScanProcedure::SendMessagrToUIForEndWaitCursor()const
{
    if (m_pqm_ptr) {
        m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        //Patni-PJS/2010Jan19/Added/MVC006827
        m_pqm_ptr->WriteToUI(PQM_MSG_POST_PROCESS_END);
    }

    return;
}
//-Patni-MP/2009Mar28/Added/PSP Internal Defect

//****************************Method Header************************************
//Method Name   :WarningScan()
//Author        :PATNI/LK
//Purpose       :This method will pop up MPlus Error Dialog
//*****************************************************************************
bool CScanProcedure::WarningScan(
    const int f_scanwarning_code,
    const CString& f_warning_source,
    const int f_scanwarning_buttonstype,
    int f_scanwarning_level,
    const int f_scanwarning_dialogtype,
    const bool f_continue
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::WarningScan");

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(f_warning_source);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of WarningScan"));

    CString l_scanwarning(_T(""));
    f_scanwarning_level = SCAN_WARN_INFORMATION;

    switch (
        f_scanwarning_code
    ) {

        case PQM_WARN_SCAN_AUTO_VOICE:
            // l_scanwarning.LoadString(IDS_WARN_SCAN_AUTO_VOICE);  // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_AUTO_VOICE")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_PV_MOVINGBED_INTEN:
            //l_scanwarning.LoadString(IDS_WARN_SCAN_PV_MOVINGBED_INTEN); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PV_MOVINGBED_INTEN"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_PREP_STUDY:
            //l_scanwarning.LoadString(IDS_WARN_SCAN_PREP_STUDY); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PREP_STUDY"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_PREP_DELAY_SKIP:
            //l_scanwarning.LoadString(IDS_WARN_SCAN_PREP_DELAY_SKIP);// Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PREP_DELAY_SKIP"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_DRKS_RECON_MODE:
            // l_scanwarning.LoadString(IDS_WARN_SCAN_DRKS_RECON_MODE); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_DRKS_RECON_MODE"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_ACQ_ERROR_PVCOIL_FILE:
            //  l_scanwarning.LoadString(IDS_ERR_PVCOIL_FILE);		 // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_PVCOIL_FILE"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_WARN_SCAN_INTENSITY_COIL:
            //  l_scanwarning.LoadString(IDS_WARN_SCAN_INTENSITY_COIL);// Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_INTENSITY_COIL"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_INTENSITY_COUCH :
            //	l_scanwarning.LoadString(IDS_WARN_SCAN_INTENSITY_COUCH);// Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_INTENSITY_COUCH"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_INTENSITY_MAP:
            //	l_scanwarning.LoadString(IDS_WARN_SCAN_INTENSITY_MAP);// Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_INTENSITY_MAP"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_INTENSITY_AREA:
            //	l_scanwarning.LoadString(IDS_WARN_SCAN_INTENSITY_AREA);// Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_INTENSITY_AREA"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_WARN_SCAN_PV_MOVINGBED_SPEEDER:
            //	l_scanwarning.LoadString(IDS_WARN_SCAN_PV_MOVINGBED_SPEEDER);// Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PV_MOVINGBED_SPEEDER"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

            //+Patni-NP/2009Aug25/Added/MVC004384
        case PQM_WARN_SCAN_DEGENERACY_MAP:
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_DEGENERACY_MAP"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

            //-Patni-NP/2009Aug25/Added/MVC004384
        case PQM_WARN_TX_INTENSITY_NOTFOUND:
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_TX_INTENSITY_NOTFOUND"));
            break;

        case PQM_WARN_INTENSITY_NOTFOUND:
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_INTENSITY_NOTFOUND"));
            break;
            //-Patni-Sribanta/2011Mar07/Added/IR-184

        case PQM_WARN_SCAN_TX_DEGENERACY_MAP:
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_TX_DEGENERACY_MAP"));
            break;

        case PQM_WARN_SCAN_TX_INTENSITY_AREA:
            l_scanwarning = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_TX_INTENSITY_AREA"));
            break;

        default:

            l_scanwarning =  CPqmUtilities::GetMultiLingualString(_T("IDS_NO_MATCHING_WARNING_ID"));   	// Added By KT/ACE-2/15-06-2009/Multilingualisation
            CPQMLogMgr::GetInstance()->DisplayError(
                //_T("No Matching Warning ID was found"), // Added By KT/ACE-2/15-06-2009/Multilingualisation
                l_scanwarning,                           // Added By KT/ACE-2/15-06-2009/Multilingualisation
                _T("CScanProcedure.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                ERR_ID, APP_NAME);
            return false;
    }

    CString l_warnapp_name(_T(""));
    l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);
    MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error ;
    l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_scanwarning,
                 l_warnapp_name,
                 f_scanwarning_level,
                 f_scanwarning_dialogtype,
                 f_scanwarning_buttonstype
                                                                                                    );

    //+Patni-NP/2009Aug25/Added/MVC004384
    if (l_response == MPlusErrResponse_OK) {
        if (f_scanwarning_code == PQM_WARN_SCAN_PREP_DELAY_SKIP) {
            PrepDelayDelete();

        } else if (PQM_WARN_TX_INTENSITY_NOTFOUND == f_scanwarning_code ||
                   PQM_WARN_SCAN_TX_INTENSITY_AREA == f_scanwarning_code ||
                   PQM_WARN_SCAN_TX_DEGENERACY_MAP == f_scanwarning_code) {

            //+Patni-Sribanta/2011Mar07/Added/IR-184

            m_pqm_ptr->GetStudy()->UpdateIntensityCorrStatus(m_pqm_protocol->GetProtocol(), VFC_INTENSITY_CORR_STATUS_RX);
            m_pqm_ptr->GetStudy()->UpdateIntensityCorrMethod(m_pqm_protocol->GetProtocol(),  VFC_INTENSITY_CORR_METHOD_RX);

        } else if (PQM_WARN_INTENSITY_NOTFOUND == f_scanwarning_code ||
                   PQM_WARN_SCAN_INTENSITY_COIL == f_scanwarning_code ||
                   PQM_WARN_SCAN_INTENSITY_COUCH == f_scanwarning_code ||
                   PQM_WARN_SCAN_PV_MOVINGBED_INTEN == f_scanwarning_code ||
                   PQM_WARN_SCAN_INTENSITY_MAP ==  f_scanwarning_code ||
                   PQM_WARN_SCAN_INTENSITY_AREA == f_scanwarning_code ||
                   PQM_WARN_SCAN_DEGENERACY_MAP == f_scanwarning_code) {
            m_pqm_ptr->GetStudy()->UpdateIntensityCorrStatus(m_pqm_protocol->GetProtocol(), VFC_INTENSITY_CORR_STATUS_OFF);
            m_pqm_ptr->GetStudy()->UpdateIntensityCorrFlag(m_pqm_protocol->GetProtocol(), VFC_INTENSITY_CORR_OFF);
        }

        //-Patni-Sribanta/2011Mar07/Added/IR-184
        m_pqm_ptr->WriteToUI(PQM_MSG_SCAN_ABORT_TRUE);

        if (f_continue) {
            m_acq_manager->AutoScanRetry(m_pqm_protocol);
        }

        return true;

    } else {

        if (f_continue) {
            if ((C_PR_STATUS_CURRENT == m_pqm_protocol->GetAcqStatus()) && (m_pqm_protocol->GetCDSInfo()->m_cds_flag)) {
                pqmAcqCdsAbort(m_pqm_ptr->GetStudy()->GetVfDBHandle(), m_pqm_protocol);
            }

            m_acq_manager->AcqDeleteLockFile();
            m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
            ResetCurrentRequest();

            //Meghana here for defect fix on Cancel button.
            if ((f_scanwarning_code == PQM_WARN_TX_INTENSITY_NOTFOUND) ||
                (f_scanwarning_code == PQM_WARN_INTENSITY_NOTFOUND) ||
                (f_scanwarning_code == PQM_WARN_SCAN_INTENSITY_COIL) ||
                (f_scanwarning_code == PQM_WARN_SCAN_INTENSITY_COUCH) ||
                (f_scanwarning_code == PQM_WARN_SCAN_INTENSITY_MAP) ||
                (f_scanwarning_code == PQM_WARN_SCAN_INTENSITY_AREA) ||
                (f_scanwarning_code == PQM_WARN_SCAN_DEGENERACY_MAP) ||
                (f_scanwarning_code == PQM_WARN_SCAN_PV_MOVINGBED_INTEN) || //+Patni-MP/2009Dec11/Added/MVC005969
                (f_scanwarning_code == PQM_WARN_SCAN_AUTO_VOICE) ||
                (f_scanwarning_code == PQM_WARN_SCAN_DRKS_RECON_MODE) ||
                (f_scanwarning_code == PQM_WARN_SCAN_TX_INTENSITY_AREA) ||
                (f_scanwarning_code == PQM_WARN_SCAN_TX_DEGENERACY_MAP)

               ) {

                m_pqm_ptr->GetStudy()->SetAcqStatus(m_acq_manager->GetScanProcedure()->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                m_acq_manager->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT);
            }

            m_pqm_ptr->GetAMB()->PqmPmAbortAmb();
        }

        //-Patni-NP/2009Aug25/Added/MVC004384
        return false;
    }
}

//****************************Method Header************************************
//Method Name   :WarningSAR
//Author        :PATNI/LK
//Purpose       :This function pop up MPlus Warning dialog
//*****************************************************************************
bool CScanProcedure::WarningSAR(
    const int f_sarwarning_code,
    const CString& f_sarwarn_source,
    const bool f_reset_current_protocol_status, //+Patni-NP/2010Jan7/Added/MVC06663
    const int f_sarwarning_dialogtype,
    int f_sarwarning_level,
    int f_sarwarning_buttonstype

)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::WarningSAR");
    PQM_SCOPED_TRACE();

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(f_sarwarn_source);
    //Patni-KSS/2011Feb21/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_sarwarning_dialogtype);

    CString l_sarwarning(_T(""));
    MPLUSERRORUSERRESPONSE_e    l_response = DisplayMessageForSAR(f_sarwarning_code);

    //+Patni-MP/PJS/2009Dec8/Modify/MVC004957

    if ((l_response == MPlusErrResponse_CANCEL)
        || (f_sarwarning_code == PQM_SAR_ERROR)
        || (f_sarwarning_code == PQM_SAR_LIMIT_OVER)
        || (f_sarwarning_code == PQM_SAR_SAE_ERROR)
        || (f_sarwarning_code == PQM_SAR_COIL_PROTECT)
        || (f_sarwarning_code == PQM_SAR_RF_PROTECT)
        || (f_sarwarning_code == PQM_SAR_COIL_PROTECT_WAIT)
        || (f_sarwarning_code == PQM_SAR_LIMIT_OVER_WAIT)) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Aborting Scan..."));

        m_acq_manager->AcqDeleteLockFile();
        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        ResetCurrentRequest();

        //Meghana here
        //+Patni-NP/2010Jan7/Added/MVC06663
        if (f_reset_current_protocol_status) {
            m_pqm_ptr->GetStudy()->SetAcqStatus(m_acq_manager->GetScanProcedure()->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        }

        //-Patni-NP/2010Jan7/Added/MVC06663
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqm_ptr->GetAMB()->PqmPmAbortAmb();
        return false;

    } else if ((PQM_SAR_SAE_WARN == f_sarwarning_code) &&
               (MPlusErrResponse_OK == l_response)) {

        return true;

    } else if (l_response == MPlusErrResponse_OK) {

        m_acq_manager->AutoScanRetry();
    }

    return false;

}

//****************************Method Header************************************
//Method Name   :WarningSARAfterAcqmanResponseForPrescan()
//Author        :iGATE/AKR
//Purpose       :
//*****************************************************************************
bool CScanProcedure::WarningSARAfterAcqmanResponseForPrescan(
    const status_t f_sts_sar,
    CPqmProtocol* f_curr_protocol
)
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::WarningSARAfterAcqmanResponseForPrescan");
    PQM_SCOPED_TRACE();

    if (NULL == f_curr_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_curr_protocol is NULL"));
        return false;
    }

    MPLUSERRORUSERRESPONSE_e    l_response = DisplayMessageForSAR(f_sts_sar);

    if ((PQM_SAR_SAE_WARN == f_sts_sar || PQM_SAR_OK_BUT_COIL == f_sts_sar || PQM_SAR_OK_BUT_SAR == f_sts_sar
         || PQM_SAR_OK_BUT_RF == f_sts_sar) && (MPlusErrResponse_OK == l_response)) {
        return true;
    }

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Aborting Scan..."));

    //Abort the scan...
    if (!f_curr_protocol->GetSeriesNumInDbFlag()) {

        m_pqm_ptr->GetStudy()->SetAcqStatus(f_curr_protocol, C_PR_STATUS_WAIT);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

    } else {

        m_pqm_ptr->GetStudy()->SetAcqStatus(f_curr_protocol, C_PR_STATUS_FAIL);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_FAIL);
    }

    m_pqm_ptr->GetStudy()->SetScanModeOfHead();
    ResetCurrentRequest();

    return false;
}
//****************************Method Header************************************
//Method Name   :AcquisitionScanRestart()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcquisitionScanRestart(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcquisitionScanRestart");
    //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - Code Review
    PQM_SCOPED_TRACE();

    CVoiceManager* l_voice_manager = CVoiceManager::GetInstance();

    //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
    if (l_voice_manager  == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("l_voice_manager pointer is NULL"));
        return;
    }

    //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();

    if (m_wait_for_prep_scan) {
        m_acq_manager->GetPqm()->WriteToUI(PQM_MSG_INJECT_TIME_INVISIBLE);

        m_acq_manager->GetPqm()->GetAMB()->PqmAmbPrepEnd();

    }

    if ((!m_enable_prep_voice) && (m_pqm_protocol->GetPrepScan())) {

        SetWaitForPrepScan(FALSE);

        //+Patni-HAR/2010Jun04/Modified/PQM Crash in Visual Prep
        //if (!m_scan_transaction->SendControl(ACQ_START_BUTTON)) {
        if (!m_acq_manager->GetLastTransaction()->SendControl(ACQ_START_BUTTON)) {
            //-Patni-HAR/2010Jun04/Modified/PQM Crash in Visual Prep

            //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
            //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
            //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Cannot Connect to AcqMan"));

            l_study->SetScanModeOfHead();
            ResetCurrentRequest();
            //Patni-PJS/2010Sep7/Modified/TMSC-REDMINE-685
            //return;
        }

        //Patni-PJS/2010Sep7/Added/TMSC-REDMINE-685/As per base code
        return;
    }

    if (m_pqm_protocol->GetDynamic() &&
        m_pqm_protocol->GetPlnAutoVoice()) {

        int l_length = l_voice_manager->GetVoiceLength(
                           m_pqm_protocol->GetPlnAutoVoice());

        for (int segment = 0; segment < m_av_time_ctrl.numSegment; segment++) {
            if (m_current_dynamic_repeat < m_av_time_ctrl.segment[segment].repeat) {
                break;
            }
        }

        if ((l_length > 0) &&
            (m_av_time_ctrl.segment[segment].start > l_length)) {
            //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
            l_voice_manager ->CmdProg(m_av_time_ctrl.segment[segment].start,
                                      0,
                                      m_pqm_protocol->GetPlnAutoVoice());
            //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941

        } else {

            if (m_wait_for_prep_scan) {
                l_voice_manager ->CmdPlay(m_pqm_protocol->GetPlnAutoVoice());

            } else {
                l_voice_manager ->CmdPlay(m_pqm_protocol->GetPlnAutoVoice(), TRUE);
            }
        }

    } else if (m_pqm_protocol->GetPlnAutoVoice()) {

        if (m_wait_for_prep_scan) {
            l_voice_manager ->CmdDelay(m_acq_manager->GetPqm()->GetAvInterval(),
                                       m_pqm_protocol->GetPlnAutoVoice());

        } else {
            l_voice_manager ->CmdDelay(m_acq_manager->GetPqm()->GetAvInterval(),
                                       m_pqm_protocol->GetPlnAutoVoice(),
                                       TRUE);
        }

        SetWaitForVoice(true);

        if (m_av_tp1_ctrl && (m_av_tp1_margin >= 0)) {
            //+Patni-Dhanesh/2009Nov04/MOdified/MVC004746
            l_voice_manager ->CmdTrap(CVoiceManager::GetInstance()->GetAvTp1Delay(),
                                      l_voice_manager->GetAvTp1Delay(),
                                      PQM_AV_VOICE_5,
                                      m_pqm_protocol->GetPlnAutoVoice());
        }

    } else if ((m_pqm_protocol->GetExeAutoVoice()) & AUTO_VOICE_VALUE) {

        if (m_wait_for_prep_scan) {
            //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
            l_voice_manager ->CmdPlay((m_pqm_protocol->GetExeAutoVoice()) & AUTO_VOICE_VALUE);
            //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941

        } else {
            l_voice_manager ->CmdPlay(((m_pqm_protocol->GetExeAutoVoice()) & AUTO_VOICE_VALUE) ,
                                      TRUE);
        }
    }

    SetWaitForPrepScan(FALSE);

    if (!m_acq_manager->GetLastTransaction()->SendControl(ACQ_START_BUTTON)) {
        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
        //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot Connect to AcqMan"));

        l_study->SetScanModeOfHead();
        ResetCurrentRequest();
        return;
    }

    //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - Code Review
}

//+Patni-Ravindra Acharya/2010June24/Added/IR-153
COlpTemp* CScanProcedure::CreateOLPTransaction()
{
    CString l_func_name = _T("CScanProcedure::CreateOLPTransaction");
    PQM_TRACE(USER_FUNC_MARKER, l_func_name, l_func_name);

    m_olp_temp = new COlpTemp(this);

    AddTransaction(m_olp_temp);

    return m_olp_temp;
}
//-Patni-Ravindra Acharya/2010June24/Added/IR-153

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmAcqAutoCoilSet
// Author       : PATNI/ HEMANT
// Purpose      : Position of the coil and the control to automatically set the coil.
//***********************************************************************************
status_t CScanProcedure::PqmAcqAutoCoilSet(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* f_cds_protocol
)
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::PqmAcqAutoCoilSet");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmAcqAutoCoilSet"));

    if (!f_cds_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_cds_protocol is NULL"));
        return E_ERROR ;
    }

    //DB_FUNC_ENTER("[PqmAcqAutoCoilSet]:");

    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    CString l_message_str  = _T("Starting with CDS Coil Adjustment and Auto setting....");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

    /*Justification*/
    CpqmCDS* l_pqm_cds = m_pqm_ptr->GetCDS();
    l_pqm_cds->PqmCdsAdjustCoilPosition(f_cds_protocol);

    /* Coordinate regulation of information written to study*/
    CpqmCDSInfo* l_curobj_cdsinfo = f_cds_protocol->GetCDSInfo();
    CCDSUtilities::PutCdsCoordinate(f_vf_handle, f_cds_protocol->GetProtocol(),
                                    l_curobj_cdsinfo->m_coordinatetbl);

    /* Coil position selection*/
    l_pqm_cds->PqmCdsPositionForCoilSel(f_cds_protocol);

    /* Write to the display position information studies */
    CPQMStudy* l_study_obj = m_pqm_ptr->GetStudy();
    l_study_obj->PutSaturnCoil(f_vf_handle,
                               f_cds_protocol->GetProtocol(),
                               f_cds_protocol->GetSaturnCoil());

    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    /* Automatic coil set*/
    if (l_pqm_cds->PqmCoilCDSAutoCoilSet(f_cds_protocol) == E_ERROR) {
        /* If no error is set coil*/
        //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_COIL_CDS_ABORT"),ERR_ERROR);
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_CDS_ERR_COIL_CDS_ABORT,
            _T("IDS_CDS_ERR_COIL_CDS_ABORT"),
            Information, L"ScanProcedure.cpp"); //Patni-HAR/2010Apr26/Modified/TMSC-REDMINE-162


        //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
        l_message_str  = _T("CDS Coil Adjustment and Auto setting Ends with Error....");
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
        //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

        /* CDS information is invalid*/
        memset(l_curobj_cdsinfo->m_coordinatetbl, 0, sizeof(Vft_CdsCoilLocation_t)*PQM_NUM_COIL_PORT);
        memset(l_curobj_cdsinfo->m_locationtbl, 0, sizeof(Vft_CdsCoilLocation_t)*PQM_NUM_COIL_PORT);

        /* Wrote the study */
        CCDSUtilities::PutCdsCoordinate(f_vf_handle,
                                        f_cds_protocol->GetProtocol(),
                                        l_curobj_cdsinfo->m_coordinatetbl);

        AcquisitionScanAbort();
        l_study_obj->SetAcqStatus(f_cds_protocol, C_PR_STATUS_WAIT);
        l_study_obj->SetScanModeOfHead();
        ResetCurrentRequest();

        l_curobj_cdsinfo->m_cds_step 	= CDS_STEP0_NONE;

        l_pqm_cds->PqmCdsReturnCoil(f_cds_protocol->GetSaturnCoil());

        //PqmPrPutSaturnCoil(vfd, f_cds_protocol->prot,&f_cds_protocol->saturnCoil);
        //PqmAllCoilGetSaturn(&f_cds_protocol->saturnCoil,&f_cds_protocol->coilDBtbl);
        //PqmPrPutConnectedCoil(vfd, f_cds_protocol->prot, f_cds_protocol);
        //PqmPrPutRecvrCoil(vfd, f_cds_protocol->prot, f_cds_protocol);
        l_study_obj->PutSaturnCoil(f_vf_handle,
                                   f_cds_protocol->GetProtocol(),
                                   f_cds_protocol->GetSaturnCoil());

        if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_cds_protocol->GetSaturnCoil(),
                f_cds_protocol->GetCoilDBTable())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
        }

        l_study_obj->PutConnectedCoil(f_vf_handle,
                                      f_cds_protocol->GetProtocol(),
                                      f_cds_protocol);

        l_study_obj->PutRecvrCoil(f_vf_handle,
                                  f_cds_protocol->GetProtocol(),
                                  f_cds_protocol);

        //+Patni-Hemant/2011Jan11/Added/MVC009567/MVC008295
        if (m_pqm_ptr->GetCoilSelectionTool()) {
            if (!m_pqm_ptr->GetCoilSelectionTool()->CSSetCoilName(f_cds_protocol, f_vf_handle)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
            }
        }

        //-Patni-Hemant/2011Jan11/Added/MVC009567/MVC008295
        //Now this will update the GUI ...
        m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

        //This is to be called after PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT msg
        //because PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT considers scan index internally
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

        return E_ERROR;
    }

    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    l_message_str  = _T("CDS Coil Adjustment and Auto setting Ends succesfully(coil is detected)....");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

    //PqmAllCoilGetSaturn( &f_cds_protocol->saturnCoil, &f_cds_protocol->coilDBtbl) ;
    if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_cds_protocol->GetSaturnCoil(),
            f_cds_protocol->GetCoilDBTable())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
    }

    //Now this will update the GUI ...
    //m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

    //if(PqmGlobal.cds_CheckCoil)
    if (l_pqm_cds->GetCDSCheckCoilFlag() != CDSCheckCoil_None) {
        /* Auto verify after coil*/
        l_pqm_cds->PqmPmCoilSelAfterAutoCoil(f_cds_protocol->GetProtocol());

        return E_NO_ERROR;
    }

    /* CDS wrote to study the status  */
    CCDSUtilities::PutCdsStatus(f_vf_handle, f_cds_protocol->GetProtocol() , &(l_curobj_cdsinfo->m_cds_status));

    /* Wrote the study normal scan coil information */
    //PqmPrPutSaturnCoil(vfd, f_cds_protocol->prot,&f_cds_protocol->saturnCoil);
    //PqmPrPutConnectedCoil(vfd, f_cds_protocol->prot, f_cds_protocol);
    //PqmPrPutRecvrCoil(vfd, f_cds_protocol->prot, f_cds_protocol);
    l_study_obj->PutSaturnCoil(f_vf_handle,
                               f_cds_protocol->GetProtocol(),
                               f_cds_protocol->GetSaturnCoil());

    l_study_obj->PutConnectedCoil(f_vf_handle,
                                  f_cds_protocol->GetProtocol(),
                                  f_cds_protocol);

    l_study_obj->PutRecvrCoil(f_vf_handle,
                              f_cds_protocol->GetProtocol(),
                              f_cds_protocol);


    /* Set the transmit coil*/
    //if(!PqmSetTxCoilInSaturn(vfd,f_cds_protocol, &f_cds_protocol->coilDBtbl ))
    if (!l_study_obj->SetTxCoilInSaturn(f_vf_handle, f_cds_protocol, f_cds_protocol->GetCoilDBTable())) {
        /* If no output message is sent to coil Eranabi*/

        //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_ILLEGAL_COIL_ID"),ERR_ERROR);
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_CDS_ERR_ILLEGAL_COIL_ID,
            _T("IDS_CDS_ERR_ILLEGAL_COIL_ID"),
            SCAN_ERR_WARNING, L"ScanProcedure.cpp"); //+Patni-NP/2010May26/Modified/Redmine-347

        AcquisitionScanAbort();
        l_study_obj->SetAcqStatus(f_cds_protocol, C_PR_STATUS_WAIT);
        l_study_obj->SetScanModeOfHead();
        ResetCurrentRequest();

        l_curobj_cdsinfo->m_cds_step 	= CDS_STEP0_NONE;
        l_curobj_cdsinfo->m_cds_rescan	= TRUE;

        //Now this will update the GUI ...
        m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        return E_ERROR;
    }

    /* Coil name*/
    //PqmPmCoilSelSetCoilName( f_cds_protocol, vfd);
    if (!m_pqm_ptr->GetCoilSelectionTool()->CSSetCoilName(f_cds_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
    }

    //apply coil change for current protocol
    m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

    if (!l_pqm_cds->CopyCDSCCoilInfoAsPerBatchSetting(f_cds_protocol)) {


        AcquisitionScanAbort();
        l_study_obj->SetAcqStatus(f_cds_protocol, C_PR_STATUS_WAIT);
        l_study_obj->SetScanModeOfHead();
        ResetCurrentRequest();

        l_curobj_cdsinfo->m_cds_step 	= CDS_STEP0_NONE;
        l_curobj_cdsinfo->m_cds_rescan	= TRUE;

        //Now this will update the GUI ...
        //This will again check for batch setting and then apply accordingly.
        //This will also consider the current protocol under scan
        m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_COMPASS_COIL_TO_ALL_BELOW);

        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        return E_ERROR;
    }

    if (CheckToDisplayCdsInDoNotShow(f_cds_protocol)) {
        l_pqm_cds->PqmPmCoilSelAfterAutoCoil(f_cds_protocol->GetProtocol());
        return E_NO_ERROR;
    }

    l_curobj_cdsinfo->m_cds_step = CDS_STEP6_DONE;	/* Automatic coil configured to CDS_STEP6_DONE */

    /* ScanStart */
    PqmAcqAfterCDSScanStart(f_cds_protocol);

    return E_NO_ERROR;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : pqmAcqCDSContinueCB
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CScanProcedure::pqmAcqCDSContinueCB()const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::pqmAcqCDSContinueCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::pqmAcqCDSContinueCB"));


    //CPqmProtocol* l_current_cds_protocol = l_pqm_cds->GetCurrentCDSProtocol();
    CPqmProtocol* l_current_cds_protocol = GetScanProcProtocol();

    if (!l_current_cds_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_cds_protocol or l_rcvcoil_p is NULL"));

        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        return ;
    }

    //	int 			*pos_list, count;
    //	PqmPrData_t 	*obj;
    //	PqmCoil_p		Rcvcoil_p;
    //
    //	//DB_FUNC_ENTER("[pqmAcqCDSContinueCB]:");
    //
    //	PqmListGetSelectedPos(&pos_list,&count,0);
    //	if(count == 0)
    //	{
    //		return;
    //	}
    //	obj = PqmPqGetItemFromPos(pos_list[0]);

    //Rcvcoil_p  = PqmGlobal.wbCoil;

    CCoil f_wb_coil ;

    m_pqm_ptr->GetDataManager()->GetPQMCoilData()->GetWBCoil(f_wb_coil);

    PqmCoil_p l_rcvcoil_p = NULL;

    if (f_wb_coil.IsWBCoil()) {
        l_rcvcoil_p = f_wb_coil.GetCoil();
    }

    if (l_rcvcoil_p) {	/* Information coil NULL (no subject coil) to check if it is also */
        CPQMStudy* l_study = m_pqm_ptr->GetStudy();
        l_study->SetWBCoilSaturn(l_study->GetVfDBHandle(),
                                 l_current_cds_protocol->GetProtocol(),
                                 l_current_cds_protocol->GetSaturnCoil(),
                                 l_rcvcoil_p,
                                 l_current_cds_protocol->GetSaturnCoil()->RcvCoil[0].portID[1]) ;

    }

    //update the GUI for updated coil
    m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

    /*After changing from the first re-check the coil*/
    PqmAcqAfterCDSScanStart(l_current_cds_protocol);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CScanProcedure::pqmAcqCDSContinueCB"));
}
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   :WarningPreScan()
//Author        :PATNI/NJK
//Purpose       :This method displays the warning dialog box to the user and calls the
//                respective function depending on reply got from the user
//*****************************************************************************
void CScanProcedure::WarningPreScan(
    const int flg,
    const CString& f_errormsg,			// Added By KT/ACE-2/11-06-2009/Multilingualisation
    CCoil* newcoil
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::WarningPreScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of WarningPreScan"));

    CString str_msg(_T(""));
    str_msg.Format(_T("Warning code = %d new coil key = %ld"), flg, (newcoil) ? newcoil->GetCoilKey() : 0);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilDB* coil_db = m_acq_manager->GetPqm()->GetCoilDB();

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_protocol == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_protocol pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoil* coil_p = coil_db->GetCoil(m_pqm_protocol->GetPqmPrMode()->GetRecvCoilId());
    CPqmCoilData* l_pqm_coil_data = m_acq_manager->GetPqm()->GetDataManager()->GetPQMCoilData();

    if (NULL == l_pqm_coil_data) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("l_pqm_coil_data pointer is NULL"));
        return;
    }

    CCoil l_coil_obj;
    l_pqm_coil_data->PqmCoilGetCoil(m_pqm_protocol->GetPqmPrMode()->GetRecvCoilId(), l_coil_obj);
    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    CString l_warning_msg = CPqmUtilities::GetMultiLingualString(f_errormsg);	// Added By KT/ACE-2/15-06-2009/Multilingualisation
    //l_warning_msg.LoadString(flg);									// Added By KT/ACE-2/15-06-2009/Multilingualisation

    CString l_warnapp_name(_T(""));
    l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

    if (newcoil != NULL) {
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        /*if (coil_p != NULL) {*/
        if (NULL != l_coil_obj.GetCoil()) {
            char		buf[128] = "";
            sprintf(buf, "%s -> %s", l_coil_obj.GetLabel(), newcoil->GetLabel());
            l_warning_msg += CString(buf);
            //CPQMLogMgr::GetInstance()->DisplayScanError(flg,SCAN_ERR_CRUCIAL, _T("Scanprocedure.cpp"));
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                l_warning_msg,	//Added By KT/ACE-2/11-06-2009/Multilingualisation
                SCAN_ERR_CRUCIAL,
                _T("PQM"),
                l_warnapp_name
            );
        }

        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

    } else {
        //+Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion
        MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

        switch (flg) {

                //+Patni-ARD/2009Sep15/Added/Pump freezer confirmation
            case IDS_PQM_WARN_PRESCAN_PUMP_AND_FREEZER:
            case IDS_PQM_WARN_PRESCAN_PUMP:


                l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                 // l_warning_msg,						//Added By KT/ACE-2/11-06-2009/Multilingualisation
                                 l_warning_msg,
                                 l_warnapp_name,
                                 SCAN_WARN_CRUCIAL,
                                 SCAN_WARN_SYSMODAL,
                                 SCAN_WARN_OK_CANCEL_BUTTONS);

                if (l_response == MPlusErrResponse_OK) {
                    ForceRMStatusCB();
                }

                break;

                //-Patni-ARD/2009Sep15/Added/Pump freezer confirmation

            case IDS_PQM_WARN_PRESCAN_COUCH:
            case IDS_PQM_WARN_PRESCAN_PROJECTION:
            case IDS_PQM_WARN_PRESCAN_FREEZER:

                l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                 // l_warning_msg,						//Added By KT/ACE-2/11-06-2009/Multilingualisation
                                 l_warning_msg,
                                 l_warnapp_name,
                                 SCAN_WARN_INFORMATION, //TMSC-Tanoue/2010Mar05/MSA0251-00247
                                 SCAN_WARN_SYSMODAL,
                                 SCAN_WARN_OK_CANCEL_BUTTONS);

                if (l_response == MPlusErrResponse_OK) {



                    if (m_pqm_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE ||
                        m_pqm_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) {


                        AUTOLOCATOR_TYPE l_altype = m_pqm_protocol->GetAutoLocatorType();

                        if (m_pqm_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE) {
                            m_acq_manager->GetPqm()->GetStudy()->DeleteCALAFields(m_pqm_protocol->GetProtocol());

                        } else if (m_pqm_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) {
                            m_acq_manager->GetPqm()->GetStudy()->DeleteHALAFields(m_pqm_protocol->GetProtocol());
                        }

                        if (FALSE == m_acq_manager->GetPqm()->SetAutoLocatorOffsetParameters(m_pqm_protocol)) {
                            PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                                      _T("Failed to process cardiac parameter settings"));
                            ResetCurrentRequest();
                            m_acq_manager->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT);
                        }

                    } else {
                        ForceParentCB();
                    }
                }

                break;

            case IDS_PQM_WARN_PRESCAN_DIFF_COIL:
                l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                 //	l_warning_msg,							//Added By KT/ACE-2/11-06-2009/Multilingualisation
                                 l_warning_msg,
                                 l_warnapp_name,
                                 SCAN_WARN_INFORMATION, //TMSC-Tanoue/2010Mar05/MSA0251-00247
                                 SCAN_WARN_SYSMODAL,
                                 SCAN_WARN_OK_CANCEL_BUTTONS
                             );

                if (l_response == MPlusErrResponse_OK) {
                    // CoilCB();
                }

                break;


            case IDS_PQM_WARN_PRESCAN_SPEEDER_COIL:

                l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                 //	l_warning_msg,							//Added By KT/ACE-2/11-06-2009/Multilingualisation
                                 l_warning_msg,
                                 l_warnapp_name,
                                 SCAN_WARN_INFORMATION, //TMSC-Tanoue/2010Mar05/MSA0251-00247
                                 SCAN_WARN_SYSMODAL,
                                 SCAN_WARN_OKBUTTON                      //-Patni-KT/2009Nov05/Commented/MVC005088 Defect

                             );
                break;

            case IDS_PQM_WARN_PRESCAN_PVC_COUCH_POSI:
                l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                 l_warning_msg,
                                 l_warnapp_name,
                                 SCAN_WARN_INFORMATION,
                                 SCAN_WARN_SYSMODAL,
                                 SCAN_WARN_OK_CANCEL_BUTTONS
                             );

                if (l_response == MPlusErrResponse_OK) {
                    ForceCoilStatusCB();
                }

                break;
        }

        //-Patni-KT/2009Nov05/Changed/MVC005088 Defect
        if ((l_response == MPlusErrResponse_CANCEL) || (l_response == MPlusErrResponse_OK && flg == IDS_PQM_WARN_PRESCAN_SPEEDER_COIL))  { //-Patni-KT/2009Nov05/Added/5088 Defect
            m_acq_manager->AcqDeleteLockFile();
            m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
            ResetCurrentRequest();

            //+Patni-ARD/2009Sep15/Added/Pump freezer confirmation
            if ((flg == IDS_PQM_WARN_PRESCAN_COUCH) || (flg == IDS_PQM_WARN_PRESCAN_PROJECTION) ||
                (flg == IDS_PQM_WARN_PRESCAN_PUMP_AND_FREEZER)  ||
                (flg == IDS_PQM_WARN_PRESCAN_PUMP) ||
                (flg == IDS_PQM_WARN_PRESCAN_FREEZER)  ||
                (flg == IDS_PQM_WARN_PRESCAN_SPEEDER_COIL)) {

                CPqm* pqm = m_acq_manager->GetPqm();
                pqm->WriteToUI(PQM_MSG_ABORT_WAIT);

                pqm->GetAMB()->PqmPmAbortAmb();
            }

            //-Patni-ARD/2009Sep15/Added/Pump freezer confirmation
        }

        //-Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion

    }

    SetProcessing(GetProcessing() != PQM_ACQ_BIT_WARNING);
    return;
}

//****************************Method Header************************************
//Method Name   :IsHasParent()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CScanProcedure::IsHasParent(
)const
{
    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    CString l_func_name = _T("CScanProcedure::IsHasParent");
    PQM_TRACE(USER_FUNC_MARKER, l_func_name, _T("Trace of IsHasParent"));

    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, l_func_name ,
                  _T("m_acq_manager pointer is NULL"));
        return FALSE;
    }

    CPqm* l_pqm = m_acq_manager->GetPqm();

    if (l_pqm == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, l_func_name, _T("l_pqm pointer is NULL"));
        return FALSE;
    }

    CPQMStudy* l_pqm_study = l_pqm->GetStudy();

    if (l_pqm_study == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, l_func_name,
                  _T("l_pqm_study pointer is NULL"));
        return FALSE;
    }

    return l_pqm_study->IsHasLocatorImage(m_prot);
    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
}

//****************************Method Header************************************
//Method Name   :CheckParentCouchOffset
//Author        :PATNI/HAR
//Purpose       :This method returns error if the couch position is grater than
//               parent maximum couchoffset else return true
//****************************************************************************
int  CScanProcedure::CheckParentCouchOffset(
    const float f_couchposition
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CheckParentCouchOffset");

    CString str_couch_pos(_T(""));
    str_couch_pos.Format(_T("%f"), f_couchposition);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_couch_pos);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* pqmmanager = m_acq_manager->GetPqm();
    CPqmProtocol* curr_protocol = GetScanProcProtocol();
    float p_couch_pos = 0.0f, s_couch_pos = 0.0f, z = 0.0f;

    if (IsHasParent() || curr_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE
        || curr_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) {
        CPqmPrMode l_parent_info;
        float couch = 0.0f;


        if ((curr_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE &&
             curr_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_PLANNED) ||
            (curr_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE &&
             curr_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_PLANNED)) {
            pqmmanager->GetRefParentInfo(curr_protocol, &l_parent_info);

        } else {
            pqmmanager->GetStudy()->GetParentInfo(curr_protocol->GetProtocol(),
                                                  &l_parent_info);
        }

        // pqmmanager->GetStudy()->GetParentInfo(curr_protocol->GetProtocol(),
        //                                 &l_parent_info);

        //scan_proc->GetParentInfo(parentinfo);

        p_couch_pos = l_parent_info.GetCouchPos();
        couch = p_couch_pos - f_couchposition;
        couch = ABSF(couch);

        f32vec3_t offset = curr_protocol->GetOffset();

        z = offset.z * 1000.0f;

        //+ Patni-MSN/2009Aug26/Modified/MVC004248
        if (curr_protocol->GetPqmPrMode()->GetPatoriEndIn() ==
            VFC_PATORI_HEAD_FIRST) {

            s_couch_pos = curr_protocol->GetPqmPrMode()->GetHomePos() + z;

        } else {
            s_couch_pos = curr_protocol->GetPqmPrMode()->GetHomePos() - z;
        }

        //- Patni-MSN/2009Aug26/Modified/MVC004248

        if (couch > pqmmanager->GetCouchConfigPtr()->GetCouchMaximumParentOffset() ||
            (ABSF(p_couch_pos - f_couchposition) > COUCH_ACCURACY_RANGE_MM &&
             ABSF(p_couch_pos - s_couch_pos) > pqmmanager->GetCouchConfigPtr()->GetCouchMaximumParentOffset())) {
            //+Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage -Couch Distortion

            WarningPreScan(IDS_PQM_WARN_PRESCAN_COUCH, _T("IDS_PQM_WARN_PRESCAN_COUCH"), 0);		// Added By KT/ACE-2/11-06-2009/Multilingualisation
            //-Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage -Couch Distortion

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_couch_pos);
            return E_ERROR;
        }
    }

    return  E_NO_ERROR;
}

//*******************************Method Header*********************************
//Method Name    :CFAOpen()
//Author         :PATNI/
//Purpose        :Notify Message to GUI about the current shim fsy flag
//*****************************************************************************
void CScanProcedure::CFAOpen(
    CPqmProtocol* const f_pqmprotocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CFAOpen");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_pqmprotocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pqmprotocol is NULL"));
        return ;
    }

    //Patni/AM/2010July22/Added/NFD001-CFA
    if (!GetAcqManager()->GetPqm()->CanDisplayCFAGraph(f_pqmprotocol)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not display CFA Graph-->CanDisplayCFAGraph returned false"));
        return;
    }

    int l_shimstate = -1, l_shimfsy_flag = -1;
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shimstate);

    if (l_shimstate == SHIM_STATE_ACTIVE) {
    }

    CPqmShimSharedData::GetInstance()->GetShimFsyFlag(&l_shimfsy_flag);

    if (l_shimfsy_flag == VFC_SHIM_FSY_FAT) {
        GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_CFA_REDRAW_FAT);

    } else {
        GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_CFA_REDRAW_WATER);
    }
}

//*******************************Method Header*********************************
//Method Name    :IsCouchMoving()
//Author         :PATNI/
//Purpose        :This method returns couch moving
//*****************************************************************************
bool CScanProcedure::IsCouchMoving(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::IsCouchMoving");

    bool result = false;

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), result);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return result;
}

//*******************************Method Header*********************************
//Method Name    :SendInterLock()
//Author         :PATNI/MRP
//Purpose        :This method send the interlock transaction to acqman
//*****************************************************************************
bool CScanProcedure::SendInterLock(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendInterLock");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SendInterLock"));

    CScanTransaction* interlock = new CInterLock(this);

    AddTransaction(interlock);
    return interlock->PrepareRequest();
}

//*******************************Method Header*********************************
//Method Name    :MoveCouchProhibitionCheck()
//Author         :PATNI / HAR - JS
//Purpose        :
//*****************************************************************************
int CScanProcedure::MoveCouchProhibitionCheck(
    CPqmProtocol* f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::MoveCouchProhibitionCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of MoveCouchProhibitionCheck"));

    int l_status = E_NO_ERROR;

    if (f_protocol->GetFluoro() ||
        f_protocol->GetAppCode() == VFC_APP_CODE_MRS ||
        f_protocol->GetAppCode() == VFC_APP_CODE_MRSCSI ||
        f_protocol->GetShimming() == VFC_SHIM_HIGH_RESOLUTION) {

        float l_couch_pos = f_protocol->GetPqmPrMode()->GetCouchPos();
        float l_home_pos = f_protocol->GetPqmPrMode()->GetHomePos();

        if ((l_couch_pos != PQM_COUCH_INIT_POSITION) &&
            (fabs(l_couch_pos - l_home_pos)) > 0.00001) {

            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_MOVE_COUCH_PROHIBITION_ERROR,
                _T("IDS_ERR_MOVE_COUCH_PROHIBITION_ERROR"),
                SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");

            //+Patni/2011Mar16/Commented/V2.0/Redmine-1548
            //m_pqm_ptr->GetStudy()->SetScanModeOfHead();
            //ResetCurrentRequest();
            //-Patni/2011Mar16/Commented/V2.0/Redmine-1548

            l_status = E_ERROR;

        } else if (f_protocol->GetHoldScan() & VFC_HOLD_COUCH) {

            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_CAN_NOT_MOVE_COUCH, _T("IDS_ERR_CAN_NOT_MOVE_COUCH"),
                SCAN_ERR_NOTIFY,
                L"ScanProcedure.cpp");

            //+Patni/2011Mar16/Commented/V2.0/Redmine-1548
            //m_pqm_ptr->GetStudy()->SetScanModeOfHead();
            //ResetCurrentRequest();
            //-Patni/2011Mar16/Commented/V2.0/Redmine-1548

            l_status = E_ERROR;
        }
    }

    return l_status;
}

//+Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
//**************************************************************************
//Method Name   : PQMAcqSearchRFL
//Author        : PATNI/DKH
//Purpose       : To Search previous RFL value to be used.
//**************************************************************************
//-Patni-DKH/2010May03/Added/Phase3#IR105
void CScanProcedure::PQMAcqSearchRFL(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_curpqmprotocol, bool f_isreopenstudy)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::PQMAcqSearchRFL");

    //Patni-KSS/2011Feb21/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);
    UNREFERENCED_PARAMETER(f_isreopenstudy);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Inside function PQMAcqSearchRFL"));

    if (NULL == f_curpqmprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_curpqmprotocol pointer is NULL"));
        return;
    }

    CPqmProtocol*	l_pqmprotocol = f_curpqmprotocol;

    f_curpqmprotocol->SetRflProt(-1);

    CPqm* l_pqmptr = m_acq_manager->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmptr is NULL"));
        return;
    }

    CPQMStudy* l_pqmstudy = l_pqmptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy is NULL"));
        return;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (NULL == l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return;
    }

    int l_rflLimit = 0, l_tuneLimit = 0;

    if (l_pqmptr->IsSaturnCoilSystem()) {

        if (l_coil_data->PqmGetRflTuneInSaturn(f_curpqmprotocol->GetCoilDBTable(),
                                               &l_rflLimit,
                                               &l_tuneLimit) != E_NO_ERROR) {

            l_rflLimit = GLOBAL_RFL_LIMIT;
            l_tuneLimit = GLOBAL_TUNE_LIMIT;
        }

    } else {
        CCoil coil;
        CPqmPrMode* l_pqm_pr_mode = f_curpqmprotocol->GetPqmPrMode();

        if (NULL == l_pqm_pr_mode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pr_mode is NULL"));
            return;
        }

        l_coil_data->PqmCoilGetCoil(l_pqm_pr_mode ->GetRecvCoilId(), coil);

        if (coil.GetCoil()) {

            l_rflLimit = coil.GetRFLLimit();
            l_tuneLimit = coil.GetTuneLimit();

        } else {

            l_rflLimit = GLOBAL_RFL_LIMIT;
            l_tuneLimit = GLOBAL_TUNE_LIMIT;
        }
    }

    while (1) {
        //Patni-AMT/2010Feb26/Modified/Ph-3# PROPOSAL_07
        l_pqmprotocol = l_pqmstudy->GetPrevItemFromPos(l_pqmprotocol);

        if (NULL == l_pqmprotocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmprotocol is NULL"));
            break;
        }

        //+Patni-DKH/2010May03/Added/Phase3#IR105
        /*	if(f_isreopenstudy){
        		if (!(l_pqmprotocol->GetAcqStatus() == C_PR_STATUS_DONE || l_pqmprotocol->GetAcqStatus() == C_PR_STATUS_FAIL )){
        			continue ;
        		}

        	}else{
        		if (l_pqmprotocol->GetAcqStatus() != C_PR_STATUS_DONE) {
        			continue;
        		}
        	}*/
        //+Patni-DKH/2010May03/Added/Phase3#IR105

        CPqmPrMode* l_pqm_pr_mode = l_pqmprotocol->GetPqmPrMode();
        CPqmPrMode* l_curpqm_pr_mode = f_curpqmprotocol->GetPqmPrMode();

        if (NULL == l_pqm_pr_mode || NULL == l_curpqm_pr_mode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pr_mode or l_curpqm_pr_mode is NULL"));
            break;
        }

        if (l_pqm_pr_mode->GetScanNumber() != l_curpqm_pr_mode->GetScanNumber()) {
            break;
        }

        if (l_pqmptr->IsSaturnCoilSystem() &&
            (l_coil_data->PqmIsSameCoilInSaturn(f_curpqmprotocol->GetSaturnCoil(),
                                                l_pqmprotocol->GetSaturnCoil(),
                                                f_curpqmprotocol->GetCoilDBTable(),
                                                l_pqmprotocol->GetCoilDBTable()) == FALSE))

        {
            break;

        } else if (!l_pqmptr->IsSaturnCoilSystem() &&
                   !IS_COIL_SAME(l_curpqm_pr_mode->GetRecvCoilId(), l_pqm_pr_mode->GetRecvCoilId())) {
            break;
        }

        if (l_curpqm_pr_mode->GetAnatomy() != l_pqm_pr_mode->GetAnatomy()) {
            continue;
        }

        if (ABSF(l_curpqm_pr_mode->GetCouchPos() - l_pqmprotocol->GetRflPos()) > (float) l_rflLimit) {
            continue;
        }

        //+Patni-NP/2010May26/Added/IR-151
        if ((false == l_pqmstudy->GetIsStudyReopened()) || (true == l_pqmptr->IsSameStudy())) {
            f_curpqmprotocol->SetRflProt(l_pqmprotocol->GetProtocol());
        }

        //-Patni-NP/2010May26/Added/IR-151
        break;
    }

    //Patni-DKH/2010Mar10/Added/Phase3#MCM0229-00187
    if (-1 != f_curpqmprotocol->GetRflProt()) {



        // MEITEC/2010Apr26/Added/IR-86
        CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("RFL DATA COPY fromProt[%d] toProt=[%d]"),
                f_curpqmprotocol->GetRflProt(),
                f_curpqmprotocol->GetProtocol());

        struct VfMap_s {

            VfName_t m_name;
            VfName_t m_type;
        } const l_vf_map [] = {

            { SVN_EXE_PRESCAN_RFL_GAIN_VALUE, SVT_EXE_PRESCAN_RFL_GAIN_VALUE},
            { SVN_EXE_PRESCAN_RFL_SCALE_VALUE, SVT_EXE_PRESCAN_RFL_SCALE_VALUE},
            { SVN_SEQ_RFL_WAVE_FAMILY, SVT_SEQ_RFL_WAVE_FAMILY},
            { SVN_SEQ_RFL_WAVE_SUM, SVT_SEQ_RFL_WAVE_SUM},
            { SVN_SEQ_RFL_WAVE_PITCH, SVT_SEQ_RFL_WAVE_PITCH},
            { SVN_SEQ_RFL_WAVE_ANGLE, SVT_SEQ_RFL_WAVE_ANGLE},
            { SVN_EXE_PRESCAN_TGC_RFOUT_RATIO, SVT_EXE_PRESCAN_TGC_RFOUT_RATIO},
            { SVN_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT, SVT_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT},
            { SVN_EXE_PRESCAN_RF_AMP_MODE_MAX, SVT_EXE_PRESCAN_RF_AMP_MODE_MAX},
            { SVN_EXE_PRESCAN_STGC_RFOUT_RATIO, SVT_EXE_PRESCAN_STGC_RFOUT_RATIO},
            { SVN_EXE_PRESCAN_NUM_MTGC, SVT_EXE_PRESCAN_NUM_MTGC},
            { SVN_EXE_PRESCAN_MTGC_INDEX, SVT_EXE_PRESCAN_MTGC_INDEX},
            { SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX, SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX},
            { SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO, SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO},
            { SVN_EXE_RTSAR_PEAK_POWER_RFL, SVT_EXE_RTSAR_PEAK_POWER_RFL}
        };


        const int			l_vf_map_size = sizeof(l_vf_map) / sizeof(*l_vf_map);
        VfFieldSpec_t       l_vfreq_get[l_vf_map_size] = {0};
        int					l_index = 0;

        for (l_index = 0; l_index < l_vf_map_size; l_index++) {

            VFF_SET_ARGS(l_vfreq_get[l_index], l_vf_map[l_index].m_name, l_vf_map[l_index].m_type, 0,
                         NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        }

        CVarFieldHandle*	vf_handle = l_pqmstudy->GetVfDBHandle();
        int					l_num_done = 0;

        VfPathElem_t        l_path[1] = {0};
        l_path[0].subtree_name = SVN_ST_PROTOCOL;
        l_path[0].index = f_curpqmprotocol->GetRflProt();


        vf_handle->GetFields(l_path, SVD_ST_PROTOCOL,
                             l_vfreq_get, l_vf_map_size, &l_num_done);

        if (l_vf_map_size != l_num_done) {

            f_curpqmprotocol->SetRflProt(-1);
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("RFL DATA COPY ERROR!!!"));
        }

        VfFieldSpec_t       l_vfreq_put[l_vf_map_size] = {0};
        int l_num = 0;

        for (l_index = 0, l_num = 0 ; l_index < l_vf_map_size ; l_index ++) {

            if (E_NO_ERROR == l_vfreq_get[l_index].status) {

                VFF_SET_ARGS(l_vfreq_put[l_num], l_vf_map[l_index].m_name , l_vf_map[l_index].m_type,
                             l_vfreq_get[l_index].size, l_vfreq_get[l_index].buffer, VFO_REPLACE);
                l_num++;
            }
        }


        if (l_num) {
            time_t l_time_stamp;
            l_num_done = 0;

            VfPathElem_t l_path1[1] = {0};
            l_path1[0].subtree_name = SVN_ST_PROTOCOL;
            l_path1[0].index = f_curpqmprotocol->GetProtocol();

            status_t l_status = vf_handle->PutFields(l_path1, SVD_ST_PROTOCOL, l_vfreq_put, l_num, &l_num_done, &l_time_stamp);

            if (E_NO_ERROR != l_status || l_num_done != l_num) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            }

            if (l_num != l_num_done) {

                f_curpqmprotocol->SetRflProt(-1);
                CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("RFL DATA COPY ERROR!!!"));
            }
        }



        //delete the data received
        for (l_index = 0; l_index < l_vf_map_size; l_index++) {
            if (l_vfreq_get[l_index].status == E_NO_ERROR && l_vfreq_get[l_index].size > 0)
                FREE_PTR(l_vfreq_get[l_index].buffer) ;
        }

        //+Patni-PJS/2010Apr21/Added/JaFT# CFA RFL Part-II
        l_pqmstudy->AddPrescanTagsForNextProtocol(l_pqmstudy->GetVfDBHandle(), NULL,
                f_curpqmprotocol->GetProtocol(), false, true);
        //-Patni-PJS/2010Apr21/Added/JaFT# CFA RFL Part-II
    }

    return;
}

//**************************************************************************
//Method Name   : PQMAcqSearchCFA
//Author        : PATNI/DKH
//Purpose       : To Search previous CFA value to be used.
//**************************************************************************
//Patni-DKH/2010May03/Added/Phase3#IR105
void CScanProcedure::PQMAcqSearchCFA(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_curpqmprotocol, bool f_isreopenstudy)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::PQMAcqSearchCFA");

    //Patni-KSS/2011Feb21/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);
    UNREFERENCED_PARAMETER(f_isreopenstudy);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function PQMAcqSearchCFA"));

    if (NULL == f_curpqmprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_curpqmprotocol pointer is NULL"));
        return;
    }

    CPqmProtocol*	l_pqmprotocol = f_curpqmprotocol;
    f_curpqmprotocol->SetCfaProt(-1);

    CPqm* l_pqmptr = m_acq_manager->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmptr is NULL"));
        return;
    }

    CPQMStudy* l_pqmstudy = l_pqmptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy is NULL"));
        return;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (NULL == l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return;
    }

    int l_rflLimit = 0, l_tuneLimit = 0;

    if (l_pqmptr->IsSaturnCoilSystem()) {

        if (l_coil_data->PqmGetRflTuneInSaturn(f_curpqmprotocol->GetCoilDBTable(),
                                               &l_rflLimit,
                                               &l_tuneLimit) != E_NO_ERROR) {

            l_rflLimit = GLOBAL_RFL_LIMIT;
            l_tuneLimit = GLOBAL_TUNE_LIMIT;
        }

    } else {
        CCoil coil;
        CPqmPrMode* l_pqm_pr_mode = f_curpqmprotocol->GetPqmPrMode();

        if (NULL == l_pqm_pr_mode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pr_mode is NULL"));
            return;
        }

        l_coil_data->PqmCoilGetCoil(l_pqm_pr_mode ->GetRecvCoilId(), coil);

        if (coil.GetCoil()) {

            l_rflLimit = coil.GetRFLLimit();
            l_tuneLimit = coil.GetTuneLimit();

        } else {

            l_rflLimit = GLOBAL_RFL_LIMIT;
            l_tuneLimit = GLOBAL_TUNE_LIMIT;
        }
    }

    int l_shim = 0;

    while (1) {
        //Patni-AMT/2010Feb26/Modified/Ph-3# PROPOSAL_07
        l_pqmprotocol = l_pqmstudy->GetPrevItemFromPos(l_pqmprotocol);

        if (NULL == l_pqmprotocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmprotocol is NULL"));
            break;
        }

        //+Patni-DKH/2010May03/Added/Phase3#IR105
        /*	if(f_isreopenstudy){
        		if (!(l_pqmprotocol->GetAcqStatus() == C_PR_STATUS_DONE || l_pqmprotocol->GetAcqStatus() == C_PR_STATUS_FAIL )){
        			continue ;
        		}

        	}else{
        		if (l_pqmprotocol->GetAcqStatus() != C_PR_STATUS_DONE) {
        			continue;
        		}
        	}*/
        //+Patni-DKH/2010May03/Added/Phase3#IR105

        CPqmPrMode* l_pqm_pr_mode = l_pqmprotocol->GetPqmPrMode();
        CPqmPrMode* l_curpqm_pr_mode = f_curpqmprotocol->GetPqmPrMode();

        if (NULL == l_pqm_pr_mode || NULL == l_curpqm_pr_mode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pr_mode or l_curpqm_pr_mode is NULL"));
            break;
        }

        if (l_pqm_pr_mode->GetScanNumber() != l_curpqm_pr_mode->GetScanNumber()) {
            break;
        }

        if (l_pqmptr->IsSaturnCoilSystem() &&
            (l_coil_data->PqmIsSameCoilInSaturn(f_curpqmprotocol->GetSaturnCoil(),
                                                l_pqmprotocol->GetSaturnCoil(),
                                                f_curpqmprotocol->GetCoilDBTable(),
                                                l_pqmprotocol->GetCoilDBTable()) == FALSE))

        {
            break;

        } else if (!l_pqmptr->IsSaturnCoilSystem() &&
                   !IS_COIL_SAME(l_curpqm_pr_mode->GetRecvCoilId(), l_pqm_pr_mode->GetRecvCoilId())) {
            break;
        }

        if (l_pqmptr->GetAMB()->PqmAMBScanSeg2Active()) {
            if (!(l_curpqm_pr_mode->GetCouchPos() == l_pqm_pr_mode->GetCouchPos())) {
                continue;
            }

        } else if (ABSF(l_curpqm_pr_mode->GetCouchPos() - l_pqmprotocol->GetCfaPos()) > (float) l_rflLimit) {
            continue;
        }

        //+Patni-NP/2010May26/Added/IR-151
        if ((false == l_pqmstudy->GetIsStudyReopened()) || (true == l_pqmptr->IsSameStudy())) {
            f_curpqmprotocol->SetCfaProt(l_pqmprotocol->GetProtocol());
        }

        //-Patni-NP/2010May26/Added/IR-151
        l_shim = l_pqmprotocol->GetShimming();
        break;
    }

    if (f_curpqmprotocol->GetCfaProt() != -1) {
        VfFieldSpec_t       vfReq[1];
        // MEITEC/2010Apr26/Added/IR-86
        CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("CFA DATA COPY fromProt[%d] toProt=[%d]")
                , f_curpqmprotocol->GetCfaProt(), f_curpqmprotocol->GetProtocol());

        int n = 0;

        if (!l_shim || (VFC_SHIM_HIGH_RESOLUTION == l_shim)) {
            VFF_SET_ARGS(vfReq[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE,
                         0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
            n++;

        } else {
            VFF_SET_ARGS(vfReq[n], SVN_EXE_SHIM_AFTER_FSY_VALUE, SVT_EXE_SHIM_AFTER_FSY_VALUE,
                         0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
            n++;

        }

        CVarFieldHandle* vf_handle = l_pqmstudy->GetVfDBHandle();
        int numDone = 0;
        VfPathElem_t path[1];
        path[0].subtree_name = SVN_ST_PROTOCOL;
        path[0].index = f_curpqmprotocol->GetCfaProt();
        vf_handle->GetFields(path, SVD_ST_PROTOCOL,
                             vfReq, n, (int*) &numDone);

        if (n != numDone) {
            f_curpqmprotocol->SetCfaProt(-1);
            // MEITEC/2010Apr26/Added/IR-86
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("CFA DATA COPY ERROR!!!"));
        }

        n = 0;
        VfFieldSpec_t       vfReq1[1];

        if (vfReq[0].status == E_NO_ERROR) {
            VFF_SET_ARGS(vfReq1[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE,
                         vfReq[0].size, vfReq[0].buffer, VFO_REPLACE);
            n++;
        }

        if (n) {
            time_t ts = 0L;
            VfPathElem_t path1[1];
            path1[0].subtree_name = SVN_ST_PROTOCOL;
            path1[0].index = f_curpqmprotocol->GetProtocol();
            status_t l_status = vf_handle->PutFields(path1, SVD_ST_PROTOCOL, vfReq1, n, (int*)&numDone, &ts);

            if (E_NO_ERROR != l_status || numDone != n) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            }
        }

        if (n != numDone) {
            f_curpqmprotocol->SetCfaProt(-1);
            // MEITEC/2010Apr26/Added/IR-86
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugAcqMonitor(_T("CFA DATA COPY ERROR!!!"));
        }

        if (vfReq[0].status == E_NO_ERROR && vfReq[n].size > 0)
            FREE_PTR(vfReq[0].buffer) ;

        //NFD001
        l_pqmptr->GetStudy()->UpdateSpectrumData(f_curpqmprotocol);
        //NFD001
    }

    return;
}

//****************************Method Header************************************
//Method Name   :CheckPlan()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CScanProcedure::CheckPlan(
    const CPqmProtocol* protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CheckPlan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckPlan"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();
    //    if(protocol->GetProbeScan() == VFC_PROBE_RTMC) {
    //      //DB_PRINTF("p",("pqmCheckPlan  (%s:%d) \n", __FILE__,__LINE__));
    //
    //      if ((path = getenv("GP_PRO_DIR")) != NULL) {
    //          sprintf(buf,"%s",path);
    //      } else {
    //          sprintf(buf,".");
    //      }
    //
    //      strcat(buf,PROBE_STUDY_NAME);
    //
    //      if ( l_study->StudyCheckPlan(buf) !=  E_NO_ERROR) {
    //          //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_SET_PROBE_PLAN"),ERR_ERROR);
    //          status = E_ERROR ;
    //      }
    //    }

    if (protocol->GetProbeScan() == VFC_PROBE_RTMC) {

        TCHAR l_probe_study_path[MAX_PATH] = {0}, l_path[MAX_PATH] = {0};

        if ((::GetEnvironmentVariable(_T("GP_PRO_DIR"), l_path, MAX_PATH)) != NULL)
            wsprintf(l_probe_study_path , _T("%s%s"), l_path, PROBE_STUDY_NAME);

        else
            wsprintf(l_probe_study_path, _T(".%s"), PROBE_STUDY_NAME);

        CString l_buff(l_probe_study_path);

        if (l_study->StudyCheckPlan(l_buff)  != E_NO_ERROR) {
            CString string = L"";
            string.LoadString(IDS_MSG_SET_PROBE_PLAN);

            //+Patni-Ravindra Acharya/2010May14/Modified/Redmine282
            //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MSG_SET_PROBE_PLAN, _T("IDS_MSG_SET_PROBE_PLAN"), SCAN_ERR_CRUCIAL, L"ScanProcedure.cpp");
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MSG_SET_PROBE_PLAN, _T("IDS_MSG_SET_PROBE_PLAN"), SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
            //-Patni-Ravindra Acharya/2010May14/Modified/Redmine282
            //	PqmUiErrorDialog(PqmUiFetchLiteral("MSG_SET_PROBE_PLAN"),ERR_ERROR);
            return E_ERROR ;
        }

        //- Patni-MSN/2009Dec16/Modify/RMC Positioning

    }

    BOOL status = E_NO_ERROR ;

    if (protocol->GetPrepScan() ||
        (protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP)) {

        //+ Patni-PJS/2009Aug12/Modify/MVC002490
        //l_prep_study = CPQMConfig::GetInstance()->GetPrepStudy();
        char			l_file[512] = {0};

        //+Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
        CPqmUtilities::GetPrepStudyName(l_file);
        //GetPrepStudyName(l_file);
        //-Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini

        CString l_prep_study = CString(l_file);

        //Patni-ARD/2010Mar8/Modified/Ph-3# IR-100 code review
        if ((!l_prep_study.IsEmpty()) &&
            l_study->StudyCheckPlan(l_prep_study) != E_NO_ERROR) {
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_VISUAL_PREP_POSITIONING_NOT_DONE,
                    _T("IDS_VISUAL_PREP_POSITIONING_NOT_DONE"),
                    SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
            m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
            ResetCurrentRequest();
            m_acq_manager->GetPqm()->GetStudy()->SetAcqStatus(m_acq_manager->GetScanProcedure()->GetScanProcProtocol(), C_PR_STATUS_WAIT);
            m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqm_ptr->GetAMB()->PqmPmAbortAmb();
            status = E_ERROR ;
        }

        //- Patni-PJS/2009Aug12/Modify/MVC002490
    }

    return status ;
}

//**************************************************************************
//Method Name   : PQMAcqReuseShim
//Author        : PATNI/DKH
//Purpose       : To set the shim values that are to be reused from
//                a previous protocol
//**************************************************************************
void CScanProcedure::PQMAcqReuseShim(CVarFieldHandle* const f_vf_handle, int	f_prot)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::PQMAcqReuseShim");

    //Patni-KSS/2011Feb21/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME
              , _T("Trace of CScanProcedure::PQMAcqReuseShim"));

    CPqm* l_pqmptr = m_acq_manager->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmptr pointer is NULL"));
        return;
    }

    CPQMStudy* l_pqmstudy = l_pqmptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return;
    }

    PqmShim_t		l_pqmshim;
    bzero(&l_pqmshim, sizeof(PqmShim_t));

    VfPathElem_t	l_pelm[] = {{SVN_ST_PROTOCOL, 0}, {SVN_ST_PROCESSED, 0}};
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot;
    l_pelm[SVD_ST_PROCESSED - 1].index = 1;
    PqmPrGetShimmingOffset(l_pqmstudy->GetVfDBHandle(), l_pelm, SVD_ST_PROCESSED, &l_pqmshim);

    l_pqmstudy->PutShimmingOffset(l_pqmstudy->GetVfDBHandle(), NULL, SVD_ST_STUDY, &l_pqmshim);

    CShimCtrl* l_shimctrl = GetShimCtrl() ;

    if (NULL == l_shimctrl) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_shimctrl pointer is NULL"));
        return;
    }

    l_shimctrl->SendShimOffset(&l_pqmshim);

    CPqmShimSharedData::GetInstance()->SetShimState(SHIM_STATE_ACTIVE);
    CPqmShimSharedData::GetInstance()->SetShimPath((VfPathElem_t*)l_pelm);

    CPqmProtocol*    l_pqmprotocol = l_pqmstudy->GetItemFromPosition(f_prot);

    if (l_pqmprotocol) {
        CPqmShimSharedData::GetInstance()->SetShimPos(l_pqmprotocol->GetPqmPrMode()->GetCouchPos());
    }
}

//*******************************Method Header*********************************
//Method Name    :IsCFValueChanged()
//Author         :PATNI/MSN
//Purpose        :Sets the Flag to true if CF Value is changed
//*****************************************************************************
bool CScanProcedure::IsCFValueChanged()const
{
    return m_cfvaluechanged;
}

//+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : pqmPmCoilSelCdsTimeOut
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CScanProcedure::pqmPmCoilSelCdsTimeOut(
    int sig
)
{
    //Patni-KSS/2011Feb21/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(sig);

    if (m_self_object) {
        m_self_object->pqmPmCoilSelCdsTimeOut();
    }
}
//-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   :RecalcSARControlAndAutoScanStartStatic()
//Author        :iGate
//Purpose       :
//*****************************************************************************
void CScanProcedure::RecalcSARControlAndAutoScanStartStatic(int f_sig)
{
    m_self_object->RecalcSARControlAndAutoScanStart();
}

//****************************Method Header************************************
//Method Name   :ScanContinue()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::ScanContinue(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::ScanContinue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ScanContinue"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }


    if (!m_acq_manager->GetLastTransaction()->SendControl(ACQ_START_BUTTON)) {

        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN,
        //        _T("IDS_FAIL_CONNECT_ACQMAN"),
        //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Cannot Connect to AcqMan"));

        m_acq_manager->GetPqm()->GetStudy()->SetScanModeOfHead();
        //      ResetCurrentRequest();
        return;
    }
}

//****************************Method Header************************************
//Method Name   :AcqSendMoveCouch()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcqSendMoveCouch(
    const int	f_function, const float f_position
)
{
    //    PqmAcqRequest_t	acqRequest;

    CMoveCouch* l_movecouch = new CMoveCouch(this);
    AddTransaction(l_movecouch);
    l_movecouch->SetFuntionAndPosition(f_function, f_position);
    l_movecouch->PrepareRequest();
}


//+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : pqmPmCoilSelCdsTimeOut
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CScanProcedure::pqmPmCoilSelCdsTimeOut(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::pqmPmCoilSelCdsTimeOut");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::pqmPmCoilSelCdsTimeOut"));

    if (!m_pqm_ptr) {
        return ;
    }

    CpqmCDS* l_pqm_cds = m_pqm_ptr->GetCDS();
    CPQMStudy* l_study_obj = m_pqm_ptr->GetStudy();
    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();


    if (l_pqm_cds && l_coil_data) {
        CVarFieldHandle* const l_vf_handle = l_study_obj->GetVfDBHandle();


        l_pqm_cds->pqmPmCoilSelCdsTimeOut();

        AcquisitionScanAbort();

        CPqmProtocol* l_cds_protocol = l_pqm_cds->GetCurrentCDSProtocol();

        l_study_obj->SetAcqStatus(l_cds_protocol, C_PR_STATUS_WAIT);
        l_study_obj->SetScanModeOfHead();
        ResetCurrentRequest();

        l_cds_protocol->GetCDSInfo()->m_cds_step     = CDS_STEP0_NONE;

        l_pqm_cds->PqmCdsReturnCoil(l_cds_protocol->GetSaturnCoil());

        l_study_obj->PutSaturnCoil(l_vf_handle,
                                   l_cds_protocol->GetProtocol(),
                                   l_cds_protocol->GetSaturnCoil());

        if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(l_cds_protocol->GetSaturnCoil(),
                l_cds_protocol->GetCoilDBTable())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
        }

        l_study_obj->PutConnectedCoil(l_vf_handle,
                                      l_cds_protocol->GetProtocol(),
                                      l_cds_protocol);

        l_study_obj->PutRecvrCoil(l_vf_handle,
                                  l_cds_protocol->GetProtocol(),
                                  l_cds_protocol);

        //first change the data in protocol and then update the GUI
        //Now this will update the GUI ...
        //This will again check for batch setting and then apply accordingly.
        //This will also consider the current protocol under scan
        m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Exiting CScanProcedure::pqmPmCoilSelCdsTimeOut"));
}
//-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

bool CScanProcedure::ContinueScanAfterSAESet()
{
    return ScanStart();
}

bool CScanProcedure::CanAbortProbeOnPrescanError()
{

    return (GetProbing() && IsSingleProcRunning());
}


bool CScanProcedure::IsSingleProcRunning(
) const
{
    return (1 == GetProcedureCount());
}


bool CScanProcedure::CanProceedCFARetry(
) const
{

    return ((1 == GetProcedureCount()) && GetCFARetry());
}

bool CScanProcedure::CanInvokeProbe(
    CString& f_error_message,
    bool& f_display_dialog
)
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::CanInvokeProbe");

    bool l_can_invoke_probe = !(GetCFARetry() || GetAPCRetry() || GetIsGateExecuting() || GetIsCoilOrGateInfoExecuting()) ;

    if (l_can_invoke_probe) {

        f_display_dialog = GetIsScanning() ;
        l_can_invoke_probe = !f_display_dialog ;

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("APC Or CFA is Running"));
        f_display_dialog = false;
    }

    if (!l_can_invoke_probe) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("APC Or CFA or Scan is Running"));
        f_error_message = CPqmUtilities::GetMultiLingualString(_T("IDS_MAIN_OR_REFPROBE_SCAN_GOING_ON"));
    }

    return l_can_invoke_probe;
}
//*******************************Method Header*********************************
//Method Name    :SendSAEInfo
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
bool CScanProcedure::SendSAEInfo(const int32_t f_total_sae)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SendSAEInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CSAEInfo* l_sae_info = new CSAEInfo(this);

    if (NULL == l_sae_info) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_sae_info is NULL"));
        return false;
    }

    l_sae_info->SetTotalSAE(f_total_sae);

    if (!l_sae_info->PrepareRequest()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to send AMCMM_RTSARCTRL message to Acqman"));
        return false;
    }

    return true;
}
//*******************************Method Header*********************************
//Method Name    :GetAvTimeCtrl
//Author         :PATNI/Sribanta
//Purpose        :
//*****************************************************************************
const VftTimeCtrlParam_t* CScanProcedure::GetAvTimeCtrl(
)const
{
    return &m_av_time_ctrl;
}

void CScanProcedure::SetCurrentDynamicRepeat(const int repeat)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetCurrentDynamicRepeat");

    m_current_dynamic_repeat = repeat;

    CString str_msg(_T(""));
    str_msg.Format(_T("Set to %d"), m_current_dynamic_repeat);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
}

//*******************************Method Header*********************************
//Method Name    :GetProcessing
//Author         :PATNI/GP
//Purpose        :This method gets the value for Processing
//*****************************************************************************
int CScanProcedure::GetProcessing(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetProcessing");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_processing);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_processing;
}

//*******************************Method Header*********************************
//Method Name    :SetProcessing
//Author         :PATNI/GP
//Purpose        :This method sets the value for Processing
//*****************************************************************************
void CScanProcedure::SetProcessing(
    const int processing
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetProcessing");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), processing);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_processing = processing;
}

//*******************************Method Header*********************************
//Method Name    :GetRequest()
//Author         :PATNI/MRP
//Purpose        :Returns the value for the request member variable
//*****************************************************************************
int CScanProcedure::GetRequest(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetRequest");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_request);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_request;
}

//*******************************Method Header*********************************
//Method Name    :SetRequest()
//Author         :PATNI/MRP
//Purpose        :Set's the value for the request member variable
//*****************************************************************************
void CScanProcedure::SetRequest(
    const int req
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetRequest"));
    m_request = req;
}


//****************************Method Header************************************
//Method Name   :GetScanProcProtocol()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
CPqmProtocol* CScanProcedure::GetScanProcProtocol(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetScanProcProtocol");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::GetScanProcProtocol"));
    return m_pqm_protocol;
}

//****************************Method Header************************************
//Method Name   :SetProtocolValue()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetScanProcProtocol(
    CPqmProtocol* protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetScanProcProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanProcProtocol"));

    m_pqm_protocol = protocol;
}

//+Patni-Ravindra Acharya/2010June10/Added/IR-153
//*******************************Method Header*********************************
//Method Name    :GetTempMode()
//Author         :PATNI/Ravindra Acharya
//Purpose        :Gets the Temp Mode
//*****************************************************************************
int CScanProcedure::GetTempMode()const
{
    return m_temp_mode ;
}

//*******************************Method Header*********************************
//Method Name    :SetTempMode()
//Author         :PATNI/Ravindra Acharya
//Purpose        :Sets the Temp Mode
//*****************************************************************************
void CScanProcedure::SetTempMode(const int f_temp_mode)
{
    m_temp_mode = f_temp_mode ;
}
//-Patni-Ravindra Acharya/2010June10/Added/IR-153

//****************************Method Header************************************
//Method Name   :GetScanStart()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetScanAbort(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetScanAbort");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetScanAbort"));

    return m_scan_abort;
}

//****************************Method Header************************************
//Method Name   :SetScanAbort()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetScanAbort(
    int scan_abort
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetScanAbort");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanAbort"));

    m_scan_abort = scan_abort;
}

//*******************************Method Header*********************************
//Method Name    :GetAPCHold()
//Author         :PATNI/HAR
//Purpose        :Get the value of APC Hold
//*****************************************************************************
int CScanProcedure::GetAPCHold(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAPCHold");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_apc_hold);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_apc_hold;
}

//*******************************Method Header*********************************
//Method Name    :SetAPCHold()
//Author         :PATNI/HAR
//Purpose        :Set the value of APC Hold
//*****************************************************************************
void CScanProcedure::SetAPCHold(
    const int apc_hold
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAPCHold");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), apc_hold);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_apc_hold = apc_hold;
}

//*******************************Method Header*********************************
//Method Name    :GetProbing()
//Author         :PATNI/HAR
//Purpose        :Get the value of probing
//*****************************************************************************
int CScanProcedure::GetProbing(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetProbing");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_probing);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_probing;
}

//*******************************Method Header*********************************
//Method Name    :SetProbing()
//Author         :PATNI/HAR
//Purpose        :Set the value of probing
//*****************************************************************************
void CScanProcedure::SetProbing(
    const int probing
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetProbing");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), probing);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_probing = probing;
}

//*******************************Method Header*********************************
//Method Name    :GetCFARetry()
//Author         :PATNI/HAR
//Purpose        :This method gets the value for CFARetry
//*****************************************************************************
int CScanProcedure::GetCFARetry(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetCFARetry");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_cfa_retry);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_cfa_retry;
}

//*******************************Method Header*********************************
//Method Name    :SetCFARetry()
//Author         :PATNI/HAR
//Purpose        :This method sets the value for CFARetry
//*****************************************************************************
void CScanProcedure::SetCFARetry(
    const int f_cfa_retry
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetCFARetry");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), f_cfa_retry);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    m_cfa_retry = f_cfa_retry;
}

//*******************************Method Header*********************************
//Method Name    :GetAPCRetry()
//Author         :PATNI/HAR
//Purpose        :This method gets the value for APCRetry
//*****************************************************************************
BOOL CScanProcedure::GetAPCRetry(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAPCRetry");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_apc_retry);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_apc_retry;
}

//*******************************Method Header*********************************
//Method Name    :SetAPCRetry()
//Author         :PATNI/HAR
//Purpose        :This method sets the value for APCRetry
//*****************************************************************************
void CScanProcedure::SetAPCRetry(
    const BOOL apc_retry
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAPCRetry");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), apc_retry);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    m_apc_retry = apc_retry;
}

//****************************Method Header************************************
//Method Name   :GetProt()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetProt(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetProt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetProt"));
    return m_prot;
}

//****************************Method Header************************************
//Method Name   :SetProt()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetProt(
    int prot
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetProt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProt"));

    m_prot = prot;
}

//****************************Method Header************************************
//Method Name   :GetPreScanStatus()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetPreScanStatus(
)const
{
    return m_prescan_status;
}

//****************************Method Header************************************
//Method Name   :SetPreScanStatus()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetPreScanStatus(
    int f_prescan_status
)
{
    m_prescan_status = f_prescan_status;
}

//*******************************Method Header*********************************
//Method Name    :GetIsScanning()
//Author         :PATNI/MRP
//Purpose        :Return the value for scan member variable
//*****************************************************************************
bool CScanProcedure::GetIsScanning(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetIsScanning");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_scanning);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_scanning;
}

//*******************************Method Header*********************************
//Method Name    :SetIsScanning()
//Author         :PATNI/MRP
//Purpose        :Sets the value for scan member variable
//*****************************************************************************
void CScanProcedure::SetIsScanning(
    const bool scan
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetIsScanning");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), scan);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_scanning = scan;
}

//****************************Method Header************************************
//  Method Name   :GetPrevAction()
//  Author        :PATNI/MRP
//  Purpose       :This method retuns the value of the Previous action
//*****************************************************************************
CPSAcqSetFlagVec* CScanProcedure::GetPrevAction(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetPrevAction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetPrevAction"));

    return m_prev_action;
}

//****************************Method Header************************************
//  Method Name   :SetPrevAction()
//  Author        :PATNI/MRP
//  Purpose       :This method sets the value of the Previous action
//*****************************************************************************
void CScanProcedure::SetPrevAction(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetPrevAction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetPrevAction"));
    m_prev_action = m_current_action;
}

//*******************************Method Header*********************************
//Method Name    :GetCFAClinical()
//Author         :PATNI/HAR
//Purpose        :This method returning the value of the cfa clinical.
//*****************************************************************************
bool CScanProcedure::GetCFAClinical(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetCFAClinical");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_cfa_clinical);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_cfa_clinical;
}

//*******************************Method Header*********************************
//Method Name    :SetCFAClinical()
//Author         :PATNI/HAR
//Purpose        :This method set the value of the cfa clinical.
//*****************************************************************************
void CScanProcedure::SetCFAClinical(
    const bool cfa_clinical
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetCFAClinical");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), cfa_clinical);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    m_cfa_clinical = cfa_clinical;
}

//*******************************Method Header*********************************
//Method Name    :GetPrescanHook()
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
BOOL CScanProcedure::GetPrescanHook(
)const
{
    return m_prescan_hook;

}

//*******************************Method Header*********************************
//Method Name    :SetPrescanHook()
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CScanProcedure::SetPrescanHook(
    const BOOL f_prescan_hook
)
{
    m_prescan_hook = f_prescan_hook;

}

//****************************Method Header************************************
//Method Name   :GetScanStart()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetScanStart(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetScanStart"));

    return m_scan_start;
}

//****************************Method Header************************************
//Method Name   :SetScanStart()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetScanStart(
    int scan_start
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanStart"));

    m_scan_start = scan_start;
}

//****************************Method Header************************************
//Method Name   :GetScanPause()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetScanPause(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetScanPause");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetScanPause"));

    return m_scan_pause;
}

//****************************Method Header************************************
//Method Name   :SetScanPause()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetScanPause(
    int scan_pause
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetScanPause");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanPause"));

    m_scan_pause = scan_pause;
}

//*******************************Method Header*********************************
//Method Name    :GetCurAction()
//Author         :PATNI/HAR
//Purpose        :This method retuns the value of the current prescan action
//*****************************************************************************
CPSAcqSetFlagVec* CScanProcedure::GetCurAction(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetCurAction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetCurAction"));

    return m_current_action;
}

//*******************************Method Header*********************************
//Method Name    :SetCurAction()
//Author         :PATNI/HAR
//Purpose        :This method sets the value of the current action
//*****************************************************************************
void CScanProcedure::SetCurAction(
    CPSAcqSetFlagVec* cur_action
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetCurAction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetCurAction"));

    m_current_action = cur_action;
}

//*******************************Method Header*********************************
//Method Name    :GetPrescan()
//Author         :PATNI/NJK
//Purpose        :This method returning the value of the prescan flage.
//*****************************************************************************
int CScanProcedure::GetPrescan(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetPrescan");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_prescan);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_prescan;
}

//*******************************Method Header*********************************
//Method Name    :SetPrescan()
//Author         :PATNI/NJK
//Purpose        :This method set the value of the prescan flag.
//*****************************************************************************
void CScanProcedure::SetPrescan(
    const int prescan
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetPrescan");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), prescan);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_prescan = prescan;
}

//*******************************Method Header*********************************
//Method Name    :GetAcqInfoRetry()
//Author         :PATNI/HAR
//Purpose        :Get the value of Acquisition Information Retry
//*****************************************************************************
int CScanProcedure::GetAcqInfoRetry(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAcqInfoRetry");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_acq_info_retry);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_acq_info_retry;
}

//*******************************Method Header*********************************
//Method Name    :SetAcqInfoRetry()
//Author         :PATNI/HAR
//Purpose        :Set the value of Acquisition Information Retry
//*****************************************************************************
void CScanProcedure::SetAcqInfoRetry(
    const int f_apq_info_retry
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAcqInfoRetry");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), f_apq_info_retry);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_acq_info_retry = f_apq_info_retry;
}

//*******************************Method Header*********************************
//Method Name    :GetBreathHold()
//Author         :PATNI/
//Purpose        :This method gets the value for Breath hold
//*****************************************************************************
BOOL CScanProcedure::GetBreathHold(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetBreathHold");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_breath_hold);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_breath_hold;
}

//*******************************Method Header*********************************
//Method Name    :SetBreathHold()
//Author         :PATNI/
//Purpose        :This method sets the value for Breath hold
//*****************************************************************************
void CScanProcedure::SetBreathHold(
    const BOOL breathhold
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetBreathHold");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), breathhold);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_breath_hold = breathhold;
}

//****************************Method Header************************************
//Method Name   :GetWaitForVoice()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
bool CScanProcedure::GetWaitForVoice(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetWaitForVoice");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetWaitForVoice"));

    return m_wait_for_voice;
}

//****************************Method Header************************************
//Method Name   :SetWaitForVoice()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetWaitForVoice(
    const bool waitforvoice
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetWaitForVoice");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetWaitForVoice"));

    m_wait_for_voice = waitforvoice;
}

//*******************************Method Header*********************************
//Method Name    :GetCFValue()
//Author         :PATNI/MSN
//Purpose        :Gets the Center Frequency Value
//*****************************************************************************
double CScanProcedure::GetCFValue()const
{
    return m_fsyvalue;
}

//*******************************Method Header*********************************
//Method Name    :SetCFValue()
//Author         :PATNI/MSN
//Purpose        :Sets the CF Value
//*****************************************************************************
void CScanProcedure::SetCFValue(double fsyvalue)
{
    m_fsyvalue = fsyvalue;
    m_cfvaluechanged = true;
}

//****************************Method Header************************************
//Method Name   :GetEnablePrepVoice()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CScanProcedure::GetEnablePrepVoice(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetEnablePrepVoice");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetEnablePrepVoice"));

    return m_enable_prep_voice;
}

//****************************Method Header************************************
//Method Name   :SetEnablePrepVoice()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetEnablePrepVoice(
    const BOOL enable_prep_voice
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetEnablePrepVoice");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetEnablePrepVoice"));

    m_enable_prep_voice = enable_prep_voice;
}

//****************************Method Header************************************
//Method Name   :GetWaitForPrepScan()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CScanProcedure::GetWaitForPrepScan(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetWaitForPrepScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetWaitForPrepScan"));

    return m_wait_for_prep_scan;
}

//****************************Method Header************************************
//Method Name   :SetWaitForPrepScan()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetWaitForPrepScan(
    const BOOL wait_for_prep_scan
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetWaitForPrepScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetWaitForPrepScan"));

    m_wait_for_prep_scan = wait_for_prep_scan;
}

//****************************Method Header************************************
//Method Name   :SetIsCdsWaitCursor()
//Author        :iGATE
//Purpose       :Set CDS wait cursor flag
//*****************************************************************************
void CScanProcedure::SetIsCdsWaitCursor(
    const bool f_cdswaitcursor
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetIsCdsWaitCursor");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetIsCdsWaitCursor"));

    m_cdswaitcursor = f_cdswaitcursor;
}

//*******************************Method Header*********************************
//Method Name    :GetPqmShimInfoptr()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
PqmShim_t* CScanProcedure::GetPqmShimInfoptr(
)const
{
    return m_pqmshiminfo;
}

//***************************Method Header*************************************
//Method Name    :GetCurrentTransaction()
//Author         :PATNI/SS
//Purpose        :Returns current transaction
//*****************************************************************************
CScanTransaction* CScanProcedure::GetCurrentTransaction(
)const
{
    return m_curr_transaction;
}

//***************************Method Header*************************************
//Method Name    :GetAcqManager()
//Author         :PATNI/HAR
//Purpose        :Returns the pointer of the Acqman Manager
//*****************************************************************************
CPqmPmAcqman* CScanProcedure::GetAcqManager(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAcqManager");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::GetAcqManager"));
    return m_acq_manager;
}

//*******************************Method Header*********************************
//Method Name    :GetShimCtrl()
//Author         :PATNI/HAR
//Purpose        :Gets Shim Ctrl Flag
//*****************************************************************************
CShimCtrl* CScanProcedure::GetShimCtrl(
)
{
    if (m_shim_ctrl == NULL) {
        return m_shim_ctrl = new CShimCtrl(this);

    } else {
        return m_shim_ctrl;
    }
}

//*******************************Method Header*********************************
//Method Name    : GetPqmPtr()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
CPqm* CScanProcedure::GetPqmPtr(
)
{
    return m_pqm_ptr;
}

//*******************************Method Header*********************************
//Method Name    :GetManualPreScanDefaultData
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
BOOL CScanProcedure::GetManualPreScanDefaultData(
    VARIANT** f_pdata
)
{
    //Patni-AMT/2009Jul02/Added/JFT# Manual Prescan Onsite Crash
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetManualPreScanDefaultData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetManualPreScanDefaultData"));

    if (m_acq_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_acq_manager is NULL"));
        return false;
    }

    CPqm* l_pqmptr = m_acq_manager->GetPqm();

    if (m_pqm_protocol == NULL) {
        m_pqm_protocol  = l_pqmptr->GetStudy()->GetTopWaitProtocol();

        if (m_pqm_protocol == NULL) return E_ERROR;
    }

    //+Patni-AMT/2009Jul02/Added/JFT# Manual Prescan Onsite Crash
    manual_prescan_ipc_t* l_manual_prescan_ctrl_values = new s_manual_prescan_ipc();
    memset(l_manual_prescan_ctrl_values, 0, sizeof(s_manual_prescan_ipc));
    double l_f_fsy = 0.0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    int l_rfl_action_value = 0, l_cfa_action_value = 0, l_rxgain_action_value = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    float l_f_rfl_gain = 0.0f, l_f_rfl_scale = 0.0f, l_f_rgn = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int   l_post_acq_mode = 0;
    int		l_status = 0;

    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections


    memset(mask, 0, sizeof(mask));
    l_status = l_pqmptr->GetStudy()->GetProtocolValues(l_pqmptr->GetStudy()->GetVfDBHandle(),
               m_pqm_protocol->GetProtocol(),
               mask,
               K_PR_PRESCAN_RFL, &l_rfl_action_value, NULL,
               K_PR_PRESCAN_FSY, &l_cfa_action_value, NULL,
               K_PR_PRESCAN_RGN, &l_rxgain_action_value, NULL,
               K_PR_PRESCAN_FSY_VALUE, &l_f_fsy, NULL,
               K_PR_PRESCAN_RFL_GAIN, &l_f_rfl_gain, NULL,
               K_PR_PRESCAN_RFL_SCALER, &l_f_rfl_scale, NULL,
               K_PR_PRESCAN_RGN_VALUE, &l_f_rgn, NULL,
               K_PR_POST_ACQ_MODE, &l_post_acq_mode, NULL,
               NULL);

    // Check FSY value read properly
    if (!BITFLD_ON(mask, K_PR_PRESCAN_FSY_VALUE)) {
        l_manual_prescan_ctrl_values->f_fsy = l_f_fsy;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get K_PR_PRESCAN_FSY_VALUE value from study file"));
        l_manual_prescan_ctrl_values->f_fsy = 0;
    }

    // Check RFL value read properly
    if (!BITFLD_ON(mask, K_PR_PRESCAN_RFL_GAIN)) {

        l_manual_prescan_ctrl_values->f_rfl_gain = l_f_rfl_gain;
        l_manual_prescan_ctrl_values->f_rfl_scale = l_f_rfl_scale;

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get K_PR_PRESCAN_RFL_GAIN value from study file"));
        l_manual_prescan_ctrl_values->f_rfl_gain = 0;
        l_manual_prescan_ctrl_values->f_rfl_scale = 0;
    }

    // Check RGN value read properly
    if (!BITFLD_ON(mask, K_PR_PRESCAN_RGN_VALUE)) {
        l_manual_prescan_ctrl_values->f_rgn = l_f_rgn;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get K_PR_PRESCAN_RGN_VALUE value from study file"));
        l_manual_prescan_ctrl_values->f_rgn = 0;

    }

    //-Patni-AMT/2009Jul02/Added/JFT# Manual Prescan Onsite Crash

    l_manual_prescan_ctrl_values->f_rfl_action = l_rfl_action_value;
    l_manual_prescan_ctrl_values->f_cfa_action = l_cfa_action_value;
    l_manual_prescan_ctrl_values->f_rxgain_action = l_rxgain_action_value;

    //+Patni-SS/ADDED/2009-June-01/JFT manual prescan#244
    if (l_pqmptr->GetVAP() && (l_post_acq_mode & VFC_MODE_CONCURRENT_RECON)) {
        l_manual_prescan_ctrl_values->post_acq_mode = TRUE;

    } else {
        l_manual_prescan_ctrl_values->post_acq_mode = FALSE;
    }

    //+Patni-SS/ADDED/2009-June-01/JFT manual prescan#244
    if (*f_pdata) {
        (*f_pdata)->byref = l_manual_prescan_ctrl_values;
    }

    return l_status ;


}

//****************************Method Header****************************
// Method Name  :GetPqmProbe
// Author       :PATNI/
// Purpose      :
//*********************************************************************
CPqmProbe* CScanProcedure::GetPqmProbe()
{
    if (m_pqm_ptr) {
        CPqmProcon* pqm_procon = m_pqm_ptr->GetPqmProCon();

        if (pqm_procon) {
            m_pqm_probe = pqm_procon->GetPQMProbe();
            return m_pqm_probe;
        }
    }

    return NULL;
}

//****************************Method Header************************************
//Method Name   :GetGateInfo()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
bool CScanProcedure::GetGateInfo(const int f_method)
{
    if (GetIsScanning() ||
        IsSingleProcRunning() ||
        m_pqm_ptr->GetPostProcessFlag() ||
        !SendGateModeInfo(f_method, m_pqm_ptr->GetGateParamObj()->GetWirelessGateParam())) {
        return FALSE;
    }

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :GetScanProc()
//Author         :PATNI/HAR
//Purpose        :This method gets the value for ScanProc
//*****************************************************************************
CString CScanProcedure::GetScanProc(
    const int index
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetScanProc");

    CString scan_proc = _T("");

    if (index >= 0 && index <= NUM_2) {
        scan_proc = m_scan_proc[index];
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, scan_proc);

    return scan_proc;
}

//****************************Method Header************************************
//Method Name   :GetAvAcqTime()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetAvAcqTime(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAvAcqTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetAvAcqTime"));
    return m_av_acq_time;
}

//****************************Method Header************************************
//Method Name   :GetAvTp1Duration()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetAvTp1Duration(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAvTp1Duration");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetAvTp1Duration"));

    return m_av_tp1_duration;
}

//****************************Method Header************************************
//Method Name   :GetAvTp1Ctrl()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetAvTp1Ctrl(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAvTp1Ctrl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetAvTp1Ctrl"));

    return m_av_tp1_ctrl;
}

//****************************Method Header************************************
//Method Name   :GetAvTp1Margin()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetAvTp1Margin(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetAvTp1Margin");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetAvTp1Margin"));

    return m_av_tp1_margin;
}

//****************************Method Header************************************
//Method Name   :SetControlKind()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetControlKind(
    int control_kind
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetControlKind");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetControlKind"));

    m_control_kind = control_kind;
}

//****************************Method Header************************************
//Method Name   :SetDynamicVoice()
//Author        :PATNI/ASG
//Purpose       :To set the Time control segments
//*****************************************************************************
void CScanProcedure::SetDynamicVoice(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetDynamicVoice");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetDynamicVoice"));


    //+Patni-Manish/Added On 11April2009//PSP1#301

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();

    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    CPqm* l_ppqm =  m_acq_manager->GetPqm();
    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    VftTimeCtrlParam_t  timeCtrl;
    m_acq_manager->GetPqm()->GetStudy()->GetProtocolValues(l_study->GetVfDBHandle(),
            m_pqm_protocol->GetProtocol(),
            mask,
            K_PR_TIME_CTRL_PARAM,
            &timeCtrl,
            NULL,
            NULL);

    m_acq_manager->GetPqm()->GetStudy()->GetSubProtValues(
        m_pqm_protocol->GetProtocol(),
        0,
        mask,
        K_PR_ACQ_TIME,
        &m_av_acq_time,
        NULL,
        NULL
    );
    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("ORIGINAL"));
    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    int i = 0;

    for (i = 0; i < timeCtrl.numSegment; i++) {
        CPQMDebugInfo::GetInstance(l_ppqm)->PqmDebugAcqMonitor(_T("[%d/%d] start = %d,period = %d,repeat = %d"),
                i + 1, timeCtrl.numSegment,
                timeCtrl.segment[i].start,
                timeCtrl.segment[i].period,
                timeCtrl.segment[i].repeat);
    }

    m_av_acq_time *= 1000;
    m_current_dynamic_repeat = 0;
    m_av_time_ctrl.numSegment = timeCtrl.numSegment;
    int last = 0;
    int repeat = 0;

    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    for (i = 0; i < timeCtrl.numSegment; i++) {
        m_av_time_ctrl.segment[i].start =  timeCtrl.segment[i].start - last;
        m_av_time_ctrl.segment[i].period = timeCtrl.segment[i].period - m_av_acq_time;
        repeat += timeCtrl.segment[i].repeat;
        m_av_time_ctrl.segment[i].repeat = repeat;
        last = timeCtrl.segment[i].start + m_av_acq_time + (timeCtrl.segment[i].period * (timeCtrl.segment[i].repeat - 1));
    }

    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("ACQ PARAM"));

    for (i = 0; i < timeCtrl.numSegment; i++) {
        CPQMDebugInfo::GetInstance(l_ppqm)->PqmDebugAcqMonitor(_T("[%d/%d] start = %d,period = %d,repeat = %d"),
                i + 1, timeCtrl.numSegment,
                m_av_time_ctrl.segment[i].start,
                m_av_time_ctrl.segment[i].period,
                m_av_time_ctrl.segment[i].repeat);
    }
}

//****************************Method Header************************************
//  Method Name   :SetPSData()
//  Author        :PATNI/MRP
//  Purpose       :This method sets the Prescan Data
//*****************************************************************************
bool CScanProcedure::SetPSData(
    CVarFieldHandle* const vf_handle,
    DWORD chk_bit
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetPSData");
    PQM_SCOPED_TRACE();

    UNREFERENCED_PARAMETER(vf_handle);

    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_acq_manager pointer is NULL"));
        return false;
    }

    if (m_pqm_ptr->GetAMB()->PqmAMBScanSeg2Active())
        chk_bit = PQM_CHK_BIT_AMB_SCAN ;

    CPreScan* l_pre_scan = CreatePreScan(chk_bit);
    AddTransaction(l_pre_scan);

    m_pqm_ptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
    bool l_return_val = false;

    if (l_pre_scan->PrepareRequest()) {

        l_pre_scan->SetAgentAsScan(m_pqm_protocol);
        l_return_val = (l_pre_scan->PreScanCheck() == E_NO_ERROR);
    }

    m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);

    if (m_cdswaitcursor) {
        m_cdswaitcursor = false ;
        m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PrepareRequest() failed."));
    return l_return_val;
}

//***************************Method Header*************************************
//Method Name    :SetCoilName
//Author         :PATNI/MRP
//Purpose        :This method sets the label for the transmitter coil and receiver coil
//*****************************************************************************
int CScanProcedure::SetCoilName(
    CVarFieldHandle* const vf_handle,
    CPqmProtocol* protocol,
    CCoil* rcv_coil
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetCoilName");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetCoilName"));


    //V9.25 coil support@AMT
    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_acq_manager pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    CPqmCoilData* l_pqm_coil_data = m_acq_manager->GetPqm()->GetDataManager()->GetPQMCoilData();

    if (NULL == l_pqm_coil_data) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("l_pqm_coil_data pointer is NULL"));
        return E_ERROR;
    }

    CCoil l_coil_obj;
    l_pqm_coil_data->PqmCoilGetTxCoil(rcv_coil->GetCoilKey(), l_coil_obj);
    CCoilSet coilSet;
    coilSet.SetReceiveCoil(rcv_coil);
    coilSet.SetTransmitCoil(&l_coil_obj);
    //m_acq_manager->GetPqm()->GetCoilDB()->GetTxCoil(rcv_coil->GetCoilKey()));
    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CCoil* transmitter_coil = coilSet.GetTransmitCoil();

    if (transmitter_coil == NULL) {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Transmitter coil is NULL"),
                ERROR_LOG_CRUCIAL1, _T("ScanProcedure.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        return E_ERROR;
    }

    CString label_str = transmitter_coil->GetLabel();
    int depth_size = label_str.GetLength() + 1;
    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    char* l_transmit_label = new char[depth_size];
    memset(l_transmit_label, 0, sizeof(char) * depth_size);
    wcstombs(l_transmit_label, label_str, depth_size);

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();
    l_study->SetProtocolValue(vf_handle,
                              protocol,
                              K_PR_TR_COIL_NAME, l_transmit_label, depth_size,
                              FALSE, PQM_STS_LIST);

    DEL_PTR_ARY(l_transmit_label);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    CCoil* receiver_coil = coilSet.GetReceiveCoil();

    if (receiver_coil == NULL) {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("Receiver coil is NULL"),
                ERROR_LOG_CRUCIAL1 ,
                _T("ScanProcedure.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        return E_ERROR;
    }

    label_str.Empty();
    label_str = receiver_coil->GetLabel();
    depth_size = label_str.GetLength() + 1;
    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    char* l_receive_label = new char[depth_size];
    // MEM_INI AMIT 20081230
    memset(l_receive_label , 0, sizeof(char) * depth_size) ;
    wcstombs(l_receive_label, label_str, depth_size);

    l_study->SetProtocolValue(vf_handle,
                              protocol,
                              K_PR_RC_COIL_NAME, l_receive_label, depth_size,
                              FALSE, PQM_STS_LIST);

    DEL_PTR_ARY(l_receive_label);

    return E_NO_ERROR;
}

//*******************************Method Header*********************************
//Method Name    :SetCouchInfo()
//Author         :PATNI/
//Purpose        :This method sets the value for couch info
//*****************************************************************************
void CScanProcedure::SetCouchInfo(
    const int position,
    float couch_position,
    int couch_status
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetCouchInfo");

    CString str_msg(_T(""));
    str_msg.Format(_T("%f,%d"), couch_position, couch_status);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CPQMStudy* study = m_acq_manager->GetPqm()->GetStudy();
    //CPqmProtocol* protocol = study->GetItemFromPosition(position);

    // + SHIMMING Lokesh 15 March 2008
    float home_position = 0.0f ; //= couch_position; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    CPqmShimSharedData::GetInstance()->ShimRegistCouchInfo(&couch_status, & couch_position, &home_position);

    if (!study->WriteToShimData()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Unable to write updated shim values in shimdata file"));
    }

    // - SHIMMING Lokesh 15 March 2008
    // + SM4 Himanshu 10 Dec 2008
    CString l_print_str(_T(""));
    l_print_str.Format(_T("Scan Number = %d, Couch Position = %f, Home Position = %d"),
                       couch_status, couch_position, home_position);

    //Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
    m_acq_manager->GetPqm()->GetStudyManagerPtr()->AddAcqConAndHomePosAtStudyLevel(home_position, couch_status);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    CPqmProtocol* l_from_protocol = study->GetProtocol(position);
    const int l_start_pos = l_from_protocol->GetPosition();
    int l_end_pos = study->GetAllItems() + 1;

    if (l_from_protocol) {
        if (m_pqm_ptr->GetAMB()->PqmAMBScanSeg2Active()) {
            l_end_pos =  l_start_pos + 1;
        }
    }


    CPqmProtocol* l_to_protocol = NULL;

    for (int i = l_start_pos ; i < l_end_pos; i++) {
        l_to_protocol = study->GetItemFromPosition(i) ;

        if (l_to_protocol == NULL || l_to_protocol->GetProtocol() == PQ_FILLER) {
            continue;
        }

        l_to_protocol->GetPqmPrMode()->SetCouchPos(couch_position) ;
        l_to_protocol->GetPqmPrMode()->SetScanNumber(couch_status);
        l_to_protocol->GetPqmPrMode()->SetHomePos(home_position) ;

        //Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
        m_acq_manager->GetPqm()->GetStudyManagerPtr()->AddAcqConAndHomePosAtSeriesLevel(
            l_to_protocol->GetSeriesLOID(), home_position, couch_status);

        BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        memset(mask, 0, sizeof(mask));

        if (E_NO_ERROR != study->SetProtocolValues(study->GetVfDBHandle(),
                l_to_protocol->GetProtocol(),
                mask,
                K_PR_SCAN_NUMBER,
                &couch_status,
                sizeof(int),
                K_PR_COUCH,
                &couch_position,
                sizeof(float),
                K_PR_HOME_POSITION,
                &home_position,
                sizeof(float),
                NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }
    }

    // - SM4 Himanshu 10 Dec 2008
    study->SearchScanMode(study->GetVfDBHandle(), K_MODE_SET_IT);
}


void CScanProcedure::SetIsCoilOrGateInfoExecuting(
    const bool f_is_coil_or_gateinfo_executing
)
{
    m_is_coil_or_gateinfo_executing = f_is_coil_or_gateinfo_executing ;
}


void CScanProcedure::SetIsGateExecuting(
    const bool f_is_gate_executing
)
{
    m_is_gate_executing = f_is_gate_executing  ;
}
//*******************************Method Header*********************************
//Method Name    :GetScanMode
//Author         :PATNI/AKR
//Purpose        :Get the scan mode
//*****************************************************************************
PQM_SCAN_MODE CScanProcedure::GetScanMode() const
{
    return m_scan_mode;
}
//*******************************Method Header*********************************
//Method Name    :SetScanMode
//Author         :PATNI/AKR
//Purpose        :Set the scan mode
//*****************************************************************************
void CScanProcedure::SetScanMode(const PQM_SCAN_MODE f_scan_mode)
{
    m_scan_mode = f_scan_mode;
}
//***************************Method Header*************************************
//Method Name    :CScanProcedure()
//Author         :PATNI/MRP
//Purpose        :Parameterized Constructor
//*****************************************************************************
CScanProcedure::CScanProcedure(
    CPqmPmAcqman* acq_manager) :
    m_pqm_ptr(NULL),
    m_acq_manager(NULL),
    m_prev_action(new CPSAcqSetFlagVec),
    m_current_action(new CPSAcqSetFlagVec),
    m_pqm_protocol(NULL),
    m_last_acquisition(new CPqmPrMode()),
    m_curr_transaction(NULL),
    m_shim_ctrl(NULL),
    m_olp_temp(NULL),
    m_is_coil_or_gateinfo_executing(false),
    m_is_gate_executing(false),
    m_scanning(false),
    m_cfa_retry(false),
    m_cfa_clinical(false),
    m_apc_retry(false),
    m_wait_for_voice(false),
    m_request(0),
    m_apc_hold(0),
    m_probing(0),
    m_prot(-1),
    m_prescan(0),
    m_processing(0),
    m_acq_info_retry(0),
    m_breath_hold(false),
    m_temp_mode(0),
    m_av_acq_time(0),
    m_av_tp1_duration(NUM_100),
    m_av_tp1_margin(0),
    m_av_tp1_ctrl(0),
    m_current_dynamic_repeat(0),
    m_wait_for_prep_scan(FALSE),
    m_enable_prep_voice(FALSE), //Patni-HAR/2010May14/Added/TMSC-REDMINE-283
    m_control_kind(0),
    m_scan_start(0),
    m_scan_abort(0),
    m_scan_pause(0),
    m_fsyvalue(0.0f),
    m_prescan_status(0),
    m_cfvaluechanged(false),
    m_pqmshiminfo(NULL),
    m_prescan_hook(FALSE),
    m_pqm_probe(NULL),
    m_scan_mode(PQM_PROTOCOL_SCAN),
    m_sar_msg_bit(0xFFFF),
    m_cdswaitcursor(false),
    m_10sec_over_wait_count(0),
    m_is_waiting_for_scan_sarctrl(false)
{
    //    m_acqseries(NULL) {

    LPCTSTR FUNC_NAME = _T("CScanProcedure::CScanProcedure");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::CScanProcedure"));

    if (acq_manager) {
        m_acq_manager = acq_manager;
        m_pqm_ptr = acq_manager->GetPqm();
    }

    m_scan_proc.push_back(_T(""));
    m_scan_proc.push_back(_T(""));
    m_scan_proc.push_back(_T(""));


    //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
    m_self_object = this ;
    //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Ravindra Acharya/2010June25/Added/IR-153
}

//***************************Method Header*************************************
//Method Name    :~CScanProcedure()
//Author         :PATNI/MRP
//Purpose        :Destructor
//*****************************************************************************
CScanProcedure::~CScanProcedure(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::~CScanProcedure");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::~CScanProcedure"));

    DeleteAllScanTransactionObjs();

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    DEL_PTR(m_pqm_probe);
    DEL_PTR(m_shim_ctrl);
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
}

//****************************Method Header************************************
//  Method Name   :CreatePreScan()
//  Author        :PATNI/MRP
//  Purpose       :This method creates the Prescan transaction depending on the
//                  check bit value
//*****************************************************************************
CPreScan* CScanProcedure::CreatePreScan(
    DWORD chk_bit
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CreatePreScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CreatePreScan"));

    CPreScan* pre_scan = NULL;

    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CString chk_str = _T("");

    if (NULL == GetAcqManager()->GetPqm()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CPQMDebugInfo pointer is NULL"));
        return pre_scan;
    }


    if (GetAPCRetry()) {

        pre_scan = new CPreScanAPC(this);
        SetPrescan(K_PRESCAN_MODE_APC);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_APC"));

    } else if (GetCFARetry()) {

        pre_scan = new CPrescanCFARetry(this);
        SetPrescan(K_PRESCAN_MODE_CFA);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_CFA"));

    } else if (chk_bit & PQM_CHK_BIT_AMB_SCAN) {

        pre_scan = new CPreScanAMB(this);
        SetPrescan(K_PRESCAN_MODE_AMB);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_AMB"));
        //+Patni-DKH/2010Feb18/Modified/Phase3#MCM0229-00187
    }

    //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    else if (chk_bit & PQM_CHK_BIT_CDS) {
        //PqmAcqGlobal.req.prescan = K_PRESCAN_MODE_CDS;
        pre_scan = new CCDSPrescan(this);
        SetPrescan(K_PRESCAN_MODE_CDS);

        PQM_TRACE(USER_FUNC_MARKER, _T("CScanProcedure::CreatePreScan"),
                  _T("------> prescan = K_PRESCAN_MODE_CDS"));
        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

    } else if (chk_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_COIL)) {//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187

        pre_scan = new CPreScanFirst(this);
        SetPrescan(K_PRESCAN_MODE_1ST_SCAN);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_1ST_SCAN"));

        chk_str = chk_str + FIRST;  //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    }

    //Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
    //else if (chk_bit & (PQM_CHK_BIT_ANATOMY | PQM_CHK_BIT_COIL)) {

    else if (chk_bit & (PQM_CHK_BIT_ANATOMY | PQM_CHK_BIT_RFL_TABLE)		&&
             chk_bit & PQM_CHK_BIT_CFA_TABLE) {
        pre_scan = new CPreScanCoil(this);
        SetPrescan(K_PRESCAN_MODE_CFA_RFL);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_CFA_RFL"));
        chk_str = chk_str +  COIL;    //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    } //else if (chk_bit & PQM_CHK_BIT_TX_TABLE) {

    else if (chk_bit & (PQM_CHK_BIT_ANATOMY | PQM_CHK_BIT_RFL_TABLE)) {

        //+Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
        //pre_scan = new CPreScanTx(this);
        //SetPrescan(K_PRESCAN_MODE_RFL_TABLE);
        pre_scan = new CPreScanRFL(this);
        SetPrescan(K_PRESCAN_MODE_RFL);
        //-Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_RFL_TABLE"));

    } else if (chk_bit & PQM_CHK_BIT_CFA_TABLE /*PQM_CHK_BIT_RFL_TABLE*/) {

        //+Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
        //pre_scan = new CPreScanRFL(this);
        pre_scan = new CPreScanCFA(this);
        //+Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
        SetPrescan(K_PRESCAN_MODE_CFA_TABLE);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_CFA_TABLE"));

        chk_str = chk_str + CFA_TABLE;//Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    }

    //+Patni-Meghna-DKH/2010Apr12/Commented/Phase3#MCM0229-00187
    else {
        pre_scan = new CPreScanModeAll(this);
        SetPrescan(K_PRESCAN_MODE_EVERY);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("------> prescan = K_PRESCAN_MODE_EVERY"));
    }

    //-Patni-DKH/2010Feb18/Modified/Phase3#MCM0229-00187
    //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqm_ptr)->  PqmDebugAcqMonitor(_T("CHK BIT = %X : %s"), chk_bit, chk_str);

    return pre_scan;
}

//****************************Method Header************************************
//Method Name   :ScanCancelCB()
//Author        :PATNI/NJK
//Purpose       :This is call back function to cancel scan after popup
//               warning message in GUI.
//******************************************************************************/
void CScanProcedure::ScanCancelCB(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::ScanCancelCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ScanCancelCB"));

    //Patni-HAR/2010Jan15/Added/DeFT#MVC006728
    GetAcqManager()->AcqDeleteLockFile();
    ResetCurrentRequest();
    CPqm* pqm  = GetAcqManager()->GetPqm();
    pqm->GetStudy()->SetScanModeOfHead();

}

//****************************Method Header************************************
//Method Name   :ForceParentCB()
//Author        :PATNI/NJK
//Purpose       :This is call back function to start scan after popup
//               warning message in GUI. This method to contine the scan
//               process after warning.
//******************************************************************************/
void CScanProcedure::ForceParentCB(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::ForceParentCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ForceParentCB"));


    CPqmProtocol* l_curr_prot = m_pqm_protocol;//acqman->GetProtocol();
    CPQMStudy* l_study = GetAcqManager()->GetPqm()->GetStudy();

    //+Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion
    l_study->InitPatientPath(l_curr_prot);

    int l_mode = PQM_INFO_BIT_COILID | PQM_INFO_BIT_COUCH |
                 PQM_INFO_BIT_RM;

    int l_shimstate = 0;
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shimstate);

    int l_processing = 0;
    l_processing = GetProcessing();
    l_processing &= ~PQM_ACQ_BIT_WARNING;


    if (l_shimstate == SHIM_STATE_ACTIVE) {
        //+Patni-MP/2009Sep18/Added/MVC004774
        PqmShim_t* l_shim = NULL;
        l_shim = l_study->SetShimmingOffset(PQM_SHIM_RESET, -1);
        SendShimCtrl(l_shim);
        SetProcessing(l_processing);
        //-Patni-MP/2009Sep18/Added/MVC004774
        SetAcqInfoRetry(l_mode);

    } else {
        //+Patni-MP/2009Sep18/Added/MVC004774
        SetProcessing(l_processing);
        //-Patni-MP/2009Sep18/Added/MVC004774
        CAcqInfo* acqinfo = new CAcqInfo(this);

        AddTransaction(acqinfo);

        acqinfo->SetAcqInfoMode(l_mode);
        acqinfo->PrepareRequest();
        //-Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion
    }
}

//-Patni-MP/2009Jun07/Added/ACE-2/Internal defect fix
//********************************Method Header********************************
//Method Name   : ForceCoilStatusCB()
//Author        : PATNI/MRP
//Purpose       :
//*****************************************************************************
void CScanProcedure::ForceCoilStatusCB(
)
{
    int l_processing = GetProcessing();
    l_processing &= ~PQM_ACQ_BIT_WARNING;
    SetProcessing(l_processing);

    CAcqInfo* acqinfo = new CAcqInfo(this);

    AddTransaction(acqinfo);

    int l_mode = PQM_INFO_BIT_COUCH | PQM_INFO_BIT_RM;
    acqinfo->SetAcqInfoMode(l_mode);
    acqinfo->PrepareRequest();
    //-Patni-MP/2009Jun07/Added/ACE-2/Internal defect fix
}

//***************************Method Header*************************************
//Method Name    :DeleteAllScanTransactionObjs()
//Author         :PATNI/MRP
//Purpose        :
//*****************************************************************************
void CScanProcedure::DeleteAllScanTransactionObjs()
{
    UTIL::DeleteAndClearContainer(&m_acqscan_transaction);

    m_curr_transaction = NULL;

    if (m_acq_manager) {
        m_acq_manager->SetLastTransaction(NULL);
    }
}

//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmAcqAfterCDSScanStart
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CScanProcedure::PqmAcqAfterCDSScanStart(
    CPqmProtocol* f_cds_protocol
)const
{
    //DB_FUNC_ENTER("[PqmAcqAfterCDSScanStart]:");
    LPCTSTR FUNC_NAME = _T("CScanProcedure::PqmAcqAfterCDSScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CScanProcedure::PqmAcqAfterCDSScanStart"));

    CPqmProtocol* l_protocol_to_scan = f_cds_protocol ;

    CPqmPmAcqman* acqman = GetAcqManager();

    if (!f_cds_protocol)	{

        l_protocol_to_scan = m_pqm_protocol;
    }

    //+Patni-Hemant/2010Aug10/Commented/MVC008295
    ////+Patni-HAR/2010Jun30/Added/V1.30#MVC008295
    //if (l_protocol_to_scan->GetASCOMPASSFlag()) {
    //    m_pqm_ptr->GetCDS()->PqmCdsBackUpCoil(l_protocol_to_scan->GetSaturnCoil());
    //}
    ////-Patni-HAR/2010Jun30/Added/V1.30#MVC008295
    //-Patni-Hemant/2010Aug10/Commented/MVC008295

    acqman->ScanStart(l_protocol_to_scan, FALSE);

}
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnCoilSelectionCancel
// Author       : PATNI/ HEMANT
// Purpose      : This function is called after user has pressed Cancel button on coil selection
//                dialog while CDS.
//***********************************************************************************
void CScanProcedure::OnCoilSelectionCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::OnCoilSelectionCancel");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CScanProcedure::OnCoilSelectionCancel"));

    CpqmCDS* l_pqm_cds = m_pqm_ptr->GetCDS();
    CPqmProtocol* l_cds_protocol = l_pqm_cds->GetCurrentCDSProtocol();

    if (l_cds_protocol && l_pqm_cds->GetCDSCoilSelStatus() == CDS_ON) {
        //obj = (PqmPrData_t *)*(obj_list + 1);
        CpqmCDSInfo* l_cds_info = l_cds_protocol->GetCDSInfo();
        CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();
        CVarFieldHandle* const l_vf_handle = l_study->GetVfDBHandle();
        CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

        l_pqm_cds->SetCDSCoilSelStatus(CDS_NONE);

        l_cds_info->m_cds_step   = CDS_STEP0_NONE;
        l_cds_info->m_cds_rescan = FALSE;

        /* CDS information is invalid*/
        memset(l_cds_info->m_coordinatetbl, 0 , sizeof(Vft_CdsCoilLocation_t)*PQM_NUM_COIL_PORT);
        memset(l_cds_info->m_locationtbl, 0, sizeof(Vft_CdsCoilLocation_t)*PQM_NUM_COIL_PORT);

        /* Wrote the study */

        CCDSUtilities::PutCdsCoordinate(l_vf_handle,
                                        l_cds_protocol->GetProtocol(),
                                        l_cds_info->m_coordinatetbl);

        AcquisitionScanAbort();

        l_study->SetAcqStatus(l_cds_protocol, C_PR_STATUS_WAIT);
        l_study->SetScanModeOfHead();
        ResetCurrentRequest();

        l_pqm_cds->PqmCdsReturnCoil(l_cds_protocol->GetSaturnCoil());
        l_study->PutSaturnCoil(l_vf_handle,
                               l_cds_protocol->GetProtocol(),
                               l_cds_protocol->GetSaturnCoil());

        if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(l_cds_protocol->GetSaturnCoil(), l_cds_protocol->GetCoilDBTable())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
        }

        l_study->PutConnectedCoil(l_vf_handle, l_cds_protocol->GetProtocol(), l_cds_protocol);
        l_study->PutRecvrCoil(l_vf_handle, l_cds_protocol->GetProtocol(), l_cds_protocol);

        //+Patni-Hemant/2010Aug19/Added/MVC008295
        //When pressing Cancel for CDS coil confirmation,
        //Coil name also should be restored in Database
        if (m_pqm_ptr->GetCoilSelectionTool()) {
            if (!m_pqm_ptr->GetCoilSelectionTool()->CSSetCoilName(l_cds_protocol, l_vf_handle)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
            }
        }

        //-Patni-Hemant/2010Aug19/Added/MVC008295

        //first change the data in protocol and then update the GUI
        //Now this will update the GUI ...
        m_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

    } else {
        if (m_pqm_ptr->PqmPmIsActive(K_PM_TYPE_AMB))
            m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

    }

    l_pqm_cds->PqmCoilSelEnd(l_cds_protocol);
}

//************************************Method Header************************************
// Method Name  : OnCoilSelectionOK
// Author       : PATNI/ HEMANT
// Purpose      : This function is called after user has pressed OK button on coil selection
//                dialog while CDS.
//                Before calling this function, hanged coil is upated in protocols
//                (GUI and database).
//***********************************************************************************
void CScanProcedure::OnCoilSelectionOK(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::OnCoilSelectionOK");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanProcedure::OnCoilSelectionOK"));

    CpqmCDS* l_pqm_cds = m_pqm_ptr->GetCDS();
    CPqmProtocol* l_cds_protocol = l_pqm_cds->GetCurrentCDSProtocol();

    if (l_cds_protocol && l_pqm_cds->GetCDSCoilSelStatus() == CDS_ON) {

        //obj = (PqmPrData_t *)*(obj_list + 1);
        CpqmCDSInfo* l_cds_info = l_cds_protocol->GetCDSInfo();
        CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();
        CVarFieldHandle* const l_vf_handle = l_study->GetVfDBHandle();


        l_pqm_cds->SetCDSCoilSelStatus(CDS_NONE);


        if (CDS_STEP5_SCANDONE == l_cds_info->m_cds_step) {
            m_cdswaitcursor = true;
            m_pqm_ptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
        }

        l_cds_info->m_cds_step = CDS_STEP6_DONE;	/* Automatic coil configured to CDS_STEP6_DONE*/


        /* CDS wrote to study the status */
        CCDSUtilities::PutCdsStatus(l_vf_handle, l_cds_protocol->GetProtocol(), &l_cds_info->m_cds_status);
        PqmAcqAfterCDSScanStart(l_cds_protocol);
    }

    l_pqm_cds->PqmCoilSelEnd(l_cds_protocol);
}
//-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

int CScanProcedure::GetProcedureCount(
) const
{
    return (GetProbing() ? 1 : 0)
           + (GetAPCRetry() ? 1 : 0)
           + (GetCFARetry() ? 1 : 0)
           + (GetIsGateExecuting() ? 1 : 0
              + (GetIsCoilOrGateInfoExecuting() ? 1 : 0));
}

bool CScanProcedure::GetIsCoilOrGateInfoExecuting(
) const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetIsCoilOrGateInfoExecuting");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_is_coil_or_gateinfo_executing);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    return m_is_coil_or_gateinfo_executing;
}


bool CScanProcedure::GetIsGateExecuting(
) const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetIsGateExecuting");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), m_is_gate_executing);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_is_gate_executing;
}

//****************************Method Header*****************************************
//Method Name   :AcqGetPVcoilIDTable
//Author        :PATNI/LK
//Purpose       :This function gets the PV coil table information from PVCoilID file
//**********************************************************************************
int CScanProcedure::AcqGetPVcoilIDTable(
    int* f_coilkey,
    int* f_pvcoilnum
)
{
    //Patni-AMT/2010Mar10/Modified/DeFT# IR-100 Code Review-COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcqGetPVcoilIDTable");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcqGetPVcoilIDTable "));

    CString         l_systemname = L"";
    CString         l_errstring = L"";

    // Get the system Name Not in this scope

    /*
        status = getSystemName(&mInfo);
        if(status){
            return status;
        }
    */
    //Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
    if (NULL != m_pqm_ptr)
        l_systemname = m_pqm_ptr->GetSysName();

    //+Patni-AMT/2010Mar10/Modified/DeFT# IR-100 Code Review
    if (l_systemname.GetLength() <= 0) {

        l_errstring.LoadString(IDS_SYSTEMNAME_NOTFOUND);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errstring);
        CPQMLogMgr::GetInstance()->WriteEventLog(l_errstring, ERROR_LOG_CRUCIAL1,
                _T("ScanProcedure.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SYSTEMNAME_NOTFOUND")),
            _T("ScanProcedure.cpp"), __LINE__,
            ERROR_LOG_CRUCIAL1, ERR_ID, APP_NAME);

        return E_ERROR;
    }

    //-Patni-AMT/2010Mar10/Modified/DeFT# IR-100 Code Review

    char l_filebuf_content[FILE_BUFFER] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    memset(l_filebuf_content, NULL, sizeof(l_filebuf_content));

    char*	l_mrmp_data_env = getenv("MRMP_DATA");
    sprintf(l_filebuf_content, "%s%s", l_mrmp_data_env, "\\PVCoil\\PVCoilID");

    FILE*           l_pvcoil_file = NULL;
    l_pvcoil_file = fopen(l_filebuf_content, "r");

    if (!l_pvcoil_file) {
        l_errstring.LoadString(IDS_ERR_PVCOIL_FILE);


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errstring);
        CPQMLogMgr::GetInstance()->WriteEventLog(l_errstring, ERROR_LOG_INFORMATION1,
                _T("PQMIPCManager.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        CPQMLogMgr::GetInstance()->DisplayError(CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_PVCOIL_FILE")),
                                                _T("CScanProcedure.cpp"), __LINE__, ERROR_LOG_INFORMATION1 , ERR_ID, APP_NAME);
        return E_ERROR;
    }

    char*           l_token1 = NULL;
    char*           l_token2 = NULL;
    int             l_coilid = 0;
    int             l_count = 0;

    while (fgets(l_filebuf_content, FILE_BUFFER, l_pvcoil_file)) {

        l_token1 = strrchr(l_filebuf_content, '\n');

        if (l_token1) {
            *l_token1 = '\0';
        }

        l_token1 = strtok(l_filebuf_content, "#");

        if ((l_token1 == l_filebuf_content) && (strlen(l_token1) > 1)) {
            l_token2 = strtok(l_token1, "\t ");

            if (l_token2) {
                CString l_tokenstr(l_token2);

                if (!(l_systemname.CompareNoCase(l_tokenstr))) {
                    for (int i = 0 ; i < NUM_3 ; i++) {
                        l_token2 = strtok(NULL, "\t ");

                        if (l_token2 == NULL) {
                            break ;
                        }

                        sscanf(l_token2, "%x", &l_coilid);
                        *(f_coilkey + l_count) = l_coilid ;
                        l_count++ ;
                    }
                }
            }
        }
    }

    fclose(l_pvcoil_file);
    *f_pvcoilnum = l_count;
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :PrepDelayDelete
//Author        :PATNI/LK
//Purpose       :This function deletes prep delay
//*****************************************************************************
void CScanProcedure::PrepDelayDelete(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::PrepDelayDelete");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PrepDelayDelete"));

    //+Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
    //l_prepstudyfile = CPQMConfig::GetInstance()->GetPrepStudy();
    char	l_prep_study_file[512] = {0};
    CPqmUtilities::GetPrepStudyName(l_prep_study_file);

    //TMSC-Tanoue/2010Jun18/Modified/MVC008345. Use CVarFieldHandle
    CVarFieldHandle vf_handle(l_prep_study_file);

    if (vf_handle.Open() != E_NO_ERROR) {
        fprintf(stderr, "error in VfOpen\n");
        return;
    }

    VfFieldSpec_t   l_vf_req[NUM_2] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int             l_count = 0;
    int             l_number = 0;
    int             l_prep_delay = 0;
    VFF_SET_ARGS(l_vf_req[l_count], SVN_EXE_PREP_SCAN_DELAY_TIME, VFT_INT32 ,
                 sizeof(l_prep_delay), &l_prep_delay, VFO_REPLACE);
    l_count++;

    VfPathElem_t    l_path[] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };
    time_t          l_time = 0L; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    vf_handle.PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req, l_count, &l_number, &l_time);
}

//+Patni-ARD/2009Sep15/Added/Pump freezer confirmation
//********************************Method Header********************************
//Method Name   : ForceRMStatusCB()
//Author        : PATNI/ARD
//Purpose       :
//*****************************************************************************
void CScanProcedure::ForceRMStatusCB()
{
    int l_processing = GetProcessing();
    l_processing &= ~PQM_ACQ_BIT_WARNING;
    SetProcessing(l_processing);

    CAcqInfo* acqinfo = new CAcqInfo(this);

    AddTransaction(acqinfo);

    int l_mode = PQM_INFO_BIT_COILID | PQM_INFO_BIT_COUCH;

    acqinfo->SetAcqInfoMode(l_mode);
    acqinfo->PrepareRequest();

}
//-Patni-ARD/2009Sep15/Added/Pump freezer confirmation

//Patni-SS/Added/2006June20/MVC Shimming reset on Projection change
void CScanProcedure::pqmForceParentCB()
{
    int		shimState = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int l_processing = 0;
    CPqmShimSharedData::GetInstance()->GetShimState(&shimState) ;

    if (shimState == SHIM_STATE_ACTIVE) {
        m_pqm_ptr->GetStudy()->SetShimmingOffset(PQM_SHIM_RESET, -1);
        \
        l_processing = GetProcessing();
        l_processing &= ~PQM_ACQ_BIT_WARNING;
        SetProcessing(l_processing);
        /* もう一度AcqInfoを実行しないと… */
        SetAcqInfoRetry(PQM_INFO_BIT_COILID |
                        PQM_INFO_BIT_COUCH |
                        PQM_INFO_BIT_RM);

    } else {
        l_processing = GetProcessing();
        l_processing &= ~PQM_ACQ_BIT_WARNING;
        SetProcessing(l_processing);

        /*PqmAcqGetInfo(PqmAcqGlobal.curObj,
              PQM_INFO_BIT_COILID | PQM_INFO_BIT_COUCH |
              PQM_INFO_BIT_RM);*/
        CAcqInfo* acqinfo = new CAcqInfo(this);

        AddTransaction(acqinfo);

        int l_mode = PQM_INFO_BIT_COILID | PQM_INFO_BIT_COUCH |
                     PQM_INFO_BIT_RM;

        acqinfo->SetAcqInfoMode(l_mode);
        acqinfo->PrepareRequest();
    }
}
//Patni-SS/Added/2006June20/MVC Shimming reset on Projection change

//*******************************Method Header*********************************
//Method Name    :ReadScanProc()
//Author         :PATNI/HAR
//Purpose        :This method reads the scan procedure from the given files
//*****************************************************************************
int CScanProcedure::ReadScanProc(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::ReadScanProc");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ReadScanProc"));


    for (int i = 0; i < NUM_3; i++) {
        if (!m_scan_proc[i].IsEmpty()) {
            m_scan_proc[i].Empty();
        }
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    :CopyTuneInfo
//Author         :PATNI/MRP
//Purpose        :Dummy function which returns the tune info. These variables
//                will be read from the shared memory.
//*****************************************************************************
int CScanProcedure::CopyTuneInfo(
    float& txtunePos,
    float& rflPos
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CopyTuneInfo");
    txtunePos = PQM_COUCH_INIT_POSITION;
    rflPos = PQM_COUCH_INIT_POSITION;

    // parser changes start

    CPTString file_name;
    CPQMXMLParser* ipcparser = NULL;

    ipcparser = new CPQMXMLParser();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //wchar_t buffer[MAX_PATH];
    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    file_name  = buffer;
    file_name += L"\\";
    file_name += L"PQM\\";
    file_name += SHARED_MEMORY_TUNE_INFO_FILENAME;

    // load xml file
    if (ipcparser->LoadXMLFile(file_name)) {
        // file contains only one node so get data from 0th position
        CPTString txtunePosstr = ipcparser->GetAttributeValue(0, NODE_TAG_NAME_TUNE_INFO, CHILD_TAG_NAME_TXTUNEPOS, ATTRIBUTE_NAME_1);
        CPTString rflposstr =  ipcparser->GetAttributeValue(0, NODE_TAG_NAME_TUNE_INFO, CHILD_TAG_NAME_RFLPOS, ATTRIBUTE_NAME_1);

        txtunePos = ipcparser->ReadFloatValue(txtunePosstr);
        rflPos = ipcparser->ReadFloatValue(rflposstr);

        DEL_PTR(ipcparser);

        // parser changes end

    } else {
        return E_ERROR;
    }

    int status = E_NO_ERROR;
    CString str_msg(_T(""));
    str_msg.Format(_T("%f, %f %d"), txtunePos, rflPos, status);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return status;
}

//***************************Method Header*************************************
//Method Name    :CopyCouchInfo
//Author         :PATNI/MRP
//Purpose        :Dummy function which returns the couch info. These variables
//                will be read from the shared memory.
//*****************************************************************************
int CScanProcedure::CopyCouchInfo(
    int& scanNumber,
    float& couchPos,
    float& homePos
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CopyCouchInfo");
    //  - Implement
    //  if(pqmShmStart() != E_NO_ERROR)
    //  return E_ERROR;
    //
    //    *scanNumber = pqmShm_p->scanNumber;
    //    *couchPos   = pqmShm_p->couchPos;
    //    *homePos    = pqmShm_p->homePos;
    //
    //    return pqmShmEnd();
    //  - Implement
    //    CPQMTraceInfo::GetInstance()->LogTrace(
    //            CTraceInfo::L2,
    //            _T("CScanProcedure::CopyCouchInfo()"));
    //
    //    int status = E_NO_ERROR;
    //
    //    MPlus::Conf::CConfVList* portion_info = NULL;
    //    MPlus::Conf::CConfMgr* manager = NULL;
    //
    //    CString portion_tag = CString(_T(""));
    //    CPQMXMLParser parser;
    //    CString couchinfo = (_T("shared_couch"));
    //
    //    m_pqm_ptr = m_acq_manager->GetPqm();
    //    try {
    //        manager = parser.CreateManager(couchinfo);
    //
    //        if (manager != NULL) {
    //            CString portion_tag = CString(_T("/CouchInformation"));
    //
    //            portion_info = manager->SelectNodes(
    //                                        portion_tag,
    //                                        MPlus::Conf::ManualUpdate,
    //                                        NULL);
    //            DWORD item_count = portion_info->Size();
    //
    //            if (item_count == 0) {
    //                // { Prasad 12-May-2006 , VSS code reusability optimisation
    //                PQM_ERROR2(IDS_ERR_NO_COUCHINFO,CErrorInfo::ERR_SEVERITY::EI_ERROR,CErrorInfo::ERR_TYPE::ET_USER);
    //                // } Prasad 12-May-2006 , VSS code reusability optimisation
    //                status = E_ERROR;
    //            } else {
    //                MPlus::Conf::CConfV* current_portion = NULL;
    //                current_portion = portion_info->Item((DWORD)0);
    //
    //                if (current_portion) {
    //                    scanNumber = parser.ReadLongValue(current_portion, IDS_SCAN_NUMBER);
    //                    couchPos = parser.ReadDoubleValue(current_portion, IDS_COUCH_POS);
    //                    homePos = parser.ReadDoubleValue(current_portion, IDS_HOME_POS);
    //                }
    //            }
    //        } else {
    //            PQM_ERROR2(IDS_ERR_NO_COUCHINFO,
    //                        CErrorInfo::ERR_SEVERITY::EI_ERROR,
    //                        CErrorInfo::ERR_TYPE::ET_USER);
    //            CString error_str = _T("");
    //            error_str.LoadString(IDS_ERR_OCCURRED);
    //            m_pqm_ptr->WriteToUI(error_str);
    //            m_pqm_ptr->DisplayError(IDS_ERR_NO_COUCHINFO,MB_SYSTEMMODAL);
    //            return E_ERROR;
    //        }
    //    } catch (...) {
    //        // { Prasad 12-May-2006 , VSS code reusability optimisation
    //        PQM_ERROR2(IDS_ERR_NO_COUCHINFO,
    //                        CErrorInfo::ERR_SEVERITY::EI_ERROR,
    //                        CErrorInfo::ERR_TYPE::ET_USER);
    //        // } Prasad 12-May-2006 , VSS code reusability optimisation
    //        CString error_str = _T("");
    //        error_str.LoadString(IDS_ERR_OCCURRED);
    //        m_pqm_ptr->WriteToUI(error_str);
    //        m_pqm_ptr->DisplayError(IDS_ERR_NO_COUCHINFO,MB_SYSTEMMODAL);
    //        return E_ERROR;
    //    }
    //
    //    CPQMTraceInfo::GetInstance()->LogTrace(CTraceInfo::L2,
    //                    _T("CScanProcedure::CopyCouchInfo(%d, %f, %f) = %d"),
    //                    scanNumber,
    //                    couchPos,
    //                    homePos,
    //                    status);
    //    CPQMTraceInfo::GetInstance()->LogTrace(
    //            CTraceInfo::L2,
    //            _T("CScanProcedure::CopyCouchInfo()"));
    // parser changes start

    CPQMXMLParser* ipcparser = NULL;
    CPTString file_name;

    ipcparser = new CPQMXMLParser();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //wchar_t buffer[MAX_PATH];
    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    file_name  = buffer;
    file_name += L"\\";
    file_name += L"PQM\\";
    file_name +=  SHARED_MEMORY_COUCH_INFO_FILENAME;

    // load xml file
    if (ipcparser->LoadXMLFile(file_name)) {
        // file contains only one node so get data from 0th position
        CPTString scanNumberstr = ipcparser->GetAttributeValue(0, NODE_TAG_NAME_COUCH_INFO, CHILD_TAG_NAME_SCANNO, ATTRIBUTE_NAME_1);
        CPTString couchPosstr = ipcparser->GetAttributeValue(0, NODE_TAG_NAME_COUCH_INFO, CHILD_TAG_NAME_COUCH_POS, ATTRIBUTE_NAME_1);
        CPTString homePosstr = ipcparser->GetAttributeValue(0, NODE_TAG_NAME_COUCH_INFO, CHILD_TAG_NAME_HOME_POS, ATTRIBUTE_NAME_1);
        scanNumber = ipcparser->ReadLongValue(scanNumberstr);
        couchPos = ipcparser->ReadFloatValue(couchPosstr);
        homePos = ipcparser->ReadFloatValue(homePosstr);

        DEL_PTR(ipcparser);

        // parser changes end

    } else {
        return E_ERROR;
    }

    CString str_msg(_T(""));
    int status = E_NO_ERROR;
    str_msg.Format(_T("%d, %f, %f %d"), scanNumber, couchPos, homePos, status);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    couchPos = PQM_COUCH_INIT_POSITION;
    homePos = PQM_COUCH_INIT_POSITION;
    scanNumber = -1;

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :AcqModeSet()
//Author        :PATNI/NJK
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcqModeSet(
    CPqmProtocol* protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcqModeSet");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcqModeSet"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    /*
    int qmode;
    int ezmode;
    */
    /*
    int n , numdone;
    VfFieldSpec_t fieldspec[NUM_5];
    VfPathElem_t path[NUM_5];
    */
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    int qmode = 0;

    if (protocol->GetShimming() ||
        protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY) {
        qmode = VFC_SET_DIRECT_QUEUE ;

    } else {
        qmode = VFC_SET_NORMAL_QUEUE ;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t fieldspec[NUM_5] = {0};
    memset(fieldspec, 0, sizeof(VfFieldSpec_t) * NUM_5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int n = 0 ;
    int ezmode = 0;
    VFF_SET_ARGS(fieldspec[n],
                 SVN_PLN_EASYSCAN_FLG,
                 VFT_BOOL,
                 sizeof(int) ,
                 &ezmode,
                 0L);

    n++;
    int numdone = 0;
    CVarFieldHandle* vf_handle = m_acq_manager->GetPqm()->GetStudy()->GetVfDBHandle();
    VfPathElem_t path[NUM_5] = {0};
    memset(path, 0, sizeof(VfPathElem_t) * NUM_5);
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol->GetProtocol();
    vf_handle->GetFields(path, SVD_ST_PROTOCOL,
                         fieldspec, n, (int*) & numdone);

    if (n == numdone && ezmode)  {
        ezmode = 0 ;
    }

    n = 0;
    VFF_SET_ARGS(fieldspec[n],
                 SVN_PRC_DIRECT_QUEUE,
                 VFT_INT32,
                 sizeof(int),
                 &qmode,
                 0L);
    n++;

    if (numdone) {
        VFF_SET_ARGS(
            fieldspec[n],
            SVN_PLN_EASYSCAN_FLG,
            VFT_BOOL,
            sizeof(int),
            &ezmode,
            0L);
        n++;
    }

    time_t ts = 0L;
    status_t l_status = vf_handle->PutFields(path, SVD_ST_PROTOCOL, fieldspec,
                        n, (int*) & numdone, &ts);

    if (E_NO_ERROR != l_status || numdone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    return ;
}

//****************************Method Header************************************
//Method Name   :AcquisitionAPCStart()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::AcquisitionAPCStart(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::AcquisitionAPCStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcquisitionAPCStart"));


    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();

    if (m_pqm_protocol->GetExeAutoVoice() >> NUM_4) {

        /* AUTO_VOICE */
        //PqmDebugAcqMonitor("waitForPrepChange = %d", PqmAcqGlobal.waitForPrepChange);

        if (m_wait_for_prep_scan) {
            CVoiceManager::GetInstance()->CmdPlay((m_pqm_protocol->GetExeAutoVoice() >> NUM_4));
            SetWaitForPrepScan(FALSE);

        } else {
            CVoiceManager::GetInstance()->CmdPlay((m_pqm_protocol->GetExeAutoVoice() >> NUM_4), TRUE);
        }
    }

    if (!m_acq_manager->GetLastTransaction()->SendControl(ACQ_START_BUTTON)) {
        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
        //        SCAN_ERR_NOTIFY, L"ScanProcedure.cpp");
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

        l_study->SetScanModeOfHead();
        //      ResetCurrentRequest();
        return;
    }
}

// + SM4 Himanshu VISUAL_PREP 13 JAN 2009
//****************************Method Header************************************
//Method Name   :VisualPrepDelayCheck
//Author        :PATNI/HAR
//Purpose       :This function checks for Visual Prep delay
//*****************************************************************************
int CScanProcedure::VisualPrepDelayCheck(
    CPqmProtocol* f_protocol
)
{
    if (f_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP ||
        f_protocol->GetMovingCouch()) {
        return E_NO_ERROR;
    }

    //+Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
    char			l_file[512] = {0};
    CPqmUtilities::GetPrepStudyName(l_file);
    //GetPrepStudyName(l_file) ;
    //-Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini

    CVarFieldHandle l_vf_handle(l_file);

    if (l_vf_handle.VuiOpen() != E_NO_ERROR) {
        return PQM_WARN_SCAN_PREP_STUDY;
    }

    //+Patni-HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441
    int n = 0;
    int l_prep_delay = -1;
    VfFieldSpec_t   l_vf_req[2] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * 2);
    VFF_SET_ARGS(l_vf_req[n], SVN_EXE_PREP_SCAN_DELAY_TIME, VFT_INT32,
                 sizeof(l_prep_delay), &l_prep_delay, 0L);
    n++;

    int num = 0;
    //Patni-HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441
    static VfPathElem_t   path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0},
        {SVN_ST_SLICEGROUP,     0},
        {SVN_ST_SLICE,          0}
    };
    l_vf_handle.GetFields(path, SVD_ST_PROTOCOL, l_vf_req, n, &num);

    int l_return_val = ((n == num) && !l_prep_delay) ? E_NO_ERROR : PQM_WARN_SCAN_PREP_DELAY_SKIP;

    return l_return_val;
    //-Patni-HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441
}

//**************************************************************************
//Method Name   : PqmPrGetShimmingOffset
//Author        : PATNI/DKH
//Purpose       : To Get shimming offset
//**************************************************************************
int CScanProcedure::PqmPrGetShimmingOffset(CVarFieldHandle* const f_vf_handle,
        VfPathElem_t*	f_path,
        int32_t			f_depth,
        PqmShim_t*		f_shim)
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::PqmPrGetShimmingOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PqmPrGetShimmingOffset"));

    DB_FUNC_ENTER("[PqmPrGetShimmingOffset]:");

    VfFieldSpec_t	l_vfReq[4] = {0};
    VFF_SET_ARGS(l_vfReq[0], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int32_t) * PQM_SHIM_LEN, f_shim->shimOffset,
                 VFO_VARIABLE_LEN);
    VFF_SET_ARGS(l_vfReq[1], SVN_SEQ_SHIM_GRAD_OFFSET, SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int32_t) * PQM_GRAD_LEN, f_shim->gradOffset,
                 VFO_VARIABLE_LEN);
    VFF_SET_ARGS(l_vfReq[2], SVN_EXE_SHIM_FSY_VALUE, SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(flt64_t), &f_shim->fsyValue, 0);

    int l_num = 0;
    status_t	l_status = f_vf_handle->GetFields(f_path, f_depth, l_vfReq, 3, &l_num);

    if (l_status != E_NO_ERROR ||
        l_num != 3 ||
        l_vfReq[0].status != E_NO_ERROR ||
        l_vfReq[1].status != E_NO_ERROR ||
        l_vfReq[2].status != E_NO_ERROR) {
        bzero(f_shim, sizeof(PqmShim_t));
        return E_ERROR;
    }

    return E_NO_ERROR;
}
//-Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187

//**************************************************************************
//Method Name   : CheckToDisplayCdsInDoNotShow
//Author        : iGATE/
//Purpose       : check for displaying CDS dialog in Don't Show case
//**************************************************************************
bool CScanProcedure::CheckToDisplayCdsInDoNotShow(
    CPqmProtocol* f_cds_protocol)
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::CheckToDisplayCdsInDoNotShow");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckToDisplayCdsInDoNotShow"));
    bool l_can_display_cds = false;

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr is NULL"));
        return l_can_display_cds;
    }

    CPQMStudy* l_study_obj = m_pqm_ptr->GetStudy();

    if (NULL == f_cds_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_cds_protocol is NULL"));
        return l_can_display_cds;
    }

    if (NULL == l_study_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study_obj is NULL"));
        return l_can_display_cds;
    }

    PqmSaturnCoil_t* l_saturn_coil = f_cds_protocol->GetSaturnCoil();

    if (NULL == l_saturn_coil) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_saturn_coil is NULL"));
        return l_can_display_cds;
    }

    BITFLD_DECL(mask, K_ALL_PR) = {0};
    CVarFieldHandle* l_vf_handle = l_study_obj->GetVfDBHandle();

    if (NULL == l_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
        return l_can_display_cds;
    }

    CDataManager* l_data_mgr = m_pqm_ptr->GetDataManager();

    if (NULL == l_data_mgr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_mgr is NULL"));
        return l_can_display_cds;
    }

    DllT* l_coil_head    = l_data_mgr->GetPQMCoilData()->GetHead();
    DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();

    if ((NULL == l_coil_head) || (NULL == l_section_head)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_head or l_section_head is NULL"));
        return l_can_display_cds;
    }


    AcqGetcoilSignal(f_cds_protocol->GetcoilChannelMode() ? m_pqm_ptr->GetCoilChannelMode()->GetSystemEnableChannel() : m_pqm_ptr->GetCoilChannelMode()->GetLimitedChannel(),
                     l_saturn_coil->numSection,
                     l_saturn_coil->RcvCoil, l_saturn_coil->Signal,
                     l_coil_head, l_section_head);

    if (TRUE != l_study_obj->MinimumChannelCheck(l_saturn_coil->numSection, l_saturn_coil->CoilList.num,
            l_saturn_coil->RcvCoil, l_saturn_coil->Signal)) {
        l_can_display_cds = true;

        const int l_max_allowable = m_pqm_ptr->GetChannelPerVFEP() * m_pqm_ptr->GetVAPChannels();

        const int l_actual_channels = f_cds_protocol->GetcoilChannelMode() ? m_pqm_ptr->GetCoilChannelMode()->GetSystemEnableChannel() : m_pqm_ptr->GetCoilChannelMode()->GetLimitedChannel();

        if (l_max_allowable > l_actual_channels) {

            l_can_display_cds = !(m_pqm_ptr->GetCoilChannelMode()->UpdateCoil(f_cds_protocol->GetProtocol(), l_max_allowable));

            f_cds_protocol->SetcoilChannelMode(CHANNEL_MODE_MAX);
            f_cds_protocol->SetSystemChannels(l_max_allowable);

            const int l_coil_channel_mode = f_cds_protocol->GetcoilChannelMode();

            if (E_NO_ERROR != l_study_obj->SetProtocolValues(l_vf_handle,
                    f_cds_protocol->GetProtocol(),
                    mask,
                    K_PR_MAX_COIL_CHANNEL_MODE,
                    &l_coil_channel_mode,
                    sizeof(l_coil_channel_mode),
                    NULL)) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
            }
        }
    }

    return l_can_display_cds;
}
//***************************Method Header*************************************
//Method Name    :ScanStart()
//Author         :PATNI/MRP
//Purpose        :This method starts with scan of the series.
//*****************************************************************************
bool CScanProcedure::ScanStart(
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::ScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ScanStart"));

    ReadScanProc();

    CAcqInfo* acqinfo = new CAcqInfo(this);

    AddTransaction(acqinfo);

    int l_mode = (PQM_INFO_BIT_COILID | PQM_INFO_BIT_COUCH | PQM_INFO_BIT_RM | PQM_INFO_BIT_TEMP);

    if (GetScanProcProtocol()->GetGatingMethod()) {
        l_mode |= PQM_INFO_BIT_GATE;
    }

    acqinfo->SetAcqInfoMode(l_mode);
    acqinfo->PrepareRequest();

    return true;
}
//***************************Method Header*************************************
//Method Name    :CheckSARforSARControl
//Author         :iGATE/
//Purpose        :
//*****************************************************************************
int CScanProcedure::CheckSARforSARControl(CPqmProtocol* f_protocol, const bool f_is_prescan_done) const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::CheckSARforSARControl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_result = PQM_SAR_OK;
    sarInfo_p l_sar_info = f_protocol->GetSarInfo();
    const int l_max_wait_time_to_wait = MAX_TIME_FOR_AUTO_SCAN_DEFAULT;


    if (!l_sar_info->sarControl.scanEnable) {

        if (l_sar_info->sarControl.rfampProtect) {

            l_result = PQM_SAR_RF_PROTECT;


        } else if ((l_sar_info->sarControl.coilProtect & IEC_OVER_10SEC) && (l_sar_info->tsar[l_sar_info->active].prealRatio > SAR_LIMIT_10SEC)) {


            l_result = PQM_SAR_COIL_PROTECT;


        } else if ((l_sar_info->sarControl.sarProtect & IEC_OVER_10SEC) && (l_sar_info->tsar[l_sar_info->active].sarRatio > (SAR_LIMIT_10SEC - SAR_LIMIT_10SEC_MARGIN))) {


            l_result = PQM_SAR_LIMIT_OVER;


        } else if ((l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) || (l_sar_info->sarControl.sarProtect & IEC_OVER_6MIN)) {

            if (f_is_prescan_done || -1 == l_sar_info->sarControl.waitTime || l_max_wait_time_to_wait < l_sar_info->sarControl.waitTime || m_apc_retry) {

                l_result = (l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) ? PQM_SAR_COIL_PROTECT : PQM_SAR_LIMIT_OVER;

            } else {

                l_result = (l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) ? PQM_SAR_COIL_PROTECT_6MIN_WAIT_FOR_AUTO_START : PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START;
            }

        } else if ((l_sar_info->sarControl.coilProtect & IEC_OVER_10SEC) || (l_sar_info->sarControl.sarProtect & IEC_OVER_10SEC)) {

            if (m_10sec_over_wait_count < 10) {

                l_result = (l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) ? PQM_SAR_COIL_PROTECT_10SEC_WAIT_FOR_AUTO_START : PQM_SAR_LIMIT_OVER_10SEC_WAIT_FOR_AUTO_START;
                m_10sec_over_wait_count++;

            } else {
                l_result = (l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) ? PQM_SAR_COIL_PROTECT : PQM_SAR_LIMIT_OVER;
            }

        } else {

            l_result = PQM_SAR_LIB_ERROR;
        }
    }

    return l_result;
}

//****************************Method Header************************************
//Method Name   :WaitTimeTimerReset()
//Author        :iGate
//Purpose       :
//*****************************************************************************
void CScanProcedure::WaitTimeTimerReset(CPqmProtocol* f_scanning_protocol)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::WaitTimeTimerReset");
    PQM_SCOPED_TRACE();

    WNP_alarm(0) ;
    WNP_signal(SIGALRM, SIG_DFL) ;

    m_pqm_ptr->WriteToUI(PQM_MSG_DISPLAY_WAIT_TIME_END);

    m_is_waiting_for_scan_sarctrl = false;

    if (NULL == f_scanning_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_scanning_protocol is NULL"));
        return;
    }

    if (!f_scanning_protocol->GetSeriesNumInDbFlag()) {

        m_pqm_ptr->GetStudy()->SetAcqStatus(f_scanning_protocol, C_PR_STATUS_WAIT);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

    } else {

        m_pqm_ptr->GetStudy()->SetAcqStatus(f_scanning_protocol, C_PR_STATUS_FAIL);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_FAIL);
    }

    m_pqm_ptr->GetStudy()->SetScanModeOfHead();
    ResetCurrentRequest();
}

//***************************Method Header*************************************
//Method Name    :GetShimState
//Author         :PATNI/MRP
//Purpose        :Dummy function which returns the shim state info. These variables
//                will be read from the shared memory.
//*****************************************************************************
int CScanProcedure::GetShimState(
    int& shimState
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetShimState");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimState"));


    // parser changes start
    CPQMXMLParser* ipcparser = NULL;
    CPTString file_name;

    ipcparser = new CPQMXMLParser();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //wchar_t buffer[MAX_PATH];
    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    file_name  = buffer;
    file_name += L"\\";
    file_name += L"PQM\\";
    file_name += SHARED_MEMORY_SHIM_STATE_FILENAME;

    // load xml file
    if (ipcparser->LoadXMLFile(file_name)) {
        // file contains only one node so get data from 0th position
        CPTString shimStatestr = ipcparser->GetAttributeValue(0, NODE_TAG_NAME_SHIM, CHILD_TAG_NAME_SHIM_STATE, ATTRIBUTE_NAME_1);
        shimState = ipcparser->ReadLongValue(shimStatestr);

        DEL_PTR(ipcparser);

    } else {
        return E_ERROR;
    }

    int status = E_NO_ERROR;
    CString str_msg(_T(""));
    str_msg.Format(_T("%d %d"), shimState, status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    shimState = SHIM_STATE_WAIT;
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :GetControlKind()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetControlKind(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetControlKind");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetControlKind"));

    return m_control_kind;
}

//+Patni-Ravindra Acharya/2010June22/Added/IR-153
COlpTemp* CScanProcedure::GetOlpTemp()const
{
    return m_olp_temp ;
}
//-Patni-Ravindra Acharya/2010June22/Added/IR-153



//*******************************Method Header*********************************
//Method Name    :SetScanProc()
//Author         :PATNI/HAR
//Purpose        :This method sets the value for ScanProc
//*****************************************************************************
void CScanProcedure::SetScanProc(
    const CString& scan_proc,
    const int index
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetScanProc");

    CString str_msg(_T(""));
    str_msg.Format(_T("%s,%d"), scan_proc, index);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    if (index >= 0 && index <= NUM_2) {
        m_scan_proc[index] = scan_proc;
    }
}

//****************************Method Header************************************
//Method Name   :SetTPCTRLAutoVoice()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CScanProcedure::SetTPCTRLAutoVoice(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetTPCTRLAutoVoice");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetTPCTRLAutoVoice"));

    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CPQMStudy* l_study = m_acq_manager->GetPqm()->GetStudy();

    l_study->GetProtocolValues(f_vf_handle,
                               protocol->GetProtocol(),
                               mask,
                               K_PR_TPCTRL,
                               &m_av_tp1_ctrl,
                               NULL,
                               NULL);

    if (m_av_tp1_ctrl) {


        float delay = (float)CVoiceManager::GetInstance()->GetAvTp1Delay() * NUM_2;

        int t1 = CVoiceManager::GetInstance()->GetVoiceLength(PQM_AV_VOICE_5);
        int t2 = CVoiceManager::GetInstance()->GetVoiceLength(
                     protocol->GetPlnAutoVoice());

        if (protocol->GetIntermitFlag()) {
            SetAvTp1Duration((protocol->GetIntermitTime() * INTERMIT_TIME) - (int)delay);

        } else {
            float tr = 0.0f, min_tr = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
            l_study->GetProtocolValues(f_vf_handle,
                                       protocol->GetProtocol(),
                                       mask,
                                       K_PR_TR,
                                       &tr,
                                       NULL,
                                       K_PR_MIN_TR,
                                       &min_tr,
                                       NULL,
                                       NULL);
            //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
            CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("TPCTRL %d TR = %g minTR = %g"),
                    m_av_tp1_ctrl, tr, min_tr);
            //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
            //          PqmDebugAcqMonitor("TPCTRL %d TR = %g minTR = %g",
            //              PqmAcqGlobal.avTP1Ctrl,TR,minTR);

            SetAvTp1Duration(
                (int32_t)(tr * TP_DURATION - min_tr * TP_DURATION) - (int)delay);
        }

        //+Patni-NP/2009Dec30/Added/MVC006508
        //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Voice:%d,%d Duration:%d Delay:%d Margin:%d"),
                t1, t2, m_av_tp1_duration,
                delay,
                m_av_tp1_margin);
        //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        SetAvTp1Margin(GetAvTp1Duration() - (t1 + t2));
        //-Patni-NP/2009Dec30/Added/MVC006508
        //      PqmDebugAcqMonitor("Voice:%d,%d Duration:%d Delay:%d Margin:%d",
        //          t1,t2,PqmAcqGlobal.avTP1Duration,
        //          PqmGlobal.avTP1Delay,
        //          PqmAcqGlobal.avTP1Margin);
    }

    return GetAvTp1Ctrl();
}

//****************************Method Header************************************
//Method Name   :SetAvAcqTime()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetAvAcqTime(
    const int avacqtime
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAvAcqTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAvAcqTime"));
    m_av_acq_time = avacqtime;
}


//****************************Method Header************************************
//Method Name   :SetAvTp1Ctrl()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetAvTp1Ctrl(
    const int avtp1ctrl
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAvTp1Ctrl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAvTp1Ctrl"));

    m_av_tp1_ctrl = avtp1ctrl;
}


//****************************Method Header************************************
//Method Name   :SetAvTp1Duration()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetAvTp1Duration(
    const int avtp1duration
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAvTp1Duration");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAvTp1Duration"));
    m_av_tp1_duration = avtp1duration;
}


//****************************Method Header************************************
//Method Name   :SetAvTp1Margin()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
void CScanProcedure::SetAvTp1Margin(
    const int av_tp1_margin
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetAvTp1Margin");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAvTp1Margin"));

    m_av_tp1_margin = av_tp1_margin;
}

//****************************Method Header************************************
//Method Name   :SetPVCoil()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
BOOL CScanProcedure::SetPVCoil(
    const int port,
    int* rcv_coil_tbl,
    const int n
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::SetPVCoil");

    UNREFERENCED_PARAMETER(port);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetPVCoil"));
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoil *coil_p;

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_acq_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_acq_manager pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilDB* coil_db = pqm->GetCoilDB();

    //    BITFLD_DECL(mask,K_ALL_PR);

    int mode = 0; //Patni-AD/2009Jun02/Modified/SU14

    if (n == 0) {
        mode = FALSE ;

    } else {
        mode = TRUE ;
    }

    int gain_data[PQM_NUM_VFEP_CHANNEL * NUM_16] = {0} ;
    memset(gain_data, 0 , sizeof(gain_data)) ;
    int pac_pattern[NUM_16] = {0} ;
    memset(pac_pattern, 0 , sizeof(pac_pattern)) ;
    char coil_name_tbl[NUM_64 * NUM_16] = {0} ;
    memset(coil_name_tbl, 0 , sizeof(coil_name_tbl)) ;
    int total = 0 ;

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (rcv_coil_tbl == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("rcv_coil_tbl pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    CPqmCoilData* l_pqm_coil_data = m_acq_manager->GetPqm()->GetDataManager()->GetPQMCoilData();

    if (NULL == l_pqm_coil_data) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("l_pqm_coil_data pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    CCoil coil_p;
    int d_size = 0;

    for (int i = 0 ; i < n ; i++) {

        //DB_PRINTF("c",("port=%d , total=%d,[0X%x]\n", port, n , *(rcvCoilTbl+i)));
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        //coil_p = coil_db->GetCoil(*(rcv_coil_tbl + i));
        l_pqm_coil_data->PqmCoilGetCoil(*(rcv_coil_tbl + i), coil_p);
        /*if (coil_p == NULL) {
            continue;
            //return    E_ERROR ;
        }*/

        //pac_pattern[i] = coil_p->GetPacPattern();
        pac_pattern[i] = coil_p.GetPacPattern();

        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        //      memcpy( &gain_data[PQM_NUM_VFEP_CHANNEL*i], coil_p->getga,
        //          sizeof(coil_p->gain_data) ) ;
        if (i != 0) {
            if ((total - 1) < 1024) {		//Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
                coil_name_tbl[total - 1] = ';' ;
                //+Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("CoilNameTbl array out of Bound"));
                return E_ERROR;
            }

            //-Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
        }

        //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        //d_size = coil_p->GetLabel().GetLength() + 1 ;
        d_size = coil_p.GetLabel().GetLength() + 1 ;

        if ((total + d_size) < 1024) {		//Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
            //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
            coil_name_tbl[total] = (char)coil_p.GetLabel().GetBuffer(d_size);
            //+Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("CoilNameTbl array out of Bound"));
            return E_ERROR;
        }

        //-Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
        total += d_size ;
    }

    //DB_PRINTF("c",("total=%d,[%s]\n",total,CoilNameTbl));
    //    pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
    //                                      port,
    //                                      mask,
    //                                      K_PR_CHANGED_RECEIVE_COIL,
    //                                      &mode,
    //                                      sizeof(mode),
    //                                      K_PR_RECVR_COIL_TBL,
    //                                      rcv_coil_tbl,
    //                                      sizeof(int)*n,
    //                                      K_PR_CHANNEL_GAIN_RATIO_TBL,
    //                                      &gain_data,
    //                                      sizeof(float)*PQM_NUM_VFEP_CHANNEL*n,
    //                                      K_PR_PAC_PATTERN_TBL,
    //                                      &pac_pattern,
    //                                      sizeof(int)*n,
    //                                      K_PR_RECVR_COILNAME_TBL,
    //                                      &coil_name_tbl,
    //                                      total,
    //                                      NULL );

    return E_NO_ERROR;
}

// + SHIMMING Lokesh 15 March 2008
//*******************************Method Header*********************************
//Method Name    :SetPqmShimInfo()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CScanProcedure::SetPqmShimInfo(
    PqmShim_t* f_pqmshim
)
{
    m_pqmshiminfo = f_pqmshim;
}
// - SHIMMING Lokesh 15 March 2008



//****************************Method Header************************************
//Method Name   :DisplayMessageForSAR()
//Author        :PATNI/Sanjay
//Purpose       :
//*****************************************************************************
MPLUSERRORUSERRESPONSE_e CScanProcedure::DisplayMessageForSAR(
    const status_t f_sts_sar
)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::DisplayMessageForSAR");
    PQM_SCOPED_TRACE();

    switch (f_sts_sar) {

        case PQM_SAR_OK_BUT_COIL:
        case PQM_SAR_OK_BUT_SAR:
        case PQM_SAR_OK_BUT_RF:
        case PQM_SAR_SAE_WARN: {
            const int l_function_bit = GetSARMessageCheckBit(f_sts_sar);

            if (!(m_sar_msg_bit & l_function_bit))  {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SAR Error (Ignored) -> ") + CPqmUtilities::GetMultiLingualString(GetSARMessageString(f_sts_sar)));
                return MPlusErrResponse_OK ;
            }

            m_sar_msg_bit = (m_sar_msg_bit ^ l_function_bit);
        }

        break;
    }

    CString l_sarwarning(_T(""));
    int l_sarwarning_buttonstype = SCAN_WARN_OKONLYBUTTON;
    int l_sarwarning_level = SCAN_WARN_INFORMATION;

    if (!GetSARMessageDisplayInformation(f_sts_sar, l_sarwarning, l_sarwarning_buttonstype, l_sarwarning_level)) {
        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_NO_MATCHING_SAR_WARNING")),
            _T("CScanProcedure.cpp"), __LINE__, ERROR_LOG_INFORMATION1 ,
            ERR_ID, APP_NAME);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SAR Error (Not Matching SAR) -> ") + CPqmUtilities::GetMultiLingualString(_T("IDS_NO_MATCHING_SAR_WARNING")));

        return MPlusErrResponse_CANCEL;
    }

    MPLUSERRORUSERRESPONSE_e   l_response =  MPlusErrResponse_OK;

    if (!l_sarwarning.IsEmpty()) {

        CString l_appname(_T(""));
        l_appname.LoadString(IDS_WARNING_APP_NAME);

        l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_sarwarning,
                     l_appname,
                     l_sarwarning_level,
                     SCAN_WARN_SYSMODAL,
                     l_sarwarning_buttonstype);
    }

    return l_response;
}


//****************************Method Header************************************
//Method Name   :ResetSARMsgCheckBit()
//Author        :PATNI/Sanjay
//Purpose       :
//*****************************************************************************
void CScanProcedure::ResetSARMsgCheckBit()
{
    m_sar_msg_bit = 0xFFFF;
}


//****************************Method Header************************************
//Method Name   :GetSARMessageCheckBit()
//Author        :PATNI/Sanjay
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetSARMessageCheckBit(
    const int f_functionbit
)const
{
    switch (f_functionbit) {

        case PQM_SAR_OK_BUT_COIL:
            return PQM_SAR_OK_BUT_COIL_CHK_BIT;

        case PQM_SAR_OK_BUT_SAR:
            return PQM_SAR_OK_BUT_SAR_CHK_BIT;

        case PQM_SAR_OK_BUT_RF:
            return PQM_SAR_OK_BUT_RF_CHK_BIT;

        case PQM_SAR_SAE_WARN:
            return PQM_SAR_SAE_WARN_CHK_BIT;
    }

    return 0 ;
}

//****************************Method Header************************************
//Method Name   :GetSARMessageDisplayInformation()
//Author        :PATNI/Sanjay
//Purpose       :
//*****************************************************************************
bool CScanProcedure::GetSARMessageDisplayInformation(
    const status_t f_sts_sar,
    CString& f_sarwarning,
    int& f_sarwarning_buttonstype,
    int& f_sarwarning_level
) const
{

    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetSARMessageDisplayInformation");

    switch (f_sts_sar) {

        case PQM_SAR_ERROR:
        case PQM_SAR_LIMIT_OVER:
        case PQM_SAR_COIL_PROTECT:
        case PQM_SAR_RF_PROTECT:
        case PQM_SAR_SAE_ERROR:
        case PQM_SAR_OK_BUT_COIL:
        case PQM_SAR_OK_BUT_SAR:
        case PQM_SAR_OK_BUT_RF:
        case PQM_SAR_LIB_ERROR:
        case PQM_SAR_OK_BUT_LIB:
        case PQM_SAR_LIMIT_OVER_WAIT:
        case PQM_SAR_COIL_PROTECT_WAIT:

            f_sarwarning_buttonstype = SCAN_WARN_OKBUTTON;
            f_sarwarning_level = SCAN_WARN_INFORMATION;
            break;

        case PQM_SAR_SAE_WARN:
            f_sarwarning_buttonstype = SCAN_WARN_OK_CANCEL_BUTTONS;
            f_sarwarning_level = SCAN_WARN_CRUCIAL;
            break;

        default:

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_sts_sar Not matched"));
            return false;
    }

    f_sarwarning = CPqmUtilities::GetMultiLingualString(GetSARMessageString(f_sts_sar));

    if (f_sts_sar == PQM_SAR_COIL_PROTECT_WAIT || f_sts_sar == PQM_SAR_LIMIT_OVER_WAIT) {


        CString l_sarwarning(f_sarwarning);
        f_sarwarning.Format(l_sarwarning, m_pqm_protocol->GetSarInfo()->sarControl.waitTime);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SAR Error -> ") + f_sarwarning);

    return true;
}


//****************************Method Header************************************
//Method Name   :GetSARMessageString()
//Author        :PATNI/Sanjay
//Purpose       :
//*****************************************************************************
const CString CScanProcedure::GetSARMessageString(
    const status_t f_sts_sar
) const
{
    switch (f_sts_sar) {

        case PQM_SAR_ERROR:
            return _T("IDS_PQM_SAR_ERROR");

        case PQM_SAR_LIB_ERROR:
        case PQM_SAR_OK_BUT_LIB:
            return _T("IDS_ERR_SYSTEM_SAR_DISPLAY");

        case PQM_SAR_LIMIT_OVER:
            return _T("IDS_PQM_SAR_LIMIT_OVER");

        case PQM_SAR_COIL_PROTECT:
            return _T("IDS_PQM_SAR_COIL_PROTECT");

        case PQM_SAR_RF_PROTECT:
            return _T("IDS_PQM_SAR_RF_PROTECT");

        case PQM_SAR_SAE_ERROR:
            return _T("IDS_PQM_SAR_SAE_ERROR");

        case PQM_SAR_OK_BUT_COIL:
            return _T("IDS_PQM_SAR_OK_BUT_COIL");

        case PQM_SAR_OK_BUT_SAR:
            return _T("IDS_PQM_SAR_OK_BUT_SAR");

        case PQM_SAR_OK_BUT_RF:
            return _T("IDS_PQM_SAR_OK_BUT_RF");

        case PQM_SAR_SAE_WARN:
            return _T("IDS_PQM_SAR_SAE_WARN");

        case PQM_SAR_COIL_PROTECT_WAIT:
            return _T("IDS_PQM_SAR_COIL_PROTECT_WAIT");

        case PQM_SAR_LIMIT_OVER_WAIT:
            return _T("IDS_PQM_SAR_LIMIT_OVER_WAIT");

        case PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START:
        case PQM_SAR_COIL_PROTECT_6MIN_WAIT_FOR_AUTO_START:
            return _T("IDS_PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START");
    }

    return _T("");
}

//****************************Method Header************************************
//Method Name   :GetStatusForCheckSAR()
//Author        :iGate
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetStatusForCheckSAR(CPqmProtocol* f_current_protocol, const bool f_is_prescan_done)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetStatusForCheckSAR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_result = PQM_SAR_OK;

    if (NULL == f_current_protocol) {
        l_result = PQM_SAR_ERROR;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_SAR_ERROR"));
        return l_result;
    }

    sarInfo_p	l_sarinfo = f_current_protocol->GetSarInfo();

    if (sarStatTsuccess != f_current_protocol->GetSARStat()) {

        l_result = PQM_SAR_LIB_ERROR;

    } else {
        if (!l_sarinfo->scanEnable && VFC_SAE_LEVEL_OVER == l_sarinfo->sae.sae_level) {

            l_result = PQM_SAR_SAE_ERROR;

        } else if (m_pqm_ptr->GetSARControlLicense()) {
            if (!l_sarinfo->sarControl.scanEnable) {

                l_result = CheckSARforSARControl(f_current_protocol, f_is_prescan_done);
            }

        } else {
            if (l_sarinfo->rfampProtect) {

                l_result = PQM_SAR_RF_PROTECT;

            } else if (l_sarinfo->coilProtect) {

                l_result = PQM_SAR_COIL_PROTECT;

            } else if (!l_sarinfo->scanEnable) {

                l_result = PQM_SAR_LIMIT_OVER;
            }
        }

        if (l_result == PQM_SAR_OK && l_sarinfo->scanEnable && VFC_SAE_LEVEL_WARNING == l_sarinfo->sae.sae_level) {
            l_result = PQM_SAR_SAE_WARN;
        }
    }

    const CString l_message_str = GetMessageStringForSARStatus(l_result);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    if (l_result == PQM_SAR_OK) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_SAR_OK"));
        return l_result;
    }

    const bool l_sar_ok = GetSAROKFlagForCoilAndAnatomy(f_current_protocol);

    if (l_sar_ok || m_pqm_ptr->GetStudy()->GetFirstScan()) {

        l_result = GetMaybeSARIDForStatus(l_result);
    }

    return l_result;
}

//****************************Method Header************************************
//Method Name   :HandleSARErrorForStatus()
//Author        :iGate
//Purpose       :
//*****************************************************************************
int CScanProcedure::HandleSARErrorForStatus(const int f_sar_status, const bool f_scan_retry)
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::HandleSARErrorForStatus");
    PQM_SCOPED_TRACE();

    if (NULL == m_pqm_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_protocol is NULL"));
        return false;
    }


    if ((PQM_SAR_COIL_PROTECT_10SEC_WAIT_FOR_AUTO_START == f_sar_status) ||
        (PQM_SAR_LIMIT_OVER_10SEC_WAIT_FOR_AUTO_START == f_sar_status)) {


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Sleeping for 1 sec."));

        m_pqm_ptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
        Sleep(1000);
        m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);

        return PQM_SAR_RECALC_AND_CHECK;

    } else if ((PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START == f_sar_status) ||
               (PQM_SAR_COIL_PROTECT_6MIN_WAIT_FOR_AUTO_START == f_sar_status)) {


        if (m_is_waiting_for_scan_sarctrl) {

            m_is_waiting_for_scan_sarctrl = false;

            if (!m_pqm_protocol->GetSeriesNumInDbFlag()) {

                m_pqm_ptr->GetStudy()->SetAcqStatus(m_pqm_protocol, C_PR_STATUS_WAIT);
                m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

            } else {

                m_pqm_ptr->GetStudy()->SetAcqStatus(m_pqm_protocol, C_PR_STATUS_FAIL);
                m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_FAIL);
            }

            m_pqm_ptr->GetStudy()->SetScanModeOfHead();
            ResetCurrentRequest();
            return false;
        }

        const int l_wait_time = m_pqm_protocol->GetSarInfo()->sarControl.waitTime;

        CString l_message_str = CPqmUtilities::GetMultiLingualString(GetSARMessageString(f_sar_status));

        CString l_sarwarning(l_message_str);

        l_sarwarning.Format(l_message_str, l_wait_time);

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_sarwarning,
                MPlus::EH::Information, FUNC_NAME, _T("PQM"));

        l_message_str.Format(_T("Waiting for %d secs"), l_wait_time);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

        m_pqm_ptr->WriteToUI(PQM_MSG_DISPLAY_WAIT_TIME);

        WNP_signal(SIGALRM, &CScanProcedure::RecalcSARControlAndAutoScanStartStatic);

        WNP_alarm(l_wait_time);

        m_is_waiting_for_scan_sarctrl = true;

        m_pqm_ptr->GetStudy()->SetWaitToCurrent(m_pqm_protocol);

        m_pqm_ptr->WriteToUI(PQM_MSG_STATUS_CURRENT);

        m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);

        return false;


    } else if (f_scan_retry) {

        return WarningSAR(f_sar_status, _T("PqmPmAcqman.cpp"));
    }

    return WarningSARAfterAcqmanResponseForPrescan(f_sar_status, m_pqm_protocol);
}

//****************************Method Header************************************
//Method Name   :RecalcSARControlAndAutoScanStart()
//Author        :iGate
//Purpose       :
//*****************************************************************************
void CScanProcedure::RecalcSARControlAndAutoScanStart()
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::RecalcSARControlAndAutoScanStart");
    PQM_SCOPED_TRACE();

    m_pqm_ptr->WriteToUI(PQM_MSG_DISPLAY_WAIT_TIME_END);

    if (NULL == m_pqm_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_protocol is NULL"));
        return;
    }

    m_pqm_ptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);

    if (!m_pqm_protocol->GetSeriesNumInDbFlag()) {

        m_pqm_ptr->GetStudy()->SetAcqStatus(m_pqm_protocol, C_PR_STATUS_WAIT);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

    } else {

        m_pqm_ptr->GetStudy()->SetAcqStatus(m_pqm_protocol, C_PR_STATUS_FAIL);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_FAIL);
    }

    m_pqm_ptr->GetStudy()->SetScanModeOfHead();
    ResetCurrentRequest();

    const int l_sar_status = m_acq_manager->CalcSARControlAndCheckSAR(m_pqm_protocol);

    m_is_waiting_for_scan_sarctrl = false;

    if (l_sar_status) {
        m_pqm_ptr->WriteToUI(AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL);
    }

    m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
}

//****************************Method Header************************************
//Method Name   :Set10SecOverWaitCount()
//Author        :iGate
//Purpose       :
//*****************************************************************************
void CScanProcedure::Set10SecOverWaitCount(const int f_10sec_over_wait_count)
{
    m_10sec_over_wait_count = f_10sec_over_wait_count;
}

//****************************Method Header************************************
//Method Name   :GetIsWaitingFlagForSARCtrl()
//Author        :iGate
//Purpose       :
//*****************************************************************************
bool CScanProcedure::GetIsWaitingFlagForSARCtrl() const
{
    return m_is_waiting_for_scan_sarctrl;
}

//****************************Method Header************************************
//Method Name   :GetMessageStringForSARStatus()
//Author        :iGate
//Purpose       :
//*****************************************************************************
const CString CScanProcedure::GetMessageStringForSARStatus(const int f_msg_code) const
{

#define CASE_OPTION(x) case x: return #x;

    switch (f_msg_code) {
            CASE_OPTION(PQM_SAR_OK);
            CASE_OPTION(PQM_SAR_LIB_ERROR);
            CASE_OPTION(PQM_SAR_RF_PROTECT);
            CASE_OPTION(PQM_SAR_COIL_PROTECT);
            CASE_OPTION(PQM_SAR_LIMIT_OVER);
            CASE_OPTION(PQM_SAR_COIL_PROTECT_WAIT);
            CASE_OPTION(PQM_SAR_LIMIT_OVER_WAIT);
            CASE_OPTION(PQM_SAR_COIL_PROTECT_10SEC_WAIT_FOR_AUTO_START);
            CASE_OPTION(PQM_SAR_LIMIT_OVER_10SEC_WAIT_FOR_AUTO_START);
            CASE_OPTION(PQM_SAR_COIL_PROTECT_6MIN_WAIT_FOR_AUTO_START);
            CASE_OPTION(PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START);
            CASE_OPTION(PQM_SAR_OK_BUT_RF);
            CASE_OPTION(PQM_SAR_OK_BUT_COIL);
            CASE_OPTION(PQM_SAR_OK_BUT_SAR);
            CASE_OPTION(PQM_SAR_OK_BUT_LIB);
            CASE_OPTION(PQM_SAR_SAE_ERROR);
            CASE_OPTION(PQM_SAR_SAE_WARN);

        default:
            return _T("Unknow Message");
    }

#undef CASE_OPTION
}

//****************************Method Header************************************
//Method Name   :GetSAROKFlagForCoilAndAnatomy()
//Author        :iGate
//Purpose       :
//*****************************************************************************
bool CScanProcedure::GetSAROKFlagForCoilAndAnatomy(CPqmProtocol* f_current_protocol) const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetSAROKFlagForCoilAndAnatomy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    bool l_enable = false;
    bool l_sarok = false;

    if (NULL == f_current_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_current_protocol is NULL"));
        return l_sarok;
    }

    CPqmProtocol*   l_previous_protocol = f_current_protocol;
    CPQMStudy*		l_pqm_study = m_pqm_ptr->GetStudy();


    if (NULL == l_pqm_study) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study is NULL"));
        return l_sarok;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (NULL == l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return l_sarok;
    }

    CPqmPrMode*     l_pqmprmode = f_current_protocol->GetPqmPrMode();

    for (int l_index = f_current_protocol->GetPosition(); l_index > 0; --l_index) {

        l_previous_protocol = l_pqm_study->GetPrevItemFromPos(l_previous_protocol);

        if (NULL == l_previous_protocol) {
            break;
        }

        if (m_pqm_ptr->IsSaturnCoilSystem()) {

            l_enable = !(l_coil_data->PqmIsSameCoilInSaturn(
                             f_current_protocol->GetSaturnCoil(),
                             l_previous_protocol->GetSaturnCoil(),
                             f_current_protocol->GetCoilDBTable(),
                             l_previous_protocol->GetCoilDBTable()) ? true : false);

        } else {
            l_enable = !(IS_COIL_SAME(l_pqmprmode->GetRecvCoilId(),
                                      l_previous_protocol->GetPqmPrMode()->
                                      GetRecvCoilId()));
        }

        if (l_previous_protocol->GetProtocol() != PQ_FILLER) {

            if ((l_pqmprmode ->GetAnatomy() !=
                 l_previous_protocol->GetPqmPrMode()->GetAnatomy())
                || l_enable) {

                l_sarok = true;
                break;
            }
        }

        int l_prevprotocol_acqstatus = l_previous_protocol->GetAcqStatus();

        if (l_prevprotocol_acqstatus == C_PR_STATUS_DONE ||
            l_prevprotocol_acqstatus == C_PR_STATUS_CURRENT ||
            l_prevprotocol_acqstatus == C_PR_STATUS_FAIL
           ) {
            break;
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_sarok ? _T("l_sarok is true.") : _T("l_sarok is false."));
    return l_sarok;

}

//****************************Method Header************************************
//Method Name   :GetMaybeSARIDForStatus()
//Author        :iGate
//Purpose       :
//*****************************************************************************
int CScanProcedure::GetMaybeSARIDForStatus(const int f_sar_status) const
{
    LPCTSTR FUNC_NAME = _T("CScanProcedure::GetMaybeSARIDForStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_result = f_sar_status;

    if (f_sar_status == PQM_SAR_RF_PROTECT) {

        l_result = PQM_SAR_OK_BUT_RF;

    } else if ((PQM_SAR_COIL_PROTECT == f_sar_status) ||
               (PQM_SAR_COIL_PROTECT_WAIT == f_sar_status) ||
               (PQM_SAR_COIL_PROTECT_10SEC_WAIT_FOR_AUTO_START == f_sar_status) ||
               (PQM_SAR_COIL_PROTECT_6MIN_WAIT_FOR_AUTO_START == f_sar_status)) {

        l_result = PQM_SAR_OK_BUT_COIL;

    } else if ((PQM_SAR_LIMIT_OVER == f_sar_status) ||
               (PQM_SAR_LIMIT_OVER_WAIT == f_sar_status) ||
               (PQM_SAR_LIMIT_OVER_10SEC_WAIT_FOR_AUTO_START == f_sar_status) ||
               (PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START == f_sar_status)) {

        l_result = PQM_SAR_OK_BUT_SAR;

    } else if (f_sar_status == PQM_SAR_LIB_ERROR) {

        l_result = PQM_SAR_OK_BUT_LIB;
    }

    const CString l_message_str = GetMessageStringForSARStatus(l_result);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    return l_result;
}