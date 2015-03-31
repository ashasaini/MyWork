//Redmine-783
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-782
// PqmProbeServer.cpp: implementation of the CPqmProbeServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmProbeServer.h"
//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <Pcommlib/ListnerSocket.h>
#include <Pcommlib/PToPSocket.h>
#include <Pcommlib/SocketFactory.h>
//-Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <PQM/PQMLogMgr.h>
#include "PeerSocket.h"
#include "PqmProbe.h"
#include "PqmProcon.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPqmProbeServer::CPqmProbeServer():
    m_pro_client(NULL),
    m_pqmprobe(NULL),
    m_pqmprocon(NULL)
{
    LPCTSTR FUNC_NAME = _T("CPqmProbeServer::CPqmProbeServer");

    m_socket = CSocketFactory::CreateListnerSocket(this);

    if (NULL == m_socket) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_socket is NULL"));
    }
}

CPqmProbeServer::CPqmProbeServer(CPqmProbe* f_pqmprobe):
    m_pro_client(NULL),
    m_pqmprobe(f_pqmprobe)
{
    LPCTSTR FUNC_NAME = _T("CPqmProbeServer::CPqmProbeServer");

    m_socket = CSocketFactory::CreateListnerSocket(this);

    if (NULL == m_socket) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_socket is NULL"));
    }

    if (!m_socket->Create(6490, true)) {
        OutputDebugString(_T("CPqmProbeServer creation successful"));

    } else {
        if (!m_socket->Listen()) {
            //Error
        } else {
            OutputDebugString(_T("CPqmProbeServer server listen successful"));
        }
    }
}

CPqmProbeServer::~CPqmProbeServer()
{
    LPCTSTR FUNC_NAME = _T("CPqmProbeServer::~CPqmProbeServer");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProbeServer::~CPqmProbeServer"));

    if (m_socket) {
        m_socket->ShutDown();
        DEL_PTR(m_socket);
    }

    DEL_PTR(m_pro_client);
}

CPqmProbe* CPqmProbeServer::GetPqmProbe()const
{
    return m_pqmprobe;
}

CPeerSocket* CPqmProbeServer::GetPeerSocket()const
{
    return m_pro_client;
}

/////////////////////////////////////////////////////////////////
//Method OnNewConnection
//Accept new connection from client
/////////////////////////////////////////////////////////////////
void CPqmProbeServer::OnNewConnection(SOCKET_EX soc_handle)
{
    LPCTSTR FUNC_NAME = _T("CPqmProbeServer::OnNewConnection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProbeServer::OnNewConnection"));

    DEL_PTR(m_pro_client);

    OutputDebugString(_T("CPqmProbeServer::OnNewConnection"));
    m_pro_client = new CPeerSocket(this);

    if (m_pro_client != NULL) {
        m_pro_client->GetSocket()->SetSocketHandle(soc_handle);

        m_pqmprocon = m_pqmprobe->GetPqmProCon();

        if (m_pqmprocon->GetProbeSockEventHandle() != NULL)
            SetEvent(m_pqmprocon->GetProbeSockEventHandle());

    } else {
        //Log Trace
    }
}

void CPqmProbeServer::OnListnerError(int error_code)
{
    _tprintf(_T("CPqmProbeServer::OnListnerError(%d)\n"), error_code);
}

