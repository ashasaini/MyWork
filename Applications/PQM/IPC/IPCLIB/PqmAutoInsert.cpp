// PqmAutoInsert.cpp: implementation of the PqmAutoInsert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmAutoInsert.h"
#include "PqmProtocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AutoInsertParameters::AutoInsertParameters()
{
    m_fov.x = 0.0;
    m_fov.y = 0.0;
    m_scan_offset.x = 0.0;
    m_scan_offset.y = 0.0;
    m_scan_offset.z = 0.0;
    m_slice_gap = -1.0;
}

AutoInsertParameters::~AutoInsertParameters()
{

}

void AutoInsertParameters::SetScanOffset(
    const f32vec3_t& f_scan_offset
)
{
    m_scan_offset = f_scan_offset ;
}

void AutoInsertParameters::SetSliceGap(
    const float f_slice_gap
)
{
    m_slice_gap = f_slice_gap;
}

void AutoInsertParameters::SetFOV(
    const f32vec2_t& f_fov
)
{
    m_fov = f_fov;
}


OriginalParameters::OriginalParameters(
):	m_sar(0),
    m_couch_position(0.0f),
    m_scan_editing_saved(false)
{
    memset(&m_saturn_coil, 0, sizeof(m_saturn_coil));
    memset(&m_scan_offset, 0, sizeof(m_scan_offset));
}

OriginalParameters::~OriginalParameters()
{
}

f32vec3_t OriginalParameters::GetScanOffset(
)const
{
    return m_scan_offset;
}

void OriginalParameters::SetScanOffset(
    const f32vec3_t& f_scan_offset
)
{
    m_scan_offset.x = f_scan_offset.x;
    m_scan_offset.y = f_scan_offset.y;
    m_scan_offset.z = f_scan_offset.z;
}

bool OriginalParameters::GetEditingFlag(
)const
{
    return m_scan_editing_saved;
}


void OriginalParameters::SetEditingFlag(
    const bool f_flag
)
{
    m_scan_editing_saved = f_flag;
}


int32_t OriginalParameters::GetAnatomy(
)const
{
    return m_sar;
}

void OriginalParameters::SetAnatomy(
    const int32_t f_anatomy
)
{
    m_sar = f_anatomy;
}


AutoInsertProtocol::AutoInsertProtocol(
) : m_target_protocol(NULL),
    m_map_autoinsert_protocol(NULL),
    m_shim_autoinsert_protocol(NULL),
    m_map_insertion_counter(0),
    m_shim_insertion_counter(0)
{
    m_original_params = new OriginalParameters;
}

AutoInsertProtocol::~AutoInsertProtocol()
{
    DEL_PTR(m_original_params);
}
void AutoInsertProtocol::IncrementMapInsertionCounter(const int f_counter)
{
    m_map_insertion_counter = m_map_insertion_counter + f_counter;
}

int AutoInsertProtocol::GetMapInsertionCounter()const
{
    return m_map_insertion_counter;
}

void AutoInsertProtocol::IncrementShimInsertionCounter(const int f_counter)
{
    m_shim_insertion_counter = m_shim_insertion_counter + f_counter;
}

int AutoInsertProtocol::GetShimInsertionCounter()const
{
    return m_shim_insertion_counter;
}
