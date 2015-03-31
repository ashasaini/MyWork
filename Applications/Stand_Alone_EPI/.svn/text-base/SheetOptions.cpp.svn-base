// SheetOptions.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "SheetOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetOptions

IMPLEMENT_DYNAMIC(CSheetOptions, CPropertySheet)

CSheetOptions::CSheetOptions(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
    : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CSheetOptions::CSheetOptions(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CSheetOptions::~CSheetOptions()
{
    if(m_pPageTwo) {
        delete m_pPageTwo;
        m_pPageTwo = NULL;
    }
}


BEGIN_MESSAGE_MAP(CSheetOptions, CPropertySheet)
    //{{AFX_MSG_MAP(CSheetOptions)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetOptions message handlers
