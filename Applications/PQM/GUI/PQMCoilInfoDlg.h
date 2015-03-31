//REDMINE_782_CPP_Test_Tool
//Redmine-783
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMCoilInfoDlg.h
 *  Overview: Structure of CPQMCoilInfoDlg class.
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
#ifndef __SM_PQM_COIL_INFO_DLG_H__
#define __SM_PQM_COIL_INFO_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "CustomStatic.h"

using namespace MPlus::UICommon ;


const int TOTAL_COLUMNS = 1;

class CPQMView;

class CPQMCoilInfoDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMCoilInfoDlg>
{
public:
    explicit CPQMCoilInfoDlg(ConnectedCoilList* f_coildata_list, CWnd* pParent = NULL);   // standard constructor

protected:
    //{{AFX_VIRTUAL(CPQMCoilInfoDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMCoilInfoDlg)
    virtual void OnCancel();
    //}}AFX_MSG

private:

    // Dialog Data
    //{{AFX_DATA(CPQMCoilInfoDlg)
    enum { IDD = IDD_DLG_COIL };
    CCustomStatic m_port;
    CCustomStatic m_name;
    CCustomStatic m_id;
    ButtonEx m_button_close;
    ListCtrlEx m_c_list_coil;
    //}}AFX_DATA
    //Patni-PJS/2010Apr05/Added/MSA0251-00280 review comment
    ConnectedCoilList* m_coildata_list;
    CPQMView* m_parent;
    ButtonEx m_close;

    //{{AFX_VIRTUAL(CPQMCoilInfoDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void DisplayCoilInfo();
    void InsertColumnsInCoilInfoList();
    void SetMultilingual();

    void SetButtonAppearence();
    BOOL SetButtonFont();
    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_COIL_INFO_DLG_H__
