//TMSC-REDMINE-777

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: MainFrm.cpp
 *  Overview: Implementation of CMainFrm class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::~CMainFrame(
)
{
}

CMainFrame::CMainFrame(
)
{
    m_bAutoMenuEnable = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


//************************************Method Header**************************************

//Method Name:  OnCreate

//Author:  PATNI/GP

//Purpose: Load PQM menu

//**************************************************************************************/
int CMainFrame::OnCreate(
    LPCREATESTRUCT lpCreateStruct
)
{

    LPCTSTR FUNC_NAME = _T("CMainFrame::OnCreate");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("Trace of OnCreate"));

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetMenu(NULL);
    return 0;
}

// This removes the capability of moving the window
UINT CMainFrame::OnNcHitTest(
    CPoint& point
)
{

    LPCTSTR FUNC_NAME = _T("CMainFrame::OnNcHitTest");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of OnNcHitTest"));

    UINT type = CFrameWnd::OnNcHitTest(point);

    if (HTCAPTION == type)
        return 0;

    else
        return type;
}
BOOL CMainFrame::PreCreateWindow(
    CREATESTRUCT& cs
)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    cs.style = WS_POPUP;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG
