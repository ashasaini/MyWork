//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IPCMessageHandler.cpp
 *  Overview: Implementation of CIPCMessageHandler class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "stdafx.h"
#include "IPCMessageHandler.h"
#include <PQM/resource.h>
#include "PQMView.h"
//Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
#include "PQMCDSController.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//int count_seq_ready = 0;
//int prev_last_index = 0;
long maxl = 0;

extern CPQMView* g_MainView;


//************************************Method Header****************************
//Method Name:  CIPCMessageHandler

//Author:  PATNI

//Purpose: Constructor

//****************************************************************************/
CIPCMessageHandler::CIPCMessageHandler(
)
{
}

//************************************Method Header****************************

//Method Name:  ~CIPCMessageHandler

//Author:  PATNI

//Purpose: Destructor

//****************************************************************************/
CIPCMessageHandler::~CIPCMessageHandler(
)
{
}

//************************************Method Header****************************

//Method Name:  ReceiveMessage

//Author:  PATNI

//Purpose: Receiving and processing of messages from DPS

//****************************************************************************/
BOOL CIPCMessageHandler::ReceiveMessage(
    const pqm_msg_tag Msg
)const
{
    LPCTSTR FUNC_NAME = _T("CIPCMessageHandler::ReceiveMessage");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Added by Hemant On 10/6/2008 12:17:21 PM
    static l_pqm_msg_seq_rdy_display_first_time = false ;
    int l_prev_last_index = 0;

    switch (Msg) {
        case PQM_MSR_RCV_SEQ: { //  Ready to receive sequences
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_MSR_RCV_SEQ"));

            if (g_MainView->GetPQMListCtrl().GetSelectionMark() == -1) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("This Protocol can't be Registered!!"));

                g_MainView->SetWaitCursor(false);
                g_MainView->SetSendSeqListFor(NULL, 0);

                return FALSE;
            }

            g_MainView->SetRegistFlag(FALSE);
            g_MainView->SetSendSeqList();
        }
        break;

        case PQM_MSG_PASCOM_REGIST_DONE: {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PASCOM_REGIST_DONE"));
            g_MainView->SetRegistFlag(TRUE);

        }
        break;

        case PQM_MSG_SEQUENCE_TRANSFER_COMPLETED:
            l_pqm_msg_seq_rdy_display_first_time = false;
            break;

        case PQM_MSG_SERIES_LOAD :  //added by Hemant

        case PQM_MSG_SEQ_RDY_DISPLAY:
        case PQM_MSG_SEQ_RDY: { // Sequences are Ready
            //Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_MSG_SEQ_RDY"));

            const int count_new_seq = g_MainView->GetSeqList()->GetCount();

            //Patni-MSN-MN/2009Dec09/Added/MSA0248-00032/CFW channel Clear Issue
            g_MainView->GetSequenceParamList();

            bool l_pqm_msg_seq_rdy_display = false;

            if (Msg == PQM_MSG_SEQ_RDY_DISPLAY) {

                g_MainView->GetSequenceList(MAX_SEQUENCE);
                l_pqm_msg_seq_rdy_display = true ;

            } else {
                g_MainView->GetSequenceList();

                if (!l_pqm_msg_seq_rdy_display_first_time) {
                    l_pqm_msg_seq_rdy_display = true ;
                }
            }

            const bool max_limit_status = g_MainView->GetPQMSequence()->
                                          GetList(g_MainView->GetSeqList(), g_MainView->GetMaxProtocolLimit());

            if (g_MainView->GetMaxProtocolLimit() == g_MainView->GetPQMListCtrl().GetItemCount() ||
                (!max_limit_status)) {

                // check sequence limit message box is displayed or not.
                if (!g_MainView->GetMsgSeqLimit()) {

                    CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_PROTOCOL,
                            _T("IDS_MAX_PROTOCOL"), SCAN_ERR_NOTIFY, _T("PQM"));
                }
            }

            //Whenever new protocols come from Memory,increment counter
            //count_seq_ready++;

            //+Patni-PJS/2011Feb1/Modify/IR-97
            if (Msg != PQM_MSG_SERIES_LOAD) {
                g_MainView->NewSequencesAdded(count_new_seq);
            }

            //-Patni-PJS/2011Feb1/Modify/IR-97

            g_MainView->InsertNewSequencesOnGUI(Msg == PQM_MSG_SERIES_LOAD, l_pqm_msg_seq_rdy_display);

            //Added by Hemant On 10/6/2008 12:04:43 PM
            //if we have done the selection for PQM_MSG_SEQ_RDY_DISPLAY message
            //then make it true..
            //so that next time for PQM_MSG_SEQ_RDY message it dones't select any seq..
            //
            //if we don't have processed PQM_MSG_SEQ_RDY_DISPLAY currently then
            //this flag will be false - reset for next time..
            l_pqm_msg_seq_rdy_display_first_time = (Msg == PQM_MSG_SEQ_RDY_DISPLAY) ;

            //following is to be executed when we are tranfering seq from memory
            //to PQM only..
            if (Msg != PQM_MSG_SERIES_LOAD) {

                //Initial settings
                //for next new protocols,apply anatomy and patient orientation
                //settings as per previous last protocol
                if (count_new_seq >= 1) {
                    l_prev_last_index = count_new_seq - 1;

                } else {
                    l_prev_last_index = 0;
                }


                if ((l_prev_last_index >= 0) && (g_MainView->GetPQMListCtrl().GetItemCount() > 1)) {

                    CPqmPrMode prmode = g_MainView->GetPrModeOfSelectedSequence(
                                            l_prev_last_index);

                    g_MainView->ApplyAnatomyForNewProtocols(
                        l_prev_last_index, prmode.m_anatomy, prmode.m_str_trsc);
                    g_MainView->ApplyPatientOrientationForNewProtocols(
                        l_prev_last_index, prmode);
                    g_MainView->ApplyReceiverCoilForNewProtocols(
                        l_prev_last_index, prmode.m_rcv_coil_id,
                        prmode.m_recv_coil_label, prmode.m_str_trsc);

                    //Patni-Hemant/2011Feb03/Commented/REDMINE-772_Reopen
                    //g_MainView->OnSelectionStatusChanged();
                }
            }

            //Patni-Hemant/2011Feb03/Added/REDMINE-772_Reopen
            g_MainView->OnSelectionStatusChanged();

            //Patni-ARD/2009September10/Modified/Prototype 2 Control checking at display time
            if (Msg != PQM_MSG_SEQ_RDY_DISPLAY) {

                //+Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
                //+Patni-HAR/2010Feb24/Modified/MSA0251-00187
                result_r l_result = g_MainView->GetGUIListProtocolInfo();

                if (TYPE_FALSE == l_result) {
                    g_MainView->SetMismatchErrorFlag(TRUE);

                } else if (TYPE_ERR_MANAGER_EXCP == l_result) {
                    g_MainView->PostMessage(WM_CMD_FILE_NOT_EXIST);

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("Error Manager Exception occurred"));

                } else {
                    g_MainView->SetMismatchErrorFlag(FALSE);
                }

                //-Patni-HAR/2010Feb24/Modified/MSA0251-00187
                //-Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1
            }

            g_MainView->UpdateHoldValue();

            //+Patni-HEMANT/ADDED On 4/29/2009 12:55:44 PM/ ACE / Internal Defect
            //update the T R S C fully from last present seq index
            g_MainView->UpdateFullTRSCFromGivenIndex(count_new_seq);

            break;


        }	//Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections

        case PQM_MSG_INSERT_SHIM_PROTOCOL:
        case PQM_MSG_INSERT_MAP_PROTOCOL : {


            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("PQM_MSG_INSERT_MAP_PROTOCOL"));

            const bool max_limit_status = g_MainView->GetPQMSequence()->
                                          GetList(g_MainView->GetSeqList(), g_MainView->GetMaxProtocolLimit());

            if (g_MainView->GetMaxProtocolLimit() == g_MainView->GetPQMListCtrl().GetItemCount() ||
                (!max_limit_status)) {

                // check sequence limit message box is displayed or not.
                if (!g_MainView->GetMsgSeqLimit()) {

                    CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_PROTOCOL,
                            _T("IDS_MAX_PROTOCOL"), SCAN_ERR_NOTIFY, _T("PQM"));
                    break;

                }
            }

            g_MainView->InsertAutoInsertProtocol(Msg);

        }

        break;

        case PQM_MSG_ERROR_INIT:        //ERROR INITIALIZED // "ERROR INITIALIZED"))) == 0 )


            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_MSG_ERROR_INIT"));

            break;

        case PQM_MSG_INVOKE_MEMORY:         //Invoke Memory  // "Invoke Memory"))) >= 0 )


            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_MSG_INVOKE_MEMORY"));

            if (!g_MainView->InvokeMemory()) {

                CPQMLogMgr::GetInstance()->WriteEventLog(
                    _T(""), ERROR_LOG_CRUCIAL1,
                    _T("IPCMessageHandler.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82


                CPQMLogMgr::GetInstance()->DisplayError(
                    CPQMUtility::GetMultiLingualString(_T("IDS_ERR_CLIENT_CONNECTION")),
                    _T("IPCMessageHandler.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                    ERR_ID, APP_NAME);
            }

            break;

            //added by Himanshu - for coil
        case PQM_MSG_PQM2COILSEL_CONNECT_COIL:
            //+Patni-Sribanta/2011Feb07/Modified/REDMINE-1026
            //g_MainView->GetConnectCoil();
            g_MainView->PostMessage(WM_PQM2COILSEL_CONNECT_COIL, 0, 0);
            //-Patni-Sribanta/2011Feb07/Modified/REDMINE-1026
            break;

        case PQM_MSG_INVOKE_PROBE_DIALOG:
            g_MainView->PostMessage(WM_INVOKE_PROBE_DIALOG, 0, 0);
            break;

        case PQM_MSG_PQM2COILSEL_SHOW_COIL:
            g_MainView->GetShowCoilList();
            break;

        case PQM_MSG_PQM2COILSEL_SELECTE_COIL:
            g_MainView->GetSelectCoilList();
            break;

        case PQM_MSG_LOC_BTN:
            g_MainView->EnableDisableLocatorButton();//ashish SM3
            break;

        case PQM_MSG_COILINFO_SHOW:
            g_MainView->DisplayCoilInfo();
            break;

            //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
        case PQM_MSG_CDS_GET_CENTER_POS:

            if (g_MainView) {
                g_MainView->CoilSelSetCenterPos();
            }

            break;

        case PQM_MSG_CDS_DEBUG:

            if (g_MainView && g_MainView->GetPQMCDSControllerInterface()) {


                //g_MainView->GetPQMCDSControllerInterface()->OpenCompassDebug();
                g_MainView->PostMessage(WM_CDS_LOCATION_DEBUG, 0, 0);
                //g_MainView->OpenCompassDebug();
            }

            break;

        case PQM_MSG_CDS_APPLY_COMPASS_COIL_TO_ALL_BELOW:

            if (g_MainView && g_MainView->GetPQMCDSControllerInterface()) {
                g_MainView->GetPQMCDSControllerInterface()->UpdateCompassCoilToBelowProtocols();
            }

            break ;

        case PQM_MSG_CDS_SELECT_CURRENT_PROTOCOL:

            if (g_MainView) {
                g_MainView->SelectCurrentProtocol();
            }

            break ;

        case PQM_MSG_CDS_COIL_CONFIRMATION :

            if (g_MainView && g_MainView->GetPQMCDSControllerInterface()) {

                //g_MainView->OpenCoilConfirmationDialog(0,0);
                g_MainView->PostMessage(WM_CDS_COIL_CONFIRMATION, 0, 0);
                //g_MainView->GetPQMCDSControllerInterface()->DisplayCoilSelforCOMPASSConfirmation();
            }

            break ;

        case PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT :

            if (g_MainView && g_MainView->GetPQMCDSControllerInterface()) {

                //g_MainView->PostMessage(WM_CDS_APPLY_CMPS_COIL_CUR_PROT,0,0);
                g_MainView->GetPQMCDSControllerInterface()->UpdateCompassCoilToCurrentProtocol();
            }

            break ;

        case PQM_MSG_CDS_UPDATE_AS_CMPS_FLAG_CUR_PROT :

            if (g_MainView && g_MainView->GetPQMCDSControllerInterface()) {
                g_MainView->GetPQMCDSControllerInterface()->UpdateAsCompassFlagForCurrentProtocol();
            }

            break ;

            //
            //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

        case PQM_MSG_SELECT_FIRST_WAIT_PROTOCOL:

            if (g_MainView) {
                g_MainView->SelectAndHighlightFirstWaitProtocol();
            }

            break ;


        case PQM_MSG_AMB_EXIT :

            g_MainView->RecAmbEditExit();
            break;

        case PQM_MSG_AMB_PQM_SCAN:

            g_MainView->RecAMBPqmScanMsg();
            break;

        case PQM_MSG_AMB_SCAN_WAIT:

            g_MainView->RecAmbScanWait();
            break;


        case PQM_MSG_AMB_SCAN_START_ERROR:

            g_MainView->RecAmbScanStartError();
            break;

        case PQM_MSG_AMB_REARRANGE:
            g_MainView->AMBRearrange();
            break;

        case PQM_MSG_AMB_SET_AUTO_SCAN:
            g_MainView->AMBApplyAutoScanToItems();
            break;

        case PQM_MSG_AMB_DUPLICATE_MASTERS:
            g_MainView->AMBSlaveDuplicate();
            break;

        case PQM_MSG_AMB_SLAVE_UPDATE:
            g_MainView->AMBUpdateSlaveprotocols();
            break;

        case PQM_MSG_AMB_ADD_SHIM_MAP:
            g_MainView->AMBAddShimMap();
            break;

        case PQM_MSG_AMB_UPDATE_STATUS_FAIL:
            g_MainView->UpdateAMBStatusFail();
            g_MainView->UpdatePQMGUIForSAROver();
            g_MainView->GetAcquisition()->HandleAbortFailForProtocol();//NFD010_UpdateIEC defect fix
            break;

        case PQM_MSG_AMB_SLAVE_INDEX_DELETE:
            g_MainView->DeleteAMBSlaveIndex();
            break;

        case PQM_MSG_AMB_SLAVE_SCAN_START:
            g_MainView->AMBSlaveScanStart();
            break;

        case PQM_MSG_AMB_AUTO_SCAN_START_FOR_FIRST_SLAVE_PROTOCOL:
            g_MainView->OnAMBAutoScanStartforFirstSlaveProtocol();
            break;

        case PQM_MSG_AMB_PARENT_IMAGE_DELETED:
            g_MainView->AMBParentImageRemovedfromProtocol();
            break;

        default:
            return TRUE;
    }

    return TRUE;
}
