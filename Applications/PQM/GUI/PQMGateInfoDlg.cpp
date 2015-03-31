//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGateInfoDlg.cpp
 *  Overview: Implementation of CPQMGateInfoDlg class.
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
#include "PQMGateInfoDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>	//added by Hemant
#include "PQMUtility.h"	// Added By KT/ACE-2/11-06-2009/Multilingualisation		
#include "DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

BEGIN_MESSAGE_MAP(CPQMGateInfoDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMGateInfoDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPQMGateInfoDlg::CPQMGateInfoDlg(CStringArray* gate_str_arr, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_DLG_GATE, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_gate_str_arr(gate_str_arr)

{
    //{{AFX_DATA_INIT(CPQMGateInfoDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CPQMGateInfoDlg::DoDataExchange(
    CDataExchange* pDX
)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMGateInfoDlg)
    DDX_Control(pDX, IDCANCEL, m_close);
    DDX_Control(pDX, IDC_LIST_GATE, m_c_list_gate);
    //}}AFX_DATA_MAP
}



/*************************************************************************************

//Method:  OnInitDialogImpl

//Author:  PATNI/GP

//Purpose: Initialize controls in dialog box.

//**************************************************************************************/
BOOL CPQMGateInfoDlg::OnInitDialogImpl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMGateInfoDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CStringArray str_arr;
    CString str = CPQMUtility::GetMultiLingualString(_T("IDS_GATE_GRS"));
    str_arr.Add(str);

    str = CPQMUtility::GetMultiLingualString(_T("IDS_GATE_MIN_GRS"));
    str_arr.Add(str);

    str = CPQMUtility::GetMultiLingualString(_T("IDS_GATE_MAX_GRS"));
    str_arr.Add(str);

    InsertColumnsInGateInfoList(str_arr);

    DisplayGateInfo(str_arr);

    m_c_list_gate.SetBkColor(BACKGROUNDCOLOR);
    m_c_list_gate.SetTextBkColor(BACKGROUNDCOLOR);
    m_c_list_gate.SetTextColor(TEXTCOLOR);

    m_c_list_gate.SetFont(_T("Arial"), 12);

    SetButtonAppearence();
    SetDialogRibbonHeight();

    SetMultilingual();

    return CZOrderDialog::OnInitDialogImpl(); // return TRUE unless you set the focus to a control
}


/*************************************************************************************

//Method:  OnCancel

//Author:  PATNI/GP

//Purpose: Closes dialog box.

//**************************************************************************************/
void CPQMGateInfoDlg::OnCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMGateInfoDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_GATE_INFO_CLOSE);

    CZOrderDialog::OnCancel();
}

BOOL CPQMGateInfoDlg::PreTranslateMessageImpl(
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

/*************************************************************************************

//Method:  DisplayGateInfo

//Author:  PATNI/GP

//Purpose: Display gate information

//**************************************************************************************/
void CPQMGateInfoDlg::DisplayGateInfo(
    const CStringArray& str_arr
)
{
    LPCTSTR FUNC_NAME = _T("CPQMGateInfoDlg::DisplayGateInfo");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CStringArray* gate_str_arr = m_gate_str_arr;

    if (gate_str_arr == NULL || (!gate_str_arr->GetSize())) {
        return;
    }

    for (int i = 0; i < gate_str_arr->GetSize(); i++) {
        m_c_list_gate.InsertItem(LVIF_TEXT | LVIF_STATE, i, gate_str_arr->GetAt(i), 0, 0, 0, 0);
    }

}

/*************************************************************************************

//Method:  InsertColumnsInGateInfoList

//Author:  PATNI/GP

//Purpose: Insert columns in gate information list

//**************************************************************************************/
void CPQMGateInfoDlg::InsertColumnsInGateInfoList(
    const CStringArray& str_arr
)
{
    for (int i = 0; i < 1; i++) {

        m_c_list_gate.InsertColumn(i, str_arr[i], LVCFMT_LEFT, GATE_COLUMN_WIDTH, -1);
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGateInfoDlg::SetButtonAppearence(
)
{
    SetButtonFont();

    m_close.SetCurveType(CURVE_LARGE_5);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGateInfoDlg::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_close.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 5);
}


//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMGateInfoDlg::SetButtonFont(
)
{
    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_close.SetPrimaryTextFont(button_font_name, size);
    m_close.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    return TRUE;
}

//Language usha
void CPQMGateInfoDlg::SetMultilingual()
{

    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_DLG_GATE_INFO_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_GATEINFO_CANCEL"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;
}