// InitialGuess.h: interface for the CInitialGuess class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_INITIALGUESS_H__B3BEC13F_D5C1_4737_94BB_293EEFD7B47F__INCLUDED_)
#define AFX_INITIALGUESS_H__B3BEC13F_D5C1_4737_94BB_293EEFD7B47F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInitialGuess
{
public://construction,copy & destruction
    CInitialGuess();
    virtual ~CInitialGuess();
    
public:
    
    int InitialGuessForEquationOne(double *f_x_cord,
        double *f_y_cord,
        int f_datapoints,
        long double *f_initial_value_of_A,
        long double *f_initial_value_of_inverse_t1);
    
    int InitialGuessForEquationTwo(double *f_x_cord,
        double *f_y_cord,
        int f_datapoints,
        long double *f_initial_value_of_A,
        long double *f_initial_value_of_inverse_t1,
        double f_repetition_time);
    
    int InitialGuessForEquationThree(double *f_x_cord,
        double *f_y_cord,
        int f_datapoints,
        long double *f_initial_value_of_A,
        long double *f_initial_value_of_inverse_t1,
        long double *f_initial_value_of_B,
        double f_repetition_time);
    
    int InitialGuessForEquationFour(double *f_x_cord,
        double *f_y_cord,
        int f_datapoints,
        long double *f_initial_value_of_A,
        long double *f_initial_value_of_inverse_t1);
    
    int InitialGuessForEquationFive(double *f_x_cord,
        double *f_y_cord,
        int f_datapoints,
        long double *f_initial_value_of_A,
        long double *f_initial_value_of_inverse_t1,
        long double *f_initial_value_of_nebla);
private:
    
    double m_initial_division_factor_for_newton_raph;
    
private:
    
    int DivisionFactorForNewtonRaphson(double y1,double y2
        );
    
    int NewtonRaphsonSolving(double x1,double x2,double y1,double y2,
        int f_datapoints,
        double f_repetition_time,
        int f_equation_flag,
        double & f_result_of_Newton_raph);
    
    int InitialFunctionForNewtonRaphson(double f_inital_guess_netwon_raph,
        int f_datapoints,
        double f_division_factor_for_newton_raph,
        double x1,double x2,double y1,double y2,
        int f_equation_flag,        
        double f_repetition_time,
        double & f_InitialVal);   
    int FirstDifferentiationOfInitialFunctionForNewtonRaphson(double f_inital_guess_netwon_raph,
        int f_datapoints,
        double f_division_factor_for_newton_raph,
        double x1,double x2,double y1,double y2,
        int f_equation_flag,        
        double f_repetition_time,
        double & f_FirstDifferential);
    int Calculate2Points(int f_datapoints,
        double *y, double*x,
        double &y1,double &y2,
        double &x1,double &x2);
    int Calculate3Points(int f_datapoints,
        double *y, double*x,
        double &y1,double &y2,double &y3,
        double &x1,double &x2,double &x3);
    
};

#endif // !defined(AFX_INITIALGUESS_H__B3BEC13F_D5C1_4737_94BB_293EEFD7B47F__INCLUDED_)
