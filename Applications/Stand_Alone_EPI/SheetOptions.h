#if !defined(AFX_SHEETOPTIONS_H__38BE9699_CF41_4F80_840E_9451D40203B7__INCLUDED_)
#define AFX_SHEETOPTIONS_H__38BE9699_CF41_4F80_840E_9451D40203B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetOptions.h : header file
//
#include "PageEPI.h"
#include "PageFSE.h"
/////////////////////////////////////////////////////////////////////////////
// CSheetOptions

class CSheetOptions : public CPropertySheet
{
    DECLARE_DYNAMIC(CSheetOptions)

// Construction
public:
    CSheetOptions() : CPropertySheet() {
        m_pPageOne = CPageEPI::GetPageOneInstance();
        m_pPageTwo = new CPageFSE;
        AddPage(m_pPageOne);
        AddPage(m_pPageTwo);
    };

    CSheetOptions(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    CSheetOptions(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
    CPageEPI * m_pPageOne;
    CPageFSE * m_pPageTwo;
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSheetOptions)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSheetOptions();

    // Generated message map functions
protected:
    //{{AFX_MSG(CSheetOptions)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETOPTIONS_H__38BE9699_CF41_4F80_840E_9451D40203B7__INCLUDED_)
