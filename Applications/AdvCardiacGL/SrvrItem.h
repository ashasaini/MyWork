// SrvrItem.h : interface of the CAdvCardiacGLSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__FD41973A_CFEC_4C37_9172_B5A014E0F1D4__INCLUDED_)
#define AFX_SRVRITEM_H__FD41973A_CFEC_4C37_9172_B5A014E0F1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdvCardiacGLSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CAdvCardiacGLSrvrItem)

// Constructors
public:
	CAdvCardiacGLSrvrItem(CAdvCardiacGLDoc* pContainerDoc);

// Attributes
	CAdvCardiacGLDoc* GetDocument() const
		{ return (CAdvCardiacGLDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvCardiacGLSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CAdvCardiacGLSrvrItem();
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

#endif // !defined(AFX_SRVRITEM_H__FD41973A_CFEC_4C37_9172_B5A014E0F1D4__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////