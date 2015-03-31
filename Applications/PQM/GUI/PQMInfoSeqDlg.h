//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
//TMSC-REDMINE-774
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMInfoSeqDlg.h
 *  Overview: Structure of CPQMInfoSeqDlg class.
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
#ifndef __SM_PQM_INFO_SEQ_DLG_H__
#define __SM_PQM_INFO_SEQ_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/EditCtrlEx.h> //Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

using namespace MPlus::UICommon ;

//-------------------------------------------------------------------------

class CPQMInfoSeqDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMInfoSeqDlg>
{
    // Construction
public:
    explicit CPQMInfoSeqDlg(CWnd* pParent = NULL);   // standard constructor
    void DisplaySeqInformation();
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CPQMInfoSeqDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMInfoSeqDlg)
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnButtonClose();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) ;
    afx_msg void OnSize(UINT nType, int cx, int cy) ;
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private:
    static CRect m_orgwindowrect;
    CRect m_org_client_rect;
    bool m_show_wait_for_infoseqdlg;

    // Dialog Data
    //{{AFX_DATA(CPQMInfoSeqDlg)
    enum { IDD = IDD_INFO_SEQ };
    ListCtrlEx m_seq_info_list;

    ButtonEx  m_c_close;
    EditCtrlEx m_seqinfo_edit;

    //}}AFX_DATA

    CWnd* m_parent;

    //{{AFX_VIRTUAL(CPQMInfoSeqDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void UpdateSeqinfoEdit(const int f_seq_index);
    void SaveSeqinfo()const;
    void SetMultilingual();

    void SetButtonAppearence();
    BOOL SetButtonFont();
    void SetDialogRibbonHeight();

    void Resize(const int f_cx , const int f_cy);
    void OffsetChild(const UINT f_ctrlid , const CSize& f_offset);
    void IncrementSizeChild(const UINT f_ctrlid , const CSize& f_inc_size);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_INFO_SEQ_DLG_H__
