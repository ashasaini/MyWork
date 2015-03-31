#if !defined(AFX_KTRECONDLG_H__77212F21_2E5F_4A49_A8D3_97DFF24193E1__INCLUDED_)
#define AFX_KTRECONDLG_H__77212F21_2E5F_4A49_A8D3_97DFF24193E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KtReconDlg.h : header file
//
#include <vector>
using namespace std;

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/ListBoxEx.h>
#include <MPlusUICommon/DialogEx.h>
using namespace MPlus::UICommon;
#include "resource.h"
#include "KtReconController.h"
#include "PopupDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CKtReconDlg dialog

class CKtReconDlg : public DialogEx
{
// Construction
public:
    CKtReconDlg(CWnd* pParent = NULL);   // standard constructor
    ~CKtReconDlg();
	
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

// Dialog Data
	//{{AFX_DATA(CKtReconDlg)
	enum { IDD = IDD_DIALOG_KTRECON };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKtReconDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKtReconDlg)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGetData();
	afx_msg void OnButtonKtrecon();
	afx_msg void OnButtonView();
	afx_msg void OnClickListScannedProtocol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message); //+Mpower/20130812/PN/added
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    void initializeControls();
private:
    FrameEx m_scannedProtDisplay;
    FrameEx m_operations;
    ButtonEx m_getDataButton;
    ButtonEx m_ktReconButton;
    ButtonEx m_viewerButton;
    ButtonEx m_exitButton;
    ListCtrlEx m_scannedProtocolListbox;
    CStatic m_lbScannedProt;
    CFont m_font;
    CBrush m_list_brush;

    //CKtReconController *m_KtReconController;
public:
	int LoadScannedListBox();
	void SetController(CKtReconController * control);
    CKtReconController *m_KtReconController;
    void FillProtocolListBox(LVITEM &lv);
    void SetDlgPos();
    void DisplayErr(int &f_nmsg_code);
	    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:
    int m_ncurrscannedrowno;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTRECONDLG_H__77212F21_2E5F_4A49_A8D3_97DFF24193E1__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////