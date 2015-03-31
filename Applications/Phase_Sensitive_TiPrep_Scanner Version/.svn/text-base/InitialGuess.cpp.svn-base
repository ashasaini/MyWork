// InitialGuess.cpp: implementation of the CInitialGuess class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "math.h"
#include "tiprep.h"
#include "InitialGuess.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#define MAX_ALLOWED_ITERATION 100
#define ZERO 0
const CString EXCEPTION_INITIALGUESS = L"Exception occurred in Initial Guess ";   
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CInitialGuess
//Purpose       : Constructor
//********************************************************************
CInitialGuess::CInitialGuess()
{
    m_initial_division_factor_for_newton_raph = 0.0;
    /*TIPREP_TRACE(CInitialGuess::CInitialGuess);*/
}
//****************************Method Header***************************
//Method Name   : CInitialGuess
//Purpose       : Destructor
//********************************************************************
CInitialGuess::~CInitialGuess()
{
    /*TIPREP_TRACE(CInitialGuess::~CInitialGuess);*/
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/* Equations:
1. f(t) = A(1 - 2exp(-ti/t1))
2. f(t) = A(1 - 2exp(-ti/t1) + exp(-tr/t1))
3. f(t) = A(1 - 2exp(-ti/t1)) +B(exp(-tr/t1))
4. f(t) = A(1 - exp(-ti/t1))
5. f(t) = A(1 -n * exp(-ti/t1))
*/

/*************************************************************
* Method:       InitialGuessForEquationOne
* Description:  This function is used
* Parameter:    double * f_x_cord
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Parameter:    long double * f_initial_value_of_A
* Parameter:    long double * f_initial_value_of_inverse_t1
* Returns:      void
*************************************************************/
int CInitialGuess::InitialGuessForEquationOne(double *f_x_cord,
                                              double *f_y_cord,
                                              int f_datapoints,
                                              long double *f_initial_value_of_A,
                                              long double *f_initial_value_of_inverse_t1)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CInitialGuess::InitialGuessForEquationOne,MRTraceMarker9);
#endif
    
    int l_equation_flag = 1;
    
    try{
        CString l_error_display_string(_T(""));
        
        if((NULL == f_x_cord) || (NULL == f_y_cord) ||
            (NULL == f_initial_value_of_A) || (NULL == f_initial_value_of_inverse_t1)) { 
            l_error_display_string = (_T("InitialGuessForEquationOne Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        double y1 = 0.0;
        double y2 = 0.0;
        double x1 = 0.0;
        double x2 = 0.0;
        if(E_NO_ERROR != Calculate2Points(f_datapoints,f_y_cord,f_x_cord,y1,y2,x1,x2)){
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Calculate2Points failed"), __LINE__, __FILE__);
            return E_ERROR;
        }
        double l_result_of_Newton_raph = 0;
        double l_repetition_time = 0;
        int l_error_status = DivisionFactorForNewtonRaphson( y1, y2);
        
        if(E_NO_ERROR != l_error_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("DivisionFactorForNewtonRaphson failed"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        NewtonRaphsonSolving( x1, x2, y1, y2,f_datapoints, 
            l_repetition_time, l_equation_flag, l_result_of_Newton_raph);
        
        if(fabs(l_result_of_Newton_raph) > 10) {
            l_result_of_Newton_raph = 1.0;
        }
        double tempY = 0.0;
        double tempX = 0.0;

        tempY = (y1+y2)*0.5;
        tempX = (x1+x2)*0.5;

        (*f_initial_value_of_inverse_t1) = l_result_of_Newton_raph;
        (*f_initial_value_of_A) = ((tempY) / (1 - (2 * exp((*f_initial_value_of_inverse_t1) * tempX))));
        //TRACE_COMMENTED
		//+Mpower/20130902/PN
#ifdef _UT_LOG_
		CString l_initial_value_of_inverse_t1 = _T("");
		l_initial_value_of_inverse_t1.Format(_T("Intial value  of T1 for eqn 1 = %lf\n"), *f_initial_value_of_inverse_t1 );
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_inverse_t1,
			_T("CInitialGuess::InitialGuessForEquationOne"));

		CString l_initial_value_of_A = _T("");
		l_initial_value_of_A.Format(_T("Intial value  of A for eqn 1 = %lf\n"), *f_initial_value_of_A);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_A,
                _T("CInitialGuess::InitialGuessForEquationOne"));
#endif

		//-Mpower/20130902/PN
		//TRACE_COMMENTED

		return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       InitialGuessForEquationTwo
* Description:  This function is used
* Parameter:    double * f_x_cord
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Parameter:    long double * f_initial_value_of_A
* Parameter:    long double * f_initial_value_of_inverse_t1
* Parameter:    double f_repetition_time
* Returns:      void
*************************************************************/
int CInitialGuess::InitialGuessForEquationTwo(double *f_x_cord,
                                              double *f_y_cord,
                                              int f_datapoints,
                                              long double *f_initial_value_of_A,
                                              long double *f_initial_value_of_inverse_t1,
                                              double f_repetition_time)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CInitialGuess::InitialGuessForEquationTwo,MRTraceMarker9);
#endif
    
    //int l_equation_flag = 2;
    
    try{
        CString l_error_display_string(_T(""));
        
        if((NULL == f_x_cord) || (NULL == f_y_cord) || 
            (NULL == f_initial_value_of_inverse_t1) || 
            (NULL == f_initial_value_of_A)) {
            l_error_display_string = (_T("InitialGuessForEquationTwo Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        //double l_result_of_Newton_raph = 0;
        int l_first_position_of_y = 0;
        int l_centre_position_of_y = 0;
        int l_last_position_of_y = 0;
        
        if(f_repetition_time > 0) {
            l_first_position_of_y = 0;
            
            if(0 != (f_datapoints % 2)) { 
                l_centre_position_of_y = ((f_datapoints - 1) / 2);
            } else {
                l_centre_position_of_y = (f_datapoints) / 2;
            }
            
            l_last_position_of_y = f_datapoints - 1;
            /*
            if(E_NO_ERROR != Calculate2Points(f_datapoints,f_y_cord,f_x_cord,y1,y2,x1,x2)){
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("Calculate2Points failed"), __LINE__, __FILE__);
                return E_ERROR;
            }
            int l_error_status = DivisionFactorForNewtonRaphson(y1,y2);
            
            if(E_NO_ERROR != l_error_status) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("DivisionFactorForNewtonRaphson failed"), __LINE__, __FILE__);
                return E_ERROR;
            }
            
            NewtonRaphsonSolving( x1, x2, y1, y2, f_datapoints, 
                f_repetition_time, l_equation_flag,l_result_of_Newton_raph);*/

            /*
            if(E_NO_ERROR != Calculate3Points(f_datapoints,f_y_cord,f_x_cord,y1,y2,y3,x1,x2,x3)){
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("Calculate2Points failed"), __LINE__, __FILE__);
                return E_ERROR;
            }
            (*f_initial_value_of_inverse_t1) = 
            (log(fabs((y2 - y1)
            / (y2 - y3))))
            / (x1 - x3);
            (*f_initial_value_of_A) = (y1
            / (1 + exp((*f_initial_value_of_inverse_t1) * f_repetition_time)
            - 2 * exp((*f_initial_value_of_inverse_t1) * x1)));
            */

            (*f_initial_value_of_inverse_t1) = 
                (log(fabs((f_y_cord[l_centre_position_of_y] - f_y_cord[l_first_position_of_y])
                / (f_y_cord[l_centre_position_of_y] - f_y_cord[l_last_position_of_y]))))
                / (f_x_cord[l_first_position_of_y] - f_x_cord[l_last_position_of_y]);
            (*f_initial_value_of_A) = (f_y_cord[l_first_position_of_y]
                / (1 + exp((*f_initial_value_of_inverse_t1) * f_repetition_time)
                - 2 * exp((*f_initial_value_of_inverse_t1) * f_x_cord [l_first_position_of_y])));
        }
        //TRACE_COMMENTED
		//+Mpower/20130902/PN
#ifdef _UT_LOG_
		CString l_initial_value_of_inverse_t1 = _T("");
		l_initial_value_of_inverse_t1.Format(_T("Intial value  of T1 for eqn 2 = %lf\n"), *f_initial_value_of_inverse_t1 );
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_inverse_t1,
			_T("CInitialGuess::InitialGuessForEquationTwo"));
		
		CString l_initial_value_of_A = _T("");
		l_initial_value_of_A.Format(_T("Intial value  of A for eqn 2 = %lf\n"), *f_initial_value_of_A);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_A,
			_T("CInitialGuess::InitialGuessForEquationTwo"));
#endif
		
		//-Mpower/20130902/PN
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       InitialGuessForEquationThree
* Description:  This function is used
* Parameter:    double * f_x_cord
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Parameter:    long double * f_initial_value_of_A
* Parameter:    long double * f_initial_value_of_inverse_t1
* Parameter:    long double * f_initial_value_of_B
* Parameter:    double f_repetition_time
* Returns:      void
*************************************************************/
int CInitialGuess::InitialGuessForEquationThree(double *f_x_cord,
                                                double *f_y_cord,
                                                int f_datapoints,
                                                long double *f_initial_value_of_A,
                                                long double *f_initial_value_of_inverse_t1,
                                                long double *f_initial_value_of_B,
                                                double f_repetition_time
                                                )
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//   TIPREP_TRACE(CInitialGuess::InitialGuessForEquationThree,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        
        if((NULL == f_x_cord) || (NULL == f_y_cord) || (NULL == f_initial_value_of_inverse_t1)
            || (NULL == f_initial_value_of_A) || (NULL == f_initial_value_of_B)) { 
            l_error_display_string = (_T("InitialGuessForEquationThree Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        int l_first_position_of_y = 0;
        int l_centre_position_of_y = 0;
        int l_last_position_of_y = 0;
        
        if(f_repetition_time > 0) {
            if(f_datapoints > 3) {
                l_first_position_of_y = 0;
                
                if(f_datapoints % 2) {
                    l_centre_position_of_y = ((f_datapoints - 1) / 2);
                } else {
                    l_centre_position_of_y = (f_datapoints) / 2;
                }
                
                l_last_position_of_y = f_datapoints - 1;
                (*f_initial_value_of_inverse_t1) = 
                    (log(fabs((f_y_cord[l_centre_position_of_y] - f_y_cord[l_first_position_of_y])
                    / (f_y_cord[l_centre_position_of_y] - f_y_cord[l_last_position_of_y]))))
                    / (f_x_cord[l_first_position_of_y] - f_x_cord[l_last_position_of_y]);
                (*f_initial_value_of_A) =    
                    fabs(0.5 * (f_y_cord[l_centre_position_of_y] - f_y_cord[l_first_position_of_y])
                    / (exp((*f_initial_value_of_inverse_t1) * f_x_cord [l_first_position_of_y])
                    - exp((*f_initial_value_of_inverse_t1) * f_x_cord [l_centre_position_of_y])));
                (*f_initial_value_of_B) = (f_y_cord[l_first_position_of_y] - ((*f_initial_value_of_A)
                    * (1 - 2 * exp((*f_initial_value_of_inverse_t1) * f_x_cord[l_first_position_of_y]))))
                    / exp((*f_initial_value_of_inverse_t1) * f_repetition_time);
            }
        }

		//+Mpower/20130902/PN
		//TRACE_COMMENTED
#ifdef _UT_LOG_
		CString l_initial_value_of_inverse_t1 = _T("");
		l_initial_value_of_inverse_t1.Format(_T("Intial value  of T1 for eqn 3 = %lf\n"), *f_initial_value_of_inverse_t1 );
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_inverse_t1,
			_T("CInitialGuess::InitialGuessForEquationThree"));
		
		CString l_initial_value_of_A = _T("");
		l_initial_value_of_A.Format(_T("Intial value  of A for eqn 3 = %lf\n"), *f_initial_value_of_A);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_A,
			_T("CInitialGuess::InitialGuessForEquationThree"));

		CString l_initial_value_of_B = _T("");
		l_initial_value_of_B.Format(_T("Intial value  of B for eqn 3 = %lf\n"), *f_initial_value_of_B);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_B,
			_T("CInitialGuess::InitialGuessForEquationThree"));
#endif
		
		//-Mpower/20130902/PN
		//TRACE_COMMENTED
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       InitialGuessForEquationFour
* Description:  This function is used
* Parameter:    double * f_x_cord
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Parameter:    long double * f_initial_value_of_A
* Parameter:    long double * f_initial_value_of_inverse_t1
* Returns:      void
*************************************************************/
int CInitialGuess::InitialGuessForEquationFour(double *f_x_cord,
                                               double *f_y_cord,
                                               int f_datapoints,
                                               long double *f_initial_value_of_A,
                                               long double *f_initial_value_of_inverse_t1)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CInitialGuess::InitialGuessForEquationFour,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        
        if((NULL == f_x_cord) || (NULL == f_y_cord) || 
            (NULL == f_initial_value_of_A) || (NULL == f_initial_value_of_inverse_t1)) { 
            l_error_display_string = (_T("InitialGuessForEquationFour Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        //int l_first_position_of_y = 0;
        //int l_centre_position_of_y = 0;
        //int l_last_position_of_y = 0;
        
        if(f_datapoints < 3) {
            l_error_display_string = (_T("Less than two Images"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        double y1 = 0.0;
        double y2 = 0.0;
        double x1 = 0.0;
        double x2 = 0.0;
        if(E_NO_ERROR != Calculate2Points(f_datapoints,f_y_cord,f_x_cord,y1,y2,x1,x2)){
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Calculate2Points failed"), __LINE__, __FILE__);
            return E_ERROR;
        }
        double l_result_of_Newton_raph = 0;
        double l_repetition_time = 0;
        int l_error_status = DivisionFactorForNewtonRaphson( y1, y2);
        
        if(E_NO_ERROR != l_error_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("DivisionFactorForNewtonRaphson failed"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        NewtonRaphsonSolving( x1, x2, y1, y2,f_datapoints, 
            l_repetition_time, 1, l_result_of_Newton_raph);
        
        if(fabs(l_result_of_Newton_raph) > 10) {
            l_result_of_Newton_raph = 1.0;
        }
        double tempY = 0.0;
        double tempX = 0.0;
        
        tempY = (y1+y2)*0.5;
        tempX = (x1+x2)*0.5;
        
        (*f_initial_value_of_inverse_t1) = fabs(l_result_of_Newton_raph);//Fabs required
        (*f_initial_value_of_A) = ((tempY) / (1 - (exp((*f_initial_value_of_inverse_t1) * tempX))));

        //TRACE_COMMENTED
		//+Mpower/20130902/PN
#ifdef _UT_LOG_
		CString l_initial_value_of_inverse_t1 = _T("");
		l_initial_value_of_inverse_t1.Format(_T("Intial value  of T1 for eqn 4 = %lf\n"), *f_initial_value_of_inverse_t1 );
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_inverse_t1,
			_T("CInitialGuess::InitialGuessForEquationFour"));
		
		CString l_initial_value_of_A = _T("");
		l_initial_value_of_A.Format(_T("Intial value  of A for eqn 4 = %lf\n"), *f_initial_value_of_A);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_A,
			_T("CInitialGuess::InitialGuessForEquationFour"));
#endif
		
		//-Mpower/20130902/PN
		//TRACE_COMMENTED
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       InitialGuessForEquationFive
* Description:  This function is used
* Parameter:    double * f_x_cord
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Parameter:    long double * f_initial_value_of_A
* Parameter:    long double * f_initial_value_of_inverse_t1
* Parameter:    long double * f_initial_value_of_nebla
* Returns:      void
*************************************************************/
int CInitialGuess::InitialGuessForEquationFive(double *f_x_cord,
                                               double *f_y_cord,
                                               int f_datapoints,
                                               long double *f_initial_value_of_A,
                                               long double *f_initial_value_of_inverse_t1,
                                               long double *f_initial_value_of_nebla)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CInitialGuess::InitialGuessForEquationFive,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        
        if((NULL == f_x_cord) || (NULL == f_y_cord) || (NULL == f_initial_value_of_inverse_t1)
            || (NULL == f_initial_value_of_nebla) || (NULL == f_initial_value_of_A)) { 
            l_error_display_string = (_T("InitialGuessForEquationFive Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        int l_first_position_of_y = 0;
        int l_centre_position_of_y = 0;
        int l_last_position_of_y = 0;
        
        if(f_datapoints < 3) {
            l_error_display_string = (_T("Less than two Images"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_first_position_of_y = 0;
        
        if(f_datapoints % 2) {
            l_centre_position_of_y = ((f_datapoints - 1) / 2);
        } else {
            l_centre_position_of_y = (f_datapoints) / 2;
        }
        
        l_last_position_of_y = f_datapoints - 1;
        (*f_initial_value_of_inverse_t1) = (log(fabs((f_y_cord[l_centre_position_of_y] - f_y_cord[l_first_position_of_y])
            / (f_y_cord[l_centre_position_of_y] - f_y_cord[l_last_position_of_y]))))
            / (f_x_cord[l_first_position_of_y] - f_x_cord[l_last_position_of_y]);
        (*f_initial_value_of_A) = fabs(0.5 * (f_y_cord[l_centre_position_of_y] - f_y_cord[l_first_position_of_y])
            / (exp((*f_initial_value_of_inverse_t1) * f_x_cord [l_first_position_of_y])
            - exp((*f_initial_value_of_inverse_t1) * f_x_cord [l_centre_position_of_y])));
        
        //To avoid Divide By Zero Error
        if(fabs(*f_initial_value_of_A) < 0.000001) {
            (*f_initial_value_of_A) = f_y_cord[0]; 
        }
        
        (*f_initial_value_of_nebla) = (1 - (f_y_cord[l_first_position_of_y]
            / (*f_initial_value_of_A)))
            / (exp((*f_initial_value_of_inverse_t1) * f_x_cord [l_first_position_of_y]));
        //TRACE_COMMENTED
		//+Mpower/20130902/PN
#ifdef _UT_LOG_
		CString l_initial_value_of_inverse_t1 = _T("");
		l_initial_value_of_inverse_t1.Format(_T("Intial value  of T1 for eqn 5 = %lf\n"), *f_initial_value_of_inverse_t1 );
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_inverse_t1,
			_T("CInitialGuess::InitialGuessForEquationFour"));
		
		CString l_initial_value_of_A = _T("");
		l_initial_value_of_A.Format(_T("Intial value  of A for eqn 5 = %lf\n"), *f_initial_value_of_A);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_A,
			_T("CInitialGuess::InitialGuessForEquationFour"));

		CString l_initial_value_of_nebla = _T("");
		l_initial_value_of_nebla.Format(_T("Intial value  of nebla for eqn 5 = %lf\n"), *f_initial_value_of_nebla);
		CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker6, l_initial_value_of_nebla,
			_T("CInitialGuess::InitialGuessForEquationFour"));
#endif
		
		//-Mpower/20130902/PN
		//TRACE_COMMENTED
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       DivisionFactorForNewtonRaphson
* Description:  This function is used
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Returns:      void
*************************************************************/
int CInitialGuess::DivisionFactorForNewtonRaphson(double y1,double y2
                                                  )
{
//    TIPREP_TRACE(CInitialGuess::DivisionFactorForNewtonRaphson,MRTraceMarker9);
    
    try{
        m_initial_division_factor_for_newton_raph = 0.0;
        if(0 < fabs(y2)) {
            m_initial_division_factor_for_newton_raph = y1/y2;
        } else {
            m_initial_division_factor_for_newton_raph = y1 ;
        }        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       NewtonRaphsonSolving
* Description:  This function is used
* Parameter:    double * f_x_cord
* Parameter:    double * f_y_cord
* Parameter:    int f_datapoints
* Parameter:    double f_repetition_time
* Parameter:    int f_equation_flag
* Returns:      double
*************************************************************/
int CInitialGuess::NewtonRaphsonSolving(double x1,double x2,double y1,double y2,
                                        int f_datapoints,
                                        double f_repetition_time,
                                        int f_equation_flag,
                                        double & f_result_of_Newton_raph)
{
//    TIPREP_TRACE(CInitialGuess::NewtonRaphsonSolving,MRTraceMarker9);
    
    try{
        
        static int l_no_of_times_newton_raph_performed = 0;
        double l_inital_guess_netwon_raph = 0.1;
        double l_result_of_initial_fun = 0.0;
        double l_result_of_first_diff_of_initial_fun = 1.0;
        l_inital_guess_netwon_raph = 1.0;

        l_no_of_times_newton_raph_performed = 0;
being:
        FirstDifferentiationOfInitialFunctionForNewtonRaphson(l_inital_guess_netwon_raph, f_datapoints,
            m_initial_division_factor_for_newton_raph, x1,x2,y1,y2,
            f_equation_flag, f_repetition_time,
            l_result_of_first_diff_of_initial_fun);
        
        if(ZERO == l_result_of_first_diff_of_initial_fun){

            if((l_inital_guess_netwon_raph - 0.000001) < 0) {
                l_inital_guess_netwon_raph = fabs(l_inital_guess_netwon_raph - 0.000001);
            } else {
                l_inital_guess_netwon_raph = l_inital_guess_netwon_raph - 0.000001;
            }
        }
        
        InitialFunctionForNewtonRaphson(l_inital_guess_netwon_raph, f_datapoints, 
            m_initial_division_factor_for_newton_raph, 
            x1,x2,y1,y2, f_equation_flag, 
            f_repetition_time, l_result_of_initial_fun);
        
        FirstDifferentiationOfInitialFunctionForNewtonRaphson(l_inital_guess_netwon_raph, 
            f_datapoints, m_initial_division_factor_for_newton_raph, 
            x1,x2,y1,y2, f_equation_flag,  f_repetition_time,
            l_result_of_first_diff_of_initial_fun);
        
        f_result_of_Newton_raph = 
            (l_inital_guess_netwon_raph - (l_result_of_initial_fun / l_result_of_first_diff_of_initial_fun));

        if(fabs(f_result_of_Newton_raph) <0.000001){
            f_result_of_Newton_raph = 1.0;
        }
        
        if(fabs((f_result_of_Newton_raph - l_inital_guess_netwon_raph) / f_result_of_Newton_raph) < 0.0001) {
            
        } else {
            l_inital_guess_netwon_raph = f_result_of_Newton_raph;
            
            if(fabs(l_inital_guess_netwon_raph) < 0.0001) {
                l_inital_guess_netwon_raph = -1.0;
            }
            
            l_no_of_times_newton_raph_performed = l_no_of_times_newton_raph_performed + 1;
            
            if((l_no_of_times_newton_raph_performed < MAX_ALLOWED_ITERATION)) {
                goto being;
            }
        }
        
        return E_NO_ERROR;
        
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_NO_ERROR;
    }
}

/*************************************************************
* Method:       InitialFunctionForNewtonRaphson
* Description:  This function is used
* Parameter:    double f_inital_guess_netwon_raph
* Parameter:    int f_datapoints
* Parameter:    double f_division_factor_for_newton_raph
* Parameter:    double * f_x_cord
* Parameter:    int f_equation_flag
* Parameter:    double * f_y_cord
* Parameter:    double f_repetition_time
* Returns:      double
*************************************************************/
int CInitialGuess::InitialFunctionForNewtonRaphson(double f_inital_guess_netwon_raph,
                                                   int f_datapoints,
                                                   double f_division_factor_for_newton_raph,
                                                   double x1,double x2,double y1,double y2,
                                                   int f_equation_flag,                                                   
                                                   double f_repetition_time,
                                                   double & f_InitialVal)
{
//    TIPREP_TRACE(CInitialGuess::InitialFunctionForNewtonRaphson,MRTraceMarker9);
    
    try{
        
        if(2 == f_equation_flag) {
            
            f_InitialVal = 
                ((1) + (2 * (y2 / y1) * exp(f_inital_guess_netwon_raph * (x1)))
                - (2 * exp(f_inital_guess_netwon_raph * (x2)))
                + (exp(f_inital_guess_netwon_raph * (f_repetition_time)) * (1 - (y2 / y1)))
                - (y2 / y1));
        } else if(1 == f_equation_flag) {
            f_InitialVal = (f_division_factor_for_newton_raph * exp(f_inital_guess_netwon_raph * x1)
                - exp(f_inital_guess_netwon_raph * x2)
                + 0.5 * (1 - f_division_factor_for_newton_raph));
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       FirstDifferentiationOfInitialFunctionForNewtonRaphson
* Description:  This function is used
* Parameter:    double f_inital_guess_netwon_raph
* Parameter:    int f_datapoints
* Parameter:    double f_division_factor_for_newton_raph
* Parameter:    double * f_x_cord
* Parameter:    int f_equation_flag
* Parameter:    double * f_y_cord
* Parameter:    double f_repetition_time
* Returns:      double
*************************************************************/
int CInitialGuess::FirstDifferentiationOfInitialFunctionForNewtonRaphson(double f_inital_guess_netwon_raph,
                                                                         int f_datapoints,
                                                                         double f_division_factor_for_newton_raph,
                                                                         double x1,double x2,double y1,double y2,
                                                                         int f_equation_flag,                                                                         
                                                                         double f_repetition_time,
                                                                         double & f_FirstDifferential)
{
//    TIPREP_TRACE(CInitialGuess::FirstDifferentiationOfInitialFunctionForNewtonRaphson,MRTraceMarker9);
    
    try{        
        if(2 == f_equation_flag) {
            f_FirstDifferential = 
                ((2 * (y2 / y1) * (x1) * exp(f_inital_guess_netwon_raph * (x1)))
                - (2 * (x2) * exp(f_inital_guess_netwon_raph * (x2)))
                + ((f_repetition_time) * exp(f_inital_guess_netwon_raph * (f_repetition_time)) * (1 - (y2 / y1)))
                );
        } else if(1 == f_equation_flag) {
            f_FirstDifferential = 
                (f_division_factor_for_newton_raph * x1 * exp(f_inital_guess_netwon_raph * x1)
                - x2 * exp(f_inital_guess_netwon_raph * x2));
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_INITIALGUESS, __LINE__, __FILE__);
        return E_ERROR;
    }
}

int CInitialGuess::Calculate2Points( int f_datapoints, double *y, double*x, double &y1,double &y2, double &x1,double &x2 )
{
    if(NULL == y || NULL == x || f_datapoints<=0){
        return E_ERROR;
    }
    int counter =0;;
    for(int i=0;i<(int)f_datapoints/2;i++){
        y1+=y[i];
        x1+=x[i];
        counter++;
    }
    y1=y1/counter;
    x1=x1/counter;

    counter = 0;
    for(i=(int)f_datapoints/2;i<f_datapoints;i++){
        y2+=y[i];
        x2+=x[i];
        counter++;
    }
    y2=y2/counter;
    x2=x2/counter;
    return E_NO_ERROR;
}

int CInitialGuess::Calculate3Points( int f_datapoints, double *y, double*x, double &y1,double &y2,double &y3, double &x1,double &x2,double &x3 )
{
    if(NULL == y || NULL == x || f_datapoints<=0){
        return E_ERROR;
    }
    int counter = 0;
    for(int i=0;i<(int)f_datapoints/3;i++){
        y1+=y[i];
        x1+=x[i];
        counter++;
    }
    y1=y1/counter;
    x1=x1/counter;

    counter = 0;
    for(i=(int)f_datapoints/3;i<(int)2*f_datapoints/3;i++){
        y2+=y[i];
        x2+=x[i];
        counter++;
    }
    y2=y2/counter;
    x2=x2/counter;
    
    counter = 0;
    for(i=(int)2*f_datapoints/3;i<f_datapoints;i++){
        y3+=y[i];
        x3+=x[i];
        counter++;
    }
    y3=y3/counter;
    x3=x3/counter;
    return E_NO_ERROR;

}



///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

