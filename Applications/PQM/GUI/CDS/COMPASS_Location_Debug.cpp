//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-777

// COMPASS_Location_Debug.cpp : implementation file
//

#include "../stdafx.h"
#include "COMPASS_Location_Debug.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "../DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

const int RIBBON_GAP = 5 ;


BEGIN_MESSAGE_MAP(CCOMPASS_Location_Debug, /*CDialog*/CZOrderDialog)
    //{{AFX_MSG_MAP(CCOMPASS_Location_Debug)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCOMPASS_Location_Debug dialog


CCOMPASS_Location_Debug::CCOMPASS_Location_Debug(CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_COMPASS_LOCATION_DEBUG, pParent)
{
    //{{AFX_DATA_INIT(CCOMPASS_Location_Debug)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


//************************************Method Header************************************
// Method Name  : GetCompassDebugParam
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::GetCompassDebugParam(
    COMPASS_Debugg_Param  f_debug_param[MAX_PARAM]
)
{
    for (int i = 0 ; i < MAX_PARAM ; i++) {

        f_debug_param[i] = m_debug_param[i] ;
    }
}


//************************************Method Header************************************
// Method Name  : SetCompassDebugParam
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::SetCompassDebugParam(
    COMPASS_Debugg_Param  f_debug_param[MAX_PARAM],
    const bool f_update /*= false*/
)
{
    for (int l_index = 0 ; l_index < MAX_PARAM; l_index ++) {

        m_debug_param[l_index] = f_debug_param[l_index] ;
    }

    if (f_update) {
        UpdateDialogParams();
    }
}


void CCOMPASS_Location_Debug::DoDataExchange(CDataExchange* pDX)
{
    /*CDialog*/CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCOMPASS_Location_Debug)
    DDX_Control(pDX, IDOK, m_set_button);
    DDX_Control(pDX, IDCANCEL, m_cancel_button);

    DDX_Control(pDX, IDC_COIL_LOCATION_1, m_coil_location[0]);
    DDX_Control(pDX, IDC_COIL_LOCATION_2, m_coil_location[1]);
    DDX_Control(pDX, IDC_COIL_LOCATION_3, m_coil_location[2]);
    DDX_Control(pDX, IDC_COIL_LOCATION_4, m_coil_location[3]);
    DDX_Control(pDX, IDC_COIL_LOCATION_5, m_coil_location[4]);
    DDX_Control(pDX, IDC_COIL_LOCATION_6, m_coil_location[5]);
    DDX_Control(pDX, IDC_COIL_LOCATION_7, m_coil_location[6]);
    DDX_Control(pDX, IDC_COIL_LOCATION_8, m_coil_location[7]);
    DDX_Control(pDX, IDC_COIL_LOCATION_9, m_coil_location[8]);
    DDX_Control(pDX, IDC_COIL_LOCATION_10, m_coil_location[9]);
    DDX_Control(pDX, IDC_COIL_LOCATION_11, m_coil_location[10]);
    DDX_Control(pDX, IDC_COIL_LOCATION_12, m_coil_location[11]);
    DDX_Control(pDX, IDC_COIL_LOCATION_13, m_coil_location[12]);

    DDX_Control(pDX, IDC_COIL_DETECTFLAG_1, m_detect_flag[0]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_2, m_detect_flag[1]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_3, m_detect_flag[2]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_4, m_detect_flag[3]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_5, m_detect_flag[4]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_6, m_detect_flag[5]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_7, m_detect_flag[6]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_8, m_detect_flag[7]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_9, m_detect_flag[8]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_10, m_detect_flag[9]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_11, m_detect_flag[10]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_12, m_detect_flag[11]);
    DDX_Control(pDX, IDC_COIL_DETECTFLAG_13, m_detect_flag[12]);


    DDX_Control(pDX, IDC_COILPORT_1, m_coil_id[0]);
    DDX_Control(pDX, IDC_COILPORT_2, m_coil_id[1]);
    DDX_Control(pDX, IDC_COILPORT_3, m_coil_id[2]);
    DDX_Control(pDX, IDC_COILPORT_4, m_coil_id[3]);
    DDX_Control(pDX, IDC_COILPORT_5, m_coil_id[4]);
    DDX_Control(pDX, IDC_COILPORT_6, m_coil_id[5]);
    DDX_Control(pDX, IDC_COILPORT_7, m_coil_id[6]);
    DDX_Control(pDX, IDC_COILPORT_8, m_coil_id[7]);
    DDX_Control(pDX, IDC_COILPORT_9, m_coil_id[8]);
    DDX_Control(pDX, IDC_COILPORT_10, m_coil_id[9]);
    DDX_Control(pDX, IDC_COILPORT_11, m_coil_id[10]);
    DDX_Control(pDX, IDC_COILPORT_12, m_coil_id[11]);
    DDX_Control(pDX, IDC_COILPORT_13, m_coil_id[12]);

    DDX_Control(pDX, IDC_PORT_NAME_1, m_port_name[0]);
    DDX_Control(pDX, IDC_PORT_NAME_2, m_port_name[1]);
    DDX_Control(pDX, IDC_PORT_NAME_3, m_port_name[2]);
    DDX_Control(pDX, IDC_PORT_NAME_4, m_port_name[3]);
    DDX_Control(pDX, IDC_PORT_NAME_5, m_port_name[4]);
    DDX_Control(pDX, IDC_PORT_NAME_6, m_port_name[5]);
    DDX_Control(pDX, IDC_PORT_NAME_7, m_port_name[6]);
    DDX_Control(pDX, IDC_PORT_NAME_8, m_port_name[7]);
    DDX_Control(pDX, IDC_PORT_NAME_9, m_port_name[8]);
    DDX_Control(pDX, IDC_PORT_NAME_10, m_port_name[9]);
    DDX_Control(pDX, IDC_PORT_NAME_11, m_port_name[10]);
    DDX_Control(pDX, IDC_PORT_NAME_12, m_port_name[11]);
    DDX_Control(pDX, IDC_PORT_NAME_13, m_port_name[12]);

    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCOMPASS_Location_Debug message handlers


//************************************Method Header************************************
// Method Name  : OnInitDialogImpl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CCOMPASS_Location_Debug::OnInitDialogImpl()
{


    // TODO: Add extra initialization here

    SetDialogRibbonHeight();

    SetTitleBarHeight(24) ;
    SetFont(_T("Arial BOLD"), 10) ;

    SetButtonStyle();
    UpdateDialogParams();

    InitCtrlForInternationalization();

    return CZOrderDialog::OnInitDialogImpl();  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}



//************************************Method Header************************************
// Method Name  : OnOK
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::OnOK()
{
    // TODO: Add extra validation here

    UpdateDataFromDialog();

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
void CCOMPASS_Location_Debug::OnCancel()
{
    // TODO: Add extra validation here

    UpdateDataFromDialog();

    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    /*CDialog*///DialogEx::OnCancel();
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}
//************************************Method Header************************************
// Method Name  : InitCtrlForInternationalization
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::InitCtrlForInternationalization(
)
{
    SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_COMPASS_LOCATION_DEBUG")));
}


//************************************Method Header************************************
// Method Name  : UpdateDialogParams
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::UpdateDialogParams(
)
{

    for (int l_index = 0 ; l_index < MAX_PARAM; l_index ++) {

        UpdateDataForIndex(l_index);
    }
}


//************************************Method Header************************************
// Method Name  : UpdateDataForIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::UpdateDataForIndex(
    const int f_index
)
{
    if (f_index >= 0 && f_index < MAX_PARAM) {

        COMPASS_Debugg_Param* l_debug_param = &m_debug_param[f_index];

        if (!(l_debug_param->m_coilid.IsEmpty())) {
            m_coil_id[f_index].SetWindowText(l_debug_param->m_coilid);

        } else {
            m_coil_id[f_index].SetWindowText(_T("----"));
        }

        CString l_str ;
        l_str.Format(_T("%d") , l_debug_param->m_location);
        m_coil_location[f_index].SetWindowText(l_str);

        l_str.Format(_T("%d") , l_debug_param->m_detectedflag);
        m_detect_flag[f_index].SetWindowText(l_str);
    }
}


//************************************Method Header************************************
// Method Name  : UpdateDataFromDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::UpdateDataFromDialog(
)
{
    for (int l_index = 0 ; l_index < MAX_PARAM; l_index ++) {

        UpdateDataItemFromDialog(l_index);
    }

}


//************************************Method Header************************************
// Method Name  : UpdateDataItemFromDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::UpdateDataItemFromDialog(
    const int f_index
)
{
    if (f_index >= 0 && f_index < MAX_PARAM) {

        COMPASS_Debugg_Param* l_debug_param = &m_debug_param[f_index];

        if (!l_debug_param->m_coilid.IsEmpty()) {

            CString l_str ;
            m_coil_location[f_index].GetWindowText(l_str);

            l_debug_param->m_location = _wtoi(l_str);

            m_detect_flag[f_index].GetWindowText(l_str);

            l_debug_param->m_detectedflag = (_wtoi(l_str) > 0);
        }
    }
}

//************************************Method Header************************************
// Method Name  : SetButtonStyle
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::SetButtonStyle(
)
{
    m_set_button.SetCurveType(CURVE_LARGE_5);
    m_cancel_button.SetCurveType(CURVE_LARGE_5);

    const CString l_button_font_name = _T("Verdana BOLD") ;
    const int l_size = 12;
    m_set_button.SetPrimaryTextFont(l_button_font_name, l_size);
    m_cancel_button.SetPrimaryTextFont(l_button_font_name, l_size);


    const int l_total_param_ctrls = MAX_PARAM;

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    for (int l_index = 0; l_index < l_total_param_ctrls ; l_index ++) {

        //color_statics[static_button]->LoadDeafultStaticCtrlColorsWithBKColor(bk_color);

        m_coil_id[l_index].SetColorData(bk_color, g_pqmdialog_static_ctrl_text_color);
        m_coil_id[l_index].SetFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
        m_coil_id[l_index].UpdateFont();

        m_port_name[l_index].SetColorData(bk_color, g_pqmdialog_static_ctrl_text_color);
        m_port_name[l_index].SetFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
        m_port_name[l_index].UpdateFont();
    }

    const int l_ctrl_font_size = 9;

    for (l_index = 0 ; l_index < l_total_param_ctrls; l_index ++) {
        m_coil_location[l_index].SetFont(l_button_font_name, l_ctrl_font_size);
        m_coil_location[l_index].SetTextAlignment(VCENTER);

        m_detect_flag[l_index].SetFont(l_button_font_name, l_ctrl_font_size);
        m_detect_flag[l_index].SetTextAlignment(VCENTER);
    }
}

//************************************Method Header************************************
// Method Name  : SetDialogRibbonHeight
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCOMPASS_Location_Debug::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_cancel_button.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}



