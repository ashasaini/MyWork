//Redmine-783
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Electrode.cpp
 *  Overview: Structure of CElectrode class.
 *  Last Modified: 2008/8/11 15:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/8/11 15:00:00     Implemented SM3 requirements & Formatted
 *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Electrode.h"
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include "pqm.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//*******************************Method Header*********************************
//Method Name    : CElectrode()
//Author         : PATNI/Ashish SM3
//Purpose        : Constructor
//*****************************************************************************
CElectrode::CElectrode(CScanProcedure* f_scan_proc):
    CScanTransaction(f_scan_proc),
    m_nelectrode_type(-1)
{
    LPCTSTR FUNC_NAME = _T("CElectrode::CElectrode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));

    if (f_scan_proc != NULL) {
        m_acqman_ptr = f_scan_proc->GetAcqManager();
    }

    if (m_acqman_ptr) {		//Patni-MJC/2009Aug17/Modified/CPP test corrections
        m_pqm_ptr = m_acqman_ptr->GetPqm();
    }
}

//*******************************Method Header*********************************
//Method Name    : ~CElectrode()
//Author         : PATNI/Ashish SM3
//Purpose        : Destructor
//*****************************************************************************
CElectrode::~CElectrode()
{
    LPCTSTR FUNC_NAME = _T("CElectrode::~CElectrode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//****************************Method Header************************************
//Method Name   : PrepareRequest()
//Author        : PATNI/Ashish SM3
//Purpose       : Prepare request by setting the action and request before sending it to Acqman.
//*****************************************************************************
bool CElectrode::PrepareRequest()
{
    LPCTSTR FUNC_NAME = _T("CElectrode::PrepareRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    // Set function
    m_header.function = AMCMM_ECG_ELECTRODE;

    struct {
        int     button;
        AM_ECGElectrode_t     cmd;
    } cmd_tbl[] = {
        { 1,  ACQ_ECG_ELECTRODE_1 } ,
        { 2,  ACQ_ECG_ELECTRODE_2 } ,
        { 3,  ACQ_ECG_ELECTRODE_3 } ,
        { 4, ACQ_ECG_ELECTRODE_AVR } ,
        { 5, ACQ_ECG_ELECTRODE_AVL } ,
        { 6, ACQ_ECG_ELECTRODE_AVF }
    } ;

    CString str_msg(_T(""));

    for (int i = 0 ; i < sizeof(cmd_tbl) / sizeof(*cmd_tbl) ; i++) {
        if (cmd_tbl[i].button == m_nelectrode_type) {

            m_data.param.ECGElectrode.config = cmd_tbl[i].cmd;

            str_msg.Format(L"m_header.function = AMCMM_ECG_ELECTRODE(%s)", GetString(cmd_tbl[i].cmd));

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

            if (SendMessage())
                return true;
        }
    }

    return false;
}

//****************************Method Header************************************
//Method Name   : SetElectrodeType()
//Author        : PATNI/Ashish SM3
//Purpose       :
//*****************************************************************************
void CElectrode::SetElectrodeType(const int f_electrode_type)
{
    m_nelectrode_type = f_electrode_type;
}

//****************************Method Header************************************
//Method Name   :ErrElectrode()
//Author        :PATNI/Ashish SM3
//Purpose       :
//*****************************************************************************
void CElectrode::ErrElectrode(
    const int f_type,
    const int f_error_id,
    const CString& f_errrmsg,					// Added By KT/ACE-2/15-06-2009/Multilingualisation
    AM_AcqManToClient_t* f_response,
    AM_ClientMessageHeader_t* f_header
)
{
    LPCTSTR FUNC_NAME = _T("CElectrode::ErrElectrode");
    CScanProcedure* l_scan_proc = GetScanProcedure();

    if (f_header) {
        m_header = *f_header;

    } else  {
        //+Patni-RUP/2009Aug20/Modified/cpp test corrections
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_header pointer is NULL"));
        return;
    }

    CPqmPmAcqman* l_acqman = l_scan_proc->GetAcqManager();

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CString str_msg(_T(""));

    str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                   l_acqman->GetStr(f_response->status),
                   l_acqman->GetStr(f_type),
                   l_acqman->GetStr(f_header->function));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    CPQMLogMgr::GetInstance()->DisplayScanError(
        f_error_id,
        f_errrmsg,						// Added By KT/ACE-2/15-06-2009/Multilingualisation
        //Patni-DKH/2010Jan06/Added/TMSC-REDMINE-1098-Part2
        //SCAN_ERR_CRUCIAL,
        SCAN_ERR_NOTIFY,
        L"Electrode.cpp");

    //return E_NO_ERROR ;
}

//****************************Method Header************************************
//Method Name   :ElectrodeCallback()
//Author        :PATNI/Ashish SM3
//Purpose       :
//*****************************************************************************
void CElectrode::ElectrodeCallback(
    const int f_type,
    AM_AcqManToClient_t* f_data,
    AM_ClientMessageHeader_t* f_header
)
{
    LPCTSTR FUNC_NAME = _T("CElectrode::ElectrodeCallback");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CScanProcedure* l_scan_proc = GetScanProcedure();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //CPqmPmAcqman* l_acqman;
    CPqmPmAcqman* l_acqman = 0;

    if (l_scan_proc) {
        l_acqman = l_scan_proc->GetAcqManager();

    } else {
        return;
    }

    if (f_header) {
        m_header = *f_header;

    } else {
        return;
    }

    CString str_msg(_T(""));
    str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                   l_acqman->GetStr(f_data->status),
                   l_acqman->GetStr(f_type),
                   l_acqman->GetStr(f_header->function));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

}

//****************************Method Header************************************
//Method Name   : ProcessResponse()
//Author        : PATNI/Ashish SM3
//Purpose       : Process the incoming response coming from Acqman
//*****************************************************************************
void CElectrode::ProcessResponse(
    AM_ClientMessageHeader_t* f_header,
    AM_AcqManToClient_t* f_data
)
{
    LPCTSTR FUNC_NAME = _T("CElectrode::ProcessResponse");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CScanProcedure* scan_proc = GetScanProcedure();

    if (f_header) {
        m_header = *f_header;
    }

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_header pointer is NULL"));
        return;
    }

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections

    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    //CPqm *pqm = acqman->GetPqm();

    int type = 0;
    int funcBit = acqman->GetFunctionBit(f_header->function);

    CString str_msg(_T(""));
    str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                   acqman->GetStr(f_data->status),
                   acqman->GetStr(type),
                   acqman->GetStr(f_header->function));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    if ((f_data->status == AMCM_SUCCESS) ||
        (f_data->status == AMCM_RECONST)) {

        if (f_header->function == AMCMR_ECG_ELECTRODE) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);

            ElectrodeCallback(type, f_data, f_header);

        } else if (f_header->function == AMCMA_ECG_ELECTRODE_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            ElectrodeCallback(type, f_data, f_header);

            // + LOKESH Intgr 5 Bug Fix
            //m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);
            // - LOKESH Intgr 5 Bug Fix

        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    } else if (f_data->status == AMCM_NACK) {

        if (f_header->function == AMCMR_ECG_ELECTRODE) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

        } else if (f_header->function == AMCMA_ECG_ELECTRODE_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
            // + LOKESH Intgr 5 Bug Fix
            //m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);
            // - LOKESH Intgr 5 Bug Fix
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        if (f_header->function == AMCMR_ECG_ELECTRODE) {
            ErrElectrode(type, IDS_ERR_ACQMAN_ECG_ELECTRODE, _T("IDS_ERR_ACQMAN_ECG_ELECTRODE"), f_data, f_header);				// Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (f_header->function == AMCMA_ECG_ELECTRODE_DONE) {
            ErrElectrode(type, IDS_ERR_ACQMAN_ECG_ELECTRODE_DONE, _T("IDS_ERR_ACQMAN_ECG_ELECTRODE_DONE"), f_data, f_header);		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

    } else {

        if (f_header->function == AMCMR_ECG_ELECTRODE) {

            type = AMCM_FUNC_KIND_REQUEST;
            int result = scan_proc->GetRequest();
            result ^= funcBit;
            //MRPdefect812
            scan_proc->SetRequest(result);
            //MRPdefect812

        } else if (f_header->function == AMCMA_ECG_ELECTRODE_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("[f_header->function = AMCMA_ECG_ELECTRODE_DONE && Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
            // + LOKESH Intgr 5 Bug Fix
            //m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);
            // - LOKESH Intgr 5 Bug Fix
        }

        if (f_header->function == AMCMR_ECG_ELECTRODE) {
            ErrElectrode(type, IDS_ERR_ACQMAN_ECG_ELECTRODE, _T("IDS_ERR_ACQMAN_ECG_ELECTRODE"), f_data, f_header);				// Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (f_header->function == AMCMA_ECG_ELECTRODE_DONE) {
            ErrElectrode(type, IDS_ERR_ACQMAN_ECG_ELECTRODE_DONE, _T("IDS_ERR_ACQMAN_ECG_ELECTRODE_DONE"), f_data, f_header);		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        if (scan_proc->GetProcessing()) {
            //  scan_proc->ResetCurrentRequest();
        }
    }
}

//****************************Method Header************************************
//Method Name   :ErrElectrode()
//Author        :PATNI/Ashish SM3
//Purpose       :Return the selected Electrode in form of string
//*****************************************************************************
CString CElectrode::GetString(
    const int f_id
)const
{
    CString str_ECGMsg(_T(""));

    switch (f_id) {
        case 1:
            str_ECGMsg = _T("ACQ_ECG_ELECTRODE_1");
            break;

        case 2:
            str_ECGMsg = _T("ACQ_ECG_ELECTRODE_2");
            break;

        case 3:
            str_ECGMsg = _T("ACQ_ECG_ELECTRODE_3");
            break;

        case 4:
            str_ECGMsg = _T("ACQ_ECG_ELECTRODE_AVR");
            break;

        case 5:
            str_ECGMsg = _T("ACQ_ECG_ELECTRODE_AVL");
            break;

        case 6:
            str_ECGMsg = _T("ACQ_ECG_ELECTRODE_AVF");
            break;
    }

    return str_ECGMsg;
}

