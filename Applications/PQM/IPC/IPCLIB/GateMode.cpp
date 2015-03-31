//REDMINE-1213_Update_29_APR
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: GateMode.cpp
 *  Overview: Implementation of CGateMode class.
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
#include "GateMode.h"
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include "pqm.h"
#include "pqmstudy.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "GPLibAdapter.h"
//Patni-DKH/2010Jan25/Added/PH#3#/IR86/DebugWindow
#include "PQMDebugInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//***************************Method Header*************************************
//Method Name    : CGateMode()
//Author         : PATNI/HAR-LK
//Purpose        : Default constructor
//*****************************************************************************
CGateMode::CGateMode(
    CScanProcedure* f_scan_proc
) : CScanTransaction(f_scan_proc),
    m_pqm_ptr(NULL),
    m_acqman_ptr(NULL),
    m_gate_method(0),
    m_gate_method_onscan(false)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::CGateMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_scan_proc) {
        m_acqman_ptr = f_scan_proc->GetAcqManager();

        if (m_acqman_ptr) {
            m_pqm_ptr = m_acqman_ptr->GetPqm();
        }
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : ~CGateMode()
//Author         : PATNI/HAR-LK
//Purpose        : Default Destructor
//*****************************************************************************
CGateMode::~CGateMode()
{
    LPCTSTR FUNC_NAME = _T("CGateMode::~CGateMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));

}

//****************************Method Header************************************
//Method Name   :SetGateModeMethod()
//Author        :PATNI/SS
//Purpose       :
//*****************************************************************************
status_t CGateMode::AcqSetInfoGate(AM_AcqManGateInfo_t* f_gate)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::AcqSetInfoGate");
    static TCHAR*	labels[12] = {
        _T("Gating Method            : "),
        _T("Resp. Ins Av.               : "),
        _T("Resp. Exp Av.             : "),
        _T("Resp. Maximum         : "),
        _T("Resp. Minimum          : "),
        _T("ECG Average             : "),
        _T("ECG Maximum           : "),
        _T("ECG Minimum            : "),
        _T("Peripheral Up Av.       : "),
        _T("Peripheral Low Av.     : "),
        _T("Peripheral Maximum  : "),
        _T("Peripheral Minimum   : ")
    };
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    char		*data_label[12];
    int			cnt,datas[12] = {0};
    register int	i;
    */
    TCHAR*		data_label[12] = {0};
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    memset(data_label, 0, sizeof(TCHAR) * 12);

    //DB_FUNC_ENTER("[pqmAcqSetInfoGate]:");
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (!m_pqm_ptr->DeleteAllGateIteams()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DeleteAllGateIteams failed."));
        return E_ERROR;
    }

    if (f_gate == NULL) {
        return E_ERROR;
    }

    if (f_gate->method == 0) {

        m_pqm_ptr->AddToGateList(_T("Gating mode is not set on GM."));
        return E_ERROR;
    }

    TCHAR		buff[128] = {0};
    _tcscpy(buff, labels[0]);



    bool l_content_added = false;

    if ((f_gate->method & VFC_GAT_METHOD_BIT_RESPIRATORY) || (f_gate->method & VFC_GAT_METHOD_BIT_RESP_ECG) || (f_gate->method & VFC_GAT_METHOD_BIT_RESP_PERP)) {
        _tcscat(buff, _T(" RESPIRATORY "));
        l_content_added = true;
    }

    if ((f_gate->method & VFC_GAT_METHOD_BIT_ECG) || (f_gate->method &  VFC_GAT_METHOD_BIT_RESP_ECG)) {

        if (l_content_added) {

            _tcscat(buff, _T("+"));
        }

        _tcscat(buff, _T(" ECG"));

    }

    if ((f_gate->method & VFC_GAT_METHOD_BIT_PERIPHERAL) || (f_gate->method & VFC_GAT_METHOD_BIT_RESP_PERP)) {

        if (l_content_added) {
            _tcscat(buff, _T("+"));
        }

        _tcscat(buff, _T(" PERIPHERAL"));
    }


    CString l_gate_type(buff);
    m_pqm_ptr->AddToGateList(l_gate_type);

    int cnt = 0, datas[12] = {0};


    if ((f_gate->method & VFC_GAT_METHOD_BIT_RESPIRATORY) || (f_gate->method & VFC_GAT_METHOD_BIT_RESP_ECG) || (f_gate->method & VFC_GAT_METHOD_BIT_RESP_PERP)) {
        datas[cnt] = f_gate->g_resp_ins;
        data_label[cnt++] = labels[1];
        datas[cnt] = f_gate->g_resp_exp;
        data_label[cnt++] = labels[2];
        datas[cnt] = f_gate->g_max_resp;
        data_label[cnt++] = labels[3];
        datas[cnt] = f_gate->g_min_resp;
        data_label[cnt++] = labels[4];
    }

    if ((f_gate->method & VFC_GAT_METHOD_BIT_ECG) || (f_gate->method &  VFC_GAT_METHOD_BIT_RESP_ECG)) {
        datas[cnt] = f_gate->g_qrs;
        data_label[cnt++] = labels[5];
        datas[cnt] = f_gate->g_max_qrs;
        data_label[cnt++] = labels[6];
        datas[cnt] = f_gate->g_min_qrs;
        data_label[cnt++] = labels[7];
    }

    if ((f_gate->method & VFC_GAT_METHOD_BIT_PERIPHERAL) || (f_gate->method & VFC_GAT_METHOD_BIT_RESP_PERP)) {
        datas[cnt] = f_gate->g_per_rise;
        data_label[cnt++] = labels[8];
        datas[cnt] = f_gate->g_per_fall;
        data_label[cnt++] = labels[9];
        datas[cnt] = f_gate->g_max_per;
        data_label[cnt++] = labels[10];
        datas[cnt] = f_gate->g_min_per;
        data_label[cnt++] = labels[11];
    }

    //    DB_PRINTF("A",("f_gate->method=%d\n",f_gate->method));

    CString l_gate = _T("");
    int val = 0;

    for (int i = 0; i < cnt; i++) {

        l_gate_type.Empty();
        l_gate_type = data_label[i];
        val = datas[i];
        l_gate.Format(_T("%d"), val);
        l_gate_type += l_gate;
        m_pqm_ptr->AddToGateList(l_gate_type);
    }



    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    : PrepareRequest()
//Author         : PATNI/HAR-LK
//Purpose        : This method prepares the GateMode request that will be sent
//                  to Acqman
//*****************************************************************************
bool CGateMode::PrepareRequest(
)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::PrepareRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    // Set function
    m_header.function = AMCMM_GATEMODE;
    m_data.param.gateMode.method = m_gate_method;

    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("AMCMM_GATEMODE(METHOD) : %d"), m_gate_method);

    m_pqm_ptr->GetGateParamObj()->UpdateWirelessGateParam(m_data);



    const bool l_return_val = SendMessage();

    if (l_return_val) {
        m_scan_procedure->SetIsGateExecuting(true);
    }

    return l_return_val;
}

//****************************Method Header************************************
//Method Name   :SetGateModeMethod()
//Author        :PATNI/HAR-LK
//Purpose       :
//*****************************************************************************
void CGateMode::SetGateModeMethod(
    const int f_gate_method
)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::SetGateModeMethod");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_gate_method = f_gate_method;
}

//****************************Method Header************************************
//Method Name   :GateModeCallback()
//Author        :PATNI/HAR-LK
//Purpose       :
//*****************************************************************************
int CGateMode::GateModeCallback(
    const int f_type,
    AM_AcqManToClient_t* f_data
)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::GateModeCallback");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (f_type == AMCM_FUNC_KIND_REQUEST) {

        return E_NO_ERROR;
    }

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_scan_procedure) {

        if (AUTO_GATE_INTERVAL_NONE == m_pqm_ptr->GetPqmProCon()->GetAutoIntervalStatus()) {
            m_pqm_ptr->GetGateParamObj()->GateModeScanCallBack(f_data) ;

        } else {
            m_pqm_ptr->GetGateParamObj()->GateModeWSECallBack(f_data);
        }


        //MRP here 30-DEC-08
        if (E_NO_ERROR != AcqSetInfoGate(&f_data->gateInfo)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AcqSetInfoGate() failed."));
        }

        if (m_scan_procedure->GetIsScanning() && m_scan_procedure->GetAcqManager()->CheckScanTimeLimitation()) {

            m_scan_procedure->AcquisitionGateInfo(PQM_INFO_BIT_COILID | PQM_INFO_BIT_COUCH | PQM_INFO_BIT_RM | PQM_INFO_BIT_TEMP);
        }
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :ErrGateMode()
//Author        :PATNI/HAR-LK
//Purpose       :
//*****************************************************************************
bool CGateMode::ErrGateMode(
    const int f_type,
    const int f_eror_id,
    AM_AcqManToClient_t* f_response
)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::ErrGateMode");

    if (!m_pqm_ptr || !m_scan_procedure) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr or m_scan_procedure is NULL"));
        return false;
    }

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(f_eror_id);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProcon* pqm_procon_ptr = m_scan_procedure->GetPqmPtr()->GetPqmProCon();

    if (pqm_procon_ptr) {
        pqm_procon_ptr->ProlimGateInterval(m_gate_method);
        AcqSetInfoGate(NULL);
    }


    if (f_response->status == AMCM_GATEMETHOD) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_ACC_GATE_UNKNOWN, _T("IDS_ERR_ACC_GATE_UNKNOWN"), SCAN_ERR_CRUCIAL, L"GateMode.cpp");

    } else {
        //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //Patni-DKH/2010Jan11/Modified/TMSC-REDMINE-1098-Part2
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_ACC_GATE_INFO, _T("IDS_ERR_ACC_GATE_INFO"),
                SCAN_ERR_NOTIFY, L"GateMode.cpp");

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in Gate Mode Scan"));
        //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
    }



    //+Patni-ARD/2009-Nov-26/Added/DeFT# MVC003149 + MVC005179
    if ((f_response->status != AMCM_SUCCESS) &&
        (f_response->status != AMCM_RECONST)) {

        CString l_str_gate_msg = _T("");
        l_str_gate_msg.LoadString(f_eror_id);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_gate_msg);
    }

    //Patni-MSN/2010-Feb-02/Added/DeFT# PQM Hang issue
    if (GetGateModeMethodOnscan() && m_scan_procedure->IsSingleProcRunning()) {
        m_pqm_ptr->GetStudy()->SetScanModeOfHead();
        GetScanProcedure()->ResetCurrentRequest();
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
    }

    //Reset on Error
    m_scan_procedure->SetIsGateExecuting(0);
    m_scan_procedure->SetIsCoilOrGateInfoExecuting(false);


    return true;
}




//****************************Method Header************************************

//Method Name   :ProcessResponse()

//Author        :PATNI/HAR-LK

//Purpose       :This method will be called to process response received from AcqMan

//*****************************************************************************
void CGateMode::ProcessResponse(
    AM_ClientMessageHeader_t* f_header,
    AM_AcqManToClient_t* f_data
)
{
    LPCTSTR FUNC_NAME = _T("CGateMode::ProcessResponse");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr is NULL"));

        if (GetGateModeMethodOnscan()) {
            SetGateModeMethodOnscan(false);
        }

        return;
    }

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == f_header) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_header is NULL"));

        if (GetGateModeMethodOnscan()) {
            SetGateModeMethodOnscan(false);
        }

        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CScanProcedure* scan_proc = GetScanProcedure();
    m_header = *f_header;
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    int type = 0;
    const int funcBit = acqman->GetFunctionBit(f_header->function);
    CString str_msg(_T(""));

    if ((f_data->status == AMCM_SUCCESS) ||
        (f_data->status == AMCM_RECONST)) {

        if (f_header->function == AMCMR_GATEMODE) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);

            GateModeCallback(type, f_data);

        } else if (f_header->function == AMCMA_GATEMODE_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            GateModeCallback(type, f_data);

            //Reset on Error
            m_scan_procedure->SetIsGateExecuting(0);
            m_scan_procedure->SetIsCoilOrGateInfoExecuting(false);

            m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);

            if (GetGateModeMethodOnscan()) {
                SetGateModeMethodOnscan(false);
            }
        }

        str_msg.Format(_T("[Status: % s, Type: % s, Function: % s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    } else if (f_data->status == AMCM_NACK) {

        if (f_header->function == AMCMR_GATEMODE) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);


        } else if (f_header->function == AMCMA_GATEMODE_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);

        }

        str_msg.Format(_T("[Status: % s, Type: % s, Function: % s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        if (f_header->function == AMCMR_GATEMODE) {
            if (!ErrGateMode(type, IDS_ERR_ACQMAN_GATEMODE, f_data)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ErrGateMode() failed."));
            }

        } else if (f_header->function == AMCMA_GATEMODE_DONE) {
            if (!ErrGateMode(type, IDS_ERR_ACQMAN_GATEMODE_DONE, f_data)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ErrGateMode() failed."));
            }
        }

        if (GetGateModeMethodOnscan()) {
            SetGateModeMethodOnscan(false);
        }

    } else {

        if (f_header->function == AMCMR_GATEMODE) {

            type = AMCM_FUNC_KIND_REQUEST;

            //MRPdefect812
            scan_proc->SetRequest((scan_proc->GetRequest() ^ funcBit));
            //MRPdefect812

        } else if (f_header->function == AMCMA_GATEMODE_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("[f_header->function = AMCMA_GATEMODE_DONE && Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);
        }

        str_msg.Format(_T("[Status: % s, Type: % s, Function: % s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        if (f_header->function == AMCMR_GATEMODE) {
            if (!ErrGateMode(type, IDS_ERR_ACQMAN_GATEMODE, f_data)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ErrGateMode() failed."));
            }

        } else if (f_header->function == AMCMA_GATEMODE_DONE) {
            if (!ErrGateMode(type, IDS_ERR_ACQMAN_GATEMODE_DONE, f_data)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ErrGateMode() failed."));
            }
        }


        if (GetGateModeMethodOnscan()) {
            SetGateModeMethodOnscan(false);
        }
    }
}
