/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_MAPWRITER_H__B39187DF_8FE4_4369_B21C_DF1E69BFDCE2__INCLUDED_)
#define AFX_MAPWRITER_H__B39187DF_8FE4_4369_B21C_DF1E69BFDCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapWriter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapWriter window

class CMapWriter : public CWnd
{
// Construction
public:
    CMapWriter();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMapWriter)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMapWriter();
    int WriteToFile(CString &f_csFileName, void *f_dImgBuf, unsigned long f_nNumOfPoints);

    // Generated message map functions
protected:
    //{{AFX_MSG(CMapWriter)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPWRITER_H__B39187DF_8FE4_4369_B21C_DF1E69BFDCE2__INCLUDED_)
