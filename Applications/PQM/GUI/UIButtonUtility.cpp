// UIButtonUtility.cpp: implementation of the CUIButtonUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIButtonUtility.h"
#include <libStudyManager/Mutex/ScopedLock.h>


using namespace MR::ACQ::SM::COMMON::MUTEX;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CUIButtonUtility* CUIButtonUtility::m_UIButtonUtility = NULL;

CUIButtonUtility* CUIButtonUtility::GetInstance()
{

    if (NULL == m_UIButtonUtility) {
        m_UIButtonUtility = new CUIButtonUtility;
    }

    return m_UIButtonUtility;
}

BOOL CUIButtonUtility::SetIconForButton(
    ButtonEx& f_button_object,
    const UINT bitmap_resource_id,
    const bool f_center_bitmap /*= true*/
) const
{
    LPCTSTR FUNC_NAME = _T("CUIButtonUtility::SetIconForButton");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CScopedLock l_scoped_lock(&m_mplusui_sync);

    try {
        CString l_icon_path = GetIconPath(bitmap_resource_id);

        if (l_icon_path.IsEmpty()) {
            try {
                f_button_object.SetIcon(NULL);

            } catch (...) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in f_button_object.SetIcon(NULL)."));
                throw;
            }

            return TRUE ;
        }

        HICON hIcon;

        try {
            hIcon = (HICON)::LoadImage(NULL, l_icon_path, IMAGE_ICON,
                                       0, 0, LR_LOADFROMFILE);

            if (NULL == hIcon) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("hIcon is NULL."));
                return FALSE;
            }

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in LoadImage function."));
            throw;
        }

        try {
            f_button_object.SetIcon(hIcon);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in Setting the Icon."));
            throw;
        }

        if (!f_center_bitmap) {
            try {
                f_button_object.SetPicturePos(-1, -1);

            } catch (...) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in f_button_object.SetPicturePos(-1, -1)."));
                throw;
            }

            //Patni-Hemant/2009Jan15/Added/GDI Leak Fix
            try {
                DestroyIcon(hIcon);

            } catch (...) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred While destroying the icon."));
                throw;
            }

            return TRUE;
        }

        BITMAP bitmap_object ;
        ICONINFO iconInfo;

        try {
            GetIconInfo(hIcon, &iconInfo);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred GetIconInfo"));
            throw;
        }

        try {
            GetObject((HGDIOBJ)iconInfo.hbmColor, sizeof(BITMAP), &bitmap_object);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred GetIconInfo"));
            throw;
        }

        CRect button_rect;

        try {
            f_button_object.GetClientRect(&button_rect);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in f_button_object.GetClientRect()"));
            throw;
        }

        const int pos_x = (int)((button_rect.Width() - bitmap_object.bmWidth) * 0.5) - 3;

        const int pos_y = (int)((button_rect.Height() - bitmap_object.bmHeight) * 0.5) - 4;

        try {
            f_button_object.SetPicturePos(pos_x, pos_y);

        } catch (...) {

            CString str;
            str.Format(_T("pos_x = %d, pos_y = %d"), pos_x, pos_y);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in f_button_object.SetPicturePos()") + str);
            throw;
        }

        //+Patni-Hemant/2009Jan15/Added/GDI Leak Fix
        try {
            DestroyIcon(hIcon);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred while DestroyIcon."));
            throw;
        }

        if (iconInfo.hbmColor) {
            try {
                DeleteObject((HGDIOBJ)iconInfo.hbmColor);

            } catch (...) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred while DeleteObject"));
                throw;
            }
        }

        if (iconInfo.hbmMask) {
            try {
                DeleteObject((HGDIOBJ)iconInfo.hbmMask);

            } catch (...) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred while DeleteObject"));
                throw;
            }
        }

        //-Patni-Hemant/2009Jan15/Added/GDI Leak Fix

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in Function..."));
        throw;
    }

    return TRUE ;
}



void CUIButtonUtility::SetEnableDisableButtonColor(
    ButtonEx& f_button ,
    const bool f_enable /*= true*/,
    const bool f_redraw_button /*= false*/
)
{

    static bool flag = true;
    static BUTTON_COLORS_t m_btnDefColor;

    if (flag) {
        f_button.GetButtonColors(m_btnDefColor);
        flag = false;
    }

    BUTTON_COLORS_t& l_button_color = f_enable ? m_btnHightlightColor : m_btnDefColor ;

    BUTTON_COLORS_t btnColor;
    f_button.GetButtonColors(btnColor);

    if ((btnColor.crBrushFaceStart == l_button_color.crBrushFaceStart)
        && (btnColor.crBrushFaceEnd == l_button_color.crBrushFaceEnd)
       )
        return ;

    f_button.SetButtonColors(l_button_color);

    if (f_redraw_button) {
        f_button.Invalidate();
        f_button.UpdateWindow();
    }
}


BOOL CUIButtonUtility::SetBitmapForButton(
    ButtonEx& button_object,
    const UINT bitmap_resource_id,
    COLORREF f_mask_color /*= RGB(0,255,0)*/,
    const bool center_bitmap /*= true*/,
    const bool right_align /*= false */
) const
{
    LPCTSTR FUNC_NAME = _T("CUIButtonUtility::SetBitmapForButton"); // Pavan-Redmine-2085
    PQM_TRACE_SCOPED(FUNC_NAME);
    CScopedLock l_scoped_lock(&m_mplusui_sync);

    try {
        if (bitmap_resource_id <= 0) {

            try {
                button_object.SetBitmap(NULL);

            } catch (...) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in SetBitmap(NULL)"));
                throw;
            }

            ASSERT(FALSE);
            return FALSE;
        }

        CBitmap button_bitmap;

        try {

            if (!button_bitmap.LoadBitmap(bitmap_resource_id)) {

                CString str;
                str.Format(_T("bitmap_resource_id = %d"), bitmap_resource_id);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("LoadBitmap Failed...") + str);
                return FALSE ;
            }

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred while LoadBitmap"));
            throw;
        }

        try {
            button_object.SetBitmap((HBITMAP)button_bitmap.m_hObject, f_mask_color);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in SetBitmap();"));
            throw;
        }

        BITMAP bitmap_object ;

        try {
            button_bitmap.GetBitmap(&bitmap_object);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in GetBitmap()"));
            throw;
        }

        CRect button_rect;

        try {
            button_object.GetClientRect(&button_rect);

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in GetClientRect()"));
            throw;
        }

        int pos_x = -1 ;
        int pos_y = -1 ;

        if (!center_bitmap) {

            if (right_align) {

                pos_x = (button_rect.Width() - bitmap_object.bmWidth) - 6 ;
                pos_y = (int)((button_rect.Height() - bitmap_object.bmHeight) * 0.5) - 4;
            }

        } else {

            pos_x = (int)((button_rect.Width() - bitmap_object.bmWidth) * 0.5) - 4;
            pos_y = (int)((button_rect.Height() - bitmap_object.bmHeight) * 0.5) - 4;
        }

        try {
            button_object.SetPicturePos(pos_x, pos_y);

        } catch (...) {

            CString str;
            str.Format(_T("pos_x = %d, pos_y = %d"), pos_x, pos_y);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in f_button_object.SetPicturePos()") + str);

            throw;
        }

        try {
            DeleteObject(button_bitmap) ;

        } catch (...) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred while DeleteObject."));
            throw;
        }

        return TRUE ;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occurred in CUIButtonUtility::SetBitmapForButton"));
        throw;
    }
}


void CUIButtonUtility::InitUIButtonUtility(
    const BUTTON_COLORS_t& f_btnDefColor
)
{
    m_btnDefColor = f_btnDefColor;
    m_btnHightlightColor = f_btnDefColor;

    const COLORREF l_enable_color = RGB(49, 150, 255) ;
    const COLORREF l_enable_color_face_end = RGB(24, 73, 132) ;

    m_btnHightlightColor.crBrushFaceStart = l_enable_color ;
    m_btnHightlightColor.crBrushFaceEnd = l_enable_color_face_end ;

}




CUIButtonUtility::CUIButtonUtility()
{

}


CUIButtonUtility::~CUIButtonUtility()
{
    DEL_PTR(m_UIButtonUtility);
}


CString CUIButtonUtility::GetIconPath(
    const int f_icon_id
)const
{
    char*	path = NULL;

    if ((path = getenv("MRMP_DATA")) == NULL)
        path = ".";

    CString l_path(path);
    l_path += _T("\\PQM\\");

    switch (f_icon_id) {
        case IDI_UTILITY_BUTTON_IMAGE:
            return l_path + _T("res\\PQM_IMAGES\\customize.ico");

        case IDI_PASS_BUTTON_IMAGE:
            return l_path + _T("res\\PQM_IMAGES\\memory.ico");

        case IDI_DELETE_BUTTON_IMAGE:
            return l_path + _T("res\\PQM_IMAGES\\delete.ico");

        case IDI_ADD_BUTTON_IMAGE:
            return l_path + _T("res\\PQM_IMAGES\\add.ico");

        case IDI_SCANEDIT_BUTTON_IMAGE:
            return l_path + _T("res\\PQM_IMAGES\\WSE.ico");

        case IDI_SCANOFFSET_BUTTON_IMAGE:
            return l_path + _T("res\\PQM_IMAGES\\ScanOffset.ico");

        case IDB_GRP1_COMBO11_BITMAP :
        case IDB_GRP2_COMBO11_BITMAP :

            return l_path + _T("res\\Patient Orientation Images\\Combo1\\direction_l_turningup_head.ico") ;

        case IDB_GRP1_COMBO12_BITMAP :
        case IDB_GRP2_COMBO12_BITMAP :

            return l_path + _T("res\\Patient Orientation Images\\Combo1\\direction_l_turningup_foot.ico") ;

        case IDB_GRP1_COMBO23_BITMAP :
        case IDB_GRP2_COMBO23_BITMAP :

            return l_path + _T("res\\Patient Orientation Images\\Combo2\\rotate_right.ico") ;

        case IDB_GRP1_COMBO31_BITMAP :
        case IDB_GRP2_COMBO31_BITMAP :

            return l_path + _T("res\\Patient Orientation Images\\Combo3\\view_r_side_head.ico") ;

        case IDB_GRP1_COMBO32_BITMAP :
        case IDB_GRP2_COMBO32_BITMAP :

            return l_path + _T("res\\Patient Orientation Images\\Combo3\\view_r_side_foot.ico") ;

        default :
            return _T("");
    }

    return _T("");
}


