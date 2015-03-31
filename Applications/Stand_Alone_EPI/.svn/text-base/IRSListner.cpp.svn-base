// IRSListner.cpp: implementation of the CIRSListner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IRSListner.h"
#include <iostream>
#include "EPIController.h"
#include <iostream>
#include "PageEPI.h"
//#include <tami/libWNP/WinNativePorting.h>
#include <string.h>
#include "EPIView.h"
extern CEPIView* g_view;
#define IRS_MAX_BUFFER 255
CIRSListner* g_irs_obj = NULL;
const int CHAR_SIZE = 3;
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIRSListner::CIRSListner()
{
    
}

CIRSListner::~CIRSListner()
{
    
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CIRSListner::InitializeIRS()
{
    BOOL l_status = FALSE;    
    WSADATA wsaData;
    //WNP_ProcInit_SocketInit();
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
    if ((m_irs_socket = IrsInit()) == INVALID_FD) {
		return FALSE;

        // error msg   "Can't connect to Irs!!"     
    } else {
        if (IRSRegisterReconDone() == E_NO_ERROR) {
            l_status = TRUE;
        }    else {
			return FALSE;
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
/************************************************************************/
/*                                                                      */
/************************************************************************/

int CIRSListner::GetIRSSocketFD()
{
    return m_irs_socket ;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD CIRSListner::IRSListnerThread(LPVOID f_parameter)
{
    CIRSListner* l_irs_listner = (CIRSListner*)(f_parameter);
    
    //while (TRUE) {
     if (l_irs_listner)   l_irs_listner->IRSEventHandler();
   // }
    
    ExitThread(0);
    return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CIRSListner::IRSRegisterReconDone()
{
    //log entry
    DllT*           l_field_list = NULL;
    IRS_EventT      *l_irs_event = NULL;
    study_id_t      l_study_id;
    int             l_event_cnt = 0;
    char            l_path[IRS_MAX_BUFFER] = {0}, l_path_len[IRS_MAX_BUFFER] = {0};
    status_t        l_status = E_ERROR;
    
    if (m_irs_socket == INVALID_FD) {
        return l_status;
    }
    
    l_study_id.site_id = -1;
    l_study_id.run_num = -1;
    
//     if (IrsAddEventField(IRS_EF_IMAGE_ADDED,
//         sizeof(study_id_t),
//         &l_study_id,
//         IRS_WC_NOWILDCARD,
//         &l_field_list) == E_ERROR) {
//         return l_status;
//     }
//     
//     l_event_cnt++;
//     
    
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
        //error _T("Trace of l_irs_event == NULL"));
        return l_status;
    }
    
    l_status = IrsSendEvent(m_irs_socket, l_irs_event);
    
    if (l_status == E_ERROR) {
        
        //error _T("Trace IRSSendEvent(m_irs_socket,l_irs_event) < 0"));
    }
    
    IrsFreeEvent(l_irs_event);
    l_study_id.site_id = -1;
    l_study_id.run_num = -1;
    l_field_list = NULL;
    return l_status;
}
void CIRSListner::IRSEventHandler()
{
    IRS_EventT*         l_irs_event = NULL;
    study_id_t*         l_study_id = NULL;
    CString l_csProtocolMsg = _T("");
   
    CString irs_study_id ;
    CString curr_study_id;
	BOOL whileBreak = FALSE;
    while (whileBreak== FALSE) {
		l_irs_event = IrsPollEvent(m_irs_socket);
		if (l_irs_event == NULL) {        
			continue;
    }
        switch (l_irs_event->notificationType) {
        case IRS_EF_LAST_IMAGE_DONE:
            if (l_irs_event->notificationType == IRS_EF_IMAGE_ADDED) {                
                l_study_id = (study_id_t*)IrsGetEventField(l_irs_event, IRS_EF_IMAGE_ADDED);
                if (l_study_id) {
                    irs_study_id.Format(_T("Run%d.%d"), l_study_id->run_num , l_study_id->site_id);                    
                }                
            }else if(l_irs_event->notificationType == IRS_EF_LAST_IMAGE_DONE && m_nReconCalled) {

//                 (CEPIController::GetInstance())->loadStudy(1);
			//	(CEPIController::GetInstance())->SendMsgToView();
				//::SendMessage(g_view->GetSafeHwnd(),WM_EPI_RELOAD,NULL,NULL);
				whileBreak = TRUE;
            }
            break;            
        default:
            //trace log;
            CString sbd;
            break;
        }
    }
}
