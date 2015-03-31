//Redmine-783
//Redmine-775
//Redmine-777
/*****************************************************************************
*
*  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
*
*  File Name: PQMChildThread.h
*  Overview: Structure of CPQMChildThread class.
*  Creational Date: 2009/04/07
*****************************************************************************
*                       Revision History
*****************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*  1.0				PATNI/SS		2009/04/07			Created
*
*
 *****************************************************************************/
#ifndef __SM_PQM_CHILD_THREAD_H__
#define __SM_PQM_CHILD_THREAD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPqmChildThread : private MR::ACQ::SM::COMMON::NonCopyable<CPqmChildThread>
{
public:
    CPqmChildThread();
    virtual ~CPqmChildThread();

    int CreateChildThread(CPQMView* f_pqmview);
    void PostMessageToThread(UINT, WPARAM, LPARAM)const;
    //Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    void PostMessageToScanThread(UINT, WPARAM, LPARAM)const;

private:
    static HANDLE m_PQMCurrStateEvent;
    HANDLE hThread;
    DWORD  dThreadId;

    //+Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    HANDLE hScanThread;
    DWORD dScanThreadId;
    //-Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026

    bool StopThread();

    //+Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    static DWORD WINAPI PqmChlidScanThreadProc(LPVOID lpParameter);
    //-Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026

    static DWORD WINAPI PqmChlidThreadProc(LPVOID lpParameter);
};

#endif // #ifndef __SM_PQM_CHILD_THREAD_H__
