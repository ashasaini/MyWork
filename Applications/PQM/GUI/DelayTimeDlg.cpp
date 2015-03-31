/*****************************************************************************
*
*  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
*
*  File Name: DelayTimeDlg.cpp
*  Overview: Structure of DelayTimeDlg class.
*  Purpose:  NFD002_Demo_CR-3 Delay Edit Dialog
*****************************************************************************
*                       Revision History
*****************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*
*****************************************************************************/

#include "stdafx.h"
#include "DelayTimeDlg.h"
#include <imm.h>
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include "PQM\resource.h"
#include "PqmGuiConstants.h"
#include "pqm.h"
#include "PQMView.h"
#include "PQMUtility.h"
#include "DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CPoint INITIAL_TOPLEFT(566, 580);

/////////////////////////////////////////////////////////////////////////////
// CDelayTimeDlg dialog
using namespace MR::ACQ::SM::COMMON;
using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

extern CPQMView* g_MainView;
//****************************Method Header************************************
//Module         : CDelayTimeDlg()
//Author         : PATNI/AKR
//Purpose        : Constructor
//*****************************************************************************
CDelayTimeDlg::CDelayTimeDlg(CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_DELAY, pParent),
    m_delay_time_minute(MINUTE_MIN_DELAY, MINUTE_MAX_DELAY),
    m_delay_time_sec(SECOND_MIN_DELAY, SECOND_MAX_DELAY)
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CDelayTimeDlg::CDelayTimeDlg"),
              _T("CDelayTimeDlg::CDelayTimeDlg"));
}
//****************************Method Header************************************
//Module         : ~CDelayTimeDlg()
//Author         : PATNI/AKR
//Purpose        : Destructor
//*****************************************************************************
CDelayTimeDlg::~CDelayTimeDlg()
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CDelayTimeDlg::CDelayTimeDlg"),
              _T("CDelayTimeDlg::~CDelayTimeDlg"));
}
//****************************Method Header************************************
//Module         : ConvertTimeStrToInt()
//Author         : PATNI/AKR
//Purpose        : Convert time string to integer value (e.g. 05:00 to 300)
//*****************************************************************************
int CDelayTimeDlg::ConvertTimeStrToInt(const CString& f_str)
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::ConvertTimeStrToInt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int index = f_str.Find(':');

    if (-1 == index) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("str is not in time format."));
        return 0;
    }

    CString l_temp = f_str.Left(index);
    const int l_min = _ttoi(f_str);
    l_temp = f_str.Mid(index + 1);
    const int l_sec = _ttoi(l_temp);
    return (l_min * NUM_60 + l_sec);
}
//****************************Method Header************************************
//Module         : ConvertIntStrToTimeStr()
//Author         : PATNI/AKR
//Purpose        : Convert integer to time string (e.g. 300 to 05:00)
//*****************************************************************************
CString CDelayTimeDlg::ConvertIntStrToTimeStr(const CString& f_str)
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::ConvertIntStrToTimeStr");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("Trace"));

    const int l_time = _ttoi(f_str);
    const int l_min = l_time / NUM_60;
    const int l_sec = l_time % NUM_60;
    CString l_delay_time = _T("");
    l_delay_time.Format(_T("%02d:%02d"), l_min, l_sec);
    return l_delay_time;
}
//****************************Method Header************************************
//Module         : PreTranslateMessage()
//Author         : PATNI/AKR
//Purpose        : To pretranslate messages
//*****************************************************************************
BOOL CDelayTimeDlg::PreTranslateMessageImpl(MSG* pMsg)
{
    if (pMsg->wParam == VK_RETURN) {
        if (pMsg->hwnd == m_edit_delay_min.m_hWnd || pMsg->hwnd == m_edit_delay_sec.m_hWnd) {//Redmine-2185

            SaveDelayTime();
            DisplayCurrentDelayTime();
            return TRUE;
        }
    }

    switch (pMsg->message) {
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
            return 1;

            //Redmine-2185
        case WM_KEYDOWN: {
            if ((pMsg->wParam == VK_DOWN) || (pMsg->wParam == VK_UP)) {
                ProcessKeyDownMsg(pMsg);
                return 1;
            }
        }

        default:
            break;
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}
//****************************Method Header************************************
//Module         : DoDataExchange()
//Author         : PATNI/AKR
//Purpose        : To exchange dialog data
//*****************************************************************************
void CDelayTimeDlg::DoDataExchange(CDataExchange* pDX)
{
    PQM_TRACE(USER_FUNC_MARKER, _T("CDelayTimeDlg::OnInitDialog"),
              _T("CDelayTimeDlg::DoDataExchange"));

    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDelayTimeDlg)
    DDX_Control(pDX, IDC_STATIC_LABLE_DELAY, m_lable_delay_time);////Redmine-2185
    DDX_Control(pDX, IDC_EDIT_DELAY_SEC, m_edit_delay_sec);
    DDX_Control(pDX, IDC_EDIT_DELAY_MIN, m_edit_delay_min);
    DDX_Control(pDX, IDCANCEL_DELAY, m_close);
    DDX_Control(pDX, IDOK_DELAY, m_ok);
    DDX_Control(pDX, IDC_STATIC_DELAY_COLON, m_lable_delay_colon);

    //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CDelayTimeDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CDelayTimeDlg)
    ON_BN_CLICKED(IDCANCEL_DELAY, OnDelayClose)
    ON_BN_CLICKED(IDOK_DELAY, OnDelayOk)
    ON_WM_MOUSEWHEEL()//Redmine-2185
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
//****************************Method Header************************************
//Module         : OnInitDialogImpl()
//Author         : PATNI/AKR
//Purpose        : Read selected sequence and populate on dialog
//*****************************************************************************
BOOL CDelayTimeDlg::OnInitDialogImpl()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::OnInitDialogImpl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    //Redmine-2185

    CRect l_window_rect;
    GetWindowRect(&l_window_rect) ;

    l_window_rect.OffsetRect(INITIAL_TOPLEFT - l_window_rect.TopLeft()) ;
    SetWindowPos(NULL, l_window_rect.left, l_window_rect.top, l_window_rect.Width() , l_window_rect.Height() , SWP_NOSIZE | SWP_NOZORDER) ;

    CString l_min_str = _T(""), l_sec_str = _T("");
    l_min_str.Format(_T("%02d"), m_delay_time_minute.m_min_value);
    l_sec_str.Format(_T("%02d"), m_delay_time_sec.m_min_value);

    m_edit_delay_min.SetWindowText(l_min_str);
    m_edit_delay_sec.SetWindowText(l_sec_str);


    m_edit_delay_min.SetLimitText(DELAY_TIME_LENGTH);
    m_edit_delay_sec.SetLimitText(DELAY_TIME_LENGTH);

    SetButtonAppearence();
    m_lable_delay_colon.LoadDefaultFontAttributes();
    m_lable_delay_colon.UpdateFont();
    SetMultilingual();

    DisplayCurrentDelayTime();

    return CZOrderDialog::OnInitDialogImpl(false);
}
//****************************Method Header************************************
//Module         : OnDelayClose()
//Author         : PATNI/AKR
//Purpose        : Discard changes made and close dialog
//*****************************************************************************
void CDelayTimeDlg::OnDelayClose()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::OnDelayClose");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CDelayTimeDlg::OnDelayClose"));

    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL_DELAY);
    }
}
//****************************Method Header************************************
//Module         : OnDelayOk()
//Author         : PATNI/AKR
//Purpose        : Save changes, display updated value and close dialog
//*****************************************************************************
void CDelayTimeDlg::OnDelayOk()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::OnDelayOk");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CDelayTimeDlg::OnDelayOk"));

    if (false == SaveDelayTime()) {
        DisplayCurrentDelayTime();

        return;
    }

    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDOK_DELAY);
    }
}

//****************************Method Header************************************
//Module         : SetButtonAppearence()
//Author         : PATNI/AKR
//Purpose        : Set buttons appearance
//*****************************************************************************
void CDelayTimeDlg::SetButtonAppearence()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::SetButtonAppearence");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    SetButtonFont();
    SetDialogRibbonHeight();
    m_close.SetCurveType(CURVE_LARGE_5);

    m_ok.SetCurveType(CURVE_LARGE_5);

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF l_bk_color = stColors_o.crClientColor;

    m_lable_delay_time.LoadDeafultStaticCtrlColorsWithBKColor(l_bk_color);
    m_lable_delay_colon.LoadDeafultStaticCtrlColorsWithBKColor(l_bk_color);
}
//****************************Method Header************************************
//Module         : SetButtonFont()
//Author         : PATNI/AKR
//Purpose        : Set the button fonts
//*****************************************************************************
void CDelayTimeDlg::SetButtonFont()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::SetButtonFont");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const CString l_button_font_name = _T("Verdana BOLD") ;
    const int l_size = 9;

    m_edit_delay_min.SetFont(l_button_font_name, l_size);
    m_edit_delay_sec.SetFont(l_button_font_name, l_size);

    m_close.SetPrimaryTextFont(l_button_font_name, l_size);
    m_close.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);


    m_ok.SetPrimaryTextFont(l_button_font_name, l_size);
    m_ok.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
}
//****************************Method Header************************************
//Module         : SetDialogRibbonHeight()
//Author         : PATNI/AKR
//Purpose        : To set the ribbon height of the dialog
//*****************************************************************************
void CDelayTimeDlg::SetDialogRibbonHeight()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::SetDialogRibbonHeight");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CRect l_client_rect;
    GetClientRect(&l_client_rect);

    CRect l_ok_button_rect ;
    m_close.GetWindowRect(&l_ok_button_rect);
    ScreenToClient(&l_ok_button_rect);

    SetRibbonHeight(l_client_rect.bottom - l_ok_button_rect.top + NUM_5);
}
//****************************Method Header************************************
//Module         : SaveDelayTime()
//Author         : PATNI/AKR
//Purpose        : Save delay time in sequences and vf DB
//*****************************************************************************
bool CDelayTimeDlg::SaveDelayTime()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::SaveDelayTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CString l_delay_time_min_str = _T("");
    CString l_delay_time_sec_str = _T("");

    m_edit_delay_min.GetWindowText(l_delay_time_min_str);
    m_edit_delay_sec.GetWindowText(l_delay_time_sec_str);

    const int l_min = _ttoi(l_delay_time_min_str);
    const int	l_sec = _ttoi(l_delay_time_sec_str);
    const int	l_total_sec = (l_min * NUM_60) + l_sec;
    CString l_total_sec_str;
    l_total_sec_str.Format(_T("%04d"), l_total_sec);;

    l_total_sec_str = ConvertIntStrToTimeStr(l_total_sec_str);

    if (MINUTE_MAX_DELAY < l_min || l_min < MINUTE_MIN_DELAY || l_sec > SECOND_MAX_DELAY || l_sec < SECOND_MIN_DELAY) {
        PQM_TRACE(USER_FUNC_MARKER, _T("CDelayTimeDlg::SaveDelayTime"),
                  _T("delay time is out of range."));
        //Display Error Message
        CString l_err_msg =  CPQMUtility::GetMultiLingualString(_T("IDS_DELAY_TIME_INVALID"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_err_msg, MPlus::EH::Information, _T("PQM"), _T("PQM"));
        return false;
    }

    if (g_MainView) {
        g_MainView->SaveDelayTime(l_total_sec_str);
    }

    return true;
}
//****************************Method Header************************************
//Module         : DisplayCurrentDelayTime()
//Author         : PATNI/AKR
//Purpose        : Display current delay time on delay dialog
//*****************************************************************************
void CDelayTimeDlg::DisplayCurrentDelayTime()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::DisplayCurrentDelayTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    const CString l_delay_time = GetCurrentDelayTime();
    const int l_delay = ConvertTimeStrToInt(l_delay_time);
    const int l_min = l_delay / NUM_60;
    const int l_sec = l_delay % NUM_60;
    CString l_min_str = _T(""), l_sec_str = _T("");
    l_min_str.Format(_T("%02d"), l_min);
    l_sec_str.Format(_T("%02d"), l_sec);

    m_edit_delay_min.SetWindowText(l_min_str);
    m_edit_delay_sec.SetWindowText(l_sec_str);

}
//****************************Method Header************************************
//Module         : GetCurrentDelayTime()
//Author         : PATNI/AKR
//Purpose        : Get the delay time of selected sequence
//*****************************************************************************
CString CDelayTimeDlg::GetCurrentDelayTime()
{
    return g_MainView->GetCurrentDelayTime();
}
//****************************Method Header************************************
//Module         : SetMultilingual()
//Author         : PATNI/AKR
//Purpose        : To set multilingual strings of the controls
//*****************************************************************************
void CDelayTimeDlg::SetMultilingual()
{
    LPCTSTR FUNC_NAME = _T("CDelayTimeDlg::SetMultilingual");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CDelayTimeDlg::SetMultilingual"));

    //Delay Diaolg title
    CWnd ::SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_DELAY_TIME_DLG_TITLE")));

    //Static lable- delay time
    HWND l_button_user_hwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_LABLE_DELAY);
    ::SetWindowText(l_button_user_hwnd, CPQMUtility::GetMultiLingualString(_T("IDC_STATIC_LABLE_DELAY")));

    //static lable -colon(:)
    l_button_user_hwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_DELAY_COLON);
    ::SetWindowText(l_button_user_hwnd, _T(":"));

    //Edit Control- cancle
    l_button_user_hwnd = ::GetDlgItem(m_hWnd, IDCANCEL_DELAY);
    ::SetWindowText(l_button_user_hwnd, CPQMUtility::GetMultiLingualString(_T("IDCANCEL_DELAY")));

    //Edit Control - ok
    l_button_user_hwnd = ::GetDlgItem(m_hWnd, IDOK_DELAY);
    ::SetWindowText(l_button_user_hwnd, CPQMUtility::GetMultiLingualString(_T("IDOK_DELAY")));
}

//************************************Method Header************************************
// Method Name  : OnMouseWheel
// Author       : PATNI/pruthviraj
// Purpose      : Redmine-2185
//***********************************************************************************
BOOL CDelayTimeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // we don't handle anything but scrolling
    if (nFlags & (MK_SHIFT | MK_CONTROL)) {
        return FALSE;
    }

    EditCtrlEx* l_edit_ctrl = (EditCtrlEx*)GetFocus();
    const int l_mousemovecount = 120;

    if (m_edit_delay_min.m_hWnd == l_edit_ctrl->m_hWnd) {

        CRect l_delay_time_min_rect ;
        m_edit_delay_min.GetWindowRect(&l_delay_time_min_rect);

        if (l_delay_time_min_rect.PtInRect(pt)) {

            //Increment or decrement?
            if (zDelta == l_mousemovecount) {
                IncreaseMinuteByOneUnit();

            } else if (zDelta == -(l_mousemovecount)) {
                DecreaseMinuteByOneUnit();
            }

            DisplayCurrentMinuteValue();
        }

    } else if (m_edit_delay_sec.m_hWnd == l_edit_ctrl->m_hWnd) {

        CRect l_delay_time_sec_rect ;
        m_edit_delay_sec.GetWindowRect(&l_delay_time_sec_rect);

        if (l_delay_time_sec_rect.PtInRect(pt)) {

            //Increment or decrement?
            if (zDelta == l_mousemovecount) {
                IncreaseSecondByOneUnit();

            } else if (zDelta == -(l_mousemovecount)) {
                DecreaseSecondByOneUnit();
            }

            DisplayCurrentSecondValue();

        }

    }

    return CZOrderDialog::OnMouseWheel(nFlags, zDelta, pt);
}




//************************************Method Header************************************
// Method Name  : ProcessKeyDownMsg
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::ProcessKeyDownMsg(MSG* pMsg)
{

    if (m_edit_delay_min.m_hWnd == pMsg->hwnd) {
        if (pMsg->wParam == VK_UP) {
            IncreaseMinuteByOneUnit();

        } else if (pMsg->wParam == VK_DOWN) {
            DecreaseMinuteByOneUnit();
        }

        DisplayCurrentMinuteValue();

    } else if (m_edit_delay_sec.m_hWnd == pMsg->hwnd) {
        if (pMsg->wParam == VK_UP) {
            IncreaseSecondByOneUnit();

        } else if (pMsg->wParam == VK_DOWN) {
            DecreaseSecondByOneUnit();
        }

        DisplayCurrentSecondValue();
    }
}



//************************************Method Header************************************
// Method Name  : IncreaseMinuteByOneUnit
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::IncreaseMinuteByOneUnit()
{
    CString l_str_delay_time_min(_T(""));
    m_edit_delay_min.GetWindowText(l_str_delay_time_min);
    m_delay_time_minute.m_curr_value = _wtoi(l_str_delay_time_min);

    if (m_delay_time_minute.m_curr_value < m_delay_time_minute.m_max_value) {
        m_delay_time_minute.m_curr_value += m_delay_time_minute.m_increment_value;
    }
}


//************************************Method Header************************************
// Method Name  : DecreaseMinuteByOneUnit
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::DecreaseMinuteByOneUnit()
{
    CString l_str_delay_time_min(_T(""));
    m_edit_delay_min.GetWindowText(l_str_delay_time_min);
    m_delay_time_minute.m_curr_value = _wtoi(l_str_delay_time_min);

    if (m_delay_time_minute.m_curr_value > m_delay_time_minute.m_min_value) {
        m_delay_time_minute.m_curr_value -= m_delay_time_minute.m_increment_value;
    }
}


//************************************Method Header************************************
// Method Name  : IncreaseSecondByOneUnit
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::IncreaseSecondByOneUnit()
{
    CString l_str_delay_time_sec(_T(""));
    m_edit_delay_sec.GetWindowText(l_str_delay_time_sec);
    m_delay_time_sec.m_curr_value = _wtoi(l_str_delay_time_sec);

    if (m_delay_time_sec.m_curr_value < m_delay_time_sec.m_max_value) {
        m_delay_time_sec.m_curr_value += m_delay_time_sec.m_increment_value;
    }
}


//************************************Method Header************************************
// Method Name  : DecreaseSecondByOneUnit
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::DecreaseSecondByOneUnit()
{
    CString l_str_delay_time_sec(_T(""));
    m_edit_delay_sec.GetWindowText(l_str_delay_time_sec);
    m_delay_time_sec.m_curr_value = _wtoi(l_str_delay_time_sec);

    if (m_delay_time_sec.m_curr_value > m_delay_time_sec.m_min_value) {
        m_delay_time_sec.m_curr_value -= m_delay_time_sec.m_increment_value;
    }

}

//************************************Method Header************************************
// Method Name  : DisplayCurrentMinuteValue
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::DisplayCurrentMinuteValue()
{
    CString l_min_str = _T("");
    l_min_str.Format(_T("%02d"), m_delay_time_minute.m_curr_value);
    m_edit_delay_min.SetWindowText(l_min_str);


}


//************************************Method Header************************************
// Method Name  : DisplayCurrentSecondValue
// Author       : PATNI/
// Purpose      : Redmine-2185
//***********************************************************************************
void CDelayTimeDlg::DisplayCurrentSecondValue()
{
    CString l_sec_str = _T("");
    l_sec_str.Format(_T("%02d"), m_delay_time_sec.m_curr_value);
    m_edit_delay_sec.SetWindowText(l_sec_str);
}