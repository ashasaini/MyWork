// WFDAMsgSender.h: interface for the CWFDAMsgSender class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __SM_WFDA_MSGSENDER_H__
#define __SM_WFDA_MSGSENDER_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WFDAHelper.h"
#include <libStudyManager/CFW/CFWHandler.h>
#include <libStudyManager/CFW/CFWListener.h>


class CWFDAMsgSender : private MR::ACQ::SM::COMMON::NonCopyable<CWFDAMsgSender>
{

public:
    CWFDAMsgSender();
    virtual ~CWFDAMsgSender();

    BOOL SendWFDAMessage(
        const WFDAMessageData& f_wfdamessagedata,
        CString& f_error_string,
        const bool f_sendsync = false,
        WFDAMessageData* f_wfdamessage_outdata = NULL,
        const int f_timeout = 10000) const;

private :

    bool PrepareWFDAMessageDataFor(
        const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
        WFDAMessageData& f_wfdamessagedata) const;
};

#endif // #ifndef __SM_WFDA_MSGSENDER_H__

