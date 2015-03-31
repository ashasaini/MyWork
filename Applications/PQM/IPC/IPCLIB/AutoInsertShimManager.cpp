//Redmine-783
//Redmine-779
//Redmine-780
// AutoShimManager.cpp: implementation of the CAutoInsertShimManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertShimManager.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "AutoInsertShimOperation.h"
#include "Pqm.h"
#include "PqmProtocol.h"

using namespace MPlus::EH;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertShimManager
//Author        : PATNI/AM/NFD001
//Purpose       : Default Constructor
//**************************************************************************
CAutoInsertShimManager::CAutoInsertShimManager(
    CPqm* f_pqm,
    const bool f_licence_available /*= true*/
): CAutoInsertProtocolManager(f_pqm, f_licence_available)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimManager::CAutoInsertShimManager");

    try {
        m_ptr_auto_operation = new CAutoInsertShimOperation(f_pqm, m_ptr_auto_insert_study);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured..."));
        throw;
    }
}

//**************************************************************************
//Method Name   : CAutoInsertShimManager
//Author        : PATNI/AM/NFD001
//Purpose       : Destructor
//**************************************************************************
CAutoInsertShimManager::~CAutoInsertShimManager()
{
}

//**************************************************************************
//Method Name   : UpdateWaterFatSection
//Author        : PATNI/AKR-NFD001-CFA
//Purpose       :
//**************************************************************************
void CAutoInsertShimManager::UpdateWaterFatSection(
    const int f_shimState,
    const int f_protocol
)const
{
    m_ptr_auto_operation->UpdateWaterFatSection(f_shimState, f_protocol);
}


//**************************************************************************
//Method Name   : AutoInsertProtocol
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsert_status CAutoInsertShimManager::AutoInsertProtocol(
    CPqmProtocol* const f_protocol,
    const AutoInsertData& f_autoinsert_data
)const
{
    return AutoShimming(f_protocol, f_autoinsert_data);
}



AutoInsert_status CAutoInsertShimManager::CheckOnceInsertionPerScanStartCase(
    CPqmProtocol* f_target_prot
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertShimManager::InsertAutoShimAfter");

    if (NULL == f_target_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Target Protocol..."));
        return AUTO_INSERT_FAIL;
    }

    CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
        CPqmUtilities::GetMultiLingualString(_T("ERR_SHIM_NOT_FOUND")),
        _T("Sequence Queue"),
        SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

    return AUTO_INSERT_FAIL;
}


CPqmProtocol* CAutoInsertShimManager::GetPrevAutoInsertProtocolInfo(
    CPqmProtocol* const f_target_prot,
    int& f_prev_counter
)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimManager::InsertAutoShimAfter");

    if (NULL == f_target_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Target Protocol..."));
        return CAutoInsertProtocolManager::GetPrevAutoInsertProtocolInfo(f_target_prot, f_prev_counter);
    }

    f_prev_counter = f_target_prot->GetShimInsertionCounter();
    return f_target_prot->GetAutoInsertedShimProtocol();
}


//**************************************************************************
//Method Name   : AutoShimming
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsert_status CAutoInsertShimManager::AutoShimming(
    CPqmProtocol* const f_protocol,
    const AutoInsertData& f_autoinsert_data
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertShimManager::AutoShimming");
    PQM_SCOPED_TRACE();

    if (NULL == f_protocol || NULL == m_pqm_ptr) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol, m_pqm_ptr is NULL..!!"));
        AbortTargetScantoWait(f_protocol);
        return AUTO_INSERT_FAIL;
    }

    if (!GetAutoFlag()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoShim flag is TRUE."));
        return AUTO_INSERT_NOT_REQUIRED;
    }

    if (!IsAutoInsertProtocolRequired(f_protocol)) {
        return AUTO_INSERT_NOT_REQUIRED;
    }

    AutoInsert_status l_status = AUTO_INSERT_SUCCESS;

    switch (f_protocol->GetRequireShimming()) {

        case VFC_SHIM_REQUIRED_TO_BEFORE:
        case VFC_SHIM_REQUIRED_TO_AFTER:
            l_status = InsertAutoInsertProt(f_protocol, f_autoinsert_data, SHIM_MODE);
            break;

        default :
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetRequireShimming() is not None->should not have reached here->Continue Scan..."));
            //continue scan...
            l_status = AUTO_INSERT_NOT_REQUIRED;
    }

    return l_status;
}



//**************************************************************************
//Method Name   : IsAutoInsertProtocolRequired
//Author        : PATNI/AM/NFD001
//Purpose       : Gets the auto shim flag
//**************************************************************************
bool CAutoInsertShimManager::IsAutoInsertProtocolRequired(
    CPqmProtocol* const f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimManager::IsAutoInsertProtocolRequired");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Target Protocol is NULL"));
        return false;
    }

    if (f_protocol->GetShimming()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shimming flag is true"));
        return false;
    }

    switch (f_protocol->GetRequireShimming()) {

        case VFC_SHIM_REQUIRED_TO_BEFORE:
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RequireAutoShimming Flag: VFC_SHIM_REQUIRED_TO_BEFORE"));
            break;

        case VFC_SHIM_REQUIRED_TO_AFTER:
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RequireAutoShimming Flag: VFC_SHIM_REQUIRED_TO_AFTER"));
            break;

        default: {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RequireAutoShimming Flag: NONE"));
            return false;
        }
    }

    return true;
}