// SrvrItem.h : interface of the CKtReconSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__719310AB_E2D4_454E_9DE7_DFB56466763F__INCLUDED_)
#define AFX_SRVRITEM_H__719310AB_E2D4_454E_9DE7_DFB56466763F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKtReconSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CKtReconSrvrItem)

// Constructors
public:
	CKtReconSrvrItem(CKtReconDoc* pContainerDoc);

// Attributes
	CKtReconDoc* GetDocument() const
		{ return (CKtReconDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKtReconSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CKtReconSrvrItem();
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

#endif // !defined(AFX_SRVRITEM_H__719310AB_E2D4_454E_9DE7_DFB56466763F__INCLUDED_)
