/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_ROICOPYDLG_H__1275125A_9BF4_46D7_88C2_3A43EC5233C4__INCLUDED_)
#define AFX_ROICOPYDLG_H__1275125A_9BF4_46D7_88C2_3A43EC5233C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PopupDlg.h"
#include "DisplayData.h"
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
/**
* Class is used copy ROI to other protocol.
*/

class CROICopyDlg : public CPopupDlg
{
// Construction
public:
    CROICopyDlg(CWnd* pParent = NULL);   // standard constructor
    CROICopyDlg& operator = (CROICopyDlg& f_CopyRoiDlg_ptr);
	CROICopyDlg(CROICopyDlg& f_roicopydlg);

    ~CROICopyDlg();
// Dialog Data
    //{{AFX_DATA(CROICopyDlg)
	enum { IDD = IDD_DIALOG_COPYROI };
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CROICopyDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CROICopyDlg)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    afx_msg void OnButtonRoicopy();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    HICON m_hIcon;
    ListCtrlEx m_listctrl_scannedProtocol;
    ButtonEx m_button_copy;
    ButtonEx m_button_exit;
    int m_nTotalScannedProt;
    int m_nTotalScannedSeries;
    vector <CString> m_csSeriesLoid;
    vector <long> m_nsSeriesNumberArray;
    CStatic	m_label_source_prot;
    EditCtrlEx	m_edit_source_prot;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROICOPYDLG_H__1275125A_9BF4_46D7_88C2_3A43EC5233C4__INCLUDED_)
