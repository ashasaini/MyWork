//TMSC-REDMINE-781
//TMSC-Redmine-780
//TMSC-REDMINE-777
//TMSC-REDMINE-774

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGraphCtrl.cpp
 *  Overview: Implementation of CPQMGraphCtrl class.
 *  Last Modified: 2009/05/06 15:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *	 3.0          Patni            2009/05/06 15:00:00     Re implemented according to Vantage 9.30
 *****************************************************************************/
#include "stdafx.h"
#include "PQMGraphCtrl.h"


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
const COLORREF g_text_color = RGB(136, 85, 68);
const COLORREF g_graph_line_color = RGB(136, 85, 68);
const COLORREF g_mode_marking_color = RGB(84, 255, 159);
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


BEGIN_MESSAGE_MAP(CPQMGraphCtrl, CWnd)
    //{{AFX_MSG_MAP(COpeGraph)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
//Method Name:  CPQMGraphCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPQMGraphCtrl::CPQMGraphCtrl()
{
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  : ~CPQMGraphCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPQMGraphCtrl::~CPQMGraphCtrl()
{
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header**************************************
//Method Name:  Create

//Author:  PATNI\MSN

//Purpose: Creates the Graph View to draw the CFA Graph

//**************************************************************************************/
BOOL CPQMGraphCtrl::Create(LPCTSTR lpszCaption,
                           DWORD dwStyle,
                           const RECT& rect,
                           CWnd* pParentWnd,
                           const UINT nID)
{

    return CWnd::Create(NULL, lpszCaption, dwStyle, rect, pParentWnd, nID);
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  : OnPaint
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGraphCtrl::OnPaint()
{
    CPaintDC m_dc(this); // device context for painting

    CBrush l_mybrush;
    l_mybrush.CreateSolidBrush(RGB(0, 0, 0));
    CPen frame_rect_pen(PS_SOLID, 1 , RGB(42, 80, 145));

    CBrush* const l_poldbrush = m_dc.SelectObject(&l_mybrush);
    CPen* const l_poldpen = m_dc.SelectObject(&frame_rect_pen);

    RECT l_rect;
    GetClientRect(&l_rect);
    m_dc.Rectangle(&l_rect);

    const COLORREF old_color = m_dc.SetTextColor(g_text_color);

    // Create a solid red pen of width 1.
    CPen gray_pen(PS_SOLID , 1 , g_graph_line_color);
    m_dc.SelectObject(&gray_pen);

    CRect rect = GetDrawingRect();
    shimPlotViewDrawAxis(&m_dc, &rect);

    CPen blue_pen(PS_SOLID , 1 , g_mode_marking_color);
    m_dc.SelectObject(&blue_pen);

    shimPlotViewDrawData(&m_dc, &rect);

    m_dc.SetTextColor(old_color);
    m_dc.SelectObject(l_poldpen);
    m_dc.SelectObject(l_poldbrush);

    //+GDI Object leak removal
    if (l_mybrush.m_hObject) {
        l_mybrush.DeleteObject();
    }

    //-GDI Object leak removal
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  : shimPlotViewDrawAxis
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGraphCtrl::shimPlotViewDrawAxis(
    CDC* dc, CRect* rect
)
{
    int x2 = 0;
    int x1 = x2 = rect->left + rect->Width() / 2;
    int y1 = rect->top;
    int y2 = rect->top + rect->Height() - 1;

    DrawLine(dc, CPoint(x1, y1), CPoint(x2, y2));

    x1 = rect->left;
    x2 = rect->left + rect->Width();
    y1 = y2 = rect->top + rect->Height();

    DrawLine(dc, CPoint(x1, y1), CPoint(x2, y2));

    y1 = rect->top;
    y2 = rect->top + rect->Height() - 1;
    y1 = y2 - 5;

    int	bk_mode = 0;
    CSize l_size;

    for (int i = 0; i < ShimWindow.num_scale; i++) {

        x1 = (int)(rect->left + rect->Width() * shimDataX(ShimWindow.scale[i].x));
        DrawLine(dc, CPoint(x1, y1), CPoint(x1, y2));




        bk_mode = dc->SetBkMode(TRANSPARENT);
        l_size = dc->GetTextExtent(ShimWindow.scale[i].str);

        dc->TextOut(x1 - (l_size.cx / 2), y2 + (l_size.cy / 2) - 2 , ShimWindow.scale[i].str);
        dc->SetBkMode(bk_mode);
    }
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGraphCtrl::shimPlotViewDrawData(
    CDC* dc, CRect* rect
)
{
    CPoint P1(0, 0), P2(0, 0);

    for (int i = 0; i < ShimWindow.cf_drawPoint; i++) {

        ShimWindow.cf_curveP[i].x = (long)(rect->left + rect->Width() * shimDataX((float)i));
        ShimWindow.cf_curveP[i].y = (long)(rect->top + rect->Height() * shimDataY((float)ShimWindow.cf_drawP[i]));

        if (i > 0) {

            P1.x = ShimWindow.cf_curveP[i - 1].x;
            P1.y = ShimWindow.cf_curveP[i - 1].y;

            P2.x = ShimWindow.cf_curveP[i].x;
            P2.y = ShimWindow.cf_curveP[i].y;

            DrawLine(dc, P1, P2);
        }
    }

    //XDrawLines(XtDisplay(w), XtWindow(w), ShimWindow.fore,
    //       ShimWindow.cf_curveP, ShimWindow.cf_drawPoint, CoordModeOrigin);

    int	x1 = 0, y1 = 0, bk_mode = 0;

    for (i = 0; i < 2; i++) {
        if (ShimWindow.peak[i].sw) {

            x1 = (int)(rect->left + rect->Width() * shimDataX((float)ShimWindow.peak[i].x) + 2);
            y1 = (int)(rect->top + rect->Height() * shimDataY((float)ShimWindow.peak[i].y));

            //XDrawImageString(XtDisplay(w),XtWindow(w),ShimWindow.axis,
            //		 x1,y1,ShimWindow.peak[i].str,1);
            bk_mode = dc->SetBkMode(TRANSPARENT);
            dc->TextOut(x1 , y1, ShimWindow.peak[i].str);
            dc->SetBkMode(bk_mode);
        }
    }
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
float CPQMGraphCtrl::shimDataX(
    const float x
)const
{
    if (ShimWindow.x[2] == 0.0) {
        return 0.5;
    }

    return (float)(x / ShimWindow.x[2]);
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
float CPQMGraphCtrl::shimDataY(const float y)const
{
    if (ShimWindow.y[2] == 0.0) {
        return 0.5;
    }

    return (float)(1.0 - (y - ShimWindow.y[0]) / ShimWindow.y[2]);
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413


//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  : DrawLine
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGraphCtrl::DrawLine(
    CDC* dc ,
    const CPoint& f_start_point,
    const CPoint& f_end_point
)
{

    POINT lp ;
    ::MoveToEx(dc->m_hDC, f_start_point.x , f_start_point.y , &lp);
    ::LineTo(dc->m_hDC, f_end_point.x , f_end_point.y);
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413



//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CRect CPQMGraphCtrl::GetDrawingRect(
)
{
    CRect l_graph_rect;
    GetWindowRect(&l_graph_rect);
    const int l_graph_width = l_graph_rect.Width();
    const int l_graph_height = l_graph_rect.Height();

    const int l_drawing_width = MAX(l_graph_width - 20, 20);
    const int l_drawing_height = MAX(l_graph_height - 40, 20);

    CRect l_rect_to_draw;
    l_rect_to_draw.left = l_graph_width - l_drawing_width - 10;
    l_rect_to_draw.top = l_graph_height - l_drawing_height - 20;
    l_rect_to_draw.right = l_rect_to_draw.left + l_drawing_width ;
    l_rect_to_draw.bottom = l_rect_to_draw.left + l_drawing_height ;

    return l_rect_to_draw ;
}
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413