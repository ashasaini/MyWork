//Redmine-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-781
//Redmine-782
// OlpTemp.cpp: implementation of the COlpTemp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OlpTemp.h"
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include <PQM/resource.h>

//+Patni-Ravindra Acharya/2010June22/Added/IR-153

#include "Pqm.h"
#include "pqmpmacqman.h"
#include "PqmPrMode.h"
#include "scanprocedure.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COlpTemp::COlpTemp(CScanProcedure* f_scan_proc) :
    CScanTransaction(f_scan_proc)
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::COlpTemp");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}

COlpTemp::~COlpTemp()
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::~COlpTemp");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//*******************************Method Header*********************************
//Method Name    :PqmAcqGetOLPTemp
//Author         :PATNI/Ravindra Acharya
//Purpose        :
//*****************************************************************************
//+Patni-Ravindra Acharya/2010Jun09/Added/IR-153
bool  COlpTemp::PqmAcqGetOLPTemp(CPqmProtocol* f_prObj, const int f_mode)
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::PqmAcqGetOLPTemp");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    if (!f_prObj) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("f_prObj is NULL"));
        return false ;
    }

    //+Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    CPqmPmAcqman* l_acqman_manager = GetScanProcedure()->GetAcqManager();

    if (NULL == l_acqman_manager) {
        return false ;
    }

    CPqm* l_pqm_ptr = l_acqman_manager->GetPqm();

    if (NULL == l_pqm_ptr) {
        return false ;
    }

    CPQMStudy* l_study_ptr = l_pqm_ptr->GetStudy();

    if (NULL == l_study_ptr) {
        return false ;
    }

    GetScanProcedure()->SetScanProcProtocol(f_prObj);

    //+Patni-Ravindra Acharya/2010Aug17/Modified/IR-153-Review Comments Implemented
    //if(!pqmAcqSendOLPTemp(PQM_TEMP_SHIM))

    if (!pqmAcqSendOLPTemp(f_mode)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqmAcqSendOLPTemp() failed."));

        //+Patni-Ravindra Achayra/2010Aug16/Commented/IR-153-Part1
        //DisplayMessage(f_mode) ;

        //+Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1

        DisplayMessage(_T("IDS_ERR_CONNECT_ACQMAN"), MPlus::EH::Information) ;
        //-Patni-Ravindra Acharya/2010Aug17/Modified/IR-153-Review Comments Implemented

        //-Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1

        l_study_ptr->SetScanModeOfHead();
        //-Patni-KSS/2011March10/Modified/TMSC-Review-Comments
        GetScanProcedure()->ResetCurrentRequest();

        return false ;
    }

    return true;
}

//*******************************Method Header*********************************
//Method Name    :pqmAcqSendOLPTemp
//Author         :PATNI/Ravindra Acharya
//Purpose        : Prepares and Sends the Message to Acqman
//*****************************************************************************
//+Patni-Ravindra Acharya/2010Jun09/Added/IR-153
bool COlpTemp::pqmAcqSendOLPTemp(const int f_mode)
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::pqmAcqSendOLPTemp");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    //+Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    CPqmPmAcqman* l_acqman_manager = GetScanProcedure()->GetAcqManager();

    if (NULL == l_acqman_manager) {
        return false ;
    }

    CPqm* l_pqm_ptr = l_acqman_manager->GetPqm();

    if (NULL == l_pqm_ptr) {
        return false ;
    }

    CPQMStudy* l_pqm_study = l_pqm_ptr->GetStudy();

    if (NULL == l_pqm_study) {
        return false ;
    }

    CPqmProtocol* l_curr_protocol = GetScanProcedure()->GetScanProcProtocol();

    if (NULL == l_curr_protocol) {
        return false ;
    }

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    m_header.function =  AMCMM_OLPTEMP_MEASUREMENT;

    _tcscpy(m_data.study_file, l_pqm_study->GetStudyName());

    m_data.protocol_node = l_curr_protocol->GetProtocol();

    GetScanProcedure()->SetTempMode(f_mode) ;
    //-Patni-KSS/2011March10/Modified/TMSC-Review-Comments

    CPQMDebugInfo::GetInstance(l_pqm_ptr)->PqmDebugAcqMonitor(_T("pqmAcqSendOLPTemp %i"), f_mode);
    //Patni-HAR/2010Aug13/Modified/Warning Removed
    return (l_acqman_manager->SendMessage(this) ? true : false) ;
}
//-Patni-Ravindra Acharya/2010Jun09/Added/IR-153

void COlpTemp::DisplayMessage(const CString& f_errmsg, const MPlus::EH::MPLUSERRORLEVEL_e l_error_level)
{
    //+Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1

    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
        CPqmUtilities::GetMultiLingualString(f_errmsg),
        l_error_level,
        _T("PQM"),
        _T("PQM")) ;
    //-Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1
}

void COlpTemp::CFAReady() const
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::CFAReady");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    CPqm* l_pqm_ptr = NULL;
    //Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    CPqmPmAcqman* l_acqman_manager = GetScanProcedure()->GetAcqManager();

    if (NULL != l_acqman_manager) {
        l_pqm_ptr = l_acqman_manager->GetPqm();
    }

    if (NULL != l_pqm_ptr) {
        l_pqm_ptr->WriteToUI(PQM_MSG_CFA_OK_BTN_STATE) ;
    }
}

//****************************Method Header************************************
//Method Name   :PrepareRequest
//Author        :PATNI/Ravindra Acharya
//Purpose       :This method prepares the OlpTemp request that will be sent to Acqman
//****************************************************************************
//+Patni-Ravindra Acharya/2010Jun09/Added/IR-153
bool COlpTemp::PrepareRequest()
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::PrepareRequest");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));
    //+Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    CPqmPmAcqman* l_acqman_ptr  = GetScanProcedure()->GetAcqManager() ;

    if (NULL == l_acqman_ptr) {
        return false;
    }

    CPqm* l_pqm_ptr = l_acqman_ptr->GetPqm() ;

    if (NULL == l_pqm_ptr) {
        return false ;
    }

    GetScanProcedure()->SetProcessing(0) ;

    if (!pqmAcqSendOLPTemp(PQM_TEMP_SHIM)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqmAcqSendOLPTemp() failed."));

        l_pqm_ptr->GetStudy()->SetScanModeOfHead();
        GetScanProcedure()->ResetCurrentRequest() ;
        //-Patni-KSS/2011March10/Modified/TMSC-Review-Comments
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Sending AMCMM_OLPTEMP_MEASUREMENT message to AcqMan failed"));

        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :ProcessResponse
//Author        :PATNI/Ravindra Acharya
//Purpose       :
//****************************************************************************
//+Patni-Ravindra Acharya/2010Jun09/Added/IR-153
void COlpTemp::ProcessResponse(
    AM_ClientMessageHeader_t* f_hdr,
    AM_AcqManToClient_t* f_body)
{

    LPCTSTR FUNC_NAME = _T("COlpTemp::ProcessResponse");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));
    //+Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    CPqmPmAcqman* l_acqman_ptr = GetScanProcedure()->GetAcqManager();

    if (l_acqman_ptr == NULL) {
        return ;
    }

    CPqm* l_pqm_ptr = l_acqman_ptr->GetPqm();

    if (NULL == l_pqm_ptr) {
        return ;
    }

    CPQMStudy* l_pqm_study = l_pqm_ptr->GetStudy();

    if (NULL == l_pqm_study) {
        return ;
    }

    CPqmProtocol* l_current_protocol = l_acqman_ptr->GetProtocol();

    CVarFieldHandle* const l_vf_handle = l_pqm_study->GetVfDBHandle();

    int l_type = 0;

    int l_func_bit = l_acqman_ptr->GetFunctionBit(f_hdr->function);

    CString l_str = l_acqman_ptr->GetStr(f_hdr->function);

    if ((l_str.CompareNoCase(_T("Unknown Message !!")) == 0)) {
        l_str.Format(_T("Function (%d)"), f_hdr->function);
    }

    if ((f_body->status == AMCM_SUCCESS) || (f_body->status == AMCM_RECONST)) {
        if (f_hdr->function == AMCMR_OLPTEMP_MEASUREMENT) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: REQUEST]"));

            l_type = AMCM_FUNC_KIND_REQUEST;
            GetScanProcedure()->SetProcessing(GetScanProcedure()->GetProcessing() | l_func_bit);

            if (E_NO_ERROR != pqmAcqOLPTempCallback(l_vf_handle, l_type, f_body)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqmAcqOLPTempCallback() failed."));
            }

        } else if (f_hdr->function == AMCMA_OLPTEMP_MEASUREMENT_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            GetScanProcedure()->SetProcessing(GetScanProcedure()->GetProcessing() ^ l_func_bit);
            GetScanProcedure()->SetRequest(GetScanProcedure()->GetRequest() ^ l_func_bit);

            if (E_NO_ERROR != pqmAcqOLPTempCallback(l_vf_handle, l_type, f_body)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqmAcqOLPTempCallback() failed."));
            }

            bool l_check_next = false;

            l_pqm_ptr->SetPostProcessFlag(FALSE);

            if (PQM_CFA_RETRY_AND_SCAN == GetScanProcedure()->GetCFARetry()) {
                if (!l_pqm_ptr->CanDisplayCFAGraph(l_current_protocol)) {

                    l_pqm_ptr->WriteToUI(PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END);
                    l_check_next = true;

                } else {
                    l_pqm_ptr->WriteToUI(PQM_MSG_CFA_PRESCAN_END);
                }
            }

            GetScanProcedure()->SetCFARetry(PQM_CFA_NONE);

            if (l_check_next && (NULL != l_pqm_ptr) && (NULL != l_acqman_ptr)) {
                GetScanProcedure()->CheckNextProtocol((NULL != l_current_protocol) ? l_current_protocol->GetProtocol() : 0 , TRUE);
            }

        }

        l_str.Empty();
        l_str.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                     l_acqman_ptr->GetStr(f_body->status),
                     l_acqman_ptr->GetStr(l_type),
                     l_acqman_ptr->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str);

    } else if (f_body->status == AMCM_NACK) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("[Response type: AMCM_NACK]"));

        if (f_hdr->function == AMCMR_OLPTEMP_MEASUREMENT) {

            l_type = AMCM_FUNC_KIND_REQUEST;
            GetScanProcedure()->SetRequest(GetScanProcedure()->GetRequest() ^ l_func_bit);


        } else if (f_hdr->function == AMCMA_OLPTEMP_MEASUREMENT_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            GetScanProcedure()->SetProcessing(GetScanProcedure()->GetProcessing() ^ l_func_bit);
            GetScanProcedure()->SetRequest(GetScanProcedure()->GetRequest() ^ l_func_bit);
        }

        l_str.Empty();
        l_str.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                     l_acqman_ptr->GetStr(f_body->status),
                     l_acqman_ptr->GetStr(l_type),
                     l_acqman_ptr->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str);

        if (f_hdr->function == AMCMR_OLPTEMP_MEASUREMENT) {
            pqmAcqErrOLPTemp(l_type, IDS_ERR_OLPTEMP_MEASUREMENT, _T("IDS_ERR_OLPTEMP_MEASUREMENT"), f_body);

        } else if (f_hdr->function == AMCMA_OLPTEMP_MEASUREMENT_DONE) {
            pqmAcqErrOLPTemp(l_type, IDS_ERR_OLPTEMP_MEASUREMENT_DONE, _T("IDS_ERR_OLPTEMP_MEASUREMENT_DONE"), f_body);
        }

        l_pqm_ptr->GetStudy()->SetAcqStatus(GetScanProcedure()->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        l_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        if (PQM_CFA_RETRY_AND_SCAN == GetScanProcedure()->GetCFARetry()) {

            l_pqm_ptr->SetPostProcessFlag(FALSE);
            l_pqm_ptr->WriteToUI(l_pqm_ptr->CanDisplayCFAGraph(l_current_protocol) ? PQM_MSG_CFA_PRESCAN_END : PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END);
            GetScanProcedure()->SetCFARetry(PQM_CFA_NONE);
        }

    } else {
        if (f_hdr->function == AMCMR_OLPTEMP_MEASUREMENT) {

            l_type = AMCM_FUNC_KIND_REQUEST;
            int l_result = GetScanProcedure()->GetRequest();
            l_result ^= l_func_bit;
            GetScanProcedure()->SetRequest(l_result);

        } else if (f_hdr->function == AMCMA_OLPTEMP_MEASUREMENT_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            GetScanProcedure()->SetProcessing(GetScanProcedure()->GetProcessing() ^ l_func_bit);
            GetScanProcedure()->SetRequest(GetScanProcedure()->GetRequest() ^ l_func_bit);
        }

        l_str.Empty();
        l_str.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                     l_acqman_ptr->GetStr(f_body->status),
                     l_acqman_ptr->GetStr(l_type),
                     l_acqman_ptr->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str);

        if (f_hdr->function == AMCMR_OLPTEMP_MEASUREMENT) {

            pqmAcqErrOLPTemp(l_type, IDS_ERR_OLPTEMP_MEASUREMENT, _T("IDS_ERR_OLPTEMP_MEASUREMENT"), f_body);

        } else if (f_hdr->function == AMCMA_OLPTEMP_MEASUREMENT_DONE) {

            pqmAcqErrOLPTemp(l_type, IDS_ERR_OLPTEMP_MEASUREMENT_DONE, _T("IDS_ERR_OLPTEMP_MEASUREMENT_DONE"), f_body);
        }

        GetScanProcedure()->GetProcessing() ;
        l_pqm_ptr->GetStudy()->SetAcqStatus(l_current_protocol, C_PR_STATUS_WAIT);
        //-Patni-KSS/2011March10/Modified/TMSC-Review-Comments
        l_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        if (PQM_CFA_RETRY_AND_SCAN == GetScanProcedure()->GetCFARetry()) {

            l_pqm_ptr->SetPostProcessFlag(FALSE);
            l_pqm_ptr->WriteToUI(l_pqm_ptr->CanDisplayCFAGraph(l_current_protocol) ? PQM_MSG_CFA_PRESCAN_END : PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END);
            GetScanProcedure()->SetCFARetry(PQM_CFA_NONE);
        }
    }
}
//-Patni-Ravindra Acharya/2010Jun09/Added/IR-153

//*******************************Method Header*********************************
//Method Name    :pqmAcqOLPTempCallback
//Author         :PATNI/Ravindra Acharya
//Purpose        :
//*****************************************************************************
//+Patni-Ravindra Acharya/2010Jun09/Added/IR-153
status_t COlpTemp::pqmAcqOLPTempCallback(CVarFieldHandle* const vf_handle,
        const int f_type,
        AM_AcqManToClient_t* f_ptrData)
{
    LPCTSTR FUNC_NAME = _T("COlpTemp::pqmAcqOLPTempCallback");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(vf_handle);
    UNREFERENCED_PARAMETER(f_ptrData);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (f_type == AMCM_FUNC_KIND_REQUEST) {
        return E_NO_ERROR;
    }

    //+Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    if (PQM_TEMP_PRE == GetScanProcedure()->GetTempMode()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_TEMP_PRE had executed"));

        CPqmPmAcqman* l_acqman_manager = GetScanProcedure()->GetAcqManager();
        CPqmPrMode* l_last_scan = l_acqman_manager->GetProtocolModeLastScan();

        if (NULL == l_last_scan) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_last_scan is NULL"));
            return E_ERROR;
        }

        // Ravindra Acharya - IR 153 - To be Checked
        PostProcess(l_last_scan->GetCouchPos(), l_last_scan->GetScanNumber() , AMCMR_OLPTEMP_MEASUREMENT) ;

    } else if (PQM_TEMP_SHIM == GetScanProcedure()->GetTempMode()) {
        CPqmPmAcqman* l_acqman_manager = GetScanProcedure()->GetAcqManager();
        CPqm* l_pqm = GetScanProcedure()->GetPqmPtr();

        //+Patni-Hemant/2010Oct25/Modified/NFD1_Merging-Part1
        GetScanProcedure()->SetTempMode(0) ;


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_TEMP_SHIM had executed"));
    }

    return E_NO_ERROR;
}
//-Patni-Ravindra Acharya/2010Jun09/Added/IR-153

//*******************************Method Header*********************************
//Method Name    : pqmAcqErrOLPTemp
//Author         : PATNI/Ravindra Acharya
//Purpose        :
//*****************************************************************************
//+Patni-Ravindra Acharya/2010June10/Added/IR-153
status_t COlpTemp::pqmAcqErrOLPTemp(const int f_type,
                                    const int f_error_id,
                                    const CString& f_errmsg,
                                    AM_AcqManToClient_t* f_response) const
{

    LPCTSTR FUNC_NAME = _T("COlpTemp::pqmAcqErrOLPTemp");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));
    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_type);
    UNREFERENCED_PARAMETER(f_error_id);
    UNREFERENCED_PARAMETER(f_response);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution

    //+Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    //+Hemant-CF Shift
    CPqmPmAcqman* l_acqman_manager = GetScanProcedure()->GetAcqManager();

    if (PQM_TEMP_PRE == GetScanProcedure()->GetTempMode()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_TEMP_PRE had executed"));

        l_acqman_manager->AcqmanAbort() ;

    } else if (PQM_TEMP_SHIM == GetScanProcedure()->GetTempMode()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_TEMP_SHIM had executed"));
        CFAReady() ;
    }

    //+Patni-Ravindra Achayra/2010Aug16/Commented/IR-153-Part1
    // DisplayMessage(f_errmsg) ;

    //+Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1
    DisplayMessage(f_errmsg, MPlus::EH::Crucial) ;
    //-Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1

    GetScanProcedure()->SetTempMode(0) ;
    //-Hemant-CF Shift
    //-Patni-KSS/2011March10/Modified/TMSC-Review-Comments

    return E_NO_ERROR;
}
//-Patni-Ravindra Acharya/2010June10/Added/IR-153




