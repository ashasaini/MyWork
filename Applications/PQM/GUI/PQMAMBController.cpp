// PQMAMBController.cpp: implementation of the CPQMAMBController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pqm.h"
#include "PQMAMBController.h"
#include "PQMView.h"
#include "PQMAMBDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CPQMView* g_MainView;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//************************************Method Header**********************************
// Method Name  : CPQMAMBController
// Author       : iGATE
// Purpose      :
//***********************************************************************************
CPQMAMBController::CPQMAMBController(
) : m_pqm_view(NULL),
    m_amb_dlg(NULL),
    m_is_amb_opened_before_swithcing_page(false),
    m_batch_setting_for_coil_sar_backup(true),
    m_canreset_ambprotocols_on_wse_exit(true),
    m_is_wait_for_amb_dlg_open(false)
{

}


//************************************Method Header**********************************
// Method Name  : ~CPQMAMBController
// Author       : iGATE
// Purpose      :
//***********************************************************************************
CPQMAMBController::~CPQMAMBController()
{
    DEL_PTR(m_amb_dlg)
}


//************************************Method Header**********************************
// Method Name  : InitializeAMBController
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::InitializeAMBController(
    CPQMView* const f_pqm_view,
    const CRect& f_parent_rect
)
{
    m_pqm_view = f_pqm_view;

    if (!m_amb_dlg) {
        m_amb_dlg = new CPQMAMBDlg(this, f_parent_rect , m_pqm_view);
    }
}

//************************************Method Header************************************
// Method Name  : ShowHideOpenedWindows
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::ShowHideOpenedWindows(
    const bool f_show /*= true*/
)
{
    m_pqm_view->ShowHideOpenedWindows(m_amb_dlg , m_is_amb_opened_before_swithcing_page, f_show);
}
//************************************Method Header**********************************
// Method Name  : OnAMBScanMenu
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::OnAMBScanMenu()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBController::OnAMBScanMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    VARIANT l_pData;

    int* l_selected_items = NULL ;
    int l_size = 0;
    m_pqm_view->AMBGetSelectedItems(&l_selected_items, l_size);

    if ((NULL == l_selected_items) || l_size <= 0) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_size is not valid OR l_selected_items is NULL"));
        return ;
    }

    l_pData.byref = NULL;

    if (!m_pqm_view->GetAcquisition()->GetAMBStatusData(l_selected_items, l_size, &l_pData)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetAMBStatusData is Successful...Invoking AMB Dialog Now."));

        m_batch_setting_for_coil_sar_backup = g_MainView->IsBatchSettingOn();
        g_MainView->SetBatchSettingOnOff(false);

        m_pqm_view->GetAcquisition()->SetActiveDialog(IS_AMB_DIALOG, true);

        BOOL l_create_flag = m_amb_dlg->Create(IDD_AMB_DIALOG, m_pqm_view);
        m_amb_dlg->ShowWindow(SW_SHOW);




    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetAMBStatusData is UnSuccessful...Can not Invok AMB Dialog."));

        m_pqm_view->ResetAMBPlanProtocols();
        m_pqm_view->OnAMBClosed();
    }
}
//************************************Method Header**********************************
// Method Name  : IsAMBInvoked
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::IsAMBInvoked() const
{
    return (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd)) ;
}
//************************************Method Header**********************************
// Method Name  : CloseAMBOnError
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::CloseAMBOnError() const
{
    if (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd))
        m_amb_dlg->CloseAMBOnError();
}
//************************************Method Header**********************************
// Method Name  : RecAmbEditExit
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::RecAmbEditExit() const
{
    if (IsAMBInvoked()) {
        m_amb_dlg->PostMessage(WM_CLOSE, 0, 0);

    }
}
//************************************Method Header**********************************
// Method Name  : RecAMBPqmScanMsg
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::RecAMBPqmScanMsg() const
{

    m_amb_dlg->RecAMBPqmScanMsg();
}
//************************************Method Header**********************************
// Method Name  : RecAmbScanWait
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::RecAmbScanWait() const
{

    m_amb_dlg->RecAmbScanWait();
}

//************************************Method Header**********************************
// Method Name  : RecAmbScanStartError
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::RecAmbScanStartError() const
{

    m_amb_dlg->RecAmbScanStartError();
}
//************************************Method Header**********************************
// Method Name  : RecAmbScanStartMsg
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::RecAmbScanStartMsg(const bool f_is_irs_msg) const
{

    m_amb_dlg->OnClickAMBScanStart(f_is_irs_msg);
}

//************************************Method Header**********************************
// Method Name  : GetAMBDlgPtr
// Author       : iGATE
// Purpose      :
//***********************************************************************************
CPQMAMBDlg* CPQMAMBController::GetAMBDlgPtr(
)const
{
    return m_amb_dlg;
}

bool CPQMAMBController::IsAMBScanStarted() const
{

    if (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd))
        return m_amb_dlg->IsAMBScanStarted();

    return false;
}

//************************************Method Header**********************************
// Method Name  : AMBScanStartThread
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::AMBScanStartThread()
{

    VARIANT l_pData;
    l_pData.byref = &m_amb_dlg->GetAMBStatus();

    BOOL f_is_scan_start = FALSE;
    g_MainView->GetAcquisition()->AMBScanStart(&l_pData, &f_is_scan_start);
    return (TRUE == f_is_scan_start);
}
//************************************Method Header**********************************
// Method Name  : UpdateAMBAbortButtonState
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::UpdateAMBAbortButtonState(const BOOL f_amb_abort_btn_state)
{

    if (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd))
        m_amb_dlg->UpdateAMBAbortButtonState(f_amb_abort_btn_state);
}

//************************************Method Header**********************************
// Method Name  : UpdateAMBScanButtonState
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::UpdateAMBScanButtonState(const BOOL f_amb_scan_btn_state)
{

    if (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd))
        m_amb_dlg->UpdateAMBScanButtonState(f_amb_scan_btn_state);
}

//************************************Method Header**********************************
// Method Name  : GetAmbPlan
// Author       : iGATE
// Purpose      :
//***********************************************************************************
//Implement CAMBDialogInterface methods.
void CPQMAMBController::GetAmbPlan(VARIANT* f_pData)
{
    g_MainView->GetAcquisition()->GetAmbPlan(f_pData);
}
//************************************Method Header**********************************
// Method Name  : RemoteCheckActiveFlag
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::RemoteCheckActiveFlag()
{
    return (S_OK == g_MainView->GetAcquisition()->RemoteCheckActiveFlag()) ;
}
//************************************Method Header**********************************
// Method Name  : OnClickButtonScanAbort
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::OnClickButtonScanAbort()
{
    if (!g_MainView->GetPostProcessStatus() && !g_MainView->CheckCFADialogStatus()) {
        g_MainView->OnClickButtonScanAbort();
    }
}
//************************************Method Header**********************************
// Method Name  : IsWaitCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::IsWaitCursor() const
{
    return g_MainView->IsWaitCursor();
}
//************************************Method Header**********************************
// Method Name  : AMBScanStart
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::AMBScanStart()
{
    return g_MainView->AMBScanStart();
}
//************************************Method Header**********************************
// Method Name  : OnClickScanStart
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::OnClickScanStart()
{
    if (!(!g_MainView->GetDynamicScan() && g_MainView->IsScanning()) &&
        g_MainView->GetIsCFAokpressed()  &&
        !g_MainView->CheckCoilDialogStatus() &&
        !g_MainView->GetPostProcessStatus()) {
        g_MainView->PQMMessageScanStart();
    }
}
//************************************Method Header**********************************
// Method Name  : AMBScanCancel
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::AMBScanCancel(VARIANT* f_pData)
{
    g_MainView->GetAcquisition()->AMBScanCancel(f_pData);
}

//************************************Method Header**********************************
// Method Name  : AMBScanAbort
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::AMBScanAbort()
{
    g_MainView->GetAcquisition()->AMBScanAbort();;
}

//************************************Method Header**********************************
// Method Name  : AMBDlgCancel
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::AMBDlgCancel()
{
    m_pqm_view->GetAcquisition()->SetActiveDialog(IS_AMB_DIALOG, false);
    g_MainView->SetBatchSettingOnOff(m_batch_setting_for_coil_sar_backup);
    m_pqm_view->ResetAMBPlanProtocols();
    m_pqm_view->OnAMBClosed();
}

//************************************Method Header**********************************
// Method Name  : EnabledisableAMBScanRelatedButtons
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::EnabledisableAMBScanRelatedButtons(const bool f_is_enable)
{
    m_amb_dlg->EnabledisableAMBScanRelatedButtons(f_is_enable);
}

//************************************Method Header**********************************
// Method Name  : AmbScanStateSetting
// Author       : iGATE
// Purpose      : Enable/Disable the AMB dialog buttons
//***********************************************************************************
void CPQMAMBController::AmbScanStateSetting()
{

    m_amb_dlg->AmbScanStateSetting();
}

//************************************Method Header**********************************
// Method Name  : IsAcqumanReadyForNextRequest
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::IsAcqumanReadyForNextRequest()const
{
    return m_pqm_view->IsAcqumanReadyForNextRequest();
}

//************************************Method Header**********************************
// Method Name  : UpdateStatesForCtrls
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::UpdateStatesForCtrls(const pqm_msg_tag f_message_to_process)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBController::UpdateStatesForCtrls");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd))
        m_amb_dlg->UpdateStatesForCtrls(f_message_to_process);

}

//************************************Method Header**********************************
// Method Name  : SetIsCanResetAMBprotocolsOnWseExit
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::SetIsCanResetAMBprotocolsOnWseExit(const bool f_canreset_ambprotocols_on_wse_exit)
{
    m_canreset_ambprotocols_on_wse_exit = f_canreset_ambprotocols_on_wse_exit;
}

//************************************Method Header**********************************
// Method Name  : GetIsCanResetAMBprotocolsOnWseExit
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::GetIsCanResetAMBprotocolsOnWseExit() const
{
    return m_canreset_ambprotocols_on_wse_exit;
}

//************************************Method Header**********************************
// Method Name  : SetIswaitforAMBDlgOpen
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::SetIswaitforAMBDlgOpen(const bool f_is_wait_for_amb_dlg_open)
{
    m_is_wait_for_amb_dlg_open = f_is_wait_for_amb_dlg_open;
}

//************************************Method Header**********************************
// Method Name  : GetIswaitforAMBDlgOpen
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMAMBController::GetIswaitforAMBDlgOpen() const
{
    return m_is_wait_for_amb_dlg_open;
}

//************************************Method Header**********************************
// Method Name  : UpdateDelayTimeState
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBController::UpdateDelayTimeState()
{
    if (m_amb_dlg && ::IsWindow(m_amb_dlg->m_hWnd))
        m_amb_dlg->UpdateDelayTimeState();
}
