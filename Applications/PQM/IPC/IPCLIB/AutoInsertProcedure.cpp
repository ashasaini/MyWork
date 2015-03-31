// CAutoInsertProcedure.cpp: implementation of the CAutoInsertProcedure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProcedure.h"
#include <libStudyManager/Mutex/ScopedLock.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include "AutoInsertProtocolManager.h"
#include "AutoInsertMapManager.h"
#include "AutoInsertShimManager.h"
#include "PqmPmAcqman.h"
#include "ScanProcedure.h"
#include "PqmProtocol.h"
#include "Pqm.h"

using namespace MR::ACQ::SM::COMMON::MUTEX;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoInsertProcedure::CAutoInsertProcedure(
) : m_ptr_automap_mgr(NULL),
    m_ptr_autoshim_mgr(NULL),
    m_target_prot(NULL)
{
}

CAutoInsertProcedure::~CAutoInsertProcedure()
{
    DEL_PTR(m_ptr_automap_mgr);
    DEL_PTR(m_ptr_autoshim_mgr);
}


void CAutoInsertProcedure::InitAutoInsertProc(
    CPqm* f_pqm,
    const bool f_auto_insert_lic /*= true*/
)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProcedure::InitAutoInsertProc");
    PQM_SCOPED_TRACE();

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_auto_insert_lic ? _T("AutoInsert License Available") : _T("AutoInsert License NOT Available"));

    if (f_pqm) {
        m_pqm = f_pqm;
        m_ptr_automap_mgr =  new CAutoInsertMapManager(m_pqm, f_auto_insert_lic);
        m_ptr_autoshim_mgr = new CAutoInsertShimManager(m_pqm, f_auto_insert_lic);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pqm is NULL"));
    }
}


int CAutoInsertProcedure::AutoMapAutoShimming(
    CPqmProtocol* const f_protocol,
    const AutoInsertData& f_autoinsert_data
)
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProcedure::AutoMapAutoShimming");
    PQM_SCOPED_TRACE();

    if ((NULL == f_protocol) || (NULL == m_pqm)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_protocol, m_pqm or f_data NULL..!! Continue Target Scan"));
        return -1;
    }

    if (NULL == m_ptr_automap_mgr || NULL == m_ptr_autoshim_mgr) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptr_automap_mgr Or m_ptr_autoshim_mgr is NULL..!! Continue Target Scan"));
        return -1;
    }

    if ((!f_protocol->GetOriginalParameters()->GetEditingFlag())) {
        if ((m_target_prot == f_protocol)
            &&
            ((VFC_SHIM_REQUIRED_TO_AFTER == f_protocol->GetRequireShimming())
             || (VFC_MAP_REQUIRED_TO_AFTER == f_protocol->GetMapRequired()))
            && (!(f_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN))
           ) {

            //we have executed AFTER case.
            //Target (auto Scan) (scanning)
            //Auto Insert (auto Scan)
            //After auto insert AFTER, Target comes here again. so ignore this time.

            if ((!f_protocol->GetCDSInfo()->m_cds_flag) ||
                ((f_protocol->GetCDSInfo()->m_cds_flag) && (CDS_STEP6_DONE == f_protocol->GetCDSInfo()->m_cds_step))) {
                m_target_prot = NULL;
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_target_prot and given target f_protocol are same, so it is AFTER case.Continue Normal scan."));
            return -1;
        }

    }

    m_target_prot = NULL;

    CScopedLock l_scoped_lock(m_pqm->GetStudy()->GetSeqTransferMutex());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SeqTransferMutex Acquried..."));

    bool l_can_display_relimit_msg = true;
    AutoInsert_status l_map_status = m_ptr_automap_mgr->AutoInsertProtocol(f_protocol, f_autoinsert_data, l_can_display_relimit_msg);

    if ((AUTO_INSERT_ERROR == l_map_status) || (AUTO_INSERT_FAIL == l_map_status)) {
        m_pqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        m_pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
        return 0;
    }

    l_can_display_relimit_msg = !m_ptr_automap_mgr->GetHasDisplayedRelimitMessage();
    AutoInsert_status l_shimming_status = m_ptr_autoshim_mgr->AutoInsertProtocol(f_protocol, f_autoinsert_data, l_can_display_relimit_msg);

    if ((AUTO_INSERT_ERROR == l_shimming_status) || (AUTO_INSERT_FAIL == l_shimming_status)) {
        m_pqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
        m_pqm->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
        return 0;
    }

    if ((VFC_SHIM_REQUIRED_NONE != f_protocol->GetRequireShimming()) || (VFC_MAP_REQUIRED_NONE != f_protocol->GetMapRequired())) {

        if (((AUTO_INSERT_SUCCESS == l_map_status) && (AUTO_INSERT_NOT_REQUIRED == l_shimming_status)) ||
            ((AUTO_INSERT_SUCCESS == l_shimming_status) && (AUTO_INSERT_NOT_REQUIRED == l_map_status)) ||
            ((AUTO_INSERT_SUCCESS == l_shimming_status) && (AUTO_INSERT_SUCCESS == l_map_status))) {

            m_target_prot = f_protocol;

            if ((VFC_SHIM_REQUIRED_TO_AFTER == f_protocol->GetRequireShimming()) ||
                (VFC_MAP_REQUIRED_TO_AFTER == f_protocol->GetMapRequired())) {
                f_protocol->SetAtuomapAutoShimAdded(true);
            }


            m_pqm->WriteToUI(PQM_MSG_START_SCAN_FIRST_WAIT);
            m_pqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);

            //save the target pointer to check Target when this function is again called for Target
            if (!((VFC_SHIM_REQUIRED_TO_AFTER == f_protocol->GetRequireShimming())
                  || (VFC_MAP_REQUIRED_TO_AFTER == f_protocol->GetMapRequired()))
               ) {

                m_target_prot = NULL;
            }

            return 0;
        }
    }

    return -1;
}


BOOL CAutoInsertProcedure::GetAutoInsertedSeqDataToDisplay(
    VARIANT** f_autinsert_data,
    int* f_insertedmappos,
    int* f_target_pos,
    int* const f_is_shim_required,
    int* const f_is_map_required
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProcedure::GetAutoInsertedSeqDataToDisplay");
    PQM_SCOPED_TRACE();

    try {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

        if (NULL == m_pqm) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
            return FALSE;
        }

        if (!f_autinsert_data || !(*f_autinsert_data)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
            return FALSE;
        }

        SequenceList* l_autinsert_data = (SequenceList*)(*f_autinsert_data)->byref;
        const auto_insert l_mode =	(auto_insert)(* f_autinsert_data)->wReserved1 ;
        //Patni/SS/2010July20/Added/NFD001-AutoShim

        CAutoInsertProtocolManager* l_auto_insert_obj = NULL;

        switch (l_mode) {
            case AUTO_MAP:
                l_auto_insert_obj = m_ptr_automap_mgr;
                break;

            case AUTO_SHIM:
                l_auto_insert_obj = m_ptr_autoshim_mgr;
                break;

            default :
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown input for function"));
                return FALSE;
        }


        if (!l_auto_insert_obj) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Auto Insert Protocol Not Found"));
            return FALSE;
        }

        return l_auto_insert_obj->GetAutoInsertDataForGUI(l_autinsert_data, f_insertedmappos, f_target_pos, f_is_shim_required, f_is_map_required);


    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured..."));
        throw;
    }

    return NULL;
}

void CAutoInsertProcedure::UpdateWaterFatSection(
    const int f_shimState,
    const int f_protocol
)const
{
    if (m_ptr_autoshim_mgr)
        m_ptr_autoshim_mgr->UpdateWaterFatSection(f_shimState, f_protocol);
}

void CAutoInsertProcedure::ProtocolEditingCompleted(
    CPqmProtocol* const f_protocol
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProcedure::ProtocolEditingCompleted");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if ((f_protocol->GetMapInsertionCounter() > 0) || (f_protocol->GetShimInsertionCounter())) {
        f_protocol->GetOriginalParameters()->SetEditingFlag(true);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Target Protocol Edited..."));

    } else {

        switch (f_protocol->GetProtocolKind()) {

            case ProtocolKind_AutoShim:
            case ProtocolKind_AutoMap:
                f_protocol->SetProtocolKind(ProtocolKind_Normal);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoInserted Protocol Edited...Making them normal"));
                break;
        }
    }
}


bool CAutoInsertProcedure::CanDisplayCFAGraph(
    CPqmProtocol* const f_protocol
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProcedure::CanDisplayCFAGraph");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Operation Performed..."));
        return true;
    }

    if (PQM_CFA_RETRY == m_pqm->GetAcqManager()->GetScanProcedure()->GetCFARetry()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("User Performed CFA Retry for CFA Dialog..."));
        return true;
    }

    if ((ProtocolKind_AutoShim == f_protocol->GetProtocolKind()) &&
        ((!(f_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) || (VFC_AMB_MODE_NONE != f_protocol->GetAMBModeFlag()))
       ) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Do not display CFA for f_protocol == ProtocolKind_AutoShim"));
        return false;

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Display CFA for f_protocol..."));
        return true;
    }
}

void CAutoInsertProcedure::GetTargetProtInformation(
    int* f_target_pos
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProcedure::GetTargetProtInformation");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_target_prot) {
        (*f_target_pos) = m_target_prot->GetPosition() - 1;

    } else {
        (*f_target_pos) = -1;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invlide Operation..."));
    }
}


