// WirelessGatingController.cpp: implementation of the CWirelessGatingController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WirelessGatingController.h"
#include <tami/vfStudy/vf_appcodes.h>
#include <PDataParser/INIReader.h>
#include "PQMView.h"
#include "PQMWirelessGating.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWirelessGatingController::CWirelessGatingController(
) : m_pqmview(NULL),
    m_pqmwirelessgating_dlg(NULL),
    m_is_pqmwirelessgating_window_opened_before_swithcing_page(false)
{
}

CWirelessGatingController::~CWirelessGatingController()
{
}

void CWirelessGatingController::OnClickMenuitemWirelessGating(
)
{
    LPCTSTR FUNC_NAME = _T("CWirelessGatingController::OnClickMenuitemWirelessGating");
    PQM_TRACE_SCOPED(FUNC_NAME);

    s_wireless_gating l_wireless_gating;
    GetWirelessGatingSelections(&l_wireless_gating);

    m_pqmwirelessgating_dlg = new CPQMWirelessGating(&l_wireless_gating, NULL);
    m_pqmview->ChangeViewOfModelessDialogs(false);

    if (IDOK == m_pqmwirelessgating_dlg->DoModal()) {

        VARIANT l_pdata;
        l_pdata.byref = &l_wireless_gating;

        if (S_OK != m_pqmview->GetAcquisition()->SendWirelessGateParam(&l_pdata)) {

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Failed to SendWirelessGateParam"));
        }
    }

    m_pqmview->ChangeViewOfModelessDialogs(true);

    DEL_PTR(m_pqmwirelessgating_dlg)

}

void CWirelessGatingController::ShowHideOpenedWindows(
    const bool f_show /*= true*/
)
{

    if (m_pqmwirelessgating_dlg) {
        if (::IsWindow(m_pqmwirelessgating_dlg->m_hWnd)) {

            if (!f_show) {
                m_is_pqmwirelessgating_window_opened_before_swithcing_page = m_pqmwirelessgating_dlg->IsWindowVisible() ? true : false;
            }

            m_pqmwirelessgating_dlg->ShowWindow((f_show && m_is_pqmwirelessgating_window_opened_before_swithcing_page) ? SW_SHOW : SW_HIDE);
        }
    }
}

void CWirelessGatingController::UpdateOKBtnStatus()
{

    LPCTSTR FUNC_NAME = _T("CWirelessGatingController::UpdateOKBtnStatus") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    if ((m_pqmwirelessgating_dlg) && (::IsWindow(m_pqmwirelessgating_dlg->m_hWnd))) {

        m_pqmwirelessgating_dlg->UpdateOKBtnStatus(CanEnableWirelessDlgOKBtn());
    }
}


void CWirelessGatingController::InitializeWirelessParametersWithConfigDefault(
)
{
    SetWirelessDefaultParamtoIPC(m_wireless_default_config_param);
}
CPQMWirelessGating* CWirelessGatingController::GetWirelessGatingDialog()const
{
    return m_pqmwirelessgating_dlg;
}

void CWirelessGatingController::SetPQMView(
    CPQMView* f_pqmview
)
{
    m_pqmview = f_pqmview ;
}

//************************************Method Header************************************
// Method Name  : CanEnableWirelessDlgOKBtn
// Author       :
// Purpose      :
//***********************************************************************************
bool CWirelessGatingController::CanEnableWirelessDlgOKBtn(
)
{
    return (m_pqmview->IsAcqumanReadyForNextRequest() &&
            (!m_pqmview->IsWaitCursor()) &&
            (!m_pqmview->IsScanning()) &&
            (-1 == m_pqmview->GetScanIndex()) &&
            (!m_pqmview->IsRMCRunning()) &&
            (!m_pqmview->GetPostProcessStatus()));

}


//************************************Method Header************************************
// Method Name  : GetWirelessGatingSelections
// Author       :
// Purpose      :
//***********************************************************************************
bool CWirelessGatingController::GetWirelessGatingSelections(
    s_wireless_gating* const f_wireless_gating
)const
{

    LPCTSTR FUNC_NAME = _T("CWirelessGatingController::GetWirelessGatingSelections");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VARIANT l_data;

    if (S_OK != m_pqmview->GetAcquisition()->GetWirelessGatingSelections(&l_data)) {
        return false;
    }

    Wireless_gating_t* l_wirelessgatingselections = (Wireless_gating_t*)l_data.byref;

    if (NULL != l_wirelessgatingselections) {
        (*f_wireless_gating) = (*l_wirelessgatingselections);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_wirelessgatingselections is NULL"));
    }

    return true;

}



//************************************Method Header************************************
// Method Name  : ReadWirelessParamfromConfig
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CWirelessGatingController::ReadWirelessParamfromConfig(
    CIniReader* f_ini_reader
)
{


    LPCTSTR FUNC_NAME = _T("CWirelessGatingController::ReadWirelessParamfromConfig");

    LPCUSTR l_section = _T("Wireless Gating");

    CString l_str_key_value;

    if (f_ini_reader->SectionExists(l_section)) {

        l_str_key_value = f_ini_reader->GetKeyValue(_T("mode"), l_section);

        m_wireless_default_config_param.m_mode = _ttoi(l_str_key_value);

        l_str_key_value = f_ini_reader->GetKeyValue(_T("filter"), l_section);

        m_wireless_default_config_param.m_wireless_filter = _ttoi(l_str_key_value);

        l_str_key_value = f_ini_reader->GetKeyValue(_T("cmd"), l_section);

        m_wireless_default_config_param.m_command = _ttoi(l_str_key_value);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Wireless Gating Section is not present in PQMConfig.ini"));

    }

    ValidateWireLessGatingConfigParam(&m_wireless_default_config_param);
}


//************************************Method Header************************************
// Method Name  : ValidateWireLessGatingConfigParam
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CWirelessGatingController::ValidateWireLessGatingConfigParam(
    Wireless_gating_t* f_wireless_default_config_param
)
{

    LPCTSTR FUNC_NAME = _T("CWirelessGatingController::SetWirelessGatingStruct");

    if (f_wireless_default_config_param) {

        switch (f_wireless_default_config_param->m_command) {

            case VFC_WIRELESS_GATE_TRIG_FILTER_CMD_HIGH_GAIN:
            case VFC_WIRELESS_GATE_TRIG_FILTER_MODE_NONE:

                break;

            default:

                f_wireless_default_config_param->m_command = VFC_WIRELESS_GATE_TRIG_FILTER_CMD_HIGH_GAIN;
        }

        switch (f_wireless_default_config_param->m_mode) {

            case VFC_WIRELESS_GATE_TRIG_FILTER_MODE_T_WAVE_SUPPRESSION:
            case VFC_WIRELESS_GATE_TRIG_FILTER_MODE_NONE:

                break;

            default:
                f_wireless_default_config_param->m_mode = VFC_WIRELESS_GATE_TRIG_FILTER_MODE_NONE;

        }

        switch (f_wireless_default_config_param->m_wireless_filter) {

            case VFC_WIRELESS_GATE_TRIG_FILTER_CARDIAC:
            case VFC_WIRELESS_GATE_TRIG_FILTER_MRI_B:
            case VFC_WIRELESS_GATE_TRIG_FILTER_MRI_A:
                break;

            case VFC_WIRELESS_GATE_TRIG_FILTER_MONITOR:
            default:
                f_wireless_default_config_param->m_wireless_filter = VFC_WIRELESS_GATE_TRIG_FILTER_MRI_A;
        }
    }

}

//************************************Method Header************************************
// Method Name  : SetWirelessDefaultParamtoIPC
// Author       : PATNI
// Purpose      :
//***********************************************************************************
void CWirelessGatingController::SetWirelessDefaultParamtoIPC(
    const Wireless_gating_t& f_wireless_param
)
{
    LPCTSTR FUNC_NAME = _T("CWirelessGatingController::SetWirelessDefaultParamtoIPC");

    VARIANT* pData = new VARIANT;

    if (NULL == pData) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pointer pData is NULL"));
        return;
    }

    pData->byref = (Wireless_gating_t*)(&f_wireless_param);

    m_pqmview->GetAcquisition()->SetWirelessParamtoProt(&pData);

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }
}
