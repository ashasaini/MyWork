// EPIDoc.h : interface of the CEPIDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPIDOC_H__A2873046_12FB_4D28_9BC3_602E8249D03D__INCLUDED_)
#define AFX_EPIDOC_H__A2873046_12FB_4D28_9BC3_602E8249D03D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEPIDoc : public CDocument
{
protected: // create from serialization only
    CEPIDoc();
    DECLARE_DYNCREATE(CEPIDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEPIDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CEPIDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CEPIDoc)
    afx_msg void OnFileSave();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPIDOC_H__A2873046_12FB_4D28_9BC3_602E8249D03D__INCLUDED_)
