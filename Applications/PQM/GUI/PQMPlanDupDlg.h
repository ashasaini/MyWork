//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
#ifndef __SM_PQM_PLAN_DUPDLG_H__
#define __SM_PQM_PLAN_DUPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PQMPlanDupDlg.h : header file

#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

using namespace MPlus::UICommon ;

/////////////////////////////////////////////////////////////////////////////
// CPQMPlanDupDlg dialog

class CPQMPlanDupDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMPlanDupDlg>
{
public:
    explicit CPQMPlanDupDlg(CWnd* pParent = NULL);   // standard constructor
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CPQMPlanDupDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMPlanDupDlg)
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnBtnSave();
    afx_msg void OnBtnClose();
    afx_msg void OnChkSlab();
    afx_msg void OnChkTr();
    afx_msg void OnChkSlicethk();
    afx_msg void OnChkSlicegap();
    afx_msg void OnChkFov();
    afx_msg void OnChkMatrix();
    afx_msg void OnChkNowrap();
    afx_msg void OnChkFlipangle();
    afx_msg void OnFlopangle();
    afx_msg void OnChkPresat();
    afx_msg void OnChkLocimginfo();
    afx_msg void OnChkPrescanCoil(); //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private:
    // Dialog Data
    //{{AFX_DATA(CPQMPlanDupDlg)
    enum { IDD = IDD_PLANDUP_DIALOG };
    //}}AFX_DATA

    int32_t m_currentmask;
    int32_t m_defaultmask;
    int32_t m_flag;
    bool m_show_wait_cursor_for_plandupdlg;

    char    m_studyfile[MAX_PATH];

    CheckBoxEx	m_chkslicegap;
    CheckBoxEx	m_chkflopangle;
    ButtonEx	m_btnsave;
    ButtonEx	m_btnclose;
    CheckBoxEx	m_chkbasicscan;
    CheckBoxEx	m_chkflipangle;
    CheckBoxEx	m_chkfov;
    CheckBoxEx	m_chklocimginfo;
    CheckBoxEx	m_chkmatrix;
    CheckBoxEx	m_chknowrap;
    CheckBoxEx	m_chkpresat;
    CheckBoxEx	m_chkslab;
    CheckBoxEx	m_chkslicethk;
    CheckBoxEx	m_chktr;
    CheckBoxEx  m_chkprescancoil;  //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    //{{AFX_VIRTUAL(CPQMPlanDupDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    bool ReadPlanDuplicateSettings();
    bool InitializeUI();
    bool SaveCurrentmask(int32_t f_mask)const;
    void ToggleCheck(CheckBoxEx& f_checkbox)const;
    int32_t GetCurrentUIMaskSettings()const;
    void SetMultilingual();
    void SetButtonAppearence();
    BOOL SetButtonFont();

    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_PLAN_DUPDLG_H__
