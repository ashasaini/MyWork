//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-781
//TMSC-REDMINE-777

// PQMDebugWinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PQMDebugWinDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
//Patni/KSS/2011Mar31/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include <PQM/resource.h>
#include "DPSManager.h"
#include "PQMView.h"

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;
//Patni/KSS/2011Mar31/Added/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CPQMView* g_MainView ;
/////////////////////////////////////////////////////////////////////////////
// CPQMDebugWinDlg dialog

BEGIN_MESSAGE_MAP(CPQMDebugWinDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMDebugWinDlg)
    ON_BN_CLICKED(IDC_GLBDATA, OnGlbdata)
    ON_BN_CLICKED(IDC_CLEAR, OnClear)
    ON_BN_CLICKED(IDC_ACQMON, OnAcqmon)
    ON_BN_CLICKED(IDC_LISTCNT, OnListcnt)
    ON_BN_CLICKED(IDC_LISTARR, OnListarr)
    ON_BN_CLICKED(IDC_PORTINFO, OnPortinfo)
    ON_BN_CLICKED(IDC_COUCHINFO, OnCouchinfo)
    ON_BN_CLICKED(IDC_BTNSIZE, OnBtnsize)
    ON_BN_CLICKED(IDC_SAR, OnSar)
    ON_BN_CLICKED(IDC_SHIM, OnShim)
    ON_BN_CLICKED(IDC_LISTMON, OnListmon)
    ON_BN_CLICKED(IDC_PROLIMMON, OnProlimmon)
    ON_BN_CLICKED(IDC_SARMON, OnSarmon)
    ON_BN_CLICKED(IDC_IRSMON, OnIrsmon)
    ON_BN_CLICKED(IDC_MONOFF, OnMonoff)
    ON_BN_CLICKED(IDC_ACQINFO, OnAcqinfo)
    ON_BN_CLICKED(IDC_STUDYDATA, OnStudydata)
    ON_WM_SETCURSOR()

    ON_WM_ACTIVATE()

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//**************************************************************************
//Method Name   : CPQMDebugWinDlg
//Author        : PATNI\DKH
//Purpose       : Constructor
//**************************************************************************
CPQMDebugWinDlg::CPQMDebugWinDlg(CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_PQMDEBUG_DIALOG, pParent),
    m_buttonmask(0),
    m_debuglinecount(0),
    m_show_wait_cursor_for_debugwindlg(false)
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01

{

    //{{AFX_DATA_INIT(CPQMDebugWinDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}

//**************************************************************************
//Method Name   : UpdateDebugData
//Author        : PATNI\DKH
//Purpose       : To show the debug string on the ui
//**************************************************************************
void CPQMDebugWinDlg::UpdateDebugData(const CString& f_csdebugdata)
{
    int count = m_debug_info.GetItemCount();

    m_debug_info.InsertItem(count, f_csdebugdata);
    m_debuglinecount++;

    if (m_debuglinecount > 20) {
        m_debug_info.EnsureVisible(count, FALSE);
    }
}


//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
//**************************************************************************
//Method Name   : InsertDebugStr
//Author        : PATNI\Sribanta
//Purpose       : Add debug string to list
//**************************************************************************
void CPQMDebugWinDlg::InsertDebugStr(
    const CString& f_csdebugdata
)
{
    PushMsg(f_csdebugdata);
}
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026


//+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
//**************************************************************************
//Method Name   : UpdateDebugWnd
//Author        : PATNI\Sribanta
//Purpose       : To Update the Debug window with latest-inqueue message strings
//**************************************************************************
void CPQMDebugWinDlg::UpdateDebugWnd(
)
{
    CString	l_log_str(_T(""));

    if (PopMsg(l_log_str)) {
        UpdateDebugData(l_log_str);
    }
}
//-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMDebugWinDlg::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_debugwindlg = f_show_wait_cursor;
}

void CPQMDebugWinDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMDebugWinDlg)
    DDX_Control(pDX, IDC_STATIC_LISTINFO, m_listinfo);
    DDX_Control(pDX, IDC_STATIC_PROTOCOLDATA, m_protocoldata);
    DDX_Control(pDX, IDCANCEL, m_close);
    DDX_Control(pDX, IDC_SHIM, m_shimming);
    DDX_Control(pDX, IDC_SARMON, m_sar_mon);
    DDX_Control(pDX, IDC_SAR, m_sar);
    DDX_Control(pDX, IDC_PROLIMMON, m_prolim_mon);
    DDX_Control(pDX, IDC_PORTINFO, m_prot_info);
    DDX_Control(pDX, IDC_MONOFF, m_monitor_off);
    DDX_Control(pDX, IDC_LISTMON, m_list_mon);
    DDX_Control(pDX, IDC_LISTCNT, m_list_cnt);
    DDX_Control(pDX, IDC_LISTARR, m_list_array);
    DDX_Control(pDX, IDC_IRSMON, m_irs_mon);
    DDX_Control(pDX, IDC_CLEAR, m_clear);
    DDX_Control(pDX, IDC_BTNSIZE, m_size);
    DDX_Control(pDX, IDC_ACQMON, m_acq_mon);
    DDX_Control(pDX, IDC_ACQINFO, m_acq_info);
    DDX_Control(pDX, IDC_DEBUGINFO, m_debug_info);
    DDX_Control(pDX, IDC_STUDYDATA, m_study_data);
    DDX_Control(pDX, IDC_COUCHINFO, m_couch_info);
    DDX_Control(pDX, IDC_GLBDATA, m_global_data);
    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPQMDebugWinDlg message handlers

//**************************************************************************
//Method Name   : OnInitDialogImpl
//Author        : PATNI\DKH
//Purpose       : To initialize the dialog
//**************************************************************************
BOOL CPQMDebugWinDlg::OnInitDialogImpl()
{
    SetButtonAppearence();
    SetDialogRibbonHeight();
    SetMultilingual();

    m_debug_info.SetBkColor(BACKGROUNDCOLOR);
    m_debug_info.SetTextBkColor(BACKGROUNDCOLOR);

    m_debug_info.SetTextColor(TEXTCOLOR);
    m_debug_info.SetFont(_T("Courier"), 12);
    //Set the coulmn but it wont be visible.
    CRect client_rect;
    GetClientRect(&client_rect);

    LPCTSTR l_str = _T("Debug Data");
    m_debug_info.InsertColumn(0, l_str, LVCFMT_LEFT, client_rect.Width() , -1);

    return CZOrderDialog::OnInitDialogImpl(); // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
//**************************************************************************
//Method Name   : OnCancel
//Author        : PATNI\DKH
//Purpose       : To handle on cancel event of dialog
//**************************************************************************
void CPQMDebugWinDlg::OnCancel()
{
    ShowWindow(FALSE);

}
//**************************************************************************
//Method Name   : OnGlbdata
//Author        : PATNI\DKH
//Purpose       : To handle the click event of global data button
//**************************************************************************
void CPQMDebugWinDlg::OnGlbdata()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_GLOBAL, m_buttonmask);

}


//**************************************************************************
//Method Name   : OnClear
//Author        : PATNI\DKH
//Purpose       : To handle the click event of clear button
//**************************************************************************
void CPQMDebugWinDlg::OnClear()
{
    m_debug_info.DeleteAllItems();
    //Patni-DKH/2010May07/Modified/Redmine184
    m_debuglinecount = 0;


    //Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    ClearDebugStrList();
}

//**************************************************************************
//Method Name   : OnAcqmon
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Acq monitor button
//**************************************************************************
void CPQMDebugWinDlg::OnAcqmon()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_ACQ_MONITOR, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnListcnt
//Author        : PATNI\DKH
//Purpose       : To handle the click event of List Count button
//**************************************************************************
void CPQMDebugWinDlg::OnListcnt()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_LIST_COUNT, m_buttonmask);

}
//**************************************************************************
//Method Name   : OnListarr
//Author        : PATNI\DKH
//Purpose       : To handle the click event of list Array button
//**************************************************************************
void CPQMDebugWinDlg::OnListarr()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_LIST_ARRAY, m_buttonmask);

}
//**************************************************************************
//Method Name   : OnPortinfo
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Port Info button
//**************************************************************************
void CPQMDebugWinDlg::OnPortinfo()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    //Set the selected list data and the selected count
    //which is needed to show information of selected protocols
    g_MainView->GetAcquisition()->SelectListandTotalCnt(g_MainView->GetSeletedProtocols(), g_MainView->GetTotalProtocols());
    g_MainView->StartPQMDebug(DEBUG_PROT_INFO, m_buttonmask);

}
//**************************************************************************
//Method Name   : OnCouchinfo
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Couch Info button
//**************************************************************************
void CPQMDebugWinDlg::OnCouchinfo()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_COUCH_INFO, m_buttonmask);

}
//**************************************************************************
//Method Name   : OnBtnsize
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Size button
//**************************************************************************
void CPQMDebugWinDlg::OnBtnsize()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_SIZE, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnSar
//Author        : PATNI\DKH
//Purpose       : To handle the click event of SAR button
//**************************************************************************
void CPQMDebugWinDlg::OnSar()
{
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    //Set the selected list data and the selected count
    //which is needed to show sar information of selected protocols
    g_MainView->GetAcquisition()->SelectListandTotalCnt(g_MainView->GetSeletedProtocols(), g_MainView->GetTotalProtocols());
    g_MainView->StartPQMDebug(DEBUG_SAR_INFO, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnShim
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Shim info button
//**************************************************************************
void CPQMDebugWinDlg::OnShim()
{
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_SHIMMING, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnListmon
//Author        : PATNI\DKH
//Purpose       : To handle the click event of List monitor button
//**************************************************************************
void CPQMDebugWinDlg::OnListmon()
{
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_LIST_MONITOR, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnProlimmon
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Prolim monitor button
//**************************************************************************
void CPQMDebugWinDlg::OnProlimmon()
{
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_PRO_MONITOR, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnSarmon
//Author        : PATNI\DKH
//Purpose       : To handle the click event of  SAR monitor button
//**************************************************************************
void CPQMDebugWinDlg::OnSarmon()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_SAR_MONITOR, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnIrsmon
//Author        : PATNI\DKH
//Purpose       : To handle the click event of IRS monitor button
//**************************************************************************
void CPQMDebugWinDlg::OnIrsmon()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_IRS_MONITOR, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnMonoff
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Monitor off button
//**************************************************************************
void CPQMDebugWinDlg::OnMonoff()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_MONITOR_OFF, m_buttonmask);

}
//**************************************************************************
//Method Name   : OnAcqinfo
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Acq Info button
//**************************************************************************
void CPQMDebugWinDlg::OnAcqinfo()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_RESET, m_buttonmask);
}
//**************************************************************************
//Method Name   : OnStudydata
//Author        : PATNI\DKH
//Purpose       : To handle the click event of Study data button
//**************************************************************************
void CPQMDebugWinDlg::OnStudydata()
{
    //Check Key state , if ctrl button is pressed then we have
    //to create the log file.
    m_buttonmask = 0;

    if ((GetKeyState(VK_CONTROL) < 0) && (GetKeyState(VK_MENU) < 0)) {
        m_buttonmask = CTRL_BUTTON_MASK ;
    }

    g_MainView->StartPQMDebug(DEBUG_STUDY, m_buttonmask);

}

//**************************************************************************
//**************************************************************************
//Method Name   : OnActivate
//Author        : PATNI\DKH
//Purpose       :  To handle the on Activate event
//**************************************************************************
void CPQMDebugWinDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CZOrderDialog::OnActivate(nState, pWndOther, bMinimized);

    //To ensure that the last string is visible
    int count = m_debug_info.GetItemCount();

    if (count > 0) {
        m_debug_info.EnsureVisible(count - 1, FALSE);
    }
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************

BOOL CPQMDebugWinDlg::OnSetCursor(CWnd* pWnd,
                                  UINT nHitTest,
                                  UINT message)
{
    if (m_show_wait_cursor_for_debugwindlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
//Method Name   : PreTranslateMessage
//Author        : PATNI\DKH
//Purpose       : To handle pretranslate message
//**************************************************************************
BOOL CPQMDebugWinDlg::PreTranslateMessageImpl(MSG* pMsg)
{
    if (m_show_wait_cursor_for_debugwindlg) {
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

    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
        pMsg->wParam = NULL;
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}


//+Patni-Sribanta/2011Jan12/Added/TMSC-REDMINE-1026
//****************************Method Header************************************
//Method Name   : PushMsg()
//Author        : PATNI\Sribanta
//Purpose       : Push message to list synchronizationally
//*****************************************************************************
void CPQMDebugWinDlg::PushMsg(
    const CString& f_msg
)
{
    //+Patni/KSS/2011Mar31/Modified/TMSC-REDMINE-1559
    CScopedLock l_scoped_lock(&m_cs);
    m_log_list.AddHead(f_msg);
    //-Patni/KSS/2011Mar31/Modified/TMSC-REDMINE-1559

}
//-Patni-Sribanta/2011Jan12/Modified/TMSC-REDMINE-1026


//+Patni-Sribanta/2011Jan12/Added/TMSC-REDMINE-1026
//****************************Method Header************************************
//Method Name   : PopMsg()
//Author        : PATNI\Sribanta
//Purpose       : Pop message from list synchronizationally
//*****************************************************************************
bool CPQMDebugWinDlg::PopMsg(
    CString& f_msg
)
{

    bool l_return_val = false;
    //Patni/KSS/2011Mar31/Added/TMSC-REDMINE-1559
    CScopedLock l_scoped_lock(&m_cs);

    if (!m_log_list.IsEmpty()) {

        f_msg = m_log_list.GetTail();

        if (f_msg) {
            l_return_val = true;
            m_log_list.RemoveTail();
        }
    }

    return l_return_val;
}
//-Patni-Sribanta/2011Jan12/Modified/TMSC-REDMINE-1026



//+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
//**************************************************************************
//Method Name   : UpdateDebugWnd
//Author        : PATNI\Sribanta
//Purpose       : Clear the inQueur Debug String list
//**************************************************************************
void CPQMDebugWinDlg::ClearDebugStrList(
)
{
    if (!m_log_list.IsEmpty()) {
        m_log_list.RemoveAll();
    }
}
//-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

//************************************Method Header************************************
// Method Name  : SetDialogRibbonHeight
// Author       : PATNI/RP
// Purpose      : to set the ribbon height of the dialog
//***********************************************************************************
void CPQMDebugWinDlg::SetDialogRibbonHeight(
)
{
    SetRibbonHeight(0);
}

//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/RP
// Purpose      : To set the button font
//***********************************************************************************
BOOL CPQMDebugWinDlg::SetButtonFont(
)
{
    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_global_data.SetPrimaryTextFont(button_font_name, size);
    m_global_data.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_couch_info.SetPrimaryTextFont(button_font_name, size);
    m_couch_info.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_study_data.SetPrimaryTextFont(button_font_name, size);
    m_study_data.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_close.SetPrimaryTextFont(button_font_name, size);
    m_close.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_shimming.SetPrimaryTextFont(button_font_name, size);
    m_shimming.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_sar_mon.SetPrimaryTextFont(button_font_name, size);
    m_sar_mon.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_sar.SetPrimaryTextFont(button_font_name, size);
    m_sar.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_prolim_mon.SetPrimaryTextFont(button_font_name, size);
    m_prolim_mon.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_prot_info.SetPrimaryTextFont(button_font_name, size);
    m_prot_info.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_monitor_off.SetPrimaryTextFont(button_font_name, size);
    m_monitor_off.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_list_mon.SetPrimaryTextFont(button_font_name, size);
    m_list_mon.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_list_cnt.SetPrimaryTextFont(button_font_name, size);
    m_list_cnt.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_list_array.SetPrimaryTextFont(button_font_name, size);
    m_list_array.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_irs_mon.SetPrimaryTextFont(button_font_name, size);
    m_irs_mon.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_clear.SetPrimaryTextFont(button_font_name, size);
    m_clear.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_size.SetPrimaryTextFont(button_font_name, size);
    m_size.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_acq_mon.SetPrimaryTextFont(button_font_name, size);
    m_acq_mon.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_acq_info.SetPrimaryTextFont(button_font_name, size);
    m_acq_info.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    return TRUE;
}


//************************************Method Header************************************
// Method Name  : SetButtonAppearence
// Author       : PATNI/RP
// Purpose      : to set the appearance of the buttons.
//***********************************************************************************
void CPQMDebugWinDlg::SetButtonAppearence(
)
{
    SetButtonFont();

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);

    m_listinfo.LoadDeafultStaticCtrlColorsWithBKColor(stColors_o.crClientColor);
    m_listinfo.SetFontName(_T("Arial BOLD"));
    m_listinfo.SetFontSize(90);
    m_listinfo.UpdateFont();

    m_protocoldata.LoadDeafultStaticCtrlColorsWithBKColor(stColors_o.crClientColor);
    m_protocoldata.SetFontName(_T("Arial BOLD"));
    m_protocoldata.SetFontSize(90);
    m_protocoldata.UpdateFont();

}



//**************************************************************************
//Method Name   : SetMultilingual
//Author        : PATNI\DKH
//Purpose       : To set multilingual strings of the controls
//**************************************************************************
void CPQMDebugWinDlg::SetMultilingual()
{


    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PQMDEBUGWINDOW_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    //For Global data
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_GLOBAL_DATA"));
    HWND l_chkbtn = ::GetDlgItem(m_hWnd, IDC_GLBDATA);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For study data
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_STUDY_INFO"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_STUDYDATA);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For list info
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_LIST_INFO"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_STATIC_LISTINFO);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For List count
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_LIST_COUNT"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_LISTCNT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For List Array
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_LIST_ARRAY"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_LISTARR);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For static protocol data
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PROTOCOL_DATA"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_STATIC_PROTOCOLDATA);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For prot info
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PROT_INFO"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_PORTINFO);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For couch info
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_COUCH_INFO"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_COUCHINFO);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For size
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_SIZE_DATA"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_BTNSIZE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For sar
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_SAR_DATA"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_SAR);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For Shimming
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_SHIMMING_DATA"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_SHIM);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For List Monitor
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_LIST_MONITOR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_LISTMON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For Acq monitor
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_ACQ_MONITOR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_ACQMON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For prolim monitor
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PROLIM_MONITOR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_PROLIMMON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For sar monitor
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_SAR_MONITOR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_SARMON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For IRS monitor
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_IRS_MONITOR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_IRSMON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For monitor off
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_MONITOR_OFF"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_MONOFF);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For acq info
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_ACQ_INFO"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_ACQINFO);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For clear
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_DEBUG_CLEAR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CLEAR);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For cancel
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_DEBUG_CANCEL"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;
}



