// WFDAMsgReceiver.cpp: implementation of the CWFDAMsgReceiver class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WFDAMsgReceiver.h"
#include <libStudyManager/CFW/CFWReceiveMsg.h>
#include <libStudyManager/CFW/CFWReplyMsg.h>
#include "WFDAHelper.h"
#include "WFDAHandlerInterface.h"

using MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWListener;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWFDAMsgReceiver* CWFDAMsgReceiver::m_wfdamsgreceiver = NULL ;

CWFDAMsgReceiver::CWFDAMsgReceiver()
{
    m_wfdamsgreceiver = NULL ;
    m_wfda_handlerinterface = NULL ;
}

CWFDAMsgReceiver::CWFDAMsgReceiver(
    CWFDAHandlerInterface* f_wfda_handlerinterface
)
{
    m_wfdamsgreceiver = NULL ;
    m_wfda_handlerinterface = f_wfda_handlerinterface ;
}


//************************************Method Header************************************
// Method Name  : ~CWFDAMsgReceiver
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAMsgReceiver::~CWFDAMsgReceiver()
{
    m_wfda_handlerinterface = NULL ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CWFDAMsgReceiver::DeleteWFDAMsgReceiver(
)
{
    DEL_PTR(m_wfdamsgreceiver) ;
}


//************************************Method Header************************************
// Method Name  : CreateWFDAMsgReceiver
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CWFDAMsgReceiver::CreateWFDAMsgReceiver(
    CWFDAHandlerInterface* f_wfda_handlerinterface
)
{
    if (!m_wfdamsgreceiver) {

        CWFDAHelper::PrepareLogInformation(_T("Createing WFDAMsgReceiver"));

        m_wfdamsgreceiver = new CWFDAMsgReceiver(f_wfda_handlerinterface);
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAMsgReceiver* CWFDAMsgReceiver::GetWFDAMsgReceiver(
)
{
    return m_wfdamsgreceiver ;
}



//************************************Method Header************************************
// Method Name  : RecvCallBack
// Author       : PATNI/ HEMANT
// Purpose      : processes received messages
//***********************************************************************************
BOOL CWFDAMsgReceiver::RecvCallBack(const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
                                    MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg* const replyMsg,
                                    MR::ACQ::SM::COMMON::CFW::CCFWListener* const listener
                                   )
{
    //+TMSC-Tanoue/2010Sep03/MVC008324
    LPCTSTR FUNC_NAME = _T("CWFDAMsgReceiver::RecvCallBack");

    try {

        CWFDAHelper::PrepareLogInformation(_T("RecvCallBack"));

        if (!m_wfda_handlerinterface) {

            ASSERT(FALSE);
            return FALSE;
        }

        WFDAMessageData l_data;

        if (PrepareWFDAMessageDataFor(recvMsg, l_data)) {
            //here, we do support the message for wfda..
            m_wfda_handlerinterface->ProcessReceivedMsg(l_data);
        }


        return TRUE ;

    } catch (CException* e) {
        const int MSG_SIZE = 1024;
        TCHAR errmsg[MSG_SIZE] = {_T('\0')};
        e->GetErrorMessage(errmsg, MSG_SIZE);
        CString strerr = _T("");
        strerr.Format(_T("[Exception] %s"), errmsg);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, strerr);
        throw;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("[EXCEPTION] unknown"));
        throw;
    }

    return TRUE;

    //-TMSC-Tanoue/2010Sep03/MVC008324
}


//************************************Method Header************************************
// Method Name  : PrepareWFDAMessageDataFor
// Author       : PATNI/ HEMANT
// Purpose      : Prepare the WFDAMessageData from IMplus Message
//***********************************************************************************
bool CWFDAMsgReceiver::PrepareWFDAMessageDataFor(
    const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
    WFDAMessageData& f_wfdamessagedata
)
{


    CString l_command_name = recvMsg.GetCommand() ;
    WFDAMessageEnum wfdamessage_enum = CWFDAHelper::GetCommandMessageFor(l_command_name);

    if (wfdamessage_enum == WFDA_Invalid) {

        ASSERT(FALSE);

        CString l_log_str = _T("Could not prepare the WFDA data for received message :") + l_command_name;
        CWFDAHelper::PrepareLogInformation(l_log_str);

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

    return true ;


    ASSERT(FALSE);
    return false ;
}


