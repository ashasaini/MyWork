/*****************************************************************************
*
*  (c) Copyright 2011 TOSHIBA Corporation. All Rights Reserved
*
*  File Name: DPSManager.cpp
*  Overview: Structure of DPSManager class.
*  Last Modified:
*****************************************************************************
*                       Revision History
*****************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*
 *****************************************************************************/

#include "stdafx.h"
#include "DPSManager.h"
#include <libStudyManager/destroy.h>
#include <libStudyManager/CFW/CFWReceiveMsg.h>
#include <libStudyManager/CFW/CFWReplyMsg.h>
#include "PQMView.h"
#include "PQMUtility.h"
#include "WFDAMsgReceiver.h"



using MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWListener;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


extern CPQMView* g_MainView;



//************************************Method Header**************************************

//Method Name:  Initialize

//Author:  PATNI

//Purpose: Create new connector for communication with DPS

//**************************************************************************************/
bool CDPSManager::Initialize(
)
{
    LPCTSTR FUNC_NAME = _T("CDPSManager::Initialize");
    const CString NAME = CPQMUtility::GetInstance()->GetPQMParentName() + CString(_T(".")) + CPQMUtility::GetInstance()->GetApplicationName();

    if (FALSE == CCFWListener::Initialize(NAME, NAME, CPQMUtility::GetInstance()->GetPQMParentName(), CPQMLogMgr::GetInstance())) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME,
                  _T("CDPSManager::Initialize Failed"));

        g_MainView->SetConnectorState(FALSE);

        return false;
    }

    return true;
}

//************************************Method Header**************************************

//Method Name:  HaveToShowSR

//Author:  TMSC

//Purpose: Checking to show SRGL on FW_Hide/FW_Show

//**************************************************************************************/
bool CDPSManager::HaveToShowSR() const
{
    return ((!m_is_recv_GL_SR_ShowPQM)
            && CPQMUtility::GetInstance()->GetPQMParentName() == _T("Acquire"));
}

//************************************Method Header**************************************
//Method Name:  CDPSManager

//Author:  PATNI

//Purpose: Constructor

//**************************************************************************************/
CDPSManager::CDPSManager(
): CCFWListener(),
    m_is_recv_GL_SR_ShowPQM(false)
{
    this->Attach(this);

}

//************************************Method Header**************************************

//Method Name:  ~CDPSManager

//Author:  PATNI

//Purpose: Destructor

//**************************************************************************************/
CDPSManager::~CDPSManager(
)
{

}

//************************************Method Header****************************
//Method Name:  RecvCallBack
//Author:  PATNI
//Purpose: Receiving and processing of messages from DPS
//****************************************************************************/
BOOL CDPSManager::RecvCallBack(const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
                               MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg* const replyMsg,
                               MR::ACQ::SM::COMMON::CFW::CCFWListener* const listener)

{
    LPCTSTR FUNC_NAME = _T("CCDPSManager::RecvCallBack");

    TRY {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS,
        FUNC_NAME,
        _T("Trace of RecvCallBack"));


        const CString& strcmd = recvMsg.GetCommand();
        const CString& strparam = recvMsg.GetParameter();
        const CString& sender = recvMsg.GetSender();
        CString senderaddr = recvMsg.GetFromAddress();
        const CString& strcustom = recvMsg.GetCustom();



        CString str;
        str.Format(_T("cmd= %s, param= %s,sender = %s, custom = %s"), strcmd, strparam, sender, strcustom);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, str);

        CPQMUtility::GetInstance()->SetSenderParam(strparam);


        // + Patni - HAR / 2009Mar19 / Actual SR Changes
        if ((senderaddr == _T("Schedule.SRGL"))) {
            senderaddr = _T("Schedule");
        }

        // - Patni - HAR / 2009Mar19 / Actual SR Changes

        if (strcmd == _T("GL_SR_ShowPQM"))
            //Defect # 740 by Manish 03/02/09
        {
            //if an instance of PQM is opened, it will return to scheduled page
            if (false == g_MainView->GetPQMCloseFlag()) {

                g_MainView->SwitchPage(DPS_PAGENAME_SCHEDULE);

                //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                _T("PQM is already opened!"));

                //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179

                // MEITEC/2010May15/Modified/MVC007802
                return FALSE;
            }

            //Defect # 740 by Manish 03/02/09
            m_is_recv_GL_SR_ShowPQM = true;
            CString l_strparam = strparam;
            //+Patni-PJS/2010Nov4/Added/REDMINE-859/Error Scenario Fixed
            l_strparam.TrimLeft();
            l_strparam.TrimRight();

            //+Patni-PJS/2010Oct27/Modified/REDMINE-859
            if (l_strparam.IsEmpty() || FALSE == g_MainView->SetSRParam(l_strparam, strcustom)) {

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    CPQMUtility::GetMultiLingualString(_T("IDS_FAILED_TO_LOAD_STUDY")),
                    MPlus::EH::Information, _T("PQM"), _T("PQM"));

                g_MainView->PostMessage(WM_CMD_FILE_NOT_EXIST);
                return FALSE;
            }

            //-Patni-PJS/2010Oct27/Modified/REDMINE-859

            CDPSManager::GetInstance()->ReplyMsgAtNotLast(recvMsg, g_MainView->GetSRParam());

            g_MainView->PostMessage(WM_CMD_GL_SR_SHOW);


        } else if (strcmd == DPS_MSGID_REQ_SHOW) {

            CDPSManager::GetInstance()->ReplyMsgAtNotLast(recvMsg, _T("S_OK"));

            g_MainView->PostMessage(DPS_MSGID_REP_SHOW);

        } else if (strcmd == DPS_MSGID_REQ_HIDE) {

            CDPSManager::GetInstance()->ReplyMsgAtNotLast(recvMsg, _T("S_OK"));

            g_MainView->PostMessage(DPS_MSGID_REP_HIDE);

        } else if (strcmd == _T("RE_FW_RequestShowGL")) {

        } else if (strcmd == _T("RE_FW_RequestHideGL")) {

            if (g_MainView->GetPQMCloseFlag()) {
                g_MainView->SendMessage(DPS_MSGID_REP_HIDE, reinterpret_cast<WPARAM>(&recvMsg),
                                        reinterpret_cast<LPARAM>(replyMsg));

                g_MainView->GetPQMListCtrl().DeleteAllItems();
                g_MainView->CleanUp();
            }

            //+Patni+MP/2010Mar10/Modified/MSA0251-00243

        } else if (strcmd == DPS_MSGID_REP_LOADGL) {

            if (strparam.CompareNoCase(_T("E_FAIL")) == 0) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("LoadGL failed!"));

                if (true == g_MainView->GetAutoLocatorController().GetLoadGLFlag()) {
                    HANDLE l_autolocator_wait = OpenEvent(EVENT_MODIFY_STATE, FALSE, PQM_AUTOLOCATOR_WAIT_FOR_REPLY);

                    if (NULL != l_autolocator_wait) {
                        SetEvent(l_autolocator_wait);
                        CloseHandle(l_autolocator_wait);
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENT SET as Handle closed as LOADGL reply is E_FAIL"));
                    }
                }

                if ((g_MainView->GetMemInvocationProcessSt() | MEMORY_INVOKATION_STARTS) == MEMORY_INVOKATION_STARTS) {

                    g_MainView->SetMemoryAliveFlag(false);
                    g_MainView->SetWaitCursor(false);
                    g_MainView->SetMemInvocationProcessSt((g_MainView->GetMemInvocationProcessSt())& MEMORY_INVOKATION_ENDS);
                    g_MainView->UpdateEndExamButtonStatus();
                }

            } else if (strparam.CompareNoCase(_T("S_OK")) == 0) {
                if (true == g_MainView->GetAutoLocatorController().GetLoadGLFlag()) {
                    g_MainView->GetAutoLocatorController().SetLoadGLFlag(false);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech LOADGL flag is reset"));
                }

                // Invoke successfully
            }

            //Patni-MP/2010Mar10/Modified/MSA0251-00243

        } else if (strcmd == DPS_MSGID_REQ_CLOSE) {

            //+ Patni-HAR 2009Mar06 / Added / Page Reset
            if (!strcustom.CompareNoCase(_T("PageReset/SA"))) {
                g_MainView->UpdatePagerArea(_T("FW_ACQINFO_CLR"), _T("1"), _T(""));
                g_MainView->SendMessageToProcon(PQM_PROCON_PRO_END, false, true);
                // - Patni-HAR 2009Mar06 / Added / Page Reset

            } else {
                // + SM4 DEFECT 168/424 Himanshu Fix
                g_MainView->SendMessageToProcon(PQM_PROCON_TERMINATE, false, true);
            }

            // - SM4 DEFECT 168/424 Himanshu Fix
            g_MainView->SendMessage(DPS_MSGID_REP_CLOSE, reinterpret_cast<WPARAM>(&recvMsg),
                                    reinterpret_cast<LPARAM>(replyMsg));

        } else if (strcmd == DPS_MSGID_REP_REQUESTCLOSE) {

            g_MainView->SendMessage(DPS_MSGID_REP_CLOSE, reinterpret_cast<WPARAM>(&recvMsg),
                                    reinterpret_cast<LPARAM>(replyMsg));



        } else if (strcmd == DPS_MSGID_REQ_CHECKSTATE) {


            CDPSManager::GetInstance()->ReplyMsgAtLast(recvMsg, replyMsg,
                    CPQMUtility::GetInstance()->GetPQMStatusString());

        } else if (strcmd == _T("Close_Memory")) {

            g_MainView->SetMemoryAliveFlag(false);
            g_MainView->SetFlagRelatedCtrlsStates();

            //+Patni-HAR/2010Jun28/Added/V1.20#MVC008324_0
            CString l_log_string(_T(""));
            l_log_string.Format(_T("[Memory Invocation State : %d]"),
                                g_MainView->GetMemInvocationProcessSt());

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);
            //-Patni-HAR/2010Jun28/Added/V1.20#MVC008324_0

            //+Patni-PJS/2010Mar10/Modified/MSA0251-00243
            if ((g_MainView->GetMemInvocationProcessSt() | MEMORY_INVOKATION_STARTS) == MEMORY_INVOKATION_STARTS) {
                //Patni-HAR/2010Jun28/Added/V1.20#MVC008324_0
                //g_MainView->SetWaitCursor(false);
                g_MainView->SetMemInvocationProcessSt((g_MainView->GetMemInvocationProcessSt())& MEMORY_INVOKATION_ENDS);
            }

            //-Patni-PJS/2010Mar10/Modified/MSA0251-00243
            if (FALSE == g_MainView->GetRegistFlag()) {

                g_MainView->SetWaitCursor(false);
                g_MainView->SetRegistFlag(-1);
            }
        }

        //+Patni-HEMANT/ADDED On 3/27/2009 1:56:29 PM/ PSP#311
        else if (strcmd == _T("Another_Instance_Running_Of_Memory")) {

            if (!g_MainView->GetMemoryAliveFlag()) {

                //this is necessary though we have already this flag as false..
                g_MainView->SetMemoryAliveFlag(false);
                //Patni-PJS/2010Mar10/Modified/MSA0251-00243
                g_MainView->SetMemInvocationProcessSt((g_MainView->GetMemInvocationProcessSt())& MEMORY_INVOKATION_ENDS);
                //Memory sends this msg to PQM also when Memory is not able to open correctly,
                //[one isntace is running]
                g_MainView->SetWaitCursor(false);
            }
        }

        //-Patni-HEMANT/ ADDED On3/27/2009 1:56:45 PM/ PSP#311
        else if (strcmd == _T("Invoke_Memory")) {

            if (g_MainView->GetMemorymode()) {
                g_MainView->SendNotifyToMemory();
            }

            g_MainView->SetMemoryMode(false);

            g_MainView->SetMemoryAliveFlag(true) ;
            //Patni-PJS/2010Mar10/Modified/MSA0251-00243
            g_MainView->SetMemInvocationProcessSt((g_MainView->GetMemInvocationProcessSt())& MEMORY_INVOKATION_ENDS);

        } else if (strcmd == _T("File_Open")) {

            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_STUDY_OPEN,
                    _T("IDS_STUDY_OPEN"),
                    SCAN_ERR_NOTIFY, _T("CDPSManager.cpp"));

        } else if (strcmd == _T("FW_EnableButton")) {
            if (strparam == _T("ScanStart")) {

                //if(sender == _bstr_t(_T("AcqManGL"))){ // + Patni-Manish/2009Aug07/ACE2-ITC204/Commented the code.
                if (recvMsg.GetCustom() == _T("true")) {
                    g_MainView->SendMessage(WM_CMD_SCAN_START, (WPARAM)true, NULL);

                } else {
                    g_MainView->SendMessage(WM_CMD_SCAN_START, (WPARAM)false, NULL);
                }

                //-Patni-Manish/2009Aug07/ACE2-ITC204/Commented the code.
            }

            //Patni-DKH/2010Jan29/Added/PH#3#/FBI

        } else if (strcmd == _T("GL_FBI_SEND_DELAY")) {
            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("GL_FBI_SEND_DELAY message recived"));

            CString l_studyloid = _T("");
            int l_delay1 = -1 , l_delay2 = -1;
            //Patni-AMT/2010May04/Modified/JaFT# REDMINE-209
            BOOL l_ret = GetFBIDelayInfo(strparam, l_studyloid, l_delay1, l_delay2);

            //+Patni-DKH/2010Feb04/Added/PH#3#/IR86/DebugWindow
            CString l_debugstr = _T("");
            l_debugstr.Format(_T("IRS_EF_FBI_NOTIFY: delay=(%d, %d), Study LOID=%s"), l_delay1, l_delay2, l_studyloid);
            g_MainView->GetPqmDebugWinDlg().UpdateDebugData(l_debugstr);
            //-Patni-DKH/2010Feb04/Added/PH#3#/IR86/DebugWindow

            if (0 != l_studyloid.CompareNoCase(g_MainView->Getstudyloid())) {

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    CPQMUtility::GetMultiLingualString(_T("IDS_PQM_FBI_DELAY_STUDY")),
                    MPlus::EH::Information, _T("PQM"), _T("PQM"));

                l_delay1 = -1;
                l_delay2 = -1;

                //+Patni-DKH/2010Feb04/Added/PH#3#/IR86/DebugWindow
                l_debugstr.Format(_T("site_id or run_num unmatch"));
                g_MainView->GetPqmDebugWinDlg().UpdateDebugData(l_debugstr);
                //-Patni-DKH/2010Feb04/Added/PH#3#/IR86/DebugWindow
            }

            g_MainView->SetPqmPmDelay(l_delay1, l_delay2);

            //+Patni-AMT/2010May04/Added/JaFT# REDMINE-209
            CDPSManager::GetInstance()->ReplyMsgAtLast(recvMsg, replyMsg,
                    (FALSE == l_ret) ? _T("E_FAIL") : _T("S_OK"));

            //-Patni-DKH/2010Jan29/Added/PH#3#/FBI

        } else if (strcmd == _T("GL_PQM_RegisterToPQM")) {
            int l_process_id = _ttoi(strparam);
            CString process_id = _T("");
            process_id.Format(_T("CALA Process ID : %d"), l_process_id);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, process_id);

            g_MainView->GetAutoLocatorController().WaitForAutoLocatorProcess(l_process_id);

        } else if (strcmd == _T("GL_PQM_QueryProtocolIndexAndWorkingMode")) {

            CDPSManager::GetInstance()->ReplyMsgAtNotLast(recvMsg, g_MainView->GetAutoLocatorController().GetProtIndexAndWorkingMode());
            g_MainView->GetAutoLocatorController().SetProtIndexAndWorkingMode(EMPTY_STRING);

        } else if (strcmd == _T("GL_ParamSet_Ready")) {

            GLParamSetReadyOpenEvent();
            g_MainView->SendMessageToProcon(PQM_PROCON_INSCAN_SWITCH_DONE, false, true);
            CDPSManager::GetInstance()->ReplyMsgAtNotLast(recvMsg, _T("S_OK"));
			
        } else if (strcmd == _bstr_t(_T("SARMgr_Start"))) {
			
			
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
				_T("SARMgr_Start message recived from SARManager"));
			
            g_MainView->SendMessage(WM_CMD_SARMANAGER_START, NULL, NULL);
			
        }else if (strcmd == _bstr_t(_T("RE_GL_Pager_GetPageInfo"))) {
			//	if (g_MainView->GetTiPrepController().GetTiPrepLoadWait()) {
			g_MainView->GetTiPrepController().ExtractPagerInfo(strparam);
			//} 
			/*	}else if (strcmd == _bstr_t(_T("RE_FW_QueryEmptyDisplayPage"))) {
			if (g_MainView->GetTiPrepController().GetTiPrepLoadWait()) {
			g_MainView->GetTiPrepController().ChangeStudyInEmptyTemplate(strparam);
			} 
			} else if (strcmd == _bstr_t(_T("RE_FW_ChangePatient"))) {
			if (strparam.CompareNoCase(_T("S_OK")) == 0) {
			if (g_MainView->GetTiPrepController().GetTiPrepLoadWait()) {
			g_MainView->GetTiPrepController().SendLoadTiPrep();
			} 	
			}
			*/
		}else if (strcmd == _bstr_t(_T("RE_GL_Pager_AssignPage"))) {
			if (!strparam.IsEmpty()){
				if (g_MainView->GetTiPrepController().GetTiPrepLoadWait()) {
					g_MainView->SwitchPage(strparam);
					g_MainView->GetTiPrepController().SendLoadTiPrep(strparam);
				} 
			} else {
				//Anulekha:Display Error Message that TiPrep cannot be invoked
			}
			
		}else if (strcmd == _bstr_t(_T("REGISTER_TIPREP_TO_PQM"))) {
			int pid = 0;
			g_MainView->GetTiPrepController().OnRecvTiPrepRegister(pid,strparam,sender);
		}else if (strcmd == _bstr_t(_T("CAN_TIPREP_UPDATE"))) {
			g_MainView->GetTiPrepController().ProcessUpdateRequest(strparam,sender);
			
		} else if (strcmd == _bstr_t(_T("TIPREP_CONNECTION_CLOSE"))) {
			g_MainView->GetTiPrepController().OnRecvConnectionClose((const CString)strparam,sender);
		} else if (strcmd == _bstr_t(_T("TIPREP_UPDATE_COMPLETED"))) {
			g_MainView->SetWaitCursor(false);
		} 		else if (strcmd == _bstr_t(_T("RE_GL_Pager_SwitchPage"))) {
		
			
		}else {
            //Patni-PJS/2010Feb02/Added/DeFT# MVC006916 code review- RE_FW_CheckState removed
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, strcmd);
        }

        // MEITEC/2010May15/Modified/MVC007802
        return FALSE;
        //+ MEITEC/2010Jun23/Modified/MVC008324_1

    } CATCH(CException, e) {

        const int MSG_SIZE = 1024;
        TCHAR errmsg[MSG_SIZE] = {L'\0'};
        e->GetErrorMessage(errmsg, MSG_SIZE);
        CString strerr = _T("");
        strerr.Format(_T("[Exception] %s"), errmsg);
        PQM_TRACE(SYSTEM_FUNC_MARKER,
                  FUNC_NAME,
                  strerr);
        return FALSE;
    }
    END_CATCH
    //- MEITEC/2010Jun23/Modified/MVC008324_1

}

//**************************************************************************
//Method Name   : GLParamSetReadyOpenEvent
//Author        : PATNI\DKH
//Purpose       :
//*****************************************************************************
void CDPSManager::GLParamSetReadyOpenEvent()const
{
    LPCTSTR FUNC_NAME = _T("CDPSManager::GLParamSetReadyOpenEvent");
    HANDLE l_paramsetready_wait = OpenEvent(EVENT_ALL_ACCESS, FALSE, PQM_WFDA_FOR_PARAM_SETREADY_REPL);

    if (NULL != l_paramsetready_wait) {
        SetEvent(l_paramsetready_wait);
        CloseHandle(l_paramsetready_wait);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ParamSetReady EVENT set as RE_GL_ParamSet_READY message is received."));

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ParamSetReady EVENT NOT FOUND."));
    }
}

//+Patni-DKH/2010Feb01/Added/PH#3#/FBI
//**************************************************************************
//Method Name   : GetFBIDelayInfo
//Author        : PATNI\DKH
//Purpose       : To get study loid, delay1 and delay2 data from parameter
//*****************************************************************************
//Patni-AMT/2010May04/Modified/JaFT# REDMINE-209 Return type changed
BOOL CDPSManager::GetFBIDelayInfo(
    const CString f_param,
    CString& f_stuydyloid,
    int& f_delay1,
    int& f_delay2
)
{
    CString l_studyloid = _T("");
    CString l_delay1 = _T("");
    CString l_delay2 = _T("");
    CString l_param = _T("");

    l_param = f_param ;
    l_param.TrimLeft();
    l_param.TrimRight();
    int l_idelay1 = -1 , l_idelay2 = -1;

    int l_firstslashpos = 0 , l_secondslashpos = 0;

    l_firstslashpos = l_param.Find(_T("\\"));
    l_secondslashpos = l_param.Find(_T("\\"), (l_firstslashpos + 1));
    l_studyloid = l_param.Left(l_firstslashpos);
    l_delay1 = l_param.Mid(l_firstslashpos + 1 , l_secondslashpos - (l_firstslashpos + 1));
    l_delay2 = l_param.Right(f_param.GetLength() - (l_secondslashpos + 1));



    l_idelay1 = _ttoi(l_delay1);

    if (l_idelay1 == 0) {
        if (l_delay1 != _T("0")) {
            l_idelay1 = -1 ;
        }

    } else if (l_idelay1 < 0) {
        l_idelay1 = -1;
    }

    l_idelay2 =  _ttoi(l_delay2);

    if (l_idelay2 == 0) {
        if (l_delay2 != _T("0")) {
            l_idelay2 = -1 ;
        }

    } else if (l_idelay2 < 0) {
        l_idelay2 = -1;
    }

    f_stuydyloid = l_studyloid ;
    f_delay1 = l_idelay1 ;
    f_delay2 = l_idelay2 ;

    //+Patni-AMT/2010May04/Added/JaFT# REDMINE-209
    if (f_delay1 == -1 || f_delay2 == -1 || f_stuydyloid.IsEmpty()) {
        return FALSE;
    }

    return TRUE;
    //-Patni-AMT/2010May04/Added/JaFT# REDMINE-209
}
//-Patni-DKH/2010Feb01/Added/PH#3#/FBI




