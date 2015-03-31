//TMSC-REDMINE-783
//TMSC-Redmine-780
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMEdit.h
 *  Overview: Structure of CPQMEdit class.
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
#ifndef __SM_PQM_EDIT_H__
#define __SM_PQM_EDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//! The class is used to display edit control

//Added by Hemant On 6/4/2008 5:14:48 PM
#include <MPlusUICommon/EditCtrlEx.h>
using namespace MPlus::UICommon ;

//Modified by Hemant
class CPQMEdit : public EditCtrlEx
{
public:
    CPQMEdit();
    virtual ~CPQMEdit();
    bool ValidateStringWithDotAndMinusChars(const CString& str, const int valid_dot_pos = 3) const;

    // Generated message map functions
protected:
    //{{AFX_MSG(CPQMEdit)
    afx_msg void OnChar(UINT chara, UINT rep_cnt, UINT flags);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_EDIT_H__
