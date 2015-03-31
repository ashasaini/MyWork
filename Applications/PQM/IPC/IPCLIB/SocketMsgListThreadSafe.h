// SocketMsgListThreadSafe.h: interface for the CSocketMsgListThreadSafe class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_SOCKETMSGLISTTHREADSAFE_H__
#define __SM_IPCLIB_SOCKETMSGLISTTHREADSAFE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/Mutex/ThreadMutex.h>


class CSocketMsg;


class CSocketMsgListThreadSafe :
    private MR::ACQ::SM::COMMON::NonCopyable<CSocketMsgListThreadSafe>
{


public:
    CSocketMsgListThreadSafe();
    virtual ~CSocketMsgListThreadSafe();

    void PushMsg(const CSocketMsg& f_msg);
    bool PopMsg(CSocketMsg** f_msg);


private:

    typedef CList<CSocketMsg*, CSocketMsg*> CSocketMsgList;

    CSocketMsgList m_socketmsg_list;
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_cs;
};



#endif // #ifndef __SM_IPCLIB_SOCKETMSGLISTTHREADSAFE_H__
