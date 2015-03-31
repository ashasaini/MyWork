// PolarityCorrection.h: interface for the CPolarityCorrection class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_POLARITYCORRECTION_H__CC2F551F_A214_499B_8BD7_68AA51A23700__INCLUDED_)
#define AFX_POLARITYCORRECTION_H__CC2F551F_A214_499B_8BD7_68AA51A23700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonCalculations.h"
#include <vector>
using namespace std;
class CLocalDBMgr;
class CPolarityCorrection
{
public:
    CPolarityCorrection();
    CPolarityCorrection(CPolarityCorrection& f_polarity_correction_ptr);
    CPolarityCorrection& operator = (CPolarityCorrection& f_polarity_correction_ptr);
    virtual ~CPolarityCorrection();
    CString GetFirstLOID() {
        if(m_image_loid_magnitude.size() > 0) {
            return m_image_loid_magnitude.at(0);
        } else {
            return CString(_T(""));
        }
    }

    int PolarityCorrectionCalculation(byte* f_image_roi_data[], int f_roi_checked, CLocalDBMgr *f_localdb);

    int SetImageLOIDForPolarityCorrection(long f_no_of_images_selected,
                                          /*In*/vector <CString> & f_image_loid);
    int GetNumSelectedImgs() {
        return m_no_of_images_selected;
    }

private:
    vector <CString> m_image_loid_magnitude;
    long m_no_of_images_selected;
    long SetNoOfSelectedImages();
};

#endif // !defined(AFX_POLARITYCORRECTION_H__CC2F551F_A214_499B_8BD7_68AA51A23700__INCLUDED_)
