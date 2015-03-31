//Redmine-783
//Redmine-782
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: SrvrItem.h
 *  Overview: Structure of CSrvrItem class.
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
#ifndef __SM_SRVR_ITEM_H__
#define __SM_SRVR_ITEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPQMDoc;

class CPQMSrvrItem : public CDocObjectServerItem,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMSrvrItem>
{
    // Constructors
public:
    explicit CPQMSrvrItem(CPQMDoc* pContainerDoc);
    ~CPQMSrvrItem();

protected:
    virtual void Serialize(CArchive& ar);   // overridden for document i/o

private:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPQMSrvrItem)
    virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
    virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
    //}}AFX_VIRTUAL

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    CPQMDoc* GetDocument() const {
        return (CPQMDoc*)CDocObjectServerItem::GetDocument();
    }

    DECLARE_DYNAMIC(CPQMSrvrItem)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_SRVR_ITEM_H__
