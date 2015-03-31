// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EPI.h"
#include "MainFrm.h"
#include <stdlib.h>
#include <cderr.h>   // For CommDlgExtendedError

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
CString CMainFrame::szCustomDefFilter(_T("All Files (*.*)|*.*|Raw Files (*.raw)|*.bad||"));


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_UPDATE_COMMAND_UI(ID_BUTTON_1x1, OnUpdateBUTTON1x1)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_1x2, OnUpdateBUTTON1x2)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_2x2, OnUpdateBUTTON2x2)
    ON_COMMAND(ID_BUTTON_1x1, OnBUTTON1x1)
    ON_COMMAND(ID_BUTTON_1x2, OnBUTTON1x2)
    ON_COMMAND(ID_BUTTON_2x2, OnBUTTON2x2)
    ON_COMMAND(ID_BUTTON_RESETIMAGE, OnButtonResetimage)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_RESETIMAGE, OnUpdateButtonResetimage)
    ON_COMMAND(ID_BUTTON_FITIMAGE, OnButtonFitimage)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_FITIMAGE, OnUpdateButtonFitimage)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_COMMAND(ID_VIEW_WWWL, OnWWWL)
    ON_COMMAND(ID_VIEW_IMAGER, OnImager)
    ON_UPDATE_COMMAND_UI(ID_VIEW_WWWL, OnWWWLUI)
    ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGER, OnImagerUI)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnTooltipHandler)
    ON_COMMAND(ID_BUTTON_OPENIMAGE, OnButtonOpenimage)
    ON_UPDATE_COMMAND_UI(ID_BUTTON_OPENIMAGE, OnUpdateButtonOpenimage)
    ON_COMMAND(ID_DIRECTORY_SELECT, OnDirectorySelect)
    ON_UPDATE_COMMAND_UI(ID_DIRECTORY_SELECT, OnUpdateDirectorySelect)
    ON_COMMAND(ID_BUTTON_REFRESH, OnButtonRefreshStudyFile)
	ON_COMMAND(ID_BUTTON_OPEN_RAW, OnButtonOpenRaw)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_OPEN_RAW, OnUpdateButtonOpenRaw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_INITIAL_DIR,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

/*************************************************************
* Method:       CMainFrame
* Description:  This function is used
* Returns:
*************************************************************/
CMainFrame::CMainFrame(): m_CheckFlagImager(TRUE), m_CheckFlagWWWL(TRUE), m_szInitialDir(_T("C:/EPI_Data")),
    m_ImgLstDlg(NULL)
{
    // TODO: add member initialization code here
}

/*************************************************************
* Method:       ~CMainFrame
* Description:  This function is used
* Returns:
*************************************************************/
CMainFrame::~CMainFrame()
{
    if(m_ImgLstDlg) {
        if(m_ImgLstDlg->GetSafeHwnd()) {
            m_ImgLstDlg->DestroyWindow();
        }

        delete m_ImgLstDlg;
        m_ImgLstDlg = NULL;
    }
}

/*************************************************************
* Method:       OnCreate
* Description:  This function is used
* Parameter:    LPCREATESTRUCT lpCreateStruct :
* Returns:      int
*************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if(CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
                              | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
            !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if(!m_wndToolBarImager.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
                                    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
            !m_wndToolBarImager.LoadToolBar(IDR_IMAGER_TOOLBAR)) {
        TRACE0("Failed to create IDR_IMAGER_TOOLBAR\n");
        return -1;      // fail to create
    }

    if(!m_wndStatusBar.Create(this) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    //CHANGE:
    m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_INITIAL_DIR, SBPS_NORMAL, _MAX_PATH * 2/*256*/);
    m_wndStatusBar.SetPaneText(1, m_szInitialDir, TRUE);
    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    // Allow the dialog bar to be dockable
    // on the left or the right of the frame
    m_wndToolBarImager.EnableDocking(CBRS_ALIGN_ANY);
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);
    DockControlBar(&m_wndToolBarImager, AFX_IDW_DOCKBAR_RIGHT);
    CreateSlideBar();
    m_wndToolBarImager.EnableToolTips(TRUE);
    m_wndSlideBar.EnableToolTips(TRUE);
    return 0;
}

/*************************************************************
* Method:       OnCreateClient
* Description:  This function is used
* Parameter:    LPCREATESTRUCT  :
* Parameter:    CCreateContext * pContext :
* Returns:      BOOL
*************************************************************/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
                                CCreateContext* pContext)
{
    if(!m_wndSplitter.CreateStatic(this, 1, 2)) {
        return FALSE;
    }

    //SAMPLE: create a view like we were asked
    // initial size of 100, 100 is arbitrary
    pContext->m_pNewViewClass = RUNTIME_CLASS(CEPIView);
    CRect cr;
    GetWindowRect( &cr );
    int l_nHeight = cr.Height();
    int l_nWidth = cr.Width();

    if(!m_wndSplitter.CreateView(0, 0,
                                 pContext->m_pNewViewClass, CSize(2 * l_nWidth/5, l_nHeight), pContext)) {
        return FALSE;
    }

    if(!m_wndSplitter2.CreateStatic(&m_wndSplitter,
                                    3, 1,
                                    WS_CHILD | WS_VISIBLE | WS_BORDER,
                                    m_wndSplitter.IdFromRowCol(0, 1))) {
        MessageBox("Error setting up m_mainSplitter", "ERROR", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if(!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CFormImageViewer),
                                  CSize(0, (3* l_nHeight/5 + 90)), pContext)) {
        m_wndSplitter2.DestroyWindow();
        return FALSE;
    }

    if(!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CFormImageSelect),
                                  CSize(0, (2* l_nHeight/5) - 40), pContext)) {
        m_wndSplitter2.DestroyWindow();
        return FALSE;
    }

    if(!m_wndSplitter2.CreateView(2, 0, RUNTIME_CLASS(CFormStatus),
                                  CSize(0, 0), pContext)) {
        m_wndSplitter2.DestroyWindow();
        return FALSE;
    }

    m_wndSplitter2.RecalcLayout();
    return TRUE;
}

/*************************************************************
* Method:       PreCreateWindow
* Description:  This function is used
* Parameter:    CREATESTRUCT & cs :
* Returns:      BOOL
*************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if(!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
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


/*************************************************************
* Method:       GetEPIFormView
* Description:  This function is used
* Returns:      CFormImageSelect *
*************************************************************/
CFormImageSelect * CMainFrame::GetEPIFormView()
{
    //m_wndSplitter.GetActivePane()
    int row = 1;
    int col = 0;
    m_wndSplitter2.SetActivePane(1, 0);
    return (CFormImageSelect*)m_wndSplitter2.GetActivePane(&row, &col);
    //return pWnd;
}

/*************************************************************
* Method:       GetFormRightView
* Description:  This function is used
* Returns:      CFormImageViewer *
*************************************************************/
CFormImageViewer * CMainFrame::GetFormRightView()
{
    int row = 0;
    int col = 0;
    m_wndSplitter2.SetActivePane(0, 0);
    return (CFormImageViewer*)m_wndSplitter2.GetActivePane(&row, &col);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


/*************************************************************
* Method:       OnUpdateBUTTON1x1
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnUpdateBUTTON1x1(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
    // TODO: Add your command update UI handler code here
}

/*************************************************************
* Method:       OnUpdateBUTTON1x2
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnUpdateBUTTON1x2(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
}

/*************************************************************
* Method:       OnUpdateBUTTON2x2
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnUpdateBUTTON2x2(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
}

/*************************************************************
* Method:       OnBUTTON1x1
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnBUTTON1x1()
{
    // TODO: Add your command handler code here
    CFormImageViewer *ptr  = NULL;
    ptr = GetFormRightView();

    if(NULL != ptr) {
        ptr->OnBUTTON1x1();
    }
}

/*************************************************************
* Method:       OnBUTTON1x2
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnBUTTON1x2()
{
    // TODO: Add your command handler code here
    CFormImageViewer *ptr  = NULL;
    ptr = GetFormRightView();

    if(NULL != ptr) {
        ptr->OnBUTTON1x2();
    }
}

/*************************************************************
* Method:       OnBUTTON2x2
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnBUTTON2x2()
{
    // TODO: Add your command handler code here
    CFormImageViewer *ptr  = NULL;
    ptr = GetFormRightView();

    if(NULL != ptr) {
        ptr->OnBUTTON2x2();
    }
}

/*************************************************************
* Method:       OnButtonResetimage
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnButtonResetimage()
{
    // TODO: Add your command handler code here
    CFormImageViewer *ptr  = NULL;
    ptr = GetFormRightView();

    if(NULL != ptr) {
        ptr->OnButtonResetimage();
    }
}

/*************************************************************
* Method:       OnUpdateButtonResetimage
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnUpdateButtonResetimage(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
}

/*************************************************************
* Method:       OnButtonFitimage
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnButtonFitimage()
{
    // TODO: Add your command handler code here
    CFormImageViewer *ptr  = NULL;
    ptr = GetFormRightView();

    if(NULL != ptr) {
        ptr->OnButtonFitimage();
    }
}

/*************************************************************
* Method:       OnUpdateButtonFitimage
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnUpdateButtonFitimage(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
}

/*************************************************************
* Method:       CreateSlideBar
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CMainFrame::CreateSlideBar()
{
    if(!m_wndSlideBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT | CBRS_ORIENT_HORZ
                               /*| CBRS_GRIPPER*/ | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)) {
        return FALSE;
    }

    m_wndSlideBar.LoadToolBar(IDR_SLIDER_BAR);
    m_wndSlideBar.SetWindowText(_T("WW/WL:"));
    m_wndSlideBar.EnableDocking(/*CBRS_ALIGN_ANY*/CBRS_ALIGN_RIGHT);
    DockControlBarLeftOf(&m_wndSlideBar, &m_wndToolBar);
    m_wndSlideBar.ShowWindow(SW_SHOW);
    return TRUE;
}


/*************************************************************
* Method:       DockControlBarLeftOf
* Description:  This function is used
* Parameter:    CToolBar * Bar :
* Parameter:    CToolBar * LeftOf :
* Returns:      void
*************************************************************/
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
    CRect rect;
    CRect newRect;
    CRect rectbar;
    DWORD dw;
    UINT n;
    // get MFC to adjust the dimensions of all docked ToolBars
    // so that GetWindowRect will be accurate
    RecalcLayout(TRUE);
    GetWindowRect(&rectbar);
    LeftOf->GetWindowRect(&rect);
    rect.OffsetRect(rectbar.right - rect.Width(), 0);
    dw = LeftOf->GetBarStyle();
    n = 0;
    n = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
    n = (dw&CBRS_ALIGN_BOTTOM && n == 0) ?
        AFX_IDW_DOCKBAR_BOTTOM : n;
    n = (dw&CBRS_ALIGN_LEFT && n == 0) ?
        AFX_IDW_DOCKBAR_LEFT : n;
    n = (dw&CBRS_ALIGN_RIGHT && n == 0) ?
        AFX_IDW_DOCKBAR_RIGHT : n;
    // When we take the default parameters on rect, DockControlBar
    // will dock each Toolbar on a seperate line. By calculating a
    // rectangle, we are simulating a Toolbar being dragged to that
    // location and docked.
    DockControlBar(Bar, n, &rect);
}

/*************************************************************
* Method:       OnSize
* Description:  This function is used
* Parameter:    UINT nType :
* Parameter:    int cx :
* Parameter:    int cy :
* Returns:      void
*************************************************************/
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CFrameWnd::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here

    if(m_wndSlideBar.GetSafeHwnd() && m_wndToolBar.GetSafeHwnd()) {
        if(m_wndSlideBar.IsWindowVisible()) {
            DockControlBarLeftOf(&m_wndSlideBar, &m_wndToolBar);
        }
    }
}

/*************************************************************
* Method:       OnDestroy
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnDestroy()
{
    CFrameWnd::OnDestroy();
    // TODO: Add your message handler code here
}

/*************************************************************
* Method:       OnWWWL
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnWWWL()
{
    m_CheckFlagWWWL = !m_CheckFlagWWWL;

    if(TRUE == m_wndSlideBar.IsWindowVisible()) {
        m_wndSlideBar.ShowWindow(SW_HIDE);
    } else {
        m_wndSlideBar.ShowWindow(SW_SHOW);

        if(m_wndSlideBar.GetSafeHwnd() && m_wndToolBar.GetSafeHwnd()) {
            DockControlBarLeftOf(&m_wndSlideBar, &m_wndToolBar);
        }
    }
}

/*************************************************************
* Method:       OnImager
* Description:  This function is used
* Returns:      void
*************************************************************/
void CMainFrame::OnImager()
{
    m_CheckFlagImager = !m_CheckFlagImager;

    if(TRUE == m_wndToolBarImager.IsWindowVisible()) {
        m_wndToolBarImager.ShowWindow(SW_HIDE);
    } else {
        m_wndToolBarImager.ShowWindow(SW_SHOW);
    }
}

/*************************************************************
* Method:       OnWWWLUI
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnWWWLUI(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_CheckFlagWWWL);
}

/*************************************************************
* Method:       OnImagerUI
* Description:  This function is used
* Parameter:    CCmdUI * pCmdUI :
* Returns:      void
*************************************************************/
void CMainFrame::OnImagerUI(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_CheckFlagImager);
}

BOOL CMainFrame::OnTooltipHandler(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    UNREFERENCED_PARAMETER(pResult);
    UNREFERENCED_PARAMETER(id);
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = static_cast<int>(pNMHDR->idFrom);
    CString strTip = _T("");

    switch(nID) {
        case ID_BUTTON_2x2 :
            strTip = "2x2";
            break;
        case ID_BUTTON_1x2 :
            strTip = "1x2";
            break;
        case ID_BUTTON_1x1 :
            strTip = "1x1";
            break;
        case ID_BUTTON_RESETIMAGE :
            strTip = "Reset";
            break;
        case ID_BUTTON_FITIMAGE :
            strTip = "Fit_Stretch";
            break;
        case ID_FILE_OPEN:
            strTip = "Open StudyFile";
            break;
        case ID_BUTTON_OPENIMAGE:
            strTip = "Open Images";
            break;
        case ID_FILE_SAVE:
            strTip = "Save RawData";
            break;
        case ID_BUTTON_REFRESH:
            strTip = "Refresh Study";
            break;
        case ID_BUTTON_OPEN_RAW:
            strTip = "Open Raw File";
            break;

    }

    _tcscpy(pTTT->szText, strTip.GetBuffer(0));
    strTip.ReleaseBuffer();
    return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    return CFrameWnd::PreTranslateMessage(pMsg);
}

CFormStatus * CMainFrame::getImageInfoFormView()
{
    int row = 1;
    int col = 0;
    m_wndSplitter2.SetActivePane(1, 0);
    return (CFormStatus*)m_wndSplitter2.GetActivePane(&row, &col);
}
CFormStatus * CMainFrame::getErrorInfoFormView()
{
    int row = 2;
    int col = 0;
    m_wndSplitter2.SetActivePane(2, 0);
    return (CFormStatus*)m_wndSplitter2.GetActivePane(&row, &col);
}


void CMainFrame::OnButtonOpenimage()
{
#define MAX_CFileDialog_FILE_COUNT 99
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)
    // TODO: Add your command handler code here
    SetMessageText(_T("Select Files!"));
    //POSITION pos;
    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_LONGNAMES
                    | OFN_SHOWHELP | OFN_NODEREFERENCELINKS;
    //dwFlags |= OFN_EXPLORER;
    //dwFlags |= OFN_ENABLETEMPLATE;
    dwFlags |= OFN_ALLOWMULTISELECT;
    CFileDialog m_CustomfileDlg(TRUE, NULL, NULL, dwFlags, szCustomDefFilter, this);
    m_CustomfileDlg.m_ofn.Flags |= dwFlags;
    m_CustomfileDlg.m_ofn.Flags &= ~(OFN_EXPLORER | OFN_SHOWHELP);

    if(!m_szInitialDir.IsEmpty()) {
        m_CustomfileDlg.m_ofn.lpstrInitialDir = m_szInitialDir;
    }

    CString fileName;
    wchar_t* p = (wchar_t*)fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
    m_CustomfileDlg.m_ofn.lpstrFile = (LPSTR)p;
    m_CustomfileDlg.m_ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

    if(m_CustomfileDlg.DoModal() == IDCANCEL) {
        DWORD err;

        if((err = CommDlgExtendedError()) != 0) {
            CString strErrMsg;

            if(err == FNERR_BUFFERTOOSMALL) {
                unsigned short nBuffNeeded = (unsigned short)m_CustomfileDlg.m_ofn.lpstrFile;
                strErrMsg.Format("Filename buffer too small. It needs to be %d characters in size.",
                                 nBuffNeeded);
            } else {
                strErrMsg.Format("Common dialog error! (%x)", err);
            }

            AfxMessageBox(strErrMsg);
        }

        return;
    }

    fileName.ReleaseBuffer(); //cleanup
    SetMessageText(AFX_IDS_IDLEMESSAGE);

    if(m_ImgLstDlg == NULL) {
        m_listDisplayNamesPath.RemoveAll();
    }

    POSITION pos(m_CustomfileDlg.GetStartPosition());
    CString l_ListDlgTitle(_T(""));

    while(pos) {
        CString csFileName(m_CustomfileDlg.GetNextPathName(pos));
        m_listDisplayNamesPath.Add(csFileName);
    }

    if(NULL == m_ImgLstDlg) {
        m_ImgLstDlg = new CImageListDlg(this);
        m_ImgLstDlg->SetListPaths(m_listDisplayNamesPath);
        m_ImgLstDlg->Create(IDD_DIALOG_IMAGELIST, this);
        m_ImgLstDlg->ShowWindow(SW_NORMAL);
    } else {
        m_ImgLstDlg->ShowWindow(SW_SHOW);
        m_ImgLstDlg->SetListPaths(m_listDisplayNamesPath);
        m_ImgLstDlg->RefreshList();
    }
}

void CMainFrame::OnUpdateButtonOpenimage(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
}

void CMainFrame::OnDirectorySelect()
{
    // TODO: Add your command handler code here
    SetMessageText(_T("Select a Directory!"));
    // This is the recommended way to select a directory
    // in Win95 and NT4.
    BROWSEINFO bi;
    memset((LPVOID)&bi, 0, sizeof(bi));
    TCHAR szDisplayName[_MAX_PATH];
    szDisplayName[0] = '\0';
    bi.hwndOwner = GetSafeHwnd();
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szDisplayName;
    bi.lpszTitle = _T("Pick a folder, any folder:");
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    LPITEMIDLIST pIIL = ::SHBrowseForFolder(&bi);
    TCHAR szInitialDir[_MAX_PATH];
    BOOL bRet = ::SHGetPathFromIDList(pIIL, (char*)&szInitialDir);

    if(bRet) {
        CString l_tmp(szInitialDir);

        if(l_tmp.CompareNoCase(_T("")) != 0) {
            m_szInitialDir = szInitialDir;
            m_wndStatusBar.SetPaneText(1, m_szInitialDir, TRUE);
        }

        LPMALLOC pMalloc;
        HRESULT hr = SHGetMalloc(&pMalloc);
        UNREFERENCED_PARAMETER(hr);
        pMalloc->Free(pIIL);
        pMalloc->Release();
    }

    SetMessageText(AFX_IDS_IDLEMESSAGE);
}

void CMainFrame::OnUpdateDirectorySelect(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
}

void CMainFrame::OnButtonRefreshStudyFile()
{
    (CEPIController::GetInstance())->loadStudy(0);

}

void CMainFrame::OnButtonOpenRaw() 
{
#define MAX_CFileDialog_FILE_COUNT 99
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)
    // TODO: Add your command handler code here
    SetMessageText(_T("Select RawFile!"));
    //POSITION pos;
    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_LONGNAMES
        | OFN_SHOWHELP | OFN_NODEREFERENCELINKS;
    //dwFlags |= OFN_ALLOWMULTISELECT;
    CFileDialog m_CustomfileDlg(TRUE, NULL, NULL, dwFlags, szCustomDefFilter, this);
    m_CustomfileDlg.m_ofn.Flags |= dwFlags;
    m_CustomfileDlg.m_ofn.Flags &= ~(OFN_EXPLORER | OFN_SHOWHELP);
    
    if(!m_szInitialDir.IsEmpty()) {
        m_CustomfileDlg.m_ofn.lpstrInitialDir = m_szInitialDir;
    }
    
    CString fileName;
    wchar_t* p = (wchar_t*)fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
    m_CustomfileDlg.m_ofn.lpstrFile = (LPSTR)p;
    m_CustomfileDlg.m_ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;
    
    if(m_CustomfileDlg.DoModal() == IDCANCEL) {
        DWORD err;
        
        if((err = CommDlgExtendedError()) != 0) {
            CString strErrMsg;
            
            if(err == FNERR_BUFFERTOOSMALL) {
                unsigned short nBuffNeeded = (unsigned short)m_CustomfileDlg.m_ofn.lpstrFile;
                strErrMsg.Format("Filename buffer too small. It needs to be %d characters in size.",
                    nBuffNeeded);
            } else {
                strErrMsg.Format("Common dialog error! (%x)", err);
            }
            
            AfxMessageBox(strErrMsg);
        }
        
        return;
    }
    
    fileName.ReleaseBuffer(); //cleanup
    SetMessageText(AFX_IDS_IDLEMESSAGE);
    
    CString l_csFilePath(m_CustomfileDlg.GetPathName());


	COpenRawDlg l_RawDlg;
    l_RawDlg.SetRawFilePath(l_csFilePath);
    l_RawDlg.DoModal();
}

void CMainFrame::OnUpdateButtonOpenRaw(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);

}
