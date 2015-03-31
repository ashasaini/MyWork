// PQMImageUtility.h: interface for the CPQMImageUtility class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_PQMIMAGEUTILITY_H__
#define __SM_PQMIMAGEUTILITY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPQMImageUtility
{
public:
    static void DrawTrans(HDC hDC, const int x, const int y, const UINT image_resourceid, const CRect& display_rect, const COLORREF f_mask_color, const int f_crop_left = 0, const int f_crop_right = 0);
    static void DrawBitmap(const UINT f_bitmap_id, const CRect& f_rect, CDC* f_dc, const CRect& f_client_rect = CRect(0, 0, 0, 0), const COLORREF f_client_bk_color = RGB(0, 0, 0)) ;

    static const CString GetToolTipValueFor(const CString f_param, const CString f_value, CDC* f_dc, CRect& f_tooltip_text_rect) ;

private:
    CPQMImageUtility();
    virtual ~CPQMImageUtility();
    static void CreateMask(HDC hDC, HBITMAP& m_hbmMask, CBitmap& m_thumb_image , const CRect& display_rect, const COLORREF f_mask_color);
    static void DrawTransBitmap(HDC hDC, const int x, const int y, const CRect& display_rect, const COLORREF f_mask_color, CBitmap& f_bitmap, const int f_crop_left = 0, const int f_crop_right = 0);

};

#endif // #ifndef __SM_PQMIMAGEUTILITY_H__
