// WFDAHandler.h: interface for the CWFDAHandler class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM__WFDA_HANDLER_H__
#define __SM__WFDA_HANDLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PQMView.h"
#include "WFDAHandlerInterface.h"
//+Patni-PJS/2011Feb1/Modify/IR-97
#include "ErrorDisp\ErrorDispConsts.h"
#include "ErrorDisp\ErrorDisplay.h"
using namespace MPlus::EH;
//-Patni-PJS/2011Feb1/Modify/IR-97

class CWFDAMsgSender;



class CWFDAHandler : public CWFDAHandlerInterface
{

public:
    explicit CWFDAHandler(CPQMView* f_pqm_view);

    virtual ~CWFDAHandler();
    bool IsProtocolInScan(const int f_acq_order);
    bool ProcessReceivedMsg(const WFDAMessageData& f_data) ;
    bool NewSequencesAdded(const int f_count);
    bool PrepareInScanData(WFDAMessageData& f_data,
                           const WFDAMessageEnum f_wfda_msg,
                           const int f_acq_order,
                           const WFDA_Protocol_Status f_wfda_protocol_status,
                           CString* f_error_string = NULL
                          );

    bool PrepareWFDAMessageData(WFDAMessageData& f_data,
                                const WFDAMessageEnum f_wfda_msg,
                                const WFDA_Protocol_Status f_wfda_protocol_status,
                                const WFDAProtocolInfoStruct* f_wfdaprotocol_infostruct = NULL,
                                CString* f_error_string = NULL
                               );

    bool DuplicateProtocol(const int f_duplicate_index) ;
    bool ScanStatusChanged(const int f_acq_order, const WFDA_Protocol_Status f_status);

    void DisplayErrorMsgForFailCondition(const WFDAMessageEnum f_reply_msg);
    //Patni-PJS/2011Feb1/Modify/IR-97
    MPLUSERRORUSERRESPONSE_e DisplayWarningDialog(const WFDAMessageEnum f_wfda_msg_enum, const int f_btn_type, int f_warn_level);
    bool GetErrorMessageForFailCondition(const WFDAMessageEnum f_reply_msg, CString& f_error_string);

    bool OnScanEditStatusChangedForProtocol(const int f_acq_order, const WFDA_Protocol_Status f_wfda_protocol_status);

private :

    CWFDAMsgSender* m_wfda_msgsender;
    CPQMView* m_pqm_view;
    bool m_showinscanproconreplyflag;

private :
    bool SendDataToInScanServer(const WFDAMessageData f_wfdamessagedata, CString& f_error_string, const bool f_send_sync = false);
    bool SendMessageToInScanServerFor(const WFDAMessageData* f_wfdamessagedata_to_send,
                                      const WFDAMessageEnum f_wfda_msg = WFDA_Invalid,
                                      const WFDA_Protocol_Status f_wfda_protocol_status = WFDA_Protocol_Status_Normal,
                                      const int f_acq_order = -1,
                                      CString* const f_error_string = NULL,
                                      const bool f_send_sync = false
                                     );
    void WaitForProWFDAReply()const;

};

#endif // #ifndef __SM__WFDA_HANDLER_H__

