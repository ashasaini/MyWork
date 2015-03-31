/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrep.h"
#include "LineFitStrategy.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include <float.h>
#include <limits>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "InitialGuess.h"
//#include "MatrixSolver.h"
using namespace std;

const CString EXCEPTION_LINEFIT = L"Exception occurred in Line Fitting ";  
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CLineFitStrategy
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CLineFitStrategy::CLineFitStrategy(): m_dAFactor(0.0),
    m_dBFactor(0.0),
    m_dMFactor(0.0),
    m_dNFactor(0.0),
    m_dPrev_AFactor(0.0),
    m_dPrev_BFactor(0.0),
    m_dPrev_MFactor(0.0),
    m_dPrev_NFactor(0.0),
    m_dPrev_AFactor_Minrev(0.0),
    m_dPrev_BFactor_Minrev(0.0),
    m_dPrev_MFactor_Minrev(0.0),
m_dPrev_NFactor_Minrev(0.0),

m_timestamp_calc(0),
m_dTempOutDataY(0),
m_dMatrixA(0),
m_dInvMatrixA(0)
{
}
//****************************Method Header***************************
//Method Name   : CLineFitStrategy
//Purpose       : Destructor
//********************************************************************
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CLineFitStrategy::~CLineFitStrategy()
{
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       fit_exp_curve
* Description:  This function is used for curve fitting.
* Parameter:    double * f_dorg_data
*               Contain data on which line to be fitted.
* Parameter:    int f_ndatalen
*               Contains number of data.
* Parameter:    double * f_dfact_a
*               Contains factor 'a' for line fit.
* Parameter:    double * f_dfact_b
*               Contains factor 'b' for line fit.
* Parameter:    double * f_derr
*               Contains error factor value for line fittng.
* Parameter:    int * f_ntime_data
*               Contains timestamp data.
* Returns:      int
*               '0' on success.
*************************************************************/
int CLineFitStrategy::fit_exp_curve(double *f_dorg_data,
                                    int f_ndatalen,
                                    double *f_dfact_a,
                                    double *f_dfact_b,
                                    double *f_derr,
                                    int *f_ntime_data)
{
    //TIPREP_TRACE(CLineFitStrategy::fit_exp_curve,MRTraceMarker9);

    try{
    double l_dX2iYi = 0.0;
    double l_dYilnYi = 0.0;
    double l_dXiYi = 0.0;
    double l_dXiYilnYi = 0.0;
    double l_dYi = 0.0;
    double l_da = 0.0;
    double l_db = 0.0;
    int l_ni = 0;
    //int    l_nstatus = 0;
    double l_dtemp = 0.0;
	CString l_error_display_string(_T(""));

        if((NULL == f_dorg_data) || (NULL == f_ntime_data) || 
            (NULL == f_dfact_a) || 
            (NULL == f_dfact_b)||(NULL == f_derr)) {
            l_error_display_string = (_T("fit_exp_curve Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
    for(l_ni = 0; l_ni < f_ndatalen; l_ni++) {
        l_dX2iYi += f_dorg_data[l_ni] * f_ntime_data[l_ni] * f_ntime_data[l_ni] * CONVERT_FACT * CONVERT_FACT;
        l_dYilnYi += f_dorg_data[l_ni] * log(f_dorg_data[l_ni]);
        l_dXiYi += f_ntime_data[l_ni] * f_dorg_data[l_ni] * CONVERT_FACT;
        l_dXiYilnYi += f_ntime_data[l_ni] * f_dorg_data[l_ni] * log(f_dorg_data[l_ni]) * CONVERT_FACT;
        l_dYi += f_dorg_data[l_ni];
    }

    l_dtemp = (l_dYi * l_dX2iYi - l_dXiYi * l_dXiYi); 

    if(0.000001 < fabs(l_dtemp)) { 
        l_da = (l_dX2iYi * l_dYilnYi - l_dXiYi * l_dXiYilnYi) / (l_dYi * l_dX2iYi - l_dXiYi * l_dXiYi);
        l_db = (l_dYi * l_dXiYilnYi - l_dXiYi * l_dYilnYi) / (l_dYi * l_dX2iYi - l_dXiYi * l_dXiYi);
    }

    *f_dfact_a = l_da;
    *f_dfact_b = l_db;
    *f_derr = 0;
#ifdef _UT_LOG_
    CString l_csval(_T(""));
    l_csval.Format(_T("Curve Fit coefficients a = %f, b = %f"), l_da, l_db);
   // CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_csval, _T("CLineFitStrategy::fit_exp_curve"));
#endif

    for(l_ni = 0; l_ni < f_ndatalen; l_ni++) {
        *f_derr += (f_dorg_data[l_ni] - (exp(l_da) * exp(l_db * f_ntime_data[l_ni] * CONVERT_FACT))) *
                   (f_dorg_data[l_ni] - (exp(l_da) * exp(l_db * f_ntime_data[l_ni] * CONVERT_FACT)));
        f_dorg_data[l_ni] = exp(l_da) * exp(l_db * f_ntime_data[l_ni] * CONVERT_FACT);
    }

    return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_LINEFIT, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       invert_data
* Description:  This function is used invert given data on which
*               line to be fitted.
* Parameter:    double * f_dOrg_Data
*               Contains original data which is inverted for line
*               fitting.
* Parameter:    int f_nselectImg
*               Contains count of number of data present.
* Returns:      int
*               status on success.
*************************************************************/
int CLineFitStrategy::invert_data(double *f_dOrg_Data, int f_nselectImg)
{
    //TIPREP_TRACE(CLineFitStrategy::invert_data,MRTraceMarker9);
    int    l_nstatus = 0;

    for(int l_ni = 0; l_ni < f_nselectImg ; l_ni++) {
        f_dOrg_Data[l_ni] = -f_dOrg_Data[l_ni];
    }

    return(l_nstatus);
}

/*************************************************************
* Method:       translate_data_to_all_positive
* Description:  This function translate data to positive.
* Parameter:    double * f_dorg_data
*               Contains data.
* Parameter:    int f_ndatalen
*               Contains number of data element.
* Parameter:    int f_nroiNo
*               Contains ROI number.
* Parameter:    double * f_dmaxSigData
*               Contains maximum single data present in data.
* Returns:      int
*               0 on success.
*************************************************************/
int CLineFitStrategy::translate_data_to_all_positive(double *f_dorg_data,
        int f_ndatalen,
        int f_nroiNo,
        double *f_dmaxSigData)
{
    //TIPREP_TRACE(CLineFitStrategy::translate_data_to_all_positive,MRTraceMarker9);

    if(NULL == f_dorg_data || NULL == f_dmaxSigData) {
        CString l_cserr = _T("translate_data_to_all_positive():: Input Parameters NULL");
        CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        return E_ERROR;
    }

    for(int l_ni = 0; l_ni < f_ndatalen ; l_ni++) {
        // to make 1 as lowest value as log(0) is undefined
        f_dorg_data[l_ni] = f_dorg_data[l_ni] + f_dmaxSigData[f_nroiNo] + 1;
    }

    return(E_NO_ERROR);
}

/*************************************************************
* Method:       translate_back_to_org
* Description:  This function is used convert data back to original
*               format after line fitting.
* Parameter:    double * f_dorg_data
*               Contains data.
* Parameter:    int f_ndatalen
*               Contains number of data element.
* Parameter:    int f_nroiNo
*               Contains ROI number.
* Parameter:    double * f_dmaxSigData
*               Contains maximum single data present in data.
* Returns:      int
*               0 on success.
*************************************************************/
int CLineFitStrategy::translate_back_to_org(double *f_dorg_data,
        int f_ndatalen,
        int f_nroiNo,
        double *f_dmaxSigData)
{
   // TIPREP_TRACE(CLineFitStrategy::translate_back_to_org,MRTraceMarker9);

    if((NULL == f_dorg_data) || (NULL == f_dmaxSigData)) {
        CString l_cserr = _T("translate_back_to_org():: Input Parameters NULL");
        CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
        return E_ERROR;
    }

    int    l_status = 0;

    for(int l_ni = 0; l_ni < f_ndatalen ; l_ni++) {
        // to make 1 as lowest value as log(0) is undefined
        f_dorg_data[l_ni] = f_dorg_data[l_ni] - f_dmaxSigData[f_nroiNo] - 1;
    }

    return(l_status);
}

/*************************************************************
* Method:       CalculateTi_Linear
* Description:  This function is to calculate inversion time(Ti).
* Parameter:    double * f_dval_Ti
*               Out parameter which holds inversion time(Ti) values.
* Parameter:    int f_nroiNo
*               Contains number of ROI for which Ti value is calculated.
* Parameter:    double * f_dinputData
*               Contains data for which Ti value is calculated.
* Parameter:    double * f_doutputData
*               Out parameter which holds line fitting data.
* Parameter:    double * f_dfitErr
*               Out parameter which holds fitting error value.
* Parameter:    double * f_dfactB
*               Contains factor for line fitting equation.
* Parameter:    int f_nselectImg
*               Contains count of data.
* Parameter:    int * f_ntime_data
*               Contains timestamp data.
* Parameter:    double * f_dmaxSigData
*               Contains maximum single data present in input data.
* Returns:      int
*               0 on success.
*************************************************************/
int CLineFitStrategy::CalculateTi_Linear(double *f_dval_Ti, int f_nroiNo,
        double *f_dinputData, double *f_doutputData,
        double *f_dfitErr, double *f_dfactB,
        int f_nselectImg, int *f_ntime_data,
        double *f_dmaxSigData)    
{
    //TIPREP_TRACE(CLineFitStrategy::CalculateTi_Linear,MRTraceMarker9);
    
    try{
        if(NULL == f_dval_Ti || NULL == f_dinputData || NULL == f_doutputData || NULL == f_dfitErr || NULL == f_dfactB || NULL == f_dfactB || NULL == f_ntime_data || NULL == f_dmaxSigData) {
            CString l_cserr = _T("CalculateTi_Linear():: Input Parameters NULL");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        double *l_dorg_data = NULL;   //dynamic
        l_dorg_data = new double [f_nselectImg];
        
        if(NULL == l_dorg_data) {
            CString l_cserr = _T("Memory allocation failed");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            return E_MEM_ALLOC;
        }
        
        double l_dfact_a = 0.0, l_dfact_b = 0.0;
        int   l_nstatus = E_NO_ERROR;
        
        for(int l_ni = 0; l_ni < f_nselectImg; l_ni++) {
            l_dorg_data[l_ni] = f_dinputData[l_ni];
        }
        
        f_dval_Ti[f_nroiNo] = 0; 
        l_nstatus = invert_data(l_dorg_data, f_nselectImg);
        
        if(E_NO_ERROR != l_nstatus) { 
            CString l_cserr = _T("CalculateTi_Linear(): invert_data() returned error");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            
            if(NULL != l_dorg_data) {   
                delete [] l_dorg_data;
                l_dorg_data = NULL;
            }
            
            return E_ERROR;
        }
        
        l_nstatus = translate_data_to_all_positive(l_dorg_data, f_nselectImg,
            f_nroiNo, f_dmaxSigData);
        
        if(E_NO_ERROR != l_nstatus) { 
            CString l_cserr = _T("CalculateTi_Linear(): translate_data_to_all_positive() returned error");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            
            if(NULL != l_dorg_data) {   
                delete [] l_dorg_data;
                l_dorg_data = NULL;
            }
            
            return E_ERROR;
        }
        
        l_nstatus = fit_exp_curve(l_dorg_data, f_nselectImg, &l_dfact_a,
            &l_dfact_b, f_dfitErr, f_ntime_data);
        
        if(E_NO_ERROR != l_nstatus) { 
            CString l_cserr = _T("CalculateTi_Linear(): fit_exp_curve() returned error");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            
            if(NULL != l_dorg_data) {   
                delete [] l_dorg_data;
                l_dorg_data = NULL;
            }
            
            return E_ERROR;
        }
        
        f_dval_Ti[f_nroiNo] = (log(f_dmaxSigData[f_nroiNo] + 1) - l_dfact_a) / (l_dfact_b * CONVERT_FACT);
        *f_dfactB = l_dfact_b;
        l_nstatus = translate_back_to_org(l_dorg_data, f_nselectImg,
            f_nroiNo, f_dmaxSigData);
        
        if(E_NO_ERROR != l_nstatus) { 
            CString l_cserr = _T("translate_back_to_org(): invert_data() returned error");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            
            if(NULL != l_dorg_data) {   
                delete [] l_dorg_data;
                l_dorg_data = NULL;
            }
            
            return E_ERROR;
        }
        
        l_nstatus = invert_data(l_dorg_data, f_nselectImg);
        
        if(E_NO_ERROR != l_nstatus) { 
            CString l_cserr = _T("CalculateTi_Linear(): invert_data() returned error");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            
            if(NULL != l_dorg_data) {   
                delete [] l_dorg_data;
                l_dorg_data = NULL;
            }
            
            return E_ERROR;
        }
        
        for(l_ni = 0; l_ni < f_nselectImg ; l_ni++) {
            f_doutputData[l_ni] = l_dorg_data[l_ni];
        }
        
        l_dfact_a = 0.0;
        l_dfact_b = 0.0;
        
        if(NULL != l_dorg_data) {   
            delete [] l_dorg_data;
            l_dorg_data = NULL;
        }
        
        return l_nstatus;
 } catch(...) {
     CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_LINEFIT, __LINE__, __FILE__);
     return E_ERROR;
 }
}

/*************************************************************
* Method:       CalculateTi_NonLinear
* Description:  This function is used
* Parameter:    double * f_dval_Ti
* Parameter:    double * f_dval_T1
* Parameter:    int f_nroiNo
* Parameter:    double * f_dRoi_inputData
* Parameter:    double * f_dRoi_outputData
* Parameter:    double * f_dfitErr
* Parameter:    int f_nselectImg
* Parameter:    int * f_ntime_data
* Parameter:    double f_dRepetationTime
* Returns:      int
*************************************************************/
int CLineFitStrategy::CalculateTi_NonLinear(double *f_dval_Ti,
        double*f_dval_T1,
        int f_nroiNo,
        double *f_dRoi_inputData,
        double *f_dRoi_outputData,
        double *f_dfitErr,
        int f_nselectImg,
        int *f_ntime_data,
        double f_dRepetationTime,
        int f_nSelectedEqn,
        CExtraPointsGen*f_CExtraPoint,
        int f_nMinrevFlag, int f_nManualInitialGuessChk)
{
//   // TIPREP_TRACE(CLineFitStrategy::CalculateTi_NonLinear,MRTraceMarker9);
    
    try{
        if(NULL == f_dRoi_inputData || NULL == f_dRoi_outputData || NULL == f_dfitErr) {
            CString l_cserr = _T("CalculateTi_NonLinear():: Input Parameters NULL");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        memset(f_dRoi_outputData , 0x0, f_nselectImg * sizeof(double));
        memcpy(f_dRoi_outputData , f_dRoi_inputData, f_nselectImg * sizeof(double));
        
        //equation 2 and 3
        if(1 == f_nSelectedEqn || 2 == f_nSelectedEqn) {
            f_dRepetationTime = (double)(f_dRepetationTime / 1000);
        }
        
        //Call Non-linear curve fitting.
        double l_dfact_a = 1.0;
        double l_dfact_b = 1.0;
        double l_dfact_m = 1.0;
        double l_dfact_k = 1.0;
        int l_nStatus = E_NO_ERROR;
        l_nStatus = nonLinearCurveFit(m_timestamp_calc, f_dRoi_outputData,
            f_dfitErr, f_dRepetationTime, l_dfact_a,
            l_dfact_b, l_dfact_m, l_dfact_k, 250,
            f_nselectImg, f_nSelectedEqn, f_nManualInitialGuessChk);      

            if(E_NO_ERROR != l_nStatus ||0.0000001 >fabs(l_dfact_a) ||0.0000001 > fabs(l_dfact_b)){                
                f_dval_Ti[f_nroiNo] = 0;
                f_dval_T1[f_nroiNo] = -1;
            	return E_ERROR;
            }
        
        l_nStatus = tiT1Calculation(f_dval_Ti, f_dval_T1,
            f_dRepetationTime, l_dfact_a, l_dfact_b,
            l_dfact_m, l_dfact_k, f_nSelectedEqn, f_nroiNo);
        
        if(E_NO_ERROR != l_nStatus) {
            return l_nStatus;
        }
        
        if(NULL != f_CExtraPoint) {
            f_CExtraPoint->GenerateBuffgenY(l_dfact_a, l_dfact_b,
                l_dfact_m, l_dfact_k, f_dRepetationTime,
                f_nroiNo, f_nSelectedEqn, f_nMinrevFlag);
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_LINEFIT, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       nonLinearCurveFit
* Description:  This function is used
* Parameter:    double * f_dTimeStamp
* Parameter:    double * f_dRoiOutData
* Parameter:    double * f_dFitError
* Parameter:    double & f_dRepetationTime
* Parameter:    double & f_dFactorA
* Parameter:    double & f_dFactorB
* Parameter:    double & f_dFactorM
* Parameter:    double & f_dFactorK
* Parameter:    int f_nIteration
* Parameter:    int f_nSelImg
* Parameter:    int f_nEqnFlag
* Returns:      int
*************************************************************/
int CLineFitStrategy::nonLinearCurveFit(double *f_dTimeStamp,
                                        double *f_dRoiOutData,
                                        double *f_dFitError,
                                        double &f_dRepetationTime,
                                        double &f_dFactorA,
                                        double &f_dFactorB,
                                        double &f_dFactorM,
                                        double &f_dFactorK,
                                        int f_nIteration,
                                        int f_nSelImg,
                                        int f_nEqnFlag,
                                        int f_nManualInitialGuessChk)
{
    CMatrixSolver l_cMatrixSolObj;
    int l_nMatrixDim = 2;

    try{
        int l_nCounter  = 0;
        long double l_dInitialA0 = 0.0;
        long double l_dInitialB0 = 0.0;
        long double l_dInitialM0 = 0.0;
        long double l_dInitialK0 = 0.0;
        long double l_dXd[3] = {0.0};
        long double l_dBd[3] = {0.0};
        int l_nStatus = 0;
													//	double *l_dRoiOutData = NULL;
        double l_dFitError_New = 0.0;
        double l_dFitError_Old = 0.0;
        double l_DampingLambda = 0.01;
        int l_UpdateJacobian =1;
        int l_nBreakFlag = 0;
        CString l_error_display_string;

//        TIPREP_TRACE(CLineFitStrategy::nonLinearCurveFit,MRTraceMarker9);
        
        if(NULL == f_dTimeStamp || NULL == f_dRoiOutData || NULL == f_dFitError) {
            l_error_display_string = (_T("setInitialValues Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            fflush(stdout);
            return (E_ERROR_NULL_FUNCPARAMS);
        }
        
        if(0 >= f_nSelImg) {
            l_error_display_string = (_T("Number of Images selected are invalid"));
														CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
														return (I_FEW_IMG);
													}
													
													if(NULL == m_dTempOutDataY ) {
														l_error_display_string = (_T("Unable to allocate memory"));
														CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
														return (E_MEM_ALLOC);
													}
													
													for(int l_nImgCnt = 0; l_nImgCnt < f_nSelImg; l_nImgCnt++) {
														m_dTempOutDataY[l_nImgCnt] = f_dRoiOutData[l_nImgCnt];
            f_dRoiOutData[l_nImgCnt] = f_dRoiOutData[l_nImgCnt] / 10000;
        }
        
        if(1 == f_nManualInitialGuessChk) {
            l_dInitialA0 = (m_dAFactor / 10000.0);
            l_dInitialB0 = (-1000.0 / m_dBFactor);
            l_dInitialM0 = m_dMFactor;
            l_dInitialK0 = m_dNFactor;
        } else {
            
            switch(f_nEqnFlag) {
            case 0:{CInitialGuess l_cInitialGus0;
                l_nStatus = l_cInitialGus0.InitialGuessForEquationOne(f_dTimeStamp, f_dRoiOutData,
                    f_nSelImg, &l_dInitialA0,
                    &l_dInitialB0);
                l_nMatrixDim = 2;}
                break;
            case 1:{ CInitialGuess l_cInitialGus1;
                l_nStatus = l_cInitialGus1.InitialGuessForEquationTwo(f_dTimeStamp, f_dRoiOutData,
                    f_nSelImg, &l_dInitialA0,
                    &l_dInitialB0, f_dRepetationTime);
                l_nMatrixDim = 2;}
                break;
            case 2:{CInitialGuess l_cInitialGus2;
                l_nStatus = l_cInitialGus2.InitialGuessForEquationThree(f_dTimeStamp, f_dRoiOutData,
                    f_nSelImg, &l_dInitialA0,
                    &l_dInitialB0, &l_dInitialM0,
                    f_dRepetationTime);
                l_nMatrixDim = 3;}
                break;
            case 3:{CInitialGuess l_cInitialGus3;
                l_nStatus = l_cInitialGus3.InitialGuessForEquationFour(f_dTimeStamp, f_dRoiOutData,
                    f_nSelImg, &l_dInitialA0,
                    &l_dInitialB0);
                l_nMatrixDim = 2;}
                break;
            case 4:{CInitialGuess l_cInitialGus4;
                l_nStatus = l_cInitialGus4.InitialGuessForEquationFive(f_dTimeStamp, f_dRoiOutData,
                    f_nSelImg, &l_dInitialA0,
                    &l_dInitialB0, &l_dInitialK0);
                l_nMatrixDim = 3;}
                break;
            default:
                break;
            }
        }

        if (E_NO_ERROR != l_nStatus)
        {

            l_error_display_string = (_T("Initial Guess Failed."));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return l_nStatus;
        }

        
		if(NULL == m_dMatrixA || NULL == m_dInvMatrixA) {
            l_error_display_string = (_T("Unable to allocate memory for matrix."));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return (E_ERROR);
        }
        
		l_nStatus = l_cMatrixSolObj.InitializeMatrix(m_dMatrixA, l_nMatrixDim);
		l_nStatus = l_cMatrixSolObj.InitializeMatrix(m_dInvMatrixA, l_nMatrixDim);
        
        //--------------------Calculate Error for initial state-------------------
        l_dFitError_Old = 0.0;
        
        l_nStatus = curveFitDataCalculation(&l_dFitError_Old, f_dTimeStamp,
														 NULL, f_dRepetationTime,
            f_nEqnFlag, f_nSelImg, l_dInitialA0,
            l_dInitialB0, l_dInitialM0, l_dInitialK0,
														m_dTempOutDataY);
        
        if(E_NO_ERROR != l_nStatus) {
            l_error_display_string = (_T("Curvefit data calculation failed for l_dFitError_Old"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            l_dInitialA0 = 0.0;
            l_dInitialB0 = 0.0;
            l_dInitialM0 = 0.0;
            l_dInitialK0 = 0.0;
            return (E_ERROR);
        }


        while(1) {
            long double l_dtempA = 0.0;
            long double l_dtempB = 0.0;
            long double l_dtempM = 0.0;
            long double l_dtempK = 0.0;
            char l_chBuff[256] = {'\0'};
            double l_dDeterminant = 0.0;

            if(1 == l_UpdateJacobian){//Matrix Fill
                l_cMatrixSolObj.setInitialValues(l_dInitialA0, l_dInitialB0, 
                    l_dInitialM0, l_dInitialK0, f_dTimeStamp, f_nSelImg, f_dRepetationTime);
            
                switch(f_nEqnFlag) {
                case 0:
                    l_nStatus = l_cMatrixSolObj.MatrixFillingForEquationOne 
                        (f_dRoiOutData, f_nSelImg, m_dMatrixA, l_dBd /*,l_nMatrixDim*/);
                    break;
                case 1:
                    l_nStatus = l_cMatrixSolObj.MatrixFillingForEquationTwo
                        (f_dRoiOutData, f_nSelImg, m_dMatrixA, l_dBd, l_nMatrixDim);
                    break;
                case 2:
                    l_nStatus = l_cMatrixSolObj.MatrixFillingForEquationThree
                        (f_dRoiOutData, f_nSelImg, m_dMatrixA, l_dBd, l_nMatrixDim);
                    break;
                case 3:
                    l_nStatus = l_cMatrixSolObj.MatrixFillingForEquationFour
                        (f_dRoiOutData, f_nSelImg, m_dMatrixA, l_dBd, l_nMatrixDim);
                    break;
                case 4:
                    l_nStatus = l_cMatrixSolObj.MatrixFillingForEquationFive
                        (f_dRoiOutData, f_nSelImg, m_dMatrixA, l_dBd, l_nMatrixDim);
                    break;
                default:
                    //error.
                    break;
                }
                
                if(E_NO_ERROR != l_nStatus) {
                    l_error_display_string.Format(_T("matrix filling failed."));
                    CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                    l_dInitialA0 = 0;
                    l_dInitialB0 = 0;
                    l_dInitialM0 = 0;
                    l_dInitialK0 = 0;
                    l_nCounter = 0;
                    break;
                }
            }//Matrix is ready 

//-----------Update Hessian Matrix with new updated Lambda in previous iteration H=J'*J; 
            //if l_DampingLambda =0 then Newtonian method method 
														if(E_NO_ERROR != l_cMatrixSolObj.UpdateMatrixAWithDampingFactor(m_dMatrixA,l_DampingLambda,l_nMatrixDim)){
                l_error_display_string.Format(_T("UpdateMatrixAWithDampingFactor failed."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                l_dInitialA0 = 0;
                l_dInitialB0 = 0;
                l_dInitialM0 = 0;
                l_dInitialK0 = 0;
                l_nCounter = 0;
                break;
            }

														l_dDeterminant = l_cMatrixSolObj.DeterminantOfMatrix(m_dMatrixA, l_nMatrixDim);
            sprintf(l_chBuff, "%x", l_dDeterminant);
            
            if(0 == strcmp(l_chBuff, "0") /*|| l_dDeterminant < 0.0000001*/ ) { 
//                 l_error_display_string.Format(_T("Determinant is zero."));
//                 CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                l_dInitialA0 = 1000;
                l_dInitialB0 = 0.5;
                l_dInitialM0 = 1;
                l_dInitialK0 = 0.5;
                l_nCounter++;
                if(f_nIteration == l_nCounter) {
//                     l_error_display_string = (_T("Iteration Reached"));
//                     CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                    
                    l_dInitialA0 = 0.0;
                    l_dInitialB0 = 0.0;
                    l_dInitialM0 = 0.0;
                    l_dInitialK0 = 0.0;
                    break;
                }
                continue;
               // break;
            }
														l_nStatus = l_cMatrixSolObj.InverseOfMatrix(m_dMatrixA, m_dInvMatrixA, l_nMatrixDim);
            if (E_NO_ERROR != l_nStatus)
            {
                l_error_display_string.Format(_T("Inverse matrix failed."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                l_dInitialA0 = 0.0;
                l_dInitialB0 = 0.0;
                l_dInitialM0 = 0.0;
                l_dInitialK0 = 0.0;
                break;
            }
														l_nStatus = l_cMatrixSolObj.SolverOfMatrices(m_dInvMatrixA, l_dXd, l_dBd, l_nMatrixDim);
            if (E_NO_ERROR != l_nStatus)
            {
                l_error_display_string.Format(_T("Matrix solved failed."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                l_dInitialA0 = 0.0;
                l_dInitialB0 = 0.0;
                l_dInitialM0 = 0.0;
                l_dInitialK0 = 0.0;
                break;
            }
            l_dtempA = l_dInitialA0 + l_dXd[0];
            l_dtempB = l_dInitialB0 + l_dXd[1];
            
            if(2 == f_nEqnFlag) {
                l_dtempM = l_dInitialM0 + l_dXd[2];
            } else if(4 == f_nEqnFlag) {
                l_dtempK = l_dInitialK0 + l_dXd[2];
            } else {
                l_dtempM = 0.0;
                l_dtempK = 0.0;
            }
            
            l_nStatus = l_cMatrixSolObj.DifferentialErrorCheck(l_dInitialA0, l_dInitialB0,
                l_dInitialM0, l_dInitialK0,
                l_dtempA, l_dtempB, l_dtempM,
                l_dtempK, f_dTimeStamp,
                f_dRoiOutData, f_nSelImg,
                f_dRepetationTime, f_nEqnFlag, l_nBreakFlag);
            
            if(E_NO_ERROR != l_nStatus) {

//KJ Commented DEFECT FIX 2013 NOV ----no need to break as damping factor controls the values

                /*l_error_display_string = (_T("DifferentialErrorCheck failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);

                l_dInitialA0 = 0.0;
                l_dInitialB0 = 0.0;
                l_dInitialM0 = 0.0;
                l_dInitialK0 = 0.0;
                break;*/
            }

//--------------------Calculate Error for current state-------------------
            l_dFitError_New = 0.0;
														//memset(l_dRoiOutData,0x0,f_nSelImg*sizeof(double));

            l_nStatus = curveFitDataCalculation(&l_dFitError_New, f_dTimeStamp,
															/*l_dRoiOutData*/NULL, f_dRepetationTime,
                f_nEqnFlag, f_nSelImg, l_dtempA,
                l_dtempB, l_dtempM, l_dtempK,
															m_dTempOutDataY);
            
            if(E_NO_ERROR != l_nStatus) {
                l_error_display_string = (_T("Curvefit data calculation failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
                l_dInitialA0 = 0.0;
                l_dInitialB0 = 0.0;
                l_dInitialM0 = 0.0;
                l_dInitialK0 = 0.0;
                break;
            }
//---------------------adjust damping factor here------------------------
            if(fabs(l_dFitError_New) < fabs(l_dFitError_Old)){
                l_dInitialA0 = l_dtempA;
                l_dInitialB0 = l_dtempB;
                l_dInitialM0 = l_dtempM;
                l_dInitialK0 = l_dtempK;
															//                l_nCounter++;
                l_dFitError_Old = l_dFitError_New;
                l_DampingLambda = l_DampingLambda* .5;//reduce it
                l_UpdateJacobian = 1; //go for filling matrix again  and solve it with new lambda
            }else{
                l_DampingLambda = l_DampingLambda* 50;//increase it
                l_UpdateJacobian = 0;//dont fill matrix only solve it with new lambda
            }
            if(1 == l_nBreakFlag) {
                l_nBreakFlag = 0;
                break;
            }
            
            if(f_nIteration == l_nCounter) {
//                 l_error_display_string = (_T("Iteration Reached"));
//                 CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);

                l_dInitialA0 = 0.0;
                l_dInitialB0 = 0.0;
                l_dInitialM0 = 0.0;
                l_dInitialK0 = 0.0;
                break;
            }
														l_nCounter++;
    }
    
    (*f_dFitError) = 0.0;
    l_nStatus = curveFitDataCalculation(f_dFitError, f_dTimeStamp,
        f_dRoiOutData, f_dRepetationTime,
        f_nEqnFlag, f_nSelImg, l_dInitialA0,
        l_dInitialB0, l_dInitialM0, l_dInitialK0,
									m_dTempOutDataY);
    
    if(E_NO_ERROR != l_nStatus) {
        l_error_display_string = (_T("Curvefit data calculation failed"));
        CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
    }
    
    if(1 == f_nEqnFlag) { // 2nd
        if(fabs(l_dInitialA0) < PRECISIOND && fabs(l_dInitialB0) < PRECISIOND) {

            (*f_dFitError) = 0.0;
        }
    } else if(2 == f_nEqnFlag) { // 3rd
        if(fabs(l_dInitialA0) < PRECISIOND && fabs(l_dInitialB0) < PRECISIOND && fabs(l_dInitialM0) < PRECISIOND) {
            (*f_dFitError) = 0.0;
        }
    } else if(3 == f_nEqnFlag) { // 4th
        if(fabs(l_dInitialA0) < PRECISIOND && fabs(l_dInitialB0) < PRECISIOND) {

            (*f_dFitError) = 0.0;
        }
    } else if(4 == f_nEqnFlag) { // 5th
        if(fabs(l_dInitialA0) < PRECISIOND && fabs(l_dInitialB0) < PRECISIOND && fabs(l_dInitialK0) < PRECISIOND) {

            (*f_dFitError) = 0.0;
        }
    } else { // 1st
        if(fabs(l_dInitialA0) < PRECISIOND && fabs(l_dInitialB0) < PRECISIOND) {

            (*f_dFitError) = 0.0;
        }
    }
        
    f_dFactorA = l_dInitialA0;
    f_dFactorB = l_dInitialB0;
    f_dFactorM = l_dInitialM0;
    f_dFactorK = l_dInitialK0;
    return E_NO_ERROR;

     } catch(...) {
         CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_LINEFIT, __LINE__, __FILE__);
         return E_ERROR;
     }
}

/*************************************************************
* Method:       tiT1Calculation
* Description:  This function is used
* Parameter:    double * f_dval_Ti
* Parameter:    double * f_dval_T1
* Parameter:    double & f_dRepetationTime
* Parameter:    double & f_dFactorA
* Parameter:    double & f_dFactorB
* Parameter:    double & f_dFactorM
* Parameter:    double & f_dFactorK
* Parameter:    int f_nEqnFlag
* Parameter:    int f_nRoi
* Returns:      int
*************************************************************/
inline int CLineFitStrategy::tiT1Calculation(double *f_dval_Ti,
                                      double*f_dval_T1,
                                      double &f_dRepetationTime,
                                      double &f_dFactorA,
                                      double &f_dFactorB,
                                      double &f_dFactorM,
                                      double &f_dFactorK,
                                      int f_nEqnFlag, int f_nRoi)
{
   //TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CLineFitStrategy::tiT1Calculation,MRTraceMarker9);
#endif
    try{
        switch(f_nEqnFlag) {
        case 0:
            f_dval_Ti[f_nRoi] = (log(0.5) / f_dFactorB) * (1000.0);//Seconds to Milliseconds Conversion
            break;
        case 1:
            f_dval_Ti[f_nRoi] = (log(fabs(1 + exp(f_dFactorB * f_dRepetationTime)) * 0.5) / f_dFactorB) * (1000.0);
            break;
        case 2:
            f_dval_Ti[f_nRoi] = (log(fabs(1 + f_dFactorM * exp(f_dFactorB * f_dRepetationTime) / f_dFactorA) * 0.5) / f_dFactorB) * (1000.0);
            break;
        case 3:
            //have doubt it should be
            //f_dval_Ti[f_nRoi] = (log (1) / f_dFactorB) * (1000 );
            //but it will be always zero.
            f_dval_Ti[f_nRoi] = 0.00;//fabs(1 / f_dFactorB) * (1000.0);//KJ -Explanation as this goes zero i.e at y=0 the x=0 then this graph should pass through origin
            break;
        case 4:
            f_dval_Ti[f_nRoi] = fabs(log(1 / f_dFactorK) / f_dFactorB) * (1000.0);
            break;
        default:
            break;
            //error
        }
        
        f_dval_T1[f_nRoi] = fabs(1 / f_dFactorB) * (1000.0);
        
//         if((f_dval_Ti[f_nRoi] > DBL_MAX) || (f_dval_Ti[f_nRoi] < DBL_MIN)){
//             f_dval_Ti[f_nRoi] = -1;
//         }
        
        /*if((f_dval_T1[f_nRoi] > DBL_MAX) || (f_dval_T1[f_nRoi] < DBL_MIN)){*/
        if(f_dval_T1[f_nRoi] == numeric_limits<double>::infinity( )){
            f_dval_T1[f_nRoi] = -1;
        }
//         }
    }catch(...){
        CString l_error_display_string;
        l_error_display_string = (_T("Exception occured in Curvefit"));
        CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:       curveFitDataCalculation
* Description:  This function is used
* Parameter:    double * f_dFitError
* Parameter:    double * f_dTimeStamp
* Parameter:    double * f_dRoiOutData
* Parameter:    double & f_dRepetationTime
* Parameter:    int f_nEqnFlag
* Parameter:    int f_nSelImg
* Parameter:    long double & f_dFactorA
* Parameter:    long double & f_dFactorB
* Parameter:    long double & f_dFactorM
* Parameter:    long double & f_dFactorK
* Parameter:    double * f_dInputDataY
* Returns:      int
*************************************************************/
inline int CLineFitStrategy::curveFitDataCalculation(double *f_dFitError, double *f_dTimeStamp, 
                                              double *f_dRoiOutData,double &f_dRepetationTime, 
                                              int f_nEqnFlag, int f_nSelImg,
                                              long double &f_dFactorA,long double &f_dFactorB,
                                              long double &f_dFactorM,long double &f_dFactorK, 
                                              double *f_dInputDataY)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CLineFitStrategy::curveFitDataCalculation,MRTraceMarker9);
#endif
    (*f_dFitError) = 0.0;
	
    try{
		double out_val = 0;
		for(int l_nCnt = 0; l_nCnt < f_nSelImg; l_nCnt++) {
		/*	if (f_dRoiOutData)
		{
		f_dRoiOutData[l_nCnt] = 0.0;
		}*/
			
			switch(f_nEqnFlag) {
            case 0:
                out_val = (f_dFactorA) * (1 - 2 * exp(f_dFactorB * f_dTimeStamp[l_nCnt]));
                break;
            case 1:
                out_val = (f_dFactorA) * (1 - 2 * exp(f_dFactorB * f_dTimeStamp[l_nCnt]) + exp(f_dFactorB * f_dRepetationTime));
                break;
            case 2:
                out_val = (f_dFactorA) * (1 - 2 * exp(f_dFactorB * f_dTimeStamp[l_nCnt])) + f_dFactorM * (exp(f_dFactorB * f_dRepetationTime));
                break;
            case 3:
                out_val = (f_dFactorA) * (1 - exp(f_dFactorB * f_dTimeStamp[l_nCnt]));
                break;
            case 4:
				out_val = (f_dFactorA) * (1 - (f_dFactorK) * exp(f_dFactorB * f_dTimeStamp[l_nCnt]));
                break;
            default:
                break;
			}
			
			out_val = out_val*10000;
			if (f_dRoiOutData)
			{
				f_dRoiOutData[l_nCnt] = /*f_dRoiOutData[l_nCnt]*/out_val;
			}
			(*f_dFitError) = (*f_dFitError) + (out_val - f_dInputDataY[l_nCnt]) * (out_val - f_dInputDataY[l_nCnt]);
		}

    return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_LINEFIT, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	setInitialGuessValues
* Description: 	This function is used 
* Parameter: 	const double & f_dAFactor :
* Parameter: 	const double & f_dBFactor :
* Parameter: 	const double & f_dMFactor :
* Parameter: 	const double & f_dNFactor :
* Returns:   	int
*************************************************************/
int CLineFitStrategy::setInitialGuessValues(const double &f_dAFactor, const double &f_dBFactor,
        const double &f_dMFactor, const double &f_dNFactor)

{
    m_dAFactor = f_dAFactor;
    m_dBFactor = f_dBFactor;
    m_dMFactor = f_dMFactor;
    m_dNFactor = f_dNFactor;
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	getInitialGuessValues
* Description: 	This function is used 
* Parameter: 	double & f_dAFactor :
* Parameter: 	double & f_dBFactor :
* Parameter: 	double & f_dMFactor :
* Parameter: 	double & f_dNFactor :
* Returns:   	int
*************************************************************/
int CLineFitStrategy::getInitialGuessValues(double &f_dAFactor, double &f_dBFactor,
        double &f_dMFactor, double &f_dNFactor)
{
    f_dAFactor = m_dAFactor;
    f_dBFactor = m_dBFactor;
    f_dMFactor = m_dMFactor;
    f_dNFactor = m_dNFactor;
    return E_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
int CLineFitStrategy::AllocTimeStamp(int *timestamp, int numimgs)
{
	try {
		if (numimgs <= 0)
			return 1;
		if (m_timestamp_calc)delete []m_timestamp_calc;
		m_timestamp_calc = NULL;
		m_timestamp_calc = new double[numimgs];
		if (m_timestamp_calc  == NULL)
			return 1;
		
		for (int i=0; i < numimgs; i++)
		{
			m_timestamp_calc[i] = (double)timestamp[i] / (1000);
		}
		return 0;
	}catch (...)
	{
		return 1;
		//log error
	}
}

int CLineFitStrategy::DeAllocTimeStampData()
{
	try {
		if (m_timestamp_calc)delete []m_timestamp_calc;
		m_timestamp_calc = NULL;
		return 0;
	}catch(...) {
		return 1;
	}
	
}
int CLineFitStrategy::AllocTmpBuffers(int numimgs, int f_eqn)
{
	if (numimgs <= 0)
		return 1;
	if (m_dTempOutDataY)delete []m_dTempOutDataY;
	m_dTempOutDataY = NULL;
	m_dTempOutDataY = new double[numimgs];
	if (m_dTempOutDataY  == NULL)
		return 1;
	memset(m_dTempOutDataY,0,sizeof(double)*numimgs);
	CMatrixSolver l_cMatrixSolObj;
	int l_nMatrixDim = 0;
	switch (f_eqn)
	{
	case 0:
		l_nMatrixDim = 2;
		break;
	case 1:
		l_nMatrixDim = 2;
		break;
	case 2:
		l_nMatrixDim = 3;
		break;
	case 3:
		l_nMatrixDim = 2;
		break;
	case 4:
		l_nMatrixDim = 3;
		break;
	}
	m_dMatrixA = l_cMatrixSolObj.dmatrix(1, l_nMatrixDim, 1, l_nMatrixDim);
	m_dInvMatrixA = l_cMatrixSolObj.dmatrix(1, l_nMatrixDim, 1, l_nMatrixDim);
	if (m_dMatrixA == NULL || m_dInvMatrixA == NULL)
		return 1;
	l_cMatrixSolObj.InitializeMatrix(m_dMatrixA, l_nMatrixDim);
	l_cMatrixSolObj.InitializeMatrix(m_dInvMatrixA, l_nMatrixDim);
	return 0;
}
int CLineFitStrategy::DeAllocTmpBuffers(int f_eqn)
{
	try {
		CMatrixSolver l_cMatrixSolObj;
		int l_nMatrixDim = 0;
		switch (f_eqn)
		{
		case 0:
			l_nMatrixDim = 2;
			break;
		case 1:
			l_nMatrixDim = 2;
			break;
		case 2:
			l_nMatrixDim = 3;
			break;
		case 3:
			l_nMatrixDim = 2;
			break;
		case 4:
			l_nMatrixDim = 3;
			break;
		}
		l_cMatrixSolObj.free_dmatrix(m_dMatrixA, 1, l_nMatrixDim, 1, l_nMatrixDim);
		l_cMatrixSolObj.free_dmatrix(m_dInvMatrixA, 1, l_nMatrixDim, 1, l_nMatrixDim);
		if (m_dTempOutDataY){
			delete []m_dTempOutDataY;
			m_dTempOutDataY = NULL;
		}
		return 0;
	}catch (...)
	{
		return 1;
		//log error
	}
}

void CLineFitStrategy::DummyTest(double *y, int *x, int numimgs, double * outy)
{
	double sumy;
	double sumx;
for (int i  = 0; i < numimgs; i++)
{
sumx += x[i];
sumy += y[i];
outy[i]  = y[i]*100;
}
	

}
