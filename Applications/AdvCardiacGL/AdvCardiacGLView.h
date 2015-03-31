// AdvCardiacGLView.h : interface of the CAdvCardiacGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADVCARDIACGLVIEW_H__F360A36E_EB85_4F3E_8D5D_839B849A11E4__INCLUDED_)
#define AFX_ADVCARDIACGLVIEW_H__F360A36E_EB85_4F3E_8D5D_839B849A11E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AdvCardiacGLDoc.h"
#include "ACGAppController.h"

class CAdvCardiacGLView : public CFormView
{
protected: // create from serialization only
	CAdvCardiacGLView();
	DECLARE_DYNCREATE(CAdvCardiacGLView)
		
public:
	//{{AFX_DATA(CAdvCardiacGLView)
	enum{ IDD = IDD_ADVCARDIACGL_FORM };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// Attributes
public:
	CAdvCardiacGLDoc* GetDocument();
    
    LRESULT OnInitAdvCardiacGL(WPARAM wParam,LPARAM lParam);
    LRESULT OnCmdClose(WPARAM wParam,LPARAM lParam);
    LRESULT OnCmdShow(WPARAM wParam,LPARAM lParam);
    LRESULT OnCmdHide(WPARAM wParam,LPARAM lParam);
    static CAdvCardiacGLView * GetView();
	int StartGUI();
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvCardiacGLView)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL
	
	// Implementation
public:	
    //ACGAppController* m_ACGLController; 
	CACGLDlg *m_maindlg;
	//	void StartGUI();
    void SetDlg(CACGLDlg *f_dlg);
	virtual ~CAdvCardiacGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
protected:
	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvCardiacGLView)
	afx_msg void OnCancelEditSrvr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AdvCardiacGLView.cpp
inline CAdvCardiacGLDoc* CAdvCardiacGLView::GetDocument()
{ return (CAdvCardiacGLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVCARDIACGLVIEW_H__F360A36E_EB85_4F3E_8D5D_839B849A11E4__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////