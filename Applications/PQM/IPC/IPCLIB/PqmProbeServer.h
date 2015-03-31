//Redmine-783
//Redmine-774
//Rdemine-780
//Redmine-782
// PqmProbeServer.h: interface for the CPqmProbeServer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMPROBE_SERVER_H__
#define __SM_IPCLIB_PQMPROBE_SERVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PCommLib/IListnerNotifier.h>
#include <libStudyManager/NonCopyable.h>

class CListnerSocket;
class CPeerSocket;
class CPqmProbe;
class CPqmProcon;

class CPqmProbeServer : public IListnerNotifier
    , private MR::ACQ::SM::COMMON::NonCopyable<CPqmProbeServer>
{

public:

    CPqmProbeServer();
    explicit CPqmProbeServer(CPqmProbe* f_pqmprobe);
    virtual ~CPqmProbeServer();

    CPqmProbe*   GetPqmProbe() const;
    CPeerSocket* GetPeerSocket() const;


private:

    CListnerSocket* m_socket;
    CPeerSocket*    m_pro_client;
    CPqmProbe*      m_pqmprobe;
    CPqmProcon*		m_pqmprocon;

    virtual void OnNewConnection(SOCKET_EX soc_handle);
    virtual void OnListnerError(int error_code);

};

#endif // #ifndef __SM_IPCLIB_PQMPROBE_SERVER_H__
