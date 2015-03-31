// PQMWirelessGating.cpp : implementation file
//
#include "stdafx.h"
#include "PQMWirelessGating.h"
#include <tami/vfStudy/vf_appcodes.h>
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include "DPSManager.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;



BEGIN_MESSAGE_MAP(CPQMWirelessGating, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMWirelessGating)
    ON_BN_CLICKED(ID_WIRELESS_OK, OnBtnOK)
    ON_BN_CLICKED(IDCANCEL, OnBtnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPQMWirelessGating dialog

//**************************************************************************
//Method Name   :   CPQMWirelessGating
//Author        :
//Purpose       :   constructor
//**************************************************************************
CPQMWirelessGating::CPQMWirelessGating(
    Wireless_gating_t*  f_wireless_gating,
    CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_WIRELESS_GATING_DIALOG, pParent),
    m_wireless_gating_val(f_wireless_gating)
{


}

//**************************************************************************
//Method Name   :   ~CPQMWirelessGating
//Author        :
//Purpose       :   Destructor
//**************************************************************************

CPQMWirelessGating::~CPQMWirelessGating(
)
{
}



void CPQMWirelessGating::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMWirelessGating)
    DDX_Control(pDX, IDC_HIGH_GAIN, m_hGain);
    DDX_Control(pDX, IDC_TWAVE_SUPPRESSION, m_twsupp);
    DDX_Control(pDX, IDC_CARDIAC, m_filter_cardiac);
    DDX_Control(pDX, ID_WIRELESS_OK, m_ok);
    DDX_Control(pDX, IDCANCEL, m_btncancel);
    DDX_Control(pDX, IDC_COMMAND_STATIC, m_command);
    DDX_Control(pDX, IDC_MODE_STATIC, m_mode);
    DDX_Control(pDX, IDC_FILTER_STATIC, m_filter);
    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPQMPlanDupDlg message handlers


//**************************************************************************
//Method Name   : UpdateOKBtnStatus
//Author        :
//Purpose       :
//**************************************************************************

void CPQMWirelessGating::UpdateOKBtnStatus(
    const BOOL f_enable
)
{
    if (IsWindow(GetSafeHwnd())) {
        ((CButton*)GetDlgItem(ID_WIRELESS_OK))->EnableWindow(f_enable);
    }
}

//**************************************************************************
//Method Name   : OnInitDialogImpl
//Author        :
//Purpose       : Dialog initialization
//**************************************************************************
BOOL CPQMWirelessGating::OnInitDialogImpl()
{

    LPCTSTR FUNC_NAME = _T("CPQMWirelessGating::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    SetMultilingual();

    InitUI();

    return CZOrderDialog::OnInitDialogImpl();
}

//**************************************************************************
//Method Name   : OnBtnOK
//Author        :
//Purpose       :
//**************************************************************************
void CPQMWirelessGating::OnBtnOK()
{

    LPCTSTR FUNC_NAME = _T("CPQMWirelessGating::OnBtnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    GetSelecValues();

    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDOK);
    }

    return;
}
//**************************************************************************
//Method Name   : OnBtnCancel
//Author        :
//Purpose       : To handle close button click event
//**************************************************************************
void CPQMWirelessGating::OnBtnCancel()
{

    LPCTSTR FUNC_NAME = _T("CPQMWirelessGating::OnBtnCancel");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of OnBtnClose"));

    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    return;
}


//**************************************************************************
//Method Name   : InitUI
//Author        :
//Purpose       : To Initialize the UI
//**************************************************************************
bool CPQMWirelessGating::InitUI()
{

    SetTitleBarHeight(24) ;
    SetFont(_T("Arial BOLD"), 12) ;

    SetDialogRibbonHeight();
    SetButtonAppearance();
    SetButtonFont();

    m_hGain.SetCheck(m_wireless_gating_val->m_command);
    m_twsupp.SetCheck(m_wireless_gating_val->m_mode);

    CButton* l_radio_btn = NULL ;

    switch (m_wireless_gating_val->m_wireless_filter) {

        case VFC_WIRELESS_GATE_TRIG_FILTER_CARDIAC :
            l_radio_btn = &m_filter_cardiac;
            break;

        case VFC_WIRELESS_GATE_TRIG_FILTER_MRI_B :
            l_radio_btn = (CButton*)GetDlgItem(IDC_MRIB);
            break;

        case VFC_WIRELESS_GATE_TRIG_FILTER_MRI_A :
        case VFC_WIRELESS_GATE_TRIG_FILTER_MONITOR:
            l_radio_btn = (CButton*)GetDlgItem(IDC_MRIA);
            break;

        default:
            l_radio_btn = &m_filter_cardiac;
    }

    if (l_radio_btn) {
        l_radio_btn->SetCheck(TRUE);
    }

    return true;
}



//**************************************************************************
//Method Name   : SetMultilingual
//Author        :
//Purpose       :
//**************************************************************************
void CPQMWirelessGating::SetMultilingual()
{


    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDD_WIRELESS_GATING_DIALOG"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

}
//**************************************************************************
//Method Name   : SetButtonAppearance
//Author        :
//Purpose       : To set button Appearance
//**************************************************************************
void CPQMWirelessGating::SetButtonAppearance()
{
    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    m_hGain.SetBkColor(bk_color);
    m_twsupp.SetBkColor(bk_color);

    m_filter_cardiac.SetBkColor(bk_color);

    m_command.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_mode.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_filter.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);

    m_command.SetFontAttributes(_T("Arial BOLD"), 100);
    m_command.UpdateFont();

    m_mode.SetFontAttributes(_T("Arial BOLD"), 100);
    m_mode.UpdateFont();

    m_filter.SetFontAttributes(_T("Arial BOLD"), 100);
    m_filter.UpdateFont();

    m_ok.SetCurveType(CURVE_LARGE_5);
    m_btncancel.SetCurveType(CURVE_LARGE_5);


}

//**************************************************************************
//Method Name   : SetButtonFont
//Author        :
//Purpose       : To set the button font
//**************************************************************************
BOOL CPQMWirelessGating::SetButtonFont()
{
    LPCTSTR button_font_name = _T("Verdana BOLD") ;

    const int size = 9;
    m_hGain.SetFont(button_font_name, size);
    m_twsupp.SetFont(button_font_name, size);
    m_filter_cardiac.SetFont(button_font_name, size);

    m_command.SetFontAttributes(button_font_name, size);
    m_command.SetFontAttributes(button_font_name, size);
    m_command.SetFontAttributes(button_font_name, size);

    const int button_size = 12;
    m_ok.SetPrimaryTextFont(button_font_name, button_size);
    m_ok.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_btncancel.SetPrimaryTextFont(button_font_name, button_size);
    m_btncancel.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    return TRUE;
}


//**************************************************************************
//Method Name   : SetDialogRibbonHeight
//Author        :
//Purpose       : To set the dialog ribbon height
//**************************************************************************
void CPQMWirelessGating::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_ok.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 5);

}
//**************************************************************************
//Method Name   : GetSelecValues
//Author        :
//Purpose       :
//**************************************************************************

void CPQMWirelessGating::GetSelecValues(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMWirelessGating::GetSelecValues");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_wireless_gating_val != NULL) {

        m_wireless_gating_val->m_command = m_hGain.GetCheck() ? VFC_WIRELESS_GATE_TRIG_FILTER_CMD_HIGH_GAIN : VFC_WIRELESS_GATE_TRIG_FILTER_CMD_NONE ;
        m_wireless_gating_val->m_mode = m_twsupp.GetCheck() ? VFC_WIRELESS_GATE_TRIG_FILTER_MODE_T_WAVE_SUPPRESSION : VFC_WIRELESS_GATE_TRIG_FILTER_MODE_NONE ;

        if (TRUE == m_filter_cardiac.GetCheck()) {
            m_wireless_gating_val->m_wireless_filter = VFC_WIRELESS_GATE_TRIG_FILTER_CARDIAC;

        } else if (TRUE == ((CButton*)GetDlgItem(IDC_MRIB))->GetCheck()) {
            m_wireless_gating_val->m_wireless_filter = VFC_WIRELESS_GATE_TRIG_FILTER_MRI_B;

        } else if (TRUE == ((CButton*)GetDlgItem(IDC_MRIA))->GetCheck()) {
            m_wireless_gating_val->m_wireless_filter = VFC_WIRELESS_GATE_TRIG_FILTER_MRI_A;

        } else {
            m_wireless_gating_val->m_wireless_filter = VFC_WIRELESS_GATE_TRIG_FILTER_MRI_A;
        }
    }
}