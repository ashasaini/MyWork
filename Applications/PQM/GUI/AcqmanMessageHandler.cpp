//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777

/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: AcqmanMessageHandler.cpp
 *  Overview: Implementation of CAcqmanMessageHandler class.
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
#include "AcqmanMessageHandler.h"
#include "PQMView.h"
#include "PQMUtility.h"
#include "PQMCFADlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CPQMView* g_MainView;

//************************************Method Header**************************************

//Method Name:  CAcqmanMessageHandler

//Author:  PATNI

//Purpose: Constructor

//**************************************************************************************/
CAcqmanMessageHandler::CAcqmanMessageHandler(
)
{
}

//************************************Method Header**************************************

//Method Name:  ~CAcqmanMessageHandler

//Author:  PATNI

//Purpose: Destructor

//**************************************************************************************/
CAcqmanMessageHandler::~CAcqmanMessageHandler(
)
{
}

//************************************Method Header**************************************

//Method Name:  ReceiveMessage

//Author:  PATNI

//Purpose: Receiving and processing of messages from Acqman

//**************************************************************************************/
BOOL CAcqmanMessageHandler::ReceiveMessage(
    const pqm_msg_tag Msg
)const
{
    LPCTSTR FUNC_NAME = _T("CAcqmanMessageHandler::ReceiveMessage");

    PQM_TRACE_SCOPED(FUNC_NAME);

    try {

        CString str;

        g_MainView->NotifyReceivedMessage(Msg);

        //Patni-ARD/2010Feb05/Removed l_seq initialize/Deft# MVC006963
        CSequence* l_seq = NULL;

        //+Patni-PJS/2011Feb1/Modify/IR-97
        CString l_string_acq_order = _T("");
        int l_acq_order = -1;

        //-Patni-PJS/2011Feb1/Modify/IR-97
        switch (Msg) {

            case PQM_MSG_STATUS_CURRENT:  // PROTOCOL_CURRENT  // ACQMAN_MSGID_PROTOCOL_CURRENT

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_STATUS_CURRENT"));

                g_MainView->GetPQMListCtrl().UpdateProtocolStatus(
                    g_MainView->GetScanIndex(), PR_ACQ_STATUS_CURRENT);

                g_MainView->DisplayTotalRemainingTimeOnPQM();

                //Patni-DKH/2010Dec03/commented/TMSC-REDMINE-979
                //g_MainView->SendScanMessageToSR(DPS_MSGID_REQ_SCANSTART,true);

                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break;

                //+Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979
            case PQM_MSG_SRGL_SCANSTART:
                g_MainView->SendScanMessageToSR(DPS_MSGID_REQ_SCANSTART, true);
                break;

                //-Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979
            case PQM_MSG_SCANDONE_SHIMMING:	//Patni-Manish/Added On 20March2009//PSP1#253
            case PQM_MSG_STATUS_DONE: { // PROTOCOL_DONE  // ACQMAN_MSGID_PROTOCOL_DONE)== 0)

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN,
                          FUNC_NAME,
                          (Msg == PQM_MSG_STATUS_DONE ? _T("PQM_MSG_STATUS_DONE") : _T("PQM_MSG_SCANDONE_SHIMMING")));

                str = _T("PROTOCOL_DONE");

                g_MainView->GetPQMListCtrl().UpdateProtocolStatus(
                    g_MainView->GetScanIndex(), PR_ACQ_STATUS_SUCCESS);

                //+Patni-PJS/2011Feb1/Modify/IR-97
                if (Msg == PQM_MSG_STATUS_DONE) {
                    l_seq = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(g_MainView->GetScanIndex());
                    CString l_string_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"));
                    int l_acq_order = _ttoi(l_string_acq_order);

                    if (g_MainView->IsProtocolInscan(l_acq_order)) {
                        g_MainView->ProcessScanRelatedMsgForWFDA(l_acq_order, PQM_MSG_STATUS_DONE);
                    }
                }

                //-Patni-PJS/2011Feb1/Modify/IR-97


                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("After updating protocol status on GUI"));

                int l_nxt_protocol = g_MainView->GetScanIndex() + 1;

                //+Patni-HAR/2010Mar05/Added/MSA00251-00241
                //+Patni-HAR/2010May12/Added/Code Review-MVC007927
                if (g_MainView->IsValidSequenceIndex(l_nxt_protocol)) {

                    //Patni-ARD/2010Feb05/Removed l_seq initialize/Deft# MVC006963
                    CSequence* l_seq  = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(l_nxt_protocol);

                    if (NULL != l_seq && l_seq->GetChildThreadStatus() != CT_DELETE) {

                        if (g_MainView->GetPQMListCtrl().GetItemCount() > g_MainView->GetScanIndex() + 1) {

                            //+Patni-Manish/Added On 19March2009//PSP1#253
                            if (Msg == PQM_MSG_STATUS_DONE) {
                                g_MainView->GetPQMListCtrl().SelectOnlyItem(l_nxt_protocol);
                                g_MainView->GetPQMListCtrl().EnsureVisible(l_nxt_protocol, true);
                            }

                            //Added by Hemant On 7/16/2008 10:58:37 AM
                            g_MainView->GetPQMListCtrl().ModifyStyle(LVS_SINGLESEL, 0);
                        }
                    }
                }

                //-Patni-HAR/2010May12/Added/Code Review-MVC007927

                //+Patni-HAR/2010Mar05/Added/MSA00251-00241

                //+Patni-SS/UnCOMMENTED/2009-May-15/JFT#139
                if (Msg == PQM_MSG_STATUS_DONE) {

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("[Before Setting IsScanning false]"));
                    g_MainView->SetIsScanning(false);
                }

                //+Patni-SS/UnCOMMENTED/2009-May-15/JFT#139
                g_MainView->SetAbortFlag(true);

                g_MainView->SendScanMessageToSR(DPS_MSGID_REQ_SCANEND);

                //+Patni-SS/Added/2009-May-15/IDS#242
                g_MainView->UpdateWindow();

                //+Patni-Hemant/2009Sept01/Added/Move_Couch and duplicate Error\IDS#826
                if (Msg == PQM_MSG_STATUS_DONE) {
                    //we are doing SetIsScanning(false); when this message is coming...
                    g_MainView->SetScanIndex(-1);
                }

                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
            }
            break;

            case PQM_MSG_ABORT_WAIT :

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_ABORT_WAIT"));

                //Added by Hemant for defect #809
                //When scan is not running and opening coil dialog, IPC receives
                //NACK message from Acqman, then IPC sends us ABORT_WAIT mesage to GUI
                //Now GUI will not proceed in processing this message as scan is not going on.
                if (!g_MainView->IsScanning()) {
                    break;
                }

                g_MainView->SetAbortFlag(true);

                str = _T("ABORT_WAIT");


                g_MainView->GetPQMListCtrl().UpdateProtocolStatus(g_MainView->GetScanIndex(), PR_ACQ_STATUS_WAIT);

                //+Patni-PJS/2011Feb1/Modify/IR-97
                l_seq = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(g_MainView->GetScanIndex());
                l_string_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"));
                l_acq_order = _ttoi(l_string_acq_order);

                if (g_MainView->IsProtocolInscan(l_acq_order)) {
                    g_MainView->ProcessScanRelatedMsgForWFDA(l_acq_order, PQM_MSG_ABORT_FAIL);
                }

                //-Patni-PJS/2011Feb1/Modify/IR-97
                g_MainView->DisplayAPCStartLabel(false);

                g_MainView->SetIsScanning(false);
                g_MainView->SetScanIndex(-1);

                g_MainView->SendMessage(WM_CMD_ABORT_WAIT, 0, 0);

                //Patni-PJS/2010Jun24/Added/JaFT#MVC008406
                g_MainView->BlinkPage();
                break;

            case PQM_MSG_SR_ABORT_WAIT :    // ABORT_WAIT  // ACQMAN_MSGID_ABORT_WAIT)) == 0)

                g_MainView->SendScanMessageToSR(DPS_MSGID_REQ_SCANEND);
                //UpdateProtocolStatus(selected_index,PR_ACQ_STATUS_FAIL);
                break;

            case PQM_MSG_APC_RETRY_WAIT	:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_APC_RETRY_WAIT"));

                g_MainView->SetAbortFlag(true);

                str = _T("ABORT_WAIT");


                g_MainView->GetPQMListCtrl().UpdateProtocolStatus(
                    g_MainView->GetScanIndex(), PR_ACQ_STATUS_WAIT);

                //Patni-PJS/2010Jul20/Added/MVC008406
                g_MainView->BlinkPage();
                g_MainView->SetIsScanning(false);
                g_MainView->SetFlagRelatedCtrlsStates();
                //-Patni-HM-MN/2009Sept01/Added/Code Review/Enable loc.,scanedit,endexam on APCRetry - MVC004921

                g_MainView->SendScanMessageToSR(DPS_MSGID_REQ_SCANEND);

                //Patni-HM-MN/2009Sept01/Commented/Code Review/Enable loc.,scanedit,endexam on APCRetry - MVC004921
                //g_MainView->SetIsScanning(false);

                //+Patni-Hemant/2009Sept01/Added/Move_Couch and duplicate Error\IDS#826
                g_MainView->SetScanIndex(-1);

                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                g_MainView->RemoevAutoScanFromFirstWaitItem();
                g_MainView->UpdateEndExamButtonStatus();
                break;

            case PQM_MSG_ABORT_FAIL: {

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_ABORT_FAIL"));

                if (!g_MainView->IsValidSequenceIndex(g_MainView->GetScanIndex())) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("Scan index is not valid"));

                    g_MainView->SetIsScanning(false);
                    g_MainView->SetScanIndex(-1);
                    return FALSE;
                }

                //Patni-Hemant/2009Dec03/Commented/MVC006052
                //g_MainView->SetCFWMsg(false);  //Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
                g_MainView->SetAbortFlag(true);

                g_MainView->GetAcquisition()->HandleAbortFailForProtocol();

                str = _T("ABORT_FAIL");

                g_MainView->GetPQMListCtrl().UpdateProtocolStatus(
                    g_MainView->GetScanIndex(), PR_ACQ_STATUS_FAIL);

                //+Patni-PJS/2011Feb1/Modify/IR-97
                l_seq = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(g_MainView->GetScanIndex());
                l_string_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"));
                l_acq_order = _ttoi(l_string_acq_order);

                if (g_MainView->IsProtocolInscan(l_acq_order)) {
                    g_MainView->ProcessScanRelatedMsgForWFDA(l_acq_order, PQM_MSG_ABORT_FAIL);
                }

                //-Patni-PJS/2011Feb1/Modify/IR-97

                const int l_next_to_scan_index = g_MainView->GetScanIndex() + 1;

                if (g_MainView->GetPQMListCtrl().GetItemCount() > l_next_to_scan_index) {
                    //+ Patni-PJS/2009Nov18/Added/MSA0248-00011
                    if (g_MainView->GetScanIndex() != -1) {

                        g_MainView->GetPQMListCtrl().SelectOnlyItem(l_next_to_scan_index);

                        g_MainView->GetPQMListCtrl().EnsureVisible(l_next_to_scan_index, true);

                        //+Patni-HAR/2010Mar05/deleted/MSA00251-00241
                        g_MainView->GetPQMListCtrl().UpdateProtocolStatus(l_next_to_scan_index, PR_ACQ_STATUS_WAIT);
                    }

                    //- Patni-PJS/2009Nov18/Added/MSA0248-00011
                }

                g_MainView->SetIsScanning(false);
                g_MainView->SetScanIndex(-1);

                g_MainView->SendMessage(WM_CMD_ABORT_FAIL, 0, 0);

                //Patni-PJS/2010Jun24/Added/JaFT#MVC008406
                g_MainView->BlinkPage();
            }
            break;

            case PQM_MSG_SR_ABORT_FAIL:

                g_MainView->SendScanMessageToSR(DPS_MSGID_REQ_SCANEND);
                break;

            case PQM_MSG_CONTROL_DONE:      //AMCMA_CONTROL_DONE // ACQMAN_MSGID_CONTROL_DONE)) == 0)

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_CONTROL_DONE"));

                str = _T("AMCMA_CONTROL_DONE");


                break;

            case PQM_MSG_PAUSE_DONE :       //PAUSE_DONE // ACQMAN_MSGID_PAUSE_DONE)) == 0)

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_PAUSE_DONE"));

                str = _T("PAUSE_DONE");


                //Added by Hemant On 7/18/2008 9:13:39 PM
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);

                break;

            case PQM_MSG_RESUME_DONE:       //RESUME_DONE //ACQMAN_MSGID_RESUME_DONE)) == 0)

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_RESUME_DONE"));

                str = _T("RESUME_DONE");

                //Added by Hemant On 7/18/2008 9:13:39 PM
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);

                break;


            case PQM_MSG_ERROR: //ERROR OCCURRED // ACQMAN_MSGID_ERROR_OCCURRED)) == 0)

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_ERROR"));

                //commented by Hemant - not needed for new GUI - will implement differently
                //g_MainView->SelectPreviousSelectedRows();

                g_MainView->GetPQMListCtrl().UpdateProtocolStatus(
                    g_MainView->GetScanIndex(), PR_ACQ_STATUS_FAIL);

                break;

            case PQM_MSG_SCAN_PAUSE:
            case PQM_MSG_IRS_SCANPAUSE:
            case PQM_MSG_SCAN_RESUME:
            case PQM_MSG_IRS_SCANABORT:
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break;

            case PQM_MSG_SCAN_START:  // SCAN_START // ACQMAN_MSGID_SCAN_START)) == 0)
            case PQM_MSG_IRS_SCANSTART:
            case PQM_MSG_SCAN_START_BREATH_HOLD:		//added by Hemant

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_SCAN_START"));
                //Added by Hemant On 7/18/2008 9:13:39 PM
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);

                break;

                //+Patni-MP/2009Apr1/Commented/ACE-1/MCM0213-00167
            case PQM_MSG_INVOKE_LOCATOR:
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break;

                //-Patni-MP/2009Apr1/Commented/ACE-1/MCM0213-00167
            case PQM_MSG_APC_START:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_APC_START"));
                g_MainView->DisplayAPCStartLabel(true);

                break;

            case PQM_MSG_APC_END:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_APC_END"));
                g_MainView->DisplayAPCStartLabel(false);

                //+Patni-Hemant/2009Dec1/Commented/MVC005971
                //Following code will enable the scan start button when doing APC pause and then pressing APC Retry button.
                //
                ////+Patni-HM-MN/2009Sept01/Added/Code Review/Enable loc.,scanedit,endexam on APCRetry - MVC004921
                //g_MainView->SetIsScanning(false);
                //g_MainView->SetFlagRelatedCtrlsStates();
                ////-Patni-HM-MN/2009Sept01/Added/Code Review/Enable loc.,scanedit,endexam on APCRetry - MVC004921
                //-Patni-Hemant/2009Dec1/Commented/MVC005971

                break;

            case PQM_MSG_CFA_PRESCAN_END:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_CFA_PRESCAN_END"));

                g_MainView->SetWaitCursor(false);
                g_MainView->SetIsScanning(false);

                //+Patni-HEMANT/ADDED On 9/12/2009 4:45:17 PM/ IDS#826
                g_MainView->SetScanIndex(-1);
                g_MainView->SetFlagRelatedCtrlsStates();

                g_MainView->GetCFADlgPtr()->ProcessShimmingMsg(Msg);

                g_MainView->UpdateOKBtnStatusofWirelssDlg();

                //Patni-HAR/2010Jun28/Added/V1.20#MVC008392
                g_MainView->BlinkPage();

                break;

                //+NFD001 Autoshimming Update GUI controls after Shimming post processing done in case CFA graph is not dispalyed
            case PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END"));
                g_MainView->SetWaitCursor(false);
                g_MainView->EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
                break;

                //-NFD001 Autoshimming

            case PQM_MSG_CFA_PRESCAN_COUCH:
            case PQM_MSG_CFA_PRESCAN_ABORT:

                //+Patni-HEMANT/ADDED On 9/12/2009 4:45:17 PM/ IDS#826
                g_MainView->SetScanIndex(-1);
                g_MainView->SetIsScanning(false);
                g_MainView->SetFlagRelatedCtrlsStates();

                g_MainView->GetCFADlgPtr()->ProcessShimmingMsg(Msg);

                g_MainView->SetWaitCursor(false);
                //Patni-PJS/2010Jun24/Added/JaFT#MVC008406
                g_MainView->BlinkPage();
                g_MainView->UpdateEndExamButtonStatus();

                break;

            case PQM_MSG_GATE_SCAN_END: {

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                          _T("PQM_MSG_GATE_SCAN_END"));
            }
            break;

            case PQM_MSG_GATE_DIALOG_SHOW: {
                g_MainView->DisplayGateDialog();
            }
            break;

            case PQM_MSG_CAN_AUTO_SCAN_START: {

                if (g_MainView->IsScanning()) {
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Scan is running..."));
                    return true;
                }

                const int l_acq_order = g_MainView->GetAcqOderForGivenProtocolIndex(g_MainView->GetIndexForAutoScanProt());

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, (-1 == l_acq_order) ? _T("Can Not Start Auto Scan") : _T("Can Start Auto Scan"));

                if ((S_OK != g_MainView->GetAcquisition()->SetFirstWaitProtForAutoScan(l_acq_order))) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetFirstWaitProtForAutoScan Failed"));
                }

                g_MainView->UpdateEndExamButtonStatus();
            }
            break;

            case PQM_MSG_CFA_REDRAW_WATER:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN,
                          FUNC_NAME,
                          _T("PQM_MSG_CFA_REDRAW_WATER"));
                g_MainView->SetShimStateFlag(VFC_SHIM_FSY_WATER);
                g_MainView->SetShimDoneFlag(CFA_DLG_SHIMMING_SEQUENCE);

                g_MainView->SetIsScanning(false);
                //+Patni-Sribanta/2011Feb10/Modified/REDMINE-1026
                //g_MainView->SendMessage(WM_CMD_CFA_SHOW, 0, 0);
                g_MainView->PostMessage(WM_CMD_CFA_SHOW, 0, 0);
                //+Patni-Sribanta/2011Feb10/Modified/REDMINE-1026

                break;

            case PQM_MSG_CFA_REDRAW_FAT:

                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN,
                          FUNC_NAME,
                          _T("PQM_MSG_CFA_REDRAW_FAT"));
                g_MainView->SetShimStateFlag(VFC_SHIM_FSY_FAT);
                g_MainView->SetShimDoneFlag(CFA_DLG_SHIMMING_SEQUENCE);

                g_MainView->SetIsScanning(false);

                //+Patni-Sribanta/2011Feb10/Modified/REDMINE-1026
                //g_MainView->SendMessage(WM_CMD_CFA_SHOW, 0, 0);
                g_MainView->PostMessage(WM_CMD_CFA_SHOW, 0, 0);
                //-Patni-Sribanta/2011Feb10/Modified/REDMINE-1026

                break;

            case PQM_MSG_CFA_SET_FSY_FAT:
                g_MainView->SetShimStateFlag(VFC_SHIM_FSY_FAT);
                g_MainView->GetCFADlgPtr()->SetPreviousShimFlag(VFC_SHIM_FSY_FAT);
                break;

            case PQM_MSG_CFA_SET_FSY_WATER:
                g_MainView->SetShimStateFlag(VFC_SHIM_FSY_WATER);
                g_MainView->GetCFADlgPtr()->SetPreviousShimFlag(VFC_SHIM_FSY_WATER);
                break;

                // + Patni-Sribanta/2010Jun09/Modified/MVC8032
                /*
                        case PQM_MSG_MULTICOVERAGE:
                //+Patni-SS/COMMENTED-2009May13/JFT End Exam Requirement
                //g_MainView->SetBtnSensitive(IDC_CLOSE,FALSE);
                g_MainView->SetBtnSensitive(IDC_END_EXQAM, FALSE);
                //+Patni-SS/COMMENTED-2009May13/JFT End Exam Requirement
                g_MainView->SetBtnSensitive(IDC_DELETE_BUTTON, TRUE);
                break;
                */
                // - Patni-Sribanta/2010Jun09/Modified/MVC8032
                //+saurabh
            case PQM_MSG_SCAN_STARTPAUSE_DISABLE:
                g_MainView->SetBtnSensitive(IDC_SCAN_START, FALSE);
                g_MainView->SetBtnSensitive(IDC_PAUSE_RESUME, FALSE);
                break;
                //-saurabh

                //+ Patni-MSN/2010Mar17/Modified/RMC - Phase3_10
            case PQM_MSG_PROBE_SCAN_STARTPAUSE_DISABLE:
                g_MainView->SetRMCScanRunning(true);
                //+Patni-KSS/2010Sep02/MODIFIED/TMSC-REDMINE-558-REOPEN
                //g_MainView->SetBtnSensitive(IDC_SCAN_START, FALSE);
                //g_MainView->SetBtnSensitive(IDC_PAUSE_RESUME, FALSE);
                //return TRUE;
                break;
                //-Patni-KSS/2010Sep02/MODIFIED/TMSC-REDMINE-558-REOPEN

            case PQM_MSG_PROBE_SCAN_STARTPAUSE_ENABLE:
                g_MainView->SetRMCScanRunning(false);
                //+Patni-KSS/2010Sep02/MODIFIED/TMSC-REDMINE-558-REOPEN
                //g_MainView->SetBtnSensitive(IDC_SCAN_START, TRUE);
                //g_MainView->SetBtnSensitive(IDC_PAUSE_RESUME, FALSE);
                //return TRUE;
                break;
                //-Patni-KSS/2010Sep02/MODIFIED/TMSC-REDMINE-558-REOPEN

            case PQM_MSG_SCAN_ABORT_TRUE:
                g_MainView->SetBtnSensitive(IDC_ABORT, TRUE);
                g_MainView->SetAbortFlag(false);
                return TRUE;
                //- Patni-MSN/2010Mar17/Modified/RMC - Phase3_10

            case PQM_MSG_SCANSTART_ENABLE:
                g_MainView->SetBtnSensitive(IDC_SCAN_START, TRUE);
                g_MainView->SetBtnSensitive(IDC_COIL_BUTTON, TRUE); //Patni-Manish/2009Jun8/Added/JFT#250-Enable COIL button
                //g_MainView->SetBtnSensitive(IDC_PAUSE_RESUME,TRUE);	//Commented by Ashish SM3 23/09/2008

                break;
                //Ashish SM3

                //for MRS scan messages..
            case PQM_MSG_MRS_SCAN :
            case PQM_MSG_MRS_LOCALSHIM_BEGIN :
            case PQM_MSG_MRS_LOCALSHIM_DONE :
            case PQM_MSG_MRS_F0_BEGIN :
            case PQM_MSG_MRS_F0_DONE :
            case PQM_MSG_MRS_REF_BEGIN :
            case PQM_MSG_MRS_REF_DONE :
            case PQM_MSG_MRS_WSAT_BEGIN :
            case PQM_MSG_MRS_WSAT_DONE :
            case PQM_MSG_MRS_PRESCAN_DONE:

                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break ;

                // new message added for proCon server
            case PQM_MSG_PROCON_QUEUE_NEXT:
            case PQM_MSG_PROCON_QUEUE_EXIT:
            case PQM_MSG_PROCON_NEXT_COPY:
            case PQM_MSG_PROCON_RESET:
            case PQM_MSG_PROCON_ERR:
            case PQM_MSG_PROCON_CMD_EXIT:
            case PQM_MSG_PROCON_CONNECTED:
            case PQM_MSG_PROCON_NOT_CONNECTED:
            case PQM_MSG_PROCON_QUEUE_DONE:
            case AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL:

            case PQM_MSG_PROCON_CON_ALLOW_SELECTIONS :		//+Patni-HEMANT/ADDED On 3/6/2009 9:12:24 PM/ #232
            case PQM_MSG_PROCON_CON_INHIBIT_SELECTIONS :	//+Patni-HEMANT/ADDED On 3/6/2009 9:12:24 PM/ #232

            case PQM_MSG_CONNECT_FROM_MAIN_THREAD :	//+Patni-HEMANT/ADDED On 3/31/2009 3:02:57 PM/ PSP/ IDS#202, IDS#192
            case PQM_MSG_START_SCAN_FIRST_WAIT:
            case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_SHOW:
            case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN:
            case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_CLOSE:
            case PQM_MSG_PROCON_INSCAN_SWITCH:

                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);

                break;

            case PQM_MSG_ACQMAN_CONNECTED:
            case PQM_MSG_ACQMAN_DISCONNECTED:
            case PQM_MSG_ACQINFO_ERROR:

                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break;

                ///MRP here
            case PQM_MSG_SCAN_START_TRUE :
                g_MainView->SetDynamicScan(true);
                break;

                ///End of addition
                /////////End of addition
                // + SM4 Himanshu CORRECT_COIL 15 Jan 2009
            case PQM_MSG_CORRECT_COIL:
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break;

                // - SM4 Himanshu CORRECT_COIL 15 Jan 2009
                //+Patni-ARD/2010Feb05/Added/Deft# MVC006963
            case PQM_MSG_APC_RETRY_WAIT_ON_DISCONNECT :

                //Patni-Hemant/2011Jan11/Commented/REDMINE-1095_Reopen
                //g_MainView->SetCanAPCPause(APC_EnableFromPQM);

                //-Patni-ARD/2010Feb05/Added/Deft# MVC006963
                //+Patni-PJS/2010Dec16/Added/REDMINE-1098
                break;

            case PQM_MSG_ACQ_MGR_BSY_ERR:
                g_MainView->SetScanIndex(-1);
                break;

                //-Patni-PJS/2010Dec16/Added/REDMINE-1098

            case PQM_MSG_INJECT_TIMER_VISIBLE:
                g_MainView->DisplayTimer();
                break;

            case PQM_MSG_INJECT_TIMER_DISPLAY:
                g_MainView->CalculateTimeDiff();
                g_MainView->DisplayTimer();
                break;

            case PQM_MSG_INJECT_TIME_INVISIBLE:
                g_MainView->ShowTimerButton();
                break;

            case PQM_MSG_UPDATE_WAIT_FOR_AUTO_SCAN_ICON:
                g_MainView->UpdateWaitForAutoScanIcon();
                g_MainView->OnMessageFromAcqmanMsgHandler(Msg);
                break;

            case PQM_MSG_CLOSE_WSE:
                g_MainView->CloseWSE();

            default:
                return TRUE;
        }

        //+Patni-HEMANT/ADDED On 4/6/2009 1:18:42 PM/ PSP / Internal fix
        //we need it over here, we are getting ABORT_WAIT two times,
        //first time, its OK. But for second time the message is not procesed
        //inside case statements..so we need it here
        g_MainView->EnableDisableScanRelatedButtonsForSelectedProtocolStatus();
        //-Patni-HEMANT/ADDED On 4/6/2009 1:18:42 PM/ PSP / Internal fix

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION Occured"));
        throw;
    }

    return FALSE;
}




