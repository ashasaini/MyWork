// KtRecon.h : main header file for the KTRECON application
//

#if !defined(AFX_KTRECON_H__7C14E410_27D0_4B40_A7F1_E0F95FB5A443__INCLUDED_)
#define AFX_KTRECON_H__7C14E410_27D0_4B40_A7F1_E0F95FB5A443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKtReconApp:
// See KtRecon.cpp for the implementation of this class
//

class CKtReconApp : public CWinApp
{
public:
	CKtReconApp();
public:
    bool m_bMouseActive;
    //bool m_bMouseActive;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKtReconApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CKtReconApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTRECON_H__7C14E410_27D0_4B40_A7F1_E0F95FB5A443__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////