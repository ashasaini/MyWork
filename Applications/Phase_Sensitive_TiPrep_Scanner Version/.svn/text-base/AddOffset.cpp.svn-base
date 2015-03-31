// AddOffset.cpp: implementation of the CAddOffset class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "AddOffset.h"
#include "CommonCalculations.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"
#include "TiPrepController.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define MAX_IMAGE_NODE 1024
#define DATA_BIT_16 16
#define DATA_BIT_32 32
const CString EXCEPTION_ADDOFFSETCALC = L"Exception occurred in Add Offset Calculation";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//

/*************************************************************
* Method:    	CAddOffset
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CAddOffset::CAddOffset(): m_no_of_images_selected(0), m_offset_value(_T(""))
{
    /*TIPREP_TRACE(CAddOffset::CAddOffset);*/
}

/*************************************************************
* Method:    	~CAddOffset
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CAddOffset::~CAddOffset()
{
    m_image_loid_addoffset.clear();
}

/*************************************************************
* Method:    	CAddOffset
* Description: 	This function is used 
* Parameter: 	CAddOffset & f_add_offset_ptr :
* Returns:   	
*************************************************************/
CAddOffset::CAddOffset(CAddOffset & f_add_offset_ptr)
{
    /*TIPREP_TRACE(CAddOffset::CAddOffset);*/
    
    try {
        m_offset_value = (_T(""));
        m_no_of_images_selected = f_add_offset_ptr.m_no_of_images_selected;
        
        if(f_add_offset_ptr.m_image_loid_addoffset.size() > 0) {
            SetImageLOIDForAddOffset(f_add_offset_ptr.m_image_loid_addoffset.size(),
                f_add_offset_ptr.m_image_loid_addoffset);
        }
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in add offset copy constructor"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    }
}


/*************************************************************
* Method:    	operator=
* Description: 	This function is used 
* Parameter: 	CAddOffset & f_add_offset_ptr :
* Returns:   	CAddOffset&
*************************************************************/
CAddOffset& CAddOffset::operator = (CAddOffset& f_add_offset_ptr)
{
    /*TIPREP_TRACE(CAddOffset::operator);*/
    
    try {
        if(this == &f_add_offset_ptr) {
            return(*this);
        }
        
        if(f_add_offset_ptr.m_image_loid_addoffset.size() > 0) {
            SetImageLOIDForAddOffset(f_add_offset_ptr.m_image_loid_addoffset.size(),
                f_add_offset_ptr.m_image_loid_addoffset);
        }
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in add offset assignment operator"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    }
    
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       SetImageLOIDForAddOffset
* Description:  This function is used to set the selected image_loid
* Parameter:    long f_no_of_images_selected
* Parameter:    vector <CString> f_image_loid
* Returns:      int
*************************************************************/
int CAddOffset::SetImageLOIDForAddOffset(long f_no_of_images_selected,
        /*In*/vector <CString> & f_image_loid)
{
    TIPREP_TRACE(CAddOffset::SetImageLOIDForAddOffset,MRTraceMarker3);
    
    try {
        vector<CString>::iterator l_image_iterator;
        CString l_selected_protocol_name(_T(""));
        CString l_selected_series_no(_T(""));
        m_image_loid_addoffset.clear();
        l_image_iterator = f_image_loid.begin();
        m_no_of_images_selected = f_no_of_images_selected;
        
        for(l_image_iterator = f_image_loid.begin() ; 
        l_image_iterator < f_image_loid.end(); 
        l_image_iterator++) {
            m_image_loid_addoffset.push_back(*l_image_iterator);
        }
        
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETCALC, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}
/*************************************************************
* Method:       GetSelectedImageLOIDForAddOffset
* Description:  This function is used to get the selected image_loid
* Parameter:    vector <CString> & f_image_loid
* Returns:      int
*************************************************************/
int CAddOffset:: GetSelectedImageLOIDForAddOffset(/*out*/vector <CString> &f_image_loid)
{
    TIPREP_TRACE(CAddOffset::GetSelectedImageLOIDForAddOffset,MRTraceMarker3);
    
    try {
        vector<CString>::iterator l_image_iterator;
        l_image_iterator = m_image_loid_addoffset.begin();
        f_image_loid.clear();
        
        for(l_image_iterator = m_image_loid_addoffset.begin() ;
        l_image_iterator < m_image_loid_addoffset.end();
        l_image_iterator++) {
            f_image_loid.push_back(*l_image_iterator);
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETCALC, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetOffsetData
* Description: 	This function is used 
* Parameter: 	CString f_offset_value :
* Parameter: 	int f_add_offset_check_box_status :
* Parameter: 	byte * f_image_roi_data :
* Parameter: 	int f_roi_checked :
* Returns:   	int
*************************************************************/
int CAddOffset::GetOffsetData(CString &f_offset_value,
                              int f_add_offset_check_box_status,
                              byte* f_image_roi_data,
                              int f_roi_checked,
                              int *f_actual_expected_offset_value)
{
    TIPREP_TRACE(CAddOffset::GetOffsetData,MRTraceMarker3);
    int l_size = 0;
    unsigned short l_image_width = 0;
    unsigned short l_image_height = 0;
    unsigned short l_image_bit = 0;
    int l_image_parsing = 0;
    int l_tag_reading_status = E_NO_ERROR;
    float l_img_scale_factor =0;
    void *l_img_data = NULL;
    char l_conversion_string[150];
	//int l_actual_expected_offset_value =0; //-Mpower/20130722/PN/added//OFFSET CHECK 
    
    try {
        memset(l_conversion_string, 0, 150 *(sizeof(char)));
        m_offset_value = f_offset_value;
        wcstombs(l_conversion_string, m_offset_value, 150);
        int l_adding_offset_to_image = 0;
        l_adding_offset_to_image = atof(l_conversion_string);
        m_offset_value.Format(_T("%lf"), l_adding_offset_to_image);
        vector<CString> l_out_image_loid;
        CLocalDBMgr l_localdb;
        
        int l_status_of_valid_offset_check =CalculateValidOffset(l_adding_offset_to_image,
            f_add_offset_check_box_status,f_roi_checked,f_actual_expected_offset_value);
        
        if(l_status_of_valid_offset_check != E_NO_ERROR) {
            return l_status_of_valid_offset_check;
		}
        if(m_image_loid_addoffset.size() > 0) {
            int l_image_duplication_status = E_NO_ERROR;
//             l_image_duplication_status = l_localdb.CreateSeriesCopy(m_image_loid_addoffset,
//                 l_out_image_loid,l_csseries);
            l_image_duplication_status = CTiPrepController::GetInstance()->CreateSeries(m_image_loid_addoffset,l_out_image_loid);
            if(E_NO_ERROR != l_image_duplication_status) {
                CTiPrepLogHelper::WriteToErrorLog(_T("duplication of protocol unsuccessful"), __LINE__, __FILE__);
                return l_image_duplication_status;
            }
        }
        
        CCommonCalculations *l_common_calc = new CCommonCalculations();
        
        if(NULL == l_common_calc) {
            CTiPrepLogHelper::WriteToErrorLog(_T("common calculation pointer NULL"), __LINE__, __FILE__);
            return E_MEM_ALLOC;
        }
        
        double **l_org_roi_data = new double *[MAX_NUM_ROI];
        long l_no_of_selected_images = 0;
        l_no_of_selected_images = m_no_of_images_selected;
        
        for(int l_roi_parsing = 0; l_roi_parsing < MAX_NUM_ROI; l_roi_parsing++) {
            l_org_roi_data[l_roi_parsing] = new double [l_no_of_selected_images];
        }
        
        int l_min_index_default_value = 0;
        int l_fun_execution_status = l_common_calc-> FillingROIBuffer(
            &l_min_index_default_value, m_image_loid_addoffset, 
            l_no_of_selected_images, l_org_roi_data);
        
        if(E_NO_ERROR != l_fun_execution_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("ROI data not filled properly"), __LINE__, __FILE__);
            return l_fun_execution_status;
        }
        
        for(l_image_parsing = 0; l_image_parsing < (signed) l_out_image_loid.size(); l_image_parsing++) {
            l_img_data = NULL;
            l_tag_reading_status = l_localdb.TagReadingForImageData(
                l_out_image_loid.at(l_image_parsing),
                &l_size,
                &l_image_width,
                &l_image_height,
                &l_image_bit,
                &l_img_data,
                &l_img_scale_factor);
            
            if(l_tag_reading_status != 0) {
                return l_tag_reading_status;
            }

		
			if(l_image_bit ==0) {
				CTiPrepLogHelper::WriteToErrorLog(_T("image bit is zero"), __LINE__, __FILE__);
				return E_ERROR;
			}
			void *l_buffer_size_of_img = malloc(l_image_width*l_image_height*l_image_bit);	
            int l_y_incement = 0;
            int l_x_incement = 0;
            int l_increment = 0;
            double l_roi_data_to_be_added = 0;
            l_increment = 0;
            
            for(l_y_incement = 0; l_y_incement < l_image_height ; l_y_incement++) {
                for(l_x_incement = 0; l_x_incement < l_image_width ; l_x_incement++) {
					if(l_image_bit == DATA_BIT_16) {
                    short l_tmp_buffer_for_calculation = 0;
						l_tmp_buffer_for_calculation = ((short *)l_img_data)[l_increment];
                    
                    if(1 == f_image_roi_data[l_increment]) {
							if(BST_CHECKED == f_add_offset_check_box_status) {
                            int l_fun_execution_status = AddOffsetCalculation(
                                l_org_roi_data, f_roi_checked, &l_roi_data_to_be_added,l_image_parsing,l_img_scale_factor,f_actual_expected_offset_value);
                            
                            if(E_NO_ERROR != l_fun_execution_status) {
                                CTiPrepLogHelper::WriteToErrorLog(_T("Offset calculation failed"), __LINE__, __FILE__);
                                return l_fun_execution_status;
                            }
                            
                            ((short*)l_buffer_size_of_img)[l_increment] =  
                                l_tmp_buffer_for_calculation + l_roi_data_to_be_added;
                        } else {
                            ((short*)l_buffer_size_of_img)[l_increment] =  
                                l_tmp_buffer_for_calculation + ((*f_actual_expected_offset_value)*l_img_scale_factor);
                        }
                    } else {
                        ((short*)l_buffer_size_of_img)[l_increment] = l_tmp_buffer_for_calculation;
						}
					} else if(l_image_bit == DATA_BIT_32){
						int l_tmp_buffer_for_calculation = 0;
						l_tmp_buffer_for_calculation =((int*)  l_img_data)[l_increment];
						if(1 == f_image_roi_data[l_increment]) {
							if(1 == f_add_offset_check_box_status) {
								int l_fun_execution_status = AddOffsetCalculation(
									l_org_roi_data, f_roi_checked, &l_roi_data_to_be_added,l_image_parsing,l_img_scale_factor,f_actual_expected_offset_value);
								if(E_NO_ERROR != l_fun_execution_status) {
									CTiPrepLogHelper::WriteToErrorLog(_T("Offset calculation failed"), __LINE__, __FILE__);
									return l_fun_execution_status;
								}
								((int*)l_buffer_size_of_img)[l_increment] =  
									l_tmp_buffer_for_calculation + l_roi_data_to_be_added;
							} else {
								((int*)l_buffer_size_of_img)[l_increment] =  
									l_tmp_buffer_for_calculation + ((*f_actual_expected_offset_value)*l_img_scale_factor);
							}
						} else {
							((int*)l_buffer_size_of_img)[l_increment] = l_tmp_buffer_for_calculation;
						}
                    }
                     l_increment++;
                }
            }
            
            int l_img_buffer_update_status = 
                l_localdb.UpdateImageBuffer(l_out_image_loid.at(l_image_parsing), 
                (void *)l_buffer_size_of_img, l_size);
            
            if(E_NO_ERROR != l_img_buffer_update_status) {
                CTiPrepLogHelper::WriteToErrorLog(_T("Image buffer update failed"), __LINE__, __FILE__);
                return l_img_buffer_update_status;
            }
            
            if(NULL != l_buffer_size_of_img) {
                free (l_buffer_size_of_img);
                l_buffer_size_of_img = NULL;
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
        
        if(NULL != l_common_calc) {
            delete l_common_calc;
            l_common_calc = NULL;
        }
    }
    catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETCALC, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	AddOffsetCalculation
* Description: 	This function is used 
* Parameter: 	double * * f_roi_buffer :
* Parameter: 	int f_selected_roi :
* Parameter: 	double * f_roi_data_to_be_added :
* Returns:   	int
*************************************************************/
int CAddOffset :: AddOffsetCalculation(double** f_roi_buffer, 
                                       int f_selected_roi, 
                                       double *f_roi_data_to_be_added,
                                       int f_adding_offset_to_image, 
                                       float f_img_scale_factor,
                                       int *f_actual_expected_offset_value)
{
    try {
        if(NULL == f_roi_buffer || NULL == f_roi_data_to_be_added)
        {
            CString l_csmsg(_T("Function parameters are NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        }
        
       // int l_adding_offset_to_image = 0;
        //int l_roi_buffer_memory_allocation = 0;
        //double l_new_image_data = 0;
       /* double l_offset_value = 0.0;
        char l_conversion_string[150];
        memset(l_conversion_string, 0, 150 *(sizeof(char)));
        wcstombs(l_conversion_string, m_offset_value, 150);
        l_offset_value = atof(l_conversion_string);
        m_offset_value.Format(_T("%lf"), l_offset_value);*/
        //long l_no_of_selected_images = m_no_of_images_selected;
        
        //check logic here
       // for(l_adding_offset_to_image = 0; l_adding_offset_to_image < l_no_of_selected_images ; l_adding_offset_to_image++) {
            *f_roi_data_to_be_added = f_roi_buffer[f_selected_roi][f_adding_offset_to_image]; //priyanka-add-offset
            *f_roi_data_to_be_added = *f_roi_data_to_be_added + ((*f_actual_expected_offset_value)*f_img_scale_factor);
        //}
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETCALC, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

//+Mpower/20130722/PN/added//OFFSET CHECK
/*************************************************************
* Method:    	CalculateValidOffset
* Description: 	This function is used 
* Parameter: 	int f_image_bit :
* Returns:   	int
*************************************************************/
int CAddOffset ::CalculateValidOffset(int f_offset_value_entered,
									  int f_add_offset_check_box_status,
									  int f_selected_roi,
									  int *f_actual_expected_offset_value)
{
	int l_size = 0;
    unsigned short l_image_width = 0;
    unsigned short l_image_height = 0;
    unsigned short l_image_bit = 0;
    int l_image_parsing = 0;
    int l_tag_reading_status = E_NO_ERROR;
    float l_img_scale_factor =0;
    void *l_img_data = NULL;
	signed short l_max_pixel_value_of_img_from_tag =0;
	int l_tag_reading_status_for_max_pixel_value = E_NO_ERROR;
	CLocalDBMgr l_localdb;
	int l_max_bit_value_of_image =0;
	double l_max_allowable_offset_value = 0.0;
//	float l_max_scale_factor =0.0f;
//	int l_actual_expected_offset_value =0;
    int l_temp_max =f_offset_value_entered;
    int l_err_code = E_NO_ERROR; //E_OFFSET_RANGE_ERROR ;
	CCommonCalculations *l_common_calc = new CCommonCalculations();
	if(NULL == l_common_calc) {
		CTiPrepLogHelper::WriteToErrorLog(_T("common calculation pointer NULL"), __LINE__, __FILE__);
		return E_MEM_ALLOC;
	}
	double **l_org_roi_data = new double *[MAX_NUM_ROI];
	long l_no_of_selected_images = 0;
	l_no_of_selected_images = m_no_of_images_selected;
	for(int l_roi_parsing = 0; l_roi_parsing < MAX_NUM_ROI; l_roi_parsing++) {
		l_org_roi_data[l_roi_parsing] = new double [l_no_of_selected_images];
	}
	int l_min_index_default_value = 0;
	int l_fun_execution_status = l_common_calc-> FillingROIBuffer(
		&l_min_index_default_value, m_image_loid_addoffset, 
            l_no_of_selected_images, l_org_roi_data);
    if(E_NO_ERROR != l_fun_execution_status){
        CTiPrepLogHelper::WriteToErrorLog(_T("FillingROIBuffer() returned error"), __LINE__, __FILE__);
		return E_ERROR;
    }
    for(l_image_parsing = 0; l_image_parsing < (signed) m_image_loid_addoffset.size(); l_image_parsing++) {
        l_img_data = NULL;
        l_tag_reading_status = l_localdb.TagReadingForImageData(
            m_image_loid_addoffset.at(l_image_parsing),
            &l_size,
            &l_image_width,
            &l_image_height,
            &l_image_bit,
            &l_img_data,
            &l_img_scale_factor);
        if(l_tag_reading_status != 0) {
            return l_tag_reading_status;
        }
        l_tag_reading_status_for_max_pixel_value = l_localdb.TagReadingForMaxPixelValueOfImage(m_image_loid_addoffset.at(l_image_parsing),
            &l_max_pixel_value_of_img_from_tag);
        if(l_tag_reading_status_for_max_pixel_value != 0) {
            return l_tag_reading_status_for_max_pixel_value;
        }
        if(DATA_BIT_16 == l_image_bit) {
		l_max_bit_value_of_image = (int)pow(2, 15)  - 1;

        } else if(DATA_BIT_32 == l_image_bit) {
		l_max_bit_value_of_image = (int)pow(2, 31) - 1;

	} else {
		CTiPrepLogHelper::WriteToErrorLog(_T("Invalid Data bit"), __LINE__, __FILE__);
		return E_ERROR; //have to put proper error code
		
	}

            l_max_allowable_offset_value =l_max_bit_value_of_image-l_max_pixel_value_of_img_from_tag;
        
        if(1 == f_add_offset_check_box_status) {
            l_max_allowable_offset_value =/*(int)*/(l_max_allowable_offset_value -l_org_roi_data[f_selected_roi][l_image_parsing]);
        }
        if(l_img_scale_factor > 0.000001f) {
            l_max_allowable_offset_value = /*(int)*/(l_max_allowable_offset_value/l_img_scale_factor);
        }
        if(l_max_allowable_offset_value  < l_temp_max) {
            l_temp_max = l_max_allowable_offset_value;
            l_err_code =E_OFFSET_RANGE_ERROR;
        }
	}
    if (l_err_code !=E_OFFSET_RANGE_ERROR) {
            *f_actual_expected_offset_value = f_offset_value_entered;
    } else {
        *f_actual_expected_offset_value = l_temp_max;
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
    if(NULL != l_common_calc) {
        delete l_common_calc;
        l_common_calc = NULL;
    }
	return l_err_code;

}

//-Mpower/20130722/PN/added//OFFSET CHECK
///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////


