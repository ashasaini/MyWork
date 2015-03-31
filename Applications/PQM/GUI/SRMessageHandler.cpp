//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: SRMessageHandler.cpp
 *  Overview: Implementation of CSRMessageHandler class.
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
#include "SRMessageHandler.h"
#include <libStudyManager/CFW/CFWReceiveMsg.h>
#include <libStudyManager/CFW/CFWReplyMsg.h>
#include "PQMView.h"
#include "PQMUtility.h"
#include "DPSManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWListener;

extern CPQMView* g_MainView;

//************************************Method Header**************************************
//Method Name:  CSRMessageHandler
//Author:  PATNI
//Purpose: Constructor
//**************************************************************************************/
CSRMessageHandler::CSRMessageHandler(
)
{
}

//************************************Method Header**************************************
//Method Name:  ~CSRMessageHandler
//Author:  PATNI
//Purpose: Destructor
//**************************************************************************************/
CSRMessageHandler::~CSRMessageHandler(
)
{
}


//************************************Method Header**************************************
//Method Name:  ReceiveMessage
//Author:  PATNI
//Purpose: Receiving and processing of messages from SR
//**************************************************************************************/
BOOL CSRMessageHandler::ReceiveMessage(
    const pqm_msg_tag Msg
)const
{

    LPCTSTR FUNC_NAME = _T("CSRMessageHandler::ReceiveMessage") ;
    PQM_TRACE_SCOPED(FUNC_NAME);

    CCFWReceiveMsg  l_recv_msg ;

    switch (Msg) {



        case PQM_MSG_GL_SR_SQSAVED : //"GL_SR_SQSaved")))>=0)

            CDPSManager::GetInstance()->SendASyncMsg(_T("Schedule.SRGL"),
                    _T("GL_SR_SQSaved"), g_MainView->GetSRParamInfo());

            break;

        case PQM_MSG_GL_EXAMUPDATE :

            g_MainView->SendUpdateCurrentExamData();
            break;

        default:
            return FALSE;
    }

    return TRUE;
}



