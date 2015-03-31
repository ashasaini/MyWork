//REDMINE-1213_Update_03_MAY
//REDMINE-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: prescan.cpp
 *  Overview: Implementation of CPreScan class.
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
#include "StdAfx.h"
#include "PreScan.h"
#include <mq.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <tami/libgpdb/coil.h> // Edit by Jitendra for using cdb_coil_type_e
#include <PDataTypes/PTString.h>
//Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
#include <libStudyManager/strcnv.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/pqmcommon.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "PQMIPC.h"
#include "Pqm.h"
#include "StringDefines.h"
//+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "scanprocedure.h"
#include "pqmpmacqman.h"
#include "PqmProbe.h"

#include "PqmMRSScan.h"
#include "PqmPrMode.h"
#include "ProtocolUtil.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "OlpTemp.h"			//+Patni-Ravindra Acharya/2010June25/Added/IR-153
#include "CFShiftCheck.h"
#include "MasterSlave.h"
//+Patni-HEMANT/ADDED On 6/30/2009 7:04:06 PM/
//Patni-PJS/2010Feb2/Modified/IR-97
//#include "WFDAImplementer.h"

//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
#include "CDSUtilities.h"
#include "pqmCDS.h"
#include "pqmAMB.h"
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
#include "PqmPmSARManager.h"
#include "PQMdBdt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//Patni-ARD/20090305/Added/ACE-1# handling of stabilization End
extern "C" {
    extern int AcqFileCopy(char* infile, char* outfile);
}
//Patni-ARD/20090305/Added/ACE-1# handling of stabilization End
#define APC_PAUSE_BTN                           1

//Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
using namespace MR::ACQ::SM::COMMON;

using namespace MPlus::EH;

//*******************************Method Header*********************************
//Method Name    :CPreScan()
//Author         :PATNI/HAR
//Purpose        :Parameterized Construction
//*****************************************************************************
CPreScan::CPreScan(
    CScanProcedure* scan_procedure) : CScanTransaction(scan_procedure
                                                          )
{
    LPCTSTR FUNC_NAME = _T("CPreScan::CPreScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}

//*******************************Method Header*********************************
//Method Name    :~CPreScan()
//Author         :PATNI/HAR
//Purpose        :Destruction
//*****************************************************************************
CPreScan::~CPreScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::~CPreScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//*******************************Method Header*********************************
//Method Name    :StartRFLPrescan()
//Author         :PATNI/MSN
//Purpose        :
//*****************************************************************************
bool CPreScan::StartRFLPrescan(
    const int f_scan_id
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_scan_id);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_scan_procedure) {
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy* l_study = m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy();
    CPqmProtocol* l_protocol = NULL;
    l_study->GetTopWaitPos(&l_protocol);

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    //if (l_protocol == NULL && l_protocol->GetProtocol() < 0) {
    if (l_protocol == NULL || l_protocol->GetProtocol() < 0) {
        //-Patni-RUP/2009Aug25/Modified/cpp test corrections
        //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_ILLEGAL_PROTOCOL"),ERR_ERROR);
        l_study->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        return false;
    }

    //Patni-HAR/2010Jul13/Added/V1.30#MVC008333
    l_study->CopyCurrentProtocol(l_protocol->GetProtocol());

    CPqm* l_pqm = m_scan_procedure->GetAcqManager()->GetPqm();

    if (l_pqm->IsSaturnCoilSystem()) {

        if (AcqCheckSaturnCoil(
                l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil(),
                l_protocol->GetSaturnCoil(),
                FALSE) != E_NO_ERROR) {

            //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_COIL_DATA"),ERR_ERROR);
            return false;
        }

        l_study->PutSaturnCoil(l_study->GetVfDBHandle(), l_protocol->GetProtocol(),
                               l_protocol->GetSaturnCoil());

        l_study->PutConnectedCoil(l_study->GetVfDBHandle(),
                                  l_protocol->GetProtocol(), l_protocol);

        //+Patni-NP/2010MAR16/Added/MCM0229-00166
        //PqmSaturnCoil_p l_saturn_coil = NULL;
        //+Patni-NP/2010MAR16/Added/MCM0229-00166_CDR_01
        PqmSaturnCoil_p l_saturn_coil = l_protocol->GetSaturnCoil();

        if (NULL == l_saturn_coil) {
            return false;
        }

        //-Patni-NP/2010MAR16/Added/MCM0229-00166_CDR_01
        CDataManager* l_data_mgr = l_pqm->GetDataManager();
        CPqmCoilData* l_pqm_coil_data = l_data_mgr->GetPQMCoilData();
        DllT* l_coil_head    = l_pqm_coil_data->GetHead();
        DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();
        AcqGetcoilSignal(l_protocol->GetSystemChannels(), l_saturn_coil->numSection,
                         l_saturn_coil->RcvCoil, l_saturn_coil->Signal,
                         l_coil_head, l_section_head);

        l_study->PutSaturnCoil(l_study->GetVfDBHandle(),
                               l_protocol->GetProtocol(), l_protocol->GetSaturnCoil());

        if (l_pqm_coil_data->PqmCoilIsDegeneracy(
                l_protocol->GetSaturnCoil()->numSection,
                l_protocol->GetSaturnCoil()->RcvCoil)
           ) {
            l_saturn_coil->numSignalDegeneracy = l_pqm_coil_data->PqmCoilGetMinChannel(l_saturn_coil->numSection,
                                                 l_saturn_coil->RcvCoil);
            AcqGetcoilSignal(l_saturn_coil->numSignalDegeneracy, l_saturn_coil->numSection,
                             l_saturn_coil->RcvCoil, l_saturn_coil->SignalDegeneracy,
                             l_coil_head, l_section_head);


        } else {
            l_saturn_coil->numSignalDegeneracy = 0;
        }

        l_study->PutSaturnCoilDegeneracy(l_protocol->GetProtocol(),
                                         l_protocol->GetSaturnCoil());

    }

    //-Patni-NP/2010MAR16/Added/MCM0229-00166
    m_scan_procedure->SetScanProcProtocol(l_protocol);
    m_scan_procedure->SetProt(l_protocol->GetProtocol());
    m_scan_procedure->SetIsScanning(false);
    m_scan_procedure->SetAPCRetry(TRUE);
    m_scan_procedure->SetScanMode(PQM_APC_SCAN);

    if (!PreScanGo(AMCMR_UNKNOWN)) {
        return false;

    } else {
        return true;
    }
}

//*******************************Method Header*********************************
//Method Name    :RetryCFA()
//Author         :PATNI/HAR- MSN
//Purpose        :
//*****************************************************************************
bool CPreScan::RetryCFA(
    const int f_scan_id,
    const CPqmProtocol* f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::RetryCFA");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_scan_id);

    //Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I

    CPqmPmAcqman* acqman = GetScanProcedure()->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();
    CPQMStudy* l_pqm_study = pqm->GetStudy();

    //+Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
    //l_current_protocol = l_pqm_study->GetProtocol(f_scan_id);

    //+Patni-Sribanta/2010Aug11/Modified/TMSC-REDMINE-557
    //l_current_protocol = l_pqm_study->GetLastDone();
    CPqmProtocol* l_current_protocol = l_pqm_study->GetCFStudyProt();
    //-Patni-Sribanta/2010Aug11/Modified/TMSC-REDMINE-557

    if (NULL == l_current_protocol) {
        return false;
    }

    //-Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187

    //+Patni-Sribanta/2010Aug11/Commented/TMSC-REDMINE-557
    //if (l_current_protocol->GetAcqStatus() == C_PR_STATUS_FAIL && f_scan_id == 0) {
    //    return false;
    //}
    //-Patni-Sribanta/2010Aug11/CommentedTMSC-REDMINE-557

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_scan_procedure) {
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    if (f_protocol == NULL) {

        m_scan_procedure->SetCFARetry(PQM_CFA_RETRY);
        m_header.function = AMCMM_PRESCAN;

    } else {

        m_scan_procedure->SetCFARetry(PQM_CFA_RETRY_AND_SCAN);
        CString l_scanproc = m_scan_procedure->GetScanProc(1);

        if (l_scanproc.IsEmpty() &&
            !f_protocol->GetPrepScan() &&
            !f_protocol->GetFluoro() &&
            (f_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP) &&
            (f_protocol->GetShimming() != VFC_SHIM_STANDARD)) {

            m_header.function = AMCMM_PRESCAN_CONTINUE;

        } else {

            m_header.function = AMCMM_PRESCAN;
        }
    }

    //+Patni-DKH+PJS/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
    if (E_NO_ERROR != l_pqm_study->MaintainCfStudy(l_current_protocol, m_data.study_file)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error occured in MaintainCfStudy"));
        pqm->SetPostProcessFlag(FALSE);
        pqm->WriteToUI(PQM_MSG_CFA_PRESCAN_ABORT);

        if (PQM_CFA_RETRY_AND_SCAN == m_scan_procedure->GetCFARetry()) {

            pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
        }

        return false;

    }

    //-Patni-DKH+PJS/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
    //    CString l_study_path(_T("/gp/data/preScan//CfDaily_Current.vfs"));

    //    for ( int index = 0; index < l_study_path.GetLength(); index++) {
    //
    //          m_data.study_file[index] = l_study_path.GetAt(index);
    //      }
    //      m_data.study_file[index] = '\0';


    //_tcscpy(m_data.study_file, l_study);
    m_data.protocol_node = 0;
    m_data.param.preScan.kind[0] = ACQ_PRESCAN_CFA;
    m_data.param.preScan.reqCnt  = 1;

    m_scan_procedure->SetScanProcProtocol(NULL);
    m_scan_procedure->SetProt(0);
    m_scan_procedure->SetIsScanning(false);

    //    l_pqm_study->SetAnatomyInDB(ANATOMY_GUI, l_current_protocol->GetProtocol());

    // + Values that did not upload from Binary study file
    //    l_current_protocol->GetPqmPrMode()->SetRecvCoilId(pqm->GetSelectedCoilKey());
    // - Values that did not upload from Binary study file

    //     scan_proc->SetScanProcProtocol(l_current_protocol);
    //
    //    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    //
    //    if (l_current_protocol == NULL) {
    //        scan_proc->SetCFARetry(PQM_CFA_RETRY);
    //        m_header.function = AMCMM_PRESCAN;
    //    } else {
    //        scan_proc->SetCFARetry(PQM_CFA_RETRY_AND_SCAN);
    //        if ((scan_proc->GetScanProc(1).IsEmpty() ) &&
    //            (!l_current_protocol->GetPrepScan()) &&
    //            (!l_current_protocol->GetFluoro())  &&
    //            (l_current_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP) &&
    //            (l_current_protocol->GetShimming() != VFC_SHIM_STANDARD)
    //            ) {
    //            m_header.function = AMCMM_PRESCAN_CONTINUE;
    //        } else {
    //            m_header.function = AMCMM_PRESCAN;
    //        }
    //    }
    //
    //    m_data.protocol_node = 0;
    //    m_data.param.preScan.kind[0] = ACQ_PRESCAN_CFA;
    //    m_data.param.preScan.reqCnt = 1;
    //
    //    CString l_study_path = _T("");
    //    CString l_pqm_section = _T("Study");
    //    CIniReader ini_reader(CPQMIPCConfigurator::GetConfigFileName());
    //
    //    if ( ini_reader.SectionExists(l_pqm_section) ) {
    //        l_study_path = ini_reader.GetKeyValue(_T("Study Path"),l_pqm_section);
    //    } else {
    //
    //        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_STUDY_NOT_FOUND,
    //            ERROR_LOG_CRUCIAL1 ,_T("PreScan.cpp"));
    //
    //        return false;
    //    }
    //    _tcscpy(m_data.study_file, l_study_path);
    //
    //    scan_proc->SetScanProcProtocol(NULL);
    //    scan_proc->SetProt(0);
    //     scan_proc->SetIsScanning(false);

    //    scan_proc->SetScanStart(PQM_SCAN_START);

    if (!SendMessage()) {
        return false;
    }

    pqm->WriteToUI(PQM_MSG_CFA_PRESCAN_BEGIN);
    return true;
}

//*******************************Method Header*********************************
//Method Name    :PQMManualPrescanApply
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CPreScan::PQMManualPrescanApply(
    VARIANT* f_pdata
)
{
    CScanProcedure* l_scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == l_scan_proc) {
        return;
    }

    if (NULL == m_scan_procedure) {
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    m_scan_procedure->SetProcessing(
        m_scan_procedure->GetProcessing() &
        (~PQM_ACQ_BIT_PRESCAN_HOOK));

    SetPrescan(f_pdata);

    if (!SendPreScan()) {
        CPqm* l_pqmptr = l_scan_proc->GetAcqManager()->GetPqm();
        CPqmProtocol* l_curr_protocol = l_scan_proc->GetScanProcProtocol();

        //	PqmPqSetAcqStatus(PqmAcqGlobal.curObj,C_PR_STATUS_WAIT);
        l_pqmptr->GetStudy()->SetAcqStatus(l_curr_protocol, C_PR_STATUS_WAIT);
        //	PqmAcqClose();
        //	PQM_SET_SELECTED_STATUS();
        //	PqmUiErrorDialog(PqmUiFetchLiteral("ERR_CONNECT_ACQMAN"),ERR_ERROR);
        //	PqmPqSetScanModeOfHead();
        l_pqmptr->GetStudy()->SetScanModeOfHead();
        //    }
        //    request_p = NULL;

    }
}

//*******************************Method Header*********************************
//Method Name    :PQMManualPrescanClose
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CPreScan::PQMManualPrescanClose(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::PQMManualPrescanClose");

    //+ UT_JIT@MANUAL PRESCAN CLOSE CRASH
    if (m_scan_procedure == NULL) {
        return;
    }

    //- UT_JIT@MANUAL PRESCAN CLOSE CRASH
    if (m_scan_procedure->GetIsScanning() == true) {

        m_scan_procedure->SetProcessing(
            m_scan_procedure->GetProcessing() &
            (~PQM_ACQ_BIT_PRESCAN_HOOK));

        CScanProcedure* l_scan_proc = GetScanProcedure();
        CPqm* l_pqmptr = l_scan_proc->GetAcqManager()->GetPqm();
        CPqmProtocol* l_curr_protocol = l_scan_proc->GetScanProcProtocol();


        if (l_curr_protocol != NULL) {
            //PqmPqSetAcqStatus(PqmAcqGlobal.curObj,C_PR_STATUS_WAIT);
            l_pqmptr->GetStudy()->SetAcqStatus(l_curr_protocol, C_PR_STATUS_WAIT);
            l_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            //		PqmAcqClose();
            //		PQM_SET_SELECTED_STATUS();
            l_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            //		PqmUiErrorDialog(PqmUiFetchLiteral("ERR_CONNECT_ACQMAN"),ERR_ERROR);

            //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
            //        SCAN_ERR_NOTIFY, L"prescan.cpp");
            //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));


            l_curr_protocol = NULL;
        }
    }
}

//*******************************Method Header*********************************
//Method Name    :PreScanCheck()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPreScan::PreScanCheck(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::PreScanCheck");
    PQM_SCOPED_TRACE();

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();
    CPqmProtocol* l_current_protocol = scan_proc->GetScanProcProtocol();

    if (scan_proc->GetPrescanHook() &&
        (!(m_scan_procedure->GetPreScanStatus() & STATUS_PRESCANHOOK))) {

        m_scan_procedure->SetPreScanStatus(
            m_scan_procedure->GetPreScanStatus() | STATUS_PRESCANHOOK);

        m_scan_procedure->SetPrescan(K_PRESCAN_MODE_EVERY);
        pqm->WriteToUI(PQM_MSG_ENABLE_APPLY_BTN_MANUAL_PRESCAN);

        m_scan_procedure->SetProcessing(
            m_scan_procedure->GetProcessing() | PQM_ACQ_BIT_PRESCAN_HOOK);

        pqm->WriteToUI(PQM_MSG_MANUAL_PRESCAN_VALUES);

    } else {

        if (!SendPreScan()) {

            pqm->GetStudy()->SetAcqStatus(l_current_protocol, C_PR_STATUS_WAIT) ;
            //      PqmAcqClose();
            //      PQM_SET_SELECTED_STATUS();

            //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
            //        SCAN_ERR_NOTIFY, L"PreScan.cpp");
            //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179


            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

            pqm->GetStudy()->SetScanModeOfHead();

            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}

//*******************************Method Header*********************************
//Method Name    : AcqProbingPreScanStart()
//Author         : PATNI/SG
//Purpose        : Maintain the probe study required for RMC scanning
//*****************************************************************************
bool CPreScan::AcqProbingPreScanStart(
    const int f_scan_id,
    const CPqmProtocol* f_protocol
)
{
    //Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
    LPCTSTR FUNC_NAME = _T("CPreScan::AcqProbingPreScanStart");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_protocol);

    CPqmPmAcqman* acqman = GetScanProcedure()->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();
    CPQMStudy* l_pqm_study = pqm->GetStudy();

    CPqmProtocol* l_current_protocol = l_pqm_study->GetProtocol(f_scan_id);

    if (l_current_protocol->GetAcqStatus() == C_PR_STATUS_FAIL && f_scan_id == 0) {
        return false;
    }

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    TCHAR buf[MAX_PATH] = {0}, path[MAX_PATH] = {0}, tmp_path[MAX_PATH] = {0};
    TCHAR dst[MAX_PATH] = {0};

    if ((::GetEnvironmentVariable(_T("GP_PRO_DIR"), path, MAX_PATH)) != NULL)
        wsprintf(buf , _T("%s%s"), path, PROBE_STUDY_NAME);

    else
        wsprintf(buf, _T(".%s"), PROBE_STUDY_NAME);

    CString l_buff(buf);

    //+ Patni-MSN/2009Dec16/Modify/RMC Positioning
    if (l_pqm_study->StudyCheckPlan(l_buff)  != E_NO_ERROR) {
        CString string = L"";
        string.LoadString(IDS_MSG_SET_PROBE_PLAN);

        //+Patni-Ravindra Acharya/2010May14/Modified/Redmine282
        // CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MSG_SET_PROBE_PLAN, _T("IDS_MSG_SET_PROBE_PLAN"), SCAN_ERR_CRUCIAL, L"PreScan.cpp");
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MSG_SET_PROBE_PLAN, _T("IDS_MSG_SET_PROBE_PLAN"), SCAN_ERR_NOTIFY, L"PreScan.cpp");
        //-Patni-Ravindra Acharya/2010May14/Modified/Redmine282

        //Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
        return false;
    }

    //- Patni-MSN/2009Dec16/Modify/RMC Positioning

    //  TlGetTmpPath(cmd);
    if ((::GetEnvironmentVariable(_T("GP_TMP_DIR"), tmp_path, MAX_PATH)) != NULL)
        wsprintf(dst, _T("%s%s"), tmp_path, PROBE_STUDY_NAME);

    else
        wsprintf(dst, _T(".%s"), PROBE_STUDY_NAME);

    //sprintf( cmd, "copy %s %s", buf, dst ) ;
    //sts = system(cmd);
    //+Patni-ARD/20090306/commented/ACE-1# handling of stabilization
    CString l_dst(dst);
    //CString l_dst(dst);	//Patni-ARD/20090306/commented/ACE-1# handling of stabilization
    //BOOL l_result = CopyFile(l_buff,l_dst,FALSE);

    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int l_len = l_buff.GetLength() + 1;
    char* l_buffer = new char[l_len];

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    if (l_buffer == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_buffer pointer is NULL"));
        return false;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections
    memset(l_buffer, 0, sizeof(char) * (l_len));
    wcstombs(l_buffer, l_buff, l_len);

    l_len = l_dst.GetLength() + 1;
    char* l_destbuffer = new char[l_len];

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    if (l_destbuffer == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_destbuffer pointer is NULL"));
        return false;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections
    memset(l_destbuffer, 0, sizeof(char) * (l_len));
    wcstombs(l_destbuffer, l_dst, l_len);
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int l_result = AcqFileCopy(l_buffer, l_destbuffer);	//Patni-ARD/20090306/Added/ACE-1# handling of stabilization
    //+Patni/2011Jan17/Modified/TMSC-REDMINE-655-Review Comment
    CString l_wcs_source(_T("")), l_wcs_dest(_T(""));
    UTIL::Mbs2Wcs(&l_wcs_source, l_buffer);
    UTIL::Mbs2Wcs(&l_wcs_dest, l_destbuffer);
    DEL_PTR_ARY(l_buffer);
    DEL_PTR_ARY(l_destbuffer);

    if (l_result != E_NO_ERROR) {
        CString l_errmsg = _T("");
        l_errmsg.Format(_T("AcqFileCopy(%s, %s) function FAILED.Error Value : %d."),
                        l_wcs_source, l_wcs_dest, l_result);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errmsg);
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_MSG_STUDY_WRITE_ERROR, _T("IDS_ERR_MSG_STUDY_WRITE_ERROR"),
                SCAN_ERR_NOTIFY, FUNC_NAME);
        //-Patni/2011Jan17/Modified/TMSC-REDMINE-655-Review Comment

        return false;

    }

    //-Patni-DKH+PJS/2010Dec15/Modified/TMSC-REDMINE-655-Part-I

    //-Patni-ARD/20090306/commented/ACE-1# handling of stabilization
    m_header.function = AMCMM_PRESCAN;
    m_data.protocol_node = 0;
    m_data.param.preScan.kind[0] = ACQ_PRESCAN_CFA;
    m_data.param.preScan.reqCnt  = 1;

    m_scan_procedure->SetScanProcProtocol(NULL);
    m_scan_procedure->SetProt(0);
    m_scan_procedure->SetIsScanning(false);
    m_scan_procedure->SetProbing(true);

    CString l_prob_study_file(dst);
    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = l_prob_study_file.GetLength() + 1;

    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));	// MEM_INI AMIT 20081230
    wcstombs(char_string, dst, n);
    //char_string[n] = '\0';
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    status_t	sts = MaintainMotionProbingStudy(char_string, l_current_protocol);

    //+Patni-HAR/2009Aug11/Added/Memory Leaks
    DEL_PTR_ARY(char_string);

    //-Patni-HAR/2009Aug11/Added/Memory Leaks
    //+Patni-MSN/2009Mar10/Modified/Optimization
    if (sts == E_NO_ERROR) {
        m_scan_procedure->SetProbing(true);
        wcscpy(m_data.study_file, l_prob_study_file);
        m_data.protocol_node = 0 ;

        if (!SendMessage()) {
            return false;
        }

        //+Patni-PJS/2010Jan2/JaFT/TMSC_REDMINE-111

    } else {
        return false;
    }

    //-Patni-PJS/2010Jan2/JaFT/TMSC_REDMINE-111
    return true;
}

//*******************************Method Header*********************************
//Method Name    : AcqProbingScanStart
//Author         : PATNI/MSN
//Purpose        : Send AMCMM_MO_MONITOR message to AcqMan
//*****************************************************************************
status_t CPreScan::AcqProbingScanStart(const CPqmProtocol* obj
                                      )
{
    LPCTSTR FUNC_NAME = _T("CPreScan::AcqProbingScanStart");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(obj);

    //+Patni-MSN/2009Mar10/Modified/Optimization

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of AcqProbingScanStart"));

    m_header.function = AMCMM_MO_MONITOR;

    wchar_t l_buff[MAX_PATH] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("GP_TMP_DIR"), l_buff, MAX_PATH);

    TCHAR	dst[128] = {0};
    wsprintf(dst, _T("%s%s"), l_buff, PROBE_STUDY_NAME);

    CString l_dest_study(dst);

    wcscpy(m_data.study_file, l_dest_study);
    m_data.protocol_node = 0 ;

    return SendMessage() ;
    //-Patni-MSN/2009Mar10/Modified/Optimization
}

//*******************************Method Header*********************************
//Method Name    : AcqProbingScanAbort
//Author         : PATNI/MSN
//Purpose        : Send AMCMM_MO_MONITOR_ABORT message to AcqMan
//*****************************************************************************
status_t CPreScan::AcqProbingScanAbort(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::AcqProbingScanAbort");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of AcqProbingScanAbort"));

    //    PqmDebugAcqMonitor("AMCMM_MO_MONITOR_ABORT : abort ");

    m_header.function = AMCMM_MO_MONITOR_ABORT;

    return SendMessage() ;
}

//*******************************Method Header*********************************
//Method Name    :PrepareRequest()
//Author         :PATNI/MRP
//Purpose        :This method prepares the Prescan request that will be sent to Acqman
//*****************************************************************************
bool CPreScan::PrepareRequest(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::PrepareRequest");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of PrepareRequest"));
    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* pqmptr = scan_proc->GetAcqManager()->GetPqm();
    CPQMStudy* pqmstudyptr = pqmptr->GetStudy();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    if ((scan_proc->GetScanProc(1).IsEmpty()) &&
        (scan_proc->GetAPCRetry() != TRUE) &&
        (!curr_protocol->GetPrepScan()) &&
        (!curr_protocol->GetFluoro())   &&
        (!(curr_protocol->GetProbeScan()) == VFC_PROBE_RTMC) &&
        (curr_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP)
       ) {
        m_header.function = AMCMM_PRESCAN_CONTINUE;

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("************ PRESCAN  CONTINUE ************"));

    } else {
        m_header.function = AMCMM_PRESCAN;

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("************ PRESCAN ************"));
    }

    if (scan_proc->GetAPCRetry()) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("Trace of PrepareRequest - APC Retry"));

        m_data.param.preScan.kind[0] = ACQ_PRESCAN_RFLEVEL;
    }

    CString l_study_path = pqmptr->GetStudyUidPath();

    if (l_study_path.GetLength() <= 0) {
        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(string,
                ERROR_LOG_CRUCIAL1 , _T("PreScan.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        /*CPQMLogMgr::GetInstance()->WriteEventLog(IDS_STUDY_NOT_FOUND,
                ERROR_LOG_CRUCIAL1 , _T("PreScan.cpp"));*/
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
        return false;

    }

    _tcscpy(m_data.study_file, l_study_path);

    m_data.protocol_node = curr_protocol->GetProtocol();

    if (m_data.protocol_node < 0) {
        m_data.protocol_node = scan_proc->GetProt();
    }

    if (!SetRequest()) {
        return false;
    }

    if (!SetAction()) {
        return false;
    }

    // Moving Couch
    if (curr_protocol->GetMovingCouch() == VFC_MOVING_COUCH_MANUAL) {
        curr_protocol->SetHoldScan(
            curr_protocol->GetHoldScan() | VFC_HOLD_MOVE_BED);

    } else {
        curr_protocol->SetHoldScan(
            curr_protocol->GetHoldScan() & ~VFC_HOLD_MOVE_BED);
    }

    /* APC PAUSE */
    if (scan_proc->GetAPCHold()) {
        //Patni-Hemant/2011Jan11/Commented/REDMINE-1095_Reopen
        //scan_proc->SetAPCHold(TRUE);
        curr_protocol->SetHoldScan(curr_protocol->GetHoldScan() | VFC_HOLD_APC);

    } else {

        //+Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
        //scan_proc->SetAPCHold(curr_protocol->GetHoldScan() & VFC_HOLD_APC);
        curr_protocol->SetHoldScan(curr_protocol->GetHoldScan() & ~VFC_HOLD_APC);
        //-Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
    }

    //Patni-Hemant/2010Jun4/Commented/MaFT/TMSC-REDMINE-23/Visual Prep Case
    //Its not in base code
    //scan_proc->SetAPCHold(pre_apc_hold);

    /* AUTO_VOICE */
    curr_protocol->SetAutoHold(0);

    if (curr_protocol->GetExeAutoVoice() >> 4) {

        //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        //
        //        curr_protocol->SetAutoHold(curr_protocol->GetAutoHold() |
        //                                   (VFC_HOLD_CFA | VFC_HOLD_RGN | VFC_HOLD_APC));
        curr_protocol->SetAutoHold(curr_protocol->GetAutoHold() |
                                   (VFC_HOLD_CFA | VFC_HOLD_RGN | VFC_HOLD_APC | VFC_HOLD_CDS));
        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    }

    if ((curr_protocol->GetExeAutoVoice() & 0x0F) ||
        curr_protocol->GetPlnAutoVoice()) {

        curr_protocol->SetAutoHold(curr_protocol->GetAutoHold() | VFC_HOLD_COVERAGE);
    }

    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    memset(mask, 0, sizeof(mask));

    int l_hold_scan = curr_protocol->GetHoldScan();
    int l_auto_hold = curr_protocol->GetAutoHold();
    float rx_gain_offset = pqmptr->GetRxGainOffset();

    int l_status = E_ERROR;

    if (E_NO_ERROR != pqmstudyptr->SetProtocolValues(pqmstudyptr->GetVfDBHandle(),
            curr_protocol->GetProtocol(),
            mask,
            K_PR_HOLD,
            &l_hold_scan,
            sizeof(int),
            K_PR_AUTO_HOLD,
            &l_auto_hold,
            sizeof(int),
            K_PR_PRESCAN_RGN_OFFSET,
            &rx_gain_offset,
            sizeof(float),
            NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
    }

    if (!scan_proc->GetCFARetry()) {
        if (-1 != pqmptr->GetAcqManager()->GetWaitingForAutoScanStartAcqOrder() && !scan_proc->GetAPCRetry()) {
            pqmptr->GetAcqManager()->AutoScanTimerReset();
        }

        pqmstudyptr->SetWaitToCurrent(curr_protocol);
        pqmptr->WriteToUI(PQM_MSG_STATUS_CURRENT);

    }

    return true;
}

//*******************************Method Header*********************************
//Method Name    :SetAgentAsScan()
//Author         :PATNI/MRP
//Purpose        :This method sets the scan agent
//*****************************************************************************
//+Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan
int CPreScan::SetAgentAsScan(
    CPqmProtocol* f_curr_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SetAgentAsScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAgentAsScan"));

    if (f_curr_protocol == NULL)
        return E_ERROR;

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_proc pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();
    CPqmProtocol* curr_protocol = f_curr_protocol;
    char*    pt = NULL, buf[PQM_MAX_STRING] = {'\0'}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    //size = sizeof(buf);
    int size = PQM_MAX_STRING;
    //+Patni-RUP/2009Aug25/Modified/cpp test corrections

    memset(mask, 0, sizeof(mask));
    int result = pqm->GetStudy()->GetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                 curr_protocol->GetProtocol(),
                 mask,
                 K_PR_SCAN_AGENT,
                 buf,
                 &size,
                 NULL);

    if (result != E_NO_ERROR || BITFLD_ON(mask, K_PR_SCAN_AGENT)) {
        return E_ERROR;
    }

    for (pt = strtok(buf, ":"); pt != NULL; pt = strtok(NULL, ":")) {

        if (!strncmp(pt, "RG", 2)) {
            //+Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan
            pt += 2;
            //+Patni-MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan
            SetAgentRG(curr_protocol, pt);

        } else {
            if (!strncmp(pt, "CF", 2)) {

                SetAgentCF(pt);
            }
        }
    }

    return result;
}
//-Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan

//*******************************Method Header*********************************
//Method Name    :AddRequest()
//Author         :PATNI/MRP
//Purpose        :Set the Request
//*****************************************************************************
void CPreScan::AddRequest(
    AM_PreScanKind_t req
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::AddRequest");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), req);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_request.push_back(req);
}

//*******************************Method Header*********************************
//Method Name    :AddFlag()
//Author         :PATNI/MRP
//Purpose        :Add the Flag
//*****************************************************************************
void CPreScan::AddFlag(
    CPSAcqSetFlag* flag
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::AddFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AddFlag"));

    m_flag.push_back(flag);
}

//*******************************Method Header*********************************
//Method Name    :CFARedraw()
//Author         :PATNI/MRP
//Purpose        :Dummy Function
//*****************************************************************************
void CPreScan::CFARedraw(
    const bool redraw
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::CFARedraw");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), redraw);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
}

//*******************************Method Header*********************************
//Method Name    :CFAReady()
//Author         :PATNI/MRP
//Purpose        :Dummy Function
//*****************************************************************************
void CPreScan::CFAReady(
)const
{
    LPCTSTR FUNC_NAME = _T("CPreScan::CFAReady");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CFAReady"));
}

//*******************************Method Header*********************************
//Method Name    :GetFlags()
//Author         :PATNI/MRP
//Purpose        :Get the Flags
//*****************************************************************************
CPSAcqSetFlagVec& CPreScan::GetFlags(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::GetFlags");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetFlags"));

    return m_flag;
}

//*******************************Method Header*********************************
//Method Name    :SetAgentRG()
//Author         :PATNI/MRP
//Purpose        :This method sets the scan Agent as RG
//*****************************************************************************
//+Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan
void CPreScan::SetAgentRG(
    CPqmProtocol* f_current_protocol,
    char* data
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SetAgentRG");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAgentRG"));

    if (f_current_protocol == NULL || data == NULL) {	//Patni-MJC/2009Aug17/Modified/cpp test corrections
        return;
    }

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_proc pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();
    CPqmProtocol* curr_protocol  = f_current_protocol;
    CPSAcqSetFlag::DATA mode = CPSAcqSetFlag::PQM_VFC_PRESCAN_NONE; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    BOOL setvalue = FALSE;
    int val = 0;
    float rgn = 0.0f;

    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if (*data == 'V') {

        mode = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD;
        setvalue = TRUE;
        data++;

        if (*data == '+' || *data == '-') {

            val = (int) strtol(data, NULL, 0);

            if (pqm->GetStudy()->GetPreviousAgent(curr_protocol, 1, &rgn) != E_NO_ERROR) {

                mode = (CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL;
                setvalue = FALSE;

            } else {
                rgn += (float)val;
            }

        } else if (*data == '\0') {
            setvalue = FALSE;

        } else {
            rgn = (float)strtod(data, NULL);
        }

    } else if (*data == 'P') {

        mode = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_PREVIOUS;
        data++;

        if (*data != '\0') {
            val = (int)strtol(data, NULL, 0);

            if (pqm->GetStudy()->GetPreviousAgent(curr_protocol, val, &rgn) == E_NO_ERROR) {

                mode = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD;
                setvalue = TRUE;
            }
        }
    }

    CPSAcqSetFlagVec current_action = *(scan_proc->GetCurAction());

    for (int i = 0; i < NUM_ACQ_FLAG; i++) {

        CPSAcqSetFlag::DATA data = current_action[i]->GetData();
        CPSAcqSetFlag::TAG tag = current_action[i]->GetTag();

        if (tag == K_PR_PRESCAN_RGN && data != mode) {

            memset(mask, 0, sizeof(mask));
            pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                                               curr_protocol->GetProtocol(),
                                               mask,
                                               tag,
                                               &mode,
                                               sizeof(int),
                                               NULL);

            current_action[i]->SetData(mode);
            break;
        }
    }

    if (setvalue) {
        memset(mask, 0, sizeof(mask));
        pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                                           curr_protocol->GetProtocol(),
                                           mask,
                                           K_PR_PRESCAN_RGN_VALUE,
                                           &rgn,
                                           sizeof(float),
                                           NULL);
    }
}
//-Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan

//*******************************Method Header*********************************
//Method Name    :SetAgentCF()
//Author         :PATNI/MRP
//Purpose        :This method sets the scan Agent as CF
//*****************************************************************************
void CPreScan::SetAgentCF(
    char* data
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SetAgentCF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAgentCF"));

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("scan_proc pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();

    CPSAcqSetFlag::DATA mode = CPSAcqSetFlag::PQM_VFC_PRESCAN_NONE; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    mode = (CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL;
    CPSAcqSetFlagVec current_action = *(scan_proc->GetCurAction());

    for (int i = 0; i < NUM_ACQ_FLAG; i++) {

        CPSAcqSetFlag::DATA data = current_action[i]->GetData();
        CPSAcqSetFlag::TAG tag = current_action[i]->GetTag();

        if (tag == K_PR_PRESCAN_FSY && data != mode) {

            memset(mask, 0, sizeof(mask));

            pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                                               m_data.protocol_node,
                                               mask,
                                               tag,
                                               &mode,
                                               sizeof(int),
                                               NULL);
            current_action[i]->SetData(mode);
            break;
        }
    }
}

//*******************************Method Header*********************************
//Method Name    :SetRequest()
//Author         :PATNI/MRP
//Purpose        :Set the prescan request
//*****************************************************************************
bool CPreScan::SetRequest(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SetRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetRequest"));

    CScanProcedure*  scan_proc  = GetScanProcedure();
    CPqmPmAcqman*    acqman     = scan_proc->GetAcqManager();
    CPqm*            pqm        = acqman->GetPqm();
    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilDB*         coil_db    = pqm->GetCoilDB();

    CPqmProtocol*    curr_protocol = scan_proc->GetScanProcProtocol();
    CPqmPrMode*      curr_protocol_mode = curr_protocol->GetPqmPrMode();

    int     reqcnt    = 0;
    int     coil_type = 0;

    // + V9,25 coil support@AMT
    CPqmCoilData*    l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    CCoil           l_tx_coil;

    if (pqm->IsSaturnCoilSystem()) {	//Saturn
        PqmCoil_t* l_tmp_tx_coil = NULL;
        coil_type  = pqm->GetStudy()->GetTxCoilInSaturn(
                         curr_protocol->GetCoilDBTable(),
                         &l_tmp_tx_coil) ;
        l_tx_coil.SetCoil(l_tmp_tx_coil);

    } else {
        int receive_coil_id = curr_protocol_mode->GetRecvCoilId();
        l_coil_data->PqmCoilGetTxCoil(receive_coil_id, l_tx_coil);

        if (receive_coil_id == l_tx_coil.GetCoilId()) {
            coil_type = CDB_TRANSMIT_N_RCV;
        }
    }

    // - V9,25 coil support@AMT

    if (l_tx_coil.GetCoil() != NULL &&
        GP_MAGNET(pqm->GetSysType()) != GP_MAGNET_50 &&
        coil_type == CDB_TRANSMIT_N_RCV) {

        // If system is not 50GP and RXCoil == TXCoil, don't tune RXCoil.
        for (int i = 0 ; i < GetRequestCount() ; i++) {

            AM_PreScanKind_t request = GetRequest(i);

            if (request != ACQ_PRESCAN_RXCOIL) {
                m_data.param.preScan.kind[reqcnt++] = request;
            }

        }

    } else {

        for (int i = 0 ; i < GetRequestCount() ; i++) {
            m_data.param.preScan.kind[reqcnt++] = GetRequest(i);
        }
    }

    m_data.param.preScan.reqCnt = reqcnt;
    //  DB_PRINTF("A",("===> acqRequest.data.param.preScan.reqCnt=%d\n",
    //         acqRequest_p->data.param.preScan.reqCnt));
    return true;
}

//*******************************Method Header*********************************
//Method Name    :SetAction()
//Author         :PATNI/MRP
//Purpose        :Set the action for Prescan
//*****************************************************************************
bool CPreScan::SetAction(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SetAction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAction"));

    int status = 0;
    CScanProcedure* scan_proc = GetScanProcedure();
    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();
    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilDB* coil_db = pqm->GetCoilDB();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    BITFLD_DECL(mask, K_ALL_PR) = {0};  //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    CPSAcqSetFlagVec* new_action = new CPSAcqSetFlagVec;
    CPSAcqSetFlagVec acq_flag = GetFlags();

    int prescan_value = scan_proc->GetPrescan();

    ////////////////////////////
    CPSAcqSetFlagVec prev_action = *(scan_proc->GetPrevAction());
    CPSAcqSetFlagVec curr_action = *(scan_proc->GetCurAction());

    //CPSAcqSetFlag *vect_inst = NULL;
    //CPSAcqSetFlagVec::iterator v1_Iter;
    for (int noflags = 0; noflags < NUM_ACQ_FLAG ; noflags++) {
        CPSAcqSetFlag* curr_flg = new CPSAcqSetFlag();
        curr_flg->SetData(CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_NONE);
        curr_flg->SetTag(CPSAcqSetFlag::TAG::K_PR_PRESCAN_NONE);
        curr_action.push_back(curr_flg);
    }

    scan_proc->SetCFAClinical(false);
    CPSAcqSetFlag::TAG tag = CPSAcqSetFlag::K_PR_PRESCAN_NONE; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    int action_int = 0;
    int j = 0 ;

    for (int i = 0 ; i < GetFlagCount() ; i++) {

        CPSAcqSetFlag::DATA action = CPSAcqSetFlag::PQM_VFC_PRESCAN_NONE;
        CPSAcqSetFlag::DATA data = CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_NONE;
        tag = acq_flag[i]->GetTag();
        //		if(i < acq_flag.size())
        //		{
        //			vect_inst = acq_flag.at(i);
        //			tag = vect_inst->GetTag();
        //		}
        //
        action_int = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

        memset(mask, 0, sizeof(mask));

        status = pqm->GetStudy()->GetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                 m_data.protocol_node,
                 mask,
                 acq_flag[i]->GetTag(),
                 &action_int,
                 NULL,
                 NULL);

        action = (CPSAcqSetFlag::DATA)action_int;

        if (BITFLD_ON(mask, acq_flag[i]->GetTag())) {
            action = CPSAcqSetFlag::PQM_VFC_PRESCAN_NONE;;
        }

        curr_action[i]->SetTag(acq_flag[i]->GetTag());

        if ((curr_protocol->GetFluoro() ||
             prescan_value == K_PRESCAN_MODE_AMB) &&
            (tag == K_PR_PRESCAN_RGN) &&
            (action == ((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD))) {

            curr_action[i]->SetData((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD);
            data  = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD;

        } else if ((tag == K_PR_PRESCAN_RGN) &&
                   ((prescan_value != K_PRESCAN_MODE_EVERY) ||
                    (pqm->GetStudy()->Check1stScan() == true &&
                     pqm->IsSkipRgn() == true))) {

            pqm->GetStudy()->SetDefaultRGNValue(m_data.protocol_node);
            curr_action[i]->SetData((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD);
            data = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD;

        } else if (ezscan_retry &&
                   tag  == K_PR_PRESCAN_RFL &&
                   action == ((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD)) {

            curr_action[i]->SetData((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD);;
            data = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD;

        } else {
            curr_action[i]->SetData(acq_flag[i]->GetData());
            data = acq_flag[i]->GetData();

            if (acq_flag[i]->GetData() == ((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_PREVIOUS)) {

                for (j = 0 ; j < (int)prev_action.size() ; j++) {

                    if (prev_action[j]->GetTag() == acq_flag[i]->GetTag() &&
                        (prev_action[j]->GetData() ==
                         ((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD))) {

                        //  DB_PRINTF("A",("prev_p is USE_VARFIELD\n"));
                        data = (CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL;
                        break;

                    }
                }
            }
        }

        //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        if ((curr_action[i]->GetTag()  == K_PR_PRESCAN_RFL)			&&
            (curr_protocol->GetRflProt() != -1)			&&
            (curr_action[i]->GetData() == VFC_PRESCAN_CLINICAL)) {
            curr_action[i]->SetData((CPSAcqSetFlag::DATA)VFC_PRESCAN_LOAD_VARFIELD);
            data = (CPSAcqSetFlag::DATA)VFC_PRESCAN_LOAD_VARFIELD;
        }

        if ((curr_action[i]->GetTag()  == K_PR_PRESCAN_FSY)			&&
            (curr_protocol->GetCfaProt() != -1)			&&
            (curr_action[i]->GetData() == VFC_PRESCAN_CLINICAL)) {
            //char	*path;
            //char	buf[128];	/* 1024 */

            curr_action[i]->SetData((CPSAcqSetFlag::DATA)VFC_PRESCAN_LOAD_VARFIELD);
            data = (CPSAcqSetFlag::DATA)VFC_PRESCAN_LOAD_VARFIELD;

            /*if((path = getenv("GP_DATA")) == NULL)
            	path = ".";
            sprintf(buf,"%s/preScan/%s",path,PRESCAN_CF_CURVE_DATA);
            unlink(buf);*/
        }

        // Check Titan system type
        if (!pqm->GetAMB()->PqmAMBScanSeg2Active() && pqm->IsCFShiftSupportedSystem()) {

            //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
            CCFShiftCheck* l_cfshiftcheck = pqm->GetCFShiftCheck();

            if (NULL == l_cfshiftcheck) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_cfshiftcheck is NULL"));
                return CFA_SHIFT_ERROR;
            }

            if ((tag == K_PR_PRESCAN_FSY) &&
                (data != ((CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL)) &&
                (!l_cfshiftcheck->GetIsCFARetryExecuted())) {

                switch (l_cfshiftcheck->CheckCFShiftValue(data, curr_protocol, scan_proc)) {

                    case CFA_REQUIRED :
                        data = ((CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL);
                        break ;

                    case CFA_SHIFT_ERROR:
                        return FALSE;

                    default :
                        break ;
                }
            }

            if (tag == K_PR_PRESCAN_FSY) {
                l_cfshiftcheck->SetIsCFARetryExecuted(TRUE);
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("System type is not Titan"));
        }

        if ((tag == K_PR_PRESCAN_FSY) &&
            (data == ((CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL))) {
            scan_proc->SetCFAClinical(true);
        }

        //Redmine-2187
        if (pqm->GetMulitScanDynamic_License()) {

            if ((K_PR_PRESCAN_RGN == tag) &&
                data == ((CPSAcqSetFlag::DATA)VFC_PRESCAN_CLINICAL) &&
                PQM_SLAVE_PROTOCOL == pqm->IsMasterSlaveProtocol(curr_protocol)) {

                CPqmProtocol* l_master_protocol = curr_protocol->GetMasterProtocol();

                if (NULL != l_master_protocol) {

                    if (E_NO_ERROR == pqm->GetStudy()->SetMasterRGNValueToSlave(m_data.protocol_node, l_master_protocol->GetProtocol())) {
                        curr_action[i]->SetData((CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD);
                        data = (CPSAcqSetFlag::DATA)VFC_PRESCAN_USE_VARFIELD;
                    }
                }
            }
        }

        if (action != data) {

            BITFLD_DECL(sub_mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
            BITFLD_INIT(sub_mask, K_ALL_PR, 0);
            status = pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                     m_data.protocol_node,
                     sub_mask,
                     tag,
                     &data,
                     sizeof(int),
                     NULL);

            if_error(status) {
                //DB_PRINTF("",("PqmPrVaSetProtValues ERROR.\n"));
                //              if (m_scan_procedure->GetIsScanning()) {
                //                  //mAcqDeleteLockFile();
                //              }
                return false;
            }
        }

        new_action->push_back(new CPSAcqSetFlag(tag, data));
    }

    scan_proc->SetCurAction(new_action);
    scan_proc->SetPrevAction();
    return true;
}

//****************************Method Header****************************
// Method Name  :ExecuteMrsPrescan
// Author       :PATNI/
// Purpose      :
//*********************************************************************
BOOL CPreScan::ExecuteMrsPrescan(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::ExecuteMrsPrescan");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_scan_procedure == NULL) {
        return FALSE;
    }    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmMRSScan* l_mrs_scan  = new CPqmMRSScan(m_scan_procedure);
    l_mrs_scan->SetPrescan(this);
    //m_scan_procedure->AddTransaction(l_mrs_scan);

    //l_mrs_scan->InitMRS();
    if (l_mrs_scan->InitMRS() == FALSE) {

        //+Patni-Manishkumar/2009Oct07/Added/MVC004983-CoilSel. dlg not displayed
        CPqmPmAcqman* l_acqman_ptr = m_scan_procedure->GetAcqManager();

        if (NULL == l_acqman_ptr) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmPmAcqman object is NULL"));
            return FALSE;
        }

        CPqm* l_pqmptr = l_acqman_ptr->GetPqm();

        if (NULL == l_pqmptr) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqm object is NULL"));
            return FALSE;
        }

        CPQMStudy* l_pPQMStudy = l_pqmptr->GetStudy();

        if (NULL == l_pPQMStudy) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy object is NULL"));
            return FALSE;
        }

        l_pPQMStudy->SetAcqStatus(m_scan_procedure->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        l_pPQMStudy->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        //-Patni-Manishkumar/2009Oct07/Added/MVC004983-CoilSel. dlg not displayed

        l_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        return FALSE;
    }

    return TRUE;
}

//+Patni-AKR/2010Aug13/Added/NFD001
//**************************************************************************
//Method Name   : UpdateWaterFatSection
//Author        : PATNI/AKR/NFD001
//Purpose       : Update Water Fat section in VF DB
//**************************************************************************
void CPreScan::UpdateWaterFatSection(CPqmProtocol* f_curr_protocol, CPqm* f_pqmptr)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::UpdateWaterFatSection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of UpdateWaterFatSection"));

    int l_shimState = 0;
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shimState);

    CPqmProtocol* l_CFAUpdateCFA_Protocol = (NULL != f_curr_protocol) ? f_curr_protocol : f_pqmptr->GetStudy()->GetLastDone();

    if (NULL !=  l_CFAUpdateCFA_Protocol) {
        f_pqmptr->GetAutoInsertProc()->UpdateWaterFatSection(l_shimState, l_CFAUpdateCFA_Protocol->GetProtocol());
    }
}

//*******************************Method Header*********************************
//Method Name    :ErrPrescan()
//Author         :PATNI/HAR
//Purpose        :This method does the processing after an error message
//                is received from Acqman.
//*****************************************************************************
bool CPreScan::ErrPrescan(
    const int type,
    const int error_id,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::ErrPrescan");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(error_id);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ErrPrescan"));

    int ERR_ID_SCAN = -1;

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman_ptr = scan_proc->GetAcqManager();
    CPqm* pqmptr = acqman_ptr->GetPqm();

    if (m_scan_procedure->CanAbortProbeOnPrescanError()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanAbortProbeOnPrescanError TRUE"));
        pqmptr->GetPqmProCon()->StopProlim();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanAbortProbeOnPrescanError FALSE"));
    }

    //Check here for the error id and code etc as per base code
    //Get the error strings and display
    //MRP to diplay all the error code here...
    CString err_msg = _T("");				   // Added By KT/ACE-2/11-06-2009/Multilingualisation

    int l_error_level = SCAN_WARN_CRUCIAL ;		//+Patni-Ravindra Acharya/2010April12/Added/PH#3#/IR-128

    //+Patni-RUP/2010May07/Modified/To fix TMSC-REDMINE-138
    switch (body->status) {
        case AMCM_NACK: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_NAK ;
                err_msg = _T("IDS_ERR_ACC_NAK");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_NAK;
                err_msg = _T("IDS_ERR_NAK");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_COUCH: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_COUCH ;
                err_msg = _T("IDS_ERR_ACC_COUCH");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_COUCH;
                err_msg = _T("IDS_ERR_COUCH");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_TXCOIL: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_TXCOIL ;
                err_msg = _T("IDS_ERR_ACC_TXCOIL");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_TXCOIL;
                err_msg = _T("IDS_ERR_TXCOIL");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_RXCOIL: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_RXCOIL ;
                err_msg = _T("IDS_ERR_ACC_RXCOIL");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_RXCOIL;
                err_msg = _T("IDS_ERR_RXCOIL");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_GATEMETHOD: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_GATEMETHOD ;
                err_msg = _T("IDS_ERR_ACC_GATEMETHOD");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_GATEMETHOD;
                err_msg = _T("IDS_ERR_GATEMETHOD");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_SEQGEN: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_SEQGEN ;
                err_msg = _T("IDS_ERR_ACC_SEQGEN");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_SEQGEN;
                err_msg = _T("IDS_ERR_SEQGEN");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_SAR: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_SAR ;
                err_msg = _T("IDS_ERR_ACC_SAR");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                //+PATNI-MP/Modified/MVC4957
                ERR_ID_SCAN = IDS_ERR_SAR;
                err_msg = _T("IDS_ERR_SAR");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
                //-PATNI-MP/Modified/MVC4957
            }
        }
        break;

        case AMCM_PCOIL: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_SAR_COIL ;
                err_msg = _T("IDS_ERR_ACC_SAR_COIL");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_SAR_COIL;
                err_msg = _T("IDS_ERR_SAR_COIL");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;

        case AMCM_GCOILTEMP: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_ACC_GCOILTEMP  ;
                err_msg = _T("IDS_ERR_ACC_GCOILTEMP");

            } else {
                ERR_ID_SCAN = IDS_ERR_GCOILTEMP;
                err_msg = _T("IDS_ERR_GCOILTEMP");
            }

            //+ MEITEC/2010Apr21/Modified/PH#3#/IR-128
            l_error_level = SCAN_WARN_WARNING ;
            //- MEITEC/2010Apr21/Modified/PH#3#/IR-128

        }
        break;

        default: {
            if (type == AMCM_FUNC_KIND_REQUEST) {
                ERR_ID_SCAN = IDS_ERR_FRM_ACC_PRESCAN ;
                err_msg = _T("IDS_ERR_FRM_ACC_PRESCAN");	// Added By KT/ACE-2/11-06-2009/Multilingualisation

            } else {
                ERR_ID_SCAN = IDS_ERR_PRESCAN;
                err_msg = _T("IDS_ERR_PRESCAN");	// Added By KT/ACE-2/11-06-2009/Multilingualisation
            }
        }
        break;
    }

    //-Patni-RUP/2010May07/Modified/To fix TMSC-REDMINE-138


    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();
    CPqmPrMode* last_scan = acqman_ptr->GetProtocolModeLastScan();

    if (curr_protocol != NULL) {
        //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        //This code is not present in Vantage.. But its necessary.
        if (curr_protocol->GetASCOMPASSFlag()) {
            //reset the steps..
            curr_protocol->GetCDSInfo()->m_cds_step	  = CDS_STEP0_NONE;
        }

        scan_proc->ResetCurrentRequest();
        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);

        pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT); // L"ABORT_WAIT"
        pqmptr->GetAMB()->PqmPmAbortAmb();
        pqmptr->GetStudy()->SetScanModeOfHead();

        if (body->status == AMCM_SAR) {


            last_scan->SetRecvCoilId(curr_protocol->GetPqmPrMode()->GetRecvCoilId());
            last_scan->SetAnatomy(curr_protocol->GetPqmPrMode()->GetAnatomy());
            CString tmpStr(_T(""));
            tmpStr.Format(_T("last_scan->SetAnatomy(curr_protocol->GetPqmPrMode()->GetAnatomy()) = %d"), curr_protocol->GetPqmPrMode()->GetAnatomy());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
            last_scan->SetCouchPos(body->couchInfo.position);
            last_scan->SetScanNumber(body->couchInfo.couchStatusFlag);
            last_scan->SetContrast(curr_protocol->GetPqmPrMode()->GetContrast());

            if (pqmptr->IsSaturnCoilSystem()) {
                // + Himanshu SM 2
                memcpy(acqman_ptr->GetLastSaturnCoil(),
                       curr_protocol->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));
                memcpy(acqman_ptr->GetLastSaturnCoilTbl(),
                       //+ Patni-MP/2009Aug13/Modified/MVC4208
                       curr_protocol->GetCoilDBTable(), sizeof(PqmSaturnRcvCoil_t));
                // - Himanshu SM 2
            }
        }
    }

    //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
    if (ERR_ID_SCAN != -1) {

        //+Patni-PJS/2010Dec16/Added/REDMINE-1098
        if (scan_proc->GetCFARetry()) {
            pqmptr->WriteToUI(PQM_MSG_ACQ_MGR_BSY_ERR);
        }

        //-Patni-PJS/2010Dec16/Added/REDMINE-1098

        //Patni-MP/2010May21/Added/TMSC-REDMINE-138 - Multilingualization
        CString l_error_String =  CPqmUtilities::GetMultiLingualString(err_msg);

        if ((ERR_ID_SCAN == IDS_ERR_ACC_NAK) && (l_error_String.GetLength() == 0)) {
            l_error_String = CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_MGR_ERROR"));
        }

        if (ERR_ID_SCAN == IDS_ERR_ACC_SAR || ERR_ID_SCAN == IDS_ERR_SAR) {
            l_error_level = SCAN_WARN_INFORMATION;
        }

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //+Patni-PJS/2009Dec3/Modified & Modified/MVC004957
        //+PATNI-MP/Modified/MVC4957
        //+Patni-RUP/2010May07/Modified/To fix TMSC-REDMINE-138
        if (ERR_ID_SCAN == IDS_ERR_ACC_SAR || ERR_ID_SCAN == IDS_ERR_SAR
            || ERR_ID_SCAN == IDS_ERR_ACC_GCOILTEMP || ERR_ID_SCAN == IDS_ERR_GCOILTEMP) {
            //-Patni-RUP/2010May07/Modified/To fix TMSC-REDMINE-138

            MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error;
            CString l_warnapp_name = _T("");
            l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

            l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                             l_error_String, l_warnapp_name, l_error_level ,				//+Patni-Ravindra Acharya/2010April12/Modified/PH#3#/IR-128
                             SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

        } else {
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_String,
                    MPlus::EH::Information, _T("prescan.cpp"), _T("PQM"));
        }

        pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);

        //-Patni-PJS/2009Dec3/Modified & Modified/MVC004957
    }

    if (m_scan_procedure->CanAbortProbeOnPrescanError()) {
        pqmptr->GetStudy()->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanAbortProbeOnPrescanError FALSE"));

        if (m_scan_procedure->GetCFARetry()) {
            m_scan_procedure->SetCFARetry(PQM_CFA_NONE);
            pqmptr->SetPostProcessFlag(FALSE);
            pqmptr->WriteToUI(PQM_MSG_CFA_PRESCAN_END);

        }

        if (m_scan_procedure->GetAPCRetry()) {
            m_scan_procedure->SetAPCRetry(FALSE);
        }
    }

    //Patni-PJS/2010Feb24/Commented/DeFT# PROPOSAL_08-Remove EZScan Else part removed
    return true;
}

//*******************************Method Header*********************************
//Method Name    :PreScanDone()
//Author         :PATNI/HAR
//Purpose        :This function is called after Prescan callback is done successfully
//*****************************************************************************
void CPreScan::PreScanDone(
    const int status,
    float couch_pos,
    const int couch_status
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::PreScanDone");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d, %f, %d"), status, couch_pos, couch_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan procedure pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* acqman_manager = scan_proc->GetAcqManager();	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* pqmptr = acqman_manager->GetPqm();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();
    CPqmPrMode* curr_protocol_mode = curr_protocol->GetPqmPrMode();
    CPqmPrMode* last_scan = acqman_manager->GetProtocolModeLastScan();



    //+Patni-Sarjeet/2010Apr12/Modified/Phase3#MCM0229-00187
    CPSAcqSetFlagVec l_set_flag = *(scan_proc->GetCurAction());

    pqmptr->GetStudy()->SetFirstScan(FALSE);

    last_scan->SetRecvCoilId(curr_protocol_mode->GetRecvCoilId());
    last_scan->SetAnatomy(curr_protocol_mode->GetAnatomy());
    CString tmpStr = _T("");
    tmpStr.Format(_T("last_scan->SetAnatomy(curr_protocol_mode->GetAnatomy()) = %d"), curr_protocol_mode->GetAnatomy());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
    last_scan->SetHomePos(curr_protocol_mode->GetHomePos());
    last_scan->SetCouchPos(couch_pos);
    last_scan->SetScanNumber(couch_status);
    last_scan->SetContrast(curr_protocol_mode->GetContrast());

    // + V9.25 coil support@AMT
    if (pqmptr->IsSaturnCoilSystem()) {	// Saturn coil support
        memcpy(acqman_manager->GetLastSaturnCoil(),
               curr_protocol->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));
        memcpy(acqman_manager->GetLastSaturnCoilTbl(),
               //+ Patni-MP/2009Aug13/Modified/MVC4208
               curr_protocol->GetCoilDBTable(), sizeof(PqmSaturnRcvCoil_t));
    }

    // - V9.25 coil support@AMT

    // + SHIMMING Lokesh 15 March 2008
    for (int i = 0; i < m_data.param.preScan.reqCnt; i++) {
        if (m_data.param.preScan.kind[i] == ACQ_PRESCAN_TXCOIL) {
            //Patni-SS/2010APR27/Commented/MCM0229-00187_CDR_05 TMSC REview Comments
            //float l_cfapos = PQM_COUCH_INIT_POSITION ; //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
            CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(&couch_pos, NULL, NULL);
            pqmptr->GetStudy()->WriteToShimData();
            break;
        }
    }

    BITFLD_DECL(mask, K_ALL_PR);

    if (l_set_flag.empty()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPSAcqSetFlagVec vector is Empty"));
        return;
    }

    for (i = 0; i < NUM_ACQ_FLAG; i++) {

        //+Patni-Sarjeet/2010Apr12/Modified/Phase3#MCM0229-00187
        //l_acqflag = GetFlag(i);
        //l_acqflag = set_flag[i];

        CPSAcqSetFlag::TAG  l_tagflag = l_set_flag[i]->GetTag();
        CPSAcqSetFlag::DATA  l_dataflag = l_set_flag[i]->GetData();

        //Modified by Meghana On 9/16/2008 6:03:33 PM
        //-Patni-Sarjeet/2010Apr12/Modified/Phase3#MCM0229-00187
        if (l_tagflag == CPSAcqSetFlag::TAG::K_PR_PRESCAN_RFL) {
            if (l_dataflag == CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_CLINICAL) {
                //+Patni-DKH/2010Feb18/Modified/Phase3#MCM0229-00187
                CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(NULL, &couch_pos, NULL);
                curr_protocol->SetRflPos(couch_pos);
                //Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC REview comment
                float l_rflpos = curr_protocol->GetRflPos();
                pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                                                      curr_protocol->GetProtocol(),
                                                      mask,
                                                      K_PR_RFL_COUCH,
                                                      &l_rflpos,
                                                      sizeof(l_rflpos), ////Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC REview comment
                                                      NULL);
                pqmptr->GetStudy()->WriteToShimData();

            } else if (l_dataflag == CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_LOAD_VARFIELD ||
                       (l_dataflag == CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_USE_VARFIELD &&
                        pqmptr->GetAMB()->PqmAMBScanSeg2Active())) {
                CPqmProtocol* l_reuse_pqmprotocol = pqmptr ->GetStudy()->GetItemFromProtocol(curr_protocol->GetRflProt());

                if (l_reuse_pqmprotocol) {
                    curr_protocol->SetRflPos(l_reuse_pqmprotocol->GetRflPos());
                    //Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC REview comment
                    float l_rflpos = curr_protocol->GetRflPos();
                    CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(NULL, &l_rflpos, NULL);
                    pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                                                          curr_protocol->GetProtocol(),
                                                          mask,
                                                          K_PR_RFL_COUCH,
                                                          &l_rflpos,
                                                          sizeof(l_rflpos), //Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC Review comment
                                                          NULL);
                }
            }
        }

        if (l_tagflag == CPSAcqSetFlag::TAG::K_PR_PRESCAN_FSY) {
            if (l_dataflag == CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_CLINICAL) {
                CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(NULL, NULL, &couch_pos);
                curr_protocol->SetCfaPos(couch_pos);
                //Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC Review comment
                float l_cfapos = curr_protocol->GetCfaPos();
                pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                                                      curr_protocol->GetProtocol(),
                                                      mask,
                                                      K_PR_CFA_COUCH,
                                                      &l_cfapos,
                                                      sizeof(l_cfapos),  //Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC Review comment
                                                      NULL);

            } else if (l_dataflag == CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_LOAD_VARFIELD ||
                       (l_dataflag == CPSAcqSetFlag::DATA::PQM_VFC_PRESCAN_USE_VARFIELD
                        && pqmptr->GetAMB()->PqmAMBScanSeg2Active())) {
                CPqmProtocol* l_reuse_pqmprotocol = pqmptr ->GetStudy()->GetItemFromProtocol(curr_protocol->GetCfaProt());

                if (l_reuse_pqmprotocol) {
                    curr_protocol->SetCfaPos(l_reuse_pqmprotocol->GetCfaPos());
                    //Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC Review comment
                    float l_cfapos = curr_protocol->GetCfaPos();
                    CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(NULL, NULL, &l_cfapos);
                    pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                                                          curr_protocol->GetProtocol(),
                                                          mask,
                                                          K_PR_CFA_COUCH,
                                                          &l_cfapos,
                                                          sizeof(l_cfapos),//Patni-SS/2010APR27/Modified/MCM0229-00187_CDR_06 TMSC Review comment
                                                          NULL);
                }
            }
        }

        //-Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187

    }

    // - SHIMMING Lokesh 15 March 2008

    CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("Couch Position = %f\n"), couch_pos);
    //Patni-SS/2010Apr12/Modified/Phase3#MCM0229-00187
    CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("Couch Status = %d\n"), couch_status); //Code Review
    //PqmDebugAcqMonitor("Couch Position = %f\n",couchPos);
    //PqmDebugAcqMonitor("Scan Number    = %d\n",couchStatus);
}

//*******************************Method Header*********************************
//Method Name    :PreScanCallback()
//Author         :PATNI/HAR
//Purpose        :This function is called after the response is got from Acqman
//                after PQM makes a request for Prescan
//*****************************************************************************
int CPreScan::PreScanCallback(
    const int type,
    AM_AcqManToClient_t* data
)
{

    LPCTSTR FUNC_NAME = _T("CPreScan::PreScanCallback");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of PreScanCallback"));

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("scan procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* acqman_manager = scan_proc->GetAcqManager();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (acqman_manager == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("Acqman manager procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* pqmptr = acqman_manager->GetPqm();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();
    CVarFieldHandle* l_db_handle = pqmptr->GetStudy()->GetVfDBHandle();

    if (NULL == l_db_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_db_handle is NULL"));
        return E_ERROR;
    }

    if (type == AMCM_FUNC_KIND_REQUEST) {
        scan_proc->SetScanAbort(PQM_SCAN_ABORT);
        return E_NO_ERROR;
    }

    //+Patni-Ravindra Acharya/2010June09/Added/IR-153
    bool l_OlpTemp_Flag = FALSE ;
    CScanProcedure* scanproc = GetScanProcedure();
    CPqmPmAcqman* acqman = scanproc->GetAcqManager();
    CPqm*  l_pqm_ptr = acqman->GetPqm();
    CPQMStudy* l_study = l_pqm_ptr->GetStudy();
    //-Patni-Ravindra Acharya/2010June09/Added/IR-153

    if (scan_proc->CanProceedCFARetry()) {

        pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);

        //+Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
        if (scan_proc->GetCFARetry() == PQM_CFA_RETRY_AND_SCAN) {
            //CString FUNC_NAME = _T("CPreScan::PreScanCallback");
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PQM_CFA_RETRY_AND_SCAN"));
            CPqmProtocol* l_pqmprotocol = pqmptr->GetStudy()->GetLastDone();

            if (l_pqmprotocol) {
                PqmAcqShimmingDone(pqmptr->GetStudy()->GetVfDBHandle(), l_pqmprotocol, data->couchInfo.position);
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PqmAcqShimmingDone had been executed"));

                //+Patni-Ravindra Acharya/2010June09/Added/IR-153
                //Patni-DKH/2010Aug03/Modified/IR-153-Review Comment
                if (l_pqm_ptr->IsCFShiftSupportedSystem()) { //Patni-DKH/2010Jul23/Added/IR-153

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IsCFShiftSupportedSystem execution Completed"));

                    l_OlpTemp_Flag = TRUE ;

                    COlpTemp* l_olptemp = m_scan_procedure->CreateOLPTransaction() ;

                    if (!l_olptemp || !l_olptemp->PqmAcqGetOLPTemp(l_pqmprotocol, PQM_TEMP_SHIM)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("PqmAcqGetOLPTemp had been executed"));

                        if (l_study != NULL)
                            l_study->SetScanModeOfHead();

                        else
                            return E_ERROR ;

                        if (scanproc != NULL)
                            scanproc->ResetCurrentRequest();

                        else
                            return E_ERROR ;
                    }

                    //+Patni-Ravindra Acharya/2010July08/Added/IR-153

                    //						if (type == AMCM_FUNC_KIND_REQUEST) {
                    //					       scan_proc->SetScanAbort(PQM_SCAN_ABORT);
                    //					       return E_NO_ERROR;
                    //							 }
                    //-Patni-Ravindra Acharya/2010July08/Added/IR-153


                }

                //-Patni-Ravindra Acharya/2010June09/Added/IR-153
            }
        }

        //-Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
        //+Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan

        //+Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
        CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();
        l_pqmshimshareddat->ShimRegistTuneInfo(NULL, NULL, &data->couchInfo.position);
        //-Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187

        //+Patni-AKR/2010Aug13/Added/NFD001
        UpdateWaterFatSection(curr_protocol, pqmptr);
        //-Patni-AKR/2010Aug13/Added/NFD001

        //+Patni-Hemant+Meghana/2011Apr05/Modified/AutoShimming_AutoScan_Issue
        //Moved from where this function is called and then modified
        //+Patni-MRP/2010Oct07/Added/NFD-Merging-Fix

        CPqmProtocol* l_current_protocol = acqman_manager->GetProtocol();

        pqmptr->SetPostProcessFlag(FALSE);

        if (!pqmptr->CanDisplayCFAGraph(l_current_protocol) && !pqmptr->IsCFShiftSupportedSystem()) {

            scan_proc->SetCFARetry(false);
            pqmptr->WriteToUI(PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END);

            CPqmProtocol* l_current_protocol = acqman->GetProtocol();
            scan_proc->CheckNextProtocol((NULL != l_current_protocol) ? l_current_protocol->GetProtocol() : 0 , TRUE);

        } else {
            if (!l_pqm_ptr->IsCFShiftSupportedSystem() || (PQM_CFA_RETRY == m_scan_procedure->GetCFARetry())) {
                scan_proc->SetCFARetry(false);
                pqmptr->WriteToUI(PQM_MSG_CFA_PRESCAN_END);
            }
        }

        //-Patni-Hemant+Meghana/2011Apr05/Modified/AutoShimming_AutoScan_Issue

        return E_NO_ERROR;
    }


    if (scan_proc->GetProbing() && scan_proc->IsSingleProcRunning()) {

        AcqProbingScanStart(curr_protocol) ;
        return E_NO_ERROR;
    }

    if (NULL == curr_protocol) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("curr_protocol is NULL"));
        return E_ERROR;
    }


    //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

    /* CDS at the end of coil set automatic scans to pre-V9.50 */
    if (scan_proc->GetPrescan() == K_PRESCAN_MODE_CDS) {
        CpqmCDS* l_pqm_cds = pqmptr->GetCDS();
        PqmAcqStatic_s* l_pqmacqstatic = l_pqm_cds->GetPqmAcqStatic();

        curr_protocol->GetCDSInfo()->m_cds_step = CDS_STEP5_SCANDONE;	/* CDS scanned */

        /* Save as information on CDS*/
        l_pqmacqstatic->m_lastCdscouchPos	 = data->couchInfo.position;
        l_pqmacqstatic->m_lastCdsscanNumber	 = data->couchInfo.couchStatusFlag;
        memcpy(&l_pqmacqstatic->m_lastCdsCoil, curr_protocol->GetSaturnCoil() , sizeof(PqmSaturnCoil_t));
        memcpy(&l_pqmacqstatic->m_lastCdstbl , curr_protocol->GetCoilDBTable() , sizeof(PqmSaturnRcvCoil_t));

        /* Location is measured in obtained acqMan */
        CCDSUtilities::GetCdsLocation(l_db_handle, curr_protocol->GetProtocol(), curr_protocol->GetCDSInfo()->m_locationtbl);

        /* CDS debug **********************************************/
        if (l_pqm_cds->PqmUiCdsDebug()) {
            l_pqm_cds->PqmUiCdsDebugWindowCB(curr_protocol);

            return E_NO_ERROR;
        }/*********************************************************/


        if (scan_proc->PqmAcqAutoCoilSet(l_db_handle, curr_protocol) == E_NO_ERROR) {
            return E_NO_ERROR;

        } else {
            return E_ERROR;
        }

        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement


    } else if (scan_proc->GetPrescan() != K_PRESCAN_MODE_AMB) {

        if (scan_proc->GetPrescan() != K_PRESCAN_MODE_EVERY) {

            CPqmProtocol* wait_protocol = NULL;
            pqmptr->GetStudy()->GetTopWaitPos(&wait_protocol);

            //Get New RFL value--MRP
            pqmptr->GetStudy()->GetNewRflSAR(GetScanProcedure()->GetProt());
            pqmptr->GetStudy()->GetValueFromVfSAR(curr_protocol);

        } else {

            pqmptr->GetStudy()->GetNewRflSAR(GetScanProcedure()->GetProt());
            pqmptr->GetStudy()->RecalculateSAR(curr_protocol);
        }

        pqmptr->GetStudy()->SetNewSAR(curr_protocol->GetPosition());

        CPqmPmSARManager* l_sarmgr_ptr = pqmptr->GetSARManager();

        if (l_sarmgr_ptr) {

            if (K_PRESCAN_MODE_EVERY == scan_proc->GetPrescan()) {
                l_sarmgr_ptr->ResetSarPreScanParam(curr_protocol->GetSarInfo());
            }
        }

        if (K_PRESCAN_MODE_EVERY == scan_proc->GetPrescan()) {

            if (E_NO_ERROR != pqmptr->GetdBdt()->IECCheck(l_db_handle, curr_protocol, true)) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IECCheck returns Error..."));
                return E_ERROR;
            }

        } else {

            if (!acqman_manager->CalcSARControlAndCheckSAR(curr_protocol)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CalcSARControlAndCheckSAR returns Error..."));
                return E_ERROR;
            }
        }
    }

    if (scan_proc->GetCFAClinical() || scan_proc->GetCFARetry()) {
        CFARedraw(true);
    }

    if (scan_proc->GetAPCRetry()) {
        scan_proc->SetAPCRetry(FALSE);
        pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
        //+Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
        CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();
        l_pqmshimshareddat->ShimRegistTuneInfo(NULL, &data->couchInfo.position, NULL);
        //-Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
        pqmptr->WriteToUI(PQM_MSG_APC_RETRY_WAIT);
        return E_NO_ERROR;
    }

    PreScanDone(data->status, data->couchInfo.position,
                data->couchInfo.couchStatusFlag);
    pqmptr->SeriesPrescanDone();

    //+Patni-AKR/2010Aug13/Added/NFD001
    UpdateWaterFatSection(curr_protocol, pqmptr);
    //-Patni-AKR/2010Aug13/Added/NFD001
    //+Patni-HEMANT/ADDED On 6/30/2009 6:56:43 PM/ ACE-2 / WFDA
    //Patni-PJS/2010Dec10/Modified/IR-97
    //pqmptr->ProcessScanRelatedMsgForWFDA(curr_protocol, PQM_MSG_SCAN_START);

    if (scan_proc->GetPrescan() == K_PRESCAN_MODE_EVERY ||
        scan_proc->GetPrescan() == K_PRESCAN_MODE_AMB) {

        scan_proc->SetPreScanStatus(scan_proc->GetPreScanStatus() & ~STATUS_IEC_CHECK);

        if (!scan_proc->SendAcquisition()) {

            //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            //CPQMLogMgr::GetInstance()->DisplayScanError(
            //    IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
            //    SCAN_ERR_NOTIFY, L"PreScan.cpp");
            //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Cannot Connect to AcqMan"));

            //+Patni-DKH/2010Oct15/Modified/TMSC-REDMINE-860
            pqmptr->GetStudy()->SetScanModeOfHead();
            scan_proc->ResetCurrentRequest();

            if (!curr_protocol->GetSeriesNumInDbFlag()) {
                //Patni-HAR/2010Oct28/Added/V1.30#TMSC-REDMINE-860
                l_study->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
                //Patni-HAR/2010Oct28/Added/V1.30#TMSC-REDMINE-860
                pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);

                pqmptr->GetAMB()->PqmPmAbortAmb();

            } else {
                l_study->SetAcqStatus(curr_protocol, C_PR_STATUS_FAIL);
                pqmptr->WriteToUI(PQM_MSG_ABORT_FAIL);

                pqmptr->GetAMB()->PqmPmAbortAmb();
            }

            //-Patni-DKH/2010Oct15/Modified/TMSC-REDMINE-860
            //+Patni-HEMANT/ADDED On 6/30/2009 6:56:43 PM/ ACE-2 / WFDA
            //Patni-PJS/2011Jan25/Modified/IR-97
            //pqmptr->ProcessScanRelatedMsgForWFDA(curr_protocol, PQM_MSG_ABORT_FAIL);
        }

        if (scan_proc->GetAPCHold() ||
            curr_protocol->GetProbeScan() ||
            curr_protocol->GetRetroMode()) {
            pqmptr->GetButtonState(APC_PAUSE_BTN);
        }

    } else {
        //Patni-PJS/2009Jun13/Added/JFT# 252/IR(5)
        //+Patni-Meghna-DKH/2010Apr12/Added/Phase3#MCM0229-00187
        //if (!PreScanGo(AMCMR_UNKNOWN)) {
        if (!PostProcess(data->couchInfo.position, data->couchInfo.couchStatusFlag, AMCMR_UNKNOWN)) {
            //-Patni-Meghna-DKH/2010Apr12/Added/Phase3#MCM0229-00187
            //Patni-RSG/2009Sep14/Added/MVC004493
            if (curr_protocol->GetAcqStatus() != C_PR_STATUS_WAIT) {
                pqmptr->UpdateSARInfoOnPage();
            }

            return E_ERROR;
        }
    }

    //Patni-RSG/2009Sep14/Added/MVC004493
    if (curr_protocol->GetAcqStatus() != C_PR_STATUS_WAIT) {
        pqmptr->UpdateSARInfoOnPage();
    }

    return E_NO_ERROR;
}

void CPreScan::ProbeActionComplete(
) const
{
    LPCTSTR FUNC_NAME = _T("CPreScan::ProbeActionComplete");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering ProbeActionComplete Function"));

    SendMessagetoProbe(RMC_STOP_WAIT_CURSOR);
}


void CPreScan::SendMessagetoProbe(
    const int f_code
) const
{

    LPCTSTR FUNC_NAME = _T("CPreScan::SendMessagetoProbe");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering ProbeScanAbort Function"));

    CScanProcedure* l_scan_proc = GetScanProcedure();

    if (NULL == l_scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc pointer is NULL"));
        return ;
    }

    CPqmPmAcqman* l_acqman_ptr = l_scan_proc->GetAcqManager();

    if (NULL == l_acqman_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_acqman_ptr pointer is NULL"));
        return ;
    }

    CPqm* l_pqmptr = l_acqman_ptr->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmptr pointer is NULL"));
        return;
    }

    CPqmProcon* l_procon = l_pqmptr->GetPqmProCon();

    if (NULL == l_procon) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_procon pointer is NULL"));
        return;
    }

    CPqmProbe* l_probe = l_procon->GetPQMProbe();

    if (NULL == l_probe) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_probe pointer is NULL"));
        return ;
    }

    proMsg_t l_msg;
    memset(&l_msg, 0, sizeof(proMsg_t));
    l_msg.length = 0;
    l_msg.data = NULL;
    l_msg.code = f_code;
    l_probe->SendMsgToProbe(&l_msg);
}

//*******************************Method Header*********************************
//Method Name    :SendPreScan()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
bool CPreScan::SendPreScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SendPreScan");
    PQM_SCOPED_TRACE();

    //+Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
    // MEITEC/2010Apr26/Added/IR-86

    //Patni-AMT/2009Jun26/Added/JFT# Shimming offset issue.
    CScanProcedure* scan_proc = GetScanProcedure();
    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();
    CPqmProtocol* l_current_protocol = scan_proc->GetScanProcProtocol();

    CPQMDebugInfo::GetInstance(pqm)->PrescanDebug(m_data);

    if (E_ERROR == pqm->GetStudy()->MaintainCfStudy(l_current_protocol, NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error occured in MaintainCfStudy"));
        pqm->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
        pqm->GetAMB()->PqmPmAbortAmb();
        return false;
    }

    pqm->GetStudy()->CheckCFAValueinScratchPad(l_current_protocol, scan_proc->GetCFARetry());
    //+Patni-Himanshu-Sribanta/2010Sep13/Modified/MVC008773
    bool l_result = false;

    if ((!scan_proc->GetAPCRetry()) && (l_current_protocol->GetAppCode() == VFC_APP_CODE_MRS ||
                                        l_current_protocol->GetAppCode() == VFC_APP_CODE_MRSCSI)) {
        //-Patni-Himanshu-Sribanta/2010Sep13/Modified/MVC008773

        // execute MRS prescan
        if (TRUE == ExecuteMrsPrescan()) {
            m_scan_procedure->SetProcessing(
                m_scan_procedure->GetProcessing() | PQM_ACQ_BIT_SCAN_PROC);
            l_result = true;
        }

    } else if (!m_scan_procedure->GetScanProc(0).IsEmpty()) {

        m_scan_procedure->SetProcessing(
            m_scan_procedure->GetProcessing() | PQM_ACQ_BIT_SCAN_PROC);
        l_result = true;

    } else {
        pqm->WriteToUI(PQM_MSG_SCAN_ABORT_TRUE);
        l_result = SendMessage();
    }

    if (false == l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Executing function"));
    }

    return l_result;
}

//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
//**************************************************************************
//Method Name   : PqmAcqShimmingDone
//Author        : PATNI/DKH
//Purpose       : To set shimming done in varfield.
//**************************************************************************
void CPreScan::PqmAcqShimmingDone(CVarFieldHandle* const f_vf_handle,
                                  CPqmProtocol*			f_pqmprotocol,
                                  const float				f_couchPos)
{
    //+Patni/2011Feb11/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
    LPCTSTR FUNC_NAME = _T("CPreScan::PqmAcqShimmingDone");

    CScanProcedure* l_scan_proc = GetScanProcedure();

    if (NULL == l_scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc pointer is NULL"));
        return ;
    }

    CPqmPmAcqman* l_acqman_ptr = l_scan_proc->GetAcqManager();

    if (NULL == l_acqman_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_acqman_ptr pointer is NULL"));
        return ;
    }

    CPqm* l_pqmptr = l_acqman_ptr->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmptr pointer is NULL"));
        return;
    }

    char* l_path = NULL, l_file[256] = {0};

    if ((l_path = getenv("PRESCAN")) != NULL) {
        strcpy(l_file, l_path);

    } else {
        strcpy(l_file, ".");
    }

    //strcat(l_file, "CfDaily_Current.vfs");
    if ((PQM_CFA_RETRY_AND_SCAN == l_scan_proc->GetCFARetry()) &&
        (FALSE == l_pqmptr->CanDisplayCFAGraph(f_pqmprotocol))
       ) {
        strcat(l_file, "CfDaily_2Slice_Current.vfs");

    } else {
        strcat(l_file, "CfDaily_Current.vfs");
    }

    //
    //-Patni/2011Feb11/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1


    CVarFieldHandle l_shim_vf_handle(l_file);

    //-Patni-MP/2010Oct19/Modified/Reuse CFA defect
    if (l_shim_vf_handle.Open() == E_NO_ERROR) {
        VfFieldSpec_t       vfReq[1];
        int n = 0;
        VFF_SET_ARGS(vfReq[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        n++;
        int numDone = 0;
        VfPathElem_t        path0[1];
        path0[0].subtree_name = SVN_ST_PROTOCOL;
        path0[0].index = 0;
        l_shim_vf_handle.GetFields(path0, SVD_ST_PROTOCOL, vfReq, n, (int*)&numDone);

        n = 0;
        VfFieldSpec_t       vfReq1[1];

        if (vfReq[0].status == E_NO_ERROR) {
            VFF_SET_ARGS(vfReq1[n], SVN_EXE_SHIM_AFTER_FSY_VALUE, SVT_EXE_SHIM_AFTER_FSY_VALUE,
                         vfReq[0].size, vfReq[0].buffer, VFO_REPLACE);
            n++;
        }

        if (n) {
            VfPathElem_t        path1[1];
            path1[0].subtree_name = SVN_ST_PROTOCOL;
            path1[0].index = f_pqmprotocol->GetProtocol();
            time_t ts = 0L;
            status_t l_status = f_vf_handle->PutFields(path1, SVD_ST_PROTOCOL, vfReq1,
                                n, (int*)&numDone, &ts);

            if (E_NO_ERROR != l_status || numDone != n) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            }
        }

        if (vfReq[0].status == E_NO_ERROR && vfReq[n]. size > 0) {
            FREE_PTR(vfReq[0].buffer) ;
        }

    }

    CPqmShimSharedData::GetInstance()->SetShimPos(f_couchPos);
    f_pqmprotocol->SetCfaPos(f_couchPos);
}
//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187

//*******************************Method Header*********************************
//Method Name    : MaintainMotionProbingStudy()
//Author         : PATNI/SG
//Purpose        : Maintain the probe study required for RMC scanning
//*****************************************************************************
status_t CPreScan::MaintainMotionProbingStudy(
    char* f_file,
    CPqmProtocol* f_protocol     //Patni-MSN/2009Mar02/Modified/RMC - Phase3-7 reported by TMSC
)
{

    LPCTSTR FUNC_NAME = _T("CPreScan::MaintainMotionProbingStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of MaintainMotionProbingStudy"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    CScanProcedure* scan_procedure = GetScanProcedure();

    if (scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* l_pqm = scan_procedure->GetAcqManager()->GetPqm();
    CPQMStudy* l_pqm_study = l_pqm->GetStudy();

    //+Patni-MSN/2009Mar10/Modified/Optimization

    BITFLD_DECL(mask, MAX_NODES) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    memset(mask, 0, sizeof(mask));

    //+Patni-HAR/2009Aug11/Added/Memory Leaks
    //TCHAR* tchar_string = new TCHAR[n +1];
    //memset( tchar_string, 0, sizeof(TCHAR) * (n + 1));
    //mbstowcs(tchar_string, f_file, n);
    //tchar_string[n] = _T('\0');
    //-Patni-HAR/2009Aug11/Added/Memory Leaks


    CVarFieldHandle vf_handle(f_file);

    if (vf_handle.Open() != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("File handle NULL"));

        return E_ERROR;
    }

    //+Patni-MSN/2009Mar02/Modified/RMC - Phase3-7 reported by TMSC
    if (l_pqm->IsSaturnCoilSystem()) {

        VftCoilListInfo_t*  l_last_acq_coil = NULL;
        PqmSaturnCoil_t*    l_saturn_coil   = f_protocol->GetSaturnCoil();

        l_last_acq_coil = l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil();

        if (AcqCheckSaturnCoil(l_last_acq_coil, l_saturn_coil, FALSE) != E_NO_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AcqCheckSaturnCoil - Failed"));

            //			PqmUiErrorDialog(PqmUiFetchLiteral("ERR_COIL_DATA"),ERR_ERROR);
            //+Patni-KSS/2010Sep02/Commented/TMSC-REDMINE-558-REOPEN
            //+Patni-PJS/2010Jan2/TMSC_REDMINE-111
            //CScanProcedure* scan_proc = GetScanProcedure();
            //CPqmPmAcqman* acqman_ptr = scan_proc->GetAcqManager();
            //CPqm* pqmptr = acqman_ptr->GetPqm();
            //pqmptr->GetPqmProCon()->AbortProbeScan();
            //-Patni-PJS/2010Jan2/TMSC_REDMINE-111
            //-Patni-KSS/2010Sep02/Commented/TMSC-REDMINE-558-REOPEN

            //+Patni-HAR/2010Jun08/Modified/TMSC-REDMINE-347
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_COIL_DATA")),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));

            //l_error_string.LoadString(IDS_ERR_COIL_DATA);
            //CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            //	l_error_string,
            //	MPlus::EH::Crucial,
            //	_T("prescan.cpp"),
            //	_T("PQM"));
            //-Patni-HAR/2010Jun08/Modified/TMSC-REDMINE-347

            return E_ERROR;
        }
    }

    //+Patni-MSN/2009Mar02/Modified/RMC - Phase3-7 reported by TMSC

    //-Patni-MSN/2009Mar10/Modified/Optimization
    //+Patni-HAR/2010Jun01/Added/TMSC-REDMINE-111
    CString l_file(f_file);

    status_t sts = 0 ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if ((sts = l_pqm_study->MaintainStudy(l_file, f_protocol, MAINTAIN_MOTION_PROBING)) != E_NO_ERROR) {
        //-Patni-HAR/2010Jun01/Added/TMSC-REDMINE-111
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("MaintainStudy - Failed"));

        return sts ;
    }

    memset(mask, 0, sizeof(mask));
    int32_t	l_fsy_type = VFC_PRESCAN_USE_PREVIOUS ;
    l_pqm_study->SetProtValues(
        &vf_handle,
        0,
        mask,
        K_PR_PRESCAN_FSY,
        &l_fsy_type,
        sizeof(l_fsy_type),
        NULL);

    PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME,
              _T("MaintainMotionProbingStudy End"));

    return  E_NO_ERROR ;

}

//*******************************Method Header*********************************
//Method Name    : ProcessResponse()
//Author         : PATNI/HAR
//Purpose        : This method will be called after a response that Prescan
//                 is done successfully is received .This method does the
//                 setting of the last scan done values with the values of
//                 the current protocol.
//*****************************************************************************
void CPreScan::ProcessResponse(
    AM_ClientMessageHeader_t* hdr,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::ProcessResponse");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ProcessResponse"));

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();

    int type = 0;
    int result = 0;

    int funcBit = acqman->GetFunctionBit(hdr->function);

    if ((body->status == AMCM_SUCCESS) ||
        (body->status == AMCM_RECONST)) {

        //Saurabh@RMC
        if ((hdr->function == AMCMR_MO_MONITOR) || (hdr->function == AMCMA_MO_MONITOR_DONE)) {
            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

            ProbeActionComplete();

        } else if ((hdr->function == AMCMR_MO_MONITOR_ABORT) || (hdr->function == AMCMA_MO_MONITOR_ABORT_DONE)) {

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

            ProbeActionComplete();
        }

        //Saurabh@RMC
        else if ((hdr->function == AMCMR_PRESCAN) ||
                 (hdr->function == AMCMR_PRESCAN_CONTINUE)) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);
            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
            PreScanCallback(type, body);

        } else if ((hdr->function == AMCMA_PRESCAN_DONE) ||
                   (hdr->function == AMCMA_PRESCAN_CONTINUE_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

            //+Patni-Hemant+Meghana/2011Apr05/Modified/AutoShimming_AutoScan_Issue
            if (scan_proc->GetCFARetry()) {
                //+Patni-MP/2010Oct19/Added/NFD001_AT_6
                CPQMStudy* l_study = pqm->GetStudy();

                if (NULL != l_study) {
                    CPqmProtocol* curr_protocol = l_study->GetLastDone();
                    l_study->CopySpectrumDataFromShimFile(curr_protocol);
                }

                //-Patni-MP/2010Oct19/Added/NFD001_AT_6
            }

            //-Patni-Hemant+Meghana/2011Apr05/Modified/AutoShimming_AutoScan_Issue

            PreScanCallback(type, body);

        } else if ((hdr->function & AM_OFFSET_AMCMS) == AM_OFFSET_AMCMS) {

            if (AcquisitionStatusCallback(type, hdr, body) == E_NO_ERROR) {
                return;
            }
        }

    } else if (body->status == AMCM_NACK) {

        //Saurabh@RMC
        if ((hdr->function == AMCMR_MO_MONITOR) || (hdr->function == AMCMA_MO_MONITOR_DONE)) {
            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

            //+Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
            //ProbeScanAbort();
            //-Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
            ProbeActionComplete();

        } else if ((hdr->function == AMCMR_MO_MONITOR_ABORT) || (hdr->function == AMCMA_MO_MONITOR_ABORT_DONE)) {

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
            //+Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
            //ProbeScanAbort();
            //-Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
            ProbeActionComplete();
        }

        //Saurabh@RMC

        else if ((hdr->function == AMCMR_PRESCAN) ||
                 (hdr->function == AMCMR_PRESCAN_CONTINUE)) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        } else if ((hdr->function == AMCMA_PRESCAN_DONE) ||
                   (hdr->function == AMCMA_PRESCAN_CONTINUE_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;
            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
        }

        int errorId = body->status;

        if (hdr->function == AMCMA_PRESCAN_DONE) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN_DONE, body);

        } else if (hdr->function == AMCMA_PRESCAN_CONTINUE_DONE) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN_CONTINUE_DONE, body);

        } else if (hdr->function == AMCMR_PRESCAN) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN, body);

        } else if (hdr->function == AMCMR_PRESCAN_CONTINUE) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN_CONTINUE, body);
        }

    } else {

        //Saurabh@RMC
        if ((hdr->function == AMCMR_MO_MONITOR) || (hdr->function == AMCMA_MO_MONITOR_DONE)) {
            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
            ProbeActionComplete();

        } else if ((hdr->function == AMCMR_MO_MONITOR_ABORT) || (hdr->function == AMCMA_MO_MONITOR_ABORT_DONE)) {

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
            //+Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
            //ProbeScanAbort();
            //-Patni-KSS/2010Aug10/ADDED/TMSC-REDMINE-558
            ProbeActionComplete();
        }

        //Saurabh@RMC

        else if ((hdr->function == AMCMR_PRESCAN) ||
                 (hdr->function == AMCMR_PRESCAN_CONTINUE)) {

            type = AMCM_FUNC_KIND_REQUEST;
            result = scan_proc->GetRequest();
            result ^= funcBit;
            //MRPdefect812
            scan_proc->SetRequest(result);
            //MRPdefect812

        } else if ((hdr->function == AMCMA_PRESCAN_DONE) ||
                   (hdr->function == AMCMA_PRESCAN_CONTINUE_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;
            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            CString str_msg(_T(""));
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

            if (scan_proc->GetAPCRetry()) {
                scan_proc->SetAPCRetry(FALSE);

                //+Patni-SS/UnCOMMENTED/2009-May-15/JFT#137
                pqm->WriteToUI(PQM_MSG_APC_RETRY_WAIT);
            }

            if (scan_proc->GetCFARetry()) {
                //scan_proc->SetCFARetry(PQM_CFA_NONE);//REDMINE-2861
                //+Patni-SS/UnCOMMENTED/2009-May-15/JFT#137,#223
                pqm->WriteToUI(PQM_MSG_CFA_PRESCAN_COUCH);
            }
        }

        if (hdr->function == AMCMA_PRESCAN_DONE) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN_DONE, body);

        } else if (hdr->function == AMCMA_PRESCAN_CONTINUE_DONE) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN_CONTINUE_DONE, body);

        } else if (hdr->function == AMCMR_PRESCAN) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN, body);

        } else if (hdr->function == AMCMR_PRESCAN_CONTINUE) {
            ErrPrescan(type, IDS_ERR_ACQMAN_PRESCAN_CONTINUE, body);
        }

        if (scan_proc->GetProcessing()) {
            //  scan_proc->ResetCurrentRequest();
        }
    }
}

//*******************************Method Header*********************************
//Method Name    :GetErrorMessage()
//Author         :PATNI/MRP
//Purpose        :This method returns the error message depending on the
//                reply got from Acqman
//*****************************************************************************
CString CPreScan::GetErrorMessage(
    const int status,
    const int type
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::GetErrorMessage");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetErrorMessage"));

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman_ptr = scan_proc->GetAcqManager();

    CString message(_T("")), buff(_T(""));

    switch (status) {

        case AMCM_NACK:
            message = NAK_STR;
            break;

        case AMCM_COUCH:
            message = COUCH_STR;
            break;

        case AMCM_TXCOIL:
            message = TXCOIL_STR;
            break;

        case AMCM_RXCOIL:
            message = RXCOIL_STR;
            break;

        case AMCM_GATEMETHOD:
            message = GATEMETHOD_STR;
            break;

        case AMCM_SEQGEN:
            message = SEQGEN_STR;
            break;

        case AMCM_SAR:
            message = SAR_STR;
            break;

        case AMCM_PCOIL:
            message = SAR_COIL_STR;
            break;
    }

    if (type == AMCM_FUNC_KIND_REQUEST) {
        buff.Format(IDS_ERR_REQ, message);

    } else {
        buff.Format(IDS_REPLY_ERR, message);
    }

    if (message.IsEmpty()) {
        if (type == AMCM_FUNC_KIND_REQUEST) {
            buff.LoadString(IDS_ERR_ACC_PRESCAN);

        } else {
            // + ASSERTION OCCURS needs to be revisted
            //buff.LoadString(IDS_ERR_PRESCAN);
            // + ASSERTION OCCURS needs to be revisted
        }
    }

    CString str_msg(_T(""));
    str_msg.Format(_T("%d, %s) = %s"), status, acqman_ptr->GetStr(type), buff);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return buff;
}

//*******************************Method Header*********************************
//Method Name    :GetRequestCount()
//Author         :PATNI/MRP
//Purpose        :Get the Request Count
//*****************************************************************************
int CPreScan::GetRequestCount(
)const
{
    LPCTSTR FUNC_NAME = _T("CPreScan::GetRequestCount");
    int count = m_request.size();

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), count);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetRequestCount"));
    return count;
}

//*******************************Method Header*********************************
//Method Name    :GetFlagCount()
//Author         :PATNI/MRP
//Purpose        :Get the Flag count
//*****************************************************************************
int CPreScan::GetFlagCount(
)const
{
    LPCTSTR FUNC_NAME = _T("CPreScan::GetFlagCount");
    int count = m_flag.size();

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), count);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return count;
}

//*******************************Method Header*********************************
//Method Name    :GetRequest()
//Author         :PATNI/MRP
//Purpose        :Get the Request
//*****************************************************************************
AM_PreScanKind_t CPreScan::GetRequest(
    const int index
)const
{
    LPCTSTR FUNC_NAME = _T("CPreScan::GetRequest");
    AM_PreScanKind_t request = ACQ_PRESCAN_TXCOIL;
    CString l_test(_T(""));

    if (index >= 0 && index < GetRequestCount()) {
        request = m_request[index];
        l_test.Format(_T("request = %d"), request);
    }

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), request);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return request;
}

//*******************************Method Header*********************************
//Method Name    :GetFlag()
//Author         :PATNI/MRP
//Purpose        :Get the Flag
//*****************************************************************************
CPSAcqSetFlag* CPreScan::GetFlag(
    const int index
)const
{
    LPCTSTR FUNC_NAME = _T("CPreScan::GetFlag");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), index);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    CPSAcqSetFlag* flag = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if (index >= 0 && index < GetFlagCount()) {
        flag = (m_flag[index]);
    }

    return flag;
}

//*******************************Method Header*********************************
//Method Name    :GetManualPreScanDefaultData()
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
BOOL CPreScan::GetManualPreScanDefaultData(
    VARIANT** f_data
)
{

    CScanProcedure* l_scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_scan_proc == NULL) {
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* l_pqmptr = l_scan_proc->GetAcqManager()->GetPqm();
    CPqmProtocol* l_curr_protocol = l_scan_proc->GetScanProcProtocol();

    manual_prescan_ipc_t* l_manual_prescan_ctrl_values = new s_manual_prescan_ipc();

    double l_f_fsy = 0.0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    float l_f_rfl_gain = 0.0f, l_f_rfl_scale = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int l_rfl_action_value = 0, l_cfa_action_value = 0, l_rxgain_action_value = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    float l_f_rgn = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int   l_post_acq_mode = 0;
    int   status = 0;

    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections


    memset(mask, 0, sizeof(mask));
    status = l_pqmptr->GetStudy()->GetProtocolValues(l_pqmptr->GetStudy()->GetVfDBHandle(),
             l_curr_protocol->GetProtocol(),
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
    l_manual_prescan_ctrl_values->f_rfl_action = l_rfl_action_value;
    l_manual_prescan_ctrl_values->f_cfa_action = l_cfa_action_value;
    l_manual_prescan_ctrl_values->f_rxgain_action = l_rxgain_action_value;
    l_manual_prescan_ctrl_values->f_fsy = l_f_fsy;
    l_manual_prescan_ctrl_values->f_rfl_gain = l_f_rfl_gain;
    l_manual_prescan_ctrl_values->f_rfl_scale = l_f_rfl_scale;
    l_manual_prescan_ctrl_values->f_rgn = l_f_rgn;
    l_manual_prescan_ctrl_values->post_acq_mode = l_post_acq_mode;

    if (*f_data) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections
        (*f_data)->byref = l_manual_prescan_ctrl_values;
    }

    return status ;
}


//*******************************Method Header*********************************
//Method Name    :SetPrescan
//Author         :PATNI/JS
//Purpose        :
//*****************************************************************************
void CPreScan::SetPrescan(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPreScan::SetPrescan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    manual_prescan_p l_manual_prescan_values = (manual_prescan_p)f_pdata->byref;

    BOOL l_b_CFA = l_manual_prescan_values->bCFA;
    BOOL l_b_RFL = l_manual_prescan_values->bRFL;
    BOOL l_b_RxCoil = l_manual_prescan_values->bRxCoil;
    BOOL l_b_RxGain = l_manual_prescan_values->bRxGain;
    BOOL l_b_TxCoil = l_manual_prescan_values->bTxCoil;
    BOOL l_b_Vap = l_manual_prescan_values->bVap;

    int l_rfl_option = l_manual_prescan_values->rfl_option;
    int l_cfa_option = l_manual_prescan_values->cfa_option;
    int l_rx_gain_option = l_manual_prescan_values->rx_gain_option;

    CString l_rfl_gain = l_manual_prescan_values->rfl_gain;
    CString l_rfl_scale = l_manual_prescan_values->rfl_scale;
    CString l_fsy_value = l_manual_prescan_values->fsy_value;
    CString l_rgn_value = l_manual_prescan_values->rgn_value;

    char* l_c_fsy_data = new char[l_fsy_value.GetLength() + 1];
    _wcstombsz(l_c_fsy_data, l_fsy_value, l_fsy_value.GetLength() + 1);
    double l_f_data_FSY = 0.0f;
    l_f_data_FSY = atof(l_c_fsy_data);


    char* l_c_rfl_gain = new char[l_rfl_gain.GetLength() + 1];
    _wcstombsz(l_c_rfl_gain, l_rfl_gain, l_rfl_gain.GetLength() + 1);
    float l_f_rfl_gain = 0.0f;
    l_f_rfl_gain = (float)atof(l_c_rfl_gain);


    char* l_c_rfl_scale = new char[l_rfl_scale.GetLength() + 1];
    _wcstombsz(l_c_rfl_scale, l_rfl_scale, l_rfl_scale.GetLength() + 1);
    float l_f_rfl_scale = 0.0f;
    l_f_rfl_scale = (float)atof(l_c_rfl_scale);

    char* l_c_rgn_value = new char[l_rgn_value.GetLength() + 1];
    _wcstombsz(l_c_rgn_value, l_rgn_value, l_rgn_value.GetLength() + 1);
    float l_f_rgn_value = 0.0f;
    l_f_rgn_value = (float)atof(l_c_rgn_value);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    CScanProcedure* scan_proc = GetScanProcedure();

    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan procedure pointer is NULL"));

        DEL_PTR_ARY(l_c_fsy_data);

        DEL_PTR_ARY(l_c_rfl_gain);

        DEL_PTR_ARY(l_c_rfl_scale);

        DEL_PTR_ARY(l_c_rgn_value);

        return;
    }

    //
    //	memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //	memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    //
    //	if ( (scan_proc->GetScanProc(1).IsEmpty() ) &&
    //		(scan_proc->GetAPCRetry() != TRUE) &&
    //		(!curr_protocol->GetPrepScan()) &&
    //		(!curr_protocol->GetFluoro())   &&
    //		(!(curr_protocol->GetProbeScan()) == VFC_PROBE_RTMC) &&
    //		(curr_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP)
    //		) {
    //		m_header.function = AMCMM_PRESCAN_CONTINUE;
    //
    //
    //	} else {
    //		m_header.function = AMCMM_PRESCAN;
    //
    //
    //	}

    int request_count = 0;

    if (l_b_RxCoil == TRUE) {

        m_data.param.preScan.kind[0] = ACQ_PRESCAN_RXCOIL;
        request_count++;

    }

    if (l_b_TxCoil == TRUE) {

        m_data.param.preScan.kind[request_count] = ACQ_PRESCAN_TXCOIL;
        request_count++;

    }

    int status = 0;

    //+Patni-MP/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / CR MANUAL PRESCAN
    float l_rfl_default_gain_value_limit = 63.0f;
    //-Patni-MP/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / CR MANUAL PRESCAN

    CPqm* pqmptr = scan_proc->GetAcqManager()->GetPqm();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    if (l_b_RFL == TRUE) {

        m_data.param.preScan.kind[request_count] = ACQ_PRESCAN_RFLEVEL;
        request_count++;

        switch (l_rfl_option) {

            case VFC_PRESCAN_CLINICAL:

                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_RFL,
                         &l_rfl_option ,
                         sizeof(l_rfl_option),
                         NULL);
                break;

            case VFC_PRESCAN_USE_PREVIOUS:

                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_RFL,
                         &l_rfl_option ,
                         sizeof(l_rfl_option),
                         NULL);
                break;

            case VFC_PRESCAN_USE_VARFIELD:

                //+Patni-MP/MODIFIED On 6/17/2009 4:22:38 PM/ ACE-2 / CR MANUAL PRESCAN
                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_RFL,
                         &l_rfl_option,
                         sizeof(l_rfl_option),
                         K_PR_PRESCAN_RFL_GAIN,
                         &l_f_rfl_gain ,
                         sizeof(l_f_rfl_gain),
                         K_PR_PRESCAN_RFL_SCALER,
                         &l_f_rfl_scale ,
                         sizeof(l_f_rfl_scale),
                         K_PR_PRESCAN_RFL_GAIN_VALUE_LIMIT,
                         &l_rfl_default_gain_value_limit,
                         sizeof(l_rfl_default_gain_value_limit),
                         NULL);
                break;

            default:
                break;


        }
    }

    if (l_b_CFA == TRUE) {

        m_data.param.preScan.kind[request_count] = ACQ_PRESCAN_CFA;
        request_count++;

        switch (l_cfa_option) {

            case VFC_PRESCAN_CLINICAL:

                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_FSY,
                         &l_cfa_option ,
                         sizeof(l_cfa_option),
                         NULL);
                break;

            case VFC_PRESCAN_USE_PREVIOUS:

                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_FSY,
                         &l_cfa_option ,
                         sizeof(l_cfa_option),
                         NULL);
                break;

            case VFC_PRESCAN_USE_VARFIELD:
                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_FSY,
                         &l_cfa_option,
                         sizeof(l_cfa_option),
                         K_PR_PRESCAN_FSY_VALUE,
                         &l_f_data_FSY ,
                         sizeof(l_f_data_FSY),
                         NULL);
                break;

            default:
                break;


        }

    }

    if (l_b_RxGain == TRUE) {

        m_data.param.preScan.kind[request_count] = ACQ_PRESCAN_RXGAIN;
        request_count++;

        switch (l_rx_gain_option) {

            case VFC_PRESCAN_CLINICAL:
                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_RGN,
                         &l_rx_gain_option ,
                         sizeof(l_rx_gain_option),
                         NULL);
                break;

            case VFC_PRESCAN_USE_PREVIOUS:
                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_RGN,
                         &l_rx_gain_option ,
                         sizeof(l_rx_gain_option),
                         NULL);
                break;

            case VFC_PRESCAN_USE_VARFIELD:
                status = pqmptr->GetStudy()->SetProtocolValues(pqmptr->GetStudy()->GetVfDBHandle(),
                         curr_protocol->GetProtocol(),
                         mask,
                         K_PR_PRESCAN_RGN,
                         &l_rx_gain_option ,
                         sizeof(l_rx_gain_option),
                         K_PR_PRESCAN_RGN_VALUE,
                         &l_f_rgn_value ,
                         sizeof(l_f_rgn_value),
                         NULL);
                break;

            default:
                break;


        }

    }

    m_data.param.preScan.reqCnt = request_count;

    if (l_b_Vap == TRUE) {

    }

    DEL_PTR_ARY(l_c_fsy_data);		//+Patni-HAR/2009Aug11/Added/Memory Leaks

    DEL_PTR_ARY(l_c_rfl_gain);		//+Patni-HAR/2009Aug11/Added/Memory Leaks

    DEL_PTR_ARY(l_c_rfl_scale);	//+Patni-HAR/2009Aug11/Added/Memory Leaks

    DEL_PTR_ARY(l_c_rgn_value);	//+Patni-HAR/2009Aug11/Added/Memory Leaks
}
