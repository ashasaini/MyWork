//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-779
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSocket.cpp
 *  Overview: Implementation of CPQMSocket class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6
                                                            requirements & Formatted
 *   3.0          Patni/LK         2008/5/10 12:00:00     Code Restructured
                                                            after TA comments
 *****************************************************************************/
#include "stdafx.h"
#include "PQMSocket.h"
#include <Pcommlib/SocketFactory.h>
#include <Pcommlib/PToPSocket.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "PQMAdapter.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"


//*******************************Method Header*********************************
//Method Name    :CPQMSocket()
//Author         :PATNI/LK
//Purpose        :Constructor
//*****************************************************************************
CPQMSocket::CPQMSocket(
) : m_socket(NULL),
    m_is_created(FALSE),
    m_is_connected(FALSE)
{
    LPCTSTR FUNC_NAME = _T("CPQMSocket::CPQMSocket");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMSocket"));

}

//*******************************Method Header*********************************
//Method Name    :~CPQMSocket()
//Author         :PATNI/LK
//Purpose        :Destructor
//*****************************************************************************
CPQMSocket::~CPQMSocket(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSocket::~CPQMSocket");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of ~CPQMSocket"));

    if (m_socket) {
        Close();
        DEL_PTR(m_socket);
    }
}

//****************************Method Header************************************
//Method Name   : Create()
//Author        : PATNI\LK
//Purpose       : On New Connection, Create CPToPSocket object
//*****************************************************************************
BOOL CPQMSocket::Create(
    IPToPNotifier* f_notifier,
    CPQMAdapter* f_adapter,
    BOOL f_isadapetr_needed
)
{

    LPCTSTR FUNC_NAME = _T("CPQMSocket::Create");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Create"));

    BOOL l_result = TRUE;

    m_socket = CSocketFactory::CreatePToPSocket(f_notifier,
               (f_adapter) ? (f_adapter) : ((f_isadapetr_needed) ? (new CPQMAdapter()) : NULL));

    if (!m_socket) {
        CString l_errstring(_T(""));
        l_errstring.LoadString(IDS_ERR_SOCKET);


        CPQMLogMgr::GetInstance()->WriteEventLog(l_errstring,
                ERROR_LOG_CRUCIAL1 ,
                _T("PQMSocket.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        CPQMLogMgr::GetInstance()->DisplayError(CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_SOCKET")),
                                                _T("PQMSocket.cpp"), __LINE__,
                                                ERROR_LOG_CRUCIAL1 , ERR_ID, APP_NAME);

        l_result  = FALSE;
        m_is_created = FALSE;

    } else {
        m_is_created = TRUE;

        if (! m_socket->Create()) {
            //+Patni-PJS/2009May10/Modify/IR-82 review comment
            CString string = _T("");
            string.LoadString(IDS_ERR_SOCKET);
            CPQMLogMgr::GetInstance()->WriteEventLog(string,
                    ERROR_LOG_CRUCIAL1,
                    _T("PQMSocket.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
            /*
            CPQMLogMgr::GetInstance()->WriteEventLog(IDS_ERR_SOCKET,
                    ERROR_LOG_CRUCIAL1,
                    _T("PQMSocket.cpp"));
            */
            //-Patni-PJS/2009May10/Modify/IR-82 review comment
            l_result  = false;
            m_is_created = FALSE;
        }
    }

    return l_result;
}


//****************************Method Header************************************
//Method Name   :Connect
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
BOOL CPQMSocket::Connect(
    const CString& f_server_ip_addr,
    const CString& f_server_port
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSocket::Connect");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of Connect"));

    BOOL l_status = FALSE;

    if (!m_is_connected && m_is_created) {
        if (ValidateIPAdd(f_server_ip_addr) && ValidatePortNumber(f_server_port)) {
            if (! m_socket->Connect(f_server_ip_addr, _wtoi(f_server_port))) {
                CString l_errstring(L"Unable to connect server !");
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_errstring);

                CPQMLogMgr::GetInstance()->WriteEventLog(l_errstring ,
                        ERROR_LOG_CRUCIAL1,
                        _T("PQMSocket.cpp"),
                        TRUE);

            } else {

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Connected to Server"));
                m_is_connected = TRUE;
                l_status = TRUE;
            }
        }

    } else {
        l_status = FALSE;
    }

    return l_status;
}

//////////////////////////////////////////////////////////////////////////
// This is aded temporary to connect with proCon server
// same method is connect but in that method if we passing port with doing htons then its not getting connected with procon,
// and if doing htons then it not getting connected with Acqman server......
//****************************Method Header************************************
//Method Name   :ConnectProcon
//Author        :PATNI/LK
//Purpose       : this method is connect with proCon server
//*****************************************************************************
BOOL CPQMSocket::ConnectProcon(
    const CString& f_server_ip_addr,
    const CString& f_server_port
)
{

    LPCTSTR FUNC_NAME = _T("CPQMSocket::ConnectProcon");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ConnectProcon"));

    BOOL l_status = FALSE;

    if (!m_is_connected && m_is_created) {
        if (ValidateIPAdd(f_server_ip_addr) && ValidatePortNumber(f_server_port)) {
            //+Patni-MSN/2009Mar20/Modified/Changes to work with new welks lib of LibWNP package
            if (! m_socket->Connect(f_server_ip_addr, _wtoi(f_server_port))) {
                //-Patni-MSN/2009Mar20/Modified/Changes to work with new welks lib of LibWNP package
                LPCTSTR l_errstring = _T("Unable to connect server !");
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_errstring);
                CPQMLogMgr::GetInstance()->WriteEventLog(l_errstring ,
                        ERROR_LOG_CRUCIAL1,
                        _T("PQMSocket.cpp"),
                        TRUE);

            } else {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Connected to Server"));

                m_is_connected = TRUE;
                l_status = TRUE;
            }
        }

    } else {
        l_status = FALSE;
    }

    return l_status;
}
//****************************Method Header************************************
//Method Name   : SendData()
//Author        : PATNI\LK
//Purpose       : Sends the data through PToPSocket
//*****************************************************************************
BOOL CPQMSocket::SendData(
    const CSocketMsg& f_socket_msg
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSocket::SendData");
    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of SendData"));

    if (m_socket && m_is_connected) {
        m_socket->SendData(const_cast<CSocketMsg&>(f_socket_msg)); // FIXME CSocketMsg should be fixed
        return TRUE;
    }

    return FALSE;
}

//****************************Method Header************************************
//Method Name   : Close()
//Author        : PATNI\LK
//Purpose       : Function to Close Socket Connection
//*****************************************************************************
BOOL CPQMSocket::Close(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSocket::Close");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Close"));

    if (m_socket && m_is_connected) {
        //+ Patni-PP/2009Sep22/Modified/PQM Crash
        //l_status = m_socket->ShutDown();
        m_socket->CloseConnection();

        //Patni-PJS+Hemant/2010May19/Modified/JaFT/TMSC-REDMINE-23
        m_is_connected = FALSE;

        return TRUE;
        //- Patni-PP/2009Sep22/Modified/PQM Crash
        // delete should happen in destructor
        //delete m_socket;
        //m_socket = NULL;
    }

    return FALSE;
}

//Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   : GetSocketID
//Author        : PATNI\DKH
//Purpose       : To get the socket ID
//**************************************************************************
UINT CPQMSocket::GetSocketID()const
{
    UINT l_socketid = 0 ;

    if (NULL != m_socket) {
        l_socketid = m_socket->GetSocketID();
    }

    return l_socketid ;
}

//****************************Method Header************************************
//Method Name   : ValidateIPAdd()
//Author        : PATNI\LK
//Purpose       : To validate the IP add
//*****************************************************************************
BOOL CPQMSocket::ValidateIPAdd(
    const CString& f_ip_add
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSocket::ValidateIPAdd");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ValidateIPAdd"));

    // check the ip_add should not be blank
    if (f_ip_add.IsEmpty()) {
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   : ValidatePortNumber()
//Author        : PATNI\LK
//Purpose       : To validate the port
//*****************************************************************************
BOOL CPQMSocket::ValidatePortNumber(
    CString f_port_number
)
{

    LPCTSTR FUNC_NAME = _T("CPQMSocket::ValidatePortNumber");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ValidatePortNumber"));

    // validate the port number....it must be number between 1024 and 65535
    if (_wtoi(f_port_number.GetBuffer(f_port_number.GetLength())) < PORT_MIN_NUMBER
        || _wtoi(f_port_number.GetBuffer(f_port_number.GetLength())) > PORT_MAX_NUMBER) {
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   : SelectRead()
//Author        : PATNI
//Purpose       : To wait on sockt FD
//*****************************************************************************
int CPQMSocket::SelectRead(
    const int f_timeout
)
{
    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_timeout);

    switch (select(0, NULL, NULL, NULL, NULL)) {

        case SOCKET_ERROR:
        case SOC_SEL_TIMEOUT:
            return FALSE;
    }

    return TRUE;
}
