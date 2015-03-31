//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ControlStateMgr.h
 *  Overview: Structure of CControlStateMgr class.
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
#ifndef __SM_CONTROL_STATE_MGR_H__
#define __SM_CONTROL_STATE_MGR_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <libStudyManager/NonCopyable.h>

// ! This class is used to control state of GUI controls

typedef enum PQMControlState_Tag {
    STATE_DEFAULT = -1,
    STATE_ENABLE = 0,
    STATE_DISABLE,
    STATE_VISIBLE,
    STATE_INVISIBLE,
    STATE_VISIBLE_ENABLE,
    STATE_VISIBLE_DISABLE

} PQMControlState;

class CControlStateMgr : private MR::ACQ::SM::COMMON::NonCopyable<CControlStateMgr>
{

public:
    CControlStateMgr();
    virtual ~CControlStateMgr();

    static PQMControlState GetState(const pqm_msg_tag msg, const int control_id);

};

#endif  // #ifndef __SM_CONTROL_STATE_MGR_H__

// - GUIControl