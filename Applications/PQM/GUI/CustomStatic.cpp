//TMSC-REDMINE-782
//TMSC-Redmine-780
//TMSC-REDMINE-777
// CustomStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CustomStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCustomStatic CCustomStatic::m_default_static_data ;


BEGIN_MESSAGE_MAP(CCustomStatic, CStatic)
    //{{AFX_MSG_MAP(CCustomStatic)
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CCustomStatic::CCustomStatic():
    m_bkcolor(GetSysColor(COLOR_BTNFACE)),
    m_textcolor(GetSysColor(COLOR_WINDOWTEXT)),
    m_font_size(0),
    m_font_name(_T("Arial"))
{
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CCustomStatic::~CCustomStatic()
{
    if (m_back_color.GetSafeHandle())
        m_back_color.DeleteObject();

    if (m_font.GetSafeHandle())
        m_font.DeleteObject();
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::UpdateFont(
)
{
    LPCTSTR FUNC_NAME = _T("CCustomStatic::UpdateFont");

    if (m_back_color.GetSafeHandle()) {
        m_back_color.DeleteObject();
    }

    //+ MEITEC/2010Sep22/Added/REDMINE-756
    try {
        m_back_color.CreateSolidBrush(m_bkcolor);

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CreateSolidBrush() EXCEPTION"));
        throw;
    }

    //- MEITEC/2010Sep22/Added/REDMINE-756

    CDC* clpDC = GetDC() ;

    if (clpDC) {

        if (m_font.GetSafeHandle())
            m_font.DeleteObject();

        m_font.CreatePointFont(m_font_size, m_font_name, clpDC) ;

        SetFont(&m_font) ;
    }

    //Patni-Hemant/2009Jan15/Added/GDI Leak Fix
    ReleaseDC(clpDC);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::LoadDefaultFontAttributes(
)
{
    SetFontAttributes(m_default_static_data.m_font_name, m_default_static_data.m_font_size);
}

void CCustomStatic::LoadDeafultStaticCtrlColorsWithBKColor(
    const COLORREF f_bk_color
)
{
    LoadDefaultFontAttributes();
    LoadDefaultTextColor();
    SetBkColor(f_bk_color);
    UpdateFont();
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetDefaultFontAttributes(
    const CString& font_name,
    const int font_size
)
{

    m_default_static_data.m_font_size = font_size;
    m_default_static_data.m_font_name = font_name;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetDefaultTextColor(
    const COLORREF textcolor
)
{
    m_default_static_data.m_textcolor = textcolor;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetColorData(
    const COLORREF back_color,
    const COLORREF textcolor
)
{
    m_bkcolor = back_color ;
    m_textcolor = textcolor ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetTextColor(
    const COLORREF textcolor
)
{
    m_textcolor = textcolor ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetFontName(
    const CString& font_name
)
{

    m_font_name = font_name;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetFontSize(
    const int font_size
)
{

    m_font_size = font_size;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::SetFontAttributes(
    const CString& font_name,
    const int font_size
)
{
    SetFontName(font_name);
    SetFontSize(font_size);

    //UpdateFont();
}

/////////////////////////////////////////////////////////////////////////////
// CCustomStatic message handlers

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
HBRUSH CCustomStatic::CtlColor(
    CDC* pdc,
    UINT ctl_color
)
{
    ASSERT(pdc != NULL);

    // Change static text color and background color of
    // static control
    pdc->SetTextColor(m_textcolor);
    pdc->SetBkColor(m_bkcolor);

    return (HBRUSH) m_back_color;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::LoadDefaultColors(
)
{
    SetColorData(m_default_static_data.m_bkcolor, m_default_static_data.m_textcolor);
    SetFontAttributes(m_default_static_data.m_font_name, m_default_static_data.m_font_size);
    UpdateFont();
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCustomStatic::LoadDefaultTextColor(
)
{
    SetTextColor(m_default_static_data.m_textcolor);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline void CCustomStatic::SetBkColor(
    const COLORREF bkcolor
)
{
    m_bkcolor = bkcolor ;
}




