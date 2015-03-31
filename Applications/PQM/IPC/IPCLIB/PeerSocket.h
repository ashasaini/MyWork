//Redmine-783
//Redmine-782
// PeerSocket.h: interface for the CPeerSocket class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PEERSOCKET_H__
#define __SM_IPCLIB_PEERSOCKET_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libproMsg/proMsg.h>
#include <PCommLib/IPToPNotifier.h>
#include <libStudyManager/NonCopyable.h>

class CPToPSocket;
class CPqmProbeServer;
class CPQMProConAdapter;

class CPeerSocket : public IPToPNotifier
    , private MR::ACQ::SM::COMMON::NonCopyable<CPeerSocket>
{
public:
    //Constructor / Destructor
    CPeerSocket();
    explicit CPeerSocket(CPqmProbeServer* f_socket_server);
    virtual ~CPeerSocket();

    void SendData(proMsg_t* f_prob_msg);
    //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
    bool IsSocketAlive(void);

    CPToPSocket* GetSocket();

private:

    CPToPSocket* m_socket;
    CPQMProConAdapter* m_probe_adapter;
    CPqmProbeServer* m_socket_server;
    //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
    bool m_alive;

    virtual void OnConnect();
    virtual void OnSend();
    virtual void OnShutDown(uint socket_id);
    virtual void OnPToPError(int error_code);
    virtual void OnDisconnect(uint socket_id);
    virtual void OnReceive(uint socket_id, CSocketMsg& msg);//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
};
#endif // #ifndef __SM_IPCLIB_PEERSOCKET_H__ 
