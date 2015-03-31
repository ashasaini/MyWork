#if !defined(AFX_LINEFITSTRATEGY_H__B470953A_A4F7_49DF_9EA3_BBC18643D600__INCLUDED_)
#define AFX_LINEFITSTRATEGY_H__B470953A_A4F7_49DF_9EA3_BBC18643D600__INCLUDED_
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>
#include "TiPrepLogHelper.h"
#include "ExtraPointsGen.h"
#include "TiPrepTrace.h"
#include<math.h>

#include "MatrixSolver.h"


#define CONVERT_FACT 10
#define PRECISIOND 0.0000000000000001
#define PRECISIONF 0.0000000001f

class CLineFitStrategy
{
//==================================================================================

    // construction,copy & destruction
public:
    CLineFitStrategy();
    virtual ~CLineFitStrategy();
    
    // Implementation
public:
	void DummyTest(double *y,int * x, int n, double *outy);
	int AllocTmpBuffers( int numigs, int eqn);
int DeAllocTmpBuffers(int f_eqn);
	int DeAllocTimeStampData();
	int AllocTimeStamp(int * timestamp,int numimgs);
	double * m_timestamp_calc;
    int CalculateTi_Linear(double *f_dval_Ti,
        int f_nroiNo,
        double *f_dinputData,
        double *f_doutputData,
        double *f_dfitErr,
        double *f_dfactB,
        int f_nselectImg,
        int *f_ntime_data,
        double *f_maxSigData);//LINEAR
    int CalculateTi_NonLinear(double *f_dval_Ti,
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
        int f_nMinrevFlag, int f_nManualInitialGuessChk); //NONLINEAR
    int setInitialGuessValues(const double &f_dAFactor,
        const double &f_dBFactor,
        const double &f_dMFactor,
        const double &f_dNFactor);
    int getInitialGuessValues(double &f_dAFactor, double &f_dBFactor,
        double &f_dMFactor, double &f_dNFactor);
    
    // Implementation
private:
    int fit_exp_curve(double *f_dorg_data, int f_ndatalen, 
        double *f_dfact_a, double *f_dfact_b, 
        double *f_derr, int *f_ntime_data);
    int invert_data(double *orgData, int f_nselectImg);
    int translate_data_to_all_positive(double *f_dorg_data, 
        int f_ndatalen, int f_nroiNo, double *f_dmaxSigData);
    int translate_back_to_org(double *f_dorg_data, int f_ndatalen, 
        int f_nroiNo, double *f_dmaxSigData);
    int nonLinearCurveFit(double *f_dTimeStamp,
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
        int f_nManualInitialGuessChk);
    int tiT1Calculation(double *f_dval_Ti,
        double*f_dval_T1,
        double &f_dRepetationTime,
        double &f_dFactorA,
        double &f_dFactorB,
        double &f_dFactorM,
        double &f_dFactorK,
        int f_nEqnFlag, int f_nRoi);
    int curveFitDataCalculation(double *f_dFitError, double *f_dTimeStamp, 
        double *f_dRoiOutData,
        double &f_dRepetationTime, int f_nEqnFlag, int f_nSelImg,
        long double &f_dFactorA,
        long double &f_dFactorB,
        long double &f_dFactorM,
        long double &f_dFactorK, double *l_dInputDataY);

//variables
private:
    //double m_dRepetationTime;
    //int m_nSelectedEqn;
    double m_dAFactor;
    double m_dBFactor;
    double m_dMFactor;
    double m_dNFactor;

    double m_dPrev_AFactor;
    double m_dPrev_BFactor;
    double m_dPrev_MFactor;
    double m_dPrev_NFactor;

    double m_dPrev_AFactor_Minrev;
    double m_dPrev_BFactor_Minrev;
    double m_dPrev_MFactor_Minrev;
    double m_dPrev_NFactor_Minrev;

double *m_dTempOutDataY;
long double ** m_dMatrixA ;
long double ** m_dInvMatrixA ;
};

#endif // !defined(AFX_LINEFITSTRATEGY_H__B470953A_A4F7_49DF_9EA3_BBC18643D600__INCLUDED_)
