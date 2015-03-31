// SocketMgr.h: interface for the CSocketMgr class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_SOCKETMGR_H__5E63DABF_A41A_454B_804C_8BE2A85AF79A__INCLUDED_)
#define AFX_SOCKETMGR_H__5E63DABF_A41A_454B_804C_8BE2A85AF79A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CustomSocket.h"

class CSocketMgr
{
public:
    int OnClose();
    int CloseSocket();
    int AcceptClient();
    int CreateSocket();
    int OnRecv();
    int Send(char * msg);
    int GetPort() {
        return m_port;
    }
    static CSocketMgr * GetInstance();
    static int DeleteInstance();
	CSocketMgr(CSocketMgr& f_socketmgr);
    ~CSocketMgr();
    
private:
    CSocketMgr();
    unsigned int m_port;
    static CSocketMgr * m_commMgr;
    CCustomSocket m_sConnected;
    CCustomSocket m_sListener;
    
};

#endif // !defined(AFX_SOCKETMGR_H__5E63DABF_A41A_454B_804C_8BE2A85AF79A__INCLUDED_)
