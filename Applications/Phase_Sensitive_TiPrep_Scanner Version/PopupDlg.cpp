// PopupDlg.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "PopupDlg.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CPopupDlg
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CPopupDlg::CPopupDlg(UINT IDD, CWnd* pParent /*=NULL*/)
    : DialogEx(IDD, pParent)
{
    /*TIPREP_TRACE(CPopupDlg::CPopupDlg);*/
    //{{AFX_DATA_INIT(CPopupDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    CString l_fontName(FONT);
    m_pLabel_Font.CreatePointFont(100, static_cast<LPCTSTR>(l_fontName));
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CPopupDlg::DoDataExchange(CDataExchange* pDX)
{
    /*TIPREP_TRACE(CPopupDlg::DoDataExchange);*/
    DialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPopupDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupDlg, DialogEx)
ON_WM_ACTIVATE()
    //{{AFX_MSG_MAP(CPopupDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg message handlers

/*************************************************************
* Method:       OnCancel
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPopupDlg::OnCancel()
{
    TIPREP_TRACE(CPopupDlg::OnCancel,MRTraceMarker3);
    DialogEx::OnCancel();
}

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CPopupDlg::OnInitDialog()
{
    DialogEx::OnInitDialog();
    HWND hwndOwner;
    (hwndOwner = GetParent()->GetSafeHwnd());
    if(hwndOwner == NULL) {
        hwndOwner = GetDesktopWindow()->GetSafeHwnd();
    }

    CRect rc, rcDlg, rcOwner;
    ::GetWindowRect(hwndOwner, &rcOwner);
    GetWindowRect(&rcDlg);
    CopyRect(&rc, &rcOwner);
    OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
    OffsetRect(&rc, -rc.left, -rc.top);
    OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
    MoveWindow(rcOwner.left + (rc.right / 2),
               rcOwner.top + (rc.bottom / 3), rcDlg.Width(), rcDlg.Height());
    return TRUE;  
}

//****************************Method Header***************************
//Method Name   : CInPlaceFrame
//Purpose       : Destructor
//********************************************************************
CPopupDlg::~CPopupDlg()
{
    //
}

/*************************************************************
* Method:    	PreTranslateMessage
* Description: 	This function is used 
* Parameter: 	MSG * pMsg :
* Returns:   	BOOL
*************************************************************/
BOOL CPopupDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN) {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
            return TRUE;
        }
    }

    return DialogEx::PreTranslateMessage(pMsg);
}

/*************************************************************
* Method:    	OnActivate
* Description: 	This function is used 
* Parameter: 	UINT nState :
* Parameter: 	CWnd * pWndOther :
* Parameter: 	BOOL bMinimized :
* Returns:   	void
*************************************************************/
void CPopupDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    /*TIPREP_TRACE(CTiPrepDlg::OnActivate);*/
    DialogEx::OnActivate(nState, pWndOther, bMinimized);
    
    if(WA_INACTIVE != nState) {
        CWnd* pChildWnd = this ? this->GetNextWindow(GW_HWNDPREV) : NULL;
        
        while(pChildWnd) {
            {
                if(pChildWnd->IsWindowVisible()) {                  
                    pChildWnd->SetFocus();
                    break;
                }
            }
            pChildWnd = pChildWnd->GetNextWindow(GW_HWNDPREV);
             if (pChildWnd == this) break; 
        }
        
        (CTiPrepView::GetView())->ChangeWindowZOrder(this->GetSafeHwnd());
        
        
        if(pChildWnd && pChildWnd != this) {
            (CTiPrepView::GetView())->ChangeWindowZOrder(pChildWnd->GetSafeHwnd());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////