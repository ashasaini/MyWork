// OpenGLControl.h : header file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_OPENGLCONTROL_H__A4185A1A_3853_41CB_81E0_D2019ADB65D2__INCLUDED_)
#define AFX_OPENGLCONTROL_H__A4185A1A_3853_41CB_81E0_D2019ADB65D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// COpenGLControl window

#include <GL\gl.h>
#include <GL\glu.h>
#include "OpenGLDevice.h"
#define ZPERCENT 1.0f//0.10f
class COpenGLControl : public CWnd
{
// Construction
public:
    COpenGLControl();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COpenGLControl)
    //}}AFX_VIRTUAL

// Implementation
public:

    void ZoomFromCenter();
    void Pan(CPoint point);
    void CdntMappingToImgDim(CPoint point);

    void Create(CRect rect, CWnd *parent, CString cs, int Uid);

    virtual ~COpenGLControl();
    void ZoomUp();
    void ZoomDown();
    void ZoomReset();
    void PanReset();
    void SetZoomFactor(int zPercent);
    int getZoomFactor();
    void ResetPointOfInterest();
    void setImageBufferPointer(void *ptr);
    void SetDimensionInPixel(int f_HeightInPixel, int f_WidthInPixel);
    void SetT1MinMaxForScale(double f_T1Min, double f_T1Max);
    void ResetPointInsideImager();
    BOOL IsPointInsideImager();
    int GetWinUid();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    HWND GetOwnHandle();

    // Generated message map functions
private:
    int m_x_centerZoom_offset;
    CClientDC* dc;
    int m_HeightInPixel;
    int m_WidthInPixel;
    CString m_ClassName;
    CPoint m_TopLeftMpt;
    CPoint m_TopLeftSpt;
    CPoint m_PtOfInterest;
    CPoint m_OldPoint;
    int m_WinHeight;
    int m_WinWidth;
    int ZoomFlag;
    float rotation;
    int transX;
    int transY;
    float OldTransX;
    float OldTransY;
    float zoomFactor;
    float zoomPix;
    float ZoomPercent;
    int PanFlag;
    void *ImageBuffer;
    GLfloat rasterX;
    GLfloat rasterY;
    double m_T1Min;
    double m_T1Max;
    int m_centerZoom_offset_y;//MPower 069/AN/Added
    bool top_aligned;//MPower 069/AN/Added
    COLORREF        m_Background;
    HBRUSH      m_hBrush;
    BOOL m_InsideImager;
    int m_WinUid;
    HWND m_OwnHandle;
    int m_CoordinateX;
    int m_CoordinateY;
protected:
    void RenderTexture();
    void InitGL();
    void DrawGLScene();

    void SaveRectLocally(CRect rect, CString &className);
    void ColorScaleDrawString(int x, int y, char *string);

    void SavePointOfInterest(CPoint &pt);
    void UpdateRasterPoint();
    OpenGLDevice openGLDevice;
    //{{AFX_MSG(COpenGLControl)
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLCONTROL_H__A4185A1A_3853_41CB_81E0_D2019ADB65D2__INCLUDED_)
