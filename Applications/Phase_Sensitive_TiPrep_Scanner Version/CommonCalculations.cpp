// CommonCalculations.cpp: implementation of the CCommonCalculations class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "CommonCalculations.h"
#include "TiPrepView.h"
#include "CommonDefinitions.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonCalculations::CCommonCalculations()
{
    /*TIPREP_TRACE(CCommonCalculations::CCommonCalculations);*/
}

CCommonCalculations::~CCommonCalculations()
{
    //
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       FillingROIBuffer
* Description:  This function is used
* Parameter:    int * f_min_value_index_postion
* Parameter:    vector <CString> & f_image_loid
* Parameter:    long f_nNumImages
* Parameter:    double * * f_roi_buffer
* Returns:      int
*************************************************************/
int CCommonCalculations::FillingROIBuffer(int *f_min_value_index_postion,
                                          /*input*/vector <CString> &f_image_loid,
                                          long f_nNumImages,
                                          double **f_org_roi_data)
{
    if(NULL == f_min_value_index_postion || NULL == f_org_roi_data) {
        CString l_error_display_string(_T("data pointers are NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
        return E_ERROR_NULL_FUNCPARAMS;
    }
    
    try {
        TIPREP_TRACE(CCommonCalculations::FillingROIBuffer,MRTraceMarker3);
        int l_no_of_roi = 0;
        void *l_image_data = NULL;
        long l_actual_image = 0;
        int l_image_size = 0;
        byte* l_image_data_inside_roi[MAX_LINE_ROI];
        memset(l_image_data_inside_roi, 0x0, (MAX_LINE_ROI * sizeof(byte*)));
        int l_no_of_roi_column = 0;
        int l_no_of_roi_row = 0;
        int l_no_of_images = 0;
        unsigned short l_image_data_type = 0;
        
        BOOL l_roi_buffer_info_status = 
            (CTiPrepView::GetView())->GetROIInstance()->GetROIBufferInfo
            (l_image_data_inside_roi, &l_no_of_roi_column, &l_no_of_roi_row);
        
        if(!l_roi_buffer_info_status) {
            CString l_error_display_string
                (_T("FillingROIBuffer(): GetROIBufferInfo() returned error"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ROI_MAP;
        }
        
        if(NULL == l_image_data_inside_roi) {
            CString l_error_display(_T("Map is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        int l_count = 0;
        
        for(l_no_of_roi = 0; l_no_of_roi < MAX_NUM_ROI; l_no_of_roi++) {
            memset(f_org_roi_data[l_no_of_roi], 0, f_nNumImages * sizeof(double));
        }
        
        CLocalDBMgr l_localdbmgr;
        for(l_no_of_images ; l_no_of_images < (signed) f_image_loid.size(); l_no_of_images++) {
            int l_time_stamp = 0;
            float l_scale = 0.0;
//             int l_image_data_status = 
//                 (CTiPrepView::GetView())->GetLocalDBImgData
//                 (f_image_loid.at(l_no_of_images), &l_image_data, 
//                 &l_image_size, &l_scale, &l_time_stamp, &l_image_data_type,l_no_of_images);
            int l_image_data_status = l_localdbmgr.getImgDataFromDB(f_image_loid.at(l_no_of_images), &l_image_data, 
                &l_image_size, &l_scale, &l_time_stamp, &l_image_data_type,l_no_of_images);
            
            if(l_image_data_status != E_NO_ERROR) {
                CString l_error_display
                    (_T("FillingROIBuffer(): GetLocalDBImgData() returned error"));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                if(l_image_data) {
                    /*free*/operator delete(l_image_data);
                    l_image_data = NULL;
                }
                return l_image_data_status;
            }
            
            for(l_no_of_roi = 0; l_no_of_roi < MAX_NUM_ROI; l_no_of_roi++) {
                l_count = 0;
                f_org_roi_data[l_no_of_roi][l_no_of_images] = 0;
                
                for(int l_roi_column = 0; l_roi_column < l_no_of_roi_column; l_roi_column++) {
                    for(int l_roi_row = 0; l_roi_row < l_no_of_roi_row; l_roi_row++) {
                        int l_image_index = (l_no_of_roi_row) * l_roi_column + l_roi_row;
                        
                        if(1 == l_image_data_inside_roi[l_no_of_roi][l_image_index]) {
                            switch(l_image_data_type) {
                            case 16: {
                                INT16 temp = 0;
                                memcpy(&temp, ((unsigned char*)(l_image_data) + l_image_index * 2), 
                                    sizeof(INT16));
                                l_actual_image = l_actual_image + temp;
                                l_count++;
                                break;
                                     }
                            case 32: {
                                INT32 temp = 0;
                                memcpy(&temp, ((unsigned char*)(l_image_data) + l_image_index * 2), 
                                    sizeof(INT32));
                                l_actual_image = l_actual_image + temp;
                                l_count++;
                                break;
                                     }
                            default:
                                break;
                            }
                        }
                    }
                }
                
                if(0.00001 < fabs(/*l_scale * */l_count)) {
                    f_org_roi_data[l_no_of_roi][l_no_of_images] = 
                        (double)l_actual_image / (l_count /** l_scale*/);
                }
                
                l_actual_image = 0;
                
                if(0 == l_count) {
                    CString l_error_display(_T(""));
                    CString l_error_display_string
                        (_T("FillingROIBuffer(): ROI is placed outside of image region"));
                    CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                    return I_ROI_OUT_RGN;
                }
            }
            
            if(l_image_data) {
                /*free*/operator delete(l_image_data);
                l_image_data = NULL;
            }
        }
        
        for(l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
            CoTaskMemFree(l_image_data_inside_roi[l_no_of_roi]);
            l_image_data_inside_roi[l_no_of_roi] = NULL;
        }
        
        int l_min_data_value_index[MAX_NUM_ROI] = {0,0,0};
        double l_min_sig_data = NULL;
        int l_nStatus = CalculateMinSignalDataAndIndex(f_image_loid.size(),
            f_org_roi_data,
            l_min_data_value_index,
            &l_min_sig_data,
            f_min_value_index_postion);
        if(E_NO_ERROR != l_nStatus){
            CString l_error_display_string
                (_T("FillingROIBuffer(): CalculateMinSignalDataAndIndex returned error"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(l_nStatus);
        }
    } catch(CMemoryException& l_memory_exception) {
        TCHAR   l_cause_of_error[255];
        CString l_error_display_string;
        l_memory_exception.GetErrorMessage(l_cause_of_error, 255);
        l_error_display_string = _T("Memory can not be allocated because of this error: ");
        l_error_display_string += l_cause_of_error;
        CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in FillingROIBuffer"), __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       CalculateMinSignalDataAndIndex
* Description:  This function is used
* Parameter:    long f_no_of_selected_images
* Parameter:    double * * f_org_roi_data
* Parameter:    int * f_min_data_value_index
* Parameter:    double * f_min_sig_data
* Returns:      int
*************************************************************/
int CCommonCalculations::CalculateMinSignalDataAndIndex(long f_no_of_selected_images,
                                                        double **f_org_roi_data,
                                                        int *f_min_data_value_index,
                                                        double *f_min_sig_data,
                                                        int *f_min_value_index)
{
    TIPREP_TRACE(CCommonCalculations::CalculateMinSignalDataAndIndex,MRTraceMarker3);
    
    try {
        if((NULL == f_org_roi_data) || (NULL == f_min_data_value_index) || (NULL == f_min_sig_data)) {
            CString l_error_display_string(_T("data pointers are NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        int l_no_of_roi = 0;
        int l_no_of_selected_images = 0;
      //add null chk
		memset(f_min_value_index,0,(MAX_NUM_ROI*sizeof(int)));
        
        for(l_no_of_roi = 0; l_no_of_roi < MAX_NUM_ROI; l_no_of_roi++) {
            for(l_no_of_selected_images = 0; 
            l_no_of_selected_images < f_no_of_selected_images; 
            l_no_of_selected_images++) {
                if(0 == l_no_of_selected_images) {
                    f_min_sig_data[l_no_of_roi] = 
                        f_org_roi_data[l_no_of_roi][l_no_of_selected_images];
                    f_min_data_value_index[l_no_of_roi] = l_no_of_selected_images;
                } else if(f_org_roi_data[l_no_of_roi][l_no_of_selected_images] 
                    <= f_min_sig_data[l_no_of_roi]) {
                    f_min_data_value_index[l_no_of_roi] = l_no_of_selected_images;
                    f_min_value_index[l_no_of_roi] = f_min_data_value_index[l_no_of_roi];// or l_no_of_selected_images
                    f_min_sig_data[l_no_of_roi] = f_org_roi_data[l_no_of_roi][l_no_of_selected_images];
                }
            }
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in common calculations"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
