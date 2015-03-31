//Redmine-783
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
// AutoOperation.cpp: implementation of the CAutoInsertProtocolOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProtocolOperation.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include "Pqm.h"
#include "PASCOMInterface.h"
#include "AutoInsertProtocolStudy.h"
#include "ScanProcedure.h"
#include "PqmPmAcqman.h"


//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertProtocolOperation
//Author        : PATNI
//Purpose       : Destructor of CAutoInsertProtocolOperation
//**************************************************************************
CAutoInsertProtocolOperation::~CAutoInsertProtocolOperation()
{

}


void CAutoInsertProtocolOperation::InsertAutProtocolOnUI(const scan_mode f_mode)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolOperation::InsertProtocolOnUI");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_pqm_ptr) {
        m_pqm_ptr->WriteToUI(f_mode == MAP_MODE ? PQM_MSG_INSERT_MAP_PROTOCOL : PQM_MSG_INSERT_SHIM_PROTOCOL);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
    }
}

void CAutoInsertProtocolOperation::ResetCurrentScan(const int f_target_position)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolOperation::ResetCurrentScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if ((NULL == m_scan_procedure) || (NULL == m_pqm_ptr)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure or m_pqm_ptr pointer is NULL."));
        return;
    }

    CPqmProtocol* l_target_prot = m_pqm_ptr->GetStudy()->GetItemFromPosition(f_target_position);

    if (l_target_prot->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE) {
        l_target_prot->GetCDSInfo()->m_cds_step	  = CDS_STEP0_NONE;
        l_target_prot->GetCDSInfo()->m_cds_rescan   = TRUE;
    }


    m_scan_procedure->ResetCurrentRequest();
    m_pqm_ptr->GetStudy()->SetAcqStatus(l_target_prot, C_PR_STATUS_WAIT);
    m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
}

void CAutoInsertProtocolOperation::UpdateWaterFatSection(
    const int f_State,
    const int f_index
)const
{
    UNREFERENCED_PARAMETER(f_State);
    UNREFERENCED_PARAMETER(f_index);
}


//**************************************************************************
//Method Name   : AddAutoProtocol
//Author        : PATNI
//Purpose       : Add Auto Insert Protocol
//**************************************************************************
CPqmProtocol* CAutoInsertProtocolOperation::AddAutoProtocol(const int f_target_pos, SequenceList& f_sequence_list, const scan_mode f_mode, CString& f_error_id)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolOperation::AddMapProtocol");
    PQM_SCOPED_TRACE();

    CString l_trace_msg = _T("");
    l_trace_msg.Format(_T("Parameter of CAutoMapOperation::AddMapProtocol f_target_pos = %d"), f_target_pos);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);


    CPqmProtocol* l_targ_protocol =  m_pqm_ptr->GetStudy()->GetItemFromPosition(f_target_pos);
    CPqmProtocol* l_auto_prot = NULL;

    if (m_ptr_auto_insert_study) {
        //Patni/2011Feb11/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
        //Calculate ShimMapData to use...
        m_ptr_auto_insert_study->ScanInit(l_targ_protocol, f_mode);

        l_auto_prot = m_ptr_auto_insert_study->AddAutoProtocol(f_target_pos, f_mode, f_error_id);

        if (NULL == l_auto_prot) {
            return NULL;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_ptr_auto_map_shim_study pointer is NULL"));
        return NULL;
    }

    if (!m_ptr_auto_insert_study->CopyScanModeFrom(l_targ_protocol->GetPosition(), l_auto_prot->GetPosition(), f_mode)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("WARNING : CAutoMapShimStudy::CopyScanModeFrom() returns False"));

        return NULL;

    }

    //Set parent infromation from target to .
    m_ptr_auto_insert_study->SetParentInfoToAutoIsertedProtocol(l_targ_protocol, l_auto_prot);

    //Set Contrast agent for Map protocl
    CPASCOMInterface* l_passcominterface = m_pqm_ptr->GetPascomInterface();

    if (NULL == l_passcominterface) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_passcominterface pointer is NULL"));

        return NULL;
    }

    //+Patni/2011Feb11/Added+Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
    //just get the path using calculated data
    CString l_str_seq_path = m_ptr_auto_insert_study->GetAutoInsertFilePath(f_mode);
    //-Patni/2011Feb11/Added+Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

    long l_acqorder = 0;
    CPASCOMSequence* l_seq_temp = l_passcominterface->GetSingleSequence(l_str_seq_path, l_acqorder);

    if (l_seq_temp == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq_temp pointer is NULL."));
        return NULL;
    }

    f_sequence_list.RemoveAll();
    f_sequence_list.AddTail(l_seq_temp);

    if (NUM_2 == ((f_mode == MAP_MODE) ? l_targ_protocol->GetMapRequired() : l_targ_protocol->GetRequireShimming())) {
        m_ptr_auto_insert_study_manager.MakeAutoSeries(&f_sequence_list, l_auto_prot->GetPosition()) ;

    } else {
        m_ptr_auto_insert_study_manager.MakeAutoSeries(&f_sequence_list, f_target_pos);
    }

    return l_auto_prot;
}


//**************************************************************************
//Method Name   : CAutoInsertProtocolOperation
//Author        : PATNI
//Purpose       : Paramterized Constructor of CAutoInsertProtocolOperation
//**************************************************************************
CAutoInsertProtocolOperation::CAutoInsertProtocolOperation(
    CPqm* f_ptr_pqm,
    CAutoInsertProtocolStudy* const f_ptr_auto_insert_study
) : m_pqm_ptr(f_ptr_pqm),
    m_ptr_auto_insert_study(f_ptr_auto_insert_study),
    m_ptr_auto_insert_study_manager(f_ptr_pqm->GetStudyManagerPtr()),
    m_scan_procedure(f_ptr_pqm->GetAcqManager()->GetScanProcedure())
{

}