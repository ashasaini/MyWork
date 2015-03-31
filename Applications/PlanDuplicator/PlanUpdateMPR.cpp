// PlanUpdateMPR.cpp: implementation of the CPlanUpdateMPR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlanUpdateMPR.h"
#include "PDTrace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************

//Method Name   :CPlanUpdateMPR

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************

CPlanUpdateMPR::CPlanUpdateMPR()
{
    PD_TRACE(CPlanUpdateMPR::CPlanUpdateMPR);
}

//****************************Method Header************************************

//Method Name   :PlanUpdateMPR

//Author        :iGATE

//Purpose       :Destructor
//*****************************************************************************

CPlanUpdateMPR::~CPlanUpdateMPR()
{
    PD_TRACE(CPlanUpdateMPR::~CPlanUpdateMPR);
}

//****************************Method Header************************************

//Method Name   :UpdateVector

//Author        :iGATE

//Purpose       :
//*****************************************************************************
int CPlanUpdateMPR::UpdateVector(CPDCoordinates *f_MPR_Coordinates, VfPathElem_t f_path[], int f_sliceplane_selection)
{
    PD_TRACE(CPlanUpdateMPR::UpdateVector);

	f32vec3_t l_v_slice;
	f32vec3_t l_v_phase;
	f32vec3_t l_v_offset;
	plValT    l_value;
    status_t  l_plStatus = E_NO_ERROR;
    char      *l_name = NULL;

    if(NULL == f_MPR_Coordinates || 0 > f_sliceplane_selection)
    {
        CString l_cserrmsg = _T("Error in input of UpdateVector in CPlanUpdateMPR.\n");
         CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
         return(E_ERROR);
    }

	l_v_slice  =f_MPR_Coordinates->GetSliceVectorinfo(SLICE_ORI_VECTOR);
	l_v_phase  =f_MPR_Coordinates->GetSliceVectorinfo(PHASE_ORI_VECTOR);
	l_v_offset =f_MPR_Coordinates->GetSliceVectorinfo(OFFSET_VECTOR);

	/* ---------------vgSliceOffsetVector updation ------------------*/
    l_plStatus = plGetVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_OFFSET_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plGetVal PL_VG_SLICE_OFFSET_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }

	PL_VAL_FLTVEC3_X(l_value) = l_v_offset.x;
	PL_VAL_FLTVEC3_Y(l_value) = l_v_offset.y;
	PL_VAL_FLTVEC3_Z(l_value) = l_v_offset.z;
	
    l_plStatus = plPutVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_OFFSET_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plPutVal PL_VG_SLICE_OFFSET_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }
	
	/*if_error(RelimitProlim())
    {
        DB_PRINTF("E",("Error RelimitProlim %s    %d\n", __FILE__, __LINE__));
        FUNC_EXIT ();
        return(E_ERROR);
    }*/

	/* ---------------vgSliceOrientationVector updation ------------------*/
    l_plStatus = plGetVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_ORI_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plGetVal PL_VG_SLICE_ORI_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }

	PL_VAL_FLTVEC3_X(l_value) = l_v_slice.x;
	PL_VAL_FLTVEC3_Y(l_value) = l_v_slice.y;
	PL_VAL_FLTVEC3_Z(l_value) = l_v_slice.z;
	
    l_plStatus = plPutVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_OFFSET_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plPutVal PL_VG_SLICE_ORI_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }
	
	/*if_error(RelimitProlim())
    {
        DB_PRINTF("E",("Error RelimitProlim %s    %d\n", __FILE__, __LINE__));
        FUNC_EXIT ();
        return(E_ERROR);
    }*/

		/* ---------------vgSlicePhaseVector updation ------------------*/
    l_plStatus = plGetVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_PHASE_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plGetVal PL_VG_SLICE_PHASE_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }

	PL_VAL_FLTVEC3_X(l_value) = l_v_phase.x;
	PL_VAL_FLTVEC3_Y(l_value) = l_v_phase.y;
	PL_VAL_FLTVEC3_Z(l_value) = l_v_phase.z;
	
    l_plStatus = plPutVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_PHASE_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plPutVal PL_VG_SLICE_PHASE_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg ,__LINE__);
        return(l_plStatus);
    }
	
	/*if_error(RelimitProlim())
    {
        DB_PRINTF("E",("Error RelimitProlim %s    %d\n", __FILE__, __LINE__));
        FUNC_EXIT ();
        return(E_ERROR);
    }*/

	return(l_plStatus);
}

//****************************Method Header************************************
//Method Name   :UpdateSliceAttributes

//Author        :iGATE

//Purpose       :
//*****************************************************************************
int CPlanUpdateMPR::UpdateSliceAttributes(CPDCoordinates* f_Coordinates,int f_attribute_selection, int f_sliceplane_selection)
{
    PD_TRACE(CPlanUpdateMPR::UpdateSliceAttributes);
	status_t l_status = 0;

	l_status =UpdateSelectedSliceAttribute(f_Coordinates, 0);//GAP
    if (E_NO_ERROR != l_status)
    {
        CString l_cserrmsg = _T("Error in UpdateSelectedSliceAttribute.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
	l_status =UpdateSelectedSliceAttribute(f_Coordinates, 1);//NUMBER
    if (E_NO_ERROR != l_status)
    {
        CString l_cserrmsg = _T("Error in UpdateSelectedSliceAttribute.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }
	l_status =UpdateSelectedSliceAttribute(f_Coordinates, 2);//THICKNESS
    if (E_NO_ERROR != l_status)
    {
        CString l_cserrmsg = _T("Error in UpdateSelectedSliceAttribute.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }

	l_status =UpdateFOV(f_Coordinates);
    if (E_NO_ERROR != l_status)
    {
        CString l_cserrmsg = _T("Error in UpdateFOV.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return E_ERROR;
    }

	return(E_NO_ERROR);
}
////////////////////////    End Of File     /////////////////////////////////