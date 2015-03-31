//REDMINE-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ShimCtrl.cpp
 *  Overview: Implementation of CShimCtrl class.
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
#include "ShimCtrl.h"
#include <PQM/resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "pqm.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "pqmstudy.h"
#include "PQMDebugInfo.h"
#include "pqmAMB.h"


//***************************Method Header*************************************
//Method Name    : CShimCtrl()
//Author         : PATNI/HAR
//Purpose        : Default constructor
//*****************************************************************************
CShimCtrl::CShimCtrl(
    CScanProcedure* scan_proc
): CScanTransaction(scan_proc)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::CShimCtrl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}

//***************************Method Header*************************************
//Method Name    : ~CShimCtrl()
//Author         : PATNI/HAR
//Purpose        : Default destructor
//*****************************************************************************
CShimCtrl::~CShimCtrl(
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::~CShimCtrl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
    //CloseHandle(m_pi.hProcess);
    //CloseHandle(m_pi.hThread);
}

//****************************Method Header************************************
//Method Name   :SendShimOffset
//Author        :PATNI/HAR
//Purpose       :This method prepares the Shimming request that will be sent to Acqman
//*****************************************************************************
bool CShimCtrl::SendShimOffset(
    PqmShim_t* f_shim
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::SendShimOffset");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of SendShimOffset"));

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    m_header.function = AMCMM_SHIMCTRL;

    m_data.param.shimCtrl.mode = ACQ_SHIMCTRL_SETUP;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_shim == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("f_shim is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    // + INTGR 5 Bug Fix Lokesh
    m_data.param.shimCtrl.CFvalue = f_shim->fsyValue;
    // - INTGR 5 Bug Fix Lokesh

    for (int i = 0; i < PQM_SHIM_LEN; i++) {
        m_data.param.shimCtrl.value[i] = f_shim->shimOffset[i];
    }

    CString str_msg(_T(""));
    str_msg.Format(_T("m_data.param.shimCtrl.CFvalue  = %d \n"), m_shim.fsyValue);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections

    /*    str_msg.Format(_T("m_data.param.shimCtrl.value[0] = %d \n"),m_data.param.shimCtrl.value[0]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[1] = %d \n"),m_data.param.shimCtrl.value[1]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[2] = %d \n"),m_data.param.shimCtrl.value[2]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[3] = %d \n"),m_data.param.shimCtrl.value[3]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[4] = %d \n"),m_data.param.shimCtrl.value[4]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[5] = %d \n"),m_data.param.shimCtrl.value[5]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[6] = %d \n"),m_data.param.shimCtrl.value[6]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);

        str_msg.Format(_T("m_data.param.shimCtrl.value[7] = %d \n"),m_data.param.shimCtrl.value[7]);
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            COMMHANDLER_MARKER_PQM_ACQMAN,
            _T("CShimCtrl::SendShimOffset()"),
            str_msg);
    */
    for (i = 0; i < PQM_SHIM_LEN; i++) {
        str_msg.Format(_T("m_data.param.shimCtrl.value[%d] = %d \n"), i, m_data.param.shimCtrl.value[i]);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    // Set header and data added by Sarjeet and Meenakashi
    SetHeader(m_header);
    SetData(m_data);

    // -----
    if (!SendMessage()) {
        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        str_msg = _T("");
        str_msg.LoadString(IDS_NOSENDMESSAGE);
        CPQMLogMgr::GetInstance()->WriteEventLog(
            str_msg,
            ERROR_LOG_CRUCIAL1 , _T("ShimCtrl.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            IDS_NOSENDMESSAGE,
            ERROR_LOG_CRUCIAL1 , _T("ShimCtrl.cpp"));
        */
        //-Patni-PJS/2009May10/Modify/IR-82 review comment

        GetScanProcedure()->GetAcqManager()->GetPqm()->GetStudy()->SetScanModeOfHead();
        GetScanProcedure()->ResetCurrentRequest();
        return false;
    }

    return true;
}

//*******************************Method Header*********************************
//Method Name    :ShimmingAdjust
//Author         :PATNI/LK
//Purpose        :Adjusts shimming and sends CFA prescan to AcqMan
//*****************************************************************************
void CShimCtrl::ShimmingAdjust(
)const
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::ShimmingAdjust");
    //Patni-AMT/2009Oct16/Modified/MVC004789-Code Review
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of ShimmingAdjust"));

    if (m_scan_procedure) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections
        CPqm* l_pqmptr = m_scan_procedure->GetAcqManager()->GetPqm();
        //if(l_pqmptr->GetFreezeLevel() & PQM_FREEZE_FOR_SHIMMING) {
        CPqmProtocol* l_pqmprotocol = m_scan_procedure->GetScanProcProtocol();

        //+Patni-PJS/Modified/1Aug2009/Internal defect solved
        if (l_pqmprotocol) {

            //+Patni-AMT/2009Oct16/Modified/MVC004789-Code Review


            // MEITEC/2010Apr23/Added/IR86
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugProMonitor(_T("SHIM: PqmAcqRetryCFA"));

            bool l_ret_value = m_scan_procedure->RetryCFA(l_pqmprotocol->GetProtocol(), l_pqmprotocol);

            if (false == l_ret_value) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("RetryCFA failed, AMCMM_PRESCAN not send to AcqMan"));
            }

            //-Patni-AMT/2009Oct16/Modified/MVC004789-Code Review
            //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
            m_scan_procedure->CFAOpen(l_pqmprotocol);

        } else {
            //Patni-AMT/2009Oct16/Modified/MVC004789-Code Review
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("l_pqmprotocol is NULL"));
        }

        //-Patni-PJS/Modified/1Aug2009/Internal defect solved
        //    }

    } else {
        //Patni-MJC/2009Aug17/Modified/cpp test corrections
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_scan_procedure is NULL"));
    }
}

//****************************Method Header************************************
//Method Name   :ProcessResponse()
//Author        :PATNI/HAR
//Purpose       :This method will be called to process response received from Acqman
//*****************************************************************************
void CShimCtrl::ProcessResponse(
    AM_ClientMessageHeader_t* hdr,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::ProcessResponse");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ProcessResponse"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (hdr == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Header is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    m_header = *hdr;
    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    int type = 0;

    int funcBit = acqman->GetFunctionBit(hdr->function);

    CString str_msg(_T(""));

    str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                   acqman->GetStr(body->status),
                   acqman->GetStr(type),
                   acqman->GetStr(hdr->function));

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    if ((body->status == AMCM_SUCCESS) ||
        (body->status == AMCM_RECONST)) {

        if (hdr->function == AMCMR_SHIMCTRL) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);

            if (E_NO_ERROR != BeforePrescan(type, body)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("BeforePrescan failed."));
            }

        } else if (hdr->function == AMCMA_SHIMCTRL_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            if (E_NO_ERROR != BeforePrescan(type, body)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("BeforePrescan failed."));
            }
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(body->status),
                       acqman->GetStr(type),
                       acqman->GetStr(hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    } else if (body->status == AMCM_NACK) {

        if (hdr->function == AMCMR_RFINTERLOCK) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(body->status),
                       acqman->GetStr(type),
                       acqman->GetStr(hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        if (hdr->function == AMCMR_SHIMCTRL) {

            ErrReport(type, IDS_ERR_ACQMAN_SHIMCTRL, _T("IDS_ERR_ACQMAN_SHIMCTRL"), body);				// Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (hdr->function == AMCMA_SHIMCTRL_DONE) {

            ErrReport(type, IDS_ERR_ACQMAN_SHIMCTRL_DONE, _T("IDS_ERR_ACQMAN_SHIMCTRL_DONE"), body);   // Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

        acqman->GetPqm()->GetStudy()->SetAcqStatus(scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        acqman->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT); // To Active Scan Start Button

        if (scan_proc->GetScanProcProtocol() && scan_proc->GetScanProcProtocol()->GetShimming()) {
            acqman->GetPqm()->SetPostProcessFlag(FALSE);
            acqman->GetPqm()->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        }

    } else {

        if (hdr->function == AMCMR_SHIMCTRL) {

            type = AMCM_FUNC_KIND_REQUEST;
            int result = 0;
            result = scan_proc->GetRequest();
            result ^= funcBit;
            //MRPdefect812
            scan_proc->SetRequest(result);
            //MRPdefect812

        } else if (hdr->function == AMCMA_SHIMCTRL_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        if (hdr->function == AMCMR_RFINTERLOCK) {

            ErrReport(type, IDS_ERR_ACQMAN_SHIMCTRL, _T("IDS_ERR_ACQMAN_SHIMCTRL"), body);				// Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {

            ErrReport(type, IDS_ERR_ACQMAN_SHIMCTRL_DONE, _T("IDS_ERR_ACQMAN_SHIMCTRL_DONE"), body);  // Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(body->status),
                       acqman->GetStr(type),
                       acqman->GetStr(hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        if (scan_proc->GetProcessing()) {
            //  scan_proc->ResetCurrentRequest();
        }

        acqman->GetPqm()->GetStudy()->SetAcqStatus(scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        acqman->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT); // To Active Scan Start Button
    }
}

//****************************Method Header************************************
//Method Name   :PrepareRequest()
//Author        :PATNI/HAR-MSN
//Purpose       :
//*****************************************************************************
bool CShimCtrl::PrepareRequest(
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::PrepareRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PrepareRequest"));

    bool result = true;
    CScanProcedure* scan_proc = GetScanProcedure();
    double cfvalue = scan_proc->GetCFValue();
    PqmShim_t* l_pqmshim = scan_proc->GetPqmShimInfoptr();

    if (scan_proc->IsCFValueChanged()) {
        result = SendShimFrequency(cfvalue);

    } else {
        //memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
        memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

        for (int l_cnt = 0; l_cnt < PQM_SHIM_LEN; l_cnt++) {
            m_data.param.shimCtrl.value[l_cnt] = l_pqmshim->shimOffset[l_cnt];
        }

        result = SendShimOffset(l_pqmshim);
    }

    return result;
}

//****************************Method Header************************************
//Method Name   :SendShimFreq()
//Author        :PATNI/HAR
//Purpose       :This method prepares the Shimming request that will be sent to Acqman
//*****************************************************************************
bool CShimCtrl::SendShimFrequency(
    const double f_fsy_value
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::SendShimFrequency");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of SendShimFrequency()"));

    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("f_fsy_value --> %ld"), f_fsy_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    m_header.function = AMCMM_SHIMCTRL;

    m_data.param.shimCtrl.mode = ACQ_SHIMCTRL_SETUP_CFONLY;
    m_data.param.shimCtrl.CFvalue = f_fsy_value;

    CScanProcedure* scan_proc = GetScanProcedure();

    if (scan_proc->IsCFValueChanged()) {
        SetHeader(m_header);
        SetData(m_data);

    } else {
        for (int l_cnt = 0; l_cnt < PQM_SHIM_LEN; l_cnt++) {
            m_data.param.shimCtrl.value[l_cnt] = m_shim.shimOffset[l_cnt];
        }
    }

    if (!SendMessage()) {
        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        l_tracemsg = _T("");
        l_tracemsg.LoadString(IDS_NOSENDMESSAGE);
        CPQMLogMgr::GetInstance()->WriteEventLog(
            l_tracemsg,
            ERROR_LOG_CRUCIAL1 , _T("ShimCtrl.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            IDS_NOSENDMESSAGE,
            ERROR_LOG_CRUCIAL1 , _T("ShimCtrl.cpp"));
        */
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
        scan_proc->GetAcqManager()->GetPqm()->GetStudy()->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();
        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :BeforePrescan()
//Author        :PATNI/HAR
//Purpose       :This method will do the processing required after receiving the
//                reply for the Shimming request made by PQM IPC
//*****************************************************************************
int CShimCtrl::BeforePrescan(
    const int type,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::BeforePrescan");

    ASSERT(type > 0);
    ASSERT(body != NULL);


    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of BeforePrescan"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_scan_procedure == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();

    CScanProcedure* scan_proc = GetScanProcedure();

    if (type == AMCM_FUNC_KIND_REQUEST) {
        if (m_header.function == AMCMR_REMOTE_MOVECOUCH) {
            scan_proc->SetScanAbort(PQM_SCAN_ABORT);
            //          PqmAcqGlobal.scanAbort = PqmAcqScanAbort;
            //          PqmUiSetSensitive(W_SCAN_ABORT_BTN,TRUE);
            //          PqmDebugAcqMonitor("BeforePreScan Callback: Unfreeze Scan");
            //          PqmUiUnfreeze(PQM_FREEZE_FOR_SCAN);
        }

        return E_NO_ERROR;
    }

    if (curr_protocol != NULL) {
        //+Patni-PJS/Added/2011-Mar-12/REDMINE-1529
        curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
    }

    if (scan_proc->GetIsScanning()) {
        if (!PostProcess(body->couchInfo.position,
                         body->couchInfo.couchStatusFlag,
                         m_header.function)) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PostProcess() Failed"));
            return E_ERROR;
        }

    } else {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("scan_proc->GetIsScanning() Failed"));

        if (m_header.function == AMCMA_SHIMCTRL_DONE) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("After Acquisition Done sending CFA Prescan for Shimming"));

            ShimmingAdjust();

        } else {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("Trace of m_header.function != AMCMA_SHIMCTRL_DONE"));
        }
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :ErrReport()
//Author        :PATNI/HAR
//Purpose       :This method does the processing after an error message is received from Acqman
//*****************************************************************************
bool CShimCtrl::ErrReport(
    const int type,
    const int errorId,
    const CString& err_msg,			// Added By KT/ACE-2/15-06-2009/Multilingualisation
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CShimCtrl::ErrReport");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(err_msg);
    UNREFERENCED_PARAMETER(body);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* pqm =  acqman->GetPqm();

    CString str_msg(_T(""));
    str_msg.Format(_T("%s %d"), acqman->GetStr(type), errorId);


    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    str_msg.Empty();
    //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
    str_msg.LoadString(errorId);									// Added By KT/ACE-2/15-06-2009/Multilingualisation
    //error_str = CPqmUtilities::GetMultiLingualString(err_msg);		// Added By KT/ACE-2/15-06-2009/Multilingualisation

    //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
    //Patni-DKH/2010Jan11/Modified/TMSC-REDMINE-1098-Part2
    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(str_msg,
            MPlus::EH::Information, _T("CShimCtrl.cpp"), _T("PQM"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179

    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();
    pqm->GetStudy()->SetAcqStatus(curr_protocol , C_PR_STATUS_WAIT);
    pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
    pqm->GetAMB()->PqmPmAbortAmb();

    if (curr_protocol != NULL) {
        //+Patni-PJS/Added/2011-Mar-12/REDMINE-1529
        curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
        pqm->GetStudy()->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();
    }

    return true;
}


//****************************Method Header************************************
//Method Name   :CloseShimming()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
//void CShimCtrl::CloseShimming(
//) {
//    TerminateProcess(m_pi.hProcess,0);
//
//}

//****************************Method Header************************************
//Method Name   :ShimmingTimerSetup()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
//void CShimCtrl::ShimmingTimerSetup(
//) {
//	char        *l_timebuf;
//
//	// Trace Log
//
//	l_timebuf = getenv("SHIMMING_TIMEOUT");
//	if ( l_timebuf != NULL )
//	{
//		g_shimming_timeout =  atoi(l_timebuf) ;
//	}
//	else
//	{
//		g_shimming_timeout = SHIMMING_TIMEOUT_DEFAULT ;
//	}
//
//	//posixSignal(SIGALRM,pqmShimmingTimeOut);
//	//alarm( Shimming_timeout ) ;
//	//DB_PRINTF("S",("Shimming TIMER set(%d) \n",Shimming_timeout));
//	//DB_FUNC_EXIT() ;
//}


