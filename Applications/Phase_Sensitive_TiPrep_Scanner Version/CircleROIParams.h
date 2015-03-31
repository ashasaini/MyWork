// RoiParams.h: interface for the CRoiParams class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_ROIPARAMS_H__5B9639F5_1A8E_449B_93C1_FE26179D790D__INCLUDED_)
#define AFX_ROIPARAMS_H__5B9639F5_1A8E_449B_93C1_FE26179D790D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCircleROIParams
{
public://construction,copy & destruction
    CCircleROIParams();
    virtual ~CCircleROIParams();
    
public:
    float GetRadius() const ;
    void SetRadius(float val);
    float GetCenter_Y() const ;
    void SetCenter_Y(float val);
    float GetCenter_X() const;
    void SetCenter_X(float val);
    
private:
    float m_fCenter_Y;
    float m_fCenter_X;
    float m_fRadius;
};

#endif // !defined(AFX_ROIPARAMS_H__5B9639F5_1A8E_449B_93C1_FE26179D790D__INCLUDED_)
