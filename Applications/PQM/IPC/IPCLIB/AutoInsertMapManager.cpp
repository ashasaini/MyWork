//Redmine-783
//////////////////////////////////////////////////////////////////////
// AutoMapMgr.cpp: implementation of the CAutoInsertMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertMapManager.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include "Pqm.h"
#include "PqmProtocol.h"
#include "AutoInsertMapOperation.h"
#include "scanprocedure.h"
#include "PqmPmAcqman.h"
#include "PqmCoilData.h"

using namespace MPlus::EH;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertMapManager
//Author        : PATNI/DKH
//Purpose       : Parameterize constructor
//**************************************************************************
CAutoInsertMapManager::CAutoInsertMapManager(
    CPqm* const f_pqm,
    const bool f_licence_available /*= true*/
):	CAutoInsertProtocolManager(f_pqm, f_licence_available)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::CAutoInsertMapManager");

    try {
        m_ptr_auto_operation = new CAutoInsertMapOperation(f_pqm, m_ptr_auto_insert_study);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured..."));
        throw;
    }
}

//**************************************************************************
//Method Name   : ~CAutoInsertMapManager
//Author        : PATNI/DKH
//Purpose       : Destructor
//**************************************************************************
CAutoInsertMapManager::~CAutoInsertMapManager()
{
    DEL_PTR(m_ptr_auto_operation);
}


//**************************************************************************
//Method Name   : AutoInsertProtocol
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsert_status CAutoInsertMapManager::AutoInsertProtocol(
    CPqmProtocol* const f_protocol,
    const AutoInsertData& f_autoinsert_data
)const
{
    return AutoMap(f_protocol, f_autoinsert_data);
}


AutoInsert_status CAutoInsertMapManager::CheckOnceInsertionPerScanStartCase(
    CPqmProtocol* f_target_prot
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::CheckOnceInsertionPerScanStartCase");

    if (NULL == f_target_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Target Protocol..."));
        return AUTO_INSERT_FAIL;
    }

    AutoInsert_status l_status = DisplayErrorMessageForMap(f_target_prot) ;

    if (AUTO_INSERT_FAIL == l_status) {
        f_target_prot->SetAutoInsertedMapProtocol(NULL);
    }

    return l_status ;
}

CPqmProtocol* CAutoInsertMapManager::GetPrevAutoInsertProtocolInfo(
    CPqmProtocol* const f_target_prot,
    int& f_prev_counter
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::GetPrevAutoInsertProtocolInfo");

    if (NULL == f_target_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Target Protocol..."));
        return CAutoInsertProtocolManager::GetPrevAutoInsertProtocolInfo(f_target_prot, f_prev_counter);
    }

    f_prev_counter = f_target_prot->GetMapInsertionCounter();
    return f_target_prot->GetAutoInsertedMapProtocol();
}

bool CAutoInsertMapManager::IsCoilChangedForTargetProt(
    CPqmProtocol* const f_target_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::IsCoilChangedForTargetProt");

    if (NULL == f_target_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_target_protocol is NULL"));
        return false;
    }

    OriginalParameters* l_original_parms = f_target_protocol->GetOriginalParameters();
    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    return !(l_coil_data->PqmIsSameCoilInSaturnforSPEEDER(&(l_original_parms->m_saturn_coil), f_target_protocol->GetSaturnCoil()));
}


//**************************************************************************
//Method Name   : AutoMap
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsert_status CAutoInsertMapManager::AutoMap(CPqmProtocol* const f_protocol, const AutoInsertData& f_autoinsert_data)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::AutoMap");
    PQM_SCOPED_TRACE();

    if (NULL == f_protocol || NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol, m_pqm_ptr or f_data NULL..!!"));

        AbortTargetScantoWait(f_protocol);
        return AUTO_INSERT_FAIL;
    }

    //For checking automap flag ON/OFF
    if (false == GetAutoFlag()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoMap flag is FALSE."));
        return AUTO_INSERT_NOT_REQUIRED;
    }

    if (!IsAutoInsertProtocolRequired(f_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoMap is not Required"));
        return AUTO_INSERT_NOT_REQUIRED;
    }

    AutoInsert_status l_status = AUTO_INSERT_NOT_REQUIRED;

    switch (f_protocol->GetMapRequired()) {

        case VFC_MAP_REQUIRED_TO_BEFORE:
        case VFC_MAP_REQUIRED_TO_AFTER:
            l_status = InsertAutoInsertProt(f_protocol, f_autoinsert_data, MAP_MODE);
            break;

        default :
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetMapRequired() is not None->should not have reached here->Continue Scan..."));
            //continue scan...
            l_status = AUTO_INSERT_NOT_REQUIRED;
    }

    return l_status;
}




//**************************************************************************
//Method Name   : DisplayErrorMessageForMap
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsert_status CAutoInsertMapManager::DisplayErrorMessageForMap(
    CPqmProtocol* f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::DisplayErrorMessageForMap");
    PQM_SCOPED_TRACE();

    CAutoInsertMapOperation* map_operation = dynamic_cast<CAutoInsertMapOperation*>(m_ptr_auto_operation);

    int l_error_code = map_operation->GetErrorCode();

    bool l_abort_onok = false;
    int ok_cancel_buttons = SCAN_WARN_OKBUTTON;

    CString l_warning_message = _T("");

    switch (l_error_code) {

        case -1: {
            l_abort_onok = true;	//default OK action
            ok_cancel_buttons = SCAN_WARN_OK_CANCEL_BUTTONS;
            l_warning_message = _T("IDS_CAN_CONTINUE_WITHOUT_AUTO_INSERT");
        }

        //No break here...
        case ERR_CAN_NOT_SPEEDER_MAP: {

            if (ERR_CAN_NOT_SPEEDER_MAP == l_error_code) {
                l_abort_onok = false; //default OK action
                ok_cancel_buttons = SCAN_WARN_OKBUTTON;
                l_warning_message = _T("ERR_CAN_NOT_SPEEDER_MAP");
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Warning Message for AutoMap :") + l_warning_message);
            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e)CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    CPqmUtilities::GetMultiLingualString(l_warning_message),
                    _T("Sequence Queue"),
                    SCAN_WARN_INFORMATION,
                    SCAN_WARN_SYSMODAL,
                    ok_cancel_buttons);


            switch (l_response) {
                case MPlusErrResponse_CANCEL :
                    l_abort_onok = false;
                    break;
            }
        }
        break;

        case ERR_TX_INTENSITY_NOTFOUND:
            l_error_code = PQM_WARN_TX_INTENSITY_NOTFOUND ;

            //No break here...
        case ERR_INTENSITY_NOTFOUND:

            if (ERR_INTENSITY_NOTFOUND == l_error_code) {
                l_error_code = PQM_WARN_INTENSITY_NOTFOUND;
            }

            //No break here...
        default:
            l_abort_onok = m_pqm_ptr->GetAcqManager()->GetScanProcedure()->WarningScan(
                               l_error_code, _T("AutoInsertMapManager.cpp"),
                               SCAN_WARN_OK_CANCEL_BUTTONS,
                               SCAN_WARN_CRUCIAL, SCAN_WARN_SYSMODAL, false);
    }

    return l_abort_onok ? AUTO_INSERT_NOT_REQUIRED : AUTO_INSERT_FAIL ;

}




//**************************************************************************
//Method Name   : IsAutoInsertProtocolRequired
//Author        : PATNI/AKR/NFD001
//Purpose       : To check AutoMap is required or not
//**************************************************************************
bool CAutoInsertMapManager::IsAutoInsertProtocolRequired(
    CPqmProtocol* const f_auto_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertMapManager::IsAutoInsertProtocolRequired");

    if (NULL == f_auto_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_auto_protocol pointer is NULL"));
        return false;
    }

    switch (f_auto_protocol->GetMapRequired()) {

        case VFC_MAP_REQUIRED_TO_BEFORE:
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RequireAutoMAP Flag: VFC_MAP_REQUIRED_TO_BEFORE"));
            break;

        case VFC_MAP_REQUIRED_TO_AFTER:
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RequireAutoMAP Flag: VFC_MAP_REQUIRED_TO_AFTER"));
            break;

        default: {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RequireAutoMAP Flag: NONE"));
            return false;
        }
    }

    return true;
}