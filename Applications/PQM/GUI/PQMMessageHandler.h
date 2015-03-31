//TMSC-REDMINE-780
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMMessageHandler.h
 *  Overview: Structure of CPQMMessageHandler class.
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
#ifndef __SM_PQM_MESSAGE_HANDLER_H__
#define __SM_PQM_MESSAGE_HANDLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPQMMessageHandler
{
public:
    CPQMMessageHandler();
    virtual ~CPQMMessageHandler();

private:

    virtual BOOL ReceiveMessage(const pqm_msg_tag Msg)const = 0;
};

#endif // #ifndef __SM_PQM_MESSAGE_HANDLER_H__
