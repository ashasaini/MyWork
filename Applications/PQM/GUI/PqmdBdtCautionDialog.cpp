//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-777

// PqmdBdtCautionDialog.cpp : implementation file
//
#include "stdafx.h"
#include "PqmdBdtCautionDialog.h"
#include "PQMView.h"
#include "DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CPQMView* g_MainView;


BEGIN_MESSAGE_MAP(CPqmdBdtCautionDialog, DialogEx)
    //{{AFX_MSG_MAP(CPqmdBdtCautionDialog)
    //Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPqmdBdtCautionDialog dialog


CPqmdBdtCautionDialog::CPqmdBdtCautionDialog(CWnd* pParent /*=NULL*/)
    : DialogEx(IDD_CAUTION_DBDT, pParent),
      m_fail_cnt(0)
{
    //{{AFX_DATA_INIT(CPqmdBdtCautionDialog)
    //}}AFX_DATA_INIT
}


void CPqmdBdtCautionDialog::DoDataExchange(CDataExchange* pDX)
{
    DialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPqmdBdtCautionDialog)
    DDX_Control(pDX, IDC_STATIC_DBDT_PWD, m_dbdt_pwd);
    DDX_Control(pDX, IDC_STATIC_DBDT, m_dbtdt_text);
    DDX_Control(pDX, IDC_ERROR_DBDT, m_error);
    DDX_Control(pDX, IDC_PASSWD, m_passwd);
    DDX_Control(pDX, IDCANCEL, m_cancel);
    DDX_Control(pDX, IDOK, m_ok);
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPqmdBdtCautionDialog message handlers

void CPqmdBdtCautionDialog::OnOK()
{
    LPCTSTR FUNC_NAME = _T("CPqmdBdtCautionDialog::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    // TODO: Add extra validation here
    g_MainView->SetDBDtCancelFlag(false);

    CString m_password = _T("");
    GetDlgItemText(IDC_PASSWD, m_password);

    //+Patni-PJS/2010Jul22/Added/TMSC-REDMINE-491
    if (0 == m_dbdt_auth_passwd.AuthenticateUserPassword(m_password)) {
        //+Patni-PJS/2010Aug17/Modified/REDMINE-562
        if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
            EndModalLoop(IDOK);
        }

        //DialogEx::OnOK();
        //-Patni-PJS/2010Aug17/Modified/REDMINE-562
        //+Patni-PJS/2010Jun30/Added/TMSC-REDMINE-392

    } else if (m_fail_cnt >= 3) {
        OnCancel();

    } else {
        m_fail_cnt++;
        ((CPQMEdit*)GetDlgItem(IDC_PASSWD))->SetWindowText(_T(""));

        ((CPQMEdit*)GetDlgItem(IDC_ERROR))->SetWindowText(
            CPQMUtility::GetMultiLingualString(_T("IDS_INVALID_PASSWORD")));

        ((CPQMEdit*)GetDlgItem(IDC_PASSWD))->SetFocus();
    }
}

void CPqmdBdtCautionDialog::OnCancel()
{
    LPCTSTR FUNC_NAME = _T("CPqmdBdtCautionDialog::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    // TODO: Add extra cleanup here
    g_MainView->SetDBDtCancelFlag(true);

    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    //DialogEx::OnCancel();
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}

BOOL CPqmdBdtCautionDialog::OnInitDialog()
{
    LPCTSTR FUNC_NAME = _T("CPqmdBdtCautionDialog::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    DialogEx::OnInitDialog();

    ((CPQMEdit*)GetDlgItem(IDC_PASSWD))->SetFocus();
    //+Patni-PJS/2010Jul21/Added/TMSC-REDMINE-392
    LPCTSTR l_edit_ctrl_font = DLG_TITLE_FONT_NAME;
    const int l_edit_ctrl_text_size = DLG_TITLE_FONT_SIZE;
    ((CPQMEdit*)GetDlgItem(IDC_PASSWD))->SetFont(l_edit_ctrl_font, l_edit_ctrl_text_size);
    ((CPQMEdit*)GetDlgItem(IDC_PASSWD))->SetTextAlignment(VCENTER);
    //-Patni-PJS/2010Jul21/Added/TMSC-REDMINE-392

    //+Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2
    SetFont(_T("Arial"), 15);
    Invalidate() ;
    UpdateWindow();
    //-Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2

    SetMultilingual(); // Added By KT/ACE-2/11-06-2009/Multilingualisation

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    m_dbtdt_text.SetWindowText(CPQMUtility::GetMultiLingualString(_T("IDS_DBDT_DIALOG_STRING1")));

    m_dbdt_pwd.LoadDeafultStaticCtrlColorsWithBKColor(BK_COLOR);
    m_dbtdt_text.LoadDeafultStaticCtrlColorsWithBKColor(BK_COLOR);
    m_error.LoadDeafultStaticCtrlColorsWithBKColor(BK_COLOR);
    m_bkbrush.CreateSolidBrush(BK_COLOR);

    m_dbtdt_text.SetFontAttributes(_T("Arial"), 150);
    m_dbtdt_text.UpdateFont();
    m_dbdt_pwd.SetFontAttributes(_T("Arial"), 150);
    m_dbdt_pwd.UpdateFont();
    m_error.SetFontAttributes(_T("Arial"), 150);
    m_error.UpdateFont();
    //-Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2

    LPCTSTR button_font_name = _T("Arial") ;
    const int size = 15;
    //-Patni-PJS/2010July1/Added/TMSC-REDMINE-392
    m_ok.SetCurveType(CURVE_LARGE_5);
    m_ok.SetPrimaryTextFont(button_font_name, size);
    m_ok.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_cancel.SetCurveType(CURVE_LARGE_5);
    m_cancel.SetPrimaryTextFont(button_font_name, size);
    m_cancel.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    //-Patni-HEMANT/ADDED On 8/31/2009 11:34:30 AM/ MVC#4423

    m_fail_cnt = 0;
    CWnd::SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    SetDialogRibbonHeight();
    return TRUE;

}
//+Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2
//************************************Method Header************************************
// Method Name  : OnEraseBkgnd
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
BOOL CPqmdBdtCautionDialog::OnEraseBkgnd(
    CDC* pDC
)
{

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    CBrush l_new_rib_brush;
    l_new_rib_brush.CreateSolidBrush(bk_color);

    CBrush* old_brush = pDC->SelectObject(&m_bkbrush);
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_ok.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    const int height_diff = client_rect.bottom - ok_button_rect.top + 20;
    client_rect.bottom = client_rect.bottom -  height_diff;
    pDC->Rectangle(&client_rect);
    pDC->SelectObject(old_brush);

    GetClientRect(&client_rect);
    client_rect.top = client_rect.bottom -  height_diff;;
    CBrush* l_old_rib_brush = pDC->SelectObject(&l_new_rib_brush);
    pDC->Rectangle(&client_rect);
    pDC->SelectObject(l_old_rib_brush);

    //+GDI Object leak removal
    if (l_new_rib_brush.m_hObject) {
        l_new_rib_brush.DeleteObject();
    }

    //-GDI Object leak removal

    return TRUE;
}
//-Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2


//+Patni-PJS/2010Apr05/Added/MSA0251-00280 review comment
//**************************************************************************
//Method Name   :  OnDestroy
//Author        :  PATNI/PJS
//Purpose       :  This function is used to destroy the dialog and unregister
//				   the z-order window
//**************************************************************************
void CPqmdBdtCautionDialog::OnDestroy()
{
    LPCTSTR FUNC_NAME = _T("CPqmdBdtCautionDialog::OnDestroyImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+GDI Object leak removal
    if (m_bkbrush.m_hObject) {
        m_bkbrush.DeleteObject();
    }

    //-GDI Object leak removal
    DialogEx::OnDestroy();
}
//-Patni-PJS/2010Apr05/Added/MSA0251-00280 review comment

// Added By KT/ACE-2/11-06-2009/Multilingualisation
void CPqmdBdtCautionDialog::SetMultilingual()
{

    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_DBDTCAUTION_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_STATIC_DBDT_PWD"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_DBDT_PWD);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_OK"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CANCEL_BUTTON"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    //Patni-NITIN/2009AUG29/ADDED/MVC04403

    //+Patni-NP/2010Mar5/Commented/ML
    CString str1;
    str1 = CPQMUtility::GetMultiLingualString(_T("IDS_DBDT_DIALOG_STRING1")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
    /*  str2 = CPQMUtility::GetMultiLingualString(_T("IDS_DBDT_DIALOG_STRING2")); // Added By KT/ACE-2/15-06-2009/Multilingualisation*/
    l_chOutput = str1;
    //-Patni-NP/2010Mar5/Commented/ML


    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_DBDT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;
}
// Added By KT/ACE-2/11-06-2009/Multilingualisation


//+Patni-HEMANT/ADDED On 8/31/2009 11:34:30 AM/ MVC#4423
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqmdBdtCautionDialog::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_ok.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 20);
}
//-Patni-HEMANT/ADDED On 8/31/2009 11:34:30 AM/ MVC#4423


