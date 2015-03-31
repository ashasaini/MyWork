// TiPrep.h : main header file for the TIPREP application
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIPREP_H__13586205_4385_45DF_850F_49767189DF0F__INCLUDED_)
#define AFX_TIPREP_H__13586205_4385_45DF_850F_49767189DF0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTiPrepApp:
// See TiPrep.cpp for the implementation of this class
//

class CTiPrepApp : public CWinApp
{
public:

    CTiPrepApp();

public:
    bool m_bMouseActive;//Patni-SS/2009Mar14/Added/PSP1#ZOrder

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTiPrepApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
private:
    COleTemplateServer m_server;
    // Server object for document creation
    //{{AFX_MSG(CTiPrepApp)

    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPREP_H__13586205_4385_45DF_850F_49767189DF0F__INCLUDED_)
