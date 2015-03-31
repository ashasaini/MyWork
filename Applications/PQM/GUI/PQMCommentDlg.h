//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMCommentDlg.h
 *  Overview: Structure of CPQMCommentDlg class.
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
#ifndef __SM_PQM_COMMENT_DLG_H__
#define __SM_PQM_COMMENT_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "CustomStatic.h"
#include "DailogInterfaces.h"

using namespace MPlus::UICommon ;


//Patni-KSS/2010Jul20/ADDED/TMSC-REDMINE-453
const unsigned int BACKSLASH = 92;

class CScanCommGeneration;

class CPQMCommentDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMCommentDlg>
{
public:
    CPQMCommentDlg(CBaseInterfaceClassForScanCommentDlg* f_interfaceclassforscancommentdlg, ScanCommentXMLData* f_scan_dialog_data, CWnd* pParent = NULL);   // standard constructor
    ~CPQMCommentDlg();

    void UpdateAndScanComment(const CString& f_str);
    void ChangeViewOfCoilListDlg(const bool f_check)const ;
    void ShowHideOpenedWindowsOnPageChanged(const bool f_show = true);
    void DisplayCommentInformation();
    void SetWaitCursor(const bool f_show_wait_cursor);
    // Overrides
    // ClassWizard generated virtual function overrides
protected:
    //{{AFX_VIRTUAL(CPQMCommentDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMCommentDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnCancel();
    virtual BOOL PreTranslateMessageImpl(MSG* msg);
    virtual void OnOK();
    virtual void OnDestroyImpl();
    afx_msg void OnCopyToComment();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private:
    // Dialog Data
    //{{AFX_DATA(CPQMCommentDlg)
    enum { IDD = IDD_COMMENT };

    CCustomStatic m_label_id;
    CCustomStatic m_label_comment;

    EditCtrlEx m_c_scan_comment;
    EditCtrlEx m_c_scan_id;

    //}}AFX_DATA

    CScanCommGeneration* m_scancomm_generation;
    ScanCommentXMLData* m_scan_dialog_data;
    bool m_store_state_for_scan_comm_gen;

    CBaseInterfaceClassForScanCommentDlg* m_interfaceclassforscancommentdlg ;

    ButtonEx	m_close;
    ButtonEx	m_ok;  //Patni//Manishkumar//2009May15//JFT-CR-41

    bool m_isJpInputEnter; //Tanoue/MVC5849
    bool m_show_wait_cursor_for_commentdlg;


    void UpdateScanCommentInformation();
    void LoadScanCommentDialog();
    void OnScanIdEditRButtonDown();
    void EnableDisableScanCommentCtrls()const;
    void SaveScanId()const;
    void SaveScanComment()const;
    void SetMultilingual();

    BOOL SetButtonFont();
    void SetButtonAppearence();
    void SetDialogRibbonHeight();

    bool GetCurrentScanCommentAndScanID(CString& f_scan_id, CString& f_scan_comment)const;
    CString GetCurrentScanComment()const;
    CString GetCurrentScanId()const;

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_COMMENT_DLG_H__
