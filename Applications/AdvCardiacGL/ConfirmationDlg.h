#if !defined(AFX_CONFIRMATIONDLG_H__72337CE9_C612_4149_9548_29516D829083__INCLUDED_)
#define AFX_CONFIRMATIONDLG_H__72337CE9_C612_4149_9548_29516D829083__INCLUDED_
#include <MPlusUICommon/DialogEx.h>
using namespace MPlus::UICommon;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfirmationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfirmationDlg dialog

class CConfirmationDlg : public DialogEx
{
// Construction
public:
	CConfirmationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfirmationDlg)
	enum { IDD = IDD_CONFIRMATION_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
    ButtonEx	m_ConfirmOk;
    ButtonEx	m_ConfirmCancel;
	CStatic	m_label;
protected:
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	// Generated message map functions
	//{{AFX_MSG(CConfirmationDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBrush m_ConfirmationDlg_brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIRMATIONDLG_H__72337CE9_C612_4149_9548_29516D829083__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////
