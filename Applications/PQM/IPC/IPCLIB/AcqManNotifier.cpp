/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: AcqManNotifier.cpp
 *  Overview: Structure of CAcqManNotifier class.
 *  Last Modified: 2008/5/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni/LK         2008/5/10 12:00:00     Code Restructuring
 //                                                         After TA
 *****************************************************************************/

#include "stdafx.h"
#include "AcqManNotifier.h"
#include <PErrorHandler/ExceptionHandler.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PQMIPCManager.h>
#include "PqmPmAcqman.h"




//*******************************Method Header*********************************
//Method Name    :CAcqManNotifier()
//Author         :PATNI/LK
//Purpose        :Constructor
//*****************************************************************************
CAcqManNotifier::CAcqManNotifier(
    CPqmPmAcqman* f_parent_pqmpmacqman
) : m_parent_pqmpmacqman(f_parent_pqmpmacqman), m_pwndthread(NULL)
{
    //Patni-Sribanta/2011Jan06/Added/TMSC-REDMINE-1026
    m_hthread = CreateThread(NULL, 0, CAcqManNotifier::OnReceiveThreadProc, this, 0, &m_dthreadid);
}

//*******************************Method Header*********************************
//Method Name    :~CAcqManNotifier()
//Author         :PATNI/LK
//Purpose        :Destructor
//*****************************************************************************
CAcqManNotifier::~CAcqManNotifier(
)
{
    //+Patni-Sribanta/2011Jan06/Added/TMSC-REDMINE-1026
    if (m_hthread) {
        TerminateThread(m_hthread, NULL);
        CloseHandle(m_hthread);
        m_hthread = NULL;
        m_dthreadid = -1;
    }

    DEL_PTR(m_pwndthread);
    //-Patni-Sribanta/2011Jan06/Added/TMSC-REDMINE-1026
}

void CAcqManNotifier::AcqmanAbortFromnMainThread(
)
{
    LPCTSTR FUNC_NAME = _T("CAcqManNotifier::AcqmanAbortFromnMainThread");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    PostMessage(m_pwndthread->m_hWnd, ACQMAN_ABORT_FROM_GUI, NULL, NULL);
}


//****************************Method Header************************************
//Method Name   : OnReceive()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
void CAcqManNotifier::OnReceive(
    uint f_socket_id,
    CSocketMsg& f_socket_msg
)
{
    LPCTSTR FUNC_NAME = _T("CAcqManNotifier::OnReceive");
    UNREFERENCED_PARAMETER(f_socket_id);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-Sribanta/2011Jan06/Modified/TMSC-REDMINE-1026
    m_socketmsg_list.PushMsg(f_socket_msg);
    PostMessage(m_pwndthread->m_hWnd, ACQMAN_MESSAGE, NULL, NULL);
    //-Patni-Sribanta/2011Jan06/Modified/TMSC-REDMINE-1026

}

//****************************Method Header************************************
//Method Name   : OnDisconnect()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
void CAcqManNotifier::OnDisconnect(
    uint f_socket_id
)
{
    LPCTSTR FUNC_NAME = _T("CAcqManNotifier::OnDisconnect");
    UNREFERENCED_PARAMETER(f_socket_id);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_parent_pqmpmacqman) {
        m_parent_pqmpmacqman->OnAcqmanDisconnect();
    }
}

//****************************Method Header************************************
//Method Name   : CreateThreadWindow()
//Author        : iGATE
//Purpose       : To create Thread window.
//*****************************************************************************
void CAcqManNotifier::CreateThreadWindow()
{

    LPCTSTR FUNC_NAME = _T("CAcqManNotifier::CreateThreadWindow");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_pwndthread = new CThreadWnd();
    m_pwndthread->m_acqman_notifier = this;
    m_pwndthread->CreateEx(0, _T("STATIC"), _T(""), 0, CRect(0, 0, 20, 20), 0, 0);

}

//****************************Method Header************************************
//Method Name   : HandleThreadWndMessages()
//Author        : iGATE
//Purpose       : Habdles Thread window messages.
//*****************************************************************************
void CAcqManNotifier::HandleThreadWndMessages(
    const int f_message
)
{
    if (ACQMAN_MESSAGE == f_message) {

        CSocketMsg* l_socketmsg = NULL;

        if (m_socketmsg_list.PopMsg(&l_socketmsg)) {
            m_parent_pqmpmacqman->OnAcqmanMsg(*l_socketmsg);
            DEL_PTR(l_socketmsg);
        }

    } else if (ACQMAN_ABORT_FROM_GUI == f_message) {
        m_parent_pqmpmacqman->AcqmanAbortFromScanThread();
    }
}

//+Patni-Sribanta/2011Jan13/Modified/TMSC-REDMINE-1026
//****************************Method Header************************************
//Method Name   : OnReceiveThreadProc()
//Author        : PATNI/Sribanta
//Purpose       : Thread Procedure
//*****************************************************************************
DWORD WINAPI CAcqManNotifier::OnReceiveThreadProc(LPVOID lpParameter)
{
    LPCTSTR FUNC_NAME = _T("CAcqManNotifier::OnReceiveThreadProc");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    __try {

        CAcqManNotifier* l_acqman_notifier = (CAcqManNotifier*)(lpParameter);
        MSG msg;

        if (l_acqman_notifier) {

            l_acqman_notifier->CreateThreadWindow();

            while (GetMessage(&msg, NULL, 0, 0)) {

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

        } else
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_acqman_notifier is NULL"));

    } __except(RecordExceptionInfo(GetExceptionInformation(),
                                   _T("CAcqManNotifier::OnReceiveThreadProc()"))) {

    }

    ExitThread(0);
    return 0;
}
//-Patni-Sribanta/2011Jan13/Modified/TMSC-REDMINE-1026

CThreadWnd::CThreadWnd()
{
    m_acqman_notifier = NULL;
}

CThreadWnd::~CThreadWnd()
{
}

BEGIN_MESSAGE_MAP(CThreadWnd, CWnd)
    //{{AFX_MSG_MAP(CThreadWnd)
    ON_MESSAGE(ACQMAN_ABORT_FROM_GUI, OnAbortMessage)
    ON_MESSAGE(ACQMAN_MESSAGE, OnAcqManMessage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CThreadWnd::OnAbortMessage(WPARAM wParam, LPARAM lParam)
{
    m_acqman_notifier->HandleThreadWndMessages(ACQMAN_ABORT_FROM_GUI);
    return 0;
}

LRESULT CThreadWnd::OnAcqManMessage(WPARAM wParam, LPARAM lParam)
{
    m_acqman_notifier->HandleThreadWndMessages(ACQMAN_MESSAGE);
    return 0;

}