//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-781
//TMSC-Redmine-780
//Redmine-775
//TMSC-REDMINE-777
// PQMManualPreScanDlg.h : header file
//Redmine-774

//Redmine-777
#ifndef __SM_PQM_MANUAL_PRESCAN_DLG_H__
#define __SM_PQM_MANUAL_PRESCAN_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CPQMManualPreScanDlg dialog

#include <MPlusUICommon/ComboBoxEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "CustomStatic.h"
#include "ManualPrescanEdit.h"

using namespace MPlus::UICommon ;


class BaseInterfaceForPQMManualPreScanDlg;

class CPQMManualPreScanDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMManualPreScanDlg>
{

    // Construction
public:
    explicit CPQMManualPreScanDlg(BaseInterfaceForPQMManualPreScanDlg* parent_interface, CWnd* pParent = NULL);	// standard constructor
    ~CPQMManualPreScanDlg();

    BOOL GetManualPreScanDefaultData();
    void SetManualPreScanApplyBtnState(const BOOL f_button_state);
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CPQMManualPreScanDlg)
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL
    // Generated message map functions
    //{{AFX_MSG(CPQMManualPreScanDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private:
    manual_prescan_p m_manual_prescan_values;
    BOOL m_apply_button_state;
    BaseInterfaceForPQMManualPreScanDlg* m_parent_interface ;
    bool m_show_wait_cursor_for_manualprescandlg;


    //{{AFX_DATA(CPQMManualPreScanDlg)
    ComboBoxEx	m_txcoil_combo;
    ComboBoxEx	m_rxcoil_combo;
    ComboBoxEx	m_rfl_combo;
    ComboBoxEx	m_cfa_combo;
    ComboBoxEx	m_rxgain_combo;
    ComboBoxEx	m_vapthrough_combo;
    ComboBoxEx	m_rfl_option_combo;
    ComboBoxEx	m_cfa_option_combo;
    ComboBoxEx	m_rxgain_option_combo;
    CCustomStatic     m_txcoil_static;
    CCustomStatic     m_rxcoil_static;
    CCustomStatic     m_rfl_static;
    CCustomStatic     m_cfa_static;
    CCustomStatic     m_rxgain_static;
    CCustomStatic     m_vapthrough_static;
    CManualPrescanEdit       m_rfl_gain_edit;
    CManualPrescanEdit       m_rfl_scale_edit;
    CManualPrescanEdit       m_cfa_fsyvalue_edit;
    CManualPrescanEdit       m_rxgain_rgnvalue_edit;
    ButtonEx				m_apply_button ;
    ButtonEx				m_cancel_button ;
    FrameEx				m_main_static_frame;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPQMManualPreScanDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void LogManualPrescanValues()const;
    void SetButtonAppearence();
    void SetMultilingual();
    void GetControlValues()const;

    void SetPrescanHook(const BOOL f_prescan_hook);
    BOOL SetButtonFont();
    void SetDialogRibbonHeight();

    const CString GetPrescanString(const int f_mode)  const;

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_MANUAL_PRESCAN_DLG_H__
