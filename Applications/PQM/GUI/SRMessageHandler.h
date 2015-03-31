//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: SRMessageHandler.h
 *  Overview: Structure of CSRMessageHandler class.
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
#ifndef __SM_SR_MESSAGE_HANDLER_H__
#define __SM_SR_MESSAGE_HANDLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/CFW/CFWHandler.h>
#include <libStudyManager/CFW/CFWListener.h>
#include "PQMMessageHandler.h"


class CSRMessageHandler : public CPQMMessageHandler,
    private MR::ACQ::SM::COMMON::NonCopyable<CSRMessageHandler>

{
public:
    CSRMessageHandler();
    virtual ~CSRMessageHandler();

    virtual BOOL ReceiveMessage(const pqm_msg_tag Msg)const;

};

#endif // #ifndef __SM_SR_MESSAGE_HANDLER_H__
