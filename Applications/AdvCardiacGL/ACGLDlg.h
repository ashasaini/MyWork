#if !defined(AFX_ACGLDLG_H__A07D5D61_AB92_4DFC_B147_4EF6160D878C__INCLUDED_)
#define AFX_ACGLDLG_H__A07D5D61_AB92_4DFC_B147_4EF6160D878C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACGLDlg.h : header file
//
#include <MPlusUICommon/DialogEx.h>
using namespace MPlus::UICommon;

#include <MPlusUICommon/ButtonEx.h>
/////////////////////////////////////////////////////////////////////////////
// CACGLDlg dialog
/**
* This micro is used for background color of dialogbox.
*/
const COLORREF BKCOLOR = RGB(27, 36, 54);
/**
* This micro is used for background color of frame.
*/
const COLORREF FRAME_BKCOLOR = RGB(70, 80, 100);
/**
* This micro is used for text color.
*/
const COLORREF TEXT_BKCOLOR = RGB(191, 209, 255);
/**
* This micro is used for heading text size.
*/
const int FONT_SIZE = 12;
/**
* This micro is used for text size.
*/
const int TEXT_SIZE = 10;
/**
* This micro is used for text format.
*/
const TCHAR FONT[]  = _T("Arial");

class CACGLDlg : public DialogEx
{
	// Construction
public:
	int InvokeApp(int toolno,CString & cstoolname);
	CACGLDlg(CWnd* pParent = NULL);   // standard constructor
	
private:
	// Dialog Data
	//{{AFX_DATA(CACGLDlg)
	enum { IDD = IDD_ACGL_DIALOG };
	ButtonEx	m_ACGL_15;
	ButtonEx	m_ACGL_14;
	ButtonEx	m_ACGL_13;
	ButtonEx	m_ACGL_12;
	ButtonEx	m_ACGL_11;
	ButtonEx	m_ACGL_10;
	ButtonEx	m_ACGL_9;
	ButtonEx	m_ACGL_8;
	ButtonEx	m_ACGL_7;
	ButtonEx	m_ACGL_6;
	ButtonEx	m_ACGL_5;
	ButtonEx	m_ACGL_4;
	ButtonEx	m_ACGL_3;
	ButtonEx	m_ACGL_2;
	ButtonEx	m_ACGL_1;
    CFont m_ACGL_font;
    CBrush m_ACGL_brush;
	//}}AFX_DATA	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACGLDlg)
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	
	afx_msg HCURSOR OnQueryDragIcon();

	// Generated message map functions
	//{{AFX_MSG(CACGLDlg)
	afx_msg void OnButton1();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnButtonDock();
	afx_msg void OnButtonMaximise();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	void SetACGLDlgPosition();
	int DisplayToolButtons(); 

	HICON m_hIcon;
	CToolBar m_FirstToolBar;
	bool toggle;
	int width,height;	   
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACGLDLG_H__A07D5D61_AB92_4DFC_B147_4EF6160D878C__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////