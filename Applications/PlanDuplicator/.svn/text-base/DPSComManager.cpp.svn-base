// DPSComManager.cpp: Interface for CFW Communication 
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: PlanDuplicator
    Author: PATNI-PlanDuplicator Team
*******************************************************************************/

#include "stdafx.h"
#include "DPSComManager.h"
#include "PDLogHelper.h"
#include "PDErrorMsgDlg.h"
#include "PDTrace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDPSComManager* CDPSComManager::m_dps_com_manager = NULL;
IMplusConnector* CDPSComManager::m_dpsconnector = NULL;

//******************************Method Header*****************************
//Method Name   :CDPSComManager

//Author        :PATNI

//Purpose       :Constructor

//******************************************************************************
CDPSComManager::CDPSComManager()
{
    PD_TRACE(CDPSComManager::CDPSComManager);
    int l_nstatus = E_NO_ERROR;
    m_channel_name = CFW_BC::CHANNEL;
    Initialize();//sb/revw.. check return status if error add to error log
    l_nstatus = InitializeListener(m_channel_name,
        (MPLUS_MESSAGE_CALLBACK) CDPSComManager::RecvCallBack); 
    if(E_NO_ERROR != l_nstatus){		//Mpower 040/2012-08-24/AS/Added
        CString l_cserr (_T("Funcion InitializeListener() returned an error."));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
    }
}

//************************************Method Header***********************
//Method Name   :~CDPSComManager

//Author        :PATNI

//Purpose       :Destructor

//****************************************************************************
CDPSComManager::~CDPSComManager()
{
    PD_TRACE(CDPSComManager::~CDPSComManager);
    int l_nstatus = E_NO_ERROR;
    l_nstatus = DestroyListener( (MPLUS_MESSAGE_CALLBACK) CDPSComManager::RecvCallBack,
        (void **)&m_dpsconnector);
    if(E_NO_ERROR != l_nstatus)			//Mpower 040/2012-08-24/AS/Added
    {
        CString l_cserr (_T("Funcion DestroyListener() returned an error."));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
     }
    
    l_nstatus = Terminate();
    if(E_NO_ERROR != l_nstatus)
    {
        CString l_cserr (_T("Funcion Terminate() returned an error."));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
    }
}

//************************************Method Header************************
//Method Name   :Initialize

//Author        :PATNI

//Purpose       :Create new connector for communication with DPS
//******************************************************************************
void CDPSComManager::Initialize(void)
{
    PD_TRACE(CDPSComManager::Initialize);
    
    if ( NULL == m_dpsconnector ) {
        m_dpsconnector = NEW_CONNECTOR();
        if(NULL == m_dpsconnector){
            CString l_cserr (_T("Memory allocation error in Initialize()"));
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        }
    }
    
}

//*******************************Method Header*****************************
//Method Name   :  GetInstance

//Author   :  PATNI

//Purpose : Creates CDPSComManager instance if it is not created before.
//******************************************************************************
CDPSComManager* CDPSComManager::GetInstance(void)
{
    PD_TRACE(CDPSComManager::GetInstance);
    
    if ( NULL == m_dps_com_manager ) {
        m_dps_com_manager = new CDPSComManager();
    }
    if (NULL == m_dps_com_manager) {
        CString l_cserr (_T("Memory allocation error in DPS GetInstance"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
    }
    return(m_dps_com_manager);
}

//************************************Method Header*****************************
//Method Name   :Terminate

//Author        :PATNI

//Purpose:      Terminate the connection with DPS

//******************************************************************************
int CDPSComManager::Terminate(void)
{
    PD_TRACE(CDPSComManager::Terminate);
    int l_nstatus = E_NO_ERROR;
    
    try {
        
        if ( NULL != m_dpsconnector ) {
            DEL_CONNECTOR((IMplusConnector*)m_dpsconnector);
            m_dpsconnector = NULL;
        }else{
            //dummy
        }
        
    } 
    catch (...) {
        throw;
        l_nstatus = E_ERROR;
        CString l_csErrMsg(_T("IMplusConnector not terminated successfully."));
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
    }
    
    return(l_nstatus);
}

/*************************************************************
* Method:    	Connect
* Description: 	It is a wrapper function to connect to specified 
target channel
* Parameter: 	CString & f_target :
* Returns:   	int
*************************************************************/
int CDPSComManager::Connect(CString& f_target)
{
    PD_TRACE(CDPSComManager::Connect);
    int l_nstatus = E_ERROR;
    
    try{
        //We need to trim the string on the left and right to remove any
        //trailing/leading white spaces,tabs an new lines. 
        f_target.TrimLeft();
        f_target.TrimRight();
        _bstr_t l_target_channel(f_target);
        
        HRESULT hr = S_FALSE;
        if (NULL != m_dpsconnector){
            
            if (l_target_channel != _bstr_t(_T(""))){
                hr = m_dpsconnector->Connect(l_target_channel);
                
                if (SUCCEEDED(hr)){
                    l_nstatus = E_NO_ERROR;
                }else{			//Mpower 040/2012-08-24/AS/Added
                    l_nstatus = E_ERROR;
                }
            }else{
                l_nstatus = E_ERROR;
                CString l_cserr (_T("l_target_channel found empty"));
                CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);			//Mpower 040/2012-08-24/AS/ends
            }
        }
    }
    catch(...)
    {
        CString l_cserr (_T("DPS Connect Exception"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    
    return(l_nstatus);
}

//************************************Method Header*****************************
//Method Name   : SendMsg

//Author        : PATNI

/*Purpose       : It is a wrapper function used to send the message from 
the client to the server. The message can be sent 
either asynchronously or synchronously.*/
//******************************************************************************
int CDPSComManager::SendMsg(IMplusMessage *f_message_send)
{
    PD_TRACE(CDPSComManager::SendMsg);
    int l_nstatus = E_ERROR;
    
    try{
        //Send the message asynchronously. This can be changed
        //later on if required to send synchronously.
        if (NULL != f_message_send){
            
            if (NULL != m_dpsconnector ){
                HRESULT hr = m_dpsconnector->SendASync(f_message_send);
                
                if (SUCCEEDED(hr)){
                    l_nstatus = E_NO_ERROR;
                }
            }else{
                l_nstatus = E_ERROR;
                CString l_cserr (_T("m_dpsconnector found NULL"));			//Mpower 040/2012-08-24/AS/Added
                CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
            }
        }
    }
    catch(...)
    {
        CString l_cserr (_T("DPS SendMsg Exception"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    
    return(l_nstatus);
}

/************************************Method Header*****************************
Method Name   : Disconnect

  Author        : PATNI
  
    Purpose       : It is a wrapper function which disconnects the client 
    to the previously connected channel.
*****************************************************************************/
int CDPSComManager::Disconnect(void)
{
    PD_TRACE(CDPSComManager::Disconnect);
    
    int l_status = E_ERROR;
    
    try{
        if ( NULL != m_dpsconnector ){
            HRESULT hr = m_dpsconnector->DisConnect();
            
            if(SUCCEEDED(hr)){
                l_status = E_NO_ERROR;
            }
        }else {
            l_status = E_ERROR;
            CString l_cserr (_T("m_dpsconnector found NULL"));				//Mpower 040/2012-08-24/AS/Added
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        }
    }
    catch(...)
    {
        CString l_cserrmsg (_T("DPS Disconnect Exception"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    
    return(l_status);
}

//************************************Method Header*****************************
//Method Name   : InitializeListener  
//Author        : PATNI/SM
/*Purpose       : This method is a wrapper function for InitListener 
method of IMplusConnector, which initialize the MSMQ 
and register callback function "f_func" for 
receive message.*/
//******************************************************************************
int CDPSComManager::InitializeListener(CString& f_target, 
                                       MPLUS_MESSAGE_CALLBACK f_func)
{   
    PD_TRACE(CDPSComManager::InitializeListener);
    
    int l_nstatus = E_ERROR;
    
    try{
        //We need to trim the string on the left and right to remove any
        //trailing/leading white spaces,tabs an new lines. Need to verify
        //if this is really desired by the client.
        f_target.TrimLeft();
        f_target.TrimRight();
        _bstr_t l_target_channel(f_target);
        
        if ( NULL != m_dpsconnector ){
            
            if(l_target_channel != _bstr_t("") && NULL != f_func){
                HRESULT hr = m_dpsconnector->InitListener(l_target_channel, f_func, (void **)&m_dpsconnector);
                
                if(SUCCEEDED(hr)){
                    l_nstatus = E_NO_ERROR;
                }
            }
        }else {
            l_nstatus = E_ERROR; 
            CString l_cserr (_T("m_dpsconnector found NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        }
    }
    catch(...)
    {
        CString l_cserrmsg (_T("DPS InitializeListener Exception"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    
    return(l_nstatus);
}

//************************************Method Header*****************************
//Method Name   : DestroyListener  
//Author        : PATNI/SM
/*Purpose       : This method is a wrapper function to kill (destroy) 
the server(connection). Need to pass an "" string
to unlisten on the previously created channel.*/
//******************************************************************************
int CDPSComManager::DestroyListener(MPLUS_MESSAGE_CALLBACK f_func, 
                                    PVOID f_argument)
{
    PD_TRACE(CDPSComManager::DestroyListener);
    
    int l_nstatus = E_ERROR;
    
    try{
        if (NULL != m_dpsconnector){
            HRESULT hr = m_dpsconnector->InitListener("",f_func,f_argument);
            
            if (SUCCEEDED(hr)){
                CString l_cserrmsg (_T("InitListener failed"));
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                l_nstatus = E_NO_ERROR;
            }
        }else{
            l_nstatus = E_ERROR;
            CString l_cserrmmsg (_T("IMplusConnector NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmmsg,__LINE__);
        }
    }
    catch(...)
    {
        CString l_cserrmsg (_T("DPS DestroyListener Exception"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
        //throw new CAMException(IDS_ERR_UNKNOWN,Error,_T ( __FILE__ ), __LINE__);
    }
    
    return(l_nstatus);
}

/************************************Method Header******************************
//Method Name:  RecvCallBack

  //Author:       PATNI
  
    //Purpose:      Callback function used to receive the message
//*****************************************************************************/
int CDPSComManager::RecvCallBack(IMplusMessage* messageParam,
                                 IMplusMessage* messageRet,
                                 PVOID pCustom
                                 )
{
    PD_TRACE(CDPSComManager::RecvCallBack);
    
    if ((NULL == messageParam) || (NULL == messageRet) || (NULL == pCustom)){
        return(E_ERROR);
    }
    
    try 
    {  
        //       _bstr_t strsender = messageParam->GetSender();
        //       _bstr_t strcmd = messageParam->GetCommand();
        //       _bstr_t strparam = messageParam->GetParameter();
        
        IMplusMessage* pMsg = NEW_MSG();
        
        pMsg->SetCommand(_bstr_t(CFW_BC::CMD));
        
        CString l_sender = _T("TestGL");
        
        CDPSComManager::GetInstance()->Connect(l_sender);
        CDPSComManager::GetInstance()->SendMsg(pMsg);
        DEL_MSG(pMsg);
        CDPSComManager::GetInstance()->Disconnect();
        return(E_NO_ERROR);
    }
    catch(...) {
        CString l_cserrmsg (_T("DPS RecvCallBack Exception"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
}

////////////////////////    End Of File     /////////////////////////////////