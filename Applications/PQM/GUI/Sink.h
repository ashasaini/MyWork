//Redmine-783
//Redmine-775
//TMSC-REDMINE-778
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Sink.h
 *  Overview: Structure of CSink class.
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
#ifndef __SM_SINK_H__
#define __SM_SINK_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/IPCCom.h>
#include "PQMView.h"
#include "IPCMessageHandler.h"	//added by Hemant
#include "SRMessageHandler.h"	//added by Hemant
#include "AcqmanMessageHandler.h"	//added by Hemant

#import "PQM\IPCCom.tlb" raw_interfaces_only, named_guids

class CSink : public _IPqmIpcComEvents,
    private MR::ACQ::SM::COMMON::NonCopyable<CSink>
{
public:
    CSink();
    explicit CSink(CPQMView* pView);
    virtual ~CSink();

    HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID iid,
        void** ppvObject
    ) {
        if (iid == IID__IPqmIpcComEvents) {
            m_dwRefCount++;
            *ppvObject = (void*)this;
            return S_OK;
        }

        if (iid == IID_IUnknown) {
            m_dwRefCount++;
            *ppvObject = (void*)this;
            return S_OK;
        }

        return E_NOINTERFACE;
    }

private:

    DWORD m_dwRefCount;
    CPQMView* m_current_view;

    CAcqmanMessageHandler* m_acqmsghandler;
    CIPCMessageHandler* m_ipcmsghandler;
    CSRMessageHandler* m_srmsghandler;

    HRESULT STDMETHODCALLTYPE DisplayDebugData(BSTR f_displaydata);

    HRESULT STDMETHODCALLTYPE SendMsgToUI(
        int Msg
    ) {

        LPCTSTR FUNC_NAME = _T("CSink::SendMsgToUI") ;
        LPCTSTR ACQMSG = _T("m_acqmsghandler pointer is NULL") ;
        LPCTSTR IPCMSG = _T("m_ipcmsghandler pointer is NULL") ;
        LPCTSTR CURRVIEW = _T("m_current_view pointer is NULL") ;

        AfxSetResourceHandle(GetModuleHandle(NULL));
        pqm_msg_tag message = (pqm_msg_tag)Msg;
        CString str;
        str.Format(L"Sink Message : %d", message);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN,
                  FUNC_NAME,
                  str);

        switch (message) {
            case PQM_MSG_STATUS_CURRENT:// PROTOCOL_CURRENT
            case PQM_MSG_SCAN_START:    // SCAN_START
            case PQM_MSG_STATUS_DONE:   // PROTOCOL_DONE
            case PQM_MSG_ABORT_WAIT :   // ABORT_WAIT
            case PQM_MSG_ABORT_FAIL :   // ABORT_FAIL
            case PQM_MSG_CONTROL_DONE:  //AMCMA_CONTROL_DONE
            case PQM_MSG_PAUSE_DONE :   //PAUSE_DONE
            case PQM_MSG_RESUME_DONE:   //RESUME_DONE
            case PQM_MSG_ERROR:         //ERROR OCCURRED
            case PQM_MSG_APC_START:     // APC Start Pressed
            case PQM_MSG_APC_END:       // APC End
            case PQM_MSG_INJECT_TIME_VISIBLE:
            case PQM_MSG_INJECT_TIME_INVISIBLE:
            case PQM_MSG_INJECT_TIMER_VISIBLE:
            case PQM_MSG_INJECT_TIMER_DISPLAY:
            case PQM_MSG_SCAN_START_TRUE:
            case PQM_MSG_SCAN_START_BREATH_HOLD:
            case PQM_MSG_SCAN_START_FALSE:
            case PQM_MSG_SCAN_PAUSE_TRUE:
            case PQM_MSG_SCAN_PAUSE_FALSE:
            case PQM_MSG_SCAN_ABORT_TRUE:
            case PQM_MSG_SCAN_ABORT_FALSE:
            case PQM_MSG_SHIMMING_END:
            case PQM_MSG_CFA_PRESCAN_BEGIN:
            case PQM_MSG_CFA_PRESCAN_END:
            case PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END:
            case PQM_MSG_APC_RETRY_WAIT:
            case PQM_MSG_SR_ABORT_WAIT:
            case PQM_MSG_SR_ABORT_FAIL:
            case PQM_MSG_GATE_SCAN_END:
            case PQM_MSG_SENDING_STARTED:
            case PQM_MSG_SENDING_COMPLETED:
            case PQM_MSG_CFA_REDRAW_WATER:
            case PQM_MSG_CFA_REDRAW_FAT:
            case PQM_MSG_CFA_SET_FSY_FAT:
            case PQM_MSG_CFA_SET_FSY_WATER:
            case PQM_MSG_MRS_SCAN :
            case PQM_MSG_MRS_LOCALSHIM_BEGIN :
            case PQM_MSG_MRS_LOCALSHIM_DONE :
            case PQM_MSG_MRS_F0_BEGIN :
            case PQM_MSG_MRS_F0_DONE :
            case PQM_MSG_MRS_REF_BEGIN :
            case PQM_MSG_MRS_REF_DONE :
            case PQM_MSG_MRS_WSAT_BEGIN :
            case PQM_MSG_MRS_WSAT_DONE :
            case PQM_MSG_MRS_PRESCAN_DONE :
            case PQM_MSG_SCANSTART_ENABLE:
            case PQM_MSG_GATE_DIALOG_SHOW:
            case PQM_MSG_ACQMAN_CONNECTED:
            case PQM_MSG_ACQMAN_DISCONNECTED:
            case PQM_MSG_CORRECT_COIL:	// SM4 Himanshu CORRECT_COIL 15 Jan 2009
            case PQM_MSG_CFA_PRESCAN_ABORT:
            case PQM_MSG_CFA_PRESCAN_COUCH:
            case PQM_MSG_IRS_SCANSTART:   ///IRS evnt Handling Nitin
            case PQM_MSG_APC_RETRY_WAIT_ON_DISCONNECT:
            case PQM_MSG_SCAN_STARTPAUSE_DISABLE:
            case PQM_MSG_PROBE_SCAN_STARTPAUSE_DISABLE:
            case PQM_MSG_PROBE_SCAN_STARTPAUSE_ENABLE:
            case PQM_MSG_SRGL_SCANSTART:
            case PQM_MSG_CAN_AUTO_SCAN_START:
            case PQM_MSG_START_SCAN_FIRST_WAIT:
            case PQM_MSG_UPDATE_WAIT_FOR_AUTO_SCAN_ICON:
            case PQM_MSG_ACQINFO_ERROR:
            case PQM_MSG_CLOSE_WSE:

                if (NULL != m_acqmsghandler) {
                    m_acqmsghandler->ReceiveMessage(message);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              ACQMSG);

                }

                break;

            case PQM_MSG_INVOKE_LOCATOR:
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

            case PQM_MSG_CONNECT_FROM_MAIN_THREAD :	//+Patni-HEMANT/ADDED On 3/31/2009 2:59:02 PM/ PSP/ IDS#202, IDS#192
            case PQM_MSG_RESET_CURRENT_STATUS :		//+Patni-HEMANT/Added On 27Apr2009//Ace ITC-3 Internal Defect Fixing
            case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_SHOW:
            case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_OPEN:
            case PQM_MSG_PROCON_INSCAN_OPE_REQUEST_CLOSE:
            case PQM_MSG_PROCON_INSCAN_SWITCH:

                if (NULL != m_acqmsghandler) {
                    m_acqmsghandler->ReceiveMessage(message);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              ACQMSG);

                }

                break;

            case PQM_MSR_RCV_SEQ:       //  Ready to receive sequences
            case PQM_MSG_SEQ_RDY:       // Sequences are Ready
            case PQM_MSG_ERROR_INIT:    //ERROR INITIALIZED
            case PQM_MSG_INVOKE_MEMORY: //Invoke Memory
            case PQM_MSG_SEQ_RDY_DISPLAY:
            case PQM_MSG_SERIES_LOAD :
            case PQM_MSG_INVOKE_PROBE_DIALOG:
            case PQM_MSG_PQM2COILSEL_CONNECT_COIL:
            case PQM_MSG_PQM2COILSEL_SHOW_COIL:
            case PQM_MSG_PQM2COILSEL_SELECTE_COIL:
            case PQM_MSG_COILINFO_SHOW:
            case PQM_MSG_CDS_GET_CENTER_POS :
            case PQM_MSG_CDS_DEBUG :
            case PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT :
            case PQM_MSG_CDS_UPDATE_AS_CMPS_FLAG_CUR_PROT :
            case PQM_MSG_CDS_APPLY_COMPASS_COIL_TO_ALL_BELOW:
            case PQM_MSG_CDS_COIL_CONFIRMATION:
            case PQM_MSG_CDS_SELECT_CURRENT_PROTOCOL:
            case PQM_MSG_INSERT_MAP_PROTOCOL:
            case PQM_MSG_INSERT_SHIM_PROTOCOL:
            case PQM_MSG_PASCOM_REGIST_DONE:
            case PQM_MSG_SEQUENCE_TRANSFER_COMPLETED:

            case PQM_MSG_SELECT_FIRST_WAIT_PROTOCOL:
            case PQM_MSG_AMB_EXIT:
            case PQM_MSG_AMB_STATUS_MSG:
            case PQM_MSG_AMB_POPUP_MSG:
            case PQM_MSG_AMB_PQM_SCAN:
            case PQM_MSG_AMB_SCAN_START_ERROR:
            case PQM_MSG_AMB_SCAN_WAIT:
            case PQM_MSG_AMB_REARRANGE:
            case PQM_MSG_AMB_SET_AUTO_SCAN:
            case PQM_MSG_AMB_DUPLICATE_MASTERS:
            case PQM_MSG_AMB_SLAVE_UPDATE:
            case PQM_MSG_AMB_ADD_SHIM_MAP:
            case PQM_MSG_AMB_UPDATE_STATUS_FAIL:
            case PQM_MSG_AMB_SLAVE_INDEX_DELETE:
            case PQM_MSG_AMB_SLAVE_SCAN_START:
            case PQM_MSG_AMB_AUTO_SCAN_START_FOR_FIRST_SLAVE_PROTOCOL:
            case PQM_MSG_AMB_PARENT_IMAGE_DELETED:


                if (NULL != m_ipcmsghandler) {
                    m_ipcmsghandler->ReceiveMessage(message);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              IPCMSG);

                }

                break;

            case PQM_MSG_SCAN_PAUSE:
            case PQM_MSG_SCAN_RESUME:
            case PQM_MSG_IRS_SCANPAUSE:///IRS evnt Handling Nitin
            case PQM_MSG_IRS_SCANABORT:///IRS evnt Handling Nitin

                if (NULL != m_acqmsghandler) {
                    m_acqmsghandler->ReceiveMessage(message);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              ACQMSG);

                }

                break;

            case PQM_MSG_GL_EXAMUPDATE :
            case PQM_MSG_GL_SR_SQSAVED: //GL_SR_SQSaved

                if (NULL != m_srmsghandler) {
                    m_srmsghandler->ReceiveMessage(message);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              _T("m_srmsghandler pointer is NULL"));

                }

                break;

            case PQM_MSG_BEGINWAIT_CURSOR:

                if (NULL != m_current_view) {
                    m_current_view->SendMessage(WM_HANDLE_WAITCURSOR_MESSAGE, TRUE, NULL);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);

                }

                break;

            case PQM_MSG_ENDWAIT_CURSOR:

                if (NULL != m_current_view) {
                    m_current_view->PostMessage(WM_HANDLE_WAITCURSOR_MESSAGE, FALSE, NULL);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);

                }

                break;

            case PQM_MSG_BEGINWAIT_CURSOR_PQM_BUTTONS:

                if (NULL != m_current_view) {
                    m_current_view->PostMessage(WM_HANDLE_WAITCURSOR_MESSAGE_PQM_BUTTONS, TRUE, NULL);//RM-4721-Varun

                } else {
                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_ENDWAIT_CURSOR_PQM_BUTTONS:

                if (NULL != m_current_view) {
                    m_current_view->PostMessage(WM_HANDLE_WAITCURSOR_MESSAGE_PQM_BUTTONS, FALSE, NULL);//RM-4721-Varun

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);

                }

                break;

            case PQM_MSG_APC_PAUSE_END:

                if (NULL != m_current_view) {
                    m_current_view->ToggleAPCPauseMenu();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);

                }

                break;

            case PQM_MSG_INVOKE_REFPROBE:

                if (NULL != m_current_view) {
                    m_current_view->InvokeRefProbe();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);

                }

                break;

            case PQM_MSG_ENABLE_APPLY_BTN_MANUAL_PRESCAN:

                if (NULL != m_current_view) {
                    m_current_view->SetManualPreScanApplyBtnState(TRUE);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_MANUAL_PRESCAN_VALUES:

                if (NULL != m_current_view) {

                    m_current_view->GetManualPreScanDefaultData();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }


                break;

                //+ UT_JS@For pas button disable when map drive not found
            case PQM_MSG_MAP_DRIVE_NOT_FOUND:

                if (NULL != m_current_view) {
                    m_current_view->SetPASButtonState(FALSE);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

                //- UT_JS@For pas button disable when map drive not found
            case PQM_MSG_SAME_UID:

                if (NULL != m_current_view) {
                    m_current_view->SameStudyOpened();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_ENABLE_COUCHBUTTON:
                break;

            case PQM_MSG_DISABLE_COUCHBUTTON:
                break;

            case PQM_MSG_DBDT_CAUTION_DIALOG:

                if (NULL != m_current_view) {
                    m_current_view->DisplayDbDtCautionDialog();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_COIL_CHANGE_FOR_CURRENT_PROTOCOL:

                if (NULL != m_current_view) {
                    m_current_view->GetCoilStringForCurrentProtocol();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_SCANDONE_SHIMMING:

                if (NULL != m_acqmsghandler) {
                    m_acqmsghandler->ReceiveMessage(message);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              ACQMSG);

                }

                break;

            case PQM_MSG_LOADSTUDY_ERROR:
            case PQM_MSG_MDB_VDB_NOT_SYNC:

                if (NULL != m_current_view) {
                    m_current_view->PostMessage(WM_CMD_DBS_NOT_IN_SYNC, (PQM_MSG_LOADSTUDY_ERROR == message) ? TRUE : FALSE, NULL);

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);

                }

                break;

            case PQM_MSG_UPDATE_PAGER_AREA:

                if (NULL != m_current_view) {
                    m_current_view->UpdatePagerAreaFor();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_DISPLAY_WAIT_TIME:

                m_current_view->SetWaitTimeSARControlFlag(true);
                m_current_view->DisplayWaitTimeonAcquirePage();


                break;

            case PQM_MSG_DISPLAY_WAIT_TIME_END:

                m_current_view->SetWaitTimeSARControlFlag(false);
                m_current_view->ClearDPSTimer();

                break;

            case PQM_MSG_IRS_CFW_EF_LAST_IMAGE_DONE:

                if (NULL != m_current_view) {
                    m_current_view->EnableLocatorButton();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_UPDATE_COUCH_VAL:

                if (NULL != m_current_view) {
                    m_current_view->UpdateCouchVal();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case  PQM_MSG_ON_FLAG_LOCATOR:

                if (NULL != m_current_view) {
                    m_current_view->OnFlagOfLocatorEnable();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case  PQM_MSG_PARENT_IMAGE_DELETED_FOR_CURRENT:
                m_current_view->ParentImageRemovedFromCurrentProtocol();
                break;

            case PQM_MSG_REMOVE_AUTO_SCAN:
                m_current_view->RemoevAutoScanFromFirstWaitItem();
                m_current_view->UpdateEndExamButtonStatus();
                m_current_view->UpdateOKBtnStatusofWirelssDlg();
                m_current_view->UpdateCFARetry();
                break;

            case PQM_MSG_POST_PROCESS_START:

                m_current_view->SetPostProcessFlag(TRUE);
                m_current_view->UpdateEndExamButtonStatus();
                break;

            case PQM_MSG_POST_PROCESS_END:

                m_current_view->SetPostProcessFlag(FALSE);
                m_current_view->UpdateEndExamButtonStatus();
                m_current_view->UpdateOKBtnStatusofWirelssDlg();
                m_current_view->UpdateCFARetry();
                break;

            case PQM_MSG_DISABLE_PQM_GUI:
                m_current_view->EnableWindow(FALSE);
                break;

            case PQM_MSG_ENABLE_PQM_GUI:
                m_current_view->EnableWindow(TRUE);
                break;

            case PQM_MSG_CHANGE_SCAN_TIME:
                m_current_view->ChangeScanTime();
                break;

            case PQM_MSG_CFA_OK_BTN_STATE:
                m_current_view->SendMessage(WM_CFSHIFT_CFA_READY) ;
                break;

                //+Patni-PJS/2011Feb21/Added/IR-97
            case PQM_MSG_INSCAN_SCAN_START:
                m_current_view->InscanScanStart();
                break;
                //-Patni-PJS/2011Feb21/Added/IR-97

            case PQM_MSG_ACQ_MGR_BSY_ERR:
                m_acqmsghandler->ReceiveMessage(message);
                break;

                //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
            case PQM_MSG_MRSPROCESS_TOOL_CHECK_START:
                m_current_view->SetMRSProcessToolStarted(true);
                break;

            case PQM_MSG_MRSPROCESS_TOOL_CHECK_ENDS:
                m_current_view->SetMRSProcessToolStarted(false);
                break;

                //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
            case PQM_MSG_INVOKE_CALA_APPLICATION:
                m_current_view->GetAutoLocatorController().InvokeAutoLocatorApplication(AUTOLOCATOR_CARDIAC);
                break;

            case PQM_MSG_INVOKE_HALA_APPLICATION:
                m_current_view->GetAutoLocatorController().InvokeAutoLocatorApplication(AUTOLOCATOR_NEURO);
                break;

            case PQM_MSG_AUTO_COPY_DONE:
                m_current_view->UpdateGUICtrlAutoCopyMasterSlave();
                break;

            case PQM_MSG_MASTER_SLAVE_UPDATE_GUI:
                m_current_view->UpdateCurrentScanMasterSlave();
                break;

            case PQM_UPDATE_SAR_INFO:
                m_current_view->PQMMessageUpdateSarInfo();
                break;

            case PQM_MSG_UPDATE_GUI_FOR_SAR_OVER:

                if (m_current_view) {
                    m_current_view->UpdatePQMGUIForSAROver();
                }

                break;

            case PQM_MSG_START_SAR_UPDATE_TIMER:

                if (NULL != m_current_view) {
                    m_current_view->StartTimerForPager();

                } else {

                    PQM_TRACE(USER_FUNC_MARKER,
                              FUNC_NAME,
                              CURRVIEW);
                }

                break;

            case PQM_MSG_SARMANAGER_CONNECTED:

                if (m_current_view) {
                    m_current_view->HandleSARControlConnectionStatus(true);
                }

                break;

            case PQM_MSG_SARMANAGER_DISCONNECTED:

                if (m_current_view) {
                    m_current_view->HandleSARControlConnectionStatus(false);
                }

                break;

            default:
                return TRUE;

        }

        return FALSE;
    }

    ULONG STDMETHODCALLTYPE AddRef(
    ) {
        m_dwRefCount++;
        return m_dwRefCount;
    }

    ULONG STDMETHODCALLTYPE Release(
    ) {
        ULONG l = 0;

        l  = m_dwRefCount--;

        if (0 == m_dwRefCount) {
            delete this;
        }

        return l;
    }
};

#endif // #ifndef __SM_SINK_H__
