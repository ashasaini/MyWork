// SocketMgr.cpp: implementation of the CSocketMgr class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "SocketMgr.h"
#include "TiPrepView.h"
#include "TiPrepLogHelper.h"
#include "TiPrepTrace.h"
#define SERVER_PORT 9998
#define SERVER_PORT2 9999

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//Mpower 63/May-2013/AN Added

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSocketMgr *CSocketMgr::m_commMgr = NULL;
/*************************************************************
* Method:    	GetInstance
* Description: 	This function is used 
* Returns:   	CSocketMgr*
*************************************************************/
CSocketMgr* CSocketMgr::GetInstance()
{
    /*TIPREP_TRACE(CSocketMgr::GetInstance);*/
    
    try {
        if(!m_commMgr) {
            m_commMgr = new CSocketMgr();
        }
        
        if(!m_commMgr) {
            CString lcserr(_T("InitInstance memory allocation failed"));
            CTiPrepLogHelper::WriteToErrorLog(lcserr, __LINE__, __FILE__);
            return NULL;
        }
    } catch(...) {
        CString lcserr(_T("GetInstance Exception"));
        CTiPrepLogHelper::WriteToErrorLog(lcserr, __LINE__, __FILE__);
        return NULL;
    }
    
    return m_commMgr;
}
/*************************************************************
* Method:    	DeleteInstance
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CSocketMgr::DeleteInstance()
{
    /*TIPREP_TRACE(CSocketMgr::DeleteInstance);*/
    
    try {
        if(m_commMgr) {
            m_commMgr->OnClose();
            delete m_commMgr;
            m_commMgr = NULL;
        }
    } catch(...) {
        CString lcserr(_T("DeleteInstance Exception"));
        CTiPrepLogHelper::WriteToErrorLog(lcserr, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}
//****************************Method Header***************************
//Method Name   : CSocketMgr
//Purpose       : Constructor
//********************************************************************
CSocketMgr::CSocketMgr():
m_port(0)
{    
    m_sListener.SetMgr(this);
    m_sConnected.SetMgr(this);
}

//****************************Method Header***************************
//Method Name   : CSocketMgr
//Purpose       : Destructor
//********************************************************************
CSocketMgr::~CSocketMgr()
{

}

CSocketMgr::CSocketMgr(CSocketMgr & f_socketmgr)
{
   m_port = f_socketmgr.m_port ;
}
/*************************************************************
* Method:       Send
* Description:  This function is used
* Parameter:    char * msg
* Returns:      int
*************************************************************/
int CSocketMgr::Send(char *msg)
{
    TIPREP_TRACE(CSocketMgr::Send,MRTraceMarker3);
    
    try {
        m_sConnected.Send(msg, strlen(msg));
    } catch(...) {
        CString l_cserrmsg(_T(""));
        l_cserrmsg.Format(_T("CSocketMgr::Send() Exception"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       OnRecv
* Description:  This function is used
* Returns:      int
*************************************************************/
int CSocketMgr::OnRecv()
{
    TIPREP_TRACE(CSocketMgr::OnRecv,MRTraceMarker3);
    CString strData(_T(""));
    int iLen = 0;
    
    try {
        char *pBuf = NULL;
        pBuf = new char [1025];
        
        if(!pBuf) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("CSocketMgr::OnRecv() memory allocation failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        iLen = m_sConnected.Receive(pBuf, 1024);
        
        if(iLen == SOCKET_ERROR) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("CSocketMgr::OnRecv() socket communication error while recieving"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR;
        } else {
            if(iLen < 1025) {
                pBuf[iLen] = NULL;
            }
        }
        
        CTiPrepView::GetView()->OnReceive(pBuf, iLen);
        
        if(pBuf) {
            delete[] pBuf;
        }
        
        pBuf = NULL;
    } catch(...) {
        CString l_cserrmsg(_T(""));
        l_cserrmsg.Format(_T("CSocketMgr::OnRecv() Exception"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       CreateSocket
* Description:  This function is used
* Returns:      int
*************************************************************/
int CSocketMgr::CreateSocket()
{
    TIPREP_TRACE(CSocketMgr::CreateSocket,MRTraceMarker3);
    
    try {
        WSADATA wsaData;
        int status = WSAStartup(MAKEWORD(1, 1), &wsaData);
        
        if(status != 0) {
            CString l_csstatus ;
            l_csstatus.Format(_T("WSAStartup status %d"), status);
            CTiPrepLogHelper::WriteToErrorLog(l_csstatus, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        CString rSocketAddress;
        //int port = 0;
        
        if(m_sListener.Create() == FALSE) { //Let windows decide the available port
            CString lcsmsg;
            lcsmsg.Format(_T("Unable Create server socket.(Error = %d) Please restart application"), GetLastError());
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_sListener.GetSockName(rSocketAddress, m_port);
        
        if(m_sListener.Listen() == FALSE) {
            m_sListener.Close();
            CString lcsmsg;
            lcsmsg.Format(_T("Unable to Listen on that port%d, Socket communication with T1Map may fail"), m_port);
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
    } catch(...) {
        CString lcsmsg;
        lcsmsg.Format(_T("Exception occurred in CreateSocket"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       AcceptClient
* Description:  This function is used
* Returns:      int
*************************************************************/
int CSocketMgr::AcceptClient()
{
    TIPREP_TRACE(CSocketMgr::AcceptClient,MRTraceMarker3);
    //CString strIP;
    
    //UINT port;
    try {
        if(m_sListener.Accept(m_sConnected)) {
            //  m_sConnected.GetSockName(strIP,port);
            //m_status="Client Connected,IP :"+ strIP;
            //  m_sConnected.Send("Connected To Server",strlen("Connected To  Server"));
            //UpdateData(FALSE);
        } else {
            CString lcsmsg;
            lcsmsg.Format(_T("TiPrep Socket Cannot Accept Connection"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
    } catch(...) {
        CString lcsmsg;
        lcsmsg.Format(_T("Exception occured in AcceptClient"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}



/*************************************************************
* Method:       CloseSocket
* Description:  This function is used
* Returns:      void
*************************************************************/
int CSocketMgr::CloseSocket()
{
    TIPREP_TRACE(CSocketMgr::CloseSocket,MRTraceMarker3);
    
    try {
        m_sConnected.ShutDown(0);    //Stops  Sending Of Data
        m_sConnected.ShutDown(1);    //Stops Receiving of data
        m_sConnected.ShutDown(2);   // Stops Both Sending and Receiving of Data
        m_sConnected.Close();
    } catch(...) {
        CString lcsmsg;
        lcsmsg.Format(_T("Exception occured in CloseSocket"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}
/*************************************************************
* Method:       OnClose
* Description:  This function is used to close the server socket finally before tool exits
* Returns:      void
*************************************************************/

int CSocketMgr::OnClose()
{
    TIPREP_TRACE(CSocketMgr::OnClose,MRTraceMarker3);
    
    try {
        // CloseSocket();  
        //Close is called in CAsyncSocket class destructor         
        // m_sListener.Close();
        WSACleanup();
    } catch(...) {
        CString lcsmsg;
        lcsmsg.Format(_T("Exception occured in CloseSocket"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////