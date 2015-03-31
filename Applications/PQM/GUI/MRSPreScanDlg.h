//REDMINE_782_CPP_Test_Tool
//Redmine-783
//Redmine-777
#ifndef __SM_MRS_PRESCAN_DLG_H__
#define __SM_MRS_PRESCAN_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRSPreScanDlg.h : header file
//

#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "MRSScanListCtrl.h"


class CBaseInterfaceForMRSPreScanDlg;

/////////////////////////////////////////////////////////////////////////////
// CCMRSPreScanDlg dialog

class CMRSPreScanDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog
{
    // Construction
public:
    explicit CMRSPreScanDlg(CBaseInterfaceForMRSPreScanDlg* baseinterfaceformrsprescandlg, const SMRSScan_s* mrsscan_values = NULL, CWnd* pParent = NULL);	// standard constructor
    void ProcessMRSMessages(const pqm_msg_tag f_msg);

    //{{AFX_DATA(CMRSPreScanDlg)
    afx_msg void OnStartMRSPrescan(WPARAM wParam, LPARAM lParam);
    afx_msg void OnAbortMRSPrescan(WPARAM wParam, LPARAM lParam);
    afx_msg void OnAbortButton();
    //}}AFX_DATA

    void SetMRSscanValues(const SMRSScan_s& mrsscan_values);
    void SetMRSscanValues(const SMRSScan_s* mrsscan_values);
    void SetMrsLicensePresent(const bool f_mrs_flag);
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CMRSPreScanDlg)
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CMRSPreScanDlg)
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnStartButton();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
private:
    SMRSScan_s m_mrsscan_values ;
    CMRSScanListCtrl	m_mrsscan_listctrl;
    ButtonEx			m_abort_button;
    ButtonEx			m_start_button;

    MRSScanItem m_mrsitem_array[TOTAL_MRSSCAN_ROWITEM];
    CBaseInterfaceForMRSPreScanDlg* m_baseinterfaceformrsprescandlg ;
    bool m_show_wait_cursor_for_mrsprescandlg;

    //{{AFX_VIRTUAL(CMRSPreScanDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void LogMRSScanValues(const SMRSScan_s* f_mrsscan_values)const;
    void AbortMrsPrescan();
    bool CanAbortMRS();
    void StartMrsPrescan();
    BOOL InitializePQMListCtrl();
    void SetMultilingual();
    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_MRS_PRESCAN_DLG_H__
