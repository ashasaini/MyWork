// CDisplayData.cpp: implementation of the CDisplayData class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrep.h"
#include "DisplayData.h"
#include "TiPrepView.h"
#include "AvgFilter.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include <LIMITS.H>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#include <direct.h>
using namespace std;

extern int time_data[];

/**
* A constructor.
* This function is used for initialization of class variables.
*/
CDisplayData::CDisplayData() : 
m_nNum_Images_selected(0),
m_dnoiseOffsetVal(0),
m_nNumOfPoints(0),
m_nSelectedIndex(0),
m_nNewGeneratedXPoints(0),
m_csStudyFileName(_T("")),
m_nTime_stamp(NULL),
m_dROI_Org_Data(NULL),
m_dROI_Inv_Data(NULL),
m_dROI_Inv_Data_Min_Reversed(NULL),
m_dROI_Fit_Data(NULL),
m_dROI_Fit_Data_Min_Reversed(NULL),
m_dROI_TI_Val(NULL),
m_dROI_TI_Val_Min_Reversed(NULL),
m_dROI_T1_Val(NULL),
m_dROI_T1_Val_Min_Reversed(NULL),
m_dROI_Fit_Err(NULL),
m_dROI_Fit_Err_Min_Reversed(NULL),
m_dNoise_ROI_Org_Data(NULL)
{
    /*TIPREP_TRACE(CDisplayData::CDisplayData);*/
    //
}
/**
* A destructor.
* This function is used for deleting the memory if used.
*/
CDisplayData::~CDisplayData()
{
    /*TIPREP_TRACE(CDisplayData::~CDisplayData);*/
    DeAllocateMemory();
    DeallocateBufferXY();
    m_csImageLoid.clear();
}
/*************************************************************
* Method:    	CDisplayData
* Description: 	This function is used 
* Parameter: 	CDisplayData & f_DisplayData :
* Returns:   	
*************************************************************/
CDisplayData::CDisplayData(CDisplayData & f_DisplayData)
{
    /*TIPREP_TRACE(CDisplayData::CDisplayData(CDisplayData &));*/
    int l_nStatus = setImgInfo(f_DisplayData.m_nNum_Images_selected, 
        f_DisplayData.m_csImageLoid);
    
    if(E_NO_ERROR != l_nStatus) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("CDisplayData(): setImgInfo() returned error"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	operator=
* Description: 	This function is used 
* Parameter: 	CDisplayData & f_DisplayData :
* Returns:   	CDisplayData&
*************************************************************/
CDisplayData& CDisplayData::operator = (CDisplayData& f_DisplayData)
{
    /*TIPREP_TRACE(CDisplayData::operator);*/
    
    if(this == &f_DisplayData) {
        return(*this);
    }
    
    int l_nStatus = setImgInfo(f_DisplayData.m_nNum_Images_selected, 
        f_DisplayData.m_csImageLoid);
    
    if(E_NO_ERROR != l_nStatus) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setImgInfo() returned error"), __LINE__, __FILE__);
    }
    
    return (*this);
}


///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////



/*************************************************************
* Method:    	setROIData
* Description: 	This function is used to set ROI original data.
* Returns:   	int
*************************************************************/
int CDisplayData::setROIData()
{
    TIPREP_TRACE(CDisplayData::setROIData,MRTraceMarker3);
    
    if(NULL == m_nTime_stamp) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setROIData(): Time Stamp data NULL"), __LINE__, __FILE__);
        return E_ERROR;
    }
    
    try {
        m_nNumOfPoints = MAX_NUM_ROI;
        m_nNum_Images_selected = m_csImageLoid.size();
        int l_nMemStatus = AllocateMemory();
        
        if(E_NO_ERROR != l_nMemStatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("setROIData(): Memory allocation for dala buffers failed"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        l_nMemStatus = MemSetROIData();
        
        if(E_NO_ERROR != l_nMemStatus) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("setROIData(): MemSetROIData failed"), __LINE__, __FILE__);
            DeAllocateMemory();
            return(E_ERROR);
        }
        
//        double l_dFactB = 0.0;
        int l_nRoi = 0;
        void *l_pImgData = NULL;
        long AccImage = 0;
        int l_nsize = 0;
        byte* l_bImROI[MAX_LINE_ROI] = {NULL, NULL, NULL, NULL, NULL}; 
        memset(l_bImROI, 0x0, (MAX_LINE_ROI * sizeof(byte*))); 
        int l_nROIH = 0;
        int l_nROIW = 0;
        int l_nj = 0;
        unsigned short l_nlData_Type = 16;
        BOOL l_bstatus = (CTiPrepView::GetView())->GetROIInstance()->GetROIBufferInfo
            (l_bImROI, &l_nROIH, &l_nROIW);
        
        if(!l_bstatus) {
            CString l_csErrMsg(_T("Unable to get image map."));
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {  
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            DeAllocateMemory();
            return E_ROI_MAP;
        }
        
        if(NULL == l_bImROI[0]) {
            CString l_csErrMsg(_T("setROIData(): Map is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {  
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            DeAllocateMemory();
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        int l_nCount = 0;
        CLocalDBMgr l_localdbmgr;
        for(l_nj ; l_nj < (signed) m_csImageLoid.size(); l_nj++) {
            int l_nTime_stamp = 0;
            float l_fScale = 0.0;
//             int status = (CTiPrepView::GetView())->GetLocalDBImgData
//                 (m_csImageLoid.at(l_nj), &l_pImgData, &l_nsize, 
//                 &l_fScale, &l_nTime_stamp, &l_nlData_Type,l_nj);//sb
            int status = l_localdbmgr.getImgDataFromDB(m_csImageLoid.at(l_nj), &l_pImgData, &l_nsize, 
                &l_fScale, &l_nTime_stamp, &l_nlData_Type,l_nj);//sb
            
            if(status != E_NO_ERROR) {
                //Noise Correction feature development/loop should execute for all rois
                for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {  
                    CoTaskMemFree(l_bImROI[l_nRoi]);
                    l_bImROI[l_nRoi] = NULL;
                }
                
                if(l_pImgData) {
                    /*free*/operator delete(l_pImgData);
                    l_pImgData = NULL;
                }
                
                DeAllocateMemory();
                return status;
            }
            
            m_nTime_stamp[l_nj] = l_nTime_stamp;
            
            for(l_nRoi = 0; l_nRoi < MAX_NUM_ROI; l_nRoi++) {
                l_nCount = 0;
                m_dROI_Org_Data[l_nRoi][l_nj] = 0;
                
                for(int l_nq = 0; l_nq < l_nROIH; l_nq++) {
                    for(int l_nr = 0; l_nr < l_nROIW; l_nr++) {
                        int nIndex = (l_nROIW) * l_nq + l_nr;
                        
                        if(1 == l_bImROI[l_nRoi][nIndex]) {
                            switch(l_nlData_Type) {
                            case 16: {
                                INT16 temp = 0;
                                memcpy(&temp, 
                                    ((unsigned char*)(l_pImgData) + nIndex * sizeof(INT16)), sizeof(INT16));
                                AccImage = AccImage + temp;
                                l_nCount++;
                                break;
                                     }
                            case 32: {
                                INT32 temp = 0;
                                memcpy(&temp, 
                                    ((unsigned char*)(l_pImgData) + nIndex * sizeof(INT32)), sizeof(INT32));
                                AccImage = AccImage + temp;
                                l_nCount++;
                                break;
                                     }
                            default:
                                CTiPrepLogHelper::WriteToErrorLog
                                    (_T("setROIData(): Default case executed. Unknown data type"), 
                                    __LINE__, __FILE__); 
                                break;
                            }
                        }
                    }
                }
                
                if(0.00001 < fabs(l_fScale * l_nCount)) { 
                    m_dROI_Org_Data[l_nRoi][l_nj] = (double)AccImage / (l_nCount * l_fScale);
                }
                
                AccImage = 0;
                
                if(0 == l_nCount) {
                    CString l_cserr(_T(""));
                    
                    for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {  
                        CoTaskMemFree(l_bImROI[l_nRoi]);
                        l_bImROI[l_nRoi] = NULL;
                    }
                    
                    if(l_pImgData) {
                        /*free*/operator delete(l_pImgData);
                        l_pImgData = NULL;
                    }
                    
                    DeAllocateMemory();
                    return I_ROI_OUT_RGN;
                }
            }
            
            if(l_pImgData) {
                /*free*/operator delete(l_pImgData);
                l_pImgData = NULL;
            }
        }
        
        ValidateAndSetTimeStamp(m_nTime_stamp,(signed) m_csImageLoid.size());//AS/added

        for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {  
            CoTaskMemFree(l_bImROI[l_nRoi]);
            l_bImROI[l_nRoi] = NULL;
        }
        
        if(FALSE == setROI_Inv_Data(m_nNum_Images_selected)) {
            CTiPrepLogHelper::WriteToErrorLog(_T("setROI_Inv_Data failed"),__LINE__,__FILE__);
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            if(NULL != l_pImgData) {
                /*free*/operator delete(l_pImgData);
                l_pImgData = NULL;
            }
            
            DeAllocateMemory();
            return E_ERROR;
        }
        
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        
        l_csStrFormatted = _T("setROIData(): Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(CFileException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        
        l_csStrFormatted = _T("setROIData(): Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        
        l_csStrFormatted = _T("setROIData(): Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in setROIData"), __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       setNoiseROIDataFromRoi
* Description:  This function is used to read the roi data from
*               image and set Noise ROI original data.
* Returns:      int
*************************************************************/
int CDisplayData::setNoiseROIDataFromRoi()
{
    try {
        TIPREP_TRACE(CDisplayData::setNoiseROIDataFromRoi,MRTraceMarker3);
        
        m_nNum_Images_selected = m_csImageLoid.size();
        int l_nRoi = 0;
        void *l_pImgData = NULL;
        long AccImage = 0;
        int l_nsize = 0;
        byte* l_bImROI[MAX_LINE_ROI] = {NULL, NULL, NULL, NULL, NULL};
        memset(l_bImROI, 0x0, (MAX_LINE_ROI * sizeof(byte*)));
        int l_nROIH = 0;
        int l_nROIW = 0;
        int l_nj = 0;
        unsigned short l_nlData_Type = 16;
        BOOL l_bstatus = CTiPrepView::GetView()->GetROIInstance()->GetROIBufferInfo
            (l_bImROI, &l_nROIH, &l_nROIW);
        
        if(!l_bstatus) {
            CString l_csErrMsg(_T("Unable to get image map."));
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            DeAllocateMemory();
            return E_ROI_MAP;
        }
        
        if(NULL == l_bImROI[0]) {
            CString l_csErrMsg(_T("Map is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            DeAllocateMemory();
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        int l_nCount = 0;
        
        for(l_nRoi = 0; l_nRoi < MAX_NOISE_ROI; l_nRoi++) {
            if(NULL != m_dNoise_ROI_Org_Data[l_nRoi]) {
                memset(m_dNoise_ROI_Org_Data[l_nRoi], 0, m_nNum_Images_selected * sizeof(double));  
            } else {
                CString l_csErrMsg(_T(""));
                l_csErrMsg.Format
                    (_T("setNoiseROIDataFromRoi(): m_dNoise_ROI_Org_Data[%d] found NULL"), l_nRoi);
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                
                for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    CoTaskMemFree(l_bImROI[l_nRoi]);
                    l_bImROI[l_nRoi] = NULL;
                }
                
                DeAllocateMemory();
                return E_ERROR;
            }
        }
        CLocalDBMgr l_localdbmgr;

        for(l_nj ; l_nj < (signed) m_csImageLoid.size(); l_nj++) {
            int l_nTime_stamp = 0;
            float l_fScale = 0.0;
//             int status = CTiPrepView::GetView()->GetLocalDBImgData
//                 (m_csImageLoid.at(l_nj), &l_pImgData, &l_nsize, &l_fScale, &l_nTime_stamp, &l_nlData_Type,l_nj);//sb
            int status = l_localdbmgr.getImgDataFromDB(m_csImageLoid.at(l_nj), 
                &l_pImgData, &l_nsize, &l_fScale, &l_nTime_stamp, &l_nlData_Type,l_nj);//sb
            
            if(status != E_NO_ERROR) {
                
                if(l_pImgData) {
                    /*free*/operator delete(l_pImgData);
                    l_pImgData = NULL;
                }
                
                for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    CoTaskMemFree(l_bImROI[l_nRoi]);
                    l_bImROI[l_nRoi] = NULL;
                }
                
                DeAllocateMemory();
                return status;
            }
            
            m_nTime_stamp[l_nj] = l_nTime_stamp;
            int l_noiseData_index = 0;
            
            for(l_noiseData_index = 0, l_nRoi = MAX_NUM_ROI; 
            l_nRoi < MAX_LINE_ROI; l_nRoi++, l_noiseData_index++) { 
                l_nCount = 0;
                m_dNoise_ROI_Org_Data[l_noiseData_index][l_nj] = 0;
                
                for(int l_nq = 0; l_nq < l_nROIH; l_nq++) {
                    for(int l_nr = 0; l_nr < l_nROIW; l_nr++) {
                        int nIndex = (l_nROIW) * l_nq + l_nr;
                        
                        if(1 == l_bImROI[l_nRoi][nIndex]) {
                            switch(l_nlData_Type) {
                            case 16: {
                                INT16 temp = 0;
                                memcpy(&temp, ((unsigned char*)(l_pImgData) + nIndex * 2), sizeof(INT16));
                                AccImage = AccImage + temp;
                                l_nCount++;
                                break;
                                     }
                            case 32: {
                                INT32 temp = 0;
                                memcpy(&temp, ((unsigned char*)(l_pImgData) + nIndex * 2), sizeof(INT32));
                                AccImage = AccImage + temp;
                                l_nCount++;
                                break;
                                     }
                            default:
                                CTiPrepLogHelper::WriteToErrorLog
                                    (_T("setNoiseROIDataFromRoi(): Default case executed. Unknown data type"), __LINE__, __FILE__);
                                break;
                            }
                        }
                    }
                }
                
                if(0.00001 < fabs(l_fScale * l_nCount)) {
                    m_dNoise_ROI_Org_Data[l_noiseData_index][l_nj] = 
                        (double)AccImage / (l_nCount * l_fScale);
                }
                
                AccImage = 0;
                
                if(0 == l_nCount) {
                    CString l_cserr(_T(""));
                    l_cserr = _T("ROI is placed outside of image region");
                    
                    if(l_pImgData) {
                        /*free*/operator delete(l_pImgData);
                        l_pImgData = NULL;
                    }
                    
                    for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                        CoTaskMemFree(l_bImROI[l_nRoi]);
                        l_bImROI[l_nRoi] = NULL;
                    }
                    
                    DeAllocateMemory();
                    return I_ROI_OUT_RGN;
                }
            }
            
            if(l_pImgData) {
                /*free*/operator delete(l_pImgData);
                l_pImgData = NULL;
            }
        }
        ValidateAndSetTimeStamp(m_nTime_stamp,(signed) m_csImageLoid.size());//AS/added
        
        for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
            CoTaskMemFree(l_bImROI[l_nRoi]);
            l_bImROI[l_nRoi] = NULL;
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = 
            _T("setNoiseROIDataFromRoi(): Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(CFileException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("setNoiseROIDataFromRoi(): Exception occurred ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("setNoiseROIDataFromRoi():  Exception occurred");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setNoiseROIDataFromRoi(): Exception occurred in "), __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}


/*************************************************************
* Method:       setNoiseOffsetVal
* Description:  This function sets the Noise offset Value in
*               class variable
* Parameter:    double f_noiseOffsetVal
*               Contains the noise offset value(mean value of
*               noise org data).
* Returns:      void
*************************************************************/
void CDisplayData::setNoiseOffsetVal(double f_noiseOffsetVal)
{
    TIPREP_TRACE(CDisplayData::setNoiseOffsetVal,MRTraceMarker8);
    m_dnoiseOffsetVal = f_noiseOffsetVal;
}

/*************************************************************
* Method:       setRoiOrgData
* Description:  This function sets the value of Roi org data in
*               class variable from a parameter passed as a
*               parameter.
* Parameter:    double * * f_dRoi_Org_Data
*               Holds the pointer to ROI org data.
* Returns:      void
*************************************************************/
void CDisplayData::setRoiOrgData(double **f_dRoi_Org_Data)
{
    if(NULL != f_dRoi_Org_Data) {
        for(int l_nRoi = 0; l_nRoi < m_nNumOfPoints; l_nRoi++) { 
            for(int l_nImg = 0; l_nImg < m_nNum_Images_selected; l_nImg++) {
                m_dROI_Org_Data[l_nRoi][l_nImg] = f_dRoi_Org_Data[l_nRoi][l_nImg];
            }
        }
    } else {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setRoiOrgData(): Function parameter NULL error"), __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       setNoiseRoiOrgData
* Description:  This function sets the value of Noise Roi org
*               data in class variable from a parameter passed
*               as a parameter.
* Parameter:    double * * f_dRoi_Org_Data
*               Contains number of images select on ISelector.
* Returns:      void
*************************************************************/
void CDisplayData::setNoiseRoiOrgData(double **f_dNoise_Roi_Org_Data)
{
    if(NULL != f_dNoise_Roi_Org_Data) {
        for(int l_nRoi = 0; l_nRoi < MAX_NOISE_ROI; l_nRoi++) {
            for(int l_nImg = 0; l_nImg < m_nNum_Images_selected; l_nImg++) {
                m_dNoise_ROI_Org_Data[l_nRoi][l_nImg] = f_dNoise_Roi_Org_Data[l_nRoi][l_nImg];
            }
        }
    } else {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setNoiseRoiOrgData(): Function parameter NULL error"), __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       calcNoiseROIMean
* Description:  This function calculates the average(mean)
*               value of Noise org data of whichever noise roi
*               is selected.
* Parameter:    BOOL * f_bNoiseRoi
*               Pointer to an array which holds the state
*               (checked/unchecked) of noise roi check box.
* Parameter:    CString & f_csMean_NoiseRoiVal
*               Holds the Noise average value calculated in the
*               function.
* Returns:      int
*************************************************************/
int  CDisplayData::calcNoiseROIMean(BOOL *f_bNoiseRoi, CString &f_csMean_NoiseRoiVal)
{
    TIPREP_TRACE(CDisplayData::calcNoiseROIMean,MRTraceMarker3);
    
    try {
        double sum = 0.0;
        int cnt = 0;
        double l_dnoiseAvgVal = 0.0;
        double **l_dNoise_Org_Data = NULL;
        long l_NumImages = 0;
        int l_nnumRoi = 0;
        
        if(NULL == f_bNoiseRoi) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("calcNoiseROIMean(): Function parameter found NULL"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        if((false == f_bNoiseRoi[0]) && (false == f_bNoiseRoi[1])) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("calcNoiseROIMean(): No ROI is selected"), __LINE__, __FILE__);
            l_dnoiseAvgVal = 0.0;
            setNoiseOffsetVal(l_dnoiseAvgVal);
            return(E_NO_ERROR); 
            //E_NO_ERROR because it is not an error condition
            //following code need not be executed in case both the noise rois are off
        }
        
        l_dNoise_Org_Data = getNoiseROIOrgData();
        
        if(NULL == l_dNoise_Org_Data) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("calcNoiseROIMean(): l_dNoise_Org_Data is NULL"), __LINE__, __FILE__);
            l_dnoiseAvgVal = 0.0;
            setNoiseOffsetVal(l_dnoiseAvgVal);
            return(E_ERROR);
        }
        
        getSelImgNum(&l_NumImages);
        
        if(l_NumImages <= 0) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("calcNoiseROIMean(): Number of images is incorrect"), __LINE__, __FILE__);
            l_dnoiseAvgVal = 0.0;
            setNoiseOffsetVal(l_dnoiseAvgVal);
            return(E_ERROR);
        }
        
        for(int l_ni = 0; l_ni < MAX_NOISE_ROI; l_ni++) {
            if(TRUE == f_bNoiseRoi[l_ni]) { 
                l_nnumRoi = l_nnumRoi + 1;
                
                for(cnt = 0; cnt < l_NumImages; cnt++) {
                    sum = sum + l_dNoise_Org_Data[l_ni][cnt];
                }
            }
        }
        
        l_dnoiseAvgVal = (sum / l_NumImages);
        
        if(l_nnumRoi != 0) {
            l_dnoiseAvgVal = l_dnoiseAvgVal / l_nnumRoi;
            setNoiseOffsetVal(l_dnoiseAvgVal);
        } else { //implies no noise roi is selected
            l_dnoiseAvgVal = 0.0;
            setNoiseOffsetVal(l_dnoiseAvgVal);
            CTiPrepLogHelper::WriteToErrorLog
                (_T("calcNoiseROIMean(): No ROI is selected"), __LINE__, __FILE__);
            //no need to return error
        }
        
        //Print Avg value
        CString l_cs_temp_str(_T(""));
        l_cs_temp_str.Format(_T("%f"), l_dnoiseAvgVal);
        f_csMean_NoiseRoiVal = l_cs_temp_str;
    } catch(CMemoryException& e) {   
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory Exception in calcNoiseROIMean");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred in calcNoiseROIMean");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(...) {
        CString l_cserrmsg("Exception occurred in calcNoiseROIMean");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       performNoiseCorrection
* Description:  This function subtracts the Noise offset value
*               from ROI Org data and Noise ROI org data.
*               Noise corrected ROI org data and Noise ROI Org
*               data is then set in the respective class members.
* Returns:      int
*************************************************************/
int CDisplayData::performNoiseCorrection()
{
    TIPREP_TRACE(CDisplayData::performNoiseCorrection,MRTraceMarker3);
    
    try { 
        double **l_dRoi_Org_Data = NULL; 
        double **l_dNoise_Roi_Org_Data = NULL;
        double l_dnoiseOffset = 0.0;
        long l_nNumImg = 0;
        l_dRoi_Org_Data = getROIOrgData();
        l_dNoise_Roi_Org_Data = getNoiseROIOrgData();
        
        if((NULL == l_dRoi_Org_Data) || (NULL == l_dNoise_Roi_Org_Data)) {
            CString l_cserrmsg(_T("Null Pointer error"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        l_dnoiseOffset = getNoiseOffsetVal();
        getSelImgNum(&l_nNumImg);
        
        if(l_nNumImg <= 0) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("performNoiseCorrection(): Number of images is '0'"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        for(int l_nroi = 0; l_nroi < m_nNumOfPoints; l_nroi++) { 
            for(int l_nimg = 0; l_nimg < l_nNumImg; l_nimg++) {
                l_dRoi_Org_Data[l_nroi][l_nimg] = l_dRoi_Org_Data[l_nroi][l_nimg] - l_dnoiseOffset;
            }
        }
        
        for(l_nroi = 0; l_nroi < MAX_NOISE_ROI; l_nroi++) {
            for(int l_nimg = 0; l_nimg < l_nNumImg; l_nimg++) {
                l_dNoise_Roi_Org_Data[l_nroi][l_nimg] = 
                    l_dNoise_Roi_Org_Data[l_nroi][l_nimg] - l_dnoiseOffset;
            }
        }
        
        setRoiOrgData(l_dRoi_Org_Data);
        setNoiseRoiOrgData(l_dNoise_Roi_Org_Data);
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory Exception in performNoiseCorrection");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception Occurred in performNoiseCorrection");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(...) {
        CString l_cserrmsg("Exception occurred in performNoiseCorrection");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       setImgInfo
* Description:  This function sets number of images selected by
*               user on ISelector and stores image LOID
*               for each image.
* Parameter:    long f_lnNum_img
*               Contains number of images select on ISelector.
* Parameter:    vector <CString> f_csImgLOID
*               Contains LOID for each image.
* Returns:      void
*************************************************************/
int CDisplayData::setImgInfo(long f_lnNum_img,
                             /*in*/vector <CString> & f_csImgLOID)
{
    TIPREP_TRACE(CDisplayData::setImgInfo,MRTraceMarker3);
    
    try {
        m_nNum_Images_selected = f_lnNum_img;
        vector<CString>::iterator l_csit1;
        m_csImageLoid.clear();
        l_csit1 = f_csImgLOID.begin();
        
        for(l_csit1 = f_csImgLOID.begin() ; l_csit1 < f_csImgLOID.end(); l_csit1++) {
            m_csImageLoid.push_back(*l_csit1);
        }
    } catch(...) {
        CString l_cserrmsg("Exception occurred in setImgInfo");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       setTimeStampData
* Description:  This function is used to save timestamp data
*               related to ROI data.
* Parameter:    int f_nselectImg
*               Contains number of images select on ISelector.
* Returns:      void
*************************************************************/
int CDisplayData::setTimeStampData(int f_nselectImg)
{
    TIPREP_TRACE(CDisplayData::setTimeStampData,MRTraceMarker8);
    
    try{
        if(NULL != m_nTime_stamp) {
            delete [] m_nTime_stamp;
            m_nTime_stamp = NULL;
        }
        
        if(0 < f_nselectImg) {
            m_nTime_stamp = new int [f_nselectImg];
            
            if(NULL == m_nTime_stamp) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("setTimeStampData(): Memory allocation failed"), __LINE__, __FILE__);
                return E_ERROR;
            }
        }
        
        return E_NO_ERROR;
    }
    catch(...) {
        CString l_cserrmsg("Exception occurred in setTimeStampData");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
}

/*************************************************************
* Method:       setROI_Inv_Data
* Description:  This function sets inverted ROI data.
* Parameter:    int f_nselectImg
*               Contains number of images select on ISelector.
* Returns:      BOOL
*************************************************************/
BOOL CDisplayData::setROI_Inv_Data(int f_nselectImg)
{
    TIPREP_TRACE(CDisplayData::setROI_Inv_Data,MRTraceMarker8);
    bool l_bStatus = true;
    
    for(int l_ni = 0; l_ni < m_nNumOfPoints; l_ni++) {
        l_bStatus = MinReverseData(l_ni, f_nselectImg);
        
        if(true != l_bStatus) {
            return(FALSE);
        }
    }
    
    return TRUE;
}

/*************************************************************
* Method:       getSelImgNum
* Description:  This function gives number of images selected
*               on ISelector.
* Parameter:    long * f_nNum_img
*               Out parameter to get number of images selected.
* Returns:      void
*************************************************************/
void CDisplayData:: getSelImgNum(long *f_nNum_img)
{
    TIPREP_TRACE(CDisplayData::getSelImgNum,MRTraceMarker8);
    
    if(m_nNewGeneratedXPoints == 0) {
        *f_nNum_img = m_nNum_Images_selected;
    } else {
        *f_nNum_img = m_nNewGeneratedXPoints;
    }
    
    return;
}

/*************************************************************
* Method:       getSelImgLOID
* Description:  This function gives LOID of images selected
*               on ISelector.
* Parameter:    vector <CString> & f_csImgLOID
*               Out parameter to get LOID of selected images
*               on ISelector.
* Returns:      void
*************************************************************/
int CDisplayData:: getSelImgLOID(/*out*/vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CDisplayData::getSelImgLOID,MRTraceMarker3);
    
    try { 
        vector<CString>::iterator l_csit1;
        l_csit1 = m_csImageLoid.begin();
        f_csImgLOID.clear();
        
        for(l_csit1 = m_csImageLoid.begin() ; l_csit1 < m_csImageLoid.end(); l_csit1++) {
            f_csImgLOID.push_back(*l_csit1);
        }
    } catch(...) { 
        CString l_cserrmsg("Exception occurred in getSelImgLOID");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       getMinPos
* Description:  This function is used to get minimum value
*               position in ROI data.
* Parameter:    int f_nROI
*               Contains ROI number.
* Parameter:    int f_nselectImg
*               Contains number of selected images on ISelector.
* Parameter:    int * f_nMarker
* Returns:      bool
*************************************************************/
bool CDisplayData::getMinPos(int f_nROI, int f_nselectImg, int *f_nMarker)  
{
    TIPREP_TRACE(CDisplayData::getMinPos,MRTraceMarker9);
    
    try { 
        int l_nMarker = 0;
        double *l_dTempArray = NULL;
        l_dTempArray = new double[f_nselectImg];
        
        if(NULL == l_dTempArray) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Memory allocation failed"), __LINE__, __FILE__);
            return(false);
        }
        
        for(int l_ni = 0; l_ni < f_nselectImg; l_ni++) {
            l_dTempArray[l_ni] = m_dROI_Org_Data[f_nROI][l_ni];
        }
        
        for(l_ni = 0; l_ni < f_nselectImg; l_ni++) {
            int l_nIndex_of_min = l_ni;
            
            for(int l_ny = l_ni; l_ny < f_nselectImg; l_ny++) {
                if(l_dTempArray[l_nIndex_of_min] > l_dTempArray[l_ny]) {
                    l_nIndex_of_min = l_ny;
                }
            }
            
            double l_dTemp = l_dTempArray[l_ni];
            l_dTempArray[l_ni] = l_dTempArray[l_nIndex_of_min];
            l_dTempArray[l_nIndex_of_min] = l_dTemp;
        }
        
        for(l_ni = 0; l_ni < f_nselectImg; l_ni++) {
            if(l_dTempArray[0] == m_dROI_Org_Data[f_nROI][l_ni]) {
                l_nMarker = l_ni;
            }
        }
        
        (*f_nMarker) = l_nMarker;
        
        if(NULL != l_dTempArray) {
            delete []l_dTempArray;
            l_dTempArray = NULL;
        }
    } catch(...) {
        CString l_cserrmsg("Exception occurred in getMinPos");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(false);
    }
    
    return(true);
}

/*************************************************************
* Method:       MinReverseData
* Description:  This function Inverts original data from
*               minIndex position.
* Parameter:    int f_nROI
*               Number of ROI.
* Parameter:    int f_nselectImg
*               Contains number of selected images on ISelector.
* Returns:      bool
*************************************************************/
bool CDisplayData::MinReverseData(int f_nROI, int f_nselectImg) 
{
    TIPREP_TRACE(CDisplayData::MinReverseData,MRTraceMarker9);
    
    try { 
        int l_nMinPos;
        bool l_bStatus = getMinPos(f_nROI, f_nselectImg, &l_nMinPos);
        
        if(true != l_bStatus) {
            CTiPrepLogHelper::WriteToErrorLog(_T("getMinPos returned error"), __LINE__, __FILE__);
            return(false);
        }
        
        //restrict inverse data creation in case of real or imaginary data type[st]
        //vector<CString> l_csloids;       


        for(int l_ni = 0; l_ni < f_nselectImg; l_ni++) {
            //---------------------------Data Inversion--------------------------------------------- 
            if(l_ni < l_nMinPos) {//before detected Min position
                if(0 < m_dROI_Org_Data[f_nROI][l_ni]){
                    m_dROI_Inv_Data[f_nROI][l_ni] = (-1) * m_dROI_Org_Data[f_nROI][l_ni];
                }else {
                    m_dROI_Inv_Data[f_nROI][l_ni] = m_dROI_Org_Data[f_nROI][l_ni];//KJ-JULY
                }
            } else {
                m_dROI_Inv_Data[f_nROI][l_ni] = m_dROI_Org_Data[f_nROI][l_ni];
            }
            //-----------------------------------------------------------------------------------------
            //do not invert if already first point is the minimum value: Defect of graph
            if(l_nMinPos == 0) {
                m_dROI_Inv_Data_Min_Reversed[f_nROI][l_ni] = m_dROI_Org_Data[f_nROI][l_ni];
                continue;
            }
            
            if(l_ni <= l_nMinPos) {//with detected Min position
                if(0 < m_dROI_Org_Data[f_nROI][l_ni]){
                    m_dROI_Inv_Data_Min_Reversed[f_nROI][l_ni] = (-1) * m_dROI_Org_Data[f_nROI][l_ni];
                }else{
                    m_dROI_Inv_Data_Min_Reversed[f_nROI][l_ni] =  m_dROI_Org_Data[f_nROI][l_ni];//KJ-JULY
                }
            } else {
                m_dROI_Inv_Data_Min_Reversed[f_nROI][l_ni] = m_dROI_Org_Data[f_nROI][l_ni];
            }
        }
    } catch(...) {
        CString l_cserrmsg("Exception occurred in MinReverseData");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(false);
    }
    
    return(true);
}

/*************************************************************
* Method:       getMinMaxForYaxis
* Description:  This function is used to get minimum limit and
*               maximum limit for Y-axis which is used
*               for graph plotting.
* Parameter:    int f_nselectImg
*               Contains number of selected images on ISelector.
* Parameter:    BOOL * f_roi
* Parameter:    BOOL * f_DisplayOption
* Parameter:    BOOL * f_RevMin
* Parameter:    int * f_nYmax
*               Out parameter is used to max limit of Y-axis.
* Parameter:    int * f_nYmin
*               Out parameter is used to min limit of Y-axis.
* Returns:      int
*               '0' on success.
*************************************************************/
int CDisplayData::getMinMaxForYaxis(int f_nselectImg, BOOL *f_roi, 
                                    BOOL *f_DisplayOption, 
                                    BOOL *f_RevMin,
                                    int &f_nYmax, int &f_nYmin)
{
    TIPREP_TRACE(CDisplayData::getMinMaxForYaxis,MRTraceMarker3);
    
    try {
        int     i = 0;
        double  min = 0.0;
        double  max = 0.0;
        int roi = 0;
        max  = -10.0;
        min = 10.0;
        f_nYmax = 10;
        f_nYmin = -10;
        
        if(f_nselectImg <= 0) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("getMinMaxForYaxis(): Incorrect number of images"), __LINE__, __FILE__); 
            return(E_ERROR);
        }
        
        if(m_nNum_Images_selected <= 0) {
            m_nNum_Images_selected = m_csImageLoid.size();
            
            if(m_nNum_Images_selected <= 0) {
                return(E_ERROR); 
            }
        }
        
        if((NULL == f_roi) || (NULL == f_DisplayOption) || 
            (NULL == f_RevMin) /*|| (NULL == f_nYmax) || (NULL == f_nYmin)*/) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("getMinMaxForYaxis(): Function parameter NULL"), __LINE__, __FILE__); 
            return(E_ERROR);
        }
        
        if((NULL == m_dROI_Fit_Data_Min_Reversed) || (NULL == m_dROI_Org_Data) ||
            (NULL == m_dROI_Fit_Data) || (NULL == m_dROI_Inv_Data_Min_Reversed) ||
            (NULL == m_dROI_Inv_Data) || (NULL == m_dNoise_ROI_Org_Data)) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("getMinMaxForYaxis(): Data found NULL"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        for(i = 0; i < m_nNumOfPoints; i++) {
            if(f_roi[i]) {
                if(f_DisplayOption[ORG_DATA]) {
                    getMinMaxInArray(m_dROI_Org_Data[i], m_nNum_Images_selected, &min, &max);
                    if (E_ERROR == CheckIntegerLimits(min ,max)){
                        memset(m_dROI_Org_Data[i],0x0,sizeof(double)*m_nNum_Images_selected);
                        min = 0.0;
                        max = 0.0;
                    }
                }
                
                if(f_DisplayOption[FIT_DATA]) {
                    if(f_RevMin[i]) {
                        getMinMaxInArray(m_dROI_Fit_Data_Min_Reversed[i], f_nselectImg, &min, &max);
                        if (E_ERROR == CheckIntegerLimits(min ,max)){
                            memset(m_dROI_Fit_Data_Min_Reversed[i],0x0,sizeof(double)*f_nselectImg);
                            min = 0.0;
                            max = 0.0;
                            m_dROI_T1_Val_Min_Reversed[i] = -1.0;
                        }

                    } else {
                        getMinMaxInArray(m_dROI_Fit_Data[i], f_nselectImg, &min, &max);
                        if (E_ERROR == CheckIntegerLimits(min ,max)){
                            memset(m_dROI_Fit_Data[i],0x0,sizeof(double)*f_nselectImg);
                            min = 0.0;
                            max = 0.0;
                            m_dROI_T1_Val[i] = -1.0;
                        }

                    }
                }
                
                if(f_DisplayOption[INV_DATA]) {
                    if(f_RevMin[i]) {
                        getMinMaxInArray(m_dROI_Inv_Data_Min_Reversed[i], 
                            m_nNum_Images_selected, &min, &max);
                        if (E_ERROR == CheckIntegerLimits(min ,max)){
                            memset(m_dROI_Inv_Data_Min_Reversed[i],0x0,sizeof(double)*m_nNum_Images_selected);
                            min = 0.0;
                            max = 0.0;
                        }
                    } else {
                        getMinMaxInArray(m_dROI_Inv_Data[i], m_nNum_Images_selected, &min, &max);
                        if (E_ERROR == CheckIntegerLimits(min ,max)){
                            memset(m_dROI_Inv_Data[i],0x0,sizeof(double)*m_nNum_Images_selected);
                            min = 0.0;
                            max = 0.0;
                        }
                    }
                }
            }
        }
        
        for(roi = 0; roi < MAX_NOISE_ROI; roi++) {
            if(f_DisplayOption[ORG_DATA]) {                
                if(f_roi[MAX_NUM_ROI+roi]) {
                    //f_roi[MAX_NUM_ROI+roi] represents f_roi[3] & f_roi[4] i.e to check for noise rois
                    if(NULL != m_dNoise_ROI_Org_Data[roi]) {                    
                        getMinMaxInArray(m_dNoise_ROI_Org_Data[roi], m_nNum_Images_selected, &min, &max);
                        if (E_ERROR == CheckIntegerLimits(min ,max)){
                            memset(m_dNoise_ROI_Org_Data[roi],0x0,sizeof(double)*m_nNum_Images_selected);
                            min = 0.0;
                            max = 0.0;
                        }
                        
                    } else {
                        CTiPrepLogHelper::WriteToErrorLog(_T("getMinMaxForYaxis(): m_dNoise_ROI_Org_Data found NULL"), __LINE__, __FILE__);
                    }
                }
            }
        }
        
        f_nYmax = (int)ceil(max);
        f_nYmin = (int)floor(min);
        
        if(0 >= f_nYmax) {
            f_nYmax = 10;
        }
        
        if(0 <= f_nYmin) {
            f_nYmin = -10;
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory Exception occurred in getMinMaxForYaxis");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception Occurred in getMinMaxForYaxis");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(...) {
        CString l_cserrmsg("Exception occurred in getMinMaxForYaxis");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	getMinMaxForYaxisMapMode
* Description: 	This function is used 
* Parameter: 	int f_nselectImg :
* Parameter: 	BOOL f_roi :
* Parameter: 	BOOL * f_NoiseRoi :
* Parameter: 	BOOL * f_DisplayOption :
* Parameter: 	BOOL * f_RevMin :
* Parameter: 	int * f_nYmax :
* Parameter: 	int * f_nYmin :
* Returns:   	int
*************************************************************/
int CDisplayData::getMinMaxForYaxisMapMode(int f_nselectImg, BOOL f_roi, 
                                           BOOL *f_NoiseRoi, BOOL *f_DisplayOption, 
                                           BOOL *f_RevMin,int &f_nYmax, int &f_nYmin,int f_index,BOOL f_bRefreshFlag)
{
    TIPREP_TRACE(CDisplayData::getMinMaxForYaxisMapMode,MRTraceMarker3);
    f_nYmax = 10;
    f_nYmin = -10;
    
    try {
        if(f_nselectImg <= 0) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("getMinMaxForYaxisMapMode(): f_nselectImg is <= 0"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        if(m_nNum_Images_selected <= 0) {
            m_nNum_Images_selected = m_csImageLoid.size();
            
            if(m_nNum_Images_selected <= 0) {
                CTiPrepLogHelper::WriteToErrorLog
                (_T("getMinMaxForYaxisMapMode(): f_nselectImg is <= 0"), __LINE__, __FILE__);
                return(E_ERROR);
            }
        }
        
        double  min = 0.0;
        double  max = 0.0;
        int roi = 0;
        max  = -10.0;
        min = 10.0;
        
        if((NULL == m_dROI_Fit_Data_Min_Reversed) || (NULL == m_dROI_Org_Data) ||   
            (NULL == m_dROI_Org_Data[f_index]) || (NULL == m_dROI_Fit_Data) ||
            (NULL == m_dROI_Inv_Data_Min_Reversed) || (NULL == m_dROI_Inv_Data) ||
            (NULL == m_dNoise_ROI_Org_Data)) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("getMinMaxForYaxisMapMode(): Data found NULL"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        if((f_index < 0) || (f_index >= m_nNumOfPoints)) {
            CString l_csval(_T(""));
            l_csval.Format(_T("Selected Index: %d"),f_index);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker14, l_csval, _T("getMinMaxForYaxisMapMode(): Selected index out of range error"));
            return(E_ERROR);
        }
        
        if(TRUE == f_roi) { 
            if(f_DisplayOption[ORG_DATA]) {
                getMinMaxInArray(m_dROI_Org_Data[f_index], 
                    m_nNum_Images_selected, &min, &max);
                if (E_ERROR == CheckIntegerLimits(min ,max)){
                    memset(m_dROI_Org_Data[f_index],0x0,sizeof(double)*m_nNum_Images_selected);
                    min = 0.0;
                    max = 0.0;
                    if(TRUE != f_bRefreshFlag){
                        return(E_IMPROPER_INDEX);
                }
                }
            }
            
            if(f_DisplayOption[FIT_DATA]) {
                if(f_RevMin[0]) {
                    getMinMaxInArray(m_dROI_Fit_Data_Min_Reversed[f_index], 
                        f_nselectImg, &min, &max);
                    if (E_ERROR == CheckIntegerLimits(min ,max)){
                        memset(m_dROI_Fit_Data_Min_Reversed[f_index],0x0,sizeof(double)*f_nselectImg);
                        min = 0.0;
                        max = 0.0;
                        m_dROI_T1_Val_Min_Reversed[f_index] = -1.0;
                        if(TRUE != f_bRefreshFlag){
                            return(E_IMPROPER_INDEX);
                        }
                    }
                } else {
                    getMinMaxInArray(m_dROI_Fit_Data[f_index], 
                        f_nselectImg, &min, &max);
                    if (E_ERROR == CheckIntegerLimits(min ,max)){
                        memset(m_dROI_Fit_Data[f_index],0x0,sizeof(double)*f_nselectImg);
                        min = 0.0;
                        max = 0.0;
                        m_dROI_T1_Val[f_index] = -1.0;
                        if(TRUE != f_bRefreshFlag){
                            return(E_IMPROPER_INDEX);
                        }
                    }
                }
            }
            
            if(f_DisplayOption[INV_DATA]) {
                if(f_RevMin[0]) {
                    getMinMaxInArray(m_dROI_Inv_Data_Min_Reversed[f_index], 
                        m_nNum_Images_selected, &min, &max);
                    if (E_ERROR == CheckIntegerLimits(min ,max)){
                        memset(m_dROI_Inv_Data_Min_Reversed[f_index],0x0,sizeof(double)*m_nNum_Images_selected);
                        min = 0.0;
                        max = 0.0;
                        if(TRUE != f_bRefreshFlag){
                            return(E_IMPROPER_INDEX);
                    }
                    }
                } else {
                    getMinMaxInArray(m_dROI_Inv_Data[f_index], 
                        m_nNum_Images_selected, &min, &max);
                    if (E_ERROR == CheckIntegerLimits(min ,max)){
                        memset(m_dROI_Inv_Data[f_index],0x0,sizeof(double)*m_nNum_Images_selected);
                        min = 0.0;
                        max = 0.0;
                        if(TRUE != f_bRefreshFlag){
                            return(E_IMPROPER_INDEX);
                    }
                    }
                }
            }
        }
        
        for(roi = 0; roi < MAX_NOISE_ROI; roi++) {
            if(f_DisplayOption[ORG_DATA]) {
                if(f_NoiseRoi[roi]) {
                    if(NULL != m_dNoise_ROI_Org_Data[roi]) {
                        getMinMaxInArray(m_dNoise_ROI_Org_Data[roi], 
                            m_nNum_Images_selected, &min, &max);
                        if (E_ERROR == CheckIntegerLimits(min ,max)){
                            memset(m_dNoise_ROI_Org_Data[roi],0x0,sizeof(double)*m_nNum_Images_selected);
                            min = 0.0;
                            max = 0.0;
                            if(TRUE != f_bRefreshFlag){
                                return(E_IMPROPER_INDEX);
                            }
                        }
                    } else {
                        CTiPrepLogHelper::WriteToErrorLog
                            (_T("getMinMaxForYaxisMapMode(): m_dNoise_ROI_Org_Data found NULL"), __LINE__, __FILE__); 
                        return(E_ERROR);
                    }
                }
            }
        }
        
        f_nYmax = (int)ceil(max);
        f_nYmin = (int)floor(min);
        
        if(0 >= f_nYmax) {
            f_nYmax = 10;
        }
        
        if(0 <= f_nYmin) {
            f_nYmin = -10;
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory Exception occurred in getMinMaxForYaxisMapMode");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception Occurred in getMinMaxForYaxisMapMode");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    } catch(...) {
        CString l_cserrmsg("Exception occurred in getMinMaxForYaxisMapMode");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERR_EXCEPTION);
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       getMinMaxForXaxis
* Description:  This function is used to get minimum and maximum
*               limit for X-axis which is used for graph plotting.
* Parameter:    int f_nselectImg
*               Contains number of selected images on ISelector.
* Parameter:    int * f_nXmax
*               Out parameter is used to max limit of X-axis.
* Parameter:    int * f_nXmin
*               Out parameter is used to min limit of X-axis.
* Returns:      int
*************************************************************/
int CDisplayData::getMinMaxForXaxis(int f_nselectImg,
                                    int &f_nXmax, int &f_nXmin)
{
    TIPREP_TRACE(CDisplayData::getMinMaxForXaxis,MRTraceMarker3);
    
    if((NULL == m_nTime_stamp)/* || (NULL == f_nXmax) || (NULL == f_nXmin)*/) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("getMinMaxForXaxis(): setROIData Time Stamp data NULL"), __LINE__, __FILE__);
        f_nXmax = 0;
        f_nXmin = 20;
        return (E_ERROR);
    }
    
    if(0 == (m_nTime_stamp[0] - m_nTime_stamp[1])){ 
        ValidateAndSetTimeStamp(m_nTime_stamp,f_nselectImg);       
    }
    f_nXmax = m_nTime_stamp[f_nselectImg-1];
    f_nXmin = m_nTime_stamp[0];
    return E_NO_ERROR;
}

/*************************************************************
* Method:       getMinMaxInArray
* Description:  This function is used
* Parameter:    double * data_arr
* Parameter:    int data_len
* Parameter:    double * min
* Parameter:    double * max
* Returns:      int
*************************************************************/
int CDisplayData::getMinMaxInArray(double *data_arr, int f_nNumImages, 
                                   double *min, double *max)
{
    TIPREP_TRACE(CDisplayData::getMinMaxInArray,MRTraceMarker9);
    
    if((NULL != data_arr) && (NULL != min) && (NULL != max)) {
        int i = 0;
        
        for(i = 0; i < f_nNumImages; i++) {
            if(data_arr[i] > *max) {
                *max  = data_arr[i];
            }
            
            if(data_arr[i] < *min) {
                *min = data_arr[i];
            }
        }
    } else{
        CString l_cserrmsg("Exception occurred in getMinMaxInArray");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    
    return (E_NO_ERROR);
}

/*************************************************************
* Method:       getROIOrgData
* Description:  This is getter method for m_dROI_Org_Data.
* Returns:      double**
*               pointer to m_dROI_Org_Data.
*************************************************************/
double** CDisplayData::getROIOrgData()
{
    TIPREP_TRACE(CDisplayData::getROIOrgData,MRTraceMarker8);
    return m_dROI_Org_Data;
}

/*************************************************************
* Method:       getNoiseROIOrgData
* Description:  This is getter method for m_dNoise_ROI_Org_Data.
* Returns:      double**
*               pointer to m_dNoise_ROI_Org_Data.
*************************************************************/
double** CDisplayData::getNoiseROIOrgData()
{
    TIPREP_TRACE(CDisplayData::getNoiseROIOrgData,MRTraceMarker8);
    return m_dNoise_ROI_Org_Data;
}

/*************************************************************
* Method:       getNoiseOffsetVal
* Description:  This function is used
* Returns:      double
*************************************************************/
double CDisplayData::getNoiseOffsetVal()
{
    TIPREP_TRACE(CDisplayData::getNoiseOffsetVal,MRTraceMarker8);
    return m_dnoiseOffsetVal;
}

/*************************************************************
* Method:       getROIIvnData
* Description:  This is getter method for m_dROI_Inv_Data.
* Returns:      double**
*               pointer to m_dROI_Inv_Data.
*************************************************************/
double** CDisplayData::getROIIvnData()
{
    TIPREP_TRACE(CDisplayData::getROIIvnData,MRTraceMarker8);
        return m_dROI_Inv_Data;
}

/*************************************************************
* Method:       getROIMinRevData
* Description:  This is getter method for m_dROI_Inv_Data_Min_Reversed.
* Returns:      double**
*               pointer to m_dROI_Inv_Data_Min_Reversed.
*************************************************************/
double** CDisplayData::getROIMinRevData()
{
    TIPREP_TRACE(CDisplayData::getROIMinRevData,MRTraceMarker8);
    return m_dROI_Inv_Data_Min_Reversed;
}

/*************************************************************
* Method:       getROIFitData
* Description:  This is getter method for m_dROI_Fit_Data.
* Returns:      double**
*               Double pointer to m_dROI_Fit_Data.
*************************************************************/
double** CDisplayData::getROIFitData()
{
    TIPREP_TRACE(CDisplayData::getROIFitData,MRTraceMarker8);
    return m_dROI_Fit_Data;
}

/*************************************************************
* Method:       getROIFitMinRevData
* Description:  This is getter method for m_dROI_Fit_Data_Min_Reversed.
* Returns:      double**
*               Double pointer to m_dROI_Fit_Data_Min_Reversed.
*************************************************************/
double** CDisplayData::getROIFitMinRevData()
{
    TIPREP_TRACE(CDisplayData::getROIFitMinRevData,MRTraceMarker8);
    return m_dROI_Fit_Data_Min_Reversed;
}

/*************************************************************
* Method:       getROITiValues
* Description:  This is getter method for m_dROI_TI_Val.
* Returns:      double*
*               Pointer to m_dROI_TI_Val.
*************************************************************/
double* CDisplayData::getROITiValues()
{
    TIPREP_TRACE(CDisplayData::getROITiValues,MRTraceMarker8);
    return m_dROI_TI_Val;
}

/*************************************************************
* Method:       getROITiMinRevValues
* Description:  This is getter method for m_dROI_TI_Val_Min_Reversed.
* Returns:      double*
*               Pointer to m_dROI_TI_Val_Min_Reversed.
*************************************************************/
double* CDisplayData::getROITiMinRevValues()
{
    TIPREP_TRACE(CDisplayData::getROITiMinRevValues,MRTraceMarker8);
    return m_dROI_TI_Val_Min_Reversed;
}

/*************************************************************
* Method:       getROIT1Values
* Description:  This is getter method for m_dROI_T1_Val.
* Returns:      double*
*               Pointer to m_dROI_T1_Val.
*************************************************************/
double* CDisplayData::getROIT1Values()
{
    TIPREP_TRACE(CDisplayData::getROIT1Values,MRTraceMarker8);
    return m_dROI_T1_Val;
}

/*************************************************************
* Method:       getROIT1MinRevValues
* Description:  This is getter method for m_dROI_T1_Val_Min_Reversed.
* Returns:      double*
*               Pointer to m_dROI_T1_Val_Min_Reversed.
*************************************************************/
double* CDisplayData::getROIT1MinRevValues()
{
    TIPREP_TRACE(CDisplayData::getROIT1MinRevValues,MRTraceMarker8);
    return m_dROI_T1_Val_Min_Reversed;
}

/*************************************************************
* Method:       getROIFitErr
* Description:  This is getter method for m_dROI_Fit_Err.
* Returns:      double*
*               Pointer to m_dROI_Fit_Err.
*************************************************************/
double* CDisplayData::getROIFitErr()
{
    TIPREP_TRACE(CDisplayData::getROIFitErr,MRTraceMarker8);
    return m_dROI_Fit_Err;
}

/*************************************************************
* Method:       getROIFitMinRevErr
* Description:  This is getter method for m_dROI_Fit_Err_Min_Reversed.
* Returns:      double*
*               Pointer to m_dROI_Fit_Err_Min_Reversed.
*************************************************************/
double* CDisplayData::getROIFitMinRevErr()
{
    TIPREP_TRACE(CDisplayData::getROIFitMinRevErr,MRTraceMarker8);
    return m_dROI_Fit_Err_Min_Reversed;
}

/*************************************************************
* Method:       setTiValues
* Description:  This is setter method for m_dROI_TI_Val and
*               m_dROI_TI_Val_Min_Reversed.
* Parameter:    double * f_dROITi_Val
*               Contains Ti values for original data.
* Parameter:    double * f_dROITi_Min_Rev
*               Contains Ti values for min_reversed data.
* Returns:      void
*************************************************************/
void CDisplayData::setTiValues(double *f_dROITi_Val, double *f_dROITi_Min_Rev)
{
    TIPREP_TRACE(CDisplayData::setTiValues,MRTraceMarker8);
    
    if((NULL != f_dROITi_Val) && (NULL != f_dROITi_Min_Rev)) {
        if((NULL != m_dROI_TI_Val) && (NULL != m_dROI_TI_Val_Min_Reversed)) {
            for(int l_ni = 0; l_ni < m_nNumOfPoints; l_ni++) {  
                m_dROI_TI_Val[l_ni] = f_dROITi_Val[l_ni];
                m_dROI_TI_Val_Min_Reversed[l_ni] = f_dROITi_Min_Rev[l_ni];
            }
        }else{
            CTiPrepLogHelper::WriteToErrorLog
                (_T("setTiValues(): class variable NULL error"), __LINE__, __FILE__);
            return;
        }
    } else {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setTiValues(): Function parameter NULL error"), __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       setErrorValues
* Description:  This is setter method for m_dROI_Fit_Err and m_dROI_Fit_Err_Min_Reversed.
* Parameter:    double * f_dRoiFit_Err
*               Contains fitting error values for original data.
* Parameter:    double * f_dRoiFit_Min_Rev_Err
*               Contains fitting error values for minreversed data.
* Returns:      void
*************************************************************/
void CDisplayData::setErrorValues(double *f_dRoiFit_Err,
                                  double *f_dRoiFit_Min_Rev_Err)
{
    TIPREP_TRACE(CDisplayData::setErrorValues,MRTraceMarker8);
    
    if((NULL != f_dRoiFit_Err) && (NULL != f_dRoiFit_Min_Rev_Err)) {
        if((NULL != m_dROI_Fit_Err) && (NULL != m_dROI_Fit_Err_Min_Reversed)) {
            
            for(int l_ni = 0; l_ni < m_nNumOfPoints; l_ni++) {  
                m_dROI_Fit_Err[l_ni] = f_dRoiFit_Err[l_ni];
                m_dROI_Fit_Err_Min_Reversed[l_ni] = f_dRoiFit_Min_Rev_Err[l_ni];
            }
        } else{
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Class members for fit error are NULL"), __LINE__, __FILE__);
            return;
        }
    } else {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setErrorValues(): Function parameter NULL error"), __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       setT1Values
* Description:  This is setter method for m_dROI_T1_Val and
*               m_dROI_T1_Val_Min_Reversed.
* Parameter:    double * f_dRoi_T1_val
*               Contains T1 values for original data.
* Parameter:    double * f_dRoi_T1_Min_Rev_Val
*               Contains T1 values for minreversed data.
* Returns:      void
*************************************************************/
void CDisplayData::setT1Values(double *f_dRoi_T1_val,
                               double *f_dRoi_T1_Min_Rev_Val)
{
    TIPREP_TRACE(CDisplayData::setT1Values,MRTraceMarker8);
    
    if((NULL != f_dRoi_T1_val) && (NULL != f_dRoi_T1_Min_Rev_Val)) {
        if((NULL != f_dRoi_T1_val) && (NULL != f_dRoi_T1_Min_Rev_Val)) {
            
            for(int l_ni = 0; l_ni < m_nNumOfPoints; l_ni++) {  
                m_dROI_T1_Val[l_ni] = f_dRoi_T1_val[l_ni];
                m_dROI_T1_Val_Min_Reversed[l_ni] = f_dRoi_T1_Min_Rev_Val[l_ni];
            }
        } else{
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Class members for T1 value are NULL"), __LINE__, __FILE__);
            return;
        }
    } else {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("setT1Values(): Function parameter NULL error"), __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       AllocateMemory
* Description:  This function is used
* Returns:      bool
*************************************************************/
int CDisplayData::AllocateMemory()
{
    try { 
        if(NULL != m_dROI_Org_Data) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("AllocateMemory() memory allocated already"), __LINE__, __FILE__);
            //memory is already allocated
            return(E_NO_ERROR);
        }
        
        if(m_nNumOfPoints <= 0) { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("AllocateMemory(): m_nNumOfPoints <= 0 error"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        m_dROI_TI_Val = new (std::nothrow) double [m_nNumOfPoints];
        m_dROI_TI_Val_Min_Reversed = new (std::nothrow) double [m_nNumOfPoints];
        m_dROI_T1_Val = new (std::nothrow) double [m_nNumOfPoints];
        m_dROI_T1_Val_Min_Reversed = new (std::nothrow) double [m_nNumOfPoints];
        m_dROI_Fit_Err = new (std::nothrow) double [m_nNumOfPoints];
        m_dROI_Fit_Err_Min_Reversed = new (std::nothrow) double [m_nNumOfPoints];
        
        //in case above memory allocation failed for any of the above data
        //delete and free the memory if allocated
        if((NULL == m_dROI_TI_Val) || (NULL == m_dROI_TI_Val_Min_Reversed) ||
            (NULL == m_dROI_T1_Val) || (NULL == m_dROI_T1_Val_Min_Reversed) ||
            (NULL == m_dROI_Fit_Err) || (NULL == m_dROI_Fit_Err_Min_Reversed)) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("AllocateMemory() new 1 failed"), __LINE__, __FILE__);
            DeAllocateMemory();
            return(E_ERROR);
        }
        
        m_dROI_Org_Data = new (std::nothrow) double *[m_nNumOfPoints];
        m_dROI_Inv_Data = new (std::nothrow) double *[m_nNumOfPoints];
        m_dROI_Inv_Data_Min_Reversed = new (std::nothrow) double *[m_nNumOfPoints];
        m_dROI_Fit_Data = new (std::nothrow) double *[m_nNumOfPoints];
        m_dROI_Fit_Data_Min_Reversed = new (std::nothrow) double *[m_nNumOfPoints];
        m_dNoise_ROI_Org_Data = new (std::nothrow) double * [MAX_NOISE_ROI];
        
        if((NULL == m_dROI_Org_Data) || (NULL == m_dROI_Inv_Data) || (NULL == m_dROI_Inv_Data_Min_Reversed) ||
            (NULL == m_dROI_Fit_Data) || (NULL == m_dROI_Fit_Data_Min_Reversed) || (NULL == m_dNoise_ROI_Org_Data)) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("AllocateMemory() new 2 failed"), __LINE__, __FILE__);
            DeAllocateMemory();
            return(E_ERROR);
        }
        
        for(int i = 0; i < m_nNumOfPoints; i++) {
            m_dROI_Org_Data[i] = new (std::nothrow) double [m_nNum_Images_selected]();
            m_dROI_Inv_Data[i] = new (std::nothrow) double [m_nNum_Images_selected]();
            m_dROI_Inv_Data_Min_Reversed[i] = new (std::nothrow) double [m_nNum_Images_selected]();
            m_dROI_Fit_Data[i] = new (std::nothrow) double [m_nNum_Images_selected]();
            m_dROI_Fit_Data_Min_Reversed[i] = new (std::nothrow) double [m_nNum_Images_selected]();
            
            if((NULL == m_dROI_Org_Data[i]) || (NULL == m_dROI_Inv_Data[i]) ||
                (NULL == m_dROI_Inv_Data_Min_Reversed[i]) || (NULL == m_dROI_Fit_Data[i])
                || (NULL == m_dROI_Fit_Data_Min_Reversed[i])) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("AllocateMemory() new 3 failed"), __LINE__, __FILE__);

                DeAllocateMemory();
                return(E_ERROR);
            }
        }
        
        for(i = 0; i < MAX_NOISE_ROI; i++) {
            m_dNoise_ROI_Org_Data[i] = new (std::nothrow) double [m_nNum_Images_selected];
            
            if(NULL == m_dNoise_ROI_Org_Data[i]) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("AllocateMemory() new 4 failed"), __LINE__, __FILE__);
                DeAllocateMemory();
                return(E_ERROR);
            }
        }
        
    } catch(...) { 
        CTiPrepLogHelper::WriteToErrorLog
            (_T("TiPrep application failed to initilize memory."), __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       MemSetROIData
* Description:  This function is used
* Returns:      bool
*************************************************************/
int CDisplayData::MemSetROIData()
{
    if(m_nNumOfPoints <= 0) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("MemSetROIData(): m_nNumOfPoints <= 0 error"), __LINE__, __FILE__);
        return(E_ERROR);
    }
    
    try {
        if((NULL == m_dROI_Org_Data) &&
            (NULL == m_dROI_Inv_Data) &&
            (NULL == m_dROI_Inv_Data_Min_Reversed) &&
            (NULL == m_dROI_Fit_Data) &&
            (NULL == m_dROI_Fit_Data_Min_Reversed) &&
            (NULL == m_dNoise_ROI_Org_Data) &&
            (NULL == m_dROI_TI_Val) &&
            (NULL == m_dROI_TI_Val_Min_Reversed) &&
            (NULL == m_dROI_T1_Val) &&
            (NULL == m_dROI_T1_Val_Min_Reversed) &&
            (NULL == m_dROI_Fit_Err) &&
            (NULL == m_dROI_Fit_Err_Min_Reversed)
            ) {
            //memory not allocated i.e nothing to memset
            return(E_NO_ERROR);
        }
        
        memset(m_dROI_TI_Val, 0, (sizeof(double)*m_nNumOfPoints));
        memset(m_dROI_TI_Val_Min_Reversed, 0, (sizeof(double)*m_nNumOfPoints));
        memset(m_dROI_T1_Val, 0, (sizeof(double)*m_nNumOfPoints));
        memset(m_dROI_T1_Val_Min_Reversed, 0, (sizeof(double)*m_nNumOfPoints));
        memset(m_dROI_Fit_Err, 0, (sizeof(double)*m_nNumOfPoints));
        memset(m_dROI_Fit_Err_Min_Reversed, 0, (sizeof(double)*m_nNumOfPoints));
        
        for(int i = 0; i < m_nNumOfPoints; i++) {
            if(NULL != m_dROI_Org_Data[i]) {
                memset(m_dROI_Org_Data[i], 0, (sizeof(double)*m_nNum_Images_selected));
            }
            
            if(NULL != m_dROI_Inv_Data[i]) {
                memset(m_dROI_Inv_Data[i], 0, (sizeof(double)*m_nNum_Images_selected));
            }
            
            if(NULL != m_dROI_Inv_Data_Min_Reversed[i]) {
                memset(m_dROI_Inv_Data_Min_Reversed[i], 0, (sizeof(double)*m_nNum_Images_selected));
            }
            
            if(NULL != m_dROI_Fit_Data[i]) {
                memset(m_dROI_Fit_Data[i], 0, (sizeof(double)*m_nNum_Images_selected));
            }
            
            if(NULL != m_dROI_Fit_Data_Min_Reversed[i]) {
                memset(m_dROI_Fit_Data_Min_Reversed[i], 0, (sizeof(double)*m_nNum_Images_selected));
            }
        }
        
        for(i = 0; i < MAX_NOISE_ROI; i++) {
            if(NULL != m_dNoise_ROI_Org_Data[i]) {
                memset(m_dNoise_ROI_Org_Data[i], 0, (sizeof(double)*m_nNum_Images_selected));
            }
        }
        
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in MemSetROIData"), __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       DeAllocateMemory
* Description:  This function is used
* Returns:      bool
*************************************************************/
int CDisplayData::DeAllocateMemory()
{
    try { 
        CWnd * dlg = NULL;
        dlg = ((CTiPrepView::GetView())->GetCTiPrepDlgInstance());
        if (dlg && dlg->GetSafeHwnd())
        {
            (CTiPrepView::GetView())->GetCTiPrepDlgInstance()->ClearGraphCtl();
        }
        
        if(NULL != m_dROI_Org_Data) {
            for(int i = 0; i < m_nNumOfPoints; i++) {
                if(m_dROI_Org_Data[i]) {
                    delete[]m_dROI_Org_Data[i];
                    m_dROI_Org_Data[i] = NULL;
                }
                
                if(m_dROI_Inv_Data[i]) {
                    delete[]m_dROI_Inv_Data[i];
                    m_dROI_Inv_Data[i] = NULL;
                }
                
                if(m_dROI_Inv_Data_Min_Reversed[i]) {
                    delete[]m_dROI_Inv_Data_Min_Reversed[i];
                    m_dROI_Inv_Data_Min_Reversed[i] = NULL;
                }
                
                if(m_dROI_Fit_Data[i]) {
                    delete[]m_dROI_Fit_Data[i];
                    m_dROI_Fit_Data[i]  = NULL;
                }
                
                if(m_dROI_Fit_Data_Min_Reversed[i]) {
                    delete[]m_dROI_Fit_Data_Min_Reversed[i];
                    m_dROI_Fit_Data_Min_Reversed[i]  = NULL;
                }
            }
            
            for(i = 0; i < MAX_NOISE_ROI; i++) {
                if(m_dNoise_ROI_Org_Data[i]) {
                    delete[]m_dNoise_ROI_Org_Data[i];
                    m_dNoise_ROI_Org_Data[i]  = NULL;
                }
            }
            
            if(m_dROI_Org_Data) {
                delete[]m_dROI_Org_Data;
                m_dROI_Org_Data = NULL;
            }
            
            if(m_dROI_Inv_Data) {
                delete[]m_dROI_Inv_Data;
                m_dROI_Inv_Data = NULL;
            }
            
            if(m_dROI_Inv_Data_Min_Reversed) {
                delete[]m_dROI_Inv_Data_Min_Reversed;
                m_dROI_Inv_Data_Min_Reversed = NULL;
            }
            
            if(m_dROI_Fit_Data) {
                delete[]m_dROI_Fit_Data;
                m_dROI_Fit_Data  = NULL;
            }
            
            if(m_dROI_Fit_Data_Min_Reversed) {
                delete[]m_dROI_Fit_Data_Min_Reversed;
                m_dROI_Fit_Data_Min_Reversed  = NULL;
            }
            
            if(m_dNoise_ROI_Org_Data) {
                delete[]m_dNoise_ROI_Org_Data;
                m_dNoise_ROI_Org_Data  = NULL;
            }
            
            if(m_dROI_TI_Val) {
                delete[]m_dROI_TI_Val;
                m_dROI_TI_Val = NULL;
            }
            
            if(m_dROI_TI_Val_Min_Reversed) {
                delete[]m_dROI_TI_Val_Min_Reversed;
                m_dROI_TI_Val_Min_Reversed = NULL;
            }
            
            if(m_dROI_T1_Val) {
                delete[]m_dROI_T1_Val;
                m_dROI_T1_Val = NULL;
            }
            
            if(m_dROI_T1_Val_Min_Reversed) {
                delete[]m_dROI_T1_Val_Min_Reversed;
                m_dROI_T1_Val_Min_Reversed  = NULL;
            }
            
            if(m_dROI_Fit_Err) {
                delete[]m_dROI_Fit_Err;
                m_dROI_Fit_Err  = NULL;
            }
            
            if(m_dROI_Fit_Err_Min_Reversed) {
                delete[]m_dROI_Fit_Err_Min_Reversed;
                m_dROI_Fit_Err_Min_Reversed  = NULL;
            }
            
            if(m_nTime_stamp) {
                delete[]m_nTime_stamp;
                m_nTime_stamp  = NULL;
            }
            
            m_nNewGeneratedXPoints = 0;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in DeAllocateMemory"), __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       setAnnularMapData
* Description:  This function is used
* Returns:      int
*************************************************************/
int CDisplayData::setAnnularMapData(bool avgfilterFlag, int filter_sz, 
                                    bool isSaveFilterImg, 
                                    vector<CString > &newSeriesLOIDs) 
{
    try {
        TIPREP_TRACE(CDisplayData::setAnnularMapData,MRTraceMarker3);
        
        if(NULL == m_nTime_stamp) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("setAnnularMapData Time Stamp data NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_nNum_Images_selected = m_csImageLoid.size();
        int l_nRoi = 0;
        void *l_pImgData = NULL;
        void *l_newImgData = NULL; 
        int l_nsize = 0;
        byte* l_bImROI[MAX_LINE_ROI] = {NULL, NULL, NULL, NULL, NULL}; 
        memset(l_bImROI, 0x0, (MAX_LINE_ROI * sizeof(byte*)));
        int l_nROIH = 0;
        int l_nROIW = 0;
        int l_nj = 0;
        unsigned short l_nlData_Type = 16;//int16_t data type default
        int l_nCount = 0;
        BOOL l_bstatus = (CTiPrepView::GetView())->GetROIInstance()->GetROIBufferInfo
            (l_bImROI, &l_nROIH, &l_nROIW);
        
        if(!l_bstatus) {
            CString l_csErrMsg(_T("Unable to get image map."));
            DeAllocateMemory();
            return E_ROI_MAP;
        }
        
        if(NULL == l_bImROI[0] || NULL == l_bImROI[1] || NULL == l_bImROI[2]) {
            CString l_csErrMsg(_T("Map is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            DeAllocateMemory();
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        memset(l_bImROI[2], 0x0, sizeof(byte)*l_nROIW * l_nROIH);
        m_nNumOfPoints = 0;
        
        for(int l_nq = 0; l_nq < l_nROIH; l_nq++) {
            for(int l_nr = 0; l_nr < l_nROIW; l_nr++) {
                int nIndex = (l_nROIW) * l_nq + l_nr;
                l_bImROI[2][nIndex] = l_bImROI[0][nIndex] ^ l_bImROI[1][nIndex]; //EX-OR
                
                if(1 == l_bImROI[2][nIndex]) {
                    m_nNumOfPoints++;
                }
            }
        }
        
        if(m_nNumOfPoints <= 0) {
            DeAllocateMemory();
            return E_MAP_ROI_NO_DATA;
        }
        
        if(E_NO_ERROR != AllocateMemory()) {
            CString l_cserrmsg(_T("AllocateMemory failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            DeAllocateMemory();
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) { 
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            return E_MEM_ALLOC;
        }
        
        if(E_NO_ERROR != MemSetROIData()) { 
            CString l_cserrmsg(_T("MemSetROIData failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            DeAllocateMemory();
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) { 
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            return E_ERROR;
        }

        CLocalDBMgr l_localdbmgr;
        
        for(l_nj ; l_nj < (signed) m_csImageLoid.size(); l_nj++) {
            int l_nTime_stamp = 0;
            float l_fScale = 0.0;
//            int status = (CTiPrepView::GetView())->GetLocalDBImgData(m_csImageLoid.at(l_nj), &l_pImgData, &l_nsize, &l_fScale, &l_nTime_stamp, &l_nlData_Type,l_nj);//sb
            int status = l_localdbmgr.getImgDataFromDB(m_csImageLoid.at(l_nj), 
                &l_pImgData, &l_nsize, &l_fScale, &l_nTime_stamp, &l_nlData_Type,l_nj);//sb
            
            if(status != 0) {
                
                DeAllocateMemory();
                
                for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    CoTaskMemFree(l_bImROI[l_nRoi]);
                    l_bImROI[l_nRoi] = NULL;
                }
                
                if(NULL != l_pImgData) {
                    /*free*/operator delete(l_pImgData);
                    l_pImgData = NULL;
                }
                
                if(NULL != l_newImgData) {
                    delete [](l_newImgData);
                    l_newImgData = NULL;
                }
                
                return status;
            }
            
            if(avgfilterFlag && isSaveFilterImg) {
                if(l_newImgData) {
                    delete [] l_newImgData;
                    l_newImgData = NULL;
                }
                
                if(!l_newImgData) {
                    if(l_nsize <= 0) {
                        CString lcsmsg(_T("Image size is 0"));
                        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
                        DeAllocateMemory();
                        
                        for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                            CoTaskMemFree(l_bImROI[l_nRoi]);
                            l_bImROI[l_nRoi] = NULL;
                        }
                        
                        return E_ERROR;
                    }
                    
                    l_newImgData = new char [l_nsize];
                    
                    if(NULL == l_newImgData) {
                        CString lcsmsg = _T("mem alloc failed in SetAnnularMapData()");
                        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
                        DeAllocateMemory();
                        
                        for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                            CoTaskMemFree(l_bImROI[l_nRoi]);
                            l_bImROI[l_nRoi] = NULL;
                        }
                        
                        return E_ERROR;
                    }
                }
                
                if(l_newImgData) {
                    memcpy(l_newImgData, l_pImgData, l_nsize);
                }
            }
            
            if(l_nj == 0 && status == 0) {
                CString l_csStudyFileName = _T("");
                GetStudyFileName(l_csStudyFileName);
                char destFldr[100] = {'\0'};
                strcpy(destFldr, "c:/MrMplus/Data/TiPrep");
                
                if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)) {
                    char temp[50];
                    wcstombs(temp, (LPCTSTR)l_csStudyFileName, 50);
                    strcat(destFldr , "/");
                    strcat(destFldr , temp);
                    
                    if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)) {
                        CString fname = destFldr;
                        fname = fname + _T("/") + _T("T1Back");
                        CMapWriter l_CBackWriter;
                        int l_nStatus = l_CBackWriter.WriteToFile
                            (fname, l_pImgData, l_nROIW * l_nROIH * sizeof(short)); 
                        //PENDING :Need To get Correct Data SIze
                        if(E_NO_ERROR != l_nStatus){
                            CString lcsmsg = _T("SetAnnularMapData(): WriteToFile() returned error");
                            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
                            DeAllocateMemory();
                            
                            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                                CoTaskMemFree(l_bImROI[l_nRoi]);
                                l_bImROI[l_nRoi] = NULL;
                            }
                            
                            return(l_nStatus);
                        }
                    }
                } else {
                    CTiPrepLogHelper::WriteToErrorLog
                        (_T("setAnnularMapData():c:/MrMplus/Data/TiPrep  path not found"), __LINE__);
                    CString lcsmsg = _T("SetAnnularMapData(): WriteToFile() returned error");
                    CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
                    DeAllocateMemory();
                    
                    for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                        CoTaskMemFree(l_bImROI[l_nRoi]);
                        l_bImROI[l_nRoi] = NULL;
                            }
                    return E_ERROR;
                }
            }
            
            if(m_nTime_stamp) {
                m_nTime_stamp[l_nj] = l_nTime_stamp;
            }
            
            l_nCount = 0;
            
            for(int l_nq = 0; l_nq < l_nROIH; l_nq++) {
                for(int l_nr = 0; l_nr < l_nROIW; l_nr++) {
                    int nIndex = (l_nROIW) * l_nq + l_nr;
                    
                    if(1 == l_bImROI[2][nIndex]) {
                        switch(l_nlData_Type) {
                        case 16: {
                            INT16 temp = 0;
                            
                            if(!avgfilterFlag) {
                                memcpy(&temp, ((unsigned char*)(l_pImgData) + nIndex * 2), sizeof(INT16));
                            } else {
                                CAvgFilter::ApplyAvgFilter((INT16*)&temp, (INT16*)l_pImgData, 
                                    l_nROIH, l_nROIW, l_nq, l_nr, filter_sz);
                            }
                            
                            if(avgfilterFlag && isSaveFilterImg) {
                                INT16 * tmp = (INT16*)l_newImgData;
                                tmp[l_nq*l_nROIW+l_nr] = temp;
                            }
                            
                            temp = (temp / l_fScale);
                            m_dROI_Org_Data[l_nCount][l_nj] = double(temp);
                            l_nCount++;
                            break;
                                 }
                        case 32: {
                            INT32 temp = 0;
                            
                            if(!avgfilterFlag) {
                                memcpy(&temp, ((unsigned char*)(l_pImgData) + nIndex * 2), sizeof(INT32));
                            } else {
                                CAvgFilter::ApplyAvgFilter
                                    ((INT32*)&temp, (INT32*)l_pImgData, l_nROIH, 
                                    l_nROIW, l_nq, l_nr, filter_sz);
                            }
                            
                            if(avgfilterFlag && isSaveFilterImg) {
                                INT32 * tmp = (INT32 *)l_newImgData;
                                tmp[l_nq*l_nROIW+l_nr] = temp;
                            }
                            
                            temp = (temp / l_fScale);
                            m_dROI_Org_Data[l_nCount][l_nj] = double(temp);
                            l_nCount++;
                            break;
                                 }
                        default:
                            break;
                        }
                    } else {
                        //point not in annular region
                    }
                }//row
            }//height
            
            if((0 == l_nCount) || (l_nCount != m_nNumOfPoints)) {
                CString l_cserr(_T(""));
                l_cserr = _T("ROI is placed outside of image region");
                DeAllocateMemory();
                
                for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    CoTaskMemFree(l_bImROI[l_nRoi]);
                    l_bImROI[l_nRoi] = NULL;
                }
                
                if(NULL != l_pImgData) {
                    /*free*/operator delete(l_pImgData);
                    l_pImgData = NULL;
                }
                
                if(NULL != l_newImgData) {
                    delete [](l_newImgData);
                    l_newImgData = NULL;
                }
                
                return I_ROI_OUT_RGN;
            }
            
            if(avgfilterFlag && isSaveFilterImg) {
                status = (CTiPrepView::GetView())->UpdateImageBuffer
                    (newSeriesLOIDs.at(l_nj), l_newImgData, l_nsize); 
            
            
            if(status != E_NO_ERROR) {
                CString lcserrmsg(_T("UpdateImage failed"));
                CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            }
            }
            if(NULL != l_pImgData) {
                /*free*/operator delete(l_pImgData);
                l_pImgData = NULL;
            }
            
            if(NULL != l_newImgData) {
                delete [](l_newImgData);
                l_newImgData = NULL;
            }
        }//total Images
        ValidateAndSetTimeStamp(m_nTime_stamp,(signed) m_csImageLoid.size());//AS/added
        
        if(FALSE == setROI_Inv_Data(m_nNum_Images_selected)) { 
            CTiPrepLogHelper::WriteToErrorLog(_T("setROI_Inv_Data failed"),__LINE__,__FILE__);
            DeAllocateMemory();
            
            for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) { 
                CoTaskMemFree(l_bImROI[l_nRoi]);
                l_bImROI[l_nRoi] = NULL;
            }
            
            if(NULL != l_pImgData) {
                /*free*/operator delete(l_pImgData);
                l_pImgData = NULL;
            }
            
            if(NULL != l_newImgData) {
                delete [](l_newImgData);
                l_newImgData = NULL;
            }
            
            return E_ERROR;
        }
        
        if(l_bImROI[2] != NULL && l_nROIH > 0 && l_nROIW > 0) { 
            int l_nStatus = (CTiPrepView::GetView())->OnSetMap(l_bImROI[2], l_nROIH, l_nROIW);
            if(E_NO_ERROR != l_nStatus){
                CString lcsmsg = _T("SetAnnularMapData(): OnSetMap() returned error");
                CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
                
                DeAllocateMemory();
                
                for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    CoTaskMemFree(l_bImROI[l_nRoi]);
                    l_bImROI[l_nRoi] = NULL;
                }
                
                if(NULL != l_pImgData) {
                    /*free*/operator delete(l_pImgData);
                    l_pImgData = NULL;
                }
                
                if(NULL != l_newImgData) {
                    delete [](l_newImgData);
                    l_newImgData = NULL;
                }
                
                return l_nStatus;
            }
        }
        
        for(l_nRoi = 0 ; l_nRoi < MAX_LINE_ROI; l_nRoi++) { 
            CoTaskMemFree(l_bImROI[l_nRoi]);
            l_bImROI[l_nRoi] = NULL;
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(CFileException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in setAnnularMapData"), __LINE__, __FILE__);
        DeAllocateMemory();
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       GetTotalNumberOfPointsInAnnularRegion
* Description:  This function is used
* Returns:      long
*************************************************************/
long CDisplayData::GetTotalNumberOfPointsInAnnularRegion()
{
    return(m_nNumOfPoints); 
}
void CDisplayData::SetSelectedIndex(int f_nSelectedIndex)
{
    m_nSelectedIndex = f_nSelectedIndex;
}

int CDisplayData::GetSelectedIndex()
{
    return(m_nSelectedIndex);
}   

/*************************************************************
* Method:    	DeallocateBufferXY
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CDisplayData::DeallocateBufferXY()
{
    try{
        if((m_dROI_Fit_Data == NULL) && (m_dROI_Fit_Data_Min_Reversed == NULL) && (m_nTime_stamp == NULL)) {
            return E_ERROR;
        }
        
        if(m_dROI_Fit_Data != NULL) {
            for(int l_ni = 0; l_ni < m_nNumOfPoints; l_ni++) {
                if(NULL != m_dROI_Fit_Data[l_ni]) {
                    delete [] m_dROI_Fit_Data[l_ni];
                    m_dROI_Fit_Data[l_ni] = NULL;
                }
            }
            
            delete [] m_dROI_Fit_Data;
            m_dROI_Fit_Data = NULL;
        }
        
        if(m_dROI_Fit_Data_Min_Reversed != NULL) {
            for(int l_ni = 0; l_ni < m_nNumOfPoints; l_ni++) {
                if(NULL != m_dROI_Fit_Data_Min_Reversed[l_ni]) {
                    delete [] m_dROI_Fit_Data_Min_Reversed[l_ni];
                    m_dROI_Fit_Data_Min_Reversed[l_ni] = NULL;
                }
            }
            
            delete [] m_dROI_Fit_Data_Min_Reversed;
            m_dROI_Fit_Data_Min_Reversed = NULL;
        }
        
        if(NULL != m_nTime_stamp) {
            delete [] m_nTime_stamp;
            m_nTime_stamp = NULL;
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in DeallocateBufferXY while releasing buffers"), __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
}

/*************************************************************
* Method:    	AllocateBufferXY
* Description: 	This function is used 
* Parameter: 	int NewPts :
* Returns:   	int
*************************************************************/
int CDisplayData::AllocateBufferXY(int NewPts)
{
    try{
        if(NewPts <= 0) {
            CTiPrepLogHelper::WriteToErrorLog(
                _T("AllocateBufferXY(): NewPts <= 0"), __LINE__, __FILE__);
            return(false);
        }
        
        DeallocateBufferXY();
        m_nNewGeneratedXPoints = NewPts;
        
        if(NULL == m_nTime_stamp) {
            m_nTime_stamp = new int [NewPts];
            
            if(NULL == m_nTime_stamp) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("AllocateBufferXY(): Memory allocation failed"), __LINE__, __FILE__);
                return E_ERROR;
            }
            
            memset(m_nTime_stamp, 0, sizeof(int)*NewPts);
        }
        
        m_dROI_Fit_Data = new double *[m_nNumOfPoints];
        m_dROI_Fit_Data_Min_Reversed = new double *[m_nNumOfPoints];
        
        if(NULL == m_dROI_Fit_Data || (NULL == m_dROI_Fit_Data_Min_Reversed)) {
            CTiPrepLogHelper::WriteToErrorLog(
                _T("AllocateBufferXY(): Memory allocation failed"), __LINE__, __FILE__);
            DeallocateBufferXY();
            return E_ERROR;
        }
        
        for(int i = 0; i < m_nNumOfPoints; i++) {
            m_dROI_Fit_Data[i] = new double [NewPts];
            
            if(NULL == m_dROI_Fit_Data[i]) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("AllocateBufferXY(): Memory allocation failed"), __LINE__, __FILE__);
                DeallocateBufferXY();
                return E_ERROR;
            }
            
            memset(m_dROI_Fit_Data[i], 0, sizeof(double)*NewPts);
            m_dROI_Fit_Data_Min_Reversed[i] = new double[NewPts];
            
            if(NULL == m_dROI_Fit_Data_Min_Reversed[i]) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("AllocateBufferXY(): Memory allocation failed"), __LINE__, __FILE__);
                DeallocateBufferXY();
                return E_ERROR;
            }
            
            memset(m_dROI_Fit_Data_Min_Reversed[i], 0, sizeof(double)*NewPts);
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in AllocateBufferXY"),__LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
}

/*************************************************************
* Method:    	getNumImagesSelectedOnIsel
* Description: 	This function is used 
* Parameter: 	long * f_nNum_img :
* Returns:   	void
*************************************************************/
void CDisplayData::getNumImagesSelectedOnIsel(long *f_nNum_img)
{
    TIPREP_TRACE(CDisplayData::getNumImagesSelectedOnIsel,MRTraceMarker3);
    if(NULL != f_nNum_img){
        *f_nNum_img = m_nNum_Images_selected;
    } else{
        CTiPrepLogHelper::WriteToErrorLog(_T("Function parameters is NULL"),
            __LINE__, __FILE__);
    }
    
    return;
}
/*************************************************************
* Method:    	GetStudyFileName
* Description: 	This function is used 
* Parameter: 	CString & f_csStudyFileName :
* Returns:   	void
*************************************************************/
void CDisplayData::GetStudyFileName(CString &f_csStudyFileName)
{
    f_csStudyFileName = m_csStudyFileName;
}

/*************************************************************
* Method:    	SetStudyFileName
* Description: 	This function is used 
* Parameter: 	CString & f_csStudyFileName :
* Returns:   	void
*************************************************************/
void CDisplayData::SetStudyFileName(CString &f_csStudyFileName)
{
    m_csStudyFileName = f_csStudyFileName;
}

int CDisplayData::ValidateAndSetTimeStamp( int *f_nTime_stamp,int f_NumImages )//AS/added
{
    if((NULL == f_nTime_stamp) || (f_NumImages<0)){
        CTiPrepLogHelper::WriteToErrorLog(_T("Function parameters is NULL"),
            __LINE__, __FILE__);
        return(E_ERROR_NULL_FUNCPARAMS);
        
    }
    
#define TIME_INTERVAL 10
    
    int l_nDiff = 0;
    l_nDiff = f_nTime_stamp[0] - f_nTime_stamp[1];
    
    if(l_nDiff == 0){   //indicates timestamp not present
        float l_fInitialTi = f_nTime_stamp[0];
        
        for(int img_no = 0; img_no < f_NumImages; img_no++){
            f_nTime_stamp[img_no] = l_fInitialTi + img_no * TIME_INTERVAL;
        }
    }   
    else{
        return(E_NO_ERROR);
    }
    
    return(E_NO_ERROR);
}
int CDisplayData::CheckIntegerLimits(double min ,double max)
{        
    if(max  < INT_MAX && min > INT_MIN  && max  > INT_MIN && min  < INT_MAX){
        return E_NO_ERROR;
    }else{            
        CTiPrepLogHelper::WriteToErrorLog
            (_T("getMinMaxForYaxis(): Data found Beyond the Limits"), __LINE__, __FILE__);
        return(E_ERROR);
    }
}
///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
