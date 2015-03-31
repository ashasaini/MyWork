//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSocket.h
 *  Overview: Structure of CPQMSocket class.
 *  Last Modified: 2008/5/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6
                                                            requirements & Formatted
 *   3.0          Patni/LK         2008/5/10  12:00:00    Code Restructured after
                                                            TA comments
 *****************************************************************************/

#ifndef __SM_IPCLIB_PQMSOCKET_H__
#define __SM_IPCLIB_PQMSOCKET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <libStudyManager/NonCopyable.h>

class IPToPNotifier;
class CPToPSocket;
class CSocketMsg;
class CPQMAdapter;

class CPQMSocket : private MR::ACQ::SM::COMMON::NonCopyable<CPQMSocket>
{
public:
    //Constructor / Destructor
    CPQMSocket();
    virtual ~CPQMSocket();

    // Create new PToPSokcet
    BOOL Create(IPToPNotifier* f_notifier, CPQMAdapter* f_adapter = NULL, BOOL f_isadapetr_needed = true);
    BOOL Connect(const CString& f_server_ip_addr, const CString& f_server_port);
    BOOL ConnectProcon(const CString& f_server_ip_addr, const CString& f_server_port);
    BOOL SendData(const CSocketMsg& f_socket_msg);
    BOOL Close();

    UINT GetSocketID()const;

private:

    CPToPSocket*    m_socket;
    BOOL            m_is_created;
    BOOL            m_is_connected;

    // Validate the IP Address before connecting to the server
    BOOL ValidateIPAdd(const CString& f_ip_add);
    // Validate the Port number before connecting to the server
    BOOL ValidatePortNumber(CString f_port_number);

    int SelectRead(const int f_timeout);

};
#endif // #ifndef __SM_IPCLIB_PQMSOCKET_H__ 
