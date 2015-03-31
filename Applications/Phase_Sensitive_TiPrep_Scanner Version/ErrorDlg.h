/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_ERRORDLG_H__8B26E637_679E_4611_BA35_D25975A2BD86__INCLUDED_)
#define AFX_ERRORDLG_H__8B26E637_679E_4611_BA35_D25975A2BD86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
/**
* This class is used Display error messge in user defined dialog class.
*/
class CErrorDlg : public CPopupDlg
{
// Construction
public:
    CErrorDlg(CWnd* pParent = NULL);   // standard constructor
	~CErrorDlg();
//     CErrorDlg(CErrorDlg& f_ErrorDlg);
// Dialog Data
    //{{AFX_DATA(CErrorDlg)
    enum { IDD = IDD_DIALOG_ERROR };
    //}}AFX_DATA
    virtual void OnCancel();
    virtual void OnOK();


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CErrorDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CErrorDlg)
	HWND m_baseHwnd;
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnShowWindow(/*BOOL bShow, UINT nStatus*/);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    HICON m_hIcon;
    ButtonEx m_buttonOk;
    CStatic m_errorctrl;
    /**
    * This variable is used to store error message.
    */
    CString m_error_msg;
    CString m_csTitle;
    CBrush m_list_brush;
    //CBitmap Bmp1;
public:
	void ShowErrorMessage(); 
    void SetHWnd(HWND h);
    void SetErrorMessage(CString& f_error);
    void SetDlgTitle(CString& f_csTitle);
    void SetErrorValue(int ErrorValue);
    CErrorDlg& operator = (CErrorDlg& f_ErrorDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORDLG_H__8B26E637_679E_4611_BA35_D25975A2BD86__INCLUDED_)
