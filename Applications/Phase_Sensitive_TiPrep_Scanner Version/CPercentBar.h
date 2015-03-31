#if !defined(AFX_CPERCENTBAR_H__80CDF8A5_10D2_4B8C_952F_96005D6DBEBA__INCLUDED_)
#define AFX_CPERCENTBAR_H__80CDF8A5_10D2_4B8C_952F_96005D6DBEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPercentBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPercentBar window

class CPercentBar : public CStatic
{
// Construction
public:
	CPercentBar();

// Attributes
public:
		int m_Range;
		int m_currentPos;
		COLORREF m_cFillColor;
		COLORREF m_cTextColor;
		int m_val;
		bool m_bShowVal;


// Operations
public:
		void SetRange( int r );
		void SetCurrentPos( int pos);
		int	 GetRange( );
		int	 GetCurrentPos();
		void SetPercentage( int per);
		void SetFillColor( COLORREF );
		void SetTextColor( COLORREF );
		void SetValue( int val );
		void SetShowValue( bool flag ) { m_bShowVal = flag; }
		

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPercentBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPercentBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPercentBar)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPERCENTBAR_H__80CDF8A5_10D2_4B8C_952F_96005D6DBEBA__INCLUDED_)
