// SpinCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "tiprep.h"
#include "SpinCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SpinCtrl

SpinCtrl::SpinCtrl()
{
}

SpinCtrl::~SpinCtrl()
{
}


BEGIN_MESSAGE_MAP(SpinCtrl, SpinEx)
	//{{AFX_MSG_MAP(SpinCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
        ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltapos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SpinCtrl message handlers
BOOL SpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    UNREFERENCED_PARAMETER(pNMUpDown);
    GetParentOwner()->SendMessage(WM_CMD_SPIN,NULL,NULL);
    *pResult = 0;
    return TRUE;
}