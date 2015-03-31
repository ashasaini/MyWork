//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMCommentDlg.cpp
 *  Overview: Implementation of CPQMCommentDlg class.
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
#include "PQMCommentDlg.h"
#include <MPlusUICommon/PopupMenuEx.h> //Patni-AJS/2010Sep13/Modified/ScanCommentDialog/IR-163
#include <libStudyManager/constants.h> //Tanoue/MVC5849
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <imm.h> //Tanoue/MVC5849
#include <PQM/resource.h>	//added by Hemant
#include "PQMView.h"
#include "PQMUtility.h"	// Added By KT/ACE-2/11-06-2009/Multilingualisation		
#include "DPSManager.h"
#include "ScanCommGeneration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Patni-PJS/2009Nov11/Modified/MSA00248-0033/As per review comments
using namespace MR::ACQ::SM::COMMON; //Tanoue/MVC5849
using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

extern CPQMView* g_MainView;

BEGIN_MESSAGE_MAP(CPQMCommentDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMCommentDlg)
    //+Patni-NP/2010Jan18/Modified/MVC004687
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    //-Patni-SG/2009Mar9/ADDED/PSP1 Z Order implementation
    //Patni-AJS/2010Sep13/Added/ScanCommentDialog/IR-163
    ON_COMMAND(ID_COPY_TO_COMMENT, OnCopyToComment)

    ON_WM_SETCURSOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//+Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163
//CPQMCommentDlg::CPQMCommentDlg(CBaseInterfaceClassForScanCommentDlg* f_interfaceclassforscancommentdlg, CWnd* pParent /*=NULL*/)
CPQMCommentDlg::CPQMCommentDlg(
    CBaseInterfaceClassForScanCommentDlg* f_interfaceclassforscancommentdlg,
    ScanCommentXMLData* f_scan_dialog_data ,
    CWnd* pParent /*=NULL*/)
//-Patni-AJS/2010Sep3/Modified/ScanCommentDialog/IR-163
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_COMMENT, pParent),
    m_scancomm_generation(NULL),
    m_scan_dialog_data(f_scan_dialog_data),
    m_store_state_for_scan_comm_gen(false),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_interfaceclassforscancommentdlg(f_interfaceclassforscancommentdlg),
    m_isJpInputEnter(false),
    m_show_wait_cursor_for_commentdlg(false)
{
    //{{AFX_DATA_INIT(CPQMCommentDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}

//+Patni-Hemant/2010Dec09/Added/IR-163 is re-open
CPQMCommentDlg::~CPQMCommentDlg()
{
    DEL_PTR(m_scancomm_generation);
}
//-Patni-Hemant/2010Dec09/Added/IR-163 is re-open


//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
void CPQMCommentDlg::UpdateAndScanComment(
    const CString& f_str
)
{

    if (f_str.GetLength() > SCAN_COMMENT_LENGTH) {

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPQMUtility::GetMultiLingualString(_T("IDS_SCAN_COMMENT_LENGTH_NOT_ALLOWED")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));

        return ;
    }

    m_c_scan_comment.SetWindowText(f_str);

    SaveScanComment();
    DisplayCommentInformation();

}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
void CPQMCommentDlg::ChangeViewOfCoilListDlg(
    const bool f_check
)const
{

    if (m_scancomm_generation && ::IsWindow(m_scancomm_generation->m_hWnd)) {
        m_scancomm_generation->EnableWindow(f_check);
    }
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163


//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
void CPQMCommentDlg::ShowHideOpenedWindowsOnPageChanged(
    const bool f_show /*= true*/
)
{
    if (m_scancomm_generation) {

        if (::IsWindow(m_scancomm_generation->m_hWnd)) {
            if (!f_show) {

                //Modified by Hemant On 3/5/2009 2:50:59 PM
                //m_store_visible_state_for_coil_list = m_scancomm_generation->IsWindowVisible();
                m_store_state_for_scan_comm_gen = m_scancomm_generation->IsWindowVisible() ? true : false;
            }

            m_scancomm_generation->ShowWindow((f_show && m_store_state_for_scan_comm_gen) ? SW_SHOW : SW_HIDE);
        }
    }
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163



//****************************Method Header************************************
//Method Name   :DisplayCommentInformation
//Author        :PATNI/GP
//Purpose       :Display Comment Information
//*****************************************************************************
void CPQMCommentDlg::DisplayCommentInformation()
{
    //Added by Hemant On 11/14/2008 12:20:31 PM
    UpdateScanCommentInformation();
}

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMCommentDlg::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_commentdlg = f_show_wait_cursor;
}


void CPQMCommentDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CPQMCommentDlg)
    DDX_Control(pDX, IDC_STATIC_LEBEL_ID, m_label_id);
    DDX_Control(pDX, IDC_STATIC_LABEL_COMMENT, m_label_comment);
    DDX_Control(pDX, IDC_EDIT_COMMENT, m_c_scan_comment);
    DDX_Control(pDX, IDC_EDIT_ID, m_c_scan_id);
    DDX_Control(pDX, IDCANCEL, m_close);
    DDX_Control(pDX, IDOK, m_ok);  //Patni//Manishkumar//2009May15//JFT-CR-41
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPQMCommentDlg message handlers
/*************************************************************************************

//Module:  OnInitDialogImpl

//Author:  PATNI/SVP

//Purpose: Read selected protocol comment data and populates on dialog box.

//**************************************************************************************/
BOOL CPQMCommentDlg::OnInitDialogImpl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCommentDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    ((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetLimitText(SCAN_ID_LENGTH);
    ((CEdit*)GetDlgItem(IDC_EDIT_COMMENT))->SetLimitText(SCAN_COMMENT_LENGTH);

    SetButtonAppearence();


    ((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetFocus();

    SetMultilingual();
    return CZOrderDialog::OnInitDialogImpl(false);
}


/*************************************************************************************

//Module:  OnCancel

//Author:  PATNI/SVP

//Purpose: Discard changes made in comment for selected protocol.

//**************************************************************************************/
void CPQMCommentDlg::OnCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCommentDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_COMMENT_CLOSE);

    //+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    if (m_scancomm_generation && m_scancomm_generation->GetSafeHwnd() != NULL) {
        m_scancomm_generation->DestroyWindow();
    }

    //-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    //+Patni-AMT/2009May12/Added/JFT1 PSP#371
    if (this->GetSafeHwnd()) {
        DestroyWindow();
    }

    //-Patni-AMT/2009May12/Added/JFT1 PSP#371
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCommentDlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_cursor_for_commentdlg) {
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

    // +Tanoue/MVC5849/Japanese Input
    // Jp input step is :
    // 1. input Hiragana. 2. convert to Kanji by Space Key. 3. fix conversion by Enter Key.
    // 4. repleat 1-3. 5. fix input.
    // When click Enter Key on PQM Scan ID/Comment, these are saved and focus is changed,
    // but step 3 Enter Key should be avoid to save and change focus.
    // m_isJpInputEnter is usded for this trick.
    if ((pMsg->lParam & GCS_RESULTSTR) && (pMsg->message == WM_IME_COMPOSITION)) {
        m_isJpInputEnter = true;
        return CZOrderDialog::PreTranslateMessageImpl(pMsg);
    }

    // -Tanoue/MVC5849/Japanese Input
    if (pMsg->wParam == VK_RETURN) {
        // +Tanoue/MVC5849/Japanese Input
        if (m_isJpInputEnter) {
            m_isJpInputEnter = false;
            return TRUE;
        }

        // -Tanoue/MVC5849/Japanese Input

        if (pMsg->hwnd == m_c_scan_comment.m_hWnd) {

            //+Patni//Manishkumar//2009May15//JFT-CR-41
            SaveScanComment();
            DisplayCommentInformation();
            //-Patni//Manishkumar//2009May15//JFT-CR-41

            return TRUE;

        } else if (pMsg->hwnd == m_c_scan_id.m_hWnd) {

            //+Patni//Manishkumar//2009May15//JFT-CR-41
            SaveScanId();
            DisplayCommentInformation();

            CString scan_id ;
            m_c_scan_id.GetWindowText(scan_id);

            if (!scan_id.IsEmpty())
                m_c_scan_comment.SetFocus();

            //-Patni//Manishkumar//2009May15//JFT-CR-41

            return TRUE;
        }
    }

    //+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    if ((pMsg->hwnd == m_c_scan_comment.m_hWnd)
        && (pMsg->message == WM_RBUTTONDOWN)) {

        LoadScanCommentDialog();
        return TRUE;
    }

    //-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

    //+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    if ((pMsg->hwnd == m_c_scan_id.m_hWnd)
        && (pMsg->message == WM_RBUTTONDOWN)) {

        OnScanIdEditRButtonDown();
        return TRUE;
    }

    //-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163


    //+Patni-NP/2010Jan18/Modified/MVC004687
    if (pMsg->message == WM_RBUTTONDOWN) {
        return 1;
    }

    if (pMsg->message == WM_RBUTTONUP) {
        return 1;
    }

    if (pMsg->message == WM_RBUTTONDBLCLK) {
        return 1;
    }

    //-Patni-NP/2010Jan18/Modified/MVC004687

    // - SM4 Defect#742 AMIT-HIMANSHU 14-Jan-09

    //+Patni-KSS/2010Jul20/ADDED/TMSC-REDMINE-453
    if (pMsg->message == WM_CHAR || pMsg->wParam == VK_DELETE) {
        if ((pMsg->wParam == BACKSLASH) || !(m_interfaceclassforscancommentdlg->CanModifyWithCommentDialog())) {
            pMsg->wParam = NULL;
        }
    }

    //-Patni-KSS/2010Jul20/ADDED/TMSC-REDMINE-453

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}

//+Patni//Manishkumar//2009May15//JFT-CR-41
void CPQMCommentDlg::OnOK()
{
    LPCTSTR FUNC_NAME = _T("CPQMCommentDlg::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    if (m_scancomm_generation && m_scancomm_generation->GetSafeHwnd() != NULL) {
        m_scancomm_generation->DestroyWindow();
    }

    //-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    SaveScanId();
    SaveScanComment();
    DisplayCommentInformation();

    //+Patni-SS/ADDED/2009-May-22/ PQM Crash on Comment dialog
    if (this->GetSafeHwnd()) {

        DestroyWindow();
    }

}
//-Patni//Manishkumar//2009May15//JFT-CR-41
void CPQMCommentDlg::OnDestroyImpl()
{
    LPCTSTR FUNC_NAME = _T("CPQMCommentDlg::OnDestroyImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    DEL_PTR(m_scancomm_generation)

    CZOrderDialog::OnDestroyImpl();
}


//+Patni-AJS/2010Sep13/Added/ScanCommentDialog/IR-163
void CPQMCommentDlg::OnCopyToComment(
)
{
    CString l_scan_id = _T("");
    m_c_scan_id.GetWindowText(l_scan_id);
    m_c_scan_comment.SetWindowText(l_scan_id);
}
//-Patni-AJS/2010Sep13/Added/ScanCommentDialog/IR-163
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMCommentDlg::OnSetCursor(CWnd* pWnd,
                                 UINT nHitTest,
                                 UINT message)
{
    if (m_show_wait_cursor_for_commentdlg) {

        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCommentDlg::UpdateScanCommentInformation()
{

    if (m_interfaceclassforscancommentdlg) {
        //+Patni-/2010May/Modified/Code Review
        CString l_scan_id = _T("");
        CString l_scan_comment = _T("");
        GetCurrentScanCommentAndScanID(l_scan_id, l_scan_comment);

        int index = g_MainView->GetSelectedSeqIndex();

        if (index != -1) {
            if (g_MainView->GetScanIndex() == index) {
                // Replace & with && in scan id and comment string to display ampersand(&).
                l_scan_id.Replace(_T("&"), _T("&&"));
                l_scan_comment.Replace(_T("&"), _T("&&"));
            }
        }

        ((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetWindowText(l_scan_id);
        ((CEdit*)GetDlgItem(IDC_EDIT_COMMENT))->SetWindowText(l_scan_comment);
        //-Patni-/2010May/Modified/Code Review

        EnableDisableScanCommentCtrls();
    }
}


//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
void CPQMCommentDlg::LoadScanCommentDialog()
{

    if ((0 >= m_scan_dialog_data->m_seq_comment_list.GetCount()) &&
        (0 >= m_scan_dialog_data->m_cont_comment_list.GetCount()) &&
        (0 >= m_scan_dialog_data->m_opt_comment_list.GetCount())
       ) {
        return ;
    }

    CString l_comm_str = _T("");
    GetDlgItemText(IDC_EDIT_COMMENT, l_comm_str);

    if (NULL == m_scancomm_generation || NULL == m_scancomm_generation->GetSafeHwnd()) {
        DEL_PTR(m_scancomm_generation)
        m_scancomm_generation = new CScanCommGeneration(this, m_scan_dialog_data);
        m_scancomm_generation->Create(IDD_SCAN_COMMENT_XML_DIALOG);
    }

    m_scancomm_generation->ShowWindow(SW_SHOW);

    m_scancomm_generation->RefreshDialogCtrls(l_comm_str);
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163


//+Patni-AJS/2010Sep13/Added/ScanCommentDialog/IR-163
void CPQMCommentDlg::OnScanIdEditRButtonDown(
)
{
    LPCTSTR l_font_name = _T("Trebuchet MS") ;
    const int font_size = 10;

    PopupMenuEx l_context_menu;
    l_context_menu.LoadMenu(IDR_SCAN_ID_MENU);

    l_context_menu.SetFont(l_font_name, font_size);

    CRect l_rect ;
    m_c_scan_id.GetWindowRect(&l_rect);

    l_context_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, l_rect.left,
                                  l_rect.bottom, this, 0, 0);

}
//-Patni-AJS/2010Sep13/Added/ScanCommentDialog/IR-163


void CPQMCommentDlg::EnableDisableScanCommentCtrls(
)const
{

    if (m_interfaceclassforscancommentdlg) {

        const bool enable = m_interfaceclassforscancommentdlg->CanModifyWithCommentDialog() ;
        ((CEdit*)GetDlgItem(IDC_EDIT_ID))->EnableWindow(enable);
        ((CEdit*)GetDlgItem(IDC_EDIT_COMMENT))->EnableWindow(enable);

        ((CEdit*)GetDlgItem(IDOK))->EnableWindow(enable);

        if (m_scancomm_generation) {
            m_scancomm_generation->EnableDisableApply(enable);
        }
    }
}


//+Patni//Manishkumar//2009May15//JFT-CR-41
void CPQMCommentDlg::SaveScanId(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMCommentDlg::SaveScanId");

    CString f_scan_id_old = _T("");
    CString f_scan_comment_old = _T("");
    GetCurrentScanCommentAndScanID(f_scan_id_old, f_scan_comment_old);

    CString scan_id = _T("");
    m_c_scan_id.GetWindowText(scan_id);
    scan_id.TrimLeft();
    scan_id.TrimRight();

    //+Patni-PJS/2010Aug02/Modified/TMSC-REDMINE-533
    //if (f_scan_id_old.CompareNoCase(scan_id) == 0)
    if (f_scan_id_old.Compare(scan_id) == 0) {
        return ;
    }

    //-Patni-PJS/2010Aug02/Modified/TMSC-REDMINE-533

    //Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL != m_interfaceclassforscancommentdlg) {
        m_interfaceclassforscancommentdlg->SetCurrentScanCommentAndScanID(&scan_id, NULL);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_interfaceclassforscancommentdlg pointer is NULL"));
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
}
//-Patni//Manishkumar//2009May15//JFT-CR-41



//+Patni//Manishkumar//2009May15//JFT-CR-41
void CPQMCommentDlg::SaveScanComment(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMCommentDlg::SaveScanComment");

    CString f_scan_id_old = _T("");
    CString f_scan_comment_old = _T("");
    GetCurrentScanCommentAndScanID(f_scan_id_old, f_scan_comment_old);

    CString scan_comment = _T("");
    m_c_scan_comment.GetWindowText(scan_comment);
    scan_comment.TrimLeft();
    scan_comment.TrimRight();

    //+ MEITEC/2010Aug06/Modified/TMSC-REDMINE-533
    //if (f_scan_comment_old.CollateNoCase(scan_comment) == 0)
    if (f_scan_comment_old.Collate(scan_comment) == 0) {
        return ;
    }

    //- MEITEC/2010Aug06/Modified/TMSC-REDMINE-533

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL != m_interfaceclassforscancommentdlg) {
        m_interfaceclassforscancommentdlg->SetCurrentScanCommentAndScanID(NULL, &scan_comment);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_interfaceclassforscancommentdlg pointer is NULL"));

    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
}
//-Patni//Manishkumar//2009May15//JFT-CR-41

//Language usha
void CPQMCommentDlg::SetMultilingual()
{
    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COMMENT_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_STATIC_LEBEL_ID"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_LEBEL_ID);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_STATIC_LABEL_COMMENT"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_LABEL_COMMENT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COMMENTDLG_CANCEL"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    //+Patni//Manishkumar//2009May15//JFT-CR-41
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_OK"));      // Added By KT/ACE-2/11-06-2009/Multilingualisation
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;
    //-Patni//Manishkumar//2009May15//JFT-CR-41
}




//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCommentDlg::SetButtonFont(
)
{
    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_c_scan_id.SetFont(button_font_name, size);
    m_c_scan_comment.SetFont(button_font_name, size);

    m_close.SetPrimaryTextFont(button_font_name, size);
    m_close.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    //+Patni//Manishkumar//2009May15//JFT-CR-41
    m_ok.SetPrimaryTextFont(button_font_name, size);
    m_ok.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    //-Patni//Manishkumar//2009May15//JFT-CR-41
    return TRUE ;
}



//************************************Method Header************************************
// Method Name  : SetButtonAppearence
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCommentDlg::SetButtonAppearence(
)
{
    SetButtonFont();
    SetDialogRibbonHeight();
    m_close.SetCurveType(CURVE_LARGE_5);

    m_ok.SetCurveType(CURVE_LARGE_5);

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    m_label_id.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_label_comment.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCommentDlg::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_close.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    //const int height_diff = client_rect.bottom - ok_button_rect.top + 5;

    SetRibbonHeight(/*height_diff*/ client_rect.bottom - ok_button_rect.top + 5);
}





//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMCommentDlg::GetCurrentScanCommentAndScanID(
    CString& f_scan_id,
    CString& f_scan_comment
)const
{

    f_scan_id = _T("");
    f_scan_comment = _T("");

    if (m_interfaceclassforscancommentdlg) {

        return m_interfaceclassforscancommentdlg->GetCurrentScanCommentAndScanID(f_scan_id, f_scan_comment) ;
    }

    return false;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CPQMCommentDlg::GetCurrentScanComment(
)const
{

    if (m_interfaceclassforscancommentdlg) {

        return m_interfaceclassforscancommentdlg->GetCurrentScanComment() ;
    }

    return _T("");
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CPQMCommentDlg::GetCurrentScanId(
)const
{
    if (m_interfaceclassforscancommentdlg) {

        return m_interfaceclassforscancommentdlg->GetCurrentScanID() ;
    }

    return _T("");
}