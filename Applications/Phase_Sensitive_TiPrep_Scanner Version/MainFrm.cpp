// MainFrm.cpp : implementation of the CMainFrame class
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "TiPrep.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CTiPrepApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CMainFrame
//Purpose       : Constructor
//********************************************************************
CMainFrame::CMainFrame()
{
    // 
}
//****************************Method Header***************************
//Method Name   : CMainFrame
//Purpose       : Destructor
//********************************************************************
CMainFrame::~CMainFrame()
{
    //
}


/*************************************************************
* Method:       OnCreate
* Description:  This function is used
* Parameter:    LPCREATESTRUCT lpCreateStruct
* Returns:      int
*************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    return 0;
}


/*************************************************************
* Method:       PreCreateWindow
* Description:  This function is used
* Parameter:    CREATESTRUCT & cs
* Returns:      BOOL
*************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if(!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE;
    cs.x = 200;
    cs.y = 100;
    cs.cy = 1000;
    cs.cx = 800;
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
/*************************************************************
* Method:       AssertValid
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

/*************************************************************
* Method:       Dump
* Description:  This function is used
* Parameter:    CDumpContext & dc
* Returns:      void
*************************************************************/
void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/*************************************************************
* Method:       OnClose
* Description:  This function is called on closing window
* Returns:      void
*************************************************************/
void CMainFrame::OnClose()
{
    CFrameWnd::OnClose();
}

/*************************************************************
* Method:    	OnDestroy
* Description: 	This function is called before destroying window
* Returns:   	void
*************************************************************/
void CMainFrame::OnDestroy()
{
    CFrameWnd::OnDestroy();
    AfxPostQuitMessage(0);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////


