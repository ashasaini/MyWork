// IniFileReader.h : main header file for the INIFILEREADER application
//

#if !defined(AFX_INIFILEREADER_H__10A1C1AD_9F91_49C2_B42A_AA88289746EC__INCLUDED_)
#define AFX_INIFILEREADER_H__10A1C1AD_9F91_49C2_B42A_AA88289746EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIniFileReaderApp:
// See IniFileReader.cpp for the implementation of this class
//
#define E_NO_ERROR 0
#define E_ERROR 1
class CIniFileReaderApp : public CWinApp
{
public:
	CIniFileReaderApp();
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIniFileReaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIniFileReaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:


};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INIFILEREADER_H__10A1C1AD_9F91_49C2_B42A_AA88289746EC__INCLUDED_)
