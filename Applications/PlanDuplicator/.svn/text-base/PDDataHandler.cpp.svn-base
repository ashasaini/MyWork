// PDDataHandler.cpp: implementation of the CPDDataHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDDataHandler.h"
#include "PDTrace.h"
#include "PDLogHelper.h"
#include "SmartMPRData.h"
#include "SmartVOIData.h"
#include "typeinfo.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************

//Method Name   :CPDDataHandler

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************

CPDDataHandler::CPDDataHandler()
{
    PD_TRACE(CPDDataHandler::CPDDataHandler);
}
/*************************************************************
* Method:    	CPDDataHandler
* Description: 	Copy Constructor
* Parameter: 	CPDDataHandler & f_handler :
* Returns:   	
*************************************************************/
CPDDataHandler::CPDDataHandler(CPDDataHandler &f_handler)
{  
    PD_TRACE(CPDDataHandler::CPDDataHandler);
    int l_ni = 0;
    CSmartMPRData * l_mpr_data = NULL;
    CSmartVOIData * l_voi_data = NULL;
    CPDCoordinates * l_pd_data = NULL;
    for (l_ni = 0; l_ni < (int)f_handler.m_vec_VantageData.size(); l_ni++)
    {
        
        
        if (NULL != f_handler.m_vec_VantageData[l_ni])
        {
            
            
            if (NULL != (l_voi_data = dynamic_cast<CSmartVOIData *>(f_handler.m_vec_VantageData[l_ni])))
            {
                
                l_pd_data = new CSmartVOIData(*l_voi_data);
                if (NULL != l_pd_data)
                    m_vec_VantageData.push_back(l_pd_data);
                
            }
            else if (NULL != (l_mpr_data = dynamic_cast<CSmartMPRData *>(f_handler.m_vec_VantageData[l_ni])))
            {
                
                l_pd_data = new CSmartMPRData(*l_mpr_data);
                if (NULL != l_pd_data)
                    m_vec_VantageData.push_back(l_pd_data);
                
            }
        }
        if (NULL != m_vec_DicomData[l_ni])
        {
            if (NULL != (l_voi_data = dynamic_cast<CSmartVOIData *>(f_handler.m_vec_DicomData[l_ni])))
                //if ()
            {
                
                l_pd_data = new CSmartVOIData(*l_voi_data);
                if (NULL != l_pd_data)
                    m_vec_DicomData.push_back(l_pd_data);
                
            }
            
            if (NULL != (l_mpr_data = dynamic_cast<CSmartMPRData *>(f_handler.m_vec_DicomData[l_ni])))
            {
                
                l_pd_data = new CSmartMPRData(*l_mpr_data);
                if (NULL != l_pd_data)
                    m_vec_DicomData.push_back(l_pd_data);
                
            }
        }
    }
}

CPDDataHandler& CPDDataHandler::operator = (CPDDataHandler& f_handler)
{
    if (this == &f_handler)
        return *this;

    return (*this);
}

//****************************Method Header************************************

//Method Name   :CPDDataHandler

//Author        :PATNI

//Purpose       :Destructor
//*****************************************************************************

CPDDataHandler::~CPDDataHandler()
{
    PD_TRACE(CPDDataHandler::~CPDDataHandler);
    int l_ni = 0;
    for (l_ni = 0; l_ni < (int)m_vec_VantageData.size(); l_ni++)
    {
        if (NULL != m_vec_VantageData[l_ni])
        {  
            delete m_vec_VantageData[l_ni];
            m_vec_VantageData[l_ni] = NULL;
        }
    }
    for (l_ni = 0; l_ni < (int)m_vec_DicomData.size(); l_ni++)
    {
        if (NULL != m_vec_DicomData[l_ni])
        {
            delete m_vec_DicomData[l_ni];
            m_vec_DicomData[l_ni] = NULL;
        }
    }
}
//sb/revw.. have copy constructor as class have pointer.
//////////////////////////////////////////////////////////////////////
// Data Handling Functions
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************

//Method Name   :SetVantageDataVector

//Author        :PATNI

//Purpose       :Sets values in vector for Vantage Data
//*****************************************************************************
int CPDDataHandler::SetVantageDataVector(DATAVECTOR & f_vec_Vantage)
{     
    PD_TRACE(CPDDataHandler::SetVantageDataVector);

    m_vec_VantageData = f_vec_Vantage;//sb/revw.. don't assign directly as vector contains object pointers.
    return(E_NO_ERROR);
}

//****************************Method Header************************************
//Method Name   :SetDicomDataVector

//Author        :PATNI

//Purpose       :Sets values in vector for Dicom Data
//*****************************************************************************
int CPDDataHandler::SetDicomDataVector(DATAVECTOR & f_vec_Dicom)
{
    PD_TRACE(CPDDataHandler::SetDicomDataVector);
    m_vec_DicomData = f_vec_Dicom;//sb/revw.. don't assign directly as vector contains object pointers.
    return(E_NO_ERROR);
}

//****************************Method Header************************************

//Method Name   :GetVantageData

//Author        :PATNI

//Purpose       :Returns Vantage data related to a slice plane
//*****************************************************************************
f32vec3_t CPDDataHandler::GetVantageData(int f_nSlicePlane,PDData_t f_nPlaneData) const
{
    PD_TRACE(CPDDataHandler::GetVantageData);
    f32vec3_t l_vec = {0};
    if ((0 <= f_nSlicePlane ) && (f_nSlicePlane < (int)(m_vec_VantageData.size())))
    {
        if (NULL != m_vec_VantageData[f_nSlicePlane])
        {
            return  m_vec_VantageData[f_nSlicePlane]->GetSliceVectorinfo(f_nPlaneData);
        }
        else
        {
            CString l_cserrmsg (_T("CPDDataHandler::GetVantageData:: PD Data is NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return l_vec;
        }
    } 
    else
    {
        CString l_cserrmsg (_T("GetVantageData: Invalid index for Slice Plane"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return l_vec;
    }
}

//****************************Method Header************************************

//Method Name   :GetDicomData

//Author        :PATNI

//Purpose       :Returns Dicom data related to a slice plane
//*****************************************************************************
f32vec3_t  CPDDataHandler::GetDicomData(int f_nSlicePlane,PDData_t f_nPlaneData) const
{
    PD_TRACE(CPDDataHandler::GetDicomData);
    f32vec3_t l_vec = {0};
    if ((0 <= f_nSlicePlane ) && (f_nSlicePlane < (int)(m_vec_DicomData.size())))
    {
        if (NULL != m_vec_VantageData[f_nSlicePlane])
        {
            return m_vec_DicomData[f_nSlicePlane]->GetSliceVectorinfo(f_nPlaneData);
        }
        else
        {
            CString l_cserrmsg (_T("CPDDataHandler::GetDicomData:: PD Data is NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return l_vec;
        }
    } 
    else
    {
        CString l_cserrmsg (_T("GetVantageData: Invalid index for Slice Plane"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return l_vec;
    }
}

//****************************Method Header************************************

//Method Name   :GetVantageSlicePlane

//Author        :PATNI

//Purpose       : Get the vantage sliceplane data
//*****************************************************************************
CString CPDDataHandler::GetVantageSlicePlane(int f_nSlicePlane)
{
    PD_TRACE(CPDDataHandler::GetVantageSlicePlane);
	CString l_csempty (_T("")); //AN
    if ((0 <= f_nSlicePlane ) && (f_nSlicePlane < (int)(m_vec_VantageData.size())))
    {
        if (NULL != m_vec_VantageData[f_nSlicePlane])
        {
            return m_vec_VantageData[f_nSlicePlane]->getPDPlan();
        }
        else
        {
            CString l_cserrmsg (_T("GetVantageSlicePlane: PD Data is NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(l_csempty);
        }
    }
    else
    {
        CString l_cserrmsg (_T("GetVantageSlicePlane: Invalid index for Slice Plane"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_csempty);
    }
}

//****************************Method Header************************************

//Method Name   :GetDicomSlicePlane

//Author        :PATNI

//Purpose       : Get the Dicom sliceplane data
//*****************************************************************************
CString CPDDataHandler::GetDicomSlicePlane(int f_nSlicePlane)
{
    PD_TRACE(CPDDataHandler::GetDicomSlicePlane);
	CString l_csempty (_T("")); //AN
    if ((0 <= f_nSlicePlane ) && (f_nSlicePlane < (int)(m_vec_DicomData.size())))
    {
        if (NULL != m_vec_DicomData[f_nSlicePlane])
        {
            return m_vec_DicomData[f_nSlicePlane]->getPDPlan();
        }
        else
        {
            CString l_cserrmsg (_T("GetDicomSlicePlane: PD Data is NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(l_csempty);
        }
    }
    else
    {
        CString l_cserrmsg (_T("GetDicomSlicePlane: Invalid index for Slice Plane"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(l_csempty);
    }
}

//****************************Method Header************************************

//Method Name   :GetDataSize

//Author        :PATNI

//Purpose       :Get size of the data
//*****************************************************************************
int CPDDataHandler::GetDataSize()
{
    PD_TRACE(CPDDataHandler::GetDataSize);
    int l_size=(int)m_vec_VantageData.size();
    return(l_size);
}

/*************************************************************
* Method:    	ConvertDICOMAxis
* Description: 	This function is used to convert values from vanatge axis to dicom axis
* Parameter: 	f32vec3_t & f_pfin_vector :
* Parameter: 	f32vec3_t & f_pfout_vector :
* Parameter: 	int f_nflag :
* Returns:   	void
*************************************************************/
void CPDDataHandler::ConvertDICOMAxis(const f32vec3_t &f_pfin_vector,f32vec3_t& f_pfout_vector,int f_nflag)
{
    PD_TRACE(CPDDataHandler::ConvertDICOMAxis);

    if(1 == f_nflag) {
        f_pfout_vector.x = (-1)*f_pfin_vector.x;
        f_pfout_vector.y = (-1)*f_pfin_vector.y;
        f_pfout_vector.z =  f_pfin_vector.z;
    }else if(2 == f_nflag) {
        f_pfout_vector.x=  (-1)*f_pfin_vector.x;
        f_pfout_vector.y =  f_pfin_vector.y;
        f_pfout_vector.z =  (-1)*f_pfin_vector.z;
    }else {
        CString l_cserrmsg (_T("ConvertDICOMAxis invalid flag. Cannot convert data"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    }
}

/*************************************************************
* Method:    	GetVantageData
* Description: 	This function is used to get handle to PD data of a sliceplane
* Parameter: 	int f_nSlicePlane :
* Returns:   	CPDCoordinates *
*************************************************************/
CPDCoordinates * CPDDataHandler::GetVantageData(int f_nSlicePlane)
{
    PD_TRACE(CPDDataHandler::GetVantageData);
    if ((0 <= f_nSlicePlane ) && (f_nSlicePlane < (int)(m_vec_VantageData.size())))
    {
        if (NULL != m_vec_VantageData[f_nSlicePlane])
        {
            return m_vec_VantageData[f_nSlicePlane];
        }
        else
        {
            CString l_cserrmsg (_T("GetVantageData: PD Data is NULL"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(NULL);
        }
    }
    else
    {
        CString l_cserrmsg (_T("GetDicomSlicePlane: Invalid index for Slice Plane"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(NULL);
    }
}

////////////////////////////////    End of File     /////////////////////////////