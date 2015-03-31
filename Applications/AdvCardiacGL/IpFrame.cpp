// IpFrame.cpp : implementation of the CInPlaceFrame class
//

#include "stdafx.h"
#include "AdvCardiacGL.h"
#include "AdvCardiacGLView.h"
#include "IpFrame.h"
#include "DPSComManager.h"
#include "CommonDefinitions.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame
CInPlaceFrame *g_frame = NULL;
IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)
extern CAdvCardiacGLApp theApp;

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

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame construction/destruction

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar implements in-place resizing.
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // fail to create
	}

	// By default, it is a good idea to register a drop-target that does
	//  nothing with your frame window.  This prevents drops from
	//  "falling through" to a container that supports drag-drop.
	m_dropTarget.Register(this);
   g_frame = this;
	return 0;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    CString getstring;
    
    getstring = _T("Adv Cardiac GL ");
    cs.lpszName = getstring;
    cs.style |=  WS_OVERLAPPED | WS_CAPTION | WS_BORDER;
    
    return COleDocIPFrameWnd::PreCreateWindow(cs);

}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame diagnostics

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleDocIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG
BOOL CInPlaceFrame::OnCopyData(
                               CWnd* pWnd,
                               COPYDATASTRUCT* pCopyDataStruct
                               )
{
    // TODO: Add your message handler code here and/or call default
//    AfxMessageBox(_T("Inside OnCopyData"));
    
    GLInfo *pGLInfo = (GLInfo *)pCopyDataStruct->lpData;
    BSTR bstrPageName = SysAllocString(pGLInfo->csPageName); 
//    AfxMessageBox(pGLInfo->csPageName);
    CDPSComManager::GetInstance()->SetParentName(bstrPageName);
    
    CAdvCardiacGLView *pView = NULL;
    pView = (CAdvCardiacGLView *)GetActiveView();
    
    if(pView == NULL) {
        return FALSE;
//        AfxMessageBox(_T("pView is NUll"));
    }

    pView->PostMessage(WM_INIT_TOOL,0,0); 
    
    SysFreeString(bstrPageName);
    
    return COleDocIPFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

/**
* This function override of OnWindowPosChanging method, to avoid
* z-order change by OS.
* @param lpwndpos	Points to a WINDOWPAS data structure that contains
* information about the window's new size and position.
*/
void CInPlaceFrame::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    if ( theApp.m_bMouseActive )
    {
        theApp.m_bMouseActive = false;
        lpwndpos->flags = lpwndpos->flags | SWP_NOZORDER;

    }
}


/**
* The framework calls this function when the cursor is in an
* inactive window and the user presses a mouse button.
* @param pDesktopWnd	Specifies a pointer to the top-level parent window of the
* window being activated.
* @param nHitTest	Specifies the hit-test area code.
* @param message	Specifies the mouse message number.
* return Whether to activate the CWnd and whether to discard the mouse event.
*/
int CInPlaceFrame::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest,
                                   UINT message)
{
    theApp.m_bMouseActive = true;
    CAdvCardiacGLView *pView = dynamic_cast<CAdvCardiacGLView *>(GetActiveView());//(CAdvCardiacGLView *)GetActiveView();

    if (NULL == pView)
    {
        return CFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
    }
    if ( (NULL != pView) /*&& pView->m_bRegisteredZOrder*/ )
    {
        CWnd* l_wnd = AfxGetMainWnd();
        HWND hWnd = l_wnd->GetSafeHwnd();
//        ACGAppController::GetInstance()->ChangeWindowZOrder(hWnd);       
    }
    return COleDocIPFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

/**
* This function is called by the framework to notify the window
* that is losing the mouse ot stylus capture.
* @param pWnd		Specifies a pointer to the window to gain mouse or stylus capture.
*/
void CInPlaceFrame::OnCaptureChanged(CWnd* pWnd)
{
    theApp.m_bMouseActive = true;
    COleDocIPFrameWnd::OnCaptureChanged(pWnd);
}

/**
* The framework calls this member function to all top-level
* windows of the task being activated and for all top-level
* windows of the task being deactivated.
* @param bActive	Specifies whether the CWnd is being activated or deactivated.
* @param hTask		Specifies a task handle. If bActive is TRUE, the handle
* identifies the task that owns the CWnd being deactivated.
*/
void CInPlaceFrame::OnActivateApp(BOOL bActive, HTASK hTask)
{
    UNREFERENCED_PARAMETER(bActive);
    UNREFERENCED_PARAMETER(hTask);
}

/**
* The framework calls this member function when the non client
* area needs to be changed to indicate an active or inactive
* state. The default implementation draws the title bar and
* title-bar text in their active colors if bActive is TRUE
* and in their inactive colors if bActive is FALSE.
* @param bActive	Specifies when a caption bar or icon needs to be changed
* to indicate an active or inactive state. The bActive parameter
* is TRUE if an active caption or icon is to be drawn.
* It is FALSE for an inactive caption or icon.
*/
BOOL CInPlaceFrame::OnNcActivate(BOOL bActive)
{
    if (bActive)
    {
        theApp.m_bMouseActive = true;
    }

    return COleDocIPFrameWnd::OnNcActivate(bActive);
}
/**
* This function override of OnActivate method, to notify DPSPage to change
* z-order when it activated.
* @param nState		Specifies whether the CWnd is being activated or deactivated.
* @param pWndOther	Specifies a pointer to the CWnd being activated ot deactivated.
* @param bMinimized	Specifies the minimized state of the CWnd being activated
* or deactivated.
*/
void CInPlaceFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    COleDocIPFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}
///////////////////////////////// END OF FILE /////////////////////////////////