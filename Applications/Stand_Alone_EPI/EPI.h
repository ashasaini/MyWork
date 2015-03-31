// EPI.h : main header file for the EPI application
//

#if !defined(AFX_EPI_H__27D1954E_CABF_41B5_A44E_4E97B9C2EB4B__INCLUDED_)
#define AFX_EPI_H__27D1954E_CABF_41B5_A44E_4E97B9C2EB4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEPIApp:
// See EPI.cpp for the implementation of this class
//
#include "EPIDoc.h"
class CEPIApp : public CWinApp
{
private:
    CSingleDocTemplate* pDocTemplate;
public:
    CEPIApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEPIApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CEPIApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPI_H__27D1954E_CABF_41B5_A44E_4E97B9C2EB4B__INCLUDED_)
