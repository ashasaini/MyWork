//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Rdemine-779
//Redmine-780
//Redmine-781
//Redmine-782
// PqmProcon.cpp: implementation of the CPqmProcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmProcon.h"
#include <math.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <tami/proCon/proMsgCodes.h>
#include <tami/proCon/probe.h>
#include <tami/libplTree/plsftree.h>
#include <tami/libplTree/plstdtbl.h>
#include <tami/libpromsg/promsg.h>
#include <libStudyManager/strcnv.h>
#include <libStudyManager/Mutex/ScopedLock.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <Probe/Common.h>
#include <PQM/PQMIPCManager.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include <PQM/PQMCommanMessages.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "Pqm.h"
#include "PQMSocket.h"
#include "PqmProbe.h"
#include "PqmProconNotifier.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "PqmPrMode.h"
#include "PQMProConAdapter.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "MasterSlave.h"
#include "PqmPmSARManager.h"
//+Patni-HEMANT/ADDED On 6/12/2009 1:02:11 PM/ ACE-2 / WFDA
//Patni-PJS/2010Feb2/Modified/IR-97
//#include "WFDAImplementer.h"


struct infoPlanDynamicProtocol_s {
    CPqmProtocol* m_master_protocol;
    CPqmProtocol* m_slave_protocol;
} info_plan_dynamic_protocol = {NULL, NULL};


extern "C" {
    plStatusT plDetachDirSeg(void);
    plStatusT plPutLocatorInfo(char*	study, char* series);
}
// - SM4 LOCATOR_IMAGE Himanshu 3 Dec 2008

//Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34
#define PQM_REQ_CREATE_SOCK _T("PQM_REQ_CREATE_SOCK")
#define DBPTR(s) CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugProMonitor s

using namespace MPlus::EH;
using namespace MR::ACQ::SM::COMMON;
using namespace MR::ACQ::SM::COMMON::MUTEX;


//*******************************Method Header*********************************
//Method Name    :CPqmProcon()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
CPqmProcon::CPqmProcon(CPqm* f_pqmptr) :
    m_probe_invoked(false),
    m_scan_editing(false),
    m_gui_event(PQM_PROCON_NO_EVENT),
    m_wse_or_locator(PQM_PROCON_NO_EVENT),
    m_procon_socket(NULL),
    m_pqmptr(f_pqmptr),
    m_scratchpad_file(new char [MAX_BUFFER]),
    m_curr_protocol(NULL),
    m_selectCount(0), //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    m_selectList(NULL),
    m_pqm_procon(new PqmPmProlimS),
    m_pqm_exit(false),  //Patni-RUP/2010Aug06/Added/To fix PQM exit crash
    m_is_plan_dynamic_executed(false),
    m_proconreconnect_thread(NULL),//Patni-PJS/2010May12/Added/JaFT# Internal Defect fix
    m_auto_intervalstatus(AUTO_GATE_INTERVAL_NONE),
    m_result_pmLocator(false),
    m_is_locator_invoked(false),
    m_reset_flag(false),
    m_can_process_reset(true),
    m_inscan_switch_recv_flag(false)

{

    LPCTSTR FUNC_NAME = _T("CPqmProcon::CPqmProcon");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProcon::CPqmProcon"));

    memset(m_scratchpad_file, 0, sizeof(char) * MAX_BUFFER);

    memset(m_pqm_procon, 0, sizeof(PqmPmProlimS));
    m_probe = new CPqmProbe(this);

    memset(&(m_pqm_procon->delayInfo), 0, sizeof(delayInfo_t));
    m_pqm_procon->delayInfo.delay1 = -1;
    m_pqm_procon->delayInfo.delay2 = -1;
    //-Patni-DKH/2010Feb01/Added/PH#3#/FBI
}

//*******************************Method Header*********************************
//Method Name    :~CPqmProcon()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
CPqmProcon::~CPqmProcon(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::~CPqmProcon");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProcon::~CPqmProcon"));

    DEL_PTR(m_procon_socket);
    DEL_PTR_ARY(m_scratchpad_file);
    DEL_PTR(m_selectList);

    DEL_PTR(m_probe);
    //Patni-HAR/2009Aug11/Added/Memory Leaks

    DEL_PTR(m_pqm_procon);
}

//+Patni-Hemant/2010Nov13/Added/IR-167_REOPEN_ISSUE
void CPqmProcon::OnProbeClosed(
)
{
    if (m_probe) {
        m_probe->SetProbeMode(-1);
    }

    m_probe_invoked = false;
}
//-Patni-Hemant/2010Nov13/Added/IR-167_REOPEN_ISSUE

//+Patni-PJS/2010Jan2/JaFT/TMSC_REDMINE-111
//*****************************************************************************
//Method Name   : AbortProbeScan
//Author        : Patni/PJS
//Purpose       : This function abort the probe scan
//*****************************************************************************
void CPqmProcon::AbortProbeScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::AbortProbeScan");

    if ((NULL == m_probe) && !m_probe_invoked) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                  _T("m_probe pointer is NULL"));
        return;
    }

    proMsg_t l_msg;
    memset(&l_msg, 0, sizeof(proMsg_t));
    l_msg.length = 0;
    l_msg.data = NULL;
    l_msg.code = PROMSG_PQM_PROBE_ABORT;
    m_probe->SendMsgToProbe(&l_msg);

    GetPQM()->WriteToUI(PQM_MSG_PROBE_SCAN_STARTPAUSE_ENABLE);
}
//-Patni-PJS/2010Jan2/JaFT/TMSC_REDMINE-111

//*******************************Method Header*********************************
//Method Name    :pqmPmLocatorControl()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
//+MEITEC/2010Apr27/Modified/IR-86
//+Patni-SS/13-Jan-10/Modified/MVC005961
bool CPqmProcon::pqmPmLocatorControl(
    CPqmProtocol* f_protocol,
    const int f_index
)
{
    //Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
    LPCTSTR FUNC_NAME = _T("CPqmProcon::pqmPmLocatorControl");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace"));

    // + SM4 LOCATOR_IMAGE Himanshu 3 Dec 2008

    VfPathElem_t    locImages[4] = {0}, locImages2[4] = {0}, locImages3[4] = {0};
    BITFLD_DECL(protocolMask, MAXPROTMASK) = {0};
    //Patni-SS/13-Jan-10/Modified/MVC005961
    bool l_result = false;

    if (m_pqm_procon == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                  _T("m_pqm_procon pointer is NULL"));
        //Patni-SS/13-Jan-10/Modified/MVC005961
        return l_result ;
    }

    if (m_pqm_procon->treeId == 0) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                  _T("PqmPmProlimT.treeId is zero"));
        //Patni-SS/13-Jan-10/Modified/MVC005961
        return l_result ;
    }

    memset(locImages, 0, sizeof(locImages));
    BITFLD_INIT(protocolMask, MAXPROTMASK, FALSE);

    int scanNumber = f_protocol->GetPqmPrMode()->GetScanNumber();
    CString s_scan_number(_T(""));
    s_scan_number.Format(_T("Scan Numebr = %d"), scanNumber);
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, s_scan_number);

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    int l_position = f_protocol->GetPosition();
    CPqmProtocol* l_prev_protocol = NULL, *lastPlan = NULL, *lastDone = NULL;
    CPqmPrMode* l_prev_prot_mode = NULL;

    //Patni-Manish/2009Jun24/Added/MVC2800-JFT319
    for (l_prev_protocol = f_protocol/*l_study->GetItemFromPosition(--l_position)*/;
         l_prev_protocol;
         l_prev_protocol = l_study->GetItemFromPosition(--l_position)) {

        l_prev_prot_mode = l_prev_protocol->GetPqmPrMode();

        //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34
        DBPTR(("%d (%d) scanNumber %d (:%d) acqStatus %d (:%d)\n",
               l_prev_protocol->GetPosition(), l_prev_protocol->GetProtocol(),
               l_prev_prot_mode->GetScanNumber(), scanNumber,
               l_prev_protocol->GetAcqStatus(), C_PR_STATUS_DONE));

        if (l_prev_prot_mode->GetScanNumber() != scanNumber) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      _T("Scan Number of Current Protocol and Previous Protocols are differ"));
            break;
        }

        //+Patni-MP/2009Mar07/Modified/ACE-MCM0213-00155
        if (l_prev_protocol->GetAppCode() != VFC_APP_CODE_MRS &&
            l_prev_protocol->GetAppCode() != VFC_APP_CODE_MRSCSI &&
            l_prev_protocol->GetMovingCouch() != VFC_MOVING_COUCH_CONTINUOUSLY &&
            (l_prev_protocol->GetMovingCouch() != VFC_MOVING_COUCH_MANUAL) &&
            (PqmGetParentIndexFplanning(l_prev_protocol->GetProtocol(), true) >= 0)) {//+Patni-MP/2009Oct07/Modified/MVC005115

            //-Patni-MP/2009Mar07/Modified/ACE-MCM0213-00155

            if ((l_prev_protocol->GetAcqStatus() == C_PR_STATUS_DONE /*&& l_prev_protocol->GetHasImage()*/) ||
                (l_prev_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT &&
                 (l_prev_protocol->GetHasImage() || l_prev_protocol->GetProtocol() == f_index))
               ) {

                BITFLD_SET(protocolMask, l_prev_protocol->GetProtocol());

                if (lastDone == NULL && /*l_prev_protocol->GetHasImage() && */
                    // + SM4 Himanshu Temporary Comment
                    // + SM4 Himanshu Please Remove this comment when PQM will get LAST IMAGE message from IRS
                    !l_prev_protocol->GetShimming() &&
                    (l_prev_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY)) {

                    const int l_parent_index = PqmGetParentIndexFplanning(l_prev_protocol->GetProtocol(), true);
                    CString s_series_loid = (l_parent_index > 0) ? l_study->GetMappedIDForProcessedNode(l_prev_protocol->GetProtocol(), l_parent_index) : l_prev_protocol->GetSeriesLOID();

                    if (!s_series_loid.IsEmpty()) {
                        lastDone = l_prev_protocol;
                    }
                }
            }
        }

        if (lastPlan == NULL && l_prev_protocol->GetHasParent()) {
            lastPlan = l_prev_protocol;
        }
    }

    if (m_pqm_procon->referenceImage == PROLIM_REF_LAST_PLANED_IMAGE) {

        if (lastPlan != NULL) {

            for (int i = 0; i < 4; i++) {
                memcpy(&locImages[i], &lastPlan->GetLocatorImages(i),
                       sizeof(locImages));
                memcpy(&locImages2[i], &lastPlan->GetLocatorImages(i),
                       sizeof(locImages2));
                memcpy(&locImages3[i], &lastPlan->GetLocatorImages(i),
                       sizeof(locImages3));
            }

            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("PROLIM_REF_LAST_PLANED_IMAGE : Using lastPlan for Locator Images"));

        } else if (lastDone != NULL) {
            SearchParentLocatorImage(lastDone->GetProtocol(), locImages,
                                     locImages2, locImages3);

            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("PROLIM_REF_LAST_PLANED_IMAGE : Using lastDone for Locator Images"));
        }

    } else if (m_pqm_procon->referenceImage == PROLIM_REF_LAST_IMAGE) {
        if (lastDone != NULL) {
            SearchParentLocatorImage(lastDone->GetProtocol(), locImages,
                                     locImages2, locImages3);

            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("PROLIM_REF_LAST_IMAGE : Using lastDone for Locator Images"));
        }
    }

    plStatusT plStatus = plStatFailure;

    // Get Series LOID
    //Get Study LOID
    CString s_study_loid = m_pqmptr->GetStudy()->GetStudyLoid();

    //UIDLENGTH is needed for shared memory for locator.
    char l_study_loid[UIDLENGTH] = {'\0'};
    UTIL::Wcs2Mbs(l_study_loid, s_study_loid, UIDLENGTH - 1);

    CString s_series_loid = _T("0");

    if (lastDone) {
        const int l_parent_index = PqmGetParentIndexFplanning(lastDone->GetProtocol(), true);
        s_series_loid = (l_parent_index > 0) ? l_study->GetMappedIDForProcessedNode(lastDone->GetProtocol(), l_parent_index) : lastDone->GetSeriesLOID();

    } else {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("lastDone is NULL. Set dummy Series Loid."));
    }

    //Logging Study and series loid for CString format
    CString l_log_msg = _T("s_study_loid :");
    l_log_msg += s_study_loid + CString(_T(" s_series_loid:")) + s_series_loid;
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_log_msg);

    char l_series_loid[UIDLENGTH] = {'\0'};
    UTIL::Wcs2Mbs(l_series_loid, s_series_loid, UIDLENGTH - 1);

    // for performance, += is better than a + b + c for std::string.
    std::string l_tmp_msg = "l_study_loid :";
    l_tmp_msg += (l_study_loid[0] == '\0') ? "NULL" : l_study_loid;
    l_tmp_msg += "l_series_loid:";
    l_tmp_msg += (l_series_loid[0] == '\0') ? "NULL" : l_series_loid;

    //Logging Study and series loid for char* format
    l_log_msg = _T("");
    UTIL::Mbs2Wcs(&l_log_msg, l_tmp_msg.c_str());
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_log_msg);

    try {
        if (!s_study_loid.IsEmpty() && !s_series_loid.IsEmpty()) {
            plStatus = plPutLocatorInfo(l_study_loid, l_series_loid);

        } else {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("s_series_loid or s_series_loid is Empty"));
        }

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("plPutLocatorInfo() crashed"));
    }

    if (plStatus !=  plStatSuccess) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                  _T("plPutLocatorInfo failed or Could not perform"));

    } else {
        l_result = true;
    }

    //-Patni-SS/13-Jan-10/Modified/MVC005961

    try {
        plStatus = plPutLocatorControl(protocolMask, locImages, locImages2, locImages3);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("plPutLocatorControl() crashed protocolMask=%x locImages=%x locImages2=%x locImages3=%x"),
                              protocolMask, locImages, locImages2, locImages3);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
    }

    if (plStatus !=  plStatSuccess) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                  _T("plPutLocatorControl return other than plStatSuccess"));

    } else {
        l_result = true;
    }

    l_log_msg.Format(_T("Mask:[%04X%04X%04X%04X]"), protocolMask[3], protocolMask[2],
                     protocolMask[1], protocolMask[0]);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_log_msg);

    l_log_msg.Format(_T("Locator Image /%d.%d/%d.%d/%d.%d/%d.%d"),
                     locImages[0].subtree_name, locImages[0].index,
                     locImages[1].subtree_name, locImages[1].index,
                     locImages[2].subtree_name, locImages[2].index,
                     locImages[3].subtree_name, locImages[3].index);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_log_msg);

    if (locImages2[0].subtree_name) {

        l_log_msg.Format(_T("Locator Image /%d.%d/%d.%d/%d.%d/%d.%d"),
                         locImages2[0].subtree_name, locImages2[0].index,
                         locImages2[1].subtree_name, locImages2[1].index,
                         locImages2[2].subtree_name, locImages2[2].index,
                         locImages2[3].subtree_name, locImages2[3].index);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_log_msg);

    }

    if (locImages3[0].subtree_name) {

        l_log_msg.Format(_T("Locator Image /%d.%d/%d.%d/%d.%d/%d.%d"),
                         locImages3[0].subtree_name, locImages3[0].index,
                         locImages3[1].subtree_name, locImages3[1].index,
                         locImages3[2].subtree_name, locImages3[2].index,
                         locImages3[3].subtree_name, locImages3[3].index);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_log_msg);
    }

    return l_result;

}
//-MEITEC/2010Apr27/Modified/IR-86

int CPqmProcon::ProconRequestUiCB(
    int f_tag, const int f_acq_order,
    const bool f_from_server_config ,/*= true*/		//+Patni-HEMANT/ADDED On 6/22/2009 10:40:38 AM/ ACE-2 / WFDA
    const int f_maskValue              //Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::ProconRequestUiCB");
    //+Patni-AMT/2009Jan14/Modified/DeFT# MVC005961-Code Review
    //SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of ProconRequestUiCB"));

    //+Patni-HEMANT/ADDED On 6/24/2009 12:25:16 PM/ ACE-2 / WFDA

    //Note : because server config has CanProcessProconMessage() called initially,
    //we will not call again here, when this function is called from the SeverConfigToProCon()
    //
    //Here , we have pressed procol related buttons, so check we cam proceed or not
    if (!f_from_server_config) {
        //+Patni-PJS/2011Jan25/Modified/IR-97
        /*
        if (!CanProcessProconMessage(f_tag)) {

            return E_ERROR ;
        }
        */
        //-Patni-PJS/2011Jan25/Modified/IR-97
    }

    //-Patni-HEMANT/ADDED On 6/24/2009 12:25:16 PM/ ACE-2 / WFDA

    int l_result = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    int f_prev_tag = m_gui_event;
    m_gui_event = f_tag;

    if (f_tag == PQM_PROCON_SCAN_EDIT) {

        if (!m_scan_editing) {

            StartProbeClientThread(); // temporary for testing
            //+Patni-AMT/2010Jan14/Commented/DeFT# MVC005961 - Code optimization
            l_result = SendMsg2InvokeUi(PROCON_IDENT_WSE);

            if (m_pqmptr->GetMode() == K_IMAGING)
                l_result = SendMsg2InvokeUi(PROCON_IDENT_PROWFDA);

            m_scan_editing = true;
            info_plan_dynamic_protocol.m_master_protocol = NULL;
            info_plan_dynamic_protocol.m_slave_protocol = NULL;
            m_is_plan_dynamic_executed = false;

            if (f_acq_order == m_pqmptr->GetAcqManager()->GetWaitingForAutoScanStartAcqOrder()) {
                m_pqmptr->GetAcqManager()->AutoScanTimerReset();
            }
        }

    } else if (f_tag == PQM_PROCON_SCAN_PLAN) {

        if (!m_is_locator_invoked) {

            StartProbeClientThread();

            //+Patni-SS/ADDED/2009June05/IR-23
            if (!m_scan_editing) {
                l_result = SendMsg2InvokeUi(PROCON_IDENT_WSE);
                l_result = SendMsg2InvokeUi(PROCON_IDENT_PROWFDA);
                m_scan_editing = true;
            }

            l_result = SendMsg2InvokeUi(PROCON_IDENT_LOCATOR);
            m_is_locator_invoked = true;
            //-Patni-AMT/2010Jan14/Commented/DeFT# MVC005961 - Code optimization


            info_plan_dynamic_protocol.m_master_protocol = NULL;
            info_plan_dynamic_protocol.m_slave_protocol = NULL;
            m_is_plan_dynamic_executed = false;

            if (f_acq_order == m_pqmptr->GetAcqManager()->GetWaitingForAutoScanStartAcqOrder()) {
                m_pqmptr->GetAcqManager()->AutoScanTimerReset();
            }
        }

    } else if (f_tag == PQM_PROCON_QUEUE_NEXT) {
        //Patni-RUP/2010Jun24/Added/MVC007947
        m_inscan_switch_recv_flag = false;
        CloseProbePlanDlg();
        l_result = SendMessageToProcon(PROMSG_CMD_QUEUE_NEXT, 0, NULL);

    } else if (f_tag == PQM_PROCON_QUEUE_EXIT) {
        m_inscan_switch_recv_flag = false;
        m_scan_editing = false;
        m_is_locator_invoked = false;

        if (m_curr_protocol) {
            //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
            m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
        }

        //Patni-RUP/2010Jun24/Added/MVC007947
        CloseProbePlanDlg();
        l_result = SendMessageToProcon(PROMSG_CMD_QUEUE_EXIT, 0, NULL);

    }

    //+Patni-HAR/2010Jun21/Commented/V1.20#MVC008371
    //else if (f_tag == PQM_PROCON_NEXT_COPY) {
    //    l_result = SendMessageToProcon(PROMSG_CMD_QUEUE_NEXT_COPY, 0, NULL);
    //}
    //-Patni-HAR/2010Jun21/Commented/V1.20#MVC008371
    else if (f_tag == PQM_PROCON_CANCEL  || f_tag == PQM_PROCON_RESET) {

        if ((PQM_PROCON_RESET == f_tag) && m_reset_flag) {
            m_gui_event = f_prev_tag;
            return E_NO_ERROR;
        }

        m_inscan_switch_recv_flag = false;

        if (m_curr_protocol) {
            //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
            m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
        }

        //Patni-RUP/2010Jun24/Added/MVC007947
        CloseProbePlanDlg();

        l_result = SendMessageToProcon(PROMSG_CMD_EXIT, 0, NULL);

        if (l_result && PQM_PROCON_RESET == f_tag) {

            m_reset_flag = true;

            if (m_is_locator_invoked) {
                m_gui_event = PQM_PROCON_SCAN_PLAN;

            } else if (m_scan_editing) {
                m_gui_event = PQM_PROCON_SCAN_EDIT;

            }
        }

        m_scan_editing = false;
        m_is_locator_invoked = false;


        //+Patni-HEMANT/ADDED On 3/12/2009 6:13:40 PM/ PSP#232
        //
        //Previously, we asumed that we will not be getting any exit message from procon
        //when used presses [Queue and Exit] or [Cancel] button.
        //and that time, when pressing [Cancel], we wimply quit from procon communication.
        //nd that time in case of [Queue and Exit], we are doing nothing, when when PQM GUI is closing
        //and sending us PQM_PROCON_CLOSE message here...
        //But now,
        //when [Queue And Exit] is pressed : we will be getting
        //PROMSG_CMD_SAVE save from procon and that time we need this m_curr_protocol
        //and thus that time we are not doign m_curr_protocol = NULL
        //After that procon will not be sending PROMSG_CMD_EXIT message, in which we have to quit
        //from communication
        //when [Cancel] is pressed : we will be getting
        //PROMSG_CMD_EXIT save from procon and and quit communication
        //Thus after receiving PROMSG_CMD_EXIT message, we have to do followings
        //
        //m_curr_protocol = NULL;
        //
        //-Patni-HEMANT/ ADDED On3/12/2009 6:17:48 PM/

    } else if (f_tag == PQM_PROCON_CLOSE) {

        if (m_curr_protocol) {
            //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
            m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
        }

        m_scan_editing = false;
        m_is_locator_invoked = false;
        l_result = SendMessageToProcon(PROMSG_CMD_CLOSE_PQM, 0, NULL);

        m_curr_protocol = NULL;

        // + SM4 DEFECT 168/424 Himanshu Fix

    } else if (f_tag == PQM_PROCON_TERMINATE) {

        l_result = SendMessageToProcon(PROMSG_CMD_TERMINATE_PQM, 0, NULL);
        // + Patni-HAR 2009Mar06 / Added / Page Reset

    } else if (f_tag == PQM_PROCON_PRO_END) {

        l_result = SendMessageToProcon(PROMSG_CMD_PRO_END, 0, NULL);
        // - Patni-HAR 2009Mar06 / Added / Page Reset

    } else if ((f_tag == PQM_PROCON_PLAN_DUPLICATE) ||
               (f_tag == PQM_PROCON_NEXT_COPY)) { //Patni-HAR/2010Jun21/Added/V1.20#MVC008371

        //+Patni-Ajay/2010July23/Modified/TMSC-REDMINE-443
        //
        //Modified by Ashish On 2/10/2009
        //
        //l_result = SendMessageToProcon(PROMSG_CMD_PLAN_DUPLICATE, i, (void *)chrmask);
        if (f_tag == PQM_PROCON_NEXT_COPY) {
            m_inscan_switch_recv_flag = false;
        }

        int32_t proData[2] = {0, 0};

        //+Patni-HAR/2010Jun21/Added/V1.20#MVC008371
        if (f_tag == PQM_PROCON_PLAN_DUPLICATE) {
            proData[0] = f_acq_order;

        } else {
            proData[0] = m_curr_protocol->GetProtocol();
        }

        if (-1 == f_maskValue) {

            char strfilename[FILENAME_MAX] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
            wchar_t path[MAX_PATH] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
            memset(strfilename, NULL, FILENAME_MAX);

            ::GetEnvironmentVariable(_T("GP_PRO_DIR"), path, MAX_PATH);

            CString l_file_path = path;
            l_file_path += "\\planDuplicate";

            wcstombs(strfilename, l_file_path, l_file_path.GetLength() + 1); //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()

            char* chrmask = new char[MAX_PATH];

            memset(chrmask, NULL, MAX_PATH);

            FILE* file = fopen(strfilename, "r");

            if (file == NULL) {
                return 0;
            }

            int ch = 0, i = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
            ch = '\0';
            i = 0;
            ch = getc(file);

            while (!feof(file)) {
                if (((char)ch != ','))
                    chrmask[i++] = (char)ch;

                else
                    break;

                ch = getc(file);
            }

            chrmask[i] = '\0';

            fclose(file);

            int32_t l_Temp = 0;

            l_Temp = atoi(chrmask);

            proData[1] = l_Temp;

            DEL_PTR_ARY(chrmask);

        } else {
            proData[1] = f_maskValue;
        }

        size_t proDataSize = sizeof(proData);

        //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
        if (proData != NULL) {
            CString l_msg_data = _T("");
            l_msg_data.Format(_T("msg data - %d, %d"), proData[0], proData[1]);

            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_msg_data);

        }

        //-Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
        if (f_tag == PQM_PROCON_PLAN_DUPLICATE) {
            l_result = SendMessageToProcon(PROMSG_CMD_PLAN_DUPLICATE, proDataSize, (void*)proData);

        } else {
            //+Patni-RUP/2010Jun24/Added/MVC007947
            CloseProbePlanDlg();
            //-Patni-RUP/2010Jun24/Added/MVC007947
            l_result = SendMessageToProcon(PROMSG_CMD_QUEUE_NEXT_COPY, proDataSize, (void*)proData);
        }

        //
        //End Of addition By Ashish On 2/10/2009
        //Ashish changes for Plan duplicate End
        //-Patni-Ajay/2010July23/Modified/TMSC-REDMINE-443

    } else if (f_tag == PQM_PROCON_PLAN_DYNAMIC) {
        int32_t proData[2] = { -1, 0};


        info_plan_dynamic_protocol.m_slave_protocol = m_curr_protocol;
        info_plan_dynamic_protocol.m_master_protocol = m_pqmptr->GetStudy()->GetProtocol(f_acq_order);


        size_t proDataSize = sizeof(proData);
        l_result = SendMessageToProcon(PROMSG_CMD_PLAN_DYNAMIC, proDataSize, (void*)proData);

    } else if (f_tag == PQM_PROCON_INVOKE_PROBE_DIALOG) {

        InvokeProbeDialog();
        return E_NO_ERROR;

    }


    //+Patni//Manish//2009May20//Added//CR-77
    else if (f_tag == PQM_PROCON_PLAN_REFER_ROI) {
        CString l_strStudyFilePath = m_pqmptr->GetStudy()->GetStudyPath();

        l_strStudyFilePath += _T(":/1.");

        CString acq_no(_T(""));
        acq_no.Format(_T("%d"), f_acq_order);
        l_strStudyFilePath += acq_no;
        //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
        int l_strStudyFileLength = l_strStudyFilePath.GetLength() + 1;

        //+Patni-ARD/2009Aug12/Modified/Defect fix #MVC003833
        char* l_StudyFilePath = new char[l_strStudyFileLength];
        memset(l_StudyFilePath, 0, sizeof(char) * (l_strStudyFileLength));
        //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
        wcstombs(l_StudyFilePath, l_strStudyFilePath, l_strStudyFileLength);

        //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
        if (l_StudyFilePath != NULL) {
            //+Patni-PJS/2009Aug16/Added/Defect fix #MVC003833
            CString l_msg_data = _T("Message to Procon:PQM_PROCON_PLAN_REFER_ROI");
            l_msg_data += _T(" Study File path:") + l_strStudyFilePath;
            //-Patni-PJS/2009Aug16/Added/Defect fix #MVC003833
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_msg_data);
        }

        //-Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
        l_result = SendMessageToProcon(PROMSG_CMD_PLAN_REFER_ROI, l_strStudyFileLength, l_StudyFilePath);

        DEL_PTR_ARY(l_StudyFilePath);//Patni-MJC/2009Aug17/Modified/cpp test corrections

        //-Patni-ARD/2009Aug12/Modified/Defect fix #MVC003833

    } else if (f_tag == PQM_PROCOCN_INSCAN_OPE_END) {

        l_result = SendMessageToProcon(PROMSG_PQM_INSCAN_OPE_END, 0, NULL);

    } else if (f_tag == PQM_PROCON_INSCAN_SWITCH_DONE) {

        l_result = SendMessageToProcon(PROMSG_PQM_INSCAN_SWITCH_DONE, 0, NULL);
    }

    //-Patni//Manish//2009May20//Added//CR-77
    else {
        // - SM4 DEFECT 168/424 Himanshu  Fix
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Unknown TAG"));
        l_result = FALSE;
        m_scan_editing = false;
        m_is_locator_invoked = false;
    }

    //-Patni-MSN/2009Jun01/Added/Internal Defect

    //+Patni-RSG Modified and Added /2009Jun15/JFT-324
    if (!l_result) {
        return E_ERROR;

    } else {
        return E_NO_ERROR;
    }

    //+Patni-RSG Modified and Added /2009Jun15/JFT-324
    //-Patni-AMT/2009Jan14/Modified/DeFT# MVC005961-Code Review
}

//*******************************Method Header*********************************
//Method Name    :ConnectToProcon()
//Author         :PATNI
//Purpose        :Creates a new socket and connect to the Procon on the IP address
//                  and Port number mentioned in IPCConfig.ini
//*****************************************************************************
//+Patni-HEMANT/ADDED On 3/31/2009 1:20:01 PM/ PSP / IDS#202, IDS#192
//bool CPqmProcon::ConnectToProcon(
//){
bool CPqmProcon::ConnectToProcon(
    bool f_only_try_to_connect /*= false*/
)
{
    //-Patni-HEMANT/ ADDED On3/31/2009 1:19:57 PM/ PSP / IDS#202, IDS#192
    LPCTSTR FUNC_NAME = _T("CPqmProcon::ConnectToProcon");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of ConnectToProcon"));

    ////////////////////////////////////////////////////////
    // below code will be chenged for read port number from system file
    WSADATA l_wsadata = { 0 };
    WSAStartup(MAKEWORD(2, 0), &l_wsadata);

    char service_name[100] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    memset(service_name, NULL, sizeof(char) * 100);

    if (m_pqmptr) {								//Patni-MJC/2009Aug17/Modified/cpp test corrections
        if (m_pqmptr->GetMode() == K_SCHEDULING) {
            strcpy(service_name, "Schedule");
            //	l_str_port.Format(_T("%s"),_T("8888"));

        } else {
            strcpy(service_name, "Acquire");
            //	l_str_port.Format(_T("%s"),_T("8887"));
        }

    } else {										//Patni-MJC/2009Aug17/Modified/cpp test corrections
        strcpy(service_name, "Acquire");
    }

    ////////////////////////////////////////////////////////////////
    bool l_result = false;

    if (WSAStartup(MAKEWORD(1, 1), &l_wsadata) != 0) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("WSAStartup fail"));
        return l_result;
    }

    struct servent* pqmServiceP = getservbyname(service_name, "tcp");

    if (pqmServiceP  == NULL) {
        l_result = false;
        return l_result;
    }

    //Patni-ARD/2010Mar24/Modified/Ph-3# IR-100 Code review
    //int l_error_code = WSAGetLastError();
    CString l_str_port(_T(""));
    l_str_port.Format(_T("%d"), htons(pqmServiceP->s_port));

    CString l_server_ip(_T("127.0.0.1"));

    //Patni-PJS/2009May10/Modify/IR-82 review comment
    if (l_server_ip.GetLength() <= 0 || l_str_port.GetLength() <= 0) {
        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("CPqmProcon.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_CONNECT_TO_PROCON_ERROR")),
            _T("CPqmProcon.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

    } else {
        l_result = true;
    }

    if (!m_procon_socket && l_result) {
        // create socket
        m_procon_socket = new CPQMSocket;
    }

    if (m_procon_socket && m_procon_socket->Create(new CPqmProconNotifier(this), new CPQMProConAdapter())) {

        //+Patni-HEMANT/MODIFIED On 3/31/2009 1:20:01 PM/ PSP / IDS#202, IDS#192
        //
        //		// Connect to procon server
        //		if (m_procon_socket->ConnectProcon(l_server_ip,l_str_port) ){
        //			m_pqmptr->SetIsProconConnected(true);
        //			// send Negotiation message to procon
        //			l_result = NegotiationToProCon() ? true : false;
        //		} else {
        //			m_pqmptr->SetIsProconConnected(false);
        //			l_result = false;
        //		}
        //

        // Connect to procon server
        if (m_procon_socket->ConnectProcon(l_server_ip, l_str_port)) {

            //we would negotiate with protocon only if, the flag tells
            if (!f_only_try_to_connect) {
                m_pqmptr->SetIsProconConnected(true);
                // send Negotiation message to procon
                l_result = NegotiationToProCon() ? true : false;

            } else {

                l_result = true ;
            }

        } else {

            if (!f_only_try_to_connect) {

                m_pqmptr->SetIsProconConnected(false);
            }

            l_result = false;
        }

        //-Patni-HEMANT/MODIFIED On 3/31/2009 1:20:01 PM/ PSP / IDS#202, IDS#192

    } else {
        l_result = false;

    }

    //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
    if (NULL != m_pqmptr) {
        if (NULL != m_pqm_procon) {
            //+ MEITEC/2010Apr23/Modified/IR-86
            //+Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
            DBPTR((_T("planWidth = %d\n"), m_pqm_procon->planWidth));
            DBPTR((_T("planComRightPos = %d\n"), m_pqm_procon->planComRightPos));
            DBPTR((_T("planStsRightPos = %d\n"), m_pqm_procon->planStsRightPos));
            //-Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
            //- MEITEC/2010Apr23/Modified/IR-86
        }
    }

    //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
    return l_result ;
}

//*******************************Method Header*********************************
//Method Name    :OnProconMessage()
//Author         :PATNI
//Purpose        :Process messages received from Procon
//*****************************************************************************
bool CPqmProcon::OnProconMessage(
    const CSocketMsg& f_msg
)
{
    //+Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34-COMMON_CDR_07
    // DebugProconMo is changed to DBPTR() in this function
    LPCTSTR FUNC_NAME = _T("CPqmProcon::OnProconMessage");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace..."));

    //+Patni-MSN/2009Mar28/Added/PSP Internal Defect
    proMsg_t    msg;

    memset(&msg, NULL, sizeof(proMsg_t));
    memcpy(&msg, const_cast<CSocketMsg&>(f_msg).GetBuffer(), sizeof(proMsg_t));

    CString l_msg_code(_T(""));
    //Patni-RUP/2010Jan18/Internal defect fix
    //l_msg_code.Format(_T("Message from Procon = %d "), msg.code);
    //+Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow
    l_msg_code.Format(_T("Msg from Procon:Msg Code=%d Msg=%s"),
                      msg.code, GetMsgStrFromCode(msg.code));
    //-Patni-RUP/2010Jan18/Internal defect fix

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_msg_code);

    if (NULL != m_pqmptr) {
        DBPTR((l_msg_code));
    }

    //-Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow

    if (m_curr_protocol == NULL) return false;

    //Patni-HEMANT/ADDED On 6/22/2009 11:25:40 AM/ ACE-2 / WFDA
    //Patni-PJS/2011Jan25/Modified/IR-97
    //ProcessProcolMsgForInScanServer(msg.code);

    CString     message;
    int         err_code = 0;
    status_t    status = 0;

    switch (msg.code)  {

        case PROMSG_ERROR:          /** == 103 **/
        case PRO_ERRCODE_UI :       /** == 2 **/
        case PRO_ERRCODE_TREE :     /** == 3 **/
        case PRO_ERRCODE_PROLIM:     /** == 1 **/
        case PRO_ERRCODE_PROCON : { /** == 4 **/

            message.Empty();	/* V6.00 */

            /** for debug of proCon (103) **/

            //if(msg.code == PROMSG_ERROR)// Ashish Change for #136
            // 	PqmPmProlim.debug = TRUE;
            if (msg.data != NULL) {
                err_code = (int)msg.data;
                message.Format(_T("PROLIM:\n%s."), ProMsgErrcodeStr(err_code));
                //DB_PRINTF("P",("%s\n",message));
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, message);
            }

            message.Empty(); /* V6.00 */

            CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_PLEASE_RETRY"));

            message.Format(_T("%s %d %d"), temp_string, msg.code, err_code);
            CString l_warnapp_name(_T(""));
            l_warnapp_name.LoadString(IDS_ERROR_APP_NAME);

            // + MEITEC /2010Jan25/Added/MVC006906
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(message,
                    MPlus::EH::Information, _T("CPqmProcon.cpp"), _T("PQM"));
            // - MEITEC /2010Jan25/Added/MVC006906

            m_pqmptr->WriteToUI(PQM_MSG_PROCON_ERR);
        }
        break;

        case PROMSG_CMD_EXIT : {

            if (m_reset_flag && m_can_process_reset) {

                m_pqmptr->WriteToUI(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_CLOSE);
                m_curr_protocol->SetInscanSwitchFlag(false);

                if (E_NO_ERROR == SendServerConfigToProcon()) {
                    break;
                }
            }

            m_curr_protocol->SetInscanSwitchFlag(false);

            CScopedLock l_scoped_lock(m_pqmptr->GetStudy()->GetScanMutex());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

            //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
            m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);

            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      _T("PROMSG_CMD_EXIT - Exited ..."));

            //+Patni-HEMANT/ADDED On 3/12/2009 6:19:19 PM/
            //we are quiting with procon communication here only this message is
            // given by procon at last after it quits this message is the responce
            // of the [Queue and Exit] and [Cancel]
            m_curr_protocol = NULL;
            //-Patni-HEMANT/ ADDED On3/12/2009 6:20:26 PM/
            m_probe->StopProbeOnError();

            //Patni-RUP/2010Jun24/Added/MVC007947
            CloseProbePlanDlg();

            if (m_probe_invoked) {

                e_pqm_req_to_create_probesock = NULL;
                e_pqm_req_to_create_probesock = CreateEvent(NULL, TRUE, TRUE, PQM_REQ_CREATE_SOCK);
                ResetEvent(e_pqm_req_to_create_probesock);

                DWORD l_result = WaitForSingleObject(e_pqm_req_to_create_probesock, INFINITE);

                switch (l_result) {

                    case WAIT_OBJECT_0 : { // event is set
                        ResetEvent(e_pqm_req_to_create_probesock);
                        CloseHandle(e_pqm_req_to_create_probesock);
                        e_pqm_req_to_create_probesock = NULL;

                        proMsg_t l_msg;
                        l_msg.length = 0;
                        l_msg.data = NULL;
                        l_msg.code = PROMSG_PQM_PROBE_PLAN_EXIT;

                        if (m_probe->SendMsgToProbe(&l_msg))
                            OnProbeClosed();

                    }
                    break;

                    case WAIT_TIMEOUT : // waiting time out

                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("WaitForSingleObject(WAIT_TIMEOUT)"));
                        break;

                    case WAIT_FAILED: //waiting failed

                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("WaitForSingleObject(WAIT_FAILED)"));
                        break ;

                }
            }

            //- Patni-MSN/2009Apr13/Added/PSP- Internal Defect Fix
            //+Patni-MSN/2009Jun01/Added/Internal Defect
            m_scan_editing = false;
            m_is_locator_invoked = false;

            if (m_pqmptr->GetSARManager()) {
                m_pqmptr->GetSARManager()->ClearScanEditBuffer();
            }

            //Patni-HEMANT/Commented On 6/22/2009 10:40:38 AM/ ACE-2 / WFDA
            m_gui_event = PQM_PROCON_NO_EVENT;
            //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC36
            m_pqmptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
            m_pqmptr->WriteToUI(PQM_MSG_PROCON_CMD_EXIT);
            DBPTR(("PROMSG_CMD_EXIT code (%d).\n", msg.code));
            m_pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        }
        break;

        case PROMSG_CMD_REFRESH : {
            SendMessageToProcon(PROMSG_PQM_REFRESH_DONE, 0, NULL);
        }
        break;

        case PROMSG_CMD_QUEUE :
        case PROMSG_CMD_SAVE : {
            int	acqStatus = 0;
            BITFLD_DECL(mask, K_ALL_PR) = {0};

            if (msg.code == PROMSG_CMD_SAVE) {
                m_curr_protocol->SetProlimSaveFlag(TRUE);

            } else {
                m_curr_protocol->SetProlimSaveFlag(FALSE);
            }

            memset(mask, 0, sizeof(mask));
            m_pqmptr->GetStudy()->GetProtocolValues(m_pqmptr->GetStudy()->GetVfDBHandle(),
                                                    m_curr_protocol->GetProtocol(), mask,
                                                    K_PR_ACQ_STATUS,	&acqStatus,	NULL,
                                                    NULL);

            if (!m_curr_protocol->GetFluoro() &&
                (acqStatus != C_PR_STATUS_WAIT ||
                 m_curr_protocol->GetAcqStatus() != C_PR_STATUS_WAIT)) {
                //	PqmUiErrorDialog(PqmUiFetchLiteral("WARN_PROTOCOL_COPY"),ERR_ERROR);
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_QUEUE_DONE);
                SendMessageToProcon(PROMSG_PQM_QUEUE_DONE, 0, NULL);

            } else {
                // save protocol data scratchpad to study file
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                          _T("PROMSG_CMD_QUEUE - PqmPmSaveProtocol"));
                //Patni-PJS/2010Jul09/Modified/V1.20#TMSC-REDMINE-441
                if_error(m_pqmptr->GetStudy()->PqmPmSaveProtocol(m_curr_protocol, m_scratchpad_file)) {
                    //if_no_error(m_pqmptr->GetStudy()->PqmPmSaveProtocol(m_curr_protocol, m_scratchpad_file)) {
                    //+Patni-PJS+HAR/2010Jul09/Commented/V1.20#TMSC-REDMINE-441
                    //SendMessageToProcon(PROMSG_PQM_QUEUE_DONE, 0, NULL);

                    //Patni-Hemant+HAR/2010May21/Commented/MaFT/MVC007927
                    //m_pqmptr->WriteToUI(PQM_MSG_PROCON_QUEUE_DONE);

                    //PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                    //          _T("PROMSG_PQM_QUEUE_DONE-PqmPmSaveProtocol"));
                    //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC36
                    //DBPTR(("Send to proCon: PROMSG_PQM_QUEUE_DONE\n"));
                    //-Patni-PJS+HAR/2010Jul09/Commented/V1.20#TMSC-REDMINE-441

                    //Patni-PJS/2010Jul09/Added/V1.20#TMSC-REDMINE-441
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                              _T("PqmPmSaveProtocol failed"));
                }
            }

            m_is_plan_dynamic_executed = false;

            if (m_gui_event == PQM_PROCON_QUEUE_EXIT) {
                //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
                m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_QUEUE_EXIT);
            }

            info_plan_dynamic_protocol.m_master_protocol = NULL;
            info_plan_dynamic_protocol.m_slave_protocol = NULL;


        }
        break;/* PROMSG_CMD_QUEUE */

        case PROMSG_CMD_NEXT_PROTOCOL: {


            //		PqmUiFreeze(PQM_FREEZE_FOR_PROLIM);
            status = SetNextProconCurrent(m_curr_protocol);

            // Defect # 810(b) 22/01/2009 Sarjeet
            //	m_pqmptr->WriteToUI(PQM_MSG_PROCON_QUEUE_NEXT);
            if (status == E_NO_ERROR) {
                // Defect # 810(b) 22/01/2009 Sarjeet
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_QUEUE_NEXT);
                SendMessageToProcon(PROMSG_PQM_NEXT_PROTOCOL_DONE, 0, NULL);
                //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC37
                DBPTR(("Send to proCon: PROMSG_PQM_NEXT_PROTOCOL_DONE\n"));

            } else {
                // Temporary solution to exit WSE in case of Next and Copy
                SendMessageToProcon(PROMSG_CMD_EXIT, 0, NULL); // this call will reset the waiting request of WSE
                m_pqmptr->WriteToUI(PQM_MSG_CLOSE_WSE); // this call will close the WSE
                break;
            }

            m_can_process_reset = true;

            PqmSendProConInitMsgs(m_curr_protocol);
            //		PQM_SET_SELECTED_STATUS();
            //		PqmUiUnfreeze(PQM_FREEZE_FOR_PROLIM);
        }
        break;

        case PROMSG_CON_GATE_INTERVAL_INVOKEPL_REQUEST:
            SetAutoIntervalStatus(AUTO_GATE_INVOKEPL_REQUEST);

        case PROMSG_CON_GATE_INTERVAL_REQUEST : {

            if (msg.code == PROMSG_CON_GATE_INTERVAL_REQUEST) {
                SetAutoIntervalStatus(GATE_INTERVAL_REQUEST);
            }

            int	l_gate_source = 0;

            if (msg.data != NULL) {

                l_gate_source = (int)msg.data;
            }

            CString str(_T(""));
            str.Format(_T("%d"), l_gate_source);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str);

            m_pqm_procon->gate_req = l_gate_source;

            //MRP defect fix
            if (!(m_pqmptr->GetAcqManager()->GetScanProcedure()->GetIsScanning() ||
                  m_pqmptr->GetAcqManager()->GetScanProcedure()->IsSingleProcRunning() ||
                  m_pqmptr->GetPostProcessFlag())) {
                m_pqmptr->GetAcqManager()->GetScanProcedure()->SetScanProcProtocol(m_curr_protocol);
            }

            if (!m_pqmptr->GetAcqManager()->GetScanProcedure()->GetGateInfo(l_gate_source))
                ProlimGateInterval(l_gate_source);
        }
        break;

        case PROMSG_CON_SERVER_CONFIG_DONE: {


            if (m_result_pmLocator && (m_wse_or_locator == PQM_PROCON_SCAN_EDIT || m_wse_or_locator == PQM_PROCON_SCAN_PLAN)) {

                ProconRequestUiCB(m_wse_or_locator);

            } else {

                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Could not send CMD_UI to Invoke WSE or Locator"));
            }

        }

        break;



        case PROMSG_CON_TREE_ID: {

            //+Patni-SS/13-Jan-10/Modified/MVC005961

            memcpy(&msg.data, &const_cast<CSocketMsg&>(f_msg).GetBuffer()[12], sizeof(void*));
            long id = (long)msg.data;

            //memcpy(&id,msg.data,sizeof(long));
            if (m_pqm_procon->treeId != id) {

                //Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
                DBPTR(("PROMSG_CON_TREE_ID Change (%d -> %d)\n",
                       m_pqm_procon->treeId, id));

                if (m_pqm_procon->treeId) {
                    //+Patni-KT/2009Sep23/Modified/Added Try Catch
                    try {
                        plDetachDirSeg();

                    } catch (...) {
                        CString	l_print_string = _T("");
                        l_print_string.Format(_T("plDetachDirSeg() crashed"));
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                                  l_print_string);
                    }

                    //-Patni-KT/2009Sep23/Modified/Added Try Catch
                }

                m_pqm_procon->treeId = (long)msg.data;
                //memcpy(m_pqm_procon->treeId, msg.data, sizeof(long));
                //+Patni-RUP/2009Sep22/Modified/Added Try Catch
                plStatusT l_dir_seg = plStatFailure;

                try {
                    l_dir_seg = plAttachDirSeg(m_pqm_procon->treeId);

                } catch (...) {
                    CString	l_print_string = _T("");
                    l_print_string.Format(_T("plAttachDirSeg() crashed Procon_treeid=%ld"), m_pqm_procon->treeId);
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                              l_print_string);
                }

                if (l_dir_seg == plStatSuccess) {
                    //-Patni-RUP/2009Sep22/Modified/Added Try Catch
                    //+Patni-SS/13-Jan-10/Modified/MVC005961
                    m_result_pmLocator =   pqmPmLocatorControl(m_curr_protocol, -1);
                }

            } else {
                //+Patni-SS/13-Jan-10/Modified/MVC005961
                m_result_pmLocator = pqmPmLocatorControl(m_curr_protocol, -1);
            }

            CString	l_pmLocator = _T("");
            l_pmLocator.Format(_T("%d"), m_result_pmLocator);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("pqmPmLocatorControl = ") + l_pmLocator);

            DBPTR(("PROMSG_CON_TREE_ID (%d)\n", m_pqm_procon->treeId));


        }
        break;

        case PROMSG_CON_CURRENT_PROTOCOL_INDEX : {

            int prot = m_curr_protocol->GetProtocol();
            //Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
            DBPTR(("PROMSG_CON_CURRENT_PROTOCOL_INDEX (%d)\n", prot));
            SendMessageToProcon(PROMSG_PQM_CURRENT_PROTOCOL_INDEX,
                                sizeof(prot), &prot);
        }
        break;

        case PROMSG_CON_PROBE_RUN: {
            if (m_probe) {
                m_probe->SendMsgToProbe(&msg);
            }


            if (!m_probe_invoked || m_pqmptr->GetAcqManager()->GetScanProcedure()->AcqProbingPreScanStart(m_curr_protocol) != true) {
                m_probe->StopProbeOnError(true);
            }

        }
        break;

        case PROMSG_CMD_PROBE_PLAN:

            //+Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094
            if (1 == m_probe->GetProbeMode()) {

                //Display error message if refprobe dialog is open and return
                //+Patni-NP/2010May26/Modified/Redmine-347
                MRERROR_LOG_DATA l_error_log_data;
                MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error;
                CString l_warnapp_name = _T("");
                l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

                l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                 CPqmUtilities::GetMultiLingualString(_T("IDS_MAIN_OR_REFPROBE_SCAN_GOING_ON")),
                                 l_warnapp_name, SCAN_WARN_INFORMATION , SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
                //-Patni-NP/2010May26/Modified/Redmine-347

            } else {
                //-Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094
                //Check is Scanning than show error message

                m_pqmptr->WriteToUI(PQM_MSG_INVOKE_PROBE_DIALOG);

            }//Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094

            break;

        case PROMSG_CON_PROBE_STOP:

            if (m_probe)
                m_probe->SendMsgToProbe(&msg);

            //		{
            ////			/* Motion Probing STOP */
            ////			PqmAcqProbingScanAbort( pmObj->core.prObj );
            ////			DB_PRINTF("P",("PROMSG_PROBE_STOP [%s:%d]\n",__FILE__,__LINE__));
            //		}
            break;

        case PROMSG_CON_INHIBIT_SELECTIONS:
            //+Patni-HEMANT/ADDED On 3/6/2009 9:05:31 PM/ PSP#232,
        {
            m_pqmptr->WriteToUI(PQM_MSG_PROCON_CON_INHIBIT_SELECTIONS);
        }
        //-Patni-HEMANT/ADDED On 3/6/2009 9:05:31 PM/ PSP#232,
        break;

        case PROMSG_CON_ALLOW_SELECTIONS:
            //+Patni-HEMANT/ADDED On 3/6/2009 9:05:31 PM/ PSP#232,
        {

            if (m_reset_flag) {
                break;
            }

            Sleep(LOCATOR_INVOCATION_DELAY);   //Patni-RUP/2010Jul19/Added/TMSC-REDMINE-215
            const int l_acq_order = m_curr_protocol->GetProtocol();
            bool l_is_protocol_inscan = m_pqmptr->GetStudy()->IsProtocolInScan(l_acq_order);

            if (l_is_protocol_inscan && (m_pqmptr->GetMode() == K_IMAGING)) {
                if (!m_inscan_switch_recv_flag) {
                    break;
                }
            }

            m_pqmptr->WriteToUI(PQM_MSG_PROCON_CON_ALLOW_SELECTIONS);

            //+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
            if (m_probe && m_probe->GetProbeMode() == 0) {
                proMsg_t l_probe_msg;
                l_probe_msg.code = PROMSG_CON_ALLOW_SELECTIONS;
                l_probe_msg.data = NULL;
                l_probe_msg.length = 0;
                l_probe_msg.timeStamp = 0;
                m_probe->SendMsgToProbe(&l_probe_msg);
            }

            //-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        }
        //-Patni-HEMANT/ADDED On 3/6/2009 9:05:31 PM/ PSP#232,
        break;

        //+ Patni-MSN/2009Apr09/Added/PSP# 217
        case PROMSG_PROBE_EXIT_DONE: {
            m_probe->OnProbeDisconnect();
        }
        break;

        //- Patni-MSN/2009Apr09/Added/PSP# 217
        //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
        case PROMSG_PROBE_STOP: {
            if (m_probe) {
                m_probe->SendMsgToProbe(&msg);
            }
        }
        break;

        //+Patni-DKH/2010Jan29/Added/PH#3#/FBI
        case PROMSG_CON_GATE_DELAY_REQUEST:
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      _T("Request for gate delay received"));

            if (NULL != m_pqm_procon) {
                //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                if (NULL != m_pqmptr) {
                    DBPTR(("delay=(%d, %d)", m_pqm_procon->delayInfo.delay1,
                           m_pqm_procon->delayInfo.delay2));
                }

                //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow

                if (m_pqm_procon->delayInfo.delay1 == -1 || m_pqm_procon->delayInfo.delay2 == -1) {
                    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                        CPqmUtilities::GetMultiLingualString(_T("IDS_PQM_FBI_GATEVALUE_NOTSET")),
                        MPlus::EH::Information, _T("PQM"), _T("PQM")); //+Patni-NP/2010May26/Modified/Redmine-347
                }

                SendMessageToProcon(PROMSG_PQM_GATE_DELAY, sizeof(delayInfo_t), &(m_pqm_procon->delayInfo));
            }

            break;

            //-Patni-DKH/2010Jan29/Added/PH#3#/IR86/DebugWindow
        case PROMSG_CON_PLAN_DYNAMIC_DONE: {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      _T("Plan dynamic done message recieved."));

            CMasterSlave* l_master_slave_ptr = m_pqmptr->GetMasterSlavePtr();

            if (NULL == l_master_slave_ptr) {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("CMasterSlave class pointer is NULL."));
                break;
            }

            m_is_plan_dynamic_executed = true;
            AutoCopyParameter_type l_parameter;
            l_parameter.m_scratchpad = m_scratchpad_file;

            if (E_NO_ERROR != l_master_slave_ptr->PqmPmSaveForMasterSlaveProtocol(info_plan_dynamic_protocol.m_master_protocol, /*m_scratchpad_file*/l_parameter, PLAN_DYNAMIC)) {

                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Fialed to Save plan dynamic executed protocol."));
            }



            SendMessageToProcon(PROMSG_CMD_REFRESH, 0, NULL);
            break;
        }

        case PROMSG_CON_INSCAN_OPE_REQUEST : {

            char* l_msgdata = new char[ msg.length];

            if (NULL == l_msgdata) {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                          _T("NULL data received."));
                break;
            }

            memset(l_msgdata, 0, msg.length);
            memcpy(l_msgdata, &const_cast<CSocketMsg&>(f_msg).GetBuffer()[12], msg.length);

            CString l_data = (char*)l_msgdata;

            if (l_data.CompareNoCase(_T("Open")) == 0) {
                OutputDebugString(l_data);
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN);

            }	else if (l_data.CompareNoCase(_T("show")) == 0) {
                OutputDebugString(l_data);
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_SHOW);
            }

            delete[] l_msgdata ;
        }
        break;


        case PROMSG_CON_INSCAN_SWITCH : {

            char* l_msgdata = new char[ msg.length];

            if (NULL == l_msgdata) {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                          _T("NULL data received."));
                break;
            }

            m_inscan_switch_recv_flag = true;

            memset(l_msgdata, 0, msg.length);
            memcpy(l_msgdata, &const_cast<CSocketMsg&>(f_msg).GetBuffer()[12], msg.length);

            CString l_data = (char*)l_msgdata;

            delete[] l_msgdata ;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      l_data);

            int l_inscanpos = l_data.Find(_T("\\"));

            if (-1 == l_inscanpos) {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                          _T("Message data format Incorrect"));
                break;
            }

            CString l_inscanflag = l_data.Left(l_inscanpos);

            char* l_intinscan = (char*)LPCTSTR(l_inscanflag);
            int l_inscan = atoi(l_intinscan);

            //check whether string is other than zero.
            if (0 == l_inscan && (!l_inscanflag.CompareNoCase(_T("0")) == 0)) {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                          _T("Invalid Inscan flag"));
                break;
            }


            CString l_mode = l_data.Right(l_data.GetLength() - (l_inscanpos + 1));

            if (l_mode.CompareNoCase(_T("Show")) == 0) {
                m_curr_protocol->SetInscanSwitchFlag(false);
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN);

            } else if (l_mode.CompareNoCase(_T("Switch")) == 0) {
                m_curr_protocol->SetInscanSwitchFlag(true);
                m_curr_protocol->SetInscanSwitchType(l_inscan);
                m_pqmptr->WriteToUI(PQM_MSG_PROCON_INSCAN_SWITCH);

            } else {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                          _T("Incorrect Mode send"));
            }
        }
        break;

        //Previous code
        /*memcpy(&msg.data, &const_cast<CSocketMsg&>(f_msg).GetBuffer()[12], sizeof(void*));

        	int l_inscantype = (int)msg.data;

        	int current_inscantype = m_pqmptr->GetStudy()->GetInscanFlagForProtocolIndex(m_curr_protocol->GetProtocol());

        	if ( current_inscantype!=l_inscantype){
        		m_curr_protocol->SetInscanSwitchFlag(true);
        		m_curr_protocol->SetInscanSwitchType(l_inscantype);
        	}else{
        		m_curr_protocol->SetInscanSwitchFlag(false);
        	}
        	m_pqmptr->WriteToUI(PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN);
        }
        break;*/

        //+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        case PROMSG_CON_TREE_UPDATED: {
            //+Patni-RUP/2010Jun09/Added/MVC007947
            BOOL l_probe_flag = FALSE;

            if (m_probe && m_probe->GetProbeMode() == 0) {
                if (m_probe->GetProbeFlagFromShMemory(l_probe_flag) && FALSE == l_probe_flag) {
                    proMsg_t l_probe_msg;
                    l_probe_msg.code = PROMSG_PQM_PROBE_PLAN_EXIT;
                    l_probe_msg.data = NULL;
                    l_probe_msg.length = 0;
                    l_probe_msg.timeStamp = 0;
                    m_probe->SendMsgToProbe(&l_probe_msg);
                }
            }

            //-Patni-RUP/2010Jun09/Added/MVC007947
            if (m_probe && m_probe->GetProbeMode() == 0) {
                proMsg_t l_probe_msg;
                l_probe_msg.code = PROMSG_CON_TREE_UPDATED;
                l_probe_msg.data = NULL;
                l_probe_msg.length = 0;
                l_probe_msg.timeStamp = 0;
                m_probe->SendMsgToProbe(&l_probe_msg);
            }


        }

        //-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        break;

        case PROMSG_CON_UPDATE_SCAN_EDIT_BUFFER : {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      _T("PROMSG_CON_UPDATE_SCAN_EDIT_BUFFER message received"));
            m_pqmptr->GetSARManager()->UpdateScanEditBuffer();
            SendMessageToProcon(PROMSG_PQM_UPDATE_SCAN_EDIT_BUFFER, 0, NULL);
        }
        break;

        default : {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                      _T("Msg caught in default case..."));
        }
    }

    //-Patni-MSN/2009Mar28/Added/PSP Internal Defect

    //-Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
    return true;
}

//*******************************Method Header*********************************
//Method Name    :OnProconDisconnect()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
//+Patni-HEMANT/ADDED On 3/31/2009 1:17:23 PM/ PSP / IDS#202, IDS#192
//
//
//bool  CPqmProcon::OnProconDisconnect(
//){
bool  CPqmProcon::OnProconDisconnect(
    bool f_create_thread_for_new_connection /*= true*/
)
{
    //-Patni-HEMANT/ ADDED On3/31/2009 1:17:19 PM/
    LPCTSTR FUNC_NAME = _T("CPqmProcon::OnProconDisconnect");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProcon::OnProconDisconnect"));


    DEL_PTR(m_procon_socket);

    //+Patni-HEMANT/ADDED On 3/31/2009 1:18:36 PM/ PSP / IDS#202, IDS#192
    if (!f_create_thread_for_new_connection)
        return true ;

    //-Patni-HEMANT/ADDED On 3/31/2009 1:18:36 PM/ PSP / IDS#202, IDS#192
    m_probe->StopProbeOnError();
    CloseProbePlanDlg();

    //+Patni-SS/2009June02/Internal defects
    if (m_curr_protocol != NULL) {
        //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
        m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
    }

    //+Patni-HEMANT/ADDED On 7/16/2009 10:47:04 AM/ ACE-2 / WFDA
    //process WFDA
    //Patni-PJS/2011Jan25/Modified/IR-97
    //ProcessProcolMsgForInScanServer(PROMSG_CMD_EXIT, true);
    m_gui_event = PQM_PROCON_NO_EVENT;		//flag should get reset
    m_curr_protocol = NULL ;				//ptr uninitlize
    //-Patni-HEMANT/ADDED On 7/16/2009 10:47:04 AM/ ACE-2 / WFDA


    if (m_pqmptr->GetSARManager()) {
        m_pqmptr->GetSARManager()->ClearScanEditBuffer();
    }


    //+Patni-SS/ADDED/2009June05/IR-23
    m_scan_editing = false;
    m_is_locator_invoked = false;
    //+Patni-SS/2009June02/Internal defects
    m_pqmptr->WriteToUI(PQM_MSG_PROCON_NOT_CONNECTED);


    //below code is added for Defect # 676 and 677 by Sarjeet 21/01/2008
    if (m_proconreconnect_thread != NULL) {
        CloseHandle(m_proconreconnect_thread);
        m_proconreconnect_thread = NULL;
    }

    // When connection with proCon get DisConnect Invoke seprate thread to reconnect proCon
    m_proconreconnect_thread = CreateThread(
                                   NULL,                           // default security attributes
                                   NULL,                           // use default stack size
                                   CPqmProcon::ProConReConnectThread,  // thread function
                                   (void*)this,                    // argument to thread function
                                   NULL,                           // use default creation flags
                                   NULL);

    if (m_proconreconnect_thread  == NULL) {

        //+Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_CONNECT_PROCON, _T("IDS_ERR_CONNECT_PROCON"),
        //        SCAN_ERR_NOTIFY, _T("PqmProcon.cpp"));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Connection to proCon server failed  !"));
        //-Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
    }

    //-- Defect # 676 and 677

    return true;
}

//*******************************Method Header*********************************
//Method Name    :SendMessageToProcon()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
bool  CPqmProcon::SendMessageToProcon(
    const int f_code,
    const int f_length,
    void* f_data_p
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::SendMessageToProcon");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProcon::SendMessageToProcon"));
    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    proMsg_t msg;

    msg.code   = f_code;
    msg.timeStamp = clock();
    msg.length = f_length;
    msg.data   = f_data_p;

    PLBYTE  message_to_send = new pbyte[sizeof(pbyte) * PROMSG_HEAD_LEN];
    memcpy(message_to_send, &msg.code, 	PROMSG_HEAD_LEN);

    CSocketMsg message(message_to_send, PROMSG_HEAD_LEN);

    //+Patni-ARD/2009August12/Added/Defect fix #MVC003833
    CString l_str_code = _T("");

    int msg_code = (int)msg.code;
    //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34 changed msg.
    //+Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow
    l_str_code.Format(_T("Send to Procon:Msg Code=%d Msg=%s"), msg_code, GetMsgStrFromCode(msg.code));

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str_code);

    if (NULL != m_pqmptr) {
        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugProMonitor(l_str_code);
    }

    //-Patni-MSN/2010Feb26/Added/PH#3#/IR86/DebugWindow

    bool l_result = false; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    //-Patni-ARD/2009August12/Added/Defect fix #MVC003833
    if (m_procon_socket) {

        l_result = m_procon_socket->SendData(message) ? true : false;

        DEL_PTR_ARY(message_to_send);

        if (l_result && msg.length) {

            PLBYTE message_data = new pbyte[sizeof(pbyte) * f_length];
            memcpy(message_data, msg.data, 	f_length);

            CSocketMsg soc_message(message_data, f_length);
            l_result = m_procon_socket->SendData(soc_message) ? true : false;

            DEL_PTR_ARY(message_data);
        }
    }

    if (!l_result) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_ERR_CONNECT_ACQMAN,
                ERROR_LOG_CRUCIAL1 , _T("CPqmProcon.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
    }

    return l_result	;
}

//*******************************Method Header*********************************
//Method Name    :SeverConfigToProCon()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
int CPqmProcon::SeverConfigToProCon(
    int f_tag,
    const int index,
    int* pro_list,
    const int size
)
{
    //+Patni-AMT/2009Jan14/Modified/DeFT# MVC005961-Code Review
    const CString l_func_name = _T("CPqmProcon::SeverConfigToProCon");
    //SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, l_func_name, _T("SeverConfigToProCon"));

    //Patni-SS/13-Jan-10/Added/MVC005961
    m_gui_event = f_tag;

    m_can_process_reset = true;

    if ((PQM_PROCON_QUEUE_NEXT == f_tag) || (PQM_PROCON_NEXT_COPY == f_tag)) {
        m_can_process_reset = false;
    }

    if (false == m_reset_flag) {
        m_wse_or_locator = f_tag;
    }

    if (m_scan_editing || GetIsProconActive()) {
        return ProconRequestUiCB(f_tag);
    }

    //+Patni-HAR/2010May12/Modified/MVC007826 PQM Crash during WSE operations
    if (m_selectList) {
        DEL_PTR_ARY(m_selectList);
        m_selectCount = 0;
    }

    //m_selectList = pro_list; // set member variable
    m_selectList = new int[size];
    //Patni-Hemant/2010May24/Modified/MVC007826
    memcpy(m_selectList, pro_list, sizeof(*m_selectList)*size);
    m_selectCount = size; // set member variable
    //-Patni-HAR/2010May12/Modified/MVC007826 PQM Crash during WSE operations

    // Now GUI sending protocl acqorder,
    //m_curr_protocol = m_pqmptr->GetStudy()->GetItemFromPosition(index +1);  // set member variable
    m_curr_protocol = m_pqmptr->GetStudy()->GetItemFromProtocol(index);  // set member variable

    return SendServerConfigToProcon(f_tag == K_PM_PROLIM_FLUORO);
}



//*******************************Method Header*********************************
//Method Name    :StopProlim()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
bool CPqmProcon::StopProlim(
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::StopProlim");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
              _T("Trace of CPqmProcon::StopProlim"));
    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008

    proMsg_t l_msg;
    l_msg.code = PROMSG_PROBE_STOP;
    l_msg.length = GetDataForProbe(&l_msg.data);

    //+Patni-KSS/2010Sep02/MODIFIED/TMSC-REDMINE-558-REOPEN
    //+Patni-HAR-KSS/2010Aug18/ADDED/TMSC-REDMINE-558
    //Patni-HAR/2010Aug27/Modified/TMSC-REDMINE-558-Reopen
    l_msg.timeStamp = clock();

    l_msg.code = PROMSG_PQM_PROBE_ABORT;
    m_probe->SendMsgToProbe(&l_msg);
    l_msg.code = PROMSG_PROBE_STOP;

    m_pqmptr->WriteToUI(PQM_MSG_PROBE_SCAN_STARTPAUSE_ENABLE);

    //-Patni-HAR-KSS/2010Aug18/ADDED/TMSC-REDMINE-558
    //-Patni-KSS/2010Sep02/MODIFIED/TMSC-REDMINE-558-REOPEN

    //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
    if (l_msg.data != NULL) {
        CString l_msg_data = _T("");
        l_msg_data.Format(_T("msg data - %s"), l_msg.data);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_msg_data);
    }

    //-Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
    SendMessageToProcon(l_msg.code, l_msg.length, l_msg.data);

    m_pqmptr->GetAcqManager()->GetScanProcedure()->SetProbing(FALSE);

    return true;
}

//*******************************Method Header*********************************
//Method Name    :PqmPqIsGlocOk
//Author         :PATNI
//Purpose        :
//*****************************************************************************
//+Patni-RSG-MN/2009Sept11/Modified/MVC3227/Added flag for calling from PqmPqIsGlocOk
int CPqmProcon::PqmGetParentIndexFplanning(
    int	protocol,
    bool f_bGetModeScanNum
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::PqmGetParentIndexFplanning");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
              _T("PqmGetParentIndexFplanning"));
    CScopedLock l_scoped_lock(&m_pqmGetParentIndex_lock);

    VfPathElem_t    locImages[6] = {0};
    locImages[0].subtree_name = SVN_ST_PROTOCOL;
    locImages[1].subtree_name = SVN_ST_PROCESSED;
    locImages[2].subtree_name = SVN_ST_ECHO;
    locImages[3].subtree_name = SVN_ST_POSITION;
    locImages[0].index = protocol;
    locImages[1].index = 0;
    locImages[2].index = 0;
    locImages[3].index = 0;

    int parent_index = 0;

    if (f_bGetModeScanNum) {

        VfFieldSpec_t   vfReq[1] = {0};
        memset(vfReq, 0, sizeof(VfFieldSpec_t));
        VFF_SET_ARGS(vfReq[0],
                     SVN_PRC_PARENT_INDEX_FOR_PLANNING, SVT_PRC_PARENT_INDEX_FOR_PLANNING,
                     sizeof(VFT_INT32), &parent_index, 0);

        CVarFieldHandle* vf_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

        if (!vf_handle) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("vf_handle is NULL"));
            return 0;
        }

        int	num = 0;

        if (E_NO_ERROR != vf_handle->GetFields(locImages, SVD_ST_PROTOCOL, vfReq, 1, &num)) {
            parent_index = 0;
        }
    }

    return	parent_index ;
}
//-Patni-RSG-MN/2009Sept11/Modified/MVC3227/Added flag for calling from PqmPqIsGlocOk

//*******************************Method Header*********************************
//Method Name    :ProlimGateInterval()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
void CPqmProcon::ProlimGateInterval(
    const int f_gate_method
)
{

    LPCTSTR FUNC_NAME = _T("CPqmProcon::ProlimGateInterval");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of CPqmProcon::ProlimGateInterval"));

    if ((NULL != m_curr_protocol) && (AUTO_GATE_INTERVAL_NONE != m_auto_intervalstatus)) {

        if (AUTO_GATE_INVOKEPL_REQUEST == m_auto_intervalstatus) {
            if (m_pqmptr->GetGateParamObj()->HasDefaultUsableValue(f_gate_method)) {
                m_pqmptr->GetGateParamObj()->UpdateUsableValue(f_gate_method, true);
            }

        } else {
            m_pqmptr->GetGateParamObj()->UpdateUsableValue(f_gate_method, false);
        }

        int32_t l_usable_interval[MAX_GATE_VALS] = {0};
        m_pqmptr->GetGateParamObj()->GetUsableGateVal(f_gate_method, l_usable_interval);

        CString l_str(_T(""));

        l_str.Format(_T("Usable GateInterval to Procon for Gate-Type %d is %d,%d,%d"), m_pqm_procon->gate_req,
                     l_usable_interval[0], l_usable_interval[1], l_usable_interval[2]);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);

        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugProMonitor(l_str);

        SendMessageToProcon((AUTO_GATE_INVOKEPL_REQUEST == m_auto_intervalstatus) ? PROMSG_PQM_GATE_INTERVAL_INVOKEPL_REPLY : PROMSG_PQM_GATE_INTERVAL,
                            sizeof(l_usable_interval), l_usable_interval);

        SetAutoIntervalStatus(AUTO_GATE_INTERVAL_NONE);
        m_pqm_procon->gate_req = -1;
    }
}

// + SM4 LOCATOR_IMAGE Himanshu 3 Dec 2008
//*******************************Method Header*********************************
//Method Name    : SearchParentLocatorImage()
//Author         : PATNI / HAR
//Purpose        : Search Locator Images to Parent
//*****************************************************************************
void CPqmProcon::SearchParentLocatorImage(
    const int f_protocol,
    VfPathElem_t* f_loc_images,
    VfPathElem_t* f_loc_images2,
    VfPathElem_t* f_loc_images3
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::SearchParentLocatorImage");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
              _T("Trace of CPqmProcon::SearchParentLocatorImage"));
    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008

    int                 l_index = -1;
    int                 l_parent_index = 0;
    flt32_t             l_slice_pos = 0.0f, l_slice_pos_max = 0.0f; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    BITFLD_DECL(mask, MAX_BITFLD) = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPQMStudy* l_study = m_pqmptr->GetStudy();

    f_loc_images[0].subtree_name    = SVN_ST_PROTOCOL;
    f_loc_images[1].subtree_name    = SVN_ST_PROCESSED;
    f_loc_images[2].subtree_name    = SVN_ST_ECHO;
    f_loc_images[3].subtree_name    = SVN_ST_POSITION;
    f_loc_images[0].index           = f_protocol;
    f_loc_images[1].index           = 0;
    f_loc_images[2].index           = 0;
    f_loc_images[3].index           = 0;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t       l_vf_req[1] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    memset(l_vf_req, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PRC_PARENT_INDEX_FOR_PLANNING,
                 SVT_PRC_PARENT_INDEX_FOR_PLANNING,
                 sizeof(VFT_INT32),
                 &l_parent_index,
                 0);

    CVarFieldHandle* vf_handle = l_study->GetVfDBHandle();

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("vf_handle is NULL"));
        return;
    }

    int l_num = -1;
    int l_status = vf_handle->GetFields(f_loc_images, SVD_ST_PROTOCOL, l_vf_req, 1, &l_num);

    if (l_status != E_NO_ERROR) {
        l_parent_index = 0;
    }

    f_loc_images[1].index = l_parent_index;

    f_loc_images2[0].subtree_name = SVN_ST_PROTOCOL;
    f_loc_images2[1].subtree_name = SVN_ST_PROCESSED;
    f_loc_images2[2].subtree_name = SVN_ST_ECHO;
    f_loc_images2[3].subtree_name = SVN_ST_POSITION;
    f_loc_images2[0].index = f_protocol;
    f_loc_images2[1].index = l_parent_index;
    f_loc_images2[2].index = 0;
    f_loc_images2[3].index = 0;
    f_loc_images3[0].subtree_name = SVN_ST_PROTOCOL;
    f_loc_images3[1].subtree_name = SVN_ST_PROCESSED;
    f_loc_images3[2].subtree_name = SVN_ST_ECHO;
    f_loc_images3[3].subtree_name = SVN_ST_POSITION;
    f_loc_images3[0].index = f_protocol;
    f_loc_images3[1].index = l_parent_index;
    f_loc_images3[2].index = 0;
    f_loc_images3[3].index = 0;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PRC_SLICE_POSITION,
                 0,
                 sizeof(l_slice_pos),
                 &l_slice_pos,
                 0);

    memset(mask, 0, sizeof(mask));
    int l_total = MAX_BITFLD;
    int l_active = 0;
    CString l_str(_T(""));

    status_t l_scanstatus = vf_handle->ScanSubtree(f_loc_images, SVD_ST_PROCESSED,
                            SVN_ST_ECHO, mask, &l_total, &l_active);
    if_no_error(l_scanstatus) {

        f_loc_images[2].index = 0;
        f_loc_images2[2].index = l_active / 2;
        f_loc_images3[2].index = l_active - 1;

        // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
        l_str.Format(_T("total = %d active = %d"), l_total, l_active);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);
        // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    }

    if (l_active == 1) {
        memset(mask, 0, sizeof(mask));
        l_total = MAX_BITFLD;

        status_t l_scanstatus = vf_handle->ScanSubtree(f_loc_images, SVD_ST_ECHO, SVN_ST_POSITION,
                                mask, &l_total, &l_active);
        if_no_error(l_scanstatus) {

            l_slice_pos_max = 99999;
            l_index = -1;

            for (int i = 0; i < l_total; i++) {

                if (BITFLD_ON(mask, i)) {

                    f_loc_images[3].index = i;

                    //try{
                    status_t l_status = vf_handle->GetFields(f_loc_images, SVD_ST_POSITION,
                                        l_vf_req, 1, &l_num);
                    if_no_error(l_status) {
                        if (l_slice_pos < 0.00001 && l_slice_pos > -0.00001) {

                            l_index = i;
                            break;

                        } else if (fabsf(l_slice_pos) < l_slice_pos_max) {

                            l_index = i;
                            l_slice_pos_max = fabsf(l_slice_pos);
                        }
                    }
                }
            }

            if (l_index != -1) {

                f_loc_images[3].index = l_index;
                f_loc_images2[3].index = l_index;
                f_loc_images3[3].index = l_index;
            }

            // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
            l_str.Format(_T("index = %d"), l_index);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);
            // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
        }

    } else {
        memset(mask, 0, sizeof(mask));
        l_total = MAX_BITFLD;

        status_t l_scanstatus = vf_handle->ScanSubtree(f_loc_images, SVD_ST_ECHO, SVN_ST_POSITION,
                                mask, &l_total, &l_active);
        if_no_error(l_scanstatus) {
            f_loc_images[3].index = l_active / 2;

            // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
            l_str.Format(_T("active > 1  index = %d"), f_loc_images[3].index);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);
            // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
        }

        l_scanstatus = vf_handle->ScanSubtree(f_loc_images2, SVD_ST_ECHO, SVN_ST_POSITION,
                                              mask, &l_total, &l_active);
        if_no_error(l_scanstatus) {

            f_loc_images2[3].index = l_active / 2;

            // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
            l_str.Format(_T("active > 1  index2 = %d"), f_loc_images2[3].index);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);
            // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
        }

        l_scanstatus = vf_handle->ScanSubtree(f_loc_images3, SVD_ST_ECHO, SVN_ST_POSITION,
                                              mask, &l_total, &l_active);
        if_no_error(l_scanstatus) {

            f_loc_images3[3].index = l_active / 2;

            // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
            l_str.Format(_T("active > 1  index3 = %d"), f_loc_images3[3].index);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);
            // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
        }
    }

    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    l_str.Format(_T("%d.%d.%d.%d"), f_loc_images[0].index, f_loc_images[1].index,
                 f_loc_images[2].index, f_loc_images[3].index);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);

    l_str.Format(_T("%d.%d.%d.%d"), f_loc_images2[0].index, f_loc_images2[1].index,
                 f_loc_images2[2].index, f_loc_images2[3].index);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);

    l_str.Format(_T("%d.%d.%d.%d"), f_loc_images3[0].index, f_loc_images3[1].index,
                 f_loc_images3[2].index, f_loc_images3[3].index);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, l_str);
}

//*******************************Method Header*********************************
//Method Name    :PqmPqIsGlocOk
//Author         :PATNI
//Purpose        :
//*****************************************************************************
//+Patni-RSG-MN/2009Sept11/Modified/MVC3227/Added flag for calling from loadstudy
bool_t CPqmProcon::PqmPqIsGlocOk(
    const int pos,
    bool f_bGetModeScanNum
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::PqmPqIsGlocOk");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of CPqmProcon::PqmPqIsGlocOk"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON,  FUNC_NAME,
                  _T("m_pqmptr is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
    CPqmProtocol* obj = NULL;
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugProMonitor(_T("PqmPqIsGlocOk pos = %d wait_pos = %d\n"),
            pos, m_pqmptr->GetStudy()->GetTopWaitPos(&obj));

    //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
    if (pos >= m_pqmptr->GetStudy()->GetTopWaitPos(&obj)) {

        int     scan_number = 0;

        if ((obj = m_pqmptr->GetStudy()->GetItemFromPosition(pos)) != NULL) {

            if (f_bGetModeScanNum) {
                scan_number = obj->GetPqmPrMode()->GetScanNumber();

            } else {
                scan_number = obj->GetNumOfScan();
            }

        } else {
            return FALSE;
        }

        if (obj->GetFluoro()) {
            return FALSE;
        }

        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugProMonitor(_T("[pos=%d,prot=%d] scan_number = %d"),
                obj->GetPosition(), obj->GetProtocol(), scan_number);
        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        CPqmProtocol* l_protocol = NULL;

        for (l_protocol  = m_pqmptr->GetStudy()->GetFirstProtocol();
             l_protocol != NULL;
             l_protocol = m_pqmptr->GetStudy()->GetNextItem(l_protocol)) {
            //+Patni+MP/2009Mar07/Modified/ACE-MCM0213-00155
            if (l_protocol->GetAcqStatus() == C_PR_STATUS_DONE &&
                l_protocol->GetShimming() == 0 && l_protocol->GetHasImage() &&
                l_protocol->GetPqmPrMode()->GetScanNumber() == scan_number &&
                /* MRS/MRSCSI/SPEEDER_MAP/MovingBed‚à‘ÎÛŠO */
                l_protocol->GetAppCode() != VFC_APP_CODE_MRS &&
                l_protocol->GetAppCode() != VFC_APP_CODE_MRSCSI  &&
                (l_protocol->GetMovingCouch() != VFC_MOVING_COUCH_CONTINUOUSLY) &&
                (l_protocol->GetMovingCouch() != VFC_MOVING_COUCH_MANUAL) &&
                (l_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY)) {
                //-Patni-MP/2009Mar07/Modified/ACE-MCM0213-00155
                // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
                CString l_scan_number_str(_T(""));
                l_scan_number_str.Format(_T("LastDone ScanNumber = %d"), l_protocol->GetPqmPrMode()->GetScanNumber());

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_scan_number_str);

                //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                CPqmPrMode* l_pqmprmode = obj->GetPqmPrMode();

                if (l_pqmprmode) {
                    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugProMonitor(_T("LastDone ScanNumber = %d"),
                            obj->GetPqmPrMode()->GetScanNumber());
                }

                //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                if (PqmGetParentIndexFplanning(l_protocol->GetProtocol(), f_bGetModeScanNum) >= 0) {
                    return TRUE;
                }

                // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
            }
        }
    }

    //-Patni-RSG-MN/2009Sept11/Modified/MVC3227/Added flag for calling from loadstudy
    return FALSE;
}

//+Patni-RUP/2010Jan08/Close Porbe defect fix
void CPqmProcon::CloseProbe(void)
{
    if (NULL != m_probe) {
        proMsg_t l_msg;
        l_msg.code = RMC_CMD_EXIT;
        l_msg.data = NULL;
        l_msg.length = 0;
        m_probe->SendMsgToProbe(&l_msg);
    }
}
//Patni-RUP/2010Jan08/Close Porbe defect fix

//+Patni-KSS/2010Oct08/Added/V1.35#IR-167
bool CPqmProcon::ProcessProbeMsgForIRS(const pqm_msg_tag f_msg
                                      ) const
{
    if (m_probe_invoked) {

        bool f_start = false;

        switch (f_msg) {
            case PQM_MSG_IRS_SCANSTART:
            case PQM_MSG_IRS_SCANABORT:

                f_start = (f_msg == PQM_MSG_IRS_SCANSTART);

                proMsg_t l_msg;

                l_msg.data = (void*)(&f_start);
                l_msg.length = sizeof(f_start);

                l_msg.code = PQM_MSG_PROBE_PLAN_SCAN;
                m_probe->SendMsgToProbe(&l_msg);

                return true ;
        }
    }

    return false;
}
//-Patni-KSS/2010Oct08/Added/V1.35#IR-167

//*****************************************************************************
//Method Name   : SetPQMExitFlag
//Author        : PATNI/RUP
//Purpose       : This function used to get the pqm exit flag
//*****************************************************************************
bool CPqmProcon::GetPQMExitFlag()const
{
    return m_pqm_exit;
}

//*****************************************************************************
//Method Name   : SetPQMExitFlag
//Author        : PATNI/RUP
//Purpose       : This function used to set the pqm exit flag
//*****************************************************************************
void CPqmProcon::SetPQMExitFlag(const bool& f_pqm_exit_flag)
{
    m_pqm_exit = f_pqm_exit_flag;
}

//*******************************Method Header*********************************
//Method Name    : GetCurrentEditProt()
//Author         : PATNI / SS
//Purpose        : Search Locator Images to Parent
//*****************************************************************************
int CPqmProcon::GetCurrentEditProt()const
{
    return m_curr_protocol->GetProtocol();
}

//*******************************Method Header*********************************
//Method Name    :SetNextProconCurrent()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
int CPqmProcon::GetDataForProbe(
    void** data
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::GetDataForProbe");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
              _T("Trace of CPqmProcon::GetDataForProbe"));

    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    int		l_dsize = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    //+Patni-MSN/2009Mar28/Added/PSP - Optimization

    if (m_pqmptr) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections
        CString l_studyname = m_pqmptr->GetStudy()->GetStudyName();

        //Patni-SS/20091016/TMSC review comment.
        int protocol = 0, license = 1, l_uicount = 1; // License value should be 1
        l_dsize = sizeof(protocol);
        l_dsize += sizeof(license);
        l_dsize += l_studyname.GetLength() + 1;
        l_dsize += strlen(m_scratchpad_file) + 1;
        l_dsize += sizeof(l_uicount);
        //-Patni-MSN/2009Mar28/Added/PSP - Optimization

        char* l_data = new char[l_dsize];
        memset(l_data, 0, sizeof(char) * l_dsize) ; 	// MEM_INI AMIT 20081230
        char* l_pt = l_data;

        //+Patni-MSN/2009Dec15/Internal defect fix
        //protocol = m_curr_protocol->GetProtocol();
        if (!m_pqmptr->GetAcqManager()->GetScanProcedure()->GetIsScanning()) {
            if (m_curr_protocol) {
                protocol = m_curr_protocol->GetProtocol();
            }

        } else {
            protocol = m_pqmptr->GetAcqManager()->GetScanProcedure()->GetScanProcProtocol()->GetProtocol();
        }

        //+Patni-MSN/2009Dec15/Internal defect fix

        memset(l_pt, 0, l_dsize);
        memcpy(l_pt, &protocol, sizeof(protocol));
        l_pt += sizeof(protocol);

        memcpy(l_pt, &license, sizeof(license));
        l_pt += sizeof(license);
        //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
        int l_len = l_studyname.GetLength() + 1;
        //char * l_study_path = new char[];
        char* l_study_path = new char[l_len];
        memset(l_study_path, 0, sizeof(char) * (l_len)) ; 	// MEM_INI AMIT 20081230
        wcstombs(l_study_path, l_studyname, l_len);
        //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
        memcpy(l_pt, l_study_path, l_studyname.GetLength());
        l_pt += l_studyname.GetLength() + 1;

        memcpy(l_pt, &l_uicount, sizeof(l_uicount));
        l_pt += sizeof(l_uicount);

        memcpy(l_pt, m_scratchpad_file, strlen(m_scratchpad_file));


        DEL_PTR_ARY(l_study_path);

        *data = (void*)l_data;
    }

    return l_dsize;
}

//*******************************Method Header*********************************
//Method Name    :GetPQM()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
CPqm* CPqmProcon::GetPQM()const
{
    return m_pqmptr;
}

//*******************************Method Header*********************************
//Method Name    :GetCurrentProtocol()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPqmProcon::GetCurrentProtocol(
)const
{
    return m_curr_protocol;
}

//+Patni-MSN/2009Mar28/Added/PSP Internal Defect
//*******************************Method Header*********************************
//Method Name    :GetPQMProbe()
//Author         :PATNI/MSN
//Purpose        :
//*****************************************************************************
CPqmProbe* CPqmProcon::GetPQMProbe(
) const
{
    return m_probe;
}
//-Patni-MSN/2009Mar28/Added/PSP Internal Defect

//+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
//**************************************************************************
//Method Name   : GetPqmPmProlim
//Author        : PATNI\RUP
//Purpose       : To get PqmPmProlimT* pointer
//**************************************************************************
PqmPmProlimT* CPqmProcon::GetPqmPmProlim(void)
{
    return m_pqm_procon;
}
//-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC

//+Patni-MP/2009May02/Modified/ACE-MCM0213-00155
bool CPqmProcon::GetIsProconActive(
) const
{
    if (m_curr_protocol != NULL &&
        (m_curr_protocol->GetProcessing() == PROCESSNG_PROEDIT)) {
        return true;

    } else {
        return false;
    }
}
//-Patni-MP/2009May02/Modified/ACE-MCM0213-00155

//+Patni-DKH/2010Jan29/Added/PH#3#/FBI
//**************************************************************************
//Method Name   : SetPqmPmDelay
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
void CPqmProcon::SetPqmPmDelay(delayInfo_t& f_delayinfo)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::SetPqmPmDelay");

    if (NULL == m_pqm_procon) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_procon is NULL"));
        return ;
    }

    m_pqm_procon->delayInfo = f_delayinfo ;
}
//-Patni-DKH/2010Jan29/Added/PH#3#/FBI


//*****************************************************************************
//Method Name   : IsPlanDynamicExecutedForCurrent
//Author        : PATNI/AKR
//Purpose       : This function will return true if plan dynamic is
//				  executed for current editing protocol.
//*****************************************************************************
bool CPqmProcon::IsPlanDynamicExecutedForCurrent() const
{
    return m_is_plan_dynamic_executed;
}


//+Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34
//*****************************************************************************
//Method Name   : ConvertToChar
//Author        : Patni/AMT
//Purpose       : This function used to convert data from CPTString to
//                 char*. Size is hardcode to UIDLENGTH as data are going to
//                 written in procon shared memoery for locator.
//*****************************************************************************
inline char* CPqmProcon::ConvertToChar(
    const CString& f_data
)
{
    if (f_data.IsEmpty()) {
        return NULL;
    }

    int     l_length = f_data.GetLength() + 1;
    char*    l_ret_data = new char[UIDLENGTH];
    memset(l_ret_data, 0, sizeof(char) * UIDLENGTH);    // MEM_INI AMIT 20081230
    wcstombs(l_ret_data, f_data, l_length);

    return l_ret_data;
}
//-Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34


status_t CPqmProcon::CreateProconScratchPad(
    const bool f_fluoro /*=false*/
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::CreateProconScratchPad");
    PQM_SCOPED_TRACE();

    char* path = getenv("GP_TMP_DIR");

    memset(m_scratchpad_file, 0, MAX_BUFFER);

    if (m_pqmptr->GetMode() == K_SCHEDULING) {

        sprintf(m_scratchpad_file, "%s\\PQM.procon.schedule", path);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, CString(m_scratchpad_file));

    } else if (f_fluoro) {

        sprintf(m_scratchpad_file, "%s\\PQM.procon.fluoro", path);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, CString(m_scratchpad_file));

    } else {

        sprintf(m_scratchpad_file, "%s\\PQM.procon.imaging", path);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, CString(m_scratchpad_file));
    }

    // create empty scratchpad
    struct _stat buf;

    if (_stat(m_scratchpad_file, &buf) != 0) {

        FILE* fp = NULL;
        fp = fopen(m_scratchpad_file, "a");

        if (fp) {
            fclose(fp);
        }
    }

    if (E_ERROR == MaintainScratchPadForProcon(m_curr_protocol)) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("error in PqmPmMaintainScratchPad()"));
        m_gui_event = PQM_PROCON_NO_EVENT;
        return E_ERROR;
    }

    if (m_pqmptr->GetSARManager() && m_curr_protocol) {
        m_pqmptr->GetSARManager()->CreateScanEditBuffer(m_curr_protocol->GetProtocol());
    }

    return E_NO_ERROR;
}


status_t CPqmProcon::SendServerConfigToProcon(
    const bool f_fluoro/*=false*/
)
{

    LPCTSTR FUNC_NAME = _T("CPqmProcon::SendServerConfigToProcon");
    PQM_SCOPED_TRACE();

    if (NULL == m_curr_protocol) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("m_curr_protocol is NULL"));
        return E_ERROR;
    }

    if (E_ERROR == CreateProconScratchPad(f_fluoro)) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("error in CreateProconScratchPad()"));
        return E_ERROR;
    }

    m_curr_protocol->SetProcessing(PROCESSNG_PROEDIT);

    int protocol = 0, license = 1; // License value should be 1
    int l_dsize = sizeof(protocol);
    l_dsize += sizeof(license);
    l_dsize += m_pqmptr->GetStudy()->GetStudyName().GetLength() + 1;
    l_dsize += strlen(m_scratchpad_file) + 1;

    char* l_data = new char[l_dsize];

    if (NULL == l_data) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("error in memory new"));
        return E_ERROR;
    }

    memset(l_data, 0, sizeof(l_dsize));
    char* l_pt = l_data;

    CString study = m_pqmptr->GetStudy()->GetStudyName();
    protocol = m_curr_protocol->GetProtocol();

    memset(l_pt, 0, l_dsize);
    memcpy(l_pt, &protocol, sizeof(protocol));
    l_pt += sizeof(protocol);
    memcpy(l_pt, &license, sizeof(license));
    l_pt += sizeof(license);

    int l_len = study.GetLength() + 1;
    char* l_study_path = new char[l_len];

    memset(l_study_path, 0, sizeof(char) * (l_len));
    wcstombs(l_study_path, study, l_len);

    memcpy(l_pt, l_study_path, study.GetLength());
    l_pt += study.GetLength() + 1;
    memcpy(l_pt, m_scratchpad_file, strlen(m_scratchpad_file));


    if (l_data != NULL) {
        CString l_msg_data = _T("");
        l_msg_data.Format(_T("msg data - %s"), l_data);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_msg_data);
    }

    bool l_status = SendMessageToProcon(PROMSG_CMD_SERVER_CONFIG, l_dsize, l_data);
    m_reset_flag = false;

    DEL_PTR_ARY(l_study_path);
    DEL_PTR_ARY(l_data);

    return l_status ? E_NO_ERROR : E_ERROR;

}

//+Patni-AMT/2010Jan14/Added/DeFT# MVC005961 - Code optimization
//**************************************************************************
//Method Name   : CPqmProcon::SendMsg2InvokeUi
//Author        : Patni/AMT
//Purpose       : Send message to Procon to invoke UI specified by f_exe_name
//                  This code contains changes for follwing
//                  //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
//                  IR-23 by SS
//**************************************************************************
bool CPqmProcon::SendMsg2InvokeUi(
    char* f_exe_name
)
{

    LPCTSTR FUNC_NAME = _T("CPqmProcon::SendMsg2InvokeUi");

    if (NULL == f_exe_name) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_exe_name is NULL"));
        return false;
    }

    CString l_exe_name = CString(f_exe_name) ;
    const int l_len      = strlen(f_exe_name) + 1;
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              l_exe_name);

    return SendMessageToProcon(PROMSG_CMD_START_UI, l_len, f_exe_name);
}
//-Patni-AMT/2010Jan14/Added/DeFT# MVC005961 - Code optimization

//*******************************Method Header*********************************
//Method Name    :PqmPmSetNextProtocol()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
void CPqmProcon::PqmPmSetNextProtocol(
    CPqmProtocol* f_current_protocol
)
{
    //Patni-AMT/2010Mar22/Modified/Ph-3# IR-86-TC34
    LPCTSTR FUNC_NAME = _T("CPqmProcon::PqmPmSetNextProtocol");
    //SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace"));

    int next_flg = PROMSG_PQM_NEXT_PROTOCOL_FALSE;

    if (m_selectCount > 1) {

        bool next_protocol_found = false ;

        for (int i = 0; i < m_selectCount; i++) {

            if (m_selectList[i] ==  f_current_protocol->GetProtocol()) {

                // Defect # 810(b) 22/01/2009 Sarjeet
                i++ ;

                for (; i < m_selectCount; i++) {

                    CPqmProtocol* l_nextObj = m_pqmptr->GetStudy()->GetItemFromProtocol(m_selectList[(i)]);

                    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
                    if (l_nextObj == NULL) {
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                                  _T("l_nextObj pointer is NULL"));
                        break;
                    }

                    //-Patni-RUP/2009Aug24/Modified/cpp test corrections

                    if (l_nextObj->GetAcqStatus() == C_PR_STATUS_WAIT) {

                        next_protocol_found = true ;
                        break;
                    }
                }

                if (next_protocol_found) break ;

                // Defect # 810(b) 22/01/2009 Sarjeet
            }
        }

        if (i < m_selectCount) {
            next_flg = PROMSG_PQM_NEXT_PROTOCOL_TRUE;
        }

    } else {
        CPqmProtocol* l_protocol = NULL;

        for (l_protocol = m_pqmptr->GetStudy()->GetNextItem(f_current_protocol);
             l_protocol ;
             l_protocol = m_pqmptr->GetStudy()->GetNextItem(l_protocol)) {

            if (l_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {

                if (!(l_protocol->GetProcessing() & PROCESSNG_COILSEL)) {
                    next_flg = PROMSG_PQM_NEXT_PROTOCOL_TRUE;
                }
            }
        }
    }

    SendMessageToProcon(next_flg, 0, NULL);
    // m_pqmptr->WriteToUI(PQM_MSG_PROCON_QUEUE_NEXT_FALSE);
    //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34
    DBPTR((_T("Send to proCon: PROMSG_PQM_NEXT_PROTOCOL_%s"),
           next_flg == PROMSG_PQM_NEXT_PROTOCOL_TRUE ? _T("TRUE") : _T("FALSE")));
}

//*******************************Method Header*********************************
//Method Name    :PqmSendProConInitMsgs()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
void CPqmProcon::PqmSendProConInitMsgs(
    CPqmProtocol* f_protocol
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::PqmSendProConInitMsgs");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PqmSendProConInitMsgs"));
    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    bool_t	glockflg = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (f_protocol == NULL) {
        return;
    }

    glockflg = (bool_t)PqmPqIsGlocOk(f_protocol->GetPosition());

    PqmPmSetNextProtocol(f_protocol);

    if (glockflg) {
        SendMessageToProcon(PROMSG_PQM_LOCATOR_IMAGES_TRUE, 0, NULL);

    } else {
        SendMessageToProcon(PROMSG_PQM_LOCATOR_IMAGES_FALSE, 0, NULL);
    }

    // PqmSetAutoViewmode( PqmGlobal.autoView ) ;	/* V5.40 */
    if (m_pqmptr->GetAutoView()) {
        SendMessageToProcon(PROMSG_PQM_AUTOVIEW_ON, 0, NULL);

    } else {
        SendMessageToProcon(PROMSG_PQM_AUTOVIEW_OFF, 0, NULL);
    }

}

//*******************************Method Header*********************************
//Method Name    : ProMsgErrcodeStr()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
TCHAR* CPqmProcon::ProMsgErrcodeStr(
    int code
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::ProMsgErrcodeStr");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
              _T("Trace of CPqmProcon::ProMsgErrcodeStr"));
    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    static TCHAR    errbuf[64];

    switch (code) {

        case PRO_ERRCODE_PROLIM:
            return _T("prolim error");
            break;

        case PRO_ERRCODE_UI:
            return _T("ui died too many times");
            break;

        case PRO_ERRCODE_TREE:
            return _T("could not create plsftree");
            break;

        case PRO_ERRCODE_PROCON:
            return _T("proCon error");
            break;

        case PRO_ERRCODE_EXEC:
            return _T("proCon could not exec() after fork()");
            break;

        case PRO_ERRCODE_SPAWN:
            return _T("cannot invoke prolim");
            break;

        case PRO_ERRCODE_STARTUP:
            return _T("cannot make prolim tree");
            break;

        case PRO_ERRCODE_PLSAVEVF:
            return _T("prolim error 'statPlSaveVf'");
            break;

        case PRO_ERRCODE_PLERROR:
            return _T("prolim error 'statPlError'");
            break;

        case PRO_ERRCODE_PLTIMEOUT:
            return _T("prolim error 'statPlTimeOut'");
            break;

        case PRO_ERRCODE_PLDEAD:
            return _T("prolim error 'statPlDead'");
            break;
    }

    wsprintf(errbuf, _T("unknown error code (%d)"), code);
    return errbuf;
}

//*******************************Method Header*********************************
//Method Name    :NegotiationToProCon()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
int CPqmProcon::NegotiationToProCon(
)
{
    CString l_func_name = _T("CPqmProcon::NegotiationToProCon");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, l_func_name, _T("NegotiationToProCon"));

    int l_result    = false;
    char*           negData = (char*)malloc(sizeof(pid_t) + 4);

    if (negData == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, l_func_name, _T("Error in "));
        return l_result;
    }

    pid_t pid = 0l;	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    pid = getpid();
    memset(negData, 0, (sizeof(pid_t) + 4));
    memcpy(negData, &pid, sizeof(pid));
    memcpy(negData + sizeof(pid), PROCON_IDENT_PQM, 4);
    //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
    CString l_msg_data = _T("");
    l_msg_data.Format(_T("process id - %d"), negData);
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, l_func_name, l_msg_data);
    //-Patni-ARD/2009Aug12/Added/Defect fix #MVC003833

    unsigned int    dsize = sizeof(pid_t) + strlen(PROCON_IDENT_PQM) + 1;
    l_result = SendMessageToProcon(PROMSG_CMD_REGIST, dsize, (void*)negData);

    FREE_PTR(negData);

    //+Patni-HEMANT/ADDED On 7/31/2009 12:28:58 PM/ ACE-2 / HANDLE NSDE
    l_msg_data.Empty();

    if (l_result) {
        l_msg_data = _T("PROMSG_CMD_REGIST message sent to Procon");

    } else {
        l_msg_data = _T("Could not send PROMSG_CMD_REGIST message to Procon");
    }

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, l_func_name, l_msg_data);
    //-Patni-HEMANT/ADDED On 7/31/2009 12:28:58 PM/ ACE-2 / HANDLE NSDE
    return l_result;
    // + Patni - MP / 2009Jul23 / ACE-2 Message Change
}


bool CPqmProcon::InvokeProbeDialog(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::InvokeProbeDialog");

    CString l_error_msg = _T("");
    bool l_display_error_in_dialog = false;

    if (!m_pqmptr->GetAcqManager()->GetScanProcedure()->CanInvokeProbe(l_error_msg, l_display_error_in_dialog)) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("CanInvokeProbe() failed"));

        CString l_warnapp_name = _T("");
        l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

        if (l_display_error_in_dialog) {

            MPLUSERRORUSERRESPONSE_e  l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    l_error_msg, l_warnapp_name, SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

        } else {

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                l_error_msg, MPlus::EH::Information, _T("PqmProcon.cpp"), l_warnapp_name);

        }


        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("InvokeProbeDialog() returns FALSE"));
        return false;
    }

    m_probe->SetProbeMode(0);
    PostThreadMessage(CPqmProbe::GetThreadID(), MSG_PROBE, (WPARAM)NULL, (LPARAM)NULL);
    m_probe_invoked = true;

    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("InvokeProbeDialog() returns TRUE"));

    return true;

}

//*******************************Method Header*********************************
//Method Name    :SetNextProconCurrent()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
void CPqmProcon::StartProbeClientThread(
) const
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::StartProbeClientThread");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of CPqmProcon::StartProbeClientThread"));

    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    if (m_probe) {
        m_probe->StartProbeClientThread();
    }
}

//+Patni-RUP/2010Jun24/Added/MVC007947
//*****************************************************************************
//Method Name   : CloseProbePlanDlg
//Author        : Patni/RUP
//Purpose       : This function is used to close the probe plan dialog
//*****************************************************************************
void CPqmProcon::CloseProbePlanDlg()
{
    if (m_probe && m_probe_invoked) {
        SendMessageToProcon(PROMSG_PROBE_EXIT, 0, NULL);
        proMsg_t l_msg;
        l_msg.length = 0;
        l_msg.data = NULL;
        l_msg.code = PROMSG_PQM_PROBE_PLAN_EXIT;

        if (m_probe->SendMsgToProbe(&l_msg))
            OnProbeClosed();

        //+Patni-Hemant/2010Nov13/Modified/IR-167_REOPEN_ISSUE
        //m_probe->SetProbeMode(-1);
        //m_probe_invoked = false;

        //-Patni-Hemant/2010Nov13/Modified/IR-167_REOPEN_ISSUE
    }
}
//-Patni-RUP/2010Jun24/Added/MVC007947


//************************************Method Header************************************
// Method Name  : ProcessProcolMsgForInScanServer
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
void CPqmProcon::ProcessProcolMsgForInScanServer(
    const int32_t f_code,
    const bool f_procon_disconnected /*= false*/
//)
/*
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    int mode = 0;

    if (m_pqmptr) {
        mode = m_pqmptr->GetMode();
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_curr_protocol && (mode == K_IMAGING)) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections

        if ((f_code == PROMSG_CMD_EXIT) || f_procon_disconnected) {
            const int l_acq_order = m_curr_protocol->GetProtocol();
            bool l_is_protocol_inscan = m_pqmptr->GetStudy()->IsProtocolInScan(l_acq_order);

            if (!l_is_protocol_inscan)
                return ;

            //if we have pressed close button on WSE , we will not have m_gui_event evenet here..
            //so default is cancel
            WFDA_Protocol_Status l_wfda_protocol_status =  WFDA_Protocol_WSEEditing_Cancel;

            //Note : m_gui_event should get reset after PROMSG_CMD_EXIT exit only..
            //PROMSG_CMD_EXIT is the last message when editing is finished with Cancel or save

            //Note : m_gui_event will be last button pressed event
            //So when pressing close button
            //on WSE/ when prolim /procon kills/ we will have previous value in m_gui_event.
            //but that time we will process only WFDA_Protocol_WSEEditing_Cancel cancel.
            //If the button pressed is PQM_PROCON_QUEUE_EXIT then only we will save..

            if (!f_procon_disconnected) {
                switch (m_gui_event) {

                    case PQM_PROCON_QUEUE_EXIT :
                        l_wfda_protocol_status = WFDA_Protocol_WSEEditing_Save ;
                        break ;
                }
            }

            //Patni-PJS/2011Jan25/Modified/IR-97
            //OnScanEditStatusChangedForCurrentProtocol(l_wfda_protocol_status, m_curr_protocol);
        }
    }
}*/
//-Patni-PJS/2010Feb2/Modified/IR-97

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
void CPqmProcon::OnScanEditStatusChangedForCurrentProtocol(
    const WFDA_Protocol_Status f_wfda_protocol_status,
    CPqmProtocol* f_protocol
)
{
    if (m_pqmptr) {

        CWFDAImplementer* wfda_implementer = m_pqmptr->GetWFDAImplementer();	//Patni-MJC/2009Aug17/Modified/cpp test corrections

        if (wfda_implementer) {													//Patni-MJC/2009Aug17/Modified/cpp test corrections
            wfda_implementer->OnScanEditStatusChangedForProtocol(f_protocol, f_wfda_protocol_status);
        }
    }
}
*/
//-Patni-PJS/2010Feb2/Modified/IR-97

//************************************Method Header************************************
// Method Name  : CanProcessProconMessage
// Author       : PATNI/ HEMANT
// Purpose      : we can not do Scan edit for inscan kind of seq in schedule mode so return false
//				  when processing for those. else return true.
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
bool CPqmProcon::CanProcessProconMessage(
    int& f_message_code	//this code may get changed according to WFDA
)
{
    if (!m_pqmptr) {
        ASSERT(FALSE);
        return false ;
    }

    int l_new_code = f_message_code;

    //l_protocol_to_check is current editing protocol OR next protocol to be edited
    CPqmProtocol* l_protocol_to_check = NULL;
    CPqmProtocol* l_current_protocol = m_curr_protocol;

    switch (f_message_code) {

        //Patni-PJS/2010Dec10/Modified/IR-97
        case PQM_PROCON_SCAN_PLAN:
        case PQM_PROCON_SCAN_EDIT: {

            l_protocol_to_check = l_current_protocol;
        }
        break ;
        case PQM_PROCON_QUEUE_NEXT :
        case PQM_PROCON_NEXT_COPY : {

            l_protocol_to_check = GetNextProconCurrent();
        }
        break ;

        default :
            return true ;
    }

    if (!l_protocol_to_check) {

        return false;
    }

    //Note : we will process only editing message here.. i.e. Scan Edit, Q&N , N&Copy
    //now we will check weather the current/next protcol to be edited is inscan or not..
    int f_acq_order = l_protocol_to_check->GetProtocol();
    int l_current_protocol_acq_order = (l_protocol_to_check == l_current_protocol) ? f_acq_order : l_current_protocol->GetProtocol();

    bool l_is_protocol_inscan = m_pqmptr->GetStudy()->IsProtocolInScan(f_acq_order);
    bool l_is_protocol_inscan_current_protocol = (l_protocol_to_check == l_current_protocol) ? l_is_protocol_inscan : m_pqmptr->GetStudy()->IsProtocolInScan(l_current_protocol_acq_order);

    if (!l_is_protocol_inscan && !l_is_protocol_inscan_current_protocol) {
        //we can only process Non InScan protocols in schedule page
        return true ;
    }

    if (m_pqmptr->GetMode() != K_IMAGING) {
        //we have inscan protocol here

        //we will display an error message here saying: we can not edit inscan sequnce in schedule
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_CAN_NOT_EDIT_INSCAN_SEQ_IN_SCHEDULE_MODE")),
            MPlus::EH::Crucial, _T("PQM"), _T("PQM"));


        if (l_is_protocol_inscan) {
            //if the next protocol is inscan then only..
            //if its Scan Edit-> l_is_protocol_inscan = (current protocol as inscan)..

            switch (f_message_code) {

                //Patni-PJS/2010Dec10/Modified/IR-97
                case PQM_PROCON_SCAN_PLAN:
                case PQM_PROCON_SCAN_EDIT: {

                    //don't go ahead.. with processing
                    l_new_code = PQM_PROCON_NO_EVENT;
                }
                break ;

                case PQM_PROCON_QUEUE_NEXT :
                case PQM_PROCON_NEXT_COPY : {

                    //we will save the current protocol and will exit.. Not to process next inscan protocol for editing..
                    l_new_code = PQM_PROCON_QUEUE_EXIT ;
                }
                break ;

                default : {
                    ASSERT(FALSE);
                    return true ;
                }
            }
        }

    } else {

        //only acquire can process..so we are in acquire..
        switch (f_message_code) {
            //Patni-PJS/2010Dec10/Modified/IR-97
            case PQM_PROCON_SCAN_PLAN:
            case PQM_PROCON_SCAN_EDIT: {
                //Patni-PJS/2011Jan25/Modified/IR-97
                //OnScanEditStatusChangedForCurrentProtocol(WFDA_Protocol_WSEEditing_Start, m_curr_protocol);
            }
            break ;

            case PQM_PROCON_QUEUE_NEXT :
            case PQM_PROCON_NEXT_COPY : {
                //Patni-PJS/2011Jan25/Modified/IR-97
                //OnScanEditStatusChangedForCurrentProtocol(WFDA_Protocol_WSEEditing_Save, m_curr_protocol);

                if (l_is_protocol_inscan) {
                    //Patni-PJS/2011Jan25/Modified/IR-97
                    //OnScanEditStatusChangedForCurrentProtocol(WFDA_Protocol_WSEEditing_Start, l_protocol_to_check);
                }
            }
            break ;
            default : {
                ASSERT(FALSE);
                return true ;
            }
        }
    }

    f_message_code = l_new_code ;

    //return true when we haven't any message for processing..
    return (f_message_code != PQM_PROCON_NO_EVENT) ;
}
*/
//-Patni-PJS/2010Feb2/Modified/IR-97


//this Method added for Defect # 676 and 677 by Sarjeet 21/01/2008
//*******************************Method Header*********************************
//Method Name    :ProConReConnectThread()
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
DWORD CPqmProcon::ProConReConnectThread(
    LPVOID f_parameter
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::ProConReConnectThread");

    CPqmProcon* l_pqmProcon = (CPqmProcon*)(f_parameter);
    int timeout = 0;

    while (timeout < 100) {
        Sleep(1000);

        //+Patni-HEMANT/ MODIFIED On3/31/2009 1:19:15 PM/ PSP / IDS#202, IDS#192
        //
        //		if(TRUE ==l_pqmProcon->ConnectToProcon()){
        //		l_pqmProcon->m_pqmptr->WriteToUI(PQM_MSG_PROCON_CONNECTED);//	SendMessageToGUI(PQM_MSG_PROCON_CONNECTED);
        //			break;
        //		} else {
        //			if(l_pqmProcon->m_procon_socket){
        //				delete l_pqmProcon->m_procon_socket;
        //				l_pqmProcon->m_procon_socket = NULL;
        //			}
        //		}
        //+Patni-RUP/2010Aug06/Added/To fix PQM exit crash
        if (NULL != l_pqmProcon && false != l_pqmProcon->GetPQMExitFlag()) {
            if (l_pqmProcon->m_procon_socket) {
                DEL_PTR(l_pqmProcon->m_procon_socket);
            }

            break;
        }

        //-Patni-RUP/2010Aug06/Added/To fix PQM exit crash
        //check that procon is ready for connection..
        //this is done by checking socket connection
        if (TRUE == l_pqmProcon->ConnectToProcon(true)) {

            //procon is ready for connection, procon is alive now.
            //Now disconnect explicitly the connection and send message to
            //GUI for re connecting with main thread..(because, this thread will
            //terminate after connection and socket should be connected to alive thread only
            //to communicate)

            //pass false to not to create theread for reconnecting.
            l_pqmProcon->OnProconDisconnect(false);
            l_pqmProcon->m_pqmptr->WriteToUI(PQM_MSG_CONNECT_FROM_MAIN_THREAD);
            break;

        } else {
            if (l_pqmProcon->m_procon_socket) {
                DEL_PTR(l_pqmProcon->m_procon_socket);
            }
        }

        //-Patni-HEMANT/ MODIFIED On3/31/2009 1:19:15 PM/ PSP / IDS#202, IDS#192

        timeout += 20;
    }

    if (timeout >= 100 && !l_pqmProcon->m_pqmptr->GetIsProconConnected()) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("PQM could not connect to prCon !"));

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_CAN_NOT_CONNECT_PROCON, _T("IDS_ERR_CAN_NOT_CONNECT_PROCON"), SCAN_ERR_WARNING,
            _T("CPqmProcon::ProConReConnectThread"));

    }

    ExitThread(0);
    return 0;
}

//************************************Method Header************************************
// Method Name  : GetNextProconCurrent()
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
//+Patni-PJS/2010Feb2/Modified/IR-97
/*
CPqmProtocol* CPqmProcon::GetNextProconCurrent(
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::GetNextProconCurrent");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of GetNextProconCurrent!"));

    if (!m_curr_protocol) {
        return NULL ;
    }

    CPqmProtocol* l_next_procon_current = NULL;

    bool next_protocol_found = false ;

    if (m_selectCount > 1) {
        for (int i = 0; i < m_selectCount; i++) {

            if (m_selectList[i]  == (m_curr_protocol->GetProtocol())) {  // Now GUI sending Acqorder list so its has to be checked by protocol

                // Defect # 810(b) 22/01/2009 Sarjeet

                i++ ;

                for (; i < m_selectCount; i++) {
                    l_next_procon_current = NULL;
                    l_next_procon_current = m_pqmptr->GetStudy()->GetItemFromProtocol(m_selectList[(i)]);

                    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
                    if (l_next_procon_current == NULL) {
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                                  _T("l_next_procon_current is NULL"));
                        break;
                    }

                    //-Patni-RUP/2009Aug24/Modified/cpp test corrections
                    if (l_next_procon_current->GetAcqStatus() == C_PR_STATUS_WAIT) {

                        next_protocol_found = true ;
                        break;
                    }
                }

                if (next_protocol_found)break ;
            }
        }

        if (i < m_selectCount) {
            l_next_procon_current = NULL;
            l_next_procon_current = m_pqmptr->GetStudy()->GetItemFromProtocol(m_selectList[i]);

        } else {
            l_next_procon_current = NULL;
            l_next_procon_current = m_pqmptr->GetStudy()->GetNextWaitProtocol(m_curr_protocol);
        }

    } else {
        l_next_procon_current = NULL;
        l_next_procon_current = m_pqmptr->GetStudy()->GetNextWaitProtocol(m_curr_protocol);
    }

    return l_next_procon_current;
}
*/
//-Patni-PJS/2010Feb2/Modified/IR-97


//*******************************Method Header*********************************
//Method Name    :SetNextProconCurrent()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
status_t CPqmProcon::SetNextProconCurrent(
    CPqmProtocol* f_current_protocol
)
{
    // + SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008
    LPCTSTR FUNC_NAME = _T("CPqmProcon::SetNextProconCurrent");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace of CPqmProcon::SetNextProconCurrent"));
    // - SM4 LOCATOR_IMAGE Himanshu 4 Dec 2008

    CPqmProtocol* nextObj = NULL; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    bool next_protocol_found = false ;

    if (m_selectCount > 1) {
        for (int i = 0; i < m_selectCount; i++) {

            if (m_selectList[i]  == (f_current_protocol->GetProtocol())) {  // Now GUI sending Acqorder list so its has to be checked by protocol

                // Defect # 810(b) 22/01/2009 Sarjeet
                i++ ;

                for (; i < m_selectCount; i++) {
                    nextObj = NULL;
                    nextObj = m_pqmptr->GetStudy()->GetItemFromProtocol(m_selectList[(i)]);

                    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
                    if (nextObj  == NULL) {
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME,
                                  _T("l_nextObj pointer is NULL"));
                        break;
                    }

                    //-Patni-RUP/2009Aug25/Modified/cpp test corrections
                    if (nextObj ->GetAcqStatus() == C_PR_STATUS_WAIT) {

                        next_protocol_found = true ;
                        break;
                    }
                }

                if (next_protocol_found)break ;

                // Defect # 810(b) 22/01/2009 Sarjeet
            }
        }

        if (i < m_selectCount) {
            nextObj = NULL;
            nextObj = m_pqmptr->GetStudy()->GetItemFromProtocol(m_selectList[i]);

        } else {
            nextObj = NULL;
            nextObj = m_pqmptr->GetStudy()->GetNextWaitProtocol(f_current_protocol);
        }

    } else {
        nextObj = NULL;
        nextObj = m_pqmptr->GetStudy()->GetNextWaitProtocol(f_current_protocol);
    }

    status_t status = E_ERROR;

    if (nextObj != NULL) {

        status = MaintainScratchPadForProcon(nextObj);

        if (status == E_NO_ERROR) {
            //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
            m_curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
            m_curr_protocol = nextObj;
            m_curr_protocol->SetProcessing(PROCESSNG_PROEDIT);

            if (m_pqmptr->GetSARManager() && m_curr_protocol) {
                m_pqmptr->GetSARManager()->CreateScanEditBuffer(m_curr_protocol->GetProtocol());
            }
        }

    } else {
        status = E_ERROR;
    }

    return status;
}

//+Patni-RUP/2010Jan19/RMC Trace Implementation
//****************************Method Header************************************
//Method Name   : GetMsgStrFromCode()
//Author        : PATNI\RUP
//Purpose       : To message string from message code
//*****************************************************************************
CString CPqmProcon::GetMsgStrFromCode(const int f_msg_code)
{
    CString l_msg_str = _T("");

    //+Patni-Ravindra Acharya/2010May10/Modified/To fix TMSC-REDMINE-184
    switch (f_msg_code) {
        case PROMSG_PROBE_EXIT_DONE:
            l_msg_str = _T("PROMSG_PROBE_EXIT_DONE");
            break;

        case PROMSG_PROBE_EXIT:
            l_msg_str = _T("PROMSG_PROBE_EXIT");
            break;

        case PROMSG_CMD_PROBE_OK:
            l_msg_str = _T("PROMSG_CMD_PROBE_OK");
            break;

        case PROMSG_PROBE_STOP:
            l_msg_str = _T("PROMSG_PROBE_STOP");
            //Patni-HAR-KSS/2010Aug18/ADDED/TMSC-REDMINE-558
            break;

        case PROMSG_CON_PROBE_RUN:
            l_msg_str = _T("PROMSG_CON_PROBE_RUN");
            break;

        case PROMSG_CON_ALLOW_SELECTIONS:
            l_msg_str = _T("PROMSG_CON_ALLOW_SELECTIONS");
            break;

        case PROMSG_CON_INHIBIT_SELECTIONS:
            l_msg_str = _T("PROMSG_CON_INHIBIT_SELECTIONS");
            break;

        case PROMSG_CON_CURRENT_PROTOCOL_INDEX:
            l_msg_str = _T("PROMSG_CON_CURRENT_PROTOCOL_INDEX");
            break;

        case PROMSG_CON_TREE_ID:
            l_msg_str = _T("PROMSG_CON_TREE_ID");
            break;

        case PROMSG_CON_GATE_INTERVAL_REQUEST:
            l_msg_str = _T("PROMSG_CON_GATE_INTERVAL_REQUEST");
            break;

        case PROMSG_CON_GATE_INTERVAL_INVOKEPL_REQUEST:
            l_msg_str = _T("PROMSG_CON_GATE_INTERVAL_INVOKEPL_REQUEST");
            break;

        case PROMSG_CMD_NEXT_PROTOCOL:
            l_msg_str = _T("PROMSG_CMD_NEXT_PROTOCOL");
            break;

        case PROMSG_CMD_REFRESH:
            l_msg_str = _T("PROMSG_CMD_REFRESH");
            break;

        case PROMSG_CMD_QUEUE:
            l_msg_str = _T("PROMSG_CMD_QUEUE");
            break;

        case PROMSG_CMD_SAVE:
            l_msg_str = _T("PROMSG_CMD_SAVE");
            break;

        case PROMSG_CMD_EXIT:
            l_msg_str = _T("PROMSG_CMD_EXIT");
            break;

        case PROMSG_ERROR:
            l_msg_str = _T("PROMSG_ERROR");
            break;

        case PRO_ERRCODE_UI:
            l_msg_str = _T("PRO_ERRCODE_UI");
            break;

        case PRO_ERRCODE_TREE:
            l_msg_str = _T("PRO_ERRCODE_TREE");
            break;

        case PRO_ERRCODE_PROLIM:
            l_msg_str = _T("PRO_ERRCODE_PROLIM");
            break;

        case PRO_ERRCODE_PROCON:
            l_msg_str = _T("PRO_ERRCODE_PROCON");
            break;

        case PROMSG_CMD_PROBE_PLAN:
            l_msg_str = _T("PROMSG_CMD_PROBE_PLAN");
            break;

            //+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        case PROMSG_CON_TREE_UPDATED:
            l_msg_str = _T("PROMSG_CON_TREE_UPDATED");
            break;

            //-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        case PROMSG_CMD_SAVE_EXIT:
            l_msg_str = _T("PROMSG_CMD_SAVE_EXIT");
            break;

        case PROMSG_CMD_START_UI:
            l_msg_str = _T("PROMSG_CMD_START_UI");
            break;

        case PROMSG_CMD_QUEUE_EXIT:
            l_msg_str = _T("PROMSG_CMD_QUEUE_EXIT");
            break;

        case PROMSG_CMD_QUEUE_NEXT:
            l_msg_str = _T("PROMSG_CMD_QUEUE_NEXT");
            break;

        case PROMSG_CMD_CLOSE:
            l_msg_str = _T("PROMSG_CMD_CLOSE");
            break;

        case PROMSG_CMD_PLAN_DUPLICATE:
            l_msg_str = _T("PROMSG_CMD_PLAN_DUPLICATE");
            break;

        case PROMSG_CMD_QUEUE_PREV:
            l_msg_str = _T("PROMSG_CMD_QUEUE_PREV");
            break;

        case PROMSG_CMD_CANCEL_NEXT:
            l_msg_str = _T("PROMSG_CMD_CANCEL_NEXT");
            break;

        case PROMSG_CMD_CANCEL_PREV:
            l_msg_str = _T("PROMSG_CMD_CANCEL_PREV");
            break;

        case PROMSG_CMD_PREV_PROTOCOL:
            l_msg_str = _T("PROMSG_CMD_PREV_PROTOCOL");
            break;

        case PROMSG_CMD_MAP:
            l_msg_str = _T("PROMSG_CMD_MAP");
            break;

        case PROMSG_CMD_UNMAP:
            l_msg_str = _T("PROMSG_CMD_UNMAP");
            break;

        case PROMSG_CMD_REFRESH_STUDY:
            l_msg_str = _T("PROMSG_CMD_REFRESH_STUDY");
            break;

        case PROMSG_CMD_SERVER_CONFIG:
            l_msg_str = _T("PROMSG_CMD_SERVER_CONFIG");
            break;

        case PROMSG_CMD_PREP_SWITCH:
            l_msg_str = _T("PROMSG_CMD_PREP_SWITCH");
            break;

        case PROMSG_CMD_QUEUE_NEXT_COPY:
            l_msg_str = _T("PROMSG_CMD_QUEUE_NEXT_COPY");
            break;

        case PROMSG_CMD_PROBE_SWITCH:
            l_msg_str = _T("PROMSG_CMD_PROBE_SWITCH");
            break;

        case PROMSG_CMD_REGIST:
            l_msg_str = _T("PROMSG_CMD_REGIST");
            break;

        case PROMSG_CON_PROBE_STOP:
            l_msg_str = _T("PROMSG_CON_PROBE_STOP");
            break;

        case PROMSG_CON_PROBE_EXIT:
            l_msg_str = _T("PROMSG_CON_PROBE_EXIT");
            break;

        case PROMSG_CMD_START_PROBE_PLAN:
            l_msg_str = _T("PROMSG_CMD_START_PROBE_PLAN");
            break;

        case PROMSG_CMD_PROBE_NG:
            l_msg_str = _T("PROMSG_CMD_PROBE_NG");
            break;

        case PROMSG_CMD_NEGOTIATION:
            l_msg_str = _T("PROMSG_CMD_NEGOTIATION");
            break;

        case PROMSG_CMD_QUEUE_LIM_END:
            l_msg_str = _T("PROMSG_CMD_QUEUE_LIM_END");
            break;

        case PROMSG_CMD_LIM_END:
            l_msg_str = _T("PROMSG_CMD_LIM_END");
            break;

        case PROMSG_CMD_CLOSE_PQM:
            l_msg_str = _T("PROMSG_CMD_CLOSE_PQM");
            break;

        case PROMSG_CMD_PRO_END:
            l_msg_str = _T("PROMSG_CMD_PRO_END");
            break;

        case PROMSG_CMD_TERMINATE_PQM:
            l_msg_str = _T("PROMSG_CMD_TERMINATE_PQM");
            break;

        case PROMSG_CMD_PLAN_REFER_ROI:
            l_msg_str = _T("PROMSG_CMD_PLAN_REFER_ROI");
            break;

        case PROMSG_CMD_CLOSE_MEMORY:
            l_msg_str = _T("PROMSG_CMD_CLOSE_MEMORY");
            break;

        case PROMSG_CMD_CLOSE_PROBE_PLAN:
            l_msg_str = _T("PROMSG_CMD_CLOSE_PROBE_PLAN");
            break;

        case PROMSG_CON_WINDOW_ID:
            l_msg_str = _T("PROMSG_CON_WINDOW_ID");
            break;

        case PROMSG_PQM_QUEUE_DONE:
            l_msg_str = _T("PROMSG_PQM_QUEUE_DONE");
            break;

        case PROMSG_PQM_NEXT_PROTOCOL_DONE:
            l_msg_str = _T("PROMSG_PQM_NEXT_PROTOCOL_DONE");
            break;

        case PROMSG_PQM_NEXT_PROTOCOL_TRUE:
            l_msg_str = _T("PROMSG_PQM_NEXT_PROTOCOL_TRUE");
            break;

        case PROMSG_PQM_NEXT_PROTOCOL_FALSE:
            l_msg_str = _T("PROMSG_PQM_NEXT_PROTOCOL_FALSE");
            break;

        case PROMSG_PQM_LOCATOR_IMAGES_TRUE:
            l_msg_str = _T("PROMSG_PQM_LOCATOR_IMAGES_TRUE");
            break;

        case PROMSG_PQM_LOCATOR_IMAGES_FALSE:
            l_msg_str = _T("PROMSG_PQM_LOCATOR_IMAGES_FALSE");
            break;

        case PROMSG_PQM_REFRESH_DONE:
            l_msg_str = _T("PROMSG_PQM_REFRESH_DONE");
            break;

        case PROMSG_PQM_SAVE_DONE:
            l_msg_str = _T("PROMSG_PQM_SAVE_DONE");
            break;

        case PROMSG_PQM_GATE_INTERVAL:
            l_msg_str = _T("PROMSG_PQM_GATE_INTERVAL");
            break;

        case PROMSG_PQM_GATE_INTERVAL_INVOKEPL_REPLY:
            l_msg_str = _T("PROMSG_PQM_GATE_INTERVAL_INVOKEPL_REPLY");
            break;

        case PROMSG_PQM_PREV_PROTOCOL_DONE:
            l_msg_str = _T("PROMSG_PQM_PREV_PROTOCOL_DONE");
            break;

        case PROMSG_PQM_PROEDIT_CTRL:
            l_msg_str = _T("PROMSG_PQM_PROEDIT_CTRL");
            break;

        case PROMSG_PQM_LOCATOR_CTRL:
            l_msg_str = _T("PROMSG_PQM_LOCATOR_CTRL");
            break;

        case PROMSG_CON_LOCATOR_CTRL:
            l_msg_str = _T("PROMSG_CON_LOCATOR_CTRL");
            break;

        case PROMSG_PQM_CURRENT_PROTOCOL_INDEX:
            l_msg_str = _T("PROMSG_PQM_CURRENT_PROTOCOL_INDEX");
            break;

        case PROMSG_PQM_AUTOVIEW_ON:
            l_msg_str = _T("PROMSG_PQM_AUTOVIEW_ON");
            break;

        case PROMSG_PQM_AUTOVIEW_OFF:
            l_msg_str = _T("PROMSG_PQM_AUTOVIEW_OFF");
            break;

        case PROMSG_CON_GATE_DELAY_REQUEST:
            l_msg_str = _T("PROMSG_CON_GATE_DELAY_REQUEST");
            break;

        case PROMSG_PQM_GATE_DELAY:
            l_msg_str = _T("PROMSG_PQM_GATE_DELAY");
            break;

        case PROMSG_CON_RELIMIT:
            l_msg_str = _T("PROMSG_CON_RELIMIT") ;
            break;

        case PROMSG_LIM_RELIMIT_DONE:
            l_msg_str = _T("PROMSG_LIM_RELIMIT_DONE") ;
            break;

        case PROMSG_CON_SAVE_TREE:
            l_msg_str = _T("PROMSG_CON_SAVE_TREE") ;
            break;

        case PROMSG_LIM_SAVE_DONE:
            l_msg_str = _T("PROMSG_LIM_SAVE_DONE") ;
            break;

        case PROMSG_CON_RELOAD_TREE:
            l_msg_str = _T("PROMSG_CON_RELOAD_TREE") ;
            break;

        case PROMSG_CON_PREP_SWITCH:
            l_msg_str = _T("PROMSG_CON_PREP_SWITCH") ;
            break;

        case PROMSG_CON_PROBE_SWITCH:
            l_msg_str = _T("PROMSG_CON_PROBE_SWITCH") ;
            break;

        case PROMSG_LIM_RESPAWN:
            l_msg_str = _T("PROMSG_LIM_RESPAWN") ;
            break;

        case PROMSG_CON_RESET_TREE:
            l_msg_str = _T("PROMSG_CON_RESET_TREE") ;
            break;

        case PROMSG_UI_TREE_ACCESS_REQUEST:
            l_msg_str = _T("PROMSG_UI_TREE_ACCESS_REQUEST") ;
            break;

        case PROMSG_UI_TREE_ACCESS_END:
            l_msg_str = _T("PROMSG_UI_TREE_ACCESS_END") ;
            break;

        case PROMSG_CON_TREE_AVAILABLE:
            l_msg_str = _T("PROMSG_CON_TREE_AVAILABLE") ;
            break;

        case PROMSG_CON_RAISE_WINDOW:
            l_msg_str = _T("PROMSG_CON_RAISE_WINDOW") ;
            break;

        case PROMSG_UI_WINDOW_ID:
            l_msg_str = _T("PROMSG_UI_WINDOW_ID") ;
            break;

        case PROMSG_UI_GATE_INTERVAL_REQUEST:
            l_msg_str = _T("PROMSG_UI_GATE_INTERVAL_REQUEST") ;
            break;

        case PROMSG_CON_GATE_INTERVAL:
            l_msg_str = _T("PROMSG_CON_GATE_INTERVAL") ;
            break;

        case PROMSG_UI_TREE_GRAB_REQUEST:
            l_msg_str = _T("PROMSG_UI_TREE_GRAB_REQUEST") ;
            break;

        case PROMSG_CON_TREE_GRAB_GRANTED:
            l_msg_str = _T("PROMSG_CON_TREE_GRAB_GRANTED") ;
            break;

        case PROMSG_UI_TREE_GRAB_END:
            l_msg_str = _T("PROMSG_UI_TREE_GRAB_END") ;
            break;

        case PROMSG_UI_RELIMIT_REQUEST:
            l_msg_str = _T("PROMSG_UI_RELIMIT_REQUEST") ;
            break;

        case PROMSG_UI_SAVE_TREE:
            l_msg_str = _T("PROMSG_UI_SAVE_TREE") ;
            break;

        case PROMSG_CON_SAVE_TREE_DONE:
            l_msg_str = _T("PROMSG_CON_SAVE_TREE_DONE") ;
            break;

        case PROMSG_CON_LOCATOR_BEGIN:
            l_msg_str = _T("PROMSG_CON_LOCATOR_BEGIN") ;
            break;

        case PROMSG_CON_LOCATOR_END:
            l_msg_str = _T("PROMSG_CON_LOCATOR_END") ;
            break;

        case PROMSG_CON_AUTOVIEW_ON:
            l_msg_str = _T("PROMSG_CON_AUTOVIEW_ON") ;
            break;

        case PROMSG_CON_AUTOVIEW_OFF:
            l_msg_str = _T("PROMSG_CON_AUTOVIEW_OFF") ;
            break;

        case PROMSG_CON_HIDE_UI:
            l_msg_str = _T("PROMSG_CON_HIDE_UI") ;
            break;

        case PROMSG_CON_SHOW_UI:
            l_msg_str = _T("PROMSG_CON_SHOW_UI") ;
            break;

        case PROMSG_CMD_PLAN_DYNAMIC:
            l_msg_str = _T("PROMSG_CMD_PLAN_DYNAMIC") ;
            break;

        case PROMSG_CON_PLAN_DYNAMIC_DONE:
            l_msg_str = _T("PROMSG_CON_PLAN_DYNAMIC_DONE") ;
            break;

        case PROMSG_CON_SERVER_CONFIG_DONE:
            l_msg_str = _T("PROMSG_CON_SERVER_CONFIG_DONE") ;
            break;

        case PROMSG_CON_INSCAN_OPE_REQUEST:
            l_msg_str = _T("PROMSG_CON_INSCAN_OPE_REQUEST") ;
            break;

        case PROMSG_CON_INSCAN_SWITCH:
            l_msg_str = _T("PROMSG_CON_INSCAN_SWITCH") ;
            break;

        case PROMSG_PQM_INSCAN_OPE_END:
            l_msg_str = _T("PROMSG_PQM_INSCAN_OPE_END");
            break;

        case PROMSG_PQM_INSCAN_SWITCH_DONE:
            l_msg_str = _T("PROMSG_PQM_INSCAN_SWITCH_DONE");
            break;

        case PROMSG_CON_UPDATE_SCAN_EDIT_BUFFER:
            l_msg_str = _T("PROMSG_CON_UPDATE_SCAN_EDIT_BUFFER");
            break;

        case PROMSG_PQM_UPDATE_SCAN_EDIT_BUFFER:
            l_msg_str = _T("PROMSG_PQM_UPDATE_SCAN_EDIT_BUFFER");
            break;

            //+Patni-Ravindra Acharya/2010May10/Modified/To fix TMSC-REDMINE-184
        default:
            l_msg_str = _T("UNKNOWN_MESSAGE");
            break;
    }

    return l_msg_str;
}
//-Patni-RUP/2010Jan19/RMC Trace Implementation



//****************************Method Header************************************
//Method Name   : GetMasterSlaveProtocolOnPlanDynimc()
//Author        : PATNI\AKR
//Purpose       :
//*****************************************************************************
CPqmProtocol* CPqmProcon::GetMasterProtocolForPlanDynimc()
{
    return info_plan_dynamic_protocol.m_master_protocol;
}

CPQMSocket* CPqmProcon::GetProconSocket()
{
    return m_procon_socket;
}
//****************************Method Header************************************
//Method Name   : GetAutoIntervalStatus()
//Author        : PATNI
//Purpose       :
//*****************************************************************************

AUTO_GATE_INTERVAL_STATUS CPqmProcon::GetAutoIntervalStatus() const
{

    return m_auto_intervalstatus;
}

//****************************Method Header************************************
//Method Name   : SetAutoIntervalStatus()
//Author        : PATNI
//Purpose       :
//*****************************************************************************

void CPqmProcon::SetAutoIntervalStatus(
    const AUTO_GATE_INTERVAL_STATUS f_auto_intervalstatus
)
{

    m_auto_intervalstatus = f_auto_intervalstatus;
}

//****************************Method Header************************************
//Method Name   : GetProbeSockEventHandle()
//Author        : PATNI
//Purpose       : Get the handle of the Event for request a creation of probe socket
//*****************************************************************************
HANDLE CPqmProcon::GetProbeSockEventHandle()
{
    return e_pqm_req_to_create_probesock;
}
//****************************Method Header************************************
//Method Name   : MaintainScratchPadForProcon()
//Author        : iGate
//Purpose       :
//*****************************************************************************
status_t CPqmProcon::MaintainScratchPadForProcon(CPqmProtocol* f_current_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPqmProcon::MaintainScratchPadForProcon");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("Trace"));

    if (NULL == f_current_protocol) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("f_current_protocol is NULL"));
        return E_ERROR;
    }

    SCRATCHPAD_CREATION_MODE l_sp_creation_mode = SP_CREATE_MODE_WSE;

    if (m_pqmptr->GetIsAMBPlanning()) {
        l_sp_creation_mode = SP_CREATE_MODE_WSE_AMB;

    } else if (m_pqmptr->GetManagerPtr()->IsSelectedSeqMRS(f_current_protocol->GetProtocol())) {

        l_sp_creation_mode = SP_CREATE_MODE_WSE_MRS;
    }

    return m_pqmptr->GetStudy()->PqmPmMaintainScratchPad(f_current_protocol, m_scratchpad_file, l_sp_creation_mode);
}