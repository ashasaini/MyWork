//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//redmine-779

// PQMPlanDupDlg.cpp : implementation file
//
#include "stdafx.h"
#include "PQMPlanDupDlg.h"
#include <tami/libpl/plDuplicate.h>
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include "DPSManager.h"
#include "PQMView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

extern CPQMView* g_MainView;

BEGIN_MESSAGE_MAP(CPQMPlanDupDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMPlanDupDlg)
    ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_CHK_SLAB, OnChkSlab)
    ON_BN_CLICKED(IDC_CHK_TR, OnChkTr)
    ON_BN_CLICKED(IDC_CHK_SLICETHK, OnChkSlicethk)
    ON_BN_CLICKED(IDC_CHK_SLICEGAP, OnChkSlicegap)
    ON_BN_CLICKED(IDC_CHK_FOV, OnChkFov)
    ON_BN_CLICKED(IDC_CHK_MATRIX, OnChkMatrix)
    ON_BN_CLICKED(IDC_CHK_NOWRAP, OnChkNowrap)
    ON_BN_CLICKED(IDC_CHK_FLIPANGLE, OnChkFlipangle)
    ON_BN_CLICKED(IDC_FLOPANGLE, OnFlopangle)
    ON_BN_CLICKED(IDC_CHK_PRESAT, OnChkPresat)
    ON_BN_CLICKED(IDC_CHK_LOCIMGINFO, OnChkLocimginfo)
    ON_BN_CLICKED(IDC_CHK_PRESCANCOIL, OnChkPrescanCoil)      // MEITEC/2010Apr26/Modified/Function Name changed
    ON_WM_SETCURSOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPQMPlanDupDlg dialog

//**************************************************************************
//Method Name   :   CPQMPlanDupDlg
//Author        :   PATNI\DKH
//Purpose       :   constructor
//**************************************************************************
CPQMPlanDupDlg::CPQMPlanDupDlg(CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_PLANDUP_DIALOG, pParent),
    m_currentmask(0),
    m_defaultmask(0),
    m_flag(0),
    m_show_wait_cursor_for_plandupdlg(false)

{
    memset(m_studyfile, 0, (sizeof(char) * MAX_PATH));
}

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMPlanDupDlg::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_plandupdlg = f_show_wait_cursor;
}
void CPQMPlanDupDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMPlanDupDlg)
    DDX_Control(pDX, IDC_CHK_SLICEGAP, m_chkslicegap);
    DDX_Control(pDX, IDC_FLOPANGLE, m_chkflopangle);
    DDX_Control(pDX, IDC_BTN_SAVE, m_btnsave);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_btnclose);
    DDX_Control(pDX, IDC_CHK_BASICSCAN, m_chkbasicscan);
    DDX_Control(pDX, IDC_CHK_FLIPANGLE, m_chkflipangle);
    DDX_Control(pDX, IDC_CHK_FOV, m_chkfov);
    DDX_Control(pDX, IDC_CHK_LOCIMGINFO, m_chklocimginfo);

    DDX_Control(pDX, IDC_CHK_PRESCANCOIL, m_chkprescancoil);  //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    DDX_Control(pDX, IDC_CHK_MATRIX, m_chkmatrix);
    DDX_Control(pDX, IDC_CHK_NOWRAP, m_chknowrap);
    DDX_Control(pDX, IDC_CHK_PRESAT, m_chkpresat);
    DDX_Control(pDX, IDC_CHK_SLAB, m_chkslab);
    DDX_Control(pDX, IDC_CHK_SLICETHK, m_chkslicethk);
    DDX_Control(pDX, IDC_CHK_TR, m_chktr);
    //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPQMPlanDupDlg message handlers

//**************************************************************************
//Method Name   : OnInitDialogImpl
//Author        : PATNI\DKH
//Purpose       : Dialog initialization
//**************************************************************************
BOOL CPQMPlanDupDlg::OnInitDialogImpl()
{

    LPCTSTR FUNC_NAME = _T("CPQMPlanDupDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);



    //Set multilingual captions
    SetMultilingual();

    //Read Settings from File
    ReadPlanDuplicateSettings();

    //InitializeUI
    InitializeUI();

    return CZOrderDialog::OnInitDialogImpl(false);
}


//**************************************************************************
//Method Name   : OnBtnSave
//Author        : PATNI\DKH
//Purpose       : To save the settings
//**************************************************************************
void CPQMPlanDupDlg::OnBtnSave()
{

    LPCTSTR FUNC_NAME = _T("CPQMPlanDupDlg::OnBtnSave");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int32_t l_mask = GetCurrentUIMaskSettings();
    l_mask =  l_mask | PL_DUP_MASK_ORI_VEC ;
    l_mask =  l_mask | PL_DUP_MASK_OFF_VEC ;

    CString l_log_str;
    l_log_str.Format(_T("GetCurrentUIMaskSettings mask value = %d "), l_mask);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    if (NULL != g_MainView) {
        g_MainView->GetAcquisition()->WritePlanDuplicateMask(l_mask, TRUE);
    }

    return;
}
//**************************************************************************
//Method Name   : OnBtnClose
//Author        : PATNI\DKH
//Purpose       : To handle close button click event
//**************************************************************************
void CPQMPlanDupDlg::OnBtnClose()
{

    LPCTSTR FUNC_NAME = _T("CPQMPlanDupDlg::OnBtnClose");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of OnBtnClose"));

    DestroyWindow();
    return;
}


//**************************************************************************
//Method Name   : OnChkSlab
//Author        : PATNI\DKH
//Purpose       : To handle on click of SLAB
//**************************************************************************
void CPQMPlanDupDlg::OnChkSlab()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkslab.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_SLAB;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_SLAB;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkslab);
    }
}
//**************************************************************************
//Method Name   : OnChkTr
//Author        : PATNI\DKH
//Purpose       : To handle on click of TR checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkTr()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chktr.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_TR;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_TR;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chktr);
    }
}
//**************************************************************************
//Method Name   : OnChkSlicethk
//Author        : PATNI\DKH
//Purpose       : To handle on click of Slice thickness checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkSlicethk()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkslicethk.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_THK;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_THK;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkslicethk);
    }
}
//**************************************************************************
//Method Name   : OnChkSlicegap
//Author        : PATNI\DKH
//Purpose       : To handle on click of Slice Gap checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkSlicegap()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkslicegap.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_GAP;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_GAP;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkslicegap);
    }
}
//**************************************************************************
//Method Name   : OnChkFov
//Author        : PATNI\DKH
//Purpose       : To handle on click of FOV checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkFov()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkfov.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_FOV;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_FOV;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkfov);
    }
}
//**************************************************************************
//Method Name   : OnChkMatrix
//Author        : PATNI\DKH
//Purpose       : To handle on click of MATRIX checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkMatrix()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkmatrix.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_MTX;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_MTX;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkmatrix);
    }
}
//**************************************************************************
//Method Name   : OnChkNowrap
//Author        : PATNI\DKH
//Purpose       : To handle on click of No Wrap checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkNowrap()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chknowrap.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_WRAP;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_WRAP;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chknowrap);
    }
}
//**************************************************************************
//Method Name   : OnChkFlipangle
//Author        : PATNI\DKH
//Purpose       : To handle on click of Flip Angle checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkFlipangle()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkflipangle.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_FLIP;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_FLIP;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkflipangle);
    }
}
//**************************************************************************
//Method Name   : OnFlopangle
//Author        : PATNI\DKH
//Purpose       : To handle on click of Flop Angle checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnFlopangle()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkflopangle.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_FLOP;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_FLOP;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkflopangle);
    }
}
//**************************************************************************
//Method Name   : OnChkPresat
//Author        : PATNI\DKH
//Purpose       : To handle on click of Presaturization checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkPresat()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chkpresat.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_PRESAT;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_PRESAT;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkpresat);
    }
}
//**************************************************************************
//Method Name   : OnChkLocimginfo
//Author        : PATNI\DKH
//Purpose       : To handle on click of Locator Image info checkbox
//**************************************************************************
void CPQMPlanDupDlg::OnChkLocimginfo()
{
    int32_t l_previousmask = m_currentmask ;

    if (1 == m_chklocimginfo.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_LOC_IMAGE;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_LOC_IMAGE;
    }

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chklocimginfo);
    }
}

//+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
//**************************************************************************
//Method Name   : OnChkPrescanCoil
//Author        : PATNI\DKH
//Purpose       : To handle on click of Prescan/Coil
//**************************************************************************
void CPQMPlanDupDlg::OnChkPrescanCoil()
{
    int32_t l_previousmask = m_currentmask ;

    //+ MEITEC/2010Apr26/Modified/mask data changed
    if (1 == m_chkprescancoil.GetCheck()) {
        m_currentmask = m_currentmask | PL_DUP_MASK_PRESCAN_COIL;

    } else {
        m_currentmask = m_currentmask ^ PL_DUP_MASK_PRESCAN_COIL;
    }

    //- MEITEC/2010Apr26/Modified/mask data changed

    if (false == SaveCurrentmask(m_currentmask)) {
        //revert back the change
        m_currentmask = l_previousmask ;
        ToggleCheck(m_chkprescancoil);
    }
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMPlanDupDlg::OnSetCursor(CWnd* pWnd,
                                 UINT nHitTest,
                                 UINT message)
{
    if (m_show_wait_cursor_for_plandupdlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}
//-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

//**************************************************************************
//Method Name   : PreTranslateMessage
//Author        : PATNI\DKH
//Purpose       : To handle key's
//**************************************************************************
BOOL CPQMPlanDupDlg::PreTranslateMessageImpl(MSG* pMsg)
{
    if (m_show_wait_cursor_for_plandupdlg) {
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

//**************************************************************************
//Method Name   : ReadPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : To read the plan duplicate settings from the file
//**************************************************************************
bool CPQMPlanDupDlg::ReadPlanDuplicateSettings(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMPlanDupDlg::ReadPlanDuplicateSettings");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Read the initial Setting from "PlanDuplicate.h"
    VARIANT* l_data = new VARIANT;

    if (NULL == l_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pointer l_data is NULL"));
        return false;
    }

    g_MainView->GetAcquisition()->ReadPlanDuplicateSettings(&l_data);
    PlanDupSettings* l_plandupsettings = (PlanDupSettings*)l_data->byref;

    if (NULL != l_plandupsettings) {
        m_currentmask = l_plandupsettings->mask;
        m_defaultmask = l_plandupsettings->defaultmask;
        m_flag        = l_plandupsettings->flag;

        strcpy(l_plandupsettings->studyfile, m_studyfile);
        DEL_PTR(l_plandupsettings)

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pointer l_plandupsettings is NULL"));
    }

    DEL_PTR(l_data)

    return true ;
}
//**************************************************************************
//Method Name   : InitializeUI
//Author        : PATNI\DKH
//Purpose       : To Initialize the UI
//**************************************************************************
bool CPQMPlanDupDlg::InitializeUI()
{

    SetDialogRibbonHeight();
    SetButtonAppearence();
    SetButtonFont();

    //Disable m_chkbasicscan as per vantage behaviour
    m_chkbasicscan.SetCheck(1);
    m_chkbasicscan.EnableWindow(FALSE);

    m_chkslab.SetCheck(0);
    m_chktr.SetCheck(0);
    m_chkslicethk.SetCheck(0);
    m_chkslicegap.SetCheck(0);
    m_chkfov.SetCheck(0);
    m_chkmatrix.SetCheck(0);
    m_chknowrap.SetCheck(0);
    m_chkflipangle.SetCheck(0);
    m_chkflopangle.SetCheck(0);
    m_chkpresat.SetCheck(0);
    m_chklocimginfo.SetCheck(0);

    m_chkprescancoil.SetCheck(0);  //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    if (m_currentmask & PL_DUP_MASK_SLAB) {
        m_chkslab.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_TR) {
        m_chktr.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_THK) {
        m_chkslicethk.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_GAP) {
        m_chkslicegap.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_FOV) {
        m_chkfov.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_MTX) {
        m_chkmatrix.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_WRAP) {
        m_chknowrap.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_FLIP) {
        m_chkflipangle.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_FLOP) {
        m_chkflopangle.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_PRESAT) {
        m_chkpresat.SetCheck(1);
    }

    if (m_currentmask & PL_DUP_MASK_LOC_IMAGE) {
        m_chklocimginfo.SetCheck(1);
    }

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    if (m_currentmask & PL_DUP_MASK_PRESCAN_COIL) {
        m_chkprescancoil.SetCheck(1);
    }

    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    return true;
}


//**************************************************************************
//Method Name   : SaveCurrentmask
//Author        : PATNI\DKH
//Purpose       : To save the current mask settings
//**************************************************************************
bool CPQMPlanDupDlg::SaveCurrentmask(int32_t f_mask)const
{
    HRESULT l_result = S_OK ;
    int32_t l_mask = f_mask ;
    l_mask =  l_mask | PL_DUP_MASK_ORI_VEC ;
    l_mask =  l_mask | PL_DUP_MASK_OFF_VEC ;

    if (NULL != g_MainView) {
        l_result = (g_MainView->GetAcquisition())->WritePlanDuplicateMask(l_mask, FALSE);
    }

    if (S_FALSE == l_result) {
        return false;
    }

    return true;
}

//**************************************************************************
//Method Name   : ToggleCheck
//Author        : PATNI\DKH
//Purpose       : function to toggle the check
//**************************************************************************
void  CPQMPlanDupDlg::ToggleCheck(CheckBoxEx& f_checkbox)const
{
    if (NULL == f_checkbox.m_hWnd) {
        return;
    }

    if (1 == f_checkbox.GetCheck()) {
        f_checkbox.SetCheck(0);

    } else {
        f_checkbox.SetCheck(1);
    }
}


//**************************************************************************
//Method Name   : GetCurrentUIMaskSettings
//Author        : PATNI\DKH
//Purpose       : Get the mask settings done by user
//**************************************************************************
int32_t CPQMPlanDupDlg::GetCurrentUIMaskSettings()const
{

    LPCTSTR FUNC_NAME = _T("CPQMPlanDupDlg::GetCurrentUIMaskSettings");
    int32_t l_mask = 0 ;

    if (1 == m_chkslab.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_SLAB;
    }

    if (1 == m_chktr.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_TR;
    }

    if (1 == m_chkslicethk.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_THK;
    }

    if (1 == m_chkslicegap.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_GAP;
    }

    if (1 == m_chkfov.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_FOV ;
    }

    if (1 == m_chkmatrix.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_MTX ;
    }

    if (1 == m_chknowrap.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_WRAP ;
    }

    if (1 == m_chkflipangle.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_FLIP ;
    }

    if (1 == m_chkflopangle.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_FLOP ;
    }

    if (1 == m_chkpresat.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_PRESAT;
    }

    if (1 == m_chklocimginfo.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_LOC_IMAGE ;
    }

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    if (1 == m_chkprescancoil.GetCheck()) {
        l_mask = l_mask | PL_DUP_MASK_PRESCAN_COIL;
    }

    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    CString l_log_str;
    l_log_str.Format(_T("GetCurrentUIMaskSettings mask value = %d "), l_mask);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    return l_mask;
}

//**************************************************************************
//Method Name   : SetMultilingual
//Author        : PATNI\DKH
//Purpose       : To multilingualize string data
//**************************************************************************
void CPQMPlanDupDlg::SetMultilingual()
{

    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PQMPLANDUPENTRIES_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    //For BASIC SCAN OPTION
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_BASICSCAN"));
    HWND l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_BASICSCAN);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For SLAB OPTION
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_SLAB"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_SLAB);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For TR OPTION
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_TR"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_TR);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For SLICE THICKNESS
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_SLICETHK"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_SLICETHK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For SLICE GAP
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_SLICEGAP"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_SLICEGAP);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For FOV
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_FOV"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_FOV);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For MATRIX
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_MATRIX"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_MATRIX);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;


    //For NOWRAP
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_NOWRAP"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_NOWRAP);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For FLIPANGLE
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_FLIPANGLE"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_FLIPANGLE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For FLOPANGLE
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_FLOPANGLE"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_FLOPANGLE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For PRESATURATION
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_PRESAT"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_PRESAT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For Locator Image Info
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_LOCIMGINFO"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_LOCIMGINFO);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //+ MEITEC/2010Apr26/Added/MVC7909/Prescan/Coil
    //For Prescan/Coil Info
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PRESCANCOIL"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_CHK_PRESCANCOIL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;
    //- MEITEC/2010Apr26/Added/MVC7909/Prescan/Coil

    //For SAVE button
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_SAVE"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_BTN_SAVE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

    //For CLOSE button
    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PLANDUP_CLOSE"));
    l_chkbtn = ::GetDlgItem(m_hWnd, IDC_BTN_CLOSE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_chkbtn, l_chOutput);
    }

    l_chOutput = _T("");
    l_chkbtn = NULL;

}
//**************************************************************************
//Method Name   : SetButtonAppearence
//Author        : PATNI\DKH
//Purpose       : To set button appearence
//**************************************************************************
void CPQMPlanDupDlg::SetButtonAppearence()
{
    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);

    m_chkslicegap.SetBkColor(stColors_o.crClientColor);
    m_chkflopangle.SetBkColor(stColors_o.crClientColor);
    m_chkbasicscan.SetBkColor(stColors_o.crClientColor);
    m_chkflipangle.SetBkColor(stColors_o.crClientColor);
    m_chkfov.SetBkColor(stColors_o.crClientColor);
    m_chklocimginfo.SetBkColor(stColors_o.crClientColor);
    m_chkprescancoil.SetBkColor(stColors_o.crClientColor);  //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    m_chkmatrix.SetBkColor(stColors_o.crClientColor);
    m_chknowrap.SetBkColor(stColors_o.crClientColor);
    m_chkpresat.SetBkColor(stColors_o.crClientColor);
    m_chkslab.SetBkColor(stColors_o.crClientColor);
    m_chkslicethk.SetBkColor(stColors_o.crClientColor);
    m_chktr.SetBkColor(stColors_o.crClientColor);

}

//**************************************************************************
//Method Name   : SetButtonFont
//Author        : PATNI\DKH
//Purpose       : To set the button font
//**************************************************************************
BOOL CPQMPlanDupDlg::SetButtonFont()
{
    LPCTSTR button_font_name = _T("Arial BOLD") ;
    const int size = 12;

    m_chkslicegap.SetFont(button_font_name, size);
    m_chkflopangle.SetFont(button_font_name, size);
    m_chkbasicscan.SetFont(button_font_name, size);
    m_chkfov.SetFont(button_font_name, size);
    m_chklocimginfo.SetFont(button_font_name, size);
    m_chkprescancoil.SetFont(button_font_name, size);  //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    m_chkmatrix.SetFont(button_font_name, size);
    m_chknowrap.SetFont(button_font_name, size);
    m_chkpresat.SetFont(button_font_name, size);
    m_chkslab.SetFont(button_font_name, size);
    m_chkslicethk.SetFont(button_font_name, size);
    m_chktr.SetFont(button_font_name, size);
    m_chkflipangle.SetFont(button_font_name, size);

    m_btnsave.SetPrimaryTextFont(button_font_name, size);
    m_btnsave.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_btnclose.SetPrimaryTextFont(button_font_name, size);
    m_btnclose.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    return TRUE;
}


//**************************************************************************
//Method Name   : SetDialogRibbonHeight
//Author        : PATNI\DKH
//Purpose       : To set the dialog ribbon height
//**************************************************************************
void CPQMPlanDupDlg::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_btnsave.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 5);

}


