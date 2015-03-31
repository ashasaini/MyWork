// CPercentBar.cpp : implementation file
//

#include "stdafx.h"
#include "CPercentBar.h"
const COLORREF BKCOLOR = RGB(27, 36, 54);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPercentBar

CPercentBar::CPercentBar()
{
	m_Range		 = 100;
	m_currentPos = 0;
	m_cFillColor = RGB(255,0,255);
	m_cTextColor = RGB(0,0,255);
	m_bShowVal = false;
	m_val = 0;

}

CPercentBar::~CPercentBar()
{
}


BEGIN_MESSAGE_MAP(CPercentBar, CStatic)
	//{{AFX_MSG_MAP(CPercentBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPercentBar message handlers

void CPercentBar::SetRange( int r )
{
	m_Range = r;
}
void CPercentBar::SetCurrentPos(int pos)
{
	m_currentPos = pos;
}
int CPercentBar::GetRange()
{
	return m_Range;
}
int CPercentBar::GetCurrentPos()
{
	return m_currentPos;
}
void CPercentBar::SetPercentage( int per)
{
	m_currentPos = per * m_Range / 100;
	Invalidate();
}
void CPercentBar::SetValue( int val )
{
	m_val = val;
	m_currentPos = (m_val/*%100*/) * 100 / m_Range;
	Invalidate();

}
void CPercentBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	//Get the client rect
	CRect clientRect;
	GetClientRect(&clientRect);

	CRect rectHalf;
	if( !m_Range )
		return;

	rectHalf = clientRect;
	rectHalf.left	= 0;
	rectHalf.right	= clientRect.Width() * m_currentPos / 100;
	
	CString str = "";
	if( m_bShowVal )
		str.Format(_T("%d"),m_val );
	else

		str.Format(_T("%d%%"),m_currentPos );
	CSize size = dc.GetTextExtent(str);

	//Drawing the first half
	COLORREF oldBkColor ;
	COLORREF oldTxtColor;
    
	oldBkColor = dc.SetBkColor(m_cFillColor);
	oldTxtColor = dc.SetTextColor(RGB(255,255,255));
	dc.ExtTextOut( (clientRect.Width() - size.cx)/2,
				   (clientRect.Height() - size.cy)/2,
				    ETO_CLIPPED | ETO_OPAQUE,
					rectHalf,
					str,
					NULL
		         );

	//Drawing the second half
	rectHalf.left = rectHalf.right+1;
	rectHalf.right = clientRect.right;
	dc.SetBkColor(/*RGB(255,255,255)*/BKCOLOR);
	dc.SetTextColor(m_cTextColor);
	dc.ExtTextOut( (clientRect.Width() - size.cx)/2,
						(clientRect.Height() - size.cy)/2,
				    ETO_CLIPPED | ETO_OPAQUE,
					rectHalf,
					str,
					NULL
		         );

	 dc.SetBkColor(oldBkColor);
	 dc.SetTextColor(oldTxtColor);


}

void CPercentBar::SetFillColor( COLORREF frgb )
{
	m_cFillColor = frgb;
}
void CPercentBar::SetTextColor( COLORREF frgb)
{
	m_cTextColor = frgb;
}