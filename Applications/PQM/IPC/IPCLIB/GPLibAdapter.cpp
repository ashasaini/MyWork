//Redmine-780
/**
 * Copyright(c) 2010 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 * @file   GPLibAdapter.cpp
 *
 * implementation
 * - AbstractGPLibAdapter
 * - libVfToUiAdapter
 * - libSARAdapter
 * - libmsCalcSizeAdapter
 * - libmsCalcSizeAMBAdapter
 * - libmsVfAdapter
 * - libSpeederAdapter
 *
 * @author K. Tanoue
 */
#include "stdafx.h"
#include "GPLibAdapter.h"
#include <tami/libms/msFtMatrix.h>
extern "C" {
#include <tami/libvfToUi/vfToUiproto.h>
#include <tami/libSpeeder/speederProc.h>
#include <tami/libmathServer/msCheckPostProc.h>
#include <tami/msAP/msAP.h>
#include <tami/libms/msCheckB1Intensity.h>
}
#include <libStudyManager/strcnv.h>
#include <PQM/PQMLogMgr.h>
#include "VarFieldHandle.h"

extern "C" {
    status_t VuiDeleteProtocol(VfDBHandle_t, int); // FIXME tami/libvfToUi/vfToUiProto.h is incorrect !
    status_t VuiCopyProtocolFromScratchPad(VfDBHandle_t, int, char*, int, int, int*);
}

using namespace MR::ACQ::SM::COMMON;

#define SCOPED_LOCK_AND_TRACE(log) CVFHandleScopedLockAndLog logobj(this->m_vf_handle, log)

/**
 * Constructor
 * @params handle [in]
 */
CAbstractGPLibAdapter::CAbstractGPLibAdapter(CVarFieldHandle* const handle)
    : m_vf_handle(handle)
{
    LPCTSTR FUNC_NAME = _T("CAbstractGPLibAdapter::CAbstractGPLibAdapter");

    if (!m_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[ERROR] vf handle is NULL."));
    }
}

/**
 * Destructor
 */
CAbstractGPLibAdapter::~CAbstractGPLibAdapter()
{
    m_vf_handle = NULL;
}

/**
 * Constructor
 * @params handle [in]
 */
ClibVfToUiAdapter::ClibVfToUiAdapter(CVarFieldHandle* const handle)
    : CAbstractGPLibAdapter(handle)
{
}

/**
 * Destructor
 */
ClibVfToUiAdapter::~ClibVfToUiAdapter()
{
}

/**
 * Wrapper for VuiCopyProtocol
 */
status_t ClibVfToUiAdapter::VuiCopyProtocol(const int vf_src_index, const int vf_dst_index,
        const int image_group, int* const protocol)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocol");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiCopyProtocol(m_vf_handle->Get(), vf_src_index, vf_dst_index,
                                 image_group, protocol);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiCopyProtocolToFile
 */
status_t ClibVfToUiAdapter::VuiCopyProtocolToFile(const int vf_src_index, const char* const dst_file,
        const int vf_dst_index, const int image_group,
        int* const protocol)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocolToFile");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    status_t l_result = -1;
    int l_retry_cnt = 0;
    CString l_log_msg = _T("");

    try {
        while (l_result != E_NO_ERROR && l_retry_cnt < 5) {
            l_log_msg.Format(_T("[Retry Count : %d]"), l_retry_cnt);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
            l_result = ::VuiCopyProtocolToFile(m_vf_handle->Get(), vf_src_index, const_cast<char*>(dst_file),
                                               vf_dst_index, image_group, protocol);
            l_retry_cnt++;
            Sleep(500);
        }

        return l_result;

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiCopyProtocolFromFile
 * @date Patni-DKH/2011Jan24/Added/V1.35/MVC009566-Part1
 */
status_t ClibVfToUiAdapter::VuiCopyProtocolFromFile(const int vf_dst_index, const char* const vf_src_file,
        const int vf_src_index, const int image_group,
        int* const protocol)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocolFromFile");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    status_t reopen_status = m_vf_handle->ReOpen();

    if (reopen_status != E_NO_ERROR) {
        return reopen_status;
    }

    status_t copy_status = E_ERROR;

    try {
        copy_status = ::VuiCopyProtocolFromFile(m_vf_handle->Get(), vf_dst_index, const_cast<char*>(vf_src_file),
                                                vf_src_index, image_group, protocol);

        if (copy_status != E_NO_ERROR) {
            m_vf_handle->Log(FUNC_NAME, _T("VuiCopyProtocolFromFile failed"));
        }

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        m_vf_handle->ReOpen();
        throw;
    }

    reopen_status = m_vf_handle->ReOpen();
    return (copy_status != E_NO_ERROR) ? copy_status : reopen_status;
}

/**
 * Wrapper for VuiCopyProtocolFromScratchPad(
 */
status_t ClibVfToUiAdapter::VuiCopyProtocolFromScratchPad(const int vf_dst_index, const char* const vf_src_file,
        const int vf_src_index, const int image_group,
        int* const protocol)
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocolFromScratchPad()");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    status_t reopen_status = m_vf_handle->ReOpen();

    if (reopen_status != E_NO_ERROR) {
        return reopen_status;
    }

    status_t copy_status = E_ERROR;

    try {
        copy_status = ::VuiCopyProtocolFromScratchPad(m_vf_handle->Get(), vf_dst_index, const_cast<char*>(vf_src_file),
                      vf_src_index, image_group, protocol);

        if (copy_status != E_NO_ERROR) {
            m_vf_handle->Log(FUNC_NAME, _T("VuiCopyProtocolFromScratchPad failed"));
        }

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        m_vf_handle->ReOpen();
        throw;
    }

    reopen_status = m_vf_handle->ReOpen();
    return (copy_status != E_NO_ERROR) ? copy_status : reopen_status;
}

/**
 * Wrapper for VuiCopyProtocolsFromFile
 */
status_t ClibVfToUiAdapter::VuiCopyProtocolsFromFile(const char* const src_file,
        const int* const prot_tbl, const int num_prot,
        const int image_group,
        status_t(*func)(VfDBHandle_t, int, void*), void* data)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocolsFromFile");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiCopyProtocolsFromFile(m_vf_handle->Get(), const_cast<char*>(src_file),
                                          const_cast<int*>(prot_tbl), num_prot, image_group, func, data);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiGetAllProt
 */
status_t ClibVfToUiAdapter::VuiGetAllProt(bitfield_t* const mask, int* const branches)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiGetAllProt");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiGetAllProt(m_vf_handle->Get(), mask, branches);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiAddToAcqOrder
 */
status_t ClibVfToUiAdapter::VuiAddToAcqOrder(const int protocol)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiAddToAcqOrder");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiAddToAcqOrder(m_vf_handle->Get(), protocol);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiSetAcqOrder
 */
status_t ClibVfToUiAdapter::VuiSetAcqOrder(const int from_ndx, const int to_ndx)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiSetAcqOrder");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiSetAcqOrder(m_vf_handle->Get(), from_ndx, to_ndx);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiDeleteProtocol
 */
status_t ClibVfToUiAdapter::VuiDeleteProtocol(const int protocol)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiDeleteProtocol");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiDeleteProtocol(m_vf_handle->Get(), protocol);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiGetProtocolImgGr
 */
status_t ClibVfToUiAdapter::VuiGetProtocolImgGr(const int protocol, int* const imgGr,
        const int numImgGr)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiGetProtocolImgGr");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiGetProtocolImgGr(m_vf_handle->Get(), protocol, imgGr, numImgGr);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiProtNum
 */
status_t ClibVfToUiAdapter::VuiProtNum(const int index)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiProtNum");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiProtNum(m_vf_handle->Get(), index);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiAcqNum
 */
status_t ClibVfToUiAdapter::VuiAcqNum(const int index)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiAcqNum");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiAcqNum(m_vf_handle->Get(), index);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiGetVfAcqOrder
 */
status_t ClibVfToUiAdapter::VuiGetVfAcqOrder(VuiAcqOrder_t* const l_acq_order)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiGetVfAcqOrder");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiGetVfAcqOrder(m_vf_handle->Get(), l_acq_order);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiGetOffsetVector
 */
status_t ClibVfToUiAdapter::VuiGetOffsetVector(const int index, f32vec3_t* const vec)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiGetOffsetVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiGetOffsetVector(m_vf_handle->Get(), index, vec);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiSetOffsetVector
 */
status_t ClibVfToUiAdapter::VuiSetOffsetVector(const int index, const f32vec3_t* const vec)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiSetOffsetVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiSetOffsetVector(m_vf_handle->Get(), index, const_cast<f32vec3_t*>(vec));

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiGetSliceVector
 */
status_t ClibVfToUiAdapter::VuiGetSliceVector(const int index, f32vec3_t* const vec)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiGetSliceVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiGetSliceVector(m_vf_handle->Get(), index, vec);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiSetSliceVector
 */
status_t ClibVfToUiAdapter::VuiSetSliceVector(const int index, const f32vec3_t* const vec)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiSetSliceVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiSetSliceVector(m_vf_handle->Get(), index, const_cast<f32vec3_t*>(vec));

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiGetPhaseVector
 */
status_t ClibVfToUiAdapter::VuiGetPhaseVector(const int index, f32vec3_t* const vec)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiGetPhaseVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiGetPhaseVector(m_vf_handle->Get(), index, vec);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiSetPhaseVector
 */
status_t ClibVfToUiAdapter::VuiSetPhaseVector(const int index, const f32vec3_t* const vec)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiSetPhaseVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiSetPhaseVector(m_vf_handle->Get(), index, const_cast<f32vec3_t*>(vec));

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiShiftOffsetVector
 */
status_t ClibVfToUiAdapter::VuiShiftOffsetVector(const int protocol, const int mode,
        const int z_val, const float offset)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiShiftOffsetVector");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiShiftOffsetVector(m_vf_handle->Get(), protocol, mode, z_val, offset);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for VuiSaturnCoilName
 */
status_t ClibVfToUiAdapter::VuiSaturnCoilName(const int protocol_num, char* const coil_name)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiSaturnCoilName");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VuiSaturnCoilName(protocol_num, coil_name, m_vf_handle->Get());

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Constructor
 * @params handle [in]
 */
ClibSARAdapter::ClibSARAdapter(CVarFieldHandle* const handle)
    : CAbstractGPLibAdapter(handle)
{
}

/**
 * Destructor
 */
ClibSARAdapter::~ClibSARAdapter()
{
}

/**
 * Wrapper for SarGetVfParams
 */
sarStat_t ClibSARAdapter::SarGetVfParams(const int32_t protoId,
        sarVfParams_p const sar_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarGetVfParams");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarGetVfParams(m_vf_handle->Get(), protoId, sar_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for SarGetVfRflParams
 */
sarStat_t ClibSARAdapter::SarGetVfRflParams(const int32_t protoId,
        sarVfRflParams_p const sarVf_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarGetVfRflParams");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarGetVfRflParams(m_vf_handle->Get(), protoId, sarVf_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for SarPutVfRflParams
 */
sarStat_t ClibSARAdapter::SarPutVfRflParams(const int32_t protoId,
        const sarVfRflParams_p const sarVf_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarPutVfRflParams");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarPutVfRflParams(m_vf_handle->Get(), protoId,
                                   const_cast<sarVfRflParams_p>(sarVf_p));

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for SarGetVfTGCRFoutRatioHighSAR
 */
sarStat_t ClibSARAdapter::SarGetVfTGCRFoutRatioHighSAR(const int32_t protoId,
        sarVfTGCParams_p const sarVf_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarGetVfTGCRFoutRatioHighSAR");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarGetVfTGCRFoutRatioHighSAR(m_vf_handle->Get(), protoId, sarVf_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for SarPutVfTGCRFoutRatioHighSAR
 */
sarStat_t ClibSARAdapter::SarPutVfTGCRFoutRatioHighSAR(const int32_t protoId,
        const sarVfTGCParams_p const sarVf_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarPutVfTGCRFoutRatioHighSAR");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarPutVfTGCRFoutRatioHighSAR(m_vf_handle->Get(), protoId,
                                              const_cast<sarVfTGCParams_p>(sarVf_p));

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for SarGetVfSepParams
 */
sarStat_t ClibSARAdapter::SarGetVfSepParams(const int32_t protoId,
        sarVfSepParams_p const sarVf_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarGetVfSepParams");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarGetVfSepParams(m_vf_handle->Get(), protoId, sarVf_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for SarCalcAndPutVf
 */
sarStat_t ClibSARAdapter::SarCalcAndPutVf(const int32_t protoId, sarInfo_p const sar_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarCalcAndPutVf");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarCalcAndPutVf(m_vf_handle->Get(), protoId, sar_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}
/**
 * Wrapper for SarControlCalcAndPutVfForAll
 */
sarStat_t ClibSARAdapter::SarControlCalcAndPutVfForAll(const sarControlWaitProtocols_p sarControl_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarControlCalcAndPutVfForAll");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarControlCalcAndPutVfForAll(m_vf_handle->Get(), sarControl_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}
/**
 * Wrapper for SarGetVfRTSARFixedOpeMode
 */
sarStat_t ClibSARAdapter::SarGetVfRTSARFixedOpeMode(const int f_protocol_index, int32_t* f_fixed_ope_mode)
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarGetVfRTSARFixedOpeMode");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarGetVfRTSARFixedOpeMode(m_vf_handle->Get(), f_protocol_index, f_fixed_ope_mode);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}
/**
 * Wrapper for SarGetVfPreScanParam
 */
sarStat_t ClibSARAdapter::SarGetVfPreScanParam(const int32_t protoId, sarVfPreScanParam_p const PreScanParam_p)const
{
    LPCTSTR FUNC_NAME = _T("ClibSARAdapter::SarGetVfPreScanParam");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::SarGetVfPreScanParam(m_vf_handle->Get(), protoId, PreScanParam_p);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Constructor
 * @params study_file_name [in]
 * @param protocol_index [in]
 * @param mode [in]
 */
ClibmsCalcSizeAdapter::ClibmsCalcSizeAdapter(CVarFieldHandle* const handle,
        const int32_t protocol_index, const int32_t mode)
    : CAbstractGPLibAdapter(handle)
    , m_protocol_index(protocol_index)
    , m_mode(mode)
{
}

/**
 * Destructor
 */
ClibmsCalcSizeAdapter::~ClibmsCalcSizeAdapter()
{
}

/**
 * Wrapper for msCheckFtMatrix
 */
status_t ClibmsCalcSizeAdapter::msCheckFtMatrix(int32_t* const pe, int32_t* const ro)
{
    LPCTSTR FUNC_NAME = _T("ClibmsCalcSizeAdapter::msCheckFtMatrix");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::msCheckFtMatrix(const_cast<char*>(m_vf_handle->GetName().c_str()),
                                 m_protocol_index, pe, ro);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for calcTotalSize
 */
status_t ClibmsCalcSizeAdapter::calcTotalSize(uint32_t* const size)
{
    LPCTSTR FUNC_NAME = _T("ClibmsCalcSizeAdapter::calcTotalSize");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::calcTotalSize(const_cast<char*>(m_vf_handle->GetName().c_str()),
                               m_protocol_index, m_mode, size);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for calcTotalEngineSize
 */
status_t ClibmsCalcSizeAdapter::calcTotalEngineSize(uint32_t* const size)
{
    LPCTSTR FUNC_NAME = _T("ClibmsCalcSizeAdapter::calcTotalEngineSize");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::calcTotalEngineSize(const_cast<char*>(m_vf_handle->GetName().c_str()),
                                     m_protocol_index, m_mode, size);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for calcTotalVapSize
 */
status_t ClibmsCalcSizeAdapter::calcTotalVapSize(uint32_t* const size)
{
    LPCTSTR FUNC_NAME = _T("ClibmsCalcSizeAdapter::calcTotalVapSize");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::calcTotalVapSize(const_cast<char*>(m_vf_handle->GetName().c_str()),
                                  m_protocol_index, m_mode, size);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Constructor
 * @params study_file_name [in]
 * @param protocol_index [in]
 * @param mode [in]
 */
ClibmsCalcSizeAMBAdapter::ClibmsCalcSizeAMBAdapter(CVarFieldHandle* const handle,
        const ProtocolTable_t& protocol_table,
        const int32_t mode)
    : CAbstractGPLibAdapter(handle)
    , m_mode(mode)
{
    memcpy(&m_protocol_table, &protocol_table, sizeof(m_protocol_table));
}

/**
 * Destructor
 */
ClibmsCalcSizeAMBAdapter::~ClibmsCalcSizeAMBAdapter()
{
}

/**
 * Wrapper for calcTotalSizeAMB
 */
status_t ClibmsCalcSizeAMBAdapter::calcTotalSizeAMB(uint32_t* const size)const
{
    LPCTSTR FUNC_NAME = _T("ClibmsCalcSizeAMBAdapter::calcTotalSizeAMB");

    try {
        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::calcTotalSizeAMB(const_cast<char*>(m_vf_handle->GetName().c_str()),
                                  m_protocol_table, m_mode, size);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for calcTotalEngineSizeAMB
 */
status_t ClibmsCalcSizeAMBAdapter::calcTotalEngineSizeAMB(uint32_t* const size)const
{
    LPCTSTR FUNC_NAME = _T("ClibmsCalcSizeAMBAdapter::calcTotalEngineSizeAMB");

    try {
        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::calcTotalEngineSizeAMB(const_cast<char*>(m_vf_handle->GetName().c_str()),
                                        m_protocol_table, m_mode, size);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Constructor
 * @params handle [in]
 */
ClibmsVfAdapter::ClibmsVfAdapter(CVarFieldHandle* const handle)
    : CAbstractGPLibAdapter(handle)
{
}

/**
 * Destructor
 */
ClibmsVfAdapter::~ClibmsVfAdapter()
{
}

/**
 * Wrapper for msVfCountSlabs
 */
status_t ClibmsVfAdapter::msVfCountSlabs(const int32_t protocol_index,
        int32_t* const totalSlabsP)const
{
    LPCTSTR FUNC_NAME = _T("ClibmsVfAdapter::msVfCountSlabs");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        VgSgMap_t* nVgSgMapP = NULL;

        status_t sts = ::msVfCountSlabs(m_vf_handle->Get(), protocol_index,
                                        &nVgSgMapP, totalSlabsP);
        FREE_PTR(nVgSgMapP);

        return sts;

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Wrapper for msVfReconMode
 */
status_t ClibmsVfAdapter::msVfReconMode(const int32_t protocolIndex, int* reconMode)const
{
    LPCTSTR FUNC_NAME = _T("ClibmsVfAdapter::msVfReconMode");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::msVfReconMode(m_vf_handle->Get(), protocolIndex, reconMode);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Constructor
 * @params study_file_name [in]
 * @param protocol_index [in]
 */
ClibSpeederAdapter::ClibSpeederAdapter(const CString& study_name, const int protocol_index)
    : m_study_name("")
    , m_protocol_index(protocol_index)
{
    UTIL::Wcs2Mbs(&m_study_name, study_name);
}

/**
 * Destructor
 */
ClibSpeederAdapter::~ClibSpeederAdapter()
{
}

/**
 * Wrapper for SpeederGetMaxRatioFromVF
 */
spdrStat_t ClibSpeederAdapter::SpeederGetMaxRatioFromVF(int* const Tmax,
        int* const PEmax, int* const SEmax,
        int* const PEexpand, int* const SEexpand)const
{
    LPCTSTR FUNC_NAME = _T("ClibSpeederAdapter::SpeederGetMaxRatioFromVF");

    try {
        return ::SpeederGetMaxRatioFromVF(const_cast<char*>(m_study_name.c_str()),
                                          m_protocol_index, Tmax, PEmax, SEmax,
                                          PEexpand, SEexpand);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}


/**
 * Constructor
 * @params handle [in]
 */
ClibMathServerAdapter::ClibMathServerAdapter(CVarFieldHandle* const handle)
    : CAbstractGPLibAdapter(handle)
{
}

/**
 * Destructor
 */
ClibMathServerAdapter::~ClibMathServerAdapter(
)
{

}

/**
 * Wrapper for msCheckPostProc
 */
status_t ClibMathServerAdapter::msCheckPostProc(
    const char* studyName,
    const int protocolIndex
) const
{

    LPCTSTR FUNC_NAME = _T("ClibMathServerAdapter::msCheckPostProc");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {

        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::msCheckPostProc(const_cast<char*>(studyName), protocolIndex);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}


/**
 * Constructor
 */
ClibmsAdapter::ClibmsAdapter(CVarFieldHandle* const handle)
    : CAbstractGPLibAdapter(handle)
{
}

/**
 * Destructor
 */
ClibmsAdapter::~ClibmsAdapter(
)
{

}
/**
 * Wrapper for msCheckB1Intensity
 */
status_t ClibmsAdapter::msCheckB1Intensity(
    const	int32_t protocolIndex
)
{

    LPCTSTR FUNC_NAME = _T("ClibmsAdapter::msCheckB1Intensity");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {

        status_t sts = m_vf_handle->ReOpen();

        if (E_NO_ERROR != sts) {
            m_vf_handle->Log(FUNC_NAME, _T("ReOpen failed"));

            return sts;
        }

        return ::msCheckB1Intensity(m_vf_handle->Get(), protocolIndex);

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}


//****************************Method Header************************************

//Method Name   :VuiCopyProtocolToPrivateProlimScratchPad()

//Author        :PATNI/SP

//Purpose       :Wrapper class for Writing into Scratchpadfile from study file in case private prolim

//*****************************************************************************
status_t ClibVfToUiAdapter::VuiCopyProtocolToPrivateProlimScratchPad(const int vf_src_index, const char* const dst_file,
        const int vf_dst_index, const int image_group,
        int* const protocol)const
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocolToPrivateProlimScratchPad");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    status_t l_result = -1;
    int l_retry_cnt = 0;
    CString l_log_msg = _T("");

    try {
        while (l_result != E_NO_ERROR && l_retry_cnt < 5) {
            l_log_msg.Format(_T("[Retry Count : %d]"), l_retry_cnt);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
            l_result = ::VuiCopyProtocolToPrivateProlimScratchPad(m_vf_handle->Get(), vf_src_index, const_cast<char*>(dst_file),
                       vf_dst_index, image_group, protocol);
            l_retry_cnt++;
            Sleep(500);
        }

        return l_result;

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

//****************************Method Header************************************

//Method Name   :VuiCopyProtocolFromPrivateProlimScratchPad()

//Author        :PATNI/SP

//Purpose       :Wrapper class for reading from Scratchpadfile in case of private prolim

//*****************************************************************************
status_t ClibVfToUiAdapter::VuiCopyProtocolFromPrivateProlimScratchPad(const int vf_dst_index, const char* const vf_src_file,
        const int vf_src_index, const int image_group,
        int* const protocol)
{
    LPCTSTR FUNC_NAME = _T("ClibVfToUiAdapter::VuiCopyProtocolFromPrivateProlimScratchPad()");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    status_t reopen_status = m_vf_handle->ReOpen();

    if (reopen_status != E_NO_ERROR) {
        return reopen_status;
    }

    status_t copy_status = E_ERROR;

    try {
        copy_status = ::VuiCopyProtocolFromPrivateProlimScratchPad(m_vf_handle->Get(), vf_dst_index, const_cast<char*>(vf_src_file),
                      vf_src_index, image_group, protocol);

        if (copy_status != E_NO_ERROR) {
            m_vf_handle->Log(FUNC_NAME, _T("VuiCopyProtocolFromPrivateProlimScratchPad failed"));
        }

    } catch (...) {
        m_vf_handle->Log(FUNC_NAME, _T("EXCEPTION"));
        m_vf_handle->ReOpen();
        throw;
    }

    reopen_status = m_vf_handle->ReOpen();
    return (copy_status != E_NO_ERROR) ? copy_status : reopen_status;
}

