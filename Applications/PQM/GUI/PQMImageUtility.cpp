// PQMImageUtility.cpp: implementation of the CPQMImageUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PQMImageUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPQMImageUtility::CPQMImageUtility()
{

}

CPQMImageUtility::~CPQMImageUtility()
{

}



void CPQMImageUtility::DrawTrans(HDC hDC, const int x, const int y, const UINT image_resourceid, const CRect& display_rect, const COLORREF f_mask_color, const int f_crop_left, const int f_crop_right)
{
    CBitmap m_thumb_image;
    m_thumb_image.LoadBitmap(image_resourceid);
    DrawTransBitmap(hDC, x, y, display_rect, f_mask_color, m_thumb_image, f_crop_left, f_crop_right);
    m_thumb_image.DeleteObject();

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMImageUtility::CreateMask(HDC hDC, HBITMAP& m_hbmMask, CBitmap& m_thumb_image , const CRect& display_rect, const COLORREF f_mask_color)
{
    //GetThumbRect(&thumb_rect);
    BITMAP bitmap_object ;
    m_thumb_image.GetBitmap(&bitmap_object);

    int dx = bitmap_object.bmWidth;
    int dy = bitmap_object.bmHeight;


    // Nuke any existing mask.
    if (m_hbmMask) {
        ::DeleteObject(m_hbmMask);
    }

    // Create memory DCs to work with.
    HDC hdcMask = ::CreateCompatibleDC(hDC);
    HDC hdcImage = ::CreateCompatibleDC(hDC);
    // Create a monochrome bitmap for the mask.
    m_hbmMask = ::CreateBitmap(/*bitmap_object.bmWidth*/dx,
                /*bitmap_object.bmHeight*/dy,
                1,
                1,
                NULL);
    // Select the mono bitmap into its DC.
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);
    // Select the image bitmap into its DC.
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_thumb_image.m_hObject);
    // Set the transparency color to be the top-left pixel.

    ::SetBkColor(hdcImage, f_mask_color);

    // Make the mask.
    ::BitBlt(hdcMask,
             0, 0,
             /*bitmap_object.bmWidth*/dx,
             /*bitmap_object.bmHeight*/dy,
             hdcImage,
             0, 0,
             SRCCOPY);

    // Tidy up.
    ::SelectObject(hdcMask, hbmOldMask);
    ::SelectObject(hdcImage, hbmOldImage);
    ::DeleteDC(hdcMask);
    ::DeleteDC(hdcImage);
}


void CPQMImageUtility::DrawBitmap(
    const UINT f_bitmap_id,
    const CRect& f_rect,
    CDC* f_dc,
    const CRect& f_client_rect,
    const COLORREF f_client_bk_color
)
{

    CBitmap l_bitmap;

    if (!l_bitmap.LoadBitmap(f_bitmap_id)) {
        return;
    }

    CDC l_memdc;
    l_memdc.CreateCompatibleDC(f_dc);

    CBitmap* l_oldbmp = l_memdc.SelectObject(&l_bitmap);

    if (!f_client_rect.IsRectEmpty()) {

        CBrush l_brush(f_client_bk_color);
        CBrush* Old_brush = f_dc->SelectObject(&l_brush);

        f_dc->Rectangle(&f_client_rect);
        f_dc->SelectObject(Old_brush);
    }

    f_dc->BitBlt(f_rect.left, f_rect.top, f_rect.Width(), f_rect.Height(), &l_memdc, 0, 0, SRCCOPY);
    l_memdc.SelectObject(l_oldbmp);

    l_bitmap.DeleteObject();
}


//************************************Method Header*****************************
// Method Name  : GetToolTipValueFor
// Author       :
// Purpose      :
//******************************************************************************
const CString CPQMImageUtility::GetToolTipValueFor(
    const CString f_param,
    const CString f_value,
    CDC* f_dc,
    CRect& f_tooltip_text_rect
)
{
    CString l_str_tooltip = f_param;

    CRect l_rect1(0, 0, 0, 0) , l_rect2(0, 0, 0, 0), l_rect3(0, 0, 0, 0);

    f_dc->DrawText(l_str_tooltip, &l_rect1, DT_CALCRECT);
    f_tooltip_text_rect = l_rect1;
    CString l_sep = _T("---") ;
    f_dc->DrawText(l_sep, &l_rect3, DT_CALCRECT);

    while (l_rect3.right < l_rect1.right) {

        l_sep += _T("-") ;
        f_dc->DrawText(l_sep, &l_rect3, DT_CALCRECT);
    }

    l_str_tooltip += _T("\n") ;
    l_str_tooltip +=  l_sep;

    if (f_value.IsEmpty()) {

        return l_str_tooltip;
    }

    f_dc->DrawText(f_value, &l_rect2, DT_CALCRECT);
    f_tooltip_text_rect.UnionRect(l_rect1, l_rect2);


    l_str_tooltip += _T("\n") + f_value;

    return l_str_tooltip;
}

void CPQMImageUtility::DrawTransBitmap(HDC hDC, const int x, const int y, const CRect& display_rect, const COLORREF f_mask_color, CBitmap& f_bitmap, const int f_crop_left, const int f_crop_right)
{

    BITMAP bitmap_object ;
    f_bitmap.GetBitmap(&bitmap_object);

    ASSERT(hDC);

    HBITMAP m_hbmMask = 0 ;
    CreateMask(hDC, m_hbmMask, f_bitmap, display_rect, f_mask_color);

    ASSERT(m_hbmMask);

    int dx = bitmap_object.bmWidth;
    int dy = bitmap_object.bmHeight;
    //dx = display_rect.Width() ;
    //dy = display_rect.Height();

    // Create a memory DC to which to draw.
    HDC hdcOffScr = ::CreateCompatibleDC(hDC);
    // Create a bitmap for the off-screen DC that is really
    // color-compatible with the destination DC.
    HBITMAP hbmOffScr = ::CreateBitmap(dx, dy,
                                       (BYTE)GetDeviceCaps(hDC, PLANES),
                                       (BYTE)GetDeviceCaps(hDC, BITSPIXEL),
                                       NULL);
    // Select the buffer bitmap into the off-screen DC.
    HBITMAP hbmOldOffScr = (HBITMAP)::SelectObject(hdcOffScr, hbmOffScr);

    // Copy the image of the destination rectangle to the
    // off-screen buffer DC, so we can play with it.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy, hDC, x, y, SRCCOPY);

    // Create a memory DC for the source image.
    HDC hdcImage = ::CreateCompatibleDC(hDC);
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, f_bitmap.m_hObject);

    // Create a memory DC for the mask.
    HDC hdcMask = ::CreateCompatibleDC(hDC);
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);

    // XOR the image with the destination.
    ::SetBkColor(hdcOffScr, RGB(255, 255, 255)) ;

    ::BitBlt(hdcOffScr, 0, 0, dx, dy , hdcImage, 0, 0, SRCINVERT);

    // AND the destination with the mask.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy, hdcMask, 0, 0, SRCAND);

    // XOR the destination with the image again.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy, hdcImage, 0, 0, SRCINVERT);

    dx = display_rect.Width();
    dy = display_rect.Height();

    int l_new_x = x + f_crop_left;
    dx = dx - f_crop_left - f_crop_right;

    // Copy the resultant image back to the screen DC.
    ::BitBlt(hDC, l_new_x, y, dx, dy , hdcOffScr, f_crop_left, 0, SRCCOPY);
    //::StretchBlt(hDC, x, y, dx, dy, hdcOffScr, 0, 0, bitmap_dx,bitmap_dy, SRCCOPY);

    // Tidy up.
    ::SelectObject(hdcOffScr, hbmOldOffScr);
    ::SelectObject(hdcImage, hbmOldImage);
    ::SelectObject(hdcMask, hbmOldMask);
    ::DeleteObject(hbmOffScr);
    ::DeleteDC(hdcOffScr);
    ::DeleteDC(hdcImage);
    ::DeleteDC(hdcMask);

    ::DeleteObject(m_hbmMask);
}
