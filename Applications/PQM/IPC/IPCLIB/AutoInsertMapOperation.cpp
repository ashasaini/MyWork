//Redmine-777
//Redmine-780
//Redmine-781

// AutoMapOP.cpp: implementation of the CAutoInsertMapOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertMapOperation.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include "Pqm.h"
#include "PqmSpeeder.h"
#include "scanprocedure.h"
#include "AutoInsertProtocolStudy.h"
#include "PqmCoilData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertMapOperation
//Author        : PATNI/DKH
//Purpose       : This is the constructor
//**************************************************************************
CAutoInsertMapOperation::CAutoInsertMapOperation(
    CPqm* const f_pqm_ptr,
    CAutoInsertProtocolStudy* const f_ptr_auto_insert_study
): CAutoInsertProtocolOperation(f_pqm_ptr, f_ptr_auto_insert_study)
{
}
//**************************************************************************
//Method Name   : ~CAutoInsertMapOperation
//Author        : PATNI/DKH
//Purpose       : This is the destructor
//**************************************************************************
CAutoInsertMapOperation::~CAutoInsertMapOperation()
{

}

//**************************************************************************
//Method Name   : CheckForReuseMap
//Author        : PATNI/DKH
//Purpose       : This function will check whether reuseable map data is
//                available for current scan under the given conditions
//**************************************************************************
AutoInsert_status CAutoInsertMapOperation::CheckForReuse(const int f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertMapOperation::CheckForReuse");

    CString l_tracemsg = _T("");
    l_tracemsg.Format(_T("f_protocol = %d"), f_protocol);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    m_error_code = -1;
    int l_error_code  = -1;
    AutoInsert_status  status  = SearchMapDataInDoneProtocol(f_protocol, l_error_code);
    m_error_code = l_error_code;

    return status;
}


//**************************************************************************
//Method Name   : SearchMapDataInDoneProtocol
//Author        : PATNI/
//Purpose       : This function  will Search the resusable map protocol
//**************************************************************************
AutoInsert_status CAutoInsertMapOperation::SearchMapDataInDoneProtocol(
    const int f_protocol,
    int& f_error_code
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertMapOperation::SearchMapDataInDoneProtocol");
    PQM_SCOPED_TRACE();

    AutoInsert_status l_status = AUTO_INSERT_NOT_REQUIRED;

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return  AUTO_INSERT_ERROR;
    }

    CPqmProtocol* l_objprotocol = m_pqm_ptr->GetStudy()->GetProtocol(f_protocol);

    if (NULL == l_objprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_objprotocol pointer is NULL"));
        return AUTO_INSERT_ERROR;
    }

    CPqmSpeeder* l_pqmspeeder = m_pqm_ptr->GetSpeederPtr();

    if (NULL == l_pqmspeeder) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmspeeder pointer is NULL"));
        return AUTO_INSERT_ERROR;
    }

    //Getting back up
    const int l_map_type_bak = l_objprotocol->GetSpeederType();
    const int l_intensity_type_bak = l_objprotocol->GetIntensityType();
    const int l_protcolcorrstatus = l_objprotocol->GetProtcolCorrStatus();
    const int l_degenracy_flag = l_objprotocol->GetSaturnCoil()->DegeneracyFlg;

    bool l_both_speeder_intensity_off = false;

    if ((l_objprotocol->GetSpeederType() == VFC_SPEEDER_TYPE_NONE) &&
        (l_objprotocol->GetIntensityType() == FALSE)) {

        //Setting Flags true explicitly
        l_objprotocol->SetSpeederType(VFC_SPEEDER_TYPE_SPEEDER);

        l_objprotocol->SetIntensityType(true);
        l_objprotocol->SetProtcolCorrStatus(l_pqmspeeder->CanSupportB1Correction(l_objprotocol) ? VFC_INTENSITY_CORR_STATUS_TX_RX : VFC_INTENSITY_CORR_STATUS_RX);
        l_both_speeder_intensity_off = true;
        l_objprotocol->GetSaturnCoil()->DegeneracyFlg = FALSE;
    }

    if ((VFC_SPEEDER_TYPE_SPEEDER == l_objprotocol->GetSpeederType()) && (!l_pqmspeeder->IsValidSpeederCoil(l_objprotocol))) {

        if (!l_both_speeder_intensity_off) {
            //Restore the Org values..
            l_objprotocol->SetSpeederType(l_map_type_bak);
            l_objprotocol->SetIntensityType(l_intensity_type_bak);
            l_objprotocol->SetProtcolCorrStatus(l_protcolcorrstatus);
            l_objprotocol->GetSaturnCoil()->DegeneracyFlg = l_degenracy_flag;

            CString message = m_pqm_ptr->GetSpeederPtr()->GetErrorString(ERR_ILLEGAL_SPEEDER_COIL);

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(message),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));

            return AUTO_INSERT_ERROR;

        } else {
            l_objprotocol->SetSpeederType(l_map_type_bak);
        }
    }

    float l_pos = m_ptr_auto_insert_study->CalcTargetPosition(l_objprotocol);

    f_error_code = l_pqmspeeder->SpeederCheck(l_objprotocol, MAP_AUTO_MODE, l_pos);

    if (f_error_code >= 0) {
        if (ERR_SPEEDER_MAX_RATIO == f_error_code) {

            CString message = m_pqm_ptr->GetSpeederPtr()->GetErrorString(f_error_code);

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(message),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));

            l_status = AUTO_INSERT_ERROR;

        } else {
            l_status = AUTO_INSERT_SUCCESS;
        }

    } else {

        if ((f_error_code = l_pqmspeeder->IntensityCheck(l_objprotocol, MAP_AUTO_MODE, l_pos)) != E_NO_ERROR) {
            l_status = AUTO_INSERT_SUCCESS;
        }
    }

    //Restore the Org values..
    l_objprotocol->SetSpeederType(l_map_type_bak);
    l_objprotocol->SetIntensityType(l_intensity_type_bak);
    l_objprotocol->SetProtcolCorrStatus(l_protcolcorrstatus);
    l_objprotocol->GetSaturnCoil()->DegeneracyFlg = l_degenracy_flag;

    if (l_both_speeder_intensity_off) {
        f_error_code = -1;
    }

    CString l_tracemsg;
    l_tracemsg .Format(_T("return value l_status = %d"), l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    return l_status;
}

void CAutoInsertMapOperation::SetErrorCode(
    const int f_error_code
)
{
    m_error_code = f_error_code;
}


int CAutoInsertMapOperation::GetErrorCode(
) const
{
    return m_error_code;
}

