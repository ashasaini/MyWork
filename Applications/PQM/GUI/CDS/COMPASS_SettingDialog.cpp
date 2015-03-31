//TMSC-REDMINE-781
//TMSC-REDMINE-777

// COMPASS_SettingDialog.cpp : implementation file
//

#include "../stdafx.h"
#include "COMPASS_SettingDialog.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "../DPSManager.h"
#include "../UIButtonUtility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

const int RIBBON_GAP = 5 ;

BEGIN_MESSAGE_MAP(CCOMPASS_SettingDialog, /*CDialog*/CZOrderDialog)
    //{{AFX_MSG_MAP(CCOMPASS_SettingDialog)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_PARAM1_UP, OnParam1Up)
    ON_BN_CLICKED(IDC_PARAM2_DOWN, OnParam2Down)
    ON_BN_CLICKED(IDC_PARAM2_UP, OnParam2Up)
    ON_BN_CLICKED(IDC_PARAM1_DOWN, OnParam1Down)
    ON_MESSAGE(WM_EDIT_KILL_FOCUS, OnKillFocusEditCtrl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCOMPASS_SettingDialog dialog

//************************************Method Header************************************
// Method Name  : CCOMPASS_SettingDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CCOMPASS_SettingDialog::CCOMPASS_SettingDialog(CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_COMPASS_SETTING_DIALOG, pParent)
{
    //{{AFX_DATA_INIT(CCOMPASS_SettingDialog)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}




//************************************Method Header************************************
// Method Name  : GetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
ASCompassSettingParam* CCOMPASS_SettingDialog::GetASCOMPASSParameter(
    const AS_COMPASS_PARAM_ENUM f_parameter_type
)
{

    switch (f_parameter_type) {

        case AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH:
            return &m_dist_from_magnetic_center;
            break;

        case AS_COMPASS_PARAM_COIL_COVERAGE:
            return &m_min_coverage_for_section;
            break;

        default :
            ASSERT(FALSE) ;
    }

    return NULL ;
}

//************************************Method Header************************************
// Method Name  : SetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::SetASCOMPASSParameter(
    const AS_COMPASS_PARAM_ENUM f_parameter_type,
    const ASCompassSettingParam& f_parameter_values
)
{
    switch (f_parameter_type) {

        case AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH:
            m_dist_from_magnetic_center = f_parameter_values ;
            break;

        case AS_COMPASS_PARAM_COIL_COVERAGE:
            m_min_coverage_for_section = f_parameter_values ;
            break;

        default :
            ASSERT(FALSE) ;
    }
}



void CCOMPASS_SettingDialog::DoDataExchange(CDataExchange* pDX)
{
    /*CDialog*/CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCOMPASS_SettingDialog)
    DDX_Control(pDX, IDOK, m_ok_button);
    DDX_Control(pDX, IDCANCEL, m_cancel_button);
    DDX_Control(pDX, IDC_EDIT_PARAM1, m_edit_param1);
    DDX_Control(pDX, IDC_EDIT_PARAM2, m_edit_param2);
    DDX_Control(pDX, IDC_SLIDER_PARAM1, m_slider_param1);
    DDX_Control(pDX, IDC_SLIDER_PARAM2, m_slider_param2);
    DDX_Control(pDX, IDC_FRAM_1, m_fram_1);
    DDX_Control(pDX, IDC_FRAM_2, m_fram_2);

    DDX_Control(pDX, IDC_PARAM1_UP, m_param1_up);
    DDX_Control(pDX, IDC_PARAM1_DOWN, m_param1_down);
    DDX_Control(pDX, IDC_PARAM2_UP, m_param2_up);
    DDX_Control(pDX, IDC_PARAM2_DOWN, m_param2_down);

    DDX_Control(pDX, IDC_MIN_PARAM1, m_min_param1);
    DDX_Control(pDX, IDC_MAX_PARAM1, m_max_param1);
    DDX_Control(pDX, IDC_MIN_PARAM2, m_min_param2);
    DDX_Control(pDX, IDC_MAX_PARAM2, m_max_param2);

    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCOMPASS_SettingDialog message handlers


//************************************Method Header************************************
// Method Name  : OnInitDialogImpl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CCOMPASS_SettingDialog::OnInitDialogImpl()
{


    SetDialogRibbonHeight();
    SetButtonStyle();

    SetTitleBarHeight(24) ;
    SetFont(_T("Arial BOLD"), 12) ;

    InitializeSliderCtrls();
    InitializeCtrlsWithData();
    UpdateMinMaxStaticText();

    InitCtrlForInternationalization();

    return CZOrderDialog::OnInitDialogImpl(); // return TRUE  unless you set the focus to a control
}


//************************************Method Header************************************
// Method Name  : OnOK
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnOK()
{
    // TODO: Add extra validation here
    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDOK);
    }

    /*CDialog*///DialogEx::OnOK();
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}

//************************************Method Header************************************
// Method Name  : OnCancel
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnCancel()
{
    // TODO: Add extra cleanup here
    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    /*CDialog*///DialogEx::OnCancel();
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}



//************************************Method Header************************************
// Method Name  : OnHScroll
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    if (pScrollBar) {

        if (pScrollBar->m_hWnd == m_slider_param1.m_hWnd) {

            OnSliderChange(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH, nPos);

        } else if (pScrollBar->m_hWnd == m_slider_param2.m_hWnd) {

            OnSliderChange(AS_COMPASS_PARAM_COIL_COVERAGE, nPos);
        }
    }

    CZOrderDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


//************************************Method Header************************************
// Method Name  : OnParam1Up
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnParam1Up()
{
    // TODO: Add your control notification handler code here
    IncreamentDecrementParameter(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH , true);
}

//************************************Method Header************************************
// Method Name  : OnParam2Down
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnParam2Down()
{
    // TODO: Add your control notification handler code here
    IncreamentDecrementParameter(AS_COMPASS_PARAM_COIL_COVERAGE , false);
}

//************************************Method Header************************************
// Method Name  : OnParam2Up
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnParam2Up()
{
    // TODO: Add your control notification handler code here
    IncreamentDecrementParameter(AS_COMPASS_PARAM_COIL_COVERAGE , true);
}

//************************************Method Header************************************
// Method Name  : OnParam1Down
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnParam1Down()
{
    // TODO: Add your control notification handler code here
    IncreamentDecrementParameter(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH , false);
}

//************************************Method Header************************************
// Method Name  : OnKillFocusEditCtrl
// Author       : PATNI/ HEMANT
// Purpose      : This function will udpate the contrls with latest saved param valeus
//***********************************************************************************
LRESULT CCOMPASS_SettingDialog::OnKillFocusEditCtrl(
    WPARAM wParam,
    LPARAM lParam
)
{
    AS_COMPASS_PARAM_ENUM  l_parameter_type =  AS_COMPASS_PARAM_INVALID;

    switch ((int)wParam) {

        case IDC_EDIT_PARAM1: {
            l_parameter_type = AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH;
        }
        break ;

        case IDC_EDIT_PARAM2: {
            l_parameter_type = AS_COMPASS_PARAM_COIL_COVERAGE;
        }
        break ;

        default :
            return 0 ;
    }

    //don't update the edit and slider with text in edit.
    //but take the last saved values.

    UpdateEditCtrl(l_parameter_type);
    UpdateSliderCtrl(l_parameter_type);

    return 0 ;
}

//************************************Method Header************************************
// Method Name  : PreTranslateMessage
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CCOMPASS_SettingDialog::PreTranslateMessageImpl(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    ProcessEditCtrlForEnterKey(pMsg);

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}


//************************************Method Header************************************
// Method Name  : InitCtrlForInternationalization
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::InitCtrlForInternationalization(
)
{
    SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_COMPASS_SETTING_DIALOG")));

    m_fram_1.SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_DISTANCE_FROM_MAGNETIC_CENTER_FRAME")));
    m_fram_2.SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_MIN_COVERAGE_FOR_SECTION_FRAME")));
}


//************************************Method Header************************************
// Method Name  : UpdateEditCtrl
// Author       : PATNI/ HEMANT
// Purpose      : edit box is updated from data structure..
//***********************************************************************************
void CCOMPASS_SettingDialog::UpdateEditCtrl(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{
    EditCtrlEx* l_editctrl = GetEditCtrl(f_parameter_type);
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_editctrl && l_compass_param) {

        CString l_pos_str = GetStringValueFor(f_parameter_type);

        l_editctrl->SetWindowText(l_pos_str);
    }
}


//************************************Method Header************************************
// Method Name  : OnSliderChange
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnSliderChange(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type,
    const UINT nPos
)
{
    SliderCtrlEx* l_sldier = GetSlider(f_parameter_type);
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_sldier && l_compass_param) {

        l_compass_param->m_value = GetSliderPos(f_parameter_type);

        UpdateEditCtrl(f_parameter_type);
    }
}


//************************************Method Header************************************
// Method Name  : UpdateSliderCtrl
// Author       : PATNI/ HEMANT
// Purpose      : slider is updated from data structure..
//***********************************************************************************
void CCOMPASS_SettingDialog::UpdateSliderCtrl(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{
    SliderCtrlEx* l_sldier = GetSlider(f_parameter_type);
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_sldier && l_compass_param) {

        l_sldier->SetPos(l_compass_param->m_value);
    }
}


//************************************Method Header************************************
// Method Name  : IncreamentDecrementParameter
// Author       : PATNI/ HEMANT
// Purpose      : Function will increment or decrement the dialog parameter indetified by
//                f_parameter_type. f_increment identifies increment / decrement
//***********************************************************************************
void CCOMPASS_SettingDialog::IncreamentDecrementParameter(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type,
    const bool f_increment /*= true*/
)
{
    EditCtrlEx* l_editctrl = GetEditCtrl(f_parameter_type);
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_editctrl && l_compass_param) {

        const int l_edit_value = GetEditCtrlTextValue(f_parameter_type) + (f_increment ? (1) : (-1));
        l_compass_param->m_value = GetValidatedValueFor(f_parameter_type, l_edit_value) ;

        UpdateSliderCtrl(f_parameter_type);

        //also update the edit ctrl text..
        UpdateEditCtrl(f_parameter_type);
    }
}


//************************************Method Header************************************
// Method Name  : InitializeCtrlsWithData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::InitializeCtrlsWithData(
)
{
    UpdateEditCtrl(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH);
    UpdateEditCtrl(AS_COMPASS_PARAM_COIL_COVERAGE);

    UpdateSliderCtrl(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH);
    UpdateSliderCtrl(AS_COMPASS_PARAM_COIL_COVERAGE);
}


//************************************Method Header************************************
// Method Name  : UpdateMinMaxStaticText
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::UpdateMinMaxStaticText(
)
{
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH);

    CString l_static_text = _T("") ;

    if (l_compass_param) {

        l_static_text.Format(_T("%d"), l_compass_param->m_min_value);
        m_min_param1.SetWindowText(l_static_text);

        l_static_text.Format(_T("%d"), l_compass_param->m_max_value);
        m_max_param1.SetWindowText(l_static_text);
    }

    l_compass_param = GetASCOMPASSParameter(AS_COMPASS_PARAM_COIL_COVERAGE);

    if (l_compass_param) {

        l_static_text.Format(_T("%d"), l_compass_param->m_min_value);
        m_min_param2.SetWindowText(l_static_text);

        l_static_text.Format(_T("%d"), l_compass_param->m_max_value);
        m_max_param2.SetWindowText(l_static_text);
    }
}


//************************************Method Header************************************
// Method Name  : InitializeSliderCtrls
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::InitializeSliderCtrls(
)
{
    InitializeSliderCtrl(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH) ;
    InitializeSliderCtrl(AS_COMPASS_PARAM_COIL_COVERAGE) ;
}


//************************************Method Header************************************
// Method Name  : InitializeSliderCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::InitializeSliderCtrl(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{

    SliderCtrlEx* l_sldier = GetSlider(f_parameter_type);
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_sldier && l_compass_param) {

        l_sldier->SetRange(l_compass_param->m_min_value, l_compass_param->m_max_value);
        l_sldier->SetPos(l_compass_param->m_value);
    }
}


//************************************Method Header************************************
// Method Name  : ProcessEditCtrlForEnterKey
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CCOMPASS_SettingDialog::ProcessEditCtrlForEnterKey(
    MSG* pMsg
)
{

    if (pMsg) {

        if (pMsg->message == WM_KEYDOWN) {

            if ((int)pMsg->wParam == VK_RETURN) {

                if (m_edit_param1.GetSafeHwnd() == pMsg->hwnd) {

                    OnEditCtrlUpdated(AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH);

                } else if (m_edit_param2.GetSafeHwnd() == pMsg->hwnd) {

                    OnEditCtrlUpdated(AS_COMPASS_PARAM_COIL_COVERAGE);
                }
            }
        }
    }

    return true ;
}


//************************************Method Header************************************
// Method Name  : OnEditCtrlUpdated
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::OnEditCtrlUpdated(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{
    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);
    l_compass_param->m_value = GetValidatedValueFor(f_parameter_type, GetEditCtrlTextValue(f_parameter_type)) ;

    UpdateSliderCtrl(f_parameter_type);

    //also update the edit ctrl text..
    UpdateEditCtrl(f_parameter_type);
}

//************************************Method Header************************************
// Method Name  : SetButtonStyle
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::SetButtonStyle(
)
{
    m_ok_button.SetCurveType(CURVE_LARGE_5);
    m_cancel_button.SetCurveType(CURVE_LARGE_5);

    const CString l_button_font_name = _T("Verdana BOLD") ;
    const int l_primary_size = 12;
    m_ok_button.SetPrimaryTextFont(l_button_font_name, l_primary_size);
    m_cancel_button.SetPrimaryTextFont(l_button_font_name, l_primary_size);

    const int l_edit_font = 9;
    m_edit_param1.SetFont(l_button_font_name, l_edit_font);
    m_edit_param2.SetFont(l_button_font_name, l_edit_font);

    m_fram_1.SetFont(l_button_font_name, l_edit_font);
    m_fram_2.SetFont(l_button_font_name, l_edit_font);
    m_fram_1.SetCurveType(CURVE_SMALL);
    m_fram_2.SetCurveType(CURVE_SMALL);

    m_param1_up.SetButtonType(TUSBUTTON_ROUND_NONE);
    SetBitmapForButton(m_param1_up, IDB_ARROW_UP);

    m_param1_down.SetButtonType(TUSBUTTON_ROUND_NONE);
    SetBitmapForButton(m_param1_down, IDB_ARROW_DOWN);

    m_param2_up.SetButtonType(TUSBUTTON_ROUND_NONE);
    SetBitmapForButton(m_param2_up, IDB_ARROW_UP);

    m_param2_down.SetButtonType(TUSBUTTON_ROUND_NONE);
    SetBitmapForButton(m_param2_down, IDB_ARROW_DOWN);


    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    SLIDER_COLORS_t slider_colors ;
    m_slider_param1.GetColors(slider_colors);
    slider_colors.clrBk = bk_color;

    m_slider_param1.SetColors(slider_colors);
    m_slider_param2.SetColors(slider_colors);
    m_slider_param1.SetChannelThickness(8);
    m_slider_param2.SetChannelThickness(8);

    m_slider_param1.SetPageSize(10);
    m_slider_param2.SetPageSize(10);

    const int static_controls = 4;
    CCustomStatic* color_statics[static_controls] = { &m_min_param1, &m_min_param2, &m_max_param1, &m_max_param2} ;

    for (int static_ctrl_index = 0; static_ctrl_index < static_controls ; static_ctrl_index ++) {

        //color_statics[static_button]->LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
        //        color_statics[static_ctrl_index]->SetColorData(bk_color, g_pqm_static_textcolor);
        //        color_statics[static_ctrl_index]->SetFontAttributes(pqm_static_font_name, pqm_static_font_size);
        //        color_statics[static_ctrl_index]->UpdateFont();

        color_statics[static_ctrl_index]->SetColorData(bk_color, g_pqmdialog_static_ctrl_text_color);
        color_statics[static_ctrl_index]->SetFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
        color_statics[static_ctrl_index]->UpdateFont();
    }
}

//************************************Method Header************************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::SetBitmapForButton(
    ButtonEx& f_button_object,
    const UINT f_bitmap_resource_id,
    const bool f_center_bitmap /*= true*/
)const
{
    CUIButtonUtility::GetInstance()->SetBitmapForButton(f_button_object, f_bitmap_resource_id, RGB(0, 255, 0), f_center_bitmap);
}

//************************************Method Header************************************
// Method Name  : GetValidatedValueFor
// Author       : PATNI/ HEMANT
// Purpose      : This function will validate the given f_value according to given parameter type
//***********************************************************************************
int CCOMPASS_SettingDialog::GetValidatedValueFor(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type,
    const int f_value
)
{

    int l_value_to_return = f_value ;

    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_compass_param) {

        if (l_value_to_return > l_compass_param->m_max_value) {
            l_value_to_return = l_compass_param->m_max_value ;

        } else if (l_value_to_return < l_compass_param->m_min_value) {
            l_value_to_return = l_compass_param->m_min_value ;
        }
    }

    return l_value_to_return ;
}


//************************************Method Header************************************
// Method Name  : GetEditCtrlTextValue
// Author       : PATNI/ HEMANT
// Purpose      : This function will return the edit conotrol text value for given
//                type of parameter
//***********************************************************************************
int CCOMPASS_SettingDialog::GetEditCtrlTextValue(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{
    EditCtrlEx* l_editctrl = GetEditCtrl(f_parameter_type);

    if (l_editctrl) {

        CString l_edit_text ;
        l_editctrl->GetWindowText(l_edit_text);

        return (_wtoi((LPCTSTR)l_edit_text));

    } else {
        ASSERT(FALSE);
    }

    return 0;
}


//************************************Method Header************************************
// Method Name  : GetStringValueFor
// Author       : PATNI/ HEMANT
// Purpose      : This functiuon will return the string value for the parameter
//                identified by given argument
//***********************************************************************************
CString CCOMPASS_SettingDialog::GetStringValueFor(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{

    ASCompassSettingParam* l_compass_param = GetASCOMPASSParameter(f_parameter_type);

    if (l_compass_param) {

        CString l_pos_str ;
        l_pos_str.Format(_T("%d"), l_compass_param->m_value);

        return l_pos_str ;
    }

    return _T("");
}


//************************************Method Header************************************
// Method Name  : GetSlider
// Author       : PATNI/ HEMANT
// Purpose      : Returns the slider control identified by given parameter
//***********************************************************************************
SliderCtrlEx* CCOMPASS_SettingDialog::GetSlider(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{

    switch (f_parameter_type) {

        case AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH:
            return &m_slider_param1;
            break;

        case AS_COMPASS_PARAM_COIL_COVERAGE:
            return &m_slider_param2;
            break;

        default :
            ASSERT(FALSE) ;
    }

    return NULL ;
}


//************************************Method Header************************************
// Method Name  : GetEditCtrl
// Author       : PATNI/ HEMANT
// Purpose      : Returns the edit control identified by given parameter
//***********************************************************************************
EditCtrlEx* CCOMPASS_SettingDialog::GetEditCtrl(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{

    switch (f_parameter_type) {

        case AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH:
            return &m_edit_param1;
            break;

        case AS_COMPASS_PARAM_COIL_COVERAGE:
            return &m_edit_param2;
            break;

        default :
            ASSERT(FALSE) ;
    }

    return NULL ;
}


//************************************Method Header************************************
// Method Name  : GetSliderPos
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CCOMPASS_SettingDialog::GetSliderPos(
    const AS_COMPASS_PARAM_ENUM  f_parameter_type
)
{
    SliderCtrlEx* l_sldier = GetSlider(f_parameter_type);

    if (l_sldier) {

        return l_sldier->GetPos();
    }

    return -1 ;
}

//************************************Method Header************************************
// Method Name  : SetDialogRibbonHeight
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_SettingDialog::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_ok_button.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}



