// IpFrame.h : interface of the CInPlaceFrame class
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_IPFRAME_H__623BEFFE_51FA_4D3A_A7BE_F0DCB4B94C38__INCLUDED_)
#define AFX_IPFRAME_H__623BEFFE_51FA_4D3A_A7BE_F0DCB4B94C38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
*  This class is a used .
*/

class CInPlaceFrame : public COleDocIPFrameWnd
{
    DECLARE_DYNCREATE(CInPlaceFrame)
public:
    CInPlaceFrame();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CInPlaceFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CInPlaceFrame();
    virtual BOOL BuildSharedMenu() {
        return TRUE;
    }; //Added to remove the Menu Bar shown on top in DPS
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    /**
    *
    */
    COleDropTarget  m_dropTarget;
    /**
    *
    */
    COleResizeBar   m_wndResizeBar;
    /**
    *
    */
    CRect           m_window_rect;

// Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct); //Aakash
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnCaptureChanged(CWnd* pWnd);
    afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
    afx_msg BOOL OnNcActivate(BOOL bActive);
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPFRAME_H__623BEFFE_51FA_4D3A_A7BE_F0DCB4B94C38__INCLUDED_)
