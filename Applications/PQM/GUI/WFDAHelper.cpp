// WFDAHelper.cpp: implementation of the CWFDAHelper class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WFDAHelper.h"
#include "PQM\PQMCommanMessages.h"
#include "PQM\resource.h"
//#include "pqmconstants.h"
//Patni-PJS/2011Feb1/Modify/IR-97
//#include "ErrorDisp/ErrorDispConsts.h"
#include <MPlusI18n/MPlusI18nCatalog.h>  // Added By KT/ACE-2/15-06-2009/Multilingualisation


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWFDAHelper::CWFDAHelper()
{

}

CWFDAHelper::~CWFDAHelper()
{

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      : Returns the WFDA message for given  command name
//***********************************************************************************
WFDAMessageEnum CWFDAHelper::GetCommandMessageFor(
    const CString& f_message
)
{
    PrepareLogInformation(_T("GetCommandMessageFor called for : ") + f_message);

    if (f_message.CompareNoCase(_T("FW_AddInScanProtocol")) == 0) {
        return WFDA_AddInScanProtocol_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_AddInScanProtocol")) == 0) {
        return WFDA_AddInScanProtocol_RE ;

    }

    if (f_message.CompareNoCase(_T("FW_DeleteInScanSequence")) == 0) {
        return WFDA_Delete_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_DeleteInScanSequence")) == 0) {
        return WFDA_Delete_RE ;

    } else if (f_message.CompareNoCase(_T("FW_DuplicateInScanSequence")) == 0) {
        return WFDA_Duplicate_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_DuplicateInScanSequence")) == 0) {
        return WFDA_Duplicate_RE ;

    } else if (f_message.CompareNoCase(_T("FW_InScanStart")) == 0) {
        return WFDA_ScanStart_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_InScanStart")) == 0) {
        return WFDA_ScanStart_RE ;

    } else if (f_message.CompareNoCase(_T("FW_InScanDone")) == 0) {
        return WFDA_ScanDone_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_InScanDone")) == 0) {
        return WFDA_ScanDone_RE ;

    } else if (f_message.CompareNoCase(_T("FW_InScanAbort")) == 0) {
        return WFDA_ScanAbort_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_InScanAbort")) == 0) {
        return WFDA_ScanAbort_RE ;

    } else if (f_message.CompareNoCase(_T("FW_ShowInScanPreferenceDialog")) == 0) {
        return WFDA_ShowInScanPreferenceDialog_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_ShowInScanPreferenceDialog")) == 0) {
        return WFDA_ShowInScanPreferenceDialog_RE ;

    } else if (f_message.CompareNoCase(_T("FW_RequestClosePreferenceDialog")) == 0) {
        return WFDA_RequestClosePreferenceDialog_FW ;

    } else if (f_message.CompareNoCase(_T("RE_FW_RequestClosePreferenceDialog")) == 0) {
        return WFDA_RequestClosePreferenceDialog_RE;

    } else if (f_message.CompareNoCase(_T("RE_FW_SwitchInScanType")) == 0) {
        return WFDA_SwitchInscan_RE;

    } else if (f_message.CompareNoCase(DPS_MSGID_REQ_HIDE) == 0) {

        return WFDA_HideInScanPreferenceDialogOnPageHide;

    } else if (f_message.CompareNoCase(DPS_MSGID_REQ_SHOW) == 0) {

        return WFDA_ShowInScanPreferenceDialogOnPageShow;
    }

    return WFDA_Invalid;
}


//************************************Method Header************************************
// Method Name  : GetCommandNameFor
// Author       : PATNI/ HEMANT
// Purpose      : Returns the command name for given WFDA message
//***********************************************************************************
CString CWFDAHelper::GetCommandNameFor(
    const WFDAMessageEnum f_wfdamessage
)
{
    CString l_str = _T("");

    switch (f_wfdamessage) {
        case WFDA_AddInScanProtocol_FW :
            l_str = _T("FW_AddInScanProtocol");
            break ;

        case WFDA_AddInScanProtocol_RE:
            l_str =  _T("RE_FW_AddInScanProtocol");
            break;

        case WFDA_Delete_FW :
            l_str = _T("FW_DeleteInScanSequence");
            break ;

        case WFDA_Delete_RE :
            l_str = _T("RE_FW_DeleteInScanSequence");
            break ;

        case WFDA_Duplicate_FW :
            //note : according to TCH - InScan Server , we need to send
            //FW_AddInScanProtocol for duplicate ..
            //l_str = _T("FW_DuplicateInScanSequence");
            l_str = _T("FW_AddInScanProtocol");
            break ;

        case WFDA_Duplicate_RE :
            //note : according to TCH - InScan Server , we need to send
            //FW_AddInScanProtocol for duplicate ..
            //l_str = _T("RE_FW_DuplicateInScanSequence");
            l_str = _T("RE_FW_AddInScanProtocol");
            break ;

        case WFDA_ScanStart_FW :
            l_str = _T("FW_InScanStart");
            break ;

        case WFDA_ScanStart_RE :
            l_str = _T("RE_FW_InScanStart");
            break ;

        case WFDA_ScanDone_FW :
            l_str = _T("FW_InScanDone");
            break ;

        case WFDA_ScanDone_RE :
            l_str = _T("RE_FW_InScanDone");
            break ;

        case WFDA_ScanAbort_FW :
            l_str = _T("FW_InScanAbort");
            break ;

        case WFDA_ScanAbort_RE :
            l_str = _T("RE_FW_InScanAbort");
            break ;

        case WFDA_ShowInScanPreferenceDialog_FW :
            l_str = _T("FW_ShowInScanPreferenceDialog");
            break;

        case WFDA_SwitchInscan_FW :
            l_str = _T("FW_SwitchInScanType");
            break;

        case WFDA_SwitchInscan_RE :
            l_str = _T("RE_FW_SwitchInScanType");
            break;

        case WFDA_ShowInScanPreferenceDialog_RE :
            l_str = _T("RE_FW_ShowInScanPreferenceDialog");
            break;

        case WFDA_RequestClosePreferenceDialog_FW :
            l_str = _T("FW_RequestClosePreferenceDialog");
            break;

        case WFDA_RequestClosePreferenceDialog_RE :
            l_str = _T("RE_FW_RequestClosePreferenceDialog");
            break;

        default :
            ASSERT(FALSE);

    }

    CString l_log_str = _T("GetCommandNameFor called for : ") + l_str;
    PrepareLogInformation(l_log_str);

    return l_str ;
}

CString CWFDAHelper::GetErrorMessageForFailCondition(
    const WFDAMessageEnum f_wfdamessage
)
{

    CString l_str = _T("");

    switch (f_wfdamessage) {

        case WFDA_AddInScanProtocol_RE:
            //l_str.LoadString(IDS_WFDA_AddInScanProtocol_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_AddInScanProtocol_RE_FAILED"));
            break ;

        case WFDA_Delete_RE :
            //l_str = _T("WFDA_Delete_RE : Failed");
            ASSERT(FALSE);
            break ;

        case WFDA_Duplicate_RE :
            //l_str.LoadString(IDS_WFDA_AddInScanProtocol_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_Duplicate_RE_FAILED"));
            break ;

        case WFDA_ScanStart_RE :
            //l_str.LoadString(IDS_WFDA_ScanStart_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_ScanStart_RE_FAILED"));
            break ;

        case WFDA_ScanDone_RE :
            //l_str.LoadString(IDS_WFDA_ScanDone_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_ScanDone_RE_FAILED"));
            break ;

        case WFDA_ScanAbort_RE :
            //l_str.LoadString(IDS_WFDA_ScanAbort_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_ScanAbort_RE_FAILED"));
            break ;

        case WFDA_ShowInScanPreferenceDialog_RE:
            //l_str.LoadString(IDS_WFDA_ShowInScanPreferenceDialog_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_ShowInScanPreferenceDialog_RE_FAILED"));
            break ;

        case WFDA_RequestClosePreferenceDialog_RE:
            //l_str.LoadString(IDS_WFDA_RequestClosePreferenceDialog_RE_FAILED);
            l_str = GetMultiLingualString(_T("IDS_WFDA_RequestClosePreferenceDialog_RE_FAILED"));
            break ;

        default :
            ASSERT(FALSE);
    }

    CString l_log_str = _T("GetErrorMessageForFailCondition called for : ") + l_str;
    PrepareLogInformation(l_log_str);

    return l_str ;
}


//************************************Method Header************************************
// Method Name  : PrepareLogInformation
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHelper::PrepareLogInformation(
    const CString& f_log_string
)
{
    LPCTSTR FUNC_NAME = _T("CWFDAHelper::PrepareLogInformation");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_log_string);
    return true;
}


//************************************Method Header************************************
// Method Name  : DisplayErrorMessage
// Author       : PATNI/ HEMANT
// Purpose      : displays the error string as crucial only
//***********************************************************************************
void CWFDAHelper::DisplayErrorMessage(
    const CString& f_error_message
)
{
    if (f_error_message.IsEmpty()) {

        ASSERT(FALSE);
        return ;
    }

    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(f_error_message, MPlus::EH::Information, _T("PQM"), _T("PQM"));
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CWFDAHelper::GetMultiLingualString(
    const CString& f_str
)
{
    return MPlus::I18n::CCatalog::instance().get(f_str);
}

//+Patni-PJS/2011Feb1/Modify/IR-97
//************************************Method Header************************************
// Method Name  : DisplayMessageinWarningDialog
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
MPLUSERRORUSERRESPONSE_e CWFDAHelper::DisplayMessageinWarningDialog(
    const CString& f_error_message,
    const int f_btn_type,
    int f_warn_level /*= SCAN_WARN_CRUCIAL*/
)
{
    if (f_error_message.IsEmpty()) {

        ASSERT(FALSE);
        return MPlusErrResponse_Error;
    }

    CString l_app_name = _T("PQM");

    return (MPLUSERRORUSERRESPONSE_e)CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(f_error_message,
            l_app_name,
            f_warn_level,
            SCAN_WARN_SYSMODAL,        // MEITEC/2010Jul14/Modified/MVC008324_2
            f_btn_type);
}
//-Patni-PJS/2011Feb1/Modify/IR-97