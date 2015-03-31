/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_)
#define AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "TiPrep.h"
#include <cfw/cfw.h>

/**
* Used to define the way of sending CFW message.
*/
typedef enum sync_type_e {
    MESSAGE_ASYNC = 0,
        MESSAGE_SYNC
} sync_type_t;

/**
* Used to define the variables required for CFW communication.
*/
namespace CFW_BC
{
    static const wchar_t CHANNEL[] = _T("TiPrep");
    static const wchar_t TARGET_CHANNEL[] = _T("Acquire.PQM");
    static const wchar_t CMD[] = _T("DPS_SEQUENCE_UPDATE");
}

/**
*  This class is an interface for CFW communication.
*/
class CDPSComManager
{
public:
    
    virtual ~CDPSComManager();
    BOOL Initialize();
    static CDPSComManager* GetInstance();
    BOOL Terminate();
    BOOL Connect(CString& f_target);
    BOOL SendMsg(IMplusMessage *f_message_send);
    BOOL Disconnect();
    BOOL InitializeListener(CString& f_target, MPLUS_MESSAGE_CALLBACK f_func);
    BOOL DestroyListener(MPLUS_MESSAGE_CALLBACK f_func, PVOID f_argument);
    static BOOL RecvCallBack(IMplusMessage* messageParam, 
        IMplusMessage* messageRet, 
        PVOID pCustom);
    IMplusMessage* CreateMessage(tiprep_message_t f_function, 
        int f_status, 
        BSTR f_bstrSender = _T(""), 
        BSTR f_parameter = _T(""), 
        BSTR f_custom = _T(""));
    void SetParentName(BSTR f_parent_name);
    BSTR GetParentName();
    BOOL InitializeServer();
    void SetTiPrepStatus(int f_status);
    BSTR GetTiPrepStatusString();
    void DeleteInstance();
    BOOL DestroyServer();
    CString GetChannelName();
	CDPSComManager& operator= (CDPSComManager& f_dpscommanager); //assignment operator
    CString GetBranchBoss() {return m_branchboss; } //AN scanner
    
private:
/**
*  This is the channel name of TiPrep application for CFW communication.
    */
    CString m_channel_name;
    
    /**
    *  This is the channel name of the parent which invokes TiPrep application.
    */
    BSTR m_parent_name;
    
    /**
    *  Stores the current status of TiPrep application.
    */
    int m_status;
    
    /**
    *  Pointer to IIMplusConnector class
    */
    static IMplusConnector* m_dpsconnector;
    
    /**
    *  Pointer to CDPSComManager class
    */
    static CDPSComManager* m_dps_com_manager;
    
	CString m_branchboss; //AN
private:
    CDPSComManager();
};

#endif // !defined(AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_)
