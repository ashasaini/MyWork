// KtReconDoc.cpp : implementation of the CKtReconDoc class
//

#include "stdafx.h"
#include "KtRecon.h"

#include "KtReconDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc

IMPLEMENT_DYNCREATE(CKtReconDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CKtReconDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CKtReconDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc construction/destruction

CKtReconDoc::CKtReconDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CKtReconDoc::~CKtReconDoc()
{
}

BOOL CKtReconDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc server implementation

COleServerItem* CKtReconDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CKtReconSrvrItem* pItem = new CKtReconSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc Active Document server implementation

CDocObjectServer *CKtReconDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}



/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc serialization

void CKtReconDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc diagnostics

#ifdef _DEBUG
void CKtReconDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CKtReconDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKtReconDoc commands
/////////////////////////////////END OF FILE /////////////////////////////////////