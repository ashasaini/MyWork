// CustomSocket.cpp: implementation of the CCustomSocket class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "CustomSocket.h"
#include "SocketMgr.h"
#include "TiPrepLogHelper.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomSocket::CCustomSocket()
{
    m_mgr  = NULL;
}
CCustomSocket& CCustomSocket::operator = (CCustomSocket& f_customsocket)
{
	if(this == &f_customsocket) {
        return(*this);
    }
	m_mgr = f_customsocket.m_mgr;
    return (*this);
}

CCustomSocket::~CCustomSocket()
{
    //
}

/////////////////////////////////////////////////////////////////////////////
// CCustomSocket member functions
//////////

/*************************************************************
* Method:       OnAccept
* Description:  This function is used
* Parameter:    int nErrorCode
* Returns:      void
*************************************************************/
void CCustomSocket::OnAccept(int nErrorCode)
{
    if(nErrorCode == 0) {
        if(m_mgr != NULL) {
            m_mgr->AcceptClient();
        } else {
            CString l_csErrMsg(_T("OnAccept mgr is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
    }

    CAsyncSocket::OnAccept(nErrorCode);
}

/*************************************************************
* Method:       OnClose
* Description:  This function is used
* Parameter:    int nErrorCode
* Returns:      void
*************************************************************/
void CCustomSocket::OnClose(int nErrorCode)
{
    
    if(nErrorCode == 0) {
        // 
    }

    if(m_mgr != NULL) {
        m_mgr->CloseSocket();
    } else {
        CString l_csErrMsg(_T("OnClose mgr is NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
    }

    CAsyncSocket::OnClose(nErrorCode);
}

/*************************************************************
* Method:       OnConnect
* Description:  This function is used
* Parameter:    int nErrorCode
* Returns:      void
*************************************************************/
void CCustomSocket::OnConnect(int nErrorCode)
{
    CAsyncSocket::OnConnect(nErrorCode);
}

/*************************************************************
* Method:       OnOutOfBandData
* Description:  This function is used
* Parameter:    int nErrorCode
* Returns:      void
*************************************************************/
void CCustomSocket::OnOutOfBandData(int nErrorCode)
{
    CAsyncSocket::OnOutOfBandData(nErrorCode);
}

/*************************************************************
* Method:       OnReceive
* Description:  This function is used
* Parameter:    int nErrorCode
* Returns:      void
*************************************************************/
void CCustomSocket::OnReceive(int nErrorCode)
{
    if(nErrorCode == 0) {
        if(m_mgr != NULL) {
            m_mgr->OnRecv();
        } else {
            CString l_csErrMsg(_T("OnReceive mgr is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
    }

    CAsyncSocket::OnReceive(nErrorCode);
}

/*************************************************************
* Method:       OnSend
* Description:  This function is used
* Parameter:    int nErrorCode
* Returns:      void
*************************************************************/
void CCustomSocket::OnSend(int nErrorCode)
{
    CAsyncSocket::OnSend(nErrorCode);
}

/*************************************************************
* Method:       SetMgr
* Description:  This function is used
* Parameter:    CSocketMgr * mgr
* Returns:      void
*************************************************************/
void CCustomSocket::SetMgr(CSocketMgr * mgr)
{
    if(mgr != NULL) {
        m_mgr = mgr;
    } else {
        CString l_csErrMsg(_T("SetMgr Parameter is NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////