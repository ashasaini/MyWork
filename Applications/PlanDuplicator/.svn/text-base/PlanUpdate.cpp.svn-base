// PlanUpdate.cpp: implementation of the CPlanUpdate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlanUpdate.h"
#include "PDTrace.h"
#include "dataTypes.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************

//Method Name   :CPlanUpdate

//Author        :iGATE

//Purpose       :Constructor
//*****************************************************************************
CPlanUpdate::CPlanUpdate()
{
    PD_TRACE(CPlanUpdate::CPlanUpdate);

}
//****************************Method Header************************************

//Method Name   :CPlanUpdate

//Author        :iGATE

//Purpose       :Destructor
//*****************************************************************************
CPlanUpdate::~CPlanUpdate()
{
    PD_TRACE(CPlanUpdate::~CPlanUpdate);
}

//****************************Method Header************************************

//Method Name   :UpdatePlanStatus

//Author        :iGATE

//Purpose       :Stores value in m_SlicePlane
//*****************************************************************************
int CPlanUpdate::UpdatePlanStatus()
{
    PD_TRACE(CPlanUpdate::setSlicePlane);

    status_t		l_plStatus;
	plValT			l_value;	
	VfPathElem_t	l_path[PROT_PATH_LEVEL];

	l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index    = 0;

    l_plStatus = plGetVal(l_path,SVD_ST_PROTOCOL,PL_STATUS,&l_value);
    if_error(l_plStatus){
    		PL_VAL_BITFIELD(l_value) = 0;
    }
    PL_VAL_BITFIELD(l_value) |= VFC_PLN_STATUS_GLOC;
    
    l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_STATUS,&l_value);

    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in PL_STATUS\n.");
    		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    		return(l_plStatus);
    }
	/*
    if_error(RelimitProlim())
    {
         CString l_cserrmsg = _T("Error RelimitProlim PL_STATUS\n.")
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }*/
	return(l_plStatus);
}
//****************************Method Header************************************

//Method Name   :UpdateLocator

//Author        :iGATE

//Purpose       :returns value of m_SlicePlane
//*****************************************************************************
int CPlanUpdate::UpdateLocator( int f_slice_number)
{
    PD_TRACE(CPlanUpdate::UpdateLocator);

    static char	*l_ImagesTables[] = {PL_LOCATOR_IMAGES , PL_LOCATOR_IMAGE_2,
                                    PL_LOCATOR_IMAGE_3, PL_LOCATOR_IMAGE_4,
                                    PL_LOCATOR_IMAGE_5, PL_LOCATOR_IMAGE_6};
    plValT       l_value;
    status_t     l_plStatus = 0;
    VfPathElem_t l_path[PROT_PATH_LEVEL];

	l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index    = 0;

	l_value.u.value[0].pathelem.subtree_name =  SVN_ST_PROTOCOL;
    //l_value.u.value[0].pathelem.index = ;//scanedProtocol
    l_value.u.value[1].pathelem.subtree_name =  SVN_ST_PROCESSED;
    //l_value.u.value[1].pathelem.index = ;// procIdx
    l_value.u.value[2].pathelem.subtree_name =  SVN_ST_ECHO;
    l_value.u.value[2].pathelem.index =  0;
    l_value.u.value[3].pathelem.subtree_name = SVN_ST_POSITION;
    l_value.u.value[3].pathelem.index = (int)f_slice_number;
    l_value.itemCt	= 4;
    l_value.units     = plUnitsTunitless;
    l_value.valueType = plValTPathElem;

    //DB_PRINTF("D",("Locator image = %d\n",l_value.u.value[3].pathelem.index));

    l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_LOCATOR_IMAGES ,&l_value);
    if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in PL_LOCATOR_IMAGES\n.");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
    }
    
    return(l_plStatus);
}

//****************************Method Header************************************

//Method Name   :UpdateSelectedSliceAttribute

//Author        :iGATE

//Purpose       :
//*****************************************************************************

int CPlanUpdate::UpdateSelectedSliceAttribute(CPDCoordinates *f_Coordinates,int f_attribute_selection)
{
    PD_TRACE(PlanUpdateVOI::UpdateSelectedSliceAttribute);

	status_t		l_plStatus = E_NO_ERROR;
	plValT			l_value;
    VfPathElem_t    l_path[PROT_PATH_LEVEL];
    float*          l_ftemp = NULL;
    long*           l_ntemp = NULL;
	l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index    = 0;

	switch(f_attribute_selection)
	{		
        case SEL_SLICE_NUM:
            l_value.itemCt = 1;
            l_ntemp = (long*)f_Coordinates->GetSliceAttributes(SLICE_NUM);// svoi_t[pos_count].sliceNo
            l_value.u.value[0].integer = *l_ntemp;
            l_value.units = plUnitsTunitless;
            l_value.valueType = plValTInt;
        
            l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_TOTAL_NUM_SLICES ,&l_value);
            if(l_plStatus){
                CString l_cserrmsg = _T("Error in plPutVal PL_VG_NUM_SLICES.\n");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(l_plStatus);
            }
            //DB_PRINTF("C",("PD_SLICE_NO\n"));
		    break;
        
        case SEL_SLICE_GAP:
			l_value.itemCt = 1;
            l_ftemp = (float*)f_Coordinates->GetSliceAttributes(SLICE_GAP);//svoi_t[pos_count].slicegap
			l_value.u.value[0].real = *l_ftemp;
			l_value.units = plUnitsTm;
			l_value.valueType = plValTFloat;

			l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_SELECTED_GAP,&l_value);
			if_error(l_plStatus){
                CString l_cserrmsg = _T("Error in plPutVal PL_SELECTED_GAP.\n");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
				return(l_plStatus);
			}
			//DB_PRINTF("C",("PD_SLICE_GAP\n"));
		break;

        case SEL_SLICE_THICKNESS:
            l_value.itemCt = 1;
            l_ftemp =(float*)f_Coordinates->GetSliceAttributes(SLICE_THICKNESS);// svoi_t[pos_count].thickness
            l_value.u.value[0].real = *l_ftemp;
            l_value.units = plUnitsTm;
            l_value.valueType = plValTFloat;
            
            l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_SELECTED_SLICE_THK ,&l_value);
            if_error(l_plStatus){
                CString l_cserrmsg = _T("Error in plPutVal PL_SELECTED_SLICE_THK.\n");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                return(l_plStatus);
            }
            //DB_PRINTF("C",("PD_SLICE_THICKNESS\n"));
		    break;

		
		
		
	}
	return(l_plStatus);	
}
//****************************Method Header************************************

//Method Name   :UpdateFOV

//Author        :iGATE

//Purpose       :
//*****************************************************************************
int CPlanUpdate::UpdateFOV(CPDCoordinates *f_Coordinates){

	status_t		l_plStatus = E_NO_ERROR;
	plValT			l_value;
    VfPathElem_t    l_path[PROT_PATH_LEVEL];


	l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index			= 0;

	/*FOV UPDATE*/
    l_plStatus = plGetVal(l_path,SVD_ST_PROTOCOL,PL_PE_FOV,&l_value);
    if_error(l_plStatus){
        CString = l_cserrmsg = _T("Error in  plGettVal PL_PE_FOV.\n");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
    }

	f32vec2_t		*l_fov = (f32vec2_t*)f_Coordinates->GetSliceAttributes(FOV);
//	l_fov = 
    PL_VAL_REAL(l_value) = l_fov->y;
    l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_PE_FOV,&l_value);
	if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in  plPutVal PL_PE_FOV.\n");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
	}

    l_plStatus = plGetVal(l_path,SVD_ST_PROTOCOL,PL_PE_FOV,&l_value);
	if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in  plGetVal PL_PE_FOV.\n");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
	}

    /* sliceHeight(FOV) */
    l_plStatus = plGetVal(l_path,SVD_ST_PROTOCOL,PL_RO_FOV,&l_value);
	if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plGetVal PL_RO_FOV\n");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
	}

    PL_VAL_REAL(l_value) = l_fov->x;
    l_plStatus = plPutVal(l_path,SVD_ST_PROTOCOL,PL_RO_FOV,&l_value);
	if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plPutVal PL_RO_FOV\n");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
	}

    l_plStatus = plGetVal(l_path,SVD_ST_PROTOCOL,PL_RO_FOV,&l_value);
	if_error(l_plStatus){
        CString l_cserrmsg = _T("Error in plGetVal PL_RO_FOV\n");
		CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
		return(l_plStatus);
	}

	return(l_plStatus);

}

/////////////////////////////////   End of File     /////////////////////////////