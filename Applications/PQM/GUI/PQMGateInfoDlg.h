//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGateInfoDlg.h
 *  Overview: Structure of CPQMGateInfoDlg class.
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
#ifndef __SM_PQM_GATE_INFO_DLG_H__
#define __SM_PQM_GATE_INFO_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

using namespace MPlus::UICommon ;
const int GATE_COLUMN_WIDTH = 500;

class CPQMGateInfoDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMGateInfoDlg>
{
    // Construction
public:
    explicit CPQMGateInfoDlg(CStringArray* gate_str_arr, CWnd* pParent = NULL);   // standard constructor

protected:
    //{{AFX_VIRTUAL(CPQMGateInfoDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    // Generated message map functions
    //{{AFX_MSG(CPQMGateInfoDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnCancel();
    //}}AFX_MSG

private:
    // Dialog Data
    //{{AFX_DATA(CPQMGateInfoDlg)
    enum { IDD = IDD_DLG_GATE };

    ButtonEx m_button_close;
    ListCtrlEx m_c_list_gate;

    //}}AFX_DATA

    //do not delete this in this class as this is not the copy of org.
    CStringArray* m_gate_str_arr ;

    ButtonEx m_close;

    //{{AFX_VIRTUAL(CPQMGateInfoDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL


    void DisplayGateInfo(const CStringArray& str_arr);
    void InsertColumnsInGateInfoList(const CStringArray& str_arr);

    void SetButtonAppearence();
    void SetDialogRibbonHeight();
    BOOL SetButtonFont();
    void SetMultilingual();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_GATE_INFO_DLG_H__
