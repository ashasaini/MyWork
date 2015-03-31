// PolarityCorrection.cpp: implementation of the CPolarityCorrection class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "PolarityCorrection.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"
#include "TiPrepController.h"
#include "IniFileReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CPolarityCorrection
//Purpose       : Constructor
//********************************************************************
CPolarityCorrection::CPolarityCorrection():    m_no_of_images_selected(0)

{
    /*TIPREP_TRACE(CPolarityCorrection::CPolarityCorrection);*/
    m_image_loid_magnitude.clear();
}

CPolarityCorrection::CPolarityCorrection(CPolarityCorrection & f_polarity_correction_ptr)
{
    /*TIPREP_TRACE(CPolarityCorrection::CPolarityCorrection);*/
    
    if(f_polarity_correction_ptr.m_image_loid_magnitude.size() > 0) {
        SetImageLOIDForPolarityCorrection(f_polarity_correction_ptr.m_image_loid_magnitude.size(), f_polarity_correction_ptr.m_image_loid_magnitude);
    }
    m_no_of_images_selected = f_polarity_correction_ptr.m_no_of_images_selected;
}
//****************************Method Header***************************
//Method Name   : CPolarityCorrection
//Purpose       : Destructor
//********************************************************************
CPolarityCorrection& CPolarityCorrection::operator = (CPolarityCorrection& f_polarity_correction_ptr)
{
    /*TIPREP_TRACE(CPolarityCorrection::operator);*/
    
    if(this == &f_polarity_correction_ptr) {
        return(*this);
    }
    
    if(f_polarity_correction_ptr.m_image_loid_magnitude.size() > 0) {
        SetImageLOIDForPolarityCorrection(f_polarity_correction_ptr.m_image_loid_magnitude.size(), f_polarity_correction_ptr.m_image_loid_magnitude);
    }
    m_no_of_images_selected = f_polarity_correction_ptr.m_no_of_images_selected;
    return (*this);
}
//****************************Method Header***************************
//Method Name   : CPolarityCorrection
//Purpose       : Overrider
//********************************************************************
CPolarityCorrection::~CPolarityCorrection()
{
    m_image_loid_magnitude.clear();
}

/*************************************************************
* Method:       PolarityCorrectionCalculation
* Description:  This function is used
* Returns:      int
*************************************************************/
int CPolarityCorrection::PolarityCorrectionCalculation(byte* f_image_roi_data[], int f_roi_checked, CLocalDBMgr *f_localdb)
{
    double **l_org_roi_data = NULL;
    short    *l_buffer_size_of_mag_img = NULL;
    CString l_error_display_string(_T(""));
    int l_size = 0;
    unsigned short l_image_width = 0;
    unsigned short l_image_height = 0;
    unsigned short l_image_bit = 0;
    vector<CString> l_out_mag_image_loid;
    int l_image_parsing = 0;
    int l_image_duplication_status = E_NO_ERROR;
    int l_tag_reading_status = E_NO_ERROR;
    int l_y_incement = 0;
    int l_x_incement = 0;
    int l_increment = 0;
    float l_img_scale_factor =0;
    void *l_img_data = NULL;
    int l_min_value_index_postion[MAX_NUM_ROI] = {0,0,0};  //changed
    CString l_cseries (_T(""));
	CIniFileReader l_ini_reader;
	int l_proc_roi_option = 0;
    try {
        if(f_image_roi_data == NULL || f_localdb == NULL) {
            CString l_csmsg(_T("PolarityCorrectionCalculation() Parameters are NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }

		int l_ini_reader_status = l_ini_reader.GetProcOption(l_proc_roi_option);
		if(E_NO_ERROR != l_ini_reader_status) {
			CTiPrepLogHelper::WriteToErrorLog(_T("ini file not read properly"), __LINE__, __FILE__);
			return l_ini_reader_status;
		}
        
        if(m_image_loid_magnitude.size() > 0) {
            l_image_duplication_status = CTiPrepController::GetInstance()->CreateSeries(m_image_loid_magnitude,l_out_mag_image_loid); 
        }
        
        if(E_NO_ERROR != l_image_duplication_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("duplication of protocol unsuccessful"), __LINE__, __FILE__);
            return l_image_duplication_status;
        }
        
        CCommonCalculations l_common_calc;
        l_org_roi_data = new double *[MAX_NUM_ROI];
        long l_no_of_selected_images = SetNoOfSelectedImages();
        
        for(int l_roi_parsing = 0; l_roi_parsing < MAX_NUM_ROI; l_roi_parsing++) {
            l_org_roi_data[l_roi_parsing] = new double [l_no_of_selected_images];
        }
        
        int l_fun_execution_status = l_common_calc.FillingROIBuffer(l_min_value_index_postion, l_out_mag_image_loid, m_image_loid_magnitude.size(), l_org_roi_data); 
        
        if(E_NO_ERROR != l_fun_execution_status) {
            for(int l_roi_deletion = 0; l_roi_deletion < MAX_NUM_ROI; l_roi_deletion++) {
                if(NULL != l_org_roi_data [l_roi_deletion]) {
                    delete  []l_org_roi_data [l_roi_deletion];
                    l_org_roi_data[l_roi_deletion] = NULL;
                }
            }                
            if(NULL != l_org_roi_data) {
                delete [] l_org_roi_data;
                l_org_roi_data = NULL;
            }
            
            CTiPrepLogHelper::WriteToErrorLog(_T("ROI data not filled properly"), __LINE__, __FILE__);
            return l_fun_execution_status;
        }
        
        for(l_image_parsing ; l_image_parsing < (signed) l_out_mag_image_loid.size(); l_image_parsing++) {
            l_img_data = NULL;
            l_tag_reading_status = f_localdb->TagReadingForImageData(l_out_mag_image_loid.at(l_image_parsing),
                &l_size,
                &l_image_width,
                &l_image_height,
                &l_image_bit,
                &l_img_data,
                &l_img_scale_factor);
            
            if(l_tag_reading_status != 0 || NULL == l_img_data) {
                
                if(NULL != l_img_data) {
                    delete l_img_data;
                    l_img_data = NULL;
                }
                for(int l_roi_deletion = 0; l_roi_deletion < MAX_NUM_ROI; l_roi_deletion++) {
                    if(NULL != l_org_roi_data [l_roi_deletion]) {
                        delete  []l_org_roi_data [l_roi_deletion];
                        l_org_roi_data[l_roi_deletion] = NULL;
                    }
                }                
                if(NULL != l_org_roi_data) {
                    delete [] l_org_roi_data;
                    l_org_roi_data = NULL;
                }
				CTiPrepLogHelper::WriteToErrorLog(_T("TagReadingForImageData failed"), __LINE__, __FILE__);
                return E_ERR_IMG_LOAD_FAIL;
            }
            
            short *l_magnitude_buffer = NULL;
            l_magnitude_buffer = (short*)l_img_data;
             l_buffer_size_of_mag_img = new short[l_image_width*l_image_height];
             if(NULL == l_buffer_size_of_mag_img || NULL == l_magnitude_buffer){
                 if(NULL != l_img_data) {
                     delete l_img_data;
                     l_img_data = NULL;
                 }
                 for(int l_roi_deletion = 0; l_roi_deletion < MAX_NUM_ROI; l_roi_deletion++) {
                     if(NULL != l_org_roi_data [l_roi_deletion]) {
                         delete  []l_org_roi_data [l_roi_deletion];
                         l_org_roi_data[l_roi_deletion] = NULL;
                     }
                 }                
                 if(NULL != l_org_roi_data) {
                     delete [] l_org_roi_data;
                     l_org_roi_data = NULL;
                 }
                 CTiPrepLogHelper::WriteToErrorLog(_T("l_buffer_size_of_mag_img NULL"), __LINE__, __FILE__);
                return E_ERROR;
             }
            l_increment = 0;
            
            for(l_y_incement = 0; l_y_incement < l_image_height ; l_y_incement++) {
                for(l_x_incement = 0; l_x_incement < l_image_width ; l_x_incement++) {
                    
                    if(l_proc_roi_option > 0 && l_image_parsing <= l_min_value_index_postion[f_roi_checked]) {
                        short l_tmp_buffer_for_calculation = 0;
                        l_tmp_buffer_for_calculation =  l_magnitude_buffer[l_increment];
                        l_tmp_buffer_for_calculation = (-1) * l_tmp_buffer_for_calculation ;
                        ((short*)l_buffer_size_of_mag_img)[l_increment] = l_tmp_buffer_for_calculation;
                    } else {
                        ((short*)l_buffer_size_of_mag_img)[l_increment] = l_magnitude_buffer[l_increment];
                    }
                    
                    l_increment++;
                }
            }
            
            int l_img_buffer_update_status = f_localdb->UpdateImageBuffer(l_out_mag_image_loid.at(l_image_parsing), (void *)l_buffer_size_of_mag_img, l_size);
            
            if(E_NO_ERROR != l_img_buffer_update_status) {
                CTiPrepLogHelper::WriteToErrorLog(_T("Image buffer update failed"), __LINE__, __FILE__);
                if(NULL != l_img_data) {
                    delete l_img_data;
                    l_img_data = NULL;
                }
                if(NULL != l_buffer_size_of_mag_img) {
                    delete [] l_buffer_size_of_mag_img;
                    l_buffer_size_of_mag_img = NULL;
                }
                for(int l_roi_deletion = 0; l_roi_deletion < MAX_NUM_ROI; l_roi_deletion++) {
                    if(NULL != l_org_roi_data [l_roi_deletion]) {
                        delete  []l_org_roi_data [l_roi_deletion];
                        l_org_roi_data[l_roi_deletion] = NULL;
                    }
                }                
                if(NULL != l_org_roi_data) {
                    delete [] l_org_roi_data;
                    l_org_roi_data = NULL;
                }
                return l_img_buffer_update_status;
            }
            
            if(NULL != l_buffer_size_of_mag_img) {
                delete [] l_buffer_size_of_mag_img;
                l_buffer_size_of_mag_img = NULL;
            }
        }
        
        for(int l_roi_deletion = 0; l_roi_deletion < MAX_NUM_ROI; l_roi_deletion++) {
            if(NULL != l_org_roi_data [l_roi_deletion]) {
                delete  []l_org_roi_data [l_roi_deletion];
                l_org_roi_data[l_roi_deletion] = NULL;
            }
        }
        
        if(NULL != l_org_roi_data) {
            delete [] l_org_roi_data;
            l_org_roi_data = NULL;
        }
        
        if(NULL != l_img_data) {
            delete l_img_data;
            l_img_data = NULL;
        }
    } catch(...) {
        for(int l_roi_deletion = 0; l_roi_deletion < MAX_NUM_ROI; l_roi_deletion++) {
            if(NULL != l_org_roi_data [l_roi_deletion]) {
                delete  []l_org_roi_data [l_roi_deletion];
                l_org_roi_data[l_roi_deletion] = NULL;
            }
        }
        if(NULL != l_org_roi_data) {
            delete [] l_org_roi_data;
            l_org_roi_data = NULL;
        }
        
        if(NULL != l_img_data) {
            delete l_img_data;
            l_img_data = NULL;
        }
        if(NULL != l_buffer_size_of_mag_img) {
            delete [] l_buffer_size_of_mag_img;
            l_buffer_size_of_mag_img = NULL;
        }
        CString l_csmsg(_T("Exception occured in polarity correction calculation"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       SetImageLOIDForPolarityCorrection
* Description:  This function is used to set the selected image loid
* Parameter:    long f_no_of_images_selected
* Parameter:    vector <CString> f_image_loid
* Returns:      int
*************************************************************/
int CPolarityCorrection::SetImageLOIDForPolarityCorrection(long f_no_of_images_selected,
                                                           /*In*/vector <CString> & f_image_loid)
{
    TIPREP_TRACE(CPolarityCorrection::SetImageLOIDForPolarityCorrection,MRTraceMarker3);
    try{
        vector<CString>::iterator l_image_iterator;
        m_no_of_images_selected = f_no_of_images_selected;
        m_image_loid_magnitude.clear();
        l_image_iterator = f_image_loid.begin();
        
        for(l_image_iterator = f_image_loid.begin() ; l_image_iterator < f_image_loid.end(); l_image_iterator++) {
            m_image_loid_magnitude.push_back(*l_image_iterator);
        }
    }catch(...){
        CString l_csmsg(_T("Exception occured in SetImageLOIDForPolarityCorrection"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       SetNoOfSelectedImages
* Description:  This function is used
* Returns:      long
*************************************************************/
long CPolarityCorrection :: SetNoOfSelectedImages()
{
    long l_no_of_selected_images = m_no_of_images_selected;
    return l_no_of_selected_images;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////