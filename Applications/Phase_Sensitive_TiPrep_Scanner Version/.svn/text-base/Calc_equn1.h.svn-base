// Calc_equn1.h: interface for the CCalc_equn1 class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
/**
*  This class is a used to calculate Recovery time for A(1-2exp-(ti/t1)) equation.
*/
#if !defined(AFX_CALC_EQUN1_H__8D3E77B0_35F1_4193_86BA_42FCC597B183__INCLUDED_)
#define AFX_CALC_EQUN1_H__8D3E77B0_35F1_4193_86BA_42FCC597B183__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcStrategy.h"
#include <math.h>

class CCalc_equn1 : public CCalcStrategy
{
public://construction,copy & destruction
    CCalc_equn1();
    virtual ~CCalc_equn1();
    
public:
    int CalcT1_Equn(double *f_ROI_Ti_values, double *f_ROI_Ti_Min_Rev,
        BOOL f_appltBtnPress, double *f_Roi_T1_val, double *f_Roi_T1_Min_Rev_val,
        int *f_time_data = NULL, int f_numImg = 0, double f_TrValue = 0);
    int CalcRecoveryTimeEqn1(double f_inversionTime, double & f_dRecoveryTime);
    
};

#endif // !defined(AFX_CALC_EQUN1_H__8D3E77B0_35F1_4193_86BA_42FCC597B183__INCLUDED_)
