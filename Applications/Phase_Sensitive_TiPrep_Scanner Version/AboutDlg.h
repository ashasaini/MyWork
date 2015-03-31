#if !defined(AFX_ABOUTDLG_H__CE5989E3_980F_4FA3_B697_E76742025DDA__INCLUDED_)
#define AFX_ABOUTDLG_H__CE5989E3_980F_4FA3_B697_E76742025DDA__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//
#define SYS_SM_CF	"sm.cf"
#define ENV_GP_ETC      "GP_ETC_PATH"
#define TMP_DEF_PATH    "/usr/tmp"
#define ETC_DEF_PATH    "/gp/etc"

#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CPopupDlg
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
     CAboutDlg(CAboutDlg& f_aboutdlg);   //copy constructor
     CAboutDlg& operator= (CAboutDlg& f_aboutdlg); //assignment operator
	 ~CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_DIALOG_ABOUT };
	ButtonEx	m_AboutOk;
	CStatic	m_StaticAbout3;
	CStatic	m_StaticAbout2;
	CStatic	m_StaticAbout1;
	CStatic	m_StaticIcon;
    CFont m_font;
    CBrush m_list_brush;
    CString m_csMpowerVersion;
	//}}AFX_DATA

    HICON m_hIcon;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
    virtual void OnCancel();
    HICON loadIcon(int x, int y);
    HICON loadIcon();
    BOOL ReadMpowerVersion();
    BOOL GetEtcPath(CString &csEtcPath);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__CE5989E3_980F_4FA3_B697_E76742025DDA__INCLUDED_)
