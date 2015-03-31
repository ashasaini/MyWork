//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-Redmine-780
//TMSC-REDMINE-777
//TMSC-REDMINE-774

// PQMManualPreScanDlg.cpp : implementation file
//
#include "stdafx.h"
#include "PQMManualPreScanDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "InterfaceForPQMManualPreScanDlg.h"
#include "PQMUtility.h"
#include "DPSManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

#define RIBBON_GAP		8
//Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2
//#define BK_COLOR RGB(63,82,118)

BEGIN_MESSAGE_MAP(CPQMManualPreScanDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMManualPreScanDlg)
    //}}AFX_MSG_MAP
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPQMManualPreScanDlg dialog

CPQMManualPreScanDlg::CPQMManualPreScanDlg(BaseInterfaceForPQMManualPreScanDlg* parent_interface, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_PQMMANUALPRESCAN_DIALOG, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_manual_prescan_values(new s_manual_prescan()),
    m_apply_button_state(FALSE),
    m_parent_interface(parent_interface),
    m_show_wait_cursor_for_manualprescandlg(false)
{
}

CPQMManualPreScanDlg::~CPQMManualPreScanDlg(
)
{
    DEL_PTR(m_manual_prescan_values)
}


//************************************Method Header***************
// Method Name  : GetManualPreScanDefaultData
// Author       : PATNI/ JS
// Purpose      :
//***************************************************************
BOOL CPQMManualPreScanDlg::GetManualPreScanDefaultData(
)
{
    //+Patni-AMT/2009Jul02/Added/JFT# Manual Prescan Onsite Crash

    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::GetManualPreScanDefaultData");

    VARIANT* l_pdata = new VARIANT;
    memset(l_pdata, NULL, sizeof(VARIANT));
    BOOL l_ret_value = E_NO_ERROR;

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_parent_interface != NULL) {
        l_ret_value = m_parent_interface->GetManualPreScanDefaultData(&l_pdata);

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("GetDlgItem(ubtnID) pointer is NULL"));

        return FALSE;
    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (l_ret_value != E_ERROR) {

        if (l_pdata != NULL && l_pdata->byref != NULL) {

            manual_prescan_ipc_t l_data;
            memset(&l_data, 0, sizeof(manual_prescan_ipc_t));
            memcpy(&l_data, l_pdata->byref, sizeof(manual_prescan_ipc_t));

            CString l_s_fsy(_T(""));

            if (l_data.f_fsy != 0) {
                l_s_fsy.Format(_T("%f"), l_data.f_fsy);
            }

            m_cfa_fsyvalue_edit.SetWindowText(l_s_fsy);

            CString l_s_rfl_gain(_T("0.0"));
            CString l_s_rfl_scale(_T("0.0"));

            if (l_data.f_rfl_gain != 0) {
                l_s_rfl_gain.Format(_T("%f"), l_data.f_rfl_gain);
                l_s_rfl_scale.Format(_T("%f"), l_data.f_rfl_scale);

            }

            m_rfl_gain_edit.SetWindowText(l_s_rfl_gain);
            m_rfl_scale_edit.SetWindowText(l_s_rfl_scale);

            CString l_s_rgn(_T("0.0"));

            if (l_data.f_rgn != 0) {
                l_s_rgn.Format(_T("%f"), l_data.f_rgn);
            }

            m_rxgain_rgnvalue_edit.SetWindowText(l_s_rgn);

            m_rfl_option_combo.SetCurSel(l_data.f_rfl_action);
            m_cfa_option_combo.SetCurSel(l_data.f_cfa_action);
            m_rxgain_option_combo.SetCurSel(l_data.f_rxgain_action);

            //+Patni-SS/ADDED/2009-June-01/JFT manual prescan#244

            //+REDMINE-781
            /*
            if (((manual_prescan_ipc_t*)l_pdata->byref)->post_acq_mode) {
                m_vapthrough_combo.EnableWindow(true);
                m_vapthrough_combo.SetCurSel(1);

            } else {

                m_vapthrough_combo.EnableWindow(false);
                m_vapthrough_combo.SetCurSel(0);
            }
            */
            m_vapthrough_combo.EnableWindow((((manual_prescan_ipc_t*)l_pdata->byref)->post_acq_mode) > 0);
            m_vapthrough_combo.SetCurSel((((manual_prescan_ipc_t*)l_pdata->byref)->post_acq_mode));
            //-REDMINE-781

            //+Patni-SS/ADDED/2009-June-01/JFT manual prescan#244

        } else {

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME, _T("l_pdata->byref is NULL  GUI"));

        }
    }

    DEL_PTR(l_pdata)

    return 0;
    //+Patni-AMT/2009Jul02/Added/JFT# Manual Prescan Onsite Crash
}

//************************************Method Header************************
// Method Name  : SetManualPreScanApplyBtnState
// Author       : PATNI/ JS
// Purpose      :
//************************************************************************
void CPQMManualPreScanDlg::SetManualPreScanApplyBtnState(
    const BOOL f_button_state
)
{
    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::SetManualPreScanApplyBtnState");

    //Applying Code review given by Tanoue-san
    LPCTSTR l_log_str = m_apply_button_state ? _T("Enabling ManualPrescan [Apply] Button") :
                        _T("Disabling ManualPrescan [Apply] Button") ;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    m_apply_button_state = f_button_state; // added by 861
    m_apply_button.EnableWindow(f_button_state);
}


//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMManualPreScanDlg::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_manualprescandlg = f_show_wait_cursor;
    SendMessage(WM_SETCURSOR, 0, 0);
}
void CPQMManualPreScanDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMManualPreScanDlg)
    DDX_Control(pDX, IDC_TXCOIL_COMBO, m_txcoil_combo);
    DDX_Control(pDX, IDC_RXCOIL_COMBO , m_rxcoil_combo);
    DDX_Control(pDX, IDC_RFL_COMBO , m_rfl_combo);
    DDX_Control(pDX, IDC_CFA_COMBO , m_cfa_combo);
    DDX_Control(pDX, IDC_RXGAIN_COMBO , m_rxgain_combo);
    DDX_Control(pDX, IDC_VAPTHROUGH_COMBO , m_vapthrough_combo);
    DDX_Control(pDX, IDC_RFL_OPTION_COMBO , m_rfl_option_combo);
    DDX_Control(pDX, IDC_CFA_OPTION_COMBO , m_cfa_option_combo);
    DDX_Control(pDX, IDC_RXGAIN_OPTION_COMBO, m_rxgain_option_combo);
    DDX_Control(pDX, IDC_TXCOIL_STATIC, m_txcoil_static);
    DDX_Control(pDX, IDC_RXCOIL_STATIC , m_rxcoil_static);
    DDX_Control(pDX, IDC_RFL_STATIC, m_rfl_static);
    DDX_Control(pDX, IDC_CFA_STATIC, m_cfa_static);
    DDX_Control(pDX, IDC_RXGAIN_STATIC , m_rxgain_static);
    DDX_Control(pDX, IDC_VAPTHROUGH_STATIC , m_vapthrough_static);
    DDX_Control(pDX, IDC_RFL_GAIN_EDIT , m_rfl_gain_edit);
    DDX_Control(pDX, IDC_RFL_SCALE_EDIT , m_rfl_scale_edit);
    DDX_Control(pDX, IDC_CFA_FSYVALUE_EDIT , m_cfa_fsyvalue_edit);
    DDX_Control(pDX, IDC_RXGAIN_RGNVALUE_EDIT , m_rxgain_rgnvalue_edit);
    DDX_Control(pDX, IDOK, m_apply_button);
    DDX_Control(pDX, IDCANCEL, m_cancel_button);
    DDX_Control(pDX, IDC_PRESCAN_MAIN_STATIC_FRAME, m_main_static_frame);

    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPQMManualPreScanDlg message handlers


BOOL CPQMManualPreScanDlg::OnInitDialogImpl()
{

    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);



    SetButtonAppearence();
    SetDialogRibbonHeight();

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (NULL == m_manual_prescan_values) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_manual_prescan_values pointer is NULL"));

        return FALSE;
    }

    if (NULL == m_parent_interface) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_parent_interface pointer is NULL"));

        return FALSE;
    }

    m_txcoil_combo.SetCurSel(!m_manual_prescan_values->bTxCoil);
    m_rxcoil_combo.SetCurSel(!m_manual_prescan_values->bRxCoil);
    m_rfl_combo.SetCurSel(!m_manual_prescan_values->bRFL);
    m_cfa_combo.SetCurSel(!m_manual_prescan_values->bCFA);
    m_rxgain_combo.SetCurSel(!m_manual_prescan_values->bRxGain);
    //Patni-PJS/2010Mar31/Added/MVC007400
    m_vapthrough_combo.SetCurSel(!m_manual_prescan_values->bVap);
    m_rfl_option_combo.SetCurSel(m_manual_prescan_values->rfl_option);
    m_rxgain_option_combo.SetCurSel(m_manual_prescan_values->rx_gain_option);
    m_cfa_option_combo.SetCurSel(m_manual_prescan_values->cfa_option);

    m_rfl_gain_edit.SetWindowText(m_manual_prescan_values->rfl_gain);
    m_rfl_scale_edit.SetWindowText(m_manual_prescan_values->rfl_scale);
    m_cfa_fsyvalue_edit.SetWindowText(m_manual_prescan_values->fsy_value);
    m_rxgain_rgnvalue_edit.SetWindowText(m_manual_prescan_values->rgn_value);

    m_apply_button.EnableWindow(FALSE);
    m_apply_button_state = FALSE;

    m_parent_interface->SetPrescanHook(TRUE);

    SetMultilingual();


    // TODO: Add extra initialization here
    return CZOrderDialog::OnInitDialogImpl(false);  // return TRUE  unless you set the focus to a control
}


void CPQMManualPreScanDlg::OnOK()
{
    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    // TODO: Add extra validation here
    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_parent_interface != NULL) {

        m_parent_interface->SetPrescanHook(TRUE);
        GetControlValues();

        VARIANT* l_pdata = new VARIANT;
        l_pdata->byref = (PVOID)m_manual_prescan_values;
        m_parent_interface->PQMManualPrescanApply(l_pdata);

        //DestroyWindow();
        m_apply_button.EnableWindow(FALSE);

        m_apply_button_state = FALSE; //added by sarjeet Defect # 861

        l_pdata->byref = NULL ;

        DEL_PTR(l_pdata)

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent_interface pointer is NULL"));

        return;
    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
}

void CPQMManualPreScanDlg::OnCancel()
{
    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    GetControlValues();

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_parent_interface != NULL) {
        m_parent_interface->SetPrescanHook(FALSE);

        if (m_apply_button_state == TRUE) { // added by sarjeet for defect #861
            m_parent_interface->PQMManualPrescanClose();
        }

        DestroyWindow() ;

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent_interface pointer is NULL"));

        return;
    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMManualPreScanDlg::OnSetCursor(CWnd* pWnd,
                                       UINT nHitTest,
                                       UINT message)
{
    if (m_show_wait_cursor_for_manualprescandlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CPQMManualPreScanDlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_cursor_for_manualprescandlg) {
        if ((pMsg->message == WM_KEYUP) ||
            (pMsg->message == WM_KEYDOWN) ||
            (pMsg->message == WM_LBUTTONDOWN) ||
            (pMsg->message == WM_RBUTTONDOWN) ||
            (pMsg->message == WM_NCLBUTTONDOWN) ||
            (pMsg->message == WM_LBUTTONDBLCLK) ||
            (pMsg->message == WM_RBUTTONDBLCLK) ||
            (pMsg->message == WM_MOUSEWHEEL))

            return TRUE ;
    }

    //+ Patni-PJS/2009May25/Added/IDS Defect #445 solved
    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
        pMsg->wParam = NULL;
    }

    //- Patni-PJS/2009May25/Added/IDS Defect #445 solved
    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}

//+Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
void CPQMManualPreScanDlg::LogManualPrescanValues(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::LogManualPrescanValues");

    if (!m_manual_prescan_values) {
        return ;
    }

    //+Patni-Hemant/2010Nov17/Modified/REDMINE-778_Part2/Incorprating Tanoue-san's Comments

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_manual_prescan_values->bTxCoil ? _T("TxCoil : ON") : _T("TxCoil : OFF"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_manual_prescan_values->bRxCoil ? _T("RxCoil : ON") : _T("RxCoil : OFF"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_manual_prescan_values->bRFL ? _T("RFL : ON") : _T("RFL : OFF"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_manual_prescan_values->bCFA ? _T("CFA : ON") : _T("CFA : OFF"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_manual_prescan_values->bRxGain ? _T("RxGain : ON") : _T("RxGain : OFF"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              m_manual_prescan_values->bVap ? _T("VAP Through : ON") : _T("VAP Through : OFF"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("RFL value = ") + GetPrescanString(m_rfl_option_combo.GetCurSel()));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("CFA value = ") + GetPrescanString(m_cfa_option_combo.GetCurSel()));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("RXGain value = ") + GetPrescanString(m_rxgain_option_combo.GetCurSel()));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("rfl_gain = ") + m_manual_prescan_values->rfl_gain);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("rfl_scale = ") + m_manual_prescan_values->rfl_scale);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("fsy_value = ") + m_manual_prescan_values->fsy_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("rgn_value = ") + m_manual_prescan_values->rgn_value);
    //-Patni-Hemant/2010Nov17/Modified/REDMINE-778_Part2/Incorprating Tanoue-san's Comments
}
//-Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2


//************************************Method Header************************************
// Method Name  : SetButtonAppearence
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMManualPreScanDlg::SetButtonAppearence(
)
{
    SetButtonFont();

    m_apply_button.SetCurveType(CURVE_LARGE_5);
    m_cancel_button.SetCurveType(CURVE_LARGE_5);

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);

    const int static_controls = 6;
    CCustomStatic* color_statics[static_controls] = { &m_txcoil_static, &m_rxcoil_static, &m_rfl_static, &m_cfa_static, &m_rxgain_static, &m_vapthrough_static} ;

    for (int ctrl_index = 0; ctrl_index < static_controls ; ctrl_index ++) {
        color_statics[ctrl_index]->LoadDeafultStaticCtrlColorsWithBKColor(stColors_o.crClientColor);
    }

    //-------------------------------------------------------------------------
    //for combos..
    const int total_combos = 9 ;
    ComboBoxEx* dialog_combos [total_combos] =  { &m_txcoil_combo, &m_rxcoil_combo, &m_rfl_combo, &m_cfa_combo, &m_rxgain_combo,
            &m_vapthrough_combo, &m_rfl_option_combo, &m_cfa_option_combo,
            &m_rxgain_option_combo
                                                } ;

    LPCTSTR static_font_name = _T("Arial");
    const int conbo_text_size = 9 ;

    for (ctrl_index = 0; ctrl_index < total_combos; ctrl_index++) {

        dialog_combos[ctrl_index]->SetFont(static_font_name, conbo_text_size);
    }

    //-------------------------------------------------------------------------

    //for edit..
    const int total_edits = 9 ;
    EditCtrlEx* dialog_edits [total_edits] =  { &m_rfl_gain_edit, &m_rfl_scale_edit, &m_cfa_fsyvalue_edit, &m_rxgain_rgnvalue_edit };

    for (ctrl_index = 0; ctrl_index < total_edits; ctrl_index++) {

        dialog_edits[ctrl_index]->SetFont(static_font_name, conbo_text_size);
    }
}


//Language usha
void CPQMManualPreScanDlg::SetMultilingual()
{

    //dialog title
    CString l_chOutput = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PQMMANUALPRESCAN_DLG_TITLE"));

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        CWnd ::SetWindowText(l_chOutput);
    }

    // other controls
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_TXCOIL_STATIC"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_TXCOIL_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;



    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_RXCOIL_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_RXCOIL_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_RFL_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_RFL_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CFA_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_CFA_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_RXGAIN_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_RXGAIN_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_VAPTHROUGH_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_VAPTHROUGH_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_VAP_OK"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_VAP_CANCEL"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

}


//************************************Method Header***********************
// Method Name  : GetControlValues
// Author       : PATNI/ JS
// Purpose      :
//***********************************************************************
void CPQMManualPreScanDlg::GetControlValues(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::GetControlValues");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_manual_prescan_values != NULL) {
        // ---------------TXCOIL------------------------//
        m_manual_prescan_values->bTxCoil = m_txcoil_combo.GetCurSel() ? FALSE : TRUE ;
        // ---------------------------------------------//

        // ---------------RXCOIL------------------------//
        m_manual_prescan_values->bRxCoil = m_rxcoil_combo.GetCurSel() ? FALSE : TRUE ;
        // ---------------------------------------------//

        // ---------------RFL------------------------//
        m_manual_prescan_values->bRFL = m_rfl_combo.GetCurSel() ? FALSE : TRUE ;
        // -------------------------------------------//

        // ---------------CFA------------------------//
        m_manual_prescan_values->bCFA = m_cfa_combo.GetCurSel() ? FALSE : TRUE ;
        // -------------------------------------------//

        // ---------------RXGAIN------------------------//
        m_manual_prescan_values->bRxGain = m_rxgain_combo.GetCurSel() ? FALSE : TRUE ;
        // -------------------------------------------//


        // ---------------VAP THROUGH------------------------//
        m_manual_prescan_values->bVap = m_vapthrough_combo.GetCurSel() ? FALSE : TRUE ;
        // -------------------------------------------//


        // ---------------RFL OPTION------------------------//
        m_manual_prescan_values->rfl_option = m_rfl_option_combo.GetCurSel();
        // -------------------------------------------//


        // ---------------CFA OPTION------------------------//
        m_manual_prescan_values->cfa_option = m_cfa_option_combo.GetCurSel();
        // -------------------------------------------//

        // ---------------RX GAIN OPTION------------------------//
        m_manual_prescan_values->rx_gain_option = m_rxgain_option_combo.GetCurSel();
        // -------------------------------------------//

        CString l_rfl_gain(_T(""));
        m_rfl_gain_edit.GetWindowText(l_rfl_gain);
        m_manual_prescan_values->rfl_gain = l_rfl_gain;

        CString l_rfl_scale(_T(""));
        m_rfl_scale_edit.GetWindowText(l_rfl_scale);
        m_manual_prescan_values->rfl_scale = l_rfl_scale;

        CString l_fsy_value(_T(""));
        m_cfa_fsyvalue_edit.GetWindowText(l_fsy_value);
        m_manual_prescan_values->fsy_value = l_fsy_value;

        CString l_rgn_value(_T(""));
        m_rxgain_rgnvalue_edit.GetWindowText(l_rgn_value);
        m_manual_prescan_values->rgn_value = l_rgn_value;

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_manual_prescan_values pointer is NULL"));

        return;
    }

    //Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
    LogManualPrescanValues();

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
}


//************************************Method Header************************
// Method Name  : SetPrescanHook
// Author       : PATNI/ JS
// Purpose      :
//*************************************************************************
void CPQMManualPreScanDlg::SetPrescanHook(
    const BOOL f_prescan_hook
)
{
    LPCTSTR FUNC_NAME = _T("CPQMManualPreScanDlg::SetPrescanHook");
    PQM_TRACE_SCOPED(FUNC_NAME);


    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_parent_interface != NULL) {
        m_parent_interface->SetPrescanHook(f_prescan_hook);

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent_interface pointer is NULL"));

        return;
    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
}


//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMManualPreScanDlg::SetButtonFont(
)
{

    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_apply_button.SetPrimaryTextFont(button_font_name, size);
    m_apply_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_cancel_button.SetPrimaryTextFont(button_font_name, size);
    m_cancel_button.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);


    return TRUE;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMManualPreScanDlg::SetDialogRibbonHeight(
)
{

    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_apply_button.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}


//+Patni-Hemant/2010Nov17/Added/REDMINE-778_Part2/Incorprating Tanoue-san's Comments
const CString CPQMManualPreScanDlg::GetPrescanString(
    const int f_mode
)  const
{

    switch (f_mode) {
        case 1:
            return _T("Use Previous");

        case 2:
            return _T("Use Varfieldl");

        case 0:
        default:
            return _T("Clinical");
    }

    return _T("");
}
//-Patni-Hemant/2010Nov17/Added/REDMINE-778_Part2/Incorprating Tanoue-san's Comments
