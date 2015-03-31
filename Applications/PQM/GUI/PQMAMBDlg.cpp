// PQMAMBDlg.cpp: implementation of the CPQMAMBDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PQMAMBDlg.h"
#include <math.h>

#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include <PQM/PQMCommanMessages.h>
#include "DPSManager.h"
#include "UIButtonUtility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;
#define COMBO_HEIGHT					27
#define RIBBON_GAP		                8
#define FONT_NAME                       _T("Arial BOLD")
#define FONT_SIZE                       13
#define SMALL_FONT                      11
#define PQM_MSG_AMB_SCAN_START			1
#define PQM_MSG_AMB_SCAN_RESTART		2
#define PQM_MSG_AMB_SCAN_ABORT			4
#define PQM_MSG_AMB_ERROR				5

BEGIN_MESSAGE_MAP(CPQMAMBDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMAMBDlg)
    //to show hour glass
    ON_WM_SETCURSOR()
    //to show hour glass
    ON_BN_CLICKED(ID_AMB_START, OnClickButtonAMBScanStart)
    ON_BN_CLICKED(ID_AMB_ABORT, OnClickButtonAMBScanAbort)
    ON_BN_CLICKED(IDC_AMB_RADIO_SUB1, OnClickRadioButtonSeg2)
    ON_BN_CLICKED(IDC_AMB_RADIO_SUB2, OnClickRadioButtonSeg3)
    ON_BN_CLICKED(IDC_AMB_RADIO_SUB3, OnClickRadioButtonSeg4)
    ON_COMMAND(ID_ITERATION1, OnMenuIteration1)
    ON_COMMAND(ID_ITERATION2, OnMenuIteration2)
    ON_COMMAND(ID_ITERATION3, OnMenuIteration3)
    ON_COMMAND(ID_ITERATION4, OnMenuIteration4)

    ON_BN_CLICKED(IDC_AMB_CHECK_SHIM, OnCheckShim)
    ON_BN_CLICKED(IDC_AMB_CHECK_MAP, OnCheckMap)
    ON_BN_CLICKED(ID_AMB_PREP, OnAMBPrep)

    ON_CBN_SELCHANGE(IDC_AMB_MASTER_DIRECTION, OnSelchangeAmbMasterDirection)
    ON_CBN_SELCHANGE(IDC_AMB_SUB1_DIRECTION, OnSelchangeAmbSub2Direction)
    ON_CBN_SELCHANGE(IDC_AMB_SUB2_DIRECTION, OnSelchangeAmbSub3Direction)
    ON_CBN_SELCHANGE(IDC_AMB_SUB3_DIRECTION, OnSelchangeAmbSub4Direction)

    ON_WM_LBUTTONDOWN()
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPQMAMBDlg::CPQMAMBDlg(CAMBDialogInterface* f_ambinterface, const CRect& parent_rect, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_AMB_DIALOG, pParent),
    m_ambdialog_utility(m_amb_status),
    m_parent_rect(parent_rect),
    m_dialog_rect(CRect(0, 0, 0, 0)),
    m_iteration_menu_seg(0),
    m_iteration_menu_station(0)

{


    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::CPQMAMBDlg");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("Trace of Constructor"));

    m_ambinterface = f_ambinterface;
    m_wait_cursor = LoadCursor(NULL, IDC_WAIT);
    memset(&m_amb_status, 0, sizeof(m_amb_status));

}

CPQMAMBDlg::~CPQMAMBDlg()
{
    if (m_wait_cursor) {
        DestroyCursor(m_wait_cursor);
    }

    if (m_org_cursor) {
        DestroyCursor(m_org_cursor);
    }
}

void CPQMAMBDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMAMBDlg)
    DDX_Control(pDX, ID_AMB_START, m_start_button);
    DDX_Control(pDX, IDCANCEL , m_cancel_button);
    DDX_Control(pDX, ID_AMB_ABORT , m_abort_button);
    DDX_Control(pDX, ID_AMB_PREP , m_prep_button);

    DDX_Control(pDX, IDC_AMB_MASTER_DIRECTION , m_master_direction_combo);
    DDX_Control(pDX, IDC_AMB_SUB1_DIRECTION , m_sub1_direction_combo);
    DDX_Control(pDX, IDC_AMB_SUB2_DIRECTION , m_sub2_direction_combo);
    DDX_Control(pDX, IDC_AMB_SUB3_DIRECTION , m_sub3_direction_combo);
    DDX_Control(pDX, IDC_AMB_COMBO_MAPAUTOVOICE, m_map_autovoice_combo);

    DDX_Control(pDX, IDC_MASTER_SETTING_LABEL, m_master_setting_label);
    DDX_Control(pDX, IDC_AMB_STATIC_STATION , m_station_lable);
    DDX_Control(pDX, IDC_AMB_STATIC_MASTER , m_master);
    DDX_Control(pDX, IDC_AMB_STATIC_MAPAUTOVOICE , m_map_autovoice);

    DDX_Control(pDX, IDC_AMB_RADIO_SUB1 , m_sub1);

    DDX_Control(pDX, IDC_AMB_CHECK_SHIM , m_shim);
    DDX_Control(pDX, IDC_AMB_CHECK_MAP , m_map);

    DDX_Control(pDX, IDC_STATION_FRAME, m_station_frame);
    DDX_Control(pDX, IDC_AMB_FRAME1, m_fram_1);
    DDX_Control(pDX, IDC_AMB_FRAME2, m_fram_2);

    DDX_Control(pDX, IDC_STATION1_SEG1, m_amb_segment_editbox[0][0]);
    DDX_Control(pDX, IDC_STATION1_SEG2, m_amb_segment_editbox[1][0]);
    DDX_Control(pDX, IDC_STATION1_SEG3, m_amb_segment_editbox[2][0]);
    DDX_Control(pDX, IDC_STATION1_SEG4, m_amb_segment_editbox[3][0]);

    DDX_Control(pDX, IDC_STATION2_SEG1, m_amb_segment_editbox[0][1]);
    DDX_Control(pDX, IDC_STATION2_SEG2, m_amb_segment_editbox[1][1]);
    DDX_Control(pDX, IDC_STATION2_SEG3, m_amb_segment_editbox[2][1]);
    DDX_Control(pDX, IDC_STATION2_SEG4, m_amb_segment_editbox[3][1]);

    DDX_Control(pDX, IDC_STATION3_SEG1, m_amb_segment_editbox[0][2]);
    DDX_Control(pDX, IDC_STATION3_SEG2, m_amb_segment_editbox[1][2]);
    DDX_Control(pDX, IDC_STATION3_SEG3, m_amb_segment_editbox[2][2]);
    DDX_Control(pDX, IDC_STATION3_SEG4, m_amb_segment_editbox[3][2]);

    DDX_Control(pDX, IDC_STATION4_SEG1, m_amb_segment_editbox[0][3]);
    DDX_Control(pDX, IDC_STATION4_SEG2, m_amb_segment_editbox[1][3]);
    DDX_Control(pDX, IDC_STATION4_SEG3, m_amb_segment_editbox[2][3]);
    DDX_Control(pDX, IDC_STATION4_SEG4, m_amb_segment_editbox[3][3]);

    DDX_Control(pDX, IDC_STATION5_SEG1, m_amb_segment_editbox[0][4]);
    DDX_Control(pDX, IDC_STATION5_SEG2, m_amb_segment_editbox[1][4]);
    DDX_Control(pDX, IDC_STATION5_SEG3, m_amb_segment_editbox[2][4]);
    DDX_Control(pDX, IDC_STATION5_SEG4, m_amb_segment_editbox[3][4]);

    DDX_Control(pDX, IDC_STATION6_SEG1, m_amb_segment_editbox[0][5]);
    DDX_Control(pDX, IDC_STATION6_SEG2, m_amb_segment_editbox[1][5]);
    DDX_Control(pDX, IDC_STATION6_SEG3, m_amb_segment_editbox[2][5]);
    DDX_Control(pDX, IDC_STATION6_SEG4, m_amb_segment_editbox[3][5]);

    DDX_Control(pDX, IDC_STATION_LABEL1, m_amb_staion_label[0]);
    DDX_Control(pDX, IDC_STATION_LABEL2, m_amb_staion_label[1]);
    DDX_Control(pDX, IDC_STATION_LABEL3, m_amb_staion_label[2]);
    DDX_Control(pDX, IDC_STATION_LABEL4, m_amb_staion_label[3]);
    DDX_Control(pDX, IDC_STATION_LABEL5, m_amb_staion_label[4]);
    DDX_Control(pDX, IDC_STATION_LABEL6, m_amb_staion_label[5]);

    //}}AFX_DATA_MAP
}


/*************************************************************************************

//Module:  OnInitDialogImpl

//Author:  iGATE

//Purpose: Intialize dialog data and populates on dialog box.

//**************************************************************************************/
BOOL CPQMAMBDlg::OnInitDialogImpl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    VARIANT l_pData;
    l_pData.byref = &GetAMBStatus();
    m_ambinterface->GetAmbPlan(&l_pData);

    CreateControlsForStation();
    CreateEditControlsForSegments();
    SetButtonStyle();

    SetFont(_T("Arial"), 14);

    m_map_autovoice.ShowWindow(SW_HIDE);
    m_map_autovoice_combo.ShowWindow(SW_HIDE);

    if (!m_parent_rect.IsRectEmpty() && m_dialog_rect.IsRectEmpty()) {

        CRect l_window_rect;
        GetWindowRect(&l_window_rect);

        m_dialog_rect = l_window_rect;
        m_dialog_rect.left = m_parent_rect.right;
        m_dialog_rect.top = m_parent_rect.top ;
        m_dialog_rect.right = m_dialog_rect.left + l_window_rect.Width();
        m_dialog_rect.bottom = m_dialog_rect.top + l_window_rect.Height();
    }

    SetWindowPos(&wndTop , m_dialog_rect.left , m_dialog_rect.top , m_dialog_rect.Width(), m_dialog_rect.Height(), SWP_NOSIZE | SWP_NOZORDER);


    AmbEditUiScanStateSet(AmbEditScanStateSetting);

    AmbEditRefresh();

    EnabledisableAMBScanRelatedButtons(m_ambinterface->IsAcqumanReadyForNextRequest());
    //SetMultilingual();
    SetDialogRibbonHeight();
    return CZOrderDialog::OnInitDialogImpl(false); // return TRUE unless you set the focus to a control

}

//************************************Method Header************************************
// Method Name  : SetButtonStyle
// Author       : iGATE
// Purpose      : Initially setting controls property
//***********************************************************************************
void CPQMAMBDlg::SetButtonStyle(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::SetButtonStyle");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //for button
    m_start_button.SetCurveType(CURVE_SMALL_3);
    m_cancel_button.SetCurveType(CURVE_SMALL_3);
    m_abort_button.SetCurveType(CURVE_SMALL_3);

    m_prep_button.SetButtonStyle(BS_PUSHBUTTON);
    m_prep_button.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_prep_button.GetButtonColors(m_btnDefColor);
    m_btnDefColor.crPrimaryCaptionNormal = RGB(255, 255, 255) ;
    m_btnDefColor.crFocusRect = m_btnDefColor.crBrushFaceStart;
    m_prep_button.SetButtonColors(m_btnDefColor);


    m_start_button.SetPrimaryTextFont(FONT_NAME, FONT_SIZE);
    m_cancel_button.SetPrimaryTextFont(FONT_NAME, FONT_SIZE);
    m_abort_button.SetPrimaryTextFont(FONT_NAME, FONT_SIZE);


    m_prep_button.SetPrimaryTextFont(FONT_NAME, SMALL_FONT);

    //for frame
    m_station_frame.SetFont(FONT_NAME, FONT_SIZE);
    m_fram_1.SetFont(FONT_NAME, FONT_SIZE);
    m_fram_2.SetFont(FONT_NAME, FONT_SIZE);

    m_fram_1.SetCurveType(CURVE_SMALL);
    m_fram_2.SetCurveType(CURVE_SMALL);

    //for combos..
    const int l_total_combos = 5 ;
    ComboBoxEx* l_dialog_combos [l_total_combos] =  { &m_master_direction_combo, &m_sub1_direction_combo,
            &m_sub2_direction_combo, &m_sub3_direction_combo, &m_map_autovoice_combo
                                                    } ;

    for (int ctrl_index = 0; ctrl_index < l_total_combos; ctrl_index++) {

        l_dialog_combos[ctrl_index]->SetFont(FONT_NAME, SMALL_FONT);
    }

    m_master_direction_combo.SetCurSel(0);
    m_sub1_direction_combo.SetCurSel(0);
    m_sub2_direction_combo.SetCurSel(0);
    m_sub3_direction_combo.SetCurSel(0);


    //Set combobox height
    m_master_direction_combo.SetItemHeight(-1, COMBO_HEIGHT);
    m_sub1_direction_combo.SetItemHeight(-1, COMBO_HEIGHT);
    m_sub2_direction_combo.SetItemHeight(-1, COMBO_HEIGHT);
    m_sub3_direction_combo.SetItemHeight(-1, COMBO_HEIGHT);



    WINDOW_COLORS_t l_stColors_o;
    GetWindowColors(l_stColors_o);
    COLORREF l_bk_color = l_stColors_o.crClientColor;

    //for static controls
    const int l_total_static = 4 ;
    CCustomStatic* l_dialog_statics [l_total_static] =  { &m_station_lable, &m_master_setting_label, &m_master, &m_map_autovoice};

    for (ctrl_index = 0; ctrl_index < l_total_static; ctrl_index++) {

        l_dialog_statics[ctrl_index]->LoadDeafultStaticCtrlColorsWithBKColor(l_bk_color);
        l_dialog_statics[ctrl_index]->SetFontAttributes(FONT_NAME, 130);
        l_dialog_statics[ctrl_index]->UpdateFont();
    }


    //for check boxes
    const int l_total_chkboxes = 2 ;
    CheckBoxEx* l_dialog_chkboxes [l_total_chkboxes] =  {&m_shim, &m_map};

    for (ctrl_index = 0; ctrl_index < l_total_chkboxes; ctrl_index++) {

        l_dialog_chkboxes[ctrl_index]->SetBkColor(l_bk_color);
        l_dialog_chkboxes[ctrl_index]->SetFont(FONT_NAME, FONT_SIZE);
    }

    //for for radio buttons
    m_sub1.SetBkColor(l_bk_color);
    m_sub1.SetFont(FONT_NAME, SMALL_FONT);

}

//************************************Method Header************************************
// Method Name  : SetDialogRibbonHeight
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::SetDialogRibbonHeight(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::SetDialogRibbonHeight");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CRect l_client_rect;
    GetClientRect(&l_client_rect);

    CRect l_start_button_rect ;
    m_start_button.GetWindowRect(&l_start_button_rect);
    ScreenToClient(&l_start_button_rect);
    SetRibbonHeight(l_client_rect.bottom - l_start_button_rect.top + 5);
}


//************************************Method Header************************************
// Method Name  : OnClickButtonAMBScanStart
// Author       : iGATE
// Purpose      : Start AMB Scan
//***********************************************************************************
void CPQMAMBDlg::OnClickButtonAMBScanStart(
)
{
    OnClickAMBScanStart();
}

//************************************Method Header************************************
// Method Name  : OnClickAMBScanStart
// Author       : iGATE
// Purpose      : Start AMB Scan
//***********************************************************************************
void CPQMAMBDlg::OnClickAMBScanStart(
    const bool f_is_irs_msg /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::OnClickButtonAMBScanStart");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Check Remote Access
    if (!f_is_irs_msg && (true == m_ambinterface->RemoteCheckActiveFlag())) {
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");

        return;
    }

    if (m_ambinterface->AMBScanStart()) {
        AmbEditUiScanStateSet(AmbEditScanStateScanning);
        AmbEditRefresh();
        m_ambinterface->OnClickScanStart();
    }

}
//************************************Method Header************************************
// Method Name  : CloseAMBOnError
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::CloseAMBOnError(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::CloseAMBOnError");
    PQM_TRACE_SCOPED(FUNC_NAME);

    OnClickButtonCancel();
}

//************************************Method Header************************************
// Method Name  : OnClose
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::OnClose()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::OnClose");
    PQM_TRACE_SCOPED(FUNC_NAME);

    OnCancel();
}

//************************************Method Header************************************
// Method Name  : OnCancel
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::OnCancel()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);
    DestroyWindow();
    OnClickButtonCancel();

}

//************************************Method Header************************************
// Method Name  : OnClickButtonCancel
// Author       : iGATE
// Purpose      : For Close the AMB Dialog
//***********************************************************************************
void CPQMAMBDlg::OnClickButtonCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::OnClickButtonCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    AmbEditUiScanStateSet(AmbEditScanStateSetting);

    m_ambinterface->AMBDlgCancel();

}

//************************************Method Header************************************
// Method Name  : OnClickButtonAMBScanAbort
// Author       : iGATE
// Purpose      : Abort AMB Scan
//***********************************************************************************
void CPQMAMBDlg::OnClickButtonAMBScanAbort(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::OnClickButtonAMBScanAbort");
    PQM_TRACE_SCOPED(FUNC_NAME);

    m_ambinterface->OnClickButtonScanAbort();

}
//************************************Method Header************************************
// Method Name  : CreateControlsForStation
// Author       : iGATE
// Purpose      : Create controls for stations
//***********************************************************************************
void CPQMAMBDlg::CreateControlsForStation()
{

    WINDOW_COLORS_t l_stColors_o;
    GetWindowColors(l_stColors_o);
    COLORREF l_bk_color = l_stColors_o.crClientColor;

    for (int l_stn_lbl_index = 0 ; l_stn_lbl_index < AMB_STATION_MAX ; l_stn_lbl_index++) {
        m_amb_staion_label[l_stn_lbl_index].LoadDeafultStaticCtrlColorsWithBKColor(l_bk_color);
        m_amb_staion_label[l_stn_lbl_index].SetFontAttributes(FONT_NAME, 130);
        m_amb_staion_label[l_stn_lbl_index].UpdateFont();
        m_amb_staion_label[l_stn_lbl_index].ShowWindow(SW_HIDE);

    }
}

//************************************Method Header************************************
// Method Name  : CreateEditControlsForSegments
// Author       : PATNI / SANJAY
// Purpose      : Create controls for segments
//***********************************************************************************
void CPQMAMBDlg::CreateEditControlsForSegments()
{

    for (int l_segment_index = 1; l_segment_index <= AMB_SEGMENT_MAX; l_segment_index++) {
        for (int l_station_index = 1 ; l_station_index <= AMB_STATION_MAX ; l_station_index++) {
            EditCtrlEx& l_edit = GetSegEditForStation(l_segment_index, l_station_index) ;
            UdpateEditStyle(l_edit) ;
        }
    }

}
//************************************Method Header************************************
// Method Name  : UdpateEditStyle
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::UdpateEditStyle(
    EditCtrlEx& f_edit
)
{
    const CString l_font_name = _T("Arial") ;

    f_edit.SetReadOnly(TRUE);
    f_edit.SetBorderColorReadOnly(RGB(0, 0, 0));
    f_edit.SetTextAlignment(VCENTER);
    f_edit.SetFont(l_font_name, SMALL_FONT);
    f_edit.ShowWindow(SW_HIDE);

}

//************************************Method Header************************************
// Method Name  : AmbEditUiShimmingSet
// Author       : iGATE
// Purpose      : Set shimming status
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiShimmingSet(const int f_status)
{
    m_shim.SetCheck((f_status == ON) ? 1 : 0);
}

//************************************Method Header************************************
// Method Name  : AmbEditUiMapSet
// Author       : iGATE
// Purpose      : Set MAP status
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiMapSet(const int f_status)
{
    m_map.SetCheck((f_status == ON) ? 1 : 0);
    m_map.EnableWindow(FALSE);
}


//************************************Method Header************************************
// Method Name  : AmbEditUiPrepSet
// Author       : iGATE
// Purpose      : Set MAP status
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiPrepSet(const int f_status, const int f_prepEnableStatus)
{

    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::AmbEditUiPrepSet");
    PQM_TRACE_SCOPED(FUNC_NAME);

    BOOL l_prep_status, l_prepEnable;

    l_prep_status = (f_status == ON) ? TRUE : FALSE;
    l_prepEnable = (f_prepEnableStatus == ENABLE) ? true : false;

    BUTTON_COLORS_t l_button_color = m_btnDefColor;

    if (l_prep_status) {

        l_button_color.crBrushFaceStart = RGB(49, 150, 255) ;
        l_button_color.crBrushFaceEnd = RGB(24, 73, 132) ;

        l_button_color.crFocusRect = l_button_color.crBrushFaceStart;
        l_button_color.crPushedFaceStart = l_button_color.crBrushFaceStart;
        l_button_color.crPushedFaceEnd = l_button_color.crBrushFaceEnd;
        l_button_color.crPressedSurround = l_button_color.crBrushFaceStart;

        m_prep_button.SetButtonColors(l_button_color);

    } else {
        m_prep_button.SetButtonColors(m_btnDefColor);
    }

    m_prep_button.EnableWindow(l_prepEnable);
}

//************************************Method Header************************************
// Method Name  : UpdateSegmentEditForSeg
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::UpdateSegmentEditForSeg(const int f_count)
{

    for (int l_segment_index = 0; l_segment_index < AMB_SEGMENT_MAX; l_segment_index++) {
        for (int l_station_index = 0 ; l_station_index < m_amb_status.numStation ; l_station_index++) {
            if (l_segment_index < f_count) {
                m_amb_segment_editbox[l_segment_index][l_station_index].ShowWindow(SW_SHOW);
                m_amb_segment_editbox[l_segment_index][l_station_index].SetBorderColorReadOnly(RGB(255, 255, 255));

            } else {
                m_amb_segment_editbox[l_segment_index][l_station_index].SetBorderColorReadOnly(RGB(0, 0, 0));
                m_amb_segment_editbox[l_segment_index][l_station_index].ShowWindow(SW_HIDE);
            }
        }
    }

    m_ambdialog_utility.AmbEditSetNumSegment(f_count);
}
//************************************Method Header************************************
// Method Name  : UpdateComboEditForSeg
// Author       : iGATE / SANJAY
// Purpose      :
//***********************************************************************************
void CPQMAMBDlg::UpdateComboEditForSeg(const int f_count)
{

    switch (f_count) {
        case 2:
            m_sub1_direction_combo.ShowWindow(SW_SHOW);
            m_sub2_direction_combo.ShowWindow(SW_HIDE);
            m_sub3_direction_combo.ShowWindow(SW_HIDE);
            break;

        case 3:
            m_sub1_direction_combo.ShowWindow(SW_SHOW);
            m_sub2_direction_combo.ShowWindow(SW_SHOW);
            m_sub3_direction_combo.ShowWindow(SW_HIDE);
            break;

        case 4:
            m_sub1_direction_combo.ShowWindow(SW_SHOW);
            m_sub2_direction_combo.ShowWindow(SW_SHOW);
            m_sub3_direction_combo.ShowWindow(SW_SHOW);
            break;
    }
}

//************************************Method Header************************************
// Method Name  : OnClickRadioButtonSeg2
// Author       : iGATE
// Purpose      : Handler of segment2 radion button click
//***********************************************************************************
void CPQMAMBDlg::OnClickRadioButtonSeg2(
)
{
    UpdateSegmentEditForSeg(2);
    UpdateComboEditForSeg(2);

    AmbEditRefresh();
}


//************************************Method Header************************************
// Method Name  : OnClickRadioButtonSeg3
// Author       : PATNI / SANJAY
// Purpose      : Handler of segment3 radion button click
//***********************************************************************************
void CPQMAMBDlg::OnClickRadioButtonSeg3(
)
{
    UpdateSegmentEditForSeg(3);
    UpdateComboEditForSeg(3);

    AmbEditRefresh();
}

//************************************Method Header************************************
// Method Name  : OnClickRadioButtonSeg4
// Author       : iGATE
// Purpose      : Handler of segment4 radion button click
//***********************************************************************************
void CPQMAMBDlg::OnClickRadioButtonSeg4(
)
{
    UpdateSegmentEditForSeg(4);
    UpdateComboEditForSeg(4);

    AmbEditRefresh();
}

//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMAMBDlg::OnSetCursor(CWnd* pWnd,
                             UINT nHitTest,
                             UINT message)
{
    if (m_ambinterface->IsWaitCursor()) {
        m_org_cursor = ::SetCursor(m_wait_cursor);
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}
//************************************Method Header************************************
// Method Name  : PreTranslateMessageImpl
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMAMBDlg::PreTranslateMessageImpl(MSG* pMsg)
{

    if (pMsg) {

        if ((m_ambinterface->IsWaitCursor()) ||
            ((AmbEditScanStateScanning == m_amb_scan_state || AmbEditScanStatePrep == m_amb_scan_state || AMBEditScanStateDelayTime == m_amb_scan_state || AMBEditScanStateAbort == m_amb_scan_state) &&
             (m_abort_button.GetSafeHwnd() != pMsg->hwnd && m_start_button.GetSafeHwnd() != pMsg->hwnd))) {

            if ((pMsg->message == WM_KEYUP) ||
                (pMsg->message == WM_KEYDOWN) ||
                (pMsg->message == WM_LBUTTONDOWN) ||
                (pMsg->message == WM_RBUTTONDOWN) ||
                (pMsg->message == WM_LBUTTONDBLCLK) ||
                (pMsg->message == WM_RBUTTONDBLCLK) ||
                (pMsg->message == WM_MOUSEWHEEL)
               )

                return TRUE ;
        }

        if ((pMsg->wParam == VK_RETURN) && (m_abort_button.GetSafeHwnd() != pMsg->hwnd && m_start_button.GetSafeHwnd() != pMsg->hwnd && m_cancel_button.GetSafeHwnd() != pMsg->hwnd)) {
            pMsg->message = NULL;
        }

        if (pMsg->message == WM_LBUTTONDOWN) {

            if (m_amb_scan_state == AmbEditScanStateScanning && (m_abort_button.GetSafeHwnd() != pMsg->hwnd && m_start_button.GetSafeHwnd() != pMsg->hwnd))
                return TRUE;

            if (m_abort_button.GetSafeHwnd() != pMsg->hwnd) {
                if (IgnoreLeftClickOnSegmentsAndStations(pMsg)) {
                    return TRUE;
                }
            }
        }

        if (pMsg->message == WM_RBUTTONDOWN) {

            if (m_amb_scan_state == AmbEditScanStateScanning && m_abort_button.GetSafeHwnd() != pMsg->hwnd)
                return TRUE;

            if (m_abort_button.GetSafeHwnd() != pMsg->hwnd) {
                if (LoadSegmentIterationMenu(pMsg)) {
                    return TRUE;
                }
            }
        }
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}


//************************************Method Header************************************
// Method Name : IgnoreLeftClickOnSegmentsAndStations
// Author      : iGATE
// Purpose     : Ignore left click on segments control
//***********************************************************************************

BOOL CPQMAMBDlg::IgnoreLeftClickOnSegmentsAndStations(MSG* pMsg)
{

    CRect l_edit_rect;

    for (int l_segment_index = 0; l_segment_index < AMB_SEGMENT_MAX; l_segment_index++) {
        for (int l_stn_index = 0 ; l_stn_index < m_amb_status.numStation ; l_stn_index++) {
            if (m_amb_segment_editbox[l_segment_index][l_stn_index].GetSafeHwnd() == pMsg->hwnd) {
                m_amb_segment_editbox[l_segment_index][l_stn_index].GetWindowRect(&l_edit_rect);

                ScreenToClient(&l_edit_rect);

                CWnd* window =  CWnd::FromHandle(pMsg->hwnd);
                CPoint point = (CPoint) pMsg->lParam ;
                window->ClientToScreen(&point);
                ScreenToClient(&point);

                if (l_edit_rect.PtInRect(CPoint(point.x, point.y)))
                    return TRUE;
            }
        }
    }

    return FALSE;
}
//************************************Method Header************************************
// Method Name : UpdateRepeat
// Author      : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::UpdateRepeat(const int f_rep)
{

    m_ambdialog_utility.AmbEditSetCellRepeat(m_iteration_menu_seg + 1, m_iteration_menu_station + 1, f_rep);
    AmbEditRefresh();
}

//************************************Method Header************************************
// Method Name : OnMenuIteration1
// Author      : iGATE
// Purpose     : Handler of Iteration1 menu
//***********************************************************************************
void CPQMAMBDlg::OnMenuIteration1()
{
    UpdateRepeat(1);
}

//************************************Method Header************************************
// Method Name : OnMenuIteration2
// Author      : iGATE
// Purpose     : Handler of Iteration2 menu
//***********************************************************************************
void CPQMAMBDlg::OnMenuIteration2()
{
    UpdateRepeat(2);
}

//************************************Method Header************************************
// Method Name : OnMenuIteration3
// Author      : iGATE
// Purpose     : Handler of Iteration3 menu
//***********************************************************************************
void CPQMAMBDlg::OnMenuIteration3()
{
    UpdateRepeat(3);
}

//************************************Method Header************************************
// Method Name : OnMenuIteration4
// Author      : iGATE
// Purpose     : Handler of Iteration4 menu
//***********************************************************************************
void CPQMAMBDlg::OnMenuIteration4()
{
    UpdateRepeat(4);
}


//************************************Method Header************************************
// Method Name : LoadSegmentIterationMenu
// Author      : iGATE
// Purpose     : Load segment iteration menu on segment control right click
//***********************************************************************************
bool CPQMAMBDlg::LoadSegmentIterationMenu(MSG* pMsg)
{

    for (int l_segment_index = 0; l_segment_index < AMB_SEGMENT_MAX; l_segment_index++) {
        for (int l_station_index = 0 ; l_station_index < m_amb_status.numStation ; l_station_index++) {
            if (m_amb_segment_editbox[l_segment_index][l_station_index].GetSafeHwnd() == pMsg->hwnd) {
                if ((0 == l_segment_index)) {
                    return true;
                }

                m_iteration_menu_seg = l_segment_index;
                m_iteration_menu_station = l_station_index;

                CRect l_edit_rect ;
                m_amb_segment_editbox[l_segment_index][l_station_index].GetWindowRect(&l_edit_rect);

                ScreenToClient(&l_edit_rect);

                CWnd* l_window =  CWnd::FromHandle(pMsg->hwnd);
                CPoint l_point = (CPoint) pMsg->lParam ;
                l_window->ClientToScreen(&l_point);

                PopupMenuEx m_iteration_context_menu;
                UpdateIterationMenu(&m_iteration_context_menu, m_iteration_menu_seg, m_iteration_menu_station);

                m_iteration_context_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, l_point.x,
                                                        l_point.y, this, 0, 0);

                return true;
            }
        }
    }

    return false;
}
//************************************Method Header************************************
// Method Name : UpdateIterationMenu
// Author      : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::UpdateIterationMenu(
    PopupMenuEx* f_iterationmenu,
    const int f_seg,
    const int f_station
)
{

    f_iterationmenu->LoadMenu(IDR_AMB_ITERATION_MENU);

    LPCTSTR l_font_name = _T("Arial");
    f_iterationmenu->SetFont(l_font_name, SMALL_FONT);

    const UINT f_command_id = ID_ITERATION1 + m_amb_status.cell[f_seg][f_station].repeat - 1 ;
    f_iterationmenu->CheckMenuItem(f_command_id, MF_BYCOMMAND | MF_CHECKED);
}

//************************************Method Header************************************
// Method Name : InitSettingforSegment2
// Author      : iGATE
// Purpose     : Initially setting of segment2 controls
//***********************************************************************************
void CPQMAMBDlg::InitSettingforSegment2()
{
    m_sub1.SetCheck(1);
    OnClickRadioButtonSeg2();
}
//************************************Method Header************************************
// Method Name : AmbEditRefresh
// Author      : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditRefresh()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::AmbEditRefresh");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int l_prepStatus;
    int l_prepEnableStatus;
    int l_firstStation;
    int l_isTop;
    int l_sensitive;
    int l_segmentIndex;


    //////////Update Status
    m_ambdialog_utility.ambEditRefreshPrep(&l_prepStatus, &l_prepEnableStatus);
    m_ambdialog_utility.ambEditRefreshAutoVoice();
    m_ambdialog_utility.ambEditRefreshRepeat();

    //Station
    AmbEditUiStationSet((int)m_amb_status.numStation);

    //Segemnt
    AmbEditUiSegmentSet((int)m_amb_status.numSegment);

    //Shimming
    AmbEditUiShimmingSet((int)m_amb_status.shimming);

    //Map
    AmbEditUiMapSet((int)m_amb_status.map);

    //prep : Button only
    AmbEditUiPrepSet(l_prepStatus, l_prepEnableStatus);

    //Direction : Button only
    for (l_segmentIndex = 1; l_segmentIndex <= AMB_SEGMENT_MAX; l_segmentIndex++) {
        AmbEditUiDirectionSet(l_segmentIndex, (int)m_amb_status.direction[l_segmentIndex - 1]);
    }

    for (int l_stationIndex = 1; l_stationIndex <= AMB_STATION_MAX; l_stationIndex++) {

        for (int l_segmentIndex = 1; l_segmentIndex <= AMB_SEGMENT_MAX; l_segmentIndex++) {

            m_ambdialog_utility.AmbEditFindFirstStation(l_segmentIndex, &l_firstStation);
            l_isTop = (l_stationIndex == l_firstStation) ? 1 : 0;
            l_sensitive = (l_segmentIndex <= (m_amb_status.numSegment)) ? 1 : 0;

            AmbEditUiScanCellSet(l_segmentIndex,
                                 l_stationIndex,
                                 l_isTop,
                                 (int)m_amb_status.cell[l_segmentIndex - 1][l_stationIndex - 1].prep,
                                 (int)m_amb_status.cell[l_segmentIndex - 1][l_stationIndex - 1].repeat,
                                 /*AmbProgress.NumberOfTimes*/
                                 l_sensitive);
        }
    }

    //Scan Buttons
    AmbEditUiScanButtonSet();
}

//************************************Method Header************************************
// Method Name : AmbEditUiScanCellSet
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiScanCellSet(
    const int f_segment,
    const int f_station,
    const int f_isTop,
    const int f_prep,
    const int f_repeat,
    const int f_sensitive
)
{


    char* l_nullSt = "";
    char* l_prepSt = "p";
    char* l_leftArrowSt = ">";
    char* l_rightArrowSt = "<";

    char* l_pSt = (f_prep == ON) ? l_prepSt : l_nullSt;
    char* l_lSt = ((f_isTop == 1) && (f_station == 1)) ? l_leftArrowSt : l_nullSt;
    char* l_rSt = ((f_isTop == 1) && (f_station != 1)) ? l_rightArrowSt : l_nullSt;

    CString l_tmpStr;

    if (f_sensitive == 1) {
        if (f_repeat != 1) {
            l_tmpStr.Format(_T("%s%s%d%s"), l_lSt, l_pSt, f_repeat, l_rSt);

        } else {
            l_tmpStr.Format(_T("%s%s%s"), l_lSt, l_pSt, l_rSt);
        }

    } else {
        l_tmpStr.Format(_T("%s"), l_nullSt);
    }

    GetSegEditForStation(f_segment, f_station).SetWindowText(l_tmpStr);
}
//************************************Method Header************************************
// Method Name : AmbEditUiStationSet
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiStationSet(const int f_station)
{

    for (int l_station_index = 0 ; l_station_index < f_station ; l_station_index++) {
        m_amb_staion_label[l_station_index].ShowWindow(SW_SHOW);
    }
}

//************************************Method Header************************************
// Method Name : AmbEditUiSegmentSet
// Author      : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiSegmentSet(int f_segment)
{
    CButton* l_radio_btn = NULL ;

    switch (f_segment) {
        case 2:
            l_radio_btn = &m_sub1;
            l_radio_btn->SetCheck(1);
            UpdateSegmentEditForSeg(2);
            UpdateComboEditForSeg(2);
            break;

        case 3:
            l_radio_btn = (CButton*)GetDlgItem(IDC_AMB_RADIO_SUB2);
            l_radio_btn->SetCheck(1);
            UpdateSegmentEditForSeg(3);
            UpdateComboEditForSeg(3);
            break;

        case 4:
            l_radio_btn = (CButton*)GetDlgItem(IDC_AMB_RADIO_SUB3);
            l_radio_btn->SetCheck(1);
            UpdateSegmentEditForSeg(4);
            UpdateComboEditForSeg(4);
            break;
    }
}
//************************************Method Header************************************
// Method Name : OnSelchangeAmbMasterDirection
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnSelchangeAmbMasterDirection()
{
    // TODO: Add your control notification handler code here

    m_ambdialog_utility.AmbEditSetDirection(1, (m_master_direction_combo.GetCurSel() == 0) ? AMB_COUCH_OUTSIDE : AMB_COUCH_INSIDE);
    AmbEditRefresh();
}
//************************************Method Header************************************
// Method Name : OnSelchangeAmbSub2Direction
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnSelchangeAmbSub2Direction()
{
    // TODO: Add your control notification handler code here

    m_ambdialog_utility.AmbEditSetDirection(2, (m_sub1_direction_combo.GetCurSel() == 0) ? AMB_COUCH_OUTSIDE : AMB_COUCH_INSIDE);
    AmbEditRefresh();
}
//************************************Method Header************************************
// Method Name : OnSelchangeAmbSub3Direction
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnSelchangeAmbSub3Direction()
{
    // TODO: Add your control notification handler code here

    m_ambdialog_utility.AmbEditSetDirection(3, (m_sub2_direction_combo.GetCurSel() == 0) ? AMB_COUCH_OUTSIDE : AMB_COUCH_INSIDE);
    AmbEditRefresh();
}
//************************************Method Header************************************
// Method Name : OnSelchangeAmbSub4Direction
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnSelchangeAmbSub4Direction()
{
    // TODO: Add your control notification handler code here

    m_ambdialog_utility.AmbEditSetDirection(4, (m_sub3_direction_combo.GetCurSel() == 0) ? AMB_COUCH_OUTSIDE : AMB_COUCH_INSIDE);
    AmbEditRefresh();
}
//************************************Method Header************************************
// Method Name : AmbEditUiScanStateSet
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiScanStateSet(const AmbEditScanState_e f_state)
{
    m_amb_scan_state = f_state;
}

//************************************Method Header************************************
// Method Name : AmbEditUiScanButtonSet
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiScanButtonSet()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::AmbEditUiScanButtonSet");
    PQM_TRACE_SCOPED(FUNC_NAME);


    switch (m_amb_scan_state) {
        case AmbEditScanStateSetting:
            m_start_button.EnableWindow(TRUE);
            m_cancel_button.EnableWindow(TRUE);
            m_abort_button.EnableWindow(FALSE);
            break;

        case AmbEditScanStateScanning:
            m_start_button.EnableWindow(FALSE);
            m_cancel_button.EnableWindow(FALSE);
            m_abort_button.EnableWindow(TRUE);
            break;

        case AmbEditScanStatePrep:
            m_start_button.EnableWindow(TRUE);
            m_cancel_button.EnableWindow(FALSE);
            m_abort_button.EnableWindow(TRUE);
            break;

        case AMBEditScanStateAcqman:
            m_start_button.EnableWindow(FALSE);
            m_cancel_button.EnableWindow(TRUE);
            m_abort_button.EnableWindow(FALSE);
            break;

        case AMBEditScanStateAbort:
            m_start_button.EnableWindow(FALSE);
            m_cancel_button.EnableWindow(FALSE);
            m_abort_button.EnableWindow(FALSE);
            break;

        case AMBEditScanStateDelayTime:
            m_start_button.EnableWindow(TRUE);
            m_cancel_button.EnableWindow(FALSE);
            m_abort_button.EnableWindow(FALSE);
            break;

    }
}

//************************************Method Header************************************
// Method Name : GetSegEditForStation
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
EditCtrlEx& CPQMAMBDlg::GetSegEditForStation(
    const int f_segment,
    const int f_station
)
{

    return m_amb_segment_editbox[f_segment - 1][f_station - 1];
}
//************************************Method Header************************************
// Method Name : OnCheckShim
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnCheckShim()
{

    m_ambdialog_utility.AmbEditSetShimming(m_shim.GetCheck() ? ON : OFF);
}
//************************************Method Header************************************
// Method Name : OnCheckMap
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnCheckMap()
{

    m_ambdialog_utility.AmbEditSetMap(m_map.GetCheck() ? ON : OFF);
}
//************************************Method Header************************************
// Method Name : OnAMBPrep
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::OnAMBPrep()
{
    int l_prepStatus;
    int l_prepEnableStatus;
    m_ambdialog_utility.ambEditRefreshPrep(&l_prepStatus, &l_prepEnableStatus);

    m_ambdialog_utility.AmbEditSetFirstCellPrep((l_prepStatus == TRUE) ? OFF : ON) ;

    AmbEditRefresh();
}
//************************************Method Header************************************
// Method Name : RecAMBPqmScanMsg
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::RecAMBPqmScanMsg()
{

    AmbEditCommPqmScan();
    AmbEditRefresh();
}

//************************************Method Header************************************
// Method Name : AmbEditCommPqmScan
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditCommPqmScan()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::AmbEditCommPqmScan");
    PQM_TRACE_SCOPED(FUNC_NAME);

    AmbEditUiScanStateSet(AmbEditScanStateScanning);
}


//************************************Method Header************************************
// Method Name : AmbEditCommScanWait
//Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditCommScanWait()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::AmbEditCommScanWait");
    PQM_TRACE_SCOPED(FUNC_NAME);

    AmbEditUiScanStateSet(AmbEditScanStatePrep);
}
//************************************Method Header************************************
// Method Name : RecAmbScanWait
//Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::RecAmbScanWait()
{

    AmbEditCommScanWait();
    AmbEditRefresh();
}



//************************************Method Header************************************
// Method Name : RecAmbScanStartError
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::RecAmbScanStartError()
{

    AmbEditCommScanStartError();
    AmbEditRefresh();
}
//************************************Method Header************************************
// Method Name : AmbEditCommScanStartError
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditCommScanStartError()
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::AmbEditCommScanStartError");
    PQM_TRACE_SCOPED(FUNC_NAME);

    AmbEditUiScanStateSet(AmbEditScanStateSetting);
}


//************************************Method Header************************************
// Method Name : UpdateAMBAbortButtonState
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::UpdateAMBAbortButtonState(const BOOL f_amb_abort_btn_state)
{
    m_abort_button.EnableWindow((AmbEditScanStateSetting == m_amb_scan_state) ? false : f_amb_abort_btn_state);
}

//************************************Method Header**********************************
// Method Name  : AmbScanStateSetting
// Author       : iGATE
// Purpose      : Enable/Disable the AMB buttons
//***********************************************************************************
void CPQMAMBDlg::AmbScanStateSetting()
{

    AmbEditUiScanStateSet(AmbEditScanStateSetting);
    AmbEditUiScanButtonSet();
}

//************************************Method Header************************************
// Method Name : UpdateAMBScanButtonState
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::UpdateAMBScanButtonState(const BOOL f_amb_scan_btn_state)
{
    BOOL l_is_enable = FALSE;

    if (AmbEditScanStateSetting == m_amb_scan_state)
        l_is_enable = TRUE;

    else if (AMBEditScanStateAbort == m_amb_scan_state)
        l_is_enable = FALSE;

    else
        l_is_enable = f_amb_scan_btn_state;

    m_start_button.EnableWindow(l_is_enable);
}

//************************************Method Header************************************
// Method Name : GetAMBStatus
// Author	   : iGATE
// Purpose     :
//***********************************************************************************
amb_t& CPQMAMBDlg::GetAMBStatus()
{
    return m_amb_status;
}

bool CPQMAMBDlg::IsAMBScanStarted() const
{
    return !((AmbEditScanStateSetting == m_amb_scan_state) || (AMBEditScanStateAbort == m_amb_scan_state) || (AmbEditScanStateScanning == m_amb_scan_state));
}


//************************************Method Header************************************
// Method Name : EnabledisableAMBScanRelatedButtons
// Author      : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::EnabledisableAMBScanRelatedButtons(const bool f_is_enable)
{
    m_amb_scan_state =  f_is_enable ? AmbEditScanStateSetting : AMBEditScanStateAcqman;
    AmbEditUiScanButtonSet();
}

//************************************Method Header************************************
// Method Name : AmbEditUiDirectionSet
// Author      : iGATE
// Purpose     :
//***********************************************************************************
void CPQMAMBDlg::AmbEditUiDirectionSet(int  f_segment,
                                       int  f_direction)
{

    switch (f_segment) {
        case 1:

            if (f_direction == AMB_COUCH_OUTSIDE)
                m_master_direction_combo.SetCurSel(0);

            else
                m_master_direction_combo.SetCurSel(1);

            break;

        case 2:

            if (f_direction == AMB_COUCH_OUTSIDE)
                m_sub1_direction_combo.SetCurSel(0);

            else
                m_sub1_direction_combo.SetCurSel(1);

            break;

        case 3:

            if (f_direction == AMB_COUCH_OUTSIDE)
                m_sub2_direction_combo.SetCurSel(0);

            else
                m_sub2_direction_combo.SetCurSel(1);

            break;

        case 4:

            if (f_direction == AMB_COUCH_OUTSIDE)
                m_sub3_direction_combo.SetCurSel(0);

            else
                m_sub3_direction_combo.SetCurSel(1);

            break;
    }
}

//************************************Method Header**********************************
// Method Name  : UpdateStatesForCtrls
// Author       : iGATE
// Purpose      : Update AMB dioalog control state
//***********************************************************************************
void CPQMAMBDlg::UpdateStatesForCtrls(const pqm_msg_tag f_message_to_process)
{
    LPCTSTR FUNC_NAME = _T("CPQMAMBDlg::UpdateStatesForCtrls");
    PQM_TRACE_SCOPED(FUNC_NAME);

    switch (f_message_to_process) {
        case PQM_MSG_ABORT_WAIT :
        case PQM_MSG_ABORT_FAIL :
            AmbEditUiScanStateSet(AMBEditScanStateAbort);
            AmbEditUiScanButtonSet();
            break ;
    }


    return ;
}

//************************************Method Header**********************************
// Method Name  : UpdateDelayTimeState
// Author       : iGATE
// Purpose      : Set AMB state as Delay Time to enable scan button.
//***********************************************************************************
void CPQMAMBDlg::UpdateDelayTimeState()
{
    AmbEditUiScanStateSet(AMBEditScanStateDelayTime);

}
