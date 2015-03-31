
// SlideToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SlideToolBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlideToolBar

CSlideToolBar::CSlideToolBar()
{
    m_WWvalue = 100;
    m_WLvalue = 50;
}

CSlideToolBar::~CSlideToolBar()
{
}


BEGIN_MESSAGE_MAP(CSlideToolBar, CToolBar)
    //{{AFX_MSG_MAP(CSlideToolBar)
    ON_WM_CREATE()
    ON_WM_HSCROLL()
    ON_WM_SHOWWINDOW()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlideToolBar message handlers

int CSlideToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(CToolBar::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    //##################################################//
    // Load the dummy buttons toolbar.
    //
    if(!LoadToolBar(IDR_SLIDER_BAR)) {
        return -1;
    }

    //##################################################//
    // Spinner Toolbar button modifying method
    // DC to select font into
    CClientDC dc(this);
    // CFont object used to set the desired control fonts
    m_font.CreatePointFont(100, _T("MS Sans Serif"));
    CFont* pOldFont = dc.SelectObject(&m_font);
    // TEXTMETRIC is Useful to measure and place the controls
    // in the desired positions
    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    int cxChar = tm.tmAveCharWidth;
    int cyChar = tm.tmHeight + tm.tmExternalLeading;
    dc.SelectObject(pOldFont);
    int n = 0;
    CRect rect;
    //################# WW Slider/Edit set #################
    n++;
    SetButtonInfo(n, ID_EDIT_WW, TBBS_SEPARATOR , cxChar * 6);
    GetItemRect(n, &rect);
    rect.bottom = rect.top + (cyChar + 6);

    if(!m_wndEdit_WW.CreateEx(WS_EX_CLIENTEDGE, // Make a 3D-border
                              _T("EDIT"), NULL,
                              //WS_TABSTOP |
                              ES_READONLY | ES_AUTOHSCROLL | ES_LEFT | ES_NOHIDESEL | WS_VISIBLE | WS_CHILD,
                              rect, this, ID_EDIT_WW)) {
        return -1;
    }

    m_wndEdit_WW.SetFont(&m_font);
    n++;
    SetButtonInfo(n, ID_SLIDE_WW, TBBS_SEPARATOR, cxChar * 12);
    GetItemRect(n, &rect);
    rect.bottom = rect.top + (cyChar + 2);

    if(!m_wndSlider_WW.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | // dwStyle
                              TBS_HORZ |
                              //TBS_AUTOTICKS |
                              TBS_NOTICKS | TBS_TOOLTIPS |
                              TBS_BOTH,
                              rect,                   // rect
                              this,       // CWnd* pParentWnd
                              ID_SLIDE_WW)) {     // UINT  nID
        return -1;
    }

    // Not required to be visible, so we hide it by not using//
    // the WS_VISIBLE style bit //

    if(!m_wndSpin_WW.Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | // dwStyle
                            UDS_SETBUDDYINT | UDS_AUTOBUDDY,
                            rect,                   // rect
                            this,       // CWnd* pParentWnd
                            ID_SPIN_WW)) {      // UINT  nID
        return -1;
    }

    //################# WL Slider/Edit set #################
    n++;
    n++;/*n++;*/
    SetButtonInfo(n, ID_EDIT_WL, TBBS_SEPARATOR , cxChar * 6);
    GetItemRect(n, &rect);
    rect.bottom = rect.top + (cyChar + 6);

    if(!m_wndEdit_WL.CreateEx(WS_EX_CLIENTEDGE, // Make a 3D-border
                              _T("EDIT"), NULL,
                              //WS_TABSTOP |
                              ES_READONLY | ES_AUTOHSCROLL | ES_LEFT | ES_NOHIDESEL | WS_VISIBLE | WS_CHILD,
                              rect, this, ID_EDIT_WL)) {
        return -1;
    }

    m_wndEdit_WL.SetFont(&m_font);
    n++;
    SetButtonInfo(n, ID_SLIDE_WL, TBBS_SEPARATOR, cxChar * 12);
    GetItemRect(n, &rect);
    rect.bottom = rect.top + (cyChar + 2);

    if(!m_wndSlider_WL.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | // dwStyle
                              TBS_HORZ |
                              //TBS_AUTOTICKS |
                              TBS_NOTICKS | TBS_TOOLTIPS |
                              TBS_BOTH,
                              rect,                   // rect
                              this,       // CWnd* pParentWndntWnd
                              ID_SLIDE_WL)) {     // UINT  nID
        return -1;
    }

    // Not required to be visible, so we hide it by not using//
    // the WS_VISIBLE style bit //

    if(!m_wndSpin_WL.Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | // dwStyle
                            UDS_SETBUDDYINT | UDS_AUTOBUDDY ,
                            rect,                   // rect
                            this,       // CWnd* pParentWnd
                            ID_SPIN_WL)) {      // UINT  nID
        return -1;
    }

#if 0
    //################# Rot Z Slider/Edit set #################
    SetButtonInfo(4, ID_EDIT_ROTZ, TBBS_SEPARATOR , cxChar * 8);
    GetItemRect(4, &rect);
    rect.bottom = rect.top + (cyChar + 6);

    if(!m_wndEdit_Rot_Z.CreateEx(WS_EX_CLIENTEDGE, // Make a 3D-border
                                 _T("EDIT"), NULL,
                                 //WS_TABSTOP |
                                 ES_READONLY | ES_AUTOHSCROLL | ES_LEFT | ES_NOHIDESEL | WS_VISIBLE | WS_CHILD,
                                 rect, this, ID_EDIT_ROTZ)) {
        return -1;
    }

    m_wndEdit_Rot_Z.SetFont(&m_font);
    SetButtonInfo(5, ID_SLIDE_ROTZ, TBBS_SEPARATOR, cxChar * 12);
    GetItemRect(5, &rect);
    rect.bottom = rect.top + (cyChar + 2);

    if(!m_wndSlider_Rot_Z.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | // dwStyle
                                 TBS_HORZ |
                                 //TBS_AUTOTICKS |
                                 TBS_NOTICKS |
                                 TBS_BOTH,
                                 rect,                   // rect
                                 this,       // CWnd* pParentWndntWnd
                                 ID_SLIDE_ROTZ)) {   // UINT  nID
        return -1;
    }

    // Not required to be visible, so we hide it by not using//
    // the WS_VISIBLE style bit //

    if(!m_wndSpin_Rot_Z.Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT // dwStyle
                               UDS_AUTOBUDDY | UDS_SETBUDDYINT,
                               rect,                   // rect
                               this,       // CWnd* pParentWnd
                               ID_SPIN_ROTZ)) {    // UINT  nID
        return -1;
    }

#endif
    CString cs(_T("WW"));

    if(m_wndStatic_WW.GetSafeHwnd()) {
        m_wndStatic_WW.SetWindowText(cs);
    }

    SetPositions();
    m_wndSpin_WW.SetsliderCtrl(&m_wndSlider_WW);
    m_wndSpin_WL.SetsliderCtrl(&m_wndSlider_WL);
    return 0;
}


void CSlideToolBar::SetPositions()
{
    m_wndSlider_WW.SetRange(1, 100);
    m_wndSlider_WL.SetRange(1, 100);
//  m_wndSlider_Rot_Z.SetRange(-100, 100);
    m_wndSpin_WW.SetRange(1, 100);
    m_wndSpin_WL.SetRange(1, 100);
//  m_wndSpin_Rot_Z.SetRange(-100, 100);
    m_wndSpin_WW.SetBuddy(&m_wndEdit_WW);
    m_wndSpin_WL.SetBuddy(&m_wndEdit_WL);
//  m_wndSpin_Rot_Z.SetBuddy(&m_wndEdit_Rot_Z);
    m_wndSlider_WW.SetPos(m_WWvalue);
    m_wndSlider_WL.SetPos(m_WLvalue);
//  m_wndSlider_Rot_Z.SetPos(m_nRotation[2]);
    m_wndSpin_WW.SetPos(m_WWvalue);
    m_wndSpin_WL.SetPos(m_WLvalue);
//  m_wndSpin_Rot_Z.SetPos(m_nRotation[2]);
}

void CSlideToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    UNREFERENCED_PARAMETER(nSBCode);
    UNREFERENCED_PARAMETER(nPos);
    UNREFERENCED_PARAMETER(pScrollBar);
    UpdateValues();
//  HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_SLIDER);
//  ::SetCursor(hCursor);
    CMainFrame *pChild = NULL;
    CMDIFrameWnd *pFrame = NULL;
    CFormImageViewer *tmp = NULL;
    pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();

    // Get the active MDI child window.
    if(pFrame) {
        pChild = (CMainFrame*)pFrame->GetActiveFrame();
    }

    if(pChild) {
        tmp = pChild->GetFormRightView();
    }

    if(tmp && tmp->GetSafeHwnd()) {
        Sleep(10);
        tmp->PostMessage(WM_CMD_WWWL, (WPARAM)m_WWvalue, (LPARAM)m_WLvalue);
    }

    CToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSlideToolBar::UpdateValues()
{
    m_wndSpin_WW.SetPos(m_wndSlider_WW.GetPos());
    m_wndSpin_WL.SetPos(m_wndSlider_WL.GetPos());
//  m_wndSpin_Rot_Z.SetPos(m_wndSlider_Rot_Z.GetPos());
    m_WWvalue = m_wndSpin_WW.GetPos();
    m_WLvalue = m_wndSpin_WL.GetPos();
}

void CSlideToolBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CToolBar::OnShowWindow(bShow, nStatus);
    // TODO: Add your message handler code here
}

void CSlideToolBar::OnSize(UINT nType, int cx, int cy)
{
    CToolBar::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here
}

void CSlideToolBar::GetWWWL(int &WW, int &WL)
{
    WW = m_WWvalue;
    WL = m_WLvalue;
}

void CSlideToolBar::UpdateFromSpin()
{
    m_WWvalue = m_wndSpin_WW.GetPos();
    m_WLvalue = m_wndSpin_WL.GetPos();
    CMainFrame *pChild = NULL;
    CMDIFrameWnd *pFrame = NULL;
    CFormImageViewer *tmp = NULL;
    pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();

    // Get the active MDI child window.
    if(pFrame) {
        pChild = (CMainFrame*)pFrame->GetActiveFrame();
    }

    if(pChild) {
        tmp = pChild->GetFormRightView();
    }

    if(tmp && tmp->GetSafeHwnd()) {
        Sleep(5);
        tmp->PostMessage(WM_CMD_WWWL, (WPARAM)m_WWvalue, (LPARAM)m_WLvalue);
    }
}
