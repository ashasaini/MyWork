//Redmine-783
//Redmine-774
//Redmine-777
//Redmine-780
//Redmine-781
// AutoShimOperation.cpp: implementation of the CAutoInsertShimOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AutoInsertShimOperation.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PDataParser/INIReader.h>
#include <PQM/PQMLogMgr.h>
#include "Pqm.h"
#include "scanprocedure.h"
#include "AutoInsertProtocolStudy.h"
#include "pqmAMB.h"

//**************************************************************************
//Method Name   : CAutoInsertShimOperation
//Author        : PATNI/AM/NFD001
//Purpose       : This is the copy constructor function
//**************************************************************************
CAutoInsertShimOperation::CAutoInsertShimOperation(
    CPqm* f_pqm_ptr,
    CAutoInsertProtocolStudy* const f_ptr_auto_insert_study
): CAutoInsertProtocolOperation(f_pqm_ptr, f_ptr_auto_insert_study)
{
}


//**************************************************************************
//Method Name   : CAutoInsertShimOperation
//Author        : PATNI/AM/NFD001
//Purpose       : This is the destructor function
//**************************************************************************
CAutoInsertShimOperation::~CAutoInsertShimOperation()
{
}

//**************************************************************************
//Method Name   : UpdateWaterFatSection
//Author        : PATNI/AKR/NFD001-CFA
//Purpose       :
//**************************************************************************
void CAutoInsertShimOperation::UpdateWaterFatSection(const int f_shimState, const int f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimOperation::UpdateWaterFatSection");
    PQM_SCOPED_TRACE();

    if (SHIM_STATE_ACTIVE == f_shimState) {

        if (!m_pqm_ptr->CanDisplayCFAGraph(m_pqm_ptr->GetStudy()->GetProtocol(f_protocol))) {
            bool l_shimfsy_flag = (0 == GetShimFSYFlagValue()) ? VFC_SHIM_FSY_WATER : VFC_SHIM_FSY_FAT;
            CPqmShimSharedData::GetInstance()->SetShimFsyFlag(l_shimfsy_flag);

            if (l_shimfsy_flag == VFC_SHIM_FSY_FAT) {
                m_pqm_ptr->WriteToUI(PQM_MSG_CFA_SET_FSY_FAT);

            } else {
                m_pqm_ptr->WriteToUI(PQM_MSG_CFA_SET_FSY_WATER);
            }

        }
    }

    if (!m_pqm_ptr->GetAMB()->PqmAMBScanSeg2Active())
        m_pqm_ptr->GetStudy()->SetShimmingInfo(f_protocol, (SHIM_STATE_ACTIVE == f_shimState) ? TRUE : FALSE);
}

//**************************************************************************
//Method Name   : CheckForReuse
//Author        : PATNI/SS
//Purpose       : This function will check whether reuseable shim data is
//                available for current scan under the given conditions
//**************************************************************************
AutoInsert_status CAutoInsertShimOperation::CheckForReuse(const int f_protocol)const
{
    return SearchShimDataInDoneProtocol(f_protocol) ? AUTO_INSERT_NOT_REQUIRED : AUTO_INSERT_SUCCESS;
}

//**************************************************************************
//Method Name   : GetShimFSYFlagValue
//Author        : PATNI/AKR/NFD001-CFA
//Purpose       : Dummy getter of ShimFSY flag.
//**************************************************************************
int CAutoInsertShimOperation::GetShimFSYFlagValue() const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimOperation::GetShimFSYFlagValue");
    PQM_SCOPED_TRACE();

    LPCUSTR l_section = _T("FSY VALUE");

    try {

        //Exception May occur while reading INI file...
        CIniReader ini_reader(GetAutoShimFSYPath());

        int l_nkey_value = 0;

        if (ini_reader.SectionExists(l_section)) {
            CString l_value = ini_reader.GetKeyValue(_T("SHIM_FSY_VALUE"), l_section);
            l_nkey_value = _wtoi(l_value);
        }

        CString l_trace_msg = _T("");
        l_trace_msg.Format(_T("GetShimFSYFlagValue, l_nkey_value = %d"), l_nkey_value);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

        return l_nkey_value;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception found while reading ini file"));
        throw;
    }
}

//**************************************************************************
//Method Name   : GetAutoShimFSYPath
//Author        : PATNI/AKR/NFD001-CFA
//Purpose       : Path getter for FSY flag
//**************************************************************************
CString CAutoInsertShimOperation::GetAutoShimFSYPath() const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimOperation::GetAutoShimFSYPath");

    char* l_path = NULL;
    char f_auto_shim_FSY_path[MAX_PATH] = {0};

    if ((l_path = getenv("MRMP_DATA")) != NULL) {
        strcpy(f_auto_shim_FSY_path, l_path);

    } else {
        strcpy(f_auto_shim_FSY_path, "\\mrmplus\\data");
    }

    strcat(f_auto_shim_FSY_path, "\\pqm\\FSY_VALUE.ini");
    TCHAR tmp[MAX_PATH] = {0};
    mbstowcs(tmp, f_auto_shim_FSY_path, strlen(f_auto_shim_FSY_path));

    CString str(tmp);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoShimFSYPath :") + str);

    return str;
}

//**************************************************************************
//Method Name   : SearchShimDataInDoneProtocol
//Author        : PATNI/SS
//Purpose       : This function  will Search the reusable shim protocol
//**************************************************************************
bool CAutoInsertShimOperation::SearchShimDataInDoneProtocol(const int f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertShimOperation::SearchShimDataInDoneProtocol");
    PQM_SCOPED_TRACE();

    CString l_tracemsg = _T("");
    l_tracemsg.Format(_T("Value of f_protocol = %d"), f_protocol);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return false;
    }

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (NULL == l_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
        return false;
    }

    CPqmProtocol* l_objprotocol = m_pqm_ptr->GetStudy()->GetProtocol(f_protocol);

    if (NULL == l_objprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_objprotocol pointer is NULL"));
        return false;
    }

    CPqmProtocol* l_current_protocol = m_scan_procedure->GetScanProcProtocol();
    const float l_pos = m_ptr_auto_insert_study->CalcTargetPosition(l_objprotocol);

    m_scan_procedure->PQMAcqSearchShim(l_study->GetVfDBHandle(), l_current_protocol, AUTOSHIM_MODE, l_pos);

    return (-1 != l_current_protocol->GetShimProt());
}






