#if !defined(AFX_OPENRAWDLG_H__70016AF3_FC4D_467A_96E6_6697902BC46D__INCLUDED_)
#define AFX_OPENRAWDLG_H__70016AF3_FC4D_467A_96E6_6697902BC46D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenRawDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenRawDlg dialog

class COpenRawDlg : public CDialog
{
// Construction
public:
	COpenRawDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenRawDlg)
	enum { IDD = IDD_DIALOG_OPEN_RAW };
	CComboBox	m_ComboRawForm;
	CEdit	m_EditRawWidth;
	CEdit	m_EditRawHeight;
	CComboBox	m_ComboRawType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenRawDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenRawDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CString m_csRawFilePath;
public:
    void SetRawFilePath(CString &csFilepath);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENRAWDLG_H__70016AF3_FC4D_467A_96E6_6697902BC46D__INCLUDED_)
