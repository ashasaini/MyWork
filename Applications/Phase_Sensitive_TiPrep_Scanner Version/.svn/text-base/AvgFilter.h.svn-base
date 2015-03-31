// AvgFilter.h: interface for the CAvgFilter class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_AVGFILTER_H__3D4689BA_9C7B_4415_AB58_BBC2DC2492F9__INCLUDED_)
#define AFX_AVGFILTER_H__3D4689BA_9C7B_4415_AB58_BBC2DC2492F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include <math.h>
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "CommonDefinitions.h"
using namespace std;

class CAvgFilter
{
public:
    CAvgFilter();
    virtual ~CAvgFilter();
    
public:
    /*************************************************************
    * Method:    	ApplyAvgFilter
    * Description: 	This function is used 
    * Parameter: 	T * outImg :
    * Parameter: 	T * ImageArea :
    * Parameter: 	int ImageYLength :
    * Parameter: 	int ImageXLength :
    * Parameter: 	int pos_y :
    * Parameter: 	int pos_x :
    * Parameter: 	int filter_sz :
    * Returns:   	int
    *************************************************************/
    template <typename T>
        static int ApplyAvgFilter(T * outImg, T *ImageArea,
        int ImageYLength, int ImageXLength,
        int pos_y, int pos_x, int filter_sz) {
        try {
            if(NULL == outImg || NULL == ImageArea) {
                CString l_csStrFormatted = _T("");
                l_csStrFormatted = _T("ApplyAvgFilter parameter is NULL");
                CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
                return E_ERROR;
            }
            
            if(ImageYLength <= 0 || ImageXLength <= 0) {
                CString l_csStrFormatted = _T("");
                l_csStrFormatted = _T("ApplyAvgFilter ImageXLength value is invalid");
                CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
                return E_ERROR;
            }
            
            if(filter_sz <= 0 || filter_sz % 2 == 0) {
                
                CString l_csStrFormatted = _T("");
                l_csStrFormatted = _T("ApplyAvgFilter filter size value is invalid");
                CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
                return E_ERROR;
            }
            
            double sum = 0;
            int increment = 0;
            int half_filter_size = filter_sz / 2;
            int i = 0, j = 0;
            
            for(i = pos_y - half_filter_size ; i <= (pos_y  + half_filter_size) ; i++) {
                for(j = pos_x - half_filter_size ; j <= (pos_x  + half_filter_size) ; j++, increment++) {
                    if(j < 0 || j >= (ImageXLength) || i < 0 || i >= (ImageYLength)) {
                        continue; //outside image boundary
                    }
                    
                    //get the pixel value
                    sum += (ImageArea[i*ImageXLength + j]/* * g_filter[increment]*/);
                }
            }
            
            *outImg = (double)(sum / (filter_sz * filter_sz));
        } catch(...) {
            CString l_csmsg(_T("Exception occurred in Average Filter"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return E_ERR_EXCEPTION;
        }
        
        return E_NO_ERROR;
    }
};

#endif // !defined(AFX_AVGFILTER_H__3D4689BA_9C7B_4415_AB58_BBC2DC2492F9__INCLUDED_)
