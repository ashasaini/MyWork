// ImageOffsetWnd.cpp : implementation file
//

#include "..\stdafx.h"
#include "ImageOffsetWnd.h"
#include "..\PQMImageUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DRAG_IMAGE_SIZE 11
#define MAXSIZE_CM 25.f

/////////////////////////////////////////////////////////////////////////////
// CImageOffsetWnd

//************************************Method Header**************************************
//Method Name:  CImageOffsetWnd
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
CImageOffsetWnd::CImageOffsetWnd(const float f_max_x, const float f_max_y) :
    m_image_offset_pos(0, 0, 0, 0),
    m_dragging_image_offset(false),
    m_drag_offset_point(0, 0),
    m_border_rect(0, 0, 0, 0),
    m_max_x_CM(f_max_x),
    m_max_y_CM(f_max_y)
{
}


//************************************Method Header**************************************
//Method Name:  ~CImageOffsetWnd
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
CImageOffsetWnd::~CImageOffsetWnd()
{
}


//************************************Method Header**************************************
//Method Name:  GetDragFactorX
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const float CImageOffsetWnd::GetDragFactorX(
) const
{

    const CRect l_center = GetCenterImageRect();
    return (m_image_offset_pos.left - l_center.left) / (float)(m_border_rect.Width() / 2);
}


//************************************Method Header**************************************
//Method Name:  GetDragFactorY
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const float CImageOffsetWnd::GetDragFactorY(
) const
{

    const CRect l_center = GetCenterImageRect();
    return ((GetCenterImageRect().top) - m_image_offset_pos.top) / (float)(m_border_rect.Height() / 2);
}


//************************************Method Header**************************************
//Method Name:  SetImagePos
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::SetImagePos(
    const float f_percentX,
    const float f_percentY
)
{

    const int l_temp = DRAG_IMAGE_SIZE / 2 ;

    CRect l_prev_rect = m_image_offset_pos;

    const int l_width = (int)(m_border_rect.Width() / 2.f);
    const int l_height = (int)(m_border_rect.Height() / 2.f);

    m_image_offset_pos.left = (int)(m_border_rect.left + l_width + (l_width * f_percentX) - l_temp);
    m_image_offset_pos.right = m_image_offset_pos.left + DRAG_IMAGE_SIZE;

    m_image_offset_pos.top = (int)(m_border_rect.top + l_height + (l_height * (-f_percentY)) - l_temp) ;
    m_image_offset_pos.bottom = m_image_offset_pos.top + DRAG_IMAGE_SIZE;

    l_prev_rect.UnionRect(l_prev_rect, m_image_offset_pos);
    l_prev_rect.InflateRect(2, 2);

    InvalidateRect(l_prev_rect, true);
}



BEGIN_MESSAGE_MAP(CImageOffsetWnd, CWnd)
    //{{AFX_MSG_MAP(CImageOffsetWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CImageOffsetWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CImageOffsetWnd message handlers


//************************************Method Header**************************************
//Method Name:  OnPaint
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here
    PaintStaticImages(&dc);

    PaintImageOffsetPos(&dc);

    // Do not call CWnd::OnPaint() for painting messages
}

//************************************Method Header**************************************
//Method Name:  UpdateBorderRect
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::UpdateBorderRect(
)
{
    const int l_frame_max_dim = 201;

    const int l_width = (int)(l_frame_max_dim * (m_max_x_CM / MAXSIZE_CM));
    const int l_height = (int)(l_frame_max_dim * (m_max_y_CM / MAXSIZE_CM));

    CRect l_rect;
    GetClientRect(&l_rect);
    CPoint l_center = l_rect.CenterPoint();

    m_border_rect.left  = l_center.x - l_width / 2;
    m_border_rect.right = m_border_rect.left + l_width;
    m_border_rect.top  = l_center.y - l_height / 2;
    m_border_rect.bottom = m_border_rect.top + l_height;

    if (m_border_rect.Width() % 2 == 0) {
        m_border_rect.right ++;
    }

    if (m_border_rect.Height() % 2 == 0) {
        m_border_rect.bottom ++;
    }
}


//************************************Method Header**************************************
//Method Name:  OnCreate
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
int CImageOffsetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here

    CRect l_rect;
    GetWindowRect(&l_rect);
    GetParent()->ScreenToClient(&l_rect);
    SetWindowPos(NULL, l_rect.left , l_rect.top, l_rect.Width(), l_rect.Height(), SWP_NOZORDER);

    UpdateBorderRect();

    m_image_offset_pos = GetCenterImageRect();
    m_dragging_image_offset = false;
    m_drag_offset_point = CPoint(0, 0);

    return 0;
}


//************************************Method Header**************************************
//Method Name:  OnLButtonDown
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (m_image_offset_pos.PtInRect(point)) {

        CPoint l_center_rect = m_image_offset_pos.CenterPoint(); //GetCenterImageRect();
        m_drag_offset_point = point - l_center_rect;


        m_dragging_image_offset = true;

        SetCapture();
    }

    CWnd::OnLButtonDown(nFlags, point);
}


//************************************Method Header**************************************
//Method Name:  OnMouseMove
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (nFlags& MK_LBUTTON == MK_LBUTTON) {

        if (m_dragging_image_offset) {
            DragImageOffset(point);
        }
    }

    CWnd::OnMouseMove(nFlags, point);
}


//************************************Method Header**************************************
//Method Name:  OnLButtonUp
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (m_dragging_image_offset) {
        m_dragging_image_offset = false;
        ReleaseCapture();
    }

    CWnd::OnLButtonUp(nFlags, point);
}


//************************************Method Header**************************************
//Method Name:  DragImageOffset
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::DragImageOffset(
    const CPoint& f_point
)
{
    CPoint l_point = f_point;

    int l_halfimage = DRAG_IMAGE_SIZE / 2;
    CPoint l_offset = f_point - m_drag_offset_point;

    CRect l_prev_rect = m_image_offset_pos;


    const int l_image_width = m_image_offset_pos.Width() ;
    const int l_image_height = m_image_offset_pos.Height() ;
    m_image_offset_pos.left = l_offset.x - l_halfimage;
    m_image_offset_pos.right = m_image_offset_pos.left + l_image_width ;
    m_image_offset_pos.top = l_offset.y - l_halfimage;
    m_image_offset_pos.bottom = m_image_offset_pos.top + l_image_height ;


    CRect l_wnd_rect;
    GetClientRect(&l_wnd_rect);
    l_wnd_rect = m_border_rect;

    CPoint l_center_point = CPoint(m_image_offset_pos.left + l_halfimage + 1,
                                   m_image_offset_pos.top + l_halfimage + 1
                                  );

    if (l_center_point.x < l_wnd_rect.left) {
        m_image_offset_pos.left = l_wnd_rect.left - l_halfimage ;
        m_image_offset_pos.right = m_image_offset_pos.left + DRAG_IMAGE_SIZE;

    } else if (l_center_point.x > l_wnd_rect.right) {
        m_image_offset_pos.left = l_wnd_rect.right - l_halfimage - 1;
        m_image_offset_pos.right = m_image_offset_pos.left + DRAG_IMAGE_SIZE ;
    }

    if (l_center_point.y < l_wnd_rect.top) {
        m_image_offset_pos.top = l_wnd_rect.top - l_halfimage;
        m_image_offset_pos.bottom = m_image_offset_pos.top + DRAG_IMAGE_SIZE;

    } else if (l_center_point.y > l_wnd_rect.bottom) {
        m_image_offset_pos.bottom = l_wnd_rect.bottom  + l_halfimage;
        m_image_offset_pos.top = m_image_offset_pos.bottom - DRAG_IMAGE_SIZE;
    }


    l_prev_rect.UnionRect(l_prev_rect, m_image_offset_pos);

    l_prev_rect.InflateRect(1, 1);

    InvalidateRect(l_prev_rect, true);

    ::SendMessage(GetParent()->m_hWnd, WM_UPDATE_IMAGE_OFFSET, 0, 0);
}


//************************************Method Header**************************************
//Method Name:  PaintPatientImage
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::PaintPatientImage(
    CDC* f_dc
)
{
    CBitmap l_offset_image;
    l_offset_image.LoadBitmap(IDB_IMAGE_OFFSET_WND_BITMAP);
    BITMAP bitmap_object ;
    l_offset_image.GetBitmap(&bitmap_object);
    l_offset_image.DeleteObject();

    const int l_image_width = bitmap_object.bmWidth;
    const int l_image_height = bitmap_object.bmHeight;

    CRect l_rect;
    GetClientRect(&l_rect);
    CRect client_rect  = l_rect;

    CPoint l_center = l_rect.CenterPoint();
    l_rect.left  = l_center.x - (l_image_width / 2) + 2;
    l_rect.right = l_rect.left + l_image_width;
    l_rect.top  = l_center.y - (l_image_height / 2) + 2;
    l_rect.bottom = l_rect.top + l_image_height;

    CPQMImageUtility::DrawBitmap(IDB_IMAGE_OFFSET_WND_BITMAP, l_rect, f_dc, client_rect, RGB(19, 25, 35));
}


//************************************Method Header**************************************
//Method Name:  PaintStaticImages
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::PaintStaticImages(
    CDC* f_dc
)
{

    PaintPatientImage(f_dc);
    CPen pen(PS_SOLID, 1, RGB(68, 136, 204));

    CPen* Old_pen = f_dc->SelectObject(&pen);
    CBrush* Old_brush = (CBrush*)f_dc->SelectStockObject(NULL_BRUSH);

    f_dc->Rectangle(&m_border_rect);

    CPoint l_vertical_line_top(m_border_rect.left + m_border_rect.Width() / 2 , m_border_rect.top);
    CPoint l_vertical_line_bottom(m_border_rect.left + m_border_rect.Width() / 2 , m_border_rect.bottom);
    f_dc->MoveTo(l_vertical_line_top.x, l_vertical_line_top.y);
    f_dc->LineTo(l_vertical_line_bottom.x, l_vertical_line_bottom.y);

    CPoint l_horizontal_line_left(m_border_rect.left, m_border_rect.top + m_border_rect.Height() / 2);
    CPoint l_horizontal_line_right(m_border_rect.right , m_border_rect.top + m_border_rect.Height() / 2);
    f_dc->MoveTo(l_horizontal_line_left);
    f_dc->LineTo(l_horizontal_line_right);

    f_dc->SelectObject(Old_pen);
    f_dc->SelectObject(Old_brush);
}


//************************************Method Header**************************************
//Method Name:  PaintImageOffsetPos
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CImageOffsetWnd::PaintImageOffsetPos(
    CDC* f_dc
)
{
    int mode = f_dc->SetBkMode(TRANSPARENT);

    CPQMImageUtility::DrawTrans(f_dc->m_hDC, m_image_offset_pos.left, m_image_offset_pos.top, IDB_IMAGE_OFFSET_CENTER_BITMAP, m_image_offset_pos, RGB(0, 255, 0));
    f_dc->SetBkMode(mode);
}


//************************************Method Header**************************************
//Method Name:  GetCenterImageRect
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
CRect CImageOffsetWnd::GetCenterImageRect(
) const
{

    CRect l_rect;
    GetClientRect(&l_rect);
    l_rect = m_border_rect;

    const int l_temp = DRAG_IMAGE_SIZE / 2 ;

    l_rect.left = l_rect.left + (l_rect.Width() / 2) - (l_temp) ;
    l_rect.right = l_rect.left + (DRAG_IMAGE_SIZE);

    l_rect.top = l_rect.top + (l_rect.Height() / 2) - (l_temp) ;
    l_rect.bottom = l_rect.top + (DRAG_IMAGE_SIZE);

    return l_rect;
}
