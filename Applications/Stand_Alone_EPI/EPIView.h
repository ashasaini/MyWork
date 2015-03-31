// EPIView.h : interface of the CEPIView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPIVIEW_H__89CF0CEF_B582_45F1_9A49_22E854B1B6E9__INCLUDED_)
#define AFX_EPIVIEW_H__89CF0CEF_B582_45F1_9A49_22E854B1B6E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EPIDoc.h"
#include"SheetOptions.h"
#define WM_EPI_RELOAD          (WM_USER + 100)

class CEPIView : public CView
{
protected: // create from serialization only
    CEPIView();
    DECLARE_DYNCREATE(CEPIView)

// Attributes
public:
    CEPIDoc* GetDocument();

// Operations
public:
afx_msg void Oncmdreload(WPARAM wParam, LPARAM lParam);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEPIView)
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CEPIView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    //
public:
    CSheetOptions * m_pOptionsSheet;
// Generated message map functions
protected:
    //{{AFX_MSG(CEPIView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EPIView.cpp
inline CEPIDoc* CEPIView::GetDocument()
{
    return (CEPIDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPIVIEW_H__89CF0CEF_B582_45F1_9A49_22E854B1B6E9__INCLUDED_)
