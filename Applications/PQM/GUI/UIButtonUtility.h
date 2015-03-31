// UIButtonUtility.h: interface for the CUIButtonUtility class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_UIBUTTONUTILITY_H__
#define __SM_UIBUTTONUTILITY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <MPlusUICommon/ButtonEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/Mutex/ThreadMutex.h>


using namespace MPlus::UICommon;

class CUIButtonUtility : private MR::ACQ::SM::COMMON::NonCopyable<CUIButtonUtility>
{
public:

    static CUIButtonUtility* GetInstance();

    BOOL SetIconForButton(ButtonEx& f_button_object, const UINT bitmap_resource_id, const bool f_center_bitmap = true) const;

    void SetEnableDisableButtonColor(
        ButtonEx& f_button ,
        const bool f_enable = true,
        const bool f_redraw_button = false
    ) ;

    BOOL SetBitmapForButton(
        ButtonEx& button_object,
        const UINT bitmap_resource_id,
        COLORREF f_mask_color = RGB(0, 255, 0),
        const bool center_bitmap = true,
        const bool right_align = false
    ) const;

    void InitUIButtonUtility(const BUTTON_COLORS_t& f_btnDefColor);

private:

    static CUIButtonUtility* m_UIButtonUtility;

    BUTTON_COLORS_t m_btnDefColor;
    BUTTON_COLORS_t m_btnHightlightColor;

    mutable MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_mplusui_sync;

    CUIButtonUtility() ;
    ~CUIButtonUtility() ;

    CString GetIconPath(const int f_icon_id)const;

};

#endif // #ifndef __SM_UIBUTTONUTILITY_H__
