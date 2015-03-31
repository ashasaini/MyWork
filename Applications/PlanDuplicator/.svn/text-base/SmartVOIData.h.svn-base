// SmartVOIData.h: interface for the CSmartVOIData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMARTVOIDATA_H__854B1161_0272_4680_85CF_730E76E75EBD__INCLUDED_)
#define AFX_SMARTVOIDATA_H__854B1161_0272_4680_85CF_730E76E75EBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PDCoordinates.h"

class CSmartVOIData : public CPDCoordinates  
{
public:
    CSmartVOIData();
    virtual ~CSmartVOIData();

    int GetSliceAttributes(PDData_t f_pddata,char * f_pval);
    void  SetSliceAttributes(PDData_t f_pddata,char * f_pval);

    f32vec3_t   GetSliceVectorinfo(PDData_t f_nDataIndex);
    void        SetSliceVectorinfo(PDData_t f_nDataIndex,f32vec3_t& f_vecData);


private:
    float       m_couch_offset;
    f32vec3_t   m_v_coord1;
    f32vec3_t   m_v_coord2;
};

#endif // !defined(AFX_SMARTVOIDATA_H__854B1161_0272_4680_85CF_730E76E75EBD__INCLUDED_)
