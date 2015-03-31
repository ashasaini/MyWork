// AutoInsertProtConfig.h: interface for the CAutoInsertProtConfig class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_PROTCONFIG_H__
#define __SM_IPCLIB_AUTOINSERT_PROTCONFIG_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "PqmAutoInsert.h"
#include "AutoInsertXmlData.h"
#include "AutoInsertConstant.h"
#include "PqmIpcLibConstants.h"

class CPQMStudy ;
class CScanProcedure;
class CVarFieldHandle;
class CVector3D ;

typedef struct msPosSlabInfo_s msPosSlabInfo_t;


class CAutoInsertProtConfig :
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProtConfig>
{
public:
    CAutoInsertProtConfig(CPQMStudy* f_pqm_study, ShimMapStudy* f_shimmapstudy_ptr);
    virtual ~CAutoInsertProtConfig();

    status_t ConfigureAutoInsertedProtocol(CVarFieldHandle* const vf_handle,
                                           const int32_t tarProtocol,
                                           const int32_t insertProt,
                                           const scan_mode f_mode,
                                           const bool f_can_display_message,
                                           bool& f_has_displayed_message);
private :
    CPQMStudy* m_pqm_study;
    ShimMapStudy* m_shim_map_study_ptr;
    char* m_auto_insert_scratch_pad;

    bool CompareCalculatedParameters(const AutoInsertParameters& f_calc_params, const AutoInsertParameters& f_validated_params, const bool f_flag) const;
    status_t CreateAutoInsertProlimScratchPad(const int f_protocol) const;
    status_t UpdateValidatedParametersToStudy(const int f_insert_protocol) const;
    void DeleteAutoInsertScratchPad(char* f_file_name) const;
    bool WriteCalculatedParametersToScratchPad(const AutoInsertParameters& f_calc_paramaters, const bool f_flag) const;
    bool GetValidatedParametersFromSctratchPad(AutoInsertParameters& f_validated_params, const bool f_flag) const;
    void CompareAndGetCalculatedParametersWithLimit(AutoInsertParameters& f_calc_params, const msPosSlabInfo_t* const f_insPosSlab_Info, const flt32_t f_maximum_z, const bool f_slice_flag) const;
    void GetMaxZ(const flt32_t f_vector_length_target_Zval, const flt32_t f_vector_length_insert_Zval, flt32_t& f_max_Zval, bool& f_slice_flag) const;
    status_t GetCalculatedParameters(AutoInsertParameters& f_calc_parameters, const int f_insert_prot, const int f_tarProtocol, CVarFieldHandle* const f_vf_handle, const scan_mode f_mode, bool& f_flag) const;
    status_t GetMinMaxForTargetProt(CVarFieldHandle* const f_vf_handle, const int f_tarProtocol, CVector3D& f_tarMaxPos, CVector3D& f_tarMinPos) const;
    status_t GetMinMaxForInsertedProt(msPosSlabInfo_t* f_insPosSlabInfo, const int f_insertprotVisuals, CVarFieldHandle* const f_vf_handle, const int f_inserProt, CVector3D& f_tarMaxPos, CVector3D& f_tarMinPos) const ;

    void AddScanEnlargementPercentage(CVector3D* l_vector_length_target, const flt32_t f_pe_enlargement_percent, const flt32_t f_ro_enlargement_percent, const flt32_t f_sl_enlargement_percent) const;
    void CalculateSliceGap(const msPosSlabInfo_t* const f_insPosSlabInfo, const flt32_t f_sl_target_val, AutoInsertParameters& f_calc_params, const bool f_is_round_up) const;
    void CalculateFOV(const CVector3D& l_vector_length_target, const CVector3D& l_vector_length_insert, const scan_mode f_mode, AutoInsertParameters& f_calc_params) const;

    void GetScanEnlargementPercentage(flt32_t& f_pe_enlargement_percent, flt32_t& f_ro_enlargement_percent, flt32_t& f_sl_enlargement_percent) const;
    bool ValidateScratchPad(char* f_scratchpad, const int f_insertProt) const;

    status_t GetValidatedParameters(const AutoInsertParameters& l_calc_paramaters, AutoInsertParameters& f_validated_params, const int f_insertProt, const bool f_flag) const;
    void GetMinMaxForEachSlab(msPosSlabInfo_t* tarPosSlabInfo, CVector3D* tarMaxPos, CVector3D* tarMinPos) const;
    const ShimMapStudy* const GetShimMapStudy() const;

};

#endif // #ifndef __SM_IPCLIB_AUTOINSERT_PROTCONFIG_H__
