//TMSC-Redmine-783
//TMSC-Redmine-780
//Redmine-775
//Redmine-777
#ifndef __SM_CUSTOM_STATIC_H__
#define __SM_CUSTOM_STATIC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomStatic.h : header file
//
#include <libStudyManager/NonCopyable.h>
/////////////////////////////////////////////////////////////////////////////
// CCustomStatic window

class CCustomStatic : public CStatic,
    private MR::ACQ::SM::COMMON::NonCopyable<CCustomStatic>
{
    // Construction
public:
    CCustomStatic();
    virtual ~CCustomStatic();

    void UpdateFont();
    void LoadDefaultFontAttributes();
    void LoadDeafultStaticCtrlColorsWithBKColor(const COLORREF f_bk_color);

    static void SetDefaultFontAttributes(const CString& font_name, const int font_size);
    static void SetDefaultTextColor(const COLORREF textcolor);

    void SetColorData(const COLORREF bkcolor, const COLORREF textcolor);
    void SetTextColor(const COLORREF textcolor);
    void SetFontName(const CString& font_name);
    void SetFontSize(const int font_size);
    void SetFontAttributes(const CString& font_name, const int font_size);
    // Generated message map functions
protected:
    //{{AFX_MSG(CCustomStatic)
    afx_msg HBRUSH CtlColor(CDC* pdc, UINT ctl_color);
    //}}AFX_MSG

private:
    static CCustomStatic m_default_static_data;
    CBrush m_back_color;
    COLORREF m_bkcolor;
    COLORREF m_textcolor;
    int m_font_size;
    CString m_font_name;
    CFont m_font;

    void LoadDefaultColors();
    void LoadDefaultTextColor();

    void SetBkColor(const COLORREF bkcolor);

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_CUSTOM_STATIC_H__
