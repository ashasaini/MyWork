// AutoLocatorAppController.cpp: implementation of the CAutoLocatorAppController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoLocatorAppController.h"
#include "DPSManager.h"
#include "PQMView.h"

extern CPQMView* g_MainView;

using namespace MR::ACQ::SM::COMMON;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoLocatorAppController
//Author        : PATNI
//Purpose       : Constructor
//**************************************************************************
CAutoLocatorAppController::CAutoLocatorAppController():
    m_processtool_thread(NULL),
    m_thread_id(0),
    m_process_id(-1),
    m_prot_index_workg_mode(_T("")),
    m_load_gl_flag(false),
    m_easytech_started_event(NULL)
{

}
//**************************************************************************
//Method Name   : CAutoLocatorAppController
//Author        : PATNI
//Purpose       : Desstructor
//**************************************************************************
CAutoLocatorAppController::~CAutoLocatorAppController()
{
    if (NULL != m_easytech_started_event) {
        CloseHandle(m_easytech_started_event);
    }
}
//**************************************************************************
//Method Name   : InvokeAutoLocatorApplication
//Author        : PATNI
//Purpose       : Invokes autolocator application
//**************************************************************************
BOOL CAutoLocatorAppController::InvokeAutoLocatorApplication(const AUTOLOCATOR_TYPE f_autolocator_type,
        const bool f_cala_invoke_by_menu)
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorAppController::InvokeAutoLocatorApplication");
    int* selected_seq = NULL;
    int l_seq_count = g_MainView->GetPQMListCtrl().GetselectedItems(&selected_seq);

    if (NULL == selected_seq) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("selected_seq is NULL"));
        return FALSE;
    }

    VARIANT* pData = new VARIANT;

    if (f_cala_invoke_by_menu) {
        CSequence* l_seq = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(selected_seq[0]);

        if (NULL == l_seq) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("l_seq is NULL"));
            return FALSE;
        }

        int l_acq_order = -1;
        l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

        g_MainView->GetAcquisition()->GetProtIndexAndWorkingMode(l_acq_order, pData);
        m_prot_index_workg_mode = pData->bstrVal;
        int l_index = m_prot_index_workg_mode.Find(_T("\\"));
        TCHAR l_val = DEFAULT_AL_INVOKE_MODE;
        m_prot_index_workg_mode.SetAt((l_index + 1), l_val);

    } else {
        g_MainView->GetAcquisition()->GetProtIndexAndWorkingModeForDoneProt(pData);
        m_prot_index_workg_mode = pData->bstrVal;
    }

    CString l_result;
    l_result.Format(_T("index and Invoke mode : %s"), m_prot_index_workg_mode);
    PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, l_result);

    m_easytech_started_event = CreateEvent(NULL, TRUE, TRUE, PQM_AUTOLOCATOR_WAIT_FOR_REPLY);

    if (NULL != m_easytech_started_event) {
        ResetEvent(m_easytech_started_event);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENT RESET before sending LOADGL to Easytech"));
    }

    m_load_gl_flag = true;

    BOOL l_ret = FALSE;

    if (AUTOLOCATOR_NEURO == f_autolocator_type) {
        l_ret = CDPSManager::GetInstance()->SendASyncMsgToParentPage(DPS_MSGID_FW_LOADGL, DPS_HALA_PARAM, DPS_CALA_CUSTOM);

    } else if (AUTOLOCATOR_CARDIAC == f_autolocator_type) {
        l_ret = CDPSManager::GetInstance()->SendASyncMsgToParentPage(DPS_MSGID_FW_LOADGL, DPS_CALA_PARAM, DPS_CALA_CUSTOM);

    } else {
        l_ret =  FALSE;
    }

    if (TRUE == l_ret) {
        return TRUE;

    } else {
        m_load_gl_flag =  false;

        if (NULL != m_easytech_started_event) {
            SetEvent(m_easytech_started_event);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Easytech EVENT SET as Handle closed as LOADGL FAILED"));
        }

        return FALSE;
    }
}
//**************************************************************************
//Method Name   : GetAutoLocatorProcessID
//Author        : PATNI
//Purpose       : To get process id of Autolocator application running
//**************************************************************************
int CAutoLocatorAppController::GetAutoLocatorProcessID() const
{
    return m_process_id;
}
//**************************************************************************
//Method Name   : SetAutoLocatorProcessID
//Author        : PATNI
//Purpose       : To set process id of Autolocator application running
//**************************************************************************
void CAutoLocatorAppController::SetAutoLocatorProcessID(const int f_cala_process_id)
{
    m_process_id = f_cala_process_id;
}
//**************************************************************************
//Method Name   : WaitForAutoLocatorProcess
//Author        : PATNI
//Purpose       : To wait for autolocator application to get closed
//**************************************************************************
void CAutoLocatorAppController::WaitForAutoLocatorProcess(const int f_process_id)
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorAppController::WaitForAutoLocatorProcess");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside function"));
    m_process_id = f_process_id;
    m_processtool_thread = (HANDLE)_beginthreadex(0,
                           0,
                           CPQMUtility::WaitForCALAApplcationProcess,
                           g_MainView,
                           0,
                           &m_thread_id);

    if (!m_processtool_thread) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T(" failed to create thread handle"));
    }
}
//**************************************************************************
//Method Name   : GetProtIndexAndWorkingMode
//Author        : PATNI
//Purpose       : To get protocol index working mode
//**************************************************************************
CString CAutoLocatorAppController::GetProtIndexAndWorkingMode()const
{
    return m_prot_index_workg_mode;
}
//**************************************************************************
//Method Name   : SetProtIndexAndWorkingMode
//Author        : PATNI
//Purpose       : To set protocol index working mode
//**************************************************************************
void CAutoLocatorAppController::SetProtIndexAndWorkingMode(const CString& f_cala_prot_index_workg_mode)
{
    m_prot_index_workg_mode = f_cala_prot_index_workg_mode;
}
void CAutoLocatorAppController::SetLoadGLFlag(const bool f_loadglflag)
{
    m_load_gl_flag = f_loadglflag ;
}
bool CAutoLocatorAppController::GetLoadGLFlag()
{
    return m_load_gl_flag;
}

