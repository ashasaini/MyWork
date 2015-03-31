// CalcStrategy.h: interface for the CCalcStrategy class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_CALCSTRATEGY_H__9CAC680D_A9AA_49CE_9342_6354621BD4EA__INCLUDED_)
#define AFX_CALCSTRATEGY_H__9CAC680D_A9AA_49CE_9342_6354621BD4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**
* This class is abstract class which is used to calculate T1 value using different
* equations.
*/
class CCalcStrategy
{
public://construction,copy & destruction
    CCalcStrategy();
    virtual ~CCalcStrategy();

public:
    virtual int CalcT1_Equn(double *f_dROI_Ti_values,
                            double *f_dROI_Ti_Min_Rev,
                            BOOL f_appltBtnPress,
                            double *f_dRoi_T1_val,
                            double *f_dRoi_T1_Min_Rev_val,
                            int *f_nTime_data = NULL,
                            int f_nNumImg = 0,
                            double f_dTrValue = 0) = 0;


};

#endif // !defined(AFX_CALCSTRATEGY_H__9CAC680D_A9AA_49CE_9342_6354621BD4EA__INCLUDED_)
