#if !defined(AFX_INFOVIEW_H__AB040AC1_932D_4990_A575_32EABE933B59__INCLUDED_)
#define AFX_INFOVIEW_H__AB040AC1_932D_4990_A575_32EABE933B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormInfo : public CFormView
{
protected:
    CFormInfo();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFormInfo)

// Form Data
public:
    //{{AFX_DATA(CFormInfo)
    enum { IDD = IDD_FORM_INFO };
    CStatic m_ctrlImgInfo;
    //}}AFX_DATA

// Attributes
public:
    int setErrorMessage(CString &f_csErrorMsg);
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormInfo)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CFormInfo();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CFormInfo)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEW_H__AB040AC1_932D_4990_A575_32EABE933B59__INCLUDED_)
