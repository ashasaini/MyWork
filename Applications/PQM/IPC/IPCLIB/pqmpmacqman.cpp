//REDMINE-1213_Update_03_MAY
//REDMINE-1213
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
 *  File Name: pqmpmacqman.cpp
 *  Overview: Implementation of CPqmPmAcqman class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requ-
                                                            irements & Formatted
 *   3.0          Patni/LK         2008/5/10 12:00:00     Restructured after TA
                                                            comments
 *****************************************************************************/
#include "stdafx.h"
#include "PqmPmAcqman.h"
#include <sys/types.h>
#include <ErrorDisp/ErrorDispConsts.h>
extern "C" {
#include <tami/libAcq/AcqCoilSignal.h>
}
//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <Pcommlib/ListnerSocket.h>
//-Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <PDataParser/INIReader.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <libStudyManager/Mutex/ScopedLock.h>
#include <libStudyManager/Math.h>
#include <Probe/Probe.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PQMIPCManager.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "Pqm.h"
#include "CheckSequence.h"
#include "ScanTransaction.h"
#include "PQMConfig.h"
#include "voicemanager.h"
#include "PQMStudyMgr.h"
#include "PqmSpeeder.h"
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "scanprocedure.h"
#include "PQMSocket.h"
#include "PqmProbe.h"
#include "PqmPrMode.h"
#include "CFShiftCheck.h"

//Patni-MP/2009Jan28/Added/Phase-3 MVC002888
#include "DBServerWrapper.h"

#include "AcqManNotifier.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "MasterSlave.h"
#include "CoilChannelMode.h"
#include "pqmAMB.h"
#include "PqmPmSARManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

int CPqmPmAcqman::m_timer_time = 0;
int CPqmPmAcqman::m_delay_time = 0;
CPqmPmAcqman* CPqmPmAcqman::m_acq_self_object = NULL;

using namespace MPlus::EH;
using namespace MR::ACQ::SM::COMMON::MUTEX;



//*******************************Method Header*********************************
//Method Name    :OnAcqmanMsg()
//Author         :PATNI/HAR - SS
//Purpose        :
//*****************************************************************************
void CPqmPmAcqman::OnAcqmanMsg(
    const CSocketMsg& msg
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::OnAcqmanMsg");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of OnAcqmanMsg"));

    CScopedLock l_scoped_lock(m_pqmptr->GetStudy()->GetScanMutex());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));
    AM_ClientMessageHeader_t received_header;
    AM_AcqManToClient_t received_body;

    unsigned int offset = 0u; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    // + Pqm & Himanshu Test Please Remove it
    int l_size_header = sizeof(AM_ClientMessageHeader_t);
    CString l_str(_T(""));
    l_str.Format(_T("%d"), l_size_header);

    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
               _T("sizeof(AM_ClientMessageHeader_t) --> " + l_str));

    int l_size_data = sizeof(AM_AcqManToClient_t);
    l_str = _T("");
    l_str.Format(_T("%d"), l_size_data);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("sizeof(AM_AcqManToClient_t) --> " + l_str));

    const int l_msg_size = const_cast<CSocketMsg&>(msg).GetSize(); // FIXME CSocketMsg should be fixed as const
    l_str = _T("");
    l_str.Format(_T("%d"), l_msg_size);

    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
               _T("sizeof(msg.GetSize()) --> " + l_str));

    int l_extra_size = l_msg_size - (sizeof(AM_ClientMessageHeader_t) + sizeof(AM_AcqManToClient_t));
    // - Pqm & Himanshu Test Please Remove it

    memcpy(&received_header, const_cast<CSocketMsg&>(msg).GetBuffer(),
           l_msg_size - (sizeof(AM_AcqManToClient_t) + l_extra_size));
    offset = sizeof(AM_ClientMessageHeader_t);

    memcpy(&received_body, &const_cast<CSocketMsg&>(msg).GetBuffer()[offset], sizeof(AM_AcqManToClient_t));


    CString s_str(L"\r\n\t\t*** Function : ");
    s_str += GetStr(received_header.function);
    s_str += L", Status : ";
    s_str += GetStatusString(received_body.status);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, s_str);

    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    int l_func_bit = GetFunctionBit(received_header.function);
    CString l_strfunction = GetStr(received_header.function);

    if ((l_strfunction.CompareNoCase(_T("Unknown Message !!")) == 0)) {
        l_strfunction.Format(_T("Function (%d)"), received_header.function);

    }

    CString l_strstatus = GetStatusString(received_body.status);
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("%-32s : %s [0x%x]"), l_strfunction, l_strstatus, l_func_bit);



    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    //+Patni-MP/2009Sep05/Added/MVC4505
    //AfxMessageBox(s_str);
    if ((received_header.function == AMCMS_SCAN_BEGIN) ||
        (received_header.function == AMCMS_DYNAMIC_SCAN_BEGIN) ||
        (received_header.function == AMCMS_MOVINGBED_SCAN_BEGIN)) {
        if (m_pqmptr) {
            m_pqmptr->UpdateSARInfoOnPage();
        }
    }

    //-Patni-MP/2009Sep05/Added/MVC4505
    if (received_header.function == CONNECTION_GRANTED) {
        m_pqmptr->GetManagerPtr()->SetIsAcqmanConnected(true);
        return;

    } else if (received_header.function == CONNECTION_DENIED) {

        //+Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_CONNECTION_DENIED, _T("IDS_ERR_CONNECTION_DENIED"),
        //        SCAN_ERR_NOTIFY, _T("PqmPmAcqman.cpp"));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("*** Connection Denied By Acqman ***"));
        //-Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179

        m_pqmptr->GetManagerPtr()->SetIsAcqmanConnected(false);
        return;
    }

    if (received_header.function == AMCMR_CONTROL) {
        if (received_body.status == AMCM_ERROR) {
            AcqmanAbort();

        } else if (received_body.status == AMCM_NACK) {

            if (m_scan_procedure->GetControlKind() == ACQ_PAUSE_BUTTON) {

                m_pqmptr->WriteToUI(PQM_MSG_RESUME_DONE);
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PQM_MSG_RESUME_DONE"));

            } else if (m_scan_procedure->GetControlKind() == ACQ_RESUME_BUTTON) {

                m_pqmptr->WriteToUI(PQM_MSG_PAUSE_DONE);
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PQM_MSG_PAUSE_DONE"));
            }
        }

        return;

    } else if (received_header.function == AMCMA_CONTROL_DONE) {

        if (m_scan_procedure->GetControlKind() == ACQ_ABORT_BUTTON) {

            //+Patni-SS/MODDI/2009June03/MRSABORT Abort hang defect #256,258
            CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();

            if (curr_protocol != NULL &&
                (curr_protocol->GetAppCode() == VFC_APP_CODE_MRS ||
                 curr_protocol->GetAppCode() == VFC_APP_CODE_MRSCSI)) {

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("It is MRS Protocol !!!"));

            }

            //+Patni-SS/MODDI/2009June03/MRSABORT and Abort hang defect #256,258
            //+Patni-RSG/HAR/2009Aug20/MVC3221
            else if (!m_scan_procedure->GetProcessing()
                     && (curr_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT)

                    ) {
                //Patni-/PJS-AP/2009Sep21/Added/MVC004818, MVC004938
                m_continue_scan = TRUE;


            }

            //-Patni-RSG/HAR/2009Aug20/MVC3221
        }

        //+Patni-MP/Modified/15Mar2009/PSP/Defect Fix#100
        //+Patni-PJS/Modified/5Jan2010/MVC006497
        /*else if (m_scan_procedure->GetControlKind() == ACQ_PAUSE_BUTTON) {

            m_pqmptr->WriteToUI(PQM_MSG_PAUSE_DONE);

        } else if (m_scan_procedure->GetControlKind() == ACQ_RESUME_BUTTON) {

            m_pqmptr->WriteToUI(PQM_MSG_RESUME_DONE);

        }*/
        //-Patni-PJS/Modified/5Jan2010/MVC006497
        //-Patni-MP/Modified/15Mar2009/PSP/Defect Fix#100

        m_scan_procedure->SetControlKind(0);

    } else if ((received_body.status == AMCM_ABORT) ||
               (received_body.status == AMCM_ERROR)) {
        //+Patni-HAR/2009July28/Added/MVC#3626
        //+ Patni-PJS/2009Aug14/Modify/MVC002490 Unit Testing
        CPqmProtocol* protocol = m_scan_procedure->GetScanProcProtocol();

        if (protocol) {
            if (protocol->GetPrepScan() ||
                (protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP)) {
                m_pqmptr->WriteToUI(PQM_MSG_INJECT_TIME_INVISIBLE);
            }
        }

        //- Patni-PJS/2009Aug14/Modify/MVC002490 Unit Testing
        //+Patni-SL,SS(Onsite)/2009Aug27/Modified/PE-MVC#4559
        if (received_header.function == AMCMR_GATEMODE || received_header.function == AMCMA_GATEMODE_DONE) {
            CScanTransaction* scan_transaction = GetLastTransaction();

            if (scan_transaction != NULL) {
                scan_transaction->ProcessResponse(&received_header, &received_body);

                if (m_scan_procedure->GetIsScanning()) {
                    m_scan_procedure->SetIsScanning(false);
                    AcqmanAbort();
                }
            }

            //-Patni-SL,SS(Onsite)/2009Aug27/Modified/PE-MVC#4559

        } else if (received_header.function != AMCMS_SUBPROTOCOL_END) {

            if ((AMCMR_ACQINFO == received_header.function) || (AMCMA_ACQINFO_DONE == received_header.function)) {
                m_scan_procedure->SetIsCoilOrGateInfoExecuting(false);
                m_pqmptr->GetCoilSelectionTool()->ResetAcqInfoFlags();
                m_pqmptr->WriteToUI(PQM_MSG_ACQINFO_ERROR);
            }

            AcqmanAbort();
        }

        //-Patni-HAR/2009July28/Added/MVC#3626

        if (received_header.function == AMCMA_PRESCAN_CONTINUE_DONE ||
            received_header.function == AMCMA_PRESCAN_DONE) {
            m_pqmptr->GetManagerPtr()->SetPrescanDone(TRUE);

        } else {
        }

        return;

    } else {
        CScanTransaction* scan_transaction = GetLastTransaction();

        if (received_body.acqman.hold & VFC_HOLD_APC) {
            CString str(_T(""));
            str.Format(L"Acqman hold value = 0x%x", received_body.acqman.hold);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str);
        }

        if (scan_transaction != NULL) {

            CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();

            scan_transaction->ProcessResponse(&received_header, &received_body);

            if (curr_protocol && (AMCM_NACK == received_body.status)) {

                //Acqman sent NACK and we have stopped scanning then...
                if (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE) {

                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Reset CDS ScanInfo Variables..."));

                    /* If PreScan is aborted during the CDS is not complete */
                    curr_protocol->GetCDSInfo()->m_cds_step	  = CDS_STEP0_NONE;
                    curr_protocol->GetCDSInfo()->m_cds_rescan   = TRUE;
                }
            }

        } else {
            //+Patni-PJS/2009May10/Modify/IR-82 review comment
            CString l_error_str = _T("");
            l_error_str.LoadString(IDS_CANNOT_RESPOND);
            /*CPQMLogMgr::GetInstance()->WriteEventLog(IDS_CANNOT_RESPOND,
                    ERROR_LOG_CRUCIAL1  , _T("PqmPmAcqman.cpp"));*/
            CPQMLogMgr::GetInstance()->WriteEventLog(l_error_str,
                    ERROR_LOG_CRUCIAL1  , _T("PqmPmAcqman.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

            if (m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
                m_pqmptr->GetAMB()->PqmPmAbortAmb() ;
            }

            //-Patni-PJS/2009May10/Modify/IR-82 review comment
            return;
        }
    }
}

//*******************************Method Header*********************************
//Method Name    :OnAcqmanDisconnect()
//Author         :PATNI - SS
//Purpose        :
//*****************************************************************************
void CPqmPmAcqman::OnAcqmanDisconnect(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::OnAcqmanDisconnect");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("AcqMan has Disconnected ! "));

    //Patni-SS-RG/2010Mar05/Added/MSA00251-00251
    m_pqmptr->GetManagerPtr()->SetIsAcqmanConnected(false);
    //+ Patni-SS/2009May18/Added/JFT - Wrong protocol Scanning problem

    //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Call PqmPmAbortScanProc()"));
    //+Patni-HAR/2010feb25/Modified/MSA00251-00214
    //+Patni-HAR/2010Feb01/Modified/MVC006963
    //CScanProcedure* scan_procedure = m_last_trans->GetScanProcedure();
    bool l_is_scanning_flag = m_scan_procedure->GetIsScanning();
    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();
    //-Patni-HAR/2010feb25/Modified/MSA0251-00214

    m_scan_procedure->SetScanMode(PQM_PROTOCOL_SCAN);


    if (m_last_trans && m_scan_procedure && l_is_scanning_flag) {//Patni-HAR/2010feb25/Modified/MSA0251-00214
        //-Patni-HAR/2010Feb01/Modified/MVC006963
        //- Patni-SS/2009May18/Added/JFT - Wrong protocol Scanning problem

        if (curr_protocol) {

            CString l_print_str(_T(""));
            l_print_str.Format(_T("[Acquisition Status : %d]"), curr_protocol->GetAcqStatus());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

            if (curr_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT) {

                l_print_str.Format(_T("[GetProcessing : %d][Before PQM_ACQ_BIT_ACQUISITION : %d]"), m_scan_procedure->GetProcessing(), (m_scan_procedure->GetProcessing() & PQM_ACQ_BIT_ACQUISITION) ? _T("TRUE") : _T("FALSE"));
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

                if ((m_scan_procedure->GetProcessing() & PQM_ACQ_BIT_ACQUISITION) ||
                    //Patni-HAR/2010Oct28/Added/V1.30#TMSC-REDMINE-860
                    curr_protocol->GetSeriesNumInDbFlag()) {//Patni-HAR/2010feb25/Modified/MSA0251-00214

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Setting Fail Status]"));
                    m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_FAIL);
                    // + Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB
                    CString l_series_loid = curr_protocol->GetSeriesLOID();
                    const BOOL l_postmsg_status = m_pqmptr->GetDbsaStudyMgr()->
                                                  SetAcquisitionStatusInLocalDB((BSTR)l_series_loid.AllocSysString(),
                                                          curr_protocol->GetAcqStatus());

                    // - Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB

                    //+Patni-HEMANT/ADDED On 6/30/2009 6:56:43 PM/ ACE-2 / WFDA
                    //QTS#38 :sending scan fail message to InScan server just before changing the GUI status.
                    //Patni-PJS/2011Jan25/Modified/IR-97
                    //m_pqmptr->ProcessScanRelatedMsgForWFDA(curr_protocol, PQM_MSG_ABORT_FAIL);
                    //-Patni-HEMANT/ ADDED On6/30/2009 6:57:26 PM/ ACE-2 / WFDA


                    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
                    m_pqmptr->WriteToUI(PQM_MSG_ABORT_FAIL);
                    m_scan_procedure->SetIsScanning(false);//Patni-HAR/2010feb25/Modified/MSA0251-00214
                    //+Patni-PJS/2009Dec15/Added/MVC006256

                    if (!curr_protocol->GetSeriesNumInDbFlag()) {
                        int l_status = m_pqmptr->GetDbsaStudyMgr()->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

                        if (l_status == E_ERROR) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to set series in DB"));

                        } else {
                            curr_protocol->SetSeriesNumInDbFlag(TRUE);
                        }
                    }

                    //Patni-HAR/2010Mar26/Added/MVC007422

                    //-Patni-PJS/2009Dec15/Added/MVC006256
                    // - Sarjeet

                    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Setting Wait Status]"));
                    m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
                    m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                    m_scan_procedure->SetIsScanning(false);//Patni-HAR/2010feb25/Modified/MSA0251-00214

                }

                //+Patni-HEMANT/ADDED On 3/20/2009 11:41:08 AM/ PSP1#282
                //when scan is going on and acqman is termintaed, we should show abort message
                //and at last we should also show disconnect message.
                CPQMLogMgr::GetInstance()->DisplayScanError(
                    IDS_ERR_SCAN_ABORTED, _T("IDS_ERR_SCAN_ABORTED"),
                    SCAN_ERR_NOTIFY,
                    L"PqmPmAcqman.cpp");
                //-Patni-HEMANT/ADDED On 3/20/2009 11:41:08 AM/ PSP1#282
                //+Patni-HAR/2009May05/Added/JFT/Defect#82

            } else {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Setting Wait Status if current protocol is not CURRENT]"));
                m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
                m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                m_scan_procedure->SetIsScanning(false);//Patni-HAR/2010feb25/Modified/MSA0251-00214
            }

            //-Patni-HAR/2009May05/Added/JFT/Defect#82

        }

        //+Patni-HAR/2010Feb01/Modified/MVC006963

    } else if (curr_protocol && m_scan_procedure->GetAPCRetry()) {//Patni-HAR/2010feb25/Modified/MSA0251-00214

        if (curr_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT) {

            m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        }

        m_scan_procedure->SetAPCRetry(FALSE);//Patni-HAR/2010feb25/Modified/MSA0251-00214
        m_pqmptr->WriteToUI(PQM_MSG_APC_RETRY_WAIT);
        //Patni-ARD/2010Feb05/Added/Deft# MVC006963
        m_pqmptr->WriteToUI(PQM_MSG_APC_RETRY_WAIT_ON_DISCONNECT);

        //-Patni-HAR/2010Feb01/Modified/MVC006963

    } else {
        if ((m_last_trans == NULL) && (curr_protocol != NULL)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Setting Wait Status]"));
            m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_scan_procedure->SetIsScanning(false);//Patni-HAR/2010feb25/Modified/MSA0251-00214

        } else {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("m_last_trans is NULL or Current Protocol is NULL")); //Patni-HAR/2010Feb01/Modified/MVC006963
        }
    }

    DEL_PTR(m_acqman_socket);

    //+Patni-KSS/2010Sep08/ADDED/TMSC-REDMINE-558-REOPEN
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

    CPqmProcon* l_procon = m_pqmptr->GetPqmProCon();

    if (NULL == l_procon) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_procon pointer is NULL"));
        return;
    }

    if (m_scan_procedure->GetProbing()) {
        l_procon->StopProlim();
        m_scan_procedure->SetProbing(FALSE);
    }

    CPqmProbe* l_pqm_probe = m_scan_procedure->GetPqmProbe();

    if (l_pqm_probe && 1 == l_pqm_probe->GetProbeMode()) {
        l_pqm_probe->RefProbeScanEnd(); //RMC
    }

    if (m_scan_procedure->GetCFARetry()) {
        m_scan_procedure->SetCFARetry(PQM_CFA_NONE);
        m_pqmptr->SetPostProcessFlag(FALSE);
        m_pqmptr->WriteToUI(PQM_MSG_CFA_PRESCAN_END);
    }

    //-Patni-KSS/2010Sep08/ADDED/TMSC-REDMINE-558-REOPEN
    //Patni-HAR/2010Mar05/Added/MSA00251-00251
    m_scan_procedure->SetScanStart(NULL);
    m_pqmptr->WriteToUI(PQM_MSG_ACQMAN_DISCONNECTED); // let GUI to know Acqman is disconnected;

    m_pqmptr->GetAMB()->PqmPmAbortAmb(FALSE);

    //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
    //CPQMLogMgr::GetInstance()->DisplayScanError(
    //    IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
    //    SCAN_ERR_NOTIFY, L"PqmPmAcqman.cpp");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to Acqman"));
    //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
    //
    //-Patni-HEMANT/ADDED On 20MAR2009/ PSP1#282
}

//*******************************Method Header*********************************
//Method Name    :AutoScanStart()
//Author         :PATNI/HAR
//Purpose        :This method starts the auto scan for the next protocol in the
//                wait condition.
//*****************************************************************************
void CPqmPmAcqman::AutoScanStart()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AutoScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AutoScanStart"));

    CPqm* l_pqm = GetPqm();
    CPQMStudy* l_study = l_pqm->GetStudy();

    CPqmProtocol* l_topprotocol = NULL;
    l_study->GetTopWaitPos(&l_topprotocol);

    if (l_topprotocol != NULL) {

        //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
        CPQMIPCManager* l_ppqmipcmanager = l_pqm->GetManagerPtr();

        if (NULL != l_ppqmipcmanager) {
            if (l_ppqmipcmanager->RemoteCheckActiveFlag() && !l_pqm->PqmPmIsActive(K_PM_TYPE_AMB)) {
                l_topprotocol->SetHoldScan(VFC_HOLD_PRE_SCAN);
                int l_hold_scan = l_topprotocol->GetHoldScan();
                l_study->SetProtocolValue(l_study->GetVfDBHandle(), l_topprotocol, K_PR_HOLD, &l_hold_scan,
                                          sizeof(int32_t), TRUE, NULL);

                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                        CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE")),
                        SCAN_ERR_NOTIFY, L"CPqmPmAcqman.cpp");

                l_pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
                return;
            }
        }

        if (m_pqmptr->GetSARControlLicense() && !m_pqmptr->GetManagerPtr()->GetIsSARMgrConnected()) {
            AutoScanTimerReset();

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPqmUtilities::GetMultiLingualString(_T("IDS_STOP_AUTO_SCAN_AS_SARCONTROL_OFF")),
                    MPlus::EH::Information, _T("CPqmPmAcqman.cpp"), _T("PQM"));

            l_pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
            l_pqm->GetAMB()->PqmPmAbortAmb();
            return;
        }


        //-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole

        if (l_topprotocol->GetHoldScan() & VFC_HOLD_PRE_SCAN) {
            AutoScanTimerReset();
            l_pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
            return;
        }


        if (!CanAutoScanStart(l_topprotocol->GetProtocol())) {
            AutoScanTimerReset();
            l_pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
            return ;
        }

        //If next protocol is AutoLocator Planned wait for the it to get invoked
        CheckWhetherToWaitForAutoLocatorPlannedProt(l_topprotocol);

        CheckDelayAndScanStartForFirstWaitProtocol(l_topprotocol);
    }
}



//****************************Method Header********************************
//Method Name   :ScanStart()
//Author        :PATNI/MRP
//Purpose       :This method will start the scan procedure. It will check the
//               Acquisition condition and proceed with the scan.
//*************************************************************************
void CPqmPmAcqman::ScanStart(
    CPqmProtocol* protocol,
    const BOOL retry
)
{
    //Patni-AMT/2010Apr16/Modified/JaFT# COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::ScanStart");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace of ScanStart"));

    //+Patni-PJS/2010Dec10/Modified/IR-97
    CPQMStudy*  l_study = m_pqmptr->GetStudy();
    m_current_protocol = protocol;
    int         status = 0;
    int         sar_status = 0;
    static int  step_level ;

    if (NULL == l_study) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("l_study pointer is NULL"));
        return;
    }

    //-Patni-PJS/2010Dec10/Modified/IR-97

    if (m_scan_procedure ==  NULL || m_pqmptr == NULL) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("Sacn Procedure pointer or pqm pointer is NULL"));
        return;
    }

    m_scan_procedure->SetScanMode(PQM_PROTOCOL_SCAN);

    if (NULL == protocol || protocol->GetChildThreadStatus() == CT_DELETE) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Protocol is marked for deletion so cannot start scan or is NULL"));

        if (NULL != protocol) {
            protocol->SetChildThreadStatus(CT_NONE);
        }

        m_pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return ;
    }

    m_scan_retry = (retry == TRUE);

    if (!m_scan_retry) {
        m_scan_procedure->DeleteAllScanTransactionObjs();
    }

    m_pqmptr->GetCFShiftCheck()->SetIsCFARetryExecuted(FALSE);

    //Check whether protocol AutoLocator Enabled and
    //protocol consition likes slice number etc.
    if (!m_pqmptr->CheckAutoLocatorScanConditions(protocol)) {
        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        CString l_str = CPqmUtilities::GetMultiLingualString(_T("ID_INVALID_CALA_PROTOCOL_PARAM"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_str,
                MPlus::EH::Information, _T("CPqmPmAcqman::ScanStart"),
                _T("PQM"));
        return;
    }

    //Check whether protocol is planned with Autolocator
    //if yes the set offset parameters
    if (FALSE == m_pqmptr->SetAutoLocatorOffsetParameters(protocol)) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("Failed to process cardiac parameter settings"));
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }



    //+Patni-Hemant/2010Aug19/Modified/MVC008295
    if (!m_scan_procedure->GetIsScanning() &&
        m_pqmptr->GetCDS() && protocol && protocol->GetASCOMPASSFlag()
        && (protocol->GetCDSInfo()->m_cds_step != CDS_STEP6_DONE)) {

        /* COMPASS is set , backed up coil */
        m_pqmptr->GetCDS()->PqmCdsBackUpCoil(protocol->GetSaturnCoil());
    }

    //-Patni-Hemant/2010Aug19/Modified/MVC008295

    //+Patni-PJS/2011Feb09/Added/MVC009736
    if ((protocol->GetNumOfCoverages() > 1) && (protocol->GetMovingCouch() > 0)) {

        const CString l_log_str = CPqmUtilities::GetMultiLingualString(_T("IDS_MULTI_COVG_MOVING_BED_ERROR")) ;
        CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_log_str, _T("PQM"),
                SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }

    //-Patni-PJS/2011Feb09/Added/MVC009736

    //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

    /* If you reset before scanning V9.50 CDS */
    if (protocol->GetCDSInfo()->m_cds_step <= CDS_STEP4_SCAN) {
        protocol->GetCDSInfo()->m_cds_step = CDS_STEP0_NONE;
    }

    //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement


    //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoilDB*    l_coildb_ptr = m_pqmptr->GetCoilDB();

    //+Patni-PJS/2010Dec10/Modified/IR-97

    /*
    CPQMStudy*  l_study = m_pqmptr->GetStudy();
    m_current_protocol = protocol;
    int         sar_status = 0;
    static int  step_level ;
    */
    //-Patni-PJS/2010Dec10/Modified/IR-97
    if (NULL == m_current_protocol) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("m_current_protocol pointer is NULL"));
        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }


    if (E_ERROR == m_pqmptr->GetMasterSlavePtr()->CheckForSlaveScan(protocol)) {
        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }




    if (m_pqmptr->IsSaturnCoilSystem()) {
        // Node code found in base code.@AMT20080802
    } else {
        //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("pqmAcqScanStart [%d] %s: Coil[%x]"),
                protocol->GetProtocol(), l_study->GetStudyName(), protocol->GetPqmPrMode()->GetRecvCoilId());
        //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

        int l_protocol = protocol->GetProtocol();
        CString l_protocol_str(_T(""));
        CString l_string(_T("Protocol : "));

        l_protocol_str.Format(_T("%d"), l_protocol);

        l_string += l_protocol_str;
        l_string += _T(" ");
        l_string += _T("Study Name : ");
        l_protocol_str = l_study->GetStudyName();
        l_string += l_protocol_str;

        int l_recv_coil = protocol->GetPqmPrMode()->GetRecvCoilId();

        l_string += _T(" ");
        l_string += _T("Receiver Coil Id : ");
        l_protocol_str.Empty();
        l_protocol_str.Format(_T("%d"), l_recv_coil);
        l_string += l_protocol_str;

        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, l_string);
    }

    //  Later Phases of Study Manager
    if (!retry) {
        m_pqmptr->GetAMB()->PqmAmbEachAcquisitionStart(protocol, 0) ;
    }

    m_scan_procedure->SetPreScanStatus(0);
    m_delay_start_obj = NULL;

    if (!retry) {
        step_level = 0;
    }

    /*  ProbeCorrection */
    CPqmProbe* pqm_probe = m_scan_procedure->GetPqmProbe();	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (pqm_probe) {											//Patni-MJC/2009Aug17/Modified/cpp test corrections

        //+Patni-Hemant/10Nov2010/Modified/TMSC-REDMINE-946
        if (!pqm_probe->RefProbeScanStart()) {

            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();

            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return;
        }

        //-Patni-Hemant/10Nov2010/Modified/TMSC-REDMINE-946
    }

    if (protocol->GetFluoro() /*&& PqmPmIsActive(K_PM_TYPE_PROLIM)*/) {

        // Cannot execute FLUORO while running Sequence Editor!!
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_CANNOT_START_FLUORO, _T("IDS_ERR_CANNOT_START_FLUORO"), SCAN_ERR_CRUCIAL, L"PqmPmAcqman.cpp");

        l_study->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();

        m_pqmptr->GetStudy()->SetAcqStatus(protocol, C_PR_STATUS_WAIT);
        //added by Hemant
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }

    //+ Patni-SS/2009Jun23/Added/Internal defect solved
    m_scan_procedure->SetScanProcProtocol(protocol);

    if (step_level == 0) {

        CString l_str(_T(""));
        //Patni-ARD/2009Jun17/Modified/JFT# AcqOrder Checking between GUI and IPC
        l_str.Format(_T("Scan Protocol --> %d"), m_current_protocol->GetProtocol());

        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  l_str + _T(" ") + m_pqmptr->GetStudy()->GetStudyName());

        //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Scan Start [%d] %s"),
                m_current_protocol->GetProtocol(), l_study->GetStudyName());

        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("Dynamic : %d  Auto Voice : PLN=%02X, EXE=%02X"),
                m_current_protocol->GetDynamic(),
                m_current_protocol->GetPlnAutoVoice(),
                m_current_protocol->GetExeAutoVoice());
        //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        step_level++;

        //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

        if (!m_scan_retry &&
            !((protocol->GetCDSInfo()->m_cds_flag) && !(protocol->GetCDSInfo()->m_cds_step <= CDS_STEP0_NONE)) &&
            !(protocol->GetAtuomapAutoShimAdded())
           ) {

            m_scan_procedure->ResetSARMsgCheckBit();
        }

        protocol->SetAtuomapAutoShimAdded(false);
        m_pqmptr->GetStudy()->GetValueFromVfSAR(protocol);
        m_pqmptr->GetStudy()->RecalculateSAR(protocol);

        CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("!!!! CheckSar !!!! SCAN_START prot = %d"), m_current_protocol->GetProtocol());

        if (!CalcSARControlAndCheckSAR(protocol, true)) {
            return;
        }
    }

    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    CDataManager* l_data_mgr = CDataManager::GetInstance();

    if (NULL == l_data_mgr) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("l_data_mgr is NULL"));

        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }

    CPqmCoilData* l_pqm_coil_data = l_data_mgr ->GetPQMCoilData();

    if (NULL == l_pqm_coil_data) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("l_pqm_coil_data is NULL"));

        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    // + V9.25 coil support@AMT
    // For testing purpose condition is changed to not (!)
    if (m_pqmptr->IsSaturnCoilSystem()) {	// Saturn

        // + LibAcq AMT
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        /*CDataManager* l_data_mgr = CDataManager::GetInstance();
        CPqmCoilData* l_pqm_coil_data = l_data_mgr ->GetPQMCoilData();*/
        // - LibAcq AMT
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        if (protocol->GetSaturnCoil()->numSection == 0 ||
            protocol->GetCoilDBTable()->numCoil == 0 ||
            l_pqm_coil_data->PqmInstallCheckSaturn(
                &protocol->GetSaturnCoil()->CoilList) != E_NO_ERROR
           ) {

            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            if (!protocol->GetCDSInfo()->m_cds_flag || protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE) {
                //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(
                    IDS_ERR_MSG_SET_RCV_COIL, _T("IDS_ERR_MSG_SET_RCV_COIL"), SCAN_ERR_NOTIFY, L"PqmPmAcqman.cpp");

                l_study->SetScanModeOfHead();
                m_scan_procedure->ResetCurrentRequest();

                //added by Hemant
                l_study->SetAcqStatus(protocol, C_PR_STATUS_WAIT);
                m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                m_pqmptr->GetAMB()->PqmPmAbortAmb();
                return;
                //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            }

            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        }

        //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        else {
            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            // + LibAcq AMT

            DllT* l_coil_head    = l_data_mgr->GetPQMCoilData()->GetHead();
            DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();
            PqmSaturnCoil_t* l_saturn_coil = protocol->GetSaturnCoil();

            AcqGetcoilSignal(protocol->GetSystemChannels(), l_saturn_coil->numSection,
                             l_saturn_coil->RcvCoil, l_saturn_coil->Signal,
                             l_coil_head, l_section_head);
            // - LibAcq AMT

            //l_coildb_ptr->CoilSelectSetSignal();

            //CCoilChannelMode* l_coilchannle_mode = m_pqmptr->GetCoilChannelMode();

            //if ((l_coilchannle_mode != NULL) && (l_coilchannle_mode->IsCoilChannelModeApplicable()) && (TRUE != protocol->GetCDSInfo()->m_cds_flag)) {////REDMINE-3305
            if ((TRUE != protocol->GetCDSInfo()->m_cds_flag)) {
                if (TRUE != l_study->MinimumChannelCheck(l_saturn_coil->numSection, l_saturn_coil->CoilList.num, l_saturn_coil->RcvCoil, l_saturn_coil->Signal)) {

                    const CString l_log_str = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_COIL_RESELECT")) ;
                    CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_log_str, _T("PQM"),
                            SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

                    l_study->SetAcqStatus(protocol, C_PR_STATUS_WAIT);
                    l_study->SetScanModeOfHead();
                    m_scan_procedure->ResetCurrentRequest();


                    m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                    m_pqmptr->GetAMB()->PqmPmAbortAmb();
                    m_pqmptr->WriteToUI(PQM_MSG_CORRECT_COIL);

                    return;
                }

            }


            l_study->PutSaturnCoil(l_study->GetVfDBHandle(), protocol->GetProtocol(),
                                   protocol->GetSaturnCoil()
                                  );

            if (l_pqm_coil_data->PqmCoilIsDegeneracy(
                    protocol->GetSaturnCoil()->numSection,
                    protocol->GetSaturnCoil()->RcvCoil)
               ) {

                protocol->GetSaturnCoil()->numSignalDegeneracy =
                    l_pqm_coil_data->PqmCoilGetMinChannel(
                        protocol->GetSaturnCoil()->numSection,
                        protocol->GetSaturnCoil()->RcvCoil) ;
                // + LibAcq AMT
                // + OnSite HAR-AMT
                AcqGetcoilSignal(protocol->GetSaturnCoil()->numSignalDegeneracy,
                                 l_saturn_coil->numSection,
                                 l_saturn_coil->RcvCoil,
                                 l_saturn_coil->SignalDegeneracy,
                                 l_coil_head, l_section_head);
                // - OnSite HAR-AMT
                // - LibAcq AMT

            } else {
                protocol->GetSaturnCoil()->numSignalDegeneracy = 0;
            }

            l_study->PutSaturnCoilDegeneracy(protocol->GetProtocol(),
                                             protocol->GetSaturnCoil());
            // - V9.25 coil support@AMT
            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        }

        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

    } else {
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        CCoil l_coil_obj;
        l_pqm_coil_data->PqmCoilGetCoil(protocol->GetPqmPrMode()->GetRecvCoilId(), l_coil_obj);

        if ((protocol->GetPqmPrMode()->GetRecvCoilId() == 0) ||
            (l_coil_obj.GetCoil() == NULL)) {
            /*(m_pqmptr->GetCoilDB()->GetCoil(
                 protocol->GetPqmPrMode()->GetRecvCoilId()) == NULL)) {*/
            //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_MSG_SET_RCV_COIL,
                _T("IDS_ERR_MSG_SET_RCV_COIL"),
                SCAN_ERR_NOTIFY,
                L"PqmPmAcqman.cpp");

            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();

            //Added by Hemant On 7/20/2008 8:37:33 PM
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return;
        }
    }

    if (protocol->GetPqmPrMode()->GetAnatomy() < 1) {

        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_MSG_SET_SAR_ANATOMY,
            _T("IDS_ERR_MSG_SET_SAR_ANATOMY"),
            SCAN_ERR_NOTIFY,
            L"PqmPmAcqman.cpp");

        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();


        //added by Hemant
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }

    //+Patni-MP/2009Mar16/Added/ACE-1/MCM0213-00181
    if (m_pqmptr->GetStudy()->DeleteParentIndexFplanning(protocol->GetProtocol()) != E_NO_ERROR) {

        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();

        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_MSG_STUDY_WRITE_ERROR,
            _T("IDS_ERR_MSG_STUDY_WRITE_ERROR"),
            SCAN_ERR_NOTIFY,
            L"PqmPmAcqman.cpp");
        CString l_errString = _T("");
        l_errString.LoadString(IDS_ERR_MSG_STUDY_WRITE_ERROR);
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, l_errString);
        return;
    }

    if (!m_pqmptr->CheckScanEditForInscan()) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("CheckScanEditForInscan() Failed."));
        return;
    }

    //-Patni-MP/2009Mar16/Added/ACE-1/MCM0213-00181
    if (m_scan_procedure->MoveCouchProhibitionCheck(protocol) == E_ERROR) {

        //+Patni/2011Mar16/Added/V2.0/Redmine-1548
        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        //-Patni/2011Mar16/Added/V2.0/Redmine-1548

        //added by Hemant
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();
        return;
    }

    //pqmPostProcMk(&PqmGlobal.pqmStudy, prObj) ;
    //+Patni+MP/2009Jun13/Added/ACE-2/Missing Vantage Auto Post Proc
    m_pqmptr->GetStudy()->PostProcessMk(protocol);
    //-Patni+MP/2009Jun13/Added/ACE-2/Missing Vantage Auto Post Proc
    m_pqmptr->GetStudy()->NoiseReductionProcessCheck(protocol->GetProtocol());
    m_pqmptr->GetStudy()->EMTONE_LicenseCheck(protocol->GetProtocol());


    if (AcqMakeLockFile() != E_NO_ERROR) {
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_ACQ_LOCK_FILE,
            _T("IDS_ERR_ACQ_LOCK_FILE"),
            SCAN_ERR_NOTIFY,
            L"PqmPmAcqman.cpp");

        m_pqmptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();

        //added by Hemant
        m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
        m_pqmptr->GetAMB()->PqmPmAbortAmb();

        return;
    }

    m_scan_procedure->SetScanProcProtocol(protocol);
    m_scan_procedure->SetProt(m_scan_procedure->GetScanProcProtocol()->GetProtocol());
    m_scan_procedure->SetIsScanning(true);
    //PqmShmRegistProtocol(PqmAcqGlobal.req.prot);
    l_study->CopyCurrentProtocol(protocol->GetProtocol());
    // + Shimming Lokesh 13 March 2008
    CPqmShimSharedData::GetInstance()->ShimRegistProtocol(protocol->GetProtocol());
    // - Shimming Lokesh 13 March 2008

    if (step_level == STEP_LEVEL_1) {
        step_level++;

        //+Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
        ////Patni-PJS/2009Aug25/Added/MVC004366
        //CVoiceManager::GetInstance()->SafeExit();
        //-Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026

        /* AUTO_VOICE */
        if (protocol->GetPlnAutoVoice()) {
            if (m_scan_procedure->SetTPCTRLAutoVoice(l_study->GetVfDBHandle(),
                    m_scan_procedure->GetScanProcProtocol())) {

                if (m_scan_procedure->GetAvTp1Margin() < 0) {

                    m_scan_procedure->WarningScan(
                        PQM_WARN_SCAN_AUTO_VOICE,
                        L"PqmPmAcqman.cpp");
                    //Patni-NP/2009Dec30/Commented/MVC006508
                    //m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);

                    return;
                }
            }
        }
    }

    //  COVERAGES Check
    if (step_level == STEP_LEVEL_2) {
        step_level++;
    }

    //Check disk space is not confirmed, so if CDS STEP6 (after the final coil) to the
    //int status = 0;
    if (!protocol->GetCDSInfo()->m_cds_flag || protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE) {
        if ((status = PqmCheckDisk(protocol)) != E_NO_ERROR) {
            return;
        }
    }

    //+Patni-AJS/2011Jan31/Added/V1.35/MVC009666
    else {
        m_pqmptr->GetStudy()->DelPCVAPMode(protocol->GetProtocol());
    }

    //-Patni-AJS/2011Jan31/Added/V1.35/MVC009666

    /* MovingBed Scan is checked before Speeder/Intensity Scan */
    if (status = m_pqmptr->GetSpeederPtr()->MovingBed_SpeederCheck(protocol)) {
        if (status == PQM_WARN_SCAN_PV_MOVINGBED_SPEEDER) {
            //+Patni-PJS/2009Dec01/Modified & Added/MVC05969
            CString l_warnapp_name = _T("");
            l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);
            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PV_MOVINGBED_SPEEDER")),
                    l_warnapp_name,
                    SCAN_WARN_INFORMATION, //TMSC-Tanoue/2010Mar05/MSA0251-00247
                    SCAN_WARN_SYSMODAL,
                    SCAN_WARN_OKBUTTON);
            //-Patni-PJS/2009Dec01/Modified & Added/MVC05969
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();

            //+Patni-PJS+Hemant/2010May19/Modified/JaFT/TMSC-REDMINE-111
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();

        } else if (status == PQM_WARN_SCAN_PV_MOVINGBED_INTEN) {

            m_scan_procedure->WarningScan(status, L"PqmPmAcqman.cpp");
        }

        return;
        //-Patni-PJS+Hemant/2010May19/Modified/JaFT/TMSC-REDMINE-111

    }

    if (step_level == STEP_LEVEL_3) {
        step_level++ ;
        status = m_scan_procedure->VisualPrepDelayCheck(protocol);	// + SM4 Himanshu VISUAL_PREP 13 JAN 2009

        //+Patni-PJS+HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441
        if (status != E_NO_ERROR) {

            if (false == m_scan_procedure->WarningScan(
                    status,
                    L"PqmPmAcqman.cpp")) {

                m_pqmptr->GetStudy()->SetScanModeOfHead();
                m_scan_procedure->ResetCurrentRequest();
                m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                m_pqmptr->GetAMB()->PqmPmAbortAmb();

            }

            return;
        }

        //-Patni-PJS+HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441
    }

    if (step_level == STEP_LEVEL_4) {
        step_level++ ;

        if (m_pqmptr->GetStudy()->DRKSCheck(protocol)) {

            m_scan_procedure->WarningScan(PQM_WARN_SCAN_DRKS_RECON_MODE, L"PqmPmAcqman.cpp");
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return;
        }
    }

    if (step_level == STEP_LEVEL_5) {
        step_level++;

        if (m_current_protocol->GetShimming() == VFC_SHIM_HIGH_RESOLUTION  && !m_pqmptr->GetMrsLicense()) {

            m_scan_procedure->WarningScan(
                PQM_WARN_SCAN_HR_SHIMMING,
                _T("PqmPmAcqman.cpp"));

            //added by Hemant
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return;
        }
    }

    m_scan_procedure->AcqModeSet(protocol);

    if ((protocol->GetAMBModeFlag() != VFC_AMB_MODE_NONE)
        && !m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
        //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
        time_t ts = 0L;
        int n = 0 , numDone = 0  ;
        VfFieldSpec_t fieldSpec[5] = {0};
        VfPathElem_t path[5] = {0};		//Patni-MJC/2009Aug17/Modified/cpp test corrections
        //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
        int32_t mode = VFC_AMB_MODE_NONE;
        int32_t sub = FALSE;

        memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 5);	//Patni-MJC/2009Aug17/Modified/cpp test corrections
        memset(path, 0, sizeof(VfPathElem_t) * 5);			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        path[0].subtree_name = SVN_ST_PROTOCOL;
        path[0].index = protocol->GetProtocol();

        n = 0;
        VFF_SET_ARGS(fieldSpec[n],
                     SVN_PLN_AMB_MODE_FLG,
                     SVT_PLN_AMB_MODE_FLG,
                     sizeof(mode),
                     &mode,
                     VFO_REPLACE);
        n++;
        VFF_SET_ARGS(fieldSpec[n],
                     SVN_PLN_AMB_SUBTRACTION,
                     SVT_PLN_AMB_SUBTRACTION,
                     sizeof(sub),
                     &sub,
                     VFO_REPLACE);
        n++;

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T("SVT_PLN_AMB_MODE_FLG =%d, SVT_PLN_AMB_SUBTRACTION = %d"), mode, sub);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        CVarFieldHandle* vf_handle = l_study->GetVfDBHandle();

        if (!vf_handle) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);

            return;
        }

        status_t l_status = vf_handle->PutFields(path,  SVD_ST_PROTOCOL,
                            fieldSpec,  n, (int*) & numDone, &ts);

        if (E_NO_ERROR != l_status || numDone != n) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);

            return;
        }

        //Patni-RAJ/Modified On 10/09/2009
        //CPQMStudy::PutFields(l_study->GetVfDBHandle(),  path,  SVD_ST_PROTOCOL,
        //	 fieldSpec,  n,   (int*)&numDone, &ts);
        //
        protocol->SetAMBModeFlag(mode);
    }

    if (m_scan_procedure->GetScanProcProtocol()->GetSpeederType() !=
        VFC_SPEEDER_TYPE_SENSITIVITY) {

        int l_mode = FALSE;
        BITFLD_DECL(mask, K_ALL_PR) = {NULL}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

        if (E_NO_ERROR != m_pqmptr->GetStudy()->SetProtocolValues(m_pqmptr->GetStudy()->GetVfDBHandle(),
                m_scan_procedure->GetScanProcProtocol()->GetProtocol(),
                mask,
                K_PR_CHANGED_RECEIVE_COIL,
                &l_mode,
                sizeof(int),
                NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }
    }

    if (m_current_protocol->GetShimming()) {
        m_pqmptr->GetStudy()->ProtocolShimmingStudy();
    }

    m_pqmptr->GetStudy()->SetRGNCoverageMode(
        m_scan_procedure->GetScanProcProtocol()->GetProtocol(), m_pqmptr->GetAllCoverageRgn());

    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("ScanProc[0] %s"), m_scan_procedure->GetScanProc(0));
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("ScanProc[1] %s"), m_scan_procedure->GetScanProc(1));
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("ScanProc[2] %s"), m_scan_procedure->GetScanProc(2));

    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    if (m_current_protocol->GetDynamic() &&
        m_current_protocol->GetPlnAutoVoice()) {

        m_scan_procedure->SetDynamicVoice();
    }

    //+Patni-PJS/2010Jun17/Added/MVC008293 CR
    if (E_ERROR == l_study->ClearFieldsOnScanStart(protocol->GetProtocol())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to clear field on scan start"));
    }

    //-Patni-PJS/2010Jun17/Added/MVC008293 CR

    if (m_pqmptr->GetStudy()->GetTopWaitGateProt(&protocol)) {

        if (!GetWirelessFilterAndMode(protocol->GetProtocol(),
                                      m_pqmptr->GetGateParamObj()->GetWirelessGateParam()->m_wireless_filter,
                                      m_pqmptr->GetGateParamObj()->GetWirelessGateParam()->m_mode)) {
            return;
        }

        if (!m_scan_procedure->SendGateModeInfo(
                protocol->GetGatingMethod(),
                m_pqmptr->GetGateParamObj()->GetWirelessGateParam(), true)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));


            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return;
        }

    } else if (!m_pqmptr->GetStudy()->GetRMSAEStatusForStudy()) {

        if (!m_pqmptr->GetStudy()->SendSAEInfo()) {

            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
        }

    } else if (CheckScanTimeLimitation()) {
        if (!m_scan_procedure->ScanStart()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return;
        }
    }
}

//****************************Method Header************************************
//Method Name   :CheckScanTimeLimitation()
//Author        :iGATE\Varun
//Purpose       :Compare calculated scan limit time with scan total time
//               and display warning message if total time exceeded
//*****************************************************************************
bool CPqmPmAcqman::CheckScanTimeLimitation()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CheckScanTimeLimitation");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace of CheckScanTimeLimitation"));

    int32_t l_temp_max_scan_time_flag = 0;

    if (!GetProtocolMaxScanTimeFlag(m_current_protocol->GetProtocol(), &l_temp_max_scan_time_flag)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Varfield handle is null or scan time Tags are not present in study file"));
    }

    if (l_temp_max_scan_time_flag != 0) {

        ProtocolScanTimeData_t l_protocol_scan_time_data;

        if (!GetProtocolScanTimeData(m_current_protocol->GetProtocol(), &l_protocol_scan_time_data)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Varfield handle is null or scan time Tags are not present in study file"));
        }

        const float l_scan_limit_time = l_protocol_scan_time_data.num_temp_max_scan_time * l_protocol_scan_time_data.temp_max_scan_time_ratio;

        const int l_rounded_scan_limit_time = static_cast<int>(MR::ACQ::SM::COMMON::MATH::RoundDown(l_scan_limit_time, 0));

        if (l_protocol_scan_time_data.scan_total_time <= l_rounded_scan_limit_time) {
            return true;

        } else {
            //Display scan limit time on warning message box
            CString l_display_scan_limit_time = _T("");

            int l_timehour = l_rounded_scan_limit_time / 3600;
            const int l_nscan_time_format = 9;

            if (l_timehour > l_nscan_time_format) {
                l_display_scan_limit_time.Format(_T("%02d:%02d:%02d"), (l_rounded_scan_limit_time) / 3600, (l_rounded_scan_limit_time % 3600) / 60, l_rounded_scan_limit_time % 60);

            } else if (l_timehour <= 9 && l_timehour > 0) {
                l_display_scan_limit_time.Format(_T("%01d:%02d:%02d"), (l_rounded_scan_limit_time) / 3600, (l_rounded_scan_limit_time % 3600) / 60, l_rounded_scan_limit_time % 60);

            } else {
                l_display_scan_limit_time.Format(_T("%02d:%02d"), (l_rounded_scan_limit_time % 3600) / 60, l_rounded_scan_limit_time % 60);
            }

            CPqm* l_pqm = GetPqm();
            CPQMStudy* l_study = l_pqm->GetStudy();

            if (NULL == l_study) {
                PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                          _T("l_study pointer is NULL"));
                return false;
            }

            CString l_limt_scan_time_error_string = _T("");
            l_limt_scan_time_error_string.Format(CPqmUtilities::
                                                 GetMultiLingualString(_T("IDS_LIMIT_SCAN_TIME_FOR_HW_PROTECTION")), l_display_scan_limit_time);

            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->
                                                  DisplayMessageinWarningDialog(l_limt_scan_time_error_string,
                                                          _T("PQM"), SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKONLYBUTTON);

            if ((l_response == MPlusErrResponse_OK)) {
                m_scan_procedure->ResetCurrentRequest();
                l_study->SetScanModeOfHead();
                l_study->SetAcqStatus(m_current_protocol, C_PR_STATUS_WAIT);
                m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                m_pqmptr->GetAMB()->PqmPmAbortAmb();
                return false;
            }
        }
    }

    return true;
}
//****************************Method Header************************************
//Method Name   :GetProtocolMaxScanTimeFlag()
//Author        :iGATE\Varun
//Purpose       :Get max scan time flag from varfield.
//*****************************************************************************
bool CPqmPmAcqman::GetProtocolMaxScanTimeFlag(
    const int f_current_protocol,
    int32_t* f_temp_max_scan_time_flag) const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetProtocolMaxScanTimeFlag");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace of GetProtocolMaxScanTimeFlag"));

    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

    if (!l_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vfdb_handle is NULL"));
        return false;
    }

    int				l_num_done = 0, l_num = 0;
    int				l_status = E_ERROR;
    VfFieldSpec_t	l_fieldSpec[NUM_1] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_current_protocol;


    VFF_SET_ARGS(l_fieldSpec[l_num],
                 SVN_PLN_TEMP_MAX_SCAN_TIME_FLAG,
                 SVT_PLN_TEMP_MAX_SCAN_TIME_FLAG,
                 sizeof(f_temp_max_scan_time_flag),
                 f_temp_max_scan_time_flag,
                 0);

    l_num++;

    l_status = l_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec,
                                        l_num, &l_num_done);

    if ((E_ERROR == l_status) || (l_num_done != l_num)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan time flag Tag is not present in study file"));
        return false;
    }

    return true;

}
//****************************Method Header************************************
//Method Name   :GetProtocolScanTimeData()
//Author        :iGATE\Varun
//Purpose       :Get scan total time, max scan time, max scan time ratio from varfield.
//*****************************************************************************
bool CPqmPmAcqman::GetProtocolScanTimeData(
    const int f_current_protocol,
    ProtocolScanTimeData_t* f_protScanTime_data) const
{

    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetProtocolScanTimeData");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace of GetProtocolScanTimeData"));

    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();

    if (!l_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vfdb_handle is NULL"));
        return false;
    }

    int				l_num_done = 0, l_num = 0;
    int				l_status = E_ERROR;
    VfFieldSpec_t	l_fieldSpec[NUM_3] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_current_protocol;


    VFF_SET_ARGS(l_fieldSpec[l_num],
                 SVN_PLN_TOTAL_TIME,
                 SVT_PLN_TOTAL_TIME,
                 sizeof(f_protScanTime_data->scan_total_time),
                 &f_protScanTime_data->scan_total_time,
                 0);
    l_num++;

    VFF_SET_ARGS(l_fieldSpec[l_num],
                 SVN_PLN_TEMP_MAX_SCAN_TIME,
                 SVT_PLN_TEMP_MAX_SCAN_TIME,
                 sizeof(f_protScanTime_data->num_temp_max_scan_time),
                 &f_protScanTime_data->num_temp_max_scan_time,
                 0);
    l_num++;

    VFF_SET_ARGS(l_fieldSpec[l_num],
                 SVN_PLN_TEMP_MAX_SCAN_TIME_RATIO,
                 SVT_PLN_TEMP_MAX_SCAN_TIME_RATIO,
                 sizeof(f_protScanTime_data->temp_max_scan_time_ratio),
                 &f_protScanTime_data->temp_max_scan_time_ratio,
                 0);
    l_num++;

    l_status = l_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec,
                                        l_num, &l_num_done);

    if ((E_ERROR == l_status) || (l_num_done != l_num)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan time Tags are not present in study file"));
        return false;
    }

    return true;
}

//*******************************Method Header*********************************
//Method Name    :AcqmanAbort()
//Author         :PATNI/HAR - SS
//Purpose        :
//*****************************************************************************
void CPqmPmAcqman::AcqmanAbort(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AcqmanAbort");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace of AcqmanAbort"));


    //  pqmUiContrastInjectManaged(FALSE);
    //    if (tableMoving) {
    //
    //      DB_PRINTF("A",(">>>> Move couch is aborted!\n"));
    //      PqmUiTableAbort();
    //      PqmAcqClose();
    //      PQM_SET_SELECTED_STATUS();
    //      return;
    //    }
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections


    if (m_scan_procedure == NULL) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return;
    }

    CScopedLock l_scoped_lock(m_pqmptr->GetStudy()->GetScanMutex());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

    CVoiceManager::GetInstance()->QuitVoiceManager();

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    m_scan_procedure->SetScanStart(PQM_SCAN_START);
    CPqmProbe* pqm_probe = m_scan_procedure->GetPqmProbe();			//Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (pqm_probe) {													//Patni-MJC/2009Aug17/Modified/cpp test corrections
        pqm_probe->RefProbeScanEnd(); //RMC
    }

    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();

    // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
    if (curr_protocol != NULL/* && curr_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT*/) {	//Ashish SM4 Abort MsgDisplay multiple time
        //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
        curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);

        //+ Patni-Manish//2009Apr04//Added//PSP1-352
        //bool l_bFlagWaitForVoice = curr_protocol->GetFlagWaitForVoice();
        //+Patni-PJS/2009Sep2/Added/Internal defect
        /*if(FALSE == l_bFlagWaitForVoice)
        {
           //Do nothing...
        } *//* AUTO_VOICE */
        /*else*/
        //-Patni-PJS/2009Sep2/Added/Internal defect

        if (curr_protocol->GetExeAutoVoice() || curr_protocol->GetPlnAutoVoice()) {
            //+Patni-DKH/2009Nov11/Added/Auto Voice Defect Fix - MVC004942,MVC004366

            //Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
            //CVoiceManager::GetInstance()->SafeExit();

            //+Patni-DKH/2009Dec08/Added/Auto Voice Defect Fix - MVC005845 Code review
            //Sleep(1000);
            //-Patni-DKH/2009Nov11/Added/Auto Voice Defect Fix - MVC004942,MVC004366
            CVoiceManager::GetInstance()->CmdPlay(PQM_AV_VOICE_5, TRUE);
            //CVoiceManager::GetInstance()->AvSafeExit();
            //Patni-PJS/2009Aug25/Added/MVC004366
            //Patni-DKH/2009Nov11/Commented/Auto Voice Defect Fix - MVC004942,MVC004366
            //CVoiceManager::GetInstance()->SafeExit();
            m_scan_procedure->SetWaitForVoice(FALSE);
            curr_protocol->SetFlagWaitForVoice(FALSE);
        }

        //-Patni-Manish//2009Apr04//Added//PSP1-352

        if ((m_last_trans->GetScanProcedure()->GetProcessing() & PQM_ACQ_BIT_ACQUISITION) ||
            //Patni-HAR/2010Oct28/Added/V1.30#TMSC-REDMINE-860
            curr_protocol->GetSeriesNumInDbFlag()) {
            //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
            m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_FAIL);
            //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock

            // + Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB
            /* PostThreadMessage(CPQMIPCManager::dThreadId,MSG_UPDATE_ACQ_STATUS_IN_DB,
            (WPARAM)(LPCTSTR)curr_protocol->GetSeriesLOID(),
            (LPARAM)curr_protocol->GetAcqStatus());

            WaitForSingleObject(CPQMIPCManager::m_ThreadEvent,INFINITE);*/

            CString l_series_loid = curr_protocol->GetSeriesLOID();

            const BOOL l_postmsg_status = m_pqmptr->GetDbsaStudyMgr()->
                                          SetAcquisitionStatusInLocalDB((BSTR)l_series_loid.AllocSysString(),
                                                  curr_protocol->GetAcqStatus());


            // - Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB

            //+Patni-HEMANT/ADDED On 6/30/2009 6:56:43 PM/ ACE-2 / WFDA
            //QTS#38 :sending scan fail message to InScan server just before changing the GUI status.
            //Patni-PJS/2010Dec10/Modified/IR-97
            //m_pqmptr->ProcessScanRelatedMsgForWFDA(curr_protocol, PQM_MSG_ABORT_FAIL);
            //-Patni-HEMANT/ ADDED On6/30/2009 6:57:26 PM/ ACE-2 / WFDA

            m_pqmptr->WriteToUI(PQM_MSG_SR_ABORT_FAIL);//meghana here for bug fixing
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_FAIL); //L"ABORT_FAIL"

            /// + Sarjeet
            //				PostThreadMessage(CPQMIPCManager::dThreadId,MSG_SETSERISENO_IN_DB,(WPARAM)(LPCTSTR)curr_protocol->GetSeriesLOID(),(LPARAM)curr_protocol->GetAcqStatus());
            //				WaitForSingleObject(CPQMIPCManager::m_ThreadEvent,INFINITE);
            //+Patni-PJS/2009Dec15/Added/MVC006256

            if (!curr_protocol->GetSeriesNumInDbFlag()) {
                int l_status = m_pqmptr->GetDbsaStudyMgr()->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

                if (l_status == E_ERROR) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to set series in DB"));

                } else {
                    curr_protocol->SetSeriesNumInDbFlag(TRUE);
                }
            }

            //-Patni-PJS/2009Dec15/Added/MVC006256

            //-Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
            // - Sarjeet
            m_scan_procedure->SetIsScanning(false);


            //+ Patni-MSN/Added On 22March2009//PSP1#284 Abort from AcqMan APC retry

        } else if (m_scan_procedure->GetAPCRetry()) {
            m_scan_procedure->SetScanMode(PQM_APC_SCAN);
            m_scan_procedure->SetAPCRetry(FALSE);
            m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
            m_pqmptr->WriteToUI(PQM_MSG_APC_RETRY_WAIT);
            m_scan_procedure->SetIsScanning(false);
            //- Patni-MSN/Added On 22March2009//PSP1#284 Abort from AcqMan APC retry

        } else if (m_scan_procedure->GetIsScanning()) { //Patni/MP/28-Sep-10/TMSC-REDMINE-790
            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            CpqmCDS* l_pqm_cds = m_pqmptr->GetCDS();
            CPQMStudy* l_study = m_pqmptr->GetStudy();
            CPqmCoilData* l_coil_data = m_pqmptr->GetDataManager()->GetPQMCoilData();
            CVarFieldHandle* l_vf_handle = l_study->GetVfDBHandle();

            if (!l_vf_handle) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
                return;
            }

            if (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE) {
                /* If PreScan is aborted during the CDS is not complete */
                curr_protocol->GetCDSInfo()->m_cds_step	  = CDS_STEP0_NONE;
                curr_protocol->GetCDSInfo()->m_cds_rescan   = TRUE;

            } else if (curr_protocol->GetCDSInfo()->m_cds_step >= CDS_STEP4_SCAN) {
                // CDS state is not automatic configuration of the start of the scan coils
                //CDS that during the scan back to the original WB coil because the coil is set

                l_pqm_cds->PqmCdsReturnCoil(curr_protocol->GetSaturnCoil());
                l_study->PutSaturnCoil(l_vf_handle, curr_protocol->GetProtocol(), curr_protocol->GetSaturnCoil());

                if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(curr_protocol->GetSaturnCoil(), curr_protocol->GetCoilDBTable())) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
                }

                l_study->PutConnectedCoil(l_vf_handle, curr_protocol->GetProtocol(), curr_protocol);
                l_study->PutRecvrCoil(l_vf_handle, curr_protocol->GetProtocol(), curr_protocol);

                //Coil name
                if (!m_pqmptr->GetCoilSelectionTool()->CSSetCoilName(curr_protocol, l_vf_handle)) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
                }

                //update GUI with for coil change
                m_pqmptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);
            }

            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            m_pqmptr->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
            m_pqmptr->WriteToUI(PQM_MSG_SR_ABORT_WAIT);  //meghana here for bug fixing
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT); // L"ABORT_WAIT"
            m_scan_procedure->SetIsScanning(false);
        }

        if (m_scan_procedure->GetIsScanning()) {
            AcqDeleteLockFile();
        }

        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_SCAN_ABORTED, _T("IDS_ERR_SCAN_ABORTED"),
            SCAN_ERR_NOTIFY,
            L"PqmPmAcqman.cpp");
    }

    // +Patni- SS/2009/ Changed/PSP1- CFARetry
    // below code has sifted for CFARetry dialog close on ScanAbort
    // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
    if (!m_scan_procedure->GetIsScanning() && m_scan_procedure->GetCFARetry()) {

        m_pqmptr->SetPostProcessFlag(FALSE);
        m_pqmptr->WriteToUI(PQM_MSG_CFA_PRESCAN_ABORT);

        if (PQM_CFA_RETRY_AND_SCAN == m_scan_procedure->GetCFARetry()) {

            m_pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
        }

    }

    // -Patni- SS/2009/ Changed/PSP1- CFARetry
    if (m_scan_procedure->GetCFARetry()) {

        //PqmAcqCFAClose();
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_SCAN_ABORTED, _T("IDS_ERR_SCAN_ABORTED"),
            SCAN_ERR_NOTIFY,
            L"PqmPmAcqman.cpp");
        m_scan_procedure->SetScanMode(PQM_CFA_SCAN);

        m_scan_procedure->SetCFARetry(PQM_CFA_NONE);
    }


    if (m_scan_procedure->GetProbing() == TRUE) {

        //pqmAcqErrProbing(NULL,NULL,NULL) ;
        m_pqmptr->GetPqmProCon()->StopProlim();
        m_scan_procedure->SetScanMode(PQM_RMC_SCAN);
        m_scan_procedure->SetProbing(FALSE);
        AcqDeleteLockFile();           /* lock file delete */
    }

    //Patni-MP/2010-Jan-29/Added/IDS754/MVC006963
    m_scan_procedure->ResetCurrentRequest();
    m_pqmptr->GetStudy()->SetScanModeOfHead();
    //+ Patni-MSN/2009Sept02/Added/MVC004457
    m_pqmptr->UpdateSARInfoOnPage();
    //- Patni-MSN/2009Sept02/Added/MVC004457

    m_scan_procedure->SetScanMode(PQM_PROTOCOL_SCAN);

    //m_scan_procedure->SetScanStart(NULL);
    //PqmAcqClose();
    //PQM_SET_SELECTED_STATUS();
    m_pqmptr->GetAMB()->PqmPmAbortAmb();
}

//*******************************Method Header*********************************
//Method Name    :AcqDeleteLockFile
//Author         :PATNI/LK
//Purpose        :This function deletes the lock file
//*****************************************************************************
int CPqmPmAcqman::AcqDeleteLockFile(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AcqDeleteLockFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcqDeleteLockFile"));

    int     l_status = E_NO_ERROR;
    FILE*   l_fp_lockfile = NULL;
    char    l_filecontent_buff[256] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (m_scan_lockfile == NULL) {
        m_scan_lockfile = AcqGetLockFileName();

    }

    if (!WNP_access(m_scan_lockfile, 0)) {
        if (!(l_fp_lockfile = fopen(m_scan_lockfile, "r"))) {
            l_status = E_ERROR;

        } else {
            fgets(l_filecontent_buff, sizeof(l_filecontent_buff), l_fp_lockfile);

            if (strlen(l_filecontent_buff) && !strcmp(l_filecontent_buff, LOCK_FILE)) {

                fclose(l_fp_lockfile);
                _unlink(m_scan_lockfile);
                return E_NO_ERROR;

            } else {
                l_status = E_ERROR;
            }

            fclose(l_fp_lockfile);
        }
    }

    return  l_status;
}

//****************************Method Header********************************
//Method Name   :SendMessage()
//Author        :PATNI/MRP
//Purpose       :This method will Translate the Transaction object sent
//               as a parameter to IMplusMessage, connect to Acqman. If
//               a synchronous message needs to be sent SendSync is called
//               else sendAsyc is called.
//*************************************************************************
BOOL CPqmPmAcqman::SendMessage(
    CScanTransaction* scantransaction,
    bool synchronous
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::SendMessage");

    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(synchronous);

    BOOL l_result = FALSE;

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of CPqmPmAcqman::SendMessage"));

    if (scantransaction == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("scantransaction == NULL"));
        return l_result;
    }

    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    PLBYTE message_to_send = NULL;
    int size_header = 0 ;
    int size_data = 0;
    int size = 0;
    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    m_last_trans = scantransaction;
    AM_ClientMessageHeader_t l_Header = scantransaction->GetHeader();

    l_Header.client = ACQUISITION;
    l_Header.version_number = AM_VERSION_NUMBER;

    AM_ClientToAcqMan_unicode_t l_data = scantransaction->GetData();

    // - SM4 Himanshu CFA Fix
    if (!CPQMConfig::GetInstance()->IsUniCodeSupportedAcqMan()) {

        l_Header.byte_count = sizeof(AM_ClientToAcqMan_t);
        AM_ClientToAcqMan_t l_AcqMandata;


        memset(l_AcqMandata.study_file, 0, sizeof(l_AcqMandata.study_file));
        wcstombs(l_AcqMandata.study_file, l_data.study_file, sizeof(l_AcqMandata.study_file));

        ConvertClientMsgToAscii(l_Header, l_data, &l_AcqMandata);

        l_AcqMandata.protocol_node = l_data.protocol_node;

        // V5.00 for client kind for rbcontrol
        l_AcqMandata.clientKind =  ACQ_CLIENT_PQM;

        // Not Need AcqMan window
        l_AcqMandata.normalize = TRUE;

        //Set body in the custom part of Message.
        size_header = sizeof(l_Header);
        size_data = sizeof(l_AcqMandata);
        size = size_header + size_data;

        message_to_send = new pbyte[sizeof(pbyte) * size];
        memcpy(message_to_send, &l_Header, size_header);
        memcpy(message_to_send + size_header, &l_AcqMandata, size_data);
        //LogHeaderAndData(l_Header,l_data);

    } else {

        l_Header.byte_count = sizeof(AM_ClientToAcqMan_unicode_t);
        // Not Need AcqMan window
        l_data.normalize = TRUE;

        // V5.00 for client kind for rbcontrol
        l_data.clientKind = ACQ_CLIENT_PQM ;

        //Set body in the custom part of Message.
        size_header = sizeof(l_Header);
        size_data = sizeof(l_data);
        size = size_header + size_data;

        message_to_send = new pbyte[sizeof(pbyte) * size];
        memcpy(message_to_send, &l_Header, size_header);
        memcpy(message_to_send + size_header, &l_data, size_data);
        // LogHeaderAndData(l_Header,l_data);
    }

    CString s_str(L"\r\n\t\t*** Function : ");
    s_str += GetStr(l_Header.function);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, s_str);

    //Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("%s Message Sent to Acqman"), GetStr(l_Header.function));

    CSocketMsg message(message_to_send, size);

    //Patni-SS-RG/2010Mar05/Added/MSA00251-00251
    if (m_acqman_socket && m_pqmptr->GetManagerPtr()->GetIsAcqmanConnected()) {
        l_result = m_acqman_socket->SendData(message);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Data sent to Acqman"));
    }

    if (!l_result) {

        //+Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_ERR_CONNECT_ACQMAN,
                ERROR_LOG_CRUCIAL1 , _T("PqmPmAcqman.cpp"));
        		*/
        //-Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));
    }

    //MRPdefect812
    int func_bit = GetFunctionBit(l_Header.function);
    func_bit |= m_scan_procedure->GetRequest();
    m_scan_procedure->SetRequest(func_bit);
    //MRPdefect812
    DEL_PTR_ARY(message_to_send);
    // + LOKESH INTGR 5 Bug Fix
    //    m_pqmptr->WriteToUI(PQM_MSG_SENDING_COMPLETED);
    // - LOKESH INTGR 5 Bug Fix
    return l_result;
}

//*******************************Method Header*********************************
//Method Name    :ConnectToAcqman()
//Author         :PATNI/LK
//Purpose        :Creates a new socket and connect to the AcqMan on the IP address
//                  and Port number mentioned in IPCConfig.ini
//*****************************************************************************
bool CPqmPmAcqman::ConnectToAcqman(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::ConnectToAcqman");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::ConnectToAcqman"));

    //+Patni-ARD/2010Mar24/Modified/Ph-3# IR-100
    char service_name[100] = {0};
    memset(service_name, NULL, sizeof(char) * 100);
    strcpy(service_name, "Acqman");

    //l_server_ip  = CPQMConfig::GetInstance()->GetAcqManIP();
    CString l_server_ip = m_pqmptr->GetAcqmanIP();


    WSADATA l_wsaData;
    WSAStartup(MAKEWORD(2, 2), &l_wsaData);

    struct servent* l_pqm_serviceP = getservbyname(service_name, "tcp");

    CString l_str_port(_T(""));
    bool l_result = false;

    if (l_pqm_serviceP == NULL) {
        l_result = false;

    } else {
        l_str_port.Format(_T("%d"), htons(l_pqm_serviceP->s_port));
    }

    //+Patni-ARD/2010APR14/Added/Ph-3# IR-100 Acqman port empty in service
    if (l_str_port.Compare(_T("")) == 0) {
        l_str_port = _T("5500");
    }

    //-Patni-ARD/2010APR14/Added/Ph-3# IR-100 Acqman port empty in service

    //l_str_port = CPQMConfig::GetInstance()->GetAcqManPort();
    //-Patni-ARD/2010Mar24/Modified/Ph-3# IR-100
    //+Patni-PJS/2009May10/Modify/IR-82 review comment
    if (l_server_ip.GetLength() <= 0 || l_str_port.GetLength() <= 0) {
        /*CPQMLogMgr::GetInstance()->WriteEventLog(_T("IDS_NO_READ_VALUE"),
                ERROR_LOG_CRUCIAL1,
                _T("PqmPmAcqman.cpp"));*/
        CString l_error_str = _T("");
        l_error_str.LoadString(IDS_NO_READ_VALUE);
        CPQMLogMgr::GetInstance()->WriteEventLog(l_error_str,
                ERROR_LOG_CRUCIAL1,
                _T("PqmPmAcqman.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        //-Patni-PJS/2009May10/Modify/IR-82 review comment


        CPQMLogMgr::GetInstance()->DisplayError(
            //	_T("InValid Values to connect with AcqMan"), // Added By KT/ACE-2/15-06-2009/Multilingualisation
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_VALUES_TO_CONNECT_WITH_ACQMAN")),
            _T("PqmPmAcqman.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);
        return l_result;
    }

    if (!m_acqman_socket) {
        m_acqman_socket = new CPQMSocket;
    }

    m_acqmannotifier = new CAcqManNotifier(this);

    if (! m_acqman_socket->Create(m_acqmannotifier)) {
        return l_result;
    }

    if (m_acqman_socket->Connect(l_server_ip, l_str_port)) {
        m_pqmptr->WriteToUI(PQM_MSG_ACQMAN_CONNECTED); // let GUI to know Acqman is connected;
        l_result  = true;
    }

    //Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqLogOpen();

    return l_result ;
}

//*******************************Method Header*********************************
//Method Name    :AutoScanRetry
//Author         :PATNI/LK
//Purpose        :This function retries the scan
//*****************************************************************************
void CPqmPmAcqman::AutoScanRetry(CPqmProtocol* f_pqm_protocol/* = NULL*/
                                )
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AutoScanRetry");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AutoScanRetry"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmProtocol* l_protocol = m_pqmptr->GetStudy()->GetTopWaitProtocol();

    //+Patni-AMT/2009Jun16/Added/JFT# MVC2941 Code Review
    if (NULL != f_pqm_protocol) {
        l_protocol = f_pqm_protocol;
    }

    if (NULL != l_protocol) {
        ScanStart(l_protocol, TRUE);
    }//-Patni-AMT/2009Jun16/Added/JFT# MVC2941 Code Review
}

//+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
//**************************************************************************
//Method Name   : pqmAcqResetCopySourceProt
//Author        : PATNI\Manish
//Purpose       :
//**************************************************************************
void CPqmPmAcqman::pqmAcqResetCopySourceProt(CVarFieldHandle* const vf_handle,
        CPqmProtocol* l_pstartobj)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::pqmAcqResetCopySourceProt");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(vf_handle);
    UNREFERENCED_PARAMETER(l_pstartobj);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution

    BITFLD_DECL(mask, MAX_NODES);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Inside function pqmAcqResetCopySourceProt"));

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    CPQMStudy* l_pqmstudy = m_pqmptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return ;
    }

    protocolvec l_protocolvector;
    l_protocolvector = l_pqmstudy->GetProtocolvector();
    protocolvec::iterator v1_Iter  = NULL;

    CPqmProtocol* l_pqmprotocol = NULL;
    int l_resetvalue = -1;

    for (v1_Iter = l_protocolvector.begin();
         v1_Iter != l_protocolvector.end();
         v1_Iter++) {
        l_pqmprotocol = (CPqmProtocol*) * v1_Iter;

        if (l_pqmprotocol == NULL || l_pqmprotocol->GetProtocol() == PQ_FILLER) {
            continue;

        } else {
            l_pqmprotocol->SetCopySourceProt(l_resetvalue);
            l_pqmstudy->SetProtocolValues(l_pqmstudy->GetVfDBHandle(),
                                          l_pqmprotocol->GetProtocol(),
                                          mask,
                                          K_PR_COPY_SOURCE_PROTOCOL,
                                          &l_resetvalue,
                                          sizeof(int),
                                          NULL);

        }
    }

    return;
}
//-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

//************************************Method Header************************************
// Method Name  : PqmAcqAutoCoilSet
// Author       : PATNI/ HEMANT
// Purpose      : performs the auto coil
//***********************************************************************************
status_t CPqmPmAcqman::PqmAcqAutoCoilSet(
    CPqmProtocol* f_cds_protocol
)
{
    if (m_pqmptr) {
        CPQMStudy* l_study = m_pqmptr->GetStudy();

        if (m_scan_procedure && l_study) {
            return m_scan_procedure->PqmAcqAutoCoilSet(l_study->GetVfDBHandle(), f_cds_protocol);
        }
    }

    return E_ERROR ;
}


//*******************************Method Header*********************************
//Method Name    :CanAutoScanStart()
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
BOOL CPqmPmAcqman::CanAutoScanStart(
    const int f_topprotocol
)const
{

    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CanAutoScanStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPqm* l_pqm = GetPqm();

    if (NULL == l_pqm) {
        return FALSE;
    }

    l_pqm->WriteToUI(PQM_MSG_CAN_AUTO_SCAN_START);

    if (l_pqm->GetFirstWaitProtForAutoScan() != f_topprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GUI and IPC acqorder of AutoScan Protocol are NOT Same"));
        return FALSE;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GUI and IPC acqorder of AutoScan Protocol are Same"));

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :CheckDelayAndScanStartForFirstWaitProtocol()
//Author         :PATNI/AKR
//Purpose        :This function will check for delay time and start scan accordingly
//*****************************************************************************
void CPqmPmAcqman::CheckDelayAndScanStartForFirstWaitProtocol(CPqmProtocol* f_top_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CheckDelayAndScanStartForFirstWaitProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_top_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_top_protocol is NULL"));
        return;
    }

    if (CheckDelayTimeForTopProtocol(f_top_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckDelayTimeForTopProtocol returns true, waiting for delay time to elapse"));
        //Note: Do not remove auto scan
        return;
    }

    if (m_pqmptr->GetAMB()->PqmAMBScanSeg2Active())
        m_pqmptr->WriteToUI(PQM_MSG_AMB_AUTO_SCAN_START_FOR_FIRST_SLAVE_PROTOCOL);

    else
        m_pqmptr->WriteToUI(AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL);

}


//*******************************Method Header*********************************
//Method Name    :AutoScanTimerReset()
//Author         :PATNI/PVN
//Purpose        :This method is called to Reset timer
//*****************************************************************************
void CPqmPmAcqman::AutoScanTimerReset()
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CPqmPmAcqman::AutoScanTimerReset"), _T("CPqmPmAcqman::AutoScanTimerReset"));

    if (-1 != m_waiting_for_auto_scan_acq_order) {
        WNP_alarm(0) ;		 //TIMER RESET
        WNP_signal(SIGALRM, SIG_DFL) ;
        m_waiting_for_auto_scan_acq_order = -1;
    }
}


void CPqmPmAcqman::AcqmanAbortFromnMainThread(
)
{

    if (m_acqmannotifier) {
        m_acqmannotifier->AcqmanAbortFromnMainThread();
    }
}

void CPqmPmAcqman::AcqmanAbortFromScanThread(
)
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CPqmPmAcqman::AcqmanAbortFromScanThread"), _T("CPqmPmAcqman::AcqmanAbortFromScanThread"));
    m_scan_procedure->AcquisitionScanAbort();
}

//*******************************Method Header*********************************
//Method Name    :GetStr()
//Author         :PATNI/HAR
//Purpose        :This Function Returns string for the requested int value
//*****************************************************************************
CString CPqmPmAcqman::GetStr(
    const int func
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetStr");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::GetStr"));

    CString str = _T("");

    switch (func) {
        case AMCMM_UNKNOWN:                                 //  0
            str = _T("AMCMM_UNKNOWN");
            break;

        case CONNECTION_GRANTED:
            str = _T("CONNECTION_GRANTED");
            break;

        case CONNECTION_DENIED:
            str = _T("CONNECTION_DENIED");
            break;

        case AMCM_ABORT:                                    //  9
            str = _T("AMCM_ABORT");
            break;

        case AMCMM_PROTOCOL:                                //  10
            str = _T("AMCMM_PROTOCOL");
            break;

        case AMCMM_PRESCAN:                                 //  11
            str = _T("AMCMM_PRESCAN");
            break;

        case AMCMM_PRESCAN_CONTINUE:                        //  12
            str = _T("AMCMM_PRESCAN_CONTINUE");
            break;

        case AMCMM_ACQUISITION:                             //  13
            str = _T("AMCMM_ACQUISITION");
            break;

        case AMCMM_ACQUISITION_CONTINUE:                    //  14
            str = _T("AMCMM_ACQUISITION_CONTINUE");
            break;

        case AMCMM_FLUORO:                                  //  15
            str = _T("AMCMM_FLUORO");
            break;

        case AMCMM_FLUORO_CONTR:                            //  16
            str = _T("AMCMM_FLUORO_CONTR");
            break;

        case AMCMM_PREP_SCAN:                               //  17
            str = _T("AMCMM_PREP_SCAN");
            break;

        case AMCMM_MO_MONITOR:                              //  18
            str = _T("AMCMM_MO_MONITOR");
            break;

        case AMCMM_MO_MONITOR_ABORT:                        //  19
            str = _T("AMCMM_MO_MONITOR_ABORT");
            break;

        case AMCMM_MO_CORRECT:                              //  20
            str = _T("AMCMM_MO_CORRECT");
            break;

        case AMCMM_AMB_SCAN:                                //  21
            str = _T("AMCMM_AMB_SCAN");
            break;

        case AMCMM_CONTROL:                                 //  30
            str = _T("AMCMM_CONTROL");
            break;

        case AMCMM_GATEMODE:                                //  31
            str = _T("AMCMM_GATEMODE");
            break;

        case AMCMM_MOVECOUCH:                               //  32
            str = _T("AMCMM_MOVECOUCH");
            break;

        case AMCMM_REMOTE_MOVECOUCH:                        //  33
            str = _T("AMCMM_REMOTE_MOVECOUCH");
            break;

        case AMCMM_RFINTERLOCK:                             //  34
            str = _T("AMCMM_RFINTERLOCK");
            break;

        case AMCMM_SHIMCTRL:                                //  35
            str = _T("AMCMM_SHIMCTRL");
            break;

        case AMCMM_RSBCALIBRATE:                            //  36
            str = _T("AMCMM_RSBCALIBRATE");
            break;

        case AMCMM_DISPLAYMSG:                              //  37
            str = _T("AMCMM_DISPLAYMSG");
            break;

        case AMCMM_ECG_ELECTRODE:                           //  38
            str = _T("AMCMM_ECG_ELECTRODE");
            break;

        case AMCMM_HELIUM_LEVEL:                            //  50
            str = _T("AMCMM_HELIUM_LEVEL");
            break;

        case AMCMM_SET_ENVIRON:
            str = _T("AMCMM_SET_ENVIRON");                  //  70
            break;

        case AMCMM_STATUSCOPY:                              //  71
            str = _T("AMCMM_STATUSCOPY");
            break;

        case AMCMM_STATUSCLEAR:                             //  72
            str = _T("AMCMM_STATUSCLEAR");
            break;

        case AMCMM_ACQINFO:                                 //  73
            str = _T("AMCMM_ACQINFO");
            break;

        case AMCMM_TRACE:                                   //  74
            str = _T("AMCMM_TRACE");
            break;

            //+Patni-MSN/2009Mar10/Modified/Optimization
        case AMCMM_PROBE_CHANGE_THRESH:
            str = _T("AMCMM_PROBE_CHANGE_THRESH");
            break;
            //-Patni-MSN/2009Mar10/Modified/Optimization

            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMM_MRS_PRESCAN:
            str = (_T("AMCMM_MRS_PRESCAN"));
            break;

            // - Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMR_UNKNOWN:                                 //  4096
            str = _T("AMCMR_UNKNOWN");
            break;

        case AMCMR_PROTOCOL:                                //  4106
            str = _T("AMCMR_PROTOCOL");
            break;

        case AMCMR_PRESCAN:                                 //  4107
            str = _T("AMCMR_PRESCAN");
            break;

        case AMCMR_PRESCAN_CONTINUE:                        //  4108
            str = _T("AMCMR_PRESCAN_CONTINUE");
            break;

        case AMCMR_ACQUISITION:                             //  4109
            str = _T("AMCMR_ACQUISITION");
            break;

        case AMCMR_ACQUISITION_CONTINUE:                    //  4110
            str = _T("AMCMR_ACQUISITION_CONTINUE");
            break;

        case AMCMR_FLUORO:                                  //  4111
            str = _T("AMCMR_FLUORO");
            break;

        case AMCMR_FLUORO_CONTR:                            //  4112
            str = _T("AMCMR_FLUORO_CONTR");
            break;

        case AMCMR_PREP_SCAN:                               //  4113
            str = _T("AMCMR_PREP_SCAN");
            break;

        case AMCMR_MO_MONITOR:                              //  4114
            str = _T("AMCMR_MO_MONITOR");
            break;

        case AMCMR_MO_MONITOR_ABORT:                        //  4115
            str = _T("AMCMR_MO_MONITOR_ABORT");
            break;

        case AMCMR_MO_CORRECT:                              //  4116
            str = _T("AMCMR_MO_CORRECT");
            break;

        case AMCMR_AMB_SCAN:                                //  4117
            str = _T("AMCMR_AMB_SCAN");
            break;

        case AMCMR_CONTROL:                                 //  4126
            str = _T("AMCMR_CONTROL");
            break;

        case AMCMR_GATEMODE:                                //  4127
            str = _T("AMCMR_GATEMODE");
            break;

        case AMCMR_MOVECOUCH:                               //  4128
            str = _T("AMCMR_MOVECOUCH");
            break;

        case AMCMR_REMOTE_MOVECOUCH:                        //  4129
            str = _T("AMCMR_REMOTE_MOVECOUCH");
            break;

        case AMCMR_RFINTERLOCK:                             //  4130
            str = _T("AMCMR_RFINTERLOCK");
            break;

        case AMCMR_SHIMCTRL:                                //  4131
            str = _T("AMCMR_SHIMCTRL");
            break;

        case AMCMR_RSBCALIBRATE:                            //  4132
            str = _T("AMCMR_RSBCALIBRATE");
            break;

        case AMCMR_DISPLAYMSG:                              //  4133
            str = _T("AMCMR_DISPLAYMSG");
            break;

        case AMCMR_ECG_ELECTRODE:                           //  4134
            str = _T("AMCMR_ECG_ELECTRODE");
            break;

        case AMCMR_HELIUM_LEVEL:                            //  4146
            str = _T("AMCMR_HELIUM_LEVEL");
            break;

        case AMCMR_SET_ENVIRON:                             //  4166
            str = _T("AMCMR_SET_ENVIRON");
            break;

        case AMCMR_STATUSCOPY:                              //  4167
            str = _T("AMCMR_STATUSCOPY");
            break;

        case AMCMR_STATUSCLEAR:                             //  4168
            str = _T("AMCMR_STATUSCLEAR");
            break;

        case AMCMR_ACQINFO:                                 //  4169
            str = _T("AMCMR_ACQINFO");
            break;

        case AMCMR_TRACE:                                   //  4170
            str = _T("AMCMR_TRACE");
            break;

        case AMCMR_PROBE_CHANGE_THRESH:                   //  12321
            str = _T("AMCMR_PROBE_CHANGE_THRESH");
            break;

            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMR_MRS_PRESCAN:
            str = (_T("AMCMR_MRS_PRESCAN"));
            break;

            // - Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMA_UNKNOWN_DONE:                            //  8192
            str = _T("AMCMA_UNKNOWN_DONE");
            break;

        case AMCMA_PROTOCOL_DONE:                           //  8202
            str = _T("AMCMA_PROTOCOL_DONE");
            break;

        case AMCMA_PRESCAN_DONE:                            //  8203
            str = _T("AMCMA_PRESCAN_DONE");
            break;

        case AMCMA_PRESCAN_CONTINUE_DONE:                   //  8204
            str = _T("AMCMA_PRESCAN_CONTINUE_DONE");
            break;

        case AMCMA_ACQUISITION_DONE:                        //  8205
            str = _T("AMCMA_ACQUISITION_DONE");
            break;

        case AMCMA_ACQUISITION_CONTINUE_DONE:               //  8206
            str = _T("AMCMA_ACQUISITION_CONTINUE_DONE");
            break;

        case AMCMA_FLUORO_DONE:                             //  8207
            str = _T("AMCMA_FLUORO_DONE");
            break;

        case AMCMA_FLUORO_CONTR_DONE:                       //  8208
            str = _T("AMCMA_FLUORO_CONTR_DONE");
            break;

        case AMCMA_PREP_SCAN_DONE:                          //  8209
            str = _T("AMCMA_PREP_SCAN_DONE");
            break;

        case AMCMA_MO_MONITOR_DONE:                         //  8210
            str = _T("AMCMA_MO_MONITOR_DONE");
            break;

        case AMCMA_MO_MONITOR_ABORT_DONE:                   //  8211
            str = _T("AMCMA_MO_MONITOR_ABORT_DONE");
            break;

        case AMCMA_MO_CORRECT_DONE:                         //  8212
            str = _T("AMCMA_MO_CORRECT_DONE");
            break;

        case AMCMA_AMB_SCAN_DONE:                           //  8213
            str = _T("AMCMA_AMB_SCAN_DONE");
            break;

        case AMCMA_CONTROL_DONE:                            //  8222
            str = _T("AMCMA_CONTROL_DONE");
            break;

        case AMCMA_GATEMODE_DONE:                           //  8223
            str = _T("AMCMA_GATEMODE_DONE");
            break;

        case AMCMA_MOVECOUCH_DONE:                          //  8224
            str = _T("AMCMA_MOVECOUCH_DONE");
            break;

        case AMCMA_REMOTE_MOVECOUCH_DONE:                   //  8225
            str = _T("AMCMA_REMOTE_MOVECOUCH_DONE");
            break;

        case AMCMA_RFINTERLOCK_DONE:                        //  8226
            str = _T("AMCMA_RFINTERLOCK_DONE");
            break;

        case AMCMA_SHIMCTRL_DONE:                           //  8227
            str = _T("AMCMA_SHIMCTRL_DONE");
            break;

        case AMCMA_RSBCALIBRATE_DONE:                       //  8228
            str = _T("AMCMA_RSBCALIBRATE_DONE");
            break;

        case AMCMA_DISPLAYMSG_DONE:                         //  8229
            str = _T("AMCMA_DISPLAYMSG_DONE");
            break;

        case AMCMA_ECG_ELECTRODE_DONE:                      //  8230
            str = _T("AMCMA_ECG_ELECTRODE_DONE");
            break;

        case AMCMA_HELIUM_LEVEL_DONE:                       //  8242
            str = _T("AMCMA_HELIUM_LEVEL_DONE");
            break;

        case AMCMA_SET_ENVIRON_DONE:                        //  8262
            str = _T("AMCMA_SET_ENVIRON_DONE");
            break;

        case AMCMA_STATUSCOPY_DONE:                         //  8263
            str = _T("AMCMA_STATUSCOPY_DONE");
            break;

        case AMCMA_STATUSCLEAR_DONE:                        //  8264
            str = _T("AMCMA_STATUSCLEAR_DONE");
            break;

        case AMCMA_ACQINFO_DONE:                            //  8265
            str = _T("AMCMA_ACQINFO_DONE");
            break;

        case AMCMA_TRACE_DONE:                              //  8266
            str = _T("AMCMA_TRACE_DONE");
            break;

        case AMCMA_PROBE_CHANGE_THRESH_DONE:                   //  12322
            str = _T("AMCMA_PROBE_CHANGE_THRESH_DONE");
            break;

            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMA_MRS_PRESCAN_DONE:
            str = (_T("AMCMA_MRS_PRESCAN_DONE"));
            break;

            // - Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMS_WAIT_FOR_START:                          //  12288
            str = _T("AMCMS_WAIT_FOR_START");
            break;

        case AMCMS_SUBPROTOCOL_BEGIN:                       //  12289
            str = _T("AMCMS_SUBPROTOCOL_BEGIN");
            break;

        case AMCMS_SUBPROTOCOL_END:                         //  12290
            str = _T("AMCMS_SUBPROTOCOL_END");
            break;

        case AMCMS_AMB_PROTOCOL_END:                        //  12291
            str = _T("AMCMS_AMB_PROTOCOL_END");
            break;

        case AMCMS_PRESCAN_TXCOIL_BEGIN:                    //  12292
            str = _T("AMCMS_PRESCAN_TXCOIL_BEGIN");
            break;

        case AMCMS_PRESCAN_TXCOIL_END:                      //  12293
            str = _T("AMCMS_PRESCAN_TXCOIL_END");
            break;

        case AMCMS_PRESCAN_RXCOIL_BEGIN:                    //  12294
            str = _T("AMCMS_PRESCAN_RXCOIL_BEGIN");
            break;

        case AMCMS_PRESCAN_RXCOIL_END:                      //  12295
            str = _T("AMCMS_PRESCAN_RXCOIL_END");
            break;

        case AMCMS_PRESCAN_CFA_BEGIN:                       //  12296
            str = _T("AMCMS_PRESCAN_CFA_BEGIN");
            break;

        case AMCMS_PRESCAN_CFA_END:                         //  12297
            str = _T("AMCMS_PRESCAN_CFA_END");
            break;

        case AMCMS_PRESCAN_RFL_BEGIN:                       //  12298
            str = _T("AMCMS_PRESCAN_RFL_BEGIN");
            break;

        case AMCMS_PRESCAN_RFL_END:                         //  12299
            str = _T("AMCMS_PRESCAN_RFL_END");
            break;

        case AMCMS_PRESCAN_RXGAIN_BEGIN:                    //  12300
            str = _T("AMCMS_PRESCAN_RXGAIN_BEGIN");
            break;

        case AMCMS_PRESCAN_RXGAIN_END:                      //  12301
            str = _T("AMCMS_PRESCAN_RXGAIN_END");
            break;

        case AMCMS_PRESCAN_PHCOR_BEGIN:                     //  12302
            str = _T("AMCMS_PRESCAN_PHCOR_BEGIN");
            break;

        case AMCMS_PRESCAN_PHCOR_END:                       //  12303
            str = _T("AMCMS_PRESCAN_PHCOR_END");
            break;

        case AMCMS_SCAN_BEGIN:                              //  12304
            str = _T("AMCMS_SCAN_BEGIN");
            break;

        case AMCMS_SCAN_END:                                //  12305
            str = _T("AMCMS_SCAN_END");
            break;

        case AMCMS_SCAN_PAUSE:                              //  12306
            str = _T("AMCMS_SCAN_PAUSE");
            break;

        case AMCMS_SCAN_RESUME:                             //  12307
            str = _T("AMCMS_SCAN_RESUME");
            break;

        case AMCMS_COUCH_POSITION:                          //  12308
            str = _T("AMCMS_COUCH_POSITION");
            break;

        case AMCMS_FLUORO_INFO:                             //  12309
            str = _T("AMCMS_FLUORO_INFO");
            break;

        case AMCMS_DYNAMIC_SCAN_BEGIN:                      //  12310
            str = _T("AMCMS_DYNAMIC_SCAN_BEGIN");
            break;

        case AMCMS_DYNAMIC_SCAN_END:                        //  12311
            str = _T("AMCMS_DYNAMIC_SCAN_END");
            break;

        case AMCMS_VISUALPREP_BEGIN:                        //  12312
            str = _T("AMCMS_VISUALPREP_BEGIN");
            break;

        case AMCMS_VISUALPREP_END:                          //  12313
            str = _T("AMCMS_VISUALPREP_END");
            break;

        case AMCMS_MOVINGBED_SCAN_BEGIN:                    //  12314
            str = _T("AMCMS_MOVINGBED_SCAN_BEGIN");
            break;

        case AMCMS_MOVINGBED_SCAN_END:                      //  12315
            str = _T("AMCMS_MOVINGBED_SCAN_END");
            break;

        case AMCMS_RMSCAN_END:                              //  12316
            str = _T("AMCMS_RMSCAN_END");
            break;

            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMS_MRS_PRESCAN_F0_BEGIN:
            str = _T("AMCMS_MRS_PRESCAN_F0_BEGIN");
            break;

        case AMCMS_MRS_PRESCAN_F0_END:
            str = _T("AMCMS_MRS_PRESCAN_F0_END");
            break;

        case AMCMS_MRS_PRESCAN_REF_BEGIN:
            str = _T("AMCMS_MRS_PRESCAN_REF_BEGIN");
            break;

        case AMCMS_MRS_PRESCAN_REF_END:
            str = _T("AMCMS_MRS_PRESCAN_REF_END");
            break;

        case AMCMS_MRS_PRESCAN_WSAT_BEGIN:
            str = _T("AMCMS_MRS_PRESCAN_WSAT_BEGIN");
            break;

        case AMCMS_MRS_PRESCAN_WSAT_END:
            str = _T("AMCMS_MRS_PRESCAN_WSAT_END");
            break;
            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS

            //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
        case AMCMS_PRESCAN_CDS_BEGIN :
            str = _T("AMCMS_PRESCAN_CDS_BEGIN");
            break ;

        case AMCMS_PRESCAN_CDS_END :
            str = _T("AMCMS_PRESCAN_CDS_END");
            break ;
            //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

            //+Patni-Ravindra Acharya/2010June25/Added/IR-153
        case AMCMM_OLPTEMP_MEASUREMENT :
            str = _T("AMCMM_OLPTEMP_MEASUREMENT");
            break ;

        case AMCMR_OLPTEMP_MEASUREMENT :
            str = _T("AMCMR_OLPTEMP_MEASUREMENT");
            break ;

        case AMCMA_OLPTEMP_MEASUREMENT_DONE :
            str = _T("AMCMA_OLPTEMP_MEASUREMENT_DONE");
            break ;

            //-Patni-Ravindra Acharya/2010June25/Added/IR-153
        case AMCMM_RTSARCTRL:
            str = _T("AMCMM_RTSARCTRL");
            break;

        case AMCMR_RTSARCTRL:
            str = _T("AMCMR_RTSARCTRL");
            break;

        case AMCMA_RTSARCTRL_DONE:
            str = _T("AMCMA_RTSARCTRL_DONE");
            break;


        default :
            str = _T("Unknown Message !!");
            break;
    }

    return str;
}

//****************************Method Header********************************
//Method Name   :GetPqm()
//Author        :PATNI/HAR
//Purpose       :This method returning the pointer of the class CPqm
//*************************************************************************
CPqm* CPqmPmAcqman::GetPqm(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetPqm");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::GetPqm"));

    return m_pqmptr;
}

//****************************Method Header********************************
//Method Name   :GetFunctionBit()
//Author        :PATNI/MRP
//Purpose       :This method will return the function bit based on the response
//               received from Acqman
//*************************************************************************
int CPqmPmAcqman::GetFunctionBit(
    const int function
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetFunctionBit");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetFunctionBit"));

    int func_bit = 0;

    switch (function) {
        case AMCMM_ACQINFO:
        case AMCMR_ACQINFO:
        case AMCMA_ACQINFO_DONE:
            func_bit = PQM_ACQ_BIT_ACQINFO;
            break;

        case AMCMM_GATEMODE:
        case AMCMR_GATEMODE:
        case AMCMA_GATEMODE_DONE:
            func_bit = PQM_ACQ_BIT_GATEMODE;
            break;

        case AMCMM_RFINTERLOCK:
        case AMCMR_RFINTERLOCK:
        case AMCMA_RFINTERLOCK_DONE:
            func_bit = PQM_ACQ_BIT_RFINTERLOCK;
            break;

        case AMCMM_SHIMCTRL:
        case AMCMR_SHIMCTRL:
        case AMCMA_SHIMCTRL_DONE:
            func_bit = PQM_ACQ_BIT_SHIMCTRL;
            break;

        case AMCMM_MOVECOUCH:
        case AMCMR_MOVECOUCH:
        case AMCMA_MOVECOUCH_DONE:
            func_bit =  PQM_ACQ_BIT_MOVECOUCH;
            break;

        case AMCMM_REMOTE_MOVECOUCH:
        case AMCMR_REMOTE_MOVECOUCH:
        case AMCMA_REMOTE_MOVECOUCH_DONE:
            func_bit = PQM_ACQ_BIT_REMOTE_MOVECOUCH;
            break;

        case AMCMM_CONTROL:
        case AMCMR_CONTROL:
        case AMCMA_CONTROL_DONE:
            func_bit = PQM_ACQ_BIT_CONTROL;
            break;

        case AMCMM_PRESCAN:
        case AMCMR_PRESCAN:
        case AMCMA_PRESCAN_DONE:
        case AMCMM_PRESCAN_CONTINUE:        /* V5.00 */
        case AMCMR_PRESCAN_CONTINUE:
        case AMCMA_PRESCAN_CONTINUE_DONE:

            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMM_MRS_PRESCAN:
        case AMCMR_MRS_PRESCAN:
        case AMCMA_MRS_PRESCAN_DONE:
            // - Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
            func_bit = PQM_ACQ_BIT_PRESCAN;
            break;

        case AMCMM_RTSARCTRL:
        case AMCMR_RTSARCTRL:
        case AMCMA_RTSARCTRL_DONE:
            func_bit = PQM_ACQ_BIT_SAEINFO;
            break;


        case AMCMM_ACQUISITION:
        case AMCMR_ACQUISITION:
        case AMCMA_ACQUISITION_DONE:
        case AMCMM_PREP_SCAN:
        case AMCMR_PREP_SCAN:
        case AMCMA_PREP_SCAN_DONE:
        case AMCMM_ACQUISITION_CONTINUE:    /* V5.00 */
        case AMCMR_ACQUISITION_CONTINUE:
        case AMCMA_ACQUISITION_CONTINUE_DONE:
        case AMCMM_MO_MONITOR:              /* V5.00 */
        case AMCMR_MO_MONITOR:
        case AMCMA_MO_MONITOR_DONE:
        case AMCMM_MO_MONITOR_ABORT:        /* V5.00 */
        case AMCMR_MO_MONITOR_ABORT:
        case AMCMA_MO_MONITOR_ABORT_DONE:
        case AMCMM_MO_CORRECT:              /* V5.00 */
        case AMCMR_MO_CORRECT:
        case AMCMA_MO_CORRECT_DONE:
        case AMCMM_ECG_ELECTRODE:           /* V5.00 */
        case AMCMR_ECG_ELECTRODE:
        case AMCMA_ECG_ELECTRODE_DONE:
        case AMCMM_AMB_SCAN:
        case AMCMR_AMB_SCAN:
        case AMCMA_AMB_SCAN_DONE:
            func_bit = PQM_ACQ_BIT_ACQUISITION;
            break;

        case AMCMS_WAIT_FOR_START:
        case AMCMS_SUBPROTOCOL_BEGIN:
        case AMCMS_SUBPROTOCOL_END:
        case AMCMS_AMB_PROTOCOL_END:
        case AMCMS_PRESCAN_TXCOIL_BEGIN:
        case AMCMS_PRESCAN_TXCOIL_END:
        case AMCMS_PRESCAN_RXCOIL_BEGIN:
        case AMCMS_PRESCAN_RXCOIL_END:
        case AMCMS_PRESCAN_CFA_BEGIN:
        case AMCMS_PRESCAN_CFA_END:
        case AMCMS_PRESCAN_RFL_BEGIN:
        case AMCMS_PRESCAN_RFL_END:
        case AMCMS_PRESCAN_RXGAIN_BEGIN:
        case AMCMS_PRESCAN_RXGAIN_END:
        case AMCMS_PRESCAN_PHCOR_BEGIN:
        case AMCMS_PRESCAN_PHCOR_END:
        case AMCMS_SCAN_BEGIN:
        case AMCMS_SCAN_END:
        case AMCMS_SCAN_PAUSE:
        case AMCMS_SCAN_RESUME:
        case AMCMS_COUCH_POSITION:
        case AMCMS_FLUORO_INFO:
        case AMCMS_DYNAMIC_SCAN_BEGIN:
        case AMCMS_DYNAMIC_SCAN_END:
        case AMCMS_VISUALPREP_BEGIN:
        case AMCMS_VISUALPREP_END:
        case AMCMS_MOVINGBED_SCAN_BEGIN:
        case AMCMS_MOVINGBED_SCAN_END:
        case AMCMS_RMSCAN_END:

            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS
        case AMCMS_MRS_PRESCAN_F0_BEGIN:
        case AMCMS_MRS_PRESCAN_F0_END:
        case AMCMS_MRS_PRESCAN_REF_BEGIN:
        case AMCMS_MRS_PRESCAN_REF_END:
        case AMCMS_MRS_PRESCAN_WSAT_BEGIN:
        case AMCMS_MRS_PRESCAN_WSAT_END:
            // + Patni-HAR/2009Mar05/Added/PSP1-HRS MRS

            return E_NO_ERROR;
    }

    if (!func_bit) {
        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString l_error_str = _T("");
        l_error_str.LoadString(IDS_NO_FUNC_BIT);
        /*CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_FUNC_BIT,
                ERROR_LOG_CRUCIAL1  , _T("PqmPmAcqman.cpp"));*/
        CPQMLogMgr::GetInstance()->WriteEventLog(l_error_str,
                ERROR_LOG_CRUCIAL1  , _T("PqmPmAcqman.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        //-Patni-PJS/2009May10/Modify/IR-82 review comment

    } else {
        CString str_msg(_T(""));
        str_msg.Format(_T("%d %d"), function, func_bit);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        CString l_trace_msg = L"CPqmPmAcqman::GetFunctionBit() = ";
        l_trace_msg += GetStr(function);
    }

    return func_bit;
}

//+Patni-HAR/2009Aug20/Added/Memory Leak Removal
//*******************************Method Header*********************************
//Method Name    :GetPQMSocket()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
CPQMSocket* CPqmPmAcqman::GetPQMSocket(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetPQMSocket");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::GetPQMSocket"));
    return m_acqman_socket;
}

//*******************************Method Header*********************************
//Method Name    :GetScanProcedure()
//Author         :PATNI/ASG
//Purpose        :This Function returns the pointer of CScanProcedure
//*****************************************************************************
CScanProcedure* CPqmPmAcqman::GetScanProcedure(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetScanProcedure");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::GetScanProcedure"));

    return m_scan_procedure;
}

//*******************************Method Header*********************************
//Method Name    :GetProtocol()
//Author         :PATNI/HAR
//Purpose        :This method returning the pointer of CSeries.
//*****************************************************************************
CPqmProtocol* CPqmPmAcqman::GetProtocol(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetProtocol");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::GetProtocol"));

    return m_current_protocol;
}

//*******************************Method Header*********************************
//Method Name    :GetProtocolModeLastScan()
//Author         :PATNI/HAR - SS
//Purpose        :
//*****************************************************************************
CPqmPrMode* CPqmPmAcqman::GetProtocolModeLastScan(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetProtocolModeLastScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetProtocolModeLastScan"));
    return m_last_scan;
}

//*******************************Method Header*********************************
//Method Name    : GetLastSaturnCoil()
//Author         : PATNI
//Purpose        : Return object of Last Saturn coil
//*****************************************************************************
PqmSaturnCoil_t* CPqmPmAcqman::GetLastSaturnCoil(
)
{
    return &m_last_saturn_coil;
}

//*******************************Method Header*********************************
//Method Name    : GetLastSaturnCoilTbl()
//Author         : PATNI
//Purpose        : Return object of Last Saturn Receiver coil
//*****************************************************************************
PqmSaturnRcvCoil_t* CPqmPmAcqman::GetLastSaturnCoilTbl(
)
{
    return &m_last_saturn_tbl;
}

//*******************************Method Header*********************************
//Method Name    :GetDelayStartObj()
//Author         :PATNI/HAR
//Purpose        :This method returning the pointer of CSeries.
//*****************************************************************************
CPqmProtocol* CPqmPmAcqman::GetDelayStartObj(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetDelayStartObj");

    CString str_delay_series(_T(""));
    str_delay_series.Format(_T("%d"), (m_delay_start_obj) ? m_delay_start_obj->GetProtocol() : 0);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (_T("GetDelayStartObj = ") + str_delay_series));
    return m_delay_start_obj;
}

//*******************************Method Header*********************************
//Method Name    :SetDelayStartObj()
//Author         :PATNI/HAR
//Purpose        :This method stores the pointer of the delay start series
//*****************************************************************************
void CPqmPmAcqman::SetDelayStartObj(
    CPqmProtocol* delay_start_obj
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::SetDelayStartObj");

    CString str_delay_series(_T(""));
    str_delay_series.Format(_T("%d"), (delay_start_obj) ? delay_start_obj->GetProtocol() : 0);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (_T("SetDelayStartObj = ") + str_delay_series));

    m_delay_start_obj = delay_start_obj;
}

//+Patni-PJS-AP/2009Sep21/Added/MVC004818, MVC004938
//*******************************Method Header*********************************
//Method Name    : GetIsContinueScan()
//Author         : PATNI/PJS-AP
//Purpose        : Gets the value of m_continue_scan.
//*****************************************************************************
BOOL CPqmPmAcqman::GetIsContinueScan() const
{
    return m_continue_scan;
}

//*******************************Method Header*********************************
//Method Name    : GetIsContinueScan()
//Author         : PATNI/PJS-AP
//Purpose        : Gets the value of m_continue_scan.
//*****************************************************************************
void CPqmPmAcqman::SetIsContinueScan(const BOOL f_bcontinue_scan)
{
    m_continue_scan = f_bcontinue_scan;
}
//-Patni-PJS-AP/2009Sep21/Added/MVC004818, MVC004938

//*******************************Method Header*********************************
//Method Name    :GetLastTransaction()
//Author         :PATNI/ASG
//Purpose        :This Function returns the pointer of CScanTransaction
//*****************************************************************************
CScanTransaction* CPqmPmAcqman::GetLastTransaction(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetLastTransaction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetLastTransaction"));
    return m_last_trans;
}
//+Patni-HAR/2009Aug20/Added/Memory Leak Removal
/*******************************Method Header*********************************
//Method Name    :SetLastTransaction()
//Author         :PATNI/HAR
//Purpose        :This Function returns the pointer of CScanTransaction
//*****************************************************************************/
void CPqmPmAcqman::SetLastTransaction(
    CScanTransaction* f_scan_trans
)
{
    m_last_trans = f_scan_trans;
}

//*******************************Method Header*********************************
//Method Name    :SetInjectTimePresent()
//Author         :PATNI/AKR
//Purpose        :This method is called to set the flag true if Inject time present
//*****************************************************************************
void CPqmPmAcqman::SetInjectTimePresent(const bool f_inject_time_present)
{
    m_inject_time_present = f_inject_time_present;
}

//*******************************Method Header*********************************
//Method Name    :GetInjectTimePresent()
//Author         :PATNI/AKR
//Purpose        :This method is called to Get the flag true if Inject time present
//*****************************************************************************
bool CPqmPmAcqman::GetInjectTimePresent() const
{
    return m_inject_time_present;
}


//*******************************Method Header*********************************
//Method Name    :GetIsScanRetryFlag()
//Author         :PATNI/
//Purpose        : Returns true if Scan is Re started internally
//*****************************************************************************
bool CPqmPmAcqman::GetIsScanRetryFlag() const
{
    return m_scan_retry;
}
//*******************************Method Header*********************************
//Method Name    :GetWaitingForAutoScanStartAcqOrder()
//Author         :PATNI/Nilesh
//Purpose        :
//*****************************************************************************
int CPqmPmAcqman::GetWaitingForAutoScanStartAcqOrder()
{
    return m_waiting_for_auto_scan_acq_order;
}


//*******************************Method Header*********************************
//Method Name    :CPqmPmAcqman()
//Author         :PATNI/HAR
//Purpose        :Copy Constructor
//*****************************************************************************
CPqmPmAcqman::CPqmPmAcqman(
    CPqm* pqmptr) :
    m_acqmannotifier(NULL),
    m_current_protocol(NULL),	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    m_delay_start_obj(NULL),
    m_last_scan(new CPqmPrMode()),
    m_pqmptr(pqmptr),
    m_scan_procedure(NULL),
    m_last_trans(NULL),
    m_acqman_socket(new CPQMSocket),
    m_scan_lockfile(NULL),
    m_after_autoScan(0),
    m_continue_scan(FALSE), //Patni-PJS/AP/2009Sep21/Added/MVC004818, MVC004938
    m_inject_time_present(false),
    m_scan_retry(false),
    m_waiting_for_auto_scan_acq_order(-1)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CPqmPmAcqman");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::CPqmPmAcqman"));

    memset(&m_last_saturn_coil, 0, sizeof(m_last_saturn_coil));
    memset(&m_last_saturn_tbl, 0, sizeof(m_last_saturn_tbl));

    m_scan_procedure = new CScanProcedure(this);
    m_acq_self_object = this;
}


//****************************Method Header********************************
//Method Name   :~CPqmPmAcqman()
//Author        :PATNI/MRP
//Purpose       :Destructor
//*************************************************************************
CPqmPmAcqman::~CPqmPmAcqman(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::~CPqmPmAcqman");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmPmAcqman::~CPqmPmAcqman"));

    DEL_PTR(m_scan_procedure);

    DEL_PTR(m_acqman_socket);
    DEL_PTR(m_scan_lockfile);

    //+Patni-HAR/2009Aug11/Added/Memory Leaks
    DEL_PTR(m_last_scan);
    //-Patni-HAR/2009Aug11/Added/Memory Leaks
}

//****************************Method Header********************************
//Method Name   :AddScanTransaction()
//Author        :PATNI/MRP
//Purpose       :This method adds the scan transaction to the map
//               maintained for the transactions
//*************************************************************************
bool CPqmPmAcqman::AddScanTransaction(
    int request_uid,
    CScanTransaction* scan_transaction
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AddScanTransaction");

    CString str_uid(_T(""));
    str_uid.Format(_T("%d"), request_uid);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (_T("Request UID = ") + str_uid));

    m_map_uid_scan_transactions[request_uid] = scan_transaction;

    return true;
}

//****************************Method Header********************************
//Method Name   :ConvertClientMsgToAscii()
//Author        :PATNI/SC
//Purpose       :
//*************************************************************************
bool CPqmPmAcqman::ConvertClientMsgToAscii(
    const AM_ClientMessageHeader_t& header,
    /*AM_ClientToAcqMan_t client_data,*/
    const AM_ClientToAcqMan_unicode_t& client_data,
    /*AM_ClientToAcqMan_t_Ascii* */ AM_ClientToAcqMan_t* client_data_ascii
)
{

    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::ConvertClientMsgToAscii");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ConvertClientMsgToAscii"));


    switch (header.function) {
        case AMCMM_ACQINFO:
            memcpy(&(client_data_ascii->param.acqInfo), &client_data.param.acqInfo, sizeof(client_data.param.acqInfo));
            break;

        case AMCMM_RFINTERLOCK:
            client_data_ascii->param.interlock.mode = client_data.param.interlock.mode;
            client_data_ascii->param.interlock.watt = client_data.param.interlock.watt;
            break;

        case AMCMM_SHIMCTRL:
            wcstombs(client_data_ascii->param.shimCtrl.padd, client_data.param.shimCtrl.padd, sizeof(client_data_ascii->param.shimCtrl.padd));
            client_data_ascii->param.shimCtrl.CFvalue = client_data.param.shimCtrl.CFvalue;
            client_data_ascii->param.shimCtrl.mode = client_data.param.shimCtrl.mode;
            memcpy(&(client_data_ascii->param.shimCtrl.value), &client_data.param.shimCtrl.value, sizeof(client_data.param.shimCtrl.value));
            break;

        case AMCMM_PREP_SCAN:
            wcstombs(client_data_ascii->param.visualPrep.prep_study, client_data.param.visualPrep.prep_study, sizeof(client_data_ascii->param.visualPrep.prep_study));
            client_data_ascii->param.visualPrep.prep_protocol = client_data.param.visualPrep.prep_protocol;
            break;

        case AMCMM_DISPLAYMSG:
            wcstombs(client_data_ascii->param.displayMsg.message, client_data.param.displayMsg.message, sizeof(client_data_ascii->param.displayMsg.message));
            break;

        case AMCMM_ECG_ELECTRODE:
            memcpy(&(client_data_ascii->param.ECGElectrode), &client_data.param.ECGElectrode, sizeof(client_data.param.ECGElectrode));
            break;

        case AMCMM_FLUORO:
        case AMCMM_FLUORO_CONTR:
            client_data_ascii->param.fluoroContr.kind = client_data.param.fluoroContr.kind;
            break;

        case AMCMM_GATEMODE:
            client_data_ascii->param.gateMode.method = client_data.param.gateMode.method;
            break;

        case AMCMM_PRESCAN:
        case AMCMM_PRESCAN_CONTINUE:
            memcpy(&(client_data_ascii->param.preScan), &client_data.param.preScan, sizeof(client_data.param.preScan));
            break;

        case AMCMM_CONTROL:
            memcpy(&(client_data_ascii->param.control), &client_data.param.control, sizeof(client_data.param.control));
            break;

        case AMCMM_MOVECOUCH:
            client_data_ascii->param.moveCouch.position = client_data.param.moveCouch.position;
            break;

        case AMCMM_REMOTE_MOVECOUCH:
            client_data_ascii->param.remoteMoveCouch.position = client_data.param.remoteMoveCouch.position;
            client_data_ascii->param.remoteMoveCouch.speed = client_data.param.remoteMoveCouch.speed;
            break;

        case AMCMM_MO_CORRECT:
            wcstombs(client_data_ascii->param.motionCorrect.probe_study, client_data.param.motionCorrect.probe_study, sizeof(client_data_ascii->param.motionCorrect.probe_study));
            client_data_ascii->param.motionCorrect.probe_protocol = client_data.param.motionCorrect.probe_protocol;
            break;
    }

    return true ;
}


//*******************************Method Header*********************************
//Method Name    :LogHeaderAndData
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CPqmPmAcqman::LogHeaderAndData(
    const AM_ClientMessageHeader_t& f_header,
    /*AM_ClientToAcqMan_t f_data */const AM_ClientToAcqMan_unicode_t& f_data
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::LogHeaderAndData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of LogHeaderAndData"));

    CString l_header_str(_T(""));
    l_header_str.Format(L"client = %d , function = %d ,version_nember = %d , byte_count = %d",
                        f_header.client, f_header.function, f_header.version_number, f_header.byte_count);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_header_str);

    CString l_data_str(_T(""));
    l_data_str.Format(L",protocol_node = %d, clientKind = %d , normalize = %d \r \
       param.acqInfo.kind = %d \r param.reqCnt = %d \r  param.control.kind = %d \r \
       param.ECGElectrode.config = %d \r \
       param.fluoroContr.kind = %d \r param.gateMode.method = %d \r \
       param.interlock.mode = %d \r param.interlock.watt = %d \r \
       param.motionCorrect.probe_protocol = %d \r param.motionCorrect.probe_study = %s \r \
       param.moveCouch.position = %d \r param.preScan.kind = %d \r \
       param.preScan.reqCnt = %d \r param.remoteMoveCouch.position = %d \r \
       param.remoteMoveCouch.speed =%d \r param.RSBCalibrate.frequency = %d \r\
       param.shimCtrl.CFvalue = %d \r param.shimCtrl.mode = %d \r \
       param.shimCtrl.padd = %d \r param.shimCtrl.value = %d \r \
       param.visualPrep.prep_protocol = %d",
                      f_data.protocol_node, f_data.clientKind, f_data.normalize,
                      f_data.param.acqInfo.kind, f_data.param.acqInfo.reqCnt,
                      f_data.param.control.kind,
                      f_data.param.ECGElectrode.config, f_data.param.fluoroContr.kind,
                      f_data.param.gateMode.method, f_data.param.interlock.mode,
                      f_data.param.interlock.watt, f_data.param.motionCorrect.probe_protocol,
                      f_data.param.motionCorrect.probe_study, f_data.param.moveCouch.position,
                      f_data.param.preScan.kind, f_data.param.preScan.reqCnt,
                      f_data.param.remoteMoveCouch.position, f_data.param.remoteMoveCouch.speed,
                      f_data.param.RSBCalibrate.frequency, f_data.param.shimCtrl.CFvalue,
                      f_data.param.shimCtrl.mode, f_data.param.shimCtrl.padd,
                      f_data.param.shimCtrl.value, f_data.param.visualPrep.prep_protocol);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_data_str);
}

//*******************************Method Header*********************************
//Method Name    :LogHeaderAndData
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CPqmPmAcqman::LogHeaderAndData(
    const AM_ClientMessageHeader_t& f_received_header,
    const AM_AcqManToClient_t& f_received_body
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::LogHeaderAndData");

    CString l_header_str(_T(""));
    l_header_str.Format(L"client = %d , function = %d ,version_nember = %d , byte_count = %d",
                        f_received_header.client, f_received_header.function, f_received_header.version_number, f_received_header.byte_count);


    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_header_str);

    CString l_data_str(_T(""));
    l_data_str.Format(L"acqman.hold = %d \r \
                    cfInfo.cfValue =%d \r coilInfo.port =%d \r \
                    couchInfo.couchStatusFlag =%d \r couchInfo.position =%d \r \
                    gateInfo.g_max_per = %d \r gateInfo.g_max_qrs =%d \r \
                    gateInfo.g_max_resp = %d \r gateInfo.g_min_per = %d \r \
                    gateInfo.g_min_qrs = %d \r gateInfo.g_min_resp = %d \r \
                    gateInfo.g_per_fall = %d \r gateInfo.g_per_rise = %d \r \
                    gateInfo.g_qrs %d \r gateInfo.g_resp_exp =%d \r \
                    gateInfo.g_resp_ins =%d \r gateInfo.method =%d \r \
                    status = %d \r statusList.lines = %d",
                      f_received_body.acqman.hold,
                      f_received_body.cfInfo.cfValue,
                      f_received_body.coilInfo.port,
                      f_received_body.couchInfo.couchStatusFlag,
                      f_received_body.couchInfo.position,
                      f_received_body.gateInfo.g_max_per,
                      f_received_body.gateInfo.g_max_qrs,
                      f_received_body.gateInfo.g_max_resp,
                      f_received_body.gateInfo.g_min_per,
                      f_received_body.gateInfo.g_min_qrs,
                      f_received_body.gateInfo.g_min_resp,
                      f_received_body.gateInfo.g_per_fall,
                      f_received_body.gateInfo.g_per_rise,
                      f_received_body.gateInfo.g_qrs,
                      f_received_body.gateInfo.g_resp_exp,
                      f_received_body.gateInfo.g_resp_ins,
                      f_received_body.gateInfo.method,
                      f_received_body.status,
                      f_received_body.statusList.lines);


    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_data_str);
}

//*******************************Method Header*********************************
//Method Name    :AcqMakeLockFile
//Author         :PATNI/LK
//Purpose        :This function checks the scan lock file information
//*****************************************************************************
int CPqmPmAcqman::AcqMakeLockFile(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AcqMakeLockFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcqMakeLockFile"));

    FILE*       l_fp_lockfile = NULL;
    int         l_status = E_NO_ERROR;
    char        l_filecontent_buff[256] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (m_scan_lockfile == NULL) {
        m_scan_lockfile = AcqGetLockFileName();
    }

    //  DB_PRINTF("A",("PqmAcqStatic.lock_file=%s\n",PqmAcqStatic.lock_file));
    memset(l_filecontent_buff, NULL, sizeof(l_filecontent_buff));

    // to verify if the lock file exists
    if (!WNP_access(m_scan_lockfile, 00)) {
        //      DB_PRINTF("e",("Lock file exists.\n"));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Lock file exists"));

        if (!(l_fp_lockfile = fopen(m_scan_lockfile, "r"))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("open ERROR(read)"));
            l_status = E_ERROR;

        } else {
            fgets(l_filecontent_buff, sizeof(l_filecontent_buff), l_fp_lockfile);

            //          DB_PRINTF("A",("buff=%s\n",buff));
            if (strlen(l_filecontent_buff) && !strcmp(l_filecontent_buff, LOCK_FILE)) {
                l_status = E_NO_ERROR;

            } else {
                l_status = E_ERROR;
            }

            fclose(l_fp_lockfile);
        }

    } else if (!(l_fp_lockfile = fopen(m_scan_lockfile, "w"))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("open ERROR(write)"));
        l_status = E_ERROR;

    } else {
        sprintf(l_filecontent_buff, "%s", LOCK_FILE);
        fputs(l_filecontent_buff, l_fp_lockfile);
        fclose(l_fp_lockfile);
        l_status = E_NO_ERROR;
    }

    return  l_status;
}

//*******************************Method Header*********************************
//Method Name    :TlGetTmpPath
//Author         :PATNI/LK
//Purpose        :This function gets the path of the scan lock file
//*****************************************************************************
int CPqmPmAcqman::TlGetTmpPath(
    char* f_path
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::TlGetTmpPath");
    //+Patni-PJS/2009Sep16/Modified/Internal review comment
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of TlGetTmpPath"));

    if (f_path == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_path is NULL"));
        return E_ERROR;
    }

    char*   l_envpath = getenv("MRMP_DATA");

    if (l_envpath != NULL) {
        strcpy(f_path, l_envpath);

        return E_NO_ERROR;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get value of MRMP_DATA environment variable"));
    return E_ERROR;
    //-Patni-PJS/2009Sep16/Modified/Internal Code review comment
}

//*******************************Method Header*********************************
//Method Name    :AcqGetLockFileName
//Author         :PATNI/LK
//Purpose        :This function gets the scan lock file name
//*****************************************************************************
char* CPqmPmAcqman::AcqGetLockFileName(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::AcqGetLockFileName");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcqGetLockFileName"));

    char    l_filenamebuff[256] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    TlGetTmpPath(l_filenamebuff);
    strcat(l_filenamebuff, "\\");
    strcat(l_filenamebuff, LOCK_FILE);
    char* l_lockfile = strdup(l_filenamebuff);

    if (l_lockfile == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_lockfile == NULL"));

        //          PqmUiSafeExit(PQM_FATAL);
        AcqDeleteLockFile();
    }

    return l_lockfile;
}

//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmCheckDisk
// Author       : PATNI/ HEMANT
// Purpose      : Check disk space of functions V9.50
//***********************************************************************************
status_t CPqmPmAcqman::PqmCheckDisk(
    CPqmProtocol* f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::PqmCheckDisk");

    //+Patni-AMT/2010Feb16/Modified/Ph-3# MVC002888-Code Optimization
    //Patni-AMT/2010Apr22/Modified/JaFT# MVC002888_CDR_01
    CCheckSequence l_checksequence(m_pqmptr);
    CString temp_string = _T("");

    //Patni-Hemant/2010Mar6/Added/ACE-Phase#3/CDS Requirement
    int status = 0;
    bool l_abortScan = false;

    //+Patni-AMT/2010Feb16/Modified/Ph-3# MVC002888-Code Optimization
    if ((status = l_checksequence.CheckDiskSpace(f_protocol)) != E_NO_ERROR) {
        int l_msg_id = 0;
#define CASE_MSG_ID(c,n) {case c: l_msg_id=n; temp_string=#n; break;}

        switch (status) {
                CASE_MSG_ID(PQM_ERROR_VAP_NO_SPACE,     IDS_MSG_NO_VAP_SPACE);
                CASE_MSG_ID(PQM_ERROR_MATRIX_OVER,      IDS_MSG_MATRIX_OVER);
                CASE_MSG_ID(PQM_ERROR_CHANNEL_OVER,     IDS_MSG_CHANNEL_OVER);
                CASE_MSG_ID(PQM_ERROR_VAP_RATIO_OVER,   IDS_MSG_VAP_RATIO_OVER);
                CASE_MSG_ID(PQM_ERROR_HOSTDISK_SPACE,   IDS_MSG_NO_HOST_SPACE);
                CASE_MSG_ID(PQM_ERROR_ENGINEDISK_SPACE, IDS_MSG_NO_ENGINE_SPACE);
                CASE_MSG_ID(PQM_ERROR_OVER_2GB,         IDS_MSG_OVER_2GB);
                //Patni-HAR/2009Dec3/Added/DeFT# MVC005533+MCM0229-00114-PQM_ERROR_IMGSRV_MEMORY
                CASE_MSG_ID(PQM_ERROR_IMGSRV_MEMORY,    IDS_MSG_IMGSRV_MEMORY);
                CASE_MSG_ID(PQM_ERROR_VAP_STUDY,        IDS_MSG_VAP_STUDY);
                CASE_MSG_ID(PQM_WARNING_MATRIX_OVER,    IDS_MSG_MATRIX_OVER_WARNING);
                CASE_MSG_ID(PQM_ERROR_OVER_MRS_DATA,    IDS_MSG_OVER_MRS_DATA_WARNING);


            default:
                l_msg_id = IDS_MSG_VAP_STUDY;
                temp_string = _T("IDS_MSG_VAP_STUDY");
                break;
        }

        if (status == PQM_WARNING_MATRIX_OVER) {
            CString l_appname(_T(""));
            l_appname.LoadString(IDS_WARNING_APP_NAME);
            CString temp_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_MSG_MATRIX_OVER_WARNING"));
            MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error ;
            l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(temp_string,
                         l_appname,
                         SCAN_WARN_INFORMATION,
                         SCAN_WARN_SYSMODAL,
                         SCAN_WARN_OK_CANCEL_BUTTONS);


            if (l_response == MPlusErrResponse_CANCEL) {
                l_abortScan = true;

            }

        } else if (status == PQM_ERROR_OVER_MRS_DATA) {

            CString l_appname(_T(""));
            l_appname.LoadString(IDS_WARNING_APP_NAME);
            CString temp_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_MSG_OVER_MRS_DATA_WARNING"));
            MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error ;
            l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(temp_string,
                         l_appname,
                         SCAN_WARN_INFORMATION,
                         SCAN_WARN_SYSMODAL,
                         SCAN_WARN_OKONLYBUTTON);


            if (l_response == MPlusErrResponse_OK) {
                l_abortScan = true;
            }

        } else {
            CPQMLogMgr::GetInstance()->DisplayScanError(l_msg_id, temp_string,
                    SCAN_ERR_NOTIFY, L"PqmPmAcqman.cpp");

            l_abortScan = true;
        }

        if (l_abortScan) {
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            return status;

        } else {
            status = E_NO_ERROR;
        }

#undef CASE_MSG_ID
        //Patni-AMT/2010Apr22/commented/JaFT# MVC002888_CDR_01 deletion of pointer

    }

    //+Patni-AMT/2010Mar03/Modified/Ph-3# MVC00288-SkipMPlus Disk Space
    if (FALSE == l_checksequence.SkipMPlusDiskCheck()) {
        //+Patni-MP/2009Jan28/Added/Phase-3 MVC002888
        long l_LocalDBdiskspace = 0;

        if (!CDBServerWrapper::CheckLocalDBSpace(l_LocalDBdiskspace)) {

            CString l_str = _T("");
            l_str.Format(_T("InSufficient Space in Local DB=%d"), l_LocalDBdiskspace);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MSG_NO_LOCALDB_SPACE,
                    _T("IDS_MSG_NO_LOCALDB_SPACE"), SCAN_ERR_NOTIFY, L"PqmPmAcqman.cpp");

            m_pqmptr->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            m_pqmptr->GetAMB()->PqmPmAbortAmb();
            //Patni-AMT/2010Apr22/Commented/JaFT# MVC002888_CDR_01 deletion of pointer
            return status;
        }

        //-Patni-MP/2009Jan28/Added/Phase-3 MVC002888
    }

    //Patni-AMT/2010Apr22/Commented/JaFT# MVC002888_CDR_01 Pointer deletion
    //-Patni-AMT/2010Feb16/Modified/Ph-3# MVC002888-Code Optimization

    return E_NO_ERROR;
}
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement


//****************************Method Header********************************
//Method Name   :CanAutoScanStartForWaitingProtocol()
//Author        :PATNI/AKR
//Purpose       :This metod will check wether scan can be started or not
//*************************************************************************
bool CPqmPmAcqman::CanAutoScanStartForWaitingProtocol()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CanAutoScanStartForWaitingProtocol");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    bool l_result = false;

    if (NULL == m_scan_procedure) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return l_result;
    }

    l_result = !m_scan_procedure->GetAPCRetry() && !m_scan_procedure->GetCFARetry() && !m_scan_procedure->GetProbing() ;


    if (!l_result) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Can not scan start for delay protocol"));
        m_waiting_for_auto_scan_acq_order = -1;
        m_pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
    }

    return l_result;
}


//****************************Method Header************************************
//Method Name   :GetWirelessFilterAndMode()
//Author        :iGATE
//Purpose       :Returns FALSE if doesn't get the wireless filter and wireless
//				 mode value from VF
//*****************************************************************************
bool CPqmPmAcqman::GetWirelessFilterAndMode(
    const int f_acq_order,
    int32_t& f_wireless_filter,
    int32_t& f_wireless_mode
) const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetWirelessFilterAndMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    CVarFieldHandle* l_vfdb_handle = m_pqmptr->GetStudy()->GetVfDBHandle();
    int				l_n = 0;

    if (l_vfdb_handle == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vfdb_handle is NULL."));
        return false;
    }

    VfFieldSpec_t vfReq[NUM_2] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_acq_order;
    VFF_SET_ARGS(vfReq[0],
                 SVN_PLN_WIRELESS_GATE_TRIG_FILTER,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &f_wireless_filter,
                 NULL);

    l_n++;

    VFF_SET_ARGS(vfReq[1],
                 SVN_PLN_WIRELESS_GATE_TRIG_FILTER_MODE,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &f_wireless_mode,
                 NULL);

    l_n++;

    int num = 0;
    const status_t status = l_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, vfReq, l_n, &num);

    if ((E_ERROR == status) || (num != l_n)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Wireless filter and wireless mode Tags not present!!"));
        return false;
    }

    return true;
}


//****************************Method Header********************************
//Method Name   :GetScanTransaction()
//Author        :PATNI/MRP
//Purpose       :This method searches for the scan transaction for the given
//               request uid in the map of scan transactions
//*************************************************************************
CScanTransaction* CPqmPmAcqman::GetScanTransaction(
    const int request_uid
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetScanTransaction");

    CString str_uid(_T(""));
    str_uid.Format(_T("%d"), request_uid);


    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, str_uid);

    CScanTransaction* scan_transaction = NULL;
    CScanTransactionMap::iterator it = m_map_uid_scan_transactions.find(request_uid);

    if (it != m_map_uid_scan_transactions.end()) {
        scan_transaction = it->second;
    }

    return scan_transaction;
}

//*******************************Method Header*********************************
//Method Name    :GetStatusString()
//Author         :PATNI/HAR
//Purpose        :This Function Returns string for the requested int value
//*****************************************************************************
CString CPqmPmAcqman::GetStatusString(
    const int status
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::GetStatusString");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetStatusString"));

    CString str = _T("");

    switch (status) {
        case 0:
            str = _T("AMCM_SUCCESS");
            break;

        case 1:
            str = _T("AMCM_WARNING");
            break;

        case 2:
            str = _T("AMCM_ERROR");
            break;

        case 3:
            str = _T("AMCM_NACK");
            break;

        case 4:
            str = _T("AMCM_COUCH");
            break;

        case 5:
            str = _T("AMCM_TXCOIL");
            break;

        case 6:
            str = _T("AMCM_RXCOIL");
            break;

        case 7:
            str = _T("AMCM_RECONST");
            break;

        case 8:
            str = _T("AMCM_GATEMETHOD");
            break;

        case 9:
            str = _T("AMCM_ABORT");
            break;

        case 10:
            str = _T("AMCM_SEQGEN");
            break;

        case 11:
            str = _T("AMCM_SAR");
            break;

        case 12:
            str = _T("AMCM_PCOIL");
            break;

        case 13:
            str = _T("AMCM_RMERROR");
            break;

        case 14:
            str = _T("AMCM_TCMERROR");
            break;

        case 15:
            str = _T("AMCM_HELIUM");
            break;
    }

    return str;
}

//+ Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   : GetPQMSocketID
//Author        : PATNI\DKH
//Purpose       : To get the socket id
//**************************************************************************
UINT CPqmPmAcqman::GetPQMSocketID() const
{
    UINT l_socketid = 0;
    l_socketid =  m_acqman_socket->GetSocketID();
    return l_socketid;
}
//- Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow

//*******************************Method Header*********************************
//Method Name    : GetAfterAutoScan()
//Author         : PATNI/MRP
//Purpose        : Gets the value of after auto scan.
//*****************************************************************************
int CPqmPmAcqman::GetAfterAutoScan() const
{
    return m_after_autoScan;
}

void CPqmPmAcqman::SetAfterAutoScan(
    const int f_after_autoScan
)
{
    m_after_autoScan = f_after_autoScan;
}

//*******************************Method Header*********************************
//Method Name    : SetLastSaturnCoilTbl()
//Author         : PATNI
//Purpose        : Sets object of Last Saturn Receiver coil
//*****************************************************************************
bool CPqmPmAcqman::SetLastSaturnCoilTbl(
    PqmSaturnRcvCoil_t* f_last_saturn_coil_tbl
)
{
    if (f_last_saturn_coil_tbl != NULL) {
        m_last_saturn_tbl = *f_last_saturn_coil_tbl ;
        return true;
    }

    return false;
}

//*******************************Method Header*********************************
//Method Name    : SetLastSaturnCoil()
//Author         : PATNI
//Purpose        : Set object of Last Saturn coil
//*****************************************************************************
bool CPqmPmAcqman::SetLastSaturnCoil(
    PqmSaturnCoil_t* f_last_saturn_coil
)
{
    if (f_last_saturn_coil != NULL) {
        m_last_saturn_coil = *f_last_saturn_coil;
        return true;
    }

    return false;
}

//*******************************Method Header*********************************
//Method Name    :SetProtocolModeLastScan()
//Author         :PATNI/HAR - SS
//Purpose        :
//*****************************************************************************
void CPqmPmAcqman::SetProtocolModeLastScan(
    CPqmPrMode* last_scan
)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::SetProtocolModeLastScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProtocolModeLastScan"));

    m_last_scan->Copy(last_scan);
}

//*******************************Method Header*********************************
//Method Name    :CheckDelayTimeForTopProtocol()
//Author         :PATNI/AKR
//Purpose        :This function will check for delay time and check wether to wait for auto scan or not
//*****************************************************************************
bool CPqmPmAcqman::CheckDelayTimeForTopProtocol(const CPqmProtocol* f_top_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CheckDelayTimeForTopProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_waiting_for_auto_scan_acq_order = -1;


    if (m_inject_time_present) {
        if (!WaitForAutoScan(f_top_protocol)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delay time is less than timer time, scan should start"));

            if (m_scan_procedure->GetProbing()) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Prob scan is going on, scan can not start."));
                m_pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
                return true;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoScan is Waiting to elapse delay time."));
            m_waiting_for_auto_scan_acq_order = f_top_protocol->GetProtocol();
            m_pqmptr->WriteToUI(PQM_MSG_UPDATE_WAIT_FOR_AUTO_SCAN_ICON);
            return true;
        }
    }

    return false;
}


//*******************************Method Header*********************************
//Method Name    :WaitForAutoScan()
//Author         :PATNI/AKR
//Purpose        :This method is called to check, if autoscan has to wait or not
//*****************************************************************************
bool CPqmPmAcqman::WaitForAutoScan(const CPqmProtocol* f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::WaitForAutoScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmPmAcqman::WaitForAutoScan"));

    if (NULL == f_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol pointer is NULL"));
        return false;
    }

    VARIANT l_pdata;
    memset(&l_pdata, 0, sizeof(l_pdata));

    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study pointer is NULL"));
        return false;
    }

    l_pqm_study->CalculateTimeDiff(&l_pdata);
    CPqmPmAcqman::m_timer_time = l_pdata.intVal;

    if (MAX_TIMER_TIME < m_timer_time  || MIN_TIMER_TIME > m_timer_time)  {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inject timer has crossed its limit"));
        return false;
    }

    CString l_delay_time = f_protocol->GetDelayTime();

    if (l_delay_time.IsEmpty()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delay Time is not present for this protocol."));
        return false;
    }

    CString l_temp = l_delay_time.Left(NUM_2);
    int l_min = _ttoi(l_temp);

    l_delay_time = l_delay_time.Mid(NUM_3);
    int l_sec = _ttoi(l_delay_time);

    m_delay_time = l_min * SIXTY_SEC + l_sec;

    if (PQM_SLAVE_PROTOCOL == m_pqmptr->IsMasterSlaveProtocol(f_protocol)) {

        m_delay_time -=  GetEarlyScanTimeFromIni();
    }

    bool l_ret = false;

    if (m_timer_time < m_delay_time) {
        l_ret = true;
        int l_waiting_time = m_delay_time - m_timer_time;
        CString l_trace_msg(_T(""));
        l_trace_msg.Format(_T("Waiting for Auto Scan to elapse delay time for %d secs"), l_waiting_time);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);
        AutoScanTimerSetUp(l_waiting_time);
    }

    return l_ret;
}
//*******************************Method Header*********************************
//Method Name    :AutoScanTimerSetUp()
//Author         :PATNI/PVN
//Purpose        :This method is called to wait for AutoScan timer
//*****************************************************************************
void CPqmPmAcqman::AutoScanTimerSetUp(int f_time_out)
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CPqmPmAcqman::AutoScanTimerSetUp"), _T("Trace"));
    WNP_signal(SIGALRM, &CPqmPmAcqman::WaitAutoStartTimerOutProc);
    WNP_alarm(f_time_out);
}
//*******************************Method Header*********************************
//Method Name    :WaitAutoStartTimerOutProc()
//Author         :PATNI/PVN
//Purpose        :This method is called when Timer out
//*****************************************************************************
void CPqmPmAcqman::WaitAutoStartTimerOutProc(int sig)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::WaitAutoStartTimerOutProc");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_acq_self_object) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_acq_self_object is NULL."));
        return;
    }

    if (m_acq_self_object->CanAutoScanStartForWaitingProtocol()) {
        m_acq_self_object->AutoScanStart();
    }
}
//*******************************Method Header*********************************
//Method Name    :GetEarlyScanTimeFromIni()
//Author         :PATNI/AKR
//Purpose        :This method is called to get early scan time from autoseparate ini file.
//*****************************************************************************
int CPqmPmAcqman::GetEarlyScanTimeFromIni()
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CPqmPmAcqman::GetEarlyScanTimeFromIni"),
              _T("CPqmPmAcqman::GetEarlyScanTimeFromIni"));

    CString l_str_filename = _T("");
    GetAutoSepIniFilePath(l_str_filename);

    CIniReader l_ini_reader(l_str_filename);
    CString l_str = _T("");

    if (l_ini_reader.SectionExists(EARLY_SCAN_TIME_SECTION)) {
        l_str = l_ini_reader.GetKeyValue(EARLY_SCAN_TIME_KEY, EARLY_SCAN_TIME_SECTION);
    }

    int l_length = l_str.GetLength();
    int l_subtract_delay_time = DEFAULT_DELAY_TIME;

    if (l_length > 0) {
        l_subtract_delay_time = _ttoi(l_str);

        if (0 == l_subtract_delay_time) {
            if (1 != l_str.GetLength() || ZERO_ASCII != (int)l_str[0]) {
                l_subtract_delay_time = DEFAULT_DELAY_TIME;
            }
        }
    }

    return l_subtract_delay_time;
}
//*****************************************************************************
//Method Name   : GetAutoSepIniFilePath
//Author        : Patni
//Purpose       : Returns the AutoSepDynamic.ini path
//*****************************************************************************
void CPqmPmAcqman::GetAutoSepIniFilePath(CString& f_str_filename)
{
    wchar_t l_buffer[MAX_PATH] = {L'\0'};
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    f_str_filename  = l_buffer;
    f_str_filename += AUTO_SEP;
}

void CPqmPmAcqman::CheckWhetherToWaitForAutoLocatorPlannedProt(CPqmProtocol* f_protocol)
{

    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CheckWhetherToWaitForAutoLocatorPlannedProt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmPmAcqman::CheckWhetherToWaitForAutoLocatorPlannedProt"));

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol pointer is NULL"));
        return;
    }

    //check whether next Autoscan is planned for HALA/CALA
    if (f_protocol->GetCalaPlannedStatus() == PLANNED_STATUS_NOVECTOR || f_protocol->GetCalaPlannedStatus() == PLANNED_STATUS_STARTED
        || f_protocol->GetHALAPlannedStatus() == PLANNED_STATUS_NOVECTOR || f_protocol->GetHALAPlannedStatus() == PLANNED_STATUS_STARTED) {

        HANDLE l_autolocator_handle = CreateEvent(NULL, TRUE, TRUE, PQM_AUTOLOCATOR_WAIT_FOR_REPLY);

        if (NULL != l_autolocator_handle) {
            if (ERROR_ALREADY_EXISTS != ::GetLastError()) {
                ResetEvent(l_autolocator_handle);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENT is RESET in wait AutoScanstart function to wait for eaytech application."));
            }
        }

        //wait for GL_REGISTER_PQM from AutoLocator application
        if (NULL != l_autolocator_handle) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Waiting for Easytech EVENT to set after GL_Register is received"));

            if (WAIT_TIMEOUT == WaitForSingleObject(l_autolocator_handle, g_autolocator_register_timeout)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GL_PQM_RegisterToPQM not recived from EasyTech Application"));
            }

            SetEvent(l_autolocator_handle);
            CloseHandle(l_autolocator_handle);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENT is set after GL_Register is received or timed out "));

        }
    }
}

//*****************************************************************************
//Method Name   : CalcSARControlAndCheckSAR
//Author        : iGate
//Purpose       :
//*****************************************************************************
bool CPqmPmAcqman::CalcSARControlAndCheckSAR(CPqmProtocol* f_current_protocol, bool f_scan_retry, bool f_is_prescan_done)
{

    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CalcSARControlAndCheckSAR");
    PQM_SCOPED_TRACE();

    if (NULL == f_current_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_current_protocol is NULL"));
        return false;;
    }

    m_scan_procedure->Set10SecOverWaitCount(0);
    return CalcSARControlForAllAndCheckSAR(f_current_protocol, f_scan_retry, f_is_prescan_done);
}

//*****************************************************************************
//Method Name   : CalcSARControlForAllAndCheckSAR
//Author        : iGate
//Purpose       :
//*****************************************************************************
bool CPqmPmAcqman::CalcSARControlForAllAndCheckSAR(CPqmProtocol* f_current_protocol, const bool f_scan_retry, const bool f_is_prescan_done)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmAcqman::CalcSARControlForAllAndCheckSAR");
    PQM_SCOPED_TRACE();

    int l_return_status = 1;

    m_pqmptr->GetSARManager()->CalcSARControlAndPutVFForAll(TRUE);

    if (VFC_AMB_MODE_SLAVE == f_current_protocol->GetAMBModeFlag()) {

        while (f_current_protocol && f_current_protocol->GetAMBModeFlag() == VFC_AMB_MODE_SLAVE) {
            const int l_sar_status =  m_scan_procedure->GetStatusForCheckSAR(f_current_protocol, f_is_prescan_done);

            if (PQM_SAR_OK != l_sar_status) {
                l_return_status = m_scan_procedure->HandleSARErrorForStatus(l_sar_status, f_scan_retry);

                if (PQM_SAR_RECALC_AND_CHECK == l_return_status) {
                    return CalcSARControlForAllAndCheckSAR(f_current_protocol, f_scan_retry);

                } else if (!l_return_status) {
                    return false;
                }
            }

            f_current_protocol = m_pqmptr->GetStudy()->GetNextItemFromPos(f_current_protocol);
        }

        return true;
    }

    const int l_sar_status = m_scan_procedure->GetStatusForCheckSAR(f_current_protocol, f_is_prescan_done);

    if (PQM_SAR_OK == l_sar_status) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_SAR_OK"));
        return true;

    } else if (f_scan_retry && (CDS_STEP6_DONE == f_current_protocol->GetCDSInfo()->m_cds_step)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CDS_STEP6_DONE"));
        return true;
    }

    l_return_status = m_scan_procedure->HandleSARErrorForStatus(l_sar_status, f_scan_retry);

    if (PQM_SAR_RECALC_AND_CHECK == l_return_status) {
        return CalcSARControlForAllAndCheckSAR(f_current_protocol, f_scan_retry);
    }

    return l_return_status ? true : false;
}
