#if !defined(AFX_TIPREPPROGRESSDLG_H__DF3B29F1_212A_4999_A4EA_D9D7BC15E4E2__INCLUDED_)
#define AFX_TIPREPPROGRESSDLG_H__DF3B29F1_212A_4999_A4EA_D9D7BC15E4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PopupDlg.h"
#include "CPercentBar.h"
#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////
// CTiPrepProgressDlg dialog

class CTiPrepProgressDlg : public CPopupDlg
{
// Construction
public:
	HANDLE *m_threads;
	void SetThreadHandles(HANDLE *hnd_arr);
	int m_nrange;
	void SetRange(int range);
	void UpdateProgress(BOOL);
	void SetTotalThreadsRunning(int);
	CTiPrepProgressDlg(CWnd* pParent = NULL);   // standard constructor
    ~CTiPrepProgressDlg();
    virtual void OnOK();
// Dialog Data
	//{{AFX_DATA(CTiPrepProgressDlg)
	enum { IDD = IDD_PROGRESS_DIALOG };
	CPercentBar	m_ctrlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTiPrepProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTiPrepProgressDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_ntotalthreads;
	int m_ncurrentthreads;
    int m_nProgressVal;
	    CCriticalSection m_CriticalSection; //AN/Revw/No global objects here, usage is required for progress dialog UpdateProgress method, not here

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPREPPROGRESSDLG_H__DF3B29F1_212A_4999_A4EA_D9D7BC15E4E2__INCLUDED_)
