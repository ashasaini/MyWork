//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-779
//Redmine-782
// PeerSocket.cpp: implementation of the CPeerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PeerSocket.h"
//+Patni-NP/2010Apr26/Modified/MaFT MSA251-204
#include <Pcommlib/SocketFactory.h>
#include <Pcommlib/PToPSocket.h>
//-Patni-NP/2010Apr26/Modified/MaFT MSA251-204
#include "PqmProbe.h"
#include "PqmProbeServer.h"
#include "PQMProConAdapter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPeerSocket::CPeerSocket():
    m_probe_adapter(new CPQMProConAdapter()),
    m_socket_server(NULL),
    m_alive(true)
{
    m_socket = CSocketFactory::CreatePToPSocket(this, m_probe_adapter);

    if (m_socket == NULL) {
        //Log Trace
    } else {
    }
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
CPeerSocket::CPeerSocket(CPqmProbeServer* f_socket_server):
    m_probe_adapter(new CPQMProConAdapter()),
    m_socket_server(f_socket_server),
    m_alive(true) //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
{
    m_socket = CSocketFactory::CreatePToPSocket(this, m_probe_adapter);
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
CPeerSocket::~CPeerSocket(
)
{
    if (m_socket) {
        m_socket->ShutDown();
        m_socket->CloseConnection();
        DEL_PTR(m_socket);
    }

    m_alive = false; //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::SendData(
    proMsg_t* f_prob_msg
)
{
    f_prob_msg->timeStamp = clock();

    PLBYTE l_message_to_send = NULL;

    if (f_prob_msg->length) {
        l_message_to_send = new pbyte[sizeof(pbyte) * (PROMSG_HEAD_LEN + f_prob_msg->length)];

    } else {
        l_message_to_send = new pbyte[sizeof(pbyte) * PROMSG_HEAD_LEN];
    }

    if (l_message_to_send) {
        memset(l_message_to_send, 0, (PROMSG_HEAD_LEN + f_prob_msg->length));
        memcpy(l_message_to_send, &(f_prob_msg->code), PROMSG_HEAD_LEN);

        if (f_prob_msg->length) {
            memcpy((l_message_to_send + PROMSG_HEAD_LEN), f_prob_msg->data, f_prob_msg->length);
        }

        CSocketMsg l_message(l_message_to_send, (PROMSG_HEAD_LEN + f_prob_msg->length));

        if (m_socket != NULL) {
            m_socket->SendData(l_message);

            //delete byte array
            DEL_PTR_ARY(l_message_to_send);
        }
    }

    //-Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08
}

//+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
bool CPeerSocket::IsSocketAlive(void)
{
    return m_alive;
}
//-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC

CPToPSocket* CPeerSocket::GetSocket(
)
{
    return m_socket;
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::OnConnect(
)
{
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::OnSend(
)
{
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::OnShutDown(
    uint socket_id
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(socket_id);
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::OnPToPError(
    int error_code
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(error_code);
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::OnDisconnect(
    uint socket_id
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(socket_id);

    if (NULL != m_socket_server) {
        m_socket_server->GetPqmProbe()->OnProbeDisconnect();
    }

    //+Patni-RUP/2010Jan08/Close Porbe defect fix
    DEL_PTR(m_socket);

    //-Patni-RUP/2010Jan08/Close Probe defect fix
    m_alive = false;  //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
}

//******************************************************************************
// Function Name:-
// Author       :- PATNI
// Purpose      :-
//******************************************************************************
void CPeerSocket::OnReceive(
    uint socket_id,
    CSocketMsg& msg
)//Patni-NP/2010Apr26/Modified/MaFT MSA251-204
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(socket_id);

    if (NULL != m_socket_server) {
        m_socket_server->GetPqmProbe()->OnProbeMessage(msg);
    }
}

