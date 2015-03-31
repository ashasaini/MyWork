// WFDAHelper.h: interface for the CWFDAHelper class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __SM_WFDA_HELPER_H__
#define __SM_WFDA_HELPER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WFDAConstants.h"
#include "PQMLogMgr.h"
//+Patni-PJS/2011Feb1/Modify/IR-97
#include "ErrorDisp\ErrorDispConsts.h"
#include "ErrorDisp\ErrorDisplay.h"
using namespace MPlus::EH;
//-Patni-PJS/2011Feb1/Modify/IR-97

class CWFDAHelper : private MR::ACQ::SM::COMMON::NonCopyable<CWFDAHelper>
{

public:
    static CString GetCommandNameFor(
        const WFDAMessageEnum f_wfdamessage
    );

    static WFDAMessageEnum GetCommandMessageFor(
        const CString& f_message
    );

    static CString GetErrorMessageForFailCondition(
        const WFDAMessageEnum f_wfdamessage
    ) ;


    static bool PrepareLogInformation(
        const CString& f_log_string
    );

    static void DisplayErrorMessage(
        const CString& f_error_message
    );

    static CString GetMultiLingualString(const CString& f_str);

    //Patni-PJS/2011Feb1/Modify/IR-97
    static MPLUSERRORUSERRESPONSE_e DisplayMessageinWarningDialog(const CString& f_error_message, const int f_btn_type, int f_warn_level);

private :
    CWFDAHelper();
    virtual ~CWFDAHelper();
};


#endif // #ifndef __SM_WFDA_HELPER_H__

