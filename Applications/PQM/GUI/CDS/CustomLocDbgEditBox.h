// CustomLocDbgEditBox.h: Declaration of the CCustomLocDbgEditBox class.
///////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __SM_CUSTOM_LOC_DBG_EDITBOX_H__
#define __SM_CUSTOM_LOC_DBG_EDITBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCustomLocDbgEditBox.h : header file
//

#include <MPlusUICommon/EditCtrlEx.h>
#include <libStudyManager/NonCopyable.h>

using namespace MPlus::UICommon ;

// Edit window class.
class CCustomLocDbgEditBox : public EditCtrlEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CCustomLocDbgEditBox>
{
public:
    CCustomLocDbgEditBox();
    virtual ~CCustomLocDbgEditBox();

    // Generated message map functions
protected:
    //{{AFX_MSG(CCustomLocDbgEditBox)

    afx_msg void OnContextMenu(CWnd* pNewWnd , CPoint point);

    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()


};

#endif // __SM_CUSTOM_LOC_DBG_EDITBOX_H__







