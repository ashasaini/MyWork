// MoveCouchWnd.cpp : implementation file
//

#include "..\stdafx.h"
#include "MoveCouchWnd.h"
#include <PQM/resource.h>
#include "..\PQMImageUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMoveCouchWnd
//************************************Method Header**************************************
//Method Name:  CMoveCouchWnd
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
CMoveCouchWnd::CMoveCouchWnd(
    const COUCH_POSITION f_couch_pos,
    const PatientInsertDirection f_orientationoption1,
    const PatientRotationDirection f_orientationoption2,
    const float f_current_z_offset_min,
    const float f_current_z_offset_max
) :
    m_couch_center_left((f_couch_pos == COUCH_POSITION_LEFT) ? 166 : 337),
    m_couch_center_top((f_couch_pos == COUCH_POSITION_LEFT) ?  188 : 188),
    m_couch_pos(f_couch_pos),
    m_patient_ori_image_id(GetPatientOriImageID(f_couch_pos, f_orientationoption1, f_orientationoption2)),
    m_patient_ori_rect(0, 0, 0, 0),
    m_couch_image_rect(0, 0, 0, 0),
    m_dragging_couch(false),
    m_drag_couch_offset_point(0, 0)
{
    UpdateCouchImageRect();

    const int l_left_gantry_end = ((f_couch_pos == COUCH_POSITION_LEFT) ? 76 : 249) ;
    const int l_right_gantry_end = ((f_couch_pos == COUCH_POSITION_LEFT) ?  254 : 427);
    const int l_max_gantry_inout_limit = 200;

    const float f_inside_range = (f_couch_pos == COUCH_POSITION_LEFT) ? f_current_z_offset_min * (-1) : f_current_z_offset_max;
    const float f_outside_range = (f_couch_pos == COUCH_POSITION_LEFT) ? f_current_z_offset_max : f_current_z_offset_min * (-1);

    m_inside_limit = (int)(m_couch_center_left - ((m_couch_center_left - l_left_gantry_end) * (f_inside_range / l_max_gantry_inout_limit) + 0.5));
    m_outside_limit = (int)(m_couch_center_left + ((l_right_gantry_end - m_couch_center_left) * (f_outside_range / l_max_gantry_inout_limit) + 0.5));

}


//************************************Method Header**************************************
//Method Name:  ~CMoveCouchWnd
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
CMoveCouchWnd::~CMoveCouchWnd()
{
}

//************************************Method Header**************************************
//Method Name:  InitMoveCouchWnd
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::InitMoveCouchWnd()
{

    m_patient_ori_rect = GetPatientOriImageRect(m_couch_pos) ;
}


//************************************Method Header**************************************
//Method Name:  GetDragFactorX
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const float CMoveCouchWnd::GetDragFactorX(
) const
{

    const CPoint l_org_center = GetOrgCenterPoint();
    const CPoint l_current_center = GetCenterPoint();
    return (l_current_center.x - l_org_center.x) / GetDivFactor() ;
}



//************************************Method Header**************************************
//Method Name:  SetImagePos
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::SetImagePos(const float f_percentX)
{

    CRect l_prev_rect = m_couch_image_rect;

    const CPoint l_org_center = GetOrgCenterPoint();
    const int l_x_offset = (int)(l_org_center.x + (f_percentX * GetDivFactor()));
    CenterRect(m_couch_image_rect, CPoint(l_x_offset, m_couch_image_rect.CenterPoint().y));

    l_prev_rect.UnionRect(l_prev_rect, m_couch_image_rect);
    l_prev_rect.InflateRect(2, 2);

    InvalidateRect(l_prev_rect, true);
}


BEGIN_MESSAGE_MAP(CMoveCouchWnd, CWnd)
    //{{AFX_MSG_MAP(CMoveCouchWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMoveCouchWnd message handlers

//************************************Method Header**************************************
//Method Name:  OnPaint
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here
    PaintStaticImages(&dc);
    DrawCouchImage(&dc);
    DrawPatientOriImage(&dc);
}


//************************************Method Header**************************************
//Method Name:  OnLButtonDown
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (m_couch_image_rect.PtInRect(point)) {

        CPoint l_center_rect = m_couch_image_rect.CenterPoint(); //GetCenterImageRect();
        m_drag_couch_offset_point = point - l_center_rect;

        m_dragging_couch = true;

        SetCapture();
    }

    CWnd::OnLButtonDown(nFlags, point);
}


//************************************Method Header**************************************
//Method Name:  OnMouseMove
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (nFlags& MK_LBUTTON == MK_LBUTTON) {

        if (m_dragging_couch) {
            DragCouchImage(point);
        }
    }

    CWnd::OnMouseMove(nFlags, point);
}


//************************************Method Header**************************************
//Method Name:  OnLButtonUp
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_dragging_couch) {
        m_dragging_couch = false;
        ReleaseCapture();

    } else {
        //TempUpdatePatientOri();
    }

    CWnd::OnLButtonUp(nFlags, point);
}


//************************************Method Header**************************************
//Method Name:  OnEraseBkgnd
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
BOOL CMoveCouchWnd::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}


//************************************Method Header**************************************
//Method Name:  PaintStaticImages
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::PaintStaticImages(
    CDC* f_dc
) const
{
    CRect l_rect;
    GetClientRect(&l_rect);

    CPQMImageUtility::DrawBitmap((m_couch_pos == COUCH_POSITION_LEFT) ? IDB_MOVE_COUCH_LEFT : IDB_MOVE_COUCH_RIGHT , l_rect, f_dc);

    CPen pen(PS_SOLID, 1, RGB(28, 60, 85));
    CPen* Old_pen = f_dc->SelectObject(&pen);
    CBrush* Old_brush = (CBrush*)f_dc->SelectStockObject(NULL_BRUSH);

    l_rect.InflateRect(1, 1);
    f_dc->Rectangle(&l_rect);

    f_dc->SelectObject(Old_pen);
    f_dc->SelectObject(Old_brush);
}

//************************************Method Header**************************************
//Method Name:  DrawCouchImage
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::DrawCouchImage(
    CDC* f_dc
) const
{
    CPQMImageUtility::DrawTrans(f_dc->m_hDC, m_couch_image_rect.left, m_couch_image_rect.top, IDB_MOVE_COUCH_CENTER_BITMAP, m_couch_image_rect, RGB(0, 255, 0));
}


//************************************Method Header**************************************
//Method Name:  DrawPatientOriImage
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::DrawPatientOriImage(
    CDC* f_dc
) const
{
    if (m_patient_ori_image_id > 0) {
        CPQMImageUtility::DrawTrans(f_dc->m_hDC, m_patient_ori_rect.left, m_patient_ori_rect.top, m_patient_ori_image_id, m_patient_ori_rect, RGB(11, 15, 23));
    }
}


//************************************Method Header**************************************
//Method Name:  DragCouchImage
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::DragCouchImage(
    const CPoint& f_point
)
{
    CRect l_prev_rect = m_couch_image_rect;

    CPoint l_point = f_point;
    const int l_image_width = m_couch_image_rect.Width();
    const int l_halfimage = l_image_width / 2;
    const CPoint l_offset = f_point - m_drag_couch_offset_point;

    m_couch_image_rect.left = l_offset.x - l_halfimage;
    m_couch_image_rect.right = m_couch_image_rect.left + l_image_width ;

    CPoint l_center_point = CPoint(m_couch_image_rect.left + l_halfimage + 1,
                                   m_couch_image_rect.top + l_halfimage + 1);

    CRect l_update_rect = m_couch_image_rect;

    if (l_center_point.x <= m_inside_limit) {

        m_couch_image_rect.left = (int)(m_inside_limit - l_halfimage) ;
        m_couch_image_rect.right = m_couch_image_rect.left + l_image_width;

    } else if (l_center_point.x >= m_outside_limit) {
        m_couch_image_rect.left = (int)(m_outside_limit - l_halfimage) ;
        m_couch_image_rect.right = m_couch_image_rect.left + l_image_width ;
    }

    if (m_couch_image_rect == l_prev_rect) {
        return;
    }

    l_prev_rect.UnionRect(l_prev_rect, m_couch_image_rect);
    l_prev_rect.InflateRect(2, 2);

    InvalidateRect(l_prev_rect, true);

    ::SendMessage(GetParent()->m_hWnd, WM_UPDATE_COUCH_OFFSET, 0, 0);
}



//************************************Method Header**************************************
//Method Name:  CenterRect
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::CenterRect(
    CRect& f_rect,
    CPoint f_center_point
) const
{

    const int l_width = f_rect.Width();
    const int l_height = f_rect.Height();

    f_rect.left = f_center_point.x - l_width / 2;
    f_rect.right = f_rect.left + l_width;

    f_rect.top = f_center_point.y - l_height / 2;
    f_rect.bottom = f_rect.top + l_height;
}


//************************************Method Header**************************************
//Method Name:  UpdateCouchImageRect
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
void CMoveCouchWnd::UpdateCouchImageRect()
{

    CBitmap bitmap ;
    BITMAP bitmap_object;
    bitmap.LoadBitmap(IDB_MOVE_COUCH_CENTER_BITMAP);
    bitmap.GetBitmap(&bitmap_object);

    m_couch_image_rect.left = m_couch_center_left - bitmap_object.bmWidth / 2;
    m_couch_image_rect.right = m_couch_image_rect.left + bitmap_object.bmWidth;

    m_couch_image_rect.top = m_couch_center_top - bitmap_object.bmHeight / 2;
    m_couch_image_rect.bottom = m_couch_image_rect.top + bitmap_object.bmHeight;

    bitmap.DeleteObject();
}


//************************************Method Header**************************************
//Method Name:  GetDivFactor
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const float CMoveCouchWnd::GetDivFactor(
)
{
    const CPoint l_org_center = GetOrgCenterPoint();
    const CPoint l_current_center = GetCenterPoint();

    return (float)((l_current_center.x > l_org_center.x) ? (float)(m_outside_limit - l_org_center.x) : (float)(l_org_center.x - m_inside_limit));

}


//************************************Method Header**************************************
//Method Name:  GetPatientOriImageRect
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const CRect CMoveCouchWnd::GetPatientOriImageRect(
    const COUCH_POSITION f_couch_pos
) const
{

    CRect l_client_rect;
    GetClientRect(&l_client_rect);
    CRect l_new_rect;

    const int l_edge_offset_x = 45;
    const int l_edge_offset_y = 75;

    const int g_patient_ori_image_width = 131;
    const int g_patient_ori_image_height = 37;

    switch (f_couch_pos) {

        case COUCH_POSITION_LEFT:

            l_new_rect.right = l_client_rect.right - l_edge_offset_x;
            l_new_rect.left = l_new_rect.right - g_patient_ori_image_width;

            l_new_rect.top = l_client_rect.top + l_edge_offset_y;
            l_new_rect.bottom = l_new_rect.top + g_patient_ori_image_height;

            break;

        case COUCH_POSITION_RIGHT:

            l_new_rect.left = l_client_rect.left + l_edge_offset_x;
            l_new_rect.right = l_new_rect.left + g_patient_ori_image_width;

            l_new_rect.top = l_client_rect.top + l_edge_offset_y;
            l_new_rect.bottom = l_new_rect.top + g_patient_ori_image_height;

            break;

        default:
            return CRect(0, 0, 0, 0);
    }

    return l_new_rect;
}



//************************************Method Header**************************************
//Method Name:  GetPatientOriImageID
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const UINT CMoveCouchWnd::GetPatientOriImageID(
    const COUCH_POSITION f_couch_pos,
    PatientInsertDirection f_patient_in_or_out,
    PatientRotationDirection f_patient_side
) const
{

    if (((COUCH_POSITION_LEFT == f_couch_pos) && (Patient_FeetFirst == f_patient_in_or_out)) ||
        ((COUCH_POSITION_RIGHT == f_couch_pos) && (Patient_HeadFirst == f_patient_in_or_out))
       ) {

        switch (f_patient_side) {
            case PatientRotation_SUPINE:
                return IDB_SCAN_OFFSET_ORI_LEFT_UP;

            case PatientRotation_PRONE:
                return IDB_SCAN_OFFSET_ORI_LEFT_DOWN;

            case PatientRotation_RIGHT_UP:
                return IDB_SCAN_OFFSET_ORI_LEFT_BACK;

            case PatientRotation_LEFT_UP:
                return IDB_SCAN_OFFSET_ORI_LEFT_FRONT;
        }

    } else if (((COUCH_POSITION_LEFT == f_couch_pos) && (Patient_HeadFirst == f_patient_in_or_out)) ||
               ((COUCH_POSITION_RIGHT == f_couch_pos) && (Patient_FeetFirst == f_patient_in_or_out))
              ) {

        switch (f_patient_side) {
            case PatientRotation_SUPINE:
                return IDB_SCAN_OFFSET_ORI_RIGHT_UP;

            case PatientRotation_PRONE:
                return IDB_SCAN_OFFSET_ORI_RIGHT_DOWN;

            case PatientRotation_RIGHT_UP:
                return IDB_SCAN_OFFSET_ORI_RIGHT_BACK;

            case PatientRotation_LEFT_UP:
                return IDB_SCAN_OFFSET_ORI_RIGHT_FRONT;
        }
    }

    return 0;
}


//************************************Method Header**************************************
//Method Name:  GetDivFactor
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const float CMoveCouchWnd::GetDivFactor() const
{

    CPoint l_current_center = GetCenterPoint();
    CPoint l_org_center = GetOrgCenterPoint();

    float l_div_factor = 1;

    if (l_current_center.x > l_org_center.x) {
        l_div_factor = (float)(m_outside_limit - l_org_center.x) ;

    } else if (l_current_center.x < l_org_center.x) {
        l_div_factor = (float)(l_org_center.x - m_inside_limit);
    }

    return l_div_factor;
}


//************************************Method Header**************************************
//Method Name:  GetCenterPoint
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const CPoint CMoveCouchWnd::GetCenterPoint(
) const
{

    return m_couch_image_rect.CenterPoint();
}


//************************************Method Header**************************************
//Method Name:  GetOrgCenterPoint
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
const CPoint CMoveCouchWnd::GetOrgCenterPoint(
) const
{
    return CPoint(m_couch_center_left, m_couch_center_top);
}
