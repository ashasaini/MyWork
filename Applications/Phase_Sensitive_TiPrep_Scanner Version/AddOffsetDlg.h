/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_ADDOFFSETDLG_H__DAF98C58_127F_46C7_962E_9C5D989038A7__INCLUDED_)
#define AFX_ADDOFFSETDLG_H__DAF98C58_127F_46C7_962E_9C5D989038A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
/////////////////////////////////////////////////////////////////////////////
// CAddOffsetDlg dialog

class CAddOffsetDlg : public CPopupDlg
{
// Construction
public:
    CAddOffsetDlg(CWnd* pParent = NULL);   // standard constructor
//     CAddOffsetDlg(CAddOffsetDlg& f_offsetdlg);
    CAddOffsetDlg& operator = (CAddOffsetDlg& f_offsetdlg);
	CAddOffsetDlg(CAddOffsetDlg& f_offsetdlg);
	~CAddOffsetDlg();
// Dialog Data
    //{{AFX_DATA(CAddOffsetDlg)
    enum { IDD = IDD_DIALOG_ADD_OFFSET };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAddOffsetDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
// Implementation
public:
    void RefreshSelectedSeries(CString &f_csSeriesNumber);
// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CAddOffsetDlg)
    afx_msg void OnButtonProtSelect();
    afx_msg void OnButtonApplyOffset();
    afx_msg void OnCheckAddRoiAvg();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

// Implementation
private:
    void InitializeAddOffsetDlgControls();
//class members
private:
    HICON m_hIcon;
    int m_add_offset_check_box_status;
    ButtonEx m_protocol_select_btn;
    ButtonEx m_apply_offset_btn;
    ButtonEx m_exit_add_offset_btn;
    EditCtrlEx m_edit_protocol;
    EditCtrlEx m_edit_offset;
    EditCtrlEx m_label_protocol;
    EditCtrlEx m_label_offset;
    CheckBoxEx m_add_roi_avg_chkbox;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDOFFSETDLG_H__DAF98C58_127F_46C7_962E_9C5D989038A7__INCLUDED_)
