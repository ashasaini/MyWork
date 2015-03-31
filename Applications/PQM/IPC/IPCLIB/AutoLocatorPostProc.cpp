// AutoLocatorPostProc1.cpp: implementation of the CAutoLocatorPostProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoLocatorPostProc.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include "Pqm.h"
#include "CheckFOVInMap.h"
#include "PQMDBSALib.h"
#include "GPLibAdapter.h"


using namespace MR::ACQ::SM::COMMON::MUTEX;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoLocatorPostProc
//Author        : PATNI
//Purpose       : Constructor
//**************************************************************************
CAutoLocatorPostProc::CAutoLocatorPostProc(
    CPqm* f_pqm
): m_pqm_ptr(f_pqm),
    m_auto_locator_running(false)
{
    char*	timebuf = getenv("CALA_RECON_TIMEOUT");

    if (timebuf != NULL) {
        m_recon_timeout = atoi(timebuf);

    } else {
        m_recon_timeout = TIME_OUT_VAL_FOR_RECON;

    }
}
//**************************************************************************
//Method Name   : ~CAutoLocatorPostProc
//Author        : PATNI
//Purpose       : Destructor
//**************************************************************************
CAutoLocatorPostProc::~CAutoLocatorPostProc()
{

}
//**************************************************************************
//Method Name   : HandleAutoLocatorInvocation
//Author        : PATNI
//Purpose       : TO handle the invocation of Autolocator
//**************************************************************************
bool CAutoLocatorPostProc::HandleAutoLocatorInvocation(
    CPqmProtocol* f_protocol,
    const bool f_Autolocator_after_acquisition
)
{

    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::HandleAutoLocatorInvocation");
    PQM_SCOPED_TRACE();

    //Before sending message to GUI for invoking AutoLocator
    //Check all the conditions
    if (!CheckAutoLocatorConditions(f_protocol)) {

        return true;
    }

    m_cs.Lock();

    AUTOLOCATOR_TYPE l_al_type = f_protocol->GetAutoLocatorType();

    if (f_protocol->CanInvokeAutoLocator()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanInvokeAutoLocator is True"));

        //You are here it means Reconstruction is over.
        if (!m_auto_locator_running) {
            if (AUTOLOCATOR_CARDIAC == l_al_type) {
                m_pqm_ptr->WriteToUI(PQM_MSG_INVOKE_CALA_APPLICATION);

            } else if (AUTOLOCATOR_NEURO == l_al_type) {
                m_pqm_ptr->WriteToUI(PQM_MSG_INVOKE_HALA_APPLICATION);

            } else {

                m_cs.Unlock();
                return true ;
            }

        } else {
            //Display message in error view
            CString l_error_string =  CPqmUtilities::GetMultiLingualString(_T("ID_CARDIAC_APPLICATION_RUNNING_MSG"));
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                l_error_string, MPlus::EH::Information, _T("PQM"), _T("PQM"));

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CALA-HALA application is already running"));
        }

        f_protocol->SetCanInvokeAutoLocator(FALSE);

        //if called after reconstruction set the wait event and
        //make the hour glass to normal
        if (!f_Autolocator_after_acquisition) {
            m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
            HANDLE l_cala_handle = NULL;

            if (NULL != (l_cala_handle = OpenEvent(EVENT_ALL_ACCESS, FALSE, PQM_CALA_WAIT_FOR_RECON_EVENT))) {
                SetEvent(l_cala_handle);
                CloseHandle(l_cala_handle);
            }
        }

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CanInvokeAutoLocator is False"));

        //You are here it means reconstruction is still going on.

        //If called after reconstruction no need to wait
        //as till acquisition is not over so next scan won't start
        if (f_Autolocator_after_acquisition) {
            //Wait for reconstruction as for Auto Locator Enabled Application
            //User cannot start other scan till it is finished.
            //Do not Unlock m_cs here, otherwise Recon will not come inside m_cs.Lock();
            WaitForReconstruction(f_protocol);
            return false;

        } else {
            f_protocol->SetCanInvokeAutoLocator(TRUE);
        }
    }

    m_cs.Unlock();

    return true;
}


//**************************************************************************
//Method Name   : CheckAutoLocatorConditions
//Author        : PATNI
//Purpose       : To check autolocator conditions
//**************************************************************************
bool CAutoLocatorPostProc::CheckAutoLocatorConditions(
    CPqmProtocol* f_protocol
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::CheckAutoLocatorConditions");

    AUTOLOCATOR_TYPE l_al_type = f_protocol->GetAutoLocatorType();

    //Conditions checked
    //1.AutoLocator License should be present
    //2.Invocation type should not be NONE
    //3.And it is Autolocator Enabled

    if ((m_pqm_ptr->GetAutoLocatorLicense(l_al_type)) && l_al_type != AUTOLOCATOR_NONE &&
        ((f_protocol->GetAutoLocatorInvokeMode(l_al_type) == INVOKE_MODE_FOREGROUND)
         || (f_protocol->GetAutoLocatorInvokeMode(l_al_type) == INVOKE_MODE_BACKGROUND))) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckAutoLocatorConditions = true"));

        return true;

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckAutoLocatorConditions = false"));
        return false;
    }
}


//**************************************************************************
//Method Name   : WaitForReconstruction
//Author        : PATNI
//Purpose       : To wait till reconstruction is over
//**************************************************************************
void CAutoLocatorPostProc::WaitForReconstruction(
    CPqmProtocol* f_pqmprotocol
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::WaitForReconstruction");

    f_pqmprotocol->SetCanInvokeAutoLocator(TRUE);

    //Show the hour glass and
    //set flag to indicate that after this Auto Locator application can
    //be invoked.
    m_pqm_ptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
    m_pqm_ptr->WriteToUI(PQM_MSG_STATUS_DONE);


    m_cs.Unlock();

    //We open the reconstruction event and
    //wait for it(this event is signalled in IRSEventHAndler
    //where reconstruction done scenario is handled).
    HANDLE l_cala_handle = CreateEvent(NULL, TRUE, TRUE, PQM_CALA_WAIT_FOR_RECON_EVENT);
    ResetEvent(l_cala_handle);

    if (WAIT_TIMEOUT == WaitForSingleObject(l_cala_handle, m_recon_timeout)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Time Out Occured for CALA-HALA invocation"));

        f_pqmprotocol->SetCanInvokeAutoLocator(FALSE);
        CString l_error_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_CARDIAC_LOCATOR_TIMEOUT"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            l_error_string, MPlus::EH::Information, _T("PQM"), _T("PQM"));
        m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    }
}
//**************************************************************************
//Method Name   : IsAutoLocatorRunning
//Author        : PATNI
//Purpose       : To check whether autolocator is running or not
//**************************************************************************
BOOL CAutoLocatorPostProc::IsAutoLocatorRunning() const
{
    return m_auto_locator_running ;
}
//**************************************************************************
//Method Name   : SetAutoLocatorRunning
//Author        : PATNI
//Purpose       : To set auto locator is running or not
//**************************************************************************
void CAutoLocatorPostProc::SetAutoLocatorRunning(const BOOL f_autolocator_running)
{
    m_auto_locator_running = f_autolocator_running ;
}
//**************************************************************************
//Method Name   : IsValidCALAProtocolForScan
//Author        : PATNI
//Purpose       : To check whether the protocol is a valid CALA protocol for scan
//**************************************************************************
BOOL CAutoLocatorPostProc::IsValidCALAProtocolForScan(
    CPqmProtocol* f_prot
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::IsValidCALAProtocolForScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    int                 n = 0;
    VfFieldSpec_t       l_reqflds[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int                 l_typeformatdepth = SVD_ST_PROTOCOL;
    int                 l_numdone = -1;
    int                 l_status = E_ERROR;
    bool_t				l_cala_enable_flag = false;
    int                 l_total_slices = -1;
    int					l_slice_orientation = -1;
    int					l_cala_invoke_mode = -1;

    VfPathElem_t    pelm[1] = {0};
    memset(pelm, 0, sizeof(VfPathElem_t));
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_prot->GetProtocol();
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 2);


    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES,
                 sizeof(int), &l_total_slices, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_SLICE_ORIENTATION, SVT_PLN_SLICE_ORIENTATION,
                 sizeof(int), &l_slice_orientation, 0);
    n++;

    l_status = m_pqm_ptr->GetStudy()->GetVfDBHandle()->GetFields(pelm, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR) {
        l_total_slices = FIELDPROFILEIMAGES;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to read SVN_PLN_TOTAL_SLICES from shim study"));
        return FALSE;
    }

    ClibmsVfAdapter libmsvfAdapter(m_pqm_ptr->GetStudy()->GetVfDBHandle());
    int32_t	  l_totSlabs = 0;

    l_status = libmsvfAdapter.msVfCountSlabs(f_prot->GetProtocol(), &l_totSlabs);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the count of the slab"));
        return FALSE;
    }

    CString log;
    log.Format(_T("Total slices: %d,  Slice Ori : %d   Slabs: %d"), l_total_slices, l_slice_orientation, l_totSlabs);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log);

    if ((l_total_slices < AL_PROTOCOL_SLICE_NUM_LIMIT) || (l_slice_orientation != VFC_ORI_TRANSAXIAL) || (l_totSlabs > CALA_PROTOCOL_SLAB_LIMIT)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Cardiac protocol"));
        return FALSE;

    }

    return TRUE;
}
//**************************************************************************
//Method Name   : IsValidAutoLocatorProtocolForScan
//Author        : PATNI
//Purpose       : To check whether the protocol is a valid Autolocator protocol for scan
//**************************************************************************
BOOL CAutoLocatorPostProc::IsValidAutoLocatorProtocolForScan(
    CPqmProtocol* f_prot,
    const AUTOLOCATOR_TYPE f_autolocator_type
) const
{
    //check  auto loactor type and call the appropriate function
    //condition may added on addition of different auto locator types
    if (AUTOLOCATOR_CARDIAC == f_autolocator_type) {
        return IsValidCALAProtocolForScan(f_prot);

    } else if (AUTOLOCATOR_NEURO == f_autolocator_type) {
        return IsValidHALAProtocolForScan(f_prot);

    } else {
        return FALSE;
    }
}
//**************************************************************************
//Method Name   : IsValidHALAProtocolForScan
//Author        : PATNI
//Purpose       : To check whether the protocol is a valid HALA protocol for scan
//**************************************************************************
BOOL CAutoLocatorPostProc::IsValidHALAProtocolForScan(
    CPqmProtocol* f_prot
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::IsValidHALAProtocolForScan::");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    int                 n = 0;
    VfFieldSpec_t       l_reqflds[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int                 l_typeformatdepth = SVD_ST_PROTOCOL;
    int                 l_numdone = -1;
    int                 l_status = E_ERROR;
    bool_t				l_cala_enable_flag = false;
    int                 l_total_slices = -1;
    int					l_slice_orientation = -1;
    int					l_cala_invoke_mode = -1;

    VfPathElem_t    pelm[1] = {0};
    memset(pelm, 0, sizeof(VfPathElem_t));          //Patni-MJC/2009Aug17/Modified/cpp test corrections
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_prot->GetProtocol();
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t));


    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES,
                 sizeof(int), &l_total_slices, 0);
    n++;


    l_status = m_pqm_ptr->GetStudy()->GetVfDBHandle()->GetFields(pelm, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR) {
        l_total_slices = FIELDPROFILEIMAGES;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to read SVN_PLN_TOTAL_SLICES from shim study"));
        return FALSE;
    }

    ClibmsVfAdapter libmsvfAdapter(m_pqm_ptr->GetStudy()->GetVfDBHandle());
    int32_t	  l_totSlabs = 0;

    l_status = libmsvfAdapter.msVfCountSlabs(f_prot->GetProtocol(), &l_totSlabs);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the count of the slab"));
        return FALSE;
    }

    CString log;
    log.Format(_T("Total slices: %d,  Slice Ori : %d   Slabs: %d"), l_total_slices, l_slice_orientation, l_totSlabs);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log);

    if ((l_total_slices < AL_PROTOCOL_SLICE_NUM_LIMIT) || (l_totSlabs != HALA_PROTOCOL_SLAB_LIMIT)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Neuro protocol"));
        return FALSE;

    }

    return TRUE;
}
//**************************************************************************
//Method Name   : IsValidSequenceToInvokeCALAAppln
//Author        : PATNI
//Purpose       : To check whether the protocol is valid to invoke CALA application
//**************************************************************************
BOOL CAutoLocatorPostProc::IsValidSequenceToInvokeCALAAppln(
    CPqmProtocol* f_protocol
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::IsValidSequenceToInvokeCALAAppln");

    if (!f_protocol) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_protocol object is NULL"));
        return FALSE;
    }

    if (!IsValidCALAProtocolForScan(f_protocol)) {
        return FALSE;
    }

    if (f_protocol->GetHasImage()) {
        if ((f_protocol->IsProtocolCALAEnable() == VFC_CALA_ENABLE_FLAG_ON) &&
            (f_protocol->GetAcqStatus() == C_PR_STATUS_DONE)) {
            return TRUE;
        }
    }

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Invalid sequence to invoke CALA Application"));
    return FALSE;
}
//**************************************************************************
//Method Name   : IsValidSequenceToInvokeAutoLocator
//Author        : PATNI
//Purpose       : To check whether the protocol is valid to invoke Auto Locator application
//**************************************************************************
BOOL CAutoLocatorPostProc::IsValidSequenceToInvokeAutoLocator(
    const int f_acq_order,
    const AUTOLOCATOR_TYPE f_autolocator_type
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::IsValidSequenceToInvokeAutoLocator");

    CPqmProtocol* protocol = m_pqm_ptr->GetStudy()->GetProtocol(f_acq_order);

    if (!protocol) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("protcol object is NULL"));
        return FALSE;
    }

    if (m_auto_locator_running) {
        return FALSE;
    }

    //depending on Auto Locator type check the condition
    AUTOLOCATOR_TYPE l_al_type = protocol->GetAutoLocatorType();

    if (f_autolocator_type != l_al_type) {
        return FALSE;
    }

    if (AUTOLOCATOR_CARDIAC == l_al_type) {
        return IsValidSequenceToInvokeCALAAppln(protocol);

    } else if (AUTOLOCATOR_NEURO == l_al_type) {
        return IsValidSequenceToInvokeHALAAppln(protocol);
    }

    return FALSE;
}
//**************************************************************************
//Method Name   : IsValidSequenceToInvokeHALAAppln
//Author        : PATNI
//Purpose       : To check whether the protocol is valid to invoke HALA application
//**************************************************************************
BOOL CAutoLocatorPostProc::IsValidSequenceToInvokeHALAAppln(
    CPqmProtocol* f_protocol
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoLocatorPostProc::IsValidSequenceToInvokeHALAAppln");

    if (!f_protocol) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_protocol object is NULL"));
        return FALSE;
    }

    if (!IsValidHALAProtocolForScan(f_protocol)) {
        return FALSE;
    }

    if (f_protocol->GetHasImage()) {
        if ((f_protocol->GetAutoLocatorType() == AUTOLOCATOR_NEURO) &&
            (f_protocol->GetAcqStatus() == C_PR_STATUS_DONE)) {
            return TRUE;
        }
    }

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Invalid sequence to invoke HALA Application"));

    return FALSE;
}


//**************************************************************************
//Method Name   : CheckALProtocolScanConditions
//Author        : PATNI
//Purpose       : To check whether the protocol is valid to invoke Auto Locator application
//**************************************************************************
BOOL CAutoLocatorPostProc::CheckALProtocolScanConditions(
    CPqmProtocol* f_protocol
) const
{
    AUTOLOCATOR_TYPE l_altype = f_protocol->GetAutoLocatorType();

    //Since the protocol is not enabled for any Auto Locator
    //don't proceed
    if (AUTOLOCATOR_NONE == l_altype) {
        return TRUE;
    }

    if (((f_protocol->GetAutoLocatorInvokeMode(l_altype) != INVOKE_MODE_NONE))
        && !IsValidAutoLocatorProtocolForScan(f_protocol, l_altype)) {
        return FALSE;
    }

    return TRUE;
}
