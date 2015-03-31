//REDMINE-1213_Update_03_MAY
//REDMINE-1213
//Redmine-783
//Redmine-777
//Redmine-780
//Redmine-782
// AutoManager.cpp: implementation of the CAutoInsertProtocolManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProtocolManager.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "Pqm.h"
#include "PqmProtocol.h"
#include "AutoInsertProtocolStudy.h"
#include "AutoInsertProtocolOperation.h"
#include "PqmCoilData.h"
#include "scanprocedure.h"
#include "PqmPmAcqman.h"


//**************************************************************************
//Method Name   : CAutoInsertProtocolManager
//Author        : PATNI
//Purpose       : Destructor of CinsertProtocolManager
//**************************************************************************
CAutoInsertProtocolManager::~CAutoInsertProtocolManager()
{

    DEL_PTR(m_ptr_auto_operation);
    DEL_PTR(m_ptr_auto_insert_study);
}


AutoInsert_status CAutoInsertProtocolManager::AutoInsertProtocol(
    CPqmProtocol* const f_target_prot,
    const AutoInsertData& f_autoinsert_data,
    const bool f_can_display_relimit_message
) const
{

    m_can_display_relimit_message = f_can_display_relimit_message ;
    m_has_displayed_relimit_message = false;

    return AutoInsertProtocol(f_target_prot, f_autoinsert_data);
}


void CAutoInsertProtocolManager::UpdateWaterFatSection(
    const int f_shimState,
    const int f_target_prot
)const
{
}

//**************************************************************************
//Method Name   : StartAutoInsertProtocol
//Author        : PATNI
//Purpose       :
//**************************************************************************
bool CAutoInsertProtocolManager::StartAutoInsertProtocol(const int f_target_protocol_number, const scan_mode f_mode)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::StartAutoInsertProtocol");

    CString l_trace_msg;
    l_trace_msg.Format(_T("Parameters: f_target_protocol_number = %d"), f_target_protocol_number);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_msg);

    if (NULL == m_ptr_auto_operation) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_ptr_auto_operation pointer is NULL."));
        return false;
    }

    // Reset Current scan request for target protocol
    m_pqm_ptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);

    const bool l_is_scanning = m_pqm_ptr->GetAcqManager()->GetScanProcedure()->GetIsScanning();
    m_ptr_auto_operation->ResetCurrentScan(f_target_protocol_number);

    if (!l_is_scanning) {
        m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    }

    CPqmProtocol* l_target_protocol = m_pqm_ptr->GetStudy()->GetItemFromPosition(f_target_protocol_number);

    if (NULL == l_target_protocol) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_target_protocol points to NULL"));
        return false;
    }

    l_target_protocol->GetOriginalParameters()->SetEditingFlag(false);
    l_target_protocol->SetOriginalParameters();

    //insert Auto protocol
    CString l_error_id = _T("");
    CPqmProtocol* l_auto_protocol = m_ptr_auto_operation->AddAutoProtocol(f_target_protocol_number, const_cast<SequenceList&>(m_sequence_list), f_mode, l_error_id);

    bool l_return_val = true;

    if (NULL == l_auto_protocol || NULL == l_auto_protocol->GetPqmPrMode() || 0 == m_sequence_list.GetCount()) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_auto_protocol points to NULL"));
        l_return_val = false;
    }

    if (l_return_val) {

        m_auto_func.SetAutoProtocol(l_auto_protocol);

        //Auto Insertion of protocol is done... so making the flag ON
        l_auto_protocol->SetTargetProtocol(l_target_protocol);

        if (MAP_MODE == f_mode) {
            l_target_protocol->SetAutoInsertedMapProtocol(l_auto_protocol);
            l_target_protocol->IncrementMapInsertionCounter(NUM_1);
            l_auto_protocol->SetProtocolKind(ProtocolKind_AutoMap);

        } else if (SHIM_MODE == f_mode) {
            l_target_protocol->SetAutoInsertedShimProtocol(l_auto_protocol);
            l_target_protocol->IncrementShimInsertionCounter(NUM_1);
            l_auto_protocol->SetProtocolKind(ProtocolKind_AutoShim);
        }


        //+Patni/2011Feb11/Modified+Added/V2.0/IR-181_NFD1_Prod_Req_Part1
        if (E_ERROR == m_ptr_auto_insert_study->ConfigureAutoInsertedProtocol(
                m_pqm_ptr->GetStudy()->GetVfDBHandle(),
                l_target_protocol->GetProtocol(), l_auto_protocol->GetProtocol(), f_mode, m_can_display_relimit_message, m_has_displayed_relimit_message)
           ) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ConfigureAutoInsertedProtocol returns E_ERROR"));
            l_return_val = false;
        }

        m_pqm_ptr->GetStudy()->GetProtocolItemValues(m_pqm_ptr->GetStudy()->GetVfDBHandle(), l_auto_protocol);
        m_ptr_auto_operation->InsertAutProtocolOnUI(f_mode); //Reshufling the location of call to get the target protocol.
    }

    if (!l_return_val) {

        l_error_id = l_error_id.IsEmpty() ? ((MAP_MODE == f_mode) ? _T("IDS_FAIL_ADD_MAP_PROTOCOL") : _T("IDS_FAIL_ADD_SHIM_PROTOCOL")) : l_error_id;
        CString temp_string = CPqmUtilities::GetMultiLingualString(l_error_id);

        if (temp_string.IsEmpty()) {
            temp_string = _T("Error occurred while adding ") ;
            temp_string = temp_string + ((MAP_MODE == f_mode) ? _T("Auto Map protocol") : _T("Auto Shim protocol")) ;
        }

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(temp_string,
                MPlus::EH::Information, _T("CAutoInsertProtocolManager.cpp"), _T("PQM"));
    }

    return l_return_val;
}



BOOL CAutoInsertProtocolManager::GetAutoInsertDataForGUI(
    SequenceList* const f_autinsert_data,
    int* const f_insertedmappos,
    int* const f_target_pos,
    int* const f_is_shim_required,
    int* const f_is_map_required
)
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::GetAutoInsertDataForGUI");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    POSITION l_pos = m_sequence_list.GetHeadPosition();

    if (NULL == l_pos) {
        return FALSE;
    }

    f_autinsert_data->AddTail(m_sequence_list.GetNext(l_pos));

    CPqmProtocol* l_protocol = GetAutoProtocol();
    (*f_insertedmappos) = l_protocol->GetPosition() - 1;
    (*f_target_pos) = l_protocol->GetTargetProtocol()->GetPosition() - 1;

    (*f_is_shim_required) = l_protocol->GetTargetProtocol()->GetRequireShimming();
    (*f_is_map_required) = l_protocol->GetTargetProtocol()->GetMapRequired();

    return TRUE;

}

bool CAutoInsertProtocolManager::GetHasDisplayedRelimitMessage(
) const
{

    return m_has_displayed_relimit_message;
}

//**************************************************************************
//Method Name   : CAutoInsertProtocolManager
//Author        : PATNI
//Purpose       : Parameterized Constructor of CAutoInsertProtocolManager
//**************************************************************************
CAutoInsertProtocolManager::CAutoInsertProtocolManager(
    CPqm* f_ptr_pqm,
    const bool f_licence_available /*= true*/
):  m_pqm_ptr(f_ptr_pqm),
    m_ptr_auto_operation(NULL),
    m_ptr_auto_insert_study(new CAutoInsertProtocolStudy(f_ptr_pqm->GetStudy())),
    m_auto_func(f_licence_available),
    m_sequence_list(),
    m_can_display_relimit_message(true),
    m_has_displayed_relimit_message(false)
{

}

CPqmProtocol* CAutoInsertProtocolManager::GetPrevAutoInsertProtocolInfo(
    CPqmProtocol* const f_target_prot,
    int& f_prev_counter
)const
{
    f_prev_counter = 0;
    return NULL ;
}


AutoInsert_status CAutoInsertProtocolManager::CheckForReuse(const int f_target_prot)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::CheckforReuse");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_ptr_auto_operation) {
        return AUTO_INSERT_FAIL;
    }

    return  m_ptr_auto_operation->CheckForReuse(f_target_prot);
}


//**************************************************************************
//Method Name   : InsertAutoInsertProt
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsert_status CAutoInsertProtocolManager::InsertAutoInsertProt(
    CPqmProtocol* const f_target_prot,
    const AutoInsertData& f_autoinsert_data,
    const scan_mode f_mode
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::InsertAutoInsertProt");
    PQM_SCOPED_TRACE();

    //take current value befor update...
    const float l_org_couch_pos = f_target_prot->GetPqmPrMode()->GetCouchPos();
    const int l_org_scan_number = f_target_prot->GetPqmPrMode()->GetScanNumber();

    f_target_prot->GetPqmPrMode()->SetCouchPos(f_autoinsert_data.m_couch_position);
    f_target_prot->GetPqmPrMode()->SetScanNumber(f_autoinsert_data.m_couchstatusflag);

    const AutoInsert_status l_can_reuse_data = CheckForReuse(f_target_prot->GetProtocol());

    //store current values again
    f_target_prot->GetPqmPrMode()->SetCouchPos(l_org_couch_pos);
    f_target_prot->GetPqmPrMode()->SetScanNumber(l_org_scan_number);

    if (l_can_reuse_data != AUTO_INSERT_SUCCESS) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckForReuse Not Success"));

        switch (l_can_reuse_data) {

            case AUTO_INSERT_FAIL :
            case AUTO_INSERT_ERROR :
                AbortTargetScantoWait(f_target_prot);
                break;
        }

        return l_can_reuse_data;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckForReuse Failed"));

    int l_prev_auto_insert_prot_counter = 0;
    CPqmProtocol* const l_prev_auto_insert_prot =  GetPrevAutoInsertProtocolInfo(f_target_prot, l_prev_auto_insert_prot_counter);

    AutoInsert_status l_status = AUTO_INSERT_NOT_REQUIRED;

    //Do the checking here for inserting protocol only once
    if ((NULL == l_prev_auto_insert_prot)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoInserted Shim Protocol is not present"));

        //By default insert only..
        l_status = AUTO_INSERT_SUCCESS;

        //also check weather already inserted ?
        if (l_prev_auto_insert_prot_counter > 0) {

            //If Target hs Already auto inserted and if it is Autoscan And Non-Edited
            //and user has not changed any data of Target then no need to insert again..
            if (!IsAutoInsertionRequired(f_target_prot)) {
                l_status = AUTO_INSERT_NOT_REQUIRED;
            }
        }

    } else {

        //Here Shim is already added before..but this Target protocol could not reuse it
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoInserted Shim Protocol is present"));

        //check weather we can insert new SHIM directly again...
        if (IsAutoInsertionRequired(f_target_prot)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IsAutoInsertionRequired() is TRUE"));
            l_status = AUTO_INSERT_SUCCESS;

        } else {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IsAutoInsertionRequired() is FALSE"));

            switch (l_prev_auto_insert_prot->GetAcqStatus()) {

                case C_PR_STATUS_DONE:
                    l_status = CheckOnceInsertionPerScanStartCase(f_target_prot);
                    break;

                case C_PR_STATUS_FAIL:

                    //Already inserted shim was failed so could not found while Target scanning..
                    //insert again..
                    l_status = AUTO_INSERT_SUCCESS;
                    break;

                case C_PR_STATUS_WAIT:

                    //ALready inserted shim is re arranged by user so it is not completed..
                    //so continue with normal scan and it will calculate CFA ...
                    l_status = AUTO_INSERT_NOT_REQUIRED;
                    break;
            }
        }
    }

    switch (l_status) {

        case AUTO_INSERT_SUCCESS:

            if (StartAutoInsertProtocol(f_target_prot->GetPosition(), f_mode)) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("StartAutoInsertProtocol() SUCCESS"));
                l_status = AUTO_INSERT_SUCCESS;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("StartAutoInsertProtocol() FAILED"));
                l_status = AUTO_INSERT_FAIL;
            }

            break;

        case AUTO_INSERT_FAIL:

            AbortTargetScantoWait(f_target_prot);
            break;
    }

    return l_status;
}



void CAutoInsertProtocolManager::AbortTargetScantoWait(
    CPqmProtocol* f_target_prot
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::AbortTargetScantoWait");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Aborting Target Protocol Scan..."));

    m_pqm_ptr->GetStudy()->SetScanModeOfHead();
    m_pqm_ptr->GetAcqManager()->GetScanProcedure()->ResetCurrentRequest();
    m_pqm_ptr->GetStudy()->SetAcqStatus(f_target_prot, C_PR_STATUS_WAIT);
    m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
}


//**************************************************************************
//Method Name   : IsAutoInsertionRequired
//Author        : PATNI
//Purpose       :
//**************************************************************************
bool CAutoInsertProtocolManager::IsAutoInsertionRequired(
    CPqmProtocol* const f_target_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::IsAutoInsertionRequired");

    if (NULL == f_target_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_target_protocol is NULL"));
        return false;
    }

    if (!(f_target_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {

        return (IsTargetProtEdited(f_target_protocol)) ||
               (f_target_protocol->GetOriginalParameters()->GetEditingFlag() ? true : false);
    }

    return true;
}



bool CAutoInsertProtocolManager::IsTargetProtEdited(
    CPqmProtocol* const f_target_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::IsTargetProtEdited");

    if (NULL == f_target_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_target_protocol is NULL"));
        return false;
    }

    CPqmPrMode* l_pr_mode = f_target_protocol->GetPqmPrMode();

    return (IsCoilChangedForTargetProt(f_target_protocol) ||
            ((l_pr_mode->GetAnatomy() - f_target_protocol->GetOriginalParameters()->GetAnatomy()) != NUM_0) ||
            (f_target_protocol->GetOffset().z - f_target_protocol->GetOriginalParameters()->GetScanOffset().z != NUM_0)) ? true : false ;

}


bool CAutoInsertProtocolManager::IsCoilChangedForTargetProt(
    CPqmProtocol* const f_target_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolManager::IsCoilChangedForTargetProt");

    if (NULL == f_target_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_target_protocol is NULL"));
        return false;
    }

    OriginalParameters* l_original_parms = f_target_protocol->GetOriginalParameters();
    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    return !(l_coil_data->PqmIsSameCoilInSaturn(&(l_original_parms->m_saturn_coil), f_target_protocol->GetSaturnCoil(),
             f_target_protocol->GetCoilDBTable(), f_target_protocol->GetCoilDBTable()) == TRUE) ;
}


//**************************************************************************
//Method Name   : GetAutoMapFlag
//Author        : PATNI/DKH
//Purpose       : Sets the auto map flag
//**************************************************************************
bool CAutoInsertProtocolManager::GetAutoFlag() const
{
    return m_auto_func.GetAutoFlag();
}

//**************************************************************************
//Method Name   : GetAutoProtocol
//Author        : PATNI/DKH
//Purpose       :
//**************************************************************************
inline CPqmProtocol* CAutoInsertProtocolManager::GetAutoProtocol() const
{
    return m_auto_func.GetAutoProtocol();
}

//**************************************************************************
//Method Name   : SetAutoProt
//Author        : PATNI/DKH
//Purpose       :
//**************************************************************************
inline void CAutoInsertProtocolManager::SetAutoProt(CPqmProtocol* const f_auto_protocol)
{
    m_auto_func.SetAutoProtocol(f_auto_protocol);
}