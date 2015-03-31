
#if !defined(AFX_SLIDETOOLBAR_H__992196A1_7C20_11D4_BEC3_B9FC12288B45__INCLUDED_)
#define AFX_SLIDETOOLBAR_H__992196A1_7C20_11D4_BEC3_B9FC12288B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlideToolBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSlideToolBar window
#include "EPISpinCtrl.h"

class CSlideToolBar : public CToolBar
{
// Construction
public:
    CSlideToolBar();

// Attributes
public:

    // WW Set
    CSliderCtrl m_wndSlider_WW;
    CEdit m_wndEdit_WW;
    CEdit m_wndStatic_WW;
    CEPISpinCtrl m_wndSpin_WW;

    // WL Set
    CSliderCtrl m_wndSlider_WL;
    CEdit m_wndEdit_WL;
    CEPISpinCtrl m_wndSpin_WL;

    // Z Rotation Set
//  CSliderCtrl m_wndSlider_Rot_Z;
//  CEdit m_wndEdit_Rot_Z;
//  CSpinButtonCtrl m_wndSpin_Rot_Z;

    // Font used to fill the edit controls
    CFont m_font;


    int m_WWvalue;
    int m_WLvalue;

// Operations
public:

    void SetPositions();
    void GetWWWL(int &WW, int &WL);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSlideToolBar)
    //}}AFX_VIRTUAL

// Implementation
public:
    void UpdateValues();
    virtual ~CSlideToolBar();
    void UpdateFromSpin();
    // Generated message map functions
protected:
    //{{AFX_MSG(CSlideToolBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDETOOLBAR_H__992196A1_7C20_11D4_BEC3_B9FC12288B45__INCLUDED_)
