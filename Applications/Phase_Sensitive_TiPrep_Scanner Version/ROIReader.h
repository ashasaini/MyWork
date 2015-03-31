// ROIInfo.h: interface for the CROIInfo class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_ROIINFO_H__68874164_26AE_45B6_A9C1_9048DD1F66AA__INCLUDED_)
#define AFX_ROIINFO_H__68874164_26AE_45B6_A9C1_9048DD1F66AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**
* Class is used to read/save ROI parameters from/in file respectively.
*/

#include "CircleROIParams.h"
#include "EllipticalROIParams.h"

#include <vector>
using namespace std;

#define ROI_INFO_TOTAL_LINE_COUNT 26    //Mpower 050/130508/AS/Added/Noise Correction feature development
#define ROI_INFO_TOTAL_LINE_COUNT_CIRCLE 16
class CROIReader
{
public:
    CROIReader();
    virtual ~CROIReader();
    BOOL ReadROIInfo(CString &f_csfileName, CString &f_csFindstr, vector<CCircleROIParams> &f_vRoiVector);
    BOOL WriteROIInfo(CString &f_csfileName,
                      CString &f_csProtocolName,
                      std::vector<CCircleROIParams> &f_vRoiVector);
    /************************************************************************/
    /* Overloaded functions for ellipse ROI.                                                                     */
    /************************************************************************/
    BOOL ReadROIInfo(CString &f_csfileName, CString &f_csFindstr,
                     vector<CEllipticalROIParams> &f_vRoiVector);
    BOOL WriteROIInfo(CString &f_csfileName,
                      CString &f_csProtocolName,
                      vector<CEllipticalROIParams> &f_vRoiVector);

};

#endif // !defined(AFX_ROIINFO_H__68874164_26AE_45B6_A9C1_9048DD1F66AA__INCLUDED_)
