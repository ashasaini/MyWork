// CommonCalculations.h: interface for the CCommonCalculations class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_COMMONCALCULATIONS_H__49ED7477_1F0E_4866_B781_22B70716867A__INCLUDED_)
#define AFX_COMMONCALCULATIONS_H__49ED7477_1F0E_4866_B781_22B70716867A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class CCommonCalculations
{
public://construction,copy & destruction
    CCommonCalculations();
    virtual ~CCommonCalculations();
public:
    int FillingROIBuffer(int *f_min_value_index_postion,
        /*Input*/vector <CString> &f_image_loid,
        long f_nNumImages,
        double **l_org_roi_data); 
    
private:
    int CalculateMinSignalDataAndIndex(long f_no_of_selected_images,
        double **f_org_roi_data,
        int *f_min_data_value_index,
        double *f_min_sig_data,
        int * f_min_value_index);
    
};

#endif // !defined(AFX_COMMONCALCULATIONS_H__49ED7477_1F0E_4866_B781_22B70716867A__INCLUDED_)
