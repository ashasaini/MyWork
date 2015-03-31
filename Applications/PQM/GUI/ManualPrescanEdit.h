//TMSC-REDMINE-783
//Redmine-775
//Redmine-777
#ifndef __SM_MANUAL_PRESCAN_EDIT_H__
#define __SM_MANUAL_PRESCAN_EDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualPrescanEdit.h : header file
//

#include <MPlusUICommon/EditCtrlEx.h>
#include <libStudyManager/NonCopyable.h>

using namespace MPlus::UICommon ;


/////////////////////////////////////////////////////////////////////////////
// CManualPrescanEdit window

class CManualPrescanEdit : public EditCtrlEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CManualPrescanEdit>
{
public:
    CManualPrescanEdit();
    virtual ~CManualPrescanEdit();

    // Generated message map functions
protected:
    //{{AFX_MSG(CManualPrescanEdit)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnContextMenu(CWnd* pNewWnd , CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_MANUAL_PRESCAN_EDIT_H__
