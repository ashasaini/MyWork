// SrvrItem.h : interface of the CTiPrepSrvrItem class
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_SRVRITEM_H__0635E3B3_D49F_4250_9BF3_2160DE752B0D__INCLUDED_)
#define AFX_SRVRITEM_H__0635E3B3_D49F_4250_9BF3_2160DE752B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTiPrepSrvrItem : public CDocObjectServerItem
{
    DECLARE_DYNAMIC(CTiPrepSrvrItem)

// Constructors
public:
    CTiPrepSrvrItem(CTiPrepDoc* pContainerDoc);

// Attributes
    CTiPrepDoc* GetDocument() const {
        return (CTiPrepDoc*)CDocObjectServerItem::GetDocument();
    }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTiPrepSrvrItem)
public:
    virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
    virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
    //}}AFX_VIRTUAL

// Implementation
public:
    ~CTiPrepSrvrItem();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__0635E3B3_D49F_4250_9BF3_2160DE752B0D__INCLUDED_)
