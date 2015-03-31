// CustomSocket.h: interface for the CCustomSocket class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_CUSTOMSOCKET_H__C118D4F8_AAEB_4C28_9951_8CBCB4E34913__INCLUDED_)
#define AFX_CUSTOMSOCKET_H__C118D4F8_AAEB_4C28_9951_8CBCB4E34913__INCLUDED_
#include <afxsock.h>  
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CSocketMgr;
class CCustomSocket  : public CAsyncSocket
{
public://construction,copy & destruction
    CCustomSocket();
    virtual ~CCustomSocket();
	CCustomSocket& operator= (CCustomSocket& f_customsocket); //assignment operator
    virtual void OnAccept(int nErrorCode);
    virtual void OnClose(int nErrorCode);
    virtual void OnConnect(int nErrorCode);
    virtual void OnOutOfBandData(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
    virtual void OnSend(int nErrorCode);
    void SetMgr(CSocketMgr * mgr);
private:
    CSocketMgr * m_mgr;
};

#endif // !defined(AFX_CUSTOMSOCKET_H__C118D4F8_AAEB_4C28_9951_8CBCB4E34913__INCLUDED_)
