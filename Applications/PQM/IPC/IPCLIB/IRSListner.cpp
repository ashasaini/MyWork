//REDMINE-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-779
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IRSListner.cpp
 *  Overview: Implementation of CIRSListner class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/5/30 12:00:00     Restructured after
 TA comments
*****************************************************************************/
#include "stdafx.h"
#include "IRSListner.h"
#include <tami/proCon/proMsgCodes.h> //Ashish SM3
#include <tami/BDM/bdmDefs.h>
extern "C" {
#include <tami/libIrs/IrsEventFields.h>
}
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <PErrorHandler/ExceptionHandler.h>

#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include <PQM/PQMIPCManager.h>
#include "pqm.h"
#include "scanprocedure.h"
#include "pqmpmacqman.h"
#include "PqmSpeeder.h"
#include "pqmstudy.h"
#include "ShimmingRequest.h"
#include "ShimCtrl.h"
#include "PQMDebugInfo.h"
#include "pqmAMB.h"



//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;

extern VfPathElem_t s_shimechopath[];
CIRSListner* g_irs_obj = NULL;

//****************************Method Header************************************
//Method Name   : CIRSListner()
//Author        : PATNI\LK
//Purpose       : Constructor
//*****************************************************************************
CIRSListner::CIRSListner(
    CPqm* f_pqm
) : m_bIndexToPass(-1),    //Patni-AMT/2009Oct16/Added/Locator button issue
    m_process_node(-1),
    m_parent_pqmptr(f_pqm),
    m_irs_socket(-1),
    m_irslisner_thread(NULL)
{
    g_irs_obj = this;
}

//****************************Method Header************************************
//Method Name   : ~CIRSListner()
//Author        : PATNI\HAR - SC
//Purpose       : Destructor
//*****************************************************************************
CIRSListner::~CIRSListner(
)
{
    if (m_irslisner_thread) {
        TerminateThread(m_irslisner_thread, NULL);
        m_irslisner_thread = NULL;
    }

    g_irs_obj = NULL;
}

//*******************************Method Header*********************************
//Method Name    :InitializeIRS()
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CIRSListner::InitializeIRS(
)
{
    LPCTSTR FUNC_NAME = _T("CIRSListner::InitializeIRS");
    BOOL l_status = FALSE;

    WSADATA wsaData;
    WNP_ProcInit_SocketInit();

    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    if ((m_irs_socket = IrsInit()) == INVALID_FD) {
        //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_CONNECT_IRS,
        //        _T("IDS_ERR_CONNECT_IRS"),
        //        SCAN_ERR_NOTIFY, _T("IRSListner.cpp"));
        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_pqmptr) {
            CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(
                _T("Can't connect to Irs!!"));
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("*** Cannot Connect to IRS ***"));
        //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179

    } else {
        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_pqmptr) {
            CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(
                _T("PqmGlobal.sockIrs=%d"), m_parent_pqmptr->GetIRSScoketFD());
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (IRSRegisterReconDone() == E_NO_ERROR) {
            l_status = TRUE;

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IRSRegisterReconDone() failed."));
        }


        if (IrsRegisterButtons() ==  E_NO_ERROR) {
            l_status = TRUE;

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IrsRegisterButtons() failed."));
        }
    }

    //	m_is_irs_event = false;
    //	m_timer_id = SetTimer(NULL,-1,10,(TIMERPROC)TimerProcedure);
    if (l_status) {
        m_irslisner_thread = CreateThread(
                                 NULL,                           // default security attributes
                                 NULL,                           // use default stack size
                                 CIRSListner::IRSListnerThread,  // thread function
                                 (void*)this,                    // argument to thread function
                                 NULL,                           // use default creation flags
                                 NULL);

        if (!m_irslisner_thread) {
            l_status = FALSE;
        }
    }

    return l_status;
}

//+Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   :
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
int CIRSListner::GetIRSSocketFD()const
{
    return m_irs_socket ;
}

//*******************************Method Header*********************************
//Method Name    :IRSRegisterReconDone()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
status_t CIRSListner::IRSRegisterReconDone(
)const
{
    LPCTSTR FUNC_NAME = _T("CIRSListner::IRSRegisterReconDone");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    status_t        l_status = E_ERROR;

    if (m_irs_socket == INVALID_FD) {
        return l_status;
    }

    DllT*           l_field_list = NULL;
    study_id_t      l_study_id;
    int             l_event_cnt = 0;
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //char            l_path[IRS_MAX_BUFFER], l_path_len[IRS_MAX_BUFFER];

    l_study_id.site_id = -1;
    l_study_id.run_num = -1;

    /* now register for locator images */

    if (IrsAddEventField(IRS_EF_IMAGE_ADDED,
                         sizeof(study_id_t),
                         &l_study_id,
                         IRS_WC_NOWILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }

    l_event_cnt++;
    char            l_path_len[IRS_MAX_BUFFER] = {0};

    if (IrsAddEventField(IRS_EF_VF_PATHLENGTH,
                         IRS_MAX_BUFFER,
                         l_path_len,
                         IRS_WC_WILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }

    l_event_cnt++;
    char			l_path[IRS_MAX_BUFFER] = {0};

    if (IrsAddEventField(IRS_EF_VF_PATH,
                         IRS_MAX_BUFFER,
                         l_path,
                         IRS_WC_WILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }

    l_event_cnt++;

    IRS_EventT*     l_irs_event = IrsCreateEvent(m_irs_socket,
                                  IRS_ED_MSGTYP,
                                  IRS_MSGTYP_REGISTER,
                                  IRS_ED_NOTIFTYP,
                                  IRS_EF_IMAGE_ADDED,
                                  IRS_ED_RESTRICTFLG,
                                  IRS_RESFLG_NONE,
                                  IRS_ED_NUMEVENTFIELDS,
                                  l_event_cnt,
                                  IRS_ED_EVENTFIELDLIST,
                                  l_field_list,
                                  NULL);

    if (l_irs_event == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_irs_event == NULL"));
        return l_status;
    }

    if (IrsSendEvent(m_irs_socket, l_irs_event) == E_ERROR) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("IrsSendEvent(m_irs_socket,l_irs_event) fails"));
    }

    IrsFreeEvent(l_irs_event);

    l_study_id.site_id = -1;
    l_study_id.run_num = -1;

    /* now register for locator images */
    l_field_list = NULL;

    if (IrsAddEventField(IRS_EF_LAST_IMAGE_DONE,
                         sizeof(study_id_t),
                         &l_study_id,
                         IRS_WC_NOWILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }

    l_event_cnt++;

    if (IrsAddEventField(IRS_EF_VF_PATHLENGTH,
                         IRS_MAX_BUFFER,
                         l_path_len,
                         IRS_WC_WILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }

    l_event_cnt++;

    if (IrsAddEventField(IRS_EF_VF_PATH,
                         IRS_MAX_BUFFER,
                         l_path,
                         IRS_WC_WILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }

    l_event_cnt++;

    l_irs_event = IrsCreateEvent(m_irs_socket,
                                 IRS_ED_MSGTYP,
                                 IRS_MSGTYP_REGISTER,
                                 IRS_ED_NOTIFTYP,
                                 IRS_EF_LAST_IMAGE_DONE,
                                 IRS_ED_RESTRICTFLG,
                                 IRS_RESFLG_NONE,
                                 IRS_ED_NUMEVENTFIELDS,
                                 l_event_cnt,
                                 IRS_ED_EVENTFIELDLIST,
                                 l_field_list,
                                 NULL);

    if (l_irs_event == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_irs_event == NULL"));
        return l_status;
    }

    l_status = IrsSendEvent(m_irs_socket, l_irs_event);

    if (l_status == E_ERROR) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("IRSSendEvent(m_irs_socket,l_irs_event) < 0"));
    }

    IrsFreeEvent(l_irs_event);
    //+Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
    l_study_id.site_id = -1;
    l_study_id.run_num = -1;
    l_field_list = NULL;
    int socb = 0;	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    IrsAddEventField(CFW_EF_LAST_IMAGE_DONE,
                     sizeof(int), &socb,
                     IRS_WC_WILDCARD,
                     &l_field_list);
    l_event_cnt = 1;
    l_irs_event = IrsCreateEvent(m_irs_socket,
                                 IRS_ED_MSGTYP, IRS_MSGTYP_REGISTER,
                                 IRS_ED_RESTRICTFLG, IRS_RESFLG_NONE,
                                 IRS_ED_EVENTFIELDLIST, l_field_list,
                                 IRS_ED_NOTIFTYP, CFW_EF_LAST_IMAGE_DONE, //+Patni-MP/201004/Added/REDMINE-508
                                 NULL);

    //This msg is from IRS MSMQ Client
    if (l_irs_event == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_irs_event == NULL"));
        return l_status;
    }

    l_status = IrsSendEvent(m_irs_socket, l_irs_event);

    if (l_status == E_ERROR) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("IRSSendEvent(m_irs_socket,l_irs_event) < 0"));
    }

    IrsFreeEvent(l_irs_event);
    //-Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
    return l_status;
}

//*******************************Method Header*********************************
//Method Name    :IRSEventHandler()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CIRSListner::IRSEventHandler(
)
{
    // + Patni - HAR / 2009 Apr 13 / Modified / Optimization
    //Patni-PJS/2010May09/Added/JaFT# IR-141
    LPCTSTR FUNC_NAME = _T("CIRSListner::IRSEventHandler");

    VfPathElem_t*       l_path_ptr  = NULL;
    int*                l_path_depth = NULL;

    CScanProcedure*     l_scan_proc = NULL;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    CPQMStudy*          l_pqm_study = NULL;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    CString l_protocol_for_Locator = _T("");

    if (m_parent_pqmptr) {
        if (m_parent_pqmptr->GetAcqManager()) {
            l_scan_proc = m_parent_pqmptr->GetAcqManager()->GetScanProcedure();

        } else {
            return;
        }

        l_pqm_study = m_parent_pqmptr->GetStudy();

    } else {
        return;
    }

    CPqmProtocol*   l_protocol = NULL;

    if (l_scan_proc) {
        l_protocol = l_scan_proc->GetScanProcProtocol();
    }

    IRS_EventT*         l_irs_event = NULL;

    if ((l_irs_event = IrsPollEvent(m_irs_socket)) == NULL) {

        closesocket(m_irs_socket);
        //+Patni-MP/2009July23/Modified/ACE-2 Close socket requirement
        //WNP_close(m_irs_socket);
        //-Patni-MP/2009July23/Modified/ACE-2 Close socket requirement
    }

    //+Patni-Hemant+SS/2009Oct30/Added/Crash in QA testing
    CString irs_study_id = _T("");
    CString curr_study_id = _T("");
    //-Patni-Hemant+SS/2009Oct30/Added/Crash in QA testing
    //+ Patni-Tanoue san/2009Nov05/Added/Thread Synchronization

    study_id_t*         l_study_id = NULL;

    try {

        while (l_irs_event != NULL) {

            BOOL l_auto_plan = false;

            switch (l_irs_event->notificationType) {
                case IRS_EF_IMAGE_ADDED:
                case IRS_EF_LAST_IMAGE_DONE: {
                    //+Patni-Hemant+SS/2009Oct30/Modified+Added/Crash in QA testing
                    if (l_irs_event->notificationType == IRS_EF_IMAGE_ADDED) {

                        //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                        CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("IRS_EF_IMAGE_ADDED"));

                        l_study_id = (study_id_t*)IrsGetEventField(l_irs_event, IRS_EF_IMAGE_ADDED);

                        //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                        if (l_study_id) {
                            irs_study_id.Format(_T("Run%d.%d"), l_study_id->run_num , l_study_id->site_id);

                            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                      _T("IRS_EF_IMAGE_ADDED : ") + irs_study_id);
                            //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                        }

                    } else {
                        //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                        CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("IRS_EF_LAST_IMAGE_DONE"));

                        l_study_id = (study_id_t*)IrsGetEventField(l_irs_event, IRS_EF_LAST_IMAGE_DONE);

                        //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                        if (l_study_id) {
                            irs_study_id.Format(_T("Run%d.%d"), l_study_id->run_num , l_study_id->site_id);
                            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                      _T("IRS_EF_LAST_IMAGE_DONE : ") + irs_study_id);
                            //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                        }
                    }

                    curr_study_id.Format(_T("Run%d.%d"), l_pqm_study->GetRumNum() , l_pqm_study->GetSiteId());

                    // ThreadSafe for Study information with LoadStudy.
                    // To avoid update study info between IRS Study Check and Get Value.
                    CScopedLock l_scoped_lock(l_pqm_study->GetStudyLock(), CPQMLogMgr::GetInstance());

                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, curr_study_id);

                    l_path_depth = (int*)IrsGetEventField(l_irs_event, IRS_EF_VF_PATHLENGTH);
                    l_path_ptr = (VfPathElem_t*)IrsGetEventField(l_irs_event, IRS_EF_VF_PATH);
                    //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                    CString l_path = PrintPath(l_path_ptr, *l_path_depth);
                    CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("%s"), l_path);
                    //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow

                    //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                    if (l_study_id == NULL) {
                        CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("No Study ID !!! ERROR"));
                        break;
                    }

                    CPQMStudy* l_pqmstudy = m_parent_pqmptr->GetStudy() ;

                    if (l_pqmstudy) {
                        CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("Cntxt: %d.%d Recv: %d.%d"),
                                l_pqmstudy->GetStudyID().run_num,
                                l_pqmstudy->GetStudyID().site_id,
                                l_study_id->run_num,
                                l_study_id->site_id);
                    }

                    //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                    if ((l_pqm_study->GetRumNum() != l_study_id->run_num) ||
                        (l_pqm_study->GetSiteId() != l_study_id->site_id)
                       ) {

                        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                  _T("Study ID Mismatch for IRS_EF_LAST_IMAGE_DONE : [irs_study_id : ")
                                  + irs_study_id + _T("][curr_study_id : ") + curr_study_id + _T("]"));

                        break;
                    }

                    //-Patni-Hemant+SS/2009Oct30/Modified+Added/Crash in QA testing

                    //Patni-AMT/2009Oct16/Added/Locator button issue
                    m_bIndexToPass  = l_path_ptr[0].index;
                    m_process_node = l_path_ptr[SVD_ST_PROCESSED - 1].index;

                    l_protocol_for_Locator.Format(_T("IRS_EF_LAST_IMAGE_DONE-Locator protocol index =%d"),
                                                  m_bIndexToPass);
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                              l_protocol_for_Locator);

                    if ((l_protocol = l_pqm_study->GetProtocol(l_path_ptr[0].index)) != NULL) {

                        if (l_protocol) {
                            if (l_irs_event->notificationType == IRS_EF_LAST_IMAGE_DONE) {

                                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                          _T("l_irs_event->notificationType == IRS_EF_LAST_IMAGE_DONE"));



                                //check wether protocol is autolocator enabled and can invoke AL application
                                m_parent_pqmptr->CheckForAutoLocator(l_protocol, false);

                                if (l_protocol->GetShimming() && l_path_ptr[1].index == 0) {

                                    if (l_protocol->GetShimmingReady()) {

                                        CShimmingRequest* l_shimming_request = new CShimmingRequest(l_pqm_study);
                                        SShimReqInfo_t l_shimreq_info;
                                        memset(&l_shimreq_info, 0, sizeof(SShimReqInfo_t));
                                        CString l_studyname = l_pqm_study->GetStudyName();
                                        int l_del_place = l_studyname.ReverseFind(_T('/'));
                                        int l_diff = l_studyname.GetLength() - l_del_place;
                                        CString l_del_str = l_studyname.Left(l_studyname.GetLength() - (l_diff - 1));
                                        l_studyname.Delete(0, l_del_str.GetLength());

                                        CString l_new_str = _T("/study/");
                                        l_new_str += l_studyname;
                                        //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
                                        int l_len = l_new_str.GetLength() + 1;
                                        char* l_str = new char[l_len];
                                        memset(l_str, 0, sizeof(char) * (l_len));
                                        //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
                                        wcstombs(l_str, l_new_str, l_len);
                                        strcpy(l_shimreq_info.study, l_str);

                                        //+Patni-HAR/2009Aug11/Added/Memory Leaks
                                        DEL_PTR_ARY(l_str);

                                        //-Patni-HAR/2009Aug11/Added/Memory Leaks
                                        l_shimreq_info.protocol =  l_protocol->GetProtocol();
                                        l_shimreq_info.path_len =  0;

                                        //+Patni-HAR/2010Mar04/Added/MSA00251-00176-00
                                        CShimmingRequest::ShimmingTimerReset();
                                        CShimmingRequest::ShimmingTimerSetup();
                                        //-Patni-HAR/2010Mar04/Added/MSA00251-00176-00
                                        //Patni-PJS/2011Mar9/Added/TMSC-REDMINE-1536
                                        l_scan_proc->ResetCurrentRequest();
                                        SR_STATUS l_shim_req_status = l_shimming_request->
                                                                      ProcessShimmingRequest(&l_shimreq_info);

                                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                                  l_shim_req_status ? _T("SHIMMING_CANCEL") : _T("SHIMMING_ADJUST_DONE"));
                                        //Patni-HAR/2010Feb18/Added/MSA0251-00176
                                        CShimmingRequest::ShimmingTimerReset();

                                        //+Patni-HAR/2009Aug11/Added/Memory Leaks
                                        DEL_PTR(l_shimming_request);

                                        //-Patni-HAR/2009Aug11/Added/Memory Leaks
                                        //Patni-KSS/2011Feb17/Commented/CPP Test tool Execution
                                        //VfPathElem_t* l_path = s_shimechopath;
                                        int l_shim_flag = l_pqm_study->GetShimFlag(s_shimechopath);

                                        //+Patni-Manishkumar/2009Sept15/Added/MVC004760/End wait cursor, set in scan done
                                        if (l_shim_flag == VFC_SHIM_HIGH_RESOLUTION) {
                                            PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                                                      _T("End Wait Cursor"));
                                            m_parent_pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
                                            //Patni-PJS/2010Jan19/Added/MVC006827
                                            m_parent_pqmptr->WriteToUI(PQM_MSG_POST_PROCESS_END);

                                            //Patni-HAR/2010Mar08/Added/MSA0251-00275
                                            m_parent_pqmptr->SetPostProcessFlag(FALSE);

                                        }

                                        //-Patni-Manishkumar/2009Sept15/Added/MVC004760/End wait cursor, set in scan done

                                        DEL_PTR_ARY(l_str);	//Patni-MJC/2009Aug17/Modified/cpp test corrections


                                        // for a Shimming sequence, send the shim offset value to AcqMan and send ShimCtrl msg
                                        if (l_shim_flag == VFC_SHIM_STANDARD) {
                                            if (l_shim_req_status == SHIMMING_ADJUST_DONE) {
                                                PqmShim_t l_shim;
                                                PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                                                          _T("Before Sending SendShimOffset"));

                                                l_pqm_study->GetShimmingOffset(&l_shim);
                                                l_scan_proc->GetShimCtrl()->SendShimOffset(&l_shim);
                                                //+Patni-HAR-SS/MODDIFIED/2009-June-05/JFT#266

                                                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                                          _T("if --> PQM_MSG_SCANDONE_SHIMMING"));

                                                m_parent_pqmptr->WriteToUI(PQM_MSG_SCANDONE_SHIMMING);

                                                //Patni-HAR/2010Mar08/Added/MSA0251-00275

                                                m_parent_pqmptr->SetPostProcessFlag(TRUE);

                                            } else {
                                                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                                          _T("else --> PQM_MSG_SCANDONE_SHIMMING"));

                                                //Patni-HAR/2010Mar04/Modified/MSA00251-00176-00
                                                m_parent_pqmptr->WriteToUI(PQM_MSG_SCANDONE_SHIMMING);
                                                //-Patni-HAR-SS/MODDIFIED/2009-June-05/JFT#266
                                                m_parent_pqmptr->SetPostProcessFlag(FALSE);//-Patni-MP/2010-Sep-13/REDMINE-500

                                                if (!m_parent_pqmptr->CanDisplayCFAGraph(l_protocol)) {
                                                    m_parent_pqmptr->WriteToUI(PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END);

                                                } else {

                                                    m_parent_pqmptr->WriteToUI(PQM_MSG_CFA_PRESCAN_END);
                                                }

                                                m_parent_pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN); //+Raghavendra/MODDIFIED/2012-Jan-25/Redmine-2780-reopen
                                            }

                                        } else {

                                            m_parent_pqmptr->WriteToUI(PQM_MSG_POST_PROCESS_END);
                                            m_parent_pqmptr->SetPostProcessFlag(FALSE);

                                        }

                                        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                                                  _T("SetShimmingReady(FALSE)"));
                                        l_protocol->SetShimmingReady(FALSE);

                                    } else {
                                        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                                                  _T("SetShimmingReady(TRUE)"));
                                        l_protocol->SetShimmingReady(TRUE);
                                    }

                                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                              _T("GetShimming() l_path_ptr[1].index == 0"));

                                } else if (l_protocol->GetSpeederType() ==
                                           VFC_SPEEDER_TYPE_SENSITIVITY) {

                                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                              _T("GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY"));

                                    CPqmSpeeder* l_speeder = m_parent_pqmptr->GetSpeederPtr();

                                    if (l_pqm_study->SpeederMAPExist(l_protocol->GetProtocol()) ==
                                        E_NO_ERROR) {
                                        l_speeder->SpeederDone(l_protocol, E_NO_ERROR) ;

                                    } else {
                                        l_speeder->SpeederDone(l_protocol, E_ERROR) ;
                                    }

                                    l_protocol->SetIsMapPostProcDone(true);//Patni-MRP/2010Aug06/Added/TMSC-REDMINE498
                                    m_parent_pqmptr->SetPostProcessFlag(FALSE);
                                    m_parent_pqmptr->WriteToUI(PQM_MSG_POST_PROCESS_END);

                                }
                            }

                            if ((!l_protocol->GetHasImage()) &&
                                (m_parent_pqmptr->GetPqmProCon()->PqmGetParentIndexFplanning(
                                     l_path_ptr[0].index) == l_path_ptr[1].index) &&
                                l_protocol->GetAppCode() != VFC_APP_CODE_MRS &&
                                l_protocol->GetAppCode() != VFC_APP_CODE_MRSCSI  &&
                                (l_protocol->GetMovingCouch() != VFC_MOVING_COUCH_CONTINUOUSLY) &&
                                !l_protocol->GetShimming() &&
                                (l_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY)) {

                                //l_protocol->SetHasImage(TRUE);

                                int l_data_kind = l_pqm_study->GetImageDataKind(
                                                      l_path_ptr[0].index, l_path_ptr[1].index);

                                l_protocol->SetDataKind(l_data_kind);

                                m_parent_pqmptr->WriteToUI(PQM_MSG_LOC_BTN);//ashish SM3		Auto Plan

                                if (m_parent_pqmptr->GetIsProconConnected()) {

                                    //+Patni-MP/2009May02/Modified/ACE-MCM0213-00155
                                    if (m_parent_pqmptr->GetPqmProCon()->GetIsProconActive()) {
                                        //Patni-AMT/2009Aug24/Deleted/MVC004331
                                        l_auto_plan = FALSE;
                                    }

                                    //-Patni-MP/2009May02/Modified/ACE-MCM0213-00155
                                }
                            }
                        }
                    }

                    break;
                }

                //+Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
                //+Patni-MP/2010Aug04/Added/REDMINE-508
                case CFW_EF_LAST_IMAGE_DONE: { //This msg is from IRS MSMQ Client
                    //Patni-AMT/2009Oct16/Added/Locator button issue
                    l_protocol_for_Locator.Format(
                        _T("PQM_MSG_IRS_CFW_EF_LAST_IMAGE_DONE-Locator protocol index=%d"),
                        m_bIndexToPass);
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                              l_protocol_for_Locator);

                    // ThreadSafe for Study information with LoadStudy.
                    // To avoid update study info between IRS Study Check and Get Value.
                    CScopedLock l_scoped_lock(l_pqm_study->GetStudyLock());

                    //+Patni-Hemant+SS/2009Oct30/Added/Crash in QA testing
                    const char* irs_msg = (char*) IrsGetEventField(l_irs_event, CFW_EF_LAST_IMAGE_DONE);
                    const CString logstr = irs_msg;
                    const CString l_irs_study_loid = this->GetStudyLoidFromCFWIRSMsg(irs_msg);

                    if (l_irs_study_loid.IsEmpty()) {
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                  _T("CFW msg doesn't have StudyLoid ->") + logstr);
                        break;
                    }

                    curr_study_id = l_pqm_study->GetStudyLoid();

                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                              _T("CFW_EF_LAST_IMAGE_DONE : [l_irs_study_loid : ")
                              + l_irs_study_loid + _T("][current_study_id : ") + curr_study_id + _T("]"));

                    if (curr_study_id.  CompareNoCase(l_irs_study_loid) != 0) {
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                  _T("Study ID Mismatch for CFW_EF_LAST_IMAGE_DONE"));

                        break;
                    }

                    const int l_parent_index = m_parent_pqmptr->GetPqmProCon()->PqmGetParentIndexFplanning(m_bIndexToPass, true);

                    if (l_parent_index > m_process_node) {
                        l_protocol_for_Locator.Format(
                            _T("PQM_MSG_IRS_CFW_EF_LAST_IMAGE_DONE - parent index > image's processed node. Locator protocol index = %d and  Process node = %d"),
                            m_bIndexToPass, m_process_node);
                        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                  l_protocol_for_Locator);
                        break;
                    }

                    //-Patni-Hemant+SS/2009Oct30/Added/Crash in QA testing

                    //+ Patni-PJS/2009June19/Modify/IDS#615
                    // This modification for Auto plan and auto plan should not execute for shimming,HRS ,speeder and MAP scan
                    if (!l_pqm_study->GetHasImageInStudy()) {
                        //+Patni-PJS/2010May09/Added/JaFT# IR-141
                        if (!m_parent_pqmptr->RaiseInstancesAuditEvent()) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RaiseInstancesAuditEvent failed"));
                        }

                        //-Patni-PJS/2010May09/Added/JaFT# IR-141

                        l_protocol = l_pqm_study->GetProtocol(m_bIndexToPass);


                        if (!((l_protocol != NULL) && l_protocol->GetShimming())) {
                            l_pqm_study->SetHasImageInStudy(TRUE);
                        }

                        l_auto_plan = TRUE;

                    } else {
                        l_auto_plan = FALSE;
                    }

                    //Patni-AMT/2009Oct16/Added/Locator button issue
                    if ((l_protocol = l_pqm_study->GetProtocol(m_bIndexToPass)) != NULL) {   //Patni-Manishkumar/2009Oct12/ADDED/MVC004921/Loc. enable for 3rd prot.
                        if ((!l_protocol->GetHasImage()) &&
                            l_protocol->GetAppCode() != VFC_APP_CODE_MRS &&
                            l_protocol->GetAppCode() != VFC_APP_CODE_MRSCSI  &&
                            (l_protocol->GetMovingCouch() != VFC_MOVING_COUCH_CONTINUOUSLY) &&
                            !l_protocol->GetShimming() &&
                            (l_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY)) {
                            //+Patni-RSG-MN/2009Sept11/Added/MVC3227/To enable locator button
                            CPqmProcon* l_pqm_procon = m_parent_pqmptr->GetPqmProCon();

                            if (l_pqm_procon != NULL) {
                                //Patni-AMT/2009Oct16/Added/Locator button issue
                                if (l_pqm_procon->PqmGetParentIndexFplanning(
                                        m_bIndexToPass, true) >= 0) {
                                    m_parent_pqmptr->WriteToUI(PQM_MSG_IRS_CFW_EF_LAST_IMAGE_DONE);
                                    l_protocol->SetHasImage(TRUE);

                                    if (m_parent_pqmptr->GetIsProconConnected()) {

                                        //+Patni-MP/2009May02/Modified/ACE-MCM0213-00155
                                        if (m_parent_pqmptr->GetPqmProCon()->GetIsProconActive()) {
                                            //Patni-AMT/2009Oct16/Added/Locator button issue
                                            m_parent_pqmptr->GetPqmProCon()->pqmPmLocatorControl(
                                                l_protocol, m_bIndexToPass);
                                            //Patni-AMT/2009Aug24/Deleted/MVC004331
                                            l_auto_plan = FALSE;
                                        }
                                    }

                                    if (m_parent_pqmptr->IsActiveDialog()) {
                                        l_auto_plan = FALSE;
                                    }

                                    if (m_parent_pqmptr->GetAutoPlan() && l_auto_plan) {
                                        //Patni-AMT/2009Oct16/Added/Locator button issue
                                        m_parent_pqmptr->ProlimAutoPlan(m_bIndexToPass);
                                    }

                                } else {

                                    AUTOLOCATOR_TYPE l_al_type = l_protocol->GetAutoLocatorType();

                                    if ((AUTOLOCATOR_CARDIAC == l_al_type) || (AUTOLOCATOR_NEURO == l_al_type)) {

                                        l_protocol->SetHasImage(TRUE);
                                    }
                                }

                            } else {
                                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                                          _T("CPqmProcon pointer NULL"));
                            }
                        }
                    }

                    //- Patni-PJS/2009June19/Modify/IDS#615
                    break;
                }

                //-Patni-Manish/2009June12/Added/JFT#MVC2350-Locator issue
                //- Patni-Tanoue san/2009Nov05/Added/Thread Synchronization
                case IRS_EF_BBOX_BUTTON_PRESS:
                    //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                    CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("IRS_EF_BBOX_BUTTON_PRESS"));
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                              _T("IRS_EF_BBOX_BUTTON_PRESS"));
                    pqmIrsBDMHandler(l_irs_event);

                    break;

                default:
                    //Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
                    CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(_T("DEFAULT irsEvent->notificationType=%d")
                            , l_irs_event->notificationType);
                    break;
            }

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("IrsPollEvent...."));

            IrsFreeEvent(l_irs_event);
            l_irs_event = IrsPollEvent(m_irs_socket);
            // - Patni - HAR / 2009 Apr 13 / Modified / Optimization
        }

    } catch (...) {

        CString l_log_str;
        l_log_str.Format(_T("Exception Occured while IRS event handling->IRS Socket ID : %d"), m_irs_socket);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_log_str);
        throw;
    }
}

void CIRSListner::pqmIrsBDMHandler(IRS_EventT*		irsEvent)const
{
    DB_FUNC_ENTER("[pqmIrsBDMHandler]:");
    int*		button = (int*)IrsGetEventField(irsEvent,
                         IRS_EF_BBOX_BUTTON_PRESS);

    if (button == NULL)
        return;

    //+Patni-HAR/2010Mar08/Added/MSA0251-00275
    if (m_parent_pqmptr->GetPostProcessFlag() || m_parent_pqmptr->GetProcessing()) {
        return;
    }

    //-Patni-HAR/2010Mar08/Added/MSA0251-00275

    if (*button == START_ON_CONSOLE_BUTTON ||
        *button == START_ON_GANTRY_BUTTON) {
        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_pqmptr) {
            CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(
                _T("START_ON_CONSOLE_BUTTON or START_ON_GANTRY_BUTTON"));
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        /*	if ( PqmGlobal.mode == K_EZSCAN )
        		PqmEzscanScanStart() ;

        	else if(PqmPmIsActive(K_PM_TYPE_AMB))
        		PqmAmbScanStart();*/
        //else


        //+Patni-KSS/2010Oct08/Modified/V1.35#IR-167
        //m_parent_pqmptr->WriteToUI(PQM_MSG_IRS_SCANSTART);
        ProcessProbeMsgForIRS(PQM_MSG_IRS_SCANSTART);
        //-Patni-KSS/2010Oct08/Modified/V2.00#IR-167

    } else if (*button == PAUSE_ON_CONSOLE_BUTTON ||
               *button == PAUSE_ON_GANTRY_BUTTON) {
        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_pqmptr) {
            CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(
                _T("PAUSE_ON_CONSOLE_BUTTON or PAUSE_ON_GANTRY_BUTTON"));
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (!m_parent_pqmptr->GetAMB()->PqmAMBScanSeg2Active())	/* V7.10 */
            //PqmAcqPauseResume();
            m_parent_pqmptr->WriteToUI(PQM_MSG_IRS_SCANPAUSE);

    } else if (*button == ABORT_ON_CONSOLE_BUTTON ||
               *button == ABORT_ON_GANTRY_BUTTON) {
        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_pqmptr) {
            CPQMDebugInfo::GetInstance(m_parent_pqmptr)->PqmDebugIrsMonitor(
                _T("ABORT_ON_CONSOLE_BUTTON or ABORT_ON_GANTRY_BUTTON"));
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow

        //	if ( PqmGlobal.mode == K_EZSCAN )
        //	PqmEzscanScanAbort( );
        //	else
        //PqmAcqDelayAbort();
        //+Patni-KSS/2010Oct08/Modified/V1.35#IR-167
        //m_parent_pqmptr->WriteToUI(PQM_MSG_IRS_SCANABORT);
        ProcessProbeMsgForIRS(PQM_MSG_IRS_SCANABORT);
        //-Patni-KSS/2010Oct08/Modified/V1.35#IR-167
        //	m_parent_pqmptr->WriteToUI(PQM_MSG_SR_ABORT_WAIT );
        //	m_parent_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);

    }

    /* else if(*button == 9999 && PqmGlobal.pqmStudy.study_name != NULL)
     {
     PqmAcqStartOrNext();
     }*/
}

// Functions are not used now but are completely ported
//*******************************Method Header*********************************
//Method Name    :IrsRegisterButtons()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
status_t CIRSListner::IrsRegisterButtons(
)const
{
    LPCTSTR FUNC_NAME = _T("CIRSListner::IrsRegisterButtons");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    int l_event_cnt = 0;
    status_t        l_status = E_ERROR;
    DllT* l_field_list = NULL;

    IrsAddEventField(IRS_EF_ACQBUTTONS,
                     IRS_SZ_VOID,
                     IRS_VOID_VALUE,
                     IRS_WC_NOWILDCARD,
                     &l_field_list);

    l_event_cnt++;

    if (IrsAddEventField(IRS_EF_BBOX_BUTTON_PRESS,
                         IRS_SZ_INT,
                         IRS_VOID_VALUE,
                         IRS_WC_WILDCARD,
                         &l_field_list) == E_ERROR) {
        return l_status;
    }



    l_event_cnt++;

    if (m_irs_socket != -1) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_irs_socket != -1"));

        IRS_EventT* l_irs_event = IrsCreateEvent(m_irs_socket,
                                  IRS_ED_MSGTYP,
                                  IRS_MSGTYP_REGISTER,
                                  IRS_ED_NOTIFTYP,
                                  IRS_EF_BBOX_BUTTON_PRESS,
                                  IRS_ED_RESTRICTFLG,
                                  IRS_RESFLG_NONE,
                                  IRS_ED_NUMEVENTFIELDS,
                                  l_event_cnt,
                                  IRS_ED_EVENTFIELDLIST,
                                  l_field_list,
                                  NULL);

        /* if(!l_irs_event || IrsSendEvent(m_irs_socket,l_irs_event) < 0) {
            CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                COMMHANDLER_MARKER_PQM_ACQMAN,
                 _T("CIRSListner::IrsRegisterButtons"),
                 _T("Trace of !l_irs_event || IrsSendEvent(m_irs_socket,l_irs_event) < 0"));
         }*/

        l_status = IrsSendEvent(m_irs_socket, l_irs_event);

        if (l_status == E_ERROR) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("IRSSendEvent(m_irs_socket,l_irs_event) < 0"));
        }

        IrsFreeEvent(l_irs_event);

    }

    return l_status;
}

//+Patni-KSS/2010Oct08/Added/V1.35#IR-167
void CIRSListner::ProcessProbeMsgForIRS(const pqm_msg_tag f_msg)const
{
    if (!m_parent_pqmptr->ProcessProbeMsgForIRS(f_msg))  {

        m_parent_pqmptr->WriteToUI(f_msg);
    }

}
//-Patni-KSS/2010Oct08/Added/V1.35#IR-167

//**************************************************************************
//Method Name   : PrintPath
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
CString CIRSListner::PrintPath(VfPathElem_t* pathP, int depth)const
{
    CString l_strpath = _T("PATH=");
    CString l_tmp = _T("");

    if (pathP) {
        for (; depth--; pathP++) {
            l_tmp = _T("");
            l_tmp.Format(_T("/%d.%d"), pathP->subtree_name, pathP->index);
            l_strpath += l_tmp ;
        }
    }

    return l_strpath;
}

// Functions are not used now but are completely ported
//*******************************Method Header*********************************
//Method Name    :IrsNotifyScanStart()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CIRSListner::IrsNotifyScanStart(
    study_id_t* f_study_id
)const
{
    LPCTSTR FUNC_NAME = _T("CIRSListner::IrsNotifyScanStart");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));


    if (m_irs_socket != (-1)) {

        DllT* l_event_field_list = NULL;
        IrsAddEventField(IRS_EF_SCAN_START,
                         sizeof(study_id_t),
                         f_study_id,
                         IRS_WC_NOWILDCARD,
                         &l_event_field_list);

        IRS_EventT* l_irs_event = IrsCreateEvent(m_irs_socket,
                                  IRS_ED_MSGTYP,
                                  IRS_MSGTYP_NOTIFY,
                                  IRS_ED_EVENTFIELDLIST,
                                  l_event_field_list,
                                  NULL);

        if (l_irs_event == NULL) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("l_irs_event == NULL"));
        }

        //pqmdebugirsmonitor("send scan_start");

        /* send a registration irs event */
        if (IrsSendEvent(m_irs_socket, l_irs_event) == NULL) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("IRSSendEvent(m_irs_socket,l_irs_event) < 0"));
        }

        IrsFreeEvent(l_irs_event);
    }
}

//*******************************Method Header*********************************
//Method Name    :IRSListnerThread()
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
DWORD CIRSListner::IRSListnerThread(
    LPVOID f_parameter
)
{
    __try {

        CIRSListner* l_irs_listner = (CIRSListner*)(f_parameter);

        /*IRS_EventT *        l_irs_event = NULL;
        VfPathElem_t *      l_path_ptr  = NULL;
        int*                l_path_depth = NULL;
        study_id_t*         l_study_id = NULL;

        CScanProcedure*     l_scan_proc;
        CPQMStudy*          l_pqm_study; */

        while (TRUE) {
            l_irs_listner->IRSEventHandler();
            /*    TIMEVAL  l_socket_timout;
            	//////
            /*
            	if(l_irs_listner->m_parent_pqmptr){
            		if(l_irs_listner->m_parent_pqmptr->GetAcqManager()){
            			l_scan_proc = l_irs_listner->m_parent_pqmptr->GetAcqManager()->GetScanProcedure();
            		}else {
            			//return ;
            		}
            		l_pqm_study = l_irs_listner->m_parent_pqmptr->GetStudy();
            	} else {
            		//return;
            	}
            	if ((l_irs_event = IrsPollEvent(l_irs_listner->m_irs_socket)) == NULL) {

            		CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            			COMMHANDLER_MARKER_PQM_ACQMAN,
            			_T("CIRSListner::IRSListnerThread"),
            			_T("Trace of (l_irs_event= IrsPollEvent(m_irs_socket)) == NULL"),true);


            		  closesocket(l_irs_listner->m_irs_socket);
            		//pqmIrsDelayInit(NULL,NULL);	//to do in next phase
            	}

            	while (l_irs_event != NULL) {

            		l_irs_listner->m_is_irs_event = true;
            		l_irs_listner->m_current_event = l_irs_event->notificationType;

            		l_irs_listner->m_path_ptr  = (VfPathElem_t*)IrsGetEventField(l_irs_event,IRS_EF_VF_PATH);
            		l_irs_listner->m_path_depth = (int*)IrsGetEventField(l_irs_event,IRS_EF_VF_PATHLENGTH);
            		l_irs_listner->m_study_id = (study_id_t *)IrsGetEventField(l_irs_event,IRS_EF_IMAGE_ADDED);

            		IrsFreeEvent(l_irs_event);
            		l_irs_event = IrsPollEvent(l_irs_listner->m_irs_socket);
            	}



            	/////
            */
        }
    }

    __except(RecordExceptionInfo(GetExceptionInformation(),
                                 _T("CIRSListner::IRSListnerThread"))) {

    }

    ExitThread(0);
    return 0;
}

/**
 * GetStudyLoidFromCFWIRSMsg
 *
 * Broardcast loid format is patient/study/series/image
 * Each loid can be blank, like:
 *     /study/seires/image, //series/image, /study//
 * If there is not study,  PQM should get Study Loid from LocalDB with Series or Image loid.
 * But we know reconstruction always send /study/series/image.
 * So, PQM treats Study Loid from CFW msg only. Not access to LocalDB.
 * If CFW msg doesn't have Study LOID, this func returns empty string.
 *
 * @param f_irs_msg [in] CFW IRS Msg
 * @retval StudyLoid. If CFW IRS Msg has StudyLoid.
 * @retval empty. If CFW IRS Msg doesn't have StudyLoid.
 */
CString CIRSListner::GetStudyLoidFromCFWIRSMsg(const char* f_irs_msg) const
{
    CString l_study_loid = _T("");
    const char SEP[] = "/";
    std::string l_extract_str = f_irs_msg;

    // find first separator, and then, remove patient and first separator.
    std::string::size_type pos = l_extract_str.find(SEP);

    if (pos == l_extract_str.npos) {
        return l_study_loid;
    }

    if (pos == 0) {
        l_extract_str.erase(0, 1);

    } else {
        l_extract_str.erase(0, pos + 1);
    }


    // find next separator and then, remove except study loid
    pos = l_extract_str.find(SEP);

    if (pos != l_extract_str.npos) {
        l_extract_str.erase(pos, l_extract_str.npos);
    }

    // check CFW msg has study loid or not.
    if (l_extract_str.empty()) {
        return l_study_loid;
    }

    l_study_loid = l_extract_str.c_str();
    return l_study_loid;
}

// Functions are not used now but are completely ported
//*******************************Method Header*********************************
//Method Name    :IrsNotifyImageAdded()
//Author         :PATNI/HAR - SC
//Purpose        :
//*****************************************************************************
//void CIRSListner::IrsNotifyImageAdded(
//    study_id_t *f_study_id,
//    VfPathElem_t *f_path,
//    int f_depth
//) {
//    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
//            COMMHANDLER_MARKER_PQM_ACQMAN,
//            _T("CIRSListner::IrsNotifyImageAdded"),
//            _T("Trace of CIRSListner::IrsNotifyImageAdded"));
//    DllT *l_event_field_list = NULL;
//    IRS_EventT *l_irs_event;
//
//    if (m_irs_socket != -1) {
//        IrsAddEventField(IRS_EF_IMAGE_ADDED,
//            sizeof(study_id_t),
//            f_study_id,
//            IRS_WC_WILDCARD,
//            &l_event_field_list);
//        IrsAddEventField(IRS_EF_VF_PATHLENGTH,
//            sizeof(int),
//            &f_depth,
//            IRS_WC_WILDCARD,
//            &l_event_field_list);
//
//        IrsAddEventField(IRS_EF_VF_PATH,
//            f_depth * sizeof(VfPathElem_t),
//            f_path,
//            IRS_WC_WILDCARD,
//            &l_event_field_list);
//
//        /* Create an IRS event */
//        l_irs_event = IrsCreateEvent(m_irs_socket,
//            IRS_ED_MSGTYP,
//            IRS_MSGTYP_NOTIFY,
//            IRS_ED_EVENTFIELDLIST,
//            l_event_field_list,
//            NULL);
//
//        /* Send a registration IRS event */
//        IrsSendEvent(m_irs_socket, l_irs_event);
//        IrsFreeEvent(l_irs_event);
//    }
//}

// Functions are not used now but are completely ported
//*******************************Method Header*********************************
//Method Name    :IrsNotifyLastImageDone()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
//void CIRSListner::IrsNotifyLastImageDone(
//    study_id_t *f_study_id,
//    VfPathElem_t *f_path,
//    int f_depth
//) {
//    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
//            COMMHANDLER_MARKER_PQM_ACQMAN,
//            _T("CIRSListner::IrsNotifyLastImageDone"),
//            _T("Trace of CIRSListner::IrsNotifyLastImageDone"));
//    DllT *l_event_field_list = NULL;
//    IRS_EventT *l_irs_event;
//
//    if (m_irs_socket != -1) {
//        IrsAddEventField(IRS_EF_LAST_IMAGE_DONE,
//            sizeof(study_id_t),
//            f_study_id,
//            IRS_WC_WILDCARD,
//            &l_event_field_list);
//        IrsAddEventField(IRS_EF_VF_PATHLENGTH,
//            sizeof(int),
//            &f_depth,
//            IRS_WC_WILDCARD,
//            &l_event_field_list);
//        IrsAddEventField(IRS_EF_VF_PATH,
//            f_depth * sizeof(VfPathElem_t),
//            f_path,
//            IRS_WC_WILDCARD,
//            &l_event_field_list);
//
//        /* Create an IRS event */
//        l_irs_event = IrsCreateEvent(m_irs_socket,
//            IRS_ED_MSGTYP,
//            IRS_MSGTYP_NOTIFY,
//            IRS_ED_EVENTFIELDLIST,
//            l_event_field_list,
//            NULL);
//
//        /* Send a registration IRS event */
//        IrsSendEvent(m_irs_socket, l_irs_event);
//        IrsFreeEvent(l_irs_event);
//    }
//}


//*******************************Method Header*********************************
//Method Name    :TimerProcedure()
//Author         :PATNI/SS
//Purpose        :
//*****************************************************************************
/*
void CALLBACK CIRSListner::TimerProcedure(
	void* hwnd,
	unsigned int event_id,
	unsigned int id_prt,
	unsigned long dwTime
){

	if(g_irs_obj != NULL){
		if(g_irs_obj->m_timer_id == id_prt){
			if(g_irs_obj->m_is_irs_event){
				g_irs_obj->IRSEventHandler();

			}
		}
	}
} */




