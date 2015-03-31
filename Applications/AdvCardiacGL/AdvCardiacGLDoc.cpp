// AdvCardiacGLDoc.cpp : implementation of the CAdvCardiacGLDoc class
//

#include "stdafx.h"
#include "AdvCardiacGL.h"

#include "AdvCardiacGLDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLDoc

IMPLEMENT_DYNCREATE(CAdvCardiacGLDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CAdvCardiacGLDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CAdvCardiacGLDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLDoc construction/destruction

CAdvCardiacGLDoc::CAdvCardiacGLDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CAdvCardiacGLDoc::~CAdvCardiacGLDoc()
{
}

BOOL CAdvCardiacGLDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLDoc server implementation

COleServerItem* CAdvCardiacGLDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CAdvCardiacGLSrvrItem* pItem = new CAdvCardiacGLSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLDoc Active Document server implementation

CDocObjectServer *CAdvCardiacGLDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}



/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLDoc serialization

void CAdvCardiacGLDoc::Serialize(CArchive& ar)
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
// CAdvCardiacGLDoc diagnostics

#ifdef _DEBUG
void CAdvCardiacGLDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CAdvCardiacGLDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG
///////////////////////////////// END OF FILE /////////////////////////////////