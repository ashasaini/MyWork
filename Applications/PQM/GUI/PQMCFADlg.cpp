//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-779
//TMSC-REDMINE-774
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMCFADlg.cpp
 *  Overview: Implementation of CPQMCFADlg class.
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
#include "PQMCFADlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <math.h>		//IR-171
#include <PQM/resource.h>
#include "PQMView.h"
#include "PQMUtility.h"	//Added By KT/ACE-2/11-06-2009/Multilingualisation		
#include "DPSManager.h"
#include "UIButtonUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

//Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
#define	MAX(a,b)	((b) > (a) ? (b) : (a))

CString PPM_FORMAT_SPEC = _T("%3.1f");

extern CPQMView* g_MainView;

PqmShimWindow CPQMGraphCtrl::ShimWindow = {0, 0, NULL, 0, 0, 0, 0};		//Patni-ARD/20090315/Added/ACE-1# Prescan


BEGIN_MESSAGE_MAP(CPQMCFADlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMCFADlg)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, OnClickButtonApply)
    ON_BN_CLICKED(IDC_BUTTON_CFA_LEFT, OnClickButtonCfaLeft)
    ON_BN_CLICKED(IDC_BUTTON_CFA_RIGHT, OnClickButtonCfaRight)
    ON_BN_CLICKED(IDC_BUTTON_CFA_RETRY, OnClickButtonCfaRetry)
    ON_BN_CLICKED(IDC_RADIO_BUTTON_FAT, OnClickRadioButtonFat)
    ON_BN_CLICKED(IDC_RADIO_BUTTON_WATER, OnClickRadioButtonWater)
    //Added by Hemant On 7/1/2008 1:32:37 PM
    ON_BN_CLICKED(IDC_UTILITY, OnClickUtility)
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_MENUITEM_ADJUST_CENTER_FREQUENCY,
               OnClickMenuitemAdjustCenterFrequency)

    //Added by Hemant On 12/18/2008 5:16:42 PM
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    //

END_MESSAGE_MAP()


//************************************Method Header**************************************

//Method Name:  CPQMCFADlg

//Author:  PATNI/SVP

//Purpose: Constructor

//**************************************************************************************/
CPQMCFADlg::CPQMCFADlg(const CRect& parent_rect /*= CRect(0,0,0,0)*/, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_CFA, pParent),
    m_ppm_offset(_T("")),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_parent_rect(parent_rect),
    m_dialog_rect(CRect(0, 0, 0, 0)),
    m_prev_shimflag(-1),
    cl_OrgWindowRect(CRect(0, 0, 0, 0)),
    m_disp_controls(false)
{

    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::CPQMCFADlg");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("Trace of Constructor"));


    //+Patni-Hemant/2009Mar17/MODIFIED/ACE-1/MCM0228-00102, MCM0213-00130
    ResetPendingCFAMessage();
}

//************************************Method Header**************************************

//Method Name:  ~CPQMCFADlg

//Author:  MSN

//Purpose: Destructor

//**************************************************************************************/
CPQMCFADlg::~CPQMCFADlg()
{
}



//+Patni-Ravindra Acharya/2010July30/Commented/IR-153
void CPQMCFADlg::OnCFAReady()
{
    if (m_apply.IsWindowVisible()) {
        m_apply.EnableWindow(TRUE);
    }
}
//-Patni-Ravindra Acharya/2010July30/Commented/IR-153


void CPQMCFADlg::DisplayOffset()
{
    CString str ;
    ((CEdit*)GetDlgItem(IDC_EDIT_CFA))->GetWindowText(str);
    CPQMGraphCtrl::ShimWindow.ppm_offset = (int)(CPQMUtility::GetInstance()->ConvertStringToDouble(str) * 10.0);

    ValidateOffsetValueAndRedraw();
}


//+Patni-MN/2009Mar13/Added/PSP1#196
//************************************Method Header************************************
// Method Name  :ProcessShimmingMsg()
// Author       :PATNI/ MANISHKUMAR
// Purpose      :To process the messages after shimming
//***********************************************************************************
void CPQMCFADlg::ProcessShimmingMsg(const pqm_msg_tag f_message_to_process)
{

    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::ProcessShimmingMsg");
    PQM_TRACE_SCOPED(FUNC_NAME);

    switch (f_message_to_process) {
        case PQM_MSG_CFA_PRESCAN_COUCH :
        case PQM_MSG_CFA_PRESCAN_ABORT : {
            //+Patni-SS/ADDED/2009-May-15/JFT#137
            g_MainView->SetIsScanning(false);

            //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
            //make the entry in queue
            if (!IsWindow(GetSafeHwnd())) {

                PendingMessageToProcess(f_message_to_process);
                return ;
            }

            //-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130

            SetBtnSensitive(IDOK, true);

            //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
            //close the dilaog on this message
            //-Patni-SS/Added-2009May28/JFT#223
            if (f_message_to_process == PQM_MSG_CFA_PRESCAN_ABORT) {
                SetFocusOnNextItemAfterScanDoneorAbort();
                //
                //-Patni-HEMANT/ADDED On 4/23/2009 10:51:51 AM/ ACE / ITC-2 Defect Fix
                OnCancel();
                //-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
            }

        }

        UpdateCFARetry();
        //-Patni-SS/Added-2009May13/JFT#121

        break;

        case PQM_MSG_CFA_PRESCAN_END : {

            if (!IsWindow(GetSafeHwnd())) {
                PendingMessageToProcess(f_message_to_process);
                return ;
            }

            PreScanCFCurve_p    l_curve_p = new SPreScanCFCurve();

            if (NULL == l_curve_p) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("l_curve_p pointer is NULL"));

                return;
            }

            PreScanReadCFCurveData(l_curve_p);
            SetBtnSensitive(IDOK, true);

            if (CFA_DLG_NORMAL != g_MainView->GetShimDoneFlag()) {

                int l_shim_state = SHIM_STATE_WAIT;
                g_MainView->GetAcquisition()->GetShimState(&l_shim_state);

                if (SHIM_STATE_ACTIVE == l_shim_state) {
                    ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->EnableWindow(true);
                    ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->EnableWindow(true);
                }

                if (g_MainView->GetShimStateFlag() == VFC_SHIM_FSY_WATER) {

                    ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->SetCheck(BST_CHECKED);
                    ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->SetCheck(BST_UNCHECKED);

                } else {

                    ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->SetCheck(BST_CHECKED);
                    ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->SetCheck(BST_UNCHECKED);

                }
            }

            UpdateCFARetry();
            PqmCFARedraw();

            DEL_PTR(l_curve_p)

        }
        break;

        case PQM_MSG_ABORT_WAIT:
        case PQM_MSG_ABORT_FAIL:
        case PQM_MSG_APC_RETRY_WAIT:
        case PQM_MSG_SCAN_START:
        case PQM_MSG_PAUSE_DONE:
        case PQM_MSG_RESUME_DONE:
        case PQM_MSG_ERROR: {

            UpdateCFARetry();
        }
        break ;
    }
}
//-Patni-MN/2009Mar13/Added/PSP1#196


//+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
//************************************Method Header************************************
// Method Name  :ShowHideDialogCtrls()
// Author       :PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::ShowHideDialogCtrls(
    const bool f_hide_disp_controls_explicitly /*= false*/
)
{
    CPQMGraphCtrl::ShimWindow.ppm_offset = 0;
    CPQMGraphCtrl::ShimWindow.shimFlag = g_MainView->GetShimStateFlag();

    //Note : when f_hide_disp_controls_explicitly is true, we will make m_disp_controls as false
    //and thus hide the related controls..
    //if f_hide_disp_controls_explicitly is false, we will not change m_disp_controls variable..
    //and wil show/hide the related controls accordingly

    switch (g_MainView->GetShimDoneFlag()) {

        case CFA_DLG_NORMAL:

            ((CButton*)GetDlgItem(IDC_BUTTON_CFA_RETRY))->ShowWindow(true);

            ((CButton*)GetDlgItem(IDCANCEL))->ShowWindow(true);

            ((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->ShowWindow(true);
            break;

        case CFA_DLG_SHIMMING_DONE:
            ((CButton*)GetDlgItem(IDC_BUTTON_CFA_RETRY))->ShowWindow(true);

            ((CButton*)GetDlgItem(IDCANCEL))->ShowWindow(true);

            ((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->ShowWindow(true);
            break;

        case CFA_DLG_SHIMMING_SEQUENCE:

            ((CButton*)GetDlgItem(IDC_BUTTON_CFA_RETRY))->ShowWindow(false);

            ((CButton*)GetDlgItem(IDCANCEL))->ShowWindow(false);

            ((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->ShowWindow(false);
            ((CButton*)GetDlgItem(IDOK))->EnableWindow(false);
            break;
    }

    int l_shim_state = SHIM_STATE_WAIT;
    g_MainView->GetAcquisition()->GetShimState(&l_shim_state);

    if (SHIM_STATE_ACTIVE == l_shim_state) {
        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->EnableWindow(true);
        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->EnableWindow(true);
    }

    if (g_MainView->GetShimStateFlag() == VFC_SHIM_FSY_WATER) {
        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->SetCheck(BST_CHECKED);

        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->SetCheck(BST_UNCHECKED);

    } else {
        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->SetCheck(BST_CHECKED);

        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->SetCheck(BST_UNCHECKED);
    }

    //intial dialog button state..
    if (f_hide_disp_controls_explicitly) {
        m_disp_controls = !f_hide_disp_controls_explicitly ;
    }

    //Initially disable CFA controls
    EnableCFAControls(m_disp_controls);

    UpdateCFARetry();

    //Patni-HEMNAT+ARD/20090315/Added/ACE-1 # Prescan Start
    PqmCFARedraw();
}
//-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130


//+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
//************************************Method Header************************************
// Method Name  :ResetPendingCFAMessage()
// Author       :PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::ResetPendingCFAMessage(
)
{
    m_pending_message_to_process = PQM_MSG_INVALID_MESSAGE;
}
//-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130

void CPQMCFADlg::UpdateCFARetry()
{

    if (g_MainView->IsNextProtocolAutoScanAfterStatusDone()) {
        return;
    }

    if (IsWindow(GetSafeHwnd())) {
        BOOL l_enable = g_MainView->CanEnableCFARetry();

        if (m_cfa_retry.GetSafeHwnd()) {
            ((CButton*)GetDlgItem(IDC_BUTTON_CFA_RETRY))->EnableWindow(l_enable);
        }

        if (m_ok.GetSafeHwnd()) {
            m_ok.EnableWindow(!g_MainView->GetPostProcessStatus());
        }

        if (m_apply.GetSafeHwnd()) {
            ((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->EnableWindow(l_enable && m_ok.IsWindowEnabled());
        }
    }
}

void CPQMCFADlg::DoCFAApply(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::DoCFAApply");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (g_MainView->IsScanning()) {
        ASSERT(FALSE);
        return ;
    }

    if (!IsWindow(GetSafeHwnd())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DoCFAApply returned. Dialog is closed."));
        return;
    }

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_CENTER_FREQ_APPLY);
    CalculateAndSendCenterFrequency();
}

void CPQMCFADlg::DoCFARetry(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::DoCFARetry");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
    if (S_OK == g_MainView->GetAcquisition()->RemoteCheckActiveFlag()) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                CPQMUtility::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE")),
                SCAN_ERR_NOTIFY, L"CPQMCFADlg.cpp");
        return;
    }

    //-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole

    if (!g_MainView->CanEnableCFARetry()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanEnableCFARetry = False"));
        return;
    }

    if (IsWindow(GetSafeHwnd())) {

        if (!IsWindow((m_cfa_retry.GetSafeHwnd())) || !m_cfa_retry.IsWindowEnabled()) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_cfa_retry is not present or is disabled"));
            return;
        }

        CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_CENTER_FREQ_CFA_RETRY);
        ((CButton*)GetDlgItem(IDC_BUTTON_CFA_RETRY))->EnableWindow(false);

        if (m_apply.GetSafeHwnd()) {
            ((CButton*)GetDlgItem(IDC_BUTTON_APPLY))->EnableWindow(false);
        }
    }

    int index = (g_MainView->GetPQMListCtrl()).GetFirstSelectedItem();

    if (index != -1) {

        CSequence* pseq = g_MainView->GetSelectedSequence(index);

        if (pseq) {
            if ((PR_ACQ_STATUS_SUCCESS == pseq->GetAcqStatus()) || PR_ACQ_STATUS_WAIT == pseq->GetAcqStatus() || PR_ACQ_STATUS_FAIL == pseq->GetAcqStatus()) {
                DoCFARetrySettings();
            }
        }
    }
}



//+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
//+Patni-HEMANT/ADDED On 3/17/2009 4:06:48 PM/ PSP #254
//************************************Method Header************************************
// Method Name  :GetPendingCFAMessage()
// Author       :PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
pqm_msg_tag CPQMCFADlg::GetPendingCFAMessage(
)const
{
    return m_pending_message_to_process;
}
//-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130


void CPQMCFADlg::SetPreviousShimFlag(const int f_prev_shimflag)
{
    m_prev_shimflag = f_prev_shimflag;
}
//************************************Method Header**************************************
//Method Name:  OnOK
//Author:  PATNI/SVP
//Purpose: To validate ppm offset & calculate center frequency,then closes dialog box.
//**************************************************************************************/
void CPQMCFADlg::OnOK(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_CENTER_FREQ_OK);

    g_MainView->GetAcquisition()->SetShimFSYFlag(g_MainView->GetShimStateFlag());
    SetFocusOnNextItemAfterScanDoneorAbort();

    g_MainView->SetIsCFAokpressed(true);
    g_MainView->GetAcquisition()->CheckNextProtocol(g_MainView->GetAcqOderForGivenProtocolIndex(g_MainView->GetScanIndex()), TRUE);

    //Patni-HAR/2010Jun28/Added/V1.20#MVC008392
    g_MainView->SetCFAWindowOpenedBeforeSwPage(false);

    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDOK);
    }

    //DialogEx::OnOK();
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}


//+Patni-HEMANT/ADDED On 9/13/2009 5:53:04 PM/ IDS reported
//************************************Method Header************************************
// Method Name  : OnCancel
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::OnCancel()
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMGraphCtrl::ShimWindow.shimFlag = m_prev_shimflag;
    g_MainView->SetShimStateFlag(m_prev_shimflag);

    g_MainView->SetIsCFAokpressed(true);

    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    //DialogEx::OnCancel();
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}
//-Patni-HEMANT/ADDED On 9/13/2009 5:53:04 PM/ IDS reported


//************************************Method Header**************************************
//Method Name:  OnClickButtonApply
//Author:  PATNI/SVP
//Purpose: This method used to validate ppm offset & calculate center frequency,closes dialog box
//**************************************************************************************/
void CPQMCFADlg::OnClickButtonApply(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnClickButtonApply");
    PQM_TRACE_SCOPED(FUNC_NAME);

    g_MainView->OnCFAOperationClick(false);
}

//************************************Method Header**************************************
//Method Name:  OnClickButtonCfaLeft
//Author:  PATNI/SVP
//Purpose: To decrease ppm offset value
//**************************************************************************************/
void CPQMCFADlg::OnClickButtonCfaLeft(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_CENTER_FREQ_LEFT);

    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    CPQMGraphCtrl::ShimWindow.ppm_offset -= 1;
    ValidateOffsetValueAndRedraw();
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
}

//************************************Method Header**************************************
//Method Name:  OnClickButtonCfaRight
//Author:  PATNI/SVP
//Purpose: To increase ppm offset value
//**************************************************************************************/
void CPQMCFADlg::OnClickButtonCfaRight(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_CENTER_FREQ_RIGHT);

    //-------------------------------------------------------------------------
    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    CPQMGraphCtrl::ShimWindow.ppm_offset += 1;
    ValidateOffsetValueAndRedraw();
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //-------------------------------------------------------------------------
}


//************************************Method Header**************************************
//Method Name:  OnClickButtonCfaRetry
//Author:  PATNI/SVP
//Purpose:
//**************************************************************************************/
void CPQMCFADlg::OnClickButtonCfaRetry(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnClickButtonCfaRetry");
    PQM_TRACE_SCOPED(FUNC_NAME);

    g_MainView->OnCFAOperationClick(true);
}

//************************************Method Header**************************************
//Method Name:  OnClickRadioButtonFat
//Author:  PATNI/SVP
//Purpose: This method used to set value of m_shim_flag to shim_fsy_fat.
//**************************************************************************************/
void CPQMCFADlg::OnClickRadioButtonFat(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnClickRadioButtonFat");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //-------------------------------------------------------------------------
    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    if (CPQMGraphCtrl::ShimWindow.shimFlag == VFC_SHIM_FSY_FAT) {

        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->SetCheck(BST_CHECKED);
        return ;

    } else {

        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->SetCheck(BST_UNCHECKED);
    }

    CPQMGraphCtrl::ShimWindow.shimFlag = VFC_SHIM_FSY_FAT ;

    g_MainView->SetShimStateFlag(CPQMGraphCtrl::ShimWindow.shimFlag);

    PqmCFARedraw(false);
    //
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //-------------------------------------------------------------------------
}

//************************************Method Header**************************************
//Method Name:  OnClickRadioButtonWater
//Author:  PATNI/SVP
//Purpose: This method used to set value of m_shim_flag to shim_fsy_water.
//**************************************************************************************/
void CPQMCFADlg::OnClickRadioButtonWater(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnClickRadioButtonWater");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //-------------------------------------------------------------------------
    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    if (CPQMGraphCtrl::ShimWindow.shimFlag == VFC_SHIM_FSY_WATER) {

        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_WATER))->SetCheck(BST_CHECKED);
        return ;

    } else {

        ((CButton*)GetDlgItem(IDC_RADIO_BUTTON_FAT))->SetCheck(BST_UNCHECKED);
    }

    CPQMGraphCtrl::ShimWindow.shimFlag = VFC_SHIM_FSY_WATER;
    g_MainView->SetShimStateFlag(CPQMGraphCtrl::ShimWindow.shimFlag);

    PqmCFARedraw(false);
    //
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCFADlg::OnInitDialogImpl(
)
{
    //-------------------------------------------------------------------------
    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Added by Hemant On 7/1/2008 2:37:14 PM
    CreateGraphCtrl();

    SetButtonAppearence();

    //Added by Hemant On 12/11/2008 11:15:28 AM
    SetTitleBarHeight(24) ;
    SetFont(_T("Arial"), 10) ;

    SetMultilingual();//Language usha

    if (!m_parent_rect.IsRectEmpty() && m_dialog_rect.IsRectEmpty()) {

        CRect window_rect;
        GetWindowRect(&window_rect);

        m_dialog_rect = window_rect;
        m_dialog_rect.left = m_parent_rect.right;
        m_dialog_rect.top = m_parent_rect.bottom - window_rect.Height();
        m_dialog_rect.right = m_dialog_rect.left + window_rect.Width();
        m_dialog_rect.bottom = m_dialog_rect.top + window_rect.Height();
    }

    //+Patni-HEMANT/ADDED On 4/20/2009 7:49:46 PM/ ACE / Internal Defect
    SetWindowPos(&wndTop , m_dialog_rect.left , m_dialog_rect.top , m_dialog_rect.Width(), m_dialog_rect.Height(), SWP_NOZORDER);

    GetWindowRect(&cl_OrgWindowRect) ;
    fnResize(cl_OrgWindowRect.Width() , cl_OrgWindowRect.Height()) ;

    SetDialogRibbonHeight();

    shimAllocCurve();		//Patni-ARD/20090315/Added/ACE-1 # Prescan
    ShowHideDialogCtrls(true);


    //Patni-MN/2009Mar09/Added/PSP1#203//Added a function to process pending message.
    ProcessPendingMessage();

    //Patni-HEMANT/ADDED On 9/13/2009 5:53:04 PM/ IDS reported
    m_prev_shimflag = CPQMGraphCtrl::ShimWindow.shimFlag;
    g_MainView->SetIsCFAokpressed(false);
    return CZOrderDialog::OnInitDialogImpl(false);  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //-------------------------------------------------------------------------
}


//************************************Method Header**************************************
//Method Name:  OnClickMenuitemAdjustCenterFrequency
//Author:  PATNI/SVP
//Purpose:
//**************************************************************************************/
void CPQMCFADlg::OnClickMenuitemAdjustCenterFrequency(
)
{
    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDM_UTILITY_ADJUST_CENTER_FREQ);


    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    m_disp_controls = !m_disp_controls;
    EnableCFAControls(m_disp_controls);
    AdjustCancelButtonPos();
    //
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::OnClickUtility(
)
{

    CRect rect_for_utility_button ;
    m_utility.GetWindowRect(&rect_for_utility_button);

    PopupMenuEx context_menu;
    context_menu.LoadMenu(IDR_MENU_CFA);

    LPCTSTR font_name = _T("Trebuchet MS") ;
    const int font_size = 10;

    context_menu.SetFont(font_name, font_size);

    context_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect_for_utility_button.left,
                                rect_for_utility_button.bottom, this, 0, 0);

}

//-------------------------------------------------------------------------
//Added by Hemant On 12/18/2008 5:14:18 PM
//
void CPQMCFADlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    // TODO: Add your message handler code here and/or call default

    if (!cl_OrgWindowRect.IsRectEmpty()) {
        lpMMI->ptMinTrackSize.x = cl_OrgWindowRect.Width() ;
        lpMMI->ptMinTrackSize.y = cl_OrgWindowRect.Height() ;
    }

    CZOrderDialog::OnGetMinMaxInfo(lpMMI);
}

//-------------------------------------------------------------------------
//Added by Hemant On 12/18/2008 5:14:18 PM



//-------------------------------------------------------------------------
//Added by Hemant On 12/18/2008 5:14:18 PM
//
void CPQMCFADlg::OnSize(UINT nType, int cx, int cy)
{
    CZOrderDialog::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here
    fnResize(cx, cy);
}

//************************************Method Header**************************************
//Method Name:  DoDataExchange
//Author:  PATNI/SVP
//Purpose: To exchange and validate dialog data for CPQMDlg
//**************************************************************************************/
void CPQMCFADlg::DoDataExchange(
    CDataExchange* pDX
)
{
    CZOrderDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CPQMCFADlg)
    DDX_Control(pDX, IDC_EDIT_CFA, m_c_edit_cfa);
    DDX_Text(pDX, IDC_EDIT_CFA, m_ppm_offset);
    DDX_Control(pDX, IDC_BUTTON_CFA_RETRY, m_cfa_retry);
    DDX_Control(pDX, IDC_BUTTON_CFA_RIGHT, m_cfa_right);
    DDX_Control(pDX, IDOK, m_ok);
    DDX_Control(pDX, IDC_BUTTON_APPLY, m_apply);
    DDX_Control(pDX, IDC_UTILITY, m_utility);
    DDX_Control(pDX, IDC_STATIC_EXCITATIONN_SELECTION, m_label_excitation);
    DDX_Control(pDX, IDC_RADIO_BUTTON_WATER, m_water);
    DDX_Control(pDX, IDC_RADIO_BUTTON_FAT, m_fat);
    DDX_Control(pDX, IDC_BUTTON_CFA_LEFT, m_cfa_left);

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    DDX_Control(pDX, IDCANCEL, m_cancel);

    //}}AFX_DATA_MAP
}


//************************************Method Header**************************************
//Method Name:  PreTranslateMessage
//Author:  PATNI\MSN
//Purpose: For handling enter key and esc key on CFA Dialog
//**************************************************************************************/
BOOL CPQMCFADlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (pMsg->wParam == VK_RETURN) {
        if (pMsg->hwnd == ((CEdit*)GetDlgItem(IDC_EDIT_CFA))->GetSafeHwnd()) { // Handling enter on Adjust edit box

            //Patni-HEMANT/ADDED On 4/20/2009 4:08:43 PM/ ACE / calling a function instead of the followings
            DisplayOffset();
        }

        pMsg->wParam = NULL;

    } else if (pMsg->wParam == VK_SPACE) {

        if (!((CWnd*)GetDlgItem(IDC_BUTTON_CFA_RETRY))->IsWindowEnabled()) {
            pMsg->wParam = NULL;
        }
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}


//************************************Method Header**************************************
//Method Name:  DoCFARetrySettings
//Author:  PATNI\PJS
//Purpose: Do CFA Retry settings.
//**************************************************************************************/
void CPQMCFADlg::DoCFARetrySettings()const
{
    g_MainView->StartCFAPrescan();
}
//-Patni-PJS/Modified/1Aug2009/Internal defect solved


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCFADlg::CreateGraphCtrl(
)
{
    CWnd* wnd = GetDlgItem(IDC_CFA_STATIC_GRAPH);

    if (wnd) {

        CRect graph_rect;
        wnd->GetWindowRect(&graph_rect);
        ScreenToClient(&graph_rect);

        wnd->DestroyWindow();

        return m_cfagraphctrl.Create(
                   CPQMUtility::GetMultiLingualString(_T("IDS_DEMO_CONTROL")),
                   WS_CHILD | WS_VISIBLE | WS_BORDER ,
                   graph_rect, this, IDC_CFA_STATIC_GRAPH
               );
    }

    return FALSE;
}


void CPQMCFADlg::fnResize(const int iaCx , const int iaCy)
{
    //must do this , as when minimizing and then agina restoring .. gives some incorrect sizing for some controls..
    if (iaCx <= 0 &&  iaCy <= 0)
        return ;

    const int niGapX = 7 ;

    CRect clWindowRect ;
    GetWindowRect(&clWindowRect) ;
    ScreenToClient(&clWindowRect) ;

    CRect clChildWndRect(0, 0, 0, 0) ;
    int niIncrementX = 0, niIncrementY = 0;
    CWnd* clpWnd =  NULL;

    if (clpWnd = GetDlgItem(IDOK)) {
        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        niIncrementY = clWindowRect.bottom - clChildWndRect.bottom - niGapX ;
    }

    if (clpWnd = GetDlgItem(/*IDC_CONVERSATION_STATIC*/IDOK)) {
        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        niIncrementX = clWindowRect.right - clChildWndRect.right - niGapX;
    }

    fnOffsetChild(IDC_STATIC_EXCITATIONN_SELECTION, CSize(niIncrementX, 0)) ;
    fnOffsetChild(IDC_RADIO_BUTTON_WATER, CSize(niIncrementX, 0)) ;
    fnOffsetChild(IDC_RADIO_BUTTON_FAT, CSize(niIncrementX, 0)) ;
    fnOffsetChild(IDC_BUTTON_CFA_RETRY, CSize(niIncrementX, 0)) ;

    fnOffsetChild(IDC_BUTTON_CFA_LEFT, CSize(niIncrementX, niIncrementY)) ;
    fnOffsetChild(IDC_BUTTON_CFA_RIGHT, CSize(niIncrementX, niIncrementY)) ;
    fnOffsetChild(IDC_EDIT_CFA, CSize(niIncrementX, niIncrementY)) ;
    fnOffsetChild(IDC_UTILITY, CSize(0, niIncrementY)) ;
    fnOffsetChild(IDC_BUTTON_APPLY, CSize(niIncrementX, niIncrementY)) ;
    fnOffsetChild(IDOK, CSize(niIncrementX, niIncrementY)) ;

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    AdjustCancelButtonPos();

    fnIncrementSizeChild(IDC_CFA_STATIC_GRAPH, CSize(niIncrementX , niIncrementY)) ;

    Invalidate(FALSE) ;
}


//-------------------------------------------------------------------------
//Added by Hemant On 12/18/2008 5:14:18 PM
//
void CPQMCFADlg::fnOffsetChild(const UINT uiaID , const CSize& claOffset)
{
    CWnd* clpWnd = NULL ;

    if (clpWnd = GetDlgItem(uiaID)) {
        CRect clChildWndRect ;

        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        clChildWndRect.OffsetRect(claOffset) ;
        clpWnd->SetWindowPos(NULL , clChildWndRect.left , clChildWndRect.top , clChildWndRect.Width() , clChildWndRect.Height() , SWP_NOZORDER | SWP_NOSIZE) ;
    }
}


//-------------------------------------------------------------------------
//Added by Hemant On 12/18/2008 5:14:18 PM
void CPQMCFADlg::fnIncrementSizeChild(const UINT uiaID , const CSize& claIncSize)
{
    CWnd* clpWnd = NULL ;

    if (clpWnd = GetDlgItem(uiaID)) {
        CRect clChildWndRect ;
        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        clChildWndRect.right += claIncSize.cx ;
        clChildWndRect.bottom += claIncSize.cy ;

        clpWnd->SetWindowPos(NULL , clChildWndRect.left , clChildWndRect.top , clChildWndRect.Width() , clChildWndRect.Height() , SWP_NOZORDER | SWP_NOMOVE) ;
    }
}



//+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
void CPQMCFADlg::AdjustCancelButtonPos()
{

    CRect l_apply_button_rect;
    m_apply.GetWindowRect(&l_apply_button_rect);
    ScreenToClient(&l_apply_button_rect);

    if (m_apply.IsWindowVisible()) {
        CRect l_ok_button_rect;
        m_ok.GetWindowRect(&l_ok_button_rect);
        ScreenToClient(&l_ok_button_rect);
        const int l_ok_apply_gap = l_ok_button_rect.left - l_apply_button_rect.right ;
        l_apply_button_rect.OffsetRect(-(l_apply_button_rect.Width() + l_ok_apply_gap), 0);

    }

    m_cancel.SetWindowPos(NULL , l_apply_button_rect.left , l_apply_button_rect.top , l_apply_button_rect.Width() , l_apply_button_rect.Height() , SWP_NOZORDER) ;

    Invalidate(FALSE);
    UpdateWindow();
}
//-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::ValidateOffsetValueAndRedraw(
    const bool f_redraw /*= true*/
)
{
    if (CPQMGraphCtrl::ShimWindow.ppm_offset < -60)
        CPQMGraphCtrl::ShimWindow.ppm_offset = -60;

    else if (CPQMGraphCtrl::ShimWindow.ppm_offset > 60)
        CPQMGraphCtrl::ShimWindow.ppm_offset = 60;

    if (f_redraw) {
        PqmCFARedraw(FALSE);
    }

    CString offset_str ;
    offset_str.Format(_T("%0.2f"), (CPQMGraphCtrl::ShimWindow.ppm_offset * 0.1f));

    m_ppm_offset = offset_str ;

    UpdateData(false);
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//************************************Method Header**************************************
//Method Name:  CalculateCenterFrequency
//Author:  PATNI/SVP
//Purpose: To calculate center frequency.
//**************************************************************************************/
void CPQMCFADlg::CalculateAndSendCenterFrequency(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::CalculateAndSendCenterFrequency");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //this statement is not present in vantage, but better to validate the offset value
    ValidateOffsetValueAndRedraw(false);

    const float fsy_value = CPQMGraphCtrl::ShimWindow.centerFreq + (float)(((float)CPQMGraphCtrl::ShimWindow.ppm_offset * CPQMGraphCtrl::ShimWindow.centerFreq / FLOAT_THOUSAND_DIVIDE_FACTOR));

    //Patni-DKH/2010Feb04/Added/PH#3#/IR86/DebugWindow
    CString l_debugstr = _T("");
    l_debugstr.Format(_T("%g MHz --> %g MHz (%g ppm)"), CPQMGraphCtrl::ShimWindow.centerFreq,
                      fsy_value, (((float)CPQMGraphCtrl::ShimWindow.ppm_offset) / 10.0));

    g_MainView->GetPqmDebugWinDlg().UpdateDebugData(l_debugstr);
    //Patni-DKH/2010Feb04/Added/PH#3#/IR86/DebugWindow

    l_debugstr = _T("");
    l_debugstr.Format(_T("PqmAcqSendShimFreq = %f MHz"), fsy_value);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_debugstr);

    g_MainView->GetAcquisition()->ChangeShimCenterFrequency(fsy_value);
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
}


//************************************Method Header**************************************
//Method Name:  EnableCFAControls
//Author:  PATNI/SVP
//Purpose: This method used to hide and show controls.
//**************************************************************************************/
void CPQMCFADlg::EnableCFAControls(
    const bool state
)
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::EnableCFAControls");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of EnableCFAControls"));

    m_c_edit_cfa.ShowWindow(state);
    m_cfa_right.ShowWindow(state);
    m_cfa_left.ShowWindow(state);
    m_apply.ShowWindow(state);

    //Patni-PJS/Modified/1Aug2009/Internal defect solved
    //+Patni-PJS/2010Mar10/Commented/MCM0229-00036
    m_apply.EnableWindow((g_MainView->CanEnableCFARetry() && m_ok.IsWindowEnabled()) ? TRUE : FALSE);
    //-Patni-PJS/2010Mar10/Added/MCM0229-00036
    //-Patni-HAR-Hemant/2009July28/Added/IDS#565
}


//************************************Method Header**************************************
//Method Name:  PreScanReadCFCurveData
//Author:  PATNI\MSN
//Purpose: Reads the CF.curveData file
//**************************************************************************************/
void CPQMCFADlg::PreScanReadCFCurveData(
    const PreScanCFCurve_p    f_curve_p
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::PreScanReadCFCurveData");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Patni-HEMANT/20090315/Added/ACE-1 # Prescan End ,Internal fix
    if (f_curve_p == NULL) return;

    char*	path = NULL;

    if ((path = getenv("GP_DATA")) == NULL)
        path = ".";

    char buf[FILENAME_MAX] = {0};
    sprintf(buf, "%s/preScan/%s", path, PRESCAN_CF_CURVE_DATA);

    FILE*	fp = NULL;

    if ((fp = fopen(buf, "r")) == NULL)
        return;

    f_curve_p->m_numpoints = PRESCAN_CF_CURVE_DATA_LEN;
    f_curve_p->m_data =	(double*)malloc(sizeof(double) * f_curve_p->m_numpoints);

    if (f_curve_p->m_data == NULL) {
        fclose(fp);
        return;
    }

    bzero(f_curve_p->m_data, sizeof(double) * f_curve_p->m_numpoints);

    char* pt1 = NULL, *pt2 = NULL;
    int cnt = 0;

    while (fgets(buf, sizeof(buf), fp)) {

        if ((pt1 = strtok(buf, " \t\n")) == NULL)
            continue;

        if ((pt2 = strtok(NULL, " \t\n")) == NULL)
            continue;

        if (*pt1 == '#') {
            pt1++;

            if (!strcmp(pt1, PRESCAN_CF_KEY_VALUE))
                f_curve_p->m_centerfreq = atof(pt2);

            else if (!strcmp(pt1, PRESCAN_CF_ADC_PITCH))
                f_curve_p->m_adcpitch = atoi(pt2);

            else if (!strcmp(pt1, PRESCAN_CF_FFT_POINTS))
                f_curve_p->m_fftpoints = atoi(pt2);

            continue;
        }

        if (*pt1 != '-' && *pt1 != '+' && *pt1 != '.' && !isdigit(*pt1))
            continue;

        if (*pt2 != '-' && *pt2 != '+' && *pt2 != '.' && !isdigit(*pt2))
            continue;

        f_curve_p->m_data[cnt++] = atof(pt2);

        if (cnt == f_curve_p->m_numpoints)
            break;
    }

    fclose(fp);

    return;
}


//+Patni-MN/2009Mar09/Added/PSP1#203//Added to process the pending messages.
//************************************Method Header************************************
// Method Name  :PendingMessageToProcess(pqm_msg_tag f_pending_message_to_process)
// Author       :PATNI/ MANISHKUMAR
// Purpose      :To highlight the "OK" button of CFA Graph
//***********************************************************************************
void CPQMCFADlg::PendingMessageToProcess(const pqm_msg_tag f_pending_message_to_process)
{
    m_pending_message_to_process = f_pending_message_to_process ;
}

//************************************Method Header************************************
// Method Name  :ProcessPendingMessage()
// Author       :PATNI/ MANISHKUMAR
// Purpose      :To highlight the "OK" button of CFA Graph
//***********************************************************************************
void CPQMCFADlg::ProcessPendingMessage()
{

    //Patni-MN/2009Mar13/Added/PSP1#196
    ProcessShimmingMsg(m_pending_message_to_process);

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    //we have processed the pending messages
    ResetPendingCFAMessage();
}
//-Patni-MN/2009Mar09/Added/PSP1#203//Added to process the pending messages.


//************************************Method Header**************************************
//Method Name:  shimRefreshCurve
//Author:  PATNI\ARD
//Purpose: Recalculate the graph points, which will actually draw the graph
//**************************************************************************************/
void CPQMCFADlg::shimRefreshCurve(const PreScanCFCurve_p	f_curve_p)const
{
    //-------------------------------------------------------------------------
    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::shimRefreshCurve");
    PQM_TRACE_SCOPED(FUNC_NAME);


    if (f_curve_p == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_curve_p pointer is NULL"));
        return;
    }

    if (f_curve_p->m_data == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_curve_p->m_data pointer is NULL"));
        return;
    }

    /* FFT Pitch, 3ppm点を計算する。
     * centerFreq は MHz単位なので、以下により3ppmを得る。
     * 3ppm = 3 * centerFreq(Hz)
     */
    flt64_t fftpitch = 1.0 / (flt64_t)f_curve_p->m_adcpitch * 1.0e6 /	(flt64_t)f_curve_p->m_fftpoints;
    flt64_t ppm_range = f_curve_p->m_centerfreq / fftpitch;

    //Patni-HEMANT/ADDED On 10/11/2009 / MVC005007
    const double l_actual_range = CPQMGraphCtrl::ShimWindow.x[1] / CF_SHOW_PPM;


    const int ppm3 = (int)(0.5 + 3.5 * l_actual_range);
    CPQMGraphCtrl::ShimWindow.scale[0].x = (float)(-ppm3 - CPQMGraphCtrl::ShimWindow.x[0]);
    CPQMGraphCtrl::ShimWindow.scale[1].x = (float)(ppm3 - CPQMGraphCtrl::ShimWindow.x[0]);

    const int center = CPQMGraphCtrl::ShimWindow.cf_drawPoint / 2;

    /* カーブデータのコピー */
    int half = PRESCAN_CF_CURVE_DATA_CENTER - center;

    if (CPQMGraphCtrl::ShimWindow.shimFlag == VFC_SHIM_FSY_FAT) {
        half += ppm3;
    }

    //+Patni-HEMANT/Modified On 10/11/2009 / MVC005007
    if (CPQMGraphCtrl::ShimWindow.ppm_offset < 0)
        half -= (int)(0.5 - (flt32_t)CPQMGraphCtrl::ShimWindow.ppm_offset * l_actual_range / 10.0);

    else
        half += (int)(0.5 + (flt32_t)CPQMGraphCtrl::ShimWindow.ppm_offset * l_actual_range / 10.0);

    //-Patni-HEMANT/Modified On 10/11/2009 / MVC005007


    for (int i = 0; i < CPQMGraphCtrl::ShimWindow.cf_drawPoint; i++) {
        if ((half + i) < 0 || (half + i) > f_curve_p->m_numpoints)
            CPQMGraphCtrl::ShimWindow.cf_drawP[i] = 0.0;

        else
            CPQMGraphCtrl::ShimWindow.cf_drawP[i] = f_curve_p->m_data[half + i];
    }

    /* 水脂肪のピーク位置を求める */
    if (CPQMGraphCtrl::ShimWindow.shimFlag == VFC_SHIM_FSY_FAT) {
        CPQMGraphCtrl::ShimWindow.peak[0].sw = 1;
        CPQMGraphCtrl::ShimWindow.peak[1].sw = 1;

        CPQMGraphCtrl::ShimWindow.peak[1].x = -ppm3 - CPQMGraphCtrl::ShimWindow.x[0];
        CPQMGraphCtrl::ShimWindow.peak[0].x = - CPQMGraphCtrl::ShimWindow.x[0];

        CPQMGraphCtrl::ShimWindow.peak[1].y = CPQMGraphCtrl::ShimWindow.cf_drawP[center - ppm3];
        CPQMGraphCtrl::ShimWindow.peak[0].y = CPQMGraphCtrl::ShimWindow.cf_drawP[center];

    } else {
        CPQMGraphCtrl::ShimWindow.peak[0].sw = 1;
        CPQMGraphCtrl::ShimWindow.peak[1].sw = 0;

        CPQMGraphCtrl::ShimWindow.peak[0].x = - CPQMGraphCtrl::ShimWindow.x[0];

        CPQMGraphCtrl::ShimWindow.peak[1].x = 0;

        CPQMGraphCtrl::ShimWindow.peak[0].y = CPQMGraphCtrl::ShimWindow.cf_drawP[center];
    }

    /* データの最大値と最小値を取得する */
    CPQMGraphCtrl::ShimWindow.y[1] = CPQMGraphCtrl::ShimWindow.y[0] = CPQMGraphCtrl::ShimWindow.cf_drawP[0];

    for (i = 1; i < CPQMGraphCtrl::ShimWindow.cf_drawPoint; i++) {
        if (CPQMGraphCtrl::ShimWindow.cf_drawP[i] > CPQMGraphCtrl::ShimWindow.y[1])
            CPQMGraphCtrl::ShimWindow.y[1] = CPQMGraphCtrl::ShimWindow.cf_drawP[i];

        if (CPQMGraphCtrl::ShimWindow.cf_drawP[i] < CPQMGraphCtrl::ShimWindow.y[0])
            CPQMGraphCtrl::ShimWindow.y[0] = CPQMGraphCtrl::ShimWindow.cf_drawP[i];
    }

    CPQMGraphCtrl::ShimWindow.x[2] = CPQMGraphCtrl::ShimWindow.x[1] - CPQMGraphCtrl::ShimWindow.x[0];
    CPQMGraphCtrl::ShimWindow.y[2] = CPQMGraphCtrl::ShimWindow.y[1] - CPQMGraphCtrl::ShimWindow.y[0];
    //
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //-------------------------------------------------------------------------
}

//-Patni-SS/ADDED/2009-May-13/JFT#137
//Patni-ARD/20090315/Added/ACE-1 # Prescan Start
//************************************Method Header**************************************
//Method Name:  shimAllocCurve
//Author:  PATNI\ARD
//Purpose: Initializition & allocation for curve data
//**************************************************************************************/
void CPQMCFADlg::shimAllocCurve()const
{
    CPQMGraphCtrl::ShimWindow.x[0] 	 =  -75;	//minx
    CPQMGraphCtrl::ShimWindow.x[1] 	 =   75;	//maxx
    CPQMGraphCtrl::ShimWindow.cf_drawP 	 = NULL;
    CPQMGraphCtrl::ShimWindow.cf_drawPoint =  151;	/* -75 -- 0 -- 75 */

    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    CPQMGraphCtrl::ShimWindow.num_scale	 =    2;	//	  no need as it is already drawn the lines for the scale
    CPQMGraphCtrl::ShimWindow.centerFreq = 0.0;
    CPQMGraphCtrl::ShimWindow.ppm_offset = 0;

    //XmTextSetString(CPQMGraphCtrl::ShimWindow.adjust.value,"0");
    ((CEdit*)GetDlgItem(IDC_EDIT_CFA))->SetWindowText(_T("0.0"));
    //
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413

    CPQMGraphCtrl::ShimWindow.peak[0].str = "W";
    CPQMGraphCtrl::ShimWindow.peak[1].str = "F";

    int dataSize = sizeof(double) * CPQMGraphCtrl::ShimWindow.cf_drawPoint;
    CPQMGraphCtrl::ShimWindow.cf_drawP = (double*)malloc(dataSize);

    if (CPQMGraphCtrl::ShimWindow.cf_drawP == NULL)
        return;

    bzero(CPQMGraphCtrl::ShimWindow.cf_drawP, dataSize);

    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    CPQMGraphCtrl::ShimWindow.scale = (shimAxisScale_t*)calloc(CPQMGraphCtrl::ShimWindow.num_scale, sizeof(shimAxisScale_t));
    CPQMGraphCtrl::ShimWindow.scale[0].str = strdup("-3.5ppm");
    CPQMGraphCtrl::ShimWindow.scale[1].str = strdup("3.5ppm");
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
}



//************************************Method Header**************************************
//Method Name:  shimAllocCurve3T
//Author:  PATNI\ARD
//Purpose: According to Vantage 9.30/Reads the CF.curve.Method file, Reset the number of points for the graph
//**************************************************************************************/
bool CPQMCFADlg::shimAllocCurve3T()
{
    PreScanCFMethod_t	CFmethod;
    memset(&CFmethod, 0, sizeof(PreScanCFMethod_t));	//Patni-PP/2009Sep14/Modified/cpp test corrections
    PreScanReadCFConfig(PRESCAN_CF_DAILY, &CFmethod);

    double RpixelBW = (pow(10, 6) / CFmethod.adcPitch) / (CFmethod.readoutSample * CFmethod.overSampleRate);

    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    //Rpixelppm = RpixelBW / m_center_freq;
    double Rpixelppm = RpixelBW / CPQMGraphCtrl::ShimWindow.centerFreq;
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413

    CPQMGraphCtrl::ShimWindow.x[1] = CF_SHOW_PPM / Rpixelppm;
    CPQMGraphCtrl::ShimWindow.x[0] = CPQMGraphCtrl::ShimWindow.x[1] * -1;

    CPQMGraphCtrl::ShimWindow.cf_drawPoint = (int)(CPQMGraphCtrl::ShimWindow.x[1]) * 2 + 1;	//changed for minimizing the points which are responsible to draw the bottom line.Now it is as per vantage

    FREE_PTR(CPQMGraphCtrl::ShimWindow.cf_drawP)

    int dataSize = sizeof(double) * CPQMGraphCtrl::ShimWindow.cf_drawPoint;
    CPQMGraphCtrl::ShimWindow.cf_drawP = (double*)malloc(dataSize);

    if (CPQMGraphCtrl::ShimWindow.cf_drawP == NULL)
        return false;

    bzero(CPQMGraphCtrl::ShimWindow.cf_drawP, dataSize);

    //+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
    FREE_PTR(CPQMGraphCtrl::ShimWindow.cf_curveP)


    dataSize = sizeof(CPoint) * CPQMGraphCtrl::ShimWindow.cf_drawPoint;
    CPQMGraphCtrl::ShimWindow.cf_curveP = (CPoint*)malloc(dataSize);

    if (CPQMGraphCtrl::ShimWindow.cf_curveP == NULL) {
        FREE_PTR(CPQMGraphCtrl::ShimWindow.cf_drawP)
        return false;
    }

    bzero(CPQMGraphCtrl::ShimWindow.cf_curveP, dataSize);
    return true;
    //-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
}



//************************************Method Header**************************************
//Method Name:  PreScanReadCFConfig
//Author:  PATNI\ARD
//Purpose: Reads the CF.curve.Method file
//**************************************************************************************/
void CPQMCFADlg::PreScanReadCFConfig(const int f_type, const PreScanCFMethod_p f_CFmethod)const
{

    LPCTSTR FUNC_NAME = _T("CPQMCFADlg::PreScanReadCFConfig");

    CString str_File_Name = (f_type == PRESCAN_CF_DAILY) ? PRESCAN_CF_METHOD_DAILY : PRESCAN_CF_METHOD_RAMPUP ;

    int l_len = str_File_Name.GetLength();

    char* file_name = new char[l_len + 1];
    memset(file_name, 0, l_len + 1);
    wcstombs(file_name, str_File_Name, l_len);

    char* path = NULL;

    if ((path = getenv("GP_DATA")) == NULL)
        path = ".";

    char   strfilename[FILENAME_MAX] = {0};//Patni-DKH/2009Aug24/Modified/cpp test corrections
    sprintf(strfilename, "%s\\preScan\\%s", path, file_name);

    DEL_PTR_ARY(file_name)

    WCHAR str[FILENAME_MAX] = {0};//Patni-DKH/2009Aug24/Modified/cpp test corrections
    memset(str, NULL, FILENAME_MAX);
    mbstowcs(str, strfilename, strlen(strfilename));

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, str);


    FILE* file = fopen(strfilename, "r");

    if (file == NULL) {
        return;
    }

    CString strline = "";
    char tmpline[MAX_LINE] = {'\0'};
    char* pt1 = NULL;
    char* pt2 = NULL;
    char* outstr = NULL;

    while (!feof(file)) {
        fgets(tmpline, MAX_LINE, file);
        strline.Format(_T("%s"), tmpline);

        // get the first token
        if ((pt1 = strtok(tmpline, " \t\n")) == NULL) {
            continue;
        }

        // get the second token
        if ((pt2 = strtok(NULL, " \t\n")) == NULL) {
            continue;
        }

        // get the CF value
        if (!strcmp(pt1, PSK_OVER_SAMPLE_RATE)) {
            f_CFmethod->overSampleRate = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_ADC_PITCH)) {
            f_CFmethod->adcPitch = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_READOUT_SAMPLE)) {
            f_CFmethod->readoutSample = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_START_POINT)) {
            f_CFmethod->startPoint = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_END_POINT)) {
            f_CFmethod->endPoint = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_CENTER_POINT)) {
            f_CFmethod->centerPoint = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_SHIFT_DIRECTION)) {
            f_CFmethod->shiftDirection = (int)strtod(pt2, &outstr);

        } else if (!strcmp(pt1, PSK_NUM_CF_AVG)) {
            f_CFmethod->numCFAvg = (int)strtod(pt2, &outstr);
        }

        continue;

    }

    fclose(file);
}

//************************************Method Header**************************************
//Method Name:  PqmCFARedraw
//Author:  PATNI\Hemant
//Purpose: Reads the CF.curveData file, Recalculate the points for graph
//**************************************************************************************/
void CPQMCFADlg::PqmCFARedraw(
    const bool f_ppm_reset /*= true*/
)
{
    if (f_ppm_reset) {
        CPQMGraphCtrl::ShimWindow.ppm_offset = 0;
        ((CEdit*)GetDlgItem(IDC_EDIT_CFA))->SetWindowText(_T("0.0"));
    }

    PreScanCFCurve_p l_curve_p = new SPreScanCFCurve();
    PreScanReadCFCurveData(l_curve_p);

    CPQMGraphCtrl::ShimWindow.centerFreq = (float) l_curve_p->m_centerfreq;

    shimAllocCurve3T();		/* V9.30 */

    //here in vantage following is happening
    //1) Get the height and width of graph - done in onpaint of graph
    //2) set the color data for the graph - not to be done
    //3) Calculate the rect in which to draw

    shimRefreshCurve(l_curve_p);

    DEL_PTR(l_curve_p)

    m_cfagraphctrl.Invalidate();
}

//************************************Method Header************************************
// Method Name  : SetButtonAppearence
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::SetButtonAppearence(
)
{
    SetButtonFont();
    SetBitmapsForButtons();

    m_cfa_left.SetCurveType(CURVE_SMALL_3);
    m_cfa_right.SetCurveType(CURVE_SMALL_3);


    m_ok.SetCurveType(CURVE_SMALL_3);
    m_apply.SetCurveType(CURVE_SMALL_3);
    m_utility.SetCurveType(CURVE_SMALL_3);
    m_cfa_retry.SetCurveType(CURVE_SMALL_3);

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    m_cancel.SetCurveType(CURVE_SMALL_3);


    //Modified by Hemant .. now bk will be according to dialog bk color..
    //m_label_excitation.LoadDefaultColors();
    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    //Added by Hemant On 12/11/2008 12:20:24 PM
    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    m_label_excitation.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_label_excitation.SetFontAttributes(button_font_name, 100);
    m_label_excitation.UpdateFont();

    m_water.SetBkColor(bk_color);
    m_fat.SetBkColor(bk_color);

    //Added by Hemant On 12/11/2008 12:20:20 PM
    const int size = 9;
    m_water.SetFont(button_font_name, size);
    m_fat.SetFont(button_font_name, size);
}

//************************************Method Header************************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCFADlg::SetBitmapForButton(
    ButtonEx& button_object,
    const UINT bitmap_resource_id,
    const bool center_bitmap /*= true*/
)
{
    return CUIButtonUtility::GetInstance()->SetBitmapForButton(button_object, bitmap_resource_id, RGB(0, 255, 0), center_bitmap);
}

//+Patni-PJS/Modified/1Aug2009/Internal defect solved


// Language usha
void CPQMCFADlg::SetMultilingual()
{
    //dialog title
    CString l_chTitle = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CFA_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_STATIC_EXCITATIONN_SELECTION"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_EXCITATIONN_SELECTION);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_RADIO_BUTTON_WATER"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_RADIO_BUTTON_WATER);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_RADIO_BUTTON_FAT"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_RADIO_BUTTON_FAT);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_CFA_RETRY"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_CFA_RETRY);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_UTILITY"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_UTILITY);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_APPLY"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_APPLY);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_OK")); // Changed By KT/ACE-2/11-06-2009/Multilingualisation
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CANCEL_BUTTON")); // Added By KT/ACE-2/11-06-2009/Multilingualisation
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    //-------------------------------------------------------------------------
    //Commented by Hemant On 12/11/2008 11:15:53 AM
    //	l_chOutput =  MPlus::I18n::CCatalog::
    //								instance().get(_T("IDS_PQM_BUTTON_CFA_LEFT"));
    //	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_BUTTON_CFA_LEFT);
    //	if (_tcscmp(l_chOutput ,_T("")) != 0)
    //	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    //	l_chOutput = _T("");
    //	l_buttonUserHwnd = NULL;
    //
    //	l_chOutput =  MPlus::I18n::CCatalog::
    //								instance().get(_T("IDS_PQM_BUTTON_CFA_RIGHT"));
    //	l_buttonUserHwnd = ::GetDlgItem(m_hWnd,IDC_BUTTON_CFA_RIGHT);
    //	if (_tcscmp(l_chOutput ,_T("")) != 0)
    //	{::SetWindowText(l_buttonUserHwnd, l_chOutput);}
    //	l_chOutput = _T("");
    //	l_buttonUserHwnd = NULL;
    //-------------------------------------------------------------------------

}

/****************************Method Header************************************
//Module         : SetBtnSensitive
//Author         : PATNI/SG
//Purpose        : Set the Button State Sensitive/Insensitive
//*****************************************************************************/
void CPQMCFADlg::SetBtnSensitive(
    const int f_btnid,
    const bool f_state
)
{
    ((CButton*)GetDlgItem(f_btnid))->EnableWindow(f_state);
}


//************************************Method Header************************************
// Method Name  : SetBitmapsForButtons
// Author       : PATNI/ HEMANT
// Purpose      : this function is called initially, to set the bitmaps to the bimap buttons
//***********************************************************************************
void CPQMCFADlg::SetBitmapsForButtons(
)
{
    //Note : prev two images were set for the CBitmapButton, one for normal bitmap, second for
    //button selection..//not there will be only one iimage on button..
    //the custom button used don't provide any other functionality to chave hover effect
    //or other effect for bitmap buttons..

    // load bitmaps on buttons
    m_cfa_left.SetWindowText(_T(""));
    m_cfa_right.SetWindowText(_T(""));
    SetBitmapForButton(m_cfa_left, IDB_BITMAP_CFA_LEFT) ;
    SetBitmapForButton(m_cfa_right, IDB_BITMAP_CFA_RIGHT) ;
}


//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMCFADlg::SetButtonFont(
)
{
    LPCTSTR button_font_name = _T("Arial BOLD") ;
    const int size = 12;

    m_ok.SetPrimaryTextFont(button_font_name, size);
    m_ok.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_apply.SetPrimaryTextFont(button_font_name, size);
    m_apply.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_utility.SetPrimaryTextFont(button_font_name, size);
    m_utility.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);
    m_cfa_retry.SetPrimaryTextFont(button_font_name, size);
    m_cfa_retry.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_c_edit_cfa.SetFont(button_font_name, size);

    //+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
    m_cancel.SetPrimaryTextFont(button_font_name, size);
    m_cancel.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    return TRUE ;
}





//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMCFADlg::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_ok.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);
    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 5);
}



//+Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130
//************************************Method Header************************************
// Method Name  :SetStateForCtrl()
// Author       :PATNI/ HEMANT
// Purpose      :Set State For Ctrl
//***********************************************************************************
void CPQMCFADlg::SetStateForCtrl(
    CWnd* wnd,
    const pqm_msg_tag f_msg
)const
{
    //+Patni-HAR-RJ/2009Mar31/Added
    if (wnd && wnd->GetSafeHwnd()) {

        switch (CControlStateMgr::GetState(f_msg, wnd->GetDlgCtrlID())) {
            case STATE_ENABLE:
                wnd->EnableWindow(true);
                break;

            case STATE_DISABLE:
                wnd->EnableWindow(false);
                break;

            case STATE_VISIBLE:
                wnd->ShowWindow(true);
                break;

            case STATE_INVISIBLE:
                wnd->ShowWindow(false);
                break;

            case STATE_VISIBLE_ENABLE:
                wnd->ShowWindow(true);
                wnd->EnableWindow(true);
                break;

            case STATE_VISIBLE_DISABLE:
                wnd->ShowWindow(true);
                wnd->EnableWindow(false);
                break;

            case STATE_DEFAULT:
            default:
                break;
        }
    }
}
//-Patni-Hemant/2009Mar17/ADDED/ACE-1/MCM0228-00102, MCM0213-00130


//+Patni-HEMANT/ADDED On 4/23/2009 10:51:51 AM/ ACE / ITC-2 Defect Fix
void CPQMCFADlg::SetFocusOnNextItemAfterScanDoneorAbort(
)const
{
    if (g_MainView->GetShimDoneFlag() == CFA_DLG_SHIMMING_SEQUENCE) {

        g_MainView->SelectAndHighlightFirstWaitProtocol();
        g_MainView->SetIsScanning(false);
    }
}
//-Patni-HEMANT/ADDED On 4/23/2009 10:51:51 AM/ ACE / ITC-2 Defect Fix

