//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
// AutoMapShimStudy.cpp: implementation of the CAutoInsertProtocolStudy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProtocolStudy.h"
#include <math.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include "PqmProtocol.h"
#include "PQM.h"
#include "PQMStudy.h"
#include "GPLibAdapter.h"
#include "PQMConfig.h"
#include "coil.h"
#include "PqmCoilData.h"
#include "AutoInsertProtConfig.h"

#include "CoilChannelMode.h"
#define SIZE 128



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertProtocolStudy
//Author        : PATNI/DKH
//Purpose       : This is the constructor function
//**************************************************************************
//Patni/2011Feb11/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
CAutoInsertProtocolStudy::CAutoInsertProtocolStudy(
    CPQMStudy* f_pqm_study
):  m_pqm_study(NULL),
    m_pqm_ptr(NULL),
    m_autoinsert_manager()
{
    SetPqmStudy(f_pqm_study);
    Init();
}


//**************************************************************************
//Method Name   : ~CAutoInsertProtocolStudy
//Author        : PATNI/DKH
//Purpose       : This is the Destructor function
//**************************************************************************
CAutoInsertProtocolStudy::~CAutoInsertProtocolStudy()
{
}


//**************************************************************************
//Method Name   : AddProtocol
//Author        : PATNI/DKH
//Purpose       :
//**************************************************************************
CPqmProtocol* CAutoInsertProtocolStudy::AddProtocol(const int f_target_position, const int* f_acqorder)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::AddProtocol");
    PQM_SCOPED_TRACE();

    if (NULL == f_acqorder || NULL == m_pqm_study) {
        return NULL;
    }

    CPqmProtocol* l_obj = m_pqm_study->ListCopyProt(m_pqm_study->GetVfDBHandle(),
                          *f_acqorder,
                          f_target_position);
    return l_obj;
}

//**************************************************************************
//Method Name   : AddAutoProtocol
//Author        : PATNI/DKH
//Purpose       :
//**************************************************************************
CPqmProtocol* CAutoInsertProtocolStudy::AddAutoProtocol(const int f_target_position, const scan_mode f_mode, CString& f_error_id)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::AddAutoProtocol");
    PQM_SCOPED_TRACE();

    try {

        if (NULL == m_pqm_study) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study pointer is NULL"));
            return NULL ;
        }

        if (false == CanPerformAutoMapAutoShim()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not perform AutoMap-AutoShim"));
            f_error_id = _T("IDS_AUTOINSERT_CONFIG_FILE_MISSING") ;
            return NULL ;
        }

        //Check for max protocol

        long l_maxprotocol_limit = CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();
        VuiAcqOrder_t l_acq_order;
        CVarFieldHandle* const l_vf_handle = m_pqm_study->GetVfDBHandle();
        ClibVfToUiAdapter vuiAdapter(l_vf_handle);

        if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_acq_order)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() Failed."));
            return NULL;
        }

        if ((l_maxprotocol_limit - l_acq_order.SzAcqOrder) <= 0) {
            f_error_id = _T("IDS_NO_SEQ_ACCEPATANCE");
            return NULL;
        }

        int l_inserted_index = -1;

        if (E_ERROR == CopyProtocolFromFile(f_target_position, l_inserted_index, f_mode)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CopyProtocolFromFile is failed !"));
            return NULL;
        }

        //Save the contrast agent value of the inserted protocol.
        BITFLD_DECL(mask, K_ALL_PR) = {0};
        memset(mask, 0, sizeof(mask));
        int l_contrast = -1;
        char l_contrast_name[SIZE] = {0};

        m_pqm_study->GetProtocolValues(m_pqm_study->GetVfDBHandle(), l_inserted_index, mask,
                                       K_PR_CONTRAST, &l_contrast, NULL,
                                       K_PR_CONTRAST_NAME, &l_contrast_name, NULL,
                                       NULL);
        //-Patni/2011Feb11/Added/V2.0/IR-181_NFD1_rod_Req_Part2
        CPqmProtocol* l_obj = AddProtocol(f_target_position, &l_inserted_index);

        if (NULL == l_obj) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_obj pointer is NULL"));
            return NULL;
        }

        CPqmProtocol* l_target_protocol = m_pqm_study->GetItemFromPosition(f_target_position);
        const int l_current_pos = l_obj->GetPosition() - 1;
        int l_new_pos = f_target_position - 1;

        switch (f_mode) {

            case MAP_MODE:

                if (VFC_MAP_REQUIRED_TO_AFTER == l_target_protocol->GetMapRequired()) {
                    //l_new_pos = f_target_position + 1;
                    l_new_pos = l_new_pos + 1 ;

                } else {
                    l_new_pos = f_target_position - 1;
                }

                break;

            case SHIM_MODE:

                if (VFC_SHIM_REQUIRED_TO_AFTER == l_target_protocol->GetRequireShimming()) {
                    l_new_pos = l_new_pos + 1;

                    //To avoid crash in case after delete map and run scan for target
                    if ((VFC_SHIM_REQUIRED_TO_AFTER == l_target_protocol->GetMapRequired()) && (NULL != l_target_protocol->GetAutoInsertedMapProtocol())) {
                        l_new_pos++;
                    }

                } else {
                    l_new_pos = f_target_position - 1;
                }

                break;
        }


        m_pqm_study->ReArrange(l_current_pos, l_new_pos);
        CPqmProtocol* l_prot = NULL;

        if ((VFC_MAP_REQUIRED_TO_AFTER == l_target_protocol->GetMapRequired()) || (VFC_SHIM_REQUIRED_TO_AFTER == l_target_protocol->GetRequireShimming())) {
            l_prot = l_target_protocol;

        } else if (l_obj->GetPosition() > 1) {
            l_prot = m_pqm_study->GetItemFromPosition(l_obj->GetPosition() - 1);
        }

        SetContrastAgentToAutoInsertedProtocol(l_obj, l_prot, l_contrast, l_contrast_name);

        return l_obj;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured CAutoInsertProtocolStudy::AddMapProtocol"));
        throw;
    }

    return NULL;
}

//**************************************************************************
//Method Name   : CopyProtocolFromFile
//Author        : PATNI/DKH
//Purpose       : Copy the map protocol from specified location to varfield database
//**************************************************************************
int CAutoInsertProtocolStudy::CopyProtocolFromFile(const int f_target_position, int& f_acqorder, const scan_mode f_mod)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::CopyProtocolFromFile");
    PQM_SCOPED_TRACE();

    UNREFERENCED_PARAMETER(f_target_position);

    //Just get the file from calculated data
    CString l_path_name = GetAutoInsertFilePath(f_mod);
    char l_seq_filepath[MAX_PATH] = {0};
    wcstombs(l_seq_filepath, l_path_name.GetBuffer(l_path_name.GetLength()), l_path_name.GetLength());

    int l_addacorder = -1;

    ClibVfToUiAdapter vuiAdapter(m_pqm_study->GetVfDBHandle());

    status_t l_status = vuiAdapter.VuiCopyProtocolFromPrivateProlimScratchPad(-1, l_seq_filepath, 0, 0, &l_addacorder);

    if (E_ERROR == m_pqm_study->RestoreSavedTagsAfterScratchpad(l_addacorder)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("RestoreSavedTagsAfterScratchpad failed to restore SVN tags value in varfield"));
    }

    f_acqorder = l_addacorder ;
    return l_status;
}

//**************************************************************************
//Method Name   : UpdateDegeneracyFlagOfAutoInsertProt
//Author        : PATNI/Rupamudra
//Purpose       :
//**************************************************************************
void CAutoInsertProtocolStudy::UpdateDegeneracyFlagOfAutoInsertProt(const int f_from_position, const int f_to_position, CPqmProtocol* f_to_Protocol)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::UpdateDegeneracyFlagOfAutoInsertProt");
    PQM_SCOPED_TRACE();

    CPqmProtocol* l_pqmprotocol = NULL;

    for (int i = f_from_position ; i <= m_pqm_study->GetAllItems() ; i++) {

        if (f_from_position == f_to_position) {
            continue;
        }

        l_pqmprotocol = m_pqm_study->GetItemFromPosition(i);

        if (l_pqmprotocol->GetAcqStatus() != C_PR_STATUS_WAIT) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Occured. l_pqmprotocol should be in WAIT state"));
            continue;
        }

        const int l_degeneracy_flag = m_pqm_study->GetTargetProtocolsDegeneracyFlag(l_pqmprotocol->GetProtocol());

        if (l_degeneracy_flag > 0) {
            m_pqm_ptr->GetStudy()->SetAutoMapProtocolsDegeneracyFlag(l_degeneracy_flag, f_to_Protocol->GetProtocol());
            break;
        }
    }
}

//**************************************************************************
//Method Name   : CopyScanModeFrom
//Author        : PATNI/DKH
//Purpose       : Copy Scan mode paramters from from_position to to_position
//**************************************************************************
bool CAutoInsertProtocolStudy::CopyScanModeFrom(const int f_from_position, const int f_to_position, const scan_mode f_mode)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::CopyScanModeFrom");
    PQM_SCOPED_TRACE();

    try {

        CString str_msg(_T(""));
        str_msg.Format(_T("[ from position: %d, to position: %d]"),
                       f_from_position, f_to_position);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        if (NULL == m_pqm_study) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_study is NULL"));
            return false;
        }

        if (!m_pqm_study->CopyScanModeFrom(m_pqm_study->GetVfDBHandle(), f_from_position, f_to_position, FALSE)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CopyScanModeFrom Failed"));
            return false;
        }


        CPqmProtocol* l_from_pos_Protocol = m_pqm_study->GetItemFromPosition(f_from_position);

        if (NULL == l_from_pos_Protocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_from_pos_Protocol Points to NULL"));
            return false;
        }

        // Get scan offset from target protocol

        CPqmProtocol* l_to_pos_Protocol = m_pqm_study->GetItemFromPosition(f_to_position);

        if (NULL == l_to_pos_Protocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_to_pos_Protocol Points to NULL"));
            return false;
        }

        if (f_mode == MAP_MODE) {
            UpdateDegeneracyFlagOfAutoInsertProt(f_from_position, f_to_position, l_to_pos_Protocol);
        }

        CCoilChannelMode* l_coilchannle_mode = m_pqm_ptr->GetCoilChannelMode();

        if (l_coilchannle_mode != NULL && l_coilchannle_mode->IsCoilChannelModeApplicable()) {

            VARIANT l_pData;
            l_coilchannle_mode->GetChannelModeSelection(l_from_pos_Protocol->GetProtocol(), &l_pData);
            int l_channel_mode = l_pData.intVal;

            if (CHANNEL_MODE_NORMAL  == l_channel_mode || CHANNEL_MODE_MAX == l_channel_mode) {
                l_coilchannle_mode->SetChannelModeSelection(l_to_pos_Protocol->GetProtocol(), l_channel_mode);
            }
        }

        VARIANT* pData = new VARIANT;

        if (FALSE == m_pqm_study->PqmUiSetScanOffset(l_from_pos_Protocol , &pData)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmUiSetScanOffset Failed"));
            pData->byref = NULL;
            DEL_PTR(pData);
            return false;
        }

        f32vec3_t* l_objvec = (f32vec3_t*) pData->byref;

        if (NULL == l_objvec) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_objvec Points to NULL"));
            pData->byref = NULL;
            DEL_PTR(pData);
            return false;
        }

        CString l_log_str = _T("");
        l_log_str.Format(_T("Calculated Scanoffset of Inserted Protocol--> %f,%f,%f"), l_objvec->x, l_objvec->y, l_objvec->z);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        // Set scan offset to Map protocol
        m_pqm_study->SetScanOffsetValue(l_to_pos_Protocol->GetProtocol(), l_objvec->x, l_objvec->y, l_objvec->z);

        pData->byref = NULL;
        DEL_PTR(l_objvec);
        DEL_PTR(pData);

        VARIANT* l_pData_move = new VARIANT;

        if (NULL == l_pData_move) {
            return false;
        }

        l_pData_move->wReserved1 = MOVE_COUCH_METHOD;
        l_pData_move->intVal = l_from_pos_Protocol->GetcouchMethodFlg();

        if (!m_pqm_study->UpdateGUIControlsInVarfieldDB(l_pData_move, l_to_pos_Protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateGUIControlsInVarfieldDB() failed."));
            DEL_PTR(l_pData_move);
            return false;
        }

        int l_hold = l_from_pos_Protocol->GetHoldScan();
        l_pData_move->wReserved1 = HOLD_VALUE;
        l_pData_move->wReserved2 = MOVE_COUCH;
        l_pData_move->intVal = l_hold;

        if (!m_pqm_study->UpdateGUIControlsInVarfieldDB(l_pData_move, l_to_pos_Protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateGUIControlsInVarfieldDB() failed."));
            DEL_PTR(l_pData_move);
            return false;
        }

        // Auto scan ON value need to set in study file for Auto Map/Shimm protocol
        // Auto scan and move couch value can't be set together, for setting move couch value in study file there are some check for coil.
        // If coil is not selected and AS_COMPAS is ON in that case Auto Map/Shimm protocol will be added in PQM but Auto scan for Auto Map/Shimm
        // protocol will not set in Study file.
        l_hold  &= (~VFC_HOLD_PRE_SCAN);
        l_pData_move->wReserved1 = HOLD_VALUE;
        l_pData_move->wReserved2 = 0;
        l_pData_move->intVal = l_hold;

        if (!m_pqm_study->UpdateGUIControlsInVarfieldDB(l_pData_move, l_to_pos_Protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateGUIControlsInVarfieldDB() failed."));
            DEL_PTR(l_pData_move);
            return false;
        }

        l_pData_move->wReserved1 = AUTO_VOICE;
        l_pData_move->wReserved2 = 0;
        l_pData_move->intVal = l_from_pos_Protocol->GetExeAutoVoice();

        if (!m_pqm_study->UpdateGUIControlsInVarfieldDB(l_pData_move, l_to_pos_Protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateGUIControlsInVarfieldDB() failed."));
        }

        //Set AS-COMPAS for Map protocol
        CopyCOMPASSSetting(l_from_pos_Protocol, l_to_pos_Protocol, f_mode);

        DEL_PTR(l_pData_move);
        return true;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured CAutoInsertProtocolStudy::CopyScanModeFrom"));
        throw;
    }

    return false;
}

//**************************************************************************
//Method Name   : CalcTargetPosition
//Author        : PATNI/DKH
//Purpose       : This function will calculate the target position depending
//                on the couch move method flag
//**************************************************************************
float CAutoInsertProtocolStudy::CalcTargetPosition(CPqmProtocol* f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::CalcTargetPosition");
    PQM_SCOPED_TRACE();

    if (NULL == m_pqm_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr is NULL"));
        return 0;
    }

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();
    CPqmProtocol* l_curr_protocol = f_protocol;

    if (NULL == l_curr_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_protocol pointer is NULL"));
        return 0;
    }

    CPqmPrMode* l_pr_mode = l_curr_protocol->GetPqmPrMode();;
    float   l_new_pos = l_pr_mode->GetCouchPos();

    const int l_couch_move_method = l_curr_protocol->GetcouchMethodFlg();

    if ((!(l_curr_protocol->GetHoldScan() & VFC_HOLD_COUCH)) || (l_couch_move_method < 0)) {
        return l_new_pos;
    }



    int l_in_side_limit = -1, l_out_side_limit = -1;
    CCoil l_coil;

    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        l_coil_data->GetInOutSideLimit(l_curr_protocol->GetCoilDBTable(),
                                       &l_in_side_limit, &l_out_side_limit);

    } else {
        l_coil_data->PqmCoilGetCoil(l_pr_mode->GetRecvCoilId(), l_coil);
        l_out_side_limit = l_coil.GetOutsideLimit() ;
        l_in_side_limit = l_coil.GetInsideLimit();
    }

    float l_distance = (l_pr_mode->GetHomePos() - l_pr_mode->GetCouchPos());

    f32vec3_t l_offset = l_curr_protocol->GetOffset();
    float z = l_offset.z * 1000.0f;
    float offset = 0.0f;

    if (VFC_PATORI_HEAD_FIRST == l_pr_mode->GetPatoriEndIn()) {
        offset = floorf(l_distance + z);

    } else {
        offset = floorf(l_distance - z);
    }

    l_new_pos = l_pr_mode->GetCouchPos() + offset;

    if (VFC_MOVE_COUCH_METHOD_PARENT_POSI == l_couch_move_method) {
        if (SVN_ST_PROTOCOL == l_curr_protocol->GetLocatorImages(0).subtree_name) {
            CPqmProtocol* l_protocol = l_study->GetItemFromProtocol(l_curr_protocol->GetLocatorImages(0).index);

            if (NULL != l_protocol) {
                l_new_pos  = l_protocol->GetPqmPrMode()->GetCouchPos();
                offset  = l_protocol->GetPqmPrMode()->GetCouchPos() - l_curr_protocol->GetPqmPrMode()->GetCouchPos();

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL."));
            }
        }
    }

    CString l_str_msg(_T(""));
    l_str_msg.Format(_T("Auto Map: Calculated couch Position %f"), l_new_pos);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

    return l_new_pos;
}



//+Patni/2011Feb11/Added + Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
//**************************************************************************
//Method Name   : ScanInit
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CAutoInsertProtocolStudy::ScanInit(
    CPqmProtocol* f_targ_protocol,
    const scan_mode f_mode
)
{
    CalculateShimMapStudyData(f_targ_protocol, f_mode);
}


//***************************Method Header*************************************
//Method Name    : ConfigureAutoInsertedProtocol
//Author         :
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtocolStudy::ConfigureAutoInsertedProtocol(
    CVarFieldHandle* const f_vf_handle,
    const int32_t f_tarProtocol,
    const int32_t f_insertProt,
    const scan_mode f_mode,
    const bool f_can_display_message,
    bool& f_has_displayed_message
)
{
    CAutoInsertProtConfig l_autoinsertprotconfig(m_pqm_study, GetShimMapStudy());

    return l_autoinsertprotconfig.ConfigureAutoInsertedProtocol(
               f_vf_handle, f_tarProtocol, f_insertProt, f_mode, f_can_display_message, f_has_displayed_message);
}




//**************************************************************************
//Method Name   : SetParentInfoToAutoIsertedProtocol
//Author        : PATNI/SS
//Purpose       : Copy the target protocol parent info and set to map protocol
//**************************************************************************
void CAutoInsertProtocolStudy::SetParentInfoToAutoIsertedProtocol(
    CPqmProtocol* f_from_protocol,
    CPqmProtocol* f_to_protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::SetParentInfoToAutoIsertedProtocol");
    PQM_SCOPED_TRACE();

    if ((NULL == f_from_protocol) || (NULL == m_pqm_study)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_from_protocol or m_pqm_study points to NULL"));
        return;
    }

    if (NULL == f_to_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_to_protocol points to NULL"));
        return;
    }

    BOOL status = false;
    VfPathElem_t path[PATH_BUFFER] = {0};
    int prot = f_from_protocol->GetProtocol();

    BITFLD_DECL(mask, K_ALL_PR);
    int size = sizeof(path);
    memset(mask, 0, sizeof(mask));

    status = m_pqm_study->GetProtocolValues(m_pqm_study->GetVfDBHandle(),
                                            prot,
                                            mask,
                                            K_PR_LOCATOR_IMAGE,
                                            path,
                                            &size,
                                            NULL);

    const int l_to_protocol = f_to_protocol->GetProtocol();

    if (E_NO_ERROR == status   &&
        !BITFLD_ON(mask, K_PR_LOCATOR_IMAGE) &&
        path[0].subtree_name > 0) {

        f_to_protocol->SetHasParent(TRUE);
        f_to_protocol->SetLocatorImages(path);

        memset(mask, 0, sizeof(mask));



        if (E_NO_ERROR != m_pqm_study->SetProtocolValues(m_pqm_study->GetVfDBHandle(),
                l_to_protocol,
                mask,
                K_PR_LOCATOR_IMAGE, path,   4 * sizeof(VfPathElem_t),
                NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }
    }

    int plan_status = f_from_protocol->GetPlnStatus();
    SetPlnStatusOfAutoInsertProtocol(plan_status, f_to_protocol);

    /// Update protocol values
    UpdateLocatorImageInfo(f_to_protocol->GetProtocol(), f_from_protocol->GetProtocol());

}






//**************************************************************************
//Method Name   : SetContrastAgentToAutoInsertedProtocol
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CAutoInsertProtocolStudy::SetContrastAgentToAutoInsertedProtocol(
    CPqmProtocol* f_auto_protocol,
    CPqmProtocol* f_contrast_target,
    const int f_contrast,
    char* f_contrast_name
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolOperation::SetContrastAgentToAutoInsertedProtocol");
    PQM_SCOPED_TRACE();

    try {

        VARIANT l_insert_data;

        l_insert_data.wReserved1 = CONTRAST_AGENT;
        CString pStr = _T("");
        BSTR bstrcontrast_agent;

        if (NULL != f_contrast_target) {
            pStr = f_contrast_target->GetContrastName();
            bstrcontrast_agent = pStr.AllocSysString();
            l_insert_data.byref = bstrcontrast_agent;
            l_insert_data.wReserved3 = f_contrast_target->GetPqmPrMode()->GetContrast();

        } else {

            pStr = f_contrast_name;
            bstrcontrast_agent = pStr.AllocSysString();

            l_insert_data.byref = bstrcontrast_agent;
            l_insert_data.wReserved3 = f_contrast;
        }

        if (!m_pqm_study->UpdateGUIControlsInVarfieldDB(&l_insert_data, f_auto_protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::UpdateGUIControlsInVarfieldDB() returned FALSE value."));
        }

        SysFreeString(bstrcontrast_agent);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured in CAutoShimOperation::SetContrastAgentToShim()"));
        throw;
    }
}


//**************************************************************************
//Method Name   : GetAutoInsertFilePath
//Author        : PATNI/DKH
//Purpose       :
//**************************************************************************
CString CAutoInsertProtocolStudy::GetAutoInsertFilePath(
    const scan_mode f_mode
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::GetAutoInsertFilePath");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CString l_shim_map_file_path = _T("") ;

    ShimMapStudy* l_shimmapstudy_ptr = GetShimMapStudy();

    if (NULL == l_shimmapstudy_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_shimmapstudy_ptr is NULL"));
        return _T("");
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetAutoInsertFilePath() return Value -> ") + l_shimmapstudy_ptr->m_filename);

    return l_shimmapstudy_ptr->m_filename;

}
//-Patni/2011Feb11/Added + Modified/V2.0/IR-181_NFD1_Prod_Req_Part1


//**************************************************************************
//Method Name   : UpdateLocatorImageInfo
//Author        : PATNI/SS
//Purpose       : Copy the protocol from specified location to varfield database
//**************************************************************************
void CAutoInsertProtocolStudy::UpdateLocatorImageInfo(const int f_target_position, const int f_from_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::UpdateLocatorImageInfo");
    PQM_SCOPED_TRACE();

    VfFieldSpec_t       vfReq[12] = {0};
    VfFieldSpec_t       vfPutReq[12] = {0};
    int         	n = 0 , num = 0;
    time_t      	ts = 0;

    VfPathElem_t        path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_VISUALGROUP,    0},
    };

    path[SVD_ST_PROTOCOL - 1].index = f_from_protocol;


    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_2, SVT_PLN_LOCATOR_IMAGE_2,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_3, SVT_PLN_LOCATOR_IMAGE_3,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_4, SVT_PLN_LOCATOR_IMAGE_4,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_5, SVT_PLN_LOCATOR_IMAGE_5,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_6, SVT_PLN_LOCATOR_IMAGE_6,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID, SVT_PLN_LOCATOR_IMAGE_ID,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_2, SVT_PLN_LOCATOR_IMAGE_ID_2,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_3, SVT_PLN_LOCATOR_IMAGE_ID_3,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_4, SVT_PLN_LOCATOR_IMAGE_ID_4,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_5, SVT_PLN_LOCATOR_IMAGE_ID_5,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_6, SVT_PLN_LOCATOR_IMAGE_ID_6,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;


    CVarFieldHandle* const vf_handle = m_pqm_study->GetVfDBHandle();

    status_t l_status = vf_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, n, &num);


    if (E_NO_ERROR != l_status  && num != n) {
        DB_PRINTF("", ("Put Data Error. (%s:%d) \n", __FILE__, __LINE__));
        DB_FUNC_EXIT();

        vf_handle->Close() ;
        return ;
    }

    memset(vfPutReq, NULL, sizeof(VfFieldSpec_t) * 12);
    n = 0;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_2, SVT_PLN_LOCATOR_IMAGE_2,
                 vfReq[n].size, vfReq[n].buffer,  VFO_REPLACE);

    n++;

    if (E_NO_ERROR == vfReq[n].status) {
        VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_3, SVT_PLN_LOCATOR_IMAGE_3,
                     vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);
    }

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_4, SVT_PLN_LOCATOR_IMAGE_4,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_5, SVT_PLN_LOCATOR_IMAGE_5,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_6, SVT_PLN_LOCATOR_IMAGE_6,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_ID, SVT_PLN_LOCATOR_IMAGE_ID,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_ID_2, SVT_PLN_LOCATOR_IMAGE_ID_2,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_ID_3, SVT_PLN_LOCATOR_IMAGE_ID_3,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfReq[n], SVN_PLN_LOCATOR_IMAGE_ID_4, SVT_PLN_LOCATOR_IMAGE_ID_4,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_ID_5, SVT_PLN_LOCATOR_IMAGE_ID_5,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;


    VFF_SET_ARGS(vfPutReq[n], SVN_PLN_LOCATOR_IMAGE_ID_6, SVT_PLN_LOCATOR_IMAGE_ID_6,
                 vfReq[n].size, vfReq[n].buffer, VFO_REPLACE);

    n++;

    path[SVD_ST_PROTOCOL - 1].index = f_target_position ;
    num = 0;

    l_status = vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num, &ts);

    if (l_status != E_NO_ERROR || num != n) {
        DB_PRINTF("", ("Put Data Error. (%s:%d) \n", __FILE__, __LINE__));
    }
}

void CAutoInsertProtocolStudy::SetPlnStatusOfAutoInsertProtocol(
    int f_pln_status,
    CPqmProtocol* const f_to_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::SetPlnStatusOfAutoInsertProtocol");
    PQM_SCOPED_TRACE();

    if (NULL == f_to_protocol) {
        return ;
    }

    VfPathElem_t        path[] = {
        {SVN_ST_PROTOCOL,       0}
    };

    path[SVD_ST_PROTOCOL - 1].index = f_to_protocol->GetProtocol();

    VfFieldSpec_t       vfReq[3] = {0};
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 3);
    int n = 0, num = 0;
    time_t      	ts = 0;


    VFF_SET_ARGS(vfReq[n], SVN_PLN_STATUS, SVT_PLN_STATUS,
                 sizeof(f_pln_status), &f_pln_status, VFO_REPLACE);
    n++;
    status_t status = E_ERROR;
    CVarFieldHandle* const vf_handle = m_pqm_study->GetVfDBHandle();

    status = vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num, &ts);

    if (status != E_NO_ERROR || num != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() failed"));
    }

    f_to_protocol->SetPlnStatus(f_pln_status);
}


void CAutoInsertProtocolStudy::CopyCOMPASSSetting(
    CPqmProtocol* const f_from_protocol,
    CPqmProtocol* const f_to_protocol,
    const scan_mode f_mode
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::CopyCOMPASSSetting");
    PQM_SCOPED_TRACE();

    bool l_copy_as_compass = true;
    const int l_map_required = f_from_protocol->GetMapRequired();
    const int l_shim_required = f_from_protocol->GetRequireShimming();

    switch (f_mode) {

        case MAP_MODE:

            if (l_map_required == VFC_MAP_REQUIRED_TO_AFTER) {
                l_copy_as_compass = false;
            }

            break;

        case SHIM_MODE:

            if (l_shim_required == VFC_SHIM_REQUIRED_TO_AFTER) {
                l_copy_as_compass = false;
            }

            break;
    }

    if (l_copy_as_compass) {
        if (!m_pqm_study->OnOffAsCompass(f_to_protocol->GetProtocol(), f_from_protocol->GetASCOMPASSFlag() == TRUE)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("OnOffAsCompass() failed."));
        }
    }
}

//**************************************************************************
//Method Name   : Init
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CAutoInsertProtocolStudy::Init()
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::Init");
    PQM_SCOPED_TRACE();

    if (K_IMAGING == m_pqm_ptr->GetMode()) {
        m_autoinsert_manager.ReadAutoInsertProtocolXML();
    }
}

//**************************************************************************
//Method Name   : CalculateShimMapStudyData
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CAutoInsertProtocolStudy::CalculateShimMapStudyData(
    CPqmProtocol* f_targ_protocol,
    const scan_mode f_mode
)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudy::CalculateShimMapStudyData");
    PQM_SCOPED_TRACE();

    if (NULL == m_autoinsert_manager.GenerateMapShimData(f_targ_protocol , f_mode)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shim Map Sate is not generated"));
    }
}

//*****************************************************************************/
//Method Name    :CanPerformAutoMapAutoShim
//Author         :PATNI
//Purpose        :
//*****************************************************************************/
inline bool CAutoInsertProtocolStudy::CanPerformAutoMapAutoShim(
) const
{
    return (NULL != GetShimMapStudy());
}

//*****************************************************************************/
//Method Name    :SetPqmStudy
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************/
void CAutoInsertProtocolStudy::SetPqmStudy(
    CPQMStudy* f_pqm_study
)
{
    if (NULL == f_pqm_study) {
        return ;
    }

    m_pqm_study = f_pqm_study;
    m_pqm_ptr = f_pqm_study->GetPqm();
}

//***************************Method Header*************************************
//Method Name    : GetShimMapStudy
//Author         :
//Purpose        :
//*****************************************************************************
ShimMapStudy* CAutoInsertProtocolStudy::GetShimMapStudy(
) const
{
    return m_autoinsert_manager.GetAutoShimMapData();
}