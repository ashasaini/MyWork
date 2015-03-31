// OpenGLControl.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "OpenGLControl.h"
#include "EPILogHelper.h"
#include "FormImageViewer.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_ZOOM 8.0f //AN
/////////////////////////////////////////////////////////////////////////////
// COpenGLControl

COpenGLControl::COpenGLControl():
    m_OwnHandle(NULL)
{
    dc = NULL;
    rotation = 0.0f;
    m_HeightInPixel = 0;
    m_WidthInPixel = 0;
    transX = 0;
    transY = 0;
    OldTransX = 0.0f;
    OldTransY = 0.0f;
    zoomFactor = 1.0f;
    ZoomPercent = ZPERCENT;
    PanFlag = 0;
    ZoomFlag = 0;
    ImageBuffer = NULL;
    m_WinHeight = 1;
    m_WinWidth = 1;
    rasterY = 0.0f;
    rasterX = 0.0f;
    m_T1Min = 0.0;
    m_T1Max = 0.0;
    m_x_centerZoom_offset = 0;
    m_centerZoom_offset_y = 0;
    m_Background = RGB(100, 100, 100);
    m_hBrush = NULL;
    m_InsideImager = FALSE;
    m_WinUid = 0;
    m_CoordinateX = 0;
    m_CoordinateY = 0;
}

COpenGLControl::~COpenGLControl()
{
    if(dc) {
        delete dc;
        dc = NULL;
    }

    ImageBuffer = NULL; //do not delete this only NULL //KJ
    DeleteObject(m_hBrush);
    DestroyWindow();
}


BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
    //{{AFX_MSG_MAP(COpenGLControl)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEWHEEL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGLControl message handlers

/*************************************************************
* Method:       OnEraseBkgnd
* Description:  This function is used
* Parameter:    CDC * pDC :
* Returns:      BOOL
*************************************************************/
BOOL COpenGLControl::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    UNREFERENCED_PARAMETER(pDC);
    return TRUE;
    //return CWnd::OnEraseBkgnd(pDC);
}
/*************************************************************
* Method:       OnPaint
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::OnPaint()
{
//     rotation += 0.01f;
//
//     if (rotation >= 360.0f)
//     {
//         rotation -= 360.0f;
//     }
//
    /** OpenGL section **/
    openGLDevice.makeCurrent();
    DrawGLScene();
    openGLDevice.makeCurrent(false);
    CWnd::OnPaint();
}


/*************************************************************
* Method:       OnCreate
* Description:  This function is used
* Parameter:    LPCREATESTRUCT lpCreateStruct :
* Returns:      int
*************************************************************/
int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try {
        if(CWnd::OnCreate(lpCreateStruct) == -1) {
            return -1;
        }

        if(dc) {
            delete dc;
            dc = NULL;
        }

        dc = new CClientDC(this);

        if(dc == NULL) {
            return -1;
        }

        openGLDevice.create(dc->m_hDC);
        InitGL();
        return 0;
    } catch(...) {
        return 1;
    }
}

/*************************************************************
* Method:       OnSize
* Description:  This function is used
* Parameter:    UINT nType :
* Parameter:    int cx :
* Parameter:    int cy :
* Returns:      void
*************************************************************/
void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
    try {
        CWnd::OnSize(nType, cx, cy);

        if(cy == 0) {
            cy = 1;
        }

        CWnd::OnSize(nType, cx, cy);

        if(cy == 0) {
            cy = 1;
        }

        m_WinHeight = cy;
        m_WinWidth = cx;
        CPoint cp;
        cp.x = cx / 2;
        cp.y = cy / 2;
        int zmx = 0, zmy = 0;
        float zoom = 0;
        zmy = (int)ceil((double)m_WinHeight / (double)m_HeightInPixel);
        zmx = (int)ceil((double)m_WinWidth / (double)m_WidthInPixel);
        zoom = float(zmy > zmx ? zmy : zmx);
        zoomFactor = zoom;
        ZoomFromCenter();
        PanFlag = false;
        SavePointOfInterest(cp);
        openGLDevice.makeCurrent(true);
        UpdateRasterPoint();
        DrawGLScene();
        // TODO: Add your message handler code here
    } catch(...) {
    }
}
//////////////////////////////////////////////////////////////////////////
///Member FUnctions
/*************************************************************
* Method:       InitGL
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::InitGL()
{
    //RenderTexture();
    //Disable Others
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_STENCIL_TEST);
    glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
    glPixelTransferi(GL_RED_SCALE, 1);
    glPixelTransferi(GL_RED_BIAS, 0);
    glPixelTransferi(GL_GREEN_SCALE, 1);
    glPixelTransferi(GL_GREEN_BIAS, 0);
    glPixelTransferi(GL_BLUE_SCALE, 1);
    glPixelTransferi(GL_BLUE_BIAS, 0);
    glPixelTransferi(GL_ALPHA_SCALE, 1);
    glPixelTransferi(GL_ALPHA_BIAS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 8) ;
    glPixelStorei(GL_PACK_ALIGNMENT, 8) ;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLoadIdentity();
}



/*************************************************************
* Method:       DrawGLScene
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::DrawGLScene()
{
    try {
        float ht = 1;
        float wt = 1;
        ht = (float)m_HeightInPixel * zoomFactor;
        wt = (float)m_WidthInPixel * zoomFactor;
        // ht = m_WinHeight;
        //wt = m_WinWidth;
        int final_y_trans = 0;
        final_y_trans = transY + m_centerZoom_offset_y;//AN
        int final_x_trans = 0;
        final_x_trans = transX + m_x_centerZoom_offset;

        glViewport((int)(final_x_trans), (int)(final_y_trans), (int)(wt + (final_x_trans)), (int)(ht + (final_y_trans))); //AN/Modified
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, wt, ht, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        //==============================================================================

        //***************************
        // DRAWING CODE
        //***************************
        glPushMatrix();

        glPixelZoom((zoomFactor), -(zoomFactor));
        glRasterPos2f(0, 0);

        if(ImageBuffer) {
            glDrawPixels(m_WidthInPixel, m_HeightInPixel, GL_RGBA, GL_UNSIGNED_BYTE, ImageBuffer);
        }
        glPopMatrix();
        //========================================================================        

        glViewport(0, 0, m_WidthInPixel , m_HeightInPixel); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_WidthInPixel, 0, m_HeightInPixel, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glPushMatrix();
        glPixelZoom((1.0f), (1.0f));

        char buf[256] = {'\0'};
        sprintf( buf, "(%d,%d)",
            m_CoordinateX, m_CoordinateY );
        glColor3f( 0.0f, 0.0f, 0.0f );
        ColorScaleDrawString(5,40, buf );        
    
        glColor3f( 0.0f, 1.0f, 0.0f );
        ColorScaleDrawString(4,39, buf );  
            
        sprintf( buf, "%d x %d",
            m_WidthInPixel, m_HeightInPixel );
        glColor3f( 0.0f, 0.0f, 0.0f );
        ColorScaleDrawString(5,5, buf );        

        glColor3f( 0.0f, 1.0f, 0.0f );
        ColorScaleDrawString(4,4, buf );        
        
        sprintf( buf, "%d%%",(int)(zoomFactor*100));

        glColor3f( 0.0f, 0.0f, 0.0f );
        ColorScaleDrawString(5,20, buf );        
        
        glColor3f( 0.0f, 1.0f, 0.0f );
        ColorScaleDrawString(4,19, buf );
        glPopMatrix();

//==============================================================================
        glFinish();
        glFlush();
        SwapBuffers(dc->m_hDC);
    } catch(...) {
    }
}


/*************************************************************
* Method:       Create
* Description:  This function is used
* Parameter:    CRect rect :
* Parameter:    CWnd * parent :
* Parameter:    CString cs :
* Returns:      void
*************************************************************/
void COpenGLControl::Create(CRect rect, CWnd *parent, CString cs, int uid)
{
    try {
        m_hBrush = CreateSolidBrush(m_Background);
        CString className = AfxRegisterWndClass(
                                CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS | CS_PARENTDC,
                                AfxGetApp()->LoadStandardCursor(IDC_CROSS),
                                (HBRUSH)GetStockObject(DKGRAY_BRUSH)/*m_hBrush*/,
                                NULL);
        SaveRectLocally(rect, cs); //Keep It before CreateEx()
        CPoint cp;
        cp.x = (rect.Width()) / 2;
        cp.y = (rect.Height()) / 2;
        SavePointOfInterest(cp);

        switch(uid) {
            case IDC_WINDOW_CONTROL1:
                m_WinUid = IDC_WINDOW_CONTROL1;
                break;
            case IDC_WINDOW_CONTROL2:
                m_WinUid = IDC_WINDOW_CONTROL2;
                break;
            case IDC_WINDOW_CONTROL3:
                m_WinUid = IDC_WINDOW_CONTROL3;
                break;
            case IDC_WINDOW_CONTROL4:
                m_WinUid = IDC_WINDOW_CONTROL4;
                break;
            default:
                break;
        }

        CreateEx(
            0,
            className,
            cs,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            rect,
            parent,
            uid, NULL);
        m_OwnHandle = GetSafeHwnd();
    } catch(...) {
    }
}


/*************************************************************
* Method:       SaveRectLocally
* Description:  This function is used
* Parameter:    CRect rect :
* Parameter:    CString & classname :
* Returns:      void
*************************************************************/
void COpenGLControl::SaveRectLocally(CRect rect, CString &classname)
{
    if((0 == m_HeightInPixel) || (0 == m_WidthInPixel)) {
        m_HeightInPixel = 512;//rect.Height();
        m_WidthInPixel = 512;//rect.Width();
    }

    m_WinHeight = rect.Height();
    m_WinWidth = rect.Width();
    m_TopLeftMpt = rect.TopLeft();
    m_ClassName = classname;
}


/*************************************************************
* Method:       RenderTexture
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::RenderTexture()
{
    unsigned int texID = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 4,
                 256,
                 256,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 ImageBuffer) ;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL) ;
    glEnable(GL_TEXTURE_2D) ;
    glShadeModel(GL_FLAT) ;
    ImageBuffer = NULL;
}

/*************************************************************
* Method:       ZoomUp
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::ZoomUp()
{
    try {
        ZoomFlag = true; //MPower 069/AN/Zoom from Center

        if(zoomFactor < MAX_ZOOM) {
            zoomFactor += ZoomPercent;
        }

        UpdateRasterPoint();
        ZoomFromCenter();//MPower 069/AN
        PanFlag = false; //Mpower 069/AN/Zoom From center
        SendMessage(WM_PAINT);
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       ZoomDown
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::ZoomDown()
{
    try {
        ZoomFlag = true;

        if(zoomFactor <= ZoomPercent) {
        } else {
            zoomFactor -= ZoomPercent;
        }

        ZoomFromCenter();
        PanFlag = false;
        UpdateRasterPoint();
        SendMessage(WM_PAINT);
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       ZoomReset
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::ZoomReset()
{
    zoomFactor = 1.0f;
    SendMessage(WM_PAINT);
}

/*************************************************************
* Method:       SetZoomFactor
* Description:  This function is used
* Parameter:    int zPercent :
* Returns:      void
*************************************************************/
void COpenGLControl::SetZoomFactor(int zPercent)
{
    zoomFactor = (float)zPercent / 100;
    ZoomFlag = true;
    ZoomFromCenter();
    PanFlag = false;
    UpdateRasterPoint();
    SendMessage(WM_PAINT);
}

/*************************************************************
* Method:       getZoomFactor
* Description:  This function is used
* Returns:      int
*************************************************************/
int COpenGLControl::getZoomFactor()
{
    return((int)(zoomFactor * 100));
}
/*************************************************************
* Method:       SavePointOfInterest
* Description:  This function is used
* Parameter:    CPoint & cp :
* Returns:      void
*************************************************************/
void COpenGLControl::SavePointOfInterest(CPoint &cp)
{
    m_PtOfInterest.x = cp.x;
    m_PtOfInterest.y = cp.y;
}
/*************************************************************
* Method:       ResetPointOfInterest
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::ResetPointOfInterest()
{
    m_PtOfInterest.x = m_WidthInPixel / 2;
    m_PtOfInterest.y = m_HeightInPixel / 2;
    SendMessage(WM_PAINT);
}

/*************************************************************
* Method:       OnLButtonDblClk
* Description:  This function is used
* Parameter:    UINT nFlags :
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SavePointOfInterest(point);
    CWnd::OnLButtonDblClk(nFlags, point);
}



/*************************************************************
* Method:       OnLButtonDown
* Description:  This function is used
* Parameter:    UINT nFlags :
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::OnLButtonDown(UINT nFlags, CPoint point)
{
    UNREFERENCED_PARAMETER(nFlags);

    
        CdntMappingToImgDim(point);
    

    // TODO: Add your message handler code here and/or call default
    SendMessage(WM_PAINT);
    CFormImageViewer* tmp = NULL;
    tmp = (CFormImageViewer*)GetParent();

    if(tmp) {
        tmp->SetImagerUid(m_WinUid);
        tmp->SetChildHandles(m_OwnHandle);
        m_InsideImager = TRUE;
    } else {
        m_InsideImager = FALSE;
    }

    CWnd::OnLButtonDown(nFlags, point);
}

/*************************************************************
* Method:       OnMouseMove
* Description:  This function is used
* Parameter:    UINT nFlags :
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
//     if(m_InsideImager == TRUE){
    if(nFlags == MK_RBUTTON) {
        Pan(point);
    } else if(nFlags != MK_RBUTTON) {
        CdntMappingToImgDim(point);
    }


    SendMessage(WM_PAINT);
//     }
    CWnd::OnMouseMove(nFlags, point);
}

/*************************************************************
* Method:       OnRButtonDown
* Description:  This function is used
* Parameter:    UINT nFlags :
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
//     if(m_InsideImager == TRUE){
    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
    m_OldPoint.x = point.x;
    m_OldPoint.y = point.y;
    SendMessage(WM_PAINT);
//     }
    CWnd::OnRButtonDown(nFlags, point);
}

/*************************************************************
* Method:       OnRButtonUp
* Description:  This function is used
* Parameter:    UINT nFlags :
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    //if(m_InsideImager == TRUE){
    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
    m_OldPoint.x = point.x;
    m_OldPoint.y = point.y;
    SendMessage(WM_PAINT);
    // }
    CWnd::OnRButtonUp(nFlags, point);
}

/*************************************************************
* Method:       PanReset
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::PanReset()
{
    try {
        PanFlag = 0;
        int zmx = 0, zmy = 0;
        float zoom = 0;
        zmy = (int)ceil((double)m_WinHeight / (double)m_HeightInPixel);
        zmx = (int)ceil((double)m_WinWidth / (double)m_WidthInPixel);
        zoom = float(zmy > zmx ? zmy : zmx);

        if(zoom > MAX_ZOOM) {
            zoom = MAX_ZOOM;
        }

        zoomFactor = zoom;
        PanFlag = false;
        transX = 0;
        transY = 0;
        ZoomFromCenter();
        rasterX = 0.0f;
        rasterY = 0.0f;
        Invalidate();
    } catch(...) {
    }
}

/*************************************************************
* Method:       setImageBufferPointer
* Description:  This function is used
* Parameter:    void * ptr :
* Returns:      void
*************************************************************/
void COpenGLControl::setImageBufferPointer(void *ptr)
{
    try {
        ImageBuffer = (void*)ptr;

        if(NULL == ImageBuffer) {
            m_HeightInPixel = 256;
            m_WidthInPixel = 256;
        }
    } catch(...) {
    }
}
/*************************************************************
* Method:       SetDimensionInPixel
* Description:  This function is used
* Parameter:    int f_HeightInPixel :
* Parameter:    int f_WidthInPixel :
* Returns:      void
*************************************************************/
void COpenGLControl::SetDimensionInPixel(int f_HeightInPixel, int f_WidthInPixel)
{
    try {
        if(f_HeightInPixel <= 0 || f_WidthInPixel <= 0) {
            f_HeightInPixel = 256;
            f_WidthInPixel = 256;
        }

        m_HeightInPixel = f_HeightInPixel;
        m_WidthInPixel = f_WidthInPixel;
    } catch(...) {
    }
}

/*************************************************************
* Method:       SetT1MinMaxForScale
* Description:  This function is used
* Parameter:    double f_T1Min :
* Parameter:    double f_T1Max :
* Returns:      void
*************************************************************/
void COpenGLControl::SetT1MinMaxForScale(double f_T1Min, double f_T1Max)
{
    if(f_T1Min < (double)INT_MIN) {
        m_T1Min = (double)INT_MIN;
    } else {
        m_T1Min = f_T1Min;
    }

    if(f_T1Max > (double)INT_MAX) {
        m_T1Max = (double)INT_MAX;
    } else {
        m_T1Max = f_T1Max;
    }
}
/*************************************************************
* Method:       UpdateRasterPoint
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::UpdateRasterPoint()
{
    if(zoomFactor >=  1.0) {
        rasterX = 0.0f;
        rasterY = 0.0f;
    } else {
        rasterX = (m_WinWidth) * ((1 - zoomFactor) * 0.5f);
        rasterY = (m_WinHeight) * ((1 - zoomFactor) * 0.5f);
    }
}
//Jun-2013/MPower 69/AN/Zoom from Center

/*************************************************************
* Method:       CdntMappingToImgDim
* Description:  This function is used
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::CdntMappingToImgDim(CPoint point)
{
    try {
        int x = 0, y = 0;
        x = point.x - transX;
        {
            int final_y_trans = 0;
            final_y_trans =  m_centerZoom_offset_y  - transY ;
            y = point.y - 2 * final_y_trans ;
            int final_x_trans = 0;
            final_x_trans = m_x_centerZoom_offset + transX;
            x = point.x - final_x_trans;
        }
        y /= (int)zoomFactor;
        x /= (int)zoomFactor;
        
        if(y >= 0 && y < m_HeightInPixel && x >= 0 && x < m_WidthInPixel) {
            m_CoordinateX = x;
            m_CoordinateY = y;
        }
    } catch(...) {
    }
}

/*************************************************************
* Method:       Pan
* Description:  This function is used
* Parameter:    CPoint point :
* Returns:      void
*************************************************************/
void COpenGLControl::Pan(CPoint point)
{
    top_aligned = false;
    ZoomFlag = false;
    PanFlag = true;//used for coordinate mapping, do not reset on r button up
    int diffX = (point.x - m_OldPoint.x);  //nx- ox //AN/Added
    int diffY = (point.y - m_OldPoint.y);
    transX += (diffX);
    transY -= (diffY);
    m_OldPoint.x = point.x; //AN/Added
    m_OldPoint.y = point.y;  //AN/Added
    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
}

/*************************************************************
* Method:       ZoomFromCenter
* Description:  This function is used
* Returns:      void
*************************************************************/
void COpenGLControl::ZoomFromCenter()
{
    try {
        ZoomFlag = true;
        int icx = (int)(m_WidthInPixel * zoomFactor / 2);
        int icy = (int)(m_HeightInPixel * zoomFactor / 2);
        int wcx = m_WinWidth / 2;
        int wcy = m_WinHeight / 2;
        UNREFERENCED_PARAMETER(wcy);
        m_x_centerZoom_offset = -icx + wcx;
        m_centerZoom_offset_y = (int)(m_WinHeight - icy * 2 + 1) / 4         ;
    } catch(...) {
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////

BOOL COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    UNREFERENCED_PARAMETER(pt);

    if(m_InsideImager == FALSE) {
        return FALSE;
    }

    if(zDelta < 0) {
        ZoomDown();
    } else if(zDelta > 0) {
        ZoomUp();
    }

    switch(nFlags) {
        case MK_CONTROL    :
            break;
        case MK_LBUTTON    :
            break;
        case MK_MBUTTON :
            break;
        case MK_RBUTTON    :
            break;
        case MK_SHIFT    :
            break;
        default:
            break;
    }

    return TRUE;
}

BOOL COpenGLControl::IsPointInsideImager()
{
    return m_InsideImager;
}

int COpenGLControl::GetWinUid()
{
    return m_WinUid;
}

void COpenGLControl::ResetPointInsideImager()
{
    m_InsideImager = FALSE;
}

HWND COpenGLControl::GetOwnHandle()
{
    return(m_OwnHandle);
}
void COpenGLControl::ColorScaleDrawString( int x, int y, char *string )
{
    glRasterPos2d(x, y);
    glPushAttrib(GL_LIST_BIT);
    glListBase(1000);
    glCallLists(strlen(string), GL_UNSIGNED_BYTE, (GLubyte *)string);
    glPopAttrib();
}
