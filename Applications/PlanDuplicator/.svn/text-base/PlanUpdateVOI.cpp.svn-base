// PlanUpdateVOI.cpp: implementation of the CPlanUpdateVOI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlanUpdateVOI.h"
#include "planduplicator.h"
#include "PDTrace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************

//Method Name   :CPlanUpdateVOI

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************

CPlanUpdateVOI::CPlanUpdateVOI()
{
    PD_TRACE(CPlanUpdateVOI::CPlanUpdateVOI);

}
//****************************Method Header************************************

//Method Name   :PlanUpdateVOI

//Author        :iGATE

//Purpose       :Destructor
//*****************************************************************************

CPlanUpdateVOI::~CPlanUpdateVOI()
{
    PD_TRACE(CPlanUpdateVOI::~CPlanUpdateVOI);
}

//****************************Method Header************************************

//Method Name   :UpdateVector

//Author        :iGATE

//Purpose       :
//*****************************************************************************
int CPlanUpdateVOI::UpdateVector(CPDCoordinates *f_VOI_Coordinates,VfPathElem_t f_path[], int f_sliceplane_selection)
{
    PD_TRACE(PlanUpdateVOI::UpdateVector);

	f32vec3_t l_v_slice;
	f32vec3_t l_v_phase;
	plValT    l_value;
    status_t  l_plStatus = E_NO_ERROR;
    char      *l_name = NULL;
	//float	  l_couch_offset = 0.0;
    float	  *l_fcouch_offset = NULL;

	if(0 == f_sliceplane_selection){  // scout selection
        l_fcouch_offset = (float*)f_VOI_Coordinates->GetSliceAttributes(COUCH_OFFSET);//structure KJ
        if_error(l_plStatus){
            CString l_cserrmsg = _T("Error in UpdateCouchOffset.\n");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(l_plStatus);
            }
    }else{
		l_v_slice = f_VOI_Coordinates->GetSliceVectorinfo(SLICE_ORI_VECTOR);
		l_v_phase  =f_VOI_Coordinates->GetSliceVectorinfo(PHASE_ORI_VECTOR);
	}


	/* ---------------vgSliceOffsetVector updation ------------------*/
    l_plStatus = plGetVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_OFFSET_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plGetVal PL_VG_SLICE_OFFSET_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }

	PL_VAL_FLTVEC3_X(l_value) = (l_v_phase.x + l_v_slice.x)/2;
	PL_VAL_FLTVEC3_Y(l_value) = (l_v_phase.y + l_v_slice.y)/2;

	//if (/*0 == flag_scout_selection*/){  // if not scout selection
		PL_VAL_FLTVEC3_Z(l_value) = (l_v_phase.z + l_v_slice.z)/2;
	//}else{
		PL_VAL_FLTVEC3_Z(l_value) = *l_fcouch_offset;
	//}	
	
    l_plStatus = plPutVal(f_path,SVD_ST_VISUALGROUP,PL_VG_SLICE_OFFSET_VECTOR,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plPutVal PL_VG_SLICE_OFFSET_VECTOR.\n");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_plStatus);
    }

	return(E_NO_ERROR);

}
//****************************Method Header************************************

//Method Name   :UpdateSliceAttributes

//Author        :iGATE

//Purpose       :
//*****************************************************************************

int CPlanUpdateVOI::UpdateSliceAttributes(CPDCoordinates *f_Coordinates,int f_attribute_selection, int f_sliceplane_selection)
{
    PD_TRACE(PlanUpdateVOI::UpdateSliceAttributes);

	status_t l_status = 0;

    if(1 == f_sliceplane_selection) {// slice plane multi slice 
		l_status = UpdateSelectedSliceAttribute(f_Coordinates,f_attribute_selection);
    } else {
		l_status = UpdateSelectedSliceAttribute(f_Coordinates,0);//PD_SLICE_GAP i.e. default updation if not multislice
        DB_PRINTF("C",("PD_SLICE_GAP\n"));
    }

	l_status = UpdateFOV(f_Coordinates);

	return(E_NO_ERROR);

}
////////////////////////    End Of File     /////////////////////////////////