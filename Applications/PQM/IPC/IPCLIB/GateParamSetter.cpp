// GateParamSetter.cpp: implementation of the CGateParamSetter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GateParamSetter.h"
#include <tami/vfStudy/vf_appcodes.h>
#include "pqm.h"
#include "PqmPmAcqman.h"
#include "ScanProcedure.h"
#include "PQMDebugInfo.h"
#include "GPLibAdapter.h"
#include "PQMPmSARManager.h"

#define TIME_DECIMAL        1000
#define MIN_PERIOD          100
#define MAX_PERIOD          5000
#define MAX_RESP_PERIOD     20000

namespace
{

/**
 * Gate Single Data class for ECG and Peripheral
 */
class CGateSingleData
{
public:
    CGateSingleData()
        : m_current(0)
        , m_usable(0)
    {}

    ~CGateSingleData()
    {}

    void Reset() {
        m_current = 0;
        m_usable = 0;
    }

    void SetCurrentValue(const int32_t current) {
        m_current = current;
    }

    int32_t GetCurrentValue() const {
        return m_current;
    }

    int32_t GetUsableValue() const {
        return m_usable;
    }

    void UpdateUsableValue() {
        m_usable = m_current;
    }

    bool IsDefaultUsableValue() const {
        return (m_usable == 0) ? true : false;
    }

private:
    int32_t m_current;
    int32_t m_usable;
};

/**
 * Gate RespiratoryData class
 */
class CRespiratoryData
{
public:
    CRespiratoryData() {
        Reset();
    }

    ~CRespiratoryData()
    {}

    void Reset() {
        memset(m_current, 0, sizeof(m_current));
        memset(m_usable, 0, sizeof(m_usable));
    }

    void SetCurrentValue(const int32_t ins, const int32_t exp) {
        m_current[0] = ins;
        m_current[1] = exp;
    }

    void GetCurrentValue(int32_t* ins, int32_t* exp) const {
        *ins = m_current[0];
        *exp = m_current[1];
    }

    void GetUsableValue(int32_t* ins, int32_t* exp) const {
        *ins = m_usable[0];
        *exp = m_usable[1];
    }

    void UpdateUsableValue() {
        m_usable[0] = m_current[0];
        m_usable[1] = m_current[1];
    }

    bool IsDefaultUsableValue() const {
        return ((m_usable[0] == 0) && (m_usable[1] == 0)) ? true : false;
    }

private:
    int32_t m_current[2];
    int32_t m_usable[2];
};

} // namespace

/**
 * Gate Interval Data class
 * it just has ecg, peripheral and respiratory data.
 */
class CGateIntervalData
{
public:
    CGateIntervalData()
        : m_ecg(),
          m_peripheral(),
          m_respiratory() {
    }

    ~CGateIntervalData() {
    }

    CGateSingleData& GetECG() {
        return m_ecg;
    }

    CGateSingleData& GetPeripheral() {
        return m_peripheral;
    }

    CRespiratoryData& GetRespiratory() {
        return m_respiratory;
    }

private:
    CGateSingleData  m_ecg;
    CGateSingleData  m_peripheral;
    CRespiratoryData m_respiratory;
};

//****************************Method Header************************************
//Method Name   :CGateParamSetter
//Author        :
//Purpose       :
//*****************************************************************************
CGateParamSetter::CGateParamSetter()
    : m_pqm_ptr(NULL),
      m_auto_interval_scratch_pad(CPqmUtilities::GetAutoGetIntervalScratchPadPath()),
      m_wireless_param(),
      m_gate_data(new CGateIntervalData())
{
}

//****************************Method Header************************************
//Method Name   :~CGateParamSetter
//Author        :
//Purpose       :
//*****************************************************************************

CGateParamSetter::~CGateParamSetter()
{

    DEL_PTR(m_auto_interval_scratch_pad);
    DEL_PTR(m_gate_data);

}

//****************************Method Header************************************
//Method Name   :UpdateWirelessGateParam()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::UpdateWirelessGateParam(AM_ClientToAcqMan_unicode_t& f_data) const
{

    LPCTSTR FUNC_NAME = _T("CGateParamSetter::UpdateWirelessGateParam");

    if (m_pqm_ptr->IsWireLessSystem()) {

        f_data.param.gateMode.wireless.filter_cmd = m_wireless_param.m_command;
        f_data.param.gateMode.wireless.filter_mode = m_wireless_param.m_mode;
        f_data.param.gateMode.wireless.filter = m_wireless_param.m_wireless_filter;

        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("(CMD = %d, MODE = %d, FILTER = %d)"),
                m_wireless_param.m_command, m_wireless_param.m_mode, m_wireless_param.m_wireless_filter);

    }

    CString l_strmsg = _T("");
    l_strmsg.Format(_T("m_header.function = AMCMM_GATEMODE and gatemode method = % d , WirelessGatecmd = % d , WirelessGateMode = % d , WirelessGateFilter = % d "),
                    f_data.param.gateMode.method , m_wireless_param.m_command, m_wireless_param.m_mode, m_wireless_param.m_wireless_filter);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strmsg);

}

//****************************Method Header************************************
//Method Name   :GetUsableGateVal()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CGateParamSetter::GetUsableGateVal(const int f_method, int32_t* f_gate_val) const
{
    memset(f_gate_val, 0, MAX_GATE_VALS);

    int i = 0;

    if (this->HasEcgBit(f_method)) {
        f_gate_val[i++] = m_gate_data->GetECG().GetUsableValue();

    } else if (this->HasPeripheralBit(f_method)) {
        f_gate_val[i++] = m_gate_data->GetPeripheral().GetUsableValue();
    }

    if (this->HasRespiratoryBit(f_method)) {
        int32_t ins = 0;
        int32_t exp = 0;
        m_gate_data->GetRespiratory().GetUsableValue(&ins, &exp);
        f_gate_val[i++] = ins;
        f_gate_val[i++] = exp;
    }
}

//****************************Method Header************************************
//Method Name   :UpdateCurrentandUsableGateValforScan()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CGateParamSetter::UpdateCurrentandUsableGateValforScan(AM_AcqManToClient_t* f_data)
{
    LPCTSTR FUNC_NAME = _T("CGateParamSetter::UpdateCurrentandUsableGateValforScan");

    SetCurrentGateval(f_data->gateInfo);

    if (this->HasDefaultUsableValue(f_data->gateInfo.method)) {
        UpdateUsableValue(f_data->gateInfo.method, true);

        int32_t l_gate_val[MAX_GATE_VALS] = {0};
        this->GetUsableGateVal(f_data->gateInfo.method, l_gate_val);

        CString l_str(_T(""));
        l_str.Format(_T("Usable GateInterval for Gate-Type %d is %d,%d,%d"), f_data->gateInfo.method,
                     l_gate_val[0], l_gate_val[1], l_gate_val[2]);
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, l_str);

        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(l_str);
    }
}

//***************************Method Header*************************************
//Method Name    : WriteValidatedGateParametertoStudy
//Author         :
//Purpose        :
//*****************************************************************************
status_t CGateParamSetter::WriteValidatedGateParametertoStudy(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    const int f_gate_method
)
{
    LPCTSTR FUNC_NAME = _T("CGateParamSetter::WriteValidatedGateParametertoStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (E_NO_ERROR != CreateAutoIntervalScratchPad(f_protocol)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in creating AutoInterval Scratchpad..!!"));
        return E_ERROR;
    }

    if (E_ERROR == WriteParametersToScratchPad(f_gate_method, f_protocol, m_auto_interval_scratch_pad)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to write into AutoInterval Scratchpad..!!"));
        m_pqm_ptr->GetStudy()->SetScanModeOfHead();
        m_pqm_ptr->GetAcqManager()->GetScanProcedure()->ResetCurrentRequest();
        m_pqm_ptr->GetStudy()->SetAcqStatus(m_pqm_ptr->GetStudy()->GetProtocol(f_protocol), C_PR_STATUS_WAIT);
        m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

        CString l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_PROLIM_RELIMITING"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_string,
                MPlus::EH::Information, _T("CAutoInsertProtocolManager.cpp"), _T("PQM"));

        DeleteAutoIntervalScratchPad(m_auto_interval_scratch_pad);
        return E_ERROR;
    }

    if (E_NO_ERROR != WriteWirelessGateParamtoStudy(f_vf_handle, f_protocol)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to write WirelessParam to Study"));
    }

    m_pqm_ptr->GetStudy()->UpdateSARControlInfoOnProtocolUpdate(m_pqm_ptr->GetStudy()->GetProtocol(f_protocol));

    DeleteAutoIntervalScratchPad(m_auto_interval_scratch_pad);

    return E_NO_ERROR;
}


//****************************Method Header************************************
//Method Name   :GateModeScanCallBack()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::GateModeScanCallBack(
    AM_AcqManToClient_t* f_data
)
{

    LPCTSTR FUNC_NAME = _T("CGateParamSetter::GateModeScanCallBack");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr is NULL..."));
        return;
    }

    UpdateCurrentandUsableGateValforScan(f_data);

    CPqmPmAcqman* const l_acqman = m_pqm_ptr->GetAcqManager();
    CScanProcedure* const l_scan_proc = l_acqman->GetScanProcedure();
    CPqmProtocol* l_current_prot = l_scan_proc->GetScanProcProtocol();

    if (!l_scan_proc->GetIsScanning() || (NULL == l_current_prot)) {
        return ;
    }

    if (0 != l_current_prot->GetGatingMethod()) {

        if (l_current_prot->GetAutoGetIntervalFlag()) {

            if (E_NO_ERROR != WriteValidatedGateParametertoStudy(
                    m_pqm_ptr->GetStudy()->GetVfDBHandle(),
                    l_current_prot->GetProtocol(), f_data->gateInfo.method)) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get Write ValidatedParameters to Study"));
            }

            m_pqm_ptr->GetStudy()->GetProtocolItemValues(m_pqm_ptr->GetStudy()->GetVfDBHandle(), l_current_prot);
            m_pqm_ptr->WriteToUI(PQM_MSG_CHANGE_SCAN_TIME);
            m_pqm_ptr->GetSARManager()->CalcSARControlAndPutVFForAll();
        }
    }
}

//****************************Method Header************************************
//Method Name   :GateModeWSECallBack()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::GateModeWSECallBack(
    AM_AcqManToClient_t* f_data
)
{

    LPCTSTR FUNC_NAME = _T("CGateParamSetter::GateModeWSECallBack");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_pqm_ptr) {
        CPqmProcon* pqm_procon_ptr = m_pqm_ptr->GetPqmProCon();

        if (pqm_procon_ptr) {
            SetCurrentGateval(f_data->gateInfo);
            pqm_procon_ptr->ProlimGateInterval(f_data->gateInfo.method);
        }
    }
}

//****************************Method Header************************************
//Method Name   :HasDefaultUsableValue()
//Author        :
//Purpose       :
//*****************************************************************************
bool CGateParamSetter::HasDefaultUsableValue(const int f_method) const
{
    if (this->HasEcgBit(f_method) && m_gate_data->GetECG().IsDefaultUsableValue()) {
        return true;
    }

    if (this->HasPeripheralBit(f_method) && m_gate_data->GetPeripheral().IsDefaultUsableValue()) {
        return true;
    }

    if (this->HasRespiratoryBit(f_method) && m_gate_data->GetRespiratory().IsDefaultUsableValue()) {
        return true;
    }

    return false;
}

//****************************Method Header************************************
//Method Name   :UpdateUsableValue()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::UpdateUsableValue(const int f_method,
        const bool f_update_default_val_only)
{

    if (this->HasEcgBit(f_method)) {
        if (!f_update_default_val_only ||
            (f_update_default_val_only && m_gate_data->GetECG().IsDefaultUsableValue())) {
            m_gate_data->GetECG().UpdateUsableValue();
        }

    } else if (this->HasPeripheralBit(f_method)) {
        if (!f_update_default_val_only ||
            (f_update_default_val_only && m_gate_data->GetPeripheral().IsDefaultUsableValue())) {
            m_gate_data->GetPeripheral().UpdateUsableValue();
        }
    }

    if (this->HasRespiratoryBit(f_method)) {
        if (!f_update_default_val_only ||
            (f_update_default_val_only && m_gate_data->GetRespiratory().IsDefaultUsableValue())) {
            m_gate_data->GetRespiratory().UpdateUsableValue();
        }
    }
}


//****************************Method Header************************************
//Method Name   :ResetGateInterval()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::ResetGateInterval(
)
{
    LPCTSTR FUNC_NAME = _T("CGateParamSetter::ResetGateInterval");

    m_gate_data->GetECG().Reset();
    m_gate_data->GetPeripheral().Reset();
    m_gate_data->GetRespiratory().Reset();

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("********Gate-Interval is Reset********"));
}

//****************************Method Header************************************
//Method Name   :SetPQM()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::SetPQM(
    CPqm* const f_pqm
)
{
    m_pqm_ptr = f_pqm;
}


//****************************Method Header************************************
//Method Name   :GetWirelessGateParam()
//Author        :
//Purpose       :
//*****************************************************************************
Wireless_gating_t* const CGateParamSetter::GetWirelessGateParam(
)
{
    return &m_wireless_param;
}

//****************************Method Header************************************
//Method Name   :SetWirelessGateParam()
//Author        :
//Purpose       :
//*****************************************************************************
void CGateParamSetter::SetWirelessGateParam(
    Wireless_gating_t* const f_wirelessgateinfo
)
{
    if (f_wirelessgateinfo) {
        m_wireless_param = *f_wirelessgateinfo;
    }
}

//***************************Method Header*************************************
//Method Name    : CreateAutoIntervalScratchPad
//Author         : PATNI
//Purpose        :
//*****************************************************************************
status_t CGateParamSetter::CreateAutoIntervalScratchPad(const int f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CGateParamSetter::CreateAutoIntervalScratchPad");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    FILE* l_file_pointer = fopen(m_auto_interval_scratch_pad, "a");

    if (l_file_pointer) {
        fclose(l_file_pointer);
        l_file_pointer = NULL;
    }

    return (m_pqm_ptr->GetStudy()->MaintainScratchPadForPrivateProlim(f_protocol, m_auto_interval_scratch_pad, SP_CREATE_MODE_PRIVATE_PROLIM));
}

//***************************Method Header*************************************
//Method Name    : DeleteAutoIntervalScratchPad
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CGateParamSetter::DeleteAutoIntervalScratchPad(char* const f_file_name)const
{
    LPCTSTR FUNC_NAME = _T("CGateParamSetter::DeleteAutoIntervalScratchPad");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    WIN32_FIND_DATA FindFileData;

    CString l_str_filename(f_file_name);
    HANDLE hFindFile = FindFirstFile(l_str_filename, &FindFileData);

    if (INVALID_HANDLE_VALUE != hFindFile) {
        DeleteFile(l_str_filename);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not find the file for deletion"));
    }

    FindClose(hFindFile);
    hFindFile = NULL;
}

//***************************Method Header*************************************
//Method Name    : WriteParametersToScartchPad
//Author         : PATNI
//Purpose        :
//*****************************************************************************
status_t CGateParamSetter::WriteParametersToScratchPad(const int f_gate_method, const int f_protocol, char* const m_auto_interval_scratch_pad) const
{
    LPCTSTR FUNC_NAME = _T("CGateParamSetter::WriteParametersToScratchPad");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CLibPLTreeWrapper l_obj_lib_wrapp(m_pqm_ptr->GetStudy()->GetVfDBHandle());

    VfPathElem_t l_path[3] = {0};
    VfFieldSpec_t l_fieldSpec[1] = {0};
    int l_num = 0, l_numDone = 0;

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = 0;
    l_path[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[SVD_ST_SUBPROTOCOL - 1].index = 0;

    CVarFieldHandle l_varfield_handle((CString)m_auto_interval_scratch_pad);

    if (l_varfield_handle.VuiOpen() != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not open Scratch pad"));
        return E_ERROR;
    }

    status_t l_status = E_NO_ERROR;
    time_t l_ts = 0;

    if ((VFC_GAT_METHOD_BIT_ECG == f_gate_method) ||
        (VFC_GAT_METHOD_BIT_RESPIRATORY == f_gate_method) ||
        (VFC_GAT_METHOD_BIT_RESP_ECG == f_gate_method)) {

        int32_t l_wirless_gate_cmd;

        VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_WIRELESS_GATE_TRIG_FILTER_CMD, SVT_PLN_WIRELESS_GATE_TRIG_FILTER_CMD,
                     sizeof(l_wirless_gate_cmd), &(l_wirless_gate_cmd), 0);
        l_num++;

        l_status = l_varfield_handle.GetFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec, l_num, &l_numDone);

        if (l_num != l_numDone) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfGetFields failed to get fields from varfield"));
            return	E_ERROR;
        }

        l_wirless_gate_cmd = m_wireless_param.m_command;

        l_num = 0;
        l_numDone = 0;

        VfFieldSpec_t l_putfieldSpec[1] = {0};


        VFF_SET_ARGS(l_putfieldSpec[l_num], SVN_PLN_WIRELESS_GATE_TRIG_FILTER_CMD, SVT_PLN_WIRELESS_GATE_TRIG_FILTER_CMD,
                     sizeof(l_wirless_gate_cmd), &l_wirless_gate_cmd, VFO_REPLACE);
        l_num++;

        l_status = l_varfield_handle.PutFields(l_path, SVD_ST_PROTOCOL, l_putfieldSpec, l_num, &l_numDone, &l_ts);

        if (l_status != E_NO_ERROR || l_num != l_numDone) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetParamCurrValue failed to get Gate tags from scratchpad"));
            return E_ERROR;
        }

    }

    VftGatingParam_s l_gating_param;
    VfPathElem_t l_gatepath[1] = {0};
    VfFieldSpec_t l_gatefieldSpec[1] = {0};
    int l_gatenum = 0, l_gatenumDone = 0;

    l_gatepath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_gatepath[SVD_ST_PROTOCOL - 1].index = 0;
    l_gatepath[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_gatepath[SVD_ST_SUBPROTOCOL - 1].index = 0;


    VFF_SET_ARGS(l_gatefieldSpec[l_gatenum], SVN_PLN_GATE_PARAM, SVT_PLN_GATE_PARAM,
                 sizeof(l_gating_param), &(l_gating_param), 0);
    l_gatenum++;

    l_status = l_varfield_handle.GetFields(l_gatepath, SVD_ST_PROTOCOL, l_gatefieldSpec, l_gatenum, &l_gatenumDone);

    if (E_NO_ERROR != l_status || l_gatenum != l_gatenumDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfGetFields failed to get fields from varfield"));
        return	E_ERROR;
    }


    UpdateGateInterval(l_gating_param, f_gate_method);

    l_gatenum = 0;
    l_gatenumDone = 0;
    l_ts = 0;

    VfFieldSpec_t l_putgatefieldSpec[1] = {0};

    VFF_SET_ARGS(l_putgatefieldSpec[l_gatenum], SVN_PLN_GATE_PARAM, SVT_PLN_GATE_PARAM,
                 sizeof(l_gating_param), &l_gating_param, VFO_REPLACE);

    l_gatenum++;

    l_status = l_varfield_handle.PutFields(l_path, SVD_ST_PROTOCOL, l_putgatefieldSpec, l_gatenum, &l_gatenumDone, &l_ts);

    if (E_NO_ERROR != l_status || l_gatenum != l_gatenumDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfGetFields failed to get fields from varfield"));
        return	E_ERROR;
    }

    if (!(l_obj_lib_wrapp.ExecutePlUpdateWrapper(m_pqm_ptr->GetStudy(), f_protocol, m_auto_interval_scratch_pad))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Prolim library loading get failed.."));
        return E_ERROR;
    }

    return l_status;

}

//***************************Method Header*************************************
//Method Name    : WriteWirelessGateParamtoStudy
//Author         : PATNI
//Purpose        :
//*****************************************************************************
status_t CGateParamSetter::WriteWirelessGateParamtoStudy(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol
) const
{

    LPCTSTR FUNC_NAME = _T("CGateParamSetter::WriteWirelessGateParamtoStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL ..!!"));
        return E_ERROR;
    }

    ClibVfToUiAdapter vuiAdapter(f_vf_handle);

    status_t l_status = vuiAdapter.VuiCopyProtocolFromPrivateProlimScratchPad(f_protocol, m_auto_interval_scratch_pad, 0, NULL, NULL);

    if (E_ERROR == l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not Update the Values to the Study file"));
    }

    if (E_ERROR == m_pqm_ptr->GetStudy()->RestoreSavedTagsAfterScratchpad(f_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("RestoreSavedTagsAfterScratchpad failed to restore SVN tags value in varfield"));
    }

    return l_status;


}

//***************************Method Header*************************************
//Method Name    : UpdateGateInterval
//Author         :
//Purpose        :
//*****************************************************************************
void CGateParamSetter::UpdateGateInterval(
    VftGatingParam_s& f_gating_param,
    const int f_gate_method
) const
{

    LPCTSTR FUNC_NAME = _T("CGateParamSetter::Updategateparam");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    flt32_t gatingPeriod = f_gating_param.gatingPeriod;
    int32_t imagPeriod = f_gating_param.imagPeriod;
    int32_t respRate = f_gating_param.respRate;

    bool check = false;
    int32_t l_gate_val[MAX_GATE_VALS] = {0};
    this->GetUsableGateVal(f_gate_method, l_gate_val);

    flt32_t period = (flt32_t)l_gate_val[0];
    flt32_t respPeriod = 0.0;

    switch (f_gate_method) {


        case VFC_GAT_METHOD_BIT_ECG:
        case VFC_GAT_METHOD_BIT_PERIPHERAL:
            period = (flt32_t)l_gate_val[0];
            check = (period < MIN_PERIOD || period > MAX_PERIOD);
            gatingPeriod = period / TIME_DECIMAL;
            break;

        case VFC_GAT_METHOD_BIT_RESP_ECG:
        case VFC_GAT_METHOD_BIT_RESP_PERP:
            period = (flt32_t)l_gate_val[0];
            respPeriod = (flt32_t)(l_gate_val[1] + l_gate_val[2]);
            check = (period < MIN_PERIOD || period > MAX_PERIOD);

            if (respPeriod < MIN_PERIOD || respPeriod > MAX_RESP_PERIOD)
                check = true;

            else if (l_gate_val[1] < MIN_PERIOD)
                check = true;

            else if (l_gate_val[2] < MIN_PERIOD)
                check = true;

            gatingPeriod = period / TIME_DECIMAL;
            imagPeriod = l_gate_val[1];
            respRate   = l_gate_val[2];
            break;

        case VFC_GAT_METHOD_BIT_RESPIRATORY:
            respPeriod = (flt32_t)(l_gate_val[0] + l_gate_val[1]);

            if (respPeriod < MIN_PERIOD || respPeriod > MAX_RESP_PERIOD)
                check = true;

            else if (l_gate_val[0] < MIN_PERIOD)
                check = true;

            else if (l_gate_val[1] < MIN_PERIOD)
                check = true;

            gatingPeriod = respPeriod / TIME_DECIMAL;
            imagPeriod = l_gate_val[0];
            respRate   = l_gate_val[1];
            break;

    }

    if (!check) {

        f_gating_param.gatingPeriod = gatingPeriod;
        f_gating_param.imagPeriod = imagPeriod;
        f_gating_param.respRate = respRate;


    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid gate intervals failed to Write in study"));

    }

}

//****************************Method Header************************************
//Method Name   :SetCurrentGateval()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CGateParamSetter::SetCurrentGateval(const AM_AcqManGateInfo_t& f_gate_data)
{

    LPCTSTR FUNC_NAME = _T("CGateParamSetter::SetCurrentGateval");

    int32_t l_gate_val[MAX_GATE_VALS] = {0};
    int i = 0;

    if (this->HasEcgBit(f_gate_data.method)) {
        m_gate_data->GetECG().SetCurrentValue(f_gate_data.g_qrs);
        l_gate_val[i++] = f_gate_data.g_qrs;

    } else if (this->HasPeripheralBit(f_gate_data.method)) {

        m_gate_data->GetPeripheral().SetCurrentValue(f_gate_data.g_per_rise);
        l_gate_val[i++] = f_gate_data.g_per_rise;
    }

    if (this->HasRespiratoryBit(f_gate_data.method)) {

        m_gate_data->GetRespiratory().SetCurrentValue(f_gate_data.g_resp_ins, f_gate_data.g_resp_exp);
        l_gate_val[i++] = f_gate_data.g_resp_ins;
        l_gate_val[i++] = f_gate_data.g_resp_exp;
    }

    CString l_str(_T(""));
    l_str.Format(_T("Current GateInterval from Acqman for Gate-Type %d is %d,%d,%d"),
                 f_gate_data.method, l_gate_val[0], l_gate_val[1], l_gate_val[2]);
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, l_str);

    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(l_str);
}

//****************************Method Header************************************
//Method Name   :HasEcgBit()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
bool CGateParamSetter::HasEcgBit(const int f_method) const
{
    return (f_method & VFC_GAT_METHOD_BIT_ECG ||
            f_method & VFC_GAT_METHOD_BIT_RESP_ECG) ? true : false;
}

//****************************Method Header************************************
//Method Name   :HasPeripheralBit()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
bool CGateParamSetter::HasPeripheralBit(const int f_method) const
{
    return (f_method & VFC_GAT_METHOD_BIT_PERIPHERAL ||
            f_method & VFC_GAT_METHOD_BIT_RESP_PERP) ? true : false;
}

//****************************Method Header************************************
//Method Name   :HasRespiratoryBit()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
bool CGateParamSetter::HasRespiratoryBit(const int f_method) const
{
    return (f_method & VFC_GAT_METHOD_BIT_RESPIRATORY ||
            f_method & VFC_GAT_METHOD_BIT_RESP_EDGE ||
            f_method & VFC_GAT_METHOD_BIT_RESP_LEVEL ||
            f_method & VFC_GAT_METHOD_BIT_RESP_ECG ||
            f_method & VFC_GAT_METHOD_BIT_RESP_PERP) ? true : false;
}
