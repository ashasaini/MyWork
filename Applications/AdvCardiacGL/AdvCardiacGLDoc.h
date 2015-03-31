// AdvCardiacGLDoc.h : interface of the CAdvCardiacGLDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADVCARDIACGLDOC_H__7A867EC7_365A_4443_ABE8_E3F14169727E__INCLUDED_)
#define AFX_ADVCARDIACGLDOC_H__7A867EC7_365A_4443_ABE8_E3F14169727E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdvCardiacGLSrvrItem;

class CAdvCardiacGLDoc : public COleServerDoc
{
protected: // create from serialization only
	CAdvCardiacGLDoc();
	DECLARE_DYNCREATE(CAdvCardiacGLDoc)

// Attributes
public:
	CAdvCardiacGLSrvrItem* GetEmbeddedItem()
		{ return (CAdvCardiacGLSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvCardiacGLDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdvCardiacGLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
	//{{AFX_MSG(CAdvCardiacGLDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVCARDIACGLDOC_H__7A867EC7_365A_4443_ABE8_E3F14169727E__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////