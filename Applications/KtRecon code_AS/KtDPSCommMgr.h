// KtDPSCommMgr.h: interface for the CKtDPSCommMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTDPSCOMMMGR_H__AC01DC6C_BBA7_4324_9E47_9FFB5825573B__INCLUDED_)
#define AFX_KTDPSCOMMMGR_H__AC01DC6C_BBA7_4324_9E47_9FFB5825573B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CommonDefinitions.h"
#include "KtRecon.h"
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


class CKtDPSCommMgr  
{
public:
	static int exit;
    virtual ~CKtDPSCommMgr();
    BOOL Initialize();
    static CKtDPSCommMgr* GetInstance();
    BOOL Terminate();
    BOOL Connect(CString& f_target);
    BOOL SendMsg(IMplusMessage *f_message_send);
    BOOL Disconnect();
    BOOL InitializeListener(CString& f_target, MPLUS_MESSAGE_CALLBACK f_func);
    BOOL DestroyListener(MPLUS_MESSAGE_CALLBACK f_func, PVOID f_argument);
    static BOOL RecvCallBack(IMplusMessage* messageParam, IMplusMessage* messageRet, PVOID pCustom);
    IMplusMessage* CreateMessage(tiprep_message_t f_function, int f_status, BSTR f_bstrSender = _T(""), BSTR f_parameter = _T(""), BSTR f_custom = _T(""));
    void SetParentName(BSTR f_parent_name);
    BSTR GetParentName();
    BOOL InitializeServer();
    void SetKtReconStatus(int f_status);
    BSTR GetKtReconStatusString();
    void DeleteInstance();
    BOOL DestroyServer();
    
private:
    CString m_channel_name;
    BSTR m_parent_name;
    int m_status;
    CKtDPSCommMgr();
    static IMplusConnector* m_dpsconnector;
    static CKtDPSCommMgr* m_dps_com_manager;
    
};

#endif // !defined(AFX_KTDPSCOMMMGR_H__AC01DC6C_BBA7_4324_9E47_9FFB5825573B__INCLUDED_)
