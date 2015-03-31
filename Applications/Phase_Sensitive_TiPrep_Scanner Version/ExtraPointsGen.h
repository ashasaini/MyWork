// ExtraPointsGen.h: interface for the CExtraPointsGen class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_EXTRAPOINTSGEN_H__6E1EA8B9_28C5_4ECE_9F83_EFE33C10FB39__INCLUDED_)
#define AFX_EXTRAPOINTSGEN_H__6E1EA8B9_28C5_4ECE_9F83_EFE33C10FB39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum Graphmode {
    NORMALGRAPH = 0,
    MINREVERSEGRAPH = 1
};

class CExtraPointsGen
{
public:
    CExtraPointsGen();
    CExtraPointsGen(int);
//     CExtraPointsGen(CExtraPointsGen& f_ExtraPntGen);
    virtual ~CExtraPointsGen();
    CExtraPointsGen& operator = (CExtraPointsGen& f_ExtraPntGen);

    int ModeSetting(int f_nSize);
    int GenerateBuffgenX(int *f_nBuffX);
    int GenerateBuffgenY(
        double f_dFactor_a, double f_dFactor_b, double f_dFactor_m, double f_dFactor_k,
        double f_dRepetationTime ,
        int f_nRoi,
        int f_nEquationFlag ,
        int f_nMinrevFlag);
    int DivideIntervalAndAllocateNewBuffers(int &f_nPts , int *f_nBuffX);
    int GetCopyOfNewBuffers(int*f_nXptr, double**f_dYptr, double**f_dYptr_Rev);
    int m_FailureFlag;

private:
    int DivideInterval(double f_dFirst, double f_dSecond, int f_nPrevpts,int & newpts);
    int AllocBuffGenX();
    int AllocBuffGenY();
    int AllocBuffGenYrev();
    void freeBuffGenYrev();
    void freeBuffGenX();
    void freeBuffGenY();
    void CopyBufferX(int*f_nXptr);
    void CopyBufferY(double**f_dYptr);
    void CopyBufferYrev(double **f_dYptr_rev);
    
private:
    double *m_BuffGenX;
    double **m_BuffGenY;
    double **m_BuffGenYrev;
    double m_diffpts;
    int m_TotalRoiNum;
    int m_Origpts;
    int m_primeFlag;
    int m_inside_pts;
    int m_Newpts;
    int m_INTDVD;
};

#endif // !defined(AFX_EXTRAPOINTSGEN_H__6E1EA8B9_28C5_4ECE_9F83_EFE33C10FB39__INCLUDED_)
