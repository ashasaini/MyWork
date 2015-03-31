#if !defined(AFX_INFOVIEW_H__AB040AC1_932D_4990_A575_32EABE933B59__INCLUDED_)
#define AFX_INFOVIEW_H__AB040AC1_932D_4990_A575_32EABE933B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormStatus form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormStatus : public CFormView
{
protected:
    CFormStatus();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFormStatus)

// Form Data
public:
    //{{AFX_DATA(CFormStatus)
	enum { IDD = IDD_FORM_STATUS };
	CEdit	m_ctrlStatus;
    CButton m_ctrlCloseButton;
	//}}AFX_DATA

// Attributes
public:
    int setErrorMessage(CString &f_csErrorMsg);
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormStatus)
	public:
	virtual void OnInitialUpdate();
	protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CFormStatus();
private:
    int m_nCnt;

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CFormStatus)
    afx_msg void OnButtonClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEW_H__AB040AC1_932D_4990_A575_32EABE933B59__INCLUDED_)
