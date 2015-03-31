// ErrorDlg.h: interface for the CErrorDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORDLG_H__41CE7D1B_7FAD_42D9_997B_8FEA1C0DEF64__INCLUDED_)
#define AFX_ERRORDLG_H__41CE7D1B_7FAD_42D9_997B_8FEA1C0DEF64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PopupDlg.h"
#include "resource.h"
#include <MPlusUICommon/ButtonEx.h>
/**
* This class is used Display error messge in user defined dialog class.
*/
class CErrorDlg : public CPopupDlg
{
// Construction
public:
    CErrorDlg(CWnd* pParent = NULL);   // standard constructor

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
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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
    CBrush m_list_brush;
    CBitmap Bmp1;
public:
	void ShowErrorMessage(); 
    HWND m_baseHwnd; 
    void SetHWnd(HWND h);
    void SetErrorMessage(CString f_error);
    void SetErrorValue(int ErrorValue);
};
#endif // !defined(AFX_ERRORDLG_H__41CE7D1B_7FAD_42D9_997B_8FEA1C0DEF64__INCLUDED_)
