#if !defined(AFX_FORMRIGHT_H__9AFF10FF_4CB3_41A3_A314_907985ED7DF3__INCLUDED_)
#define AFX_FORMRIGHT_H__9AFF10FF_4CB3_41A3_A314_907985ED7DF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormRight.h : header file
//

#include "resource.h"
#include "EPIDoc.h"
#include "OpenGLControl.h"
/////////////////////////////////////////////////////////////////////////////
// CFormImageViewer form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define IDC_WINDOW_CONTROL1 5001
#define IDC_WINDOW_CONTROL2 5002
#define IDC_WINDOW_CONTROL3 5003
#define IDC_WINDOW_CONTROL4 5004

#define MODE1X1 0
#define MODE1X2 (MODE1X1+1)
#define MODE2X2 (MODE1X2+1)
#define MODE1S1 (MODE2X2+1)

class CFormImageViewer : public CFormView
{
protected:
    CFormImageViewer();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFormImageViewer)

// Form Data
public:
    //{{AFX_DATA(CFormImageViewer)
    enum { IDD = IDD_FORM_IMAGE_VIEWER };
    //}}AFX_DATA

// Attributes
public:

// Operations
public:
    void SetImagerUid(int uid);
    void SetChildHandles(HWND hndle);
    virtual void OnInitialUpdate();
    void OnBUTTON1x1();
    void OnBUTTON1x2();
    void OnBUTTON2x2();
    void OnButtonResetimage();
    void OnButtonFitimage();
    void DeleteAllImagers();
    void DeleteAllImages();
    void InvalidateEverything();
    COpenGLControl* CreateImager(int WinUid, CString &className, void*Buffer, int Ht, int Wt , BOOL stretched);
    int displayImageStudyRaw(const int &f_nWidth, const int &f_nHeight, double *&f_dIamgeData);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormImageViewer)
public:

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CFormImageViewer();
    void BackgroundColor(HDC hDC);
    BOOL ScrnToClt(HWND hWnd, RECT *pRect) ;
    void DrawLine(CPaintDC* f_dc , CPoint &f_start_point, CPoint &f_end_point);
    void GetAllImagerHandles();
    void DrawBracketsImager(HWND hWndChild, RECT &rc , CPaintDC &dc);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CFormImageViewer)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg LRESULT AdjustnConvertWWWLimage(WPARAM, LPARAM);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:

    double *m_ImgBuf1;
    double *m_ImgBuf2;
    double *m_ImgBuf3;
    double *m_ImgBuf4;
    void * m_ulColoredImg1;
    void * m_ulColoredImg2;
    void * m_ulColoredImg3;
    void * m_ulColoredImg4;

    COLORREF        m_Background;
    COpenGLControl *m_OpenGLControl1;
    COpenGLControl *m_OpenGLControl2;
    COpenGLControl *m_OpenGLControl3;
    COpenGLControl *m_OpenGLControl4;

    HWND m_ChildHandle;
    int m_Child_uid;
    int m_WinWidth ;
    int m_WinLevel ;
    int m_ImagerCurrentMode;
    int m_nImgWidth[4];
    int m_nImgHeight[4];

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMRIGHT_H__9AFF10FF_4CB3_41A3_A314_907985ED7DF3__INCLUDED_)
