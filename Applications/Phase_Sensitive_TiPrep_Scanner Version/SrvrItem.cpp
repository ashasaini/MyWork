// SrvrItem.cpp : implementation of the CTiPrepSrvrItem class
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
// CTiPrepSrvrItem implementation

IMPLEMENT_DYNAMIC(CTiPrepSrvrItem, CDocObjectServerItem)

CTiPrepSrvrItem::CTiPrepSrvrItem(CTiPrepDoc* pContainerDoc)
    : CDocObjectServerItem(dynamic_cast<COleServerDoc*>(pContainerDoc), TRUE)
{
    //  (eg, adding additional clipboard formats to the item's data source)
}

CTiPrepSrvrItem::~CTiPrepSrvrItem()
{
    // TODO: add cleanup code here
}

/*************************************************************
* Method:       Serialize
* Description:  This function is used
* Parameter:    CArchive & ar
* Returns:      void
*************************************************************/
void CTiPrepSrvrItem::Serialize(CArchive& ar)
{
    // CTiPrepSrvrItem::Serialize will be called by the framework if
    //  the item is copied to the clipboard.  This can happen automatically
    //  through the OLE callback OnGetClipboardData.  A good default for
    //  the embedded item is simply to delegate to the document's Serialize
    //  function.  If you support links, then you will want to serialize
    //  just a portion of the document.
    if(!IsLinkedItem()) {
        CTiPrepDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        pDoc->Serialize(ar);
    }
}

/*************************************************************
* Method:       OnGetExtent
* Description:  This function is used
* Parameter:    DVASPECT dwDrawAspect
* Parameter:    CSize & rSize
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
    // Most applications, like this one, only handle drawing the content
    //  aspect of the item.  If you wish to support other aspects, such
    //  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
    //  implementation of OnGetExtent should be modified to handle the
    //  additional aspect(s).
    if(dwDrawAspect != DVASPECT_CONTENT) {
        return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);
    }

    // CTiPrepSrvrItem::OnGetExtent is called to get the extent in
    //  HIMETRIC units of the entire item.  The default implementation
    //  here simply returns a hard-coded number of units.
    CTiPrepDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: replace this arbitrary size
    rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC units
    return TRUE;
}

/*************************************************************
* Method:       OnDraw
* Description:  This function is used
* Parameter:    CDC * pDC
* Parameter:    CSize & rSize
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
    // Remove this if you use rSize
    UNREFERENCED_PARAMETER(rSize);
    CTiPrepDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: set mapping mode and extent
    //  (The extent is usually the same as the size returned from OnGetExtent)
    pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowOrg(0, 0);
    pDC->SetWindowExt(3000, 3000);
    // TODO: add drawing code here.  Optionally, fill in the HIMETRIC extent.
    //  All drawing takes place in the metafile device context (pDC).
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTiPrepSrvrItem diagnostics

#ifdef _DEBUG
/*************************************************************
* Method:       AssertValid
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepSrvrItem::AssertValid() const
{
    CDocObjectServerItem::AssertValid();
}

/*************************************************************
* Method:       Dump
* Description:  This function is used
* Parameter:    CDumpContext & dc
* Returns:      void
*************************************************************/
void CTiPrepSrvrItem::Dump(CDumpContext& dc) const
{
    CDocObjectServerItem::Dump(dc);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////