// PhaseCorrection.h: interface for the CPhaseCorrection class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_PHASECORRECTION_H__74A03920_0027_4BFE_9B66_E9419642320A__INCLUDED_)
#define AFX_PHASECORRECTION_H__74A03920_0027_4BFE_9B66_E9419642320A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <math.h>
#include "TiPrepTrace.h"
#include "CommonDefinitions.h"
#include "TiPrepLogHelper.h"
using namespace std;

class CPhaseCorrection
{
public:
    CPhaseCorrection();
    virtual ~CPhaseCorrection();
    void setImgInfo(long f_lnNum_img, vector <CString> &f_csImgLOID);
    int GetNumSelectedImgs() {
        return m_nNum_Images_selected;
    }
    CString GetFirstLOID() {
        if(m_csImageLoid.size() > 0) {
            return m_csImageLoid.at(0);
        } else {
            return CString(_T(""));
        }
    }
    void  getSelImgLOID(vector <CString> &f_csImgLOID);
    void SetRealSeriesLOID(/*long f_lnNum_img,*/ vector <CString> &f_csImgLOID);
    void SetImgnSeriesLOID(/*long f_lnNum_img,*/ vector <CString> &f_csImgLOID);
    void  getRealImgLOID(vector <CString> &f_csImgLOID);
    void  getImgnImgLOID(vector <CString> &f_csImgLOID);
    
    //Calculate real component
    template <class T>
        T CalcReal(T r1, T i1,  T r2, T i2) {
        T result = 0;
        result = (T)((r1 * r2 + i1 * i2) / (sqrt(r2 * r2 + i2 * i2)));
        return (result);
    }
    
    //Calculate Imaginary component
    template <class T>
        
        T CalcImg(T r1, T i1,  T r2, T i2) {
        T result = 0;
        
        if(r2 < 0.0000001) {
            CString lcserrmsg(_T("CalcImg() r2 is 0,avoiding Divide by zero"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
        } else {
            result = (T)(i1 * cos(atan2(i2 , r2)) - r1 * sin(atan2(i2 , r2)));
        }
        
        return (result);
    }
    
    //Calculate Magnitude component
    template <class T>
        
        T CalcMg(T r1, T i1,  T r2, T i2) {
        T result = 0, real = 0, img = 0;
        
        if(r2 < 0.0000001) {
            CString lcserrmsg(_T("CalcMg() r2 is 0,avoiding Divide by zero"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
        } else {
            real = (T)(r1 *  cos(atan2(i2 , r2)) + i1 * sin(atan2(i2 , r2)));
            img = (T)(i1 * cos(atan2(i2 , r2)) - r1 * sin(atan2(i2 , r2)));
            result = (T)(sqrt((real * real) + (img * img)));
        }
        
        return (result);
    }
    
    //Phase Correction calc
    template <typename T>
        int PhaseCorrectionT(T * outImg, T *ImageArea_real, T *ImageArea_imgr,
        T *ImageArea_real2, T *ImageArea_imgr2,
        T *ImageArea_mag, T *ImageArea_mag2,
        int newImageYLength, int newImageXLength, int datatype , byte * roiBuff,
        int OutDataForm, bool inROI, bool doroiavg, double roiavgval_re, double roiavgval_im) {
        int i = 0, j = 0;
        int increment = 0;
        bool isInROIRegion = true;
        TIPREP_TRACE(CPolarityCorrection: PhaseCorrection,MRTraceMarker3);
        int status = 0;
        
        if(NULL == outImg || NULL == ImageArea_real || ImageArea_imgr == NULL || NULL == ImageArea_real2
            || NULL == ImageArea_imgr2 || NULL == roiBuff) {
            CString lcserrmsg(_T("PhaseCorrectionT() Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        for(i = 0; i < newImageYLength; i++) {          
            for(j = 0; j < newImageXLength; j++) {
                if(/*isInROIRegion*/1 == roiBuff[i*newImageXLength+j]) {
                    if(ImageArea_real[increment] != 0) {
                        T real2 = 0, img2 = 0;
                        
                        if(!doroiavg) {
                            real2 = ImageArea_real2[increment];
                            img2 = ImageArea_imgr2[increment];
                        } else {
                            real2 = (T)roiavgval_re; 
                            img2 = (T)roiavgval_im;
                        }
                        
                        switch(OutDataForm) {
                        case REAL:
                            outImg[increment] = CalcReal(ImageArea_real[increment], ImageArea_imgr[increment], real2 , img2);
                            break;
                        case IMGN:
                            outImg[increment] = CalcImg(ImageArea_real[increment], ImageArea_imgr[increment], real2, img2);
                            break;
                        case MAGNITUDE:
                            outImg[increment] = CalcMg(ImageArea_real[increment], ImageArea_imgr[increment], real2, img2);
                            break;
                        default:
                            CString lcserrmsg(_T("PhaseCorrectionT() dataform invalid"));
                            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
                            break;
                        }
                    }
                } else {
                    switch(OutDataForm) {
                    case REAL:
                        outImg[increment] = ImageArea_real[increment];
                        break;
                    case IMGN:
                        outImg[increment] = ImageArea_imgr[increment];
                        break;
                    case MAGNITUDE:
                        outImg[increment] = ImageArea_mag[increment];
                        break;
                    default:
                        CString lcserrmsg(_T("PhaseCorrectionT() dataform invalid"));
                        CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
                        break;
                    }
                }
                
                increment ++;
            }
        }
        
        return  0;
    };
    
    CPhaseCorrection(CPhaseCorrection& f_view);
    CPhaseCorrection& operator = (CPhaseCorrection& f_view);
    status_t checkSelection(imgProcessData_t *f_imgData);
    
private:
    int m_selectd_image_dataform;
    vector <CString> m_csImageLoid;
    vector <CString> m_csRealLoid;
    vector <CString> m_csImgnLoid;
    
    int m_nNum_Images_selected;
};
#endif // !defined(AFX_PHASECORRECTION_H__74A03920_0027_4BFE_9B66_E9419642320A__INCLUDED_)
