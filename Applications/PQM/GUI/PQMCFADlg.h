//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
//TMSC-REDMINE-774
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMCFADlg.h
 *  Overview: Structure of CPQMCFADlg class.
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
#ifndef __SM_PQM_CFA_DLG_H__
#define __SM_PQM_CFA_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/CheckBoxEx.h>

#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "PQMEdit.h"
#include "CustomStatic.h"
#include "PQMGraphCtrl.h"

using namespace MPlus::UICommon ;


// ! The class is used to adjust center frequency
typedef struct SPreScanCFCurve {

    double*   m_data;		//initialize from file	//Patni-ARD/20090315/Added/ACE-1 # Prescan
    int     m_numpoints;    // Number of points
    double  m_centerfreq;   // center frequency
    int     m_adcpitch;     // ADC Pitch
    int     m_fftpoints;    // FFT

    SPreScanCFCurve() {
        m_data = NULL ;
    }

    ~SPreScanCFCurve() {
        FREE_PTR(m_data)
    }

} PreScanCFCurve_t, *PreScanCFCurve_p;

class CPQMCFADlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMCFADlg>
{
public:

    CPQMCFADlg(const CRect& parent_rect = CRect(0, 0, 0, 0), CWnd* pParent = NULL);   // standard constructor
    ~CPQMCFADlg();

    void OnCFAReady();
    void DisplayOffset();
    void ProcessShimmingMsg(const pqm_msg_tag f_pending_message_to_process);
    void ShowHideDialogCtrls(const bool f_hide_disp_controls_explicitly = false);
    void ResetPendingCFAMessage();

    void UpdateCFARetry();

    void DoCFAApply();
    void DoCFARetry();

    pqm_msg_tag GetPendingCFAMessage()const;

    void SetPreviousShimFlag(const int f_prev_shimflag);

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPQMCFADlg)
    //virtual void OnCancel();
    virtual void OnOK();
    virtual void OnCancel();

    afx_msg void OnClickButtonApply();
    afx_msg void OnClickButtonCfaLeft();
    afx_msg void OnClickButtonCfaRight();
    afx_msg void OnClickButtonCfaRetry();
    afx_msg void OnClickRadioButtonFat();
    afx_msg void OnClickRadioButtonWater();
    virtual BOOL OnInitDialogImpl();

    afx_msg void OnClickMenuitemAdjustCenterFrequency();
    afx_msg void OnClickUtility();

    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) ;
    afx_msg void OnSize(UINT nType, int cx, int cy) ;
    //}}AFX_MSG

    // Overrides
    // ClassWizard generated virtual function overrides

    //{{AFX_VIRTUAL(CPQMCFADlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

private:
    // Dialog Data
    //{{AFX_DATA(CPQMCFADlg)
    enum { IDD = IDD_CFA };
    //}}AFX_DATA
    pqm_msg_tag m_pending_message_to_process;

    //{{AFX_DATA(CPQMCFADlg)
    ButtonEx   m_cfa_left;
    ButtonEx   m_cfa_right;
    CPQMEdit m_c_edit_cfa;
    CString m_ppm_offset;
    //}}AFX_DATA


    CRect m_parent_rect ;
    CRect m_dialog_rect;

    int m_prev_shimflag;
    CRect cl_OrgWindowRect;
    bool m_disp_controls;
    ButtonEx m_cfa_retry;
    ButtonEx m_ok;
    ButtonEx m_apply;
    ButtonEx m_utility;
    ButtonEx m_cancel;
    CheckBoxEx m_water;
    CheckBoxEx m_fat;
    CCustomStatic m_label_excitation;
    CPQMGraphCtrl m_cfagraphctrl;

    //{{AFX_VIRTUAL(CPQMCFADlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL


    void DoCFARetrySettings()const;
    BOOL CreateGraphCtrl();

    void fnResize(const int iaCx , const int iaCy);
    void fnOffsetChild(const UINT uiaID , const CSize& claOffset);
    void fnIncrementSizeChild(const UINT uiaID , const CSize& claIncSize);

    void AdjustCancelButtonPos();

    void ValidateOffsetValueAndRedraw(const bool f_redraw = true);
    void CalculateAndSendCenterFrequency();
    void EnableCFAControls(const bool state);
    void PreScanReadCFCurveData(const PreScanCFCurve_p curve_p)const;
    void PendingMessageToProcess(const pqm_msg_tag f_pending_message_to_process);
    void ProcessPendingMessage();
    void shimRefreshCurve(const PreScanCFCurve_p	curve_p)const;
    void shimAllocCurve()const;
    bool shimAllocCurve3T();
    void PreScanReadCFConfig(const int type, const PreScanCFMethod_p CFmethod)const;

    void PqmCFARedraw(const bool f_ppm_reset = true);
    void SetButtonAppearence();
    BOOL SetBitmapForButton(ButtonEx& button_object, const UINT bitmap_resource_id, const bool center_bitmap = true) ;
    void SetMultilingual();

    void SetBtnSensitive(const int f_btnid , const bool f_state);
    void SetBitmapsForButtons();

    BOOL SetButtonFont();

    void SetDialogRibbonHeight();

    void SetStateForCtrl(CWnd* wnd, const pqm_msg_tag f_msg)const;
    void SetFocusOnNextItemAfterScanDoneorAbort()const;

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_CFA_DLG_H__
