// MatrixSolver.cpp: implementation of the CMatrixSolver class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "MatrixSolver.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#define NR_END 1
#define FREE_ARG char*

const CString EXCEPTION_MATRIXSOLVER = L"Exception occurred in Matrix Solver";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CMatrixSolver
//Purpose       : Constructor
//********************************************************************
CMatrixSolver::CMatrixSolver():
m_dA0(0.0),
m_dB0(0.0),
m_dtimestamp(NULL),
m_dTR(0.0),
m_dM0(0.0),
m_dK0(0.0),
m_nImgCnt(0),
l_dVv(0)
{
    /*TIPREP_TRACE(CMatrixSolver::CMatrixSolver);*/
}

//****************************Method Header***************************
//Method Name   : CMatrixSolver
//Purpose       : Copy Constructor
//********************************************************************
CMatrixSolver::CMatrixSolver( CMatrixSolver& f_matrixsolver )
{
    if(NULL != m_dtimestamp){
        delete [] m_dtimestamp;
        m_dtimestamp = NULL;
    }
    
    m_dtimestamp = new double[f_matrixsolver.m_nImgCnt];

    if(NULL != m_dtimestamp){
        for (int l_ncnt = 0;l_ncnt < f_matrixsolver.m_nImgCnt;l_ncnt++)
        {
            m_dtimestamp[l_ncnt] = f_matrixsolver.m_dtimestamp[l_ncnt];
        }
    }

    m_dA0 = f_matrixsolver.m_dA0;
    m_dB0 = f_matrixsolver.m_dB0;
    m_nImgCnt = f_matrixsolver.m_nImgCnt;
    m_dTR = f_matrixsolver.m_dTR;
    //m_ndataIndexCounter = f_matrixsolver.m_ndataIndexCounter;
    m_dM0 = f_matrixsolver.m_dM0;
    m_dK0 = f_matrixsolver.m_dK0;
}

//****************************Method Header***************************
//Method Name   : CMatrixSolver
//Purpose       : Overloaded assignment operator
//********************************************************************
CMatrixSolver& CMatrixSolver::operator=( CMatrixSolver& f_matrixsolver )
{
    if(& f_matrixsolver  == this){
        return *this;
    }

    if(NULL != m_dtimestamp){
        delete [] m_dtimestamp;
        m_dtimestamp = NULL;
    }

    m_dtimestamp = new double[f_matrixsolver.m_nImgCnt];

    if(NULL != m_dtimestamp){
        for (int l_ncnt = 0;l_ncnt < f_matrixsolver.m_nImgCnt;l_ncnt++)
        {
            m_dtimestamp[l_ncnt] = f_matrixsolver.m_dtimestamp[l_ncnt];
        }
    }

    m_dA0 = f_matrixsolver.m_dA0;
    m_dB0 = f_matrixsolver.m_dB0;
    m_nImgCnt = f_matrixsolver.m_nImgCnt;
    m_dTR = f_matrixsolver.m_dTR;
//    m_ndataIndexCounter = f_matrixsolver.m_ndataIndexCounter;
    m_dM0 = f_matrixsolver.m_dM0;
    m_dK0 = f_matrixsolver.m_dK0;

    return (*this);
}
int m_nDimension;//KJ
//****************************Method Header***************************
//Method Name   : CMatrixSolvers
//Purpose       : Destructor
//********************************************************************
CMatrixSolver::~CMatrixSolver()
{
	if (l_dVv)
		free_dvector(l_dVv, 1, m_nDimension);
    if(NULL != m_dtimestamp) {
        delete [] m_dtimestamp;
    }
}

/*************************************************************
* Method:       MatrixFillingForEquationOne
* Description:  This function is used to fill the matrix for
*               the first equation.
* Parameter:    double * f_dy
* Parameter:    int f_ndatapoints
* Parameter:    long double * * f_dMatrixA
* Parameter:    long double * f_dActResultantMtx
* Parameter:    int f_nDimensions
* Returns:      void
*************************************************************/
 int CMatrixSolver::MatrixFillingForEquationOne(double*f_dy, int f_ndatapoints, 
                                               long double **f_dMatrixA, 
                                               long double *f_dActResultantMtx 
                                               /*,int f_nDimensions*/)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::MatrixFillingForEquationOne,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        long double l_dA11 = 0.0, l_dA12 = 0.0;
        long double l_dA21 = 0.0, l_dA22 = 0.0;
        long double l_dB1 = 0.0, l_dB2 = 0.0;
        
        if((NULL == f_dy) || (NULL == f_dMatrixA) || (NULL == f_dActResultantMtx)) {
            l_error_display_string = (_T("MatrixFillingForEquationOne Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        int l_index = 0;
        for(l_index = 0; l_index < f_ndatapoints; l_index++) {
            l_dA11 = l_dA11 + (PDOfEquationOneWrtFirstUnknown(l_index) * PDOfEquationOneWrtFirstUnknown(l_index));
            l_dA12 = l_dA12 + (PDOfEquationOneWrtFirstUnknown(l_index) * PDOfEquationOneWrtSecUnknown(l_index));
            l_dA21 = l_dA21 + (PDOfEquationOneWrtSecUnknown(l_index) * PDOfEquationOneWrtFirstUnknown(l_index));
            l_dA22 = l_dA22 + (PDOfEquationOneWrtSecUnknown(l_index) * PDOfEquationOneWrtSecUnknown(l_index));
            l_dB1 = l_dB1 + ((f_dy[l_index] - (m_dA0 * PDOfEquationOneWrtFirstUnknown(l_index))) * PDOfEquationOneWrtFirstUnknown(l_index));
            l_dB2 = l_dB2 + ((f_dy[l_index] - (m_dA0 * PDOfEquationOneWrtFirstUnknown(l_index))) * PDOfEquationOneWrtSecUnknown(l_index));
        }
        
        f_dMatrixA[1][1] = l_dA11;
        f_dMatrixA[1][2] = l_dA12;
        f_dMatrixA[2][1] = l_dA21;
        f_dMatrixA[2][2] = l_dA22;
        f_dActResultantMtx[0] = l_dB1;
        f_dActResultantMtx[1] = l_dB2;
        return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       MatrixFillingForEquationTwo
* Description:  This function is used to fill the matrix for
*               the second equation.
* Parameter:    double * f_dy
* Parameter:    int f_ndatapoints
* Parameter:    long double * * f_dMatrixA
* Parameter:    long double * f_dActResultantMtx
* Parameter:    int f_nDimensions
* Returns:      void
*************************************************************/
int CMatrixSolver::MatrixFillingForEquationTwo(double*f_dy, int f_ndatapoints, 
                                               long double **f_dMatrixA, 
                                               long double *f_dActResultantMtx, 
                                               int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::MatrixFillingForEquationTwo,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        long double l_dA11 = 0.0, l_dA12 = 0.0;
        long double l_dA21 = 0.0, l_dA22 = 0.0;
        long double l_dB1 = 0.0, l_dB2 = 0.0;
        
        if((NULL == f_dy) || (NULL == f_dMatrixA) || (NULL == f_dActResultantMtx)) {
            l_error_display_string = (_T("MatrixFillingForEquationTwo Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        int l_index = 0;
        for(l_index = 0; l_index < f_ndatapoints; l_index++) {
            l_dA11 = l_dA11 + (PDOfEquationTwoWrtFirstUnknown(l_index) * PDOfEquationTwoWrtFirstUnknown(l_index));
            l_dA12 = l_dA12 + (PDOfEquationTwoWrtFirstUnknown(l_index) * PDOfEquationTwoWrtSecUnknown(l_index));
            l_dA21 = l_dA21 + (PDOfEquationTwoWrtSecUnknown(l_index) * PDOfEquationTwoWrtFirstUnknown(l_index));
            l_dA22 = l_dA22 + (PDOfEquationTwoWrtSecUnknown(l_index) * PDOfEquationTwoWrtSecUnknown(l_index));
            l_dB1 = l_dB1 + ((f_dy[l_index] - (m_dA0 * (PDOfEquationTwoWrtFirstUnknown(l_index)))) * PDOfEquationTwoWrtFirstUnknown(l_index));
            l_dB2 = l_dB2 + ((f_dy[l_index] - (m_dA0 * (PDOfEquationTwoWrtFirstUnknown(l_index)))) * PDOfEquationTwoWrtSecUnknown(l_index));
        }
        
        f_dMatrixA[1][1] = l_dA11;
        f_dMatrixA[1][2] = l_dA12;
        f_dMatrixA[2][1] = l_dA21;
        f_dMatrixA[2][2] = l_dA22;
        f_dActResultantMtx[0] = l_dB1;
        f_dActResultantMtx[1] = l_dB2;
        return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       MatrixFillingForEquationThree
* Description:  This function is used to fill the matrix for
*               the thirdsecond equation.
* Parameter:    double * f_dy
* Parameter:    int f_ndatapoints
* Parameter:    long double * * f_dMatrixA
* Parameter:    long double * f_dActResultantMtx
* Parameter:    int f_nDimensions
* Returns:      void
*************************************************************/
int CMatrixSolver::MatrixFillingForEquationThree(double*f_dy, int f_ndatapoints, long double **f_dMatrixA, long double *f_dActResultantMtx, int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::MatrixFillingForEquationThree,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        long double l_dA11 = 0.0, l_dA12 = 0.0, l_dA13 = 0.0;
        long double l_dA21 = 0.0, l_dA22 = 0.0, l_dA23 = 0.0;
        long double l_dA31 = 0.0, l_dA32 = 0.0, l_dA33 = 0.0;
        long double l_dB1 = 0.0, l_dB2 = 0.0, l_dB3 = 0.0;
        
        if((NULL == f_dy) || (NULL == f_dMatrixA) || (NULL == f_dActResultantMtx)) {
            l_error_display_string = (_T("MatrixFillingForEquationThree Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        int l_index = 0;
        for(l_index = 0; l_index < f_ndatapoints; l_index++) {
            l_dA11 = l_dA11 + (PDOfEquationThreeWrtFirstUnknown(l_index) * PDOfEquationThreeWrtFirstUnknown(l_index));
            l_dA12 = l_dA12 + (PDOfEquationThreeWrtFirstUnknown(l_index) * PDOfEquationThreeWrtSecUnknown(l_index));
            l_dA13 = l_dA13 + (PDOfEquationThreeWrtFirstUnknown(l_index) * PDOfEquationThreeWrtThirdUnknown(l_index));
            l_dA21 = l_dA21 + (PDOfEquationThreeWrtSecUnknown(l_index) * PDOfEquationThreeWrtFirstUnknown(l_index));
            l_dA22 = l_dA22 + (PDOfEquationThreeWrtSecUnknown(l_index) * PDOfEquationThreeWrtSecUnknown(l_index));
            l_dA23 = l_dA23 + (PDOfEquationThreeWrtSecUnknown(l_index) * PDOfEquationThreeWrtThirdUnknown(l_index));
            l_dA31 = l_dA31 + (PDOfEquationThreeWrtThirdUnknown(l_index) * PDOfEquationThreeWrtFirstUnknown(l_index));
            l_dA32 = l_dA32 + (PDOfEquationThreeWrtThirdUnknown(l_index) * PDOfEquationThreeWrtSecUnknown(l_index));
            l_dA33 = l_dA33 + (PDOfEquationThreeWrtThirdUnknown(l_index) * PDOfEquationThreeWrtThirdUnknown(l_index));
            l_dB1 = l_dB1 + ((f_dy[l_index] - (EquationThree(l_index))) * (PDOfEquationThreeWrtFirstUnknown(l_index)));
            l_dB2 = l_dB2 + ((f_dy[l_index] - (EquationThree(l_index))) * (PDOfEquationThreeWrtSecUnknown(l_index)));
            l_dB3 = l_dB3 + ((f_dy[l_index] - (EquationThree(l_index))) * (PDOfEquationThreeWrtThirdUnknown(l_index)));
        }
        
        f_dMatrixA[1][1] = l_dA11;
        f_dMatrixA[1][2] = l_dA12;
        f_dMatrixA[1][3] = l_dA13;
        f_dMatrixA[2][1] = l_dA21;
        f_dMatrixA[2][2] = l_dA22;
        f_dMatrixA[2][3] = l_dA23;
        f_dMatrixA[3][1] = l_dA31;
        f_dMatrixA[3][2] = l_dA32;
        f_dMatrixA[3][3] = l_dA33;
        f_dActResultantMtx[0] = l_dB1;
        f_dActResultantMtx[1] = l_dB2;
        f_dActResultantMtx[2] = l_dB3;
        return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       MatrixFillingForEquationFour
* Description:  This function is used to fill the matrix for
*               the fourth equation.
* Parameter:    double * f_dy
* Parameter:    int f_ndatapoints
* Parameter:    long double * * f_dMatrixA
* Parameter:    long double * f_dActResultantMtx
* Parameter:    int f_nDimensions
* Returns:      void
*************************************************************/
int CMatrixSolver::MatrixFillingForEquationFour(double*f_dy, 
                                                int f_ndatapoints, 
                                                long double **f_dMatrixA, 
                                                long double *f_dActResultantMtx, 
                                                int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::MatrixFillingForEquationFour,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        long double l_dA11 = 0.0, l_dA12 = 0.0;
        long double l_dA21 = 0.0, l_dA22 = 0.0;
        long double l_dB1 = 0.0, l_dB2 = 0.0;
        
        if((NULL == f_dy) || (NULL == f_dMatrixA) || (NULL == f_dActResultantMtx)) {
            l_error_display_string = (_T("MatrixFillingForEquationFour Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        int l_index = 0;
        for(l_index = 0; l_index < f_ndatapoints; l_index++) {
            l_dA11 = l_dA11 + (PDOfEquationFourWrtFirstUnknown(l_index) * PDOfEquationFourWrtFirstUnknown(l_index));
            l_dA12 = l_dA12 + (PDOfEquationFourWrtFirstUnknown(l_index) * PDOfEquationFourWrtSecUnknown(l_index));
            l_dA21 = l_dA21 + (PDOfEquationFourWrtSecUnknown(l_index) * PDOfEquationFourWrtFirstUnknown(l_index));
            l_dA22 = l_dA22 + (PDOfEquationFourWrtSecUnknown(l_index) * PDOfEquationFourWrtSecUnknown(l_index));
            l_dB1 = l_dB1 + ((f_dy[l_index] -  EquationFour(l_index)) * PDOfEquationFourWrtFirstUnknown(l_index));
            l_dB2 = l_dB2 + ((f_dy[l_index] -  EquationFour(l_index)) * PDOfEquationFourWrtSecUnknown(l_index));
        }
        
        f_dMatrixA[1][1] = l_dA11;
        f_dMatrixA[1][2] = l_dA12;
        f_dMatrixA[2][1] = l_dA21;
        f_dMatrixA[2][2] = l_dA22;
        f_dActResultantMtx[0] = l_dB1;
        f_dActResultantMtx[1] = l_dB2;
        return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       MatrixFillingForEquationFive
* Description:  This function is used to fill the matrix for
*               the fifth equation.
* Parameter:    double * f_dy
* Parameter:    int f_ndatapoints
* Parameter:    long double * * f_dMatrixA
* Parameter:    long double * f_dActResultantMtx
* Parameter:    int f_nDimensions
* Returns:      void
*************************************************************/
int CMatrixSolver::MatrixFillingForEquationFive(double*f_dy, 
                                                int f_ndatapoints, 
                                                long double **f_dMatrixA, 
                                                long double *f_dActResultantMtx, 
                                                int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::MatrixFillingForEquationFive,MRTraceMarker9);
#endif

    try{
    CString l_error_display_string(_T(""));
    long double l_dA11 = 0.0, l_dA12 = 0.0, l_dA13 = 0.0;
    long double l_dA21 = 0.0, l_dA22 = 0.0, l_dA23 = 0.0;
    long double l_dA31 = 0.0, l_dA32 = 0.0, l_dA33 = 0.0;
    long double l_dB1 = 0.0, l_dB2 = 0.0, l_dB3 = 0.0;

    if((NULL == f_dy) || (NULL == f_dMatrixA) || (NULL == f_dActResultantMtx)) {
        l_error_display_string = (_T("MatrixFillingForEquationFive Input Parameter NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
        return E_ERROR_NULL_FUNCPARAMS;
    }
    int l_index = 0;
    for(l_index = 0; l_index < f_ndatapoints; l_index++) {
        l_dA11 = l_dA11 + (PDOfEquationFiveWrtFirstUnknown(l_index) * PDOfEquationFiveWrtFirstUnknown(l_index));
        l_dA12 = l_dA12 + (PDOfEquationFiveWrtFirstUnknown(l_index) * PDOfEquationFiveWrtSecUnknown(l_index));
        l_dA13 = l_dA13 + (PDOfEquationFiveWrtFirstUnknown(l_index) * PDOfEquationFiveWrtThirdUnknown(l_index));
        l_dA21 = l_dA21 + (PDOfEquationFiveWrtSecUnknown(l_index) * PDOfEquationFiveWrtFirstUnknown(l_index));
        l_dA22 = l_dA22 + (PDOfEquationFiveWrtSecUnknown(l_index) * PDOfEquationFiveWrtSecUnknown(l_index));
        l_dA23 = l_dA23 + (PDOfEquationFiveWrtSecUnknown(l_index) * PDOfEquationFiveWrtThirdUnknown(l_index));
        l_dA31 = l_dA31 + (PDOfEquationFiveWrtThirdUnknown(l_index) * PDOfEquationFiveWrtFirstUnknown(l_index));
        l_dA32 = l_dA32 + (PDOfEquationFiveWrtThirdUnknown(l_index) * PDOfEquationFiveWrtSecUnknown(l_index));
        l_dA33 = l_dA33 + (PDOfEquationFiveWrtThirdUnknown(l_index) * PDOfEquationFiveWrtThirdUnknown(l_index));
        l_dB1 = l_dB1 + ((f_dy[l_index] - (EquationFive(l_index))) * PDOfEquationFiveWrtFirstUnknown(l_index));
        l_dB2 = l_dB2 + ((f_dy[l_index] - (EquationFive(l_index))) * PDOfEquationFiveWrtSecUnknown(l_index));
        l_dB3 = l_dB3 + ((f_dy[l_index] - (EquationFive(l_index))) * PDOfEquationFiveWrtThirdUnknown(l_index));
    }

    f_dMatrixA[1][1] = l_dA11;
    f_dMatrixA[1][2] = l_dA12;
    f_dMatrixA[1][3] = l_dA13;
    f_dMatrixA[2][1] = l_dA21;
    f_dMatrixA[2][2] = l_dA22;
    f_dMatrixA[2][3] = l_dA23;
    f_dMatrixA[3][1] = l_dA31;
    f_dMatrixA[3][2] = l_dA32;
    f_dMatrixA[3][3] = l_dA33;
    f_dActResultantMtx[0] = l_dB1;
    f_dActResultantMtx[1] = l_dB2;
    f_dActResultantMtx[2] = l_dB3;
    return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       SolverOfMatrices
* Description:  This function is used
* Parameter:    long double * * f_dMatrixAInv
* Parameter:    long double * f_dMatrixX
* Parameter:    long double * f_dMatrixB
* Parameter:    int f_nDimension
* Returns:      int
*************************************************************/
int CMatrixSolver::SolverOfMatrices(long double **f_dMatrixAInv, 
                                    long double*f_dMatrixX, 
                                    long double *f_dMatrixB, 
                                    int f_nDimension)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//	TIPREP_TRACE(CMatrixSolver::SolverOfMatrices,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        int l_nRowCount = 0, l_nColCount = 0;
        
        if((NULL == f_dMatrixAInv) || (NULL == f_dMatrixX) || (NULL == f_dMatrixB)) {
            l_error_display_string = (_T("SolverOfMatrices Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        for(l_nRowCount = 0 ; l_nRowCount <  f_nDimension; l_nRowCount ++) {         //row size of 1st matrix
            f_dMatrixX[l_nRowCount] = 0;
            
            for(l_nColCount = 0 ; l_nColCount < f_nDimension; l_nColCount ++) {  // common dimension size
                f_dMatrixX[l_nRowCount] += f_dMatrixAInv[l_nRowCount + 1][l_nColCount + 1] * f_dMatrixB[l_nColCount]; // Matrix multiplication 1D array result
            }
        }
        
        return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return(E_ERROR);
    }
}

//EQUATION 1
/*************************************************************
* Method:       EquationOne
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::EquationOne(int currindx)
{
    return(m_dA0 * (1 - 2 * exp(m_dB0 * m_dtimestamp[currindx])));
}

// PD refers to Partial derivative.
/*************************************************************
* Method:       PDOfEquationOneWrtFirstUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationOneWrtFirstUnknown(int currindx)
{
    return(1 - 2 * exp(m_dB0 * m_dtimestamp[currindx]));
}

/*************************************************************
* Method:       PDOfEquationOneWrtSecUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationOneWrtSecUnknown(int currindx)
{
    return(-2 * m_dA0 * m_dtimestamp[currindx] * exp(m_dB0 * m_dtimestamp[currindx]));
}

//EQUATION 2
/*************************************************************
* Method:       EquationTwo
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::EquationTwo(int currindx)
{
    return(m_dA0 * (1 - 2 * exp(m_dB0 * m_dtimestamp[currindx]) + exp(m_dB0 * m_dTR)));
}

/*************************************************************
* Method:       PDOfEquationTwoWrtFirstUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationTwoWrtFirstUnknown(int currindx)
{
    return(1 - 2 * exp(m_dB0 * m_dtimestamp[currindx]) + exp(m_dB0 * m_dTR));
}

/*************************************************************
* Method:       PDOfEquationTwoWrtSecUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
 inline long double CMatrixSolver::PDOfEquationTwoWrtSecUnknown(int currindx)
{
    return(-2 * m_dA0 * m_dtimestamp[currindx] * exp(m_dB0 * m_dtimestamp[currindx]) + m_dA0 * m_dTR * exp(m_dB0 * m_dTR));
}

//EQUATION 3
/*************************************************************
* Method:       EquationThree
* Description:  This function is used
* Returns:      long double
*************************************************************/
 inline long double CMatrixSolver::EquationThree(int currindx)
{
    return(m_dA0 * (1 - 2 * exp(m_dB0 * m_dtimestamp[currindx])) + m_dM0 * (exp(m_dB0 * m_dTR)));
}

/*************************************************************
* Method:       PDOfEquationThreeWrtFirstUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationThreeWrtFirstUnknown(int currindx)
{
    return(1 - 2 * exp(m_dB0 * m_dtimestamp[currindx]));
}

/*************************************************************
* Method:       PDOfEquationThreeWrtSecUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationThreeWrtSecUnknown(int currindx)
{
    return(-2 * m_dA0 * m_dtimestamp[currindx] * exp(m_dB0 * m_dtimestamp[currindx]) + m_dM0 * m_dTR * exp(m_dB0 * m_dTR));
}

/*************************************************************
* Method:       PDOfEquationThreeWrtThirdUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationThreeWrtThirdUnknown(int currindx)
{
    return(exp(m_dB0 * m_dTR));
}

//EQUATION 4
/*************************************************************
* Method:       EquationFour
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::EquationFour(int currindx)
{
    return(m_dA0 * (1 - exp(m_dB0 * m_dtimestamp[currindx])));
}

/*************************************************************
* Method:       PDOfEquationFourWrtFirstUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationFourWrtFirstUnknown(int currindx)
{
    return(1 - exp(m_dB0 * m_dtimestamp[currindx]));
}

/*************************************************************
* Method:       PDOfEquationFourWrtSecUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationFourWrtSecUnknown(int currindx)
{
    return(-(m_dA0) * m_dtimestamp[currindx] * exp(m_dB0 * m_dtimestamp[currindx]));
}

//EQUATION 5
/*************************************************************
* Method:       EquationFive
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::EquationFive(int currindx)
{
    return(m_dA0 * (1 - m_dK0 * exp(m_dB0 * m_dtimestamp[currindx])));
}

/*************************************************************
* Method:       PDOfEquationFiveWrtFirstUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationFiveWrtFirstUnknown(int currindx)
{
    return(1 - m_dK0 * exp(m_dB0 * m_dtimestamp[currindx]));
}

/*************************************************************
* Method:       PDOfEquationFiveWrtSecUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationFiveWrtSecUnknown(int currindx)
{
    return(-(m_dA0) * m_dK0 * m_dtimestamp[currindx] * 
        exp(m_dB0 * m_dtimestamp[currindx]));
}

/*************************************************************
* Method:       PDOfEquationFiveWrtThirdUnknown
* Description:  This function is used
* Returns:      long double
*************************************************************/
inline long double CMatrixSolver::PDOfEquationFiveWrtThirdUnknown(int currindx)
{
    return(-(m_dA0) * exp(m_dB0 * m_dtimestamp[currindx]));
}

/*************************************************************
* Method:       InitializeMatrix
* Description:  This function is used
* Parameter:    long double * * f_dMatrixA
* Parameter:    int f_nDimensions
* Returns:      int
*************************************************************/
 int CMatrixSolver::InitializeMatrix(long double **f_dMatrixA, int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::InitializeMatrix,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        int  l_nRowCount = 1, l_nColCount = 1;
        
        if((NULL == f_dMatrixA)) {
            l_error_display_string = (_T("InitializeMatrix Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        for(l_nRowCount = 1; l_nRowCount <= f_nDimensions; l_nRowCount++) {
            for(l_nColCount = 1; l_nColCount <= f_nDimensions; l_nColCount++) {
                f_dMatrixA[l_nRowCount][l_nColCount] = 0.0;
            }
        }
        
        return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}
#define DETERMINANT_FAILED 0;
/*************************************************************
* Method:       DeterminantOfMatrix
* Description:  This function is used
* Parameter:    long double * * f_dMatrixA
* Parameter:    int f_nDimensions
* Returns:      double
*************************************************************/
 double CMatrixSolver::DeterminantOfMatrix(long double **f_dMatrixA, int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::DeterminantOfMatrix,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        double l_dDeterminant = 0.0;
        
        if((NULL == f_dMatrixA)) {
            l_error_display_string = (_T("DeterminantOfMatrix Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return DETERMINANT_FAILED;
        }
        
        if(f_nDimensions == 2) {
            l_dDeterminant = (f_dMatrixA[1][1] * f_dMatrixA[2][2] - f_dMatrixA[1][2] * f_dMatrixA[2][1]);
        } else if(f_nDimensions == 3) {
            l_dDeterminant = (f_dMatrixA[1][1] * (f_dMatrixA[2][2] * f_dMatrixA[3][3] - f_dMatrixA[2][3] * f_dMatrixA[3][2])
                - f_dMatrixA[1][2] * (f_dMatrixA[2][1] * f_dMatrixA[3][3] - f_dMatrixA[2][3] * f_dMatrixA[3][1])
                + f_dMatrixA[1][3] * (f_dMatrixA[2][1] * f_dMatrixA[3][2] - f_dMatrixA[2][2] * f_dMatrixA[3][1]));
        } else {
            l_dDeterminant = DETERMINANT_FAILED;
        }
        
        return(l_dDeterminant);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return DETERMINANT_FAILED;
    }
}

/*************************************************************
* Method:       InverseOfMatrix
* Description:  This function is used
* Parameter:    long double * * f_dMatrixA
* Parameter:    long double * * f_dInvertedMatrix
* Parameter:    int f_nDimensions
* Returns:      int
*************************************************************/
int CMatrixSolver::InverseOfMatrix(long double **f_dMatrixA,
                                   long  double **f_dInvertedMatrix,
                                   int f_nDimensions)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::InverseOfMatrix,MRTraceMarker9);
#endif
    int *l_nIndexbuf = NULL;
    double *l_dCol = NULL;

    try{
        CString l_error_display_string(_T(""));
        double d = 0.0;
        int l_nRow = 0, l_nCol = 0;
        l_nIndexbuf = (int *)ivector(1, f_nDimensions);
        
        if((NULL == f_dMatrixA) || (NULL == f_dInvertedMatrix)) {
            l_error_display_string = (_T("InverseOfMatrix Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        if(NULL == l_nIndexbuf) {
            return(E_ERROR);
        }
        
        for(l_nRow = 1; l_nRow <= f_nDimensions; l_nRow++) {
            l_nIndexbuf[l_nRow] = 0;
        }
        
        l_dCol = dvector(1, f_nDimensions);
        
        if(NULL == l_dCol) {
            free_ivector(l_nIndexbuf, 1, f_nDimensions);
            return(E_ERROR);
        }
        
        l_dCol[0] = 0;
        msRSLudcmp(f_dMatrixA, f_nDimensions, l_nIndexbuf, &d);
        
        for(l_nCol = 1; l_nCol <= f_nDimensions; l_nCol++) {
            for(l_nRow = 1; l_nRow <= f_nDimensions; l_nRow++) {
                l_dCol[l_nRow] = 0.0f;
            }
            
            l_dCol[l_nCol] = 1.0f;
            msRSLubksb(f_dMatrixA, f_nDimensions, l_nIndexbuf, l_dCol);
            
            for(l_nRow = 1; l_nRow <= f_nDimensions; l_nRow++)  {
                f_dInvertedMatrix[l_nRow][l_nCol] = l_dCol[l_nRow];
            }
        }
        
        free_ivector(l_nIndexbuf, 1, f_nDimensions);
        free_dvector(l_dCol, 1, f_nDimensions);
        return(E_NO_ERROR);
    }
    catch(...){
        free_ivector(l_nIndexbuf, 1, f_nDimensions);
        free_dvector(l_dCol, 1, f_nDimensions);
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       msRSLubksb
* Description:  This function is used
* Parameter:    long double * * f_dMatrixA
* Parameter:    int f_nDimension
* Parameter:    int * f_nIndex
* Parameter:    double * f_dMatrixB
* Returns:      void
*************************************************************/
inline int CMatrixSolver::msRSLubksb(long double **f_dMatrixA,
                               int f_nDimension,
                               int *f_nIndex,
                               double *f_dMatrixB)
{
//    TIPREP_TRACE(CMatrixSolver::msRSLubksb,MRTraceMarker9);
    
    try{
        CString l_error_display_string(_T(""));
        int l_nRow = 0, l_nii = 0, l_nip, l_nCol = 0;;
        long double l_dSum = 0;
        
        if(!f_dMatrixB) {
            return E_ERROR;
        }
        
        if((NULL == f_dMatrixA) || (NULL == f_nIndex) || (NULL == f_dMatrixB)) {
            l_error_display_string = (_T("msRSLubksb Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        for(l_nRow = 1; l_nRow <= f_nDimension; l_nRow++) {
            l_nip = f_nIndex[l_nRow];
            l_dSum = f_dMatrixB[l_nip];
            f_dMatrixB[l_nip] = f_dMatrixB[l_nRow];
            
            if(l_nii > 0) {
                for(l_nCol = l_nii; l_nCol <= l_nRow - 1; l_nCol++) {
                    l_dSum -= f_dMatrixA[l_nRow][l_nCol] * f_dMatrixB[l_nCol];
                }
            } else if(l_dSum != 0) {
                l_nii = l_nRow;
            }
            
            f_dMatrixB[l_nRow] = l_dSum;
        }
        
        for(l_nRow = f_nDimension; l_nRow >= 1; l_nRow--) {
            l_dSum = f_dMatrixB[l_nRow];
            
            for(l_nCol = l_nRow + 1; l_nCol <= f_nDimension; l_nCol++) {
                l_dSum -= f_dMatrixA[l_nRow][l_nCol] * f_dMatrixB[l_nCol];
            }
            
            if((f_dMatrixA[l_nRow][l_nRow]) > 0.0 || (f_dMatrixA[l_nRow][l_nRow]) < 0.0 ) {
                f_dMatrixB[l_nRow] = l_dSum / f_dMatrixA[l_nRow][l_nRow];
            } else {
                f_dMatrixB[l_nRow] = l_dSum/0.0000001;//0;//To Provide Approximation
            }
        }
        
        return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }    
}

/*************************************************************
* Method:       msRSLudcmp
* Description:  This function is used
* Parameter:    long double * * f_dMatrixA
* Parameter:    int f_nDimension
* Parameter:    int * f_nIndex
* Parameter:    double * f_dMatrixB
* Returns:      int
*************************************************************/
inline int CMatrixSolver::msRSLudcmp(long double **f_dMatrixA,
                              int f_nDimension,
                              int *f_nIndex,
                              double *f_dMatrixB)
{
    //TIPREP_TRACE(CMatrixSolver::msRSLudcmp);
    /*static*/// double *l_dVv = NULL;

    try{
        CString l_error_display_string(_T(""));
        int l_nRow = 0, l_nImax = 0, l_nCol = 0, l_nK = 0;
        double l_dBig = 0.0, l_dDum = 1, l_dSum = 0.0, l_dTemp = 0.0;
        
        if(l_dVv == NULL) {
			m_nDimension = f_nDimension;
            l_dVv = dvector(1, f_nDimension);
        }
         for(l_nRow = 1; l_nRow <= f_nDimension; l_nRow++) {
			l_dVv[l_nRow] = 0;
		 }
		 if((NULL == f_dMatrixA) || (NULL == f_nIndex) || (NULL == f_dMatrixB || NULL == l_dVv)) {
            l_error_display_string = (_T("msRSLudcmp Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        *f_dMatrixB = 1.0;
        
        for(l_nRow = 1; l_nRow <= f_nDimension; l_nRow++) {
            l_dBig = 0.0;
            
            for(l_nCol = 1; l_nCol <= f_nDimension; l_nCol++)
                l_dTemp = fabs(f_dMatrixA[l_nRow][l_nCol]);
                if(l_dTemp > l_dBig) {
                    l_dBig = l_dTemp;
                }                
                if(l_dBig == 0.0) {                    
                    l_dBig = 0.000001;
                }
                
                l_dVv[l_nRow] = 1.0 / l_dBig;
        }
        
        for(l_nCol = 1; l_nCol <= f_nDimension; l_nCol++) {
            for(l_nRow = 1; l_nRow < l_nCol; l_nRow++) {
                l_dSum = f_dMatrixA[l_nRow][l_nCol];
                
                for(l_nK = 1; l_nK < l_nRow; l_nK++) {
                    l_dSum -= f_dMatrixA[l_nRow][l_nK] * f_dMatrixA[l_nK][l_nCol];
                }
                
                f_dMatrixA[l_nRow][l_nCol] = l_dSum;
            }
            
            l_dBig = 0.0;
            
            for(l_nRow = l_nCol; l_nRow <= f_nDimension; l_nRow++) {
                l_dSum = f_dMatrixA[l_nRow][l_nCol];
                
                for(l_nK = 1; l_nK < l_nCol; l_nK++) {
                    l_dSum -= f_dMatrixA[l_nRow][l_nK] * f_dMatrixA[l_nK][l_nCol];
                }
                
                f_dMatrixA[l_nRow][l_nCol] = l_dSum;
                l_dDum = l_dVv[l_nRow] * fabs(l_dSum);
                if(l_dDum >= l_dBig) {
                    l_dBig = l_dDum;
                    l_nImax = l_nRow;
                }
            }
            
            if(l_nCol != l_nImax) {
                for(l_nK = 1; l_nK <= f_nDimension; l_nK++) {
                    l_dDum = f_dMatrixA[l_nImax][l_nK];
                    f_dMatrixA[l_nImax][l_nK] = f_dMatrixA[l_nCol][l_nK];
                    f_dMatrixA[l_nCol][l_nK] = l_dDum;
                }
                
                *f_dMatrixB = -(*f_dMatrixB);
                l_dVv[l_nImax] = l_dVv[l_nCol];
            }
            
            f_nIndex[l_nCol] = l_nImax;
            
            if(f_dMatrixA[l_nCol][l_nCol] == 0.0) {
                f_dMatrixA[l_nCol][l_nCol] = TINY;
            }
            
            if(l_nCol != f_nDimension) {
                if((f_dMatrixA[l_nCol][l_nCol])) {
                    l_dDum = 1.0 / (f_dMatrixA[l_nCol][l_nCol]);
                }
                
                for(l_nRow = l_nCol + 1; l_nRow <= f_nDimension; l_nRow++) {
                    f_dMatrixA[l_nRow][l_nCol] *= l_dDum;
                }
            }
        }
        
      //  free_dvector(l_dVv, 1, f_nDimension);
        //l_dVv = NULL;
        return(E_NO_ERROR);
    }
    catch(...){
        free_dvector(l_dVv, 1, f_nDimension);
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return(E_ERROR);
    }
}
#define ACCURACY_CHECK_VAL 0.0001
/*************************************************************
* Method:       DifferentialErrorCheck
* Description:  This function is used
* Parameter:    long double f_dOldMatrixA
* Parameter:    long double f_dOldMatrixB
* Parameter:    long double f_dOldMatrixM
* Parameter:    long double f_dOldMatrixK
* Parameter:    long double f_dNewMatrixA
* Parameter:    long double f_dNewMatrixB
* Parameter:    long double f_dNewMatrixM
* Returns:      int
*************************************************************/
 int CMatrixSolver::DifferentialErrorCheck(long double f_dOldMatrixA,
        long double f_dOldMatrixB,
        long double f_dOldMatrixM,
        long double f_dOldMatrixK,
        long double f_dNewMatrixA,
        long double f_dNewMatrixB,
        long double f_dNewMatrixM,
        long double f_dNewMatrixK,
        double *f_dTimeStamp,
        double *f_dY,
        int f_nDatapoints,
        double f_dXr,
        int f_nEqnFlag, int &f_nBreakFlag)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::DifferentialErrorCheck,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        long double l_dYOld = 0.0;
        long double l_dYNew = 0.0;
        int l_nCounter = 0;
        
        if((NULL == f_dTimeStamp) || (NULL == f_dY) || (0 >= f_nDatapoints)) {
            l_error_display_string = (_T("DifferentialErrorCheck Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        /* -------------------OLD-------------------------*/
        m_dA0 = f_dOldMatrixA;
        m_dB0 = f_dOldMatrixB;
        m_dM0 = f_dOldMatrixM;
        m_dK0 = f_dOldMatrixK;
        l_dYOld = 0.0;
        
        for(l_nCounter = 0; l_nCounter < f_nDatapoints; l_nCounter++) {
            switch(f_nEqnFlag) {
            case 0:
                l_dYOld = l_dYOld + fabs(f_dY[l_nCounter] - EquationOne(l_nCounter));
                break;
            case 1:
                l_dYOld = l_dYOld + fabs(f_dY[l_nCounter] - EquationTwo(l_nCounter));
                break;
            case 2:
                l_dYOld = l_dYOld + fabs(f_dY[l_nCounter] - EquationThree(l_nCounter));
                break;
            case 3:
                l_dYOld = l_dYOld + fabs(f_dY[l_nCounter] - EquationFour(l_nCounter));
                break;
            case 4:
                l_dYOld = l_dYOld + fabs(f_dY[l_nCounter] - EquationFive(l_nCounter));
                break;
            default:
                return E_ERROR;
            }
        }
        
        /* --------------------NEW----------------------------*/
        m_dA0 = f_dNewMatrixA;
        m_dB0 = f_dNewMatrixB;
        m_dM0 = f_dNewMatrixM;
        m_dK0 = f_dNewMatrixK;
        l_dYNew = 0.0;
        
        for(l_nCounter = 0; l_nCounter < f_nDatapoints; l_nCounter++) {
            switch(f_nEqnFlag) {
            case 0:
                l_dYNew = l_dYNew + fabs(f_dY[l_nCounter] - EquationOne(l_nCounter));
                break;
            case 1:
                l_dYNew = l_dYNew + fabs(f_dY[l_nCounter] - EquationTwo(l_nCounter));
                break;
            case 2:
                l_dYNew = l_dYNew + fabs(f_dY[l_nCounter] - EquationThree(l_nCounter));
                break;
            case 3:
                l_dYNew = l_dYNew + fabs(f_dY[l_nCounter] - EquationFour(l_nCounter));
                break;
            case 4:
                l_dYNew = l_dYNew + fabs(f_dY[l_nCounter]  - EquationFive(l_nCounter));
                break;
            default:
                return E_ERROR;
            }
        }
        
        /* --------------------ERROR CALCULATION----------------------------*/
        
        if(isnanf(l_dYNew) || /*isinf(l_dYNew) || */isnanf(l_dYOld) /*|| isinf(l_dYOld)*/) {  
            return(E_ERROR);
        }
        
        /* This will decide the Accuracy of the fit */
        if(fabs((l_dYOld - l_dYNew)) < ACCURACY_CHECK_VAL) {
            f_nBreakFlag = 1;
        }
        
        return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       ivector
* Description:  This function is used to allocate an 
                unsigned long vector with subscript range v[nl..nh]
* Parameter:    long nl
* Parameter:    long nh
* Returns:      unsigned long*
*************************************************************/
inline unsigned long* CMatrixSolver::ivector(long nl, long nh)
{
    try{
    unsigned long *v = NULL;

    v = (unsigned long *)operator new((size_t)((nh - nl + 1 + NR_END) * sizeof(long)));

    if(v == NULL) {
        return NULL;
    }

    return v - nl + NR_END;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return NULL;
    }
}

/*************************************************************
* Method:       dvector
* Description:  This function is used
* Parameter:    long nl
* Parameter:    long nh
* Returns:      double*
*************************************************************/
inline double* CMatrixSolver::dvector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
    try{
        double *v = NULL;
        v = (double *)operator new((size_t)((nh - nl + 1 + NR_END) * sizeof(double)));
        
        if(NULL == v) {
            return NULL;
        }
        
        return v - nl + NR_END ;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return NULL;
    }
}

/*************************************************************
* Method:       free_ivector
* Description:  This function is used
* Parameter:    int * v
* Parameter:    long nl
* Parameter:    long nh
* Returns:      void
*************************************************************/
inline void CMatrixSolver::free_ivector(int *v, long nl, long nh)
/* free an int vector allocated with ivector() */
{
    try{
        if(NULL != v){
            operator delete((FREE_ARG)(v + nl - NR_END));
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       free_dvector
* Description:  This function is used
* Parameter:    double * v
* Parameter:    long nl
* Parameter:    long nh
* Returns:      void
*************************************************************/
inline void CMatrixSolver::free_dvector(double *v, long nl, long nh)
/* free a double vector allocated with dvector() */
{
    try{
        if(NULL != v){
            operator delete((FREE_ARG)(v + nl - NR_END));
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       dmatrix
* Description:  This function is used
* Parameter:    long nrl
* Parameter:    long nrh
* Parameter:    long ncl
* Parameter:    long nch
* Returns:      long double**
*************************************************************/
 long double** CMatrixSolver::dmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
    try{
    long i, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
    long double **m;
    /* allocate pointers to rows */
    m = (long double **) operator new((size_t)((nrow + NR_END) * sizeof(long double*)));

    if(!m) {
        //nrerror("allocation failure 1 in matrix()");
        return NULL;
    }

    m += NR_END;
    m -= nrl;
    /* allocate rows and set pointers to them */
    m[nrl] = (long double *) operator new((size_t)((nrow * ncol + NR_END) * sizeof(long double)));

    if(!m[nrl]) {
        //   nrerror("allocation failure 2 in matrix()");
        return NULL;
    }

    m[nrl] += NR_END;
    m[nrl] -= ncl;

    for(i = nrl + 1; i <= nrh; i++) {
        m[i] = m[i-1] + ncol;
    }

    /* return pointer to array of pointers to rows */
    return m;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return NULL;
    }
}

/*************************************************************
* Method:       free_dmatrix
* Description:  This function is used
* Parameter:    long double * * m
* Parameter:    long nrl
* Parameter:    long nrh
* Parameter:    long ncl
* Parameter:    long nch
* Returns:      void
*************************************************************/

 void CMatrixSolver::free_dmatrix(long double **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by dmatrix() */
{
    try{
        operator delete((FREE_ARG)(m[nrl] + ncl - NR_END));
        operator delete((FREE_ARG)(m + nrl - NR_END));
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       setInitialValues
* Description:  This function is used
* Parameter:    long double f_dA0
* Parameter:    long double f_dB0
* Parameter:    long double f_dM0
* Parameter:    long double f_dK0
* Parameter:    double * f_dTimeStamp
* Parameter:    int f_nSelImg
* Parameter:    double f_dTR
* Returns:      int
*************************************************************/
 int CMatrixSolver::setInitialValues(long double f_dA0, long double f_dB0, long double f_dM0, 
                                    long double f_dK0, double *f_dTimeStamp, int f_nSelImg, double f_dTR)
{
	//TRACE_COMMENTED
#ifdef _UT_LOG_
//    TIPREP_TRACE(CMatrixSolver::setInitialValues,MRTraceMarker9);
#endif
    
    try{
        CString l_error_display_string(_T(""));
        m_dA0 = f_dA0;
        m_dB0 = f_dB0;
        m_dM0 = f_dM0;
        m_dK0 = f_dK0;
        m_dTR = f_dTR;
        
        if(NULL == f_dTimeStamp) {
            l_error_display_string = (_T("setInitialValues Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        if(NULL != m_dtimestamp) {
            delete []m_dtimestamp;
            m_dtimestamp = NULL;
        }
        m_nImgCnt = f_nSelImg;
        m_dtimestamp = new double[f_nSelImg];
        
        if(NULL == m_dtimestamp) {
            return(E_ERROR);
        }
        
        for(int l_nCnt = 0; l_nCnt < f_nSelImg; l_nCnt++) {
            m_dtimestamp[l_nCnt] = f_dTimeStamp[l_nCnt];
        }
        
        return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}

 int CMatrixSolver::UpdateMatrixAWithDampingFactor( long double **f_dMatrixA,double f_dLambda,int f_nDimensions)
{
    try{
        CString l_error_display_string(_T(""));
        int  l_nRowCount = 1, l_nColCount = 1;
        
        if((NULL == f_dMatrixA)) {
            l_error_display_string = (_T("InitializeMatrix Input Parameter NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display_string, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        for(l_nRowCount = 1; l_nRowCount <= f_nDimensions; l_nRowCount++) {
            for(l_nColCount = 1; l_nColCount <= f_nDimensions; l_nColCount++) {
                if(l_nRowCount == l_nColCount){//only diagonal damping
                    f_dMatrixA[l_nRowCount][l_nColCount] += f_dLambda * /*fabs*/(f_dMatrixA[l_nRowCount][l_nColCount]);
                }
            }
        }
        
        return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_MATRIXSOLVER, __LINE__, __FILE__);
        return E_ERROR;
    }
}


///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

 int CMatrixSolver::AllocBuff(int matrixsz)
{
	return 0;

}

int CMatrixSolver::DeAllocBuff()
{
return 0;
}
