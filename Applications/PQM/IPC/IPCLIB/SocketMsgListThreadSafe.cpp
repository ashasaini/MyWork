// SocketMsgListThreadSafe.cpp: implementation of the CSocketMsgListThreadSafe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketMsgListThreadSafe.h"
#include <libStudyManager/Mutex/ScopedLock.h>
#include <Pcommlib/SocketMsg.h>

using namespace MR::ACQ::SM::COMMON;
using namespace MR::ACQ::SM::COMMON::MUTEX;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketMsgListThreadSafe::CSocketMsgListThreadSafe()
{

}

CSocketMsgListThreadSafe::~CSocketMsgListThreadSafe()
{

    UTIL::DeleteAndClearMFCList<CSocketMsg>(&m_socketmsg_list);
}



//****************************Method Header************************************
//Method Name   : PushMsg()
//Author        : PATNI\Sribanta
//Purpose       : Push message to list synchronizationally
//*****************************************************************************
void CSocketMsgListThreadSafe::PushMsg(
    const CSocketMsg& f_msg
)
{
    CScopedLock l_scoped_lock(&m_cs);
    CSocketMsg* l_socket_message = new CSocketMsg(f_msg) ;

    if (l_socket_message) {
        m_socketmsg_list.AddHead(l_socket_message);
    }
}



//****************************Method Header************************************
//Method Name   : PopMsg()
//Author        : PATNI\Sribanta
//Purpose       : Pop message from list synchronizationally
//*****************************************************************************
bool CSocketMsgListThreadSafe::PopMsg(
    CSocketMsg** f_msg
)
{
    bool l_return_val = false;

    CScopedLock l_scoped_lock(&m_cs);

    if (!m_socketmsg_list.IsEmpty()) {
        CSocketMsg* l_socketmsg_list = m_socketmsg_list.GetTail();

        if (l_socketmsg_list) {

            (*f_msg) = new CSocketMsg(*l_socketmsg_list);
            DEL_PTR(l_socketmsg_list);
            m_socketmsg_list.RemoveTail();

            l_return_val = true;
        }
    }

    return l_return_val;
}
