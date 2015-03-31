#if !defined(AFX_SPINCTRL_H__A6218B0F_9646_4891_9F0A_561212B07549__INCLUDED_)
#define AFX_SPINCTRL_H__A6218B0F_9646_4891_9F0A_561212B07549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpinCtrl.h : header file
//
#include "PopupDlg.h"

#include <MPlusUICommon/SpinEx.h>
/////////////////////////////////////////////////////////////////////////////
// SpinCtrl window

class SpinCtrl : public SpinEx
{
// Construction
public:
	SpinCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SpinCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(SpinCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
        BOOL OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINCTRL_H__A6218B0F_9646_4891_9F0A_561212B07549__INCLUDED_)
