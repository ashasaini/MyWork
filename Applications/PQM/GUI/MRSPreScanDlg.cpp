//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-778
//TMSC-REDMINE-777
// MRSPreScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MRSPreScanDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "PQMView.h"
#include "PQMUtility.h"
#include "DailogInterfaces.h"
#include "DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

/////////////////////////////////////////////////////////////////////////////
// CCMRSPreScanDlg dialog
//Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
extern CPQMView* g_MainView;

#define RIBBON_GAP		8


BEGIN_MESSAGE_MAP(CMRSPreScanDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CMRSPreScanDlg)
    ON_BN_CLICKED(IDC_ABORT_BUTTON, OnAbortButton)
    ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
    //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
    ON_MESSAGE(WM_CMD_START_MRS_PRESCAN, OnStartMRSPrescan)
    ON_MESSAGE(WM_CMD_ABORT_MRS_PRESCAN, OnAbortMRSPrescan)
    ON_WM_SETCURSOR()
    //-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMRSPreScanDlg dialog

CMRSPreScanDlg::CMRSPreScanDlg(CBaseInterfaceForMRSPreScanDlg* baseinterfaceformrsprescandlg, const SMRSScan_s* mrsscan_values /*= NULL*/, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_MRSPRESCANDLG_DIALOG, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_baseinterfaceformrsprescandlg(baseinterfaceformrsprescandlg),
    m_show_wait_cursor_for_mrsprescandlg(false)
{

    if (mrsscan_values)
        SetMRSscanValues(mrsscan_values);
}


//************************************Method Header************************************
// Method Name  : ProcessMRSMessages
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlg::ProcessMRSMessages(
    const pqm_msg_tag f_msg
)
{

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::ProcessMRSMessages");

    PQM_TRACE_SCOPED(FUNC_NAME);

    switch (f_msg) {

        case PQM_MSG_MRS_LOCALSHIM_BEGIN :
            // + Patni - HAR / 2009 Mar 06 / Added / MRS

            //+Patni-Ravindra Acharya/22Nov2010/IR-168
            m_mrsitem_array[LOCAL_SHIMMING].m_item_status = STATUS_CURRENT ;

            m_mrsscan_listctrl.SetSelection(LOCAL_SHIMMING);
            m_mrsscan_listctrl.InvalidateStatus(LOCAL_SHIMMING) ;
            //-Patni-Ravindra Acharya/22Nov2010/IR-168
            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_LOCALSHIM_BEGIN Message Received"));

            break;

        case PQM_MSG_MRS_LOCALSHIM_DONE :
            //+Patni-Ravindra Acharya/22Nov2010/IR-168
            m_mrsitem_array[LOCAL_SHIMMING].m_item_status = STATUS_DONE;
            m_mrsscan_listctrl.InvalidateStatus(LOCAL_SHIMMING);
            //-Patni-Ravindra Acharya/22Nov2010/IR-168
            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_LOCALSHIM_DONE Message Received"));

            break;

        case PQM_MSG_MRS_F0_BEGIN :
            m_mrsitem_array[ADJ_CENTER_FREQ].m_item_status = STATUS_CURRENT;

            //+Patni-HEMANT/ADDED On 4/29/2009 5:06:22 PM/ IDS / Defect#291
            m_mrsscan_listctrl.SetSelection(ADJ_CENTER_FREQ);
            //-Patni-HEMANT/ADDED On 4/29/2009 5:06:22 PM/ IDS / Defect#291

            m_mrsscan_listctrl.InvalidateStatus(ADJ_CENTER_FREQ);

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_F0_BEGIN Message Received"));

            break;

        case PQM_MSG_MRS_F0_DONE :
            m_mrsitem_array[ADJ_CENTER_FREQ].m_item_status = STATUS_DONE;
            m_mrsscan_listctrl.InvalidateStatus(ADJ_CENTER_FREQ);

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_F0_DONE Message Received"));

            break;

        case PQM_MSG_MRS_REF_BEGIN :
            m_mrsitem_array[WATER_SIGNAL].m_item_status = STATUS_CURRENT;

            //+Patni-HEMANT/ADDED On 4/29/2009 5:06:22 PM/ IDS / Defect#291
            m_mrsscan_listctrl.SetSelection(WATER_SIGNAL);
            //-Patni-HEMANT/ADDED On 4/29/2009 5:06:22 PM/ IDS / Defect#291

            m_mrsscan_listctrl.InvalidateStatus(WATER_SIGNAL);

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_REF_BEGIN Message Received"));

            break ;

        case PQM_MSG_MRS_REF_DONE :
            m_mrsitem_array[WATER_SIGNAL].m_item_status = STATUS_DONE;
            m_mrsscan_listctrl.InvalidateStatus(WATER_SIGNAL);

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_REF_DONE Message Received"));

            break ;

        case PQM_MSG_MRS_WSAT_BEGIN :
            m_mrsitem_array[WATER_SATURATION].m_item_status = STATUS_CURRENT;

            //+Patni-HEMANT/ADDED On 4/29/2009 5:06:22 PM/ IDS / Defect#291
            m_mrsscan_listctrl.SetSelection(WATER_SATURATION);
            //-Patni-HEMANT/ADDED On 4/29/2009 5:06:22 PM/ IDS / Defect#291

            m_mrsscan_listctrl.InvalidateStatus(WATER_SATURATION);

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_WSAT_BEGIN Message Received"));

            break ;

        case PQM_MSG_MRS_WSAT_DONE :
            m_mrsitem_array[WATER_SATURATION].m_item_status = STATUS_DONE;
            m_mrsscan_listctrl.InvalidateStatus(WATER_SATURATION);

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_WSAT_DONE Message Received"));

            break ;

        case PQM_MSG_MRS_PRESCAN_DONE:

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("PQM_MSG_MRS_PRESCAN_DONE Message Received"));

            //+Patni-HAR-RJ/2009Mar31/Added/MRS SCAN
            if (this->GetSafeHwnd()) {
                DestroyWindow();
            }

            //-Patni-HAR-RJ/2009Mar31/Added/MRS SCAN
            break ;

        case PQM_MSG_ABORT_FAIL:
        case PQM_MSG_ABORT_WAIT:

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("Abort Message Received From AcqMan"));

            //+Patni-HAR-RJ/2009Mar31/Added/MRS SCAN
            if (this->GetSafeHwnd()) {
                DestroyWindow();
            }

            //-Patni-HAR-RJ/2009Mar31/Added/MRS SCAN
            break ;
    }
}


//+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
//************************************Method Header****************************
//Method Name	: OnStartMRSPrescan
//Author		: PATNI / HAR
//Purpose		:
//*****************************************************************************
void CMRSPreScanDlg::OnStartMRSPrescan(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::OnStartMRSPrescan");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-PJS/2010Aug26/Modified/V1.30/TMSC-REDMINE-316/Re-open
    //OnStartButton();
    StartMrsPrescan();
    //-Patni-PJS/2010Aug26/Modified/V1.30/TMSC-REDMINE-316/Re-open
}

//************************************Method Header****************************
//Method Name	: OnAbortMRSPrescan
//Author		: PATNI / HAR
//Purpose		:
//*****************************************************************************
void CMRSPreScanDlg::OnAbortMRSPrescan(
    WPARAM wParam,
    LPARAM lParam
)
{
    LPCTSTR l_log_str = _T("CMRSPreScanDlg::OnAbortMRSPrescan");
    PQM_TRACE(USER_FUNC_MARKER, l_log_str, l_log_str);
    //+Patni-PJS/2010Aug26/Modified/V1.30/TMSC-REDMINE-316/Re-open
    //OnAbortButton();
    AbortMrsPrescan();
    //-Patni-PJS/2010Aug26/Modified/V1.30/TMSC-REDMINE-316/Re-open
}
//-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316

void CMRSPreScanDlg::SetMRSscanValues(
    const SMRSScan_s& mrsscan_values
)
{
    m_mrsscan_values = mrsscan_values ;
}


void CMRSPreScanDlg::SetMrsLicensePresent(
    const bool f_mrs_flag
)
{
    m_mrsscan_listctrl.SetMrsLicensePresent(f_mrs_flag);
}
//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlg::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_mrsprescandlg = f_show_wait_cursor;

}

inline void CMRSPreScanDlg::SetMRSscanValues(
    const SMRSScan_s* mrsscan_values
)
{
    if (mrsscan_values)
        m_mrsscan_values = *mrsscan_values ;
}


void CMRSPreScanDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMRSPreScanDlg)
    DDX_Control(pDX, IDC_MRSSCAN_LISTCTRL, m_mrsscan_listctrl);
    DDX_Control(pDX, IDC_ABORT_BUTTON, m_abort_button);
    DDX_Control(pDX, IDC_START_BUTTON, m_start_button);
    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMRSPreScanDlg message handlers

void CMRSPreScanDlg::OnOK()
{
    // TODO: Add extra validation here
    //we don't call following for modeless dialog..
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::OnOK");
    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, _T("MRSPreScanDlg Closseed"));
}


void CMRSPreScanDlg::OnCancel()
{
    // - Patni + HAR / 2009 Apr 17 / Added / MRS
    /*   m_baseinterfaceformrsprescandlg->MrsScanAbort();
       //+Patni-HAR-RJ/2009Mar31/Added/MRS SCAN
       if(this->GetSafeHwnd()){
       	DestroyWindow();
       }
       ///*CDialogDialogEx::OnCancel();*/
    //-Patni-HAR-RJ/2009Apr17/Added/MRS SCAN

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::OnCancel");
    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, _T("MRSPreScanDlg Closseed"));
}



BOOL CMRSPreScanDlg::OnInitDialogImpl()
{

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    SetDialogRibbonHeight();

    //Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
    LogMRSScanValues(&m_mrsscan_values);

    m_mrsitem_array[ADJ_CENTER_FREQ].SetMRSScanItemData(STATUS_WAIT, m_mrsscan_values.f0 ? true : false, _T("Adjust Center Frequency"), m_mrsscan_values.f0_change_allow);
    m_mrsitem_array[WATER_SIGNAL].SetMRSScanItemData(STATUS_WAIT, m_mrsscan_values.ref ? true : false, _T("Water Signal Reference"), m_mrsscan_values.ref_change_allow);
    m_mrsitem_array[WATER_SATURATION].SetMRSScanItemData(STATUS_WAIT, m_mrsscan_values.wsat ? true : false, _T("Auto Water-Sat Optimization"), m_mrsscan_values.wsat_change_allow);

    if (m_mrsscan_listctrl.IsMrsLicensePresent()) {

        //Patni-Ravindra Acharya/22Nov2010/Added/IR-168
        m_mrsitem_array[LOCAL_SHIMMING].SetMRSScanItemData(STATUS_WAIT, m_mrsscan_values.shim ? true : false, _T("Local Shimming"), m_mrsscan_values.shim_change_allow);
        m_mrsitem_array[SPECTRUM_GRAPH].SetMRSScanItemData(STATUS_WAIT, m_mrsscan_values.spec_graph ? true : false, _T("Manual Shimming"), m_mrsscan_values.spec_graph_change_allow);
    }

    InitializePQMListCtrl();

    SetMultilingual();//Language usha
    ShowCloseButton(false); //Patni-HAR/2009Mar05/Commented/PSP1- MRS Close Button Defect

    return CZOrderDialog::OnInitDialogImpl(false); // return TRUE  unless you set the focus to a control
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlg::OnAbortButton(
)
{
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::OnAbortButton");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
    if (S_OK == g_MainView->GetAcquisition()->RemoteCheckActiveFlag()) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_ABORT_MRS_PRESCAN,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PQM_ABORT_MRS_PRESCAN")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");
        return;
    }

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections


    //-Patni-HAR-RJ/2009Mar31/Added/MRS SCAN
    AbortMrsPrescan();
    //-Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlg::OnStartButton(
)
{
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::OnStartButton");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
    if (S_OK == g_MainView->GetAcquisition()->RemoteCheckActiveFlag()) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_MRS_PRESCAN_START,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PQM_MRS_PRESCAN_START")),
                SCAN_ERR_NOTIFY, L"CPQMView.cpp");
        return;
    }

    //+Patni-RSG/ADDED/2009-May-15/JFT#268

    StartMrsPrescan();
    //-Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************
BOOL CMRSPreScanDlg::OnSetCursor(CWnd* pWnd,
                                 UINT nHitTest,
                                 UINT message)
{
    if (m_show_wait_cursor_for_mrsprescandlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}
//************************************Method Header************************************
// Method Name  :PreTranslateMessageImpl
// Author       :iGATE
// Purpose      :
//***********************************************************************************
BOOL CMRSPreScanDlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_cursor_for_mrsprescandlg) {

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

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}

//+Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
void CMRSPreScanDlg::LogMRSScanValues(
    const SMRSScan_s* f_mrsscan_values
)const
{

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::LogMRSScanValues");

    if (!f_mrsscan_values) {
        return ;
    }

    //+Patni-Ravindra Acharya/22Nov2010/Added/IR-168
    CString l_log_str = f_mrsscan_values->shim ? _T("Local Shimming = ON, ") : _T("Local Shimming = OFF, ");
    l_log_str = l_log_str + (f_mrsscan_values->shim_change_allow ? _T("Enabled") : _T("Disabled"));

    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, l_log_str);

    //-Patni-Ravindra Acharya/22Nov2010/Added/IR-168
    l_log_str = f_mrsscan_values->f0 ? _T("Adjust Center Frequency = ON, ") : _T("Adjust Center Frequency = OFF, ");
    l_log_str = l_log_str + (f_mrsscan_values->f0_change_allow ? _T("Enabled") : _T("Disabled"));

    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, l_log_str);

    //+Patni-Ravindra Acharya/22Nov2010/Added/IR-168

    l_log_str = f_mrsscan_values->spec_graph ? _T("Show Spectrum Graph = ON, ") : _T("Show Spectrum Graph = OFF, ");
    l_log_str = l_log_str + (f_mrsscan_values->spec_graph_change_allow ? _T("Enabled") : _T("Disabled"));

    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, l_log_str);

    //-Patni-Ravindra Acharya/22Nov2010/Added/IR-168

    l_log_str = f_mrsscan_values->ref ? _T("Water Signal Reference = ON, ") : _T("Water Signal Reference = OFF, ");
    l_log_str = l_log_str + (f_mrsscan_values->ref_change_allow ? _T("Enabled") : _T("Disabled"));

    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, l_log_str);

    l_log_str = f_mrsscan_values->wsat ? _T("Auto Water-Sat Optimization = ON, ") : _T("Auto Water-Sat Optimization = OFF, ");
    l_log_str = l_log_str + (f_mrsscan_values->wsat_change_allow ? _T("Enabled") : _T("Disabled"));

    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME, l_log_str);
}
//-Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2



//+Patni-PJS/2010Apr28/Added/JaFT# Redmine-90
//*****************************************************************************
//Method Name   : AbortMrsPrescan
//Author        : Patni/PJS
//Purpose       : This function is used to Abort MRS Prescan
//*****************************************************************************
void CMRSPreScanDlg::AbortMrsPrescan(
)
{
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::AbortMrsPrescan");

    //Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (NULL == m_baseinterfaceformrsprescandlg) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_baseinterfaceformrsprescandlg pointer is NULL"));

        return;
    }

    if (CanAbortMRS()) {
        m_baseinterfaceformrsprescandlg->MrsScanAbort();

    } else {
        m_baseinterfaceformrsprescandlg->MrsScanClose();
    }

    if (this->GetSafeHwnd()) {
        DestroyWindow();
    }
}

//*****************************************************************************
//Method Name   : CanAbortMRS
//Author        : Patni/PJS
//Purpose       : This function is used to Abort MRS Prescan
//*****************************************************************************
bool CMRSPreScanDlg::CanAbortMRS()
{

    return !(((CWnd*)GetDlgItem(IDC_START_BUTTON))->IsWindowEnabled());
}


//*****************************************************************************
//Method Name   : StartMrsPrescan
//Author        : Patni/PJS
//Purpose       : This function is used to start MRS Prescan
//*****************************************************************************
void CMRSPreScanDlg::StartMrsPrescan(
)
{
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::StartMrsPrescan");

    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!m_start_button.IsWindowEnabled()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Start button Disbaled"));
        return;
    }

    m_mrsscan_listctrl.DisableMRSScanListCtrl();
    m_mrsscan_listctrl.UpdateData();
    m_start_button.EnableWindow(FALSE);
    SMRSScan_s* mrsscan_values = new SMRSScan_s;


    MRSScanItem* mrsscanitem;

    //+Patni-Ravindra Acharya/22Nov2010/Added/IR-168
    mrsscanitem = m_mrsscan_listctrl.GetMRSScanItem(ADJ_CENTER_FREQ) ;
    mrsscan_values->f0 = (int) mrsscanitem->m_checked_status ;
    //-Patni-Ravindra Acharya/22Nov2010/Added/IR-168

    mrsscanitem = m_mrsscan_listctrl.GetMRSScanItem(WATER_SIGNAL);
    mrsscan_values->ref = (int) mrsscanitem->m_checked_status;

    mrsscanitem = m_mrsscan_listctrl.GetMRSScanItem(WATER_SATURATION);
    mrsscan_values->wsat = (int) mrsscanitem->m_checked_status;

    if (m_mrsscan_listctrl.IsMrsLicensePresent()) {
        //+Patni-Ravindra Acharya/22Nov2010/Modified/IR-168
        mrsscanitem = m_mrsscan_listctrl.GetMRSScanItem(LOCAL_SHIMMING);
        mrsscan_values->shim = (int) mrsscanitem->m_checked_status;

        mrsscanitem = m_mrsscan_listctrl.GetMRSScanItem(SPECTRUM_GRAPH) ;
        mrsscan_values->spec_graph = (int) mrsscanitem->m_checked_status ;
        //-Patni-Ravindra Acharya/22Nov2010/Added/IR-168

    } else {

        mrsscan_values->shim = 1;
        mrsscan_values->spec_graph =  0;
    }

    //+Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2
    PQM_TRACE(USER_FUNC_MARKER,
              FUNC_NAME,
              _T("Printing Mrsscan values Before Scan Start :"));

    LogMRSScanValues(mrsscan_values);
    //-Patni-Hemant/2010Oct27/Added/REDMINE-778_Part2

    if (NULL == m_baseinterfaceformrsprescandlg) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_baseinterfaceformrsprescandlg pointer is NULL"));

        return;
    }

    m_baseinterfaceformrsprescandlg->MrsScanStartUiCB(mrsscan_values) ;
    m_mrsscan_listctrl.SetFocus();
}
//-Patni-PJS/2010Apr28/Added/JaFT# Redmine-90



//************************************Method Header************************************
// Method Name  : InitializePQMListCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CMRSPreScanDlg::InitializePQMListCtrl(
)
{
    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlg::InitializePQMListCtrl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    m_mrsscan_listctrl.InsertColumns();
    m_mrsscan_listctrl.SetMRSScanData(m_mrsitem_array);
    m_mrsscan_listctrl.InsertControls();

    return TRUE ;
}


//Language usha
void CMRSPreScanDlg::SetMultilingual()
{
    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_MRSPRESCANDLG_DIALOG"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    // other controls

    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ABORT_BUTTON"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_ABORT_BUTTON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_START_BUTTON"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_START_BUTTON);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlg::SetDialogRibbonHeight(
)
{

    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_start_button.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}