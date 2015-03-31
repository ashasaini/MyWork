// SAEInfo.cpp: implementation of the CSAEInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SAEInfo.h"
#include <PQM/PQMLogMgr.h>

#include "pqm.h"
#include "pqmstudy.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "PQMDebugInfo.h"
#include "pqmAMB.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//***************************Method Header*************************************
//Method Name    : CSAEInfo()
//Author         : iGATE/AKR
//Purpose        : Default constructor
//*****************************************************************************
CSAEInfo::CSAEInfo(CScanProcedure* f_scan_proc):
    CScanTransaction(f_scan_proc),
    m_total_sae(0)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::CSAEInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSAEInfo ScanTransaction Created"));
}
//////////////////////////////////////////////////////////////////////
//***************************Method Header*************************************
//Method Name    : ~CSAEInfo()
//Author         : iGATE/AKR
//Purpose        : Default destructor
//*****************************************************************************
CSAEInfo::~CSAEInfo()
{

}

//***************************Method Header*************************************
//Method Name    : PrepareRequest
//Author         : iGATE/AKR
//Purpose        :
//*****************************************************************************
bool CSAEInfo::PrepareRequest()
{
    LPCTSTR FUNC_NAME = _T("CSAEInfo::PrepareRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_scan_procedure) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return false;
    }

    CPqm* l_pqm_ptr = m_scan_procedure->GetPqmPtr();

    if (NULL == l_pqm_ptr) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_ptr is NULL"));
        return false;
    }

    CPQMDebugInfo::GetInstance(l_pqm_ptr)->PqmDebugAcqMonitor(_T("AMCMM_RTSARCTRL(Total SAE) : %ld"), m_total_sae);

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    m_header.function = AMCMM_RTSARCTRL;
    m_data.param.rtSarCtrl.sae_total = m_total_sae;

    if (!SendMessage()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SendMessage failed, acqman is not connected"));

        l_pqm_ptr->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        return false;
    }

    return true;
}

//***************************Method Header*************************************
//Method Name    : SetTotalSAE
//Author         : iGATE/AKR
//Purpose        :
//*****************************************************************************
void CSAEInfo::SetTotalSAE(const int32_t f_total_sae)
{
    m_total_sae = f_total_sae;
}

//***************************Method Header*************************************
//Method Name    : ProcessResponse
//Author         : iGATE/AKR
//Purpose        :
//*****************************************************************************
void CSAEInfo::ProcessResponse(AM_ClientMessageHeader_t* f_header, AM_AcqManToClient_t* f_body)
{
    LPCTSTR FUNC_NAME = _T("CSAEInfo::ProcessResponse");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    if (NULL == f_header || NULL == f_body) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("f_header or f_body is NULL"));
        return;
    }

    CPqmPmAcqman* l_acqman_ptr = m_scan_procedure->GetAcqManager();

    int l_type = 0;

    int l_func_bit = l_acqman_ptr->GetFunctionBit(f_header->function);

    CString l_str_msg(_T(""));

    l_str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                     l_acqman_ptr->GetStr(f_body->status),
                     l_acqman_ptr->GetStr(l_type),
                     l_acqman_ptr->GetStr(f_header->function));

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_msg);

    if ((f_body->status == AMCM_SUCCESS) ||
        (f_body->status == AMCM_RECONST)) {

        if (f_header->function == AMCMR_RTSARCTRL) {

            l_type = AMCM_FUNC_KIND_REQUEST;
            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() | l_func_bit);

        } else if (f_header->function == AMCMA_RTSARCTRL_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() ^ l_func_bit);
            m_scan_procedure->SetRequest(m_scan_procedure->GetRequest() ^ l_func_bit);

            l_str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                             l_acqman_ptr->GetStr(f_body->status),
                             l_acqman_ptr->GetStr(l_type),
                             l_acqman_ptr->GetStr(f_header->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_msg);


            CPqm* l_pqm_ptr = m_scan_procedure->GetPqmPtr();

            if (l_pqm_ptr) {
                l_pqm_ptr->GetStudy()->SetRMSAEStatusForStudy(true);

                if (m_scan_procedure->GetIsScanning() && m_scan_procedure->GetAcqManager()->CheckScanTimeLimitation()) {

                    if (!m_scan_procedure->ContinueScanAfterSAESet()) {

                        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Cannot Connect to AcqMan"));

                        l_pqm_ptr->GetStudy()->SetScanModeOfHead();
                        m_scan_procedure->ResetCurrentRequest();
                        l_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                        l_pqm_ptr->GetAMB()->PqmPmAbortAmb();
                    }
                }
            }
        }

    } else {

        if (f_header->function == AMCMR_RTSARCTRL) {

            l_type = AMCM_FUNC_KIND_REQUEST;
            m_scan_procedure->SetRequest(m_scan_procedure->GetRequest() ^ l_func_bit);

        } else if (f_header->function == AMCMA_RTSAR_CALC_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() ^ l_func_bit);
            m_scan_procedure->SetRequest(m_scan_procedure->GetRequest() ^ l_func_bit);
        }

        l_str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                         l_acqman_ptr->GetStr(f_body->status),
                         l_acqman_ptr->GetStr(l_type),
                         l_acqman_ptr->GetStr(f_header->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_msg);


        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_MGR_SAE_ERROR")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));

        if (m_scan_procedure->GetIsScanning()) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("SAE Request is not set"));

            CPqm* l_pqm_ptr = m_scan_procedure->GetPqmPtr();

            if (l_pqm_ptr) {

                //m_scan_procedure->ResetCurrentRequest();
                l_pqm_ptr->GetStudy()->SetScanModeOfHead();
                l_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            }

        }

        m_scan_procedure->ResetCurrentRequest();

    }
}