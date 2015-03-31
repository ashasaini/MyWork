// SmartVOIData.cpp: implementation of the CSmartVOIData class.
//Mpower 013/AN/Added
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "planduplicator.h"
#include "SmartVOIData.h"
#include "PDTrace.h"
#include "PDLogHelper.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSmartVOIData::CSmartVOIData() :
m_couch_offset (0)
{
    PD_TRACE(CSmartVOIData::CSmartVOIData);
    memset(&m_fov,0,sizeof(m_fov));
    memset(&m_v_coord1,0,sizeof(m_v_coord1));
    memset(&m_v_coord2,0,sizeof(m_v_coord2));
    
}

CSmartVOIData::~CSmartVOIData()
{
    PD_TRACE(CSmartVOIData::~CSmartVOIData);
}

/*************************************************************
* Method:    	GetSliceVectorinfo
* Description: 	returns vector values depending on type of PDData_t
* Parameter: 	PDData_t f_nDataIndex :
* Returns:   	f32vec3_t
*************************************************************/
f32vec3_t CSmartVOIData::GetSliceVectorinfo(PDData_t f_nDataIndex)
{
    PD_TRACE(CSmartVOIData::GetSliceVectorinfo);
    
    switch(f_nDataIndex) {
        
    case COORD1: 
        return(m_v_coord1);
        
    case COORD2:
        return(m_v_coord2);
        
    default:
        return(m_v_coord1);
    }
}

/*************************************************************
* Method:    	SetSliceVectorinfo
* Description: 	sets vector values of PDData_t type
* Parameter: 	PDData_t f_nDataIndex :
* Parameter: 	f32vec3_t & f_vecData :
* Returns:   	void
*************************************************************/
void CSmartVOIData::SetSliceVectorinfo(PDData_t f_nDataIndex,f32vec3_t& f_vecData)
{
    PD_TRACE(CSmartVOIData::SetSliceVectorinfo);
    
    switch(f_nDataIndex) {
        
    case 1: 
        m_v_coord1 = f_vecData;
        break;
    case 2:
        m_v_coord2 = f_vecData;
        break;
        
    default:
        break;
    }
}

/*************************************************************
* Method:    	GetSliceAttributes
* Description: 	returns value of parameter specified in f_pddata
* Parameter: 	PDData_t f_pddata :
* Parameter: 	char * f_pval :
* Returns:   	int
*************************************************************/
int CSmartVOIData::GetSliceAttributes(PDData_t f_pddata,char * f_pval)
{
    PD_TRACE(CSmartVOIData::GetSliceAttributes);
    
    if (NULL == f_pval)
    {
        CString l_cserrmsg = _T("GetSliceAttributes : Parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    switch(f_pddata) {
    case COUCH_OFFSET: 
        memcpy(f_pval,&m_couch_offset,sizeof(m_couch_offset));
        break;
    case SLICE_THICKNESS:
        memcpy(f_pval,&m_slice_thickness,sizeof(m_slice_thickness));
        break;
    case SLICE_GAP: 
        memcpy(f_pval,&m_slice_gap,sizeof(m_slice_gap));
        break;
    case SLICE_NUM: 
        memcpy(f_pval,&m_slice_num,sizeof(m_slice_num));
        break;
    case FOV: 
        memcpy(f_pval,&m_fov,sizeof(m_fov));
        break;
    default:
        break;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	SetSliceAttributes
* Description: 	sets values of parameter specified in f_pddata
* Parameter: 	PDData_t f_pddata :
* Parameter: 	char * f_pval :
* Returns:   	void
*************************************************************/
void CSmartVOIData::SetSliceAttributes(PDData_t f_pddata,char * f_pval)
{
    PD_TRACE(CSmartVOIData::SetSliceAttributes);
    if (ALL != f_pddata && NULL == f_pval)
    {
        CString l_cserrmsg = _T("SetSliceAttributes : Parameter NULL");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return;
    }
    switch(f_pddata) {
    case COUCH_OFFSET: 
        memcpy(&m_couch_offset,f_pval,sizeof(m_couch_offset));
        break;
    case SLICE_THICKNESS:
        memcpy(&m_slice_thickness,f_pval,sizeof(m_slice_thickness));
        break;
    case SLICE_GAP: 
        memcpy(&m_slice_gap,f_pval,sizeof(m_slice_gap));
        break;
    case SLICE_NUM: 
        memcpy(&m_slice_num,f_pval,sizeof(m_slice_num));
        break;
    case FOV: 
        memcpy(&m_fov,f_pval,sizeof(m_fov));
        break;
    case ALL:
    default:
        m_couch_offset = 0;
        m_slice_thickness = 0;
        m_slice_gap = 0;
        
        m_slice_num = 0;
        m_fov.x = 0;
        m_fov.y = 0;
    }
}
////////////////////////    End Of File     /////////////////////////////////