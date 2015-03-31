// Calc_equn2.cpp: implementation of the CCalc_equn2 class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "TiPrep.h"
#include "Calc_equn2.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
const CString EXCEPTION_EQN2 = L"Exception occurred in Equation 2 Calculation";

/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CCalc_equn2::CCalc_equn2()
{
    /*TIPREP_TRACE(CCalc_equn2::CCalc_equn2);*/
    m_dTRvalue = 0.0;
    m_nNumImg = 0;
    m_nTime_data = NULL;
}
CCalc_equn2& CCalc_equn2::operator = (CCalc_equn2& f_calcequn)
{
	if(this == &f_calcequn) {
        return(*this);
    }
	m_dTRvalue = f_calcequn.m_dTRvalue;
	m_nTime_data = f_calcequn.m_nTime_data;
	m_nNumImg = f_calcequn.m_nNumImg;
    return (*this);
}
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CCalc_equn2::~CCalc_equn2()
{
    /*TIPREP_TRACE(CCalc_equn2::~CCalc_equn2);*/
    
    if(m_nTime_data != NULL) {
        delete []m_nTime_data; //Mpower42
        m_nTime_data = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       CalcT1_Equn
* Description:  This function is used to get recovery time for
*               equation A(1-2exp-(ti/t1)+exp-(tr/t1)).
* Parameter:    double * f_dROI_Ti_values
*               Contains Ti values for each ROI data.
* Parameter:    double * f_dROI_Ti_Min_Rev
*               Contains Ti values for each ROI data with min_reversed.
* Parameter:    BOOL f_appltBtnPress
*               status of ApplyButton.
* Parameter:    double * f_dRoi_T1_val
*               Out parameter contains T1 values for each ROI data.
* Parameter:    double * f_dRoi_T1_Min_Rev_val
*               Out parameter contains T1 values for each ROI data with min_reversed.
* Parameter:    int * f_nTime_data
*               Contains time stamp data.
* Parameter:    int f_nNumImg
*               Number of images selected by user.
* Parameter:    double f_dTrValue
*               Contains Tr value entered by user.
* Returns:      void
*************************************************************/
int CCalc_equn2::CalcT1_Equn(double *f_dROI_Ti_values,
                             double *f_dROI_Ti_Min_Rev,
                             BOOL f_appltBtnPress,
                             double *f_dRoi_T1_val,
                             double *f_dRoi_T1_Min_Rev_val,
                             int *f_nTime_data,
                             int f_nNumImg,
                             double f_dTrValue)
{
    TIPREP_TRACE(CCalc_equn2::CalcT1_Equn,MRTraceMarker3);
    
    try {
        if(NULL == f_dROI_Ti_values || NULL == f_dROI_Ti_Min_Rev || NULL == f_dRoi_T1_Min_Rev_val || NULL == f_dRoi_T1_val || NULL == f_nTime_data) {
            CString l_cserrnsg(_T("CalcT1_Equn Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrnsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_dTRvalue = f_dTrValue;
        m_nNumImg = f_nNumImg;
        m_nTime_data = new int[f_nNumImg];
        
        for(int l_ni = 0; l_ni < f_nNumImg; l_ni++) {
            m_nTime_data[l_ni] = f_nTime_data[l_ni];
        }
        
        if(f_appltBtnPress) {
            for(int l_nRoiNumber = 0 ; l_nRoiNumber < MAX_NUM_ROI; l_nRoiNumber++) {
                CalcRecoveryTimeEqn2(f_dROI_Ti_values[l_nRoiNumber], f_dRoi_T1_val[l_nRoiNumber]);
                CalcRecoveryTimeEqn2(f_dROI_Ti_Min_Rev[l_nRoiNumber], f_dRoi_T1_Min_Rev_val[l_nRoiNumber]);
				//TRACE_COMMENTED
#ifdef _UT_LOG_
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("ROI Org T1 value = %lf\tROI MinRev T1 value = %lf\n"), f_dRoi_T1_val[l_nRoiNumber],
                    f_dRoi_T1_Min_Rev_val[l_nRoiNumber]);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_csTempstr, _T("CCalc_equn2::CalcT1_Equn"));
#endif
            }
        }
        
        if(NULL != m_nTime_data) {
            delete []m_nTime_data;
            m_nTime_data = NULL;
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EQN2, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       CalcRecoveryTime
* Description:  This function is used calculate recovery time
*               for A(1-2exp-(ti/t1)) equation.
* Parameter:    double f_dInversionTime
*               contains ti value for ROI.
* Returns:      double
*               Recovery time.
*************************************************************/
int CCalc_equn2::CalcRecoveryTimeEqn2(double f_dInversionTime, double & f_dT1)
{
    TIPREP_TRACE(CCalc_equn2::CalcRecoveryTimeEqn2,MRTraceMarker3);
    
    try {
        int l_nStatus = 0;
        double l_dT1positive = 0.0f;
        double l_dT1negative = 0.0f;
        f_dT1 = -1.0f;
        l_nStatus = getRootBracket(f_dInversionTime,
            &l_dT1positive,
            &l_dT1negative,
            m_nNumImg);
        
        if(E_NO_ERROR != l_nStatus) {
            return E_ERROR;
        }
        
        iterate(f_dInversionTime, l_dT1positive, l_dT1negative, &f_dT1);
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EQN2, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       getRootBracket
* Description:  This function is used to get initial left and
*               right must be chosen so that
* Parameter:    double f_dTi
*               value to calculate T1 value.
* Parameter:    double * f_dT1positive
*               out parameter which holds initial right value of bracket.
* Parameter:    double * f_dT1negative
*               out parameter which holds initial left value of bracket.
* Parameter:    int f_nNumImg
*               Number of images selected by user.
* Returns:      int
*               '0' on success.
*************************************************************/
int CCalc_equn2::getRootBracket(double f_dTi,
                                double *f_dT1positive,
                                double *f_dT1negative,
                                int f_nNumImg)
{
    TIPREP_TRACE(CCalc_equn2::getRootBracket,MRTraceMarker3);
    int l_ni = 0;
    int l_nbrkflg = 0;
    double *l_dsig = NULL;
    
    try {
        if(f_nNumImg <= 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("getRootBracket(): Incorrect Number of images "), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        l_dsig = new double [f_nNumImg];
        
        if(NULL == l_dsig) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Memory allocation failed"), __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        memset(l_dsig, 0, (sizeof(double)*f_nNumImg));
        for(l_ni = 0; l_ni < f_nNumImg; l_ni++) {
            l_dsig[l_ni] = 1.0 - 2.0 * exp(-f_dTi / (double)(m_nTime_data[l_ni])) +
                exp(-m_dTRvalue / (double)(m_nTime_data[l_ni]));
            
            if(l_ni != 0) {
                if(l_dsig[l_ni]*l_dsig[l_ni-1] < 0.0f) {
                    l_nbrkflg = 1;
                    break;
                }
            }
        }
        
        if(1 == l_nbrkflg) {
            if(l_dsig[l_ni] < 0.0f) {
                *f_dT1negative = (double)m_nTime_data[l_ni] ;
                *f_dT1positive = (double)m_nTime_data[l_ni-1] ;
            } else {
                *f_dT1positive = (double)m_nTime_data[l_ni] ;
                *f_dT1negative = (double)m_nTime_data[l_ni-1] ;
            }
        } else {
            CTiPrepLogHelper::WriteToErrorLog(_T("getRootBracket() failed to calculate T1"), __LINE__, __FILE__);
            
            if(NULL != l_dsig) {
                delete [] l_dsig;
                l_dsig = NULL;
            }
            
            return(E_ERROR);
        }
        
        if(NULL != l_dsig) {
            delete [] l_dsig;
            l_dsig = NULL;
        }
        
        return(E_NO_ERROR);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EQN2, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       iterate
* Description:  This function is used to calculate T1 value for
*               equation A(1-2exp-(ti/t1))
* Parameter:    double f_dTi
*               Inversion time(Ti) value to calculate relaxation (T1) value.
* Parameter:    double f_dT1positive
*               Initial right value used for bisection method.
* Parameter:    double f_dT1negative
*               Initial left value used for bisection method.
* Parameter:    double * f_dT1
*               Relaxation time(T1) value of ROI.
* Returns:      void
*************************************************************/
int CCalc_equn2::iterate(double f_dTi, double f_dT1positive,
                         double f_dT1negative, double *f_dT1)
{
    TIPREP_TRACE(CCalc_equn2::iterate,MRTraceMarker3);
    int l_ntest_sig = 0;
    int l_nitr = 0;
    double l_dsig_m = 0.0f;
    double l_dt1_m = 0.0f;
    double l_dt1_n = 0.0f;
    double l_dt1_p = 0.0f;
    l_dt1_n = f_dT1negative;
    l_dt1_p = f_dT1positive;
    
    try {
        do {
            l_dt1_m = (l_dt1_p + l_dt1_n) / 2.0;
            l_nitr ++;
            l_dsig_m = 1.0 - 2.0 * exp(-f_dTi / l_dt1_m) + exp(-m_dTRvalue / l_dt1_m);
            
            if(l_dsig_m > 0.0f) {
                l_dt1_p = l_dt1_m;
            } else {
                l_dt1_n = l_dt1_m;
            }
            
            l_ntest_sig = int(l_dsig_m * 1000000);
            *f_dT1 = l_dt1_m;
        } while(l_nitr < 100 && l_ntest_sig != 0);
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EQN2, __LINE__, __FILE__);
        return E_ERROR;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////
