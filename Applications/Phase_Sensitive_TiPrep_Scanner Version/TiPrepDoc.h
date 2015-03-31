// TiPrepDoc.h : interface of the CTiPrepDoc class
//
/////////////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIPREPDOC_H__2E6DD8B7_A6B9_4729_A654_27ED618EB8CF__INCLUDED_)
#define AFX_TIPREPDOC_H__2E6DD8B7_A6B9_4729_A654_27ED618EB8CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTiPrepSrvrItem;

class CTiPrepDoc : public COleServerDoc
{
protected: // create from serialization only
    CTiPrepDoc();
    DECLARE_DYNCREATE(CTiPrepDoc)

// Attributes
public:
    CTiPrepSrvrItem* GetEmbeddedItem() {
        return (CTiPrepSrvrItem*)COleServerDoc::GetEmbeddedItem();
    }

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTiPrepDoc)
protected:
    virtual COleServerItem* OnGetEmbeddedItem();
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CTiPrepDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
    //{{AFX_MSG(CTiPrepDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPREPDOC_H__2E6DD8B7_A6B9_4729_A654_27ED618EB8CF__INCLUDED_)
