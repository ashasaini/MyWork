// SmartMPRData.cpp: implementation of the CSmartMPRData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SmartMPRData.h"
#include "PDTrace.h"
#include "PDLogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************

//Method Name   :SmartMPRData

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************

CSmartMPRData::CSmartMPRData(void)
{
    PD_TRACE(CSmartMPRData::CSmartMPRData);
    memset(&m_v_offset_vec,0,sizeof(m_v_offset_vec));
    memset(&m_v_phase_vec,0,sizeof(m_v_phase_vec));
    memset(&m_v_slice_vec,0,sizeof(m_v_slice_vec));
    
}

//****************************Method Header************************************

//Method Name   :SmartMPRData

//Author        :PATNI

//Purpose       :Destructor
//*****************************************************************************

CSmartMPRData::~CSmartMPRData()
{
    PD_TRACE(CSmartMPRData::~CSmartMPRData);
}

/*************************************************************
* Method:    	GetSliceVectorinfo
* Description: 	This function is used 
* Parameter: 	PDData_t f_nDataIndex :
* Returns:   	f32vec3_t (value of offset / phase / slice vectors)
*************************************************************/
f32vec3_t CSmartMPRData::GetSliceVectorinfo(PDData_t f_nDataIndex)
{
    PD_TRACE(CSmartMPRData::GetSliceVectorinfo);
    
    switch(f_nDataIndex) {
        
    case SLICE_ORI_VECTOR:
        return(m_v_slice_vec);
    case PHASE_ORI_VECTOR:
        return(m_v_phase_vec);
    case OFFSET_VECTOR:
        return(m_v_offset_vec);
        
    default:
        CString l_cserrmsg = _T("Default case executed in GetSliceVectorinfo.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(m_v_slice_vec);
    }
}

/*************************************************************
* Method:    	SetSliceVectorinfo
* Description: 	sets vector values
* Parameter: 	PDData_t f_nDataIndex :
* Parameter: 	f32vec3_t & f_vecData :
* Returns:   	void
*************************************************************/
void CSmartMPRData::SetSliceVectorinfo(PDData_t f_nDataIndex,f32vec3_t& f_vecData)
{
    PD_TRACE(CSmartMPRData::SetSliceVectorinfo);
    
    switch(f_nDataIndex) {
        
    case SLICE_ORI_VECTOR: 
        m_v_slice_vec = f_vecData;
        break;
    case PHASE_ORI_VECTOR:
        m_v_phase_vec = f_vecData;
        break;
    case OFFSET_VECTOR:
        m_v_offset_vec = f_vecData;
        break;
        
    default:
       CString l_cserrmsg = _T("Default case executed in SetSliceVectorinfo.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        break;
    }
}

/*************************************************************
* Method:    	GetSliceAttributes
* Description: 	gets value of MPR data parameters
* Parameter: 	PDData_t f_pddata :
* Parameter: 	char * f_pval :
* Returns:   	int
*************************************************************/
int CSmartMPRData::GetSliceAttributes(PDData_t f_pddata,char * f_pval)
{
    PD_TRACE(CSmartMPRData::GetSliceAttributes);
    
    if (NULL == f_pval)
    {
        CString l_cserrmsg (_T("GetSliceAttributes() parameter 'f_pval' found NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    switch(f_pddata) {
    case SLICE_THICKNESS:
        memcpy(f_pval,&m_slice_thickness,sizeof(float));
        break;
    case SLICE_GAP: 
        memcpy(f_pval,&m_slice_gap,sizeof(float));
        break;
    case SLICE_NUM: 
        memcpy(f_pval,&m_slice_num,sizeof(int));
        break;
    case FOV: 
        memcpy(f_pval,&m_fov,sizeof(f32vec2_t));
        break;
    default:
        CString l_cserrmsg = _T("Default case executed in GetSliceAttributes.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	SetSliceAttributes
* Description: 	sets values to MPR Data parameters
* Parameter: 	PDData_t f_pddata :
* Parameter: 	char * f_pval :
* Returns:   	void
*************************************************************/
void CSmartMPRData::SetSliceAttributes(PDData_t f_pddata,char * f_pval)
{
    PD_TRACE(CSmartMPRData::SetSliceAttributes);
    
    switch(f_pddata) {
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
        CString l_cserrmsg = _T("Default case executed in SetSliceAttributes.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        m_slice_thickness = 0;
        m_slice_gap = 0;
        
        m_slice_num = 0;
        m_fov.x = 0;
        m_fov.y = 0;
    }
}

////////////////////////    End Of File     /////////////////////////////////