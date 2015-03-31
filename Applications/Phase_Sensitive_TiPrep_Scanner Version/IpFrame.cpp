/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "TiPrep.h"
#include "TiPrepview.h"
#include "IpFrame.h"
#include "DPSComManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CInPlaceFrame *g_frame = NULL;

extern CTiPrepApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleDocIPFrameWnd)
    //{{AFX_MSG_MAP(CInPlaceFrame)
    ON_WM_CREATE()
    ON_WM_COPYDATA()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_MOUSEACTIVATE()
    ON_WM_ACTIVATE()
    ON_WM_CAPTURECHANGED()
    ON_WM_ACTIVATEAPP()
    ON_WM_NCACTIVATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CInPlaceFrame
//Purpose       : Constructor
//********************************************************************
CInPlaceFrame::CInPlaceFrame()
{
    m_window_rect = CRect(500, 200, 750, 900);
}

//****************************Method Header***************************
//Method Name   : CInPlaceFrame
//Purpose       : Destructor
//********************************************************************
CInPlaceFrame::~CInPlaceFrame()
{
    //
}

/*************************************************************
* Method:       OnCreate
* Description:  This function is used
* Parameter:    LPCREATESTRUCT lpCreateStruct
* Returns:      int
*************************************************************/
int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if(FALSE == m_wndResizeBar.Create(this)) {
        TRACE0("Failed to create resize bar\n");
        return -1;      // fail to create
    }

    m_dropTarget.Register(this);
    g_frame = this;
    return 0;
}

/*************************************************************
* Method:       PreCreateWindow
* Description:  This function is used
* Parameter:    CREATESTRUCT & cs
* Returns:      BOOL
*************************************************************/
BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    CString getstring;
    getstring = _T("TiPrep");
    cs.lpszName = getstring;
    cs.style |=  WS_OVERLAPPED | WS_CAPTION | WS_BORDER;
    return COleDocIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame diagnostics

#ifdef _DEBUG
/*************************************************************
* Method:       AssertValid
* Description:  This function is used
* Returns:      void
*************************************************************/
void CInPlaceFrame::AssertValid() const
{
    COleDocIPFrameWnd::AssertValid();
}

/*************************************************************
* Method:       Dump
* Description:  This function is used
* Parameter:    CDumpContext & dc
* Returns:      void
*************************************************************/
void CInPlaceFrame::Dump(CDumpContext& dc) const
{
    COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/*************************************************************
* Method:       OnCopyData
* Description:  This function is Message Handler for WM_COPYDATA
*               which will contain the name of the Parent Page.
* Parameter:    CWnd * pWnd
*               Window handler.
* Parameter:    COPYDATASTRUCT * pCopyDataStruct
*               Pointer to the structure.
* Returns:      BOOL
*************************************************************/
BOOL CInPlaceFrame::OnCopyData(
    CWnd* pWnd,
    COPYDATASTRUCT* pCopyDataStruct
)
{
    try{
        tagWINDOWPLACEMENT lWndPlacement ;
        ZeroMemory(&lWndPlacement, sizeof(WINDOWPLACEMENT));
        GetWindowPlacement(&lWndPlacement);
        m_window_rect = lWndPlacement.rcNormalPosition;
        GLInfo *pGLInfo = (GLInfo *)pCopyDataStruct->lpData;
        BSTR bstrPageName = SysAllocString(pGLInfo->csPageName);
        CTiPrepView *pView = NULL;
        pView = dynamic_cast<CTiPrepView*> (GetActiveView());
        
        if(pView == NULL) {
            return FALSE;
        }
        
        pView->PostMessage(WM_INIT_TIPREP, 0, (LPARAM)(BSTR)bstrPageName);//Mpower_65/Jun-2013/VP Added
        SysFreeString(bstrPageName);
        return COleDocIPFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
    } catch(...) {
        return FALSE;
    }
}

/*************************************************************
* Method:       OnWindowPosChanging
* Description:  This function override of OnWindowPosChanging
*               method, to avoid z-order change by OS.
* Parameter:    WINDOWPOS * lpwndpos
*               Points to a WINDOWPAS data structure that
*               contains information about the window's new size
*               and position.
* Returns:      void
*************************************************************/
void CInPlaceFrame::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    if(theApp.m_bMouseActive) {
        theApp.m_bMouseActive = false;
        lpwndpos->flags = lpwndpos->flags | SWP_NOZORDER;
    }
}

/*************************************************************
* Method:       OnActivate
* Description:  This function override of OnActivate method, to
*               notify DPSPage to change z-order when it activated.
* Parameter:    UINT nState
*               Specifies whether the CWnd is being activated or
*               deactivated.
* Parameter:    CWnd * pWndOther
*               Specifies a pointer to the CWnd being activated or
*               deactivated.
* Parameter:    BOOL bMinimized
*               Specifies the minimized state of the CWnd being
*               activated or deactivated.
* Returns:      void
*************************************************************/
void CInPlaceFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    COleDocIPFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

/*************************************************************
* Method:       OnMouseActivate
* Description:  The framework calls this function when the cursor
*               is in an inactive window and the user presses a
*               mouse button.
* Parameter:    CWnd * pDesktopWnd
*               Specifies a pointer to the top-level parent window
*               of the window being activated.
* Parameter:    UINT nHitTest
*               Specifies the hit-test area code.
* Parameter:    UINT message
*               Specifies the mouse message number.
* Returns:      int
*               Whether to activate the CWnd and whether to discard
*               the mouse event.
*************************************************************/
int CInPlaceFrame::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest,
                                   UINT message)
{
    theApp.m_bMouseActive = true;
    CTiPrepView *pView = dynamic_cast<CTiPrepView *>(GetActiveView());

    if(NULL == pView) {
        return CFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
    }

    if((NULL != pView)) {
        CWnd* l_wnd = AfxGetMainWnd();
        HWND hWnd = l_wnd->GetSafeHwnd();
        pView->ChangeWindowZOrder(hWnd);

        if(pView->GetChildHWND()) {
            pView->ChangeWindowZOrder(pView->GetChildHWND());    
        }
    }

    return COleDocIPFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

/*************************************************************
* Method:       OnCaptureChanged
* Description:  This function is called by the framework to notify
*               the window that is losing the mouse or stylus capture.
* Parameter:    CWnd * pWnd
* Returns:      void
*************************************************************/
void CInPlaceFrame::OnCaptureChanged(CWnd* pWnd)
{
    theApp.m_bMouseActive = true;
    COleDocIPFrameWnd::OnCaptureChanged(pWnd);
}

/*************************************************************
* Method:       OnActivateApp
* Description:  The framework calls this member function to all
*               top-level windows of the task being activated and
*               for all top-level windows of the task being deactivated.
* Parameter:    BOOL bActive
*               Specifies whether the CWnd is being activated or deactivated.
* Parameter:    HTASK hTask
*               Specifies a task handle. If bActive is TRUE, the handle
*               identifies the task that owns the CWnd being deactivated.
* Returns:      void
*************************************************************/
void CInPlaceFrame::OnActivateApp(BOOL bActive, HTASK hTask)
{
    UNREFERENCED_PARAMETER(bActive);
    UNREFERENCED_PARAMETER(hTask);
}

/*************************************************************
* Method:       OnNcActivate
* Description:  The framework calls this member function when the
*               non client area needs to be changed to indicate an
*               active or inactive state. The default implementation
*               draws the title bar and title-bar text in their
*               active colors if bActive is TRUE and in their
*               inactive colors if bActive is FALSE.
* Parameter:    BOOL bActive
*               Specifies when a caption bar or icon needs to be
*               changed to indicate an active or inactive state. The
*               bActive parameter is TRUE if an active caption or icon
*               is to be drawn.
* Returns:      BOOL
*               It is FALSE for an inactive caption or icon.
*************************************************************/
BOOL CInPlaceFrame::OnNcActivate(BOOL bActive)
{
    if(bActive) {
        theApp.m_bMouseActive = true;
    }

    return COleDocIPFrameWnd::OnNcActivate(bActive);
}


///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////