/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: ACGL
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_)
#define AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "AdvCardiacGL.h"
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
// namespace CFW_BC
// {
//     static const wchar_t CHANNEL[] = _T("TiPrep");
//     static const wchar_t TARGET_CHANNEL[] = _T("Acquire.PQM");
//     static const wchar_t CMD[] = _T("DPS_SEQUENCE_UPDATE");
// }

/**
*  This class is an interface for CFW communication.
*/
typedef enum {
    FW_Error                        = -1,
        FW_CheckState                 = 1,
        FW_Close                        = 2,
        RE_FW_ReportState             = 3,
        FW_Show                     = 4,
        FW_Hide                     = 5,
        RE_FW_CheckState                = 6,
        RE_FW_Close                 = 7,
        FW_ReportState              = 8,
        RE_FW_Show                  = 9,
        RE_FW_Hide                  = 10,
        FW_ERR_UnknownCommandReceived = 11,
        RE_FW_ERR_UnknownCommandReceived = 12,
        FW_RequestClose             = 13,
        FW_RequestHideGL                = 14,
        FW_RequestShowGL                = 15,
        RE_FW_RequestHideGL         = 16,
        RE_FW_RequestShowGL         = 17,
        FW_RegisterWindowZOrder     = 18,
        FW_ChangeWindowZOrder           = 19,
        FW_UnregisterWindowZOrder       = 20,
        FW_AddZOrderLayer               = 21,
        FW_GetAllZOrderLayer            = 22,
        RE_FW_GetAllZOrderLayer     = 23,
        FW_GetAllWindowInLayer      = 24,
        RE_FW_GetAllWindowInLayer       = 25,
        GL_BlockDesign_RequestValue = 26,
        RE_GL_BlockDesign_RequestValue = 27,
        GL_BlockDesign_Update           = 28,
        RE_GL_BlockDesign_Update        = 29,
        GL_BlockDesign_SetDynamicPara = 30,
        RE_GL_BlockDesign_SetDynamicPara = 31,
        FW_ShowInScanPreferenceDialog  = 32,
        RE_FW_ShowInScanPreferenceDialog = 33,
        GL_TensorParamSet_RequestValue = 34,
        RE_GL_TensorParamSet_RequestValue = 35,
        GL_TensorParamSet_Update = 36,
        RE_GL_TensorParamSet_Update = 37,
        GL_Tool_Show                = 38,
        RE_GL_Tool_Show             = 39,
        GL_Tool_Hide                = 40,
        RE_GL_Tool_Hide             = 41,
        FW_RegisterMsg              = 42,
        RE_FW_RegisterMsg           = 43,
        GL_IS_Send_CONTROLLER_IC_Name = 44,
        GL_IS_Get_Selected_IC_Name  = 45,
        RE_GL_IS_Get_Selected_IC_Name = 46,
        FW_UnRegisterMsg              = 47,
        RE_FW_UnRegisterMsg            = 48,
        FW_Disable_Process_Btn           = 49,
        RE_FW_EnableButton              = 50,
        GL_IS_Get_Select_Series_IC_Name = 51,   
        GL_IS_Refresh_Image_Matrix      = 52  ,
        FW_LOAD_GL = 53
} acgl_message_t;
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
    IMplusMessage* CreateMessage(acgl_message_t f_function, 
        int f_status, 
        BSTR f_bstrSender = _T(""), 
        BSTR f_parameter = _T(""), 
        BSTR f_custom = _T(""));
    void SetParentName(BSTR f_parent_name);
    BSTR GetParentName();
    BOOL InitializeServer();
    void SetACGLStatus(int f_status);
    BSTR GetACGLStatusString();
    void DeleteInstance();
    BOOL DestroyServer();
    CString GetChannelName();
    BOOL SendSyncMsg(IMplusMessage *f_message_send,IMplusMessage *f_message_recv,DWORD timeout = INFINITE);

private:
/**
*  This is the channel name of ACGL application for CFW communication.
    */
    CString m_channel_name;
    
    /**
    *  This is the channel name of the parent which invokes ACGL application.
    */
    BSTR m_parent_name;
    
    /**
    *  Stores the current status of ACGL application.
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
    
private:
    CDPSComManager();
};

#endif // !defined(AFX_DPSCOMMANAGER_H__F6ABEF39_775E_4C98_BD10_B6200C95240F__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////