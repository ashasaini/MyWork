//TMSC-REDMINE-777

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMDoc.cpp
 *  Overview: Implementation of CPQMDoc class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "stdafx.h"
#include "PQMDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// helper functions for reliable and small QueryInterface calls
LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID iid)
{
    ASSERT(lpUnknown != NULL);

    LPUNKNOWN lpW = NULL;

    if (lpUnknown->QueryInterface(iid, (void**)&lpW) != S_OK)
        return NULL;

    return lpW;
}

LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID riid);
#define QUERYINTERFACE(lpUnknown, iface) \
    (iface*)_AfxQueryInterface(lpUnknown, IID_##iface)


IMPLEMENT_DYNCREATE(CPQMDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CPQMDoc, COleServerDoc)
    //{{AFX_MSG_MAP(CPQMDoc)

    //+Patni-HEMANT/ADDED On 3/28/2009 12:41:00 PM/ PSP/ Internal Defect
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    //-Patni-HEMANT/ADDED On 3/28/2009 12:41:00 PM/ PSP/ Internal Defect
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPQMDoc construction/destruction


CPQMDoc::~CPQMDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPQMDoc serialization

void CPQMDoc::Serialize(
    CArchive& ar
)
{
    if (ar.IsStoring()) {

        // TODO: add storing code here
    } else  {
        // TODO: add loading code here
    }
}

//-------------------------------------------------------------------------
//+Patni-HEMANT/ADDED On 3/28/2009 12:41:26 PM/ PSP /Internal defect
//We do not need to process these function , so don't call base class version of it.
//this is done because, we should avoid saving a PQM document using CTL + S

CPQMDoc::CPQMDoc()
{
    // Use OLE compound files
    EnableCompoundFile();
}

/////////////////////////////////////////////////////////////////////////////
// CPQMDoc server implementation

COleServerItem* CPQMDoc::OnGetEmbeddedItem()
{
    // OnGetEmbeddedItem is called by the framework to get the COleServerItem
    //  that is associated with the document.  It is only called when necessary.

    CPQMSrvrItem* pItem = new CPQMSrvrItem(this);
    ASSERT_VALID(pItem);
    return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CPQMDoc Active Document server implementation
CDocObjectServer* CPQMDoc::GetDocObjectServer(
    LPOLEDOCUMENTSITE pDocSite
)
{
    return new CDocObjectServer(this, pDocSite);
}


void CPQMDoc::OnFileSave()
{
}

void CPQMDoc::OnFileSaveAs()
{
}
//
//-Patni-HEMANT/ ADDED On3/28/2009 12:41:29 PM/ PSP /Internal defect



BOOL CPQMDoc::OnNewDocument()
{
    if (!COleServerDoc::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPQMDoc commands

void CPQMDoc::OnShowDocument(
    BOOL bShow
)
{

    COleServerDoc::OnShowDocument(bShow);
    // attempt to get in-place client-site interface
    LPOLEINPLACESITE lpInPlaceSite = QUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);

    if (!lpInPlaceSite)
        return;     // unable to get in-place client site interface

    // we'll need the parent window to set as parent
    HWND hWnd = NULL;//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    VERIFY(lpInPlaceSite->GetWindow(&hWnd) == S_OK);
    CFrameWnd* pParentWnd = NULL;//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    pParentWnd = (CFrameWnd*) CWnd::FromHandle(hWnd);
    CFrameWnd* pFrame = NULL; //Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    pFrame = GetFirstFrame();

    if (pFrame != NULL) {
        CWnd* pFrame1 = (CFrameWnd*)pFrame->SetParent(pParentWnd);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CPQMDoc diagnostics
#ifdef _DEBUG
void CPQMDoc::AssertValid() const
{
    COleServerDoc::AssertValid();
}

void CPQMDoc::Dump(CDumpContext& dc) const
{
    COleServerDoc::Dump(dc);
}
#endif //_DEBUG









