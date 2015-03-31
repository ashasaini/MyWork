// PDCoordinates.h: interface for the CPDCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDCOORDINATES_H__A387A52B_5E2E_4832_BFF2_4BBF5FDB4979__INCLUDED_)
#define AFX_PDCOORDINATES_H__A387A52B_5E2E_4832_BFF2_4BBF5FDB4979__INCLUDED_
#include "C:\MRMPlus\include\tami\common\basicdefs.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "dataTypes.h"
class CPDCoordinates  
{
public:
    CPDCoordinates();
    virtual ~CPDCoordinates();

	    void setPDPlan(CString &f_csPlane);//setter method for Plan
       CString& getPDPlan();//getter method for Plan

    virtual int GetSliceAttributes(PDData_t f_pddata,char * f_pval) = 0;
    virtual void SetSliceAttributes(PDData_t f_pddata,char * f_pval)= 0;

    virtual f32vec3_t   GetSliceVectorinfo(PDData_t f_nDataIndex) =0 ;
    virtual void        SetSliceVectorinfo(PDData_t f_nDataIndex,f32vec3_t&) =0 ;

protected:
    f32vec2_t   m_fov;
    float       m_slice_thickness;
    float       m_slice_gap;
    int         m_slice_num;
    //char        m_SlicePlane[BUFFER_SIZE_20];
	CString     m_SlicePlane;
    
};
     
  

#endif // !defined(AFX_PDCOORDINATES_H__A387A52B_5E2E_4832_BFF2_4BBF5FDB4979__INCLUDED_)


