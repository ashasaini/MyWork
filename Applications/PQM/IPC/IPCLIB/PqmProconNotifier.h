//Redmine-781_CPP_TestTool
//Redmine-782
// PqmProconNotifier.h: interface for the CPqmProconNotifier class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMPROCON_NOTIFIER_H__
#define __SM_IPCLIB_PQMPROCON_NOTIFIER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PQMNotifier.h"
#include "SocketMsgListThreadSafe.h"

class CPqmProcon;

class CPqmProconNotifier : public CPQMNotifier
{

public:

    explicit CPqmProconNotifier(CPqmProcon*   f_parent_pqmprocon);
    virtual ~CPqmProconNotifier();

protected:

    // call back function for receiving any message from AcqMan socket
    void OnReceive(uint f_socket_id, CSocketMsg& f_socket_msg);//+Patni-NP/2010Apr26/Added/MaFT MSA251-204

    // call back function after disconnecting from AcqMan socket
    void OnDisconnect(uint f_socket_id);

private:

    CPqmProcon*   m_parent_pqmprocon;

    CSocketMsgListThreadSafe m_socketmsg_list;
    HANDLE m_hthread;
    DWORD  m_dthreadid;

    static DWORD  WINAPI OnReceiveThreadProc(LPVOID lpParameter);
};
#endif // #ifndef __SM_IPCLIB_PQMPROCON_NOTIFIER_H__ 
