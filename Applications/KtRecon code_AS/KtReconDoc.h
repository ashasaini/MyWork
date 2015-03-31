// KtReconDoc.h : interface of the CKtReconDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONDOC_H__604BABAA_FAB1_4AB8_923A_9F6686B0D766__INCLUDED_)
#define AFX_KTRECONDOC_H__604BABAA_FAB1_4AB8_923A_9F6686B0D766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CKtReconSrvrItem;

class CKtReconDoc : public COleServerDoc
{
protected: // create from serialization only
	CKtReconDoc();
	DECLARE_DYNCREATE(CKtReconDoc)

// Attributes
public:
	CKtReconSrvrItem* GetEmbeddedItem()
		{ return (CKtReconSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKtReconDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKtReconDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
	//{{AFX_MSG(CKtReconDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTRECONDOC_H__604BABAA_FAB1_4AB8_923A_9F6686B0D766__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////