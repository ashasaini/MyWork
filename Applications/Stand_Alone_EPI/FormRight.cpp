// FormRight.cpp : implementation file
//

#include "stdafx.h"
#include "EPI.h"
#include "FormRight.h"
#include "ImageDataProcessor.h"
#include <IO.H>
#include <FCNTL.H>
#include "FileReader.h"
#include "CommonDefinations.h"
#include "EPILogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormImageViewer

IMPLEMENT_DYNCREATE(CFormImageViewer, CFormView)


CFormImageViewer::CFormImageViewer()
    : CFormView(CFormImageViewer::IDD), m_ImgBuf1(NULL), m_ImgBuf2(NULL), m_ImgBuf3(NULL), m_ImgBuf4(NULL), m_Child_uid(0), m_ChildHandle(NULL),
      m_OpenGLControl1(NULL), m_OpenGLControl2(NULL), m_OpenGLControl3(NULL), m_OpenGLControl4(NULL),
      m_ulColoredImg1(NULL), m_ulColoredImg2(NULL), m_ulColoredImg3(NULL), m_ulColoredImg4(NULL),
      m_WinLevel(50), m_WinWidth(100), m_ImagerCurrentMode(MODE1X1)
{
    //{{AFX_DATA_INIT(CFormImageViewer)
//  m_csDisplayString = _T("");
    //}}AFX_DATA_INIT
    m_Background = RGB(33, 33, 33);
}

CFormImageViewer::~CFormImageViewer()
{
    DeleteAllImagers();
    DeleteAllImages();
}

void CFormImageViewer::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFormImageViewer)
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormImageViewer, CFormView)
    //{{AFX_MSG_MAP(CFormImageViewer)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_MESSAGE(WM_CMD_WWWL, AdjustnConvertWWWLimage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormImageViewer diagnostics

#ifdef _DEBUG
void CFormImageViewer::AssertValid() const
{
    CFormView::AssertValid();
}

void CFormImageViewer::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormImageViewer message handlers

void CFormImageViewer::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    // TODO: Add your specialized code here and/or call the base class
    CString ClassName(_T("CONTROL1"));
    m_OpenGLControl1 = CreateImager(IDC_WINDOW_CONTROL1, ClassName, NULL, 0, 0, FALSE);
    m_Child_uid = IDC_WINDOW_CONTROL1;
    m_ChildHandle = m_OpenGLControl1->GetOwnHandle();
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();
}



void CFormImageViewer::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here
}
BOOL CFormImageViewer::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    BackgroundColor(pDC->m_hDC);
    return TRUE;
    // return CFormView::OnEraseBkgnd(pDC);
}

void CFormImageViewer::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    CPen* l_poldpen = NULL;
    HWND        hWndChild = NULL;
    RECT        rc;
    HWND l_hWndParent  = GetSafeHwnd();
    hWndChild = ::GetWindow(l_hWndParent, GW_CHILD);

    while(hWndChild != NULL) {
        if(hWndChild == m_ChildHandle) {
            CPen frame_rect_pen(PS_SOLID, 3 , RGB(204, 255, 204));
            l_poldpen = dc.SelectObject(&frame_rect_pen);
            DrawBracketsImager(hWndChild, rc, dc);
        } else {
            if(m_ImagerCurrentMode != MODE1S1) {
                CPen frame_rect_pen(PS_SOLID, 3 , RGB(31, 31, 31));
                l_poldpen = dc.SelectObject(&frame_rect_pen);
                DrawBracketsImager(hWndChild, rc, dc);
            }
        }

        hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
    };

    dc.SelectObject(l_poldpen);

    CFormView::OnPaint();
}
void CFormImageViewer::BackgroundColor(HDC hDC)
{
    HRGN        hRgn1 = NULL;
    HRGN        hRgn2 = NULL;
    HRGN        hRgn3 = NULL;
    RECT        rc;
    HBRUSH      hBrush = NULL;
    HWND        hWndChild = NULL;
    BOOL        bVisible = false;
    _ASSERTE(hDC != NULL);

    if(hDC == NULL) {
        return;
    }

    // create a brush to fill the background with
    hBrush = CreateSolidBrush(m_Background);
    // get the coordinates of the parent-window
    // and create a region-object for the whole
    // area of the window
    HWND l_hWndParent  = GetSafeHwnd();
    ::GetClientRect(l_hWndParent, &rc);
    hRgn1 = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
    hRgn2 = CreateRectRgn(0, 0, 0, 0);
    hRgn3 = CreateRectRgn(0, 0, 0, 0);
    hWndChild = ::GetWindow(l_hWndParent, GW_CHILD);

    while(hWndChild != NULL) {
        bVisible = ::IsWindowVisible(hWndChild);

        if((bVisible)) {
            ::GetWindowRect(hWndChild, &rc);
            ScrnToClt(l_hWndParent, &rc);
            ::SetRectRgn(hRgn2, rc.left, rc.top, rc.right, rc.bottom);
            ::CombineRgn(hRgn3, hRgn1, hRgn2, RGN_DIFF);
            HRGN hRgnTemp = hRgn1;
            hRgn1 = hRgn3;
            hRgn3 = hRgnTemp;
        };

        hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
    };

    ::FillRgn(hDC, hRgn1, hBrush);

    DeleteObject(hRgn3);

    DeleteObject(hRgn2);

    DeleteObject(hRgn1);

    DeleteObject(hBrush);
}
BOOL CFormImageViewer::ScrnToClt(HWND hWnd, RECT *pRect)
{
    POINT   pt1;
    POINT   pt2;
    pt1.x = pRect->left;
    pt1.y = pRect->top;
    pt2.x = pRect->right;
    pt2.y = pRect->bottom;

    if(::ScreenToClient(hWnd, &pt1) == FALSE) {
        return(FALSE);
    }

    if(::ScreenToClient(hWnd, &pt2) == FALSE) {
        return(FALSE);
    }

    pRect->left = pt1.x;
    pRect->top = pt1.y;
    pRect->right = pt2.x;
    pRect->bottom = pt2.y;
    return(TRUE);
};
void CFormImageViewer::DrawLine(CPaintDC* f_dc , CPoint &f_start_point, CPoint &f_end_point)
{
    POINT lp ;
    ::MoveToEx(f_dc->m_hDC, f_start_point.x , f_start_point.y , &lp);
    ::LineTo(f_dc->m_hDC, f_end_point.x , f_end_point.y);
}

BOOL CFormImageViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    BOOL retval = FALSE;

    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1:

            if(m_OpenGLControl2) {
                m_OpenGLControl2->ResetPointInsideImager();
            }

            if(m_OpenGLControl3) {
                m_OpenGLControl3->ResetPointInsideImager();
            }

            if(m_OpenGLControl4) {
                m_OpenGLControl4->ResetPointInsideImager();
            }

            if(m_OpenGLControl1->OnMouseWheel(nFlags,  zDelta,  pt)) {
                retval = TRUE;
            }

            break;
        case IDC_WINDOW_CONTROL2:

            if(m_OpenGLControl1) {
                m_OpenGLControl1->ResetPointInsideImager();
            }

            if(m_OpenGLControl3) {
                m_OpenGLControl3->ResetPointInsideImager();
            }

            if(m_OpenGLControl4) {
                m_OpenGLControl4->ResetPointInsideImager();
            }

            if(m_OpenGLControl2->OnMouseWheel(nFlags,  zDelta,  pt)) {
                retval = TRUE;
            }

            break;
        case IDC_WINDOW_CONTROL3:

            if(m_OpenGLControl1) {
                m_OpenGLControl1->ResetPointInsideImager();
            }

            if(m_OpenGLControl2) {
                m_OpenGLControl2->ResetPointInsideImager();
            }

            if(m_OpenGLControl4) {
                m_OpenGLControl4->ResetPointInsideImager();
            }

            if(m_OpenGLControl3->OnMouseWheel(nFlags,  zDelta,  pt)) {
                retval = TRUE;
            }

            break;
        case IDC_WINDOW_CONTROL4:

            if(m_OpenGLControl1) {
                m_OpenGLControl1->ResetPointInsideImager();
            }

            if(m_OpenGLControl2) {
                m_OpenGLControl2->ResetPointInsideImager();
            }

            if(m_OpenGLControl3) {
                m_OpenGLControl3->ResetPointInsideImager();
            }

            if(m_OpenGLControl4->OnMouseWheel(nFlags,  zDelta,  pt)) {
                retval = TRUE;
            }

            break;
        default:
            retval = FALSE;
            break;
    }

    if(retval == TRUE) {
        return TRUE;
    }

    return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CFormImageViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CFormView::OnLButtonDown(nFlags, point);
}

void CFormImageViewer::SetImagerUid(int uid)
{
    m_Child_uid = uid;
    Invalidate(TRUE);
}

void CFormImageViewer::SetChildHandles(HWND hndle)
{
    m_ChildHandle = hndle;
    Invalidate(TRUE);
}

void CFormImageViewer::DrawBracketsImager(HWND hWndChild, RECT &rc , CPaintDC &dc)
{
    HWND l_hWndParent = GetSafeHwnd();
    ::GetWindowRect(hWndChild, &rc);
    ScrnToClt(l_hWndParent, &rc);
    CPoint pt1(rc.left - 5, rc.top - 5);
    CPoint pt2(rc.left - 5, rc.top - 5 + 50);
    DrawLine(&dc, pt1, pt2);
    CPoint pt3(rc.left - 5, rc.top - 5);
    CPoint pt4(rc.left - 5 + 50, rc.top - 5);
    DrawLine(&dc, pt3, pt4);
    CPoint pt5(rc.right + 5 - 50, rc.top - 5);
    CPoint pt6(rc.right + 5, rc.top - 5);
    DrawLine(&dc, pt5, pt6);
    CPoint pt7(rc.right + 5, rc.top - 5);
    CPoint pt8(rc.right + 5, rc.top - 5 + 50);
    DrawLine(&dc, pt7, pt8);
    CPoint pt9(rc.right + 5, rc.bottom + 5);
    CPoint pt10(rc.right + 5, rc.bottom + 5 - 50);
    DrawLine(&dc, pt9, pt10);
    CPoint pt11(rc.right + 5, rc.bottom + 5);
    CPoint pt12(rc.right + 5 - 50, rc.bottom + 5);
    DrawLine(&dc, pt11, pt12);
    CPoint pt13(rc.left - 5, rc.bottom + 5 - 50);
    CPoint pt14(rc.left - 5, rc.bottom + 5);
    DrawLine(&dc, pt13, pt14);
    CPoint pt15(rc.left - 5, rc.bottom + 5);
    CPoint pt16(rc.left - 5 + 50, rc.bottom + 5);
    DrawLine(&dc, pt15, pt16);
}


void CFormImageViewer::OnBUTTON1x1()
{
    // TODO: Add your command handler code here
    DeleteAllImagers();
    InvalidateEverything();
    CString ClassName(_T("CONTROL1"));
    m_OpenGLControl1 = CreateImager(IDC_WINDOW_CONTROL1, ClassName, m_ulColoredImg1, m_nImgHeight[0], m_nImgWidth[0], FALSE);
    m_ImagerCurrentMode = MODE1X1;
    m_ChildHandle = m_OpenGLControl1->GetOwnHandle();
    m_Child_uid = IDC_WINDOW_CONTROL1;
}

void CFormImageViewer::OnBUTTON1x2()
{
    // TODO: Add your command handler code here
    DeleteAllImagers();
    InvalidateEverything();
    CString ClassName(_T("CONTROL1"));
    m_OpenGLControl1 = CreateImager(IDC_WINDOW_CONTROL1, ClassName, m_ulColoredImg1, m_nImgHeight[0], m_nImgWidth[0], FALSE);
    m_ChildHandle = m_OpenGLControl1->GetOwnHandle();
    m_Child_uid = IDC_WINDOW_CONTROL1;
    ClassName = (_T("CONTROL2"));
    m_OpenGLControl2 = CreateImager(IDC_WINDOW_CONTROL2, ClassName, m_ulColoredImg2, m_nImgHeight[1], m_nImgWidth[1], FALSE);
    m_ImagerCurrentMode = MODE1X2;
}

void CFormImageViewer::OnBUTTON2x2()
{
    // TODO: Add your command handler code here
    DeleteAllImagers();
    InvalidateEverything();
    CString ClassName(_T("CONTROL1"));
    m_OpenGLControl1 = CreateImager(IDC_WINDOW_CONTROL1, ClassName, m_ulColoredImg1, m_nImgHeight[0], m_nImgWidth[0], FALSE);
    m_ChildHandle = m_OpenGLControl1->GetOwnHandle();
    m_Child_uid = IDC_WINDOW_CONTROL1;
    ClassName = (_T("CONTROL2"));
    m_OpenGLControl2 = CreateImager(IDC_WINDOW_CONTROL2, ClassName, m_ulColoredImg2, m_nImgHeight[1], m_nImgWidth[1], FALSE);
    ClassName = (_T("CONTROL3"));
    m_OpenGLControl3 = CreateImager(IDC_WINDOW_CONTROL3, ClassName, m_ulColoredImg3, m_nImgHeight[2], m_nImgWidth[2], FALSE);
    ClassName = (_T("CONTROL4"));
    m_OpenGLControl4 = CreateImager(IDC_WINDOW_CONTROL4, ClassName, m_ulColoredImg4, m_nImgHeight[3], m_nImgWidth[3], FALSE);
    m_ImagerCurrentMode = MODE2X2;
}

void CFormImageViewer::OnButtonResetimage()
{
    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1:

            if(m_OpenGLControl1) {
                m_OpenGLControl1->ZoomReset();
                m_OpenGLControl1->PanReset();
            }

            break;
        case IDC_WINDOW_CONTROL2:

            if(m_OpenGLControl2) {
                m_OpenGLControl2->ZoomReset();
                m_OpenGLControl2->PanReset();
            }

            break;
        case IDC_WINDOW_CONTROL3:

            if(m_OpenGLControl3) {
                m_OpenGLControl3->ZoomReset();
                m_OpenGLControl3->PanReset();
            }

            break;
        case IDC_WINDOW_CONTROL4:

            if(m_OpenGLControl4) {
                m_OpenGLControl4->ZoomReset();
                m_OpenGLControl4->PanReset();
            }

            break;
        default:
            break;
    }
}

void CFormImageViewer::OnButtonFitimage()
{
    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1: {
            DeleteAllImagers();
            GetParentFrame()->RecalcLayout();
            ResizeParentToFit();
            InvalidateEverything();
            CString ClassName = (_T("CONTROL1"));
            m_OpenGLControl1 = CreateImager(IDC_WINDOW_CONTROL1, ClassName, m_ulColoredImg1, m_nImgHeight[0], m_nImgWidth[0], TRUE);
            m_Child_uid = IDC_WINDOW_CONTROL1;
            m_ChildHandle = m_OpenGLControl1->GetOwnHandle();
        }
        break;
        case IDC_WINDOW_CONTROL2: {
            DeleteAllImagers();
            GetParentFrame()->RecalcLayout();
            ResizeParentToFit();
            InvalidateEverything();
            CString ClassName = (_T("CONTROL2"));
            m_OpenGLControl2 = CreateImager(IDC_WINDOW_CONTROL2, ClassName, m_ulColoredImg2, m_nImgHeight[1], m_nImgWidth[1], TRUE);
            m_Child_uid = IDC_WINDOW_CONTROL2;
            m_ChildHandle = m_OpenGLControl2->GetOwnHandle();
        }
        break;
        case IDC_WINDOW_CONTROL3: {
            DeleteAllImagers();
            GetParentFrame()->RecalcLayout();
            ResizeParentToFit();
            InvalidateEverything();
            CString ClassName = (_T("CONTROL3"));
            m_OpenGLControl3 = CreateImager(IDC_WINDOW_CONTROL3, ClassName, m_ulColoredImg3, m_nImgHeight[2], m_nImgWidth[2], TRUE);
            m_Child_uid = IDC_WINDOW_CONTROL3;
            m_ChildHandle = m_OpenGLControl3->GetOwnHandle();
        }
        break;
        case IDC_WINDOW_CONTROL4: {
            DeleteAllImagers();
            GetParentFrame()->RecalcLayout();
            ResizeParentToFit();
            InvalidateEverything();
            CString ClassName = (_T("CONTROL4"));
            m_OpenGLControl4 = CreateImager(IDC_WINDOW_CONTROL4, ClassName, m_ulColoredImg4, m_nImgHeight[3], m_nImgWidth[3], TRUE);
            m_Child_uid = IDC_WINDOW_CONTROL4;
            m_ChildHandle = m_OpenGLControl4->GetOwnHandle();
        }
        break;
        default:
            break;
    }
}

void CFormImageViewer::DeleteAllImagers()
{
    if(m_OpenGLControl1) {
        delete m_OpenGLControl1;
        m_OpenGLControl1 = NULL;
    }

    if(m_OpenGLControl2) {
        delete m_OpenGLControl2;
        m_OpenGLControl2 = NULL;
    }

    if(m_OpenGLControl3) {
        delete m_OpenGLControl3;
        m_OpenGLControl3 = NULL;
    }

    if(m_OpenGLControl4) {
        delete m_OpenGLControl4;
        m_OpenGLControl4 = NULL;
    }

    m_ChildHandle = NULL;
    m_Child_uid = NULL;
}

COpenGLControl* CFormImageViewer::CreateImager(int WinUid, CString &className, void*Buffer, int Ht, int Wt , BOOL stretched)
{
    COpenGLControl *l_OpenGlCtrl = NULL;

    try {
        int baseUid = 0;

        switch(WinUid) {
            case IDC_WINDOW_CONTROL1:
                baseUid = IDC_STATIC_CONTROL1;
                l_OpenGlCtrl = m_OpenGLControl1;
                break;
            case IDC_WINDOW_CONTROL2:
                baseUid = IDC_STATIC_CONTROL2;
                l_OpenGlCtrl = m_OpenGLControl2;
                break;
            case IDC_WINDOW_CONTROL3:
                baseUid = IDC_STATIC_CONTROL3;
                l_OpenGlCtrl = m_OpenGLControl3;
                break;
            case IDC_WINDOW_CONTROL4:
                baseUid = IDC_STATIC_CONTROL4;
                l_OpenGlCtrl = m_OpenGLControl4;
                break;
            default:
                baseUid = IDC_STATIC_CONTROL1;
                l_OpenGlCtrl = m_OpenGLControl1;
                break;
        }

        CRect rect;

        if(TRUE == stretched) {
            GetWindowRect(&rect);
            ScreenToClient(&rect);
            m_ImagerCurrentMode = MODE1S1;
        } else {
            GetDlgItem(baseUid)->GetWindowRect(&rect);
            ScreenToClient(&rect);
        }

        l_OpenGlCtrl = new COpenGLControl();
        l_OpenGlCtrl->SetDimensionInPixel(Ht, Wt);
        l_OpenGlCtrl->setImageBufferPointer(Buffer);
        l_OpenGlCtrl->Create(rect, this, className, WinUid);
    } catch(...) {
        CString cs(_T("Exception WHile creation Imager"));
//        AfxMessageBox(cs);
    }

    return (l_OpenGlCtrl);
}

LRESULT CFormImageViewer::AdjustnConvertWWWLimage(WPARAM wparam, LPARAM lparam)
{
    double* tmpImage = NULL;
    void * tmpColor = NULL;
    int l_nImgWidth = 0;
    int l_nImgHeight = 0;

    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1:
            m_OpenGLControl1->setImageBufferPointer(NULL);

            if(m_ulColoredImg1) {
                memset(m_ulColoredImg1, 0x00, m_nImgWidth[0]*m_nImgHeight[0] * 4 * sizeof(char));
                tmpColor = m_ulColoredImg1;
                l_nImgWidth = m_nImgWidth[0];
                l_nImgHeight = m_nImgHeight[0];
                tmpImage = m_ImgBuf1;
            }

            break;
        case IDC_WINDOW_CONTROL2:
            m_OpenGLControl2->setImageBufferPointer(NULL);

            if(m_ulColoredImg2) {
                memset(m_ulColoredImg2, 0x00, m_nImgWidth[1]*m_nImgHeight[1] * 4 * sizeof(char));
                tmpColor = m_ulColoredImg2;
                l_nImgWidth = m_nImgWidth[1];
                l_nImgHeight = m_nImgHeight[1];
                tmpImage = m_ImgBuf2;
            }

            break;
        case IDC_WINDOW_CONTROL3:
            m_OpenGLControl3->setImageBufferPointer(NULL);

            if(m_ulColoredImg3) {
                memset(m_ulColoredImg3, 0x00, m_nImgWidth[2]*m_nImgHeight[2] * 4 * sizeof(char));
                tmpColor = m_ulColoredImg3;
                l_nImgWidth = m_nImgWidth[2];
                l_nImgHeight = m_nImgHeight[2];
                tmpImage = m_ImgBuf3;
            }

            break;
        case IDC_WINDOW_CONTROL4:
            m_OpenGLControl4->setImageBufferPointer(NULL);

            if(m_ulColoredImg4) {
                memset(m_ulColoredImg4, 0x00, m_nImgWidth[3]*m_nImgHeight[3] * 4 * sizeof(char));
                tmpColor = m_ulColoredImg4;
                l_nImgWidth = m_nImgWidth[3];
                l_nImgHeight = m_nImgHeight[3];
                tmpImage = m_ImgBuf4;
            }

            break;
        default:
            return 0;
    }

    m_WinWidth = (int)wparam;
    m_WinLevel = (int)lparam;

    if(NULL == tmpColor) {
        return 0;
    }

    CImageDataProcessor l_cimgproc;
    l_cimgproc.ConvertTo4_D(tmpImage, tmpColor, l_nImgWidth * l_nImgHeight, m_WinWidth, m_WinLevel);

    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1:
            m_OpenGLControl1->SetDimensionInPixel(l_nImgHeight, l_nImgWidth);
            m_OpenGLControl1->setImageBufferPointer(tmpColor);
            break;
        case IDC_WINDOW_CONTROL2:
            m_OpenGLControl2->SetDimensionInPixel(l_nImgHeight, l_nImgWidth);
            m_OpenGLControl2->setImageBufferPointer(tmpColor);
            break;
        case IDC_WINDOW_CONTROL3:
            m_OpenGLControl3->SetDimensionInPixel(l_nImgHeight, l_nImgWidth);
            m_OpenGLControl3->setImageBufferPointer(tmpColor);
            break;
        case IDC_WINDOW_CONTROL4:
            m_OpenGLControl4->SetDimensionInPixel(l_nImgHeight, l_nImgWidth);
            m_OpenGLControl4->setImageBufferPointer(tmpColor);
            break;
        default:
            break;
    }

    InvalidateEverything();
    return 0;
}

void CFormImageViewer::DeleteAllImages()
{
    if(m_ulColoredImg1) {
        free(m_ulColoredImg1);
        m_ulColoredImg1 = NULL;
    }

    if(m_ulColoredImg2) {
        free(m_ulColoredImg2);
        m_ulColoredImg2 = NULL;
    }

    if(m_ulColoredImg3) {
        free(m_ulColoredImg3);
        m_ulColoredImg3 = NULL;
    }

    if(m_ulColoredImg4) {
        free(m_ulColoredImg4);
        m_ulColoredImg4 = NULL;
    }

    if(m_ImgBuf1) {
		delete [] m_ImgBuf1;
        m_ImgBuf1 = NULL;
    }

    if(m_ImgBuf2) {
		delete [] m_ImgBuf2;
        m_ImgBuf2 = NULL;
    }

    if(m_ImgBuf3) {
		delete [] m_ImgBuf3;
        m_ImgBuf3 = NULL;
    }

    if(m_ImgBuf4) {
		delete [] m_ImgBuf4;
        m_ImgBuf4 = NULL;
    }
}

void CFormImageViewer::InvalidateEverything()
{
    if(m_OpenGLControl1 && m_OpenGLControl1->GetSafeHwnd()) {
        m_OpenGLControl1->Invalidate(TRUE);
    }

    if(m_OpenGLControl2 && m_OpenGLControl2->GetSafeHwnd()) {
        m_OpenGLControl2->Invalidate(TRUE);
    }

    if(m_OpenGLControl3 && m_OpenGLControl3->GetSafeHwnd()) {
        m_OpenGLControl3->Invalidate(TRUE);
    }

    if(m_OpenGLControl4 && m_OpenGLControl4->GetSafeHwnd()) {
        m_OpenGLControl4->Invalidate(TRUE);
    }

    Invalidate();
}

int CFormImageViewer::displayImageStudyRaw(const int &f_nWidth, const int &f_nHeight, double *&f_dIamgeData)
{
    double* tmpImage = NULL;
    void* tmpColor = NULL;

    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1:
            //=================================================================
            m_nImgHeight[0] = f_nHeight;
            m_nImgWidth[0] = f_nWidth;

            //=================================================================
            if(m_ImgBuf1) {
                delete [] m_ImgBuf1;
                m_ImgBuf1 = NULL;
            }

            m_ImgBuf1 = new double[f_nWidth*f_nHeight];

            if(m_ImgBuf1) {
                memcpy(m_ImgBuf1 , f_dIamgeData, sizeof(double)*f_nWidth * f_nHeight);
                tmpImage = m_ImgBuf1;
            } else {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ImgBuf1."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            //==================================================================
            if(m_ulColoredImg1) {
                free(m_ulColoredImg1);
                m_ulColoredImg1 = NULL;
            }

            m_ulColoredImg1 = malloc(f_nWidth * f_nHeight * 4 * sizeof(char));

            if(m_ulColoredImg1) {
                memset(m_ulColoredImg1, 0x00, f_nWidth * f_nHeight * 4 * sizeof(char));
                tmpColor = m_ulColoredImg1;
            } else {
                if(m_ImgBuf1) {
                    delete [] m_ImgBuf1;
                    m_ImgBuf1 = NULL;
                    tmpImage = NULL;
                }

                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ulColoredImg1 ."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            break;
        case IDC_WINDOW_CONTROL2:
            m_nImgHeight[1] = f_nHeight;
            m_nImgWidth[1] = f_nWidth;

            //=================================================================
            if(m_ImgBuf2) {
                delete [] m_ImgBuf2;
                m_ImgBuf2 = NULL;
            }

            m_ImgBuf2 = new double[f_nWidth*f_nHeight];

            if(m_ImgBuf2) {
                memcpy(m_ImgBuf2 , f_dIamgeData, sizeof(double)*f_nWidth * f_nHeight);
                tmpImage = m_ImgBuf2;
            } else {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ImgBuf2."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            //==================================================================
            if(m_ulColoredImg2) {
                free(m_ulColoredImg2);
                m_ulColoredImg2 = NULL;
            }

            m_ulColoredImg2 = malloc(f_nWidth * f_nHeight * 4 * sizeof(char));

            if(m_ulColoredImg2) {
                memset(m_ulColoredImg2, 0x00, f_nWidth * f_nHeight * 4 * sizeof(char));
                tmpColor = m_ulColoredImg2;
            } else {
                if(m_ImgBuf2) {
                    delete [] m_ImgBuf2;
                    m_ImgBuf2 = NULL;
                    tmpImage = NULL;
                }

                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ulColoredImg2."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            break;
        case IDC_WINDOW_CONTROL3:
            m_nImgHeight[2] = f_nHeight;
            m_nImgWidth[2] = f_nWidth;

            //=================================================================
            if(m_ImgBuf3) {
                delete [] m_ImgBuf3;
                m_ImgBuf3 = NULL;
            }

            m_ImgBuf3 = new double[f_nWidth*f_nHeight];

            if(m_ImgBuf3) {
                memcpy(m_ImgBuf3 , f_dIamgeData, sizeof(double)*f_nWidth * f_nHeight);
                tmpImage = m_ImgBuf3;
            } else {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ImgBuf3 ."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            //==================================================================
            if(m_ulColoredImg3) {
                free(m_ulColoredImg3);
                m_ulColoredImg3 = NULL;
            }

            m_ulColoredImg3 = malloc(f_nWidth * f_nHeight * 4 * sizeof(char));

            if(m_ulColoredImg3) {
                memset(m_ulColoredImg3, 0x00, f_nWidth * f_nHeight * 4 * sizeof(char));
                tmpColor = m_ulColoredImg3;
            } else {
                if(m_ImgBuf3) {
                    delete [] m_ImgBuf3;
                    m_ImgBuf3 = NULL;
                    tmpImage = NULL;
                }

                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ulColoredImg3."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            break;
        case IDC_WINDOW_CONTROL4:
            m_nImgHeight[3] = f_nHeight;
            m_nImgWidth[3] = f_nWidth;

            //=================================================================
            if(m_ImgBuf4) {
                delete [] m_ImgBuf4;
                m_ImgBuf4 = NULL;
            }

            m_ImgBuf4 = new double[f_nWidth*f_nHeight];

            if(m_ImgBuf4) {
                memcpy(m_ImgBuf4 , f_dIamgeData, sizeof(double)*f_nWidth * f_nHeight);
                tmpImage = m_ImgBuf4;
            } else {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ImgBuf4."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            //==================================================================
            if(m_ulColoredImg4) {
                free(m_ulColoredImg4);
                m_ulColoredImg4 = NULL;
            }

            m_ulColoredImg4 = malloc(f_nWidth * f_nHeight * 4 * sizeof(char));

            if(m_ulColoredImg4) {
                memset(m_ulColoredImg4, 0x00, f_nWidth * f_nHeight * 4 * sizeof(char));
                tmpColor = m_ulColoredImg4;
            } else {
                if(m_ImgBuf4) {
                    delete [] m_ImgBuf4;
                    m_ImgBuf4 = NULL;
                    tmpImage = NULL;
                }

                CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for m_ulColoredImg4."), __LINE__, __FILE__);
                return EPI_MEM_ALLOC;
            }

            break;
        default:
            return EPI_DISPLAY_IMAGE;
    }

    CImageDataProcessor l_cimgproc;
    bool l_bStatus = l_cimgproc.ConvertTo4_D(tmpImage, tmpColor, f_nWidth * f_nHeight, m_WinWidth, m_WinLevel);

    if(true != l_bStatus) {
        tmpImage = NULL;
        tmpColor = NULL;
        return EPI_DISPLAY_IMAGE;
    }

    switch(m_Child_uid) {
        case IDC_WINDOW_CONTROL1:

            if(m_OpenGLControl1) {
                m_OpenGLControl1->SetDimensionInPixel(f_nHeight, f_nWidth);
                m_OpenGLControl1->setImageBufferPointer(tmpColor);
            }

            break;
        case IDC_WINDOW_CONTROL2:

            if(m_OpenGLControl2) {
                m_OpenGLControl2->SetDimensionInPixel(f_nHeight, f_nWidth);
                m_OpenGLControl2->setImageBufferPointer(tmpColor);
            }

            break;
        case IDC_WINDOW_CONTROL3:

            if(m_OpenGLControl3) {
                m_OpenGLControl3->SetDimensionInPixel(f_nHeight, f_nWidth);
                m_OpenGLControl3->setImageBufferPointer(tmpColor);
            }

            break;
        case IDC_WINDOW_CONTROL4:

            if(m_OpenGLControl4) {
                m_OpenGLControl4->SetDimensionInPixel(f_nHeight, f_nWidth);
                m_OpenGLControl4->setImageBufferPointer(tmpColor);
            }

            break;
        default:
            break;
    }

    OnButtonResetimage();
    InvalidateEverything();
    tmpImage = NULL;
    return 0;
}

