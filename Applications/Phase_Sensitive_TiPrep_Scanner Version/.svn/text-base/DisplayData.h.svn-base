// DisplayData.h: interface for the CDisplayData class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_TIPREPDATA_H__E41892AC_2F63_4B19_9CC2_535A5447E707__INCLUDED_)
#define AFX_TIPREPDATA_H__E41892AC_2F63_4B19_9CC2_535A5447E707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <vector>
#include "LineFitStrategy.h"
#include "MapWriter.h"

using namespace std;

#define CONVERT_FACT 10
#define ORG_DATA 0
#define INV_DATA 1
#define FIT_DATA 2

/**
* This class is used to store data used for graph draw and this is singletone class.
*/

class CDisplayData
{
public://construction,copy & destruction
    CDisplayData();
    virtual ~CDisplayData();
    CDisplayData(CDisplayData & f_DisplayData);
    CDisplayData& operator = (CDisplayData& f_DisplayData);
    
public:
    int CheckIntegerLimits(double min ,double max);
    int setROIData();
    int setAnnularMapData(bool avgfilterFlag, int filter_sz, bool isSaveFilterImg, vector<CString > &newSeriesLOIDs);
    int setNoiseROIDataFromRoi();
    void setRoiOrgData(double **f_dRoi_Org_Data);
    void setNoiseRoiOrgData(double **f_dRoi_Org_Data);
    int setImgInfo(long f_lnNum_img, /*input*/vector <CString> & f_csImgLOID);
    int setTimeStampData(int f_nSelectImg);
    void setTiValues(double *f_dROITi_Val, double *f_dROITi_Min_Rev);
    void setErrorValues(double *f_dRoiFit_Err, double *f_dRoiFit_Min_Rev_Err);
    void setT1Values(double *f_dRoi_T1_val, double *f_dRoi_T1_Min_Rev_Val);
    int getSelImgLOID(/*out*/vector <CString> &f_csImgLOID);
    void getSelImgNum(long *f_nNum_img);
    void getNumImagesSelectedOnIsel(long *f_nNum_img);
    int getMinMaxForYaxis(int f_nselectImg, BOOL *f_roi, BOOL *f_DisplayOption, BOOL *f_RevMin, int &f_nYmax, int &f_nYmin);//Mpower 050/130508/AS/Added/Noise Correction feature development/added func para
    int getMinMaxForYaxisMapMode(int f_nselectImg, BOOL f_roi, BOOL *f_NoiseRoi, BOOL *f_DisplayOption, BOOL *f_RevMin, int &f_nYmax, int &f_nYmin,int f_index,BOOL f_bRefreshFlag);
    int getMinMaxForXaxis(int f_nselectImg, int &f_nXmax, int &f_nXmin);
    double** getROIOrgData();
    double** getROIIvnData();
    double** getROIMinRevData();
    double** getROIFitData();
    double** getROIFitMinRevData();
    double* getROITiValues();
    double* getROITiMinRevValues();
    double* getROIT1Values();
    double* getROIT1MinRevValues();
    double* getROIFitErr();
    double* getROIFitMinRevErr();
    double** getNoiseROIOrgData();
    double getNoiseOffsetVal();
    int calcNoiseROIMean(BOOL *f_bNoiseRoi, CString &f_csMean_NoiseRoiVal);
    int performNoiseCorrection();
    BOOL setROI_Inv_Data(int f_nselectImg);
    int AllocateMemory();
    int MemSetROIData();
    int DeAllocateMemory();
    long GetTotalNumberOfPointsInAnnularRegion();
    int GetSelectedIndex();
    void SetSelectedIndex(int f_nSelectedIndex);
    int DeallocateBufferXY();
    int AllocateBufferXY(int NewPts);
    
    int * Time_stamp() const {
        return m_nTime_stamp;
    }
    void GetStudyFileName(CString &f_csStudyFileName);
    void SetStudyFileName(CString &f_csStudyFileName);
    
    //functions
private:
    bool getMinPos(int f_nROI, int f_nselectImg, int *f_nMarker);
    bool MinReverseData(int f_nROI, int f_nselectImg);
    int getMinMaxInArray(double *data_arr, int data_len, double *min, double *max); 
    void setNoiseOffsetVal(double f_dnoiseOffsetVal);   
    int ValidateAndSetTimeStamp(int *f_nTime_stamp,int f_NumImages);
    
    //variables
private:
    //This variable stores number of images selected by user on ISelector.
    long m_nNum_Images_selected;
    //This variable stores image LOID for each selected image on ISelector.
    vector <CString> m_csImageLoid;
    double m_dnoiseOffsetVal;
    //This variable stores number of Pixels in the annular region.
    long m_nNumOfPoints;
    
    int m_nSelectedIndex;
    int m_nNewGeneratedXPoints;
    CString m_csStudyFileName;
    
    //  This variable stores timestamp related to ROI data.
    int *m_nTime_stamp;
    
    //  This variable stores original ROI data.
    double **m_dROI_Org_Data;
    
    //This variable stores inverted ROI data.
    double **m_dROI_Inv_Data;
    //This variable stores min_inverted ROI data.
    double **m_dROI_Inv_Data_Min_Reversed;
    /**
    * This variable stores curve fitting data.
    */
    double **m_dROI_Fit_Data;
    /**
    * This variable stores curve fitting data using min_inverted data.
    */
    double **m_dROI_Fit_Data_Min_Reversed;
    /**
    * This variable stores Ti values for original data.
    */
    double *m_dROI_TI_Val;
    /**
    * This variable stores Ti values for min_inverted data.
    */
    double *m_dROI_TI_Val_Min_Reversed;
    /**
    * This variable stores T1 values for original data.
    */
    double *m_dROI_T1_Val;
    /**
    * This variable stores T1 values for min_inverted data.
    */
    double *m_dROI_T1_Val_Min_Reversed;
    /**
    * This variable stores fitting error value for original data.
    */
    double *m_dROI_Fit_Err;
    /**
    * This variable stores fitting error value for min_inverted data.
    */
    double *m_dROI_Fit_Err_Min_Reversed;
    /**
    * This variable stores original Noise ROI data.
    */
    double **m_dNoise_ROI_Org_Data;
};

#endif // !defined(AFX_TIPREPDATA_H__E41892AC_2F63_4B19_9CC2_535A5447E707__INCLUDED_)