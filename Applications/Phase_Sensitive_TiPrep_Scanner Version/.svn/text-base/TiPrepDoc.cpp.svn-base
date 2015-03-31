// TiPrepDoc.cpp : implementation of the CTiPrepDoc class
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrep.h"

#include "TiPrepDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc

IMPLEMENT_DYNCREATE(CTiPrepDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CTiPrepDoc, COleServerDoc)
    //{{AFX_MSG_MAP(CTiPrepDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc construction/destruction

CTiPrepDoc::CTiPrepDoc()
{
    // Use OLE compound files
    EnableCompoundFile();
    // TODO: add one-time construction code here
}

CTiPrepDoc::~CTiPrepDoc()
{
}

/*************************************************************
* Method:       OnNewDocument
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDoc::OnNewDocument()
{
    if(!COleServerDoc::OnNewDocument()) {
        return FALSE;
    }

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc server implementation

/*************************************************************
* Method:       OnGetEmbeddedItem
* Description:  This function is used
* Returns:      COleServerItem*
*************************************************************/
COleServerItem* CTiPrepDoc::OnGetEmbeddedItem()
{
    // OnGetEmbeddedItem is called by the framework to get the COleServerItem
    //  that is associated with the document.  It is only called when necessary.
    CTiPrepSrvrItem* pItem = new CTiPrepSrvrItem(this);
    ASSERT_VALID(pItem);
    return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc Active Document server implementation

/*************************************************************
* Method:       GetDocObjectServer
* Description:  This function is used
* Parameter:    LPOLEDOCUMENTSITE pDocSite
* Returns:      CDocObjectServer *
*************************************************************/
CDocObjectServer *CTiPrepDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
    return new CDocObjectServer(this, pDocSite);
}



/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc serialization

/*************************************************************
* Method:       Serialize
* Description:  This function is used
* Parameter:    CArchive & ar
* Returns:      void
*************************************************************/
void CTiPrepDoc::Serialize(CArchive& ar)
{
    if(ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc diagnostics

#ifdef _DEBUG
/*************************************************************
* Method:       AssertValid
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDoc::AssertValid() const
{
    COleServerDoc::AssertValid();
}

/*************************************************************
* Method:       Dump
* Description:  This function is used
* Parameter:    CDumpContext & dc
* Returns:      void
*************************************************************/
void CTiPrepDoc::Dump(CDumpContext& dc) const
{
    COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDoc commands


///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////