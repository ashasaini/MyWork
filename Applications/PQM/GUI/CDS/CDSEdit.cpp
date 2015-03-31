//TMSC-REDMINE-781
// CDSEdit.cpp : implementation file
//

#include "../stdafx.h"
#include "CDSEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCDSEdit, EditCtrlEx)
    //{{AFX_MSG_MAP(CCDSEdit)
    ON_WM_GETDLGCODE()
    ON_WM_KILLFOCUS()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCDSEdit

CCDSEdit::CCDSEdit()
{
}

CCDSEdit::~CCDSEdit()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCDSEdit message handlers

UINT CCDSEdit::OnGetDlgCode()
{
    // TODO: Add your message handler code here and/or call default

    //return EditCtrlEx::OnGetDlgCode();
    return DLGC_DEFPUSHBUTTON | DLGC_WANTCHARS | DLGC_WANTARROWS | DLGC_HASSETSEL;
}

void CCDSEdit::OnKillFocus(CWnd* pNewWnd)
{
    if (GetParent()) {
        ::SendMessage(GetParent()->m_hWnd , WM_EDIT_KILL_FOCUS, (WPARAM)GetDlgCtrlID(), 0);
    }

    EditCtrlEx::OnKillFocus(pNewWnd);
    // TODO: Add your message handler code here
}


void CCDSEdit::OnRButtonDown(
    UINT nFlags,
    CPoint point
)
{
}
