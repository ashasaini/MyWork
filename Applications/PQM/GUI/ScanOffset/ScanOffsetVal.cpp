// ScanOffsetVal.cpp: implementation of the CScanOffsetVal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanOffsetVal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CString SCAN_OFFSET_FORMAT_SPEC = L"%4.1f";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ScanOffset::ScanOffset()
{
    for (int i = 0; i < ScanOffset_Total ; i++) {
        m_offset[i] = 0.f;
    }
}

ScanOffset::ScanOffset(
    const float f_x,
    const float f_y,
    const float f_z
)
{
    m_offset[0] = f_x;
    m_offset[1] = f_y;
    m_offset[2] = f_z;
}

float& ScanOffset::operator [](const ScanOffsetEnum f_scanoffsetenum)
{

    return m_offset[f_scanoffsetenum];
}

const float ScanOffset::operator [](const ScanOffsetEnum f_scanoffsetenum) const
{

    return m_offset[f_scanoffsetenum];
}


CString ScanOffset::GetLogString() const
{

    CString str;
    str.Format(_T("(%1.1f, %1.1f, %1.1f)"), m_offset[0], m_offset[1], m_offset[2]);
    return str;
}

CScanOffsetVal::CScanOffsetVal()
{
}

CScanOffsetVal::~CScanOffsetVal()
{

}

void CScanOffsetVal::SetScanOffsetData(
    const ScanOffset& f_scan_offset,
    const ScanOffset& f_min_offset,
    const ScanOffset& f_max_offset
)
{
    m_offset = f_scan_offset;
    m_offset_min = f_min_offset;
    m_offset_max = f_max_offset;

#define VALIDATE_SCAN_OFFSET(current_scanoffset_enum)	ValidateValue(								\
        m_offset[current_scanoffset_enum],			\
        m_offset[current_scanoffset_enum],			\
        m_offset_min[current_scanoffset_enum],		\
        m_offset_max[current_scanoffset_enum],		\
        m_offset[current_scanoffset_enum],			\
        NULL, false);								\
 
    VALIDATE_SCAN_OFFSET(ScanOffsetX)
    VALIDATE_SCAN_OFFSET(ScanOffsetY)
    VALIDATE_SCAN_OFFSET(ScanOffsetZ)

#undef VALIDATE_SCAN_OFFSET

}


void CScanOffsetVal::UpdateMinMax(
    const ScanOffset& f_min_offset,
    const ScanOffset& f_max_offset
)
{
    m_offset_min = f_min_offset;
    m_offset_max = f_max_offset;
}

void CScanOffsetVal::UpdateScanOffset(
    const float f_x,
    const float f_y,
    const float f_z
)
{
    m_offset[ScanOffsetX] = f_x;
    m_offset[ScanOffsetY] = f_y;
    m_offset[ScanOffsetZ] = f_z;
}


bool CScanOffsetVal::Increment(
    const ScanOffsetEnum f_scanoffsetenum,
    const float f_increment /*= 1*/
)
{

    return IncrementAndValidate(f_scanoffsetenum, f_increment);
}

bool CScanOffsetVal::UpdateValue(
    const ScanOffsetEnum f_scanoffsetenum,
    const CString& f_value
)
{

    switch (f_scanoffsetenum) {
        case ScanOffsetX:
        case ScanOffsetY:
        case ScanOffsetZ:
            break;

        default:
            return false;
    }

    if (!IsValidValue(f_value)) {
        return false;
    }

    float& l_offsetvalue_to_update = m_offset[f_scanoffsetenum] ;
    float f_valueinrange = l_offsetvalue_to_update;
    bool f_isvalue_validated = false;

    if (!ValidateValue(l_offsetvalue_to_update, (float)ConvertStringToDouble(f_value), m_offset_min[f_scanoffsetenum], m_offset_max[f_scanoffsetenum], f_valueinrange, &f_isvalue_validated) || f_isvalue_validated) {
        return false;
    }

    l_offsetvalue_to_update = f_valueinrange;

    UpdateValue(f_scanoffsetenum, (float)ConvertStringToDouble(f_value));
    return true;

}

void CScanOffsetVal::UpdateValue(
    const ScanOffsetEnum f_scanoffsetenum,
    const float f_new_value
)
{
    m_offset[f_scanoffsetenum] = f_new_value;
}




const CString CScanOffsetVal::GetString(
    const ScanOffsetEnum f_scanoffsetenum
) const
{

    CString str = _T("");

    float f_param_val = 0.f;

    if (GetParamValue(f_scanoffsetenum, f_param_val)) {

        str.Format(SCAN_OFFSET_FORMAT_SPEC, f_param_val);
        str.TrimLeft();
        str.TrimRight();
    }

    return str;
}

const ScanOffset& CScanOffsetVal::GetMinOffset(
) const
{
    return m_offset_min;
}

const ScanOffset& CScanOffsetVal::GetMaxOffset(
) const
{
    return m_offset_max;
}


const ScanOffset& CScanOffsetVal::GetOffset(
) const
{

    return m_offset;
}



double CScanOffsetVal::ConvertStringToDouble(
    const CString& str
) const
{
    CString extract_string = _T("");
    wchar_t* extract_string_buffer = extract_string.GetBuffer(0);

    double value_retrived = wcstod(str, &extract_string_buffer);

    extract_string = extract_string_buffer;
    extract_string.ReleaseBuffer();

    return value_retrived;
}


bool CScanOffsetVal::IsValidValue(
    CString f_value
) const
{

    if (f_value.Find('.') == -1) {
        f_value += _T(".0");
    }

    f_value = f_value.Left(f_value.Find('.') + 2);

    const int len = f_value.GetLength();

    if (len == 0) {
        return false;
    }

    int count_dot = 0;
    int pos_dot = 0;
    int count_minus = 0;
    int pos_minus = 0;

    for (int i = 0; i < len; i++) {

        if (f_value[i] == '-') {

            pos_minus = i;

            if ((len == 1) ||       //Length is one & first char is '-'
                (pos_minus > 0) ||  //Position of '-' not at zeroth
                (++count_minus > 1) //Number of '-' chars more than one
               ) {

                return false;
            }

        } else if (f_value[i] == '.') {

            pos_dot = i;

            if ((len == 1) ||        //Length is one & first char is '.'
                //   (pos_dot> valid_dot_pos) ||      //Position of '.' is next to 2 nd
                (++count_dot > 1)) { //Number of '.' chars more than one

                return false;
            }
        }
    }

    return true;
}


bool CScanOffsetVal::IncrementAndValidate(
    const ScanOffsetEnum f_scanoffsetenum,
    const float f_increment
)
{

    switch (f_scanoffsetenum) {

        case ScanOffsetX:
        case ScanOffsetY:
        case ScanOffsetZ:
            break;

        default:
            return false;
    }

    return ValidateValue(m_offset[f_scanoffsetenum], m_offset[f_scanoffsetenum] + f_increment, m_offset_min[f_scanoffsetenum], m_offset_max[f_scanoffsetenum], m_offset[f_scanoffsetenum]);
}


bool CScanOffsetVal::ValidateValue(
    const float f_current_value,
    const float f_new_val,
    const float f_min_val,
    const float f_max_val,
    float& f_valueinrange,
    bool* f_isvalue_validated, /*= NULL*/
    const bool f_consider_same
) const
{

    bool l_isvalue_validated = false;
    f_valueinrange = f_new_val ;

    if (f_consider_same && (f_valueinrange == f_current_value)) {
        return false;
    }

    //in below cases value is updated so return true...
    if (f_valueinrange < f_min_val) {
        f_valueinrange = f_min_val;
        l_isvalue_validated = true;

    } else if (f_valueinrange > f_max_val) {
        f_valueinrange = f_max_val;
        l_isvalue_validated = true;
    }

    if (f_isvalue_validated) {
        *f_isvalue_validated = l_isvalue_validated;
    }

    return true;
}



bool CScanOffsetVal::GetParamValue(
    const ScanOffsetEnum f_scanoffsetenum,
    float& f_param_val
)const
{

    switch (f_scanoffsetenum) {

        case ScanOffsetX:
        case ScanOffsetY:
        case ScanOffsetZ:
            break;

        default:
            return false;
    }

    f_param_val = m_offset[f_scanoffsetenum];

    return true;
}