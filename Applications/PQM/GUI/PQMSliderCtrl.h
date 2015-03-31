//Redmine-783
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSliderCtrl.h
 *  Overview: Structure of CPQMSliderCtrl class.
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
#ifndef __SM_PQM_SLIDER_CTRL_H__
#define __SM_PQM_SLIDER_CTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// This class handles all slider events


#include <MPlusUICommon/SliderCtrlEx.h>
#include <libStudyManager/NonCopyable.h>

using namespace MPlus::UICommon ;

class CPQMSliderCtrl : public SliderCtrlEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMSliderCtrl>
{

    // Construction
public:
    CPQMSliderCtrl();
    virtual ~CPQMSliderCtrl();

    // Generated message map functions

protected:
    //{{AFX_MSG(CPQMSliderCtrl)
    afx_msg void OnKeyDown(UINT ip_cChar, UINT rep_cnt, UINT flags);
    //}}AFX_MSG

private :
    int m_old_pos;

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_SLIDER_CTRL_H__
