//Redmine-781_CPP_TestTool
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMNotifier.h
 *  Overview: Structure of CPQMNotifier class.
 *  Last Modified: 2008/5/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni/LK        2008/5/10 12:00:00    New class derived for
 //                                                          socket communication
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMNOTIFIER_H__
#define __SM_IPCLIB_PQMNOTIFIER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <Pcommlib/PToPSocket.h>
//-Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <libStudyManager/NonCopyable.h>

class CPQMNotifier : public IPToPNotifier
    , private MR::ACQ::SM::COMMON::NonCopyable<CPQMNotifier>
{
public:
    CPQMNotifier();
    virtual ~CPQMNotifier();
protected:

    // call back function after connecting with the socket
    virtual void OnConnect();

    // call back function for receiving for the socket
    virtual void OnReceive(uint f_socket_id, CSocketMsg& f_socket_msg);//+Patni-NP/2010Apr26/Added/MaFT MSA251-204

    //call back function after sending with the socket
    virtual void OnSend();

    //call back function after shutting down the socket
    virtual void OnShutDown(uint f_socket_id);

    //call back function after receiving error from the socket
    virtual void OnPToPError(int f_error_code);

    //call back function after disconnecting with the socket
    virtual void OnDisconnect(uint f_socket_id);

};
#endif // #ifndef __SM_IPCLIB_PQMNOTIFIER_H__ 
