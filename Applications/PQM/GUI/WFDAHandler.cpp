// WFDAHandler.cpp: implementation of the CWFDAHandler class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WFDAHandler.h"


#include "WFDAHelper.h"

#include "PQMLogMgr.h"

#include "WFDAMsgSender.h"



//1. SVN_PLN_INSCAN_FLAG              To recognize the inscan sequence		VFT_INT
//Values can be as below
//
//    VFC_INSCAN_NONE       0   None
//
//    VFC_INSCAN_MRS         1   InScan Mode for MRS
//
//    VFC_INSCAN_PWI          2   InScan Mode for PWI
//
//    VFC_INSCAN_FMRI        3   InScan Mode for fMRI
//
//
//2. SVN_PLN_INSCAN_PREFERENCE_GUID                 "Preference GUID for WFDA"   VFT_STRING





//--------------------------------------------------------------------------------------------------------------------------------------------------
//+Patni-HEMANT/ADDED On 6/12/2009 1:28:37 PM/

//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_AddInScanProtocol" 					Database Name, Study Loid, Series Loid, P.GUID 			N/A
//Output 	"RE_FW_AddInScanProtocol" 				"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_ShowInScanPreferenceDialog" 		Database Name, Study Loid, Series Loid					N/A
//													e.g. "Local\50\40"
//Output 	"RE_FW_ShowInScanPreferenceDialog" 		"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_RequestClosePreferenceDialog" 		Database Name, Study Loid, Series Loid					"OK" or "Cancel" or "Hide"
//													e.g. "Local\50\40"
//
//Output 	"RE_FW_RequestClosePreferenceDialog" 	"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_InScanStart" 						Database Name, Study Loid, Series Loid					N/A
//													e.g. "Local\50\40"
//Output 	"RE_FW_InScanStart" 					"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_InScanDone" 						Database Name, StudyLoid, Series Loid,					N/A
//													e.g. "Local\50\40"
//Output 	"RE_FW_InScanDone" 						"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_InScanAbort" 						Database Name, StudyLoid, Series Loid,					N/A
//													e.g. "Local\50\40"
//Output 	"RE_FW_InScanAbort" 					"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//			(Memory application)
//Input 	"FW_AddInScanSequence" 					DB Name, Study Loid, Series Loid and P.GUID 			N/A
//													of the selected protocol in PQM (*A)
//													"Local\50\40\AppID_GUID"
//Output 	"RE_FW_AddInScanSequence" 				"S_OK";													P.GUID
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//
//Input 	"FW_DeleteInScanSequence" 				P.GUID 	N/A
//Output 	"RE_FW_DeleteInScanSequence" 			"S_OK";													N/A
//													"E_FAIL";
//--------------------------------------------------------------------------------------------------------------------------------------------------
//Input 	"FW_DuplicateInScanSequence" 			P.GUID 													Mode: "Replace" or "Append" N/A
//
//Output 	"RE_FW_DuplicateInScanSequence" 		"S_OK";													new P.GUID
//													"E_FAIL";

//-Patni-HEMANT/ADDED On 6/12/2009 1:28:37 PM/
//--------------------------------------------------------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//************************************Method Header************************************
// Method Name  : CWFDAHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAHandler::CWFDAHandler(
    CPQMView* f_pqm_view
) : m_pqm_view(f_pqm_view),
    m_showinscanproconreplyflag(false)
{
    m_wfda_msgsender = new CWFDAMsgSender;
    m_showinscanproconreplyflag = false;
}



//************************************Method Header************************************
// Method Name  : ~CWFDAHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAHandler::~CWFDAHandler()
{
    DEL_PTR(m_wfda_msgsender);
}


//************************************Method Header************************************
// Method Name  : ProcessReceivedMsg
// Author       : PATNI/ HEMANT
// Purpose      : processes the received message.
//***********************************************************************************
bool CWFDAHandler::ProcessReceivedMsg(
    const WFDAMessageData& f_data
)
{
    //this function will be basically called from DPSMesageHandler of PQM GUI.
    //Here we will process the message given in the data given in argument.
    LPCTSTR FUNC_NAME = _T("CWFDAHandler::ProcessReceivedMsg");
    CString l_log_str = _T("ProcessReceivedMsg : Processing the WFDA message received from INScan Server");
    CWFDAHelper::PrepareLogInformation(l_log_str);
    l_log_str = _T("WFDA msg :") + f_data.m_wfdamessage;
    CWFDAHelper::PrepareLogInformation(l_log_str);
    l_log_str = _T("WFDA msg Parameter :") + f_data.m_parameter ;
    CWFDAHelper::PrepareLogInformation(l_log_str);
    l_log_str = _T("WFDA msg Custom Parameter :") + f_data.m_custom ;
    CWFDAHelper::PrepareLogInformation(l_log_str);


    WFDAMessageEnum l_wfdamessage = f_data.m_wfdamessage ;
    CString l_fail_str = INSCAN_REPLY_FAIL;

    switch (l_wfdamessage) {

        case WFDA_Delete_RE :
        case WFDA_AddInScanProtocol_RE:
        case WFDA_Duplicate_RE :
        case WFDA_ScanStart_RE :
        case WFDA_ScanDone_RE :
        case WFDA_ScanAbort_RE :

        case WFDA_RequestClosePreferenceDialog_RE:

            if (f_data.m_parameter.CompareNoCase(l_fail_str) == 0) {

                DisplayErrorMsgForFailCondition(l_wfdamessage);
            }

            break ;

        case WFDA_HideInScanPreferenceDialogOnPageHide:
        case WFDA_ShowInScanPreferenceDialogOnPageShow:
            //return ProcessShowHideGL(f_data);

            break ;

        case WFDA_ShowInScanPreferenceDialog_RE: {
            if (m_showinscanproconreplyflag) {
                m_pqm_view->SendMessageToProcon(PQM_PROCOCN_INSCAN_OPE_END, false, true);
            }

            if (f_data.m_parameter.CompareNoCase(l_fail_str) == 0) {
                if (!m_showinscanproconreplyflag) {
                    HANDLE l_paramsetready_wait = OpenEvent(EVENT_ALL_ACCESS, FALSE, PQM_WFDA_FOR_PARAM_SETREADY_REPL);

                    if (NULL != l_paramsetready_wait) {
                        SetEvent(l_paramsetready_wait);
                        CloseHandle(l_paramsetready_wait);
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ParamSetReady EVENT set as RE_GL_ParamSet_READY message is received."));
                    }

                    m_pqm_view->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
                    m_pqm_view->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
                    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                        CPQMUtility::GetMultiLingualString(_T("IDS_FAIL_TO_EDIT_INSCAN_PROTOCOL_RESET_PAGE")),
                        MPlus::EH::Information, _T("PQM"), _T("PQM"));
                }

                DisplayErrorMsgForFailCondition(l_wfdamessage);
            }

            break ;
        }

        case WFDA_SwitchInscan_RE: {
            if (f_data.m_parameter.CompareNoCase(l_fail_str) == 0) {

                HANDLE l_paramsetready_wait = OpenEvent(EVENT_ALL_ACCESS, FALSE, PQM_WFDA_FOR_PARAM_SETREADY_REPL);

                if (NULL != l_paramsetready_wait) {
                    SetEvent(l_paramsetready_wait);
                    CloseHandle(l_paramsetready_wait);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ParamSetReady EVENT set as RE_GL_ParamSet_READY message is received."));
                }

                m_pqm_view->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
                m_pqm_view->SendMessageToProcon(PQM_PROCON_CANCEL, false, true);
                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    CPQMUtility::GetMultiLingualString(_T("IDS_FAIL_TO_EDIT_INSCAN_PROTOCOL_RESET_PAGE")),
                    MPlus::EH::Information, _T("PQM"), _T("PQM"));

            }

            break;

        }

        default : {
            ASSERT(FALSE);
        }
    }


    return true ;
}

//************************************Method Header************************************
// Method Name  : NewSequencesAdded
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHandler::NewSequencesAdded(
    const int f_count
)
{
    CWFDAHelper::PrepareLogInformation(_T("NewSequencesAdded"));

    if (m_pqm_view) {

        CSeqList* l_seq_list = m_pqm_view->GetSeqList();

        POSITION pos = NULL ;
        CSequence* pseq = NULL;

        for (int index = f_count; index < l_seq_list->GetCount(); index++) {

            pos = l_seq_list->FindIndex(index);

            if (!pos) {
                CWFDAHelper::PrepareLogInformation(_T("CWFDAHandler::NewSequencesAdded pos is NULL"));
                return false;
            }

            pseq = l_seq_list->GetAt(pos);

            if (!pseq) {

                CWFDAHelper::PrepareLogInformation(_T("CWFDAHandler::NewSequencesAdded pseq is NULL"));
                return false;
            }

            const int l_scan_index = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

            if (!IsProtocolInScan(l_scan_index)) {
                CWFDAHelper::PrepareLogInformation(_T("Protocol is Not InScan Kind"));
                continue;
            }

            if (false == SendMessageToInScanServerFor(NULL, WFDA_AddInScanProtocol_FW, WFDA_Protocol_Status_Normal, l_scan_index, NULL, true)) {
                m_pqm_view->SendDeleteReserveDEGL();
                m_pqm_view->DeleteWFDASequence(pseq);
                l_seq_list->RemoveAt(pos);
                index--;
                m_pqm_view->SendDeleteCompleteDEGL();
            }
        }

    }

    ASSERT(FALSE);
    return false;
}

//************************************Method Header************************************
// Method Name  : IsProtocolInScan
// Author       : PATNI/ HEMANT
// Purpose      : returns true if given indexed protocol is inscan kind, else returns false
//***********************************************************************************
bool CWFDAHandler::IsProtocolInScan(
    const int f_acq_order
)
{
    bool l_protocol_inscan = false ;

    if (f_acq_order >= 0) {
        if (S_OK == m_pqm_view->GetAcquisition()->IsProtocolInScan(f_acq_order)) {
            return true;
        }

        return false;
    }

    return l_protocol_inscan;
}

//************************************Method Header************************************
// Method Name  : SendMessageToInScanServerFor
// Author       : PATNI/ HEMANT
// Purpose      : if the last argument f_wfdamessagedata_to_send, this function preperares
//				  the WFDAMessageData data according to f_wfda_msg message and sends
//				  to InScanserver. else the funciton uses the f_wfdamessagedata_to_send data.
//				  The function returns true when success else false.
//				  If the f_error_string is to be set when its not NULL, and thus error message
//				  is not displayed in this function. else error message is diaplayed in this function
//***********************************************************************************
bool CWFDAHandler::SendMessageToInScanServerFor(
    const WFDAMessageData* f_wfdamessagedata_to_send,
    const WFDAMessageEnum f_wfda_msg /*= WFDA_Invalid*/,
    const WFDA_Protocol_Status f_wfda_protocol_status /*= WFDA_Protocol_Status_Normal*/,
    const int f_acq_order /*= -1*/,	//
    CString* const f_error_string /*= NULL*/,
    const bool f_send_sync /*=false*/	//Patni-PJS/2011Feb1/Modify/IR-97
)
{


    CString l_log_str = _T("");

    //If we have not given f_protocol and f_protocol_index then we must have provided f_wfdamessagedata_to_send.
    if ((f_acq_order < 0) && !f_wfdamessagedata_to_send) {

        l_log_str = _T("InCorrect function parameters received");
        CWFDAHelper::PrepareLogInformation(l_log_str);

        ASSERT(FALSE);
        return false ;
    }

    //Now we assume that the protocol is InScan only..
    //1. Check weather protocol is inscan kind or not
    //1.1 If protocol is not inscan kind - return.
    //1.2 If protocol is inscan kind do the followings
    //1.2.1 Prepare the data strucure WFDAMessageData object for f_wfda_msg operation
    //1.2.2 send the data object to the sender to be sent to InScan server

    //Now we assume that the protocol is InScan..
    //	//If we have not given CPqmProtocol* f_protocol, then check for index
    //	if(f_protocol) {
    //
    //		if( !IsProtocolInScan(f_protocol) ) {
    //			//protocol is not inscan, return without doing anything
    //			//this is success condition
    //			l_log_str = _T("Protocol is Not InScan Kind");
    //			CWFDAHelper::PrepareLogInformation(l_log_str);
    //			return true ;
    //		}
    //	}

    CString l_error_message = _T("");

    WFDAMessageData l_wfdamessagedata;

    bool l_return_value = false ;

    if (f_wfdamessagedata_to_send) {

        //we have prepared data to be sent...
        l_return_value = true ;
        l_wfdamessagedata = (*f_wfdamessagedata_to_send);

    } else {
        //pass the protocol object for data preparation
        l_return_value = PrepareInScanData(l_wfdamessagedata, f_wfda_msg , f_acq_order, f_wfda_protocol_status, &l_error_message);
    }

    //+Patni/2011Mar24/Modified/WFDA_Offshore
    //No insert log information...
    CWFDAHelper::PrepareLogInformation(_T("Just Before sending the message to InScan Server"));

    CString l_command_name = CWFDAHelper::GetCommandNameFor(l_wfdamessagedata.m_wfdamessage);
    l_log_str = _T("WFDA msg :") + l_command_name;
    CWFDAHelper::PrepareLogInformation(l_log_str);

    l_log_str = _T("WFDA msg Parameter :") + l_wfdamessagedata.m_parameter ;
    CWFDAHelper::PrepareLogInformation(l_log_str);

    l_log_str = _T("WFDA msg Custom Parameter :") + l_wfdamessagedata.m_custom ;
    CWFDAHelper::PrepareLogInformation(l_log_str);

    if (l_return_value) {
        //sending async message
        //Patni-PJS/2011Feb1/Modify/IR-97
        l_return_value = SendDataToInScanServer(l_wfdamessagedata, l_error_message, f_send_sync);

    } else {

        l_log_str = _T("PrepareInScanData Failed");
        CWFDAHelper::PrepareLogInformation(l_log_str);
    }

    l_log_str = l_command_name + (l_return_value ? _T(" : Messagr Sent successfully") : _T(" : Could not be sent"));

    //-Patni/2011Mar24/Modified/WFDA_Offshore

    CWFDAHelper::PrepareLogInformation(l_log_str);

    //If return value is false, set the error message in f_error_string (output param)
    //or display the error message here in this function only.
    if (!l_return_value) {

        //if this function receives the valid pointer of CString in f_error_string
        //we are going to set the error messasge in that CString pointer.
        //else we are to display the error message here only.
        if (f_error_string) {

            (*f_error_string) = l_error_message ;

        } else {

            CWFDAHelper::DisplayErrorMessage(l_error_message);
        }
    }

    return l_return_value ;

}

//**************************************************************************
//Method Name   : WaitForProWFDAReply
//Author        : PATNI
//Purpose       : To wait for proWFDAReply
//**************************************************************************
void CWFDAHandler::WaitForProWFDAReply()const
{
    LPCTSTR FUNC_NAME = _T("CWFDAHandler::WaitForProWFDAReply");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function"));
    HANDLE l_processtool_thread = NULL;
    unsigned int l_thread_id = NULL;
    l_processtool_thread = (HANDLE)_beginthreadex(0,
                           0,
                           CPQMUtility::WaitForGLParamSetReady,
                           m_pqm_view,
                           0,
                           &l_thread_id);

    if (!l_processtool_thread) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T(" failed to create thread handle"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Ending function"));
}

//************************************Method Header************************************
// Method Name  : SendDataToInScanServer
// Author       : PATNI/ HEMANT
// Purpose      : This function sends the message and data to inscan server using the CWFDAMsgSender
//				  object; The fucntion will return true on success else false.
//***********************************************************************************
bool CWFDAHandler::SendDataToInScanServer(
    const WFDAMessageData f_wfdamessagedata,
    CString& f_error_string,
    const bool f_send_sync /*= false*/
)
{
    CWFDAHelper::PrepareLogInformation(_T("SendDataToInScanServer"));

    if (!m_wfda_msgsender) {

        ASSERT(FALSE);
        return false;
    }

    //Patni-PJS/2011Feb1/Modify/IR-97
    WFDAMessageData l_wfda_msg_data ;

    if (TRUE != m_wfda_msgsender->SendWFDAMessage(f_wfdamessagedata, f_error_string, f_send_sync, &l_wfda_msg_data)) {
        //+Patni-PJS/2011Feb1/Modify/IR-97
        WFDAMessageEnum l_wfdamessage  = l_wfda_msg_data.m_wfdamessage;
        bool l_ret_val = false;

        if (l_wfdamessage == WFDA_ScanStart_RE) {
            MPLUSERRORUSERRESPONSE_e l_result = DisplayWarningDialog(l_wfdamessage, SCAN_WARN_OK_CANCEL_BUTTONS, SCAN_WARN_INFORMATION);

            if (l_result == MPlusErrResponse_Error) {
                l_ret_val = false;

            } else if (l_result == MPlusErrResponse_OK) {
                l_ret_val = true;

            } else if (l_result == MPlusErrResponse_CANCEL) {
                l_ret_val = false;
            }

        } else {
            DisplayErrorMsgForFailCondition(l_wfdamessage);
            l_ret_val = false;
        }

        return l_ret_val;
        //-Patni-PJS/2011Feb1/Modify/IR-97
    }

    return true ;
}

//************************************Method Header************************************
// Method Name  : PrepareInScanData
// Author       : PATNI/ HEMANT
// Purpose      : This function fills the message and parameter and custom param
//***********************************************************************************
bool CWFDAHandler::PrepareInScanData(
    WFDAMessageData& f_data,
    const WFDAMessageEnum f_wfda_msg,
    const int f_acq_order,
    const WFDA_Protocol_Status f_wfda_protocol_status,
    CString* f_error_string /*= NULL*/
)
{

    //This function is to be called before sending the message to INscan server..

    VARIANT* l_pdata = new VARIANT;

    if (S_FALSE == m_pqm_view->GetAcquisition()->GetProtocolInformation(l_pdata, f_acq_order)) {
        return false;
    }

    WFDAProtocolInfoStruct* l_wfdaprotocol_infostruct = (WFDAProtocolInfoStruct*)l_pdata->byref;

    bool l_return_value = PrepareWFDAMessageData(f_data, f_wfda_msg, f_wfda_protocol_status, l_wfdaprotocol_infostruct, f_error_string);

    if (f_error_string) {
        //No need for setting the error here..
        //*f_error_string = _T("Error str");
    }

    l_pdata->byref = NULL;
    DEL_PTR(l_pdata);

    DEL_PTR(l_wfdaprotocol_infostruct);

    return l_return_value;
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHandler::PrepareWFDAMessageData(
    WFDAMessageData& f_data,
    const WFDAMessageEnum f_wfda_msg,
    const WFDA_Protocol_Status f_wfda_protocol_status,
    const WFDAProtocolInfoStruct* f_wfdaprotocol_infostruct /*= NULL*/,
    CString* f_error_string /*= NULL*/
)
{
    bool l_return_value = true ;

    CWFDAHelper::PrepareLogInformation(_T("PrepareWFDAMessageData : Before sending to to InScan Server"));

    bool l_is_pguid_requried = false ;

    switch (f_wfda_msg) {

            /*
            	//Note : according to SDM45-01615_ACE2_Comments.doc
            	//1. we need not to send any message to InScan server when performing Delete on protocol
            	//2. we need to perform same action as protocol tranfer from memory to pqm in case of duplicating the protocol

            	case WFDA_Delete_FW:
            	case WFDA_Duplicate_FW : {

            		if(!f_wfdaprotocol_infostruct) {

            			ASSERT(FALSE);
            			return false;
            		}

            		//[Message]: "FW_DeleteInScanSequence"  [Param]: P.GUID 	[custom]: N/A

            		//[Message]: "FW_DuplicateInScanSequence" 	[Param]: P.GUID 	[custom]: Mode: "Replace" or "Append" or N/A
            		f_data.m_wfdamessage = f_wfda_msg;
            		f_data.m_parameter = f_wfdaprotocol_infostruct->m_pguid ;

            		}
            		break ;
            */

        case WFDA_Duplicate_FW:	//according to SDM45-01615_ACE2_Comments.doc, WFDA_Duplicate_FW = WFDA_AddInScanProtocol_FW.
        case WFDA_AddInScanProtocol_FW:
        case WFDA_SwitchInscan_FW:

            //p.guid is required to be sent in duplicate and add sequence case
            l_is_pguid_requried = true ;

        case WFDA_ScanStart_FW :
        case WFDA_ScanDone_FW :
        case WFDA_ScanAbort_FW :
        case WFDA_ShowInScanPreferenceDialog_FW:
        case WFDA_RequestClosePreferenceDialog_FW: {

            if (!f_wfdaprotocol_infostruct) {

                ASSERT(FALSE);
                return false;
            }

            //[Message]: "FW_InScanStart" 	[Param]: Database Name, Study Loid, Series Loid		[custom]: N/A
            //e.g. "Local\50\40"

            f_data.m_wfdamessage = f_wfda_msg;
            CString l_seperator = PARAMETER_SEPERATOR;

            CString l_param = DATABASE_NAME +  l_seperator +	\
                              f_wfdaprotocol_infostruct->m_study_loid +		\
                              l_seperator +									\
                              f_wfdaprotocol_infostruct->m_series_loid ;

            if (l_is_pguid_requried) {
                if (f_wfda_msg == WFDA_SwitchInscan_FW) {
                    if (f_wfdaprotocol_infostruct->m_inscanswitchflag) {
                        l_param = l_param + l_seperator + f_wfdaprotocol_infostruct->m_defaultpguid;

                    } else {
                        l_param = l_param + l_seperator + f_wfdaprotocol_infostruct->m_pguid;
                    }

                } else {
                    l_param = l_param + l_seperator + f_wfdaprotocol_infostruct->m_pguid;
                }
            }

            f_data.m_parameter = l_param ;
        }
        break ;

        default : {

            l_return_value = false ;
        }
    }


    //Now setting the custom param depending upon message
    if (l_return_value) {
        switch (f_wfda_msg) {

            case WFDA_RequestClosePreferenceDialog_FW: {

                //[Message]: "FW_RequestClosePreferenceDialog" 	[Param]: Database Name, Study Loid, Series Loid
                //[custom]: "OK" or "Cancel"or "Hide"

                CString l_custom_param = _T("");

                switch (f_wfda_protocol_status) {

                    case WFDA_Protocol_WSEEditing_Cancel :
                        l_custom_param = WDFA_WSE_EDITING_CANCEL ;
                        break ;

                    case WFDA_Protocol_WSEEditing_Save :
                        l_custom_param = WDFA_WSE_EDITING_SAVE ;
                        break ;

                    case WFDA_Protocol_WSEEditing_Hide :
                        l_custom_param = WDFA_WSE_EDITING_HIDE ;
                        break ;
                }

                f_data.m_custom = l_custom_param ;
            }
            break ;

            //+Patni-PJS/2010Oct26/Added/WFDA/IR-97
            case WFDA_ShowInScanPreferenceDialog_FW: {
                CString l_custom_param = _T("");

                switch (f_wfda_protocol_status) {

                    case WFDA_Protocol_WSEEditing_Start : {
                        l_custom_param = WDFA_WSE_SHOW_SCAN_PREF_DLG ;
                        m_showinscanproconreplyflag = false;
                    }
                    break ;

                    case WFDA_Protocol_BlockSetting_Show : {

                        l_custom_param = WDFA_WSE_SHOW_SCAN_BLOCK_DLG ;
                        m_showinscanproconreplyflag = true;
                    }
                    break ;
                }

                f_data.m_custom = l_custom_param ;

            }
            break;

            //-Patni-PJS/2010Oct26/Added/WFDA/IR-97
            default : {
                ASSERT(FALSE);
            }
        }
    }

    return l_return_value ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHandler::DuplicateProtocol(
    const int f_duplicate_index
)
{

    CWFDAHelper::PrepareLogInformation(_T("DuplicateProtocol : sending message to InScan Server"));

    if (m_pqm_view) {

        CSeqList* l_seq_list = m_pqm_view->GetSeqList();

        POSITION pos = l_seq_list->FindIndex(f_duplicate_index);
        CSequence* pseq = l_seq_list->GetAt(pos);


        int l_scan_index = _ttoi(pseq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

        if (!IsProtocolInScan(l_scan_index)) {

            CWFDAHelper::PrepareLogInformation(_T("Protocol is Not InScan Kind"));
            return false;
        }

        if (false == SendMessageToInScanServerFor(NULL, WFDA_Duplicate_FW,  WFDA_Protocol_Status_Normal, l_scan_index, NULL, true)) {

            m_pqm_view->SendDeleteReserveDEGL();
            m_pqm_view->DeleteWFDASequence(pseq);
            m_pqm_view->SendDeleteCompleteDEGL();
            l_seq_list->RemoveAt(pos);
            return false;
        }
    }


    return true;
}

//************************************Method Header************************************
// Method Name  : ScanStatusChanged
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHandler::ScanStatusChanged(
    const int f_acq_order,
    const WFDA_Protocol_Status f_status
)
{
    WFDAMessageEnum l_wfdamessage = WFDA_Invalid;

    switch (f_status) {

        case WFDA_Protocol_ScanStart : {
            l_wfdamessage = WFDA_ScanStart_FW;
        }
        break ;

        case WFDA_Protocol_ScanAbort : {
            l_wfdamessage = WFDA_ScanAbort_FW;
        }
        break ;

        case WFDA_Protocol_ScanDone : {
            l_wfdamessage = WFDA_ScanDone_FW;
        }
        break ;

        default :
            ASSERT(FALSE);
            return false;
    }

    CWFDAHelper::PrepareLogInformation(_T("ScanStatusChanged : sending message to InScan Server"));

    return SendMessageToInScanServerFor(NULL, l_wfdamessage, f_status, f_acq_order, NULL, (l_wfdamessage == WFDA_ScanStart_FW) ? true : false);
}

//************************************Method Header************************************
// Method Name  : DisplayErrorMsgForFailCondition
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CWFDAHandler::DisplayErrorMsgForFailCondition(
    const WFDAMessageEnum f_reply_msg
)
{

    LPCTSTR FUNC_NAME = _T("CWFDAHandler::DisplayErrorMsgForFailCondition");
    CString l_error_message = _T("");
    GetErrorMessageForFailCondition(f_reply_msg, l_error_message);

    if (l_error_message.IsEmpty()) {

        ASSERT(FALSE);		//we must have some error message
        return ;
    }

    if (f_reply_msg == WFDA_ShowInScanPreferenceDialog_RE
        || f_reply_msg == WFDA_RequestClosePreferenceDialog_RE) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_error_message);
        return;
    }

    //display the error string..l_error_message
    CWFDAHelper::DisplayErrorMessage(l_error_message);
}


//************************************Method Header************************************
// Method Name  : GetErrorMessageForFailCondition
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHandler::GetErrorMessageForFailCondition(
    const WFDAMessageEnum f_reply_msg,
    CString& f_error_string
)
{
    //This fucntion is called when an error occurs with given message..

    f_error_string = CWFDAHelper::GetErrorMessageForFailCondition(f_reply_msg);

    return true ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAHandler::OnScanEditStatusChangedForProtocol(
    const int f_acq_order,
    const WFDA_Protocol_Status f_wfda_protocol_status
)
{
    //Not In Scope in ACE-2
    //ASSERT(FALSE);
    //return true ;

    LPCTSTR FUNC_NAME = _T("CWFDAHandler::OnScanEditStatusChangedForProtocol");

    VARIANT* l_pdata = new VARIANT;

    //Get Protocol information regarding WFDA
    if (S_FALSE == m_pqm_view->GetAcquisition()->GetProtocolInformation(l_pdata, f_acq_order)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetProtocolInformation function failed"));
        l_pdata->byref = NULL;
        DEL_PTR(l_pdata);
        return false;
    }

    WFDAProtocolInfoStruct* l_wfdaprotocol_infostruct = (WFDAProtocolInfoStruct*)l_pdata->byref;

    //Generally if protocol is Non Inscan we will not send WFDA messsage to Inscanserver
    //In Inscan Switch case if Non Inscan Type Is changed to Inscan Type we have to send the WFDA message to InscanServer
    //The Inscan type is not updated in Varfield but PROLIM has updated in Shared Memeory
    //So if Inscan type is changed we will check the Inscan type received from PROCON during PROMSG_CON_INSCAN_SWITCH message
    bool l_sendwfdamessage_flag = true;

    if (l_wfdaprotocol_infostruct->m_inscanswitchflag) {
        //If Inscan type is Non Inscan and message to sent is other than SWITCH then do not sent the WFDA messages to INscanServer
        if ((l_wfdaprotocol_infostruct->m_inscanswitchtype <= 0) && (f_wfda_protocol_status != WFDA_Protocol_Inscan_Switch)) {
            //If user has clicked cancel after switching inscan type
            //then send close preference dialog message only if it is an Inscan Protocol
            if ((f_wfda_protocol_status == WFDA_Protocol_WSEEditing_Cancel) && IsProtocolInScan(f_acq_order)) {
                l_sendwfdamessage_flag = true;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Message is Not Inscan Switch and Non Inscan Type"));
                l_sendwfdamessage_flag = false;
            }
        }

    } else if (!IsProtocolInScan(f_acq_order)) {
        l_sendwfdamessage_flag = false;
    }

    l_pdata->byref = NULL;
    DEL_PTR(l_pdata);
    DEL_PTR(l_wfdaprotocol_infostruct);

    if (!l_sendwfdamessage_flag) {
        CString l_log_str = _T("Protocol is Not InScan Kind");
        CWFDAHelper::PrepareLogInformation(l_log_str);
        return true;
    }

    WFDAMessageEnum l_wfdamessage = WFDA_Invalid;

    switch (f_wfda_protocol_status) {

        case WFDA_Protocol_Inscan_Switch :
            l_wfdamessage = WFDA_SwitchInscan_FW;
            break;

        case WFDA_Protocol_WSEEditing_Start :
        case WFDA_Protocol_BlockSetting_Show :
            l_wfdamessage = WFDA_ShowInScanPreferenceDialog_FW ;

            break ;

        case WFDA_Protocol_WSEEditing_Cancel :
        case WFDA_Protocol_WSEEditing_Save :

            l_wfdamessage = WFDA_RequestClosePreferenceDialog_FW ;

            break;

        default :
            ASSERT(FALSE);
            return false;
    }

    CWFDAHelper::PrepareLogInformation(_T("OnScanEditStatusChangedForProtocol : sending message to InScan Server"));
    //+Patni-PJS/2011Feb1/Modify/IR-97
    bool l_send_sync = false;

    if (f_wfda_protocol_status == WFDA_Protocol_WSEEditing_Save) {
        l_send_sync = true;
    }

    bool l_result = SendMessageToInScanServerFor(NULL, l_wfdamessage, f_wfda_protocol_status, f_acq_order, NULL, l_send_sync);

    if (f_wfda_protocol_status == WFDA_Protocol_Inscan_Switch || f_wfda_protocol_status == WFDA_Protocol_WSEEditing_Start) {
        WaitForProWFDAReply();
    }

    return l_result;
}

//+Patni-PJS/2011Feb1/Modify/IR-97
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
MPLUSERRORUSERRESPONSE_e CWFDAHandler::DisplayWarningDialog(
    const WFDAMessageEnum f_wfda_msg_enum,
    const int f_btn_type,
    int f_warn_level
)
{
    CString l_error_message = _T("");
    GetErrorMessageForFailCondition(f_wfda_msg_enum, l_error_message);

    if (l_error_message.IsEmpty()) {

        ASSERT(FALSE);		//we must have some error message
        return MPlusErrResponse_Error;
    }

    //display the error string..l_error_message
    return CWFDAHelper::DisplayMessageinWarningDialog(l_error_message, f_btn_type, f_warn_level);


}
//-Patni-PJS/2011Feb1/Modify/IR-97