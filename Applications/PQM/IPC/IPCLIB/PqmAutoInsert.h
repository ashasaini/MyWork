// PqmAutoInsert.h: interface for the PqmAutoInsert class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMAUTOINSERT_H__
#define __SM_IPCLIB_PQMAUTOINSERT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PQM/CoilDefinitions.h>
#include "Vector3D.h"
class CPqmProtocol;

struct AutoInsertParameters {
    f32vec2_t m_fov;
    float m_slice_gap;
    f32vec3_t m_scan_offset;

    AutoInsertParameters();
    ~AutoInsertParameters();

    void SetScanOffset(const f32vec3_t& f_scan_offset);
    void SetSliceGap(const float f_slice_gap);
    void SetFOV(const f32vec2_t& f_fov);

};

struct OriginalParameters {
    PqmSaturnCoil_t m_saturn_coil;
    int32_t m_sar;
    float m_couch_position;
    f32vec3_t m_scan_offset;
    bool m_scan_editing_saved;

    OriginalParameters();
    ~OriginalParameters();

    f32vec3_t GetScanOffset()const;
    void SetScanOffset(const f32vec3_t& f_scan_offset);

    bool GetEditingFlag()const;
    void SetEditingFlag(const bool f_flag);

    int32_t GetAnatomy()const;
    void SetAnatomy(const int32_t f_anatomy);
};

struct AutoInsertProtocol {
    CPqmProtocol* m_target_protocol;
    CPqmProtocol* m_map_autoinsert_protocol;
    CPqmProtocol* m_shim_autoinsert_protocol;
    OriginalParameters* m_original_params;
    int m_map_insertion_counter;
    int m_shim_insertion_counter;

    AutoInsertProtocol();
    ~AutoInsertProtocol();
    void IncrementMapInsertionCounter(const int f_counter);
    int GetMapInsertionCounter()const;

    void IncrementShimInsertionCounter(const int f_counter);
    int GetShimInsertionCounter()const;
};

#endif // #ifndef __SM_IPCLIB_PQMAUTOINSERT_H__ 
