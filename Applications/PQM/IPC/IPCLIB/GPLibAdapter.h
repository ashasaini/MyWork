//Redmine-780
//Redmine-782
/**
 * Copyright(c) 2010 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 * @file   GPLibAdapter.h
 *
 * gp library adapters. these access varfield through VarFieldHandle for threadsafe.
 *
 * declaration
 * - AbstractGPLibAdapter
 * - libVfToUiAdapter
 * - libSARAdapter
 * - libmsCalcSizeAdapter
 * - libmsCalcSizeAMBAdapter
 * - libmsVfAdapter
 * - libSpeederAdapter
 * @author K. Tanoue
 */
#ifndef __SM_IPCLIB_GPLIBADAPTER_H_
#define __SM_IPCLIB_GPLIBADAPTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C" {
#include <tami/libvfToUi/vfToUiDef.h>
}
#include <tami/libSAR/sar.h>
#include <tami/libSAR/sarVfParams.h>
#include <tami/libms/msCalcSize.h>
#include <tami/libmsVf/libmsData.h>
#include <tami/libgpdb/speeder.h>
#include <tami/libSpeeder/speederStatus.h>

#include <libStudyManager/NonCopyable.h>

// pre-declare
class CVarFieldHandle;

/**
 * Abstract GPLib Adapter class
 */
class CAbstractGPLibAdapter : private MR::ACQ::SM::COMMON::NonCopyable<CAbstractGPLibAdapter>
{
public:
    explicit CAbstractGPLibAdapter(CVarFieldHandle* const handle);
    virtual ~CAbstractGPLibAdapter();

protected:
    CVarFieldHandle* m_vf_handle;
};

/**
 * libvfToUi Adapter
 */
class ClibVfToUiAdapter : public CAbstractGPLibAdapter
{
public:
    explicit ClibVfToUiAdapter(CVarFieldHandle* const handle);
    ~ClibVfToUiAdapter();

    status_t VuiCopyProtocol(const int vf_src_index, const int vf_dst_index,
                             const int image_group, int* const protocol)const;
    status_t VuiCopyProtocolToFile(const int vf_src_index, const char* const dst_file,
                                   const int vf_dst_index, const int image_group,
                                   int* const protocol)const;
    status_t VuiCopyProtocolFromFile(const int vf_dst_index, const char* const vf_src_file,
                                     const int vf_src_index, const int image_group,
                                     int* const protocol)const;
    status_t VuiCopyProtocolFromScratchPad(const int vf_dst_index, const char* const vf_src_file,
                                           const int vf_src_index, const int image_group,
                                           int* const protocol);
    status_t VuiCopyProtocolsFromFile(const char* const src_file,
                                      const int* const prot_tbl, const int num_prot,
                                      const int image_group,
                                      status_t(*func)(VfDBHandle_t, int, void*), void* data)const;

    status_t VuiGetAllProt(bitfield_t* const mask, int* const branches)const;
    status_t VuiAddToAcqOrder(const int protocol)const;
    status_t VuiSetAcqOrder(const int from_ndx, const int to_ndx)const;
    status_t VuiDeleteProtocol(const int protocol)const;
    status_t VuiGetProtocolImgGr(const int protocol, int* const imgGr, const int numImgGr)const;
    int VuiProtNum(const int index)const;
    int VuiAcqNum(const int index)const;
    status_t VuiGetVfAcqOrder(VuiAcqOrder_t* const l_acq_order)const;

    status_t VuiGetOffsetVector(const int index, f32vec3_t* const vec)const;
    status_t VuiSetOffsetVector(const int index, const f32vec3_t* const vec)const;
    status_t VuiGetSliceVector(const int index, f32vec3_t* const vec)const;
    status_t VuiSetSliceVector(const int index, const f32vec3_t* const vec)const;
    status_t VuiGetPhaseVector(const int index, f32vec3_t* const vec)const;
    status_t VuiSetPhaseVector(const int index, const f32vec3_t* const vec)const;
    status_t VuiShiftOffsetVector(const int protocol, const int mode,
                                  const int z_val, const float offset)const;

    status_t VuiSaturnCoilName(const int protocol_num, char* const coil_name)const;


    status_t VuiCopyProtocolToPrivateProlimScratchPad(const int vf_dst_index, const char* const vf_src_file, const int vf_src_index,
            const int image_group, int* const protocol)const;

    status_t VuiCopyProtocolFromPrivateProlimScratchPad(const int vf_dst_index, const char* const vf_src_file,
            const int vf_src_index, const int image_group, int* const protocol);


};

/**
 * libSAR Adapter
 */
class ClibSARAdapter : public CAbstractGPLibAdapter
{
public:
    explicit ClibSARAdapter(CVarFieldHandle* const handle);
    ~ClibSARAdapter();
    sarStat_t SarGetVfParams(const int32_t protoId, sarVfParams_p const sar_p)const;
    sarStat_t SarGetVfRflParams(const int32_t protoId, sarVfRflParams_p const sarVf_p)const;
    sarStat_t SarPutVfRflParams(const int32_t protoId,
                                const sarVfRflParams_p const sarVf_p)const;
    sarStat_t SarGetVfTGCRFoutRatioHighSAR(const int32_t protoId,
                                           sarVfTGCParams_p const sarVf_p)const;
    sarStat_t SarPutVfTGCRFoutRatioHighSAR(const int32_t protoId,
                                           const sarVfTGCParams_p const sarVf_p)const;
    sarStat_t SarGetVfSepParams(const int32_t protoId, sarVfSepParams_p const sarVf_p)const;
    sarStat_t SarCalcAndPutVf(const int32_t protoId, sarInfo_p const sar_p)const;
    sarStat_t SarControlCalcAndPutVfForAll(const sarControlWaitProtocols_p sarControl_p)const;
    sarStat_t SarGetVfRTSARFixedOpeMode(const int f_protocol_index, int32_t* f_fixed_ope_mode);
    sarStat_t SarGetVfPreScanParam(const int32_t protoId, sarVfPreScanParam_p const PreScanParam_p)const;
};

/**
 * libms calc size funcs Adapter
 */
class ClibmsCalcSizeAdapter : public CAbstractGPLibAdapter
{
public:
    ClibmsCalcSizeAdapter(CVarFieldHandle* const handle, const int32_t protocol_index,
                          const int32_t mode = ACQUIRE_MODE);
    ~ClibmsCalcSizeAdapter();
    status_t msCheckFtMatrix(int32_t* const pe, int32_t* const ro);
    status_t calcTotalSize(uint32_t* const size);
    status_t calcTotalEngineSize(uint32_t* const size);
    status_t calcTotalVapSize(uint32_t* const size);
private:
    const int32_t m_protocol_index;
    const int32_t m_mode;
};

/**
 * libms calc size AMB funcs Adapter
 */
class ClibmsCalcSizeAMBAdapter : public CAbstractGPLibAdapter
{
public:
    ClibmsCalcSizeAMBAdapter(CVarFieldHandle* const handle, const ProtocolTable_t& protocol_table,
                             const int32_t mode = ACQUIRE_MODE);
    ~ClibmsCalcSizeAMBAdapter();
    status_t calcTotalSizeAMB(uint32_t* const size)const;
    status_t calcTotalEngineSizeAMB(uint32_t* const size)const;
private:
    ProtocolTable_t m_protocol_table;
    const int32_t m_mode;
};

/**
 * libmsVf Adapter
 */
class ClibmsVfAdapter : public CAbstractGPLibAdapter
{
public:
    explicit ClibmsVfAdapter(CVarFieldHandle* const handle);
    ~ClibmsVfAdapter();
    status_t msVfCountSlabs(const int32_t protocol_index,
                            int32_t* const totalSlabsP)const;
    status_t msVfReconMode(const int32_t protocolIndex, int* reconMode)const;
};

/**
 * libSpeeder Adapter
 *
 * used libSpeeder func argument does not include VfDBHandle_t.
 * So it is not use VarFieldHandle. No need threadsafe.
 */
class ClibSpeederAdapter : private MR::ACQ::SM::COMMON::NonCopyable<ClibSpeederAdapter>
{
public:
    ClibSpeederAdapter(const CString& study_name, const int protocol_index);
    ~ClibSpeederAdapter();
    spdrStat_t SpeederGetMaxRatioFromVF(int* const Tmax, int* const PEmax, int* const SEmax,
                                        int* const PEexpand, int* const SEexpand)const;

private:
    std::string m_study_name;
    const int m_protocol_index;
};


/**
 * libMathServerAdapter Adapter
 */
class ClibMathServerAdapter : public CAbstractGPLibAdapter
{
public:
    explicit ClibMathServerAdapter(CVarFieldHandle* const handle);
    ~ClibMathServerAdapter();

    status_t msCheckPostProc(const char* studyName, const int protocolIndex) const;
};

/**
 * libmsAdapter Adapter
 */
class ClibmsAdapter : public CAbstractGPLibAdapter
{
public:
    explicit ClibmsAdapter(CVarFieldHandle* const handle);
    ~ClibmsAdapter();

    status_t msCheckB1Intensity(const int32_t protocolIndex);
};

#endif // #ifndef __SM_IPCLIB_GPLIBADAPTER_H_
