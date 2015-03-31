// MatrixSolver.h: interface for the CMatrixSolver class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_MATRIXSOLVER_H__896FAD4F_2EAE_466B_96B1_22641AE4F24E__INCLUDED_)
#define AFX_MATRIXSOLVER_H__896FAD4F_2EAE_466B_96B1_22641AE4F24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<math.h>

#define TINY 1.0e-50;
#define isnanf(x)       (((*(long *)&(x) & 0x7f800000L)==0x7f800000L)&& \
((*(long *)&(x) & 0x007fffffL)!=0x00000000L) )

class CMatrixSolver
{
public:    
	int DeAllocBuff();
	int AllocBuff(int matrixsz);
    CMatrixSolver();
	double *l_dVv;
    virtual ~CMatrixSolver();
    CMatrixSolver(CMatrixSolver& f_view);
    CMatrixSolver& operator = (CMatrixSolver& f_view);

    int MatrixFillingForEquationOne(double*f_dy, int f_ndatapoints, long double **f_dMatrixA, long double *f_dActResultantMtx/*, int f_nDimensions*/);
    int MatrixFillingForEquationTwo(double*f_dy, int f_ndatapoints, long double **f_dMatrixA, long double *f_dActResultantMtx, int f_nDimensions);
    int MatrixFillingForEquationThree(double*f_dy, int f_ndatapoints, long double **f_dMatrixA, long double *f_dActResultantMtx, int f_nDimensions);
    int MatrixFillingForEquationFour(double*f_dy, int f_ndatapoints, long double **f_dMatrixA, long double *f_dActResultantMtx, int f_nDimensions);
    int MatrixFillingForEquationFive(double*f_dy, int f_ndatapoints, long double **f_dMatrixA, long double *f_dActResultantMtx, int f_nDimensions);
    int InverseOfMatrix(long double **f_dMatrixA, long  double **f_dInvertedMatrix, int f_nDimensions);
    int SolverOfMatrices(long double **f_dMatrixAInv, long double*f_dMatrixX, long double *f_dMatrixB, int f_nDimension);
    int InitializeMatrix(long double **f_dMatrixA, int f_nDimensions);
    double DeterminantOfMatrix(long double **f_dMatrixA, int f_nDimensions);
    int UpdateMatrixAWithDampingFactor(long double **f_dMatrixA,double f_dLambda,int f_nDimensions);
    int DifferentialErrorCheck(long double f_dOldMatrixA,
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
        int f_nEqnFlag, int &f_nBreakFlag);
    long double** dmatrix(long nrl, long nrh, long ncl, long nch);
    void free_dmatrix(long double **m, long nrl, long nrh, long ncl, long nch);
    int setInitialValues(long double f_dA0, long double f_DB0, long double f_dM0, long double f_dK0, double *f_dTimeStamp, int f_nSelImg, double f_dTR);
//private:
    long double m_dA0;
    long double m_dB0;
    double *m_dtimestamp;
    double m_dTR;
    //int m_ndataIndexCounter;
    long double m_dM0;
    long double m_dK0;
    int m_nImgCnt;

    inline unsigned long *ivector(long nl, long nh);
    inline double *dvector(long nl, long nh);
    inline void free_ivector(int *v, long nl, long nh);
    inline void free_dvector(double *v, long nl, long nh);
    // Y=A(1-2exp(BX));//2 unknowns
    //PD refers to Partial Derivative.
    inline long double EquationOne(int currindx);
    inline long double PDOfEquationOneWrtFirstUnknown(int currindx);
    inline long double PDOfEquationOneWrtSecUnknown(int currindx);
    
    // Y=A(1-2exp(BX)+exp(B*Tr));//2 unknowns
    //PD refers to Partial Derivative.
    inline long double EquationTwo(int currindx);
    inline long double PDOfEquationTwoWrtFirstUnknown(int currindx);
    inline long double PDOfEquationTwoWrtSecUnknown(int currindx);
    
    // Y=A(1-2exp(BX))+M(exp(B*Tr));//3 unknowns
    //PD refers to Partial Derivative.
    inline long double EquationThree(int currindx);
    inline long double PDOfEquationThreeWrtFirstUnknown(int currindx);
    inline long double PDOfEquationThreeWrtSecUnknown(int currindx);
    inline long double PDOfEquationThreeWrtThirdUnknown(int currindx);
    
    //Y = A(1-exp(BX)); //2 unknowns
    //PD refers to Partial Derivative.
    inline long double EquationFour(int currindx);
    inline long double PDOfEquationFourWrtFirstUnknown(int currindx);
    inline long double PDOfEquationFourWrtSecUnknown(int currindx);
    
    // Y=A(1-Kexp(BX));//3 unknowns
    //PD refers to Partial Derivative.
    inline long double EquationFive(int currindx);
    inline long double PDOfEquationFiveWrtFirstUnknown(int currindx);
    inline long double PDOfEquationFiveWrtSecUnknown(int currindx);
    inline long double PDOfEquationFiveWrtThirdUnknown(int currindx);
    inline int msRSLubksb(long double **f_dMatrixA,
        int f_nDimension,
        int *f_nIndex,
        double *f_dMatrixB);
    inline int msRSLudcmp(long double **f_dMatrixA,
        int f_nDimension,
        int *f_nIndex,
        double *f_dMatrixB);
    
    
};

#endif // !defined(AFX_MATRIXSOLVER_H__896FAD4F_2EAE_466B_96B1_22641AE4F24E__INCLUDED_)
