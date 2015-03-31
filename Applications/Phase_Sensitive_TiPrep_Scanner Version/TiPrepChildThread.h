// TiPrepChildThread.h: interface for the CTiPrepChildThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIPREPCHILDTHREAD_H__52324725_ABE3_47E3_A284_00BA1F0576AE__INCLUDED_)
#define AFX_TIPREPCHILDTHREAD_H__52324725_ABE3_47E3_A284_00BA1F0576AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
#include "ExtraPointsGen.h"
#include "LineFitStrategy.h"

class CTiPrepChildThread  
{
public:
	CTiPrepChildThread();
	virtual ~CTiPrepChildThread();
    double *m_dval_Ti;
    int m_nroiNo;
    double*m_dval_T1;
    double **m_dinputData;
    double **m_doutputData;
    double *m_dfitErr;
    
    int m_nselectImg;
    int *m_ntime_data;
    
    double m_dRepetationTim;
    int m_nSelectedEqn;
    CExtraPointsGen*m_CExtraPoint;
    int m_nManualInitialGuessChk;
    int m_numpts;
    HWND wndhndl;
    CLineFitStrategy * m_linefitobj;
    double *m_drevval_Ti;
    
    double*m_drevval_T1;
    double **m_drevinputData;
    double **m_drevoutputData;
    double *m_drevfitErr;

    static DWORD WINAPI  TiPrepThreadProc(LPVOID lpParameter);
    int StartThread(double *f_dval_Ti,
        double*f_dval_T1,
        int f_nroiNo,
        double **f_dRoi_inputData,
        double **f_dRoi_outputData,
        double *f_dfitErr,
        int f_nselectImg,
        int *f_ntime_data,
        double f_dRepetationTime,
        int f_nSelectedEqn,
        CExtraPointsGen*f_CExtraPoint,
        int f_nManualInitialGuessChk, int numpoints,HANDLE &hndl,CLineFitStrategy * linefit,
        double *f_drev_val_Ti,
        double*f_drevval_T1,
        double **f_drevRoi_inputData,
        double **f_drevRoi_outputData,
        double *f_drevfitErr,
        HWND wndhndl
        );
private:
   // CCriticalSection m_CriticalSection; //AN/Revw/No global objects here, usage is required for progress dialog UpdateProgress method, not here

};

#endif // !defined(AFX_TIPREPCHILDTHREAD_H__52324725_ABE3_47E3_A284_00BA1F0576AE__INCLUDED_)
