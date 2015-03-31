// PDCoordinates.cpp: implementation of the CPDCoordinates class.
//

#include "stdafx.h"
#include "PDCoordinates.h"
#include "PDTrace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************

//Method Name   :PDCoordinates

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************
CPDCoordinates::CPDCoordinates():m_slice_thickness(0.0f),m_slice_gap(0.0f),m_slice_num(0)
{
    PD_TRACE(CPDCoordinates::CPDCoordinates);
    m_SlicePlane = _T("");
	//memset(m_SlicePlane,0,sizeof(m_SlicePlane));
}
//****************************Method Header************************************

//Method Name   :PDCoordinates

//Author        :PATNI

//Purpose       :Destructor
//*****************************************************************************
CPDCoordinates::~CPDCoordinates()
{
    PD_TRACE(CPDCoordinates::~CPDCoordinates);
}

//****************************Method Header************************************

//Method Name   :setPDPlan

//Author        :PATNI

//Purpose       :Stores value in m_SlicePlane
//*****************************************************************************
void CPDCoordinates::setPDPlan(CString &f_csPlane)
{
    PD_TRACE(CPDCoordinates::setPDPlan);
     m_SlicePlane =  f_csPlane; //AN
   // strcpy(m_SlicePlane,f_ptr);
}
//****************************Method Header************************************

//Method Name   :getPlan

//Author        :PATNI

//Purpose       :returns value of m_SlicePlane
//*****************************************************************************
CString& CPDCoordinates::getPDPlan(void)
{
    PD_TRACE(CPDCoordinates::getPDPlan);

    return(m_SlicePlane);
}

/*************************************************************
* Method:    	GetSliceAttributes
* Description: 	This function is used 
* Parameter: 	PDData_t f_pddata :
* Parameter: 	char * f_pval :
* Returns:   	int
*************************************************************/
int CPDCoordinates::GetSliceAttributes(PDData_t f_pddata,char * f_pval)
{
	return(E_NO_ERROR);//Mpower 040/2012-08-09/AS/Modified/return was NULL
}

/*************************************************************
* Method:    	SetSliceAttributes
* Description: 	This function is used 
* Parameter: 	PDData_t f_pddata :
* Parameter: 	char * f_pval :
* Returns:   	void
*************************************************************/
void  CPDCoordinates::SetSliceAttributes(PDData_t f_pddata,char * f_pval)
 {
 	return;
} 
/*f32vec3_t CPDCoordinates::GetSliceVectorinfo(PDData_t f_pddata)
{
   
    return();
}

void  CPDCoordinates::SetSliceVectorinfo(PDData_t f_pddata,f32vec3_t f_vec)
{
    return;
} */

/////////////////////////////////   End of File     /////////////////////////////