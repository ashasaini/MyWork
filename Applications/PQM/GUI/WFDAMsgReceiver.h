// WFDAMsgReceiver.h: interface for the CWFDAMsgReceiver class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_WFDA_MSGRECEIVER_H__
#define __SM_WFDA_MSGRECEIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <cfw/cfw.h>
#include <libStudyManager/CFW/CFWHandler.h>
#include <libStudyManager/CFW/CFWListener.h>

#include "WFDAConstants.h"

class CWFDAHandlerInterface;


class CWFDAMsgReceiver : public MR::ACQ::SM::COMMON::CFW::ICFWHandler
{

public:
    static void CreateWFDAMsgReceiver(CWFDAHandlerInterface* f_wfda_handlerinterface);
    static void DeleteWFDAMsgReceiver();
    static CWFDAMsgReceiver* GetWFDAMsgReceiver();

    virtual ~CWFDAMsgReceiver();

    BOOL RecvCallBack(const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
                      MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg* const replyMsg,
                      MR::ACQ::SM::COMMON::CFW::CCFWListener* const listener);

    static bool PrepareWFDAMessageDataFor(
        const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
        WFDAMessageData& f_wfdamessagedata);

private :

    static CWFDAMsgReceiver* m_wfdamsgreceiver;
    CWFDAHandlerInterface* m_wfda_handlerinterface;

    CWFDAMsgReceiver();
    CWFDAMsgReceiver(CWFDAHandlerInterface* f_wfda_handlerinterface);
};




#endif // #ifndef __SM_WFDA_MSGRECEIVER_H__


