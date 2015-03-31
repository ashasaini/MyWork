//Redmine-783
//Redmine-782
//Redmine-775
//Redmine-777
#ifndef __SM_PQM_DEBUG_WIN_DLG_H__
#define __SM_PQM_DEBUG_WIN_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PQMDebugWinDlg.h : header file
//
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

//Patni/KSS/2011Mar31/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ThreadMutex.h>

#include "CustomStatic.h"
using namespace MPlus::UICommon ;
/////////////////////////////////////////////////////////////////////////////
// CPQMDebugWinDlg dialog


class CPQMDebugWinDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMDebugWinDlg>
{
    // Construction
public:
    explicit CPQMDebugWinDlg(CWnd* pParent = NULL);   // standard constructor
    void UpdateDebugData(const CString& f_csdebugdata);

    //+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    void InsertDebugStr(const CString& f_csdebugdata);
    void UpdateDebugWnd();
    //-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CPQMDebugWinDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMDebugWinDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnCancel();
    afx_msg void OnGlbdata();
    afx_msg void OnClear();
    afx_msg void OnAcqmon();
    afx_msg void OnListcnt();
    afx_msg void OnListarr();
    afx_msg void OnPortinfo();
    afx_msg void OnCouchinfo();
    afx_msg void OnBtnsize();
    afx_msg void OnSar();
    afx_msg void OnShim();
    afx_msg void OnListmon();
    afx_msg void OnProlimmon();
    afx_msg void OnSarmon();
    afx_msg void OnIrsmon();
    afx_msg void OnMonoff();
    afx_msg void OnAcqinfo();
    afx_msg void OnStudydata();
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private:

    //Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    typedef CList <CString, CString> CLogStrList;

    // Dialog Data
    //{{AFX_DATA(CPQMDebugWinDlg)
    enum { IDD = IDD_PQMDEBUG_DIALOG };
    CCustomStatic	m_listinfo;
    CCustomStatic	m_protocoldata;
    ButtonEx m_close;
    ButtonEx m_shimming;
    ButtonEx m_sar_mon;
    ButtonEx m_sar;
    ButtonEx m_prolim_mon;
    ButtonEx m_prot_info;
    ButtonEx m_monitor_off;
    ButtonEx m_list_mon;
    ButtonEx m_list_cnt;
    ButtonEx m_list_array;
    ButtonEx m_irs_mon;
    ButtonEx m_clear;
    ButtonEx m_size;
    ButtonEx m_acq_mon;
    ButtonEx m_acq_info;
    ListCtrlEx m_debug_info;
    ButtonEx m_study_data;
    ButtonEx m_couch_info;
    ButtonEx m_global_data;
    //}}AFX_DATA

    int			m_buttonmask;
    long		m_debuglinecount;
    bool 		m_show_wait_cursor_for_debugwindlg;

    //+Patni/KSS/2011Mar31/Added/TMSC-REDMINE-1559
    //+Patni-Sribanta/2011Jan12/Added/TMSC-REDMINE-1026
    //+Patni-Rajeesh/2011Mar22/Modified/Using_LibStudyManager
    //CRITICAL_SECTION m_cs;
    //MR::ACQ::SM::COMMON::CThreadMutex m_cs;
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_cs;
    //-Patni-Rajeesh/2011Mar22/Modified/Using_LibStudyManager
    //-Patni/KSS/2011Mar31/Added/TMSC-REDMINE-1559

    CLogStrList m_log_list;
    //-Patni-Sribanta/2011Jan12/Added/TMSC-REDMINE-1026

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPQMDebugWinDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    //+Patni-Sribanta/2011Jan12/Added/TMSC-REDMINE-1026
    void PushMsg(const CString& f_msg);
    bool PopMsg(CString& f_msg);
    void ClearDebugStrList();
    //-Patni-Sribanta/2011Jan12/Added/TMSC-REDMINE-1026

    void SetDialogRibbonHeight();

    BOOL SetButtonFont();
    void SetButtonAppearence();
    void SetMultilingual();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_DEBUG_WIN_DLG_H__
