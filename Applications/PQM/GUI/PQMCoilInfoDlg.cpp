//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-778
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMCoilInfoDlg.cpp
 *  Overview: Implementation of CPQMCoilInfoDlg class.
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
#include "PQMCoilInfoDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>  //added by Hemant
#include "PQMUtility.h"	   //Added By KT/ACE-2/11-06-2009/Multilingualisation		
#include "PQMView.h"
#include "DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;
/////////////////////////////////////////////////////////////////////////////
// CPQMCoilInfoDlg dialog


BEGIN_MESSAGE_MAP(CPQMCoilInfoDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMCoilInfoDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPQMCoilInfoDlg::CPQMCoilInfoDlg(ConnectedCoilList* f_coildata_list, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_DLG_COIL, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_coildata_list(f_coildata_list),
    m_parent((CPQMView*)pParent)
{
    //{{AFX_DATA_INIT(CPQMCoilInfoDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}

/***************************************************************************

//Module:  OnInitDialogImpl

//Author:  PATNI/GP

//Purpose: Initialize controls in dialog box.

//***************************************************************************/
BOOL CPQMCoilInfoDlg::OnInitDialogImpl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCoilInfoDlg::OnInitDialog");
    PQM_TRACE_SCOPED(FUNC_NAME);

    InsertColumnsInCoilInfoList();

    DisplayCoilInfo();

    m_c_list_coil.SetBkColor(BACKGROUNDCOLOR);
    m_c_list_coil.SetTextBkColor(BACKGROUNDCOLOR);
    m_c_list_coil.SetTextColor(TEXTCOLOR);

    //Added by Hemant On 11/10/2008 4:20:55 PM
    m_c_list_coil.SetFont(_T("Arial"), 12);

    //Added by Hemant On 6/4/2008 5:50:39 PM
    SetButtonAppearence();
    SetDialogRibbonHeight();

    SetMultilingual();//Language usha


    return CZOrderDialog::OnInitDialogImpl(); // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CPQMCoilInfoDlg::DoDataExchange(
    CDataExchange* pDX
)
{
    CZOrderDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CPQMCoilInfoDlg)
    DDX_Control(pDX, IDC_STATIC_LABEL_PORT, m_port);
    DDX_Control(pDX, IDC_STATIC_LABEL_NAME, m_name);
    DDX_Control(pDX, IDC_STATIC_LABEL_ID, m_id);
    DDX_Control(pDX, IDC_LIST_COIL, m_c_list_coil);
    DDX_Control(pDX, IDCANCEL, m_close);
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPQMCoilInfoDlg message handlers


//*************************************************************************************

//Module:  OnCancel

//Author:  PATNI/GP

//Purpose: Close dialog

//*************************************************************************************
void CPQMCoilInfoDlg::OnCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCoilInfoDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_COIL_INFO_CLOSE);

    CZOrderDialog::OnCancel();
}

BOOL CPQMCoilInfoDlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{

    //+Patni- Manishkumar/2009May13/Added/JFT-120
    if (pMsg->wParam == VK_RETURN) {
        pMsg->wParam = NULL;
    }

    //-Patni- Manishkumar/2009May13/Added/JFT-120

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}





//*****************************************************************************

//Module:  DisplayCoilInfo

//Author:  PATNI/GP

//Purpose: Display coil information

//****************************************************************************
void CPQMCoilInfoDlg::DisplayCoilInfo(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCoilInfoDlg::DisplayCoilInfo");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_coildata_list) {

        POSITION pos = m_coildata_list->GetHeadPosition();

        CString coil_str = _T("");
        int count = 0;

        while (pos) {

            coil_str = m_coildata_list->GetNext(pos);

            //m_c_list_coil.InsertItem(LVIF_TEXT|LVIF_STATE,count,coil_str,LVIS_SELECTED,LVIS_SELECTED,count,0);
            m_c_list_coil.InsertItem(count, coil_str);
            m_c_list_coil.SetItemText(count, 0, coil_str);
            count++;
        }
    }

    return ;
}



//*************************************************************************************

//Module:  InsertColumnsInCoilInfoList

//Author:  PATNI/GP

//Purpose: Insert coluns in coil info list

//*************************************************************************************
void CPQMCoilInfoDlg::InsertColumnsInCoilInfoList(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    const int l_offset = 25;

    m_c_list_coil.InsertColumn(0 , CPQMUtility::GetMultiLingualString(_T("IDS_COIL_NAME")), LVCFMT_LEFT, client_rect.Width() - l_offset , -1);
}

void CPQMCoilInfoDlg::SetMultilingual()
{
    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COIL_INFO_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_STATIC_LABEL_PORT"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_LABEL_PORT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_STATIC_LABEL_NAME"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_LABEL_NAME);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_STATIC_LABEL_ID"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_LABEL_ID);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COIL_INFO_CANCEL"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

}

//************************************Method Header************************************
// Method Name  : SetButtonAppearence
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCoilInfoDlg::SetButtonAppearence(
)
{
    SetButtonFont();
    m_close.SetCurveType(CURVE_LARGE_5);

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    m_port.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_name.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_id.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
}


//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCoilInfoDlg::SetButtonFont(
)
{

    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_close.SetPrimaryTextFont(button_font_name, size);
    m_close.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    return TRUE ;
}//Language usha



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCoilInfoDlg::SetDialogRibbonHeight(
)
{

    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_close.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 5);
}



