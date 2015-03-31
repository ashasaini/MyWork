// AutoInsertProtConfig.cpp: implementation of the CAutoInsertProtConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProtConfig.h"
#include <math.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <libStudyManager/Math.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "PQMStudy.h"
#include "PQM.h"
#include "PQMPmSARManager.h"
#include "PqmProtocol.h"
#include "GPLibAdapter.h"
#include "VarFieldHandle.h"
#include "LibPLTreeWrapper.h"
#include "PosSlabInfo.h"

//+Patni/2011Feb11/Added+Mod/V2.0/IR-181_NFD1_Prod_Req_Part1
const static CString SCAN_ENLARGEMENT_AREA_SL = _T("SL");
const static CString SCAN_ENLARGEMENT_AREA_PE = _T("PE");
const static CString SCAN_ENLARGEMENT_AREA_RO = _T("RO");
//-Patni/2011Feb11/Added+Mod/V2.0/IR-181_NFD1_Prod_Req_Part1

using namespace MPlus::EH;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//***************************Method Header*************************************
//Method Name    : CAutoInsertProtConfig
//Author         :
//Purpose        :
//*****************************************************************************
CAutoInsertProtConfig::CAutoInsertProtConfig(
    CPQMStudy* m_pqm_study,
    ShimMapStudy* f_shimmapstudy_ptr
) : m_pqm_study(m_pqm_study),
    m_shim_map_study_ptr(f_shimmapstudy_ptr),
    m_auto_insert_scratch_pad(CPqmUtilities::GetAutoInsertProlimScratchPadPath())
{

}
//***************************Method Header*************************************
//Method Name    : ~CAutoInsertProtConfig
//Author         :
//Purpose        :
//*****************************************************************************
CAutoInsertProtConfig::~CAutoInsertProtConfig()
{
    DEL_PTR(m_auto_insert_scratch_pad);
}

//***************************Method Header*************************************
//Method Name    : ConfigureAutoInsertedProtocol
//Author         :
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::ConfigureAutoInsertedProtocol(
    CVarFieldHandle* const f_vf_handle,
    const int32_t f_tarProtocol,
    const int32_t f_insertProt,
    const scan_mode f_mode,
    const bool f_can_display_message,
    bool& f_has_displayed_message
)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::ConfigureAutoInsertedProtocol");
    PQM_SCOPED_TRACE();

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    CPqmProtocol* l_protocol = m_pqm_study->GetProtocol(f_tarProtocol);

    if (NULL == l_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol is NULL"));
        return E_ERROR;
    }

    AutoInsertParameters l_calc_paramaters;

    bool l_flag = false;
    status_t l_status = GetCalculatedParameters(l_calc_paramaters, f_insertProt, f_tarProtocol, f_vf_handle, f_mode, l_flag);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetCalculatedParameters return Error"));
        return l_status;
    }

    CPqmProtocol* l_inserted_protocol = m_pqm_study->GetProtocol(f_insertProt);

    if (NULL == l_inserted_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_inserted_protocol is NULL, return Error"));
        return l_status;
    }

    l_calc_paramaters.SetScanOffset(l_inserted_protocol->GetOffset());
    AutoInsertParameters l_validated_params;
    l_status = GetValidatedParameters(l_calc_paramaters, l_validated_params, f_insertProt, l_flag);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetValidatedParameters return Error"));
        DeleteAutoInsertScratchPad(m_auto_insert_scratch_pad);
        return l_status;
    }

    l_status = UpdateValidatedParametersToStudy(f_insertProt);                    //Shifted location of call

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not write the validated parameters to the study file"));
        DeleteAutoInsertScratchPad(m_auto_insert_scratch_pad);
        return l_status;
    }

    m_pqm_study->UpdateSARControlInfoOnProtocolUpdate(l_inserted_protocol);

    //Delete AuotoInsert ScratchPad
    DeleteAutoInsertScratchPad(m_auto_insert_scratch_pad);

    if (CompareCalculatedParameters(l_calc_paramaters, l_validated_params, l_flag)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CompareCalculatedParameters() return true"));

        const bool l_can_display_msg = ((SHIM_MODE == f_mode) && (VFC_SHIM_REQUIRED_TO_BEFORE == l_protocol->GetRequireShimming()))
                                       || ((MAP_MODE == f_mode) && (VFC_MAP_REQUIRED_TO_BEFORE == l_protocol->GetMapRequired()));

        if (f_can_display_message && l_can_display_msg) {

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(_T("IDS_AUTO_INSERT_PARAMETER_ERROR")),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));

            f_has_displayed_message = true;
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Function End"));

    return l_status;
}


//***************************Method Header*************************************
//Method Name    : CompareCalculatedParameters
//Author         : PATNI
//Purpose        :
//*****************************************************************************
bool CAutoInsertProtConfig::CompareCalculatedParameters(
    const AutoInsertParameters& f_calc_params,
    const AutoInsertParameters& f_validated_params,
    const bool f_flag
) const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::CompareCalculatedParameters");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const flt32_t l_offset = 0.001001f;

    if (f_flag) {

        if ((f_calc_params.m_slice_gap - l_offset) > f_validated_params.m_slice_gap)
            return true;
    }

    return (
               ((f_calc_params.m_fov.x - l_offset) > f_validated_params.m_fov.x) ||
               ((f_calc_params.m_fov.y - l_offset) > f_validated_params.m_fov.y) ||
               (
                   (f_calc_params.m_scan_offset.x > f_validated_params.m_scan_offset.x) &&
                   (f_calc_params.m_scan_offset.y > f_validated_params.m_scan_offset.y) &&
                   (f_calc_params.m_scan_offset.z > f_validated_params.m_scan_offset.z)
               )
           ) ;
}

//***************************Method Header*************************************
//Method Name    : CreateAutoInsertProlimScratchPad
//Author         : PATNI
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::CreateAutoInsertProlimScratchPad(
    const int f_protocol
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::CreateAutoInsertProlimScratchPad");
    PQM_SCOPED_TRACE();

    FILE* l_file_pointer = fopen(m_auto_insert_scratch_pad, "a");

    if (l_file_pointer) {
        fclose(l_file_pointer);
        l_file_pointer = NULL;
    }

    //return (m_pqm_study->CreateScratchPad(f_protocol, m_auto_insert_scratch_pad));
    return (m_pqm_study->MaintainScratchPadForPrivateProlim(f_protocol, m_auto_insert_scratch_pad, SP_CREATE_MODE_PRIVATE_PROLIM));
}



//***************************Method Header*************************************
//Method Name    : UpdateValidatedParametersToStudy
//Author         : PATNI
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::UpdateValidatedParametersToStudy(
    const int f_insert_protocol
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::UpdateValidatedParametersToStudy");
    PQM_SCOPED_TRACE();

    CVarFieldHandle* l_vfdb_handle = m_pqm_study->GetVfDBHandle();

    if (NULL == l_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vfdb_handle is NULL ..!!"));
        return E_ERROR;
    }

    ClibVfToUiAdapter vuiAdapter(l_vfdb_handle);

    status_t l_status = vuiAdapter.VuiCopyProtocolFromPrivateProlimScratchPad(f_insert_protocol, m_auto_insert_scratch_pad, 0, NULL, NULL);

    if (E_ERROR == l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not Update the Values to the Study file"));
    }

    if (E_ERROR == m_pqm_study->RestoreSavedTagsAfterScratchpad(f_insert_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("RestoreSavedTagsAfterScratchpad failed to restore SVN tags value in varfield"));
    }

    return l_status;
}

//***************************Method Header*************************************
//Method Name    : DeleteAutoInsertScratchPad
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::DeleteAutoInsertScratchPad(
    char* f_file_name
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::DeleteAutoInsertScratchPad");
    PQM_SCOPED_TRACE();

    WIN32_FIND_DATA FindFileData;

    CString l_str_filename(f_file_name);
    HANDLE hFindFile = FindFirstFile(l_str_filename, &FindFileData);

    if (INVALID_HANDLE_VALUE != hFindFile) {
        DeleteFile(l_str_filename);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not find the file for deletion"));
    }

    FindClose(hFindFile);
    hFindFile = NULL;
}



//***************************Method Header*************************************
//Method Name    : WriteCalculatedParametersToScartchPad
//Author         : PATNI
//Purpose        :
//*****************************************************************************
bool CAutoInsertProtConfig::WriteCalculatedParametersToScratchPad(
    const AutoInsertParameters& f_calc_paramaters,
    const bool f_flag
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::WriteCalculatedParametersToScartchPad");
    PQM_SCOPED_TRACE();

    VfPathElem_t l_path[NUM_3] = {0};
    memset(l_path, 0, sizeof(VfPathElem_t) * NUM_3);

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = 0;
    l_path[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[SVD_ST_SUBPROTOCOL - 1].index = 0;

    CVarFieldHandle l_varfield_handle((CString)m_auto_insert_scratch_pad);

    if (l_varfield_handle.VuiOpen() != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not open Scratch pad PQM.AutoInsert.Imaging "));
        return false;
    }

    VfFieldSpec_t l_vfReq[NUM_3] = {0};
    memset(l_vfReq, 0, sizeof(VfFieldSpec_t) * NUM_3);
    int l_n = 0, l_num = 0;
    time_t l_tm = 0;

    AutoInsertParameters l_calc_paramaters = f_calc_paramaters ;

    VFF_SET_ARGS(l_vfReq[l_n], SVN_PLN_FOV, VFT_FLTVEC2, sizeof(l_calc_paramaters.m_fov), &l_calc_paramaters.m_fov, VFO_REPLACE);
    l_n++;

    if (true == f_flag) {
        VFF_SET_ARGS(l_vfReq[l_n], SVN_PLN_SELECTED_SLICE_GAP, VFT_FLT32, sizeof(l_calc_paramaters.m_slice_gap), &l_calc_paramaters.m_slice_gap , VFO_REPLACE);
        l_n++;
    }

    if (E_NO_ERROR != l_varfield_handle.PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, l_n, &l_num, &l_tm)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not write FOV to Sctratchpad"));
        l_varfield_handle.VuiClose();
        return false;
    }

    l_varfield_handle.VuiClose();

    return true;
}

//***************************Method Header*************************************
//Method Name    : GetValidatedParametersFromSctratchPad
//Author         :
//Purpose        :
//*****************************************************************************
bool CAutoInsertProtConfig::GetValidatedParametersFromSctratchPad(
    AutoInsertParameters& f_validated_params,
    const bool f_flag
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetValidatedParametersFromSctratchPad");
    PQM_SCOPED_TRACE();

    VfPathElem_t l_path[NUM_3] = {0};
    memset(l_path, 0, sizeof(VfPathElem_t) * NUM_3);
    VfFieldSpec_t l_vfreq[NUM_3] = {0};
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t));

    int l_n = 0, l_num = 0;

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = 0;
    l_path[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[SVD_ST_SUBPROTOCOL - 1].index = 0;

    VfPathElem_t l_vpath[NUM_3] = {0};
    memset(l_vpath, 0, sizeof(VfPathElem_t) * NUM_3);

    l_vpath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_vpath[SVD_ST_PROTOCOL - 1].index = 0;
    l_vpath[SVD_ST_VISUALGROUP - 1].subtree_name =  SVN_ST_VISUALGROUP;
    l_vpath[SVD_ST_VISUALGROUP - 1].index = 0;

    CVarFieldHandle l_varfield_handle((CString)m_auto_insert_scratch_pad);

    if (l_varfield_handle.VuiOpen() != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not open Scratch pad PQM.AutoInsert.Imaging "));
        return false;
    }

    flt32_t l_validated_slice_gap = 0.0;
    flt32_t l_fov[NUM_2] = {0.0, 0.0};
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_FOV, VFT_FLTVEC2, sizeof(f_validated_params.m_fov), &f_validated_params.m_fov, 0);
    l_n++;

    if (true == f_flag) {
        VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_SELECTED_SLICE_GAP, VFT_FLT32, sizeof(f_validated_params.m_slice_gap), &f_validated_params.m_slice_gap, 0);
        l_n++;
    }

    status_t l_status = l_varfield_handle.GetFields(l_path, SVD_ST_PROTOCOL, l_vfreq, l_n, &l_num);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not get the validated FOV value from Scratchpad"));
        l_varfield_handle.VuiClose();
        return false;
    }

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * NUM_3);
    l_n = 0;
    VFF_SET_ARGS(l_vfreq[l_n], SVN_PLN_VG_OFFSET_VECTOR, VFT_FLTVEC3, sizeof(f_validated_params.m_scan_offset), &f_validated_params.m_scan_offset, 0);
    l_n++;

    l_status = l_varfield_handle.GetFields(l_vpath, SVD_ST_VISUALGROUP, l_vfreq, l_n, &l_num);

    if (E_NO_ERROR != l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not get the validated Scan offet value from Scratchpad"));
        l_varfield_handle.VuiClose();
        return false;
    }

    CString l_log_str = _T("");
    l_log_str.Format(_T("Value after validation from Prolim FOV[0],FOV[1],slicegap,scanoffset-> %f,%f,%f,%f,%f,%f"),
                     f_validated_params.m_fov.x, f_validated_params.m_fov.y, f_validated_params.m_slice_gap, f_validated_params.m_scan_offset.x,
                     f_validated_params.m_scan_offset.y, f_validated_params.m_scan_offset.z);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_varfield_handle.VuiClose();

    return true;
}

//***************************Method Header*************************************
//Method Name    : CompareAndGetCalculatedParametersWithLimit
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::CompareAndGetCalculatedParametersWithLimit(
    AutoInsertParameters& f_calc_params,
    const msPosSlabInfo_t* const f_insPosSlab_Info,
    const flt32_t f_maximum_z,
    const bool f_slice_flag) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::CompareAndGetCalculatedParametersWithLimit");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    CString l_log_str;

    const ShimMapStudy* const l_shimmapstudy_ptr = GetShimMapStudy();
    flt32_t l_limit_pe = (l_shimmapstudy_ptr->m_scanregionlimit_list[ScanRegionLimit_PE].m_limit_range) / 1000.0f;
    flt32_t l_limit_ro = (l_shimmapstudy_ptr->m_scanregionlimit_list[ScanRegionLimit_RO].m_limit_range) / 1000.0f;
    flt32_t l_limit_sl = (l_shimmapstudy_ptr->m_scanregionlimit_list[ScanRegionLimit_SL].m_limit_range) / 1000.0f;
#define GETMIN(dest, src) if((src>=0) && (src<dest)) dest = src ;

    GETMIN(f_calc_params.m_fov.x, l_limit_pe);
    GETMIN(f_calc_params.m_fov.y, l_limit_ro);

    if (f_maximum_z > l_limit_sl && l_limit_sl >= 0) {
        CalculateSliceGap(f_insPosSlab_Info, l_limit_sl, f_calc_params, false);

    } else if (f_slice_flag) {
        CalculateSliceGap(f_insPosSlab_Info, f_maximum_z, f_calc_params, true);
    }

    l_log_str.Format(_T("Calculated FOV After Comparing with ScanRangeLimit For AutoInsert Protocol: (%f,%f)"), f_calc_params.m_fov.x, f_calc_params.m_fov.y);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
}

//***************************Method Header*************************************
//Method Name    : GetMaxZ
//Author         :
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::GetMaxZ(
    const flt32_t f_vector_length_target_Zval,
    const flt32_t f_vector_length_insert_Zval,
    flt32_t& f_max_Zval,
    bool& f_slice_flag) const
{
    if (f_vector_length_target_Zval > f_vector_length_insert_Zval) {
        f_max_Zval = f_vector_length_target_Zval;
        f_slice_flag = true;

    } else {
        f_max_Zval = f_vector_length_insert_Zval;
        f_slice_flag = false;
    }
}

//***************************Method Header*************************************
//Method Name    : GetCalculatedParameters
//Author         :
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::GetCalculatedParameters(
    AutoInsertParameters& f_calc_parameters,
    const int f_insertProt,
    const int f_tarProtocol,
    CVarFieldHandle* const f_vf_handle,
    const scan_mode f_mode,
    bool& f_flag
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetCalculatedParameters");
    PQM_SCOPED_TRACE();

    CVector3D l_tarMaxPos(-9999, -9999, -9999);
    CVector3D l_tarMinPos(9999, 9999, 9999);
    status_t l_status = GetMinMaxForTargetProt(f_vf_handle, f_tarProtocol, l_tarMaxPos, l_tarMinPos);

    if (E_ERROR == l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetMinMaxForTargetProt returns E_ERROR"));
        return l_status;
    }

    int32_t	  l_insertprotVisuals = 0;

    ClibmsVfAdapter libmsvfAdapter(f_vf_handle);
    l_status = libmsvfAdapter.msVfCountSlabs(f_insertProt, &l_insertprotVisuals);

    if (E_NO_ERROR != l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msVfCountSlabs return error"));
        return l_status;
    }

    msPosSlabInfo_t* l_insPosSlabInfo = (msPosSlabInfo_t*)malloc(sizeof(msPosSlabInfo_t) * l_insertprotVisuals);

    if (!l_insPosSlabInfo) {
        l_status = E_ERROR;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not allocate memory to l_insPosSlabInfo..!!"));
        return l_status;
    }

    CVector3D l_insMaxPos(-9999, -9999, -9999);
    CVector3D l_insMinPos(9999, 9999, 9999);
    l_status = GetMinMaxForInsertedProt(l_insPosSlabInfo, l_insertprotVisuals, f_vf_handle, f_insertProt, l_insMaxPos, l_insMinPos);

    if (E_NO_ERROR != l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetMinMaxForInsertedProt returns E_ERROR"));
        FREE_PTR(l_insPosSlabInfo);
        return l_status;
    }

    CVector3D l_length_target = l_tarMaxPos - l_tarMinPos;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Target Protocol Length :") + l_length_target.ToString());

    CVector3D l_length_insert = l_insMaxPos - l_insMinPos;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoInsert Protocol Length :") + l_length_insert.ToString());

    CVector3D l_vector_length_target, l_vector_length_insert;
    l_vector_length_target = l_insPosSlabInfo->ProductOrientationLength(l_length_target);
    l_vector_length_insert = l_insPosSlabInfo->ProductOrientationLength(l_length_insert);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inserted Protocol length after mapping is") + l_vector_length_insert.ToString());

    flt32_t l_pe_enlargement_percent = 0.0f, l_ro_enlargement_percent = 0.0f, l_sl_enlargement_percent = 0.0f;
    GetScanEnlargementPercentage(l_pe_enlargement_percent, l_ro_enlargement_percent, l_sl_enlargement_percent);

    AddScanEnlargementPercentage(&l_vector_length_target, l_pe_enlargement_percent, l_ro_enlargement_percent, l_sl_enlargement_percent);

    CString l_log_str;
    l_log_str.Format(_T("Percentage Enlargement:(%f,%f,%f)"),
                     l_pe_enlargement_percent, l_ro_enlargement_percent, l_sl_enlargement_percent);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Enlarged Target Protocol Length is:") + l_vector_length_target.ToString());
    CalculateFOV(l_vector_length_target, l_vector_length_insert, f_mode, f_calc_parameters);
    l_log_str.Format(_T("Calculated FOV Before Comparing with ScanRangeLimit For AutoInsert Protocol:(%f,%f)"), f_calc_parameters.m_fov.x, f_calc_parameters.m_fov.y);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    flt32_t l_max_Z = 0.0f;
    GetMaxZ(l_vector_length_target.GetZ(), l_vector_length_insert.GetZ(), l_max_Z, f_flag);

    CompareAndGetCalculatedParametersWithLimit(f_calc_parameters, l_insPosSlabInfo, l_max_Z, f_flag);

    FREE_PTR(l_insPosSlabInfo);
    return l_status;
}


//***************************Method Header*************************************
//Method Name    : GetMinMaxForTargetProt
//Author         :
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::GetMinMaxForTargetProt(
    CVarFieldHandle* const f_vf_handle,
    const int f_tarProtocol,
    CVector3D& f_tarMaxPos,
    CVector3D& f_tarMinPos
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetMinMaxForTargetProt");
    PQM_SCOPED_TRACE();

    VfPathElem_t l_tarPath[SVD_ST_SLICEGROUP] = {0};
    VfPathElem_t l_tarVPath[SVD_ST_SLICEGROUP] = {0};
    l_tarPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_tarPath[SVD_ST_PROTOCOL - 1].index        = f_tarProtocol;
    l_tarPath[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_tarPath[SVD_ST_SUBPROTOCOL - 1].index        = 0;
    l_tarPath[SVD_ST_SLICEGROUP - 1].subtree_name  = SVN_ST_SLICEGROUP;
    l_tarPath[SVD_ST_SLICEGROUP - 1].index         = 0;

    l_tarVPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_tarVPath[SVD_ST_PROTOCOL - 1].index        = f_tarProtocol;
    l_tarVPath[SVD_ST_VISUALGROUP - 1].subtree_name = SVN_ST_VISUALGROUP;
    l_tarVPath[SVD_ST_VISUALGROUP - 1].index        = 0;

    const int l_field_spec_count = 10;
    VfFieldSpec_t 	l_fieldSpec[l_field_spec_count] = {0};
    memset(l_fieldSpec, 0, sizeof(VfFieldSpec_t) * l_field_spec_count);

    int32_t j = 0;
    int32_t	  l_tarVisuals = 0;
    ClibmsVfAdapter libmsvfAdapter(f_vf_handle);
    status_t l_status = E_NO_ERROR;
    l_status = libmsvfAdapter.msVfCountSlabs(f_tarProtocol, &l_tarVisuals);

    msPosSlabInfo_t* tarPosSlabInfo = (msPosSlabInfo_t*)malloc(sizeof(msPosSlabInfo_t) * l_tarVisuals);

    if (!tarPosSlabInfo) {
        l_status = E_ERROR;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not allocate memory to tarPosSlabInfo"));
        return l_status;
    }

    CString l_log_str = _T("Printing Target Protocol Information");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    l_log_str.Format(_T("Target Protocol->Number of Slabs : %d"), l_tarVisuals);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    for (int cntV = 0; cntV < l_tarVisuals; cntV++) {
        int i = 0;
        f32vec2_t l_fov;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_FOV, VFT_FLTVEC2,
                     sizeof(l_fov), &l_fov, 0);
        i++;
        int l_numDone    = 0;
        l_status = f_vf_handle->GetFields(l_tarPath, SVD_ST_PROTOCOL, l_fieldSpec, i, &l_numDone);

        tarPosSlabInfo[cntV].plnFov.SetXY(l_fov.x, l_fov.y);
        l_log_str.Format(_T("Target Protocol->Study File FOV : (%f, %f)"), l_fov.x, l_fov.y);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
        memset(l_fieldSpec, 0, sizeof(VfFieldSpec_t) * l_field_spec_count);
        if_error(l_status || l_numDone != i) {
            l_status = E_ERROR;
            FREE_PTR(tarPosSlabInfo);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not retrieve the FOV value for target protocol from study file..!!"));
            return l_status;
        }
        i = 0;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_THICKNESS, VFT_FLT32,
                     sizeof tarPosSlabInfo[cntV].sliceThickness, &tarPosSlabInfo[cntV].sliceThickness, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_SLICE_GAP, VFT_FLT32,
                     sizeof tarPosSlabInfo[cntV].sliceGap, &tarPosSlabInfo[cntV].sliceGap, 0);
        i++;
        l_status = f_vf_handle->GetFields(l_tarPath, SVD_ST_SLICEGROUP, l_fieldSpec, i, &l_numDone);

        l_log_str.Format(_T("Target Protocol->Slice Thickness : %f , Slice Gap : %f"),
                         tarPosSlabInfo[cntV].sliceThickness,
                         tarPosSlabInfo[cntV].sliceGap);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
        if_error(l_status || l_numDone != i) {
            l_status = E_ERROR;
            FREE_PTR(tarPosSlabInfo);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not retrieve the Slice thickness and slice Gap value from study file..!!"));
            return l_status;
        }

        f32vec3_t l_offsetvec, l_slicevec, l_pevec;
        memset(l_fieldSpec, 0, sizeof(VfFieldSpec_t) * l_field_spec_count);
        i = 0;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_VG_OFFSET_VECTOR, VFT_FLTVEC3,
                     sizeof(l_offsetvec), &l_offsetvec, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_VG_SLICE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_slicevec), &l_slicevec, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_VG_PHASE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_pevec), &l_pevec, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_SLICES_IN_VG, VFT_INT32,
                     sizeof tarPosSlabInfo[cntV].sliceNum, &tarPosSlabInfo[cntV].sliceNum, 0);
        i++;

        l_tarVPath[SVD_ST_VISUALGROUP - 1].index = cntV;

        l_status = f_vf_handle->GetFields(l_tarVPath, SVD_ST_VISUALGROUP, l_fieldSpec, i, &l_numDone);
        if_error(l_status || l_numDone != i) {
            l_status = E_ERROR;
            FREE_PTR(tarPosSlabInfo);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not retrieve the Orinetation vectors value from study file..!!"));
            return l_status;
        }
        tarPosSlabInfo[cntV].offsetVec = l_offsetvec;
        tarPosSlabInfo[cntV].sliceVec = l_slicevec;
        tarPosSlabInfo[cntV].peVec = l_pevec;

        CPqmUtilities::calcReadOriVector(&tarPosSlabInfo[cntV].peVec,
                                         &tarPosSlabInfo[cntV].sliceVec,
                                         &tarPosSlabInfo[cntV].roVec);

        msPosSlabInfo_t::makeCheckPointsInfo(&tarPosSlabInfo[cntV]);
        GetMinMaxForEachSlab(&tarPosSlabInfo[cntV], &f_tarMaxPos, &f_tarMinPos);
    }

    return l_status;
}


//***************************Method Header*************************************
//Method Name    : GetMinMaxForInsertedProt
//Author         :
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::GetMinMaxForInsertedProt(
    msPosSlabInfo_t* f_insPosSlabInfo,
    const int f_insertprotVisuals,
    CVarFieldHandle* const f_vf_handle,
    const int f_insertProt,
    CVector3D& f_insMaxPos,
    CVector3D& f_insMinPos
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetMinMaxForInsertedProt");
    PQM_SCOPED_TRACE();

    VfPathElem_t l_insertPath[SVD_ST_SLICEGROUP] = {0};
    VfPathElem_t l_insertVPath[SVD_ST_SLICEGROUP] = {0};
    l_insertPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_insertPath[SVD_ST_PROTOCOL - 1].index        = f_insertProt;
    l_insertPath[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_insertPath[SVD_ST_SUBPROTOCOL - 1].index        = 0;
    l_insertPath[SVD_ST_SLICEGROUP - 1].subtree_name  = SVN_ST_SLICEGROUP;
    l_insertPath[SVD_ST_SLICEGROUP - 1].index         = 0;

    l_insertVPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_insertVPath[SVD_ST_PROTOCOL - 1].index        = f_insertProt;
    l_insertVPath[SVD_ST_VISUALGROUP - 1].subtree_name = SVN_ST_VISUALGROUP;
    l_insertVPath[SVD_ST_VISUALGROUP - 1].index        = 0;

    const int l_field_spec_count = 10;
    VfFieldSpec_t 	l_fieldSpec[l_field_spec_count] = {0};
    memset(l_fieldSpec, 0, sizeof(VfFieldSpec_t) * l_field_spec_count);

    CString l_log_str = _T("Printing AutoInsert Protocol Information");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    l_log_str.Format(_T("AutoInsert Protocol->Number of Slabs : %d"), f_insertprotVisuals);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
    status_t l_status = E_NO_ERROR;

    for (int cntV = 0; cntV < f_insertprotVisuals; cntV++) {
        int i = 0;
        f32vec2_t l_fov;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_FOV, VFT_FLTVEC2,
                     sizeof(l_fov), &l_fov, 0);
        i++;
        int l_numDone = 0;
        l_status = f_vf_handle->GetFields(l_insertPath, SVD_ST_PROTOCOL, l_fieldSpec, i, &l_numDone);
        f_insPosSlabInfo[cntV].plnFov = CVector2D(l_fov.x , l_fov.y);
        l_log_str.Format(_T("AutoInsert Protocol->Study File FOV : (%f, %f)"), l_fov.x, l_fov.y);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
        if_error(l_status || l_numDone != i) {
            l_status = E_ERROR;
            FREE_PTR(f_insPosSlabInfo);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not retrieve FOV value for Inserted Protocol..!!"));
            return l_status;
        }

        memset(l_fieldSpec, 0, sizeof(VfFieldSpec_t) * l_field_spec_count);
        i = 0;

        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_THICKNESS, VFT_FLT32,
                     sizeof f_insPosSlabInfo[cntV].sliceThickness, &f_insPosSlabInfo[cntV].sliceThickness, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_SLICE_GAP, VFT_FLT32,
                     sizeof f_insPosSlabInfo[cntV].sliceGap, &f_insPosSlabInfo[cntV].sliceGap, 0);
        i++;

        l_status = f_vf_handle->GetFields(l_insertPath, SVD_ST_SLICEGROUP, l_fieldSpec, i, &l_numDone);
        l_log_str.Format(_T("AutoInsert Protocol->Slice Thickness : %f , Slice Gap : %f"),
                         f_insPosSlabInfo[cntV].sliceThickness,
                         f_insPosSlabInfo[cntV].sliceGap);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        if_error(l_status || l_numDone != i) {
            l_status = E_ERROR;
            FREE_PTR(f_insPosSlabInfo);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not retrieve Slice Thickness and Slice Gap...!!"));
            return l_status;
        }

        f32vec3_t l_offsetvec, l_slicevec, l_pevec;
        memset(l_fieldSpec, 0, sizeof(VfFieldSpec_t) * l_field_spec_count);
        i = 0;

        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_VG_OFFSET_VECTOR, VFT_FLTVEC3,
                     sizeof(l_offsetvec), &l_offsetvec, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_VG_SLICE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_slicevec), &l_slicevec, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_VG_PHASE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_pevec), &l_pevec, 0);
        i++;
        VFF_SET_ARGS(l_fieldSpec[i], SVN_PLN_SLICES_IN_VG, VFT_INT32,
                     sizeof f_insPosSlabInfo[cntV].sliceNum, &f_insPosSlabInfo[cntV].sliceNum, 0);
        i++;

        l_status = f_vf_handle->GetFields(l_insertVPath, SVD_ST_VISUALGROUP, l_fieldSpec, i, &l_numDone);
        if_error(l_status || l_numDone != i) {
            l_status = E_ERROR;
            FREE_PTR(f_insPosSlabInfo);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not retrieve Orientation vectors for Inserted protocols..!!"));
            return l_status;
        }

        f_insPosSlabInfo[cntV].offsetVec = l_offsetvec;
        f_insPosSlabInfo[cntV].sliceVec = l_slicevec;
        f_insPosSlabInfo[cntV].peVec = l_pevec;

        CPqmUtilities::calcReadOriVector(&f_insPosSlabInfo[cntV].peVec,
                                         &f_insPosSlabInfo[cntV].sliceVec,
                                         &f_insPosSlabInfo[cntV].roVec);

        msPosSlabInfo_t::makeCheckPointsInfo(&f_insPosSlabInfo[cntV]);
        GetMinMaxForEachSlab(&f_insPosSlabInfo[cntV], &f_insMaxPos, &f_insMinPos);
    }

    return l_status;
}

//***************************Method Header*************************************
//Method Name    : AddScanEnlargementPercentage
//Author         :
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::AddScanEnlargementPercentage(
    CVector3D* l_vector_length_target,
    const flt32_t f_pe_enlargement_percent,
    const flt32_t f_ro_enlargement_percent,
    const flt32_t f_sl_enlargement_percent
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::AddScanEnlargementPercentage");
    PQM_SCOPED_TRACE();

    const flt32_t l_max_percent = 100.0f ;

    CVector3D l_pe_ro_sl_vector(f_pe_enlargement_percent, f_ro_enlargement_percent, f_sl_enlargement_percent) ;
    (*l_vector_length_target) = (*l_vector_length_target) + (((*l_vector_length_target) * l_pe_ro_sl_vector) / l_max_percent);
}



//***************************Method Header*************************************
//Method Name    : CalculateSliceGap
//Author         :
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::CalculateSliceGap(
    const msPosSlabInfo_t* const f_insPosSlabInfo,
    const flt32_t f_sl_target_val,
    AutoInsertParameters& f_calc_params, const bool f_is_round_up
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::CalculateSliceGap");
    PQM_SCOPED_TRACE();

    //DOUBT ... why so ?
    if (NUM_1 == f_insPosSlabInfo->sliceNum) {
        return;
    }


    f_calc_params.m_slice_gap = (f_sl_target_val - (f_insPosSlabInfo->sliceNum * f_insPosSlabInfo->sliceThickness)) / (f_insPosSlabInfo->sliceNum - 1);

    CString l_slice_gap_string(_T(""));
    float l_fslice_gap = 0.0f;

    if (f_is_round_up) {
        l_fslice_gap = MR::ACQ::SM::COMMON::MATH::RoundUp(f_calc_params.m_slice_gap, 4);

    } else {
        l_fslice_gap = MR::ACQ::SM::COMMON::MATH::RoundDown(f_calc_params.m_slice_gap, 4);
    }

    l_slice_gap_string.Format(_T("Calculated Slice Gap : %f. RoundUp/Down to %f"),
                              f_calc_params.m_slice_gap, l_fslice_gap);

    f_calc_params.m_slice_gap = l_fslice_gap;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_slice_gap_string);
}

//***************************Method Header*************************************
//Method Name    : CalculateFOV
//Author         :
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::CalculateFOV(
    const CVector3D& l_vector_length_target,
    const CVector3D& l_vector_length_insert,
    const scan_mode f_mode,
    AutoInsertParameters& f_calc_params
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::CalculateFOV");
    PQM_SCOPED_TRACE();

    const CVector3D l_maxXY = Vector3DUtility::GetMaxXY(l_vector_length_target, l_vector_length_insert) ;
    f_calc_params.m_fov.x = l_maxXY.GetX();
    f_calc_params.m_fov.y = l_maxXY.GetY();

    if (SHIM_MODE == f_mode) {
        if (f_calc_params.m_fov.x > f_calc_params.m_fov.y) {
            f_calc_params.m_fov.y = f_calc_params.m_fov.x;

        } else {
            f_calc_params.m_fov.x = f_calc_params.m_fov.y;
        }
    }

    CString l_fov_string(_T(""));
    float l_fov_x = MR::ACQ::SM::COMMON::MATH::RoundUp(f_calc_params.m_fov.x, 3);
    float l_fov_y = MR::ACQ::SM::COMMON::MATH::RoundUp(f_calc_params.m_fov.y, 3);

    l_fov_string.Format(_T("Calculated FOV [0:%f][1:%f]. RoundUp to [0:%f][1:%f]"),
                        f_calc_params.m_fov.x, f_calc_params.m_fov.y, l_fov_x, l_fov_y);

    f_calc_params.m_fov.x = l_fov_x;
    f_calc_params.m_fov.y = l_fov_y;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_fov_string);
}


//***************************Method Header*************************************
//Method Name    : GetScanEnlargementPercentage
//Author         :
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::GetScanEnlargementPercentage(
    flt32_t& f_pe_enlargement_percent,
    flt32_t& f_ro_enlargement_percent,
    flt32_t& f_sl_enlargement_percent
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetScanEnlargementPercentage");
    PQM_SCOPED_TRACE();

    const ShimMapStudy* const l_shimmapstudy_ptr = GetShimMapStudy();

    if (NULL == l_shimmapstudy_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_shim_map_study is NULL"));
        return;
    }

    POSITION l_pos = l_shimmapstudy_ptr->m_scanenlargement_list.GetHeadPosition();
    ScanRegionEnlargement* l_scan_enlargement_region = NULL;

    for (int i = 0; l_pos != NULL; i++) {
        l_scan_enlargement_region = l_shimmapstudy_ptr->m_scanenlargement_list.GetNext(l_pos);

        if (SCAN_ENLARGEMENT_AREA_SL == l_scan_enlargement_region->m_tag) {
            f_sl_enlargement_percent = (flt32_t)l_scan_enlargement_region->m_percentage_range;

        } else if (SCAN_ENLARGEMENT_AREA_PE == l_scan_enlargement_region->m_tag) {
            f_pe_enlargement_percent = (flt32_t)l_scan_enlargement_region->m_percentage_range;

        } else if (SCAN_ENLARGEMENT_AREA_RO == l_scan_enlargement_region->m_tag) {
            f_ro_enlargement_percent = (flt32_t)l_scan_enlargement_region->m_percentage_range;
        }
    }

    CString l_pe_ro_sl__str = _T("");
    l_pe_ro_sl__str.Format(_T("Percentage increment for each direction(pe,ro,sl) is : %f , %f, %f"),
                           f_pe_enlargement_percent, f_ro_enlargement_percent, f_sl_enlargement_percent);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_pe_ro_sl__str);
}

//***************************Method Header*************************************
//Method Name    : ValidateScratchPad
//Author         :
//Purpose        :
//*****************************************************************************
bool CAutoInsertProtConfig::ValidateScratchPad(
    char* f_scratchpad, const int f_insertProt
) const
{

    CLibPLTreeWrapper l_obj(m_pqm_study->GetVfDBHandle()) ;
    return l_obj.ExecutePlUpdateWrapper(m_pqm_study, f_insertProt, m_auto_insert_scratch_pad);
}

//***************************Method Header*************************************
//Method Name    : GetValidatedParameters
//Author         :
//Purpose        :
//*****************************************************************************
status_t CAutoInsertProtConfig::GetValidatedParameters(
    const AutoInsertParameters& f_calc_params,
    AutoInsertParameters& f_validated_params,
    const int f_insertProt,
    const bool f_flag
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetValidatedParameters");
    PQM_SCOPED_TRACE();

    if (E_NO_ERROR != CreateAutoInsertProlimScratchPad(f_insertProt)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in creating AutoInsert Scratchpad..!!"));
        return E_ERROR;
    }

    status_t l_status = (WriteCalculatedParametersToScratchPad(f_calc_params, f_flag)   &&
                         ValidateScratchPad(m_auto_insert_scratch_pad, f_insertProt)          &&
                         GetValidatedParametersFromSctratchPad(f_validated_params, f_flag)
                        ) ? E_NO_ERROR : E_ERROR;

    return l_status;
}

//***************************Method Header*************************************
//Method Name    : GetMinMaxForEachSlab
//Author         :
//Purpose        :
//*****************************************************************************
void CAutoInsertProtConfig::GetMinMaxForEachSlab(
    msPosSlabInfo_t* f_tarPosSlabInfo,
    CVector3D* f_tarMaxPos,
    CVector3D* f_tarMinPos
) const
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtConfig::GetMinMaxForEachSlab");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    for (int i = 0; i < 8; i++) {
        Vector3DUtility::setMaxMinPosition(&f_tarPosSlabInfo->eachPos[i], f_tarMaxPos, f_tarMinPos);
    }
}

//***************************Method Header*************************************
//Method Name    : GetShimMapStudy
//Author         :
//Purpose        :
//*****************************************************************************
inline const ShimMapStudy* const CAutoInsertProtConfig::GetShimMapStudy(
) const
{
    return m_shim_map_study_ptr;
}