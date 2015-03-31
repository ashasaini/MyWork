// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D8F1CA72_3A01_4818_A5C0_5AE1DD57C649__INCLUDED_)
#define AFX_MAINFRM_H__D8F1CA72_3A01_4818_A5C0_5AE1DD57C649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FormImageViewer.h"
#include "EPIView.h"
#include "FormImageSelect.h"
#include "SlideToolBar.h"
#include "FormStatus.h"
#include "ImageListDlg.h"
#include "OpenRawDlg.h"

class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
    CSplitterWnd m_wndSplitter;
    CSplitterWnd m_wndSplitter2;
//    CSplitterWnd m_wndSplitter3;
//    CSplitterWnd m_wndSplitter4;
public:
    CSlideToolBar m_wndSlideBar;
    static CString szCustomDefFilter;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
public:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    CFormImageViewer * GetFormRightView();
    CFormImageSelect * GetEPIFormView();
    CFormStatus *getImageInfoFormView();
    CFormStatus * getErrorInfoFormView();

protected:  // control bar embedded members
    CStatusBar  m_wndStatusBar;
    CToolBar    m_wndToolBar;
    CToolBar    m_wndToolBarImager;


    BOOL CreateSlideBar();
    void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
private:
    BOOL m_CheckFlagImager;
    BOOL m_CheckFlagWWWL;
    CString m_szInitialDir;
    CImageListDlg *m_ImgLstDlg;
    CStringArray m_listDisplayNamesPath;

// Generated message map functions
protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnUpdateBUTTON1x1(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBUTTON1x2(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBUTTON2x2(CCmdUI* pCmdUI);
    afx_msg void OnBUTTON1x1();
    afx_msg void OnBUTTON1x2();
    afx_msg void OnBUTTON2x2();
    afx_msg void OnButtonResetimage();
    afx_msg void OnUpdateButtonResetimage(CCmdUI* pCmdUI);
    afx_msg void OnButtonFitimage();
    afx_msg void OnUpdateButtonFitimage(CCmdUI* pCmdUI);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
    afx_msg void OnWWWL();
    afx_msg void OnImager();
    afx_msg void OnWWWLUI(CCmdUI* pCmdUI);
    afx_msg void OnImagerUI(CCmdUI* pCmdUI);
    afx_msg BOOL OnTooltipHandler(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnButtonOpenimage();
    afx_msg void OnUpdateButtonOpenimage(CCmdUI* pCmdUI);
    afx_msg void OnDirectorySelect();
    afx_msg void OnUpdateDirectorySelect(CCmdUI* pCmdUI);
    afx_msg void OnButtonRefreshStudyFile();
	afx_msg void OnButtonOpenRaw();
	afx_msg void OnUpdateButtonOpenRaw(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D8F1CA72_3A01_4818_A5C0_5AE1DD57C649__INCLUDED_)
