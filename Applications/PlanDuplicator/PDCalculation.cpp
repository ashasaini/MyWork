// PDCalculation.cpp: implementation of the CPDCalculation class.
//MPower 007/AN/Added
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "planduplicator.h"
#include "PDCalculation.h"
#include "vfcDBMgr.h"
#include "SmartVOIData.h"
#include "stdio.h"
#include "PDTrace.h"
#include "PDLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define ABS(x)  ((x)<0?-(x):(x))
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPDCalculation::CPDCalculation(CvfcDBMgr * f_pvfcmgr) : m_vfcmgr(f_pvfcmgr)
{
    PD_TRACE(CPDCalculation::CPDCalculation);
}

CPDCalculation::~CPDCalculation()
{
    PD_TRACE(CPDCalculation::~CPDCalculation);
    m_vfcmgr = NULL; 
}
/*************************************************************
* Method:    	CPDCalculation
* Description: 	Copy constructor
* Parameter: 	CPDCalculation & f_calc :
* Returns:   	
*************************************************************/
CPDCalculation::CPDCalculation(CPDCalculation& f_calc)
{
    PD_TRACE(CPDCalculation::CPDCalculation);
    m_vfcmgr = f_calc.m_vfcmgr; //handle is not created inside this class
}

/*************************************************************
* Method:    	operator=
* Description: 	assignemt overloaded function
* Parameter: 	CPDCalculation & f_calc :
* Returns:   	CPDCalculation&
*************************************************************/
CPDCalculation& CPDCalculation::operator = (CPDCalculation& f_calc)
{
    PD_TRACE(CPDCalculation::operator=);
    if (this == &f_calc)
    {
        return(*this);
    }
    m_vfcmgr = f_calc.m_vfcmgr; //handle is not created inside this class
    return(*this);
}

/*************************************************************
* Method:    	CalculateSliceThickness
* Description: 	This function is used to calculate slice thickness
* Parameter: 	PlaneAxis f_plane :
* Parameter: 	int & f_nprot_index :
* Parameter: 	CPDCoordinates * f_pdata :
* Returns:   	int
*************************************************************/
int CPDCalculation::CalculateSliceThickness(PlaneAxis f_plane,const int &f_nprot_index, CPDCoordinates* f_pdata)
{
    PD_TRACE(CPDCalculation::CalculateSliceThickness);
    float l_fslice_thickness = 0, l_fslice_gap = 0;
    int l_nslice_num = 0;
    float               l_fheight=0.0;     //along y 
    float               l_fwidth=0.0;          //along x;
    float               l_flength_sl=0.0;         //along z 
    
    if (NULL == m_vfcmgr) {
        CString l_cserrmsg (_T("CalculateSliceThickness vfcmgr is NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    status_t  l_status = 0;
    l_status = m_vfcmgr->GetSliceGap(&l_fslice_gap,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceGap: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_status = m_vfcmgr->GetSliceNum(&l_nslice_num,f_nprot_index);
    
    if (E_NO_ERROR != l_status ) {
        CString l_cserrmsg (_T("GetSliceNum: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    f32vec3_t l_orientation,l_phase;
    f32vec2_t l_fov;
    l_orientation =  f_pdata->GetSliceVectorinfo(COORD1);
    l_phase = f_pdata->GetSliceVectorinfo(COORD2);
    l_fwidth = ABS(l_phase.x - l_orientation.x);
    l_fheight = ABS (l_phase.y - l_orientation.y);
    l_flength_sl = ABS(l_phase.z - l_orientation.z);
    
    switch(f_plane){
    case CORONAL:
        l_fslice_thickness = (l_fheight-(l_nslice_num - 1)*l_fslice_gap)/l_nslice_num;//coronal
        l_fov.x = l_fwidth;
        l_fov.y=l_flength_sl;
        break;
    case SAGITTAL:
        l_fslice_thickness = (l_fwidth-(l_nslice_num - 1)*l_fslice_gap)/l_nslice_num;//sagittal
        l_fov.x = l_flength_sl;
        l_fov.y = l_fheight;
        break;
    default:
    case AXIAL:
        l_fslice_thickness = (l_flength_sl -(l_nslice_num - 1)*l_fslice_gap)/l_nslice_num;//axial
        l_fov.x = l_fwidth;
        l_fov.y = l_fheight;
        break;
    }
    
    if (0 > l_fslice_thickness){
        l_fslice_thickness = 0.0;
    }
    
    f_pdata->SetSliceAttributes(SLICE_THICKNESS,(char*)&l_fslice_thickness);
    f_pdata->SetSliceAttributes(SLICE_GAP,(char*)&l_fslice_gap);
    f_pdata->SetSliceAttributes(SLICE_NUM,(char*)&l_nslice_num);
    f_pdata->SetSliceAttributes(FOV,(char*)&l_fov);
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	CalculateSliceGap
* Description: 	This function is used to calculate slice gap
* Parameter: 	PlaneAxis f_plane :
* Parameter: 	int & f_nprot_index :
* Parameter: 	CPDCoordinates * f_pdata :
* Returns:   	int
*************************************************************/
int CPDCalculation::CalculateSliceGap(PlaneAxis f_plane, const int &f_nprot_index, CPDCoordinates* f_pdata)
{
    PD_TRACE(CPDCalculation::CalculateSliceGap);
    float l_fslice_thickness = 0, l_fslice_gap = 0;
    int l_nslice_num = 0;
    float               l_fheight=0.0;     //along y 
    float               l_fwidth=0.0;          //along x;
    float               l_flength_sl=0.0;         //along z 
    float                   temp=0.0;
    
    if (NULL == m_vfcmgr) {
        CString l_cserrmsg (_T("CalculateSliceGap vfcmgr is NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    status_t  l_status = 0;
    l_status = m_vfcmgr->GetSliceThickness(&l_fslice_thickness,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceThickness: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_status = m_vfcmgr->GetSliceNum(&l_nslice_num,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceNum: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    f32vec3_t l_orientation,l_phase;
    f32vec2_t l_fov;
    l_orientation =  f_pdata->GetSliceVectorinfo(COORD1);
    l_phase = f_pdata->GetSliceVectorinfo(COORD2);
    l_fwidth = ABS(l_phase.x - l_orientation.x);
    l_fheight = ABS (l_phase.y - l_orientation.y);
    l_flength_sl = ABS(l_phase.z - l_orientation.z);
    
    if (1 == l_nslice_num){
        l_fslice_gap = 0.0;//to avoid divide by zero.   
        l_fov.x=l_fwidth;
        l_fov.y=l_fheight;
    } else {
        switch(f_plane){
        case CORONAL:
            temp=l_fheight-(l_fslice_thickness*l_nslice_num);           //coronal
            l_fslice_gap=temp/(l_nslice_num-1);
            l_fov.x = l_fwidth;
            l_fov.y=l_flength_sl;
            break;
        case SAGITTAL:
            temp=l_fwidth-(l_fslice_thickness*l_nslice_num);           //sagittal
            l_fslice_gap=temp/(l_nslice_num-1);
            l_fov.x = l_flength_sl;
            l_fov.y = l_fheight;
            break;
        default:
        case AXIAL:
            temp=l_flength_sl-(l_fslice_thickness*l_nslice_num);           //axial
            l_fslice_gap=temp/(l_nslice_num-1);
            l_fov.x = l_fwidth;
            l_fov.y = l_fheight;
            break;
        }
    }
    
    if(0 > l_fslice_gap){
        l_fslice_gap = 0.0;
    }
    
    f_pdata->SetSliceAttributes(SLICE_THICKNESS,(char*)&l_fslice_thickness);
    f_pdata->SetSliceAttributes(SLICE_GAP,(char*)&l_fslice_gap);
    f_pdata->SetSliceAttributes(SLICE_NUM,(char*)&l_nslice_num);
    f_pdata->SetSliceAttributes(FOV,(char*)&l_fov);
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	CalculateSliceNum
* Description: 	This function is used to calculate slice number
* Parameter: 	PlaneAxis f_plane :
* Parameter: 	int & f_nprot_index :
* Parameter: 	CPDCoordinates * f_pdata :
* Returns:   	int
*************************************************************/
int CPDCalculation::CalculateSliceNum(PlaneAxis f_plane, const int &f_nprot_index, CPDCoordinates* f_pdata)
{
    PD_TRACE(CPDCalculation::CalculateSliceNum);
    float l_fslice_thickness = 0, l_fslice_gap = 0;
    int l_nslice_num = 0;
    float               l_fheight=0.0;     //along y 
    float               l_fwidth=0.0;          //along x;
    float               l_flength_sl=0.0;         //along z 
    
    if (NULL == m_vfcmgr) {
        CString l_cserrmsg (_T("CalculateSliceNum vfcmgr is NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    status_t  l_status = E_ERROR;
    l_status = m_vfcmgr->GetSliceGap(&l_fslice_gap,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceGap: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_status = m_vfcmgr->GetSliceThickness(&l_fslice_thickness,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceThickness: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    f32vec3_t l_orientation,l_phase;
    f32vec2_t l_fov;
    l_orientation =  f_pdata->GetSliceVectorinfo(COORD1);
    l_phase = f_pdata->GetSliceVectorinfo(COORD2);
    l_fwidth = ABS(l_phase.x - l_orientation.x);
    l_fheight = ABS (l_phase.y - l_orientation.y);
    l_flength_sl = ABS(l_phase.z - l_orientation.z);
    
    switch(f_plane){
    case CORONAL:
        l_nslice_num=(int)floor((l_fheight + l_fslice_gap)/(l_fslice_gap + l_fslice_thickness));  
        l_fov.x = l_fwidth;
        l_fov.y=l_flength_sl;
        break;
    case SAGITTAL:
        l_nslice_num=(int)floor((l_fwidth + l_fslice_gap)/(l_fslice_gap + l_fslice_thickness));
        l_fov.x = l_flength_sl;
        l_fov.y = l_fheight;
        break;
    default:
    case AXIAL:
        l_nslice_num=(int)floor((l_flength_sl + l_fslice_gap)/(l_fslice_gap + l_fslice_thickness));  
        l_fov.x = l_fwidth;
        l_fov.y = l_fheight;
        break;
        
    }
    
    if(0 > l_nslice_num){
        l_nslice_num = 0;
    }
    
    f_pdata->SetSliceAttributes(SLICE_THICKNESS,(char*)&l_fslice_thickness);
    f_pdata->SetSliceAttributes(SLICE_GAP,(char*)&l_fslice_gap);
    f_pdata->SetSliceAttributes(SLICE_NUM,(char*)&l_nslice_num);
    f_pdata->SetSliceAttributes(FOV,(char*)&l_fov);
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	CalculateCouchOffset
* Description: 	This function is used to calculate couch offset value
* Parameter: 	int & f_nprot_index :
* Parameter: 	CPDCoordinates * f_pdata :
* Returns:   	int
*************************************************************/
int CPDCalculation::CalculateCouchOffset(const int &f_nprot_index,CPDCoordinates *f_pdata)
{
    PD_TRACE(CPDCalculation::CalculateCouchOffset);
    float l_fslice_thickness = 0, l_fslice_gap = 0;
    int l_nslice_num = 0;
    float l_fcouch_offset = 0;
    f32vec2_t l_fov;
    status_t  l_status = E_ERROR;
    if (NULL == m_vfcmgr)
    {
        CString l_cserrmsg (_T("CalculateCouchOffset : vfc mgr NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    l_status = m_vfcmgr->GetSliceGap(&l_fslice_gap,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceGap: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_status = m_vfcmgr->GetSliceThickness(&l_fslice_thickness,f_nprot_index);
    
    if (E_NO_ERROR != l_status) {
        CString l_cserrmsg (_T("GetSliceThickness: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_status = m_vfcmgr->GetSliceNum(&l_nslice_num,f_nprot_index);
    
    if (E_NO_ERROR != l_status ) {
        CString l_cserrmsg (_T("GetSliceThickness: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    l_status = m_vfcmgr->GetFOV(&l_fov,f_nprot_index);
    
    if (E_NO_ERROR !=l_status) {
        CString l_cserrmsg (_T("GetFOV: Error."));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
    
    f32vec3_t l_slice;
    l_slice = f_pdata->GetSliceVectorinfo(COORD1);
    l_fcouch_offset = l_slice.z;
    f_pdata->SetSliceAttributes(COUCH_OFFSET,(char *)&l_fcouch_offset);
    f_pdata->SetSliceAttributes(SLICE_THICKNESS,(char*)&l_fslice_thickness);
    f_pdata->SetSliceAttributes(SLICE_GAP,(char*)&l_fslice_gap);
    f_pdata->SetSliceAttributes(SLICE_NUM,(char*)&l_nslice_num);
    f_pdata->SetSliceAttributes(FOV,(char*)&l_fov);
    
    return(E_NO_ERROR);
}

////////////////////////    End Of File     /////////////////////////////////