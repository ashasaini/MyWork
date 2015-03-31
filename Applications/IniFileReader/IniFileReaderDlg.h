// IniFileReaderDlg.h : header file
//

#if !defined(AFX_INIFILEREADERDLG_H__D4A3C2F4_4969_4536_8DDC_447DBFCF06AC__INCLUDED_)
#define AFX_INIFILEREADERDLG_H__D4A3C2F4_4969_4536_8DDC_447DBFCF06AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIniFileReaderDlg dialog

class CIniFileReaderDlg : public CDialog
{
// Construction
public:
	CIniFileReaderDlg(CWnd* pParent = NULL);	// standard constructor
    long AddSection(CString &f_csStudy);
    long DeleteSection(CString &f_csStudy);
    long AddOrUpdateProtocol(CString &f_csStudy,CString &f_csProtocol,CString &f_csKtReconFlag);
    long DeleteProtocol(CString &f_csStudy,CString &f_csProtocol);
    long ReadProtocol(CString &f_csStudy,CString &f_csPtotocol,CString &f_csKtReconFlag,BOOL &f_bKtReconFlag );
    void ReadAllProtocols();

// Dialog Data
	//{{AFX_DATA(CIniFileReaderDlg)
	enum { IDD = IDD_INIFILEREADER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIniFileReaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIniFileReaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        int GetKtReconLicense( BOOL &f_nLicense );
    CStringList* GetAllProtocolsData(CString &f_csStudy);
private:
    CString m_csFilepath;
    CString m_csFileName;
    CString m_csFilePathWithName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INIFILEREADERDLG_H__D4A3C2F4_4969_4536_8DDC_447DBFCF06AC__INCLUDED_)
