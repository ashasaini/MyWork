// EPISpinCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "EPISpinCtrl.h"
#include "SlideToolBar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEPISpinCtrl

CEPISpinCtrl::CEPISpinCtrl()
{
}

CEPISpinCtrl::~CEPISpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CEPISpinCtrl, CSpinButtonCtrl)
    //{{AFX_MSG_MAP(CEPISpinCtrl)
    ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltapos)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEPISpinCtrl message handlers

BOOL CEPISpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    UNREFERENCED_PARAMETER(pNMUpDown);
    m_sliderCtrl->SetPos(GetPos());
    CWnd *cwnd = GetParent();
    CSlideToolBar * tmp = (CSlideToolBar*)cwnd;
    tmp->UpdateFromSpin();
    *pResult = 0;
	return TRUE;
}

void CEPISpinCtrl::SetsliderCtrl(CSliderCtrl *f_sliderctrl)
{
    m_sliderCtrl = f_sliderctrl;
}
