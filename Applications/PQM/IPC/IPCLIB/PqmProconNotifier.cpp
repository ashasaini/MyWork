//Redmine-781_CPP_TestTool
//Redmine-778
// PqmProconNotifier.cpp: implementation of the CPqmProconNotifier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmProconNotifier.h"
#include <libStudyManager/Mutex/ScopedLock.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PQMIPCManager.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "PqmProcon.h"
#include <PErrorHandler/ExceptionHandler.h>

using namespace MR::ACQ::SM::COMMON;
using namespace MR::ACQ::SM::COMMON::MUTEX;

//*******************************Method Header*********************************
//Method Name    :CPqmProconNotifier()
//Author         :PATNI
//Purpose        :Constructor
//*****************************************************************************
CPqmProconNotifier::CPqmProconNotifier(
    CPqmProcon* f_parent_pqmprocon
) : m_parent_pqmprocon(f_parent_pqmprocon
                          )
{
    LPCTSTR FUNC_NAME = _T("CPqmProconNotifier::CPqmProconNotifier");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProconNotifier::CPqmProconNotifier"));

    m_hthread = CreateThread(NULL, 0, CPqmProconNotifier::OnReceiveThreadProc, this, 0, &m_dthreadid);

}

//*******************************Method Header*********************************
//Method Name    :~CAcqManNotifier()
//Author         :PATNI
//Purpose        :Destructor
//*****************************************************************************
CPqmProconNotifier::~CPqmProconNotifier(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProconNotifier::~CPqmProconNotifier");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProconNotifier::~CPqmProconNotifier"));

    if (m_hthread) {
        TerminateThread(m_hthread, NULL);
        CloseHandle(m_hthread);
        m_hthread = NULL;
        m_dthreadid = -1;
    }
}

//****************************Method Header************************************
//Method Name   : OnReceive()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmProconNotifier::OnReceive(
    uint f_socket_id,
    CSocketMsg& f_socket_msg
)//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
{
    LPCTSTR FUNC_NAME = _T("CPqmProconNotifier::OnReceive");

    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_socket_id);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_socketmsg_list.PushMsg(f_socket_msg);
    PostThreadMessage(m_dthreadid, PROCON_MESSAGE, NULL, NULL);
}

//****************************Method Header************************************
//Method Name   : OnDisconnect()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmProconNotifier::OnDisconnect(
    uint f_socket_id
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProconNotifier::OnDisconnect");
    UNREFERENCED_PARAMETER(f_socket_id);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPqmProconNotifier::OnDisconnect"));

    if (m_parent_pqmprocon) {
        m_parent_pqmprocon->OnProconDisconnect();
    }
}



DWORD WINAPI CPqmProconNotifier::OnReceiveThreadProc(LPVOID lpParameter)
{
    __try {
        CPqmProconNotifier* l_parent_pqmprocon = (CPqmProconNotifier*)(lpParameter);
        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0)) {
            if (PROCON_MESSAGE == msg.message) {

                CSocketMsg* l_socketmsg = NULL;

                if (l_parent_pqmprocon->m_socketmsg_list.PopMsg(&l_socketmsg)) {
                    l_parent_pqmprocon->m_parent_pqmprocon->OnProconMessage(*l_socketmsg);
                    DEL_PTR(l_socketmsg);
                }
            }
        }
    }
    __except(RecordExceptionInfo(GetExceptionInformation(),
                                 _T("CPqmProconNotifier::OnReceiveThreadProc"))) {

    }


    ExitThread(0);
    return 0;
}

