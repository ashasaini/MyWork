#if !defined(AFX_PDERRORMSGDLG_H__433941F5_EDDD_4BD8_9EFB_300CB1849572__INCLUDED_)
#define AFX_PDERRORMSGDLG_H__433941F5_EDDD_4BD8_9EFB_300CB1849572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PDErrorMsgDlg.h : header file
//
#include "resource.h"
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>


using namespace MPlus::UICommon;
/////////////////////////////////////////////////////////////////////////////

typedef enum{
    AUTO_COUCH,
    AUTO_PLAN,
    MANUAL
}Mode;

// CPDErrorMsgDlg dialog

class CPDErrorMsgDlg :  public DialogEx
{
// Construction
public:
    CPDErrorMsgDlg(CWnd* pParent = NULL);   // standard constructor
    void SetErrorMessage(CString &f_error);
    enum { IDD = IDD_ERROR_DLG };
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Implementation
protected:

     virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:
    ButtonEx m_buttonOk;
    CStatic m_errorctrl;
    /**
    * This variable is used to store error message.
    */
    CString m_error_msg;
    CBrush m_list_brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDERRORMSGDLG_H__433941F5_EDDD_4BD8_9EFB_300CB1849572__INCLUDED_)
