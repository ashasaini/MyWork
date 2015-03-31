//Redmine-783
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
// PqmProbe.cpp: implementation of the CPqmProbe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmProbe.h"
//+Patni-RUP/2010Feb09/Modified/To Issue no 6 reported by TMSC
#include <ErrorDisp/ErrorDisplay.h>
#include <tami/libplTree/plsftree.h>
#include <tami/libplTree/plsfproto.h>
#include <tami/libpl/plPar.h>
//-Patni-RUP/2010Feb09/Modified/To Issue no 6 reported by TMSC
#include <tami/proCon/proMsgCodes.h>
#include <tami/proCon/probe.h>
#include <Probe/Probe.h>
#include <PQM/PqmIpcManager.h>
#include <PQM/PQMLogMgr.h>
//Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
#include <PQM/PqmUtilities.h>
#include "Pqm.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "PqmProbeNotifier.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "scan.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
//+Patni-RUP/2009Dec15/Internal defect fix
#include "PqmProbeServer.h"
#include "PeerSocket.h"
//-Patni-RUP/2009Dec15/Internal defect fix
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"

using namespace MPlus::EH;

//Global Variable/////////////////////////////////////
DWORD CPqmProbe::m_dThreadId = 0;
//+Patni-MSN/2009Mar10/Added/Optimization
HANDLE CPqmProbe::m_hThread = NULL;
CPqmProcon* CPqmProbe::m_procon = NULL;
//int CPqmProbe::m_refprobe_mode = 0;
int CPqmProbe::m_refprobe_mode = -1;  //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
//-Patni-MSN/2009Mar10/Added/Optimization

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPqmProbe::CPqmProbe() :
    m_acqman_ptr(NULL), //Patni-AD/2009Jun01/Added/SU14 //cpp test corrections
    m_server(NULL),
    m_show_probe_dlg(false)   //Patni-RUP/2010Jun09/Added/MVC007947
{
}

CPqmProbe::CPqmProbe(CPqmProcon* f_pqm_probe) :
    m_acqman_ptr(NULL), //Patni-AD/2009Jun01/Added/SU14 //cpp test corrections
    m_server(NULL),
    m_show_probe_dlg(false)   //Patni-RUP/2010Jun09/Added/MVC007947
{

    //+ Patni-MSN/2009Apr09/Modified/PSP# 217
    if (f_pqm_probe) {
        m_procon = f_pqm_probe;

    } else {
        m_procon = NULL;
    }

    //- Patni-MSN/2009Apr09/Modified/PSP# 217
    //+Patni-RUP/2009Dec15/Internal defect fix
    //+Patni-RUP/2010Apr20/Modified/To Fix SM_PH3_CDR_RMC_CDR_05
    if (m_procon && m_procon->GetPQM()->GetMode() == K_IMAGING) {
        CreatePQMServer();
    }

    //-Patni-RUP/2010Apr20/Modified/To Fix SM_PH3_CDR_RMC_CDR_05
    //-Patni-RUP/2009Dec15/Internal defect fix
}

CPqmProbe::~CPqmProbe()
{
    //+ MEITEC/2010Jun23/Added/MVC008324_1
    LPCTSTR FUNC_NAME = _T("CPqmProbe::~CPqmProbe");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of Destructor"));
    //- MEITEC/2010Jun23/Added/MVC008324_1

    //Patni-RUP/2009Dec15/Internal defect fix
    DEL_PTR(m_server);
}

//***************************Method Header*************************************
//Method Name    : RefProbeScanStart
//Author         : PATNI/SG
//Purpose        : Send Scan Start Message to RefProbe
//*****************************************************************************
//Patni-Hemant/10Nov2010/Modified/TMSC-REDMINE-946
bool CPqmProbe::RefProbeScanStart()const
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::RefProbeScanStart");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProbe::RefProbeScanStart"));

    if (1 == m_refprobe_mode) {  //Patni-RUP/2010Jun24/Added/MVC007947

        proMsg_t l_msg;
        l_msg.code = RMC_CMD_EXIT;
        //+Patni-RUP/2009Dec15/Internal defect fix
        l_msg.data = NULL;
        l_msg.length = 0;
        l_msg.timeStamp = clock();
        //-Patni-RUP/2009Dec15/Internal defect fix
        SendMsgToProbe(&l_msg);
    }     //Patni-RUP/2010Jun24/Added/MVC007947

    //+Patni-Hemant/10Nov2010/Added/TMSC-REDMINE-946
    if (0 == m_refprobe_mode) {
        //Probe window is open..
        //So display a message to User for closing it
        //till than stop the scan.

        MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->
                                              DisplayMessageinWarningDialog(CPqmUtilities::GetMultiLingualString(_T("IDS_CLOSE_PROBE_PLAN_DIALOG")),
                                                      _T("PQM"), SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKONLYBUTTON);

        return false;
    }

    //-Patni-Hemant/10Nov2010/Added/TMSC-REDMINE-946

    //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
    if (NULL != m_acqman_ptr) {
        CPqm* l_pqmptr = m_acqman_ptr->GetPqm();

        if (NULL != l_pqmptr) {
            CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugProMonitor(_T("SHIM: Send RMC_CMD_EXIT"));
        }
    }

    //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow

    //Patni-Hemant/10Nov2010/Added/TMSC-REDMINE-946
    return true ;
}

//*******************************Method Header*********************************
//Method Name    : RefProbeScanEnd()
//Author         : PATNI/SG
//Purpose        :
//*****************************************************************************
void CPqmProbe::RefProbeScanEnd(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::RefProbeScanEnd");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of CPqmProbe::RefProbeScanEnd"));

    proMsg_t l_msg;
    l_msg.data = NULL;
    l_msg.length = 0;
    l_msg.timeStamp = clock();

    if (GetProbeMode() != 1) {
        l_msg.code = PROMSG_PQM_PROBE_ABORT;

    } else {

        l_msg.code = RMC_CMD_SCAN_END;

        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (NULL != m_acqman_ptr) {
            CPqm* l_pqmptr = m_acqman_ptr->GetPqm();

            if (NULL != l_pqmptr) {
                CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugProMonitor(_T("SHIM: Send RMC_CMD_SCAN_END"));
            }
        }
    }

    SendMsgToProbe(&l_msg);
}

//*******************************Method Header*********************************
//Method Name    : StartProbeClientThread()
//Author         : PATNI/SG
//Purpose        :
//*****************************************************************************
void CPqmProbe::StartProbeClientThread()
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::StartProbeClientThread");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProbe::StartProbeClientThread"));

    if (m_hThread == NULL)	{
        /*	 Create thread for the proCon client functionality	*/
        m_hThread = CreateThread(
                        NULL,                        // default security attributes
                        NULL,                           // use default stack size
                        CPqmProbe::ThreadFunc,                  // thread function
                        (void*)this,                // argument to thread function
                        0,                           // use default creation flags
                        &CPqmProbe::m_dThreadId);

    } else {

    }

}

//*******************************Method Header*********************************
//Method Name    : OnProbeMessage()
//Author         : PATNI/SG
//Purpose        :
//*****************************************************************************
void CPqmProbe::OnProbeMessage(const CSocketMsg& f_msg
                              )
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::OnProbeMessage");
    //+Patni-RUP/2009Dec15/Internal defect fix
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProbe::OnProbeMessage"));
    proMsg_t msg;
    msg.code = 0;
    msg.data = NULL;
    msg.length = 0;
    //+ Patni-MSN/2009Apr09/Modified/PSP# 217
    //+Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08
    PLBYTE l_buf = const_cast<CSocketMsg&>(f_msg).GetBuffer(); // FIXME : CSocketMsg should be fixed as const
    int l_remain_size = const_cast<CSocketMsg&>(f_msg).GetSize();
    memcpy(&msg, l_buf, PROMSG_HEAD_LEN);
    l_remain_size = l_remain_size - PROMSG_HEAD_LEN;
    //-Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08

    //+Patni-RUP/2010Jan18/Internal defect fix
    // 	CString message;
    // 	message.Format(_T("Recevied Message Code %d & Message Size %d"), msg.code,f_msg.GetSize());
    CString message = _T("");
    message.Format(_T("Message from Probe Message Code = %d Message= %s"),
                   msg.code, GetMsgStrFromCode(msg.code));

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, message);

    //-Patni-RUP/2010Jan18/Internal defect fix

    //msg.length = m_procon->GetDataForProbe(&msg.data);
    if (msg.length == 0) {
        msg.length = m_procon->GetDataForProbe(&msg.data);
    }

    //-Patni-RUP/2009Dec15/Internal defect fix

    CPqmPmAcqman* l_acqman_ptr = m_procon->GetPQM()->GetAcqManager();
    CScanProcedure* l_scan_proc = l_acqman_ptr->GetScanProcedure();

    //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
    if (msg.data != NULL) {
        message.Format(_T("msg data - %s"), msg.data);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, message);
    }

    //-Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
    //+Patni-KSS/2010Oct07/Modified/V1.35#IR-167
    CPQMIPCManager* l_pqmipcmanager = m_procon->GetPQM()->GetManagerPtr();

    if (msg.code == PROMSG_PROBE_RUN) {
        //+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
        //CPQMIPCManager* l_pqmipcmanager = m_procon->GetPQM()->GetManagerPtr();

        if (NULL != l_pqmipcmanager) {

            //+Patni-Hemant+Rajeesh/10Nov2010/Added/TMSC-REDMINE-946
            if ((l_scan_proc->GetIsScanning() || l_scan_proc->IsSingleProcRunning()) && (0 == m_refprobe_mode)) {
                message =  CPqmUtilities::GetMultiLingualString(_T("IDS_MAIN_OR_REFPROBE_SCAN_GOING_ON"));

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(message,
                        MPlus::EH::Information, _T("PqmProbe.cpp"), _T("PQM"));

                m_procon->StopProlim();
                return;
            }

            //+Patni-Hemant+Rajeesh/10Nov2010/Added/TMSC-REDMINE-946

            bool f_is_irs_msg = false ;

            if (l_remain_size) {
                memcpy(&f_is_irs_msg, l_buf + PROMSG_HEAD_LEN, l_remain_size);
            }

            if (!f_is_irs_msg && l_pqmipcmanager->RemoteCheckActiveFlag()) {
                //-Patni-KSS/2010Oct07/Modified/V1.35#IR-167
                message =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_PQM_REMOTE_CONSOLE"));
                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PQM_REMOTE_CONSOLE,
                        message, SCAN_ERR_NOTIFY, L"CPqmProbe.cpp");

                m_procon->StopProlim();
                return;
            }
        }

        //-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
        proMsg_t l_msg;
        l_msg.code = PROMSG_PROBE_RUN;

        if (m_procon) {
            l_msg.length = m_procon->GetDataForProbe(&l_msg.data);
            m_procon->SendMessageToProcon(l_msg.code, l_msg.length, l_msg.data);
            //do functionality for scan start
            m_procon->GetPQM()->WriteToUI(PQM_MSG_SCAN_ABORT_TRUE);
            // Patni-MSN/2010Mar17/Modified/RMC - Phase3_10
            m_procon->GetPQM()->WriteToUI(PQM_MSG_PROBE_SCAN_STARTPAUSE_DISABLE);

            DEL_PTR(l_msg.data);
        }

    } else if (msg.code == PROMSG_PROBE_STOP) {
        //do the functionality for scan stop

        //+Patni-Hemant+Rajeesh/10Nov2010/Added/TMSC-REDMINE-946
        if (l_scan_proc->GetIsScanning() && (0 == m_refprobe_mode)) {
            return;
        }

        //+Patni-Hemant+Rajeesh/10Nov2010/Added/TMSC-REDMINE-946

        //+Patni-KSS/2010Oct07/Modified/V1.35#IR-167
        bool f_is_irs_msg = false ;

        if (l_remain_size) {
            memcpy(&f_is_irs_msg, l_buf + PROMSG_HEAD_LEN, l_remain_size);
        }

        if (!f_is_irs_msg && l_pqmipcmanager->RemoteCheckActiveFlag()) {
            return ;
        }

        m_procon->GetPQM()->WriteToUI(PQM_MSG_SCAN_ABORT_FALSE);
        m_procon->GetPQM()->WriteToUI(PQM_MSG_PROBE_SCAN_STARTPAUSE_ENABLE);

        if (TRUE == l_scan_proc->GetProbing()) {
            l_scan_proc->SetProbing(FALSE);
            l_scan_proc->AcqProbingScanAbort();

            if (m_procon) {
                proMsg_t l_msg;
                l_msg.code = PROMSG_PROBE_STOP;
                l_msg.length = m_procon->GetDataForProbe(&l_msg.data);
                m_procon->SendMessageToProcon(l_msg.code, l_msg.length, l_msg.data);
            }
        }

        //-Patni-KSS/2010Oct07/Modified/V1.35#IR-167

    } else if (msg.code == PROMSG_PROBE_EXIT) {
        //+ Patni-MSN/2009Apr02/Added/PSP- According to Base code
        l_scan_proc->SetProbing(FALSE);

        //- Patni-MSN/2009Apr02/Added/PSP- According to Base code
        if (m_procon) {
            m_procon->GetPQM()->WriteToUI(PQM_MSG_SCAN_PAUSE_FALSE);
            m_procon->GetPQM()->WriteToUI(PQM_MSG_SCAN_START_TRUE);
        }

        PostThreadMessage(m_dThreadId, MSG_CLOSE, 0, 0);

        if (m_hThread) {
            CloseHandle(m_hThread);
            m_hThread = NULL;
        }

    } else if (msg.code == PROBING_THRES_HOLD) {
        CScan* l_scan = (CScan*)l_acqman_ptr->GetScanProcedure()->GetCurrentTransaction();

        //+Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08
        if (l_remain_size == sizeof(thres_hold_t)) {
            thres_hold_t thres;
            memcpy(&thres, l_buf + PROMSG_HEAD_LEN, l_remain_size);
            message.Format(_T("Received thres_hold_t data Max Threshold:- %f Min Thireshold:- %f"),
                           thres.l_max_thres_hold, thres.l_min_thres_hold);
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, message);
            l_scan->AcqProbingThresHold(thres.l_max_thres_hold, thres.l_min_thres_hold);

        } else {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("thres_hold_t structure data is not received"));
        }

        //-Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08

    } else if (msg.code == PROMSG_CMD_PROBE_OK) { //+Patni-RUP/2010Feb16/Added/To fix Issue no 6 reported by TMSC
        if (m_procon && 0 == m_refprobe_mode) {  //Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094
            m_procon->SendMessageToProcon(msg.code, msg.length, msg.data);
        }

        if (m_refprobe_mode == 0) {
            proMsg_t l_probe_msg;
            l_probe_msg.code = RMC_INITIALZE_SHARED_DATA;
            l_probe_msg.data = NULL;
            l_probe_msg.length = 0;
            SendMsgToProbe(&l_probe_msg);
        }

        //+Patni-RUP/2010Feb16/Added/To fix Issue no 6 reported by TMSC

    } else {
        if (m_procon) m_procon->SendMessageToProcon(msg.code, msg.length, msg.data);
    }

    //- Patni-MSN/2009Apr09/Modified/PSP# 217

}

//*******************************Method Header*********************************
//Method Name    : OnProbeDisconnect()
//Author         : PATNI/SG
//Purpose        :
//*****************************************************************************
void CPqmProbe::OnProbeDisconnect()const
{
    //+Patni-RUP/2009Dec15/Internal defect fix
    //+ Patni-MSN/2009Apr09/Modified/PSP# 217
    LPCTSTR FUNC_NAME = _T("CPqmProbe::OnProbeDisconnect");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProbe::OnProbeDisconnect"));
    //- Patni-MSN/2009Apr09/Modified/PSP# 217
    //-Patni-RUP/2009Dec15/Internal defect fix

}

//+ Patni-MSN/2009Apr17/Modified/PSP-Code Review
//*******************************Method Header*********************************
//Method Name    : SendMsgToProbe()
//Author         : PATNI/SG
//Purpose        :
//*****************************************************************************
bool CPqmProbe::SendMsgToProbe(proMsg_t* f_msg)const
{
    //+Patni-RUP/2009Dec15/Internal defect fix
    LPCTSTR FUNC_NAME = _T("CPqmProbe::SendMsgToProbe");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CPqmProbe::SendMsgToProbe"));

    if (NULL == m_server) {
        return false;
    }

    CPeerSocket* l_probe_socket = m_server->GetPeerSocket();

    if (NULL == l_probe_socket) {
        return false;
    }

    //+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
    if (false == l_probe_socket->IsSocketAlive()) {
        return false;
    }

    //+Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08
    shared_mem_data_t l_shared_mem_data;
    l_shared_mem_data.m_resolution = 0;
    l_shared_mem_data.m_run_num = 0;
    l_shared_mem_data.m_site_id = 0;
    l_shared_mem_data.m_tr = 0;
    //-Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08

    //Patni-RUP/2010Feb16/Modified/To fix Issue no 6 reported by TMSC and IDS Defect Id 138
    if (f_msg->code == PROMSG_CON_ALLOW_SELECTIONS
        || f_msg->code == PROMSG_CON_TREE_UPDATED
        || f_msg->code == RMC_INITIALZE_SHARED_DATA) {
        //+Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08
        if (false != GetShrMemSegDataForProbe(&l_shared_mem_data)) {
            f_msg->data = (void*)(&l_shared_mem_data);
            f_msg->length = sizeof(shared_mem_data_t);

        } else {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("Failed to get shared_mem_data_t member values"));
            return false;
        }

        //+Patni-RUP/2010Apr28/Added/To Fix SM_PH3_RMC_CDR_08
    }

    //-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC

    l_probe_socket->SendData(f_msg);
    //+Patni-RUP/2010Jan18/Internal defect fix
    CString message = _T("");
    message.Format(_T("Message sent to Probe Message Code = %d Message= %s"),
                   f_msg->code, GetMsgStrFromCode(f_msg->code));
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              message);
    //-Patni-RUP/2010Jan18/Internal defect fix
    return true;
    //-Patni-RUP/2009Dec15/Internal defect fix
}

//+ Patni-PJS/2009Aug14/Modify/MVC002490
//***************************Method Header*************************************
//Method Name    : StudyPlanInit
//Author         : PATNI/PJS
//Purpose        : Initializes Plan study
//*****************************************************************************
void CPqmProbe::StudyPlanInit(
    char* f_file_name,
    const int prot
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::StudyPlanInit");

    CString l_print_str(_T(""));
    l_print_str.Format(_T("Protocol = %d"), prot);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              (CString(f_file_name) + _T(" ") + l_print_str));


    VfPathElem_t        path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_VISUALGROUP,    0},
    };

    path[SVD_ST_PROTOCOL - 1].index = prot;

    VfFieldSpec_t       vfReq[11] = {0};		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 11);

    //Pankaj S added
    int32_t         l_image_id_len = 1;
    char*            l_buffer = new char[sizeof(char)];
    memset(l_buffer, 0, sizeof(char));
    wcstombs(l_buffer, _T("\0"), l_image_id_len);
    //Patni-RAJ/ADDED On 10/09/2009

    //TMSC-Tanoue/2010Jun18/Modified/MVC008345. Use CVarFieldHandle
    CVarFieldHandle vf_handle(f_file_name);
    status_t l_status = vf_handle.Open();

    if_error(l_status) {
        //Pankaj S added comment
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("Unable to read write VFFT_FILE"));

        return;
    }

    int n = 0, num = 0, plan_status = 0;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_STATUS, SVT_PLN_STATUS,
                 sizeof(plan_status), &plan_status, VFO_REPLACE);
    n++;

    int locator[8] = { 0, 0, 0, 0, 0, 0, 0, 0 } ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGES , SVT_PLN_LOCATOR_IMAGES,
                 sizeof(locator), locator, VFO_REPLACE);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID, SVT_PLN_LOCATOR_IMAGE_ID,
                 l_image_id_len, l_buffer, VFO_ALLOCATE | VFO_REPLACE);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_2, SVT_PLN_LOCATOR_IMAGE_ID_2,
                 l_image_id_len, l_buffer, VFO_ALLOCATE | VFO_REPLACE);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_3, SVT_PLN_LOCATOR_IMAGE_ID_3,
                 l_image_id_len, l_buffer, VFO_ALLOCATE | VFO_REPLACE);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_4, SVT_PLN_LOCATOR_IMAGE_ID_4,
                 l_image_id_len, l_buffer, VFO_ALLOCATE | VFO_REPLACE);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_5, SVT_PLN_LOCATOR_IMAGE_ID_5,
                 l_image_id_len, l_buffer, VFO_ALLOCATE | VFO_REPLACE);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_6, SVT_PLN_LOCATOR_IMAGE_ID_6,
                 l_image_id_len, l_buffer, VFO_ALLOCATE | VFO_REPLACE);
    n++;

    int gate_mode = 0 ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_GATING_METHOD , SVT_PLN_GATING_METHOD,
                 sizeof(gate_mode), &gate_mode, VFO_REPLACE);
    n++;

    int Dynamic = FALSE ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_DYNAMIC_SCAN , SVT_PLN_DYNAMIC_SCAN,
                 sizeof(Dynamic), &Dynamic, VFO_REPLACE);
    n++;

    time_t      	ts = 0;		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_status = vf_handle.PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num, &ts);

    if (l_status != E_NO_ERROR && num != n) {
        DB_PRINTF("", ("Put Data Error. (%s:%d) \n", __FILE__, __LINE__));
        DB_FUNC_EXIT();

        return ;
    }

#if	0	/* VECTOR INIT */
    n = 0 ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_VG_PHASE_ORI_VECTOR , SVT_PLN_VG_PHASE_ORI_VECTOR,
                 sizeof(phaseV), &phaseV, VFO_REPLACE);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_VG_SLICE_ORI_VECTOR , SVT_PLN_VG_SLICE_ORI_VECTOR,
                 sizeof(sliceV), &sliceV, VFO_REPLACE);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_VG_OFFSET_VECTOR , SVT_PLN_VG_OFFSET_VECTOR,
                 sizeof(offsetV), &offsetV, VFO_REPLACE);
    n++;

    l_status = vf_handle.PutFields(path, SVD_ST_VISUALGROUP, vfReq, n, &num, &ts);

    if (l_status != E_NO_ERROR && num != n) {
        //Error
        //DB_PRINTF("",("Put Data Error. (%s:%d) \n", __FILE__,__LINE__));
    }

#endif

    DB_FUNC_EXIT();
    return ;

}
//- Patni-PJS/2009Aug14/Modify/MVC002490

//+Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
//******************************************************************************
//Function Name:- ShowHideProbeWindow()
//Author       :- PATNI\RUP
//Purpose      :- This function is used to send show/hide message to probe window
//******************************************************************************
void CPqmProbe::ShowHideProbeWindow(const BOOL& f_show_flag)
{
    proMsg_t l_msg;
    l_msg.length = 0;
    l_msg.data = NULL;

    if (f_show_flag) {
        l_msg.code = RMC_SHOW_PROBE_WINDOW;
        m_show_probe_dlg = true;   //Patni-RUP/2010Jun09/Added/MVC007947

    } else {
        l_msg.code = RMC_HIDE_PROBE_WINDOW;
        m_show_probe_dlg = false;  //Patni-RUP/2010Jun09/Added/MVC007947
    }

    //+Patni-RUP/2010Jun09/Added/MVC007947
    if (-1 != GetProbeMode()) {
        SendMsgToProbe(&l_msg);
    }

    //-Patni-RUP/2010Jun09/Added/MVC007947
}
//-Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect


//***************************Method Header*************************************
//Method Name    : StopProbeOnError
//Author         : PATNI/Sribanta
//Purpose        : Get Thread ID.
//*****************************************************************************
void CPqmProbe::StopProbeOnError(
    const bool f_stop_explicit /*= false*/
)
{

    LPCTSTR FUNC_NAME = _T("CPqmProbe::StopProbeOnError");

    CPqm* l_pqm = m_procon->GetPQM();

    if (NULL == l_pqm) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_pqm pointer is NULL"));
        return;
    }

    CPqmPmAcqman* l_acqman_ptr = l_pqm->GetAcqManager();

    if (NULL == l_acqman_ptr) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_acqman_ptr pointer is NULL"));
        return;
    }

    CScanProcedure* l_scan_procedure = l_acqman_ptr->GetScanProcedure();

    if (NULL == l_scan_procedure) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_scan_procedure pointer is NULL"));
        return;
    }

    if (l_scan_procedure->GetProbing() || f_stop_explicit) {
        m_procon->StopProlim();
        l_scan_procedure->SetProbing(FALSE);

        if (!f_stop_explicit) {
            l_scan_procedure->AcqProbingScanAbort();
        }
    }
}

//***************************Method Header*************************************
//Method Name    : GetThreadID
//Author         : PATNI/Sribanta
//Purpose        : Get Thread ID.
//*****************************************************************************
DWORD CPqmProbe::GetThreadID()
{
    return m_dThreadId;
}


//***************************Method Header*************************************
//Method Name    : GetProbeMode
//Author         : PATNI/MSN
//Purpose        : Set Probe Mode During Plan/During Scan
//*****************************************************************************
int CPqmProbe::GetProbeMode()const
{
    return m_refprobe_mode;
}

//***************************Method Header*************************************
//Method Name    : SetProbeMode
//Author         : PATNI/MSN
//Purpose        : Set Probe Mode During Plan/During Scan
//*****************************************************************************
void CPqmProbe::SetProbeMode(const int f_mode)
{
    m_refprobe_mode = f_mode;
}

//+Patni-RUP/2010Jun09/Added/MVC007947 Scenario II
//******************************************************************************
//Function Name:- GetProbeFlagFromShMemmory()
//Author       :- PATNI\RUP
//Purpose      :- This function is used to get PL_PROBE_FLAG from shared memory
//******************************************************************************
bool CPqmProbe::GetProbeFlagFromShMemory(BOOL& f_probe_flag)const
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::GetProbeFlagFromShMemory");

    try {
        if (NULL == m_procon) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_procon is NULL"));
            return false;
        }

        PqmPmProlimT* l_pqmpm_prolim = m_procon->GetPqmPmProlim();

        if (NULL != l_pqmpm_prolim && !l_pqmpm_prolim->treeId) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid tree Id"));
            return false;
        }

        plStatusT l_status = plAttachTree();

        if (plStatSuccess != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("plAttachTree API failed"));
            return false;
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Get the PL_PROBE_FL value from prolim"));

        plValT l_probe_flag;
        l_status = plGetVal(0, SVD_ST_STUDY, PL_PROBE_FL, &l_probe_flag);

        if (plStatSuccess != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("plGetVal API failed"));
            plDetachTree();
            return false;
        }

        f_probe_flag = PL_VAL_BOOL(l_probe_flag);
        plDetachTree();

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Exception occured. Failed to get PL_PROBE_FL value from prolim"));
        return false;
    }

    return true;
}
//-Patni-RUP/2010Jun09/Added/MVC007947

//***************************Method Header*************************************
//Method Name    : OnRefProbeDisconnect
//Author         : PATNI/SG
//Purpose        : Send Scan Start Message to RefProbe
//*****************************************************************************
void CPqmProbe::OnRefProbeDisconnect(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::OnRefProbeDisconnect");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("RefProbe has Disconnected ! "));

    if (NULL == m_procon) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_procon pointer is NULL"));
        return;
    }

    if (NULL == m_acqman_ptr) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_acqman_ptr pointer is NULL"));
        return;
    }

    CPqm* l_pqm = m_procon->GetPQM();

    if (NULL == l_pqm) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_pqm pointer is NULL"));
        return;
    }

    CPqmPmAcqman* l_acqman_ptr = l_pqm->GetAcqManager();

    if (NULL == l_acqman_ptr) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_acqman_ptr pointer is NULL"));
        return;
    }

    CScanProcedure* l_scan_procedure = l_acqman_ptr->GetScanProcedure();

    if (NULL == l_scan_procedure) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_scan_procedure pointer is NULL"));
        return;
    }

    CPqm* l_pqmptr = m_acqman_ptr->GetPqm();

    if (NULL == l_pqmptr) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_pqmptr pointer is NULL"));
        return;
    }

    CPqmProtocol* l_curr_protocol = l_scan_procedure->GetScanProcProtocol();
    CPQMStudy* l_study = l_pqmptr->GetStudy();

    if (NULL == l_study) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_study pointer is NULL"));
        return;
    }

    if (l_curr_protocol) {
        l_pqmptr->SetRefProbeNotAlive(false);

        if (!l_scan_procedure->SendAcquisition()) {

            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
            //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_FAIL_CONNECT_ACQMAN, _T("IDS_FAIL_CONNECT_ACQMAN"),
            //        SCAN_ERR_NOTIFY, L"PqmProbe.cpp");
            //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

            l_study->SetScanModeOfHead();
            l_scan_procedure->ResetCurrentRequest();
        }
    }
}

//+Patni-RUP/2009Dec15/Internal defect fix
void CPqmProbe::CreatePQMServer()
{
    m_server = new CPqmProbeServer(this);
}

//*******************************Method Header*********************************
//Method Name    : ThreadFunc()
//Author         : PATNI/SG
//Purpose        :
//*****************************************************************************
DWORD WINAPI CPqmProbe::ThreadFunc(
    LPVOID f_lpparam
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::ThreadFunc");
    CPqmProbe* l_self = (CPqmProbe*)f_lpparam;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    //+ Patni-MSN/2009Apr02/Added/PSP- According to Base code
    CPqmPmAcqman* l_acqman_ptr = m_procon->GetPQM()->GetAcqManager();
    CScanProcedure* l_scan_proc = l_acqman_ptr->GetScanProcedure();
    //- Patni-MSN/2009Apr02/Added/PSP- According to Base code

    MSG msg;

    while (1) {

        GetMessage(&msg, NULL, 0, 0);

        if (msg.message == MSG_PROBE) {
            //+Patni-MSN/2009Mar10/Added/Optimization
            //+Patni-RUP/2010May27/Added/MVC007947
            CString l_page_name = _T("");

            if (l_self->m_procon) {
                if (l_self->m_procon->GetPQM()->GetMode() == K_IMAGING) {
                    l_page_name = _T("Acquire");
                    //Patni-HAR/2010June18/Modified/MVC007947

                } else if (l_self->m_procon->GetPQM()->GetMode() == K_SCHEDULING) {
                    l_page_name = _T("Schedule");

                } else {
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Invalid page Name"));
                }
            }

            int ret = ShowProbePlan(m_refprobe_mode, l_page_name, l_self->m_show_probe_dlg); //Patni-RUP/2010Jun09/Added/MVC007947
            //-Patni-RUP/2010May27/Added/MVC007947

            if ((ret == -1) || (ret == IDABORT)) {
                if (m_refprobe_mode == 0) {  //Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094
                    proMsg_t l_msg;
                    l_msg.code = PROMSG_CMD_PROBE_NG;
                    l_msg.length = m_procon->GetDataForProbe(&l_msg.data);

                    //+Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
                    if (l_msg.data != NULL) {
                        CString l_msg_data = _T("");
                        l_msg_data.Format(_T("msg data - %s"), l_msg.data);

                        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_msg_data) ;
                    }

                    //-Patni-ARD/2009Aug12/Added/Defect fix #MVC003833
                    m_procon->SendMessageToProcon(l_msg.code, l_msg.length, l_msg.data);
                }    //Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094

            } else {
                if (m_refprobe_mode == 0) {  //Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094
                    proMsg_t l_msg;
                    l_msg.code = PROMSG_PROBE_EXIT;
                    l_msg.length = 0;
                    l_msg.data = NULL;
                    m_procon->SendMessageToProcon(l_msg.code, l_msg.length, l_msg.data);
                }    //Patni-RUP/2010Mar02/Modified/To support Vantage V9.50 features - MCM0229 - 00094

                //+ Patni-MSN/2009Apr02/Added/PSP- According to Base code
                l_scan_proc->SetProbing(FALSE);
                //- Patni-MSN/2009Apr02/Added/PSP- According to Base code
            }

            //-Patni-MSN/2009Mar10/Added/Optimization
            //+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
            if (NULL != l_self) {

                //+Patni-Hemant/2010Nov13/Modified/IR-167_REOPEN_ISSUE
                //l_self->m_refprobe_mode = -1;
                m_procon->OnProbeClosed();
                //-Patni-Hemant/2010Nov13/Modified/IR-167_REOPEN_ISSUE
            }

            //-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        }

        if (msg.message == MSG_CLOSE) {
            break;
        }
    }

    //20091006 Tanoue-san for CoUninitialize
    CoUninitialize();
    return 0;
}

//****************************Method Header************************************
//Method Name   : GetMsgStrFromCode()
//Author        : PATNI\RUP
//Purpose       : To message string from message code
//*****************************************************************************
CString CPqmProbe::GetMsgStrFromCode(const int f_msg_code)const
{
    CString l_msg_str = _T("");

    switch (f_msg_code) {
        case PROMSG_PROBE_RUN:
            l_msg_str = _T("PROMSG_PROBE_RUN");
            break;

        case PROMSG_PROBE_EXIT:
            l_msg_str = _T("PROMSG_PROBE_EXIT");
            break;

        case PROMSG_CMD_PROBE_OK:
            l_msg_str = _T("PROMSG_CMD_PROBE_OK");
            break;

        case PROMSG_PROBE_STOP:
            l_msg_str = _T("PROMSG_PROBE_STOP");
            break;

        case PROMSG_CMD_START_PROBE_PLAN:
            l_msg_str = _T("PROMSG_CMD_START_PROBE_PLAN");
            break;

        case PROBING_THRES_HOLD:
            l_msg_str = _T("PROBING_THRES_HOLD");
            break;

        case PROMSG_PQM_PROBE_PLAN_EXIT:
            l_msg_str = _T("PROMSG_PQM_PROBE_PLAN_EXIT");
            break;

        case PROMSG_CON_PROBE_RUN:
            l_msg_str = _T("PROMSG_CON_PROBE_RUN");
            break;

        case RMC_START_WAIT_CURSOR:
            l_msg_str = _T("RMC_START_WAIT_CURSOR");
            break;

        case RMC_STOP_WAIT_CURSOR:
            l_msg_str = _T("RMC_STOP_WAIT_CURSOR");
            break;

        case PROMSG_CON_PROBE_STOP:
            l_msg_str = _T("PROMSG_CON_PROBE_STOP");
            break;

        case RMC_CMD_EXIT:
            l_msg_str = _T("RMC_CMD_EXIT");
            break;

        case RMC_CMD_SCAN_END:
            l_msg_str = _T("RMC_CMD_SCAN_END");
            break;

            //+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
        case PROMSG_CON_ALLOW_SELECTIONS:
            l_msg_str = _T("PROMSG_CON_ALLOW_SELECTIONS");
            break;

        case PROMSG_CON_TREE_UPDATED:
            l_msg_str = _T("PROMSG_CON_TREE_UPDATED");
            break;

            //-Patni-RUP/2010Feb09/Added/To support Vantage V9.50 features - MCM0229 - 00094
            //+Patni-RUP/2010Feb16/Added/To fix Issue no 6 reported by TMSC
        case RMC_INITIALZE_SHARED_DATA:
            l_msg_str = _T("RMC_INITIALZE_SHARED_DATA");
            break;

            //-Patni-RUP/2010Feb16/Added/To fix Issue no 6 reported by TMSC
            //+Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
        case RMC_SHOW_PROBE_WINDOW:
            l_msg_str = _T("RMC_SHOW_PROBE_WINDOW");
            break;

        case RMC_HIDE_PROBE_WINDOW:
            l_msg_str = _T("RMC_HIDE_PROBE_WINDOW");
            break;

            //-Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
        default:
            l_msg_str = _T("UNKNOWN_MESSAGE");
            break;
    }

    return l_msg_str;
}
//-Patni-RUP/2009Dec15/Internal defect fix
//+Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
//******************************************************************************
//Function Name:- GetShrMemSegDataForProbe()
//Author       :- PATNI\RUP
//Purpose      :- This function is used to get Tr,ROResolution,Site Id, run
//                number values from the using libPLTree
//******************************************************************************
bool CPqmProbe::GetShrMemSegDataForProbe(shared_mem_data_t* const f_shared_mem_data)const
{

    LPCTSTR FUNC_NAME = _T("CPqmProbe::GetShrMemSegDataForProbe");

    try {
        if (NULL == f_shared_mem_data) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_shared_mem_data is NULL"));
            return false;
        }

        if (NULL == m_procon) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_procon is NULL"));
            return false;
        }

        PqmPmProlimT* l_pqmpm_prolim = m_procon->GetPqmPmProlim();

        if (NULL != l_pqmpm_prolim && !l_pqmpm_prolim->treeId) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid tree Id"));
            return false;
        }

        plStatusT l_status = plStatSuccess;
        l_status = plAttachTree();

        if (plStatSuccess != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("plAttachTree API failed"));
            return false;
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Get the PL_TR & PL_RO_RESOLUTION value from prolim"));

        plValT l_val_tr;
        plValT l_val_ROR;
        plValT l_run_val;
        plValT l_site_val;
        l_status = plVaGetProtocolVal(PL_TR, &l_val_tr,
                                      PL_RO_RESOLUTION, &l_val_ROR,
                                      PL_SITE_ID, &l_site_val,
                                      PL_RUN_NUM, &l_run_val,
                                      NULL);

        if (plStatSuccess != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("plVaGetProtocolVal API failed"));
            plDetachTree();
            return false;
        }

        f_shared_mem_data->m_tr = PL_VAL_REAL(l_val_tr);
        f_shared_mem_data->m_resolution = PL_VAL_REAL(l_val_ROR);
        f_shared_mem_data->m_site_id = PL_VAL_INT(l_site_val);
        f_shared_mem_data->m_run_num = PL_VAL_INT(l_run_val);
        plDetachTree();


    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Exception occured. Failed to get PL_TR & PL_RO_RESOLUTION values from prolim"));
        return false;
    }

    return true;
}
//-Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC

//***************************Method Header*************************************
//Method Name    : SetAcqmanPointer
//Author         : PATNI/SG
//Purpose        : Send Scan Start Message to RefProbe
//*****************************************************************************
BOOL CPqmProbe::SetAcqmanPointer(
    CPqmPmAcqman* f_acqman_ptr
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProbe::SetAcqmanPointer");

    ASSERT(f_acqman_ptr != NULL);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmProbe::SetAcqmanPointer"));
    m_acqman_ptr = f_acqman_ptr;

    return true;
}
//***************************Method Header*************************************
//Method Name    : GetPqmProCon
//Author         : iGate
//Purpose        : Return procon pointer
//*****************************************************************************
CPqmProcon*	 const CPqmProbe::GetPqmProCon(
)const
{
    return	m_procon;
}