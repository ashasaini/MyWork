//Redmine-781_CPP_TestTool
//Redmine-778
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMNotifier.cpp
 *  Overview: Structure of CPQMNotifier class.
 *  Last Modified: 2008/5/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni/LK        2008/5/10 12:00:00    New class for socket
                                                            communication
 *****************************************************************************/

#include "stdafx.h"
#include "PQMNotifier.h"
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

//****************************Method Header************************************
//Method Name   : CPQMNotifier()
//Author        : PATNI/LK
//Purpose       :
//*****************************************************************************
CPQMNotifier::CPQMNotifier(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::CPQMNotifier");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of constructor of CPQMNotifier"));
}

//****************************Method Header************************************
//Method Name   : ~CPQMNotifier()
//Author        : PATNI/LK
//Purpose       :
//*****************************************************************************
CPQMNotifier::~CPQMNotifier(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::~CPQMNotifier");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ~CPQMNotifier"));
}
//****************************Method Header************************************
//Method Name   : OnConnect()
//Author        : PATNI/LK
//Purpose       :
//*****************************************************************************
void CPQMNotifier::OnConnect(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::OnConnect");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of OnConnect"));
}

//****************************Method Header************************************
//Method Name   : OnReceive()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
void CPQMNotifier::OnReceive(
    uint f_socket_id,
    CSocketMsg& f_socket_msg
)//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::OnReceive");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_socket_id);
    UNREFERENCED_PARAMETER(f_socket_msg);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution


    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of OnReceive"));

}

//****************************Method Header************************************
//Method Name   : OnSend()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
void CPQMNotifier::OnSend(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::OnSend");
    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of OnSend"));
}

//****************************Method Header************************************
//Method Name   : OnShutDown()
//Author        : PATNI/LK
//Purpose       :
//*****************************************************************************
void CPQMNotifier::OnShutDown(
    uint f_socket_id
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::OnShutDown");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_socket_id);

    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of OnShutDown"));
}

//****************************Method Header************************************
//Method Name   : OnPToPError()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
void CPQMNotifier::OnPToPError(
    int f_error_code
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::OnPToPError");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_error_code);

    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of OnPToPError"));
}

//****************************Method Header************************************
//Method Name   : OnDisconnect()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
void CPQMNotifier::OnDisconnect(
    uint f_socket_id
)
{
    LPCTSTR FUNC_NAME = _T("CPQMNotifier::OnDisconnect");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_socket_id);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of OnDisconnect"));
}