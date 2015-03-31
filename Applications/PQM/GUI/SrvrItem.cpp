//TMSC-REDMINE-777

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: SrvrItem.cpp
 *  Overview: Implementation of CSrvrItem class.
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
#include "SrvrItem.h"
#include "PQM.h"
#include "PQMDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPQMSrvrItem, CDocObjectServerItem)

CPQMSrvrItem::CPQMSrvrItem(CPQMDoc* pContainerDoc)
    : CDocObjectServerItem(pContainerDoc, TRUE)
{
}

CPQMSrvrItem::~CPQMSrvrItem(
)
{
}

void CPQMSrvrItem::Serialize(
    CArchive& ar
)
{
    // CPQMSrvrItem::Serialize will be called by the framework if
    //  the item is copied to the clipboard.  This can happen automatically
    //  through the OLE callback OnGetClipboardData.  A good default for
    //  the embedded item is simply to delegate to the document's Serialize
    //  function.  If you support links, then you will want to serialize
    //  just a portion of the document.

    if (!IsLinkedItem()) {

        CPQMDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        pDoc->Serialize(ar);
    }
}

BOOL CPQMSrvrItem::OnDraw(
    CDC* pDC,
    CSize& rSize
)
{
    // Remove this if you use rSize
    UNREFERENCED_PARAMETER(rSize);

    CPQMDoc* pDoc = GetDocument();
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


BOOL CPQMSrvrItem::OnGetExtent(
    DVASPECT dwDrawAspect,
    CSize& rSize
)
{
    // Most applications, like this one, only handle drawing the content
    //  aspect of the item.  If you wish to support other aspects, such
    //  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
    //  implementation of OnGetExtent should be modified to handle the
    //  additional aspect(s).

    if (dwDrawAspect != DVASPECT_CONTENT)
        return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

    // CPQMSrvrItem::OnGetExtent is called to get the extent in
    //  HIMETRIC units of the entire item.  The default implementation
    //  here simply returns a hard-coded number of units.

    CPQMDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);


    rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC units

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPQMSrvrItem diagnostics

#ifdef _DEBUG
void CPQMSrvrItem::AssertValid() const
{
    CDocObjectServerItem::AssertValid();
}

void CPQMSrvrItem::Dump(CDumpContext& dc) const
{
    CDocObjectServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////

