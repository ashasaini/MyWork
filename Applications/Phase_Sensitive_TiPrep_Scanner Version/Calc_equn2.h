// Calc_equn2.h: interface for the CCalc_equn2 class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_CALC_EQUN2_H__EEC9144D_E4B4_4F6A_B5C1_FCF533FD8736__INCLUDED_)
#define AFX_CALC_EQUN2_H__EEC9144D_E4B4_4F6A_B5C1_FCF533FD8736__INCLUDED_

/**
*  This class is a used to calculate Recovery time for A(1-2exp-(ti/t1)+exp-(tr/t1)) equation.
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcStrategy.h"
#include <math.h>


class CCalc_equn2 : public CCalcStrategy
{
public://construction,copy & destruction
    CCalc_equn2();
    virtual ~CCalc_equn2();
    
public:
    int CalcT1_Equn(double *f_ROI_Ti_values, double *f_ROI_Ti_Min_Rev,
        BOOL f_appltBtnPress, double *f_Roi_T1_val, double *f_Roi_T1_Min_Rev_val,
        int *f_time_data = NULL, int f_numImg = 0, double f_TrValue = 0);
	CCalc_equn2& operator= (CCalc_equn2& f_calcequn); //assignment operator
    
private:
    int CalcRecoveryTimeEqn2(double f_inversionTime, double & f_dT1);
    int getRootBracket(double f_Ti, double *f_T1positive, double *f_T1negative, int f_nNumImg);
    int iterate(double f_Ti, double f_T1positive, double f_T1negative, double *T1);
    
private:
    //This stores Tr value entered by user.
    double m_dTRvalue;
    //This stores timestamp for given ROI data.
    int *m_nTime_data;
    //This stores number of images selected by user on iselector.
    int m_nNumImg;
};

#endif // !defined(AFX_CALC_EQUN2_H__EEC9144D_E4B4_4F6A_B5C1_FCF533FD8736__INCLUDED_)
