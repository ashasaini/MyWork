/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "TiPrep.h"
#include "Calc_equn1.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CCalc_equn1::CCalc_equn1()
{
    /*TIPREP_TRACE(CCalc_equn1::CCalc_equn1);*/
}
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CCalc_equn1::~CCalc_equn1()
{
    /*TIPREP_TRACE(CCalc_equn1::~CCalc_equn1);*/
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       CalcT1_Equn
* Description:  This function is used to get recovery time for
*               equation A(1-2exp-(ti/t1)).
* Parameter:    double * f_dROI_Ti_values
*               Contains Ti values for each ROI data.
* Parameter:    double * f_dROI_Ti_Min_Rev
*               Contains Ti values for each ROI data with min_reversed.
* Parameter:    BOOL f_appltBtnPress
*               status of ApplyButton.
* Parameter:    double * f_dRoi_T1_val
*               Out parameter contains T1 values for each ROI data.
* Parameter:    double * f_dRoi_T1_Min_Rev_val
*               Out parameter contains T1 values for each ROI data
*               with min_reversed.
* Parameter:    int * f_nTime_data
*               Contains time stamp data.
* Parameter:    int f_nNumImg
*               Number of images selected by user.
* Parameter:    double f_dTrValue
*               Contains Tr value entered by user.
* Returns:      void
*************************************************************/
int CCalc_equn1::CalcT1_Equn(double *f_dROI_Ti_values,
                             double *f_dROI_Ti_Min_Rev,
                             BOOL f_appltBtnPress,
                             double *f_dRoi_T1_val,
                             double *f_dRoi_T1_Min_Rev_val,
                             int *f_nTime_data,
                             int f_nNumImg,
                             double f_dTrValue)
{
    TIPREP_TRACE(CCalc_equn1::CalcT1_Equn,MRTraceMarker3);
    
    try {
		//TRACE_COMMENTED
#ifdef _UT_LOG_
        CString l_csTempstr(_T(""));
#endif
        
        if(NULL == f_dROI_Ti_values || NULL == f_dROI_Ti_Min_Rev || 
            NULL == f_dRoi_T1_Min_Rev_val || NULL == f_dRoi_T1_val) {
            CString l_cserrnsg(_T("CalcT1_Equn Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrnsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if(NULL == f_nTime_data && 0 == f_nNumImg && 0.000001 > f_dTrValue) {
            
            if(f_appltBtnPress) {
                for(int l_nRoi = 0 ; l_nRoi < MAX_NUM_ROI; l_nRoi++) {
                    CalcRecoveryTimeEqn1(f_dROI_Ti_values[l_nRoi], f_dRoi_T1_val[l_nRoi]);
                    CalcRecoveryTimeEqn1(f_dROI_Ti_Min_Rev[l_nRoi], f_dRoi_T1_Min_Rev_val[l_nRoi]);
					//TRACE_COMMENTED
#ifdef _UT_LOG_
                    l_csTempstr.Format(_T("ROI Org T1 value = %lf\tROI MinRev T1 value = %lf\n"),
                        f_dRoi_T1_val[l_nRoi], f_dRoi_T1_Min_Rev_val[l_nRoi]);
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker6, l_csTempstr, _T("CCalc_equn1::CalcT1_Equn"));
#endif
                }
            }
        } else {
            if(f_appltBtnPress) {
                for(int l_nRoi = 0 ; l_nRoi < 3; l_nRoi++) {
                    CalcRecoveryTimeEqn1(f_dROI_Ti_values[l_nRoi], f_dRoi_T1_val[l_nRoi]);
                    CalcRecoveryTimeEqn1(f_dROI_Ti_Min_Rev[l_nRoi], f_dRoi_T1_Min_Rev_val[l_nRoi]);
					//TRACE_COMMENTED
#ifdef _UT_LOG_
                    l_csTempstr.Format(_T("ROI Org T1 value = %lf\tROI MinRev T1 value = %lf\n"),
                        f_dRoi_T1_val[l_nRoi], f_dRoi_T1_Min_Rev_val[l_nRoi]);
                    CTiPrepLogHelper::WriteToTraceLog
                        (_T("MRI_TIPREP"), MRTraceMarker6, l_csTempstr, _T("CCalc_equn1::CalcT1_Equn"));
#endif
                }
            }
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in equation 1 calculation"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       CalcRecoveryTime
* Description:  This function is used to calculate recovery time
*               for A(1-2exp-(ti/t1)) equation.
* Parameter:    double f_dInversionTime
*               which contains ti(recovery time) value for ROI.
* Returns:      double
*************************************************************/
int CCalc_equn1::CalcRecoveryTimeEqn1(double f_dInversionTime, double & f_dRecoveryTime)
{
    TIPREP_TRACE(CCalc_equn1::CalcRecoveryTimeEqn1,MRTraceMarker3);
    
    try {
        f_dRecoveryTime =  -f_dInversionTime / log(0.5f);
        return E_NO_ERROR;
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in equation 1 calculation"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////