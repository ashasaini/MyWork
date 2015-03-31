//TMSC-REDMINE-781
//TMSC-REDMINE-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ControlStateMgr.cpp
 *  Overview: Implementation of CControlStateMgr class.
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
#include "ControlStateMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct SGUIControlMessage {
    pqm_msg_tag m_msg;
    int m_control_id;
    PQMControlState m_state;
};

static SGUIControlMessage pqmControlMap[] = {

    // when scan start button is pressed, prescan is going on and status is current
    {PQM_MSG_STATUS_CURRENT, IDC_SCAN_START, STATE_DISABLE},

    {PQM_MSG_STATUS_CURRENT, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE},//visible+disable
    {PQM_MSG_STATUS_CURRENT, IDC_ABORT, STATE_ENABLE},

    //when prescan is over, actual scan is going on and status is current
    {PQM_MSG_SCAN_START, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_SCAN_START, IDC_PAUSE_RESUME, STATE_VISIBLE_ENABLE},//visible+enable

    {PQM_MSG_SCAN_START, IDC_ABORT, STATE_ENABLE},
    {PQM_MSG_SCAN_START, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_SCAN_START, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    {PQM_MSG_SCAN_START, IDC_BUTTON_CFA_RETRY, STATE_DISABLE},
    {PQM_MSG_SCAN_START, ID_WIRELESS_OK, STATE_DISABLE},

    //when scan is completed,status is Done and selection is on
    //Done protocol
    {PQM_MSG_STATUS_DONE, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE},//visible+disable

    {PQM_MSG_STATUS_DONE, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_STATUS_DONE, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_STATUS_DONE, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    {PQM_MSG_STATUS_DONE, IDC_BUTTON_CFA_RETRY, STATE_ENABLE},
    {PQM_MSG_STATUS_DONE, ID_WIRELESS_OK, STATE_ENABLE},

    // when abort_wait msg received, change status to wait
    // (APC Retry is enabled for Protocol after Done/Failed
    // and APC Pause is enabled for first wait protocol
    // so not considered state of APC Retry and APC Pause here)

    {PQM_MSG_ABORT_WAIT, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_ABORT_WAIT, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE}, //visible+disable
    {PQM_MSG_ABORT_WAIT, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_ABORT_WAIT, IDC_BUTTON_CFA_RETRY, STATE_ENABLE},
    {PQM_MSG_ABORT_WAIT, ID_WIRELESS_OK, STATE_ENABLE},

    {PQM_MSG_APC_RETRY_WAIT, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_APC_RETRY_WAIT, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE}, //visible+disable
    {PQM_MSG_APC_RETRY_WAIT, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_APC_RETRY_WAIT, IDC_BUTTON_CFA_RETRY, STATE_ENABLE},
    {PQM_MSG_APC_RETRY_WAIT, ID_WIRELESS_OK, STATE_ENABLE},

    // Pause done
    {PQM_MSG_PAUSE_DONE, IDC_SCAN_START, STATE_VISIBLE_ENABLE},

    {PQM_MSG_PAUSE_DONE, IDC_PAUSE_RESUME, STATE_VISIBLE_ENABLE}, //visible+enable

    {PQM_MSG_PAUSE_DONE, IDC_ABORT, STATE_ENABLE},
    {PQM_MSG_PAUSE_DONE, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_PAUSE_DONE, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    {PQM_MSG_PAUSE_DONE, IDC_BUTTON_CFA_RETRY, STATE_DISABLE},
    {PQM_MSG_PAUSE_DONE, ID_WIRELESS_OK, STATE_DISABLE},

    // Resume done
    {PQM_MSG_RESUME_DONE, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_RESUME_DONE, IDC_PAUSE_RESUME, STATE_VISIBLE_ENABLE}, //enable+visible
    {PQM_MSG_RESUME_DONE, IDC_ABORT, STATE_ENABLE},
    {PQM_MSG_RESUME_DONE, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_RESUME_DONE, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    {PQM_MSG_RESUME_DONE, IDC_BUTTON_CFA_RETRY, STATE_DISABLE},
    {PQM_MSG_RESUME_DONE, ID_WIRELESS_OK, STATE_DISABLE},

    // when scan is failed,status is failed and selection is on
    // failed protocol

    {PQM_MSG_ERROR, IDC_SCAN_START, STATE_VISIBLE_ENABLE},

    {PQM_MSG_ERROR, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE}, //visible+disable

    {PQM_MSG_ERROR, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_ERROR, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_ERROR, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    {PQM_MSG_ERROR, IDC_BUTTON_CFA_RETRY, STATE_ENABLE},
    {PQM_MSG_ERROR, ID_WIRELESS_OK, STATE_ENABLE},
    // Inject Time button condition


    {PQM_MSG_INJECT_TIME_VISIBLE, IDC_INJECT_TIMER_BUTTON, STATE_INVISIBLE},
    {PQM_MSG_INJECT_TIME_VISIBLE, IDC_INJECT_TIME_BUTTON, STATE_VISIBLE},
    {PQM_MSG_INJECT_TIME_INVISIBLE, IDC_INJECT_TIME_BUTTON, STATE_INVISIBLE},
    {PQM_MSG_INJECT_TIMER_DISPLAY, IDC_INJECT_TIMER_BUTTON, STATE_VISIBLE},
    {PQM_MSG_INJECT_TIMER_VISIBLE, IDC_INJECT_TIME_BUTTON, STATE_INVISIBLE},


    //added by Hemant
    {PQM_MSG_SCAN_START_BREATH_HOLD, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_SCAN_START_TRUE, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_SCAN_START_FALSE, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_SCAN_PAUSE_TRUE, IDC_PAUSE_RESUME, STATE_ENABLE},
    {PQM_MSG_SCAN_PAUSE_FALSE, IDC_PAUSE_RESUME, STATE_DISABLE},
    {PQM_MSG_SCAN_ABORT_TRUE, IDC_ABORT, STATE_ENABLE},
    {PQM_MSG_SCAN_ABORT_FALSE, IDC_ABORT, STATE_DISABLE},
    //Scan Begin
    {PQM_MSG_SCAN_BEGIN, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_SCAN_BEGIN, IDC_PAUSE_RESUME, STATE_VISIBLE_ENABLE}, //enable+visible

    {PQM_MSG_SCAN_BEGIN, IDC_ABORT, STATE_ENABLE},
    {PQM_MSG_SCAN_BEGIN, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_SCAN_BEGIN, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    {PQM_MSG_SCAN_BEGIN, IDC_BUTTON_CFA_RETRY, STATE_DISABLE},
    {PQM_MSG_SCAN_BEGIN, ID_WIRELESS_OK, STATE_DISABLE},
    {PQM_MSG_SCAN_END, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE}, //visible+disable

    {PQM_MSG_SCAN_END, IDC_ABORT, STATE_DISABLE},

    //Scan Pause
    {PQM_MSG_SCAN_PAUSE, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_SCAN_PAUSE, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE},//disable+visible

    //Scan Resume
    {PQM_MSG_SCAN_RESUME, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_SCAN_RESUME, IDC_PAUSE_RESUME, STATE_VISIBLE_ENABLE}, //enable+visible

    //Abort Fail
    {PQM_MSG_ABORT_FAIL, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE},//visible+disable

    //Added by Hemant On 8/18/2008 7:48:13 PM
    {PQM_MSG_ABORT_FAIL, IDC_ABORT, STATE_VISIBLE_DISABLE},//visible+disable

    {PQM_MSG_CFA_PRESCAN_BEGIN, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_CFA_PRESCAN_BEGIN, IDC_PAUSE_RESUME, STATE_DISABLE},
    {PQM_MSG_CFA_PRESCAN_BEGIN, IDC_ABORT, STATE_ENABLE},

    {PQM_MSG_CFA_PRESCAN_END, IDC_SCAN_START, STATE_DISABLE},
    {PQM_MSG_CFA_PRESCAN_END, IDC_PAUSE_RESUME, STATE_DISABLE},
    {PQM_MSG_CFA_PRESCAN_END, IDC_ABORT, STATE_DISABLE},

    {PQM_MSG_CFA_PRESCAN_END, IDC_BUTTON_CFA_RETRY, STATE_ENABLE},
    {PQM_MSG_CFA_PRESCAN_END, ID_WIRELESS_OK, STATE_ENABLE},
    // + LOKESH INTGR5 BUG FIX

    //+Patni-SS/Uncommented/2009-May-15/JFT#137
    {PQM_MSG_CFA_PRESCAN_ABORT, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_CFA_PRESCAN_ABORT, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_CFA_PRESCAN_COUCH, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_CFA_PRESCAN_COUCH, IDC_ABORT, STATE_DISABLE},
    //+Patni-HEMANT/ADDED On 3/27/2009 1:58:14 PM/ PSP /Internal defect

    {PQM_MSG_SCANDONE_SHIMMING, IDC_PAUSE_RESUME, STATE_VISIBLE_DISABLE},//visible+disable

    {PQM_MSG_SCANDONE_SHIMMING, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_SCANDONE_SHIMMING, ID_MENUITEM_APC_RETRY, STATE_DISABLE},
    {PQM_MSG_SCANDONE_SHIMMING, ID_MENUITEM_APC_PAUSE, STATE_DISABLE},
    //+Patni-HEMANT/Added On 27Apr2009//Ace ITC-3 Internal Defect Fixing
    {PQM_MSG_SCANDONE_SHIMMING, IDC_BUTTON_CFA_RETRY, STATE_ENABLE} ,
    //-Patni-HEMANT/ADDED On 3/27/2009 1:58:14 PM/ PSP /Internal defect
    {PQM_MSG_SCANDONE_SHIMMING, ID_WIRELESS_OK, STATE_ENABLE},

    {PQM_MSG_RESET_CURRENT_STATUS, IDC_SCAN_START, STATE_ENABLE},
    {PQM_MSG_RESET_CURRENT_STATUS, IDC_ABORT, STATE_DISABLE},
    {PQM_MSG_RESET_CURRENT_STATUS, IDC_PAUSE_RESUME, STATE_DISABLE}
    //-Patni-HEMANT/Added On 27Apr2009//Ace ITC-3 Internal Defect Fixing
};

//************************************Method Header**************************************
//Method Name:  CControlStateMgr
//Author:  PATNI/GP
//Purpose: Constructor
//*************************************************************************************
CControlStateMgr::CControlStateMgr(
)
{

}

//************************************Method Header**************************************
//Method Name:  ~CControlStateMgr
//Author:  PATNI/GP
//Purpose: Destructor
//*************************************************************************************
CControlStateMgr::~CControlStateMgr(
)
{
}

//************************************Method Header**************************************
//Method Name:  GetState
//Author:  PATNI/GP
//Purpose: Get State of control as per message and control id
//*************************************************************************************
PQMControlState CControlStateMgr::GetState(
    const pqm_msg_tag msg,
    const int control_id
)
{
    const int size = sizeof(pqmControlMap) / sizeof(SGUIControlMessage);

    for (int i = 0; i < size; i++) {

        if (msg == pqmControlMap[i].m_msg &&
            control_id == pqmControlMap[i].m_control_id)  {
            return pqmControlMap[i].m_state;
        }
    }

    return STATE_DEFAULT;
}
// - GUIControl