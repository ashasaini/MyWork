//Redmine-781_CPP_TestTool
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: AcqManNotifier.h
 *  Overview: Structure of CAcqManNotifier class.
 *  Last Modified: 2008/5/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni/LK            2008/5/10 12:00:00     Code Restructuring
 //                                                          After TA
 *****************************************************************************/
#ifndef __SM_IPCLIB_ACQMANNOTIFIER_H__
#define __SM_IPCLIB_ACQMANNOTIFIER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PQMNotifier.h"
#include "SocketMsgListThreadSafe.h"


class CPqmPmAcqman;
class CThreadWnd;

class CAcqManNotifier : public CPQMNotifier
{
public:
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //CAcqManNotifier(CPqmPmAcqman* f_parent_pqmpmacqman);
    explicit CAcqManNotifier(CPqmPmAcqman* f_parent_pqmpmacqman);
    virtual ~CAcqManNotifier();

    void AcqmanAbortFromnMainThread() ;
    void HandleThreadWndMessages(const int f_message);

protected:
    // call back function for receiving any message from AcqMan socket
    //+Patni-NP/2010Apr26/Modified/MaFT MSA251-204
    void OnReceive(uint f_socket_id, CSocketMsg& f_socket_msg);
    //-Patni-NP/2010Apr26/Modified/MaFT MSA251-204

    // call back function after disconnecting from AcqMan socket
    void OnDisconnect(uint f_socket_id);

private:

    CPqmPmAcqman*   m_parent_pqmpmacqman;
    CSocketMsgListThreadSafe m_socketmsg_list;

    HANDLE m_hthread;
    DWORD  m_dthreadid;
    CThreadWnd* m_pwndthread;
    void CreateThreadWindow();
    static DWORD  WINAPI OnReceiveThreadProc(LPVOID lpParameter);
};


class CThreadWnd : public CWnd
{
public:
    CThreadWnd();
    virtual ~CThreadWnd();
    LRESULT OnAbortMessage(WPARAM wParam, LPARAM lParam);
    LRESULT OnAcqManMessage(WPARAM wParam, LPARAM lParam);

protected:
    friend CAcqManNotifier;

    CAcqManNotifier* m_acqman_notifier;

    DECLARE_MESSAGE_MAP()

};

#endif // #ifndef __SM_IPCLIB_ACQMANNOTIFIER_H__
