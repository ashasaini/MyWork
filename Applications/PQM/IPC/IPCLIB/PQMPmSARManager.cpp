
#include "stdafx.h"
#include <windows.h>
#include "PqmPmSARManager.h"

#include <PQM/PQMIPCManager.h>
#include <PQM/PqmUtilities.h>
#include "Pqm.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "GPLibAdapter.h"
#include "PQMSAR.h"

const int MAX_TIMER_TIME_DIFF = 370;

using namespace LIBSARMGR;

//********************************Method Header********************************
//Method Name   :ConnectToSARManager()
//Author        :PATNI
//Purpose       :Connect To SAR Manager
//*****************************************************************************
BOOL CPqmPmSARManager::ConnectToSARManager()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::ConnectToSARManager()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return FALSE;
    }

    if (INVALID_HANDLE_VALUE == m_module || NULL == m_module || NULL == m_lib_sar_manager) {

        m_module = LoadLibrary(_T("LibSARManager.dll"));

        if (INVALID_HANDLE_VALUE == m_module || NULL == m_module) {
            m_lib_sar_manager = NULL;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("LoadLibrary for LibSARManager.dll failed"));
            return FALSE;
        }


        typedef CLibSARManager* (*FUNC_PTR_INSTANCE)();
        FUNC_PTR_INSTANCE l_instance_ptr = (FUNC_PTR_INSTANCE)GetProcAddress(m_module, "GetInstance");

        if (NULL == l_instance_ptr) {
            FreeLibrary(m_module);
            m_module = NULL;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetInstance for LibSARManager.dll failed"));
            return FALSE;
        }

        m_lib_sar_manager = l_instance_ptr();

        if (NULL == m_lib_sar_manager) {
            FreeLibrary(m_module);
            m_module = NULL;
            return FALSE;
        }

        m_lib_sar_manager->SetClientAppPtr(m_libsar_mgr_client_interface);
    }

    if (m_lib_sar_manager->ConnectToSARManager()) {
        m_pqmptr->WriteToUI(PQM_MSG_SARMANAGER_CONNECTED);
        return TRUE;
    }

    return FALSE;
}

//********************************Method Header********************************
//Method Name   :OnSARMgrDisconnect()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CPqmPmSARManager::OnSARMgrDisconnect()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::OnSARMgrDisconnect()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    m_pqmptr->GetManagerPtr()->SetIsSARMgrConnected(FALSE);


    if (m_pqmptr->GetPqmProCon()->GetCurrentProtocol()) {
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPqmUtilities::GetMultiLingualString(_T("IDS_SEQ_EDIT_END_ON_SAR_DISCONNECT")),
                MPlus::EH::Information, _T("PqmPmSARManager.cpp"), _T("PQM"));
    }

    m_pqmptr->WriteToUI(PQM_MSG_SARMANAGER_DISCONNECTED);
}


//********************************Method Header********************************
//Method Name   :SendSessionStartMsgToSARMgr()
//Author        :PATNI
//Purpose       :send session start message to SARMgr
//*****************************************************************************
void CPqmPmSARManager::SendSessionStartMsgToSARMgr() const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::SendSessionStartMsgToSARMgr()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    const BOOL l_is_sar_mgr_connected = m_pqmptr->GetManagerPtr()->GetIsSARMgrConnected();

    //If not connected, it libSARManager will try to reconnect before sending message.
    //First time handling is required only.
    if ((m_lib_sar_manager && m_lib_sar_manager->SendSessionStartMsgToSARMgr())) {


        if (!l_is_sar_mgr_connected) {
            m_pqmptr->GetManagerPtr()->SetIsSARMgrConnected(TRUE);
            m_pqmptr->WriteToUI(PQM_MSG_SARMANAGER_CONNECTED);
        }
    }

    //Once connected, if it gets disconnt, OnSARMgrDisconnect() will be called by libSARManager.

}

//********************************Method Header********************************
//Method Name   :SendSessionEndMsgToSARMgr()
//Author        :PATNI\Nilesh
//Purpose       :Send PQM Sesstion End message to SAR Manager.
//*****************************************************************************
void CPqmPmSARManager::SendSessionEndMsgToSARMgr() const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::SendSessionEndMsgToSARMgr()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    if (NULL == m_lib_sar_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_lib_sar_manager is NULL."));
        return;
    }

    m_lib_sar_manager->SendSessionEndMsgToSARMgr();
}



//********************************Method Header********************************
//Method Name   :SendRTSARInfoForLastDoneProtocol()
//Author        :PATNI\AKR
//Purpose       :Send message to SAR manager for real time sar info on scan end
//*****************************************************************************
void CPqmPmSARManager::SendRTSARInfoForLastDoneProtocol()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::SendRTSARInfoForLastDoneProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }


    if (NULL == m_lib_sar_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_lib_sar_manager is NULL"));
        return;
    }

    CPqmProtocol* l_curr_protocol = m_pqmptr->GetStudy()->GetLastCompletedProtocol();

    if (NULL == l_curr_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("No done or fail protocol"));
        return;
    }


    int l_history_data_count = 0;
    int l_prot_position = l_curr_protocol->GetPosition();

    VfFieldSpec_t  l_vfreq[NUM_5] = {0};
    VfPathElem_t    l_pelm[NUM_2] = {0};

    flt32_t* l_historysar_wb = NULL, *l_historysar_hd = NULL, *l_historysar_pb = NULL, *l_historysar_preal = NULL;
    char l_system_time[20] = {0};

    while (l_prot_position > 0) {

        l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_pelm[SVD_ST_PROTOCOL - 1].index = l_curr_protocol->GetProtocol();

        int l_n = 0;

        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_RTSAR_EXPOSURE_HISTORY_WB, SVT_EXE_RTSAR_EXPOSURE_HISTORY_WB,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        l_n++;

        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_RTSAR_EXPOSURE_HISTORY_HEAD, SVT_EXE_RTSAR_EXPOSURE_HISTORY_HEAD,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        l_n++;

        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_RTSAR_EXPOSURE_HISTORY_PARTIAL, SVT_EXE_RTSAR_EXPOSURE_HISTORY_PARTIAL,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        l_n++;

        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_RTSAR_EXPOSURE_HISTORY_PREAL, SVT_EXE_RTSAR_EXPOSURE_HISTORY_PREAL,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        l_n++;

        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_N_RTSAR_END_POINT_TIME, SVT_EXE_N_RTSAR_END_POINT_TIME,
                     sizeof(l_system_time), &l_system_time, NULL);
        l_n++;


        int l_num = 0;
        status_t l_status = m_pqmptr->GetStudy()->GetVfDBHandle()->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, l_n, &l_num);

        if (l_status != E_NO_ERROR || l_n != l_num) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in RTSAR History tags are not present."));

            for (int l_count = 0; l_count < l_n - 1; ++l_count) {

                if (VFE_NO_ERROR == l_vfreq[l_count].status) {
                    FREE_PTR(l_vfreq[l_count].buffer);
                }
            }

            l_curr_protocol = m_pqmptr->GetStudy()->GetProtocolForIndex(--l_prot_position);

            if (NULL == l_curr_protocol) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_protocol is NULL"));
                return;
            }

            continue;

        } else {


#define BUILD_DATA(p,index)         l_history_data_count = l_vfreq[index].size/sizeof(flt32_t);	    \
    p = new flt32_t[l_history_data_count];								\
    memset(p, 0, l_vfreq[index].size);										\
    memcpy(p,l_vfreq[index].buffer, l_vfreq[index].size);						\
    FREE_PTR(l_vfreq[index].buffer);												\
     

            try {

                BUILD_DATA(l_historysar_wb, 0);
                BUILD_DATA(l_historysar_hd, 1);
                BUILD_DATA(l_historysar_pb, 2);
                BUILD_DATA(l_historysar_preal, 3);

            } catch (CMemoryException* l_exp) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("BUILD_DATA failed"));
                l_exp->Delete();
                throw;
            }

#undef BUILD_DATA

            break;
        }
    }

    SYSTEMTIME l_rt_sar_time = CPqmUtilities::ConvertLocalTimeStringToSystemTime(l_system_time);

    m_lib_sar_manager->SendRTSARInfoForLastDoneProtocol(l_history_data_count, &l_rt_sar_time, l_historysar_wb, l_historysar_pb, l_historysar_hd, l_historysar_preal);


    DEL_PTR_ARY(l_historysar_wb);
    DEL_PTR_ARY(l_historysar_hd);
    DEL_PTR_ARY(l_historysar_pb);
    DEL_PTR_ARY(l_historysar_preal);

    const int l_history_time_diff_sec = CalcHistoryTimeDiff(&l_rt_sar_time);

    if (l_history_time_diff_sec < MAX_TIMER_TIME_DIFF) {
        m_max_pager_timer_time = (MAX_TIMER_TIME_DIFF - l_history_time_diff_sec);
        m_pqmptr->WriteToUI(PQM_MSG_START_SAR_UPDATE_TIMER);
    }
}

//********************************Method Header********************************
//Method Name   :ReInvokeSARManager()
//Author        :PATNI-AKR
//Purpose       :Reinvoke SARManager
//*****************************************************************************
void CPqmPmSARManager::ReInvokeSARManager()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::ReInvokeSARManager()");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_SARMGR, FUNC_NAME, _T("Trace."));

    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    CPQMStudy* l_study = m_pqmptr->GetStudy();

    if (NULL == l_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
        return ;
    }

    if (l_study->GetWeight() <= 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetWeight() > 0 is false."));
        return ;
    }

    SendSessionStartMsgToSARMgr();
    SendRTSARInfoForLastDoneProtocol();
    CalcSARControlAndPutVFForAll();
}

//********************************Method Header********************************
//Method Name   :HandleAbortFailforSARControl()
//Author        :PATNI-AKR
//Purpose       :
//*****************************************************************************
void CPqmPmSARManager::HandleAbortFailforSARControl()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::HandleAbortFailforSARControl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    SendRTSARInfoForLastDoneProtocol();
    CalcSARControlAndPutVFForAll();
}


//********************************Method Header********************************
//Method Name   :CalcSARControlAndPutVFForAll()
//Author        :iGate/
//Purpose       :
//*****************************************************************************
void CPqmPmSARManager::CalcSARControlAndPutVFForAll(BOOL f_is_scanning/*=FALSE*/)
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::CalcSARControlAndPutVFForAll");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    if (!CanProcessSARManager()) {

        m_pqmptr->UpdateSARInfoOnPage();

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    if (m_pqmptr->GetSAR()->GetRflParams().rflTxGain <= MIN_SAR) {

        m_pqmptr->UpdateSARInfoOnPage();
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SAR(p) is not calculated, return"));
        return ;
    }


    CPqmProtocol* l_curr_prot = m_pqmptr->GetStudy()->GetTopWaitOrCurrentProtocol();

    if (NULL == l_curr_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("No wait protocols or current protocol"));
    }

    sarControlWaitProtocols_t l_sarcontrolwaitprotocols;
    memset(&l_sarcontrolwaitprotocols, 0, sizeof(l_sarcontrolwaitprotocols));


    l_sarcontrolwaitprotocols.start_gui_index = l_curr_prot ? l_curr_prot->GetPosition() : 0;

    if (l_curr_prot) {
        if (f_is_scanning) {

            l_sarcontrolwaitprotocols.protocol_info[0].skipSaveFlag = FALSE;

        } else if (C_PR_STATUS_CURRENT == l_curr_prot->GetAcqStatus() &&
                   !m_pqmptr->GetAcqManager()->GetScanProcedure()->GetIsWaitingFlagForSARCtrl()) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Set current protocol's skipSaveFlag TRUE"));

            l_sarcontrolwaitprotocols.protocol_info[0].skipSaveFlag = TRUE;
        }
    }

    sarInfo_p l_sar_info = NULL;

    for (int l_index = 0; NULL != l_curr_prot ; l_curr_prot = m_pqmptr->GetStudy()->GetNextItemFromPos(l_curr_prot), ++l_index) {

        l_sar_info = l_curr_prot->GetSarInfo();

        l_sar_info->totalAcquisitionTime =  l_curr_prot->GetScanTime();

        l_sarcontrolwaitprotocols.protocol_info[l_index].protocol_index = l_curr_prot->GetProtocol();
        l_sarcontrolwaitprotocols.protocol_info[l_index].sar_info = l_sar_info;

    }

    l_sarcontrolwaitprotocols.protocol_num = l_index;

    ClibSARAdapter sarAdapter(m_pqmptr->GetStudy()->GetVfDBHandle());
    sarAdapter.SarControlCalcAndPutVfForAll(&l_sarcontrolwaitprotocols);


    m_pqmptr->UpdateSARInfoOnPage();
    m_pqmptr->WriteToUI(PQM_MSG_UPDATE_GUI_FOR_SAR_OVER);
}

//********************************Method Header********************************
//Method Name   :CPqmPmSARManager()
//Author        :PATNI
//Purpose       :constructor
//*****************************************************************************
CPqmPmSARManager::CPqmPmSARManager(
    CPqm* f_pqmptr
):	m_pqmptr(f_pqmptr),
    m_lib_sar_manager(NULL),
    m_libsar_mgr_client_interface(NULL),
    m_module(NULL),
    m_max_pager_timer_time(0)

{
    m_libsar_mgr_client_interface = new CLibSARMgrClientAppInterface(this);
}
//********************************Method Header********************************
//Method Name   :~CPqmPmSARManager()
//Author        :PATNI
//Purpose       :destructor
//*****************************************************************************
CPqmPmSARManager::~CPqmPmSARManager()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::~CPqmPmSARManager");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    typedef void(*PFUNC_DelInstance)(void);
    PFUNC_DelInstance delinstance_ptr = (PFUNC_DelInstance)GetProcAddress(m_module, "DeleteInstance");

    if (NULL != delinstance_ptr) {
        delinstance_ptr();
        m_lib_sar_manager = NULL;
    }

    FreeLibrary(m_module);
    m_module = NULL;

    DEL_PTR(m_libsar_mgr_client_interface);

}
//********************************Method Header********************************
//Method Name   :CreateScanEditBuffer()
//Author        :PATNI/pruthviraj
//Purpose       :
//*****************************************************************************
void CPqmPmSARManager::CreateScanEditBuffer(const int32_t f_current_acq_order)
{

    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::CreateScanEditBuffer()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);


    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    if (NULL == m_lib_sar_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_lib_sar_manager is NULL."));
        return;
    }

    m_lib_sar_manager->SarControlCreateScanEditBuffer(f_current_acq_order);
}
//********************************Method Header********************************
//Method Name   :UpdateScanEditBuffer()
//Author        :PATNI/pruthviraj
//Purpose       :
//*****************************************************************************
void CPqmPmSARManager::UpdateScanEditBuffer()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::UpdateScanEditBuffer()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);


    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    if (NULL == m_lib_sar_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_lib_sar_manager is NULL."));
        return;
    }

    m_lib_sar_manager->SarControlUpdateScanEditBuffer();
}
//********************************Method Header********************************
//Method Name   :SendSequenceEditEndMsgToSARMgr()
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
void CPqmPmSARManager::ClearScanEditBuffer()
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::ClearScanEditBuffer()");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);


    if (!CanProcessSARManager()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Processing not required, return"));
        return ;
    }

    if (NULL == m_lib_sar_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_lib_sar_manager is NULL."));
        return;
    }

    m_lib_sar_manager->SarControlClearScanEditBuffer();
}


//********************************Method Header********************************
//Method Name   :CanProcessSARManager()
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
bool CPqmPmSARManager::CanProcessSARManager() const
{
    LPCTSTR FUNC_NAME = _T("CPqmPmSARManager::CanProcessSARManager");

    if (SARCTRL_LICENCE_ON != m_pqmptr->GetSARControlLicense()) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_SARMGR, FUNC_NAME, _T("SAR Control license is not present."));
        return false;
    }

    if (K_SCHEDULING == m_pqmptr->GetMode()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Schedule mode, return"));
        return false;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanProcessSARManager, true"));

    return true;
}

//****************************Method Header************************************
//Method Name	: CalcCurrentIdleTime()
//Author		: iGate/
//Purpose		:
//*****************************************************************************
int CPqmPmSARManager::CalcHistoryTimeDiff(const LPSYSTEMTIME f_rt_sar_time) const
{

    CTime l_rt_history_time(f_rt_sar_time->wYear,
                            f_rt_sar_time->wMonth,
                            f_rt_sar_time->wDay,
                            f_rt_sar_time->wHour,
                            f_rt_sar_time->wMinute,
                            f_rt_sar_time->wSecond);

    SYSTEMTIME l_current_system_time;
    GetSystemTime(&l_current_system_time);

    CTime l_curent_time(l_current_system_time.wYear,
                        l_current_system_time.wMonth,
                        l_current_system_time.wDay,
                        l_current_system_time.wHour,
                        l_current_system_time.wMinute,
                        l_current_system_time.wSecond);

    CTimeSpan l_time_diff = l_curent_time - l_rt_history_time;
    return l_time_diff.GetTotalSeconds();
}

//****************************Method Header************************************
//Method Name	: ResetSarPreScanParam()
//Author		: iGate/
//Purpose		:
//*****************************************************************************
void CPqmPmSARManager::ResetSarPreScanParam(sarInfo_p f_sar_info) const
{
    sarVfPreScanParam_t l_vf_prescan_param = {0};
    ::SarSetPreScanParam(f_sar_info, &l_vf_prescan_param);
}
