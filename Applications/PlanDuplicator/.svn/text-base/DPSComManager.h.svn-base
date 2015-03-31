// DPSComManager.h: Header file for CFW Communication. 
//
//
/*******************************************************************************

Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
Module: PlanDuplicator
Author: PATNI-PlanDuplicator Team
*******************************************************************************/
#if !defined(AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_)
#define AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "stdafx.h"
#include <cfw/cfw.h>


typedef enum sync_type_e{
    MESSAGE_ASYNC = 0,
        MESSAGE_SYNC 
}sync_type_t;

namespace CFW_BC
{
    static const wchar_t CHANNEL[] = _T("PlanDuplicator");
    static const wchar_t TARGET_CHANNEL[] = _T("Acquire.PQM");
    static const wchar_t CMD[] = _T("DPS_SEQUENCE_UPDATE");
}

class CDPSComManager
{
public:
    virtual ~CDPSComManager();
    static CDPSComManager* GetInstance(void);
    int Terminate(void);
    int Connect(CString& f_target);
    int SendMsg(IMplusMessage *f_message_send);
    int Disconnect(void);
    static int RecvCallBack(IMplusMessage* messageParam,IMplusMessage* messageRet,PVOID pCustom);

private:
    CDPSComManager();
    CDPSComManager(CDPSComManager&);
    CDPSComManager& operator = (CDPSComManager&);
    void Initialize(void);
    int InitializeListener(CString& f_target, MPLUS_MESSAGE_CALLBACK f_func);
    int DestroyListener(MPLUS_MESSAGE_CALLBACK f_func, PVOID f_argument);
    CString m_channel_name;
    static IMplusConnector* m_dpsconnector;
    static CDPSComManager* m_dps_com_manager;

};

#endif // !defined(AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_)
