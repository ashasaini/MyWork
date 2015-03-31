// WFDAMsgSender.cpp: implementation of the CWFDAMsgSender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WFDAMsgSender.h"
#include <libStudyManager/CFW/CFWReceiveMsg.h>
#include "PQM\PQMCommanMessages.h"
#include "PQM\resource.h"
#include "DPSManager.h"
#include "WFDAMsgReceiver.h"

using MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//************************************Method Header************************************
// Method Name  : CWFDAMsgSender()
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAMsgSender::CWFDAMsgSender()
{
}



//************************************Method Header************************************
// Method Name  : ~CWFDAMsgSender
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAMsgSender::~CWFDAMsgSender()
{
}



//************************************Method Header************************************
// Method Name  : SendWFDAMessage
// Author       : PATNI/ HEMANT
// Purpose      : send the message to In Scan server.
//				f_wfdamessagedata : data/ message to send
//				f_sendsync : sending sync or a sync message
//				f_wfdamessage_outdata : output data prepared in case of Sync communication only
//				f_timeout : timeout value ; if -1 then default is used.
//***********************************************************************************
BOOL CWFDAMsgSender::SendWFDAMessage(
    const WFDAMessageData& f_wfdamessagedata,
    CString& f_error_string,
    const bool f_sendsync /*= false*/,
    WFDAMessageData* f_wfdamessage_outdata /*= NULL*/,
    const int f_timeout /*= -1*/
) const
{

    //1. Prepare IMplusConnector from WFDAMessageData
    //2. Connect to wfda channel again; handle the connection error
    //3. Send the message

    //Note :
    //1) when sending Async message => f_sendsync = false ; In this case
    //there will be no use of f_wfdamessage_outdata data and f_timeout in this function
    //2) When sending Sync message f_wfdamessage_outdata will be filled with data
    //sent by out channel.This data is to be prepared here only.
    //IN this case if the return reply from Sync message is S_FALSE then the data
    //will not be filled.
    //if timeout value is -1 then default timeout value is to be used.
    LPCTSTR FUNC_NAME = _T("CWFDAMsgSender::SendWFDAMessage");
    BOOL l_return_value = FALSE ;
    CString l_erro_message = _T("");

    CCFWReceiveMsg l_re_msg ;


    if (f_sendsync) {


        //send the Sync message and get the output in second param
        l_return_value = CDPSManager::GetInstance()->SendSyncMsg(WFDA_INSCAN_SERVER_CHANNEL_NAME,
                         CWFDAHelper::GetCommandNameFor(f_wfdamessagedata.m_wfdamessage),
                         &l_re_msg, f_wfdamessagedata.m_parameter, f_wfdamessagedata.m_custom,  f_timeout);


        if (FALSE == l_return_value) {

            CWFDAHelper::DisplayErrorMessage(CWFDAHelper::GetMultiLingualString(_T("IDS_WFDA_MSGSENDING_FAILED_MESSAGE")));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Send SYNC Message"));
            return l_return_value;
        }

        if (!PrepareWFDAMessageDataFor(l_re_msg, *f_wfdamessage_outdata)) {

            CWFDAHelper::PrepareLogInformation(_T("PrepareWFDAMessageDataFor faild to prepare WFDA Msg data"));
            return FALSE;

        }


    } else {

        l_return_value =  CDPSManager::GetInstance()->SendASyncMsg(WFDA_INSCAN_SERVER_CHANNEL_NAME,
                          CWFDAHelper::GetCommandNameFor(f_wfdamessagedata.m_wfdamessage),
                          f_wfdamessagedata.m_parameter, f_wfdamessagedata.m_custom);

        if (FALSE == l_return_value) {

            CWFDAHelper::DisplayErrorMessage(CWFDAHelper::GetMultiLingualString(_T("IDS_WFDA_MSGSENDING_FAILED_MESSAGE")));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Send ASYNC Message"));
            return l_return_value;
        }
    }

    return  l_return_value;
}

//************************************Method Header************************************
// Method Name  : PrepareWFDAMessageDataFor
// Author       : PATNI/ HEMANT
// Purpose      : Prepare the WFDAMessageData from IMplus Message
//***********************************************************************************
bool CWFDAMsgSender::PrepareWFDAMessageDataFor(
    const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
    WFDAMessageData& f_wfdamessagedata
) const
{

    WFDAMessageEnum wfdamessage_enum = CWFDAHelper::GetCommandMessageFor(recvMsg.GetCommand());

    if (wfdamessage_enum == WFDA_Invalid) {

        ASSERT(FALSE);


        return false ;
    }

    f_wfdamessagedata.m_wfdamessage = wfdamessage_enum;
    f_wfdamessagedata.m_parameter =  recvMsg.GetParameter();
    f_wfdamessagedata.m_custom =  recvMsg.GetCustom();

    CString l_log_str = _T("PrepareWFDAMessageDataFor : Preparing the WFDA data for received messages");
    CWFDAHelper::PrepareLogInformation(l_log_str);
    l_log_str = _T("WFDA msg :") + f_wfdamessagedata.m_wfdamessage;
    CWFDAHelper::PrepareLogInformation(l_log_str);
    l_log_str = _T("WFDA msg Parameter :") + f_wfdamessagedata.m_parameter ;
    CWFDAHelper::PrepareLogInformation(l_log_str);
    l_log_str = _T("WFDA msg Custom Parameter :") + f_wfdamessagedata.m_custom ;
    CWFDAHelper::PrepareLogInformation(l_log_str);


    CString l_fail_str = INSCAN_REPLY_FAIL;

    if (f_wfdamessagedata.m_parameter.CompareNoCase(l_fail_str) == 0) {
        return false;
    }

    return true ;




}




