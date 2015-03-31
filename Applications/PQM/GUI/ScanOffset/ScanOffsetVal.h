// ScanOffsetVal.h: interface for the CScanOffsetVal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_SCANOFFSETVAL_H__
#define __SM_SCANOFFSETVAL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum ScanOffsetEnum {ScanOffsetX = 0, ScanOffsetY = 1, ScanOffsetZ = 2, ScanOffset_Total = 3, ScanOffset_Invalid};

class ScanOffset
{

public:

    ScanOffset() ;
    ScanOffset(const float f_x, const float f_y, const float f_z) ;

    float& operator [](const ScanOffsetEnum f_scanoffsetenum) ;
    const float operator [](const ScanOffsetEnum f_scanoffsetenum) const ;

    CString GetLogString() const ;

private:
    float m_offset[ScanOffset_Total];
};



class CScanOffsetVal
{
public:
    CScanOffsetVal();
    virtual ~CScanOffsetVal();

    void SetScanOffsetData(const ScanOffset& f_scan_offset, const ScanOffset& f_min_offset, const ScanOffset& f_max_offset);
    void UpdateMinMax(const ScanOffset& f_min_offset, const ScanOffset& f_max_offset);
    void UpdateScanOffset(const float f_x, const float f_y, const float f_z);

    bool Increment(const ScanOffsetEnum f_scanoffsetenum, const float f_increment = 1.f);
    bool UpdateValue(const ScanOffsetEnum f_scanoffsetenum, const CString& f_value);
    void UpdateValue(const ScanOffsetEnum f_scanoffsetenum, const float f_new_value);

    const CString GetString(const ScanOffsetEnum f_scanoffsetenum) const;

    const ScanOffset& GetMinOffset()const;
    const ScanOffset& GetMaxOffset()const;

    const ScanOffset& GetOffset()const;

private:

    ScanOffset m_offset;
    ScanOffset m_offset_min;
    ScanOffset m_offset_max;

    bool IsValidValue(CString f_value) const;
    double ConvertStringToDouble(const CString& str) const;

    bool IncrementAndValidate(const ScanOffsetEnum f_scanoffsetenum, const float f_increment) ;

    bool ValidateValue(const float f_current_value, const float f_new_val, const float f_min_val, const float f_max_val, float& f_valueinrange, bool* f_isvalue_validated = NULL, const bool f_consider_same = true) const ;

    bool GetParamValue(const ScanOffsetEnum f_scanoffsetenum, float& f_param_val) const;

};

#endif // #ifndef __SM_SCANOFFSETVAL_H__
