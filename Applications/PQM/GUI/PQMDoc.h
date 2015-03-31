//Redmine-783
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMDoc.h
 *  Overview: Structure of CPQMDoc class.
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
#ifndef __SM_PQM_DOC_H__
#define __SM_PQM_DOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <libStudyManager/NonCopyable.h>

class CPQMSrvrItem;

class CPQMDoc : public COleServerDoc,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMDoc>
{
public:
    virtual ~CPQMDoc();
    virtual void Serialize(CArchive& ar);

protected:
    CPQMDoc();

    //{{AFX_VIRTUAL(CPQMDoc)
    virtual COleServerItem* OnGetEmbeddedItem();
    //}}AFX_VIRTUAL

    virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

    // Generated message map functions
    //{{AFX_MSG(CPQMFormDoc)

    afx_msg void OnFileSave();
    afx_msg void OnFileSaveAs();
    //}}AFX_MSG

private:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPQMDoc)
    virtual BOOL OnNewDocument();

    virtual void OnShowDocument(BOOL bShow);
    //}}AFX_VIRTUAL

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    CPQMSrvrItem* GetEmbeddedItem() {
        return (CPQMSrvrItem*)COleServerDoc::GetEmbeddedItem();
    }

    DECLARE_DYNCREATE(CPQMDoc)
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_DOC_H__
