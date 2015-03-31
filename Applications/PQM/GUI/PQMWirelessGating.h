#ifndef __SM_PQM_WIRELESS_GATING_H__
#define __SM_PQM_WIRELESS_GATING_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PQMWirelessGating.h : header file

#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/RadioButtonEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "CustomStatic.h"

using namespace MPlus::UICommon ;

/////////////////////////////////////////////////////////////////////////////
// CPQMWirelessGating dialog

class CPQMWirelessGating : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMWirelessGating>
{
public:
    explicit CPQMWirelessGating(
        Wireless_gating_t*  f_wireless_gating,
        CWnd* pParent = NULL);   // standard constructor

    ~CPQMWirelessGating();
    void UpdateOKBtnStatus(const BOOL f_enable);

protected:
    //{{AFX_VIRTUAL(CPQMWirelessGating)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMWirelessGating)
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnBtnOK();
    afx_msg void OnBtnCancel();

    //}}AFX_MSG

private:
    // Dialog Data
    //{{AFX_DATA(CPQMWirelessGating)
    enum { IDD = IDD_WIRELESS_GATING_DIALOG };
    //}}AFX_DATA

    Wireless_gating_t* m_wireless_gating_val;

    CheckBoxEx	m_hGain;
    CheckBoxEx	m_twsupp;
    RadioButtonEx m_filter_cardiac;

    CCustomStatic	m_command;
    CCustomStatic	m_mode;
    CCustomStatic	m_filter;

    ButtonEx	m_ok;
    ButtonEx	m_btncancel;

    bool InitUI();

    void SetMultilingual();
    void SetButtonAppearance();
    BOOL SetButtonFont();
    void SetDialogRibbonHeight();
    void GetSelecValues()const;

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_WIRELESS_GATING_H__
