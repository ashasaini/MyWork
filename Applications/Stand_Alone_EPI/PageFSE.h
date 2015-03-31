#if !defined(AFX_PAGETWO_H__8E1A80E1_3D1C_48B5_B749_BB761AF2F6F2__INCLUDED_)
#define AFX_PAGETWO_H__8E1A80E1_3D1C_48B5_B749_BB761AF2F6F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageFSE.h : header file
//
#include "PageBase.h"
/////////////////////////////////////////////////////////////////////////////
// CPageFSE dialog

class CPageFSE : public CPageBase//CPropertyPage
{
    DECLARE_DYNCREATE(CPageFSE)

// Construction
public:
    CPageFSE();
    ~CPageFSE();

// Dialog Data
    //{{AFX_DATA(CPageFSE)
    enum { IDD = IDD_PROPPAGE_FSE };
    // NOTE - ClassWizard will add data members here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPageFSE)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPageFSE)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETWO_H__8E1A80E1_3D1C_48B5_B749_BB761AF2F6F2__INCLUDED_)
