// EPIDoc.cpp : implementation of the CEPIDoc class
//

#include "stdafx.h"
#include "EPI.h"

#include "EPIDoc.h"
#include "EPIController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEPIDoc

IMPLEMENT_DYNCREATE(CEPIDoc, CDocument)

BEGIN_MESSAGE_MAP(CEPIDoc, CDocument)
    //{{AFX_MSG_MAP(CEPIDoc)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEPIDoc construction/destruction

CEPIDoc::CEPIDoc()
{
    // TODO: add one-time construction code here
}

CEPIDoc::~CEPIDoc()
{
}

BOOL CEPIDoc::OnNewDocument()
{
    if(!CDocument::OnNewDocument()) {
        return FALSE;
    }

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEPIDoc serialization

void CEPIDoc::Serialize(CArchive& ar)
{
    if(ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CEPIDoc diagnostics

#ifdef _DEBUG
void CEPIDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CEPIDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEPIDoc commands
void CEPIDoc::OnFileSave()
{
    (CEPIController::GetInstance())->saveOriginalRawData();
}
