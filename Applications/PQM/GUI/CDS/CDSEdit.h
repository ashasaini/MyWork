//TMSC-REDMINE-783
//Redmine-775
//Redmine-777
#ifndef __SM_CDS_EDIT_H__
#define __SM_CDS_EDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDSEdit.h : header file
//

#include <MPlusUICommon/EditCtrlEx.h>
#include <libStudyManager/NonCopyable.h>

using namespace MPlus::UICommon ;

/////////////////////////////////////////////////////////////////////////////
// CCDSEdit window

class CCDSEdit : public EditCtrlEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CCDSEdit>
{
public:
    CCDSEdit();
    virtual ~CCDSEdit();

    // Generated message map functions
protected:
    //{{AFX_MSG(CCDSEdit)
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_CDS_EDIT_H__
