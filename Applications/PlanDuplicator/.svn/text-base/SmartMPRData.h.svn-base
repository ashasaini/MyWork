// SmartMPRData.h: interface for the CSmartMPRData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMARTMPRDATA_H__AE1D60B9_32A2_44F8_81C6_2A1952EF9BBB__INCLUDED_)
#define AFX_SMARTMPRDATA_H__AE1D60B9_32A2_44F8_81C6_2A1952EF9BBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "C:\MRMPlus\include\tami\common\basicdefs.h"
#include "PDCoordinates.h"

class CSmartMPRData  : public CPDCoordinates
{
public:
    CSmartMPRData(void);
    virtual ~CSmartMPRData();
    int GetSliceAttributes(PDData_t f_pddata,char * f_pval);
    void  SetSliceAttributes(PDData_t f_pddata,char * f_pval);
    f32vec3_t   GetSliceVectorinfo(PDData_t f_nDataIndex);
    void        SetSliceVectorinfo(PDData_t f_nDataIndex,f32vec3_t& f_vecData);
private:
        f32vec3_t m_v_offset_vec;
        f32vec3_t m_v_phase_vec;
        f32vec3_t m_v_slice_vec;


};

#endif // !defined(AFX_SMARTMPRDATA_H__AE1D60B9_32A2_44F8_81C6_2A1952EF9BBB__INCLUDED_)
