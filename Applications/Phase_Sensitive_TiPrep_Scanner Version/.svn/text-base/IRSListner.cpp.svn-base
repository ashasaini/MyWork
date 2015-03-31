// IRSListner.cpp: implementation of the CIRSListner class.
//
//////////////////////////////////////////////////////////////////////

/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "IRSListner.h"
#include "TiPrepView.h"
#include "TiPrepController.h"
#include <afxsock.h>
#include <tami/libvf/varfields.h>       /* Varfield definitions   */
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vfstudy.h>       /* Study hierarchy defs   */

#include <tami/common/study_id.h>
#include <tami/common/basicdefs.h>
#include "tami/libWNP/WNP_types.h"

#include <tami/BDM/bdmDefs.h>
#include "tami/librbvap/rbvap.h" 
#include "tami/librb/librb.h" 
#include <tami\rb\rbctl.h>
#include <tami\vfStudy\vf_appcodes.h>


extern "C" {
#include <tami/libIrs/Irs.h>
#include <tami/libIrs/IrsEventFields.h>
}

#define IRS_MAX_BUFFER 255
//CIRSListner* g_irs_obj = NULL;
const int CHAR_SIZE = 3;
const int IRS_UPDATE_PERIOD  = 60 * 1000; /* 1 minute */
#define MAX_IRS_INIT_COUNT 3

const CString EXCEPTION_IRSLISTENER = L"Exception occurred in IRS Listener";  
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*************************************************************
* Method:    	CIRSListner
* Description: 	Constructor
* Returns:   	
*************************************************************/
CIRSListner::CIRSListner(): m_bIndexToPass(-1),   //Added to solve Locator button issue
m_process_node(-1),
m_irslisner_thread(INVALID_HANDLE_VALUE),
m_IrsInitCount(0),m_irs_socket(0),
m_nReconCalled(0),m_nReconButtonClk(0),
m_nSeriseCnt(0),m_IrsTerminateF(0),m_nIRSMsgCnt(0)
{
    //g_irs_obj = this;
}
/*************************************************************
* Method:    	CIRSListner
* Description: 	Copy constructor
* Returns:   	
*************************************************************/
CIRSListner:: CIRSListner(CIRSListner &f_irsListner)
{
    /*TIPREP_TRACE(CIRSListner:: CIRSListner);*/

    if(NULL != f_irsListner.m_irslisner_thread){
        if(m_irslisner_thread) {
//             TerminateThread(m_irslisner_thread, NULL);
            CloseHandle(m_irslisner_thread);
            m_irslisner_thread = NULL; //AN Revw: setting NULL without closing / terminating thread
        }
    }    
    m_bIndexToPass = f_irsListner.m_bIndexToPass;
    m_irs_socket = f_irsListner.m_irs_socket;
    m_IrsInitCount = f_irsListner.m_IrsInitCount;
    m_irslisner_thread = f_irsListner.m_irslisner_thread;
    m_IrsTerminateF = f_irsListner.m_IrsTerminateF;
    m_nIRSMsgCnt = f_irsListner.m_nIRSMsgCnt;
    m_nReconButtonClk = f_irsListner.m_nReconButtonClk;
    m_nReconCalled = f_irsListner.m_nReconCalled;
    m_nSeriseCnt = f_irsListner.m_nSeriseCnt;
    m_process_node = f_irsListner.m_process_node;
}

/*************************************************************
* Method:    	CIRSListner
* Description: 	Overloaded assignment operator
* Returns:   	
*************************************************************/
CIRSListner& CIRSListner:: operator=(CIRSListner &f_irsListner)
{
    /*TIPREP_TRACE(CIRSListner:: operator=);*/
    
    if (this == &f_irsListner) {
        return *this;
    }
    
    if(NULL != f_irsListner.m_irslisner_thread){
        if(m_irslisner_thread) {
//             TerminateThread(m_irslisner_thread, NULL);
            CloseHandle(m_irslisner_thread);
            m_irslisner_thread = NULL; //AN Revw: setting NULL without closing / terminating thread
        }
        closesocket(m_irs_socket);
    }
    
    m_bIndexToPass = f_irsListner.m_bIndexToPass;
    m_irs_socket = f_irsListner.m_irs_socket;
    m_IrsInitCount = f_irsListner.m_IrsInitCount;
    m_irslisner_thread = f_irsListner.m_irslisner_thread;
    m_IrsTerminateF = f_irsListner.m_IrsTerminateF;
    m_nIRSMsgCnt = f_irsListner.m_nIRSMsgCnt;
    m_nReconButtonClk = f_irsListner.m_nReconButtonClk;
    m_nReconCalled = f_irsListner.m_nReconCalled;
    m_nSeriseCnt = f_irsListner.m_nSeriseCnt;
    m_process_node = f_irsListner.m_process_node;
    
    return *this;
}

/*************************************************************
* Method:    	~CIRSListner
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CIRSListner::~CIRSListner()
{
    if(m_irslisner_thread) {
        CloseHandle(m_irslisner_thread);
        m_irslisner_thread = NULL;
    }
    closesocket(m_irs_socket);
    //g_irs_obj = NULL;
}

/*************************************************************
* Method:    	InitializeIRS
* Description: 	This function is used 
* Returns:   	BOOL
*************************************************************/
BOOL CIRSListner::InitializeIRS()
{
    TIPREP_TRACE(CIRSListner::InitializeIRS,MRTraceMarker3)
    try{
        BOOL l_status = FALSE;
        m_irs_socket = IrsInit();
        if(SOCKET_ERROR/*INVALID_FD*/ == m_irs_socket ) {
            //log error
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("Invalid Fd"));
			 CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
            m_IrsInitCount++;        
            if(m_IrsInitCount > MAX_IRS_INIT_COUNT) {
                //log error
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("m_IrsInitCount = %d is greater than MAX_IRS_INIT_COUNT = % d Terminating IRS"),m_IrsInitCount,MAX_IRS_INIT_COUNT);
				CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
                IrsTerminate();
            } else {
                CString l_parameter = _T("");
                l_parameter.Format(_T("Retrying IrsInit()"), m_irs_socket);
                //log trace
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("Retrying IrsInit"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::InitializeIRS"));
            }
            
            l_status = FALSE;
        } else {
            //log trace
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("IrsInit success"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::InitializeIRS"));
            m_IrsInitCount = 0;
            
            if(IRSRegisterReconDone() == E_NO_ERROR) {
                //log trace
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("IRSRegisterReconDone success"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::InitializeIRS"));
                l_status = TRUE;
            }
        }
        DWORD dwThreadId = 0;
        if(l_status) {
            
            m_irslisner_thread = CreateThread(
                NULL,                           // default security attributes
                NULL,                           // use default stack size
                CIRSListner::IRSListnerThread,  // thread function
                (void*)this,                    // argument to thread function
                NULL/*|| CREATE_SUSPENDED*/,               // use default creation flags
                &dwThreadId);
            
            if(NULL == m_irslisner_thread) {
                //log error
                closesocket(m_irs_socket);
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("CreateThreads failed"));
				CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
                l_status = FALSE;
            }
        }
        
        CString l_csTempstr = _T("");
        l_csTempstr.Format(_T("InitializeIRS returning status = %d"),l_status);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::InitializeIRS"));
        return l_status;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_IRSLISTENER, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:    	GetIRSSocketFD
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CIRSListner::GetIRSSocketFD()
{
    return m_irs_socket ;
}

/*************************************************************
* Method:    	IRSListnerThread
* Description: 	This function is used 
* Parameter: 	LPVOID f_parameter :
* Returns:   	DWORD
*************************************************************/
DWORD CIRSListner::IRSListnerThread(LPVOID f_parameter)
{
    TIPREP_TRACE(CIRSListner::IRSListnerThread,MRTraceMarker3);
    CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11,_T("IRSListnerThread() started"),_T("CIRSListner::IRSListnerThread"));

    CIRSListner* l_irs_listner = NULL;
    l_irs_listner = (CIRSListner*)(f_parameter);
    
    if (l_irs_listner) l_irs_listner->IRSEventHandler();

//     ExitThread(0);
    return 0;
}

/*************************************************************
* Method:    	IRSRegisterReconDone
* Description: 	This function is used 
* Returns:   	status_t
*************************************************************/
status_t CIRSListner::IRSRegisterReconDone()
{
    TIPREP_TRACE(CIRSListner::IRSRegisterReconDone,MRTraceMarker3)
    try{
        //log entry
        DllT*           l_field_list = NULL;
        IRS_EventT      *l_irs_event = NULL;
        study_id_t      l_study_id;
        int             l_event_cnt = 0;
        //char            l_path[IRS_MAX_BUFFER] = {0}, l_path_len[IRS_MAX_BUFFER] = {0};
        status_t        l_status = E_ERROR;
        
        if(m_irs_socket == SOCKET_ERROR) {
            //log error
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("IRSRegisterReconDone(): Invalid Fd"));
            CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
            return l_status;
        }
        
        l_study_id.site_id = -1;
        l_study_id.run_num = -1;
        l_field_list = NULL;
        
//         if (IrsAddEventField(IRS_EF_IMAGE_ADDED,
//             sizeof(study_id_t),
//             &l_study_id,
//             IRS_WC_NOWILDCARD,
//             &l_field_list) == E_ERROR) {
//            
//             //log error
//             CString l_csTempstr = _T("");
//             l_csTempstr.Format(_T("IrsAddEventField returned error"));
//             CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
// 
//             return l_status;
//         }
//         
//          l_event_cnt++;
        if(IrsAddEventField(IRS_EF_LAST_IMAGE_DONE,
            sizeof(study_id_t),
            &l_study_id,
            IRS_WC_NOWILDCARD,
            &l_field_list) == E_ERROR) {
            //log error
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("IrsAddEventField returned error"));
            CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
            return l_status;
        }
        
        l_event_cnt++;
        
//         if(IrsAddEventField(IRS_EF_VF_PATHLENGTH,
//             IRS_MAX_BUFFER,
//             l_path_len,
//             IRS_WC_WILDCARD,
//             &l_field_list) == E_ERROR) {
//             //log error
//             CString l_csTempstr = _T("");
//             l_csTempstr.Format(_T("IrsAddEventField returned error"));
//             CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
//             return l_status;
//         }
//         
//         l_event_cnt++;
//         
//         if(IrsAddEventField(IRS_EF_VF_PATH,
//             IRS_MAX_BUFFER,
//             l_path,
//             IRS_WC_WILDCARD,
//             &l_field_list) == E_ERROR) {
//             //log error
//             CString l_csTempstr = _T("");
//             l_csTempstr.Format(_T("IrsAddEventField returned error"));
//             CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
//             return l_status;
//         }
//         
//         l_event_cnt++;
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
        
        if(l_irs_event == NULL) {
            //log error
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("IrsCreateEvent failed. l_irs_event is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
            return l_status;
        }
        
        l_status = IrsSendEvent(m_irs_socket, l_irs_event);
        
        if(l_status == E_ERROR) {
            //log trace
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("IrsSendEvent returned error"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::IRSRegisterReconDone"));
        }
        
        IrsFreeEvent(l_irs_event);
        l_study_id.site_id = -1;
        l_study_id.run_num = -1;
        l_field_list = NULL;
        return l_status;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_IRSLISTENER, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	IRSEventHandler
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CIRSListner::IRSEventHandler()
{
    TIPREP_TRACE(CIRSListner::IRSEventHandler,MRTraceMarker3)
    try{
//         LPCTSTR FUNC_NAME = _T("CIRSListner::InitializeIRS");
        IRS_EventT*         l_irs_event = NULL;
//         FILE* fp = NULL;
        //VfPathElem_t*       l_path_ptr  = NULL;
        //int*                l_path_depth = NULL;
        CString l_csProtocolMsg = _T("");
        BOOL whileBreak = FALSE;
        int cnt =0;
//        fp = fopen("c:/IRS_TIPREP.txt","w+");
        while (FALSE == whileBreak){
            l_irs_event = IrsPollEvent(m_irs_socket);
            if(NULL == l_irs_event) {
//                 if(fp) {
//                     fprintf(fp,"\nl_irs_event is NULL\n");
//                 }
                continue;
            }            
            if(l_irs_event != NULL) {
               // CTiPrepLogHelper::WriteToErrorLog(_T("IRS LOOP ENTER"), __LINE__, __FILE__);                
                CString l_csTempstr = _T("");
                switch(l_irs_event->notificationType) {
                case IRS_EF_IMAGE_ADDED:                
                    break;
                case IRS_EF_LAST_IMAGE_DONE:
//                     if(fp) {
//                         fprintf(fp,"\nIRS_EF_LAST_IMAGE_DONE received\n"); 
//                     }
                    l_csTempstr.Format(_T("IRS event notification type : IRS_EF_LAST_IMAGE_DONE"));
                    CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::IRSRegisterReconDone"));
                    if(l_irs_event->notificationType == IRS_EF_LAST_IMAGE_DONE /*&& 1 == m_nReconCalled*/) {
                        CString l_csTempstr = _T("");
                        l_csTempstr.Format(_T("IrsStatus set to \"Done\""));
                        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, l_csTempstr, _T("CIRSListner::IRSRegisterReconDone"));
                        m_nReconButtonClk = 0;
                        m_nReconCalled = 0;
                        whileBreak = TRUE;
                    }
                    break;
                default:
                    break;
                }
            }
            if(TRUE == whileBreak){
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11,_T("Breakflag TRUE:IRS LOOP EXIT"),_T("CIRSListner::IRSRegisterReconDone"));
            }
        }
//         if(fp)fclose(fp);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_IRSLISTENER, __LINE__, __FILE__);
    }
}
/**
* GetStudyLoidFromCFWIRSMsg
*
* Broardcast loid format is patient/study/series/image
* Each loid can be blank, like:
*     /study/seires/image, //series/image, /study//
* If there is not study,  TiPrep should get Study Loid from LocalDB with Series or Image loid.
* But we know reconstruction always send /study/series/image.
* So, TiPrep treats Study Loid from CFW msg only. Not access to LocalDB.
* If CFW msg doesn't have Study LOID, this func returns empty string.
*
* @param f_irs_msg [in] CFW IRS Msg
* @retval StudyLoid. If CFW IRS Msg has StudyLoid.
* @retval empty. If CFW IRS Msg doesn't have StudyLoid.
*/
CString CIRSListner::GetStudyLoidFromCFWIRSMsg(const char* f_irs_msg) const
{
    TIPREP_TRACE(CIRSListner::GetStudyLoidFromCFWIRSMsg,MRTraceMarker3)
    const CString EMPTY_STR = _T("");
    const char SEP[] = "/";
    std::string l_extract_str = f_irs_msg;
    // find first separator, and then, remove patient and first separator.
    std::string::size_type pos = l_extract_str.find(SEP);
    
    if(pos == l_extract_str.npos) {
        //log trace
        CString l_csTempstr = _T("");
        l_csTempstr.Format(_T("GetStudyLoidFromCFWIRSMsg(): empty string"));
        CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
        return EMPTY_STR;
    }
    
    if(pos == 0) {
        l_extract_str.erase(0, 1);
    } else {
        l_extract_str.erase(0, pos + 1);
    }
    
    // find next separator and then, remove except study loid
    pos = l_extract_str.find(SEP);
    
    if(pos != l_extract_str.npos) {
        l_extract_str.erase(pos, l_extract_str.npos);
    }
    
    // check CFW msg has study loid or not.
    if(l_extract_str.empty()) {
        //log trace
        CString l_csTempstr = _T("");
        l_csTempstr.Format(_T("GetStudyLoidFromCFWIRSMsg(): empty string"));
        CTiPrepLogHelper::WriteToErrorLog(l_csTempstr, __LINE__, __FILE__);
        return EMPTY_STR;
    }
    
    CString l_study_loid = l_extract_str.c_str();
    return l_study_loid;
}

void CIRSListner::IrsTerminate()
{
    m_IrsTerminateF = TRUE;
}

BOOL CIRSListner::WaitForIRSDone()
{
    try{
        if(INVALID_HANDLE_VALUE == m_irslisner_thread){
            CTiPrepLogHelper::WriteToErrorLog(_T("Handle is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
         WaitForSingleObject(m_irslisner_thread,INFINITE);
    // AN Rvew: main process should not wait for IRS, this was why there was hang in phase correction originally !! was there any particulatr issue in previous design?
    DWORD exitCode = 0;
    // examine create thread response
    if ( !GetExitCodeThread(m_irslisner_thread, &exitCode) ) {
        CString cs(_T(""));
        cs.Format(_T("GetExitCodeThread %08X\n"),GetLastError());
        CTiPrepLogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
    }else {
        CString temp;
        temp.Format(_T("Thread exitcode:%d.\n"), exitCode);
        //CTiPrepLogHelper::WriteToErrorLog(temp, __LINE__, __FILE__); //AN Revw: trace log
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker11, temp, _T("CIRSListner::WaitForIRSDone"));

    }    
    // close thread handle
    CloseHandle(m_irslisner_thread);
    }catch(...){
        CString lcsmsg(_T("Exception occurred in WaitForIRSDone"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        E_ERR_EXCEPTION;
    }
   return TRUE;
}

void CIRSListner::ForceExitIRS()
{
    if(m_irslisner_thread != INVALID_HANDLE_VALUE){
        ExitThread(0);
    }
    CloseHandle(m_irslisner_thread);
}
///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////