// AddOffset.h: interface for the CAddOffset class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_ADDOFFSET_H__0776AB01_D6E5_4979_8E89_4BEB08FF42DD__INCLUDED_)
#define AFX_ADDOFFSET_H__0776AB01_D6E5_4979_8E89_4BEB08FF42DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDefinitions.h"
#include <vector>
using namespace std;

class CAddOffset
{
public:
    CAddOffset();
    CAddOffset(CAddOffset& f_add_offset_ptr);
    CAddOffset& operator = (CAddOffset& f_add_offset_ptr);
    virtual ~CAddOffset();
public:
    int SetImageLOIDForAddOffset(long f_no_of_images_selected, /*In*/vector <CString> & f_image_loid);
    int GetSelectedImageLOIDForAddOffset(/*out*/vector <CString> &f_image_loid);
    int GetOffsetData(CString &f_offset_value,
        int f_add_offset_check_box_status,
        byte* f_image_roi_data,
        int f_roi_checked,
        int *f_actual_expected_offset_value);
    int  AddOffsetCalculation(double** f_roi_buffer, int f_selected_roi, double *f_roi_data_to_be_added,
                               int f_adding_offset_to_image,float f_img_scale_factor,
                               int *f_actual_expected_offset_value); //Mpower057/20130531/PN/added//priyanka-add-offset
	int CalculateValidOffset(int f_offset_value_entered,
							 int f_add_offset_check_box_status, 
                             int f_selected_roi,
							 int *f_actual_expected_offset_value); //Mpower/20130722/PN/added//OFFSET CHECK
    CString GetFirstLOID() {
        if(m_image_loid_addoffset.size() > 0) {
            return m_image_loid_addoffset.at(0);
        } else {
            return CString(_T(""));
        }
    }
    int GetNumSelectedImgs() {
        return m_no_of_images_selected;
    }
    
private:
    vector <CString> m_image_loid_addoffset;
    long m_no_of_images_selected;
    CString m_offset_value;
};

#endif // !defined(AFX_ADDOFFSET_H__0776AB01_D6E5_4979_8E89_4BEB08FF42DD__INCLUDED_)
