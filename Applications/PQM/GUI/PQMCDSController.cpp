//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//redmine-779
//redmine-774

// PQMCDSController.cpp: implementation of the CPQMCDSController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PQMCDSController.h"
#include <PDataParser/INIReader.h>
#include "PQMView.h"
#include "CDS/COMPASS_SettingDialog.h"
#include "CDS/COMPASS_Location_Debug.h"

const CString g_cds_Ratio     = _T("cds_Ratio") ;
const CString g_cds_Range     = _T("cds_Range") ;
const CString g_cds_Range_max = _T("cds_Range_max") ;
const CString g_cds_Range_min = _T("cds_Range_min") ;
const CString g_cds_CheckCoil = _T("cds_CheckCoil") ;
const int g_min_coverage = 1;
const int g_max_coverage = 100;

extern CPQMView* g_MainView;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPQMCDSController::CPQMCDSController(
    CPQMView* f_pqm_view
) :		m_pqm_view(f_pqm_view),
    m_ascompass_settingdialog(NULL),
    m_compass_location_debugdialog(NULL),
    m_is_ascompassse_setting_opened_before_swithcing_page(false),
    m_is_compass_debug_opened_before_swithcing_page(false),
    m_cdsdebug_flag(false)
{

    InitializeCDSParametersWithDefault(&m_cdsconfiginfo_default);
}

CPQMCDSController::~CPQMCDSController()
{
    DEL_PTR(m_ascompass_settingdialog)
    DEL_PTR(m_compass_location_debugdialog)
}

//************************************Method Header************************************
// Method Name  : InitCDSController
// Author       : PATNI/ HEMANT
// Purpose      : Initialize CDS controller
//***********************************************************************************
void CPQMCDSController::InitCDSController(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCDSController::InitCDSController");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_pqm_view && m_pqm_view->GetAcquisition()) {
        m_pqm_view->GetAcquisition()->OnOffCDSDebug(m_cdsdebug_flag);
    }
}


//************************************Method Header************************************
// Method Name  : ShowHideOpenedWindows
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::ShowHideOpenedWindows(
    const bool f_show /*= true*/
)
{
    g_MainView->ShowHideOpenedWindows(m_ascompass_settingdialog , m_is_ascompassse_setting_opened_before_swithcing_page, f_show);

    if (m_is_compass_debug_opened_before_swithcing_page && !m_compass_location_debugdialog) {
        m_is_compass_debug_opened_before_swithcing_page = false;
        OpenCompassDebug();

    } else {
        g_MainView->ShowHideOpenedWindows(m_compass_location_debugdialog , m_is_compass_debug_opened_before_swithcing_page, f_show);
    }
}


//************************************Method Header************************************
// Method Name  : InitializeCDSParametersWithDefault
// Author       : PATNI/ HEMANT
// Purpose      : Initializing CDS param with default values
//***********************************************************************************
void CPQMCDSController::InitializeCDSParametersWithDefault(
    SIPCConfigInfo* f_sIPCConfigInfo
)const
{
    if (f_sIPCConfigInfo) {
        InitializeCDSParametersWithDefault(&f_sIPCConfigInfo->m_cds_config_info);
    }
}

//************************************Method Header************************************
// Method Name  : InitializeCDSParametersWithDefault
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::InitializeCDSParametersWithDefault(
    sCDSConfigInfo* f_cdsconfiginfo
) const
{
    if (f_cdsconfiginfo) {
        f_cdsconfiginfo->m_cds_CheckCoil = g_default_cds_CheckCoil ;
        f_cdsconfiginfo->m_cds_Range_min = g_default_cds_Range_min ;
        f_cdsconfiginfo->m_cds_Range_max = g_default_cds_Range_max ;
        f_cdsconfiginfo->m_cds_Range = g_default_cds_Range ;
        f_cdsconfiginfo->m_cds_Ratio = g_default_cds_Ratio ;
    }
}


//************************************Method Header************************************
// Method Name  : ReadCDSParams
// Author       : PATNI/ HEMANT
// Purpose      : Reading CDS param from ini
//***********************************************************************************
bool CPQMCDSController::ReadCDSParams(
    CIniReader* f_ini_reader,
    SIPCConfigInfo* f_sIPCConfigInfo
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCDSController::ReadCDSParams");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (f_ini_reader && f_sIPCConfigInfo) {

        //first initialize with default values
        InitializeCDSParametersWithDefault(f_sIPCConfigInfo);

        //Now reading CDS related parameters
        LPCUSTR l_section = _T("CDS") ;

        if (f_ini_reader->SectionExists(l_section)) {

            //Read parameters

            int l_param_value = 0;
            CString l_str_key_value = f_ini_reader->GetKeyValue(g_cds_CheckCoil, l_section);

            if (!GetParameterValue(l_str_key_value, l_param_value)) {

                CString l_log_str = _T("Function Failed for : ") + g_cds_CheckCoil + _T(" = ") + l_str_key_value ;
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_str);

                return false;
            }

            f_sIPCConfigInfo->m_cds_config_info.m_cds_CheckCoil = l_param_value;

            l_str_key_value = f_ini_reader->GetKeyValue(g_cds_Range_min, l_section);

            if (!GetParameterValue(l_str_key_value, f_sIPCConfigInfo->m_cds_config_info.m_cds_Range_min)) {

                CString l_log_str = _T("Function Failed for : ") + g_cds_Range_min + _T(" = ") + l_str_key_value ;
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_str);

                return false ;
            }

            l_str_key_value = f_ini_reader->GetKeyValue(g_cds_Range_max, l_section);

            if (!GetParameterValue(l_str_key_value, f_sIPCConfigInfo->m_cds_config_info.m_cds_Range_max)) {

                CString l_log_str = _T("Function Failed for : ") + g_cds_Range_max + _T(" = ") + l_str_key_value ;
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_str);
                return false;
            }

            l_str_key_value = f_ini_reader->GetKeyValue(g_cds_Range, l_section);

            if (!GetParameterValue(l_str_key_value, f_sIPCConfigInfo->m_cds_config_info.m_cds_Range)) {

                CString l_log_str = _T("Function Failed for : ") + g_cds_Range + _T(" = ") + l_str_key_value ;
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_str);
                return false;
            }

            l_str_key_value = f_ini_reader->GetKeyValue(g_cds_Ratio, l_section);

            if (!GetParameterValue(l_str_key_value, f_sIPCConfigInfo->m_cds_config_info.m_cds_Ratio)) {

                CString l_log_str = _T("Function Failed for : ") + g_cds_Ratio + _T(" = ") + l_str_key_value ;
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_str);
                return false;
            }

            return ValidateCDSParams(f_sIPCConfigInfo) ;
        }
    }

    return false ;
}


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnASCOMPASSSettings
// Author       : PATNI/ HEMANT
// Purpose      : Open AS-COMPASS setting dialog
//***********************************************************************************
void CPQMCDSController::OnASCOMPASSSettings(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCDSController::OnASCOMPASSSettings");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!m_ascompass_settingdialog) {
        m_ascompass_settingdialog = new CCOMPASS_SettingDialog(g_MainView) ;
    }

    CDSCompassSettingParams l_compass_setting_param ;
    l_compass_setting_param.m_dist_from_magnetic_center = ASCompassSettingParam();
    l_compass_setting_param.m_min_coverage_for_section = ASCompassSettingParam();

    VARIANT* pData = new VARIANT;
    pData->byref = &l_compass_setting_param;
    g_MainView->GetAcquisition()->GetASCOMPASSParameter(&pData);

    m_ascompass_settingdialog->SetASCOMPASSParameter(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH, l_compass_setting_param.m_dist_from_magnetic_center);
    m_ascompass_settingdialog->SetASCOMPASSParameter(AS_COMPASS_PARAM_COIL_COVERAGE, l_compass_setting_param.m_min_coverage_for_section);


    //Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
    g_MainView->ChangeViewOfModelessDialogs(false);

    if (m_ascompass_settingdialog->DoModal() == IDOK) {

        l_compass_setting_param.m_dist_from_magnetic_center = *(m_ascompass_settingdialog->GetASCOMPASSParameter(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH));
        l_compass_setting_param.m_min_coverage_for_section = *(m_ascompass_settingdialog->GetASCOMPASSParameter(AS_COMPASS_PARAM_COIL_COVERAGE));

        pData->byref = &l_compass_setting_param;
        g_MainView->GetAcquisition()->SetASCOMPASSParameter(&pData);
    }

    //Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
    g_MainView->ChangeViewOfModelessDialogs(true);

    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    CString l_message_str;

    l_message_str.Format(_T("dist_from_magnetic_center : %d "), l_compass_setting_param.m_dist_from_magnetic_center.m_value);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("min_coverage_for_section : %d "), l_compass_setting_param.m_min_coverage_for_section.m_value);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

    if (pData) {
        pData->byref = NULL ;
        DEL_PTR(pData)
    }

    //delete at this.. so new dialog will be created next time..
    DEL_PTR(m_ascompass_settingdialog)
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffAsCompass
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::OnOffAsCompass(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::OnOffAsCompass");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMListCtrl* l_pqm_listctrl = GetPQMListCtrl();

    if (!g_MainView->GetAcquisition() || !l_pqm_listctrl) {

        return ;
    }

    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    int* selected_items = NULL;
    const int count = l_pqm_listctrl->GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected);

    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    if ((count == 0) || iscurrent_selected || isdone_selected ||  isfail_selected) {

        DEL_PTR_ARY(selected_items);
        return ;
    }

    CSequence* l_sel_seq = (CSequence*) l_pqm_listctrl->GetItemData(selected_items[0]);

    if (!l_sel_seq) {
        DEL_PTR_ARY(selected_items);
        return ;
    }

    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

    const bool l_on_off_flag = !(l_sel_seq->GetASCompassFlag());

    g_MainView->SetWaitCursor(true);

    for (int index = 0; index < count; index++) {

        //ignorming the protocol being edited by WSE
        if (selected_items[index] == l_pqm_listctrl->GetWSEEditingProtocolIndex()) {
            continue ;
        }

        l_sel_seq = (CSequence*) l_pqm_listctrl->GetItemData(selected_items[index]);

        if (!l_sel_seq) {

            ASSERT(FALSE) ;
            continue ;
        }

        if (l_on_off_flag == (l_sel_seq->GetASCompassFlag() > 0)) {
            continue;
        }


        if (S_OK == g_MainView->GetAcquisition()->OnOffAsCompass(
                g_MainView->GetAcqOderForGivenProtocolIndex(selected_items[index]),
                l_on_off_flag)
           ) {

            l_sel_seq->SetASCompassFlag(l_on_off_flag);
        }
    }

    //Patni-ARD/2010Apr29/Added/JaFT# MCM0229-00178_CDR_06 + code review + memory leak
    DEL_PTR_ARY(selected_items);

    g_MainView->SetWaitCursor(false);

}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement



//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      : On Off Cds debug flag
//***********************************************************************************
void CPQMCDSController::OnOffCDSDebug(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCDSController::OnOffCDSDebug");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (g_MainView && g_MainView->GetAcquisition()) {
        m_cdsdebug_flag = !m_cdsdebug_flag ;
        g_MainView->GetAcquisition()->OnOffCDSDebug(m_cdsdebug_flag);
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CDSApplyCoiltoProtocols
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when COMPASS coil are confirmed by user
//                while CDS.
//                Whiel CDS, Coil selection is opened for current protocol only.
//                So there is only one selected item = current protocol
//                Now, apply the coil to current item and also to the below protocols
//                if batch setting is ON.otherwise to current item only.
//***********************************************************************************
void CPQMCDSController::CDSApplyCoiltoProtocols(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::CDSApplyCoiltoProtocols");

    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!g_MainView) {
        return ;
    }

    const int l_start_index = g_MainView->GetScanIndex() ;

    CPQMListCtrl* l_pqm_listctrl = GetPQMListCtrl();

    if (!g_MainView->IsValidSequenceIndex(l_start_index) || !l_pqm_listctrl) {

        ASSERT(FALSE) ;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_start_index is not valid"));
        return ;
    }

    if (g_MainView->IsBatchSettingOn()) {

        const int l_total_protocols_for_cds = (l_pqm_listctrl->GetItemCount() - l_start_index) ;

        int* selected_items = new int[l_total_protocols_for_cds] ;

        int counter = 0;

        for (int item = l_start_index; counter < l_total_protocols_for_cds; item++)
            selected_items[counter++] = item;

        g_MainView->ApplyCoilToProtocols(selected_items, l_total_protocols_for_cds);

        DEL_PTR_ARY(selected_items)

    } else {

        g_MainView->ApplyCoilToProtocols(&l_start_index, 1);
    }
}
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OpenCompassDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::OpenCompassDebug(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::OpenCompassDebug");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!g_MainView->IsWindowVisible()) {
        m_is_compass_debug_opened_before_swithcing_page = true ;
        return ;
    }

    if (!m_compass_location_debugdialog) {

        m_compass_location_debugdialog = new CCOMPASS_Location_Debug(g_MainView);
    }


    COMPASS_Debugg_Param l_debug_param[MAX_PARAM] ;

    VARIANT* pData = new VARIANT;
    pData->byref = l_debug_param;
    g_MainView->GetAcquisition()->GetCDSDebugParam(&pData);

    m_compass_location_debugdialog->SetCompassDebugParam(l_debug_param);

    //Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
    g_MainView->ChangeViewOfModelessDialogs(false);

    const bool l_set = (IDOK == m_compass_location_debugdialog->DoModal());

    //Patni-AJS+Hemant/2011Jan31/Added/REDMINE-1226
    g_MainView->ChangeViewOfModelessDialogs(true);

    m_compass_location_debugdialog->GetCompassDebugParam(l_debug_param);

    pData->byref = l_debug_param;

    DEL_PTR(m_compass_location_debugdialog)

    extern CPQMView* g_MainView;
    g_MainView->GetAcquisition()->CDSDebugClosed(&pData, l_set);

    if (pData) {

        pData->byref = NULL ;
        DEL_PTR(pData)
    }

}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : IsCDSDebugON
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMCDSController::IsCDSDebugON() const
{

    return m_cdsdebug_flag ;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : UpdateCompassCoilToBelowProtocols
// Author       : PATNI/ HEMANT
// Purpose      : When batch setting is ON, this function copies the coil information to
//                all below protocols present under the currently scanned protocol.
//                This function is called when coil fonfirmation dialog is not opened
//                while CDS.
//***********************************************************************************
void CPQMCDSController::UpdateCompassCoilToBelowProtocols(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::UpdateCompassCoilToBelowProtocols");
    PQM_TRACE_SCOPED(FUNC_NAME);

    const int recv_coil_id = __recv_coil_id ; //COILKEY;
    const CString rc = RECVR_COIL ;

    CPQMListCtrl* l_pqm_listctrl = GetPQMListCtrl();

    const int l_start_index = g_MainView->GetScanIndex() ;

    if (!g_MainView->IsValidSequenceIndex(l_start_index) || !l_pqm_listctrl) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_start_index is not valid"));
        return ;
    }

    int l_total_list_items = l_pqm_listctrl->GetItemCount() ;

    if (!(g_MainView->GetPQMGlobal().GetBatchSettingFlag())) {

        //if batch setting is OFF, then consider only the first protocol
        l_total_list_items = (l_start_index + 1) ;
    }

    CString l_recv_coil_label = _T("");
    CSequence* l_seq = NULL ;

    for (int l_index = l_start_index; l_index < l_total_list_items ; l_index++) {

        l_seq = (CSequence*)(l_pqm_listctrl->GetItemData(l_index));

        if (!g_MainView->IsValidSequenceIndex(l_index) || !l_seq) {

            ASSERT(FALSE);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("l_index is not valid or l_seq is NULL"));

            break ;
        }

        //skip apply coil to protocol currently edited by WSE
        //if (m_is_wse_open && (l_index == l_pqm_listctrl->GetWSEEditingProtocolIndex()))
        //    continue ;

        //now apply settings to local seq structure..
        //consider whaever at IPC side..

        l_recv_coil_label =
            g_MainView->GetCoilStringFor(g_MainView->GetAcqOderForGivenProtocolIndex(l_index)) ;

        l_seq->SetValue(rc, l_recv_coil_label);

        CPqmPrMode& prmode = g_MainView->GetPrModeOfSelectedSequence(l_index);
        prmode.m_rcv_coil_id = recv_coil_id;
        prmode.m_recv_coil_label = l_recv_coil_label;
    }

    //update the GUI with the coil updatation
    g_MainView->DisplaySelectedCoil(l_pqm_listctrl->GetFirstSelectedItem());
}
//-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : UpdateCompassCoilToCurrentProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::UpdateCompassCoilToCurrentProtocol(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::UpdateCompassCoilToCurrentProtocol");
    PQM_TRACE_SCOPED(FUNC_NAME);

    extern CPQMView* g_MainView;
    CPQMView* l_pwm_veiw = g_MainView ;

    if (!l_pwm_veiw) {
        return ;
    }

    const int l_start_index = l_pwm_veiw->GetScanIndex() ;

    CPQMListCtrl* l_pqm_listctrl = GetPQMListCtrl();

    if (!l_pwm_veiw->IsValidSequenceIndex(l_start_index) || !l_pqm_listctrl) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_start_index is not valid"));
        return ;
    }

    //now apply settings to local seq structure..
    const int l_acq_order = l_pwm_veiw->GetAcqOderForGivenProtocolIndex(l_start_index);
    CString l_recv_coil_label = l_pwm_veiw->GetCoilStringFor(l_acq_order) ;

    CString l_log_str;
    l_log_str.Format(_T("CDS Coil applied to protocol acq_order = %d "), l_acq_order);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    l_log_str = _T("CDS receiver coil is :") + l_recv_coil_label;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    CSequence* l_seq = (CSequence*)(l_pqm_listctrl->GetItemData(l_start_index));
    l_seq->SetValue(RECVR_COIL, l_recv_coil_label);

    CPqmPrMode& prmode = l_pwm_veiw->GetPrModeOfSelectedSequence(l_start_index);
    prmode.m_rcv_coil_id = __recv_coil_id;
    prmode.m_recv_coil_label = l_recv_coil_label;

    //update the GUI with the coil updatation
    l_pwm_veiw->DisplaySelectedCoil(l_pqm_listctrl->GetFirstSelectedItem());

}
//-Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement


//************************************Method Header************************************
// Method Name  : DisplayCoilSelforCOMPASSConfirmation
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::DisplayCoilSelforCOMPASSConfirmation(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::DisplayCoilSelforCOMPASSConfirmation");
    PQM_TRACE_SCOPED(FUNC_NAME);

    extern CPQMView* g_MainView;

    if (!g_MainView) {
        return ;
    }

    const int l_start_index = g_MainView->GetScanIndex() ;

    CPQMListCtrl* l_pqm_listctrl = GetPQMListCtrl();

    if (g_MainView->IsValidSequenceIndex(l_start_index) && l_pqm_listctrl) {

        l_pqm_listctrl->SelectOnlyItem(l_start_index);
        l_pqm_listctrl->EnsureVisible(l_start_index, FALSE);

        //invoke the coil after changing the selection only..- as per vantage
        //Note : coil selection is always opened for currently selected item
        g_MainView->OpenCoilSelectionDialogForCDS();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_start_index is Invalid"));
    }

}



//+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : InitializeCDSParameters
// Author       : PATNI/ HEMANT
// Purpose      : This function initializes the CDS compass related parameters at IPC side.
//                This function should be called after IPC initialization
//***********************************************************************************
void CPQMCDSController::InitializeCDSParameters(
    const SIPCConfigInfo* f_sIPCConfigInfo
)
{
    if (f_sIPCConfigInfo) {
        InitializeCDSParameters(&f_sIPCConfigInfo->m_cds_config_info);
    }
}
//-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement

//************************************Method Header************************************
// Method Name  : InitializeCDSParametersWithConfigDefault
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::InitializeCDSParametersWithConfigDefault(
)
{
    InitializeCDSParameters(&m_cdsconfiginfo_default);
}

//+Patni-Hemant/2010Apr9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : InitializeCDSParameters
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::InitializeCDSParameters(
    const sCDSConfigInfo* f_cdsconfiginfo
)
{
    if (f_cdsconfiginfo) {

        CDSCompassSettingParams l_compass_setting_param ;

        l_compass_setting_param.m_dist_from_magnetic_center.m_min_value = f_cdsconfiginfo->m_cds_Range_min;
        l_compass_setting_param.m_dist_from_magnetic_center.m_max_value = f_cdsconfiginfo->m_cds_Range_max;
        l_compass_setting_param.m_dist_from_magnetic_center.m_value = f_cdsconfiginfo->m_cds_Range;

        l_compass_setting_param.m_min_coverage_for_section.m_min_value = g_min_coverage;
        l_compass_setting_param.m_min_coverage_for_section.m_max_value = g_max_coverage;
        l_compass_setting_param.m_min_coverage_for_section.m_value = f_cdsconfiginfo->m_cds_Ratio;


        //set the compass dialog parameters
        VARIANT* pData = new VARIANT;
        pData->byref = &l_compass_setting_param;
        g_MainView->GetAcquisition()->SetASCOMPASSParameter(&pData);

        //set the check coil flag
        g_MainView->GetAcquisition()->SetCDSCheckCoil(f_cdsconfiginfo->m_cds_CheckCoil);

        if (pData) {
            pData->byref = NULL ;
            DEL_PTR(pData)
        }
    }
}
//-Patni-Hemant/2010Apr9/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : UpdateAsCompassFlagForCurrentProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCDSController::UpdateAsCompassFlagForCurrentProtocol(
) const
{

    LPCTSTR FUNC_NAME = _T("CPQMCDSController::UpdateAsCompassFlagForCurrentProtocol");
    PQM_TRACE_SCOPED(FUNC_NAME);


    if (!g_MainView) {
        return ;
    }

    const int l_start_index = g_MainView->GetScanIndex() ;

    CPQMListCtrl* l_pqm_listctrl = GetPQMListCtrl();

    if (!g_MainView->IsValidSequenceIndex(l_start_index) || !l_pqm_listctrl) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_start_index is not valid"));
        return ;
    }

    CSequence* l_seq = (CSequence*)(l_pqm_listctrl->GetItemData(l_start_index));

    BOOL l_ascompass_flag = l_seq->GetASCompassFlag();

    if (S_OK != g_MainView->GetAcquisition()->GetASCompassFlag(
            g_MainView->GetAcqOderForGivenProtocolIndex(l_start_index),
            &l_ascompass_flag)
       ) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetASCompassFlag Failed"));
    }

    l_seq->SetASCompassFlag(l_ascompass_flag);

}
//-Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement
CCOMPASS_Location_Debug* CPQMCDSController::GetCompasLocationDebugDialog()const
{
    return m_compass_location_debugdialog;
}

//+Patni-Ravindra Acharya/2010Sep27/Added/MVC008951 & Redmine 717
//*****************************************************************************
//Method Name   : GetCOMPASS_SettingDlg
//Author        : Patni / Ravindra Acharya
//Purpose       :
//*****************************************************************************

CCOMPASS_SettingDialog* CPQMCDSController::GetCOMPASS_SettingDlg()const
{
    return m_ascompass_settingdialog ;
}

//-Patni-Ravindra Acharya/2010Sep27/Added/MVC0008951 & Redmine 717



//************************************Method Header************************************
// Method Name  : ValidateCDSParams
// Author       : PATNI/ HEMANT
// Purpose      : Validations for CDS params
//***********************************************************************************
bool CPQMCDSController::ValidateCDSParams(
    const SIPCConfigInfo* f_sIPCConfigInfo
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCDSController::ValidateCDSParams");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!f_sIPCConfigInfo) {
        return false ;
    }

    //cds_Range_max should be higher than cds_Range_min value
    if (f_sIPCConfigInfo->m_cds_config_info.m_cds_Range_max <= f_sIPCConfigInfo->m_cds_config_info.m_cds_Range_min) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("cds_Range_max is lessed than cds_Range_min"));

        return false;
    }

    //cds_Range should lie within cds_Range_Max and cds_Range_Min
    if ((f_sIPCConfigInfo->m_cds_config_info.m_cds_Range > f_sIPCConfigInfo->m_cds_config_info.m_cds_Range_max)
        || (f_sIPCConfigInfo->m_cds_config_info.m_cds_Range < f_sIPCConfigInfo->m_cds_config_info.m_cds_Range_min)
       ) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("cds_Range is not between cds_Range_max and cds_Range_min"));

        return false ;
    }

    //cds_Ratio should lie withing 0-100.
    if ((f_sIPCConfigInfo->m_cds_config_info.m_cds_Ratio > g_max_coverage)
        || (f_sIPCConfigInfo->m_cds_config_info.m_cds_Ratio < g_min_coverage)
       ) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("cds_Ratio is not between max_coverage and min_coverage"));

        return false ;
    }

    //Save default values...
    m_cdsconfiginfo_default = f_sIPCConfigInfo->m_cds_config_info;

    return true ;
}


//************************************Method Header************************************
// Method Name  : GetParameterValue
// Author       : PATNI/ HEMANT
// Purpose      : Validate read paramater
//***********************************************************************************
bool CPQMCDSController::GetParameterValue(
    const CString& l_param_value_str,
    int& l_param_value_int
)
{
    if (l_param_value_str.IsEmpty()) {
        return false;
    }

    const int l_value_to_return = _wtoi(l_param_value_str);

    CString l_str_value ;

    l_str_value.Format(_T("%d"), l_value_to_return);

    if (l_str_value.Compare(l_param_value_str) != 0) {
        return false;
    }

    l_param_value_int = l_value_to_return ;

    return true ;
}


//************************************Method Header************************************
// Method Name  : GetPQMListCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPQMListCtrl* CPQMCDSController::GetPQMListCtrl(
) const
{

    if (g_MainView) {
        return &(g_MainView->GetPQMListCtrl());
    }

    return NULL ;
}