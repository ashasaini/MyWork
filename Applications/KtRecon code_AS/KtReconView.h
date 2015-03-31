// KtReconView.h : interface of the CKtReconView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONVIEW_H__D85E1D32_42D0_448A_87FA_9B701A566955__INCLUDED_)
#define AFX_KTRECONVIEW_H__D85E1D32_42D0_448A_87FA_9B701A566955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "KtReconDoc.h"
#include "KtDPSCommMgr.h"


// #include <MPlusUICommon/ButtonEx.h>
// #include <MPlusUICommon/EditCtrlEx.h>
// #include <MPlusUICommon/FrameEx.h>
// #include <MPlusUICommon/ListCtrlEx.h>
// #include <MPlusUICommon/ListBoxEx.h>
// #include <MPlusUICommon/DialogEx.h>


/*using namespace MPlus::UICommon;*/
#include "KtReconDlg.h"
#include "KtReconController.h"	// Added by ClassView

class CKtReconView : public CFormView
{
protected: // create from serialization only
	CKtReconView();
	DECLARE_DYNCREATE(CKtReconView)

public:
	//{{AFX_DATA(CKtReconView)
	enum{ IDD = IDD_KTRECON_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CKtReconDoc* GetDocument();

// Operations
public:

// Overrides
    HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKtReconView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
//     afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKtReconView();
    BOOL RegisterWindowZOrder(HWND hWnd_i);
    BOOL ChangeWindowZOrder(HWND hWnd_i);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    LRESULT OnInitKtRecon(WPARAM wParam,LPARAM lParam);
    LRESULT OnCmdClose(WPARAM wParam,LPARAM lParam);
    LRESULT OnCmdShow(WPARAM wParam,LPARAM lParam);
    LRESULT OnCmdHide(WPARAM wParam,LPARAM lParam);

// Generated message map functions
protected:
	//{{AFX_MSG(CKtReconView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnCancelEditSrvr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        
    //CKtReconDlg *m_ktReconDlg;

public:
	int StartGUI();
	CKtReconController *m_KtReconController;
    CKtReconDlg *m_ktReconDlg;
    static CKtReconView * GetView();
    void AddZOrderLayer();
    BOOL UnRegisterWindowZOrder(HWND hWnd_i);
    

};

#ifndef _DEBUG  // debug version in KtReconView.cpp
inline CKtReconDoc* CKtReconView::GetDocument()
   { return (CKtReconDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTRECONVIEW_H__D85E1D32_42D0_448A_87FA_9B701A566955__INCLUDED_)
